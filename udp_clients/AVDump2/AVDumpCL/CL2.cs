using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using System.Collections.ObjectModel;
using System.Reflection;
using System.Collections;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.InfoGathering;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.HashAlgorithms;
using AVDump2Lib.Misc;
using System.Threading;

namespace AVDump2CL {
#if DOTNET4
	public class CL2 {
		private ModuleCollection modules;
		private XDocument xDoc;

		private static void WriteException(Exception e) {
			ExceptionXElement ex = new ExceptionXElement(e, false);
			ex.Save(Path.Combine(Environment.CurrentDirectory, "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml"));
			Console.Write(ex.ToString());
		}
		private static void Pause() {
			char c;
			do {
				c = Console.ReadKey().KeyChar;
			} while(Char.IsControl(c));
		}
		private static object FindInterface(Assembly assm, string interfaceName, object[] args) {
			foreach(Type t in assm.GetTypes()) {
				if(t.IsClass && t.GetInterface(interfaceName, true) != null) {
					return assm.CreateInstance(t.FullName, false, BindingFlags.Default, null, args, null, null);
				}
			}
			return null;
		}
		private static void Main(string[] args) { (new CL2()).Main(); }
		public static string Value(IEnumerable<XElement> settings, string key, string def) {
			var elem = settings.FirstOrDefault(xe => (xe.Attribute("type") != null ? xe.Attribute("type").Value.Equals(key) : false));
			return elem != null && elem.Attribute("value") != null ? elem.Attribute("value").Value : def;
		}
		public static bool Value(IEnumerable<XElement> settings, string key, bool def) {
			var elem = settings.FirstOrDefault(xe => (xe.Attribute("type") != null ? xe.Attribute("type").Value.Equals(key) : false));
			return elem != null && elem.Attribute("value") != null ? bool.Parse(elem.Attribute("value").Value) : def;
		}
		public static XElement Value(IEnumerable<XElement> settings, string key) { return settings.FirstOrDefault(xe => (xe.Attribute("type") != null ? xe.Attribute("type").Value.Equals(key) : false)); }

		private CL2() {
			AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler((object sender, UnhandledExceptionEventArgs e) => { WriteException((Exception)e.ExceptionObject); });
			modules = new ModuleCollection();
		}

		private void Main() {
			var cmdArgs = CmdArg.Parse(Environment.CommandLine);

			string path;
			var cmdArg = (CmdArg)((CmdArg)cmdArgs.Items.SingleOrDefault(ca => ca is CmdArg));
			if(cmdArg != null) cmdArg = (CmdArg)cmdArg.Items.SingleOrDefault(ca => ca is CmdArg && ((CmdArg)ca).Key.Equals("AppData"));

			if(cmdArg == null) {
				path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "AVDump2", "settings.xml");
				//TODO: Create settings file

			} else if(cmdArg.Items.Count == 1 && cmdArg.Items[0] is string) {
				path = (string)cmdArg.Items[0];
				if(!File.Exists(path)) throw new FileNotFoundException("File specified in AppData could not be found", path);
			} else {
				throw new ArgumentException("Wrong arguments for AppData");
			}

			xDoc = XDocument.Load(path);
			LoadExtensions();
		}
		private void LoadExtensions() {
			var extensions = from ext in xDoc.Descendants("Extensions") select ext;
			foreach(var extention in extensions) {
				string extKey = extention.Attribute("key").Value;
				string extPath = extention.Attribute("path").Value;

				if(!File.Exists(extPath)) throw new FileNotFoundException("Specified extension file not found", extPath);
				if(modules.ContainsKey(extKey)) throw new Exception("Module key duplicate: " + extKey);

				IAVDModule module;
				try {
					Assembly assm = Assembly.LoadFrom(extPath);
					module = (IAVDModule)FindInterface(assm, "IAVDModule", null);
				} catch(Exception) { throw; }
				modules.Add(extKey, module);
			}

			var settings = xDoc.Element("AVDump2").Element("SettingsContainer").Elements("Settings");
			foreach(var module in modules) module.Value.Initialize(new XElement(settings.SingleOrDefault(xe => xe.Attribute("target").Equals(module.Key))));
		}

		private void ProcessMedia(Queue<string> mediaLst, IEnumerable<XElement> settings) {
			SearchOption searchOption = Value(settings, "ExcludeSubFolders", false) ? SearchOption.TopDirectoryOnly : SearchOption.AllDirectories;
			HashSet<string> files = new HashSet<string>();
			while(mediaLst.Count != 0) {
				string media = mediaLst.Dequeue();

				if(System.IO.File.Exists(media)) {
					files.Add(media);
				} else if(System.IO.Directory.Exists(media)) {
					var exts = modules.Items<IFilter>().Aggregate(new List<string>(), (acc, filter) => { acc.AddRange(filter.ProcessExtensions); return acc; }).Distinct();
					foreach(var ext in exts) {
						foreach(var path in System.IO.Directory.GetFiles(media, "*." + ext, searchOption)) files.Add(path);
					}
				} else { Console.WriteLine(media + " is not a file or folder"); }
			}

			int i = 1;
			foreach(var file in files) {
				Console.WriteLine("Processing (" + (i++) + " of " + files.Count + "):");
				ProcessMediaFile(file, settings);
			}
		}

		private void ProcessMediaFile(string filePath, IEnumerable<XElement> settings) {
			bool error = false;
			string fileExt = System.IO.Path.GetExtension(filePath).Substring(1);

#if(!Debug)
			bool filter = modules.Items<IFilter>().Any(f => f.Filter(filePath));
			if(filter) return;
#endif

			Stream stream = System.IO.File.OpenRead(filePath);
			//System.IO.BufferedStream bs = new BufferedStream(stream, 1024);

#if(Debug)
			//stream = new NullStream(1024 * 1024 * 1024);
			//stream = new MemoryStream(System.Text.Encoding.ASCII.GetBytes("".PadLeft(1025, 'A'))); //System.IO.File.OpenRead(filePath);
			//stream = new MemoryStream(new byte[] { 0 }); //System.IO.File.OpenRead(filePath);
#endif

			#region Hashing
			DateTime startTime = DateTime.Now;
			BlockConsumerContainer blockConsumerContainer = new BlockConsumerContainer();
			if(Value(settings, "UseAllHashes", false)) {
				Console.WriteLine("Folder: " + System.IO.Path.GetDirectoryName(filePath));
				Console.WriteLine("Filename: " + System.IO.Path.GetFileName(filePath));

				var hashElem = Value(settings, "Hashes");
#if(Debug)
				//if((switches & (eSwitches.Aich)) != 0) hashContainer.AddHashAlgorithm(new Aich(), "AICH");
#endif
				if(Value(settings, "CRC", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new Crc32(), "CRC"));
				//if(Value(settings, "TIGER", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new TigerThex(), "TIGER"));
				if(Value(settings, "ED2K", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new Ed2k(), "ED2K"));
				if(Value(settings, "SHA1", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "SHA1"));
				if(Value(settings, "TTH", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new TreeHash(new TigerThex(), new TigerThex(), 1024), "TTH"));
				if(Value(settings, "MD5", false)) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new System.Security.Cryptography.MD5CryptoServiceProvider(), "MD5"));
				//if(System.IO.Path.GetExtension(filePath).ToLower().Equals(".mkv")) blockConsumerContainer.AddBlockConsumer(new MatroskaFileInfo("MKVParser"));

				/*BlockConsumerContainer.Progress progress = blockConsumerContainer.Start(blockCount, stream, blockSize * 1024);
				if(Value(settings, "SupressProgress", false)) {
					try {
						DisplayHashBuffer(progress);
					} catch(Exception ex) {
						WriteException(ex);
					}
				}*/
			}
			IEnumerable<IBlockConsumer> blockConsumers = blockConsumerContainer.Join();
			#endregion

			if(Value(settings, "PrintElapsedHashingTime", false)) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalSeconds + "s");

			foreach(var module in modules.Items<ILog>()) module.Log();

			/*string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + stream.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse);
			if(Value(settings, "OpenInBrowser", false)) System.Diagnostics.Process.Start(aniDBLink);
			if(Value(settings, "PrintAniDBLink", false)) Console.WriteLine(aniDBLink);
			if(Value(settings, "PrintEd2kLink", false)) Console.WriteLine("ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(ed2k.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/");

			if(Value(settings, "DeleteFileWhenDone", false) && !error) System.IO.File.Delete(filePath);
			if(Value(settings, "PrintTimeUsedPerFile", false)) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");
			if(Value(settings, "PauseWhenFileDone", false)) {
				Console.WriteLine("Press any key to continue");
				Pause();
			}*/
			stream.Dispose();

			#region ACreqing
#if(HasAcreq) //If you get an error below: Scroll to the top of the page and uncomment #undef HasAcreq
			/*if(!(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				string creq = Info.CreateAVDumpLog(filePath, blockConsumers).OuterXml;

				Console.Write("Sending Creq... ");
				try {
					ACreq.eACreqResult status = ACreq.DoACreq(new ACreq.ACreqArgs("avdumplib", 1, port, username, password, creq), (eSwitches.WaitForDumpReply & switches) != 0 ? 5000 : -1);
					switch(status) {
						case ACreq.eACreqResult.ACreqSent:
							Console.WriteLine("Done.");
							break;
						case ACreq.eACreqResult.AsyncCall:
							break;
						default:
							Console.WriteLine("Failed. Reason: " + System.Enum.GetName(typeof(ACreq.eACreqResult), status));
							break;
					}
				} catch(Exception ex) {
					WriteException(ex);
				}
			}*/
#endif
			#endregion

			Console.WriteLine(); Console.WriteLine();
		}

		private void DisplayHashBuffer(BlockConsumerContainer.Progress progress) {
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0; int timeElapsed;
			int lastLineIndex = 0, maxNameLength = 0;
			long fileSize = progress.StreamSize;
			int blockSize = progress.BlockSize;
			ulong blockCount = progress.BlockCount();
			string output;

			Average[] mean = new Average[progress.BlockConsumerCount];
			for(int i = 0;i < mean.Length;i++) mean[i] = new Average();
			for(int i = 0;i < progress.BlockConsumerCount;i++) if(maxNameLength < progress.Name(i).Length) maxNameLength = progress.Name(i).Length + 1;
			if(maxNameLength < "Progress".Length) maxNameLength = "Progress".Length;

			Console.CursorVisible = false;
			output = "*: Buffersize available for hashalgorithm blocksize: " + blockSize + "kb blockCount: " + blockCount + "\n";

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.WindowWidth - maxNameLength - 3) + "]";
			}
			output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.WindowWidth - maxNameLength - 3) + "]";

			Console.Write(output);
			lastLineIndex = Console.CursorTop;

			int barLength = Console.WindowWidth - maxNameLength - 3;
			bool doLoop = !progress.HasFinished;
			do {
				doLoop = !progress.HasFinished;

				bytesProcessed = 0;
				for(int i = 0;i < progress.BlockConsumerCount;i++) {
					mean[i].Add(progress.BlockCount(i));
					bufferSize = mean[i].Calc(5);
					if(bytesProcessed > progress.ProcessedBytes(i) || bytesProcessed == 0) bytesProcessed = progress.ProcessedBytes(i);

					charCount = bufferSize != 0 ? (int)((bufferSize / (double)blockCount) * barLength) : 0;
					charCount = progress.ProcessedBytes(i) == fileSize ? 0 : charCount;

					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 2);
					Console.Write("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));
				}
				Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - 1);
				charCount = (int)((double)bytesProcessed / (double)fileSize * barLength);
				Console.Write("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));

				timeElapsed = (int)progress.TimeElapsed.TotalMilliseconds;
				Console.Write("\n" +
				  "Position: " + ((double)bytesProcessed / (1 << 20)).ToString("0MB") + "/" + ((double)fileSize / (1 << 20)).ToString("0MB") + "  " +
				  "Elapsed time: " + (timeElapsed / 1000d).ToString("0.0").PadLeft(3) + "s  " +
				  "Speed: " + (((double)bytesProcessed / (1 << 20)) / (timeElapsed / 1000d)).ToString("0.00MB/s") +
				  "".PadLeft(80 - Console.CursorLeft)
				);

				Thread.Sleep(100);
			} while(doLoop);

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				if(progress.BlockConsumerObj(i) is HashCalculator) {
					var hc = (HashCalculator)progress.BlockConsumerObj(i);

					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 2);

					if(hc.HashObj is TreeHash) {
						Console.Write(BaseConverter.ToString(hc.HashObj.Hash, eBaseOption.Base32 | eBaseOption.Pad | eBaseOption.Reverse));
					} else {
						Console.Write(BaseConverter.ToString(hc.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse));
					}
				}
			}
			Console.SetCursorPosition(0, lastLineIndex);
			Console.WriteLine();
			Console.CursorVisible = true;
		}
	}

	public class CmdArg {
		public string Key { get; private set; }
		private List<object> items;
		public ReadOnlyCollection<object> Items { get { return items.AsReadOnly(); } }

		private CmdArg() { }

		public static CmdArg Parse(string cmdl) {
			Environment e = new Environment(cmdl);

			try {
				e.SkipEmpty();
				return new CmdArg { Key = "root", items = Parse(e, "") };
			} catch(Exception ex) { throw; }
		}
		private static List<object> Parse(Environment e, string delimiter) {
			List<object> items = new List<object>();

			while(!e.EOL() && !delimiter.Contains(e.Peek())) {
				if(e.Peek() == '-') {
					items.Add(ParseArg(e));
				} else {
					items.Add(ParseName(e, e.Peek() == '"'));
				}
				e.SkipEmpty();
			}

			return items;
		}
		private static CmdArg ParseArg(Environment e) {
			e.Check("-", "Invalid beginning of Key construct");
			CmdArg cmdArg = new CmdArg();

			cmdArg.Key = ParseName(e, false);
			e.SkipEmpty();
			e.Check("{", "Missing delimiter for Key construct");
			e.SkipEmpty();
			cmdArg.items = Parse(e, "}");
			e.Check("}", "Missing delimiter for Key construct");

			return cmdArg;
		}
		private static string ParseName(Environment e, bool isQuoted) {
			e.Check(!isQuoted || e.Peek() == '"', "Invalid beginning of a 'Name' construct");
			e.Check(char.IsLetter(e.Peek()), "Invalid beginning of a 'Name' construct");
			string name = e.Read().ToString();
			while(!e.EOL() && (isQuoted || e.Peek() != ' ') && (!isQuoted || e.Peek() != '"')) name += e.Read().ToString();
			if(isQuoted) e.Check("\"", "Invalid ending of a 'Name' construct");

			return name;
		}
		private class Environment {
			public int CharIndex { get; set; }
			public int LineCount { get { return src.Length; } }

			string src;

			public Environment(string src) { this.src = src; }

			#region Help Methods
			public char Peek() { return src[CharIndex]; }
			public char Read() {
				char srcChar = Peek();

				Advance();
				return srcChar;
			}
			public string ReadToEnd() {
				string val = src.Substring(CharIndex);
				CharIndex = src.Length;
				return val;
			}
			public void SkipEmpty() { while(!EOL() && Peek() == ' ') { Advance(); } }
			public void Advance() { CharIndex++; }
			public int Count() { return src.Length; }
			public bool EOL() { return Count() == CharIndex; }
			public void Check(string match, string errorMsg) {
				bool isInRange = CharIndex + match.Length <= Count();
				bool isEqual = isInRange && match.Equals(src.Substring(CharIndex, match.Length));
				if(!isInRange || !isEqual) {
					throw new Exception(errorMsg);
				} else if(isInRange) {
					CharIndex += match.Length;
				} else {
					CharIndex = Count();
				}
			}
			public void Check(bool isValid, string errorMsg) { if(!isValid) throw new Exception(errorMsg); }
			public void Throw(string errorMsg) { throw new Exception(errorMsg); }
			#endregion
		}
	}
	public interface IAVDModule {
		void Initialize(XElement e);
		void Terminate();
	}
	public interface IFilter {
		ReadOnlyCollection<string> ProcessExtensions { get; }
		bool Filter(string path);
	}
	public interface ILog {
		void Log();
	}

	public class ModuleCollection : Dictionary<string, IAVDModule> { public IEnumerable<T> Items<T>() { return from module in base.Values where module is T select (T)module; } }
#endif
}

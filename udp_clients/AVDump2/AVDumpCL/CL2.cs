/* ###Preprocessor Directives###
 * CreateProviderLogs	    (Needs Debug)
 * UseNullStream		    (Needs Debug)
 * UseFileExtensionCheck    
 * SetArgumentsIfNull	    (Needs Debug)
 * TestAVCHeader		    (Needs Debug)
 * UseAICHHash			    (Needs Debug)
 * HasACreq				    
 * DebugRelease             (Sets Debug)
 * Debug				    
 * Release				    
*/

//#define Debug
/**/
//#define DebugRelease

#if(DebugRelease)
#define Debug
#define HasACreq
#define UseFileExtensionCheck
//#define UseNullStream

#elif(Debug)
#define HasACreq
#define SetArguments
#define UseFileExtensionCheck
//#define UseNullStream

#elif(Release)
#define HasACreq
#define UseFileExtensionCheck
#endif


using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Threading;
using AVDump2CL.Misc;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.HashAlgorithms;
using AVDump2Lib.InfoGathering;
using AVDump2Lib.Misc;
using AVDump2Lib.Acreqing;
using System.Security.Cryptography;
using System.Diagnostics;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.InfoProvider;
using AVDump2CL.Exceptions;
using System.Xml;
using System.Text;

namespace AVDump2CL {
	public class AVD2Environment {
		public Version AppVersion { get { return Assembly.GetExecutingAssembly().GetName().Version; } }

		public ReadOnlyCollection<string> Media { get; private set; }
		public State PauseWhen { get; private set; }
		public State PrintTimeWhen { get; private set; }
		public Boolean DeleteFile { get; private set; }
		public Boolean SupressProgressDisplay { get; private set; }

		public BlockConsumerContainer Container { get; private set; }
	}
	public enum State { Start, End, Processing, FileProcessing }

	public class CL2 {
		#region Fields
		private static Dictionary<char, eSwitches> char2SwitchEnum;
		private static int localPort = 0;
		private static string username;
		private static string password;
		private static string hostAddress = "api.anidb.info";
		private static int hostPort = 9002;
		private static eSwitches switches;

		private static string logPath;
		private static string ed2kListPath;
		private static string doneListPath;
		private static string hashListPath;
		private static string acErrListPath;


		private static string hashLogStyle;
		private static List<string> mediaLst;
		private static List<string> doneListContent;

		private static int retries = 3;
		private static int timeout = 10;
		private static int blockCount = 4;
		private static int blockSize = 4 * 1024;
		private static int monitorSleepDuration = 60000;
		private static List<string> dumpableExtensions;
		private static List<string> processExtensions;

		private static Version appVersion = Assembly.GetExecutingAssembly().GetName().Version;
		private static bool isProcessing;
		#endregion

		static CL2() {
			if(!BitConverter.IsLittleEndian) {
				Console.WriteLine("Warning BitConverter is BigEndian");
				Pause();
			}

			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new List<String>(new string[] { "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			processExtensions.Sort();

			char2SwitchEnum = new Dictionary<char, eSwitches>();
			char2SwitchEnum['y'] = eSwitches.CreqXmlFormat; //Done
			char2SwitchEnum['Y'] = eSwitches.NewCreqXmlFormat; //Done
			char2SwitchEnum['M'] = eSwitches.MediaInfoOutPut; //Done
			char2SwitchEnum['X'] = eSwitches.MediaInfoXMLOutPut; //Done

			char2SwitchEnum['c'] = eSwitches.ExcludeSubFolders; //Done
			char2SwitchEnum['m'] = eSwitches.MonitorFolder; //Done
			char2SwitchEnum['p'] = eSwitches.PauseWhenDone; //Done
			char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone; //Done
			char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
			char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile; //Done
			char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone; //Done
			char2SwitchEnum['o'] = eSwitches.WaitForDumpReply; //Done
			char2SwitchEnum['w'] = eSwitches.SupressProgress; //Done
			char2SwitchEnum['T'] = eSwitches.PrintTotalTimeUsed; //Done

			char2SwitchEnum['0'] = eSwitches.Crc; //Done
			char2SwitchEnum['1'] = eSwitches.Sha1; //Done
			char2SwitchEnum['2'] = eSwitches.Ed2k; //Done
			char2SwitchEnum['3'] = eSwitches.Tiger; //Done
			char2SwitchEnum['4'] = eSwitches.Md4; //Done
			char2SwitchEnum['5'] = eSwitches.Md5; //Done
			char2SwitchEnum['6'] = eSwitches.Tth; //Done
			char2SwitchEnum['7'] = eSwitches.Aich;
			char2SwitchEnum['a'] = eSwitches.UseAllHashes; //Done
			char2SwitchEnum['u'] = eSwitches.PrintElapsedHashingTime; //Done
			char2SwitchEnum['e'] = eSwitches.PrintEd2kLink; //Done
			char2SwitchEnum['d'] = eSwitches.PrintAniDBLink; //Done
			char2SwitchEnum['D'] = eSwitches.OpenInBrowser; //Done
		}

		static void Main(string[] args) {
			AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(GlobalExceptionHandler);
			Console.OutputEncoding = new System.Text.UTF8Encoding(false);

			var xmlDoc = new XmlDocument();
			xmlDoc.Load("AVDump2Cl.xml");

			var configFile = new AVD2ConfigFile(xmlDoc);

#if(SetArguments)
			if(!ParseClOptions(SetArguments())) return;
#else
			if(!ParseClOptions(ArgsParser.Parse(Environment.CommandLine).Skip(1).ToArray())) return;
#endif

#if(UseNullStream)
			Dictionary<string, IBlockConsumer> blockConsumers;
			int i = 1010;


			var container = CreateContainer(blockCount, blockSize);
			while(true) {

				Console.WriteLine("\n\n" + i);

				var length = 2048;
				//var length = (new Random().Next(0, 1024 * 1024) * 1024) + i;
				//File.WriteAllBytes("test.bin", new byte[length]);
				var e = new FileEnvironment(appVersion, container, null, DateTime.Now, 1, 0, length, 0);
				ExecuteBlockConsumers(e, out blockConsumers);
				container.Reset();

				//var tigerThex = (TigerThex)(((HashCalculator)blockConsumers["TIGERThex"]).HashObj);
				//var tigerforTTH = (TigerForTTH)(((HashCalculator)blockConsumers["TIGER"]).HashObj);

				//if(!tigerThex.Hash.SequenceEqual(tigerforTTH.Hash)) {
				//    Console.WriteLine("Hash");
				//    Pause();
				//}

				if((switches & eSwitches.PauseWhenFileDone) != 0) Pause();
				i++;
			}
#endif


			ProcessMedia(new List<string>(mediaLst));
			if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();

			if((switches & eSwitches.PauseWhenDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}
		private static string[] SetArguments() {
			return new string[] {
				//@"G:\Anime\!Movies\Metropolis.mkv",
				//@"G:\Anime\!Movies\[zx] Koukaku Kidoutai 2.0 - Full Movie.mkv",
				//@"D:\My Stuff\Downloads\Hitou Meguri The Animation (Hentai) (Episode 001) [9BDBC2AC] [Shippon].mkv",
				//@"D:\My Stuff\Downloads\Usavich__Episode_022___8CBC6674___RANDOMFAGGOTS_.mkv",
				//@"D:\Anime\",
				//@"D:\Anime\Stalled2\2008\Tentai Senshi Sunred\Tentai Senshi Sunred 05 - FIGHT.05 [ANBU][DTV][848x480].mkv",
				//@"D:\My Stuff\Downloads\Tenchi Muyou! (Episode 023) [DA3523AA] [zx].ogm",
				//@"D:\My Stuff\Downloads\Tenchi Muyou! (Episode 010) [5BAA3E45] [zx].ogm",
				//@"E:\Anime\Processed\Bleach\",
				//@"D:\My Stuff\Downloads\undumpable mp3s\undumpable mp3s\01 神様のいうとおり.MP3",
				//@"E:\Anime\DL\[Mazui]_Ookami-san_-_09_[29E885FA].mkv",
				//@"D:\My Stuff\Downloads\H2 - 33 - [ILA].ogm",
				//@"D:\My Stuff\Downloads\Gensou Maden Saiyuuki - 13 - [ASS].ogm",
				//@"E:\Anime\DL\[Commie] Shiki - 04 [0A9F1BCF].mkv",
				//@"E:\Anime\bla.txt",
				//@"H:\Anime\Stalled\Groove Adventure Rave [Soldats-Choco]\Groove Adventure Rave - 50 -Choco.avi", //(15*9500*1024)
				//@"H:\Anime\Stalled\Groove Adventure Rave [Soldats-Choco]\Groove Adventure Rave - 50 -Choco.avi", //(15*9500*1024)
				//@"E:\Anime\DL\out.mkv",
				//@"E:\Anime\DL\[Rip] Apocalypse Zero\[RiP] Apocalypse Zero - 01 [71C75E2D].mkv",
				//@"D:\Anime\Stalled\[Ripped & Encoded By Ra-Calium] - [H264 - AC3] - [Weiss Kreuz]\[Ripped & Encoded by Ra-Calium] - [Weiss Kreuz] - [H264 - AC3] - [Episode 10] - [Bruder].mkv",
				//@"E:\Anime\DL\cor.tv.dnangel\cor.tv.dnangel.20.[B3D5C421].mkv",
				//@"E:\Anime\Stalled\ponyo_on_the_cliff_by_the_sea[1920x1040.h264.flac.ac3][niizk].mkv",		
				//@"E:\Anime\Stalled\Mouryou no Hako[Aero] 4v2 - The Kasha Incident.mkv",
				//@"D:\My Stuff\Downloads\[Mazui_DudeeBalls-Remux]_Ookami-san_-_06_[720p].mkv",
				//@"E:\Anime\DL\[Doki] Shukufuku no Campanella - 05 (848x480 h264 AAC) [4F458D98].mkv",
				//@"G:\Anime\[Done]Devil May Cry\[AonE-Conclave] Devil May Cry 05 - Mission 05 in Private.mp4",
				//@"H:\Anime\Stalled\[a4e]Alien_Nine\[a4e]Alien_Nine_01[divx5.05].ogm",
				//@"F:\Anime\Rurouni Kenshin\Rurouni Kenshin - The Motion Picture.ogm",
				//@"G:\Anime\!Movies\[zx] Hotaru no Haka - Complete Movie.ogm",
				//@"Z:\Node D\Music\Blind Guardian\Tokyo Tales\Blind Guardian - Tokyo Tales - 08 - Majesty.ogg",
				//@"G:\Anime\[Done]InuYasha\[AHQ] Inuyasha - 022 - A Wicked Smile; Kikyo's Wandering Soul.ogm",
				//@"G:\Anime\[Done]Slayers\Movies\[A-Supreme] Slayers Return - Complete Movie.ogm",
				//@"D:\My Stuff\Downloads\Ai_yori_Aoshi__Enishi__-_S01__Ani-Kraze_-_EAB38DBC_.avi",
				@"H:\Anime\DL\Suzumiya Haruhi no Yuuutsu\[Hitsuji].The.Melancholy.of.Haruhi.Suzumiya.Play.All.DVD.Order.mkv",
				"-q012456ry",
				//"-hlog:\"$CRC32$ $ED2K$\":hlog.txt",
				//"-bsize:10:10",
				//"-log:log.xml",
				//"-acerr:acerr.txt",
				//"-ac:arokh:Anime",
				//"-ext:avi,mpg,mpeg,mp4,rm,asf,flv",
				"-ext:mkv",
				//"-done:done.txt"
			};
		}

		private static bool ParseClOptions(string[] args) {
			if(args.Length == 0) { HelpPrinter(); Pause(); return false; }

			bool firstDeleteCharSet = false;
			bool invalidCl = false;
			string[] parts;
			for(int i = 0;i < args.Length;i++) {
				if(args[i][0].Equals('-')) {
					args[i] = args[i].Substring(1);
					parts = args[i].Split(new char[] { ':' }, StringSplitOptions.RemoveEmptyEntries);
				} else {
					mediaLst.Add(args[i]);
					continue;
				}

				if(parts.Length > 1) {
					if(parts[0] == "ac" && parts.Length == 3) {
						username = parts[1];
						password = parts[2];
						switches |= eSwitches.UseACreqHashes;

					} else if(parts[0] == "ms") {
						if(!int.TryParse(parts[1], out monitorSleepDuration)) invalidCl = true;

					} else if(parts[0] == "hlog") {
						try {
							hashLogStyle = parts[1];
							hashListPath = Path.Combine(AppPath, parts[2]);

							//hashListStream = System.IO.File.Open(parts[2], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "exp") {
						try {
							ed2kListPath = Path.Combine(AppPath, parts[1]);
							//ed2kListStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "ext") {
						processExtensions = new List<string>(parts[1].ToLower().Split(','));
						processExtensions.Sort();
					} else if(parts[0] == "log") {
						try {
							logPath = Path.Combine(AppPath, parts[1]);
							//logStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "lport") {
						if(!int.TryParse(parts[1], out localPort)) invalidCl = true;

					} else if(parts[0] == "done") {
						try {
							doneListPath = Path.Combine(AppPath, parts[1]);
							if(File.Exists(doneListPath)) {
								doneListContent.AddRange(File.ReadAllText(doneListPath).ToLower().Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries));
								doneListContent.Sort();
							}
						} catch(Exception) { invalidCl = true; }
					} else if(parts[0] == "acerr") {
						try {
							acErrListPath = Path.Combine(AppPath, parts[1]);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "tout" && parts.Length == 3) {
						if(!int.TryParse(parts[1], out timeout)) invalidCl = true;
						if(!int.TryParse(parts[2], out retries)) invalidCl = true;

					} else if(parts[0] == "bsize" && parts.Length == 3) {
						if(!int.TryParse(parts[1], out blockSize)) invalidCl = true;
						if(!int.TryParse(parts[2], out blockCount)) invalidCl = true;
						blockSize = blockSize < 512 ? 512 : (blockSize > 64 * 1024 ? 64 * 1024 : blockSize);
						blockCount = blockCount < 1 ? 1 : (blockCount > 64 ? 64 : blockCount);

					} else if(parts[0] == "host" && parts.Length == 3) {
						hostAddress = parts[1];
						hostPort = int.Parse(parts[2]);

					} else if(parts.Length == 1) {
					} else {
						invalidCl = true;
					}

				} else {
					foreach(char sw in parts[0]) {

						if(char2SwitchEnum.ContainsKey(sw)) {
							eSwitches s = char2SwitchEnum[sw];
							if(s == eSwitches.DeleteFileWhenDone && !firstDeleteCharSet) {
								firstDeleteCharSet = true;
							} else {
								switches |= char2SwitchEnum[sw];
							}

						} else {
							invalidCl = true;
						}
					}
				}

				if(invalidCl) {
					Console.WriteLine("Error in Commandline: " + args[i] + ". Aborting!\nPress any alpha-numeric key to exit");
					Pause();
					return false;
				}
			}

			if((switches & (eSwitches.PrintEd2kLink | eSwitches.PrintAniDBLink | eSwitches.OpenInBrowser)) != 0) {
				switches |= eSwitches.Ed2k;
			}

			if((switches & eSwitches.DeleteFileWhenDone) == 0 && firstDeleteCharSet) {
				Console.WriteLine("Set the z switch twice to confirm file deleting (no Recycle Bin!). Aborting!\nPress any alpha-numeric key to exit");
				Pause();
				return false;
			}

			return true;
		}

		private static void MonitorFiles() {


			while(mediaLst.Count != 0) {
				Console.WriteLine("Monitoring folders, press Ctrl+C to stop");
				Thread.Sleep(monitorSleepDuration);
				if(!isProcessing) ProcessMedia(new List<string>(mediaLst));
			}
		}

		private static void ProcessMedia(List<string> mediaLst) {
			DateTime startedOn = DateTime.Now;
			isProcessing = true;

			var searchOption = switches != eSwitches.ExcludeSubFolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;
			var files = GetFiles(mediaLst, searchOption, processExtensions);

			if(files.Count == 0) Console.WriteLine("No files to process");
			long processedBytes = 0, totalBytes = files.Sum(fi => fi.Length);

			FileEnvironment e;
			var container = CreateContainer(blockCount, blockSize);
			for(int i = 0;i < files.Count;i++) {
				e = new FileEnvironment(appVersion, container, files[i], startedOn, files.Count, i, totalBytes, processedBytes);
				try {
					ProcessMediaFile(e);
				} catch(Exception ex) {
					e.AddException("Unhandled error while processing the file.", ex);
				}
				container.Reset();
				processedBytes += e.File.Length;

				if(e.Exceptions.Count != 0) e.Exceptions.Save(Path.Combine(AppPath, "Error"));
			}

			isProcessing = false;
			if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startedOn).TotalSeconds + "s");
		}
		private static void ProcessMediaFile(FileEnvironment e) {
			Console.WriteLine("Folder: " + e.File.DirectoryName);
			Console.WriteLine("Filename: " + e.File.Name);

			var startTime = DateTime.Now;
			Dictionary<string, IBlockConsumer> blockConsumers;
			ExecuteBlockConsumers(e, out blockConsumers);
			if(blockConsumers == null) return;

			if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalSeconds + "s");

			foreach(var blockConsumer in blockConsumers.Values) {
				if(blockConsumer.Error != null) {
					e.AddException("BlockConsumer (" + blockConsumer.Name + ") threw an error.", blockConsumer.Error);
				}
			}

			var infoProvider = CreateInfoProvider(e, blockConsumers);

			WriteLogs(e, blockConsumers, infoProvider);
			HandleSwitches(e, blockConsumers);

			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalSeconds.ToString() + "s");

			Console.WriteLine(); Console.WriteLine();
		}
		private static void ExecuteBlockConsumers(FileEnvironment e, out Dictionary<string, IBlockConsumer> blockConsumers) {
			Stream fileStream;
#if(Debug && UseNullStream)
			//fileStream = new RandomStream(e.TotalBytes);
			fileStream = new NullStream(e.TotalBytes);
#else
			try {
				fileStream = File.Open(e.File.FullName, FileMode.Open, FileAccess.Read, FileShare.None);
			} catch(Exception ex) {
				Console.WriteLine(ex.Message + "\n");
				blockConsumers = null;
				return;
			}
#endif

			using(fileStream) {
				Boolean isMatroska = MatroskaParser.IsMatroskaFile(fileStream);
				Boolean isOgmOgg = OgmOggParser.IsOgmOggFile(fileStream);

				if((switches & eSwitches.UseAllHashes) != 0 || isMatroska || isOgmOgg) {

#if(Debug && UseAICHHash)
					if((switches & (eSwitches.Aich)) != 0) hashContainer.AddHashAlgorithm("AICH");
#endif
					if((switches & (eSwitches.Crc)) != 0) e.Container.AddBlockConsumer("CRC");
					if((switches & (eSwitches.Ed2k)) != 0) e.Container.AddBlockConsumer("ED2K");
					if((switches & (eSwitches.Md4)) != 0) e.Container.AddBlockConsumer("MD4");
					if((switches & (eSwitches.Md5)) != 0) e.Container.AddBlockConsumer("MD5");
					if((switches & (eSwitches.Sha1)) != 0) e.Container.AddBlockConsumer("SHA1");
					if((switches & (eSwitches.Tiger)) != 0) e.Container.AddBlockConsumer("TIGER");
					if((switches & (eSwitches.Tth)) != 0) e.Container.AddBlockConsumer("TTH");
					if(isMatroska) e.Container.AddBlockConsumer("Matroska");
					if(isOgmOgg) e.Container.AddBlockConsumer("Ogm/Ogg");

					BlockConsumerContainer.Progress progress = e.Container.Start(fileStream);

					if((switches & eSwitches.SupressProgress) == 0) {
						try {
							DisplayHashBuffer(e, progress);
						} catch(Exception ex) {
							Console.WriteLine();
							e.AddException("Error in DisplayHashBuffer", ex);
						}
					}
				}

				try {
					blockConsumers = e.Container.Join().ToDictionary(b => b.Name);
				} catch(Exception ex) {
					e.AddException("Error while waiting for BlockConsumers", ex);
					blockConsumers = null;
				}
			}
		}
		private static void WriteLogs(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers, InfoProviderBase p) {


			#region Generate/Write Logs
			string log = "";
			if((switches & (eSwitches.CreqXmlFormat | eSwitches.NewCreqXmlFormat | eSwitches.MediaInfoXMLOutPut | eSwitches.MediaInfoOutPut)) != 0) {
				log = e.File.FullName + Environment.NewLine;
			}

			if((switches & eSwitches.CreqXmlFormat) != 0) {
				var tw = new StringWriter();
				Info.CreateAVDumpLog(p).Save(new SafeXmlWriter(tw));
				log += tw.ToString();
			}
			if((switches & eSwitches.NewCreqXmlFormat) != 0) {
				var tw = new StringWriter();
				Info.CreateNewAVDumpLog(p).Save(new SafeXmlWriter(tw));
				log += tw.ToString();
			}
			if((switches & eSwitches.MediaInfoXMLOutPut) != 0) {
				var tw = new StringWriter();
				Info.CreateMediaInfoXMLLog(e.File.FullName, blockConsumers.Values).Save(new SafeXmlWriter(tw));
				log += tw.ToString();
			}
			if((switches & eSwitches.MediaInfoOutPut) != 0) log += Info.CreateMediaInfoDump(e.File.FullName);

			if(logPath != null && !String.IsNullOrEmpty(log)) File.AppendAllText(logPath, log + Environment.NewLine + Environment.NewLine);

			#endregion

			Console.WriteLine(log);
			Console.WriteLine();

#if(Debug && OgmOggDebug)
			IBlockConsumer bc;
			if(blockConsumers.TryGetValue("Ogm/Ogg", out bc) && bc != null) {
				OgmOggFile file = ((OgmOggParser)bc).OgmOggFileObj;
				var path = Path.Combine(AppPath, "OgmOggInfo");
				if(!Directory.Exists(path)) Directory.CreateDirectory(path);
				if(file.headers != null) {
					foreach(var item in file.headers) {
						File.WriteAllBytes(Path.Combine(Path.Combine(AppPath, "OgmOggInfo"), e.File.Name + " " + item.Key + ".bin"), item.Value.ToArray());
					}
				}
			}
#endif

			bool wasACreqSent = false;
#if(HasACreq)
			if(dumpableExtensions.BinarySearch(e.File.Extension.Substring(1).ToLower()) >= 0 && !(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				var result = DoACreq(e, p);
				wasACreqSent = result == ACreqResult.ACreqSent || result == ACreqResult.AsyncCall;
			}
#endif

			#region DoneLog Stream Writing
			if(doneListPath != null && (wasACreqSent || username == null || password == null) && e.Exceptions.Count == 0) {
				File.AppendAllText(doneListPath, e.File.FullName + Environment.NewLine);
				int index = doneListContent.BinarySearch(e.File.FullName.ToLower());
				if(index < 0) doneListContent.Insert(~index, e.File.FullName);
			}
			#endregion

			#region Ed2kLog Stream Writing
			Ed2k ed2k = null;
			if(blockConsumers.ContainsKey("ED2K")) ed2k = (Ed2k)((HashCalculator)blockConsumers["ED2K"]).HashObj;
			if(ed2k != null) {
				byte[] blueEd2kHash = ed2k.BlueHash; //Handle Ed2k screwup
				byte[] redEd2kHash = ed2k.Hash;

				if(ed2kListPath != null) {
					string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash) + "|/";
					if(!ed2k.BlueIsRed) {
						ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.BlueHash) + "|/";
					}
					File.AppendAllText(ed2kListPath, ed2kStr + Environment.NewLine);
				}
			}
			#endregion

			#region HashLog Stream Writing
			if(hashListPath != null) {
				string formattedStr = hashLogStyle;
				foreach(HashCalculator hashExecute in blockConsumers.Values.Where(blockConsumer => { return blockConsumer is HashCalculator; })) {
					if(hashExecute.HashObj is Ed2k) {
						string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash) + "|/";
						if(!((Ed2k)hashExecute.HashObj).BlueIsRed) {
							ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).BlueHash) + "|/";
						}

						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					} else {
						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash));
					}
				}
				File.AppendAllText(hashListPath, formattedStr + Environment.NewLine);
			}
			#endregion
		}
		private static void HandleSwitches(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers) {
			Ed2k ed2k = null;
			if(blockConsumers.ContainsKey("ED2K")) ed2k = (Ed2k)((HashCalculator)blockConsumers["ED2K"]).HashObj;

			if((switches & eSwitches.OpenInBrowser) != 0 || (switches & eSwitches.PrintAniDBLink) != 0) {
				string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + e.File.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash);
				if((switches & eSwitches.OpenInBrowser) != 0) System.Diagnostics.Process.Start(aniDBLink);
				if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(aniDBLink);
			}
			if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash) + "|/");

			if((switches & eSwitches.DeleteFileWhenDone) != 0 && e.Exceptions.Count == 0) System.IO.File.Delete(e.File.FullName);
			if((switches & eSwitches.PauseWhenFileDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}

#if(HasACreq)
		private static ACreqResult DoACreq(FileEnvironment e, InfoProviderBase infoProvider) {
			MemoryStream stream = new MemoryStream();
			Info.CreateAVDumpLog(infoProvider).Save(new SafeXmlWriter(stream, new UTF8Encoding(), Formatting.None));
			byte[] creqBytes = stream.ToArray();

			try {
				int tries = 0;
				ACreqResult result;
				do {
					Console.Write("Sending Creq... ");
					result = ACreq.Commit(new ACreqArgs("avdumplib", appVersion.Build, hostAddress, hostPort, localPort, username, password, creqBytes, (eSwitches.WaitForDumpReply & switches) != 0 ? timeout * 1000 : -1));
					tries++;
					switch(result) {
						case ACreqResult.ACreqSent: Console.WriteLine("Done."); break;
						case ACreqResult.AsyncCall: break;
						default:
							Console.ForegroundColor = ConsoleColor.Red;
							Console.WriteLine("Failed. Reason: " + System.Enum.GetName(typeof(ACreqResult), result));
							Console.ResetColor();

							if(!string.IsNullOrEmpty(acErrListPath)) File.AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.ToString() + Environment.NewLine);
							break;
					}
				} while(result == ACreqResult.TimeOut && tries <= retries);
				return result;
			} catch(Exception ex) {
				e.AddException("ACreqing exception", ex);
				return ACreqResult.Error;
			}
		}
#endif

		private static void DisplayHashBuffer(FileEnvironment e, BlockConsumerContainer.Progress progress) {
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0;
			int lastLineIndex = 0, maxNameLength = 0;
			long fileSize = progress.StreamSize;
			string output;

			Average[] mean = new Average[progress.BlockConsumerCount];
			for(int i = 0;i < mean.Length;i++) mean[i] = new Average();
			for(int i = 0;i < progress.BlockConsumerCount;i++) if(maxNameLength < progress.Name(i).Length) maxNameLength = progress.Name(i).Length + 1;
			if(maxNameLength < "Progress".Length) maxNameLength = "Progress".Length;

			Console.CursorVisible = false;
			output = "Bar: Buffer usage | blocksize: " + blockSize + "KB | blockCount: " + blockCount + "\n";

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.BufferWidth - maxNameLength - 4) + "]\n";
			}
			output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.BufferWidth - maxNameLength - 4) + "]\n\n";

			Console.Write(output);
			lastLineIndex = Console.CursorTop;

			bool doLoop;
			int barLength = Console.BufferWidth - maxNameLength - 4;
			do {
				doLoop = !progress.HasFinished;

				bytesProcessed = 0;
				for(int i = 0;i < progress.BlockConsumerCount;i++) {
					mean[i].Add(progress.BlockCount(i));
					bufferSize = mean[i].Calc(10);
					if(bytesProcessed > progress.ProcessedBytes(i) || bytesProcessed == 0) bytesProcessed = progress.ProcessedBytes(i);

					charCount = bufferSize != 0 ? (int)((bufferSize / (double)blockCount) * barLength) : 0;
					charCount = progress.ProcessedBytes(i) == fileSize ? 0 : charCount;

					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 3);
					Console.Write("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));
				}
				Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - 2);
				charCount = fileSize != 0 ? (int)((double)bytesProcessed / (double)fileSize * barLength) : barLength;
				Console.WriteLine("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));

				output =
				  "Position: " + (bytesProcessed >> 20).ToString().PadLeft(3) + "MB/" + (fileSize >> 20) + "MB  " +
				  "Elapsed time: " + progress.TimeElapsed.ToFormatedString() + " " +
				  "Speed: " + ((int)(bytesProcessed / progress.TimeElapsed.TotalSeconds) >> 20) + "MB/s";
				output += "".PadLeft(output.Length < Console.BufferWidth ? Console.BufferWidth - output.Length - 1 : 0, ' ');
				Console.WriteLine(output);

				bytesProcessed += e.ProcessedBytes;
				if(bytesProcessed != 0) {
					var totalTimeElapsed = DateTime.Now - e.StartedOn;
					output =
					  "Files: " + (e.ProcessedFiles + 1) + "/" + e.TotalFiles + " " +
					  "Bytes: " + (bytesProcessed >> 20) + "MB/" + (e.TotalBytes >> 20) + "MB " +
					  "Elapsed: " + totalTimeElapsed.ToFormatedString() + " " +
					  "ETA: " + TimeSpan.FromSeconds(e.TotalBytes * (totalTimeElapsed.TotalSeconds / bytesProcessed) - totalTimeElapsed.TotalSeconds + 0.5).ToFormatedString();
					output += "".PadLeft(output.Length < Console.BufferWidth ? Console.BufferWidth - output.Length - 1 : 0, ' ');
					Console.Write(output);
				}

				Thread.Sleep(80);
			} while(doLoop);

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 3);
				Console.Write(progress.BlockConsumerObj(i).ToString());
			}
			Console.SetCursorPosition(0, lastLineIndex);
			Console.WriteLine();
			Console.CursorVisible = true;
		}

		private static BlockConsumerContainer CreateContainer(int blockCount, int blockSize) {
			BlockConsumerContainer container = new BlockConsumerContainer(blockCount, blockSize * 1024);
			container.RegisterBlockConsumer("TIGER", new HashCalculator("TIGER", new TigerForTTH(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("AICH", new HashCalculator("AICH", new Aich(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("SHA1", new HashCalculator("SHA1", new SHA1CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("CRC", new HashCalculator("CRC", new Crc32(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("MD4", new HashCalculator("MD4", new Md4(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("MD5", new HashCalculator("MD5", new MD5CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("TTH", new HashCalculator("TTH", new TTH(Environment.ProcessorCount), h => BaseConverter.ToString(h.Hash, BaseOption.Base32)));
			container.RegisterBlockConsumer("Matroska", new MatroskaParser("Matroska"));
			container.RegisterBlockConsumer("Ogm/Ogg", new OgmOggParser("Ogm/Ogg"));

			container.RegisterBlockConsumer("ED2K", new HashCalculator("ED2K", new Ed2k(),
				h => BaseConverter.ToString(h.Hash) + (((Ed2k)h).BlueIsRed ? "" : " | " + BaseConverter.ToString(((Ed2k)h).BlueHash))
			));

			return container;
		}
		private static InfoProviderBase CreateInfoProvider(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers) {
			MatroskaProvider mkvProvider = null;
			OgmOggProvider ogmOggProvider = null;
			MediaInfoProvider milProvider = null;
			HashInfoProvider hashProvider = null;
			FileExtensionProvider extProvider = null;
			CompositeInfoProvider p = null;

			try {
				if(blockConsumers.ContainsKey("Matroska")) {
					var matroskaFile = ((MatroskaParser)blockConsumers["Matroska"]).MatroskaFileObj;
					if(matroskaFile != null) mkvProvider = new MatroskaProvider(matroskaFile);
				}
			} catch(Exception ex) { e.AddException("Failed to create MatroskaProvider", ex); }
			try {
				if(blockConsumers.ContainsKey("Ogm/Ogg")) {
					var ogmOggFile = ((OgmOggParser)blockConsumers["Ogm/Ogg"]).OgmOggFileObj;
					if(ogmOggFile != null) ogmOggProvider = new OgmOggProvider(ogmOggFile);
				}
			} catch(Exception ex) { e.AddException("Failed to create OgmOggProvider", ex); }
			try {
				milProvider = new MediaInfoProvider(e.File.FullName);
			} catch(Exception ex) { e.AddException("Failed to create MediaInfoProvider", ex); }

			try {
				hashProvider = new HashInfoProvider(blockConsumers.Values.Where(b => b is HashCalculator).Cast<HashCalculator>());
			} catch(Exception ex) { e.AddException("Failed to create HashInfoProvider", ex); }

			try {
				extProvider = new FileExtensionProvider(e.File.FullName);
			} catch(Exception ex) { e.AddException("Failed to create FileExtensionProvider", ex); }

			var providers = new Collection<InfoProviderBase>();
			if(mkvProvider != null) providers.Add(mkvProvider);
			if(ogmOggProvider != null) providers.Add(ogmOggProvider);
			if(extProvider != null) providers.Add(extProvider);
			if(milProvider != null) providers.Add(milProvider);
			if(hashProvider != null) providers.Add(hashProvider);
			p = new CompositeInfoProvider(providers);

#if(Debug && CreateProviderLogs)
				string path = Path.Combine(AppPath, "ProviderLogs");
				if(!Directory.Exists(path)) Directory.CreateDirectory(path);
				path = Path.Combine(path, System.IO.Path.GetFileNameWithoutExtension(filePath) + "_*" + System.IO.Path.GetExtension(filePath) + ".xml");

				Info.CreateAVDumpLog(p).Save(path.Replace("*", "Composite"));
				Info.CreateAVDumpLog(milProvider).Save(path.Replace("*", "MIL"));
				Info.CreateAVDumpLog(filePath, blockConsumers).Save(path.Replace("*", "ACreq"));
				if(mkvProvider != null) Info.CreateAVDumpLog(mkvProvider).Save(path.Replace("*", "MKV"));
#endif
			return p;
		}
		private static List<FileInfo> GetFiles(List<string> mediaLst, SearchOption searchOption, List<string> validExtensions) {
			List<FileInfo> files = new List<FileInfo>();
			foreach(var media in mediaLst) {
				if(System.IO.File.Exists(media)) {
					if(validExtensions.BinarySearch(System.IO.Path.GetExtension(media).Substring(1).ToLower()) >= 0) files.Add(new FileInfo(media));
				} else if(System.IO.Directory.Exists(media)) {
					files.AddRange(FileGetter.GetFiles(media, validExtensions, (switches & eSwitches.ExcludeSubFolders) == 0));
				} else {
					//TODO Error?
				}
			}

			if((switches & eSwitches.RandomFileOrder) != 0) {
				Random rng = new Random();
				int n = files.Count;
				while(n > 1) {
					n--;
					int k = rng.Next(n + 1);
					var value = files[k];
					files[k] = files[n];
					files[n] = value;
				}
			}

			return files.Where(fi => doneListContent.BinarySearch(fi.FullName.ToLower()) < 0).ToList();
		}


		#region Empty args help
		private static void HelpPrinter() {
			StringBuilder sb = new StringBuilder();

			for(int i = 0;i < help.Length;i++) {
				if(help[i] == '\x3') {
					Console.Write(sb.ToString()); sb.Length = 0;
					Console.ForegroundColor = (ConsoleColor)byte.Parse(help.Substring(i + 1, 2).ToString()); i += 2;
				} else if(help[i] == '\xF') {
					Console.Write(sb.ToString()); sb.Length = 0;
					Console.ResetColor();
				} else {
					sb.Append(help[i]);
				}
			}
			Console.Write(sb.ToString()); sb.Length = 0;
		}


		/*static string help =
@"help:      http://wiki.anidb.info/w/Avdump
usage:     avdump [-options] <media file/folder> [<media file/folder> ...]
options:    (one letter switches can be put in one string)
  input:    (all multiple letter switches requires colon)
   ac      autocreq: '-ac:<username>:<api password>' YOU SHOULD USE THIS
   ms      monitor sleep duration in milliseconds, default is " + monitorSleepDuration.ToString() + @"
   exp     export Ed2k-links to file
   ext     comma separated extension list 
   hlog    export hashes to file '-hlog:" + "\"$CRC32$ $ED2K$\"" + @"':filepath
   log     write output to file
   acerr   write filepaths of failed dumps to file
   lport   local udp Port used by ac
   done	   save processed-filepaths to file and exclude existing from proc
   tout    timeout: '-tout:<seconds>:<number Of retries>'
   bsize   buffer for hashing: '-bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>'
   host    host name of anidb udp api server, default is '" + hostAddress + ":" + hostPort + @"'
  output:   (exclusive)
   y       XML creq format
   Y       AVD2 creq Dump
   M       MediaInfo Dump
   X       MediaInfo XML Dump
  control:
   c       do _not_ recurse Into subfolders
   m       Monitor folder(s)
   p       pause when done (hold cmd window)
   t       print Time used for each file
   T       print total elapsed time
   q       pause after each file
   r       random file Order
   z       delete files after parsing
   o       wait for Response when sending dumps
   w       supress progress (silent)
  hash:
   0       CRC32 (cyclic Redundancy check)
   1       SHA1  (secure hash algorithm 1)
   2       ED2K  (edonkey2000 hash)
   3       TIGER 
   4       MD4   (message-digest algorithm 4)
   5       MD5   (message-digest algorithm 5)
   6       TTH   (tiger tree Hash)
   a       all (available) hash algorithms
   u       print elapsed time after hashing
   e       print ed2k link
   d       print AniDB link
   D       open AniDB link in default browser.

press any key to exit";*/

		static string help =
@"help:    http://wiki.anidb.info/w/Avdump

Usage:   AVDump2CL [-options] <media file/folder> [<media file/folder> ...]

Options: (one letter switches can be put in one string)
		 (all multiple letter switches requires colon)

  Autocreqing:
   ac      Autocreqing (example: -ac:<username>:<api_key>) " + '\x3' + "12YOU SHOULD USE THIS" + '\xF' + @"
   host    Hostname of AniDB UDP API server (default: " + hostAddress + ":" + hostPort + @")
   lport   Local UDP port used for autocreqing
   tout    Timeout for sending dumps (example: -tout:<seconds>:<retries>)

  Input:
   bsize   Buffer size for hashing (example: 
			   -bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>)
   ext     Comma separated extension list (example: -ext:avi,mkv)
   ms      Monitor sleep duration in milliseconds (default: " + monitorSleepDuration.ToString() + @")

  Logging:
   acerr   Write filepaths of failed dumps to file (example: -acerr:acerr.txt)
   exp     Export eD2k-links to file (example: -exp:ed2k.txt)
   done    Save processed filepaths to file and exclude existing from
			   processing (example: -done:done.txt)
   hlog    Export hashes to file (example: -hlog:" + "\"$CRC32$ $ED2K$\"" + @"':hlog.txt)
   log     Write output to file (example: -log:log.txt)

  Output:
   M       MediaInfo Dump
   X       MediaInfo XML Dump
   y       XML creq format
   Y       AVD2 creq Dump

  Control:
   c       Do _not_ recurse into subfolders
   m       Monitor folder(s)
   o       Wait for response when sending dumps
   p       Pause when done (hold cmd window)
   q       Pause after each file
   r       Random file order
   t       Print time used for each file
   T       Print total elapsed time
   w       Supress progress (silent)
   z       Delete files after parsing

  Hash:
   0       For CRC32 (because the CRC of a 0 byte file is 00000000)
   1       For SHA-1 (because of the 1)
   2       For eD2k (because of the 2)
   3       For Tiger (because a tiger in the house minimizes the Three Great
			   Risks -- thieves, fire, and evil spirits)
   4       For MD4 (because of the 4)
   5       For MD5 (because of the 5)
   6       For TTH ... because that's what's left :p
   a       All (available) hash algorithms
   e       Print eD2k-link
   d       Print AniDB link
   D       Open AniDB link in default browser
   u       Print elapsed time after hashing

Press any key to exit";
		#endregion

		public static string AppPath { get { return System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location); } }
		private static void GlobalExceptionHandler(object sender, UnhandledExceptionEventArgs e) {
			ExceptionXElement ex = new ExceptionXElement((Exception)e.ExceptionObject, false);
			string path = Path.Combine(AppPath, "Error");
			if(!Directory.Exists(path)) Directory.CreateDirectory(path);
			string fileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";

			ex.Save(XmlWriter.Create(new SafeXmlWriter(Path.Combine(path, fileName), Encoding.Unicode), new XmlWriterSettings { OmitXmlDeclaration = true }));
		}
		private static void Pause() {
			ConsoleKeyInfo cki;
			do {
				cki = Console.ReadKey();
			} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
		}
	}
}
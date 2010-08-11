// Copyright (C) 2009 DvdKhl 
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.using System;
/**/

//In Debug Build
//#define CreateProviderLogs
//#define UseNullStream
#define UseFileExtensionCheck
#define SetArgumentsIfNull
//define UseAICHHash
#undef Debug

//In Release Build
#define Debug

#undef HasAcreq
//#define HasAcreq


using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading;
using System.Xml.Linq;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.Dump;
using AVDump2Lib.HashAlgorithms;
using AVDump2Lib.InfoGathering;
using AVDump2Lib.Misc;
using System.Collections.ObjectModel;

namespace AVDump2CL {
	public class CL {
		#region Fields
		private static Dictionary<char, eSwitches> char2SwitchEnum;
		private static int port = 0;
		private static string username;
		private static string password;
		private static string host = "api.anidb.info";
		private static eSwitches switches;

		private static Stream logStream;
		private static Stream ed2kListStream;
		private static Stream doneListStream;
		private static Stream hashListStream;
		private static string hashLogStyle;
		private static List<string> mediaLst;
		private static List<string> doneListContent;

		private static int retries = 3;
		private static int timeout = 10;
		private static int blockCount = 16;
		private static int blockSize = 4 * 1024;
		private static int monitorSleepDuration = 60000;
		private static List<string> dumpableExtensions;
		private static List<string> processExtensions;

		private static bool isProcessing;
		#endregion

		static CL() {
			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new List<String>(new string[] { "mpg", "mpeg", "ts", "rm", "rmvb", "asf", "wmv", "mov", "ogm", "mp4", "mkv", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "mpg", "mpeg", "ts", "rm", "rmvb", "asf", "wmv", "mov", "ogm", "mp4", "mkv", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z" });
			processExtensions.Sort();

			char2SwitchEnum = new Dictionary<char, eSwitches>();
			char2SwitchEnum['y'] = eSwitches.CreqXmlFormat; //Done
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
			char2SwitchEnum['1'] = eSwitches.Ed2k; //Done
			char2SwitchEnum['2'] = eSwitches.Md5; //Done
			char2SwitchEnum['3'] = eSwitches.Sha1; //Done
			char2SwitchEnum['5'] = eSwitches.Tth; //Done
			char2SwitchEnum['6'] = eSwitches.Tiger; //Done
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


#if(Debug)
#if(SetArgumentsIfNull)
			if(args.Length == 0) {
				args = new string[] {
					@"E:\Anime\Processed",
					//@"E:\Anime\DL\cor.tv.dnangel\cor.tv.dnangel.20.[B3D5C421].mkv",
					//@"E:\Anime\Stalled\ponyo_on_the_cliff_by_the_sea[1920x1040.h264.flac.ac3][niizk].mkv",
					//@"E:\Anime\Stalled\Mouryou no Hako[Aero] 4v2 - The Kasha Incident.mkv",
					//@"D:\My Stuff\Downloads\[Mazui_DudeeBalls-Remux]_Ookami-san_-_06_[720p].mkv",
					//"-oy",
					//"-log:log.xml"
				};
			}
#endif
			//System.IO.File.AppendAllText("Args.txt", args.Aggregate("args[]:", (acc, str) => { return acc + " " + str; }) + "\n");
			//System.IO.File.AppendAllText("Args.txt", Environment.CommandLine + "\n");
#endif


			if(!ParseClOptions(args)) return;

			DateTime startTime = DateTime.Now;
			ProcessMedia(new Queue<string>(mediaLst));

			if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();
			if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startTime).TotalSeconds + "s");

			if(ed2kListStream != null) ed2kListStream.Dispose();
			if(doneListStream != null) doneListStream.Dispose();
			if(logStream != null) logStream.Dispose();

			if((switches & eSwitches.PauseWhenDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}


		private static bool ParseClOptions(string[] args) {
			if(args.Length == 0) {
				Console.WriteLine(help); Pause();
				return false;
			}

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
						switches |= eSwitches.UseAllHashes;

					} else if(parts[0] == "ms") {
						if(!int.TryParse(parts[1], out monitorSleepDuration)) invalidCl = true;

					} else if(parts[0] == "hlog") {
						try {
							hashLogStyle = parts[1];
							hashListStream = System.IO.File.Open(parts[2], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "exp") {
						try {
							ed2kListStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "ext") {
						processExtensions = new List<string>(parts[1].Split(','));
					} else if(parts[0] == "log") {
						try {
							logStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "port") {
						if(!int.TryParse(parts[1], out port)) invalidCl = true;

					} else if(parts[0] == "done") {
						try {
							doneListStream = System.IO.File.Open(parts[1], FileMode.OpenOrCreate, FileAccess.ReadWrite);
							StreamReader sr = new StreamReader(doneListStream);
							doneListContent.AddRange(sr.ReadToEnd().Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries));
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "tout" && parts.Length == 3) {
						if(!int.TryParse(parts[1], out timeout)) invalidCl = true;
						if(!int.TryParse(parts[2], out retries)) invalidCl = true;

					} else if(parts[0] == "bsize" && parts.Length == 3) {
						if(!int.TryParse(parts[1], out blockSize)) invalidCl = true;
						if(!int.TryParse(parts[2], out blockCount)) invalidCl = true;

					} else if(parts[0] == "host") {
						host = parts[1];

					} else if(parts.Length == 1) {
					} else {
						invalidCl = true;
					}

				} else {
					foreach(char sw in parts[0]) if(char2SwitchEnum.ContainsKey(sw)) switches |= char2SwitchEnum[sw]; else invalidCl = true;
				}

				if(invalidCl) {
					Console.WriteLine("Error in Commandline: " + args[i] + ". Aborting!\nPress any alpha-numeric key to exit");
					Pause();
					return false;
				}
			}
			return true;
		}

		private static void MonitorFiles() {
			while(true) {
				Console.WriteLine("Monitoring folders, press Ctrl+C to stop");
				Thread.Sleep(monitorSleepDuration);
				if(!isProcessing) ProcessMedia(new Queue<string>(mediaLst));
			}
		}

		private static void ProcessMedia(Queue<string> mediaLst) {
			isProcessing = true;

			SearchOption searchOption = switches != eSwitches.ExcludeSubFolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;
			List<string> files = new List<string>();
			while(mediaLst.Count != 0) {
				string media = mediaLst.Dequeue();

				if(System.IO.File.Exists(media)) {
					files.Add(media);
				} else if(System.IO.Directory.Exists(media)) {
					foreach(string ext in processExtensions) files.AddRange(System.IO.Directory.GetFiles(media, "*." + ext, searchOption));
				} else {
					//TODO Error?
				}
			}


			for(int i = 0;i < files.Count;i++) {
				Console.WriteLine("Processing (" + (i + 1) + " of " + files.Count + "):");
				ProcessMediaFile(files[i]);
			}
			isProcessing = false;
		}

		private static void ProcessMediaFile(string filePath) {
			bool error = false;
			string fileExt = System.IO.Path.GetExtension(filePath).Substring(1);

#if(!Debug || UseFileExtensionCheck)
			if (doneListContent.Contains(filePath) || processExtensions.BinarySearch(fileExt) < 0) {
				return;
			}
#endif

			Stream stream = System.IO.File.OpenRead(filePath);
			//System.IO.BufferedStream bs = new BufferedStream(stream, 1024);


#if(Debug && UseNullStream)
			stream = new NullStream(1024 * 1024 * 1024);
			stream = new MemoryStream(System.Text.Encoding.ASCII.GetBytes("".PadLeft(1025, 'A'))); //System.IO.File.OpenRead(filePath);
			stream = new MemoryStream(new byte[] { 0 }); //System.IO.File.OpenRead(filePath);
#endif

			#region Hashing
			DateTime startTime = DateTime.Now;
			BlockConsumerContainer blockConsumerContainer = new BlockConsumerContainer();
			if((switches & eSwitches.UseAllHashes) != 0 || System.IO.Path.GetExtension(filePath).ToLower().Equals(".mkv")) {
				Console.WriteLine("Folder: " + System.IO.Path.GetDirectoryName(filePath));
				Console.WriteLine("Filename: " + System.IO.Path.GetFileName(filePath));

#if(Debug && UseAICHHash)
				if((switches & (eSwitches.Aich)) != 0) hashContainer.AddHashAlgorithm(new Aich(), "AICH");
#endif
				if((switches & (eSwitches.Crc)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new Crc32(), "CRC"));
				//if((switches & (eSwitches.Tiger)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new TigerThex(), "TIGER"));
				if((switches & (eSwitches.Ed2k)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new Ed2k(), "ED2K"));
				if((switches & (eSwitches.Sha1)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "SHA1"));
				if((switches & (eSwitches.Tth)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new TreeHash(new TigerThex(), new TigerThex(), 1024), "TTH"));
				if((switches & (eSwitches.Md5)) != 0) blockConsumerContainer.AddBlockConsumer(new HashCalculator(new System.Security.Cryptography.MD5CryptoServiceProvider(), "MD5"));
				if(System.IO.Path.GetExtension(filePath).ToLower().Equals(".mkv")) blockConsumerContainer.AddBlockConsumer(new MatroskaFileInfo("MKVParser"));

				BlockConsumerContainer.Progress progress = blockConsumerContainer.Start(blockCount, stream, blockSize * 1024);
				if((switches & eSwitches.SupressProgress) == 0) {
					try {
						DisplayHashBuffer(progress);
					} catch(Exception ex) {
						WriteException(ex);
					}
				}
			}
			IEnumerable<IBlockConsumer> blockConsumers = blockConsumerContainer.Join();
			#endregion


			MatroskaProvider mkvProvider = null;
			if(blockConsumers.Any(b => b.Name.Equals("MKVParser"))) mkvProvider = new MatroskaProvider((MatroskaFileInfo)blockConsumers.First(b => b.Name.Equals("MKVParser")));
			var milProvider = new MediaInfoProvider(filePath);
			var hashProvider = new HashInfoProvider(blockConsumers.Where(b => !b.Name.Equals("MKVParser")).Cast<HashCalculator>());

			Collection<InfoProvider> providers = new Collection<InfoProvider>();
			if(mkvProvider != null) providers.Add(mkvProvider);
			providers.Add(milProvider);
			providers.Add(hashProvider);
			var p = new CompositeInfoProvider(providers);

#if(Debug && CreateProviderLogs)
			Info.CreateAVDumpLog(p).Save(System.IO.Path.GetFileNameWithoutExtension(filePath) + "_Composite" + System.IO.Path.GetExtension(filePath) + ".xml");
			Info.CreateAVDumpLog(milProvider).Save(System.IO.Path.GetFileNameWithoutExtension(filePath) + "_MIL" + System.IO.Path.GetExtension(filePath) + ".xml");
			if(mkvProvider != null) Info.CreateAVDumpLog(mkvProvider).Save(System.IO.Path.GetFileNameWithoutExtension(filePath) + "_MKVParser" + System.IO.Path.GetExtension(filePath) + ".xml");
#endif

			if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalSeconds + "s");

			#region Log Output
			string log = "";
			if((switches & eSwitches.CreqXmlFormat) != 0) {
				StringWriter sw = new StringWriter();
				Info.CreateAVDumpLog(p).Save(sw);
				log += sw.ToString();
			}
			if((switches & eSwitches.MediaInfoXMLOutPut) != 0) {
				StringWriter sw = new StringWriter();
				sw = new StringWriter();
				Info.CreateMediaInfoXMLLog(filePath, blockConsumers).Save(sw);
				log += sw.ToString();
			}
			if((switches & eSwitches.MediaInfoOutPut) != 0) {
				log += Info.CreateMediaInfoDump(filePath);
			}

			Console.WriteLine(log);
			Console.WriteLine();

			if(logStream != null && !String.IsNullOrEmpty(log)) {
				byte[] infoBytes = System.Text.Encoding.UTF8.GetBytes(log + Environment.NewLine + Environment.NewLine);
				logStream.Write(infoBytes, 0, infoBytes.Length);
			}
			#endregion

			#region DoneLog Stream Writing
			if(doneListStream != null) {
				byte[] donePathBytes = System.Text.Encoding.UTF8.GetBytes(filePath + Environment.NewLine);
				doneListStream.Write(donePathBytes, 0, donePathBytes.Length);
				doneListContent.Add(filePath);
			}
			#endregion

			#region Ed2kLog Stream Writing
			HashCalculator ed2kHashExecute = (HashCalculator)blockConsumers.FirstOrDefault(blockConsumer => { return blockConsumer is HashCalculator && ((HashCalculator)blockConsumer).HashObj is Ed2k; });
			Ed2k ed2k = (Ed2k)(ed2kHashExecute != null ? ed2kHashExecute.HashObj : null);
			if(ed2k != null) {
				byte[] blueEd2kHash = ed2k.BlueHash; //Handle Ed2k screwup
				byte[] redEd2kHash = ed2k.Hash;

				if(ed2kListStream != null) {
					string ed2kStr = "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(ed2k.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
					if(!ed2k.BlueIsRed()) {
						ed2kStr += "*" + "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(ed2k.BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
					}

					byte[] ed2kBytes = System.Text.Encoding.UTF8.GetBytes(ed2kStr + Environment.NewLine);
					ed2kListStream.Write(ed2kBytes, 0, ed2kBytes.Length);
				}
			}
			#endregion

			#region HashLog Stream Writing
			if(hashListStream != null) {
				string formattedStr = hashLogStyle;
				foreach(HashCalculator hashExecute in blockConsumers.Where(blockConsumer => { return blockConsumer is HashCalculator; })) {
					if(hashExecute.HashObj is Ed2k) {
						string ed2kStr = "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
						if(!((Ed2k)hashExecute.HashObj).BlueIsRed()) {
							ed2kStr += "*" + "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
						}

						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					} else {
						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse));
					}
				}
				byte[] formattedStrBytes = System.Text.Encoding.UTF8.GetBytes(formattedStr + Environment.NewLine);
				hashListStream.Write(formattedStrBytes, 0, formattedStrBytes.Length);
			}
			#endregion

			if((switches & eSwitches.OpenInBrowser) != 0 || (switches & eSwitches.PrintAniDBLink) != 0) {
				string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + stream.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse);
				if((switches & eSwitches.OpenInBrowser) != 0) System.Diagnostics.Process.Start(aniDBLink);
				if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(aniDBLink);
			}
			if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(ed2k.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/");

			if((switches & eSwitches.DeleteFileWhenDone) != 0 && !error) System.IO.File.Delete(filePath);
			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");
			if((switches & eSwitches.PauseWhenFileDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
			stream.Dispose();

			#region ACreqing
#if(HasAcreq) //If you get an error below: Scroll to the top of the page and uncomment #undef HasAcreq
			if(dumpableExtensions.BinarySearch(fileExt) >= 0 && !(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
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
			}
#endif
			#endregion

			Console.WriteLine(); Console.WriteLine();
		}

		private static void DisplayHashBuffer(BlockConsumerContainer.Progress progress) {
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0; int timeElapsed;
			int lastLineIndex = 0, maxNameLength = 0;
			long fileSize = progress.StreamSize;
			string output;

			Average[] mean = new Average[progress.BlockConsumerCount];
			for(int i = 0;i < mean.Length;i++) mean[i] = new Average();
			for(int i = 0;i < progress.BlockConsumerCount;i++) if(maxNameLength < progress.Name(i).Length) maxNameLength = progress.Name(i).Length + 1;
			if(maxNameLength < "Progress".Length) maxNameLength = "Progress".Length;

			Console.CursorVisible = false;
			output = "*: Buffersize available for hashalgorithm blocksize: " + blockSize + "kb blockCount: " + blockCount + "\n";

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.WindowWidth - maxNameLength - 4) + "]\n";
			}
			output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.WindowWidth - maxNameLength - 4) + "]\n";

			Console.Write(output);
			lastLineIndex = Console.CursorTop;

			int barLength = Console.WindowWidth - maxNameLength - 4;
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
				  "".PadLeft(Console.WindowWidth - Console.CursorLeft)
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

		private static void GlobalExceptionHandler(object sender, UnhandledExceptionEventArgs e) { WriteException((Exception)e.ExceptionObject); }

		private static void WriteException(Exception e) {
			ExceptionXElement ex = new ExceptionXElement(e, false);
			ex.Save(Path.Combine(Environment.CurrentDirectory, "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml"));
			Console.Write(ex.ToString());
		}

		private static void Pause() {
			ConsoleKeyInfo cki;
			do {
				cki = Console.ReadKey();
			} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
		}

		#region Empty args help
		static string help =
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
   port    udp Port used by ac
   done	   save processed-file-paths to file and exclude existing from proc
   tout    timeout: '-tout:<seconds>:<number Of retries>' (not implemented)
   bsize   buffer for hashing: '-bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>'
   host    host name of anidb udp api server, default is '" + host + @"' (not implemented)
  output:   (exclusive)
   y       XML creq format
   M       MediaInfo Dump
   X       MediaInfo XML Dump
  control:
   c       do _not_ recurse Into subfolders
   m       Monitor folder(s)
   p       pause when done (hold cmd window)
   t       print Time used for each file
   T       print total elapsed time
   q       pause after each file
   r       random file Order (not implemented)
   z       delete files after parsing
   o       wait for Response when sending dumps
   w       supress progress (silent)
  hash:
   0       CRC32 (cyclic Redundancy check)
   1       ED2K  (edonkey2000 hash)
   2       MD5   (message-digest algorithm 5)
   3       SHA1  (secure hash algorithm 1)
   5       TTH   (tiger tree Hash) (slow!)
   6       AICH  (advanced Intelligent corruption Handler) (not implemented)
   a       all (available) hash algorithms
   u       print elapsed time after hashing
   e       print ed2k link
   d       print AniDB link
   D       open AniDB link in default browser.

press any key to exit";
		#endregion
	}
}

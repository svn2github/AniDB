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

/* ###Preprocessor Directives###
 * CreateProviderLogs	    (Needs Debug)
 * CSEBMLSpeedTest		    (Needs Debug)
 * UseNullStream		    (Needs Debug)
 * UseFileExtensionCheck    
 * SetArgumentsIfNull	    (Needs Debug)
 * TestAVCHeader		    (Needs Debug)
 * UseAICHHash			    (Needs Debug)
 * DumpArguments			(Needs Debug)
 * HasAcreq				    
 * DebugRelease             (Sets Debug)
 * Debug				    
 * Release				    
*/

#define Debug
//#define DebugRelease
//---------------
#if(DebugRelease)
#define Debug
#define UseFileExtensionCheck
//###############
#elif(Debug)
#define SetArgumentsIfNull
#define UseFileExtensionCheck
//###############
#elif(Release)
#define UseFileExtensionCheck
//###############
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
		private static int blockCount = 8;
		private static int blockSize = 2 * 1024;
		private static int monitorSleepDuration = 60000;
		private static List<string> dumpableExtensions;
		private static List<string> processExtensions;

		private static Version appVersion = Assembly.GetExecutingAssembly().GetName().Version;
		private static bool isProcessing;
		#endregion

		static CL() {
			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new List<String>(new string[] { "avi", "mpg", "mpeg", "ts", "rm", "rmvb", "asf", "wmv", "mov", "ogm", "mp4", "mkv", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "avi", "mpg", "mpeg", "ts", "rm", "rmvb", "asf", "wmv", "mov", "ogm", "mp4", "mkv", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z" });
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

			MainDebugStuff(ref args);

			if(!ParseClOptions(args)) return;

			DateTime startTime = DateTime.Now;
			ProcessMedia(new List<string>(mediaLst));

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
		private static void MainDebugStuff(ref string[] args) {
#if(Debug)
			//File.WriteAllBytes(@"E:\Anime\bla.txt", new byte[9500 * 1024 * 0]);

#if(CheckForMultEd2kBlockFiles)
			var files = Directory.GetFiles(@"F:\Anime", "*", SearchOption.AllDirectories);
			foreach(var file in files) {
				if((new FileInfo(file)).Length % (9500*1024) == 0){
					Console.WriteLine(file);
				}
			}
			Console.Read();
#endif

#if(SetArgumentsIfNull)
			if(args.Length == 0) SetArguments(ref args);
#endif

#if(DumpArguments)
					//System.IO.File.AppendAllText("Args.txt", args.Aggregate("args[]:", (acc, str) => { return acc + " " + str; }) + "\n");
					//System.IO.File.AppendAllText("Args.txt", Environment.CommandLine + "\n");
#endif
#if(CSEBMLSpeedTest)
			//(new CSEBMLSpeedTest()).Start(@"G:\Anime\!Movies\[zx] Koukaku Kidoutai 2.0 - Full Movie.mkv", 4 * 1024 * 1024, 16);
			(new BufferTest()).Start(@"G:\Anime\!Movies\[zx] Koukaku Kidoutai 2.0 - Full Movie.mkv", 4 * 1024 * 1024, 16);
			return;
#endif
#endif
		}
		private static void SetArguments(ref string[] args) {
			args = new string[] {
				//@"G:\Anime\!Movies\Metropolis.mkv",
				@"G:\Anime\!Movies\[zx] Koukaku Kidoutai 2.0 - Full Movie.mkv",
				//@"D:\My Stuff\Downloads\Hitou Meguri The Animation (Hentai) (Episode 001) [9BDBC2AC] [Shippon].mkv",
				//@"D:\My Stuff\Downloads\Usavich__Episode_022___8CBC6674___RANDOMFAGGOTS_.mkv",
				//@"D:\Anime\Stalled\!Watching\Lost Universe",
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
				"-p0123",
				//"-log:log.xml"
			};
		}

		private static bool ParseClOptions(string[] args) {
			if(args.Length == 0) { Console.WriteLine(help); Pause(); return false; }

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
						processExtensions = new List<string>(parts[1].ToLower().Split(','));
						processExtensions.Sort();
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
							doneListContent.AddRange(sr.ReadToEnd().ToLower().Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries));
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
				if(!isProcessing) ProcessMedia(new List<string>(mediaLst));
			}
		}

		private static void ProcessMedia(List<string> mediaLst) {
			isProcessing = true;

			var searchOption = switches != eSwitches.ExcludeSubFolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;
			var files = GetFiles(mediaLst, searchOption, processExtensions);

			FileEnvironment e;
			var container = CreateContainer(blockCount, blockSize);
			for(int i = 0;i < files.Count;i++) {
				Console.WriteLine("Processing (" + (i + 1) + " of " + files.Count + "):");
				e = new FileEnvironment(appVersion, container, files[i]);
				try {
					ProcessMediaFile(e);
				} catch(Exception ex) {
					e.AddException("Unhandled error while processing the file.", ex);
				}
				container.Reset();
				if(e.Exceptions.Count != 0) e.Exceptions.Save(Path.Combine(AppPath, "Error"));
			}
			isProcessing = false;
		}
		private static void ProcessMediaFile(FileEnvironment e) {
			Console.WriteLine("Folder: " + e.File.DirectoryName);
			Console.WriteLine("Filename: " + e.File.Name);

#if(UseFileExtensionCheck)
			if(processExtensions.BinarySearch(e.File.Extension.Substring(1)) < 0) { Console.WriteLine("Skipped\n"); return; }
#endif

			var startTime = DateTime.Now;
			Dictionary<string, IBlockConsumer> blockConsumers;
			HashFile(e, out blockConsumers);
			if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalSeconds + "s");

			foreach(var blockConsumer in blockConsumers.Values) {
				if(blockConsumer.Error != null) {
					e.AddException("BlockConsumer (" + blockConsumer.Name + ") threw an error.", blockConsumer.Error);
				}
			}

			var infoProvider = CreateInfoProvider(e.File.FullName, blockConsumers);

			WriteLogs(e, blockConsumers, infoProvider);
			HandleSwitches(e, blockConsumers, startTime);

#if(HasAcreq)
			DoACreq(e, infoProvider);
#endif

			Console.WriteLine(); Console.WriteLine();
		}
		private static void HashFile(FileEnvironment e, out Dictionary<string, IBlockConsumer> blockConsumers) {
#if(Debug && UseNullStream)
			Stream fileStream = new NullStream(9500 * 1024);
#else
			Stream fileStream = System.IO.File.OpenRead(e.File.FullName);
#endif

			using(fileStream) {
				Boolean isMatroska = MatroskaParser.IsMatroskaFile(fileStream);
				fileStream.Position = 0;

				if((switches & eSwitches.UseAllHashes) != 0 || isMatroska) {

#if(Debug && UseAICHHash)
					if((switches & (eSwitches.Aich)) != 0) hashContainer.AddHashAlgorithm("AICH");
#endif
					if((switches & (eSwitches.Crc)) != 0) e.Container.AddBlockConsumer("CRC");
					if((switches & (eSwitches.Tiger)) != 0) e.Container.AddBlockConsumer("TIGER");
					if((switches & (eSwitches.Ed2k)) != 0) e.Container.AddBlockConsumer("ED2K");
					//if((switches & (eSwitches.Ed2k)) != 0) e.Container.AddBlockConsumer("MD4");
					if((switches & (eSwitches.Sha1)) != 0) e.Container.AddBlockConsumer("SHA1");
					if((switches & (eSwitches.Tth)) != 0) e.Container.AddBlockConsumer("TTH");
					if((switches & (eSwitches.Md5)) != 0) e.Container.AddBlockConsumer("MD5");
					if(isMatroska) e.Container.AddBlockConsumer("MKVParser");

					BlockConsumerContainer.Progress progress = e.Container.Start(fileStream);

					if((switches & eSwitches.SupressProgress) == 0) {
						try {
							DisplayHashBuffer(progress);
						} catch(Exception ex) {
							Console.WriteLine();
							e.AddException("Error in DisplayHashBuffer", ex);
						}
					}
				}
			}
			blockConsumers = e.Container.Join().ToDictionary(b => b.Name);
		}
		private static void WriteLogs(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers, InfoProviderBase p) {
			#region Generate/Write Logs
			string log = "";
			if((switches & eSwitches.CreqXmlFormat) != 0) {
				StringWriter sw = new StringWriter();
				Info.CreateAVDumpLog(p).Save(sw);
				log += sw.ToString();
			}
			if((switches & eSwitches.MediaInfoXMLOutPut) != 0) {
				StringWriter sw = new StringWriter();
				sw = new StringWriter();
				Info.CreateMediaInfoXMLLog(e.File.FullName, blockConsumers.Values).Save(sw);
				log += sw.ToString();
			}
			if((switches & eSwitches.MediaInfoOutPut) != 0) {
				log += Info.CreateMediaInfoDump(e.File.FullName);
			}

			if(logStream != null && !String.IsNullOrEmpty(log)) {
				byte[] infoBytes = System.Text.Encoding.UTF8.GetBytes(log + Environment.NewLine + Environment.NewLine);
				logStream.Write(infoBytes, 0, infoBytes.Length);
			}
			#endregion

			Console.WriteLine(log);
			Console.WriteLine();

			#region DoneLog Stream Writing
			if(doneListStream != null) {
				byte[] donePathBytes = System.Text.Encoding.UTF8.GetBytes(e.File.FullName + Environment.NewLine);
				doneListStream.Write(donePathBytes, 0, donePathBytes.Length);
				doneListContent.Add(e.File.FullName);
			}
			#endregion

			#region Ed2kLog Stream Writing
			Ed2k ed2k = null;
			if(blockConsumers.ContainsKey("ED2K")) ed2k = (Ed2k)((HashCalculator)blockConsumers["ED2K"]).HashObj;
			if(ed2k != null) {
				byte[] blueEd2kHash = ed2k.RedHash; //Handle Ed2k screwup
				byte[] redEd2kHash = ed2k.Hash;

				if(ed2kListStream != null) {
					string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
					if(!ed2k.BlueIsRed) {
						ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
					}

					byte[] ed2kBytes = System.Text.Encoding.UTF8.GetBytes(ed2kStr + Environment.NewLine);
					ed2kListStream.Write(ed2kBytes, 0, ed2kBytes.Length);
				}
			}
			#endregion

			#region HashLog Stream Writing
			if(hashListStream != null) {
				string formattedStr = hashLogStyle;
				foreach(HashCalculator hashExecute in blockConsumers.Values.Where(blockConsumer => { return blockConsumer is HashCalculator; })) {
					if(hashExecute.HashObj is Ed2k) {
						string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
						if(!((Ed2k)hashExecute.HashObj).BlueIsRed) {
							ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
						}

						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					} else {
						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse));
					}
				}
				byte[] formattedStrBytes = System.Text.Encoding.UTF8.GetBytes(formattedStr + Environment.NewLine);
				hashListStream.Write(formattedStrBytes, 0, formattedStrBytes.Length);
			}
			#endregion
		}
		private static void HandleSwitches(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers, DateTime startTime) {
			Ed2k ed2k = null;
			if(blockConsumers.ContainsKey("ED2K")) ed2k = (Ed2k)((HashCalculator)blockConsumers["ED2K"]).HashObj;

			if((switches & eSwitches.OpenInBrowser) != 0 || (switches & eSwitches.PrintAniDBLink) != 0) {
				string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + e.File.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse);
				if((switches & eSwitches.OpenInBrowser) != 0) System.Diagnostics.Process.Start(aniDBLink);
				if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(aniDBLink);
			}
			if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/");

			//if((switches & eSwitches.DeleteFileWhenDone) != 0 && !error) System.IO.File.Delete(filePath);
			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");
			if((switches & eSwitches.PauseWhenFileDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}

#if(HasAcreq)
		private static void DoACreq(FileEnvironment e, InfoProvider infoProvider) {
			if(dumpableExtensions.BinarySearch(e.File.Extension.Substring(1)) >= 0 && !(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				string creq = Info.CreateAVDumpLog(infoProvider).OuterXml;

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
					e.AddException("ACreqing exception" ,ex);
				}
			}
		}
#endif

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
				output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.BufferWidth - maxNameLength - 4) + "]\n";
			}
			output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(Console.BufferWidth - maxNameLength - 4) + "]\n";

			Console.Write(output);
			lastLineIndex = Console.CursorTop;

			int barLength = Console.BufferWidth - maxNameLength - 4;
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
				charCount = fileSize != 0 ? (int)((double)bytesProcessed / (double)fileSize * barLength) : barLength;
				Console.Write("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));

				timeElapsed = (int)progress.TimeElapsed.TotalMilliseconds;
				Console.Write("\n" +
				  "Position: " + ((double)bytesProcessed / (1 << 20)).ToString("0MB") + "/" + ((double)fileSize / (1 << 20)).ToString("0MB") + "  " +
				  "Elapsed time: " + (timeElapsed / 1000d).ToString("0.0").PadLeft(3) + "s  " +
				  "Speed: " + (((double)bytesProcessed / (1 << 20)) / (timeElapsed / 1000d)).ToString("0.00MB/s") +
				  "".PadLeft(Console.BufferWidth - Console.CursorLeft)
				);

				Thread.Sleep(80);
			} while(doLoop);

			for(int i = 0;i < progress.BlockConsumerCount;i++) {
				Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 2);
				Console.Write(progress.BlockConsumerObj(i).ToString());
			}
			Console.SetCursorPosition(0, lastLineIndex);
			Console.WriteLine();
			Console.CursorVisible = true;
		}

		private static BlockConsumerContainer CreateContainer(int blockCount, int blockSize) {
			BlockConsumerContainer container = new BlockConsumerContainer(blockCount, blockSize * 1024);
			container.RegisterBlockConsumer("TIGER", new HashCalculator("TIGER", new TigerThex(), h => BaseConverter.ToString(h.Hash, BaseOption.Base32 | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("AICH", new HashCalculator("AICH", new Aich(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("SHA1", new HashCalculator("SHA1", new SHA1CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("CRC", new HashCalculator("CRC", new Crc32(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("MD4", new HashCalculator("MD4", new Md4(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("MD5", new HashCalculator("MD5", new MD5CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("TTH", new HashCalculator("TTH", new TreeHash(new TigerThex(), new TigerThex(), 1024), h => BaseConverter.ToString(h.Hash, BaseOption.Base32 | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("MKVParser", new MatroskaParser("MKVParser"));

			container.RegisterBlockConsumer("ED2K", new HashCalculator("ED2K", new Ed2k(), h =>
				BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + (((Ed2k)h).BlueIsRed ? "" : " | " + BaseConverter.ToString(((Ed2k)h).RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse))
			));
			return container;
		}
		private static InfoProviderBase CreateInfoProvider(string filePath, Dictionary<string, IBlockConsumer> blockConsumers) {
			MatroskaProvider mkvProvider = null;
			if(blockConsumers.ContainsKey("MKVParser")) mkvProvider = new MatroskaProvider(((MatroskaParser)blockConsumers["MKVParser"]).MatroskaFileObj);
			var milProvider = new MediaInfoProvider(filePath);
			var hashProvider = new HashInfoProvider(blockConsumers.Values.Where(b => !b.Name.Equals("MKVParser")).Cast<HashCalculator>());

			var providers = new Collection<InfoProviderBase>();
			if(mkvProvider != null) providers.Add(mkvProvider);
			providers.Add(milProvider);
			providers.Add(hashProvider);
			var p = new CompositeInfoProvider(providers);

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
		private static List<string> GetFiles(List<string> mediaLst, SearchOption searchOption, List<string> validExtensions) {
			List<string> files = new List<string>();
			foreach(var media in mediaLst) {
				if(System.IO.File.Exists(media)) {
					files.Add(media);
				} else if(System.IO.Directory.Exists(media)) {
					files.AddRange(FileGetter.GetFiles(media, processExtensions));
				} else {
					//TODO Error?
				}

			}
			return files.Where(str => doneListContent.BinarySearch(str.ToLower()) < 0).ToList();
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

		public static string AppPath { get { return System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location); } }
		private static void GlobalExceptionHandler(object sender, UnhandledExceptionEventArgs e) {
			ExceptionXElement ex = new ExceptionXElement((Exception)e.ExceptionObject, false);
			string path = Path.Combine(AppPath, "Error");
			if(!Directory.Exists(path)) Directory.CreateDirectory(path);
			string fileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";
			ex.Save(Path.Combine(path, fileName));
		}
		private static void Pause() {
			ConsoleKeyInfo cki;
			do {
				cki = Console.ReadKey();
			} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
		}
	}
}

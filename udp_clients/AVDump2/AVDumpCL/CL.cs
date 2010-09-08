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

#define Debug
//#define DebugRelease

#if(DebugRelease)
#define Debug
#define HasACreq
#define UseFileExtensionCheck

#elif(Debug)
#define HasACreq
#define SetArgumentsIfNull
#define UseFileExtensionCheck
#define UseNullStream

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
	public class CL {
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

		static CL() {
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

#if(SetArgumentsIfNull)
			if(!ParseClOptions(SetArguments())) return;
#else
			if(!ParseClOptions(ArgsParser.Parse(Environment.CommandLine).Skip(1).ToArray())) return;
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
				@"D:\Anime\",
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
				"-p01256",
				//"-log:log.xml",
				//"-acerr:acerr.txt",
				//"-ac:arokh:Anime",
				//"-ext:avi,mpg,mpeg,mp4,rm,asf,flv",
				"-ext:avi",
				//"-done:done.txt"
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

					} else if(parts[0] == "host" && parts.Length == 3) {
						hostAddress = parts[1];
						hostPort = int.Parse(parts[2]);

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

			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");

			Console.WriteLine(); Console.WriteLine();
		}
		private static void ExecuteBlockConsumers(FileEnvironment e, out Dictionary<string, IBlockConsumer> blockConsumers) {
			Stream fileStream;
#if(Debug && UseNullStream)
			//fileStream = new RandomStream(1024 * 1024 * 1024);
			fileStream = new NullStream(1024 * 1024 * 1024);
#else
			try {
				fileStream = File.Open(e.File.FullName, FileMode.Open, FileAccess.Read, FileShare.None);
			} catch(Exception ex) {
				Console.WriteLine(ex.Message);
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
			string log = e.File.FullName + Environment.NewLine;
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
				byte[] blueEd2kHash = ed2k.RedHash; //Handle Ed2k screwup
				byte[] redEd2kHash = ed2k.Hash;

				if(ed2kListPath != null) {
					string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
					if(!ed2k.BlueIsRed) {
						ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
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
						string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
						if(!((Ed2k)hashExecute.HashObj).BlueIsRed) {
							ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/";
						}

						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					} else {
						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse));
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
				string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + e.File.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse);
				if((switches & eSwitches.OpenInBrowser) != 0) System.Diagnostics.Process.Start(aniDBLink);
				if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(aniDBLink);
			}
			if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + "|/");

			//if((switches & eSwitches.DeleteFileWhenDone) != 0 && !error) System.IO.File.Delete(filePath);
			if((switches & eSwitches.PauseWhenFileDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}

#if(HasACreq)
		private static ACreqResult DoACreq(FileEnvironment e, InfoProviderBase infoProvider) {
			MemoryStream stream = new MemoryStream();
			Info.CreateAVDumpLog(infoProvider).Save(new SafeXmlWriter(stream, new UTF8Encoding()));
			byte[] creqBytes = stream.ToArray();

			try {
				int tries = 0;
				ACreqResult result;
				do {
					Console.Write("Sending Creq... ");
					result = ACreq.Commit(new ACreqArgs("avdumplib", 1, hostAddress, hostPort, localPort, username, password, creqBytes, (eSwitches.WaitForDumpReply & switches) != 0 ? timeout * 1000 : -1));
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
			container.RegisterBlockConsumer("TIGER", new HashCalculator("TIGER", new TigerForTTH(), h => BaseConverter.ToString(h.Hash, BaseOption.Base32 | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("AICH", new HashCalculator("AICH", new Aich(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("SHA1", new HashCalculator("SHA1", new SHA1CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("CRC", new HashCalculator("CRC", new Crc32(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("MD4", new HashCalculator("MD4", new Md4(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("MD5", new HashCalculator("MD5", new MD5CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("TTH", new HashCalculator("TTH", new TTH(Environment.ProcessorCount), h => BaseConverter.ToString(h.Hash, BaseOption.Base32 | BaseOption.Pad | BaseOption.Reverse)));
			container.RegisterBlockConsumer("Matroska", new MatroskaParser("Matroska"));
			container.RegisterBlockConsumer("Ogm/Ogg", new OgmOggParser("Ogm/Ogg"));

			container.RegisterBlockConsumer("ED2K", new HashCalculator("ED2K", new Ed2k(),
				h => BaseConverter.ToString(h.Hash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse) + (((Ed2k)h).BlueIsRed ? "" : " | " + BaseConverter.ToString(((Ed2k)h).RedHash, BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse))
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
			if(extProvider != null) providers.Add(extProvider);
			if(mkvProvider != null) providers.Add(mkvProvider);
			if(ogmOggProvider != null) providers.Add(ogmOggProvider);
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
					/*if(validExtensions.BinarySearch(Path.GetExtension(media).Substring(1)) >= 0)*/
					files.Add(new FileInfo(media));
				} else if(System.IO.Directory.Exists(media)) {
					files.AddRange(FileGetter.GetFiles(media, validExtensions));
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
   r       random file Order (not implemented)
   z       delete files after parsing
   o       wait for Response when sending dumps
   w       supress progress (silent)
  hash:
   0       for crc because the crc of a 0 byte file is 00000000
   1       for sha1 because of the 1
   2       for ed2k because of the 2
   3       A Tiger in the house minimizes the Three Great Risks -- thieves, fire, and evil spirits
   4       for md4
   5       for md5
   6       for tth .. because thats whats left :p
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
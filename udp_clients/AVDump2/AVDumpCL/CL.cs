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
/**/
#define DebugRelease

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
using System.Net;
using System.Xml.Linq;
using System.Globalization;

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
		private static string extDiffListPath;


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

			dumpableExtensions = new List<String>(new string[] { "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "ogg", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "lrc", "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "ogg", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
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
			/*Random rnd = new Random();
			byte[] b = new byte[8];
			KeyValuePair<double, int>[] data = new KeyValuePair<double, int>[(1 << 10) * 10];
			for(int i = 0;i < data.Length;i++) {
				rnd.NextBytes(b);
				//data[i] = new KeyValuePair<double, int>(i * b[i % 8], BitConverter.ToUInt16(b, 0));
				data[i] = new KeyValuePair<double, int>(Math.Pow(10, (i >> 10) * 2) , i % 2 + 1);
			}
			var bla = Info.KMeansPP(10, data);*/

			Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;

			AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(GlobalExceptionHandler);
			//Console.OutputEncoding = new System.Text.UTF8Encoding(false);

#if(SetArguments)
			//if(!ParseClOptions(ArgsParser.Parse("-pya -hlog:\"$CRC32$ $ED2K$\":hlog.txt").ToArray())) return;
			if(!ParseClOptions(SetArguments())) return;
#else
			args = ArgsParser.Parse(Environment.CommandLine).Skip(1).ToArray();

			/*if(args.Length == 0) {
				Console.WriteLine("Error while parsing commandline arguments");
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
				return;
			}*/
			try {
				if(!ParseClOptions(ArgsParser.Parse(Environment.CommandLine).Skip(1).ToArray())) return;
			} catch(Exception) {
				Console.WriteLine("Error while interpreting commandline arguments");
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
				return;
			}

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
			//return File.ReadAllLines("diff.txt").Select(str => str.Split('\t')[1]).OrderBy(str => str).Concat(new string[]{ "-extdiff:diff2.txt", "-qy" }).ToArray();


			return new string[] {
				//@"\\ROADRUNNER\Nods\Node A\Music",
				//@"\\ROADRUNNER\Nods\Node A\Music\vivaldi, mozart, beethoven, chopin, ravel - ravel - bolero de ravel.mp3",
				//@"F:\Anime\[Done]Mahoutsukai ni Taisetsu na Koto\[SNS] Mahoutsukai ni Taisetsu Na Koto 09 - Yume, the Girl and a Seed of Summer.mkv",
				//@"F:\Anime\!Archive\Unchecked\Kanon (2006)\Kanon (2006) C1 - Opening [Coalgirls][Depr][Blu-ray].mkv",
				//@"C:\Users\Arokh\Projects\Visual Studio 2010\Projects\AVDump2\AVDumpCL\bin\Release\9996780567.flv",
				//@"D:\My Stuff\Downloads\Ducks.Take.Off.720p.QHD.CRF24.x264-CtrlHD.mkv",
				//@"D:\My Stuff\Downloads\Kurutta Kyoutou - Danzai no Gakuen (Hentai) (Episode 001) [A892042D] .mp4",
				//@"F:\Anime\Bishoujo Senshi Sailor Moon R\Bishoujo Senshi Sailor Moon R 26 - Unmerciful Rubeus! The Four Sisters of Sadness [avatar][DVD][640x496].avi",
				//@"\\ROADRUNNER\Nods\Node A\Media\Stargate.Universe.S02E10.720p.HDTV.x264-CTU.mkv",
				//@"F:\Anime\[Done]InuYasha\Movies\[AHQ] Inuyasha I - Toki wo Koeru Omoi - Part 2 of 2.mkv",
				//@"F:\Anime\Love Hina Again",
				//@"D:\My Stuff\Downloads\subtitles2\errors",
				//@"D:\My Stuff\Downloads\sub",
				//@"C:\Users\Arokh\Projects\Visual Studio 2010\Projects\AVDump2\AVDumpCL\bin\Release\[实习医生格蕾.第六季].Grey's.Anatomy.S06E20.Hook.Line.And.Sinner.HDTV.XviD-2HD.chn.srt",
				//@"D:\My Stuff\µT\Anime\Tide Line Blue\wikiupdates.com-TdLnBl-11.mkv",
				//@"E:\Anime\Seen\Unkown\[Hitsuji].The.Melancholy.of.Haruhi.Suzumiya.Play.All.DVD.Order.mkv",
				//@"D:\My Stuff\µT\mIRC\[HorribleSubs]_Mitsudomoe_2_-_07_[720p].mkv",
				@"F:\Anime\!Archive\Unchecked\Hidamari Sketch X Hoshimittsu\Hidamari Sketch X Hoshimittsu 7 - May 3rd - 4th A Day in Seven Parts [Nutbladder][HDTV].mkv",
				//@"E:\Anime\Now\1989\Ys\Ys 1 - Prologue [KRT][DVD][640x480].avi",
				//@"D:\My Stuff\µT\Anime\Tenchi Universe\zx.tenchi-universe.05.divx5.ogm",
				//@"D:\My Stuff\Downloads\New folder (2)\Mahou no Rouge Lipstick (Hentai) (Episode 001) [01F18A1F].rm",
				//@"D:\My Stuff\Downloads\New folder (2)\186e4a23.rmvb",
				//@"D:\My Stuff\Downloads\New folder (2)",
				//@"D:\My Stuff\Downloads\New folder (2)\(rar--m1a mpa1 mp1 m2a mpa2 mp2 mp3) Black.Eyed.Peas.-.Monkey.Business.-.2005.-.Mp3-VBRkbps.rar",
				//@"D:\My Stuff\Downloads\New folder (2)\New folder (3)\Taiko Drums (Japanese Festival Music).mp3",
				//@"F:\Anime\[Done]Cowbow Bebop\Cowboy Bebop - Tengoku no Tobira Complete Movie [THORA][Blu-ray].mkv",
				//@"D:\My Stuff\Downloads\New folder (4)\Gakuen no Shuryousha (Hentai) (Episode 001) [1A82A0D8] [myd].ogm",
				//@"E:\Anime\Stalled\1995\Tenchi Muyou!\Tenchi Muyou! 01 - No Need For Discussions! [zx][DVD][640x480].ogm",
				//@"D:\My Stuff\Downloads\New folder (4)\Afro Samurai (Episodes 001-005) [287F0519] [THORA].mkv",
				//@"F:\Anime\[Done]Juubee Ninpuuchou\[AQAN] Juubee Ninpuuchou - Ryuuhougyoku Hen 01 - Mujan, the Hidden Village.ogm",
				//@"E:\Anime\Processed\One Piece\One Piece 460 - An Enormous Fleet Appears - The Whitebeard Pirates Invade! [Yibis][HDTV].mkv",
				//@"E:\Anime\Processed\One Piece\One Piece\One Piece Episode of Chopper Plus - Fuyu ni Saku, Kiseki no Sakura\One Piece Episode of Chopper Plus - Fuyu ni Saku, Kiseki no Sakura 1 - Complete Movie [N - F][DTV].mp4",
				//@"E:\Anime\Processed\One Piece\One Piece Norowareta Seiken\One Piece Norowareta Seiken 1v2 - Complete Movie [K-F][DVD].mp4",
				//@"D:\My Stuff\Downloads\New folder (2)\",
				//@"D:\My Stuff\Downloads\New folder (2)\",
				"-yoq",
				//"-hlog:\"$CRC$ $ED2K$\":hlog.txt",
				//"-bsize:10:10",
				//"-log:ogmlog2.xml",
				//"-log:subs.xml",
				//"-acerr:acerr.txt",
				"-ac:arokh:Anime",
				//"-host:ommina.homeip.net:9002",
				//"-ac:arokh:containers",
				//"-ext:srt",
				//"-ext:avi,lrc",
				//"-extdiff:diff.txt",
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

					if(parts.Length == 0) continue;

					for(int j = 0;j < parts.Length;j++) parts[j] = parts[j].Replace("|Colon|", ":");

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
							if(!Directory.Exists(hashListPath)) Directory.CreateDirectory(Path.GetDirectoryName(hashListPath));
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "exp") {
						try {
							ed2kListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(ed2kListPath)) Directory.CreateDirectory(Path.GetDirectoryName(ed2kListPath));
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "ext") {
						var exts = parts[1].ToLower().Split(',');
						invalidCl = exts.Any(str => str.StartsWith("-")) && !exts.All(str => str.StartsWith("-"));

						if(!invalidCl) {
							if(exts[0][0] == '-') {
								foreach(var ext in exts) processExtensions.Remove(ext.Substring(1));
							} else {
								processExtensions = new List<string>(exts);
								processExtensions.Sort();
							}
						}


					} else if(parts[0] == "log") {
						try {
							logPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(logPath)) Directory.CreateDirectory(Path.GetDirectoryName(logPath));
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "lport") {
						if(!int.TryParse(parts[1], out localPort)) invalidCl = true;

					} else if(parts[0] == "done") {
						try {
							doneListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(doneListPath)) Directory.CreateDirectory(Path.GetDirectoryName(doneListPath));
							if(File.Exists(doneListPath)) {
								doneListContent.AddRange(File.ReadAllText(doneListPath).ToLower().Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries));
								doneListContent.Sort();
							}
						} catch(Exception) { invalidCl = true; }
					} else if(parts[0] == "acerr") {
						try {
							acErrListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(acErrListPath)) Directory.CreateDirectory(Path.GetDirectoryName(acErrListPath));
						} catch(Exception) { invalidCl = true; }
					} else if(parts[0] == "extdiff") {
						try {
							extDiffListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(extDiffListPath)) Directory.CreateDirectory(Path.GetDirectoryName(extDiffListPath));
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

			if((switches & (eSwitches.PrintEd2kLink | eSwitches.PrintAniDBLink | eSwitches.OpenInBrowser)) != 0 || ed2kListPath != null) {
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
			mediaLst = (from media in mediaLst where Directory.Exists(media) select media).ToList();
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
				if(files[i].Length == 0) continue;

				e = new FileEnvironment(appVersion, container, files[i], startedOn, files.Count, i, totalBytes, processedBytes);
				try {
					ProcessMediaFile(e);
				} catch(Exception ex) {
					e.AddException("Unhandled error while processing the file.", ex);
				}
				container.Reset();
				processedBytes += e.File.Length;

				if(e.Exceptions.Count != 0) {
					var exElem = e.Exceptions.ToXElement(true);

					string exPath = Path.Combine(AppPath, "Error");
					string exFileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";
					if(!Directory.Exists(exPath)) Directory.CreateDirectory(exPath);

					using(var writer = new SafeXmlWriter(Path.Combine(exPath, exFileName), Encoding.Unicode)) exElem.Save(writer);

					if(username != null && password != null) {
						try {
							exElem = e.Exceptions.ToXElement(false);
							MemoryStream memStream = new MemoryStream();
							using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) exElem.Save(writer);
							ACreq.Commit(new ACreqArgs(7, "avdumplib", appVersion.Build, hostAddress, hostPort, localPort, username, password, memStream.ToArray(), -1));
						} catch(Exception) { }
					}
				}
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
				fileStream = File.Open(e.File.FullName, FileMode.Open, FileAccess.Read, FileShare.Read);
			} catch(Exception ex) {
				Console.WriteLine(ex.Message + "\n");
				blockConsumers = null;
				return;
			}
#endif

			using(fileStream) {
				Boolean isMatroska = MatroskaParser.IsMatroskaFile(fileStream);
				Boolean isOgmOgg = OgmOggParser.IsOgmOggFile(fileStream);

				BlockConsumerContainer.Progress progress = null;
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

					progress = e.Container.Start(fileStream);
				}

				if((switches & eSwitches.SupressProgress) == 0) {
					try {
						DisplayBuffer(e, progress);
					} catch(Exception ex) {
						Console.WriteLine();
						e.AddException("Error in DisplayBuffer", ex);
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
				Info.CreateAVDumpLog(p).Save(new SafeXmlWriter(tw, lowerCaseElements: true));
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

			if(logPath != null && !String.IsNullOrEmpty(log)) AppendAllText(logPath, log + Environment.NewLine + Environment.NewLine, "Couldn't update logfile");

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

			var detExt = p[StreamType.General, 0, EntryKey.Extension] != null ? p[StreamType.General, 0, EntryKey.Extension].Value : null;
			if(!string.IsNullOrEmpty(extDiffListPath) && !string.IsNullOrEmpty(detExt) && !detExt.ToLower().Equals(e.File.Extension.Substring(1).ToLower())) {

				AppendAllText(extDiffListPath, e.File.Extension.Substring(1) + " => " + detExt + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension diff file");
			}

			#region DoneLog Stream Writing
			if(doneListPath != null && (wasACreqSent || username == null || password == null) && e.Exceptions.Count == 0) {
				AppendAllText(doneListPath, e.File.FullName + Environment.NewLine, "Couldn't update donelist file");
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
					AppendAllText(ed2kListPath, ed2kStr + Environment.NewLine, "Couldn't update ed2k list file");
				}
			}
			#endregion

			#region HashLog Stream Writing
			if(hashListPath != null) {
				string formattedStr = hashLogStyle;
				foreach(HashCalculator hashExecute in blockConsumers.Values.Where(blockConsumer => { return blockConsumer is HashCalculator; })) {
					//if(hashExecute.HashObj is Ed2k) {
					//    string ed2kStr = "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash) + "|/";
					//    if(!((Ed2k)hashExecute.HashObj).BlueIsRed) {
					//        ed2kStr += "*" + "ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).BlueHash) + "|/";
					//    }

					//    formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					//} else {
					formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash));
					//}
				}
				AppendAllText(hashListPath, formattedStr + Environment.NewLine, "Couldn't update hashlist file");
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


			//string xmlStr = 
			byte[] creqBytes;
			using(var writer = new SafeXmlWriter(stream, new UTF8Encoding(), Formatting.None, lowerCaseElements: true)) {
				Info.CreateAVDumpLog(infoProvider).Save(writer);

				creqBytes = new byte[stream.Length - 38];
				stream.Position = 38;
				stream.Read(creqBytes, 0, (int)stream.Length - 38);
			}

			try {
				int tries = 0;
				ACreqResult result;
				do {

					Console.Write("Sending Creq " + ((eSwitches.WaitForDumpReply & switches) != 0 ? "..." : "(Unchecked)"));
					result = ACreq.Commit(new ACreqArgs(1, "avdumplib", appVersion.Build, hostAddress, hostPort, localPort, username, password, creqBytes, (eSwitches.WaitForDumpReply & switches) != 0 ? timeout * 1000 : -1));
					tries++;
					switch(result) {
						case ACreqResult.ACreqSent: Console.WriteLine("Done."); break;
						case ACreqResult.AsyncCall: break;
						default:
							Console.ForegroundColor = ConsoleColor.Red;
							Console.WriteLine("Failed. Reason: " + System.Enum.GetName(typeof(ACreqResult), result));
							Console.ResetColor();

							if(!string.IsNullOrEmpty(acErrListPath)) AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.ToString() + Environment.NewLine, "Couldn't update acerr file");
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

		private static void DisplayBuffer(FileEnvironment e, BlockConsumerContainer.Progress progress) {
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0;
			int lastLineIndex = 0, maxNameLength = 0;
			long fileSize = e.File.Length;
			string output;

			Average[] mean = null;
			if(progress != null) {
				mean = new Average[progress.BlockConsumerCount];
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
			}

			lastLineIndex = Console.CursorTop;

			bool doLoop;
			int barLength = Console.BufferWidth - maxNameLength - 4;
			do {
				doLoop = progress != null && !progress.HasFinished;

				if(progress != null) {
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
				} else {
					bytesProcessed = fileSize;
				}

				bytesProcessed += e.ProcessedBytes;
				var totalTimeElapsed = DateTime.Now - e.StartedOn;
				var eta = e.TotalBytes * (totalTimeElapsed.TotalSeconds / bytesProcessed) - totalTimeElapsed.TotalSeconds + 0.5;
				if(!double.IsInfinity(eta)) {
					output =
					  "Files: " + (e.ProcessedFiles + 1) + "/" + e.TotalFiles + " " +
					  "Bytes: " + (bytesProcessed >> 20) + "MB/" + (e.TotalBytes >> 20) + "MB " +
					  "Elapsed: " + totalTimeElapsed.ToFormatedString() + " " +
					  "ETA: " + TimeSpan.FromSeconds(eta).ToFormatedString();
					output += "".PadLeft(output.Length < Console.BufferWidth ? Console.BufferWidth - output.Length - 1 : 0, ' ');
					Console.Write(output);
				}

				if(doLoop) Thread.Sleep(80);
			} while(doLoop);


			if(progress != null) {
				for(int i = 0;i < progress.BlockConsumerCount;i++) {
					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 3);
					Console.Write(progress.BlockConsumerObj(i).ToString());
				}
				Console.SetCursorPosition(0, lastLineIndex);

			}
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
				extProvider = new FileExtensionProvider(e.File.FullName);
			} catch(Exception ex) { e.AddException("Failed to create FileExtensionProvider", ex); }

			try {
				milProvider = new MediaInfoProvider(e.File.FullName);
			} catch(Exception ex) { e.AddException("Failed to create MediaInfoProvider", ex); }

			try {
				hashProvider = new HashInfoProvider(blockConsumers.Values.Where(b => b is HashCalculator).Cast<HashCalculator>());
			} catch(Exception ex) { e.AddException("Failed to create HashInfoProvider", ex); }


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
@"Help:    http://wiki.anidb.info/w/Avdump
Version: " + appVersion + @"
Usage:   AVDump2CL [-options] <media file/folder> [<media file/folder> ...]

Options: (one letter switches can be put in one string)
		 (all multiple letter switches require colon)

  Autocreqing:
   ac      Autocreqing (example: -ac:<username>:<api_key>) " + '\x3' + "12YOU SHOULD USE THIS" + '\xF' + @"
   host    Hostname of AniDB UDP API server (default: " + hostAddress + ":" + hostPort + @")
   lport   Local UDP port used for autocreqing
   tout    Timeout for sending dumps (example: -tout:<seconds>:<retries>)

  Input:
   bsize   Buffer size for hashing (example: 
			   -bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>)
   ext     Comma separated extension list (example: -ext:avi,mkv or -ext:-zip)
   ms      Monitor sleep duration in milliseconds (default: " + monitorSleepDuration.ToString() + @")

  Logging:
   acerr   Write filepaths of failed dumps to file (example: -acerr:acerr.txt)
   extdiff Write missmatched extensions to file (example: -extdiff:extdiff.txt)
   exp     Export eD2k-links to file (example: -exp:ed2k.txt)
   done    Save processed filepaths to file and exclude existing from
			   processing (example: -done:done.txt)
   hlog    Export hashes to file (example: -hlog:" + "\"$CRC$ $ED2K$\"" + @":hlog.txt)
   log     Write output to file (example: -log:log.txt)

  Output:
   M       MediaInfo dump
   X       MediaInfo XML dump
   y       XML creq format
   Y       AVD2 creq dump

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

  Hashing:
   0       For CRC (because the CRC of a 0 byte file is 00000000)
   1       For SHA1 (because of the 1)
   2       For ED2K (because of the 2)
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
			string path = Path.Combine(AppPath, "Error");
			string fileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";
			if(!Directory.Exists(path)) Directory.CreateDirectory(path);

			ExceptionXElement ex;
			try {
				ex = new ExceptionXElement((Exception)e.ExceptionObject, false);
			} catch(Exception) {
				ex = new ExceptionXElement(new Exception("Couldn't save Exception"), false);
			}

			using(var writer = new SafeXmlWriter(Path.Combine(path, fileName), Encoding.ASCII)) {
				ex.Save(XmlWriter.Create(writer, new XmlWriterSettings { OmitXmlDeclaration = true }));
			}

			if(username != null && password != null) {
				try {
					MemoryStream memStream = new MemoryStream();
					using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) ex.Save(writer);
					ACreq.Commit(new ACreqArgs(7, "avdumplib", appVersion.Build, hostAddress, hostPort, localPort, username, password, memStream.ToArray(), -1));
				} catch(Exception) { }
			}
		}

		private static void Pause() {
			ConsoleKeyInfo cki;
			do {
				cki = Console.ReadKey();
			} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
		}

		private static void AppendAllText(string path, string contents, string error) {
			int i = 3;
			bool written = false;
			while(!written && i != 0) {
				try {
					File.AppendAllText(path, contents);
					written = true;
				} catch(Exception) {
					Thread.Sleep(100);
				}
				i--;
			}

			if(!written) {
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine(error);
				Console.ResetColor();
			}
		}
	}
}
﻿#define HasACreq

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Xml;
using AVDump2CL.Misc;
using AVDump2Lib.Acreqing;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.HashAlgorithms;
using AVDump2Lib.InfoGathering;
using AVDump2Lib.InfoGathering.InfoProvider;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;
using AVDump2Lib.Misc;

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
		private static string crcMismatchListPath;


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
				Console.WriteLine("Warning BitConverter is BigEndian. Continuing will cause incorrect results and/or errors");
				Pause();
			}

			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new List<String>(new string[] { "m4a", "rv", "mk3d", "mkv3d", "dtshd", "thd", "xss", "js", "rt", "tts", "smil", "tmp", "pjs", "webm", "lrc", "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "ogg", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "m4a", "rv", "mk3d", "mkv3d", "dtshd", "thd", "xss", "js", "rt", "tts", "smil", "tmp", "pjs", "webm", "lrc", "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "ogg", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			processExtensions.Sort();

			char2SwitchEnum = new Dictionary<char, eSwitches>();
			char2SwitchEnum['y'] = eSwitches.CreqXmlFormat; //Done
			char2SwitchEnum['Y'] = eSwitches.NewCreqXmlFormat; //Done
			char2SwitchEnum['M'] = eSwitches.MediaInfoOutPut; //Done
			char2SwitchEnum['X'] = eSwitches.MediaInfoXMLOutPut; //Done
			char2SwitchEnum['x'] = eSwitches.TxtFormat; //Done

			char2SwitchEnum['c'] = eSwitches.ExcludeSubFolders; //Done
			char2SwitchEnum['m'] = eSwitches.MonitorFolder; //Done
			char2SwitchEnum['n'] = eSwitches.DumpAllExtensions; //Done
			char2SwitchEnum['p'] = eSwitches.PauseWhenDone; //Done
			char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone; //Done
			char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
			char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile; //Done
			char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone; //Done
			//char2SwitchEnum['o'] = eSwitches.WaitForDumpReply; //Done
			char2SwitchEnum['w'] = eSwitches.SupressProgress; //Done
			char2SwitchEnum['T'] = eSwitches.PrintTotalTimeUsed; //Done
			char2SwitchEnum['H'] = eSwitches.HashingSpeedTest; //Done

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

			var consoleIn = Console.In;
			Console.SetOut(new ConsoleOutFilter(Console.Out));

			var waitHandle = new AutoResetEvent(true);
			try {
				waitHandle.WaitOne(10);
			} catch(Exception ex) {
				Console.WriteLine(".NET 3.5 SP1 probably not installed");
				Pause();
				return;
			}
			waitHandle.Close();

			var milFileName = Path.Combine(AppPath, Environment.OSVersion.Platform == PlatformID.Unix ? (IntPtr.Size == 4 ? "libMediaInfo_x86.so" : "libMediaInfo_x64.so") : (IntPtr.Size == 4 ? "MediaInfo_x86.dll" : "MediaInfo_x64.dll"));
			if(!File.Exists(milFileName)) {
				Console.WriteLine("MediaInfoLib is missing");
				Pause();
				return;
			}



			try {
				IMediaInfo mi = IntPtr.Size == 8 ? (IMediaInfo)new MediaInfo_x64() : (IMediaInfo)new MediaInfo_x86();
				if(!mi.Option("Info_Version").Equals("MediaInfoLib - v0.7.42")) {
					Console.WriteLine("Mediainfo library version mismatch. Needed: v0.7.42, Used: " + mi.Option("Info_Version"));
					Pause();
					return;
				}
				mi.Close();
			} catch(Exception ex) {
				Console.WriteLine("Media Info Library not found. Error: " + ex.ToString());
				Pause();
				return;
			}

			Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
			AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(GlobalExceptionHandler);



#if(!PublicRelease)
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

			if((switches & eSwitches.HashingSpeedTest) != 0) {
				var container = CreateContainer(blockCount, blockSize);
				Dictionary<string, IBlockConsumer> blockConumsers;
				var e = new FileEnvironment(null, container, null, DateTime.Now, 1, 0, 4 * 1024 * 1024 * 1024L, 0);


				ExecuteBlockConsumers(e, out blockConumsers, new NullStream(e.TotalBytes));
				Pause();
				return;
			}


			ProcessMedia(new List<string>(mediaLst));
			if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();

			if((switches & eSwitches.PauseWhenDone) != 0) {
				Console.WriteLine("Press any alpha-numeric key to continue");
				Pause();
			}
		}

#if(!PublicRelease)
		private static string[] SetArguments() {
			//return File.ReadAllLines("diff.txt").Select(str => str.Split('\t')[1]).OrderBy(str => str).Concat(new string[]{ "-extdiff:diff2.txt", "-qy" }).ToArray();

			/*byte[] b = new byte[9292059];
			var f = File.OpenRead(@"E:\Anime\Processed\Hetalia Axis Powers\Hetalia Axis Powers C1 - Ending [gg][DVD].mkv");
			f.Position = Convert.ToInt64("afee1a", 16) - 5;
			f.Read(b, 0, b.Length);
			File.WriteAllBytes(@"E:\Anime\Processed\Hetalia Axis Powers\Hetalia Axis Powers C1 - Ending [gg][DVD]2.mkv", b);*/

			//switches |= eSwitches.UseACreqHashes;
			return new string[] {
				//@"\\ROADRUNNER\Nods\Node A\Music",
				//@"\\ROADRUNNER\Nods\Node A\Music\vivaldi, mozart, beethoven, chopin, ravel - ravel - bolero de ravel.mp3",
				//@"F:\Anime\[Done]Mahoutsukai ni Taisetsu na Koto\[SNS] Mahoutsukai ni Taisetsu Na Koto 09 - Yume, the Girl and a Seed of Summer.mkv",
				//@"F:\Anime\!Archive\Unchecked\Kanon (2006)\Kanon (2006) C1 - Opening [Coalgirls][Depr][Blu-ray].mkv",
				//@"C:\Users\Arokh\Projects\Visual Studio 2010\Projects\AVDump2\AVDumpCL\bin\Release\9996780567.flv",
				//@"D:\My Stuff\Downloads\Ducks.Take.Off.720p.QHD.CRF24.x264-CtrlHD.mkv",
				//@"D:\My Stuff\Downloads\Kurutta Kyoutou - Danzai no Gakuen (Hentai) (Episode 001) [A892042D] .mp4",
				//@"E:\Anime\Processed\Kaze no Tani no Nausicaa\Kaze no Tani no Nausicaa 1 - Complete Movie [THORA][Depr][Blu-ray][1920x1038].mkv",
				//@"F:\Anime\Bishoujo Senshi Sailor Moon R\Bishoujo Senshi Sailor Moon R 26 - Unmerciful Rubeus! The Four Sisters of Sadness [avatar][DVD][640x496].avi",
				//@"\\ROADRUNNER\Nods\Node A\Media\Stargate.Universe.S02E10.720p.HDTV.x264-CTU.mkv",
				//@"F:\Anime\[Done]InuYasha\Movies\[AHQ] Inuyasha I - Toki wo Koeru Omoi - Part 2 of 2.mkv",
				//@"F:\Anime\Love Hina Again",
				//@"D:\My Stuff\Downloads\Nurse_Witch_Komugi-chan_Magikarte_-_C5_-_3_Different_Textless_Endings.mkv",
				//@"C:\Users\Arokh\Projects\Visual Studio 2010\Projects\AVDump2\AVDump2CL",
				//@"D:\My Stuff\Downloads\Byousoku_5_Centimeter_-_T1_-_Trailer_-_[Triad](b2df5b8e)(dub.sub_x-in.en).mp4",
				//@"D:\My Stuff\Downloads\problemfiles",
				@"C:\Users\Arokh\Downloads\Slam Dunk! Ep003.rv",
				//@"F:\Anime",
				//@"E:\Anime\Now\1992\Kaze no Tairiku",
				//@"D:\My Stuff\Downloads\[SumiSora&CASO][MariaHolic][Information][DVDRIP][x264_AAC][5B4FD648].mp4",
				//@"D:\My Stuff\Downloads\[AniYoshi]_Crystal_Blaze_-_01_[C6D45AE8].mkv",
				//@"D:\My Stuff\Downloads\[アニメ DVD] 生徒会の一存 第01話 「駄弁る生徒会」 (1280x720 x264 AAC).mp4",

				//@"D:\My Stuff\Downloads\redcliff450.webm",
				//@"D:\My Stuff\Downloads\badapple_lulz.ogv",
				//@"D:\My Stuff\Downloads\AVDump2CL\[peerates.net] free movie from archive.org - La Liberation de Paris - 1944.ogv",
				//@"D:\My Stuff\Downloads\AVDump2CL\careca.ogv",
				//@"D:\My Stuff\Downloads\subtitles2\errors",
				//@"D:\My Stuff\Downloads\sub",
				//@"C:\Users\Arokh\Projects\Visual Studio 2010\Projects\AVDump2\AVDumpCL\bin\Release\[实习医生格蕾.第六季].Grey's.Anatomy.S06E20.Hook.Line.And.Sinner.HDTV.XviD-2HD.chn.srt",
				//@"D:\My Stuff\µT\Anime\Tide Line Blue\wikiupdates.com-TdLnBl-11.mkv",
				//@"E:\Anime\Seen\Unkown\[Hitsuji].The.Melancholy.of.Haruhi.Suzumiya.Play.All.DVD.Order.mkv",
				//@"D:\My Stuff\µT\mIRC\[HorribleSubs]_Mitsudomoe_2_-_07_[720p].mkv",
				//@"F:\Anime\!Archive\Unchecked\Hidamari Sketch X Hoshimittsu\Hidamari Sketch X Hoshimittsu 7 - May 3rd - 4th A Day in Seven Parts [Nutbladder][HDTV].mkv",
				//@"E:\Anime\Processed\Hetalia Axis Powers\Hetalia Axis Powers C1 - Ending [gg][DVD].mkv",
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
				//@"D:\My Stuff\µT\Anime\Requiem from the Darkness[DH]\Requiem From the Darkness 13 - The Death Spirits of Seven Man Point, Part 2[F4319032].mkv",
				"-y0pc",
				//"-H01256",
				//"-py01245",
				//"-exp:explog.txt",
				//"-ms:10000",
				//"-noerr",
				//"-crcerr:crcerr.txt",
				//"-hlog:\"$CRC$ $ED2K$\":hlog.txt",
				//"-bsize:10:10",
				//"-log:ogmlog2.xml",
				//"-log:log.xml",
				//"-acerr:acerr.txt",
				//"-ac:arokh:Anime",
				//"-host:ommina.homeip.net:9002",
				//"-ac:arokh:containers",
				//"-ext:srt",
				//"-ext:rt",
				"-extdiff:extdiff.txt",
				//"-done:done.txt"
			};
		}
#endif


		private static bool ParseClOptions(string[] args) {
			if(args.Length == 0) { HelpPrinter(); Pause(); return false; }

			bool firstDeleteCharSet = false;
			bool invalidCl = false;
			bool uselessCombination = true;

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

				if(parts.Length > 1 || parts[0].Equals("noerr")) { //TODO: refactor
					if(parts[0] == "ac" && parts.Length == 3) {
						username = parts[1];
						password = parts[2];
						switches |= eSwitches.UseACreqHashes;
						uselessCombination = false;

					} else if(parts[0] == "ms") {
						if(!int.TryParse(parts[1], out monitorSleepDuration)) invalidCl = true;

					} else if(parts[0] == "hlog") {
						try {
							hashLogStyle = parts[1];
							hashListPath = Path.Combine(AppPath, parts[2]);
							if(!Directory.Exists(hashListPath)) Directory.CreateDirectory(Path.GetDirectoryName(hashListPath));
							uselessCombination = false;
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "noerr") {
						switches |= eSwitches.NoErrorReporting;

					} else if(parts[0] == "exp") {
						try {
							ed2kListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(ed2kListPath)) Directory.CreateDirectory(Path.GetDirectoryName(ed2kListPath));
							uselessCombination = false;
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "crcerr") {
						try {
							switches |= eSwitches.Crc;
							crcMismatchListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(crcMismatchListPath)) Directory.CreateDirectory(Path.GetDirectoryName(crcMismatchListPath));
							uselessCombination = false;
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
							uselessCombination = false;
						} catch(Exception) { invalidCl = true; }

					} else if(parts[0] == "lport") {
						if(!int.TryParse(parts[1], out localPort)) invalidCl = true;

					} else if(parts[0] == "done") {
						try {
							doneListPath = Path.Combine(AppPath, parts[1]);
							if(!Directory.Exists(doneListPath)) Directory.CreateDirectory(Path.GetDirectoryName(doneListPath));
							if(File.Exists(doneListPath)) {
								doneListContent.AddRange(File.ReadAllText(doneListPath)/*.ToLower()*/.Split(new char[] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries));
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
							uselessCombination = false;
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

			if((switches & (eSwitches.UseAllHashes | eSwitches.HashingSpeedTest)) != 0) {
				uselessCombination = false;
			}

			if((switches & eSwitches.DeleteFileWhenDone) == 0 && firstDeleteCharSet) {
				Console.WriteLine("Set the z switch twice to confirm file deleting (no Recycle Bin!). Aborting!\nPress any alpha-numeric key to exit");
				Pause();
				return false;
			}

			if(logPath != null && (switches & (eSwitches.CreqXmlFormat | eSwitches.MediaInfoOutPut | eSwitches.MediaInfoXMLOutPut | eSwitches.NewCreqXmlFormat | eSwitches.TxtFormat)) == 0) {
				switches |= eSwitches.CreqXmlFormat;
			}

			if((switches & (eSwitches.CreqXmlFormat | eSwitches.MediaInfoOutPut | eSwitches.MediaInfoXMLOutPut | eSwitches.NewCreqXmlFormat | eSwitches.TxtFormat)) != 0) {
				uselessCombination = false;
			}

			if(uselessCombination) {
				Console.WriteLine("Combination of arguments is invalid (Nothing to do)");
				Pause();
				return false;
			}

			switches |= eSwitches.WaitForDumpReply;
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

#if(PublicRelease)
			var files = GetFiles(mediaLst, searchOption, processExtensions);
#else
			var files = GetFiles(mediaLst, searchOption, processExtensions);
#endif

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

#if(HasACreq)
					if(username != null && password != null && (switches & eSwitches.NoErrorReporting) == 0) {
						try {
							exElem = e.Exceptions.ToXElement(false);
							MemoryStream memStream = new MemoryStream();
							using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) exElem.Save(writer);

							AniDBCommit.CommitError(new AniDBCommit.CommitArgs("avdumplib", appVersion.Build, username, password, memStream.ToArray(), hostAddress + ":" + hostPort, localPort, timeout));
						} catch(Exception) { }
					}
#endif
				}

				if((switches & eSwitches.PauseWhenFileDone) != 0) {
					Console.WriteLine("Press any alpha-numeric key to continue");
					Pause();
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
			if(blockConsumers.Values.Any(bc => bc.Error != null)) return;

			WriteLogs(e, blockConsumers);

			Ed2k ed2k = null;
			if(blockConsumers.ContainsKey("ED2K")) ed2k = (Ed2k)((HashCalculator)blockConsumers["ED2K"]).HashObj;

			if((switches & eSwitches.OpenInBrowser) != 0 || (switches & eSwitches.PrintAniDBLink) != 0) {
				string aniDBLink = "http://anidb.info/perl-bin/animedb.pl?show=file&size=" + e.File.Length + "&hash=" + BaseConverter.ToString(ed2k.Hash);
				if((switches & eSwitches.OpenInBrowser) != 0) System.Diagnostics.Process.Start(aniDBLink);
				if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(aniDBLink);
			}
			if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("ed2k://|file|" + e.File.Name + "|" + e.File.Length + "|" + BaseConverter.ToString(ed2k.Hash) + "|/");
			if((switches & eSwitches.DeleteFileWhenDone) != 0 && e.Exceptions.Count == 0) System.IO.File.Delete(e.File.FullName);
			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalSeconds.ToString() + "s");

			Console.WriteLine(); Console.WriteLine();
		}
		private static void ExecuteBlockConsumers(FileEnvironment e, out Dictionary<string, IBlockConsumer> blockConsumers, Stream altStream = null) {
			Stream fileStream;
			try {
				fileStream = altStream != null ? altStream : File.Open(e.File.FullName, FileMode.Open, FileAccess.Read, FileShare.Read);
			} catch(Exception ex) {
				Console.WriteLine(ex.Message + "\n");
				blockConsumers = null;
				return;
			}

			using(fileStream) {
				Boolean isMatroska = MatroskaParser.IsMatroskaFile(fileStream);
				Boolean isOgmOgg = OgmOggParser.IsOgmOggFile(fileStream);

				BlockConsumerContainer.Progress progress = null;
				if((switches & eSwitches.UseAllHashes) != 0 || isMatroska || isOgmOgg) {

#if(UseAICHHash)
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
						Console.CursorVisible = true;
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
		private static void WriteLogs(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers/*, InfoProviderBase p*/) {
			bool acreqFile;
			DebugWrite("WriteLogs: Before CreateInfoProvider");
			var p = CreateInfoProvider(e, blockConsumers, out acreqFile);
			DebugWrite("WriteLogs: After CreateInfoProvider");

			#region Generate/Write Logs
			string log = "";
			try {
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
				if((switches & eSwitches.TxtFormat) != 0) log += Info.CreateTxtLog(e.File.FullName, p);

				if(logPath != null && !String.IsNullOrEmpty(log)) AppendAllText(logPath, log + Environment.NewLine + Environment.NewLine, "Couldn't update logfile");
			} catch(Exception ex) {
				e.AddException("Error while generating info report", ex);
			}

			#endregion

			if(!string.IsNullOrEmpty(log)) Console.WriteLine(log);
			Console.WriteLine();

#if(OgmOggDebug)
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
			if(acreqFile && (dumpableExtensions.BinarySearch(e.File.Extension.Substring(1).ToLower()) >= 0 || (switches & eSwitches.DumpAllExtensions) != 0) && !(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				try {
					var result = DoACreq(e, p);
					wasACreqSent = result.State == AniDBCommit.ACreqState.CreqSent;
					foreach(var exception in result.Exceptions) e.AddException("Internal ACreqing exception", exception);
				} catch(Exception ex) {
					e.AddException("ACreqing exception", ex);
				}
			}
#endif

			var detExt = p[StreamType.General, 0, EntryKey.Extension] != null ? p[StreamType.General, 0, EntryKey.Extension].Value : null;
			if(!string.IsNullOrEmpty(extDiffListPath) && (detExt == null || !e.File.Extension.Substring(1).ToLower().Equals(detExt.ToLower()))) {
				AppendAllText(extDiffListPath, e.File.Extension.Substring(1) + " => " + (detExt == null ? "unknown" : detExt) + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension diff file");
			}

			#region DoneLog Stream Writing
			if(doneListPath != null && (wasACreqSent || username == null || password == null) && e.Exceptions.Count == 0) {
				AppendAllText(doneListPath, e.File.FullName + Environment.NewLine, "Couldn't update donelist file");
				int index = doneListContent.BinarySearch(e.File.FullName);
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

			#region CRC Mismatch
			if(crcMismatchListPath != null && blockConsumers.ContainsKey("CRC")) {
				string crcHash = BaseConverter.ToString(((HashCalculator)blockConsumers["CRC"]).HashObj.Hash);
				if(!e.File.Name.ToLower().Contains(crcHash.ToLower())) {
					AppendAllText(crcMismatchListPath, crcHash + " " + e.File.FullName + Environment.NewLine, "Couldn't update crcerr list file");
					Console.ForegroundColor = ConsoleColor.Yellow;
					Console.WriteLine("Filename doesn't contain the calculated CRC (" + crcHash + ")");
					Console.ResetColor();
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

#if(HasACreq)
		private static AniDBCommit.ACreqResult DoACreq(FileEnvironment e, InfoProviderBase infoProvider) {
			MemoryStream stream = new MemoryStream();

			byte[] creqBytes;
			using(var writer = new SafeXmlWriter(stream, new UTF8Encoding(), Formatting.None, lowerCaseElements: true)) {
				var xmlDoc = Info.CreateAVDumpLog(infoProvider);
				if(xmlDoc["file"]["md4"] != null) xmlDoc["file"].RemoveChild(xmlDoc["file"]["md4"]);
				if(xmlDoc["file"]["tiger"] != null) xmlDoc["file"].RemoveChild(xmlDoc["file"]["tiger"]);


				xmlDoc.Save(writer);

				creqBytes = new byte[stream.Length - 38];
				stream.Position = 38;
				stream.Read(creqBytes, 0, (int)stream.Length - 38);
			}

			int tries = 0;
			AniDBCommit.ACreqResult result;
			do {
				Console.Write("Sending Data... ");
				result = AniDBCommit.CommitACreq(new AniDBCommit.CommitArgs("avdumplib", appVersion.Build, username.ToLower(), password, creqBytes, hostAddress + ":" + hostPort, localPort, timeout * 1000));
				tries++;

				if(result.State == AniDBCommit.ACreqState.CreqSent) {
					Console.WriteLine("Done");

				} else if(result.State == AniDBCommit.ACreqState.AckUnknownResponse) {
					Console.ForegroundColor = ConsoleColor.Yellow;
					Console.WriteLine("Warning. Unknown Response.");
					Console.ResetColor();

				} else if(result.State == AniDBCommit.ACreqState.LogSizeTooSmall) {
					Console.ForegroundColor = ConsoleColor.Yellow;
					Console.WriteLine("Warning. Logsize is too small.");
					Console.ResetColor();

				} else if(result.State == AniDBCommit.ACreqState.WrongVersionOrPassOrName) {
					Console.ForegroundColor = ConsoleColor.Red;
					Console.WriteLine("Failed. Either the client is outdated or your username/password combination is wrong.");
					Pause();
					Console.ResetColor();

				} else {
					Console.ForegroundColor = ConsoleColor.Red;
					Console.WriteLine("Failed. Reason: " + System.Enum.GetName(typeof(AniDBCommit.ACreqState), result.State));
					Console.ResetColor();
				}

				if(!string.IsNullOrEmpty(acErrListPath) && (result.State & (AniDBCommit.ACreqState.CreqSent | AniDBCommit.ACreqState.TimeOut)) == 0) {
					AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.State.ToString() + Environment.NewLine, "Couldn't update acerr file");
				}

			} while((result.State == AniDBCommit.ACreqState.TimeOut) && tries <= retries);

			if(tries > retries) {
				if(!string.IsNullOrEmpty(acErrListPath)) AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.State.ToString() + Environment.NewLine, "Couldn't update acerr file");
			}

			return result;
		}
#endif

		private static void DisplayBuffer(FileEnvironment e, BlockConsumerContainer.Progress progress) {
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0;
			int lastLineIndex = 0, maxNameLength = 0;
			long fileSize = e.File == null ? e.TotalBytes : e.File.Length;
			int consoleWidth = Console.BufferWidth != 0 ? Console.BufferWidth : (Console.WindowWidth != 0 ? Console.WindowWidth : 80);

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
					output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(consoleWidth - maxNameLength - 4) + "]\n";
				}
				output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(consoleWidth - maxNameLength - 4) + "]\n\n";

				Console.Write(output);
			}

			lastLineIndex = Console.CursorTop;

			bool doLoop;
			int barLength = consoleWidth - maxNameLength - 4;
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
					output += "".PadLeft(output.Length < consoleWidth ? consoleWidth - output.Length - 1 : 0, ' ');
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
					output += "".PadLeft(output.Length < consoleWidth ? consoleWidth - output.Length - 1 : 0, ' ');
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
		private static InfoProviderBase CreateInfoProvider(FileEnvironment e, Dictionary<string, IBlockConsumer> blockConsumers, out bool noErrors) { //TODO: Refactor
			DebugWrite("CreateInfoProvider: Beginning");
			MatroskaProvider mkvProvider = null;
			OgmOggProvider ogmOggProvider = null;
			MediaInfoProvider milProvider = null;
			HashInfoProvider hashProvider = null;
			FileExtensionProvider extProvider = null;
			//CompositeInfoProvider p = null;

			noErrors = true;

			DebugWrite("CreateInfoProvider: After Variables");
			try {
				if(blockConsumers.ContainsKey("Matroska")) {
					var matroskaFile = ((MatroskaParser)blockConsumers["Matroska"]).MatroskaFileObj;
					if(matroskaFile != null) mkvProvider = new MatroskaProvider(matroskaFile);
					DebugWrite("CreateInfoProvider: After Matroska");
				}
			} catch(Exception ex) { e.AddException("Failed to create MatroskaProvider", ex); noErrors = false; }

			try {
				if(blockConsumers.ContainsKey("Ogm/Ogg")) {
					var ogmOggFile = ((OgmOggParser)blockConsumers["Ogm/Ogg"]).OgmOggFileObj;
					if(ogmOggFile != null) ogmOggProvider = new OgmOggProvider(ogmOggFile);
					DebugWrite("CreateInfoProvider: After Ogm");
				}
			} catch(Exception ex) { e.AddException("Failed to create OgmOggProvider", ex); noErrors = false; }

			try {
				extProvider = new FileExtensionProvider(e.File.FullName);
				DebugWrite("CreateInfoProvider: After FileExtensionProvider");
			} catch(Exception ex) { e.AddException("Failed to create FileExtensionProvider", ex); /*noErrors = false;*/ }

			try {
				milProvider = new MediaInfoProvider(e.File.FullName);
				DebugWrite("CreateInfoProvider: After MediaInfoProvider");
			} catch(Exception ex) { e.AddException("Failed to create MediaInfoProvider", ex); noErrors = false; }

			try {
				hashProvider = new HashInfoProvider(blockConsumers.Values.Where(b => b is HashCalculator).Cast<HashCalculator>());
				DebugWrite("CreateInfoProvider: After HashInfoProvider");
			} catch(Exception ex) { e.AddException("Failed to create HashInfoProvider", ex); noErrors = false; }


			var providers = new Collection<InfoProviderBase>();
			if(mkvProvider != null) providers.Add(mkvProvider);
			if(ogmOggProvider != null) providers.Add(ogmOggProvider);
			if(extProvider != null) providers.Add(extProvider);
			if(milProvider != null) providers.Add(milProvider);
			if(hashProvider != null) providers.Add(hashProvider);
			DebugWrite("CreateInfoProvider: After Add");

#if(CreateProviderLogs)
				string path = Path.Combine(AppPath, "ProviderLogs");
				if(!Directory.Exists(path)) Directory.CreateDirectory(path);
				path = Path.Combine(path, System.IO.Path.GetFileNameWithoutExtension(filePath) + "_*" + System.IO.Path.GetExtension(filePath) + ".xml");

				Info.CreateAVDumpLog(p).Save(path.Replace("*", "Composite"));
				Info.CreateAVDumpLog(milProvider).Save(path.Replace("*", "MIL"));
				Info.CreateAVDumpLog(filePath, blockConsumers).Save(path.Replace("*", "ACreq"));
				if(mkvProvider != null) Info.CreateAVDumpLog(mkvProvider).Save(path.Replace("*", "MKV"));
#endif
			return new CompositeInfoProvider(providers);
		}
		private static List<FileInfo> GetFiles(List<string> mediaLst, SearchOption searchOption, List<string> validExtensions) {
			List<FileInfo> files = new List<FileInfo>();
			foreach(var media in mediaLst) {
				if(System.IO.File.Exists(media)) {
					if(validExtensions == null || validExtensions.BinarySearch(Path.GetExtension(media).Substring(1).ToLower()) >= 0) files.Add(new FileInfo(media));
				} else if(System.IO.Directory.Exists(media)) {
					files.AddRange(FileGetter.GetFiles(media, validExtensions, (switches & eSwitches.ExcludeSubFolders) == 0));
				} else {
					//TODO Error?
				}
			}

			if((switches & eSwitches.RandomFileOrder) != 0) {
				Random rng = new Random(4);
				int n = files.Count;
				while(n > 1) {
					n--;
					int k = rng.Next(n + 1);
					var value = files[k];
					files[k] = files[n];
					files[n] = value;
				}
			}

			return files.Where(fi => doneListContent.BinarySearch(fi.FullName) < 0).ToList();
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


		static string help =
@"Help:    http://wiki.anidb.info/w/Avdump2
Version: " + appVersion + @"
Usage:   AVDump2CL [-options] <media file/folder> [<media file/folder> ...]

Options: (one letter switches can be put in one string)
		 (all multiple letter switches require colon)

  Autocreqing:
   ac      Autocreqing (example: -ac:<username>:<api_key>) " + '\x3' + "12YOU SHOULD USE THIS" + '\xF' + @"
             Enables error reporting! (No private info) To disable add -noerr
   host    Hostname of AniDB UDP API server (default: " + hostAddress + ":" + hostPort + @")
   lport   Local UDP port used for autocreqing
   tout    Timeout for sending dumps (ex: -tout:<seconds>:<retries>)

  Input:
   bsize   Circular buffer size for hashing (ex: 
             -bsize:<blocksize in kb (" + blockSize.ToString() + @")>:<number of blocks (" + blockCount.ToString() + @")>)
   ext     Comma separated extension list (ex: -ext:avi,mkv or -ext:-zip)
   ms      Monitor sleep duration in milliseconds (default: " + monitorSleepDuration.ToString() + @")

  Logging:
   acerr   Write file paths of failed dumps to file (ex: -acerr:acerr.txt)
   crcerr  Checks if filename contains calculated crc (ex: -crc:crcerr.txt)
   extdiff Write mismatched extensions to file (ex: -extdiff:extdiff.txt)
   exp     Export eD2k-links to file (ex: -exp:ed2k.txt)
   done    Skip paths present in log and append paths to newly processed files
             (ex: -done:done.txt)
   hlog    Export hashes to file (ex: -hlog:" + "\"$CRC$ $ED2K$\"" + @":hlog.txt)
   log     Write output to file (ex: -log:log.txt)

  Output:
   M       MediaInfo dump
   X       MediaInfo XML dump
   x       Simple text format
   y       XML creq format
   Y       AVD2 creq dump

  Control:
   c       Do _not_ recurse into subfolders
   m       Monitor folder(s)
   p       Pause when done (hold cmd window)
   q       Pause after each file
   r       Random file order
   t       Print time used for each file
   T       Print total elapsed time
   w       Suppress progress (silent)
   z       Delete files after parsing
   H       Test pure hashingspeed without io limitations

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

#if(HasACreq)
			if(username != null && password != null && (switches & eSwitches.NoErrorReporting) == 0) {
				try {
					MemoryStream memStream = new MemoryStream();
					using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) ex.Save(writer);
					AniDBCommit.CommitError(new AniDBCommit.CommitArgs("avdumplib", appVersion.Build, username, password, memStream.ToArray(), hostAddress + ":" + hostPort, localPort, timeout));
				} catch(Exception) { }
			}
#endif
		}

		private static void Pause() {
			try {
				ConsoleKeyInfo cki;
				do {
					cki = Console.ReadKey();
				} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
			} catch(Exception) {

				throw;
			}
		}

		private static void AppendAllText(string path, string contents, string error) {
			int i = 5;
			bool written = false;
			while(!written && i != 0) {
				try {
					File.AppendAllText(path, contents);
					written = true;
				} catch(Exception) {
					Thread.Sleep(200);
				}
				i--;
			}

			if(!written) {
				Console.ForegroundColor = ConsoleColor.Red;
				Console.WriteLine(error);
				Console.ResetColor();
			}
		}

		private static void DebugWrite(string line) {
			return;
			Console.ForegroundColor = ConsoleColor.DarkRed;
			Console.WriteLine(line);
			Console.ResetColor();
		}
	}

	public class ConsoleOutFilter : TextWriter {
		private TextWriter textWriter;

		public ConsoleOutFilter(TextWriter textWriter) { this.textWriter = textWriter; }

		public override void Write(char value) { if(value != '•') textWriter.Write(value); }
		public override void Write(string value) { textWriter.Write(value.Replace("•", "")); }
		public override void WriteLine(char[] buffer) { textWriter.WriteLine((new String(buffer)).Replace("•", "")); }
		public override void WriteLine(string value) { textWriter.WriteLine(value.Replace("•", "")); }
		public override void WriteLine() { textWriter.WriteLine(); }

		public override void Write(char[] buffer, int index, int count) { textWriter.Write(buffer, index, count); }
		public override void Write(char[] buffer) { textWriter.Write(buffer); }
		public override void Write(object value) { textWriter.Write(value); }
		public override void Write(string format, object arg0) { textWriter.Write(format, arg0); }
		public override void Write(string format, object arg0, object arg1) { textWriter.Write(format, arg0, arg1); }
		public override void Write(string format, object arg0, object arg1, object arg2) { textWriter.Write(format, arg0, arg1, arg2); }
		public override void Write(string format, params object[] arg) { textWriter.Write(format, arg); }
		public override void WriteLine(char value) { textWriter.WriteLine(value); }
		public override void WriteLine(char[] buffer, int index, int count) { textWriter.WriteLine(buffer, index, count); }
		public override void WriteLine(object value) { textWriter.WriteLine(value); }
		public override void WriteLine(string format, object arg0) { textWriter.WriteLine(format, arg0); }
		public override void WriteLine(string format, object arg0, object arg1) { textWriter.WriteLine(format, arg0, arg1); }
		public override void WriteLine(string format, object arg0, object arg1, object arg2) { textWriter.WriteLine(format, arg0, arg1, arg2); }
		public override void WriteLine(string format, params object[] arg) { textWriter.WriteLine(format, arg); }

		public override Encoding Encoding { get { throw new NotImplementedException(); } }
	}

}
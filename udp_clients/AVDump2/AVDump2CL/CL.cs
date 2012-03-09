#define HasACreq

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
using System.Net;
using System.Diagnostics;
using AVDump2Lib.CL;
using System.IO.Pipes;
using AVDump2Lib;

namespace AVDump2CL {
	public class CL {
		#region Fields
		private static int localPort = 0;
		private static string username;
		private static string password;
		private static string hostAddress = "api.anidb.info";
		private static int hostPort = 9002;
		private static IPEndPoint host;
		private static eSwitches switches;

		private static string logPath;
		private static string ed2kListPath;
		private static string doneListPath;
		private static string hashListPath;
		private static string acErrListPath;
		private static string extDiffListPath;
		private static string crcMismatchListPath;
		private static string extFixPath;
		private static string debugPath;


		private static string hashLogStyle;
		private static List<string> mediaLst;
		private static List<string> doneListContent;

		private static int? retriesOnIOException;
		private static int retries = 3;
		private static int timeout = 10;
		private static int blockCount = 4;
		private static int blockSize = 4 * 1024;
		private static int monitorSleepDuration = 60000;
		private static HashSet<string> dumpableExtensions;
		private static HashSet<string> processExtensions;

		private static Version appVersion = Assembly.GetExecutingAssembly().GetName().Version;
		private static bool isProcessing;
		private static bool hasConsole;

		private static int sentACReqs = 0;
		private static int failedACReqs = 0;
		#endregion

		static CL() {
			if(!BitConverter.IsLittleEndian) {
				ColoredWriteLine(ConsoleColor.Red, "Error BitConverter is BigEndian. Big endian support will be added on request", true);
				Pause();
				return;
			}

			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new HashSet<String>(new string[] {"s2k", "fsb", "sup", "zeg", "m4a", "m4v", "rv", "mk3d", "mkv3d", "dtshd", "thd", "xss", "js", "rt", "tts", "smil", "tmp", "pjs", "webm", "lrc", "avi", "mpg", "mpeg", "m2ts", "ts", "rm", "ra", "rmvb", "asf", "wmv", "wma", "mov", "ogm", "ogg", "mp4", "mkv", "mks", "swf", "flv", "ogv", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "mp3", "aac", "ac3", "dts", "wav", "flac", "mka", "rar", "zip", "ace", "7z", "qt" });
			processExtensions = new HashSet<string>(dumpableExtensions);
		}

		private static CLManagement CreateCLArgParser() {
			CLManagement clArgManagement = new CLManagement();

			//Action<string, string, object> handler = (nameSpace, argName, param) => Console.WriteLine(nameSpace + "." + argName + (param != null ? "=" + param : ""));
			//Func<string, object> transformer = ldParam => ldParam;

			Func<string, object> splitParams = (param) => param.Split(new char[] { ':' }, 2);

			Func<string, object> parseHost = (param) => {
				for(int i = 0;i < 5;i++) try { return new IPEndPoint(Dns.GetHostAddresses(param.Split(':')[0]).First(), int.Parse(param.Split(':')[1])); } catch(Exception) { }
				throw new Exception("Couldn't resolve host");
			};

			Func<string, object> toInt = (ldParam) => int.Parse(ldParam);

			Func<string, object> pathParam = (ldParam) => {
				var path = Path.IsPathRooted(ldParam)? ldParam : Path.Combine(CurrentDirectory, ldParam);
				if(!Directory.Exists(Path.GetDirectoryName(path))) Directory.CreateDirectory(Path.GetDirectoryName(path));
				//if(!File.Exists(path)) File.Create(path).Close();
				return path;
			};

			Func<string, int, int, int> between = (value, min, max) => Math.Max(Math.Min((int)toInt(value), max), min);

			#region ACReq
			clArgManagement.RegisterArgGroup(new ArgGroup("ACReq",
				"Updates meta info of an existing file entry on AniDB",
				(nameSpace, argName, argParam) => {
					switch(argName) {
						case "Authentication": username = ((string[])argParam)[0]; password = ((string[])argParam)[1]; switches |= eSwitches.UseACreqHashes; break;
						case "HostEndPoint": hostAddress = (string)((object[])argParam)[0]; hostPort = (int)((object[])argParam)[1]; break;
						case "LocalPort": localPort = (int)argParam; break;
						case "TimeOut": timeout = ((int[])argParam)[0]; retries = ((int[])argParam)[1]; break;
					}
				},
				new ArgStructure("Authentication", "Auth", null,
					splitParams,
					"--Auth=<username>:<api_key>",
					"Enables ACReqing when valid credentials are provided.\nAlso enables error Reporting! (Add -noerr to disable)\nVisit anidb.net/perl-bin/animedb.pl?show=profile (Account Tab) to set key"
				),
				new ArgStructure("HostEndPoint", "Host", null,
					(ldParams) => ((string[])splitParams(ldParams)).Select((ldParam, i) => i == 0 ? ldParam : toInt(ldParam)).ToArray(),
				//parseHost,
					"--Host=<hostname>:<hostport>",
					"Change endpoint of AniDB UDP API server"
				),
				new ArgStructure("LocalPort", "LPort", null,
					toInt,
					"--LPort=<localport>",
					"Local UDP port used for ACReqing"
				),
				new ArgStructure("TimeOut", "TOut", null,
					(ldParams) => ((string[])splitParams(ldParams)).Select(ldParam => (int)toInt(ldParam)).ToArray(),
					"--TOut=<seconds>:<retries>",
					"Sets the retry count and the timeout before resending the dump"
				)
			));
			#endregion

			#region Logging
			clArgManagement.RegisterArgGroup(new ArgGroup("Logging",
				"",
				(nameSpace, argName, argParam) => {
					switch(argName) {
						case "ACReqError": acErrListPath = (string)argParam; break;
						case "CRCError": crcMismatchListPath = (string)argParam; switches |= eSwitches.Crc; break;
						case "ExtensionDifference": extDiffListPath = (string)argParam; break;
						case "FixExtensions": extFixPath = (string)argParam; break;
						case "DoneLog": doneListPath = (string)argParam; if(File.Exists(doneListPath)) doneListContent = new List<string>(File.ReadAllLines(doneListPath).OrderBy(ldPath => ldPath)); break;
						case "HashLog": hashListPath = ((string[])argParam)[1]; hashLogStyle = ((string[])argParam)[0]; break;
						case "LogFile": logPath = (string)argParam; break;
					}
				},
				new ArgStructure("ACReqError", "ACErr", null, pathParam, "--ACErr=acerr.txt", "Log file paths of failed dumps to file"),
				new ArgStructure("CRCError", "CRCErr", null, pathParam, "--CRCErr=crcerr.txt", "Logs the filepath if the calculated CRC is not contained in the filename"),
				new ArgStructure("ExtensionDifference", "ExtDiff", null, pathParam, "--ExtDiff=extdiff.txt", "Logs the filepath if the detected Extension does not match the actual extension"),
				new ArgStructure("FixExtensions", "FixExt", null, pathParam, "--FixExt=extfix.txt", "Changes the fileextension to the dectected extension and logs the action"),
				new ArgStructure("DoneLog", "Done", null, pathParam, "--Done=done.txt", "Skip paths present in the log and appends paths to newly processed files"),
				new ArgStructure("HashLog", "HLog", null,
					(ldParams) => ((string[])splitParams(ldParams)).Select((ldParam, i) => i == 0 ? ldParam : (string)pathParam(ldParam)).ToArray(),
					"--HLog=<Format>:hlog.txt",
					"Appends lines for each processed file to the specified file in a custom format.\nFormat: $HashName$ is replaced with the calculated hash. Hash must be enabled.\nFormat may not contain colons."
				),
				new ArgStructure("LogFile", "Log", null, pathParam, "--Log=log.txt", "Any enabled reports (See Report Namespace) will be appended to this file")
			));
			#endregion

			#region Report
			clArgManagement.RegisterArgGroup(new ArgGroup("Report",
				"",
				(nameSpace, argName, argParam) => {
					switch(argName) {
						case "MediaInfoTxtDump": switches |= eSwitches.MediaInfoOutPut; break;
						case "MediaInfoXmlDump": switches |= eSwitches.MediaInfoXMLOutPut; break;
						case "SimpleTxtFormat": switches |= eSwitches.TxtFormat; break;
						case "CreqFormat": switches |= eSwitches.CreqXmlFormat; break;
						case "AVD2Format": switches |= eSwitches.NewCreqXmlFormat; break;
						case "HashDump": switches |= eSwitches.HashOutput; break;
					}
				},
				new ArgStructure("MediaInfoTxtDump", "", 'M', null, "", ""),
				new ArgStructure("MediaInfoXmlDump", "", 'm', null, "", ""),
				new ArgStructure("SimpleTxtFormat", "", 't', null, "", ""),
				new ArgStructure("CreqFormat", "", 'C', null, "", ""),
				new ArgStructure("AVD2Format", "", 'c', null, "", ""),
				new ArgStructure("HashDump", "", 'h', null, "", "")
			));
			#endregion

			#region Control
			clArgManagement.RegisterArgGroup(new ArgGroup("Control",
				"",
				(nameSpace, argName, argParam) => {
					switch(argName) {
						case "RetryOnIOException": retriesOnIOException = (int)argParam; break;
						case "BlockSize": blockSize = ((int[])argParam)[0]; blockCount = ((int[])argParam)[1]; break;
						case "ExtensionList": processExtensions = (HashSet<string>)argParam; break;
						case "MonitorFolders": monitorSleepDuration = (int)argParam * 1000; switches |= eSwitches.MonitorFolder; break;
						case "NoRecurse": switches |= eSwitches.ExcludeSubFolders; break;
						case "PauseWhenDone": switches |= eSwitches.PauseWhenDone; break;
						case "PauseAfterFile": switches |= eSwitches.PauseWhenFileDone; break;
						case "Randomize": switches |= eSwitches.RandomFileOrder; break;
						case "Quiet": switches |= eSwitches.SupressProgress; break;
						case "Delete": switches |= eSwitches.DeleteFileWhenDone; break;
						case "RenameFile": break;
						case "DoneLogFileNameOnly": switches |= eSwitches.DoneLogFilenameOnly; break;
						case "Benchmark": switches |= eSwitches.HashingSpeedTest; break;
						case "UseUtf8": switches |= eSwitches.UseUTF8OutputStream; break;
						case "UseCWD": switches |= eSwitches.UseCWD; break;
						case "Debug": debugPath = (string)argParam; break;
					}
				},
				new ArgStructure("BlockSize", "BSize", null,
					(ldParams) => ((string[])splitParams(ldParams)).Select((ldParam, i) => between(ldParam, i == 0 ? 512 : 1, i == 0 ? 64 * 1024 : 64)).ToArray(),
					"--BSize=<blocksize in kb>:<block count>",
					"Circular buffer size for hashing"
				),
				new ArgStructure("ExtensionList", "Ext", null,
					ldParam => {
						var exts = ldParam.ToLower().Split(',');
						if(exts.Length == 1 && exts[0].Equals("*")) return null;
						if(exts.Any(str => str.StartsWith("-")) && !exts.All(str => str.StartsWith("-"))) throw new Exception("Inclusive and exclusive extensions cannot be mixed");
						return new HashSet<string>(exts[0][0] == '-' ? processExtensions.Except(exts.Select(ldExt => ldExt.Substring(1))) : exts);
					},
					"--Ext=avi,mkv OR --Ext=-zip,-rar OR --Ext=*", ""
				),
				new ArgStructure("MonitorFolders", "Mon", null, toInt, "--Mon=3600", "Processes the given paths again after the specified idletime (seconds)"),
				new ArgStructure("NoErrorReporting", "NoErr", null, null, "", "Disables exception reporting\nBy default an exception report is sent to AniDB if there is one.\nIt does not contain any private information.\nThis greatly helps discovering bugs."),
				new ArgStructure("NoRecurse", "", 'R', null, "", "Do _not_ recurse into subfolders"),
				new ArgStructure("DoneLogFileNameOnly", "", 'R', null, "", "Use only the filename for Donelog comparsion"),
				new ArgStructure("PauseWhenDone", "", 'p', null, "", ""),
				new ArgStructure("RetryOnIOException", "", null, toInt, "--RetryOnIOException=3", "Retries processing files which have failed with an IOException"),
				new ArgStructure("PauseAfterFile", "", 'P', null, "", ""),
				new ArgStructure("Randomize", "", 'r', null, "", ""),
				new ArgStructure("Quiet", "", 'q', null, "", ""),
				new ArgStructure("Delete", "", null, null, "", ""),
				//new ArgStructure("RenameFile", "Rename", null, null, "--Rename=Format.cs", "After processing, the file is renamed to result of the userdefined function\nThe function is run a new AppDomain with minimal trust."),
				new ArgStructure("Benchmark", "", 'B', null, "", "Used in conjunction with the hash args\nAdd one file to the args to test read speed."),
				new ArgStructure("UseCWD", "", null, null, "", "Use the current working directoy for file output"),
				new ArgStructure("UseUtf8", "", 'U', null, "", ""),
				new ArgStructure("Debug", "", null, pathParam, "", "")
			));
			#endregion

			#region Hashing
			clArgManagement.RegisterArgGroup(new ArgGroup("Hashing",
				"",
				(nameSpace, argName, argParam) => {
					switch(argName) {
						case "CRC32": switches |= eSwitches.Crc; break;
						case "SHA1": switches |= eSwitches.Sha1; break;
						case "ED2K": switches |= eSwitches.Ed2k; break;
						case "TIGER": switches |= eSwitches.Tiger; break;
						case "MD4": switches |= eSwitches.Md4; break;
						case "MD5": switches |= eSwitches.Md5; break;
						case "TTH": switches |= eSwitches.Tth; break;
						case "AllHashes": switches |= eSwitches.UseAllHashes; break;
					}
				},
				new ArgStructure("CRC32", "", '0', null, "", ""),
				new ArgStructure("SHA1", "", '1', null, "", ""),
				new ArgStructure("ED2K", "", '2', null, "", ""),
				new ArgStructure("TIGER", "", '3', null, "", ""),
				new ArgStructure("MD4", "", '4', null, "", ""),
				new ArgStructure("MD5", "", '5', null, "", ""),
				new ArgStructure("TTH", "", '6', null, "", ""),
				new ArgStructure("AllHashes", "", 'a', null, "", "")
			));
			#endregion

			#region AniDB
			clArgManagement.RegisterArgGroup(new ArgGroup("AniDB",
				"",
				(nameSpace, argName, argParam) => {
					switches |= eSwitches.Ed2k;
					switch(argName) {
						case "ExportEd2kLinks": ed2kListPath = (string)argParam; break;
						case "PrintEd2kLink": switches |= eSwitches.PrintEd2kLink; break;
						case "PrintAniDBLink": switches |= eSwitches.PrintAniDBLink; break;
						case "OpenAniDBLink": switches |= eSwitches.OpenInBrowser; break;
					}
				},
				new ArgStructure("ExportEd2kLinks", "Exp", null, pathParam, "--Exp=ed2k.txt", ""),
				new ArgStructure("PrintEd2kLink", "", null, null, "", ""),
				new ArgStructure("PrintAniDBLink", "", null, null, "", ""),
				new ArgStructure("OpenAniDBLink", "", null, null, "", "")
			));
			#endregion

			clArgManagement.SetUnnamedParamHandler(param => mediaLst.Add(param));

			return clArgManagement;
		}
		private static bool ParseArgs(string[] args) {
			bool triedOldCmd = false;

		tryOldCmd:
			try {
				var clArgParser = CreateCLArgParser();
				if(args.Length == 0 || args.All(ldArg => ldArg.Trim().Equals(string.Empty))) {
					Console.WriteLine("Help:    http://wiki.anidb.info/w/Avdump2");
					Console.WriteLine("Version: " + appVersion);
					Console.WriteLine("Usage:   AVDump2CL.exe [--options] <file/folder> [<file/folder> ...]");
					Console.WriteLine();

					clArgParser.PrintHelp(false);
					Console.WriteLine("For a quickstart use \"AVDump2CL.exe --Auth=<UserName>:<APIKey> <Path>\"");
					Console.WriteLine("Visit anidb.net/perl-bin/animedb.pl?show=profile (Account Tab) to set key");
					Console.ReadKey();
					return false;
				}
				DebugWrite("Main: Arg Parser created");

				if(!clArgParser.ParseArgs(args)) return false;
				DebugWrite("Main: Args parsed");

				var uselessCombination = true;
				if((switches & (eSwitches.UseAllHashes | eSwitches.HashingSpeedTest)) != 0) uselessCombination = false;
				if(logPath != null && (switches & (eSwitches.CreqXmlFormat | eSwitches.MediaInfoOutPut | eSwitches.MediaInfoXMLOutPut | eSwitches.NewCreqXmlFormat | eSwitches.TxtFormat | eSwitches.HashOutput)) == 0) switches |= eSwitches.CreqXmlFormat;
				if((switches & (eSwitches.CreqXmlFormat | eSwitches.MediaInfoOutPut | eSwitches.MediaInfoXMLOutPut | eSwitches.NewCreqXmlFormat | eSwitches.TxtFormat | eSwitches.HashOutput)) != 0) uselessCombination = false;

				if(uselessCombination) {
					ColoredWriteLine(ConsoleColor.Red, "Combination of arguments is invalid (Nothing to do)", true);
					Pause();
					return false;
				}

			} catch(Exception ex) {
				if(!triedOldCmd && ConvertOldArgs(ref args)) {
					triedOldCmd = true;
					goto tryOldCmd; //Well, Sue me 
				} else {
					ColoredWriteLine(ConsoleColor.Red, "Error while interpreting commandline arguments", true);
					ColoredWriteLine(ConsoleColor.Red, ex.Message + (ex.InnerException != null ? "\n" + ex.InnerException.Message : ""), true);
					Console.WriteLine("Press any alpha-numeric key to continue");
					Pause();
					return false;
				}
			}

			if(triedOldCmd) {
				ColoredWriteLine(ConsoleColor.Red, "Please update your command line arguments. Refer to --Help");
				ColoredWriteLine(ConsoleColor.Red, "Converted Args: " + string.Join(" ", args
					.Where(ldArg => ldArg.StartsWith("-"))
					.Select(ldArg => ldArg.Contains(" ") && ldArg.Contains("=") ? ldArg.Substring(0, ldArg.IndexOf('=')) + "=\"" + ldArg.Substring(ldArg.IndexOf('=')+1) + "\"" : ldArg)
					.ToArray()
				));
				Thread.Sleep(1000);
			}


			return true;
		}
		private static bool ConvertOldArgs(ref string[] origArgs) {
			int? ms = null;
			var args = origArgs.ToArray();

			var map = "M>M;X>m;x>t;y>C;Y>c;c>R;m>Mon;p>p;q>P;r>r;t>;T>;w>q;z>;H>B;U>U;0>0;1>1;2>2;3>3;4>4;5>5;6>6;a>a;e>PrintEd2kLink;d>PrintAniDBLink;D>OpenAniDBLink;u>".Split(';').ToDictionary(ldStr => ldStr.Split('>')[0][0], ldStr => ldStr.Split('>')[1]);

			try {
				for(int i = 0;i < args.Length;i++) {
					var arg = args[i].ToLower();

					if(arg.StartsWith("-ac:")) {
						args[i] = "--Auth=" + args[i].Substring("-ac:".Length);

					} else if(arg.StartsWith("-log:")) {
						args[i] = "--Log=" + args[i].Substring("-log:".Length);

					} else if(arg.StartsWith("-done:")) {
						args[i] = "--Done=" + args[i].Substring("-done:".Length);

					} else if(arg.StartsWith("-acerr:")) {
						args[i] = "--ACErr=" + args[i].Substring("-acerr:".Length);

					} else if(arg.StartsWith("-crcerr:")) {
						args[i] = "--CRCErr=" + args[i].Substring("-crcerr:".Length);

					} else if(arg.StartsWith("-extdiff:")) {
						args[i] = "--ExtDiff=" + args[i].Substring("-extdiff:".Length);

					} else if(arg.StartsWith("-exp:")) {
						args[i] = "--Exp=" + args[i].Substring("-exp:".Length);

					} else if(arg.StartsWith("-hlog:")) {
						args[i] = "--HLog=" + args[i].Substring("-hlog:".Length);

					} else if(arg.StartsWith("-ms:")) {
						ms = int.Parse(args[i].Substring(4));
						//args[i] = "--Done=" + args[i].Substring("-ms:".Length);

					} else if(arg.StartsWith("-ext:")) {
						args[i] = "--Done=" + args[i].Substring("-ext:".Length);

					} else if(arg.StartsWith("-bsize:")) {
						args[i] = "--BSize=" + args[i].Substring("-bsize:".Length);

					} else if(arg.StartsWith("-tout:")) {
						args[i] = "--TOut=" + args[i].Substring("-tout:".Length);

					} else if(arg.StartsWith("-lport:")) {
						args[i] = "--LPort=" + args[i].Substring("-lport:".Length);

					} else if(arg.StartsWith("-host:")) {
						args[i] = "--Host=" + args[i].Substring("-host:".Length);

					} else if(arg.StartsWith("-noerr:")) {
						args[i] = "--NoErr" + args[i].Substring("-noerr:".Length);

					} else if(args[i].StartsWith("-")) {
						List<string> argsToAdd = new List<string>();
						for(int j = 1;j < args[i].Length;j++) {
							if(!map.ContainsKey(args[i][j])) return false;
							string value = map[args[i][j]];

							args[i] = args[i].Replace(args[i][j].ToString(), value.Length == 1 ? value[0].ToString() : "");
							if(value.Length > 1) { argsToAdd.Add(value); j--; }
						}


						var offset = 0;
						if(argsToAdd.Count != 0) {
							Array.Resize(ref args, args.Length + argsToAdd.Count);
							for(int j = args.Length - 1;j >= i + argsToAdd.Count;j--) args[j] = args[j - argsToAdd.Count];
							for(int j = 0;j < argsToAdd.Count;j++) {
								args[i + 1 + j] = "--" + argsToAdd[j] + (argsToAdd[j].Equals("Mon") ? "=" + (ms.GetValueOrDefault(3600000) / 1000) : "");
							}
							offset += argsToAdd.Count;
						}

						if(args[i].Length == 1) {
							for(int j = i;j < args.Length - 1;j++) args[j] = args[j + 1];
							Array.Resize(ref args, args.Length - 1);
							offset--;
						}

						i += offset;
					//} else {
					//    args[i] = "\"" + args[i] + "\"";
					}
				}
			} catch(Exception ex) {
				return false;
			}

			origArgs = args;
			return true;
		}


		static void Main(string[] args) {
			DebugWrite("Main: Entry");
			try { int top = Console.CursorTop; Console.CursorTop = 1; Console.CursorTop = top; hasConsole = true; } catch(Exception) { switches |= eSwitches.SupressProgress; hasConsole = false; }
			DebugWrite("Main: After Consolewindow check");

			var consoleIn = Console.In;
			Console.SetOut(new ConsoleOutFilter(Console.Out));
			DebugWrite("Main: Console output filter set");

			if((switches & eSwitches.UseUTF8OutputStream) != 0) Console.OutputEncoding = new UTF8Encoding();


			Thread.CurrentThread.CurrentCulture = CultureInfo.InvariantCulture;
			AppDomain.CurrentDomain.UnhandledException += new UnhandledExceptionEventHandler(GlobalExceptionHandler);


#if(Debug)
			if(args.Length == 0) args = SetArguments();
#endif

			if(!ParseArgs(args)) return;

			DebugWrite("Main: Args set and valid");

			DebugWrite("Main: Before Check");
			if(!SelfCheck()) return;
			DebugWrite("Main: Check Done");

			if((switches & eSwitches.HashingSpeedTest) != 0) { Benchmark(); return; }

			DebugWrite("Main: Entering ProcessMedia");
			ProcessMedia(new List<string>(mediaLst));
			DebugWrite("Main: Left ProcessMedia");

			if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();
			if((switches & eSwitches.PauseWhenDone) != 0) { Console.WriteLine("Press any alpha-numeric key to continue"); Pause(); }
		}


		private static void Benchmark() {
			//var bla = new BlockConsumerContainer(16, 4 * 1024 * 1024);
			//bla.RegisterBlockConsumer("MKV", new MatroskaParser("MKV"));
			//bla.AddBlockConsumer("MKV");

			//var watch = new Stopwatch();
			//watch.Start();
			//bla.Start(File.OpenRead(mediaLst[0]));
			//bla.Join();
			//watch.Stop();
			//Console.WriteLine(watch.ElapsedMilliseconds);
			//Console.WriteLine();

			if(mediaLst.Count == 1 && File.Exists(mediaLst[0])) {
				Console.WriteLine("File: " + mediaLst[0]);
				Console.WriteLine("Size: " + (new FileInfo(mediaLst[0]).Length >> 20) + "mb");
				Console.WriteLine("Reading...");
				var fileStream = File.OpenRead(mediaLst[0]);
				var b = new byte[blockSize * 1024];
				var sw = new Stopwatch();
				sw.Start();
				while(fileStream.Read(b, 0, b.Length) != 0) {
					if(sw.Elapsed.Seconds % 2 == 0) { Console.CursorLeft = 0; Console.Write(fileStream.Position * 100 / fileStream.Length + "%"); }
				}
				sw.Stop();
				Console.CursorLeft = 0;
				Console.WriteLine("100%");
				Console.WriteLine("Elapsed: " + sw.Elapsed.TotalSeconds + "s");
				Console.WriteLine("Speed: " + ((new FileInfo(mediaLst[0]).Length >> 20) / sw.Elapsed.TotalSeconds).ToString("0.00") + "mb/s (may be inaccurate due to read caching)");
				Console.WriteLine();
			}

			var container = CreateContainer(blockCount, blockSize);
			Dictionary<string, IBlockConsumer> blockConumsers;
			var e = new FileEnvironment(null, container, null, DateTime.Now, 1, 0, 4 * 1024 * 1024 * 1024L, 0);


			ExecuteBlockConsumers(e, out blockConumsers, new NullStream(e.TotalBytes));
			Pause();
		}



#if(Debug)
		private static string[] SetArguments() {
			return new string[] {
				//@"-crcerr:blabla2.txt",
				//@"-p",
				//@"-exp:blabla.txt",
				//@"-1",
				//"--Mon=1",
				//"--help",

				//@"E:\Anime",
				//"--UseCWD",
				"-CPp",
				//"-exp:d:bla.txt",
				@"--Ext=tt",
				//@"--Done=log_test_done.txt",
				//@"--Log=log_test_log.txt",
				//@"--ExtDiff=log_test_extdiff.txt",
				//@"--FixExt=log_test_extfix.txt",
				//@"--ACErr=log_test_acerr.txt",
				//@"--CrcErr=log_test_crcerr.txt",
				//@"--Debug=debug.log",
				//@"--Host=127.0.0.1:9002",
			};
		}
#endif

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

			long processedBytes = 0, totalBytes;
			sentACReqs = 0;
			failedACReqs = 0;

			DebugWrite("ProcessMedia: GetFiles");
#if(PublicRelease)
			var files = GetFiles(mediaLst, searchOption, processExtensions, out totalBytes);
#else
			var files = GetFiles(mediaLst, searchOption, processExtensions, out totalBytes);
#endif
			DebugWrite("ProcessMedia: GetFiles Done");

			if(files.Count == 0) Console.WriteLine("No files to process");


			FileEnvironment e;
			bool fatalExceptionThrown = false;
			var retryOnIOExCount = retriesOnIOException;
			var container = CreateContainer(blockCount, blockSize);
			for(int i = 0;i < files.Count;i++) {
				DebugWrite("ProcessMedia: Entering ProcessMediaFile");
				e = new FileEnvironment(appVersion, container, files[i], startedOn, files.Count, i, totalBytes, processedBytes);
				try {
					ProcessMediaFile(e);
					retryOnIOExCount = retriesOnIOException;
				} catch(FatalException ex) {
					e.AddException("Fatal error while processing the file.", ex);
					fatalExceptionThrown = true;
				} catch(IOException ex) {
					if(--retryOnIOExCount != 0) {
						ColoredWriteLine(ConsoleColor.Red, "File processing failed with an IOException. Retrying...", true);
						i--;
					} else {
						ColoredWriteLine(ConsoleColor.Red, "File processing failed " + retriesOnIOException + " times with an IOException. Skipping...", true);
						retryOnIOExCount = retriesOnIOException;
					}

				} catch(Exception ex) {
					e.AddException("Unhandled error while processing the file.", ex);
				}
				DebugWrite("ProcessMedia: Left ProcessMediaFile");
				container.Reset();
				processedBytes += e.File.Length;

				if(e.Exceptions.Count != 0) {
					try {
						var exElem = e.Exceptions.ToXElement(true);
						string exPath = Path.Combine(CurrentDirectory, "Error");
						string exFileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";
						if(!Directory.Exists(exPath)) Directory.CreateDirectory(exPath);
						using(var writer = new SafeXmlWriter(Path.Combine(exPath, exFileName), Encoding.Unicode)) exElem.Save(writer);
					} catch(Exception) { }

#if(HasACreq)
					if(username != null && password != null && (switches & eSwitches.NoErrorReporting) == 0) {
						try {
							var exElem = e.Exceptions.ToXElement(false);
							MemoryStream memStream = new MemoryStream();
							using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) exElem.Save(writer);

							anidb.CommitError(host, timeout * 1000, "avdumplib", appVersion.Build, username.ToLower(), password, memStream.ToArray());
						} catch(Exception) { }
					}
#endif
				}

				if(fatalExceptionThrown) Environment.Exit(1);

				if((switches & eSwitches.PauseWhenFileDone) != 0) {
					Console.WriteLine("Press any alpha-numeric key to continue");
					Pause();
				}

			}


			DebugWrite("ProcessMedia: Left loop, wait for dumping to finish");
#if(HasACreq)
			if(anidb != null) {
				while(anidb.UnsentQueryCount != 0) {
					Console.WriteLine("ACReq( Done: " + sentACReqs + " Todo: " + (files.Count - sentACReqs - failedACReqs - anidb.UnsentQueryCount) + " Failed: " + failedACReqs + " Pending: " + anidb.UnsentQueryCount + " )");
					if(hasConsole) Console.SetCursorPosition(0, Console.CursorTop - 1);
					Thread.Sleep(200);
				}
				Console.WriteLine("ACReq( Done: " + sentACReqs + " Todo: " + (files.Count - sentACReqs - failedACReqs - anidb.UnsentQueryCount) + " Failed: " + failedACReqs + " Pending: " + anidb.UnsentQueryCount + " )");
				anidb.Stop();
				anidb.Join();
			}
#endif
			DebugWrite("ProcessMedia: Done");


			isProcessing = false;
			if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startedOn).TotalSeconds + "s");
		}
		private static void ProcessMediaFile(FileEnvironment e) {
			Console.WriteLine("Folder: " + e.File.DirectoryName);
			Console.WriteLine("Filename: " + e.File.Name);

			if(e.File.Length == 0) {
				ColoredWriteLine(ConsoleColor.Red, "Skipping 0byte File", true);
				return;
			}


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
					//if((switches & (eSwitches.Tth)) != 0) e.Container.AddBlockConsumer("TTH2");
					//if((switches & (eSwitches.Tth)) != 0) e.Container.AddBlockConsumer("TTH3");
					if(isMatroska) e.Container.AddBlockConsumer("Matroska");
					if(isOgmOgg) e.Container.AddBlockConsumer("Ogm/Ogg");

					progress = e.Container.Start(fileStream);
				}

				if((switches & eSwitches.SupressProgress) == 0) {
					try {
						DisplayBuffer(e, progress);

						//throw new Exception("Just me testing again");

					} catch(Exception ex) {
						Console.WriteLine();


						CursorVisible = true;


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

			var detExt = p[StreamType.General, 0, EntryKey.Extension] != null ? p[StreamType.General, 0, EntryKey.Extension].Value.ToLower() : null;
			var ext = e.File.Extension.Length <= 1 ? "" : e.File.Extension.Substring(1).ToLower();

			if(!string.IsNullOrEmpty(extFixPath) && !string.IsNullOrEmpty(detExt) && !ext.Equals(detExt)) {
				try {
					if(!detExt.Contains(' ')) {
						e.File.MoveTo(Path.ChangeExtension(e.File.FullName, detExt));
						AppendAllText(extFixPath, ext + " => " + detExt + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension fix file");
						ext = detExt;
					} else {
						AppendAllText(extFixPath, "FAILED: " + ext + " => " + detExt + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension fix file");
					}
				} catch(Exception ex) {
					AppendAllText(extFixPath, "FAILED: " + ext + " => " + detExt + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension fix file");
				}
			}

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
				if((switches & eSwitches.HashOutput) != 0) log += Info.CreateHashLog(e.File.FullName, p);

				if(logPath != null && !String.IsNullOrEmpty(log)) AppendAllText(logPath, log + Environment.NewLine + Environment.NewLine, "Couldn't update logfile");
			} catch(FatalException ex) {
				throw;
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

#if(HasACreq)
			if(acreqFile && (dumpableExtensions.Contains(ext) || dumpableExtensions.Contains(detExt) || (switches & eSwitches.DumpAllExtensions) != 0) && !(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				MemoryStream stream = new MemoryStream();

				byte[] creqBytes;
				using(var writer = new SafeXmlWriter(stream, new UTF8Encoding(), Formatting.None, lowerCaseElements: true)) {
					var xmlDoc = Info.CreateAVDumpLog(p);
					if(xmlDoc["file"]["md4"] != null) xmlDoc["file"].RemoveChild(xmlDoc["file"]["md4"]);
					if(xmlDoc["file"]["tiger"] != null) xmlDoc["file"].RemoveChild(xmlDoc["file"]["tiger"]);


					xmlDoc.Save(writer);

					creqBytes = new byte[stream.Length - 38];
					stream.Position = 38;
					stream.Read(creqBytes, 0, (int)stream.Length - 38);
				}
				anidb.QueryACReq(new ACReq(host, timeout * 1000, "avdumplib", appVersion.Build, username.ToLower(), password, creqBytes), new AniDB.QueryTag(e));
			}
#endif

			if(!string.IsNullOrEmpty(extDiffListPath) && !ext.Equals(detExt)) {
				AppendAllText(extDiffListPath, ext + " => " + (detExt == null ? "unknown" : detExt) + "	" + e.File.FullName + Environment.NewLine, "Couldn't update extension diff file");
			}

			#region DoneLog Stream Writing
			if(doneListPath != null && (username == null || password == null) && e.Exceptions.Count == 0) {
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

					ColoredWriteLine(ConsoleColor.Yellow, "Filename doesn't contain the calculated CRC (" + crcHash + ")");
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

		private static bool SelfCheck() {
			DebugWrite("SelfCheck: 1");
			var waitHandle = new AutoResetEvent(true);
			try {
				waitHandle.WaitOne(10);
			} catch(Exception ex) {
				ColoredWriteLine(ConsoleColor.Red, ".NET 3.5 SP1 probably not installed", true);
				Pause();
				return false;
			}
			waitHandle.Close();
			DebugWrite("SelfCheck: 2");

			//#####################################################

			if(Environment.OSVersion.Platform == PlatformID.Unix && !File.Exists(Path.Combine(AppPath, "AVDump2Lib.dll.config"))) {
				ColoredWriteLine(ConsoleColor.Red, "AVDump2Lib.dll.config is missing", true);
				Pause();
				return false;
			}
			DebugWrite("SelfCheck: 3");

			//#####################################################

			var milFileName = Path.Combine(AppPath, Environment.OSVersion.Platform == PlatformID.Unix ? (IntPtr.Size == 4 ? "libMediaInfo_x86.so" : "libMediaInfo_x64.so") : (IntPtr.Size == 4 ? "MediaInfo_x86.dll" : "MediaInfo_x64.dll"));
			if(!File.Exists(milFileName)) {
				ColoredWriteLine(ConsoleColor.Red, "MediaInfoLib is missing", true);
				Pause();
				return false;
			}
			DebugWrite("SelfCheck: 4");

			//#####################################################


#if(HasACreq)
			try {
				IMediaInfo mi = IntPtr.Size == 8 ? (IMediaInfo)new MediaInfo_x64() : (IMediaInfo)new MediaInfo_x86();
				if(!mi.Option("Info_Version").Equals("MediaInfoLib - v0.7.42")) {
					ColoredWriteLine(ConsoleColor.Red, "Mediainfo library version mismatch. Needed: v0.7.42, Used: " + mi.Option("Info_Version"), true);
					Pause();
					return false;
				}
				mi.Close();
			} catch(Exception ex) {
				ColoredWriteLine(ConsoleColor.Red, "Media Info Library not found. Error: " + ex.ToString(), true);
				Pause();
				return false;
			}
			DebugWrite("SelfCheck: 5");

			if(!string.IsNullOrEmpty(username)) {
				if(AniDB.CheckMILHash()) {
					ColoredWriteLine(ConsoleColor.Red, "Mediainfo lib hash mismatch", true);
					Pause();
					return false;
				}

				DebugWrite("SelfCheck: 5a");
				int tries = 5;
				while(tries != 0) {
					try {
						host = new IPEndPoint(Dns.GetHostAddresses(hostAddress).First(), hostPort);
						break;
					} catch(Exception) { }
					tries--;
				}
				if(host == null) {
					ColoredWriteLine(ConsoleColor.Red, "Couldn't resolve host " + hostAddress, true);
					return false;
				}
				DebugWrite("SelfCheck: 5b");

				anidb = new AniDB(AcreqResult, localPort);
				var result = anidb.TestConnectivity(host, timeout * 1000, "avdumplib", appVersion.Build, username.ToLower(), password);
				if(result.State != ACReq.CommitState.Success) {
					ColoredWriteLine(ConsoleColor.Red, "Couldn't establish session with anidb (" + result.State + ")", true);
					if(result.Error != null) {
						ColoredWriteLine(ConsoleColor.Red, result.Error.Message, true);
						if(result.Error.InnerException != null) ColoredWriteLine(ConsoleColor.Red, result.Error.InnerException.Message, true);
					}

					Pause();
					return false;
				}
				DebugWrite("SelfCheck: 5c");
			}
			DebugWrite("SelfCheck: 6");
#endif

			//#####################################################

			return true;
		}

#if(HasACreq)
		private static AniDB anidb;
		private static bool AcreqResult(ACReq.CommitResult result, object tag) {
			var queryTag = (AniDB.QueryTag)tag;

			FileEnvironment e = (FileEnvironment)queryTag.Tag;
			if(!string.IsNullOrEmpty(acErrListPath) && (result.State & (ACReq.CommitState.Success | ACReq.CommitState.TimeOut)) == 0) {
				AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.State.ToString() + Environment.NewLine, "Couldn't update acerr file");
			}

			bool resentCreq = false;
			switch(result.State) {
				case ACReq.CommitState.Success:
					sentACReqs++;
					if(doneListPath != null) {
						AppendAllText(doneListPath, e.File.FullName + Environment.NewLine, "Couldn't update donelist file");
						int index = doneListContent.BinarySearch(e.File.FullName);
						if(index < 0) doneListContent.Insert(~index, e.File.FullName);
					}
					break;

				case ACReq.CommitState.TimeOut:
					if(queryTag.Retries < CL.retries) {
						queryTag.IncreaseTries();
						resentCreq = true;
					} else {
						failedACReqs++;
						if(!string.IsNullOrEmpty(acErrListPath)) AppendAllText(acErrListPath, e.File.FullName + " ACreq " + result.State.ToString() + Environment.NewLine, "Couldn't update acerr file");
					}
					break;

				case ACReq.CommitState.WrongVersionOrPassOrName:
					failedACReqs++;
					ColoredWriteLine(ConsoleColor.Red, "Either the client is outdated or your username/password combination is wrong.", true);
					Pause();
					break;

				case ACReq.CommitState.InternalError: failedACReqs++; break;
			}

			return resentCreq;
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

				CursorVisible = false;
				output = "Bar: Buffer usage | blocksize: " + blockSize + "KB | blockCount: " + blockCount + "\n";

				for(int i = 0;i < progress.BlockConsumerCount;i++) {
					output += progress.Name(i).PadRight(maxNameLength + 1) + "[" + "".PadRight(consoleWidth - maxNameLength - 4) + "]\n";
				}
				output += "\n" + "Progress".PadRight(maxNameLength + 1) + "[" + "".PadRight(consoleWidth - maxNameLength - 4) + "]\n\n\n";

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

						Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 4);
						Console.Write("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));
					}
					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - 3);
					charCount = fileSize != 0 ? (int)((double)bytesProcessed / (double)fileSize * barLength) : barLength;
					Console.WriteLine("".PadLeft(charCount, '*') + "".PadRight(barLength - charCount, ' '));



					output =
					  "Position: " + (bytesProcessed >> 20).ToString().PadLeft(3) + "MB/" + (fileSize >> 20) + "MB  " +
					  "Elapsed time: " + progress.TimeElapsed.ToFormatedString() + " " +
					  "Speed: " + Math.Max((int)((bytesProcessed >> 20) / progress.TimeElapsed.TotalSeconds), 0) + "MB/s";

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
					//output += "".PadLeft(output.Length < consoleWidth ? consoleWidth - output.Length - 1 : 0, ' ');
					Console.WriteLine(output.PadRight(consoleWidth - 1));
				}

#if(HasACreq)
				if(anidb != null) {
					output = "ACReq( Done: " + sentACReqs + " Todo: " + (e.TotalFiles - sentACReqs - failedACReqs - anidb.UnsentQueryCount) + " Failed: " + failedACReqs + " Pending: " + anidb.UnsentQueryCount + " )";
					Console.Write(output.PadRight(consoleWidth - 1));
				}
#endif

				if(doLoop) Thread.Sleep(80);
			} while(doLoop);


			if(progress != null) {
				for(int i = 0;i < progress.BlockConsumerCount;i++) {
					Console.SetCursorPosition(maxNameLength + 2, lastLineIndex - progress.BlockConsumerCount + i - 4);
					Console.Write(progress.BlockConsumerObj(i).ToString());
				}
				Console.SetCursorPosition(0, lastLineIndex);

			}
			Console.WriteLine();
			CursorVisible = true;
		}


		private static BlockConsumerContainer CreateContainer(int blockCount, int blockSize) {
			BlockConsumerContainer container = new BlockConsumerContainer(blockCount, blockSize * 1024);
			//container.RegisterBlockConsumer("TIGER", new HashCalculator("TIGER", new TigerForTTH(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("TIGER", new HashCalculator("TIGER", new Tiger(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("AICH", new HashCalculator("AICH", new Aich(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("SHA1", new HashCalculator("SHA1", new SHA1CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("CRC", new HashCalculator("CRC", new Crc32(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("MD4", new HashCalculator("MD4", new Md4(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("MD5", new HashCalculator("MD5", new MD5CryptoServiceProvider(), h => BaseConverter.ToString(h.Hash)));
			container.RegisterBlockConsumer("TTH", new HashCalculator("TTH", new TTH(Environment.ProcessorCount), h => BaseConverter.ToString(h.Hash, BaseOption.Base32)));
			//container.RegisterBlockConsumer("TTH2", new HashCalculator("TTH2", new TTH2(Environment.ProcessorCount), h => BaseConverter.ToString(h.Hash, BaseOption.Base32)));
			//container.RegisterBlockConsumer("TTH3", new HashCalculator("TTH3", new TTH3(Environment.ProcessorCount), h => BaseConverter.ToString(h.Hash, BaseOption.Base32)));
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
			} catch(FatalException ex) {
				throw;
			} catch(Exception ex) {
				e.AddException("Failed to create MediaInfoProvider", ex); noErrors = false; 
			}

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
		private static List<FileInfo> GetFiles(List<string> mediaLst, SearchOption searchOption, HashSet<string> validExtensions, out long totalBytes) {
			List<FileInfo> files = new List<FileInfo>();

			DateTime lastDiscoverLog = DateTime.Now;
			long anonTotalBytes = 0;
			foreach(var media in mediaLst) {
				FileGetter.TraverseFiles(media, (switches & eSwitches.ExcludeSubFolders) == 0,
					(fi) => {
						if(validExtensions == null || (fi.Extension.Length != 0 && validExtensions.Contains(fi.Extension.Substring(1).ToLower()) && fi.Length != 0)) {
							files.Add(fi);
							anonTotalBytes += fi.Length;

							if((DateTime.Now - lastDiscoverLog).TotalSeconds > 0.2) {
								lastDiscoverLog = DateTime.Now;
								Console.WriteLine("Discovered " + files.Count + " files");
							}
						}
					},

					(ex) => { Console.WriteLine(ex.Message); }
				);
			}
			totalBytes = anonTotalBytes;

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

			if((switches & eSwitches.DoneLogFilenameOnly) != 0) {
				var doneLog = new HashSet<string>(doneListContent.Select(ldPath => Path.GetFileName(ldPath)));
				return files.Where(fi => doneLog.Contains(fi.Name)).ToList();
			} else {
				return files.Where(fi => doneListContent.BinarySearch(fi.FullName) < 0).ToList();
			}
		}

		public static string CurrentDirectory { get {
			return ((switches & eSwitches.UseCWD) != 0) ? Environment.CurrentDirectory : AppPath;
		} }
		public static string AppPath { get { return System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location); } }

		private static void GlobalExceptionHandler(object sender, UnhandledExceptionEventArgs e) {
			string path = Path.Combine(CurrentDirectory, "Error");
			string fileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";

			ExceptionXElement exElem;
			try {
				if(!Directory.Exists(path)) Directory.CreateDirectory(path);
				exElem = new ExceptionXElement((Exception)e.ExceptionObject, false);
				using(var writer = new SafeXmlWriter(Path.Combine(path, fileName), Encoding.ASCII)) {
					exElem.Save(XmlWriter.Create(writer, new XmlWriterSettings { OmitXmlDeclaration = true }));
				}
			} catch(UnauthorizedAccessException ex) {
				exElem = new ExceptionXElement(ex, false);

			} catch(Exception ex) {
				exElem = new ExceptionXElement(new Exception("Couldn't save Exception", ex), false);
			}


#if(HasACreq)
			if(username != null && password != null && (switches & eSwitches.NoErrorReporting) == 0) {
				try {
					MemoryStream memStream = new MemoryStream();
					using(var writer = new SafeXmlWriter(memStream, Encoding.ASCII)) exElem.Save(writer);
					anidb.CommitError(host, timeout * 1000, "avdumplib", appVersion.Build, username.ToLower(), password, memStream.ToArray());
				} catch(Exception) { }
			}
#endif
		}

		private static void Pause() {
			if(!hasConsole) return;

			try {
				ConsoleKeyInfo cki;
				do {
					cki = Console.ReadKey();
				} while(Char.IsControl(cki.KeyChar) && cki.Key != ConsoleKey.Enter);
			} catch(Exception) {
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
			if(debugPath == null) return;
			AppendAllText(debugPath, DateTime.Now.ToString("HH:mm:ss") + " " + line + "\n", "");
		}

		private static bool CursorVisible {
			set { try { Console.CursorVisible = value; } catch(Exception) { } }
		}

		private static void ColoredWriteLine(ConsoleColor color, string line, bool isError = false) {
			try {
				Console.ForegroundColor = color;
				if(!isError) Console.WriteLine(line); else Console.Error.WriteLine(line);
				Console.ResetColor();
			} catch(Exception) {
			}
		}
	}
}
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
#undef HasAcreq

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using AVDump2Lib.Hashes;
using AVDump2Lib.Dump;
using AVDump2Lib.BlockBuffer;
using AVDump2Lib.Information;
using AVDump2Lib.Hashing.Algorithms;
using AVDump2Lib.Hashing.Tools;
using System.Xml;
using System.Linq;
using System.Collections;


namespace AVDump2CL {
	class CL {
		#region Fields
		static Dictionary<char, eSwitches> char2SwitchEnum;
		static int port = 9001;
		static string username;
		static string password;
		static string host = "";
		static eSwitches switches;

		static Stream logStream;
		static Stream ed2kListStream;
		static Stream doneListStream;
		static Stream hashListStream;
		static string hashLogStyle;
		static List<string> mediaLst;
		static List<string> doneListContent;

		static int retries = 3;
		static int timeout = 10;
		static int blockCount = 16;
		static int blockSize = 2 * 1024;
		static int monitorSleepDuration = 60000;
		static List<string> dumpableExtensions;
		static List<string> processExtensions;
		#endregion

		static CL() {
			mediaLst = new List<string>();
			doneListContent = new List<string>();

			dumpableExtensions = new List<String>(new string[] { "avi", "mpg", "mpeg", "rm", "rmvb", "asf", "wmv", "mov", "ogm", "mp4", "mkv", "rar", "zip", "ace", "srt", "sub", "ssa", "smi", "idx", "ass", "txt", "swf", "flv", "ts", "ogv", "7z", "asf", "mp3", "aac", "ac3", "dts", "wav" });
			dumpableExtensions.Sort();

			processExtensions = new List<string>(new string[] { "avi", "ssa", "mkv", "ogm", "mp4", "mov", "mpg", "mpeg", "rm", "srt", "vob", "wmv", "asf", "qt", "flv" });
			processExtensions.Sort();

			char2SwitchEnum = new Dictionary<char, eSwitches>();
			char2SwitchEnum['y'] = eSwitches.CreqXmlFormat; //Done
			char2SwitchEnum['M'] = eSwitches.MediaInfoOutPut; //Done

			char2SwitchEnum['c'] = eSwitches.ExcludeSubFolders; //Done
			char2SwitchEnum['m'] = eSwitches.MonitorFolder; //Done
			char2SwitchEnum['p'] = eSwitches.PauseWhenDone; //Done
			char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone; //Done
			char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
			char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile; //Done
			char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone; //Done
			char2SwitchEnum['o'] = eSwitches.WaitForDumpReply;
			char2SwitchEnum['w'] = eSwitches.SupressProgress; //Done
			char2SwitchEnum['T'] = eSwitches.PrintTotalTimeUsed; //Done

			char2SwitchEnum['0'] = eSwitches.Crc32; //Done
			char2SwitchEnum['1'] = eSwitches.Ed2k; //Done
			char2SwitchEnum['2'] = eSwitches.Md5; //Done
			char2SwitchEnum['3'] = eSwitches.Sha1; //Done
			char2SwitchEnum['5'] = eSwitches.Tth; //Done
			char2SwitchEnum['6'] = eSwitches.Aich;
			char2SwitchEnum['a'] = eSwitches.UseAllHashes; //Done
			char2SwitchEnum['u'] = eSwitches.PrintElapsedHashingTime; //Done
		}

		static void Main(string[] args) {
			if(!ParseClOptions(args)) return;
			Console.CursorVisible = false;

			DateTime startTime = DateTime.Now;
			ProcessMedia(new Queue<string>(mediaLst));

			if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();

			if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startTime).TotalSeconds + "s");
			if((switches & eSwitches.PauseWhenDone) != 0) {
				Console.WriteLine("Press any key to continue");
				Console.ReadKey();
			}

			if(ed2kListStream != null) ed2kListStream.Dispose();
			if(doneListStream != null) doneListStream.Dispose();
			if(logStream != null) logStream.Dispose();
		}

		private static void MonitorFiles() {
			while(true) {
				Console.WriteLine("Monitoring folders, press Ctrl+C to stop");
				Thread.Sleep(monitorSleepDuration);
				ProcessMedia(new Queue<string>(mediaLst));
			}
		}

		private static void ProcessMedia(Queue<string> mediaLst) {
			SearchOption searchOption = switches != eSwitches.ExcludeSubFolders ? SearchOption.AllDirectories : SearchOption.TopDirectoryOnly;
			while(mediaLst.Count != 0) {
				string media = mediaLst.Dequeue();

				if(System.IO.File.Exists(media)) {
					ProcessMediaFile(media);
				} else if(System.IO.Directory.Exists(media)) {
					List<string> files = new List<string>();
					foreach(string ext in processExtensions) {
						files.AddRange(System.IO.Directory.GetFiles(media, "*." + ext, searchOption));
					}
					System.IO.Directory.GetFiles(media, "*." + String.Join(".*,", processExtensions.ToArray()) + ".*");
					for(int i = 0;i < files.Count;i++) ProcessMediaFile(files[i]);
				} else {
					//TODO Error?
				}
			}
		}

		private static void ProcessMediaFile(string filePath) {
			bool error = false;
			string fileExt = System.IO.Path.GetExtension(filePath).Substring(1);
			if(doneListContent.Contains(filePath) || processExtensions.BinarySearch(fileExt) < 0) return;

			Stream stream = /*new NullStream(5 * 1024);*/ System.IO.File.OpenRead(filePath);

			#region Hashing
			DateTime startTime = DateTime.Now;
			HashContainer hashContainer = new HashContainer();
			if((switches & (eSwitches.Aich | eSwitches.Crc32 | eSwitches.Ed2k | eSwitches.Md5 | eSwitches.Sha1 | eSwitches.Tth)) != 0) {
				Console.WriteLine("Hashing: " + System.IO.Path.GetFileName(filePath));

				if((switches & (eSwitches.Crc32)) != 0) hashContainer.AddHashAlgorithm(new Crc32(), "CRC32");
				if((switches & (eSwitches.Ed2k)) != 0) hashContainer.AddHashAlgorithm(new Ed2k(), "ED2K");
				if((switches & (eSwitches.Tth)) != 0) hashContainer.AddHashAlgorithm(new Tiger(), "TTH");
				//if((switches & (eSwitches.Aich)) != 0) hashContainer.AddHashAlgorithm(new Aich(), "AICH");
				if((switches & (eSwitches.Md5)) != 0) hashContainer.AddHashAlgorithm(new System.Security.Cryptography.MD5CryptoServiceProvider(), "MD5");
				if((switches & (eSwitches.Sha1)) != 0) hashContainer.AddHashAlgorithm(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "SHA1");

				HashContainer.HashingProgress progress = hashContainer.Start(blockCount, stream, blockSize * 1024);
				if((switches & eSwitches.SupressProgress) == 0) DisplayHashBuffer(progress);
			}
			List<HashExecute> hashExecutes = hashContainer.Join();
			#endregion

			if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalSeconds + "s");

			#region Log Output
			string log = "";
			if((switches & eSwitches.CreqXmlFormat) != 0) {
				StringWriter sw = new StringWriter();
				Info.CreateAVDumpLog(filePath, hashExecutes).Save(sw);
				log += sw.ToString();

				sw = new StringWriter();
				Info.CreateMediaInfoXMLLog(filePath, hashExecutes).Save(sw);
				log += sw.ToString();

			}
			if((switches & eSwitches.MediaInfoOutPut) != 0) {
				log += Info.CreateMediaInfoDump(filePath);
			}

			if(logStream != null && !String.IsNullOrEmpty(log)) {
				byte[] infoBytes = System.Text.Encoding.UTF8.GetBytes(log);
				logStream.Write(infoBytes, 0, infoBytes.Length);
			}
			#endregion

			#region DoneLog Stream Writing
			if(doneListStream != null) {
				byte[] donePathBytes = System.Text.Encoding.UTF8.GetBytes(filePath + '\n');
				doneListStream.Write(donePathBytes, 0, donePathBytes.Length);
				doneListContent.Add(filePath);
			}
			#endregion

			#region Ed2kLog Stream Writing
			HashExecute ed2kHashExecute = hashExecutes.FirstOrDefault((HashExecute he) => { return he.HashObj is Ed2k; });
			Ed2k ed2k = (Ed2k)(ed2kHashExecute != null ? ed2kHashExecute.HashObj : null);
			if(ed2k != null) {
				byte[] blueEd2kHash = ed2k.BlueHash; //Handle Ed2k screwup
				byte[] redEd2kHash = ed2k.Hash;

				if(ed2kListStream != null) {
					byte[] ed2kBytes = System.Text.Encoding.UTF8.GetBytes(BaseConverter.ToString(redEd2kHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + (blueEd2kHash != null ? "|" + BaseConverter.ToString(blueEd2kHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) : ""));
					ed2kListStream.Write(ed2kBytes, 0, ed2kBytes.Length);
				}
			}
			#endregion

			#region HashLog Stream Writing
			if(hashListStream != null) {
				string formattedStr = hashLogStyle;
				foreach(var hashExecute in hashExecutes) {
					if(hashExecute.HashObj is Ed2k) {
						string ed2kStr = "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
						if(!((Ed2k)hashExecute.HashObj).BlueIsRed()) {
							ed2kStr += "*" + "ed2k://|file|" + System.IO.Path.GetFileName(filePath) + "|" + stream.Length + "|" + BaseConverter.ToString(((Ed2k)hashExecute.HashObj).BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse) + "|/";
						}

						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", ed2kStr);
					} else {
						formattedStr = formattedStr.Replace("$" + hashExecute.Name + "$", BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Base32 | eBaseOption.Pad | eBaseOption.Reverse));
					}
				}
				byte[] formattedStrBytes = System.Text.Encoding.UTF8.GetBytes(formattedStr + "\n");
				hashListStream.Write(formattedStrBytes, 0, formattedStrBytes.Length);
			}
			#endregion

			if((switches & eSwitches.DeleteFileWhenDone) != 0 && !error) System.IO.File.Delete(filePath);
			if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");
			if((switches & eSwitches.PauseWhenFileDone) != 0) {
				Console.WriteLine("Press any key to continue");
				Console.ReadKey();
			}
			stream.Dispose();

			#region ACreqing
#if(HasAcreq) //If you get an error below: Scroll to the top of the page and comment #define HasAcreq out
			if(!(string.IsNullOrEmpty(username) || string.IsNullOrEmpty(password))) {
				string creq = Info.CreateAVDumpLog(filePath, hashExecutes).OuterXml;
				//creq = filePath + "\n" + creq;

				Console.Write("Sending Creq... ");
				ACreq.eStatus status = ACreq.DoACreq(new ACreq.ClientInfo("avdumplib", 1, port, username, password, creq), (eSwitches.WaitForDumpReply & switches) != 0);
				switch(status) {
					case ACreq.eStatus.ACreqSent:
						Console.WriteLine("Done.");
						break;
					case ACreq.eStatus.AsyncCall:
						break;
					default:
						Console.WriteLine("Failed. Reason: " + System.Enum.GetName(typeof(ACreq.eStatus), status));
						break;
				}
			}
#endif
			#endregion
		}

		private static void DisplayHashBuffer(HashContainer.HashingProgress progress) {
			Console.WriteLine("*: Buffersize available for hashalgorithm blocksize: " + blockSize + "kb blockCount: " + blockCount);

			int cursorPos = Console.CursorTop;
			long fileSize = progress.StreamSize;
			double bufferSize = 0; int charCount = 0; long bytesProcessed = 0; int timeElapsed;

			Average[] mean = new Average[progress.HashObjCount];
			for(int i = 0;i < mean.Length;i++) mean[i] = new Average();

			bool doLoop = !progress.HasFinished;
			while(doLoop) {
				doLoop = !progress.HasFinished;

				Console.CursorTop = cursorPos;
				Console.CursorLeft = 0;
				bytesProcessed = 0;
				for(int i = 0;i < progress.HashObjCount;i++) {
					mean[i].Add(progress.BlockCount(i));
					bufferSize = mean[i].Calc(5);
					if(bytesProcessed > progress.ProcessedBytes(i) || bytesProcessed == 0) bytesProcessed = progress.ProcessedBytes(i);

					charCount = bufferSize != 0 ? (int)((bufferSize / (double)blockCount) * (Console.WindowWidth - 11)) : 0;
					charCount = progress.ProcessedBytes(i) == fileSize ? 0 : charCount;

					Console.Write(progress.HashObjName(i));
					Console.CursorLeft = 8;
					Console.WriteLine("[" + "".PadLeft(charCount, '*') + "".PadRight(Console.WindowWidth - 11 - charCount, ' ') + "]");
				}

				Thread.Sleep(100);

				timeElapsed = (int)progress.TimeElapsed.TotalMilliseconds;
				Console.Write("\n" +
				  "Position: " + ((double)bytesProcessed / (1 << 20)).ToString("0MB") + "/" + ((double)fileSize / (1 << 20)).ToString("0MB") + "  " +
				  "Elapsed time: " + (timeElapsed / 1000d).ToString("0.0").PadLeft(3) + "s  " +
				  "Speed: " + (((double)bytesProcessed / (1 << 20)) / (timeElapsed / 1000d)).ToString("0.00MB/s") +
				  "".PadLeft(80 - Console.CursorLeft)
				);
			}
			Console.WriteLine();
		}

		private static bool ParseClOptions(string[] args) {
			if(args.Length == 0) { Console.WriteLine(help); Console.ReadKey(); return false; }

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
							//sr.Dispose();
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
					Console.WriteLine("Error in Commandline: " + args[i] + ". Aborting!\nPress key to exit");
					Console.ReadKey();
					return false;
				}
			}
			return true;
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

press any key to exit";
		#endregion
	}

	[FlagsAttribute]
	enum eSwitches : long {
		None = 0L,

		//Output
		CreqXmlFormat = 1L << 3,
		MediaInfoOutPut = 1L << 4,

		//Control
		ExcludeSubFolders = 1L << 16,
		MonitorFolder = 1L << 18,
		PauseWhenDone = 1L << 20,
		PauseWhenFileDone = 1L << 21,
		RandomFileOrder = 1L << 22,
		PrintTimeUsedPerFile = 1L << 23,
		DeleteFileWhenDone = 1L << 24,
		WaitForDumpReply = 1L << 25,
		SupressProgress = 1L << 27,
		PrintTotalTimeUsed = 1L << 28,

		DefAudioExtensions = 1L << 32,
		DefVideoExtensions = 1L << 33,
		DefSubtitleExtensions = 1L << 34,

		//Hash
		Crc32 = 1L << 40,
		Ed2k = 1L << 41,
		Md5 = 1L << 42,
		Sha1 = 1L << 43,
		Tth = 1L << 44,
		Aich = 1L << 45,
		UseAllHashes = Crc32 + Ed2k + Md5 + Sha1 + Tth + Aich,
		PrintElapsedHashingTime = 1L << 52
	}

	public class NullStream : Stream {
		long length;
		public NullStream(long length) {
			this.length = length * 1024 * 1024;
			Position = 0;
		}
		public override bool CanRead { get { return true; } }
		public override bool CanSeek { get { return false; } }
		public override bool CanWrite { get { return false; } }
		public override void Flush() { }
		public override long Length { get { return length; } }
		public override long Position { get; set; }
		public override long Seek(long offset, SeekOrigin origin) { throw new NotImplementedException(); }
		public override void SetLength(long value) { throw new NotImplementedException(); }
		public override void Write(byte[] buffer, int offset, int count) { throw new NotImplementedException(); }
		public override int Read(byte[] buffer, int offset, int count) {
			long bytesRead = (Position + count > Length) ? (Length - Position) : count;
			Position += bytesRead;
			return (int)bytesRead;
		}
	}
}

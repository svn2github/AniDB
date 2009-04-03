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

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Threading;
using AVDump2Lib.Hashes;
using AVDump2Lib.MediaInfoLib;



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
        static List<string> mediaLst;
        static List<string> doneListContent;

        static int retries = 3;
        static int timeout = 10;
        static int blockCount = 16;
        static int blockSize = 2048;
        static int monitorSleepDuration = 60000;
        static string dumpableExtensions = "avi,mkv,ogm,mp4,mov,mpg,mpeg,rm";
        static string processExtensions = dumpableExtensions + ",vob,wmv,asf,qt";

        static string gotoLink = "http://anidb.net/perl-bin/animedb.pl?show=file&size={0}&hash={1}";
        #endregion

        static CL() {
            mediaLst = new List<string>();
            doneListContent = new List<string>();

            char2SwitchEnum = new Dictionary<char, eSwitches>();
            char2SwitchEnum['s'] = eSwitches.ShortOutput;
            char2SwitchEnum['l'] = eSwitches.ListCodecs;
            char2SwitchEnum['x'] = eSwitches.OldXmlFormat;
            char2SwitchEnum['y'] = eSwitches.CreqCmlFormat; //Done
            char2SwitchEnum['N'] = eSwitches.NoDataOutput;

            char2SwitchEnum['c'] = eSwitches.ExcludeSubFolders; //Done
            char2SwitchEnum['i'] = eSwitches.SkipFullParsing;
            char2SwitchEnum['m'] = eSwitches.MonitorFolder; //Done
            char2SwitchEnum['n'] = eSwitches.AddNewLine; //Done
            char2SwitchEnum['p'] = eSwitches.PauseWhenDone; //Done
            char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone; //Done
            char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
            char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile; //Done
            char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone; //Done
            char2SwitchEnum['o'] = eSwitches.WaitForDumpReply;
            char2SwitchEnum['v'] = eSwitches.UseLfForProgress;
            char2SwitchEnum['w'] = eSwitches.SupressProgress; //Done
            char2SwitchEnum['T'] = eSwitches.PrintTotalTimeUsed; //Done

            char2SwitchEnum['0'] = eSwitches.Crc32; //Done
            char2SwitchEnum['1'] = eSwitches.Ed2k; //Done
            char2SwitchEnum['2'] = eSwitches.Md5; //Done
            char2SwitchEnum['3'] = eSwitches.Sha1; //Done
            char2SwitchEnum['5'] = eSwitches.Tth; //Done
            char2SwitchEnum['6'] = eSwitches.Aich;
            char2SwitchEnum['9'] = eSwitches.Mp3Hash;
            char2SwitchEnum['a'] = eSwitches.UseAllHashes; //Done
            char2SwitchEnum['h'] = eSwitches.HashingOnly; //Done
            char2SwitchEnum['e'] = eSwitches.PrintEd2kLink; //Done
            char2SwitchEnum['d'] = eSwitches.PrintAniDBLink; //Done
            char2SwitchEnum['g'] = eSwitches.GotoAniDBLink; //Done
            char2SwitchEnum['u'] = eSwitches.PrintElapsedHashingTime; //Done
        }

        static void Main(string[] args) {
            if(!ParseClOptions(args)) return;
            Console.CursorVisible = false;

            if((switches & eSwitches.NoDataOutput) != 0) Console.SetOut(new System.IO.StringWriter());

            DateTime startTime = DateTime.Now;
            ProcessMedia(new Queue<string>(mediaLst));

            if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();

            if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startTime).TotalMilliseconds.ToString());
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
                    foreach(string ext in processExtensions.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries)) {
                        files.AddRange(System.IO.Directory.GetFiles(media, "*." + ext, searchOption));
                    }
                    System.IO.Directory.GetFiles(media, "*." + processExtensions.Replace(",", ",*."), searchOption);
                    for(int i = 0;i < files.Count;i++) ProcessMediaFile(files[i]);
                } else {
                    //TODO Error?
                }
            }
        }

        private static void ProcessMediaFile(string filePath) {
            bool error = false;
            string fileExt = System.IO.Path.GetExtension(filePath).Substring(1);
            if(doneListContent.Contains(filePath) || !processExtensions.Contains(fileExt)) return;

            Stream stream = System.IO.File.OpenRead(filePath);
            DateTime startTime = DateTime.Now;
            Hasher hasher = new Hasher(stream, blockCount, blockSize * 1024);
            if((switches & (/*eSwitches.Aich |*/ eSwitches.Crc32 | eSwitches.Ed2k | eSwitches.Md5 | /*eSwitches.Mp3Hash |*/ eSwitches.Sha1 | eSwitches.Tth | eSwitches.UseAllHashes)) != 0) {
                Console.WriteLine("Hashing: " + System.IO.Path.GetFileName(filePath));

                if((switches & (eSwitches.Crc32 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Crc32(), "crc");
                if((switches & (eSwitches.Ed2k | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Ed2k(), "ed2k");
                //if((switches & (eSwitches.Tth | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Tiger(), "tth");
                if((switches & (eSwitches.Md5 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.MD5CryptoServiceProvider(), "md5");
                if((switches & (eSwitches.Sha1 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "sha1");

                hasher.Start();
                if((switches & eSwitches.SupressProgress) == 0) DisplayHashBuffer(hasher);
            }

            if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");

            if((switches & eSwitches.HashingOnly) == 0) {
                System.Xml.XmlDocument xmlDoc = (System.Xml.XmlDocument)Info.CreateInfoLog(filePath, ToLogFormatSwitch(switches), hasher.HashStringDict, stream.Length);
                if(logStream != null) {
                    byte[] filePathBytes = System.Text.Encoding.UTF8.GetBytes(filePath);
                    logStream.Write(filePathBytes, 0, filePathBytes.Length);
                    xmlDoc.Save(logStream);
                    logStream.Write(new byte[] { 13, 10 }, 0, 2);
                }
            } else {
                //TODO
            }

            if(doneListStream != null) {
                byte[] donePathBytes = System.Text.Encoding.UTF8.GetBytes(filePath + '\n');
                doneListStream.Write(donePathBytes, 0, donePathBytes.Length);
                doneListContent.Add(filePath);
            }

            if(hasher.HashStringDict.ContainsKey("ed2k")) {
                if(ed2kListStream != null) {
                    byte[] ed2kBytes = System.Text.Encoding.UTF8.GetBytes(hasher.HashStringDict["ed2k"] + '\n');
                    ed2kListStream.Write(ed2kBytes, 0, ed2kBytes.Length);
                }

                if((switches & eSwitches.PrintEd2kLink) != 0) Console.WriteLine("Ed2k: " + hasher.HashStringDict["ed2k"]);
                if((switches & eSwitches.PrintAniDBLink) != 0) Console.WriteLine(String.Format(gotoLink, stream.Length, hasher.HashStringDict["ed2k"]));
                if((switches & eSwitches.GotoAniDBLink) != 0) Process.Start(String.Format(gotoLink, stream.Length, hasher.HashStringDict["ed2k"]));
            }

            stream.Dispose();
            if((switches & eSwitches.DeleteFileWhenDone) != 0 && !error) System.IO.File.Delete(filePath);

            if((switches & eSwitches.PrintTimeUsedPerFile) != 0) Console.WriteLine("Time elapsed for file: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");

            if((switches & eSwitches.PauseWhenFileDone) != 0) {
                Console.WriteLine("Press any key to continue");
                Console.ReadKey();
            }
            if((switches & eSwitches.AddNewLine) != 0) Console.WriteLine();
        }

        private static eLogType ToLogFormatSwitch(eSwitches sw) {
            switch(sw) {
                case eSwitches.ShortOutput:
                    return 0;
                case eSwitches.ListCodecs:
                    return 0;
                case eSwitches.OldXmlFormat:
                    return 0;
                case eSwitches.CreqCmlFormat:
                    return eLogType.AVDump;
                case eSwitches.NoDataOutput:
                    return 0;
            }
            return 0;
        }

        private static void DisplayHashBuffer(Hasher hasher) {
            Console.WriteLine("*: Buffersize available for hashalgorithm blocksize: " + blockSize + "  kb blockCount: " + blockCount);

            DateTime startTime = DateTime.Now;
            int cursorPos = Console.CursorTop;
            long fileSize = hasher.Buffer.BaseStream.Length;
            Average[] mean = new Average[hasher.Hashes.Count];
            double bufferSize = 0; int charCount = 0; long bytesProcessed = 0; int timeElapsed;

            for(int i = 0;i < mean.Length;i++) mean[i] = new Average();
            bool doLoop = !hasher.IsDone();
            while(doLoop) {
                doLoop = !hasher.IsDone();

                Console.CursorTop = cursorPos;
                Console.CursorLeft = 0;
                bytesProcessed = 0;
                foreach(Hash hash in hasher.Hashes) {
                    mean[hash.ReaderID].Add(hasher.Buffer.Count(hash.ReaderID));
                    bufferSize = mean[hash.ReaderID].Calc(5);
                    if(bytesProcessed > hash.BytesProcessed || bytesProcessed == 0) bytesProcessed = hash.BytesProcessed;

                    charCount = bufferSize != 0 ? (int)(bufferSize / (double)(blockCount - 1) * 68) : 0;
                    charCount = hash.IsDone() ? 0 : charCount;

                    Console.Write(hash.Name);
                    Console.CursorLeft = 8;
                    Console.WriteLine("[" + "".PadLeft(charCount, '*') + "".PadRight(68 - charCount, ' ') + "]");
                }

                Thread.Sleep(100);

                timeElapsed = (int)(DateTime.Now - startTime).TotalMilliseconds;
                Console.Write("\n" +
                  "Position: " + ((double)bytesProcessed / (1 << 20)).ToString("0 MB") + "/" + ((double)fileSize / (1 << 20)).ToString("0 MB") + "  " +
                  "Elapsed time: " + (timeElapsed / 1000d).ToString("0.0").PadLeft(3) + "s  " +
                  "Speed: " + (((double)bytesProcessed / (1 << 20)) / (timeElapsed / 1000d)).ToString("0.00 MB/s") +
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
                    } else if(parts[0] == "exp") {
                        try {
                            ed2kListStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
                        } catch(Exception) { invalidCl = true; }
                    } else if(parts[0] == "ext") {
                        processExtensions = parts[1];
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
   ac      autocreq: '-ac:<username>:<api password>' YOU SHOULD USE THIS (not implemented)
   ms      monitor sleep duration in milliseconds, default is " + monitorSleepDuration.ToString() + @"
   exp     export Ed2k-links to file
   ext     comma separated extension list
   log     write output to file
   port    udp Port used by ac (not implemented)
   done	   save processed-file-paths to file and exclude existing from proc
   tout    timeout: '-tout:<seconds>:<number Of retries>' (not implemented)
   bsize   buffer for hashing: '-bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>'
   host    host name of anidb udp api server, default is '" + host + @"' (not implemented)
  output:   (exclusive)
   s       short (creq friendly) (not implemented)
   l       list (codecs) (not implemented)
   x       Xml old format (not implemented)
   y       XML creq format
   N       no data output.
  control:
   c       do _not_ recurse Into subfolders
   i       skip full parsing (only for Testing) (not implemented)
   m       Monitor folder(s)
   n       Add extra newline after each file
   p       pause when done (hold cmd window)
   t       print Time used for each file
   T       print total elapsed time
   q       pause after each file
   r       random file Order (not implemented)
   z       delete files after parsing
   o       wait for Response when sending dumps (not implemented)
   v       use \n instead of \r @ progress (not implemented)
   w       supress progress (silent)
  hash:
   0       CRC32 (cyclic Redundancy check)
   1       ED2K  (edonkey2000 hash)
   2       MD5   (message-digest algorithm 5)
   3       SHA1  (secure hash algorithm 1)
   5       TTH   (tiger tree Hash) (slow!)
   6       AICH  (advanced Intelligent corruption Handler) (not implemented)
   9       mp3hash w/o metadata + foosic fingerprint (test) (not implemented)
   a       all (available) hash algorithms
   h       hash mode (no a/v parsing)
   e       print ed2k link
   d       Print anidb link
   g       goto anidb link
   u       print elapsed time after hashing

press any key to exit";
        #endregion
    }

    [FlagsAttribute]
    enum eSwitches : long {
        None = 0L,

        //Output
        ShortOutput = 1L << 0,
        ListCodecs = 1L << 1,
        OldXmlFormat = 1L << 2,
        CreqCmlFormat = 1L << 3,
        NoDataOutput = 1L << 4,

        //Control
        ExcludeSubFolders = 1L << 16,
        SkipFullParsing = 1L << 17,
        MonitorFolder = 1L << 18,
        AddNewLine = 1L << 19,
        PauseWhenDone = 1L << 20,
        PauseWhenFileDone = 1L << 21,
        RandomFileOrder = 1L << 22,
        PrintTimeUsedPerFile = 1L << 23,
        DeleteFileWhenDone = 1L << 24,
        WaitForDumpReply = 1L << 25,
        UseLfForProgress = 1L << 26,
        SupressProgress = 1L << 27,
        PrintTotalTimeUsed = 1L << 28,

        //Hash
        Crc32 = 1L << 40,
        Ed2k = 1L << 41,
        Md5 = 1L << 42,
        Sha1 = 1L << 43,
        Tth = 1L << 44,
        Aich = 1L << 45,
        Mp3Hash = 1L << 46,
        UseAllHashes = 1L << 47,
        HashingOnly = 1L << 48,
        PrintEd2kLink = 1L << 49, //Useful?
        PrintAniDBLink = 1L << 50, //Useful?
        GotoAniDBLink = 1L << 51,
        PrintElapsedHashingTime = 1L << 52 //depreciated?
    }
}

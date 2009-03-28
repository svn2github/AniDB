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
using System.Text;
using AVDump2Lib.Hashes;
using System.IO;
using System.Threading;
using System.Diagnostics;
using AVDump2Lib.MediaInfoLib;

namespace AVDumpCL {
    class CL {
        #region Fields
        static Dictionary<char, eSwitches> char2SwitchEnum;
        static eSwitches switches;
        static string username;
        static string password;
        static string host = "";
        static int port = 9001;

        static Queue<string> media;
        static Stream logStream;
        static Stream ed2kListStream;
        static Stream doneListStream;
        static string[] doneListContent;

        static string dumpableExtensions = "avi,mkv,ogm,mp4,mov,mpg,rm";
        static string processExtensions = "mkv,avi,mpg,mpeg,vob,wmv,asf,qt,mov,rm,ogm,mp4";
        static int retries = 3;
        static int timeout = 10;
        static int blockCount = 16;
        static int blockSize = 2048;
        static int monitorSleepDuration = 60000;
        #endregion

        static CL() {
            media = new Queue<string>();

            char2SwitchEnum = new Dictionary<char, eSwitches>();
            char2SwitchEnum['s'] = eSwitches.ShortOutput;
            char2SwitchEnum['l'] = eSwitches.ListCodecs;
            char2SwitchEnum['x'] = eSwitches.OldXmlFormat;
            char2SwitchEnum['y'] = eSwitches.CreqCmlFormat;
            char2SwitchEnum['N'] = eSwitches.NoDataOutput;

            char2SwitchEnum['c'] = eSwitches.IncludeSubFolders;
            char2SwitchEnum['i'] = eSwitches.SkipFullParsing;
            char2SwitchEnum['m'] = eSwitches.MonitorFolder;
            char2SwitchEnum['n'] = eSwitches.AddNewLine;
            char2SwitchEnum['p'] = eSwitches.PauseWhenDone; //Done
            char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone;
            char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
            char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile;
            char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone;
            char2SwitchEnum['o'] = eSwitches.WaitForDumpReply;
            char2SwitchEnum['v'] = eSwitches.UseLfForProgress;
            char2SwitchEnum['w'] = eSwitches.SupressProgress;
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
            char2SwitchEnum['e'] = eSwitches.PrintEd2kLink;
            char2SwitchEnum['d'] = eSwitches.PrintAniDBLink;
            char2SwitchEnum['g'] = eSwitches.GotoAniDBLink;
            char2SwitchEnum['u'] = eSwitches.PrintElapsedHashingTime;
        }

        static void Main(string[] args) {
            if(!ParseClOptions(args)) return;
            Console.CursorVisible = false;

            DateTime startTime = DateTime.Now;
            ProcessMediaFile(media.Dequeue());

            if((switches & eSwitches.MonitorFolder) != 0) MonitorFiles();

            if((switches & eSwitches.PrintTotalTimeUsed) != 0) Console.WriteLine("Total time elapsed: " + (DateTime.Now - startTime).TotalMilliseconds.ToString());
            if((switches & eSwitches.PauseWhenDone) != 0) Console.ReadKey();
        }

        private static void MonitorFiles() {
            while(true) Thread.Sleep(1000);
        }

        private static void ProcessMediaFile(string filePath) {
            Stream stream = System.IO.File.OpenRead(filePath);

            DateTime startTime = DateTime.Now;
            Hasher hasher = new Hasher(stream, blockCount, blockSize * 1024);
            if((switches & (eSwitches.Aich | eSwitches.Crc32 | eSwitches.Ed2k | eSwitches.Md5 | eSwitches.Mp3Hash | eSwitches.Sha1 | eSwitches.Tth | eSwitches.UseAllHashes)) != 0) {
                Console.WriteLine("Hashing: " + System.IO.Path.GetFileName(filePath));

                if((switches & (eSwitches.Crc32 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Crc32(), "crc");
                if((switches & (eSwitches.Ed2k | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Ed2k(), "ed2k");
                if((switches & (eSwitches.Tth | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Tiger(), "tth");
                if((switches & (eSwitches.Md5 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.MD5CryptoServiceProvider(), "md5");
                if((switches & (eSwitches.Sha1 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "sha1");
                hasher.Start();
                DisplayHashBuffer(hasher);
            }
            if((switches & eSwitches.PrintElapsedHashingTime) != 0) Console.WriteLine("Time elapsed after Hashing: " + (DateTime.Now - startTime).TotalMilliseconds.ToString() + "ms");

            if((switches & eSwitches.HashingOnly) == 0) {
                System.Xml.XmlDocument xmlDoc = (System.Xml.XmlDocument)Info.CreateInfoLog(filePath, eLogType.AVDump, hasher.HashStringDict, stream.Length);
                if(logStream != null) xmlDoc.Save(logStream);
            } else {
                //TODO
            }
        }

        private static void DisplayHashBuffer(Hasher hasher) {
            Console.WriteLine("*: Buffersize available for Hashalgorithm");

            DateTime startTime = DateTime.Now;
            int[] mean = new int[hasher.Hashes.Count];
            long fileSize = hasher.Buffer.BaseStream.Length;
            int bufferSize = 0; int charCount = 0; long bytesProcessed = 0; int timeElapsed;
            int cursorPos = Console.CursorTop;

            while(!hasher.IsDone()) {
                Console.CursorTop = cursorPos;
                Console.CursorLeft = 0;
                bytesProcessed = 0;
                foreach(Hash hash in hasher.Hashes) {
                    bufferSize = hasher.Buffer.Count(hash.ReaderID);
                    //bufferSize = mean[hash.ReaderID] = (int)(((double)(mean[hash.ReaderID] * 1 + bufferSize)) / (double)2);
                    if(bytesProcessed > hash.BytesProcessed || bytesProcessed == 0) bytesProcessed = hash.BytesProcessed;

                    charCount = bufferSize != 0 ? (int)((double)bufferSize / (double)blockCount * 68) : 0;
                    charCount = hash.IsDone() ? 0 : charCount;

                    Console.Write(hash.Name);
                    Console.CursorLeft = 8;
                    Console.WriteLine("[" + "".PadLeft(charCount, '*') + "".PadRight(68 - charCount, ' ') + "]");
                }

                Thread.Sleep(100);

                timeElapsed = (int)(DateTime.Now - startTime).TotalMilliseconds;
                Console.Write("\n" +
                  "Position: " + ((double)bytesProcessed / (1 << 20)).ToString("# MB") + "/" + ((double)fileSize / (1 << 20)).ToString("# MB") + "  " +
                  "Elapsed time: " + (timeElapsed / 1000).ToString().PadLeft(3) + "s  " +
                  "Speed: " + (((double)fileSize / (1 << 20)) / (timeElapsed / 1000d)).ToString("#.00 MB/s") +
                  "".PadLeft(80 - Console.CursorLeft)
                );
            }
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
                    media.Enqueue(args[i]);
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
                            //ed2kListPath = parts[1];
                        } catch(Exception) { invalidCl = true; }
                    } else if(parts[0] == "ext") {
                        processExtensions = parts[1];
                    } else if(parts[0] == "log") {
                        try {
                            logStream = System.IO.File.Open(parts[1], FileMode.Append, FileAccess.Write);
                            //logPath = parts[1];
                        } catch(Exception) { invalidCl = true; }
                    } else if(parts[0] == "port") {
                        if(!int.TryParse(parts[1], out port)) invalidCl = true;
                    } else if(parts[0] == "done") {
                        try {
                            doneListStream = System.IO.File.Open(parts[1], FileMode.OpenOrCreate, FileAccess.ReadWrite);
                            StreamReader sr = new StreamReader(doneListStream);
                            doneListContent = sr.ReadToEnd().Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
                            sr.Dispose();
                        } catch(Exception) { invalidCl = true; }

                        //doneListPath = parts[1];
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
                    foreach(char sw in parts[0]) {
                        if(char2SwitchEnum.ContainsKey(sw)) {
                            switches |= char2SwitchEnum[sw];
                        } else {
                            invalidCl = true;
                        }
                    }
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
   ms      monitor sleep duration in milliseconds, default is " + monitorSleepDuration.ToString() + @" (not implemented)
   exp     export Ed2k-links to file (not implemented)
   ext     comma separated extension list (not implemented)
   log     write output to file
   port    udp Port used by ac (not implemented)
   done	   save processed-file-paths to file and exclude existing from proc (not implemented)
   tout    timeout: '-tout:<seconds>:<number Of retries>' (not implemented)
   bsize   buffer for hashing: '-bsize:<size in kb (" + blockSize.ToString() + @")>:<num of bufs (" + blockCount.ToString() + @")>'
   host    host name of anidb udp api server, default is '" + host + @"' (not implemented)
  output:   (exclusive)
   s       short (creq friendly) (not implemented)
   l       list (codecs) (not implemented)
   x       Xml old format (not implemented)
   y       XML creq format (not implemented)
   N       no data output.  (not implemented)
  control:
   c       do _not_ recurse Into subfolders (not implemented)
   i       skip full parsing (only for Testing) (not implemented)
   m       Monitor folder(s) (not implemented)
   n       Add extra newline after each file (not implemented)
   p       pause when done (hold cmd window) (not implemented)
   t       print Time used for each file  (not implemented)
   T       print total elapsed time
   q       pause after each file (not implemented)
   r       random file Order (not implemented)
   z       delete files after parsing (not implemented)
   o       wait for Response when sending dumps (not implemented)
   v       use \n instead of \r @ progress (not implemented)
   w       supress progress (silent) (not implemented)
  hash:
   0       CRC32 (cyclic Redundancy check)
   1       ED2K  (edonkey2000 hash)
   2       MD5   (message-digest algorithm 5)
   3       SHA1  (secure hash algorithm 1)
   5       TTH   (tiger tree Hash)
   6       AICH  (advanced Intelligent corruption Handler) (not implemented)
   9       mp3hash w/o metadata + foosic fingerprint (test) (not implemented)
   a       all (available) hash algorithms
   h       hash mode (no a/v parsing)
   e       print ed2k link (not implemented)
   d       Print anidb link (not implemented)
   g       print and goto anidb link (not implemented)
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
        IncludeSubFolders = 1L << 16,
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
        PrintEd2kLink = 1L << 49,
        PrintAniDBLink = 1L << 50,
        GotoAniDBLink = 1L << 51,
        PrintElapsedHashingTime = 1L << 52 //depreciated?
    }
}

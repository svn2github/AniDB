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

        static List<string> media;
        static Stream logStream;
        static Stream ed2kListStream;
        static Stream doneListStream;
        static string[] doneListContent;

        static string processExtensions = "mkv,avi,mpg,mpeg,vob,wmv,asf,qt,mov,ogm,mp4";
        static int retries = 3;
        static int timeout = 10;
        static int blockCount = 64;
        static int blockSize = 1024;
        static int monitorSleepDuration = 60000;
        #endregion

        static CL() {
            media = new List<string>();

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
            char2SwitchEnum['p'] = eSwitches.PauseWhenDone;
            char2SwitchEnum['q'] = eSwitches.PauseWhenFileDone;
            char2SwitchEnum['r'] = eSwitches.RandomFileOrder;
            char2SwitchEnum['t'] = eSwitches.PrintTimeUsedPerFile;
            char2SwitchEnum['z'] = eSwitches.DeleteFileWhenDone;
            char2SwitchEnum['o'] = eSwitches.WaitForDumpReply;
            char2SwitchEnum['v'] = eSwitches.UseLfForProgress;
            char2SwitchEnum['w'] = eSwitches.SupressProgress;

            char2SwitchEnum['0'] = eSwitches.Crc32;
            char2SwitchEnum['1'] = eSwitches.Ed2k;
            char2SwitchEnum['2'] = eSwitches.Md5;
            char2SwitchEnum['3'] = eSwitches.Sha1;
            char2SwitchEnum['5'] = eSwitches.Tth;
            char2SwitchEnum['6'] = eSwitches.Aich;
            char2SwitchEnum['9'] = eSwitches.Mp3Hash;
            char2SwitchEnum['a'] = eSwitches.UseAllHashes;
            char2SwitchEnum['h'] = eSwitches.HashingOnly;
            char2SwitchEnum['e'] = eSwitches.PrintEd2kLink;
            char2SwitchEnum['d'] = eSwitches.PrintAniDBLink;
            char2SwitchEnum['g'] = eSwitches.GotoAniDBLink;
            char2SwitchEnum['u'] = eSwitches.PrintTotalTimeUsed;
        }

        static void Main(string[] args) {
            if(!ParseClOptions(args)) return;

            ProcessMedia();
        }

        private static void ProcessMedia() {
            Stream stream = System.IO.File.OpenRead(media[0]);

            if((switches & (eSwitches.Aich | eSwitches.Crc32 | eSwitches.Ed2k | eSwitches.Md5 | eSwitches.Mp3Hash | eSwitches.Sha1 | eSwitches.Tth | eSwitches.UseAllHashes)) != 0) {
                Hasher hasher = new Hasher(stream, blockCount, blockSize * 1024);
                if((switches & (eSwitches.Crc32 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Crc32(), "crc");
                if((switches & (eSwitches.Ed2k | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Ed2k(), "ed2k");
                if((switches & (eSwitches.Tth | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new Tiger(), "tth");
                if((switches & (eSwitches.Md5 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.MD5CryptoServiceProvider(), "md5");
                if((switches & (eSwitches.Sha1 | eSwitches.UseAllHashes)) != 0) hasher.AddHash(new System.Security.Cryptography.SHA1CryptoServiceProvider(), "sha1");
                hasher.Start();
                DisplayHashBuffer(hasher);
            }

            if((switches & eSwitches.HashingOnly) == 0) {
                System.Xml.XmlDocument xmlDoc = (System.Xml.XmlDocument)Info.CreateInfoLog(media[0], eLogType.AVDump, hasher.HashStringDict, stream.Length);
                if(logStream != null) xmlDoc.Save(logStream);
            } else {
                //TODO
            }
        }

        private static void DisplayHashBuffer(Hasher hasher) {
            Console.Clear();
            int bufferSize = 0;
            int charCount = 0;
            int[] mean = new int[hasher.Hashes.Count];
            while(!hasher.IsDone()) {
                Console.CursorLeft = 0;
                Console.CursorTop = 0;
                foreach(Hash hash in hasher.Hashes) {
                    bufferSize = hasher.Buffer.Count(hash.ReaderID);
                    bufferSize = mean[hash.ReaderID] = (int)(((double)(mean[hash.ReaderID] * 4 + bufferSize)) / (double)5);

                    charCount = bufferSize != 0 ? (int)((double)bufferSize / (double)blockCount * 68) : 0;
                    charCount = hash.IsDone() ? 0 : charCount;

                    Console.Write(hash.Name);
                    Console.CursorLeft = 8;
                    Console.WriteLine("[" + "".PadLeft(charCount, '*') + "".PadRight(68 - charCount, ' ') + "]");
                }
                Debug.Print(bufferSize.ToString());
                Thread.Sleep(100);
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
                    media.Add(args[i]);
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
   t       print Time used for each file (not implemented)
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
   u       print time used (not implemented)

press any key to exit";
        #endregion
    }

    [FlagsAttribute]
    enum eSwitches : long {
        None = 0,

        //Output
        ShortOutput = 1 << 0,
        ListCodecs = 1 << 1,
        OldXmlFormat = 1 << 2,
        CreqCmlFormat = 1 << 3,
        NoDataOutput = 1 << 4,

        //Control
        IncludeSubFolders = 1 << 16,
        SkipFullParsing = 1 << 17,
        MonitorFolder = 1 << 18,
        AddNewLine = 1 << 19,
        PauseWhenDone = 1 << 20,
        PauseWhenFileDone = 1 << 21,
        RandomFileOrder = 1 << 22,
        PrintTimeUsedPerFile = 1 << 23,
        DeleteFileWhenDone = 1 << 24,
        WaitForDumpReply = 1 << 25,
        UseLfForProgress = 1 << 26,
        SupressProgress = 1 << 27,

        //Hash
        Crc32 = 1 << 40,
        Ed2k = 1 << 41,
        Md5 = 1 << 42,
        Sha1 = 1 << 43,
        Tth = 1 << 44,
        Aich = 1 << 45,
        Mp3Hash = 1 << 46,
        UseAllHashes = 1 << 47,
        HashingOnly = 1 << 48,
        PrintEd2kLink = 1 << 49,
        PrintAniDBLink = 1 << 50,
        GotoAniDBLink = 1 << 51,
        PrintTotalTimeUsed = 1 << 52
    }
}

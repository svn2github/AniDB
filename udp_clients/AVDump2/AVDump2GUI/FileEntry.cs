using System;
using System.Collections.Generic;
using System.Text;

namespace AVDump2GUI {
    public class FileEntry {
        private FilePath filePath;
        private Dictionary<string, byte[]> hashes;
        private long size;
        private bool served;

        public FilePath FilePath { get { return filePath; } set { filePath = value; } }
        public Dictionary<string, byte[]> Hashes { get { return hashes; } set { hashes = value; } }
        public long Size { get { return size; } set { size = value; } }
        public bool Served { get { return served; } set { served = value; } }

        public static string ByteHash2String(byte[] hash, int numberBase) {
            return null;
        }
    }
}

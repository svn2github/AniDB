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
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;

namespace AVDump2Lib.Hashes {
    public class Hash {
        private int readerID;

        private string name;
        private string hashString;
        private long bytesProcessed;

        private HashAlgorithm hashAlgorithm;
        private RefillBuffer b;
        private Thread t;

        public Hash(RefillBuffer b, int readerID, HashAlgorithm hashAlgorithm, string name) {
            this.hashAlgorithm = hashAlgorithm;
            this.readerID = readerID;
            this.name = name;
            this.b = b;
            t = new Thread(new ThreadStart(DoWork));
        }

        public string Name { get { return name; } }
        public string HashString { get { return hashString; } }
        public int ReaderID { get { return readerID; } }

        public void Start() { t.Start(); }
        public void Join() { t.Join(); }
        public bool IsDone() { return hashString != null; }
        public long BytesProcessed { get { return bytesProcessed; } }

        private void DoWork() {
            byte[] block = null;
            bool isLastBlock = false;
            int numRead = 0;

            numRead = b.Read(readerID, out block, out isLastBlock);
            while(!isLastBlock) {
                hashAlgorithm.TransformBlock(block, 0, numRead, block, 0);
                while(!b.CanRead(readerID)) Thread.Sleep(50);
                bytesProcessed += numRead;
                b.Read(readerID, out block, out isLastBlock);
            }
            hashAlgorithm.TransformFinalBlock(block, 0, numRead);
            bytesProcessed += numRead;

            hashString = "";
            for(int i = 0;i < hashAlgorithm.Hash.Length;i++) {
                hashString += hashAlgorithm.Hash[i].ToString("X");
            }

            if(hashAlgorithm is Ed2k) { //Handle Ed2k screwup
                Ed2k ed2k = (Ed2k)hashAlgorithm;
                if(!ed2k.BlueIsRed()) {
                    hashString += ";";
                    for(int i = 0;i < ed2k.BlueHash.Length;i++) {
                        hashString += ed2k.BlueHash[i].ToString("X");
                    }
                }
            }

        }
    }
}

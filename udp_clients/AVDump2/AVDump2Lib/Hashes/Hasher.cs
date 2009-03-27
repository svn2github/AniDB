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
using AVDump2Lib.BlockBuffer;
using System.Security.Cryptography;
using System.Threading;
using System.IO;

namespace AVDump2Lib.Hashes {
    public class Hasher {
        private Dictionary<string, string> hashStringDict;

        private List<Hash> hashes;

        private RefillBuffer b;


        public Hasher(Stream s, int blockCount, int blockSize) {
            b = new RefillBuffer(s, blockCount, blockSize);
            hashStringDict = new Dictionary<string, string>();
            hashes = new List<Hash>();
        }


        public Dictionary<string, string> HashStringDict { get { return hashStringDict; } }
        public void AddHash(HashAlgorithm hash, string name) {
            hash.Initialize();
            hashes.Add(new Hash(b, hashes.Count, hash, name));
        }
        public void Join() {
            for(int i = 0;i < hashes.Count;i++) {
                hashes[i].Join();
                hashStringDict[hashes[i].Name] = hashes[i].HashString;
            }

        }
        public void Start() {
            b.Start(hashes.Count);

            for(int i = 0;i < hashes.Count;i++) {
                hashes[i].Start();
            }
        }
        public bool IsDone() {
            for(int i = 0;i < hashes.Count;i++) {
                if(!hashes[i].IsDone()) return false;
            }
            return true;
        }
        public List<Hash> Hashes { get { return hashes; } }
        public RefillBuffer Buffer { get { return b; } }

    }
}

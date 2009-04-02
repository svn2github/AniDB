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

using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using AVDump2Lib.BlockBuffer;

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
        public void AddHash(HashAlgorithm hashAlgorithm, string name) {
            hashAlgorithm.Initialize();
            Hash hash = new Hash(b, hashes.Count, hashAlgorithm, name);
            hash.HashDone += (s, e) => { hashStringDict[((Hash)s).Name] = ((Hash)s).HashString; };
            hashes.Add(hash);
        }
        public List<Hash> Hashes { get { return hashes; } }
        public RefillBuffer Buffer { get { return b; } }

        public void Join() { for(int i = 0;i < hashes.Count;i++) hashes[i].Join(); }
        public void Start() {
            b.Start(hashes.Count);
            for(int i = 0;i < hashes.Count;i++) hashes[i].Start();
        }
        public bool IsDone() {
            for(int i = 0;i < hashes.Count;i++) if(!hashes[i].IsDone()) return false;
            return true;
        }
    }
}

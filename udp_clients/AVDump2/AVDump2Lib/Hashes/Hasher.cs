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
using System;

namespace AVDump2Lib.Hashes {
	public class Hasher {
		private Dictionary<string, byte[]> hashes;
		private List<HashContainer> hashAlgorithms;
		private IRefillBuffer<byte[]> b;

		public Hasher() {
			hashes = new Dictionary<string, byte[]>();
			hashAlgorithms = new List<HashContainer>();
		}

		public Dictionary<string, byte[]> Hashes { get { return hashes; } }
		public void AddHash(HashAlgorithm hashAlgorithm, string name) {
			hashAlgorithm.Initialize();
			HashContainer hash = new HashContainer(hashAlgorithms.Count, hashAlgorithm, name);
			hash.HashDone += (s, e) => { hashes[((HashContainer)s).Name] = ((HashContainer)s).Hash; };
			hashAlgorithms.Add(hash);
		}
		public List<HashContainer> HashAlgorithms { get { return hashAlgorithms; } }
		public IRefillBuffer<byte[]> Buffer { get { return b; } }

		public void Join() { for(int i = 0;i < hashAlgorithms.Count;i++) hashAlgorithms[i].Join(); }

		public void Start(int blockCount, RefillBuffer<byte[]>.IBlockSource blockSource) {
			ICircularBuffer<byte[]> circb = new CircularBuffer<byte[]>(hashAlgorithms.Count, (int)Math.Log(blockCount, 2));
			b = new RefillBuffer<byte[]>(circb, blockSource);
			b.Start();
			for(int i = 0;i < hashAlgorithms.Count;i++) hashAlgorithms[i].Start(b);
		}
		public bool IsDone() {
			for(int i = 0;i < hashAlgorithms.Count;i++) if(!hashAlgorithms[i].IsDone()) return false;
			return true;
		}
	}
}

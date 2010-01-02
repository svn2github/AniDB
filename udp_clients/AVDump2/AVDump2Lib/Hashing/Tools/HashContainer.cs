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
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;
using System.Collections;
using System.Collections.Generic;
using System.IO;

namespace AVDump2Lib.Hashing.Tools {
	public class HashContainer {
		private List<HashExecute> hashExecutes;
		private IRefillBuffer<byte[]> b;

		public HashContainer() {
			hashExecutes = new List<HashExecute>();
		}

		public int AddHashAlgorithm(HashAlgorithm hashAlgorithm, string name) {
			hashAlgorithm.Initialize();
			HashExecute hashExecute = new HashExecute(hashAlgorithm, hashExecutes.Count, name);
			hashExecutes.Add(hashExecute);
			return hashExecutes.Count - 1;
		}

		public HashingProgress Start(int blockCount, Stream source, int blockSize) {
			ByteStreamToBlock blockSource = new ByteStreamToBlock(source, blockSize);
			ICircularBuffer<byte[]> circb = new CircularBuffer<byte[]>(hashExecutes.Count, (int)Math.Log(blockCount, 2));
			b = new RefillBuffer<byte[]>(circb, blockSource);
			b.Start();

			for(int i = 0;i < hashExecutes.Count;i++) hashExecutes[i].Start(b);

			return new HashingProgress(this);
		}
		public List<HashExecute> Join() {
			for(int i = 0;i < hashExecutes.Count;i++) hashExecutes[i].Join();
			return hashExecutes;
		}

		public bool HasFinished {
			get {
				for(int i = 0;i < hashExecutes.Count;i++) if(!hashExecutes[i].HasFinished) return false;
				return true;
			}
		}

		public class HashingProgress {
			private HashContainer hashContainer;
			private Stream source;
			private DateTime startTime;

			public HashingProgress(HashContainer hashContainer) {
				startTime = DateTime.Now;

				this.hashContainer = hashContainer;
				source = ((ByteStreamToBlock)hashContainer.b.BlockSource).Source;
			}

			public long StreamSize { get { return source.Length; } }
			public long StreamPosition { get { return source.Position; } }
			public long ProcessedBytes(int index) { return hashContainer.hashExecutes[index].ProcessedBytes; }
			public int BufferLength { get { return (int)hashContainer.b.BaseStream.BlockCount; } }
			public int BlockSize { get { return ((ByteStreamToBlock)hashContainer.b.BlockSource).BlockSize; } }
			public ulong BlockCount(int hashObjId) { return hashContainer.b.Count(hashObjId); }

			public string HashObjName(int index) { return hashContainer.hashExecutes[index].Name; }

			public int HashObjCount { get { return hashContainer.hashExecutes.Count; } }

			public TimeSpan TimeElapsed { get { return DateTime.Now - startTime; } }

			/*public void Join() { hashContainer.Join(); }*/
			public bool HasFinished { get { return hashContainer.HasFinished; } }
		}
	}
}

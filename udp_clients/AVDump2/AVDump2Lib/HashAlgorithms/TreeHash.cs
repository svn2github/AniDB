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
using System.Linq;
using System.Text;
using System.Security.Cryptography;
using System.Threading;
using System.Diagnostics;
using System.Collections;

namespace AVDump2Lib.HashAlgorithms {
	public class TreeHash : HashAlgorithm {
		private static byte[] emptyArray = new byte[0];
		private static byte[] zeroArray = new byte[] { 0 };
		private static byte[] oneArray = new byte[] { 1 };

		private HashAlgorithm nodeHasher, blockHasher;
		private readonly int BLOCKSIZE;
		private int blockLengthTodo;
		private Queue<byte[]> blocks;
		private AutoResetEvent newBlocks, waitingForBlocks;
		private Thread compressor;

		private List<int> level;
		private List<byte[]> nods;

		private bool eof;

		public TreeHash(HashAlgorithm nodeHasher, HashAlgorithm blockHasher, int blockSize) {
			this.nodeHasher = nodeHasher;
			this.blockHasher = blockHasher;
			this.BLOCKSIZE = blockSize;

			this.blocks = new Queue<byte[]>();
			this.nods = new List<byte[]>();
			this.level = new List<int>();

			newBlocks = new AutoResetEvent(false);
			waitingForBlocks = new AutoResetEvent(false);
		}

		protected override void HashCore(byte[] array, int ibStart, int cbSize) {
			if(blockLengthTodo > cbSize) {
				blockHasher.TransformBlock(array, ibStart, cbSize, null, 0);

				blockLengthTodo -= cbSize;
				ibStart += cbSize;
				cbSize = 0;
				return;

			} else if(blockLengthTodo != 0) {
				blockHasher.TransformFinalBlock(array, ibStart, blockLengthTodo);
				blocks.Enqueue(blockHasher.Hash);
				blockHasher.Initialize();

				ibStart += blockLengthTodo;
				cbSize -= blockLengthTodo;
				blockLengthTodo = 0;
			}

			while(cbSize >= BLOCKSIZE) {
				blockHasher.TransformBlock(zeroArray, 0, 1, null, 0);
				blockHasher.TransformFinalBlock(array, ibStart, BLOCKSIZE);
				blocks.Enqueue(blockHasher.Hash);
				blockHasher.Initialize();


				ibStart += BLOCKSIZE;
				cbSize -= BLOCKSIZE;
			}

			if(cbSize != 0) {
				blockHasher.TransformBlock(zeroArray, 0, 1, null, 0);
				blockHasher.TransformBlock(array, ibStart, cbSize, null, 0);
				blockLengthTodo = BLOCKSIZE - cbSize;
				ibStart += cbSize;
				cbSize = 0;
			}

			CompressBlocks();
		}

		private void CompressBlocks() {
			while(blocks.Count > 1) nods.Add(HashBlocks(blocks.Dequeue(), blocks.Dequeue()));

			for(int i = 0;i < level.Count;i++) {
				while((i != 0 && (level[i - 1] - level[i] > 1)) || (i == 0 && (nods.Count - level[0] > 1))) {
					nods[level[i]] = HashBlocks(nods[level[i]], nods[level[i] + 1]);

					nods.RemoveAt(++level[i]);
					for(int j = i - 1;j >= 0;j--) level[j]--;

					if(level.Count <= i + 1) level.Add(0);
				}
			}


		}

		private byte[] HashBlocks(byte[] l, byte[] r) {
			nodeHasher.Initialize();
			nodeHasher.TransformBlock(oneArray, 0, 1, null, 0);
			if(l != null) nodeHasher.TransformBlock(l, 0, l.Length, null, 0);
			if(r != null) nodeHasher.TransformBlock(r, 0, r.Length, null, 0);
			nodeHasher.TransformFinalBlock(emptyArray, 0, 0);

			return nodeHasher.Hash;
		}

		protected override byte[] HashFinal() {
			if(blockLengthTodo != 0) {
				blockHasher.TransformFinalBlock(emptyArray, 0, 0);
				lock(blocks) blocks.Enqueue(blockHasher.Hash);
				blockHasher.Initialize();
			}

			CompressBlocks();

			nods.AddRange(blocks);

			eof = true;
			return nods.Count != 0 ? nods.Reverse<byte[]>().Aggregate((byte[] accumHash, byte[] hash) => { return HashBlocks(hash, accumHash); }) : blockHasher.ComputeHash(new byte[0]);
		}

		public override void Initialize() {
			if(compressor != null && compressor.IsAlive) {
				eof = true;
				newBlocks.Set();
				compressor.Join();
			}

			nodeHasher.Initialize();
			blockHasher.Initialize();

			this.blocks.Clear();
			this.nods.Clear();
			this.level.Clear();
			level.Add(0);

			newBlocks.Reset();
			waitingForBlocks.Reset();

			eof = false;
			blockLengthTodo = 0;

			compressor = new Thread(CompressBlocks);
			//compressor.Start();
		}
	}
}

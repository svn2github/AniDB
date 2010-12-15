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
	public class TTH : HashAlgorithm {
		public const int BLOCKSIZE = 1024;
		private static byte[] zeroArray = new byte[] { 0 };
		private static byte[] oneArray = new byte[] { 1 };
		private static byte[] emptyArray = new byte[0];

		private Environment[] environments; private AutoResetEvent[] signals;

		private HashAlgorithm nodeHasher, blockHasher;
		private bool hasLastBlock;
		private bool hasStarted;
		private int threadCount;

		private Queue<byte[]> blocks;
		private LinkedList<byte[]> nods;
		private LinkedList<LinkedListNode<byte[]>> levels;
		private byte[] dataBlock;


		public TTH(int threadCount) {
			this.blocks = new Queue<byte[]>();
			this.nods = new LinkedList<byte[]>();
			this.levels = new LinkedList<LinkedListNode<byte[]>>();

			nodeHasher = new TigerThex();
			blockHasher = new TigerThex();

			this.threadCount = threadCount /*= 1*/;

			signals = new AutoResetEvent[threadCount];
			environments = new Environment[threadCount];
			for(int i = 0;i < threadCount;i++) {
				environments[i] = new Environment(i);
				signals[i] = environments[i].WorkDone;
			}
		}

		protected override void HashCore(byte[] array, int ibStart, int cbSize) {
			if(!hasLastBlock && cbSize != 0) throw new Exception();
			if(!hasStarted) { foreach(var e in environments) e.HashThread.Start(e); hasStarted = true; }

			this.dataBlock = array;

			foreach(var e in environments) {
				e.Offset = ibStart;
				e.Length = cbSize;
				e.DoWork.Set();
			}
			WaitHandle.WaitAll(signals);


			for(int i = 0;i < cbSize / 1024;i++) blocks.Enqueue(environments[i % threadCount].Blocks.Dequeue());

			if((cbSize & (BLOCKSIZE - 1)) != 0) {
				ibStart += cbSize - (cbSize & (BLOCKSIZE - 1));
				cbSize &= BLOCKSIZE - 1;

				blockHasher.TransformBlock(zeroArray, 0, 1, null, 0);
				blockHasher.TransformFinalBlock(array, ibStart, cbSize);
				blocks.Enqueue(blockHasher.Hash);
				blockHasher.Initialize();
				hasLastBlock = false;
			}

			CompressBlocks();
		}
		private void DoWork(object obj) {
			var e = (Environment)obj;
			var envBlockSize = BLOCKSIZE * threadCount;
			var envOffset = BLOCKSIZE * e.Index;

			e.DoWork.WaitOne();
			while(!e.ThreadJoin) {
				e.Length -= envOffset;
				e.Offset += envOffset;
				while(e.Length >= BLOCKSIZE) {
					e.Blocks.Enqueue(e.BlockHasher.TTHBlockHash(dataBlock, e.Offset));
					e.Offset += envBlockSize;
					e.Length -= envBlockSize;
				}
				e.WorkDone.Set();
				e.DoWork.WaitOne();
			}
		}
		private void CompressBlocks() {
			if(levels.Last.Value == null && blocks.Count > 1) levels.Last.Value = nods.AddLast(HashBlocks(blocks.Dequeue(), blocks.Dequeue()));
			while(blocks.Count > 1) nods.AddLast(HashBlocks(blocks.Dequeue(), blocks.Dequeue()));

			var level = levels.Last;
			LinkedListNode<LinkedListNode<byte[]>> nextLevel;
			do {

				while(!(level.Value == null || //Level has no nods
				  (level.Value.Next == null) || //Level is at last node position (only one node available)
				  ((nextLevel = GetNextLevel(level)) != null && level.Value.Next == nextLevel.Value))) //Level has only one node
				{
					level.Value.Value = HashBlocks(level.Value.Value, level.Value.Next.Value);
					nods.Remove(level.Value.Next);

					if(level.Previous == null) { //New level Node
						levels.AddFirst(level.Value);
					} else if(level.Previous.Value == null) { //First node in higher level
						level.Previous.Value = level.Value;
					}

					nextLevel = GetNextLevel(level);
					if(level.Value.Next == null || (nextLevel != null && level.Value.Next == nextLevel.Value)) {
						level.Value = null;
					} else {
						level.Value = level.Value.Next;
					}
				}

			} while((level = level.Previous) != null);
		}
		private static LinkedListNode<LinkedListNode<byte[]>> GetNextLevel(LinkedListNode<LinkedListNode<byte[]>> level) {
			var nextLevel = level;
			while((nextLevel = nextLevel.Next) != null) if(nextLevel.Value != null) return nextLevel;
			return null;
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
			foreach(var e in environments) {
				e.ThreadJoin = true;
				e.DoWork.Set();
				e.HashThread.Join();
				e.HashThread = null;
			}

			foreach(var block in blocks) nods.AddLast(block);
			return nods.Count != 0 ? nods.Reverse<byte[]>().Aggregate((byte[] accumHash, byte[] hash) => HashBlocks(hash, accumHash)) : blockHasher.ComputeHash(zeroArray);
		}

		public override void Initialize() {
			nodeHasher.Initialize();
			blockHasher.Initialize();

			this.blocks.Clear();
			this.nods.Clear();
			this.levels.Clear();

			levels.AddFirst((LinkedListNode<byte[]>)null);

			hasStarted = false;
			hasLastBlock = true;

			foreach(var e in environments) {
				e.Blocks.Clear();
				e.DoWork.Reset();
				e.WorkDone.Reset();
				e.ThreadJoin = false;
				e.BlockHasher.Initialize();
				e.HashThread = new Thread(DoWork);
			}
		}

		private class Environment {
			public Thread HashThread;
			public bool ThreadJoin;
			public int Index;


			public TigerForTTH BlockHasher = new TigerForTTH();

			public Queue<byte[]> Blocks;
			public int Offset;
			public int Length;

			public AutoResetEvent WorkDone, DoWork;

			public Environment(int index) {
				DoWork = new AutoResetEvent(false);
				WorkDone = new AutoResetEvent(false);

				this.Index = index;

				Blocks = new Queue<byte[]>();
				BlockHasher.Initialize();
			}
		}
	}
}
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
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;

namespace AVDump2Lib.BlockConsumers.Tools {
	public class BlockConsumerContainer {
		private List<IBlockConsumer> items;
		private IRefillBuffer<byte[]> b;

		public BlockConsumerContainer() { items = new List<IBlockConsumer>(); }

		public int AddBlockConsumer(IBlockConsumer blockConsumer) {
			items.Add(blockConsumer);
			return items.Count - 1;
		}

		public Progress Start(int blockCount, Stream source, int blockSize) {
			ByteStreamToBlock blockSource = new ByteStreamToBlock(source, blockSize);
			ICircularBuffer<byte[]> circb = new CircularBuffer<byte[]>(items.Count, (int)Math.Log(blockCount, 2));
			b = new RefillBuffer<byte[]>(circb, blockSource);
			b.Start();

			for(int i = 0;i < items.Count;i++) items[i].Start(b, i);

			return new Progress(this);
		}
		public List<IBlockConsumer> Join() {
			for(int i = 0;i < items.Count;i++) items[i].Join();
			return items;
		}

		public bool HasFinished {
			get {
				for(int i = 0;i < items.Count;i++) if(!items[i].HasFinished) return false;
				return true;
			}
		}

		public class Progress {
			private BlockConsumerContainer blockConsumers;
			private Stream source;
			private DateTime startTime;

			public Progress(BlockConsumerContainer hashContainer) {
				startTime = DateTime.Now;

				this.blockConsumers = hashContainer;
				source = ((ByteStreamToBlock)hashContainer.b.BlockSource).Source;
			}

			public IBlockConsumer BlockConsumerObj(int index) { return blockConsumers.items[index]; }

			public long StreamSize { get { return source.Length; } }
			public long StreamPosition { get { return source.Position; } }
			public long ProcessedBytes(int index) { return blockConsumers.items[index].ProcessedBytes; }
			public int BufferLength { get { return (int)blockConsumers.b.BaseStream.BlockCount; } }
			public int BlockSize { get { return ((ByteStreamToBlock)blockConsumers.b.BlockSource).BlockSize; } }
			public ulong BlockCount(int hashObjId) { return blockConsumers.b.Count(hashObjId); }
			public ulong BlockCount() { return blockConsumers.b.BaseStream.BlockCount; }

			public string Name(int index) { return blockConsumers.items[index].Name; }

			public int BlockConsumerCount { get { return blockConsumers.items.Count; } }

			public TimeSpan TimeElapsed { get { return DateTime.Now - startTime; } }

			public bool HasFinished { get { return blockConsumers.HasFinished; } }
		}
	}
}

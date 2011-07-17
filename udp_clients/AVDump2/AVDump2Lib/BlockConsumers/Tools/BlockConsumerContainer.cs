using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;
using System.Diagnostics;

namespace AVDump2Lib.BlockConsumers.Tools {
	public class BlockConsumerContainer {
		private Dictionary<string, IBlockConsumer> availableBlockConsumers;

		private List<IBlockConsumer> items;
		private ICircularBuffer<byte[]> circb;
		private IRefillBuffer<byte[]> b;
		private ByteStreamToBlock blockSource;

		public BlockConsumerContainer(int blockCount, int blockSize) {
			availableBlockConsumers = new Dictionary<string, IBlockConsumer>();
			items = new List<IBlockConsumer>();

			blockSource = new ByteStreamToBlock(blockSize);
			circb = new CircularBuffer<byte[]>((int)Math.Log(blockCount, 2));
			b = new RefillBuffer<byte[]>(circb, blockSource);
			
		}

		public void RegisterBlockConsumer(string name, IBlockConsumer blockConsumer) { availableBlockConsumers.Add(name, blockConsumer); }

		public int AddBlockConsumer(string name) {
			items.Add(availableBlockConsumers[name]);
			return items.Count - 1;
		}

		public Progress Start(Stream source) {
			blockSource.Initialize(source);
			b.Initialize(items.Count);
			b.Start();

			for(int i = 0;i < items.Count;i++) items[i].Start(b, i);

			return new Progress(this);
		}
		public List<IBlockConsumer> Join() {
			for(int i = 0;i < items.Count;i++) items[i].Join();
			return items;
		}

		public void Reset() { items.Clear(); }

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
			//public int BufferUndrrunCount { get { return blockConsumers.b.BufferUnderrunCount; } }
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

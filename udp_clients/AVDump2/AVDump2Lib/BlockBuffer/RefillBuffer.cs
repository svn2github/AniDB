using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;
using System;

namespace AVDump2Lib.BlockBuffer {
	public delegate void Consumer<T>(T block);
	public interface IRefillBuffer<T> : IDisposable {

		void Start();

		void Initialize(int consumerCount);

		//int BufferUnderrunCount { get; }

		void Read(int consumerId, Consumer<T> consumer);
		T GetBlock(int consumerId);

		bool CanRead(int consumerId);

		void Advance(int consumerId);

		ICircularBuffer<T> BaseStream { get; }
		IBlockSource<T> BlockSource { get; }

		ulong Count(int consumerId);

		bool EndOfStream();
		bool EndOfStream(int consumerId);

	}

	public class RefillBuffer<T> : IRefillBuffer<T> {
		private ICircularBuffer<T> circBuffer;
		private IBlockSource<T> blockSource;
		private Thread tRefiller;
		private bool isEndOfStream;
		private bool disposed, stop;

		//public int BufferUnderrunCount { get; private set; }

		public RefillBuffer(ICircularBuffer<T> circBuffer, IBlockSource<T> blockSource, int consumerCount) : this(circBuffer, blockSource) { Initialize(consumerCount); }
		public RefillBuffer(ICircularBuffer<T> circBuffer, IBlockSource<T> blockSource) {
			this.circBuffer = circBuffer;
			this.blockSource = blockSource;

			for(int i = 0;i < circBuffer.Buffer.Length;i++) blockSource.InitializeBlock(out circBuffer.Buffer[i]);
		}


		public void Initialize(int consumerCount) {
			blockSource.InitializeBlock(out circBuffer.Buffer[(circBuffer.ProducerPosition - 1) & (circBuffer.BlockCount - 1)]);

			circBuffer.Initialize(consumerCount);
			tRefiller = new Thread(Filler);

			//BufferUnderrunCount = 0;
			isEndOfStream = false;
		}

		public void Start() { tRefiller.Start(); }

		public ulong Count(int consumerId) { return circBuffer.Count(consumerId); }

		public ICircularBuffer<T> BaseStream { get { return circBuffer; } }
		public IBlockSource<T> BlockSource { get { return blockSource; } }

		public T GetBlock(int consumerId) {
			if(!circBuffer.ConsumerCanRead(consumerId)) {
				if(EndOfStream(consumerId)) throw new Exception("Cannot read block when EOS is reached");
				while(!circBuffer.ConsumerCanRead(consumerId)) Thread.Sleep(20);
			};
			return circBuffer.ConsumerGet(consumerId);
		}

		public bool CanRead(int consumerId) { return circBuffer.ConsumerCanRead(consumerId); }

		public void Advance(int consumerId) {
			if(EndOfStream(consumerId)) throw new Exception("Cannot advance when EOS is reached");
			circBuffer.ConsumerAdvance(consumerId); 
		}

		public void Read(int consumerId, Consumer<T> consumer) {
			if(!circBuffer.ConsumerCanRead(consumerId)) {
				if(EndOfStream(consumerId)) throw new Exception("Cannot read block when EOS is reached");
				while(!circBuffer.ConsumerCanRead(consumerId)) Thread.Sleep(20);

			};
			consumer(circBuffer.ConsumerGet(consumerId));
			circBuffer.ConsumerAdvance(consumerId);
		}

		public bool EndOfStream() { return circBuffer.IsEmpty() && isEndOfStream; }
		public bool EndOfStream(int consumerId) { return !circBuffer.ConsumerCanRead(consumerId) && isEndOfStream; }

		private void Filler() {
			while(!blockSource.AllBlocksRead) {
				while(!stop && !circBuffer.ProducerCanWrite()) Thread.Sleep(20);
				if(stop) return;

				circBuffer.ProducerBlock = blockSource.SetNextBlock(circBuffer.ProducerBlock);
				circBuffer.ProducerAdvance();
			}
			isEndOfStream = true;
		}

		public void Stop() { stop = true; tRefiller.Join(); }

		public void Dispose() { Stop(); blockSource.Dispose(); disposed = true; }
	}

	public interface IBlockSource<T> : IDisposable {
		void InitializeBlock(out T block);
		T SetNextBlock(T block);
		bool AllBlocksRead { get; }
	}

	public class ByteStreamToBlock : IBlockSource<byte[]>, IDisposable {
		Stream source;
		bool isEndOfStream;

		int bytesRead;
		int blockSize;

		public ByteStreamToBlock(int blockSize) { this.blockSize = blockSize; }
		public ByteStreamToBlock(Stream source, int blockSize) : this(blockSize) { Initialize(source); }

		public int BlockSize { get { return blockSize; } }
		public Stream Source { get { return source; } }

		public void Initialize(Stream source) {
			this.source = source;
			bytesRead = 0;
			isEndOfStream = false;
		}

		public void InitializeBlock(out byte[] block) { block = new byte[blockSize]; }

		public byte[] SetNextBlock(byte[] block) {
			bytesRead = source.Read(block, 0, blockSize);

			if(source.Position == source.Length) {
				if(blockSize != bytesRead) System.Array.Resize<byte>(ref block, bytesRead);
				isEndOfStream = true;
			}
			return block;
		}

		public bool AllBlocksRead { get { return isEndOfStream; } }

		public void Dispose() { source.Dispose(); }

	}
}

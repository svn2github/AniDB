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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

using System.IO;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;
using System;

namespace AVDump2Lib.BlockBuffer {
	public delegate void Consumer<T>(T block);
	public interface IRefillBuffer<T> : IDisposable {

		void Start();

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

		public RefillBuffer(ICircularBuffer<T> circBuffer, IBlockSource<T> blockSource) {
			this.circBuffer = circBuffer;
			this.blockSource = blockSource;
			tRefiller = new Thread(new ThreadStart(Filler));
			isEndOfStream = false;

			for(int i = 0;i < circBuffer.Buffer.Length;i++) {
				blockSource.InitializeBlock(out circBuffer.Buffer[i]);
			}
		}

		public void Start() { tRefiller.Start(); }

		public ulong Count(int consumerId) { return circBuffer.Count(consumerId); }

		public ICircularBuffer<T> BaseStream { get { return circBuffer; } }
		public IBlockSource<T> BlockSource { get { return blockSource; } }

		public T GetBlock(int consumerId) {
			while(!circBuffer.ConsumerCanRead(consumerId)) Thread.Sleep(20);
			return circBuffer.ConsumerGet(consumerId);
		}

		public bool CanRead(int consumerId) { return circBuffer.ConsumerCanRead(consumerId); }

		public void Advance(int consumerId) { circBuffer.ConsumerAdvance(consumerId); }

		public void Read(int consumerId, Consumer<T> consumer) {
			while(!circBuffer.ConsumerCanRead(consumerId)) Thread.Sleep(20);
			consumer(circBuffer.ConsumerGet(consumerId));
			circBuffer.ConsumerAdvance(consumerId);
		}

		public bool EndOfStream() { return circBuffer.IsEmpty() && isEndOfStream; }
		public bool EndOfStream(int consumerId) { return !circBuffer.ConsumerCanRead(consumerId) && isEndOfStream; }

		private void Filler() {
			while(!blockSource.AllBlocksRead) {
				while(!circBuffer.ProducerCanWrite()) Thread.Sleep(20);

				circBuffer.ProducerBlock = blockSource.SetNextBlock(circBuffer.ProducerBlock);
				circBuffer.ProducerAdvance();
			}
			isEndOfStream = true;
		}

		public void Dispose() { blockSource.Dispose(); }
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

		public ByteStreamToBlock(Stream source, int blockSize) {
			this.source = source;
			this.blockSize = blockSize;
		}

		public int BlockSize { get { return blockSize; } }
		public Stream Source { get { return source; } }

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

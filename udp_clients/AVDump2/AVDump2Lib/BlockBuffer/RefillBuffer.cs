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

namespace AVDump2Lib.BlockBuffer {
	public interface IRefillBuffer<T> {
		void Start();

		T Read(int consumerId, bool block);

		bool EndOfStream();
		bool EndOfStream(int consumerId);
	}

	public class RefillBuffer<T> : IRefillBuffer<T> {
		ICircularBuffer<T> circBuffer;
		IRetrieveBlock<T> blockGetter;
		Thread tRefiller;

		public RefillBuffer(ICircularBuffer<T> circBuffer, IRetrieveBlock<T> blockGetter) {
			this.circBuffer = circBuffer;
			this.blockGetter = blockGetter;
			tRefiller = new Thread(new ThreadStart(Refiller));
		}

		public void Start() { tRefiller.Start(); }

		public T Read(int consumerId, bool block) { return circBuffer.Read(consumerId); }
		public bool CanRead(int consumerId) { return circBuffer.CanRead(consumerId); }

		public bool EndOfStream() { return circBuffer.IsEmpty() && blockGetter.AllBlocksRead(); }
		public bool EndOfStream(int consumerId) { return !circBuffer.CanRead(consumerId) && blockGetter.AllBlocksRead(); }

		private void Refiller() {
			T block = blockGetter.GetNextBlock();
			while(block != null) {
				while(!circBuffer.CanWrite()) Thread.Sleep(50);
				circBuffer.Write(block);
			}
		}

		public interface IRetrieveBlock<U> {
			U GetNextBlock();
			bool AllBlocksRead();
		}
		public class ByteStreamToBlock : IRetrieveBlock<byte[]> {
			byte[] buffer;
			int bytesRead;
			int blockSize;
			bool eos;
			Stream s;

			public ByteStreamToBlock(Stream s, int blockSize) {
				this.s = s;
				this.blockSize = blockSize;
				buffer = new byte[blockSize];
			}

			public byte[] GetNextBlock() {
				bytesRead = s.Read(buffer, 0, buffer.Length);
				if(bytesRead == 0) {
					eos = true;
					return null;
				} else if(buffer.Length != bytesRead) {
					System.Array.Resize<byte>(ref buffer, bytesRead);
				}
				return buffer;
			}

			public bool AllBlocksRead() {
				return eos;
			}
		}
	}

    public class RefillBuffer {
        string filePath;
        Stream stream;
        RingBuffer rb;

        Thread filler;

        int blockCount;
        int blockSize;
        int lastBlockSize;
        bool endOfBuffer;

        public RefillBuffer(Stream stream, int blockCount, int blockSize)
            : this(blockCount, blockSize) {
            this.stream = stream;
        }
        public RefillBuffer(string filePath, int blockCount, int blockSize)
            : this(blockCount, blockSize) {
            this.filePath = filePath;
        }
        private RefillBuffer(int blockCount, int blockSize) {
            filler = new Thread(new ThreadStart(RingFiller));
            this.blockCount = blockCount;
            this.blockSize = blockSize;
            lastBlockSize = -1;
            endOfBuffer = false;
        }

        public void Start(int reader) {
            if(stream == null && filePath != null) stream = System.IO.File.OpenRead(filePath);
            rb = new RingBuffer(blockCount, blockSize, reader);
            filler.Start();
        }

        public int Read(int reader, out byte[] b, out bool isLastBlock) {
            isLastBlock = rb.IsLastBlock(reader);
            b = rb.Read(reader);
            int numRead;
            if(isLastBlock) numRead = lastBlockSize; else numRead = rb.BlockSize;
            return numRead;
        }
        public bool CanRead(int reader) { return rb.CanRead(reader); }

        public int Count(int reader) { return rb.Count(reader); }
        public int BlockCount { get { return blockCount; } }
        public Stream BaseStream { get { return stream; } }

        private void RingFiller() {
            int numRead = lastBlockSize = rb.Write(stream);
            while(numRead != 0) {
                while(!rb.CanWrite()) Thread.Sleep(50);
                lastBlockSize = numRead;
                numRead = rb.Write(stream);
            }
            endOfBuffer = true;
            //stream.Dispose();
        }
    }
}

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

namespace AVDump2Lib.BlockBuffer {
	public interface IRefillBuffer<T> {
		void Start();

		T Read(int consumerId, bool wait);

		ulong Count(int consumerId);

		bool EndOfStream();
		bool EndOfStream(int consumerId);
	}

	public class RefillBuffer<T> : IRefillBuffer<T> {
		ICircularBuffer<T> circBuffer;
		IBlockSource blockSource;
		Thread tRefiller;
		bool isEndOfStream;

		object empty;
		object full;


		public RefillBuffer(ICircularBuffer<T> circBuffer, IBlockSource blockSource) {
			this.circBuffer = circBuffer;
			this.blockSource = blockSource;
			tRefiller = new Thread(new ThreadStart(Filler));
			isEndOfStream = false;

			empty = new object();
			/*for(int i = 0;i < empty.Length;i++) {
				empty[i] = new object();
			}*/
			full = new object();

			for(int i = 0;i < circBuffer.Buffer.Length;i++) {
				blockSource.InitializeBlock(out circBuffer.Buffer[i]);
			}
		}

		public void Start() { tRefiller.Start(); }

		public ulong Count(int consumerId) { return circBuffer.Count(consumerId); }

		public T Read(int consumerId, bool wait) {
			while(wait && !circBuffer.CanRead(consumerId)) Thread.Sleep(20);
			//while(wait && !circBuffer.CanRead(consumerId)) lock(empty) Monitor.Wait(empty,100); //Remove need for timeout

			T block = circBuffer.Read(consumerId);
			//lock(full) Monitor.Pulse(full);
			return block;
		}

		public bool EndOfStream() { return circBuffer.IsEmpty() && isEndOfStream; }
		public bool EndOfStream(int consumerId) { return !circBuffer.CanRead(consumerId) && isEndOfStream; }


		private void Filler() {
			while(!blockSource.AllBlocksRead) {
				while(!circBuffer.CanWrite()) Thread.Sleep(20);
				//while(!circBuffer.CanWrite()) lock(full) Monitor.Wait(full, 100);

				circBuffer.ProducerWrite = blockSource.SetNextBlock(circBuffer.ProducerWrite);
				circBuffer.ProducerAdvance();
				//lock(empty) Monitor.PulseAll(empty);
			}
			isEndOfStream = true;
		}

		public interface IBlockSource {
			void InitializeBlock(out T block);
			T SetNextBlock(T block);
			bool AllBlocksRead { get; }
		}

	}

	public class ByteStreamToBlock : RefillBuffer<byte[]>.IBlockSource {
		Stream source;
		bool isEndOfStream;

		int bytesRead;
		int blockSize;

		public ByteStreamToBlock(Stream source, int blockSize) {
			this.source = source;
			this.blockSize = blockSize;
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

	}
}

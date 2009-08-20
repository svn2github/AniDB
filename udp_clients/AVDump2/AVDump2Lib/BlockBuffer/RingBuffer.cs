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

using System.IO;
using System.Threading;

namespace AVDump2Lib.BlockBuffer {
	public class RingBuffer {
		private byte[][] buffer;
		private int[] readPos;
		private int writePos;

		private int blockCount;
		private int blockSize;
		private int blockMask;
		private int readerCount;
		private bool lastBlockReached;

		public int BlockSize { get { return blockSize; } }
		public int BlockCount { get { return blockCount; } }
		public int BlockMask { get { return blockMask; } }
		public int ReaderCount { get { return readerCount; } }

		public RingBuffer(int blockCount, int blockSize, int reader) {
			this.blockSize = blockSize;
			this.blockCount = blockCount;
			this.readerCount = reader;
			blockMask = blockCount - 1;
			lastBlockReached = false;

			readPos = new int[reader];
			writePos = 0;

			buffer = new byte[blockCount][];
			for(int i = 0;i < blockCount;i++) buffer[i] = new byte[blockSize];
		}

		public bool isEmpty(int reader) {
			if(readPos[reader] == writePos) return true; else return false;
		}
		public int Count(int reader) {
			int len = writePos - readPos[reader];
			if(len < 0) len += blockCount;
			return len;
		}
		public void Clear() {
			writePos = 0;
			readPos = new int[readPos.Length];
		}

		public void Write(byte[] data, bool isLastBlock) {
			buffer[writePos] = data;
			writePos = (writePos + 1) & blockMask;
			lastBlockReached = isLastBlock;
		}
		public int Write(Stream stream) {
			int numRead = stream.Read(buffer[writePos], 0, blockSize);
			if(numRead != 0) {
				if(stream.Position != stream.Length) writePos = (writePos + 1) & blockMask;
			} else {
				lastBlockReached = true;
			}
			return numRead;
		}
		public bool CanWrite() {
			for(int i = 0;i < readerCount;i++) {
				if(Count(i) == blockCount - 1) return false;
			}
			return true;
		}

		public bool IsLastBlock(int reader) {
			return lastBlockReached && Count(reader) == 0;
		}

		public byte[] this[int index, int reader] {
			get { return buffer[(readPos[reader] + index) & blockMask]; }
			set { buffer[(writePos + index) & blockMask] = value; }
		}
		public byte[] Peek(int reader) { return buffer[readPos[reader]]; }
		public byte[] Read(int reader) {
			int pos = readPos[reader];
			readPos[reader] = (readPos[reader] + 1) & blockMask;
			return buffer[pos];
		}
		public void Read(byte[][] buffer, int offset, int len, int reader) { for(int i = 0;i < len;++i) buffer[i] = Read(reader); }
		public bool CanRead(int reader) { return Count(reader) != 0 || (lastBlockReached && Count(reader) == 0); }

		public void Discard(int bytes, int reader) {
			if(Count(reader) < bytes) bytes = Count(reader);
			readPos[reader] = (readPos[reader] + bytes) & blockMask;
		}

		/*public string getString(int pos, int length, int reader) {
			string s = "";
			for(int i = pos;i < pos + length;++i) {
				if(this[i, reader][0] >= 32 && this[i, reader][0] <= 127) s += (char)this[i, reader][0];
			}
			return s.Trim();
		}
		public void displayBuffer(int Length, int reader) {
			int i = 0;
			while(i < Length) {
				Debug.Write(i.ToString("X2") + " : ");
				for(int j = i;j < i + 16;++j) {
					if(j < Length) System.Diagnostics.Debug.Write(this[j, reader][0].ToString("X2")); else Debug.Write("  ");
					Debug.Write(' ');
				}
				Debug.Write(": ");
				for(int j = i;j < i + 16;++j) {
					char c;
					if(j < Length) c = (char)this[j, reader][0]; else c = ' ';
					if(c < ' ' || c > '~') c = '.';
					Debug.Write(c);
				}
				i += 16;
			   Debug.WriteLine(" :");
			}
			Debug.WriteLine("");
		}*/
	}


	/// <summary>
	/// Base interface for a circular buffer with multiple consumers and one producer
	/// </summary>
	/// <typeparam name="T">The datatype to be stored</typeparam>
	public interface ICircularBuffer<T> {
		/// <summary>
		/// The blockcount to be stored (Bufferlength)
		/// </summary>
		int BlockCount { get; }
		/// <summary>
		/// The count of how many consumers are reading from the buffer
		/// </summary>
		int ConsumerCount { get; }

		/// <summary>
		/// Determines if the consumer has a block available to be read
		/// </summary>
		/// <param name="consumerId">The consumer that wants to read a block</param>
		/// <returns>Retruns true if the consumer can read a block</returns>
		bool CanRead(int consumerId);
		/// <summary>
		/// Determines if a block can be written to the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <returns>Returns true if the <see cref="ICircularBuffer"/> has enough space to store the block</returns>
		bool CanWrite();
		/*/// <summary>
		/// Determines if <paramref name="count"/> blocks can be written to the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <param name="count">The number of blocks</param>
		/// <returns>Returns true if the <see cref="ICircularBuffer"/> has enough space to store <paramref name="count"/> blocks</returns>
		bool CanWrite(int count);*/

		/*/// <summary>
		/// Gets the number of blocks actually contained in the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <returns>The number of blocks actually contained in the <see cref="ICircularBuffer"/></returns>
		int Count();*/
		/// <summary>
		/// Gets the number of blocks the consumer has available to be read
		/// </summary>
		/// <param name="consumerId">The specific consumer to get the count from</param>
		/// <returns>The number of blocks the consumer has available to be read</returns>
		int Count(int consumerId);
		/// <summary>
		/// Gets the free block space in the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <returns></returns>
		int FreeBlockCount();
		/// <summary>
		/// Determines if the <see cref="ICircularBuffer"/> has no blocks stored
		/// </summary>
		/// <returns>Returns true if the <see cref="ICircularBuffer"/> has no blocks stored</returns>
		bool IsEmpty();

		/// <summary>
		/// Clears the buffer and resets the consumers read positions
		/// </summary>
		void Reset();

		/// <summary>
		/// Gets the next block from the <see cref="ICircularBuffer"/>, without advancing the consumers read position
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <returns>Returns the next block from the <see cref="ICircularBuffer"/>, null if the block isn't available</returns>
		T Peek(int consumerId);
		/// <summary>
		/// Gets the block at <paramref name="offset"/> realtive from the current consumers position from the <see cref="ICircularBuffer"/>, without advancing to consumers read position
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <param name="offset">The relative position of the block to be read</param>
		/// <returns>Returns the block at <paramref name="offset"/> realtive from the current consumers position, null if the block isn't available</returns>
		T Peek(int consumerId, int offset);

		/// <summary>
		/// Gets the next block from the <see cref="ICircularBuffer"/>, and advances to the next block
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <returns>Returns the next block from the <see cref="ICircularBuffer"/>, null if the block isn't available</returns>
		T Read(int consumerId);
		/// <summary>
		/// Stores the next <paramref name="len"/> blocks from the <see cref="ICircularBuffer"/> in <paramref name="b"/> with <paramref name="offset"/> as the first position, and advances the position <paramref name="len"/> blocks
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <param name="b">The buffer used to store the blocks</param>
		/// <param name="offset">Offset used for <paramref name="b"/></param>
		/// <param name="len">The number of blocks to be read</param>
		/// <returns>Returns the actual number of block read into the buffer</returns>
		int Read(int consumerId, T[] b, int offset, int len);
		/// <summary>
		/// Discards <paramref name="count"/> blocks from the <paramref name="consumerId"/>s position
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <param name="count">The number of blocks to be discarded</param>
		void Discard(int consumerId, int count);

		/// <summary>
		/// Writes a block to the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <param name="block">The block to be written to the <see cref="ICircularBuffer"/></param>
		void Write(T block);
		/// <summary>
		/// Writes <paramref name="len"/> blocks to the <see cref="ICircularBuffer"/>
		/// </summary>
		/// <param name="b">The source buffer</param>
		/// <param name="offset">The offset used for the source buffer</param>
		/// <param name="len">The number of blocks read from the source buffer and written into the <see cref="ICircularBuffer"/></param>
		void Write(T[] b, int offset, int len);

		/*/// <summary>
		/// Determines if the <see cref="ICircularBuffer"/> has reached the end of the stream
		/// </summary>
		/// <returns>Returns true if  the <see cref="ICircularBuffer"/> has reached the end of the stream</returns>
		bool EndOfStream();
		/// <summary>
		/// Determines if the <paramref name="consumerId"/> has reached the end of the stream
		/// </summary>
		/// <param name="consumerId">The consumer to be used</param>
		/// <returns>Returns true if  the <paramref name="consumerId"/> has reached the end of the stream<</returns>
		bool EndOfStream(int consumerId);*/


	}
	public class CircularBuffer<T> : ICircularBuffer<T> {
		private T[] buffer;
		private int blockCount;

		private int producer;
		private int[] consumer;

		public CircularBuffer(int consumerCount, int blockCount) {
			producer = 0;
			consumer = new int[consumerCount];

			this.blockCount = blockCount;
			buffer = new T[blockCount];
		}

		public int BlockCount { get { return blockCount; } }
		public int ConsumerCount { get { return consumer.Length; } }


		public bool CanRead(int consumerId) { return Count(consumerId) != 0; }
		public bool CanWrite() {
			for(int i = 0;i < consumer.Length;i++) if(Count(i) < blockCount) return false;
			return true;
		}
		public bool CanWrite(int count) {
			for(int i = 0;i < consumer.Length;i++) if(Count(i) + count < blockCount) return false;
			return true;
		}

		public int Count() {
			int maxCount = 0, count;
			for(int i = 0;i < consumer.Length;i++) {
				count = Count(i);
				if(maxCount < count) maxCount = count;
			}
			return maxCount;
		}
		public int Count(int consumerId) {
			int len = producer - consumer[consumerId];
			if(len < 0) len += blockCount;
			return len;
		}
		public int FreeBlockCount() { return blockCount - Count(); }
		public bool IsEmpty(int consumerId) {
			return consumer[consumerId] == producer;
		}
		public bool IsEmpty() {
			bool isEmpty = true;
			for(int i = 0;i < consumer.Length;i++) isEmpty &= consumer[i] == producer;
			return isEmpty;
		}


		public void Reset() {
			consumer = new int[consumer.Length];
			producer = 0;

			buffer = new T[blockCount];
		}

		public T Peek(int consumerId) { return buffer[consumer[consumerId]]; }
		public T Peek(int consumerId, int offset) { return buffer[(consumer[consumerId] + offset) % blockCount]; }
		public T Read(int consumerId) {
			int pos = consumer[consumerId];
			consumer[consumerId] = (pos + 1) % blockCount;
			return buffer[pos];
		}
		public int Read(int consumerId, T[] b, int offset, int len) {
			int count;
			for(count = 0;count < len && !IsEmpty(consumerId);count++) {
				b[count] = Read(consumerId);
			}
			return count;
		}
		public void Discard(int consumerId, int count) { consumer[consumerId] = (consumer[consumerId] + count) % blockCount; }

		public void Write(T block) {
			buffer[producer] = block;
			producer = (producer + 1) % blockCount;
		}
		public void Write(T[] b, int offset, int len) {
			throw new System.NotImplementedException();
		}
	}
}

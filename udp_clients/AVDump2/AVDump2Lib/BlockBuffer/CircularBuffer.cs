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
using System.Collections;
using System;
using AVDump2Lib.BlockBuffer;

namespace AVDump2Lib.BlockBuffer {
	public interface ICircularBuffer<T> {

		ulong BlockCount { get; }
		ulong ProducerPosition { get; }

		T[] Buffer { get; }

		bool ConsumerCanRead(int consumerId);
		T ConsumerGet(int consumerId);
		void ConsumerAdvance(int consumerId);

		bool ProducerCanWrite();
		T ProducerBlock { get; set; }
		void ProducerAdvance();

		void Initialize(int consumerCount);

		bool IsEmpty();
		ulong Count(int consumerId);
	}

	public class CircularBuffer<T> : ICircularBuffer<T> {
		private ulong producer;
		private ulong[] consumers;

		private ulong blockCount;
		private ulong blockMask;

		private T[] buffer;

		public CircularBuffer(int blockCountPower, int consumerCount) : this(blockCountPower) { Initialize(consumerCount); }
		public CircularBuffer(int blockCountPower) {
			if(blockCountPower < 0) throw new ArgumentException("Negative power is not allowed", "blockCountPower");
			blockCount = 1UL << blockCountPower;
			blockMask = blockCount - 1;
			buffer = new T[blockCount];
		}
		public void Initialize(int consumerCount) {
			producer = 0;
			consumers = new ulong[consumerCount];
		}

		public T[] Buffer { get { return buffer; } }

		public bool ConsumerCanRead(int consumerId) { return consumers[consumerId] != producer; }
		public void ConsumerAdvance(int consumerId) { consumers[consumerId]++; }
		public T ConsumerGet(int consumerId) { return buffer[consumers[consumerId] & blockMask]; }
		public T ConsumerRead(int consumerId) {
			//if(!CanRead(consumerId)) return default(T);
			T block = buffer[consumers[consumerId] & blockMask];
			consumers[consumerId]++;

			return block;
		}

		public bool ProducerCanWrite() {
			for(int consumerId = 0;consumerId < consumers.Length;consumerId++) {
				if(consumers[consumerId] + blockCount == producer) return false;
			}
			return true;
		}
		public void ProducerAdvance() { producer++; }
		public T ProducerBlock { get { return buffer[producer & blockMask]; } set { buffer[producer & blockMask] = value; } }
		public void ProducerWrite(T block) {
			buffer[producer & blockMask] = block;
			producer++;
		}

		public bool IsEmpty() {
			for(int consumerId = 0;consumerId < consumers.Length;consumerId++) {
				if(consumers[consumerId] != producer) return false;
			}
			return true;
		}
		public ulong Count(int consumerId) { return producer - consumers[consumerId]; }

		public ulong BlockCount { get { return blockCount; } }


		public ulong ProducerPosition { get { return producer; } }
	}
}

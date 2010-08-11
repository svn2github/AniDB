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
using CSEBMLLib.Sources;
using AVDump2Lib.BlockBuffer;
using System.IO;
using System.Threading;
using System.Diagnostics;

namespace AVDump2Lib.InfoGathering.Parser.CSEBMLLib {
	public class FileSource : IEBMLDataSource, IDisposable {
		private Object syncRoot = new object();

		//private Int64 localOffset;
		private Int64 localStartPosition;

		public IRefillBuffer<byte[]> buffer;
		private int consumerId;

		public FileSource(IRefillBuffer<byte[]> buffer, int consumerId) {
			this.buffer = buffer;
			this.consumerId = consumerId;

			dataInfo = new DataInfo() {
				GetBlock = () => {	
					while(!buffer.CanRead(consumerId)) Thread.Sleep(20);
					return buffer.GetBlock(consumerId);
				},
				Advance = () => {
					while(!buffer.CanRead(consumerId)) Thread.Sleep(20);
					localStartPosition += buffer.GetBlock(consumerId).Length;
					dataInfo.Offset = 0;
					buffer.Advance(consumerId);
				}
			};
		}

		public bool IsStream { get { return true; } }

		public Int64 Length { get { return ((ByteStreamToBlock)buffer.BlockSource).Source.Length; } }

		public long Position {
			get { return localStartPosition + dataInfo.Offset; }
			set { throw new NotSupportedException(); }
		}

		public void Skip(Int64 bytes) {
			if(bytes == 0) return;
			lock(syncRoot) {
				Int64 bytesSkipped = Math.Min(buffer.GetBlock(consumerId).Length - dataInfo.Offset, bytes);
				dataInfo.Offset += bytesSkipped;
				bytes -= bytesSkipped;


				byte[] block = buffer.GetBlock(consumerId);
				if(dataInfo.Offset == block.Length) dataInfo.Advance();
				while(bytes != 0) {
					if(bytes > (block.Length - dataInfo.Offset)) {
						bytes -= block.Length - dataInfo.Offset;
						localStartPosition += block.Length;
						dataInfo.Offset = 0;

						while(!buffer.CanRead(consumerId)) Thread.Sleep(20);
						buffer.Advance(consumerId);

					} else {
						dataInfo.Offset += bytes;
						bytes = 0;
					}
					block = buffer.GetBlock(consumerId);
				}
			}
		}

		//public void Skip(Int64 bytes) {
		//    if(bytes == 0) return;
		//    lock(syncRoot) {
		//        Int64 bytesSkipped = Math.Min(buffer.GetBlock(consumerId).Length - dataInfo.Offset, bytes);
		//        dataInfo.Offset += bytesSkipped;
		//        bytes -= bytesSkipped;

		//        if(dataInfo.Offset == buffer.GetBlock(consumerId).Length) dataInfo.Advance();

		//        byte[] block;
		//        while(bytes != 0) {
		//            block = buffer.GetBlock(consumerId);
		//            if(bytes > (block.Length - dataInfo.Offset)) {
		//                bytes -= block.Length - dataInfo.Offset;
		//                localStartPosition += block.Length;
		//                dataInfo.Offset = 0;

		//                while(!buffer.CanRead(consumerId)) Thread.Sleep(20);
		//                buffer.Advance(consumerId);

		//            } else {
		//                dataInfo.Offset += bytes;
		//                bytes = 0;
		//            }
		//        }
		//    }
		//}



		private DataInfo dataInfo;
		public T ProcessData<T>(Func<DataInfo, T> dataProcessFunc) {
			T value;
			lock(syncRoot) {
				value = dataProcessFunc(dataInfo);
				if(dataInfo.Offset == buffer.GetBlock(consumerId).Length) dataInfo.Advance();
			}
			return value;
		}

		public bool EOF() { throw new NotImplementedException(); }

		public void Dispose() { buffer.Dispose(); }
	}

}

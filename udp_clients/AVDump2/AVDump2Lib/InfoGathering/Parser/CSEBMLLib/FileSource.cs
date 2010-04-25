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
	public class FileSource : IEBMLDataSource {
		private Object syncRoot = new object();

		private Int64 localOffset;
		private Int64 localStartPosition;
		private bool advanced;
		private int consumerId;

		public IRefillBuffer<byte[]> buffer;

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
					buffer.Advance(consumerId);
					advanced = true;
				}
			};
		}

		public bool IsStream { get { return true; } }

		public long Length { get { return ((ByteStreamToBlock)buffer.BlockSource).Source.Length; } }

		public long Position { get { return localStartPosition + localOffset; } set { throw new NotSupportedException(); } }

		public void Skip(long bytes) {
			lock(syncRoot) {
				Int64 bytesSkipped = Math.Min(buffer.GetBlock(consumerId).Length - localOffset, bytes);
				localOffset += bytesSkipped;
				bytes -= bytesSkipped;

				byte[] block;
				while(bytes != 0) {
					block = buffer.GetBlock(consumerId);
					if(bytes > (block.Length - localOffset)) {
						bytes -= block.Length - localOffset;
						localStartPosition += block.Length;
						localOffset = 0;

						while(!buffer.CanRead(consumerId)) Thread.Sleep(20);
						buffer.Advance(consumerId);

					} else {
						localOffset += bytes;
						bytes = 0;
					}
				}
			}
		}

		private DataInfo dataInfo;
		public object ProcessData(long length, Func<DataInfo, long> dataProcessFunc) {
			dataInfo.Offset = localOffset;
			dataInfo.Length = length;
			dataInfo.Value = null;

			long offset;
			lock(syncRoot) offset = dataProcessFunc(dataInfo);
			if(advanced) {
				localOffset = offset;
				advanced = false;
			} else {
				localOffset += offset;
			}

			return dataInfo.Value;
		}
	}
}

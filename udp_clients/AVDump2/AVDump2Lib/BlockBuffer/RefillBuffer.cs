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

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Threading;

namespace AVDump2Lib.BlockBuffer {
    public class RefillBuffer {
        RingBuffer rb;
        Stream stream;
        Thread filler;

        int blockCount;
        int blockSize;
        int lastBlockSize;
        bool endOfBuffer;

        public RefillBuffer(Stream stream, int blockCount, int blockSize) {
            filler = new Thread(new ThreadStart(RingFiller));

            this.stream = stream;
            this.blockCount = blockCount;
            this.blockSize = blockSize;
            lastBlockSize = -1;
            endOfBuffer = false;
        }

        public void Start(int reader) {
            rb = new RingBuffer(blockCount, blockSize, reader);
            filler.Start();
        }

        public int Read(int reader, out byte[] b, out bool isLastBlock) {
            int numRead;
            bool test = rb.IsLastBlock(reader);
            b = rb.Read(reader);

            if(test) {
                isLastBlock = true;
                numRead = lastBlockSize;
            } else {
                isLastBlock = false;
                numRead = rb.BlockSize;
            }
            return numRead;
        }
        public bool CanRead(int reader) { return rb.CanRead(reader); }

        public int Count(int reader) { return rb.Count(reader); }
        public int BlockCount { get { return blockCount; } }

        private void RingFiller() {
            int numRead = lastBlockSize = rb.Write(stream);
            while(numRead != 0) {
                lock(rb) {

                    while(!rb.CanWrite()) Thread.Sleep(200);
                    lastBlockSize = numRead;
                    numRead = rb.Write(stream);
                }
            }
            endOfBuffer = true;
            //Debug.Print("EOF");
        }
    }
}

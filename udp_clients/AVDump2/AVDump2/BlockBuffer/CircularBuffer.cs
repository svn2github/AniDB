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
using System.Diagnostics;

namespace AVDump2Lib.BlockBuffer {
    /// <summary>DO NOT USE</summary>
    class CircularBuffer {
        private byte[][] buffer;
        private int blockLength;
        private int blockCount;
        private int blockMask;

        private Stream stream;
        private Thread ringFillerThread;

        private int absoluteStart;
        private int absoluteEnd;
        private int lastBlock;
        private int lastBlockSize;
        private int[] read;
        private bool[] readAdvance;

        /// <summary>Creates a new CircularBuffer instance</summary>
        /// <param name="stream">The stream to be read from</param>
        /// <param name="blockCount">Blockcount in the buffer (needs to be a power of 2)</param>
        /// <param name="blockLength">Blocksize in the buffer</param>
        /// <param name="reader">Readposition count</param>
        public CircularBuffer(Stream stream, int blockCount, int blockLength, int reader) {
            this.stream = stream;
            this.blockLength = blockLength;
            this.blockCount = blockCount;
            blockMask = blockCount - 1;

            CheckConditions();

            absoluteEnd = 0;
            absoluteStart = 0;
            lastBlock = -1;
            lastBlockSize = -1;

            buffer = new byte[blockCount][];
            for(int i = 0;i < blockCount;i++) {
                buffer[i] = new byte[blockLength];
            }
            read = new int[reader];
            readAdvance = new bool[reader];

            ringFillerThread = new Thread(new ThreadStart(RingFiller));
            ringFillerThread.Start();
        }

        /// <summary>Fills the circular buffer (Thread Method)</summary>
        private void RingFiller() {
            int numRead;

            numRead = 0;
            //Read blockLength bytes until EOF
            do {
                //Wait until new writing space is available
                while((absoluteEnd - absoluteStart) > blockMask) Thread.Sleep(0);

                //Read block and advance absoluteEnd
                lastBlockSize = numRead;
                numRead = stream.Read(buffer[absoluteEnd & blockMask], 0, blockLength);
                if(numRead != 0) absoluteEnd += 1;
            } while(numRead > 0);
            lastBlock = absoluteEnd - 1;

            Debug.Print("EOF. lastBlock:" + lastBlock.ToString() + " lastBufferBLock:" + (lastBlock & blockMask).ToString());
        }

        /// <summary>Advance a read position by one block</summary>
        /// <param name="reader">Index of the read position</param>
        public void ReadAdvance(int reader) {
            //Advance read position and mark it as advanced
            read[reader] += 1;
            readAdvance[reader] = true;

            bool advanceStart = true;
            for(int i = 0;i < read.Length;i++) {
                if(!readAdvance[i]) { advanceStart = false; break; }
            }
            if(advanceStart) {
                //Every readAdvance element is true, advance absoluteStart by one block.
                absoluteStart += 1;

                //Mark every readAdvane element as false
                for(int i = 0;i < read.Length;i++) readAdvance[i] = false;
            }
        }

        /// <summary>Check if the block avaiable for the specific reader</summary>
        /// <param name="reader">The reader index</param>
        /// <returns>Blockavailabillity</returns>
        public bool BlockAvailable(int reader) {
            return read[reader] < absoluteEnd || read[reader] == lastBlock;
        }

        /// <summary>Reads the block the specific reader points to, without advancing it</summary>
        /// <param name="reader">The reader index</param>
        /// <param name="block">The block</param>
        /// <param name="isLastBlock">Is last block of the stream</param>
        /// <returns>Blocksize</returns>
        public int Read(int reader, out byte[] block, out bool isLastBlock) {
            int size;
            if(read[reader] == lastBlock) {
                //LastBlock: Set out variables
                isLastBlock = true;
                size = lastBlockSize;
            } else {
                //Not LastBlock: Set out variables with default blocklength
                isLastBlock = false;
                size = blockLength;
            }
            //Retrieve block from buffer by masking uneeded bit off
            block = buffer[read[reader] & blockMask];

            Debug.Print("reader:" + reader.ToString() + " isLastBlock:" + isLastBlock.ToString() + " size:" + size.ToString() + " bufferpos:" + (read[reader] & blockMask).ToString());
            return size;
        }

        /// <summary>Check if the blockcount is > 0 and a power of 2</summary>
        private void CheckConditions() {
            if(blockCount == 0) throw new Exception();
            int tmp = blockCount;
            for(int i = 0;i < 31;i++) {
                if((tmp & 1) == 1 && tmp != 1) throw new Exception();
                tmp = tmp >> 1;
            }

        }
    }
}

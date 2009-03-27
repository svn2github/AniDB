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

using System.Collections.Generic;
using System.Text;
using System.IO;

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
            /*if(readPos == writePos)
                throw new Exception("Buffer full");*/
        }
        public int Write(Stream stream) {
            int numRead = stream.Read(buffer[writePos], 0, blockSize);
            if(numRead != 0) {
                if(stream.Position != stream.Length) {
                    writePos = (writePos + 1) & blockMask;
                }
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
        public byte[] Peek(int reader) {
            return buffer[readPos[reader]];
        }
        public byte[] Read(int reader) {
            int pos = readPos[reader];
            readPos[reader] = (readPos[reader] + 1) & blockMask;
            return buffer[pos];
        }
        public void Read(byte[][] buffer, int offset, int len, int reader) {
            for(int i = 0;i < len;++i) buffer[i] = Read(reader);
        }
        public bool CanRead(int reader) {
            return Count(reader) != 0 || (lastBlockReached && Count(reader) == 0);
        }

        public void Discard(int bytes, int reader) {
            if(Count(reader) < bytes) bytes = Count(reader);

            readPos[reader] = (readPos[reader] + bytes) & blockMask;
        }

        public string getString(int pos, int length, int reader) {
            string s = "";
            for(int i = pos;i < pos + length;++i) {
                if(this[i, reader][0] >= 32 && this[i, reader][0] <= 127)
                    s += (char)this[i, reader][0];
            }
            return s.Trim();
        }
        public void displayBuffer(int Length, int reader) {
            int i = 0;
            while(i < Length) {
                System.Diagnostics.Debug.Write(i.ToString("X2") + " : ");
                for(int j = i;j < i + 16;++j) {
                    if(j < Length)
                        System.Diagnostics.Debug.Write(this[j, reader][0].ToString("X2"));
                    else
                        System.Diagnostics.Debug.Write("  ");
                    System.Diagnostics.Debug.Write(' ');
                }
                System.Diagnostics.Debug.Write(": ");
                for(int j = i;j < i + 16;++j) {
                    char c;
                    if(j < Length)
                        c = (char)this[j, reader][0];
                    else
                        c = ' ';
                    if(c < ' ' || c > '~')
                        c = '.';
                    System.Diagnostics.Debug.Write(c);
                }
                i += 16;
                System.Diagnostics.Debug.WriteLine(" :");
            }
            System.Diagnostics.Debug.WriteLine("");
        }
    }

}

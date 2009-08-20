/******************************************************************************
 *
 * Jacksum version 1.5.0 - checksum utility in Java
 * Copyright (C) 2001-2004 Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
 * All Rights Reserved, http://www.jonelo.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Edonkey is an implementation of the AbstractChecksum in order to calculate a
 * message digest in the form of edonkey/emule hash. It uses the MD4 algorithm
 * as an auxiliary algorithm, from the GNU crypto project,
 * http://www.gnu.org/software/classpathx/crypto
 *
 *****************************************************************************
 * Modified and ported to C# by DvdKhl
 *****************************************************************************/

using System.Collections.Generic;
using System.Security.Cryptography;
using System.Diagnostics;

namespace AVDump2Lib.Hashes {
    public class Ed2k : HashAlgorithm {
        private static int BLOCKSIZE;
        private List<byte[]> md4HashBlocks;
        private HashAlgorithm md4;
        private byte[] blueHash;
        private bool blueIsRed;
        private long length;

        public Ed2k() {
            md4HashBlocks = new List<byte[]>();
            md4 = new Md4();
        }

        protected override void HashCore(byte[] array, int ibStart, int cbSize) {
            int toWrite = cbSize - ibStart;
            int space = BLOCKSIZE - (int)(length % BLOCKSIZE);

            if(space > toWrite) {
                md4.TransformBlock(array, ibStart, cbSize, null, 0);
                length += cbSize;

            } else if(space == toWrite) {
                md4.TransformFinalBlock(array, ibStart, cbSize);
                md4HashBlocks.Add(md4.Hash);
                md4.Initialize();
                length += cbSize;

            } else if(space < toWrite) {
                md4.TransformFinalBlock(array, ibStart, space);
                md4HashBlocks.Add(md4.Hash);
                md4.Initialize();
                length += space;

                md4.TransformBlock(array, ibStart + space, toWrite - space, null, 0);
                length += toWrite - space;
            }
        }


        /// <summary>Calculates both ed2k hashes</summary>
        /// <returns>Always returns the red hash</returns>
        protected override byte[] HashFinal() {
            md4.TransformFinalBlock(new byte[0], 0, 0);
            md4HashBlocks.Add(md4.Hash);

            //Blue Hash
            if(length >= BLOCKSIZE) {
                md4.Initialize();
                for(int i = 0;i < md4HashBlocks.Count;i++) {
                    md4.TransformBlock(md4HashBlocks[i], 0, 16, null, 0);
                }
                md4.TransformFinalBlock(new byte[0], 0, 0);
                blueHash = md4.Hash;
            }

            //Red Hash
            if(length % BLOCKSIZE == 0) {
                md4.Initialize();
                for(int i = 0;i < md4HashBlocks.Count;i++) {
                    md4.TransformBlock(md4HashBlocks[i], 0, 16, null, 0);
                }
                md4.TransformFinalBlock(md4.ComputeHash(new byte[0]), 0, 0);
            }

            if(length % BLOCKSIZE != 0) blueIsRed = true;

            return md4.Hash;
        }

        public override void Initialize() {
            //Called when TransformFinalBlock is called in Mono (not in NET) !

            length = 0;
            //blueHash = null;
            md4.Initialize();
            BLOCKSIZE = 9728000;
            md4HashBlocks.Clear();
        }

        public bool BlueIsRed() { return blueIsRed; }
        public byte[] BlueHash { get { return blueHash; } }
    }
}

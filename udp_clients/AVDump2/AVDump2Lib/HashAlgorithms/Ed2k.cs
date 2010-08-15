
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Diagnostics;
using System.Collections.ObjectModel;

namespace AVDump2Lib.HashAlgorithms {
	public class Ed2kOld : HashAlgorithm {
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
		private static int BLOCKSIZE;
		private List<byte[]> md4HashBlocks;
		private Md4 md4;
		private byte[] redHash;
		private long length;

		public Ed2kOld() {
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
			byte[] hash;
			if(length < BLOCKSIZE) {
				md4.TransformFinalBlock(new byte[0], 0, 0);
				hash = md4.Hash;
			} else if(length == BLOCKSIZE) {
				hash = md4HashBlocks[0];
				md4.TransformBlock(md4HashBlocks[0], 0, 16, null, 0);
				md4.TransformFinalBlock(md4.ComputeHash(new byte[0]), 0, 16);
				redHash = md4.Hash;
			} else {
				if(length % BLOCKSIZE != 0) {
					md4.TransformFinalBlock(new byte[0], 0, 0);
					md4HashBlocks.Add(md4.Hash);
				}

				//Blue
				md4.Initialize();
				for(int i = 0;i < md4HashBlocks.Count;i++) md4.TransformBlock(md4HashBlocks[i], 0, 16, null, 0);
				md4.TransformFinalBlock(new byte[0], 0, 0);
				hash = md4.Hash;

				//Red
				if(length % BLOCKSIZE == 0) {
					md4.Initialize();
					for(int i = 0;i < md4HashBlocks.Count;i++) md4.TransformBlock(md4HashBlocks[i], 0, 16, null, 0);
					md4.TransformFinalBlock(md4.ComputeHash(new byte[0]), 0, 16);
					redHash = md4.Hash;
				}
			}

			if(redHash == null) BlueIsRed = true;

			return hash;
		}

		public override void Initialize() {
			//Called when TransformFinalBlock is called in Mono (not in NET) !

			length = 0;
			//blueHash = null;
			md4.Initialize();
			BLOCKSIZE = 9728000;
			md4HashBlocks.Clear();
		}

		public bool BlueIsRed { get; private set; }
		public byte[] RedHash { get { return redHash != null ? redHash : Hash; } }
	}

	public class Ed2k : HashAlgorithm {
		public const int BLOCKSIZE = 9500 * 1024;

		public bool BlueIsRed { get; private set; } private byte[] redHash;
		public byte[] RedHash { get { return redHash != null ? (byte[])redHash.Clone() : Hash; } }

		private byte[] nullArray = new byte[0];
		private List<byte[]> md4HashBlocks;
		private int missing = BLOCKSIZE;
		private Md4 md4;

		public Ed2k() {
			md4HashBlocks = new List<byte[]>();
			md4 = new Md4();
		}

		protected override void HashCore(byte[] b, int offset, int length) {
			while(length != 0) {
				if(length < missing) {
					md4.TransformBlock(b, offset, length, null, 0);
					missing -= length;
					length = 0;

				} else {
					md4.TransformFinalBlock(b, offset, missing);
					md4HashBlocks.Add(md4.Hash);
					md4.Initialize();

					length -= missing;
					offset += missing;
					missing = BLOCKSIZE;
				}
			}
		}


		/// <summary>Calculates both ed2k hashes</summary>
		/// <returns>Always returns the red hash</returns>
		protected override byte[] HashFinal() {
			byte[] hash;

			if(md4HashBlocks.Count == 0) {
				md4.TransformFinalBlock(nullArray, 0, 0);
				hash = md4.Hash;
			} else if(md4HashBlocks.Count == 1 && missing == BLOCKSIZE) {
				hash = md4HashBlocks[0];

				md4.TransformBlock(md4HashBlocks[0], 0, 16, null, 0); 
				md4.TransformFinalBlock(md4.ComputeHash(nullArray), 0, 16);
				redHash = md4.Hash;

			} else {
				if(missing != BLOCKSIZE) {
					md4.TransformFinalBlock(nullArray, 0, 0);
					md4HashBlocks.Add(md4.Hash);
				}

				md4.Initialize();
				foreach(var md4HashBlock in md4HashBlocks) md4.TransformBlock(md4HashBlock, 0, 16, null, 0);
				var state = md4.GetState();
				md4.TransformFinalBlock(nullArray, 0, 0);
				hash = md4.Hash;


				if(missing == BLOCKSIZE) {
					md4.Initialize(state);
					md4.TransformFinalBlock(md4.ComputeHash(nullArray), 0, 16);
					redHash = md4.Hash;
				}


			}

			if(redHash == null) BlueIsRed = true;
			return hash;
		}

		public override void Initialize() {
			//Called when TransformFinalBlock is called in Mono (not in NET) !

			missing = BLOCKSIZE;
			redHash = null;
			md4.Initialize();
			md4HashBlocks.Clear();
		}

	}

}

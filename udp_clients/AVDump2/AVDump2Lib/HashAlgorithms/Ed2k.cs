
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Diagnostics;
using System.Collections.ObjectModel;

namespace AVDump2Lib.HashAlgorithms {
	public class Ed2k : HashAlgorithm {
		public const int BLOCKSIZE = 9500 * 1024;

		public bool BlueIsRed { get; private set; }
		public byte[] RedHash { get { return redHash != null ? (byte[])redHash.Clone() : Hash; } } private byte[] redHash;
		public byte[] BlueHash { get { return blueHash != null ? (byte[])blueHash.Clone() : Hash; } } private byte[] blueHash;

		private byte[] nullArray = new byte[0];
		private byte[] nullMd4Hash;

		private List<byte[]> md4HashBlocks;
		private int missing = BLOCKSIZE;
		private Md4 md4;

		public Ed2k() {
			md4HashBlocks = new List<byte[]>();
			md4 = new Md4();

			nullMd4Hash = md4.ComputeHash(nullArray);
			md4.Initialize();
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
			if(md4HashBlocks.Count == 0) {
				md4.TransformFinalBlock(nullArray, 0, 0);
				blueHash = md4.Hash;
			} else if(md4HashBlocks.Count == 1 && missing == BLOCKSIZE) {
				blueHash = md4HashBlocks[0];

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
				blueHash = md4.Hash;

				if(missing == BLOCKSIZE) {
					md4.Initialize(state);
					md4.TransformFinalBlock(nullMd4Hash, 0, 16);
					redHash = md4.Hash;
				}
			}

			if(redHash == null) BlueIsRed = true;
			return redHash == null ? blueHash : redHash;
		}

		public override void Initialize() {
			//Called when TransformFinalBlock is called in Mono (not in NET) !

			missing = BLOCKSIZE;
			redHash = null;
			blueHash = null;
			md4.Initialize();
			md4HashBlocks.Clear();
		}

	}

}

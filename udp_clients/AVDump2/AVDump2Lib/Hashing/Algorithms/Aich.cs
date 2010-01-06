using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;
using AVDump2Lib.Hashes;
using System.Diagnostics;

namespace AVDump2Lib.Hashing.Algorithms {
	/// <summary>Untested (most likely wrong)</summary>
	public class Aich : HashAlgorithm {
		private static byte[] emptyArray = new byte[0];
		private const int BLOCKSIZE = 180 * 1024;
		private const int BLOCKSIZEREMAINDER = 140 * 1024;

		private SHA1 blockHasher;
		private SHA1 nodeHasher;
		private List<byte[]> partHashes;
		private List<byte[]> blockHashes;
		private int blockLengthTodo;

		protected override void HashCore(byte[] array, int ibStart, int cbSize) {
			while(cbSize != 0) {
				if(blockLengthTodo > cbSize) {
					blockHasher.TransformBlock(array, ibStart, cbSize, null, 0);

					blockLengthTodo -= cbSize;
					ibStart += cbSize;
					cbSize = 0;
					return;

				} else if(blockLengthTodo != 0) {
					blockHasher.TransformFinalBlock(array, ibStart, blockLengthTodo);
					blockHashes.Add(blockHasher.Hash);
					blockHasher.Initialize();

					ibStart += blockLengthTodo;
					cbSize -= blockLengthTodo;
					blockLengthTodo = 0;
				}

				while(blockHashes.Count < 52 && cbSize >= BLOCKSIZE) {
					blockHashes.Add(blockHasher.ComputeHash(array, ibStart, BLOCKSIZE));
					ibStart += BLOCKSIZE;
					cbSize -= BLOCKSIZE;
				}

				if(blockHashes.Count < 52) {
					blockHasher.TransformBlock(array, ibStart, cbSize, null, 0);
					blockLengthTodo = BLOCKSIZE - cbSize;
					ibStart += cbSize;
					cbSize = 0;

				} else if(blockHashes.Count == 52) {
					if(cbSize < BLOCKSIZEREMAINDER) {
						blockHasher.TransformBlock(array, ibStart, cbSize, null, 0);
						blockLengthTodo = BLOCKSIZEREMAINDER - cbSize;
						ibStart += cbSize;
						cbSize = 0;

					} else {
						blockHashes.Add(blockHasher.ComputeHash(array, ibStart, BLOCKSIZEREMAINDER));
						ibStart += BLOCKSIZEREMAINDER;
						cbSize -= BLOCKSIZEREMAINDER;
					}
				}

				if(blockHashes.Count == 53) {
					partHashes.Add(CreateRootHash(blockHashes));
					blockHashes.Clear();
				}
			}
		}
		protected override byte[] HashFinal() {
			if(blockLengthTodo != 0) {
				blockHasher.TransformFinalBlock(emptyArray, 0, 0);
				blockHashes.Add(blockHasher.Hash);
				blockHasher.Initialize();
			}
			if(blockHashes.Count != 0) partHashes.Add(CreateRootHash(blockHashes));

			return CreateRootHash(partHashes);
		}
		public override void Initialize() {
			partHashes = new List<byte[]>();
			blockHashes = new List<byte[]>();
			blockHasher = SHA1.Create();
			nodeHasher = SHA1.Create();
		}

		private byte[] CreateRootHash(List<byte[]> blockHashes) { return SubCreateRootHash(blockHashes, 0, blockHashes.Count - 1, true); }
		private byte[] SubCreateRootHash(List<byte[]> blockHashes, int left, int right, bool leftNode) {
			if(left == right) {
				return HashBlocks(blockHashes[left], null);
			} else if(left + 1 == right) {
				return HashBlocks(blockHashes[left], blockHashes[right]);
			} else {
				int border = ((left + right + (leftNode ? 1 : 0)) / 2);
				return HashBlocks(SubCreateRootHash(blockHashes, left, border, true), SubCreateRootHash(blockHashes, border + 1, right, false));
			}
		}
		private byte[] HashBlocks(byte[] l, byte[] r) {
			nodeHasher.Initialize();
			if(l != null) nodeHasher.TransformBlock(l, 0, l.Length, null, 0);
			if(r != null) nodeHasher.TransformBlock(r, 0, r.Length, null, 0);
			nodeHasher.TransformFinalBlock(emptyArray, 0, 0);
			return nodeHasher.Hash;
		}
	}
}

using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;

namespace AVDump2Lib.Hashing.Algorithms {
	/// <summary>Untested (most likely wrong)</summary>
	public class Aich : HashAlgorithm {
		static SHA1 sha1 = SHA1.Create();

		List<AichHash> partHashes;
		Part part;

		protected override void HashCore(byte[] array, int ibStart, int cbSize) {
			while(cbSize != 0) {
				if(part.HasFinished) {
					partHashes.Add(CreateHashTree(part.BlockHashes));
					part.Clear();
				}
				part.Update(array, ref ibStart, ref cbSize);
			}
		}

		protected override byte[] HashFinal() {
			if(part.BlockHashes.Count != 0) {
				part.Final();
				partHashes.Add(CreateHashTree(part.BlockHashes));
			}

			return CreateHashTree(partHashes).Hash;
		}

		public override void Initialize() {
			partHashes = new List<AichHash>();
			part = new Part();
		}


		public static AichHash CreateHashTree(List<AichHash> blockHashes) {
				int count = blockHashes.Count;
				while(blockHashes.Count > 1) {
					blockHashes.Add(new AichHash(blockHashes[0], blockHashes[1]));
					blockHashes.RemoveRange(0, 2);
					count -= 2;

					if(blockHashes.Count != 1) {
						if(count == 1) {
							blockHashes.Add(new AichHash(blockHashes[0]));
							blockHashes.RemoveAt(0);
						}
						count = blockHashes.Count;
					}
				}
				return blockHashes[0];
		}


		class Part {
			List<AichHash> blockHashes;
			int blockLengthTodo;
			public bool HasFinished { get; private set; }

			public Part() {
				blockLengthTodo = 0;
				blockHashes = new List<AichHash>();
			}

			public void Update(byte[] array, ref int ibStart, ref int cbSize) {
				if(blockLengthTodo != 0) {
					if(blockLengthTodo > cbSize) {
						sha1.TransformBlock(array, ibStart, cbSize, null, 0);
						blockLengthTodo -= cbSize;

						ibStart += cbSize;
						cbSize = 0;
					} else {
						sha1.TransformFinalBlock(array, ibStart, blockLengthTodo);
						blockHashes.Add(new AichHash(sha1.Hash));
						sha1.Initialize();

						ibStart += blockLengthTodo;
						cbSize -= blockLengthTodo;
						blockLengthTodo = 0;

						if(blockHashes.Count==53) HasFinished = true;
					}
				}

				while(blockHashes.Count < 52 && cbSize >= 180 * 1024) {
					blockHashes.Add(new AichHash(sha1.ComputeHash(array, ibStart, 180 * 1024)));
					ibStart += 180 * 1024;
					cbSize -= 180 * 1024;
				}

				if(blockHashes.Count == 52) {
					if(cbSize < 140 * 1024) {
						sha1.TransformBlock(array, ibStart, cbSize, null, 0);
						blockLengthTodo = 140 * 1024 - cbSize;
						ibStart += cbSize;
						cbSize = 0;
					} else {
						blockHashes.Add(new AichHash(sha1.ComputeHash(array, ibStart, 180 * 1024)));
						HasFinished = true;
						ibStart += 180 * 1024;
						cbSize -= 180 * 1024;
					}
				} else if(blockHashes.Count < 52) {
					sha1.TransformBlock(array, ibStart, cbSize, null, 0);
					blockLengthTodo = 180 * 1024 - cbSize;
					ibStart += cbSize;
					cbSize = 0;
				}
			}

			public void Final() {
				if(blockLengthTodo != 0) {
					sha1.TransformFinalBlock(new byte[0], 0, 0);
					blockHashes.Add(new AichHash(sha1.Hash));
					sha1.Initialize();
				}
			}

			public List<AichHash> BlockHashes { get { return blockHashes; } }
	
			public void Clear() {
				blockHashes.Clear();
				HasFinished = false;
			}
		}

		public class AichHash {
			byte[] hash;
			AichHash a, b;

			public bool IsLeave { get; set; }

			public AichHash(byte[] hash) {
				this.hash = hash;
				IsLeave = true;
			}
			public AichHash(AichHash a, AichHash b) {
				this.a = a;
				this.b = b;

				sha1.TransformBlock(a.Hash, 0, 20, null, 0);
				sha1.TransformFinalBlock(b.Hash, 0, 20);
				hash = sha1.Hash;
				sha1.Initialize();

				IsLeave = false;
			}
			public AichHash(AichHash a) {
				this.a = a;
				this.b = null;

				hash = sha1.ComputeHash(a.Hash);

				IsLeave = false;
			}

			public AichHash NodeA { get { return a; } }
			public AichHash NodeB { get { return b; } }
			public byte[] Hash { get { return hash; } }

		}
	}
}

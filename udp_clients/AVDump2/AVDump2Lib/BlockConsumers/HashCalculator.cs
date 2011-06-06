using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.Misc;

namespace AVDump2Lib.BlockConsumers {
	public class HashCalculator : BlockConsumerBase {
		private HashAlgorithm h;
		private Func<HashAlgorithm, string> toString;

		public HashCalculator(string name, HashAlgorithm h, Func<HashAlgorithm, string> toString)
			: base(name) {
			this.h = h;
			this.toString = toString;
		}

		public HashAlgorithm HashObj { get { if(HasFinished) return h; else throw new Exception("Hashing has not finished"); } }
		protected override void DoWork() {


			Consumer<byte[]> consumer = (byte[] block) => {
				h.TransformBlock(block, 0, block.Length, null, 0);
				ProcessedBytes += block.Length;
			};

			while(!b.EndOfStream(consumerId)) b.Read(consumerId, consumer);
			h.TransformFinalBlock(new byte[0], 0, 0);
		}


		public override string ToString() {
			//return BaseConverter.ToBase32String(h.Hash, "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567".ToLower());
			return toString(h); 
		}

		protected override void InitInternal() { h.Initialize(); }
	}

}

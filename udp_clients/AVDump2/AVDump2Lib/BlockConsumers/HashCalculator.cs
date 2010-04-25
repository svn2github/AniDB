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
using System.IO;
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;
using AVDump2Lib.BlockConsumers.Tools;

namespace AVDump2Lib.BlockConsumers {
	public class HashCalculator : BlockConsumerBase {
		private HashAlgorithm h;

		public HashCalculator(HashAlgorithm h, string name)
			: base(name) {
			this.h = h;
			h.Initialize();
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
	}

}

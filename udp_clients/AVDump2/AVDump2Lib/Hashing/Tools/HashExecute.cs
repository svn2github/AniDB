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
using System.IO;
using System.Security.Cryptography;
using AVDump2Lib.BlockBuffer;
using System;
using System.Threading;

namespace AVDump2Lib.Hashing.Tools {
	public class HashExecute {
		private IRefillBuffer<byte[]> b;
		private HashAlgorithm h;
		private Thread t;
		private string name;

		public event EventHandler HashingDone;
		private bool hasFinished;
		private int consumerId;
		private long processedBytes;

		public HashExecute(HashAlgorithm h, int consumerId, string name) {
			this.h = h;
			this.consumerId = consumerId;

			t = new Thread(new ThreadStart(DoWork));
			t.Name = h.ToString();
			this.name = name;
		}

		public HashAlgorithm HashObj { get { if(hasFinished) return h; else throw new Exception("Hashing has not finished"); } }
		public long ProcessedBytes { get { return processedBytes; } }
		public bool HasFinished { get { return hasFinished; } }
		public string Name { get { return name; } }

		public void Start(IRefillBuffer<byte[]> b) {
			this.b = b;
			t.Start();
		}

		public void Join() { t.Join(); }

		private void DoWork() {
			Consumer<byte[]> consumer = (byte[] block) => {
				h.TransformBlock(block, 0, block.Length, null, 0);
				processedBytes += block.Length;
			};

			processedBytes = 0;
			while(!b.EndOfStream(consumerId)) b.Read(consumerId, consumer);
			h.TransformFinalBlock(new byte[0], 0, 0);

			if(HashingDone != null) HashingDone(this, new EventArgs());
			hasFinished = true;
		}
	}

}

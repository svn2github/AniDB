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
using System.Security.Cryptography;
using System.Threading;
using AVDump2Lib.BlockBuffer;
using System.Collections;

namespace AVDump2Lib.Hashes {
	public class HashContainer {
		public event EventHandler HashDone;

		private string name;
		private bool isDone;
		private int readerID;
		private byte[] altHash; //Handle Ed2k screwup
		private long bytesProcessed;

		private HashAlgorithm hashAlgorithm;
		private IRefillBuffer<byte[]> b;
		private Thread t;

		public HashContainer(int readerID, HashAlgorithm hashAlgorithm, string name) {
			this.hashAlgorithm = hashAlgorithm;
			this.readerID = readerID;
			this.name = name;
			isDone = false;
			t = new Thread(new ThreadStart(DoWork));
		}

		public string Name { get { return name; } }
		public bool IsDone() { return isDone; }
		public int ReaderID { get { return readerID; } }
		public byte[] Hash { get { return hashAlgorithm.Hash; } }
		public byte[] AltHash { get { return altHash; } }
		public long BytesProcessed { get { return bytesProcessed; } }

		public void Start(IRefillBuffer<byte[]> b) {
			this.b = b;
			t.Start(); 
		}
		public void Join() { t.Join(); }


		private void DoWork() {
			byte[] block = null;

			while(!b.EndOfStream(readerID)) {
				block = b.Read(readerID, true);
				hashAlgorithm.TransformBlock(block, 0, block.Length, null, 0);
				bytesProcessed += block.Length;
			}
			hashAlgorithm.TransformFinalBlock(new byte[0], 0, 0);


			//Handle Ed2k screwup
			if(hashAlgorithm is Ed2k && !((Ed2k)hashAlgorithm).BlueIsRed()) altHash = ((Ed2k)hashAlgorithm).BlueHash;

			if(HashDone != null) HashDone(this, new EventArgs());
			isDone = true;
		}

	}
}

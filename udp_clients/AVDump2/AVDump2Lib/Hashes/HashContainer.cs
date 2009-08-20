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
		private RefillBuffer b;
		private RefillBuffer<byte[]> b2;
		private Thread t;

		public HashContainer(RefillBuffer b, int readerID, HashAlgorithm hashAlgorithm, string name) {
			this.hashAlgorithm = hashAlgorithm;
			this.readerID = readerID;
			this.name = name;
			this.b = b;
			isDone = false;
			t = new Thread(new ThreadStart(DoWork));
		}

		public HashContainer(RefillBuffer<byte[]> b2, int readerID, HashAlgorithm hashAlgorithm, string name) {
			this.hashAlgorithm = hashAlgorithm;
			this.readerID = readerID;
			this.name = name;
			this.b2 = b2;
			isDone = false;
			t = new Thread(new ThreadStart(DoWork2));
		}

		public string Name { get { return name; } }
		public bool IsDone() { return isDone; }
		public int ReaderID { get { return readerID; } }
		public byte[] Hash { get { return hashAlgorithm.Hash; } }
		public byte[] AltHash { get { return altHash; } }
		public long BytesProcessed { get { return bytesProcessed; } }

		public void Start() { t.Start(); }
		public void Join() { t.Join(); }

		private void DoWork() {
			byte[] block = null;
			bool isLastBlock = false;
			int numRead = 0;

			while(!b.CanRead(readerID)) Thread.Sleep(50);
			numRead = b.Read(readerID, out block, out isLastBlock);
			while(!isLastBlock) {
				hashAlgorithm.TransformBlock(block, 0, numRead, null, 0);
				while(!b.CanRead(readerID)) Thread.Sleep(50);
				//if(name.StartsWith("ed2k")) Thread.Sleep(50);
				bytesProcessed += numRead;
				numRead = b.Read(readerID, out block, out isLastBlock);
			}
			hashAlgorithm.TransformFinalBlock(block, 0, numRead);
			bytesProcessed += numRead;


			//Handle Ed2k screwup
			if(hashAlgorithm is Ed2k && !((Ed2k)hashAlgorithm).BlueIsRed()) altHash = ((Ed2k)hashAlgorithm).BlueHash;

			if(HashDone != null) HashDone(this, new EventArgs());
			isDone = true;
		}

		private void DoWork2() {
			byte[] block = null;

			while(!b2.EndOfStream(readerID)) {
				block = b2.Read(readerID, true);
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

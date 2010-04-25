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
using System.Linq;
using System.Text;
using System.Threading;
using AVDump2Lib.BlockBuffer;

namespace AVDump2Lib.BlockConsumers.Tools {
	public abstract class BlockConsumerBase : IBlockConsumer {
		private Thread t;
		protected IRefillBuffer<byte[]> b;
		protected int consumerId;

		public event EventHandler ProcessingDone;
		public string Name { get; private set; }
		public long ProcessedBytes { get; protected set; }

		public BlockConsumerBase(string name) {
			t = new Thread(new ThreadStart(DoWorkInternal));
			this.Name = t.Name = name;
		}

		public void Start(IRefillBuffer<byte[]> b, int consumerId) {
			this.b = b;
			this.consumerId = consumerId;
			t.Start();
		}
		public void Join() { t.Join(); }
		public bool HasFinished { get; private set; }

		private void DoWorkInternal() {
			ProcessedBytes = 0;
			HasFinished = false;

			DoWork();

			HasFinished = true;
			if(ProcessingDone != null) ProcessingDone(this, new EventArgs());
		}

		protected abstract void DoWork();
	}
}

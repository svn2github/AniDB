using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AVDump2CL.Misc {
	public class NullStream : Stream {
		private long length;
		public NullStream(long length) {
			this.length = length;
			Position = 0;
		}
		public override bool CanRead { get { return true; } }
		public override bool CanSeek { get { return false; } }
		public override bool CanWrite { get { return false; } }
		public override void Flush() { }
		public override long Length { get { return length; } }
		public override long Position { get; set; }
		public override long Seek(long offset, SeekOrigin origin) { throw new NotImplementedException(); }
		public override void SetLength(long value) { throw new NotImplementedException(); }
		public override void Write(byte[] buffer, int offset, int count) { throw new NotImplementedException(); }
		public override int Read(byte[] buffer, int offset, int count) {
			long bytesRead = (Position + count > Length) ? (Length - Position) : count;
			Position += bytesRead;
			return (int)bytesRead;
		}
	}
	public class RandomStream : Stream {
		private long length;
		private Random rnd = new Random();

		public RandomStream(long length) {
			this.length = length;
			Position = 0;
		}
		public override bool CanRead { get { return true; } }
		public override bool CanSeek { get { return false; } }
		public override bool CanWrite { get { return false; } }
		public override void Flush() { }
		public override long Length { get { return length; } }
		public override long Position { get; set; }
		public override long Seek(long offset, SeekOrigin origin) { throw new NotImplementedException(); }
		public override void SetLength(long value) { throw new NotImplementedException(); }
		public override void Write(byte[] buffer, int offset, int count) { throw new NotImplementedException(); }
		public override int Read(byte[] buffer, int offset, int count) {
			rnd.NextBytes(buffer);

			long bytesRead = (Position + count > Length) ? (Length - Position) : count;
			Position += bytesRead;
			return (int)bytesRead;
		}
	}
}

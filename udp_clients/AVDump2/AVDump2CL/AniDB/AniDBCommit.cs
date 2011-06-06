using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace AVDump2CL.AniDB {
	public sealed class AniDBCommit {
		private AniDBCommit() { }

		public static object Commit() {
			CommitEnvironment commitEnv = new CommitEnvironment();

			return null;
		}


		private class CommitEnvironment {
			public AutoResetEvent WaitHandle;

			public byte RndByte;
			public UdpClient Com;
			public IPEndPoint EndPoint;

			public CommitResult Result;
		}

		private class CommitResult {

		}

		public abstract class CommitBase {
			byte[] payload;
			public byte[] Payload { get { return (byte[])payload.Clone(); } }

			public int Timeout { get; private set; }
			public int LocalPort { get; private set; }
			public IPEndPoint Destination { get; private set; }
		}
	}
}

using System;
using AVDump2Lib.BlockBuffer;

namespace AVDump2Lib.BlockConsumers.Tools {
	public interface IBlockConsumer {
		//event EventHandler ProcessingDone;

		string Name { get; }

		void Start(IRefillBuffer<byte[]> b, int consumerId);
		void Join();
		bool HasFinished { get; }

		Exception Error { get; }

		//object FinishedStatus { get; }

		long ProcessedBytes { get; }
	}
}

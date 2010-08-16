using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2CL.Exceptions;
using AVDump2Lib.BlockConsumers.Tools;
using System.IO;

namespace AVDump2CL {
	public class FileEnvironment {
		public FileProcessingExceptionCollection Exceptions { get; private set; }
		public BlockConsumerContainer Container { get; private set; }
		public FileInfo File { get; private set; }

		public FileEnvironment(Version version, BlockConsumerContainer container, string filePath) {
			File = new FileInfo(filePath);
			Container = container;
			Exceptions = new FileProcessingExceptionCollection(version, filePath);
		}

		public void AddException(string message, Exception innerException) {
			var avd2Ex = new AVD2Exception(DateTime.Now, message, innerException);
			Exceptions.Add(avd2Ex);

			Console.ForegroundColor = ConsoleColor.Red;
			Console.Error.WriteLine(avd2Ex.Message);
			Console.ResetColor();
		}
	}
}

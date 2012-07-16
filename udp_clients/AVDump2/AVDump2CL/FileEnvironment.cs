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

		public DateTime StartedOn { get; private set; }
		public DateTime FileStartedOn { get; private set; }
		public int TotalFiles { get; private set; }
		public int ProcessedFiles { get; private set; }
		public long TotalBytes { get; private set; }
		public long ProcessedBytes { get; private set; }

		public FileEnvironment(Version version, BlockConsumerContainer container, FileInfo fileInfo, DateTime startedOn, int totalFiles, int processedFiles, long totalBytes, long processedBytes) {
			FileStartedOn = DateTime.Now;
			StartedOn = startedOn;

			TotalFiles = totalFiles;
			ProcessedFiles = processedFiles;
			TotalBytes = totalBytes;
			ProcessedBytes = processedBytes;

			File = fileInfo;
			Container = container;
			Exceptions = new FileProcessingExceptionCollection(version, fileInfo != null ? fileInfo.FullName : null);
		}

		public void AddException(string message, Exception innerException) {
			var avd2Ex = new AVD2Exception(DateTime.Now, message, innerException);
			Exceptions.Add(avd2Ex);

			Console.ForegroundColor = ConsoleColor.Red;
			Console.Error.WriteLine(avd2Ex.Message);
			Console.ResetColor();

			if(avd2Ex.InnerException != null) {
				Console.ForegroundColor = ConsoleColor.Red;
				Console.Error.WriteLine(avd2Ex.InnerException.Message);
				Console.ResetColor();
			}
		}

		public override string ToString() {
			return string.Format("(FileEnvironment Path=\"{0}\" Size={1} StartedOn=\"{2}\" Errors={3})", File.FullName, File.Length, StartedOn, Exceptions.Count);
		}
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2GUI.AVD2Proc;
using System.Collections.ObjectModel;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading;
using System.ComponentModel;

namespace AVDump2GUI.AVD2 {
	public static class AVD2Processing {

		public static AVD2Progress ProcessFiles(AVD2Args args) {
			var p = new AVD2Progress();
			var e = new AVD2Environment();

			e.ProcessingThread = new Thread(() => ProcessFiles(e, p, args));
			e.ProcessingThread.Start();

			return p;
		}
		private static void ProcessFiles(AVD2Environment e, AVD2Progress p, AVD2Args args) {
			GetAVD2Files(e, p, args);
			for(int i = 0;i < e.Files.Count;i++) {
				p.CurrentFileIndex = i;
				ProcessFile(e, p, args);
				p.ProcessedFiles++;
			}
		}

		private static void ProcessFile(AVD2Environment e, AVD2Progress p, AVD2Args args) {
			throw new NotImplementedException();
		}

		private static void GetAVD2Files(AVD2Environment e, AVD2Progress p, AVD2Args args) {
			long length = 0;
			List<AVD2File> avd2Files = new List<AVD2File>();
			foreach(var path in args.Paths) GetAVD2Files(avd2Files, path, new Regex(""), args.Settings.DefaultFileStatus(), ref length);
			e.Files = avd2Files;

			p.FileCount = avd2Files.Count;
			p.TotalBytes = length;
		}
		private static void GetAVD2Files(List<AVD2File> avd2Files, string path, Regex regex, AVD2Action action, ref long length) {
			try {
				if(Directory.Exists(path)) {
					var paths = System.IO.Directory.GetFileSystemEntries(path);
					foreach(var subPath in paths) GetAVD2Files(avd2Files, subPath, regex, action, ref length);

				} else if(File.Exists(path) && regex.IsMatch(path)) {
					var fileInfo = new FileInfo(path);
					var avd2File = new AVD2File(fileInfo);

					avd2File.Status.Change(FileStatus.StatusType.Set | FileStatus.StatusType.Todo, action);
					length += fileInfo.Length;
					avd2Files.Add(avd2File);
				}
			} catch(Exception) { }
		}



		public class AVD2Progress : INotifyPropertyChanged {
			public event PropertyChangedEventHandler PropertyChanged;


			public int FileCount { get { return fileCount; } internal set { fileCount = value; OPC("FileCount"); } } private int fileCount;
			public int ProcessedFiles { get { return processedFiles; } internal set { processedFiles = value; OPC("ProcessedFiles"); } } private int processedFiles;

			public long TotalBytes { get { return totalBytes; } internal set { totalBytes = value; /*OPC("TotalBytes");*/ } } private long totalBytes;
			public long ProcessedBytes { get { return processedBytes; } internal set { processedBytes = value; /*OPC("ProcessedBytes");*/ } } private long processedBytes;

			public long TotalFileBytes { get { return totalFileBytes; } internal set { totalFileBytes = value; /*OPC("TotalFileBytes");*/ } } private long totalFileBytes;
			public long ProcessedFileBytes { get { return processedFileBytes; } internal set { processedFileBytes = value; /*OPC("ProcessedFileBytes");*/ } } private long processedFileBytes;

			public int CurrentFileIndex { get { return currentFileIndex; } internal set { currentFileIndex = value; OPC("CurrentFileIndex"); } } private int currentFileIndex;


			protected void OPC(string propertyName){ if(PropertyChanged!= null) PropertyChanged(this, new PropertyChangedEventArgs(propertyName));  }
		}

		public class AVD2Environment {
			public List<AVD2File> Files;
			public Thread ProcessingThread;

		}

		public class AVD2Args {
			public Collection<string> Paths { get; private set; }
			public AVD2Settings Settings { get; private set; }
		}

		public abstract class AVD2Settings {
			public abstract AVD2Action DefaultFileStatus();
		}
		public enum AVD2Status { Stop, Run, Pause }
	}


	public class DynKeyedCollection<K, V> : KeyedCollection<K, V> {
		Func<V, K> getKey;
		public DynKeyedCollection(Func<V, K> getKey) { this.getKey = getKey; }
		protected override K GetKeyForItem(V item) { return getKey(item); }
	}

	public class DynComparer<T> : IComparer<T> {
		private Comparison<T> comparer;
		public DynComparer(Comparison<T> comparer) { this.comparer = comparer; }
		public int Compare(T x, T y) { return comparer(x, y); }
	}
}

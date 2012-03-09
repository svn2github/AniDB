using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AVDump2CL.Misc {
	public static class FileGetter {
		public static void TraverseFiles(string path, bool includeSubFolders, Action<FileInfo> onFile, Action<Exception> onError) {
			try {
				if(File.Exists(path)) {
					onFile(new FileInfo(path));
					return;
				} else {
					TraverseFiles(System.IO.Directory.GetFileSystemEntries(path), includeSubFolders, onFile, onError);
				}
			} catch(Exception ex) {
				onError(ex);
			}
		}

		private static void TraverseFiles(string[] paths, bool includeSubFolders, Action<FileInfo> onFile, Action<Exception> onError) {
			foreach(var path in paths) {
				try {
					if(System.IO.Directory.Exists(path)) {
						if(includeSubFolders) TraverseFiles(System.IO.Directory.GetFileSystemEntries(path), true, onFile, onError);
					} else {
						onFile(new FileInfo(path));
					}
				} catch(Exception ex) { onError(ex); }
			}
		}
	}
}

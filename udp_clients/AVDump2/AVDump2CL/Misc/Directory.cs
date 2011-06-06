using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AVDump2CL.Misc {
	public static class FileGetter {
		public static List<FileInfo> GetFiles(string directory, List<string> filter, bool includeSubFolders) {
			var files = new List<FileInfo>();

			GetFiles(files, System.IO.Directory.GetFileSystemEntries(directory), filter, includeSubFolders);
			return files;
		}
		private static void GetFiles(List<FileInfo> files, string[] paths, List<string> filter, bool includeSubFolders) {
			foreach(var path in paths) {
				string ext = System.IO.Path.GetExtension(path);
				try {
					if(System.IO.Directory.Exists(path)) {
						if(includeSubFolders) GetFiles(files, System.IO.Directory.GetFileSystemEntries(path), filter, true);
					} else if(ext.Length != 0 && (filter == null || filter.BinarySearch(ext.Substring(1).ToLower()) >= 0)) {
						files.Add(new FileInfo(path));
					}
				} catch(Exception) { }
			}
		}
	}
}

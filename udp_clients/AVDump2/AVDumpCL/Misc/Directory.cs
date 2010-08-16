using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2CL.Misc {
	public static class FileGetter {
		public static List<string> GetFiles(string directory, List<string> filter) {
			var files = new List<string>();

			GetFiles(files, System.IO.Directory.GetFileSystemEntries(directory), filter);
			return files;
		}
		private static void GetFiles(List<string> files, string[] paths, List<string> filter) {
			foreach(var path in paths) {
				try {
					if(System.IO.Directory.Exists(path)) {
						GetFiles(files, System.IO.Directory.GetFileSystemEntries(path), filter);
					} else if(filter.BinarySearch(System.IO.Path.GetExtension(path).Substring(1).ToLower()) >= 0) {
						files.Add(path);
					}
				} catch(Exception) { }
			}
		}

	}
}

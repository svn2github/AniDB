using System;
using System.Collections.Generic;
using System.Text;

namespace AVDump2GUI {
	public class FilePath {
		private string fileName;
		private string path;
		private string filePath;

		public FilePath(string filePath) { Path = filePath; }

		public string FileName {
			get { return fileName; }
			set {
				fileName = value;
				filePath = System.IO.Path.Combine(System.IO.Path.GetFullPath(filePath), fileName);
			}
		}

		public string DirPath {
			get { return path; }
			set {
				path = value;
				filePath = System.IO.Path.Combine(path, System.IO.Path.GetFileName(filePath));
			}
		}

		public string Path {
			get { return filePath; }
			set {
				filePath = value;
				fileName = System.IO.Path.GetFileName(filePath);
				path = System.IO.Path.GetDirectoryName(filePath);
			}
		}
	}
}

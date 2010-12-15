using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Collections.ObjectModel;
using System.IO;
using AVDump2Lib.Misc;
using System.Xml;

namespace AVDump2CL.Exceptions {
	public class FileProcessingExceptionCollection : Collection<AVD2Exception> {
		protected override void ClearItems() { throw new NotSupportedException(); }
		protected override void SetItem(int index, AVD2Exception item) { throw new NotSupportedException(); }
		protected override void RemoveItem(int index) { throw new NotSupportedException(); }

		private string filePath;
		private Version version;

		public FileProcessingExceptionCollection(Version version, string filePath) {
			this.filePath = filePath;
			this.version = version;
		}


		public void Save(string destPath) {
			XElement infoElem, exElem;
			XElement fileElem = new XElement("FileExceptions", infoElem = new XElement("Information"), exElem = new XElement("Exceptions"));
			infoElem.Add(new XElement("AVDump2CLVersion", version.ToString()));
			if(!string.IsNullOrEmpty(filePath)) infoElem.Add(new XElement("File", filePath));

			foreach(var item in Items) exElem.Add(item.ToXElement());

			if(!Directory.Exists(destPath)) Directory.CreateDirectory(destPath);
			string fileName = "Err " + DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + ".xml";

			using(var writer = new SafeXmlWriter(Path.Combine(destPath, fileName), Encoding.Unicode)) fileElem.Save(writer);
		}

	}
}

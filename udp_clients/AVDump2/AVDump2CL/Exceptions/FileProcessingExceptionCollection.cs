﻿using System;
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


		public XElement ToXElement(bool addPath) {
			XElement infoElem, exElem;
			XElement fileElem = new XElement("FileExceptions", infoElem = new XElement("Information"), exElem = new XElement("Exceptions"));
			infoElem.Add(new XElement("AVDump2CLVersion", version.ToString()));
			infoElem.Add(new XElement("RunOnBitness", IntPtr.Size * 8));
			//infoElem.Add(new XElement("Framework", Environment.Version));
			infoElem.Add(new XElement("OS",Environment.OSVersion.VersionString));
			if(addPath && !string.IsNullOrEmpty(filePath)) {
				infoElem.Add(new XElement("File", filePath));
			}
			foreach(var item in Items) exElem.Add(item.ToXElement());

			return fileElem;
		}
	}
}

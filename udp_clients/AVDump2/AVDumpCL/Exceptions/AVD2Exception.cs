using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using AVDump2Lib.Misc;
using System.Collections;

namespace AVDump2CL.Exceptions {
	[Serializable]
	public class AVD2Exception : Exception {
		public DateTime ThrownOn { get; private set; }

		public AVD2Exception(DateTime thrownOn) { ThrownOn = thrownOn; }
		public AVD2Exception(DateTime thrownOn, string message) : base(message) { ThrownOn = thrownOn; }
		public AVD2Exception(DateTime thrownOn, string message, Exception inner) : base(message, inner) { ThrownOn = thrownOn; }
		protected AVD2Exception(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context) : base(info, context) { }

		public XElement ToXElement() {
			XElement exElem = new XElement(GetType().ToString());
			exElem.SetAttributeValue("thrownOn", ThrownOn.ToString("yyyyMMdd HH.mm.ss.ffff"));
			if(Message != null) exElem.Add(new XElement("Message", Message));
			if(StackTrace != null) { exElem.Add(new XElement("StackTrace", from frame in StackTrace.Split('\n') let prettierFrame = frame.Trim() select new XElement("Frame", prettierFrame))); }
			if(Data.Count > 0) exElem.Add(new XElement("Data", from entry in Data.Cast<DictionaryEntry>() let key = entry.Key.ToString() let value = (entry.Value == null) ? "null" : entry.Value.ToString() select new XElement(key, value)));
			if(InnerException != null) exElem.Add(new ExceptionXElement(InnerException));
			return exElem;
		}
	}
}

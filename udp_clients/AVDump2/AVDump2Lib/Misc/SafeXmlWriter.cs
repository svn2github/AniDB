using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace AVDump2Lib.Misc {
	public class SafeXmlWriter : XmlTextWriter {
		public SafeXmlWriter(TextWriter tw) : base(tw) { Formatting = Formatting.Indented; }
		public SafeXmlWriter(Stream stream, Encoding encoding) : base(stream, encoding) { Formatting = Formatting.Indented; }
		public SafeXmlWriter(string filename, Encoding encoding) : base(filename, encoding) { Formatting = Formatting.Indented; }

		private StringBuilder sb = new StringBuilder();
		public override void WriteString(string text) {
			foreach(var character in text) if(IsLegalXmlChar(character)) sb.Append(character);
			base.WriteString(sb.ToString());
			sb.Length = 0;
		}

		public static bool IsLegalXmlChar(int character) { return (character == 0x9 || character == 0xA || character == 0xD || (character >= 0x20 && character <= 0xD7FF) || (character >= 0xE000 && character <= 0xFFFD) || (character >= 0x10000 && character <= 0x10FFFF)); }
	}
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace AVDump2Lib.Misc {
	public class ConsoleOutFilter : TextWriter {
		private TextWriter textWriter;

		public ConsoleOutFilter(TextWriter textWriter) { this.textWriter = textWriter; }

		public override void Write(char value) { if(value != '•') textWriter.Write(value); }
		public override void Write(string value) { textWriter.Write(value.Replace("•", "")); }
		public override void WriteLine(char[] buffer) { textWriter.WriteLine((new String(buffer)).Replace("•", "")); }
		public override void WriteLine(string value) { textWriter.WriteLine(value.Replace("•", "")); }
		public override void WriteLine() { textWriter.WriteLine(); }

		public override void Write(char[] buffer, int index, int count) { textWriter.Write(buffer, index, count); }
		public override void Write(char[] buffer) { textWriter.Write(buffer); }
		public override void Write(object value) { textWriter.Write(value); }
		public override void Write(string format, object arg0) { textWriter.Write(format, arg0); }
		public override void Write(string format, object arg0, object arg1) { textWriter.Write(format, arg0, arg1); }
		public override void Write(string format, object arg0, object arg1, object arg2) { textWriter.Write(format, arg0, arg1, arg2); }
		public override void Write(string format, params object[] arg) { textWriter.Write(format, arg); }
		public override void WriteLine(char value) { textWriter.WriteLine(value); }
		public override void WriteLine(char[] buffer, int index, int count) { textWriter.WriteLine(buffer, index, count); }
		public override void WriteLine(object value) { textWriter.WriteLine(value); }
		public override void WriteLine(string format, object arg0) { textWriter.WriteLine(format, arg0); }
		public override void WriteLine(string format, object arg0, object arg1) { textWriter.WriteLine(format, arg0, arg1); }
		public override void WriteLine(string format, object arg0, object arg1, object arg2) { textWriter.WriteLine(format, arg0, arg1, arg2); }
		public override void WriteLine(string format, params object[] arg) { textWriter.WriteLine(format, arg); }

		public override Encoding Encoding { get { throw new NotImplementedException(); } }
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.ObjectModel;

namespace AVDump2CL.Misc {
	public static class ArgsParser {
		public static List<string> Parse(string cmdl) {
			Environment e = new Environment(cmdl);
			List<string> args = new List<string>();

			while(!e.EOL()) {
				if(e.Peek() == '-') {
					args.Add(ParseArg(e));
				} else {
					args.Add(ParseName(e, e.Peek() == '"'));
				}
				e.SkipEmpty();
			}

			return args;
		}

		private static string ParseArg(Environment e) {
			e.Check(e.Peek() == '-', "Invalid beginning of Key construct");
			return ParseName(e, false);
		}
		private static string ParseName(Environment e, bool isQuoted) {
			string name = "";

			if(isQuoted) e.Check("\"", "Invalid beginning of a 'Name' construct");
			while(!e.EOL() && (isQuoted || e.Peek() != ' ') && (!isQuoted || e.Peek() != '"')) name += e.Read().ToString();
			if(isQuoted) e.Check("\"", "Invalid ending of a 'Name' construct");

			return name;
		}
		private class Environment {
			public int CharIndex { get; set; }
			public int LineCount { get { return src.Length; } }

			string src;

			public Environment(string src) { this.src = src; }

			#region Help Methods
			public char Peek() { return src[CharIndex]; }
			public char Read() {
				char srcChar = Peek();

				Advance();
				return srcChar;
			}
			public string ReadToEnd() {
				string val = src.Substring(CharIndex);
				CharIndex = src.Length;
				return val;
			}
			public void SkipEmpty() { while(!EOL() && Peek() == ' ') { Advance(); } }
			public void Advance() { CharIndex++; }
			public int Count() { return src.Length; }
			public bool EOL() { return Count() == CharIndex; }
			public void Check(string match, string errorMsg) {
				bool isInRange = CharIndex + match.Length <= Count();
				bool isEqual = isInRange && match.Equals(src.Substring(CharIndex, match.Length));
				if(!isInRange || !isEqual) {
					throw new Exception(errorMsg);
				} else if(isInRange) {
					CharIndex += match.Length;
				} else {
					CharIndex = Count();
				}
			}
			public void Check(bool isValid, string errorMsg) { if(!isValid) throw new Exception(errorMsg); }
			public void Throw(string errorMsg) { throw new Exception(errorMsg); }
			#endregion
		}
	}
}

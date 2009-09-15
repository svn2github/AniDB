using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVDump2Lib.Information.Parser.SubtitleInfo {
	public class IDX {
		private Dictionary<string, string> info;
		private Subs[] subtitles;

		public IDX(string source) {
			info = new Dictionary<string, string>();
			Parse(source);
		}

		private static Regex tagRegex = new Regex(@"^([^#]+?)\:[ ]?" + "([^\r\n]+)", RegexOptions.Compiled | RegexOptions.ECMAScript | RegexOptions.Multiline);
		private void Parse(string source) {
			MatchCollection matches = tagRegex.Matches(source);

			Match match, subMatch;

			int index;
			string language;
			string languageId;
			List<DateTime> sub = null;
			List<Subs> subs = new List<Subs>();

			for(int i = 0;i < matches.Count;i++) {
				match = matches[i];

				string key = match.Groups[1].Value;
				if(key.Equals("timestamp")) {
					sub.Add(DateTime.ParseExact(Regex.Match(match.Groups[2].Value, "([^,]+)").Groups[1].Value, "HH:mm:ss:FFF", null));

				} else if(key.Equals("id")) {
					subMatch = Regex.Match(match.Groups[2].Value, @"([^,]+), index\: (.+)");
					if(!subMatch.Success) throw new Exception();

					languageId = subMatch.Groups[1].Value;
					if(!int.TryParse(subMatch.Groups[2].Value, out index)) throw new Exception();

					language = source.Substring(match.Index, matches[i + 1].Index - match.Index);
					language = Regex.Match(language, @"alt\: " + "([^\r\n]+)").Groups[1].Value;

					sub = new List<DateTime>();
					subs.Add(new Subs(index, languageId, language, sub));

				} else {
					info[key] = match.Groups[2].Value;
				}
			}
		}

		public class Subs {
			private int index;
			private string languageId;
			private string language;

			private List<DateTime> subtitles;

			internal Subs(int index, string langId, string lang, List<DateTime> subs) {
				this.index = index;
				this.language = lang;
				this.languageId = langId;
				this.subtitles = subs;
			}
		}
	}
}

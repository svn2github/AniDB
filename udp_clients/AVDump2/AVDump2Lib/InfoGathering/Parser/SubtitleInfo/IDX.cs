// Copyright (C) 2009 DvdKhl 
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.using System;

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVDump2Lib.InfoGathering.Parser.SubtitleInfo {
	public class IDX {
		private Dictionary<string, string> info;
		private Subs[] subtitles;

		public Subs[] Subtitles {
			get { return (Subs[])subtitles.Clone(); }
		}

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
			TimeSpan? delay = null;
			List<TimeSpan> sub = null;
			List<Subs> subs = new List<Subs>();

			for(int i = 0;i < matches.Count;i++) {
				match = matches[i];

				string key = match.Groups[1].Value.Trim();
				if(key.Equals("timestamp")) {
					try {
						var timestampstr = Regex.Match(match.Groups[2].Value, "([^,]+)").Groups[1].Value;
						timestampstr = timestampstr.Substring(0, timestampstr.LastIndexOf(':')) + "." + timestampstr.Substring(timestampstr.LastIndexOf(':') + 1);
						sub.Add(TimeSpan.Parse(timestampstr).Add(delay != null ? delay.Value : TimeSpan.FromTicks(0)));

					} catch(Exception ex) { }

				} else if(key.Equals("Delay")) {
					var timestampstr = Regex.Match(match.Groups[2].Value, "([^,]+)").Groups[1].Value;
					timestampstr = timestampstr.Substring(0, timestampstr.LastIndexOf(':')) + "." + timestampstr.Substring(timestampstr.LastIndexOf(':') + 1);
					delay = TimeSpan.Parse(timestampstr);

				} else if(key.Equals("id")) {
					subMatch = Regex.Match(match.Groups[2].Value, @"([^,]+), index\: (.+)");
					if(!subMatch.Success) throw new Exception();

					languageId = subMatch.Groups[1].Value;
					if(!int.TryParse(subMatch.Groups[2].Value, out index)) throw new Exception();

					language = source.Substring(match.Index, (i + 1 < matches.Count ? matches[i + 1].Index : source.Length) - match.Index);
					language = Regex.Match(language, @"alt\: " + "([^\r\n]+)").Groups[1].Value;
					delay = null;

					sub = new List<TimeSpan>();
					subs.Add(new Subs(index, languageId, language, sub));

				} else {
					info[key] = match.Groups[2].Value;
				}
			}

			subtitles = subs.ToArray();
		}

		public class Subs {
			public int index { get; private set; }
			public string languageId { get; private set; }
			public string language { get; private set; }

			private List<TimeSpan> subtitles;

			public int SubtitleCount { get { return subtitles.Count; } }

			internal Subs(int index, string langId, string lang, List<TimeSpan> subs) {
				this.index = index;
				this.language = lang;
				this.languageId = langId;
				this.subtitles = subs;
			}
		}
	}
}

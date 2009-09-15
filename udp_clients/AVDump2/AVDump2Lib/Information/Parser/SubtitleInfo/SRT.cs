using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVDump2Lib.Information.Parser.SubtitleInfo {
	public class SRT {
		private static Regex regexParse = new Regex(@"(?<sequence>\d+)\n(?<start>\d{2}\:\d{2}\:\d{2},\d{3}) --\> (?<end>\d{2}\:\d{2}\:\d{2},\d{3})\n(?<text>[\s\S]*?\n\n)", RegexOptions.Compiled | RegexOptions.ECMAScript);
		private Sub[] subtitles;

		public SRT(string source) { Parse(source); }
		private void Parse(string srtSource) {
			if(srtSource.Contains("\r\n") || srtSource.Contains("\r")) {
				srtSource = srtSource.Replace('\r', '\n');
				srtSource = srtSource.Replace("\n\n", "\n");
			}

			MatchCollection matches = regexParse.Matches(srtSource);

			int index;
			string msg;
			DateTime start, end;
			List<Sub> subtitles = new List<Sub>();
			foreach(Match match in matches) {
				index = int.Parse(match.Groups["sequence"].Value);
				start = DateTime.ParseExact(match.Groups["start"].Value, "HH:mm:ss,FFF", null);
				end = DateTime.ParseExact(match.Groups["end"].Value, "HH:mm:ss,FFF", null);
				msg = match.Groups["text"].Value;
				subtitles.Add(new Sub(index, start, end, msg));
			}

			this.subtitles = subtitles.ToArray();
		}

		public class Sub {
			public Sub(int index, DateTime start, DateTime end, string text) {
				Index = index;
				Start = new DateTime(start.Ticks);
				End = new DateTime(end.Ticks);
				Text = text;
			}

			public int Index { get; private set; }
			public DateTime Start { get; private set; }
			public DateTime End { get; private set; }
			public string Text { get; private set; }
		}

		public int Count { get { return subtitles.Length; } }
		public Sub this[int index] { get { return subtitles[index]; } }
	}
}

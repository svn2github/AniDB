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

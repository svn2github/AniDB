using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Text.RegularExpressions;

namespace AVDump2Lib.Information.Parser.SubtitleInfo {
	public class SSA_ASS {
		private ScriptInfo scriptInfo;
		private Styles styles;
		private Events events;

		public SSA_ASS(string source) {
			Parse(source);
		}
		private void Parse(string source) {
			List<int> catPos = new List<int>();
			MatchCollection matches = categoryParser.Matches(source);
			foreach(Match match in matches) catPos.Add(match.Index);
			catPos.Add(source.Length - 1);

			styles = null;
			scriptInfo = null;
			events = null;

			string catContent;
			for(int i = 0;i < catPos.Count - 1;i++) {
				catContent = source.Substring(catPos[i], catPos[i + 1] - catPos[i]);
				if(catContent.StartsWith("[Script Info]")) {
					scriptInfo = new ScriptInfo(catContent);
				} else if(catContent.StartsWith("[V4+ Styles]") || catContent.StartsWith("[V4 Styles]")) {
					if(scriptInfo == null) throw new Exception();
					styles = new Styles(catContent, scriptInfo.ScriptType);
				} else if(catContent.StartsWith("[Events]")) {
					if(scriptInfo == null) throw new Exception();
					events = new Events(catContent, scriptInfo.ScriptType);
				} else if(catContent.StartsWith("[Fonts]")) {
				} else if(catContent.StartsWith("[Graphics]")) {

				}
			}
		}

		private static Regex categoryParser = new Regex(@"^\[([^\]]+)\]" + "\r?$", RegexOptions.Compiled | RegexOptions.ECMAScript | RegexOptions.Multiline);
		internal static string[][] ParseCategory(string source, string catNameRegex, Type enumType, int prefixId) {
			Match match = Regex.Match(source, @"\A" + catNameRegex, RegexOptions.ECMAScript | RegexOptions.Multiline);
			if(!match.Success) throw new Exception();


			match = Regex.Match(source, "^Format:[ ]?([^\r\n]+)", RegexOptions.ECMAScript | RegexOptions.Multiline);
			string[] formatEntries = match.Groups[1].Value.Replace(" ", "").Split(new char[] { ',' });
			int[] styleEntries = new int[formatEntries.Length];

			for(int i = 0;i < formatEntries.Length;i++) {
				if(Enum.IsDefined(enumType, formatEntries[i])) styleEntries[i] = (int)Enum.Parse(enumType, formatEntries[i]);
			}

			string regexStr = @"^([^\:;]+)\:[ ]?";
			for(int i = 0;i < formatEntries.Length;i++) {
				if(i == 0) {
					regexStr += "([^,]*)";
				} else if(i < formatEntries.Length - 1) {
					regexStr += ",([^,]*)";
				} else {
					regexStr += ",([^,\r\n]*)";
				}
			}
			Regex regexParser = new Regex(regexStr, RegexOptions.Compiled | RegexOptions.ECMAScript | RegexOptions.Multiline);


			string[] fields;
			int fieldCount = Enum.GetNames(enumType).Length;
			MatchCollection matches = regexParser.Matches(source, match.Index + match.Length);

			string[][] dataRows = new string[matches.Count][];
			for(int i = 0;i < matches.Count;i++) {
				fields = new string[fieldCount];

				match = matches[i];
				fields[prefixId] = match.Groups[1].Value;
				for(int j = 2;j < match.Groups.Count;j++) {
					fields[styleEntries[j - 2]] = match.Groups[j].Value;
				}

				dataRows[i] = fields;
			}

			return dataRows;
		}


		public class Events {
			Event[] events;

			internal Events(string eventSource, eVersion ver) {
				string[][] data = ParseCategory(eventSource, @"\[Events\]", typeof(eEventFormat), (int)eEventFormat.Prefix);

				events = new Event[data.Length];
				if((ver & eVersion.v4ASS) != 0) {
					for(int i = 0;i < data.Length;i++) events[i] = new Event_ASS(data[i]);
				} else {
					for(int i = 0;i < data.Length;i++) events[i] = new Event_SSA(data[i]);
				}
			}
		}
		public abstract class Event {
			protected string[] eventData;

			protected Event() { }
			protected Event(string[] eventData) { this.eventData = eventData; }
		}
		public class Event_SSA : Event {
			public Event_SSA(string[] eventData) : base(eventData) { }
		}
		public class Event_ASS : Event {
			public Event_ASS(string[] eventData) : base(eventData) { }
		}

		public class Styles {
			Style[] styles;

			internal Styles(string styleSource, eVersion ver) {
				string[][] data = SSA_ASS.ParseCategory(styleSource, @"\[V4[\+]? Styles\]", typeof(eStyleFormat), (int)eStyleFormat.Prefix);

				styles = new Style[data.Length];

				if((ver & eVersion.v4ASS) != 0) {
					for(int i = 0;i < data.Length;i++) styles[i] = new Style_ASS(data[i]);
				} else {
					for(int i = 0;i < data.Length;i++) styles[i] = new Style_SSA(data[i]);
				}
			}
		}
		public abstract class Style {
			protected string[] styleData;

			protected Style() { }
			protected Style(string[] styleData) { this.styleData = styleData; }


			/*public string Name { get { return (string)info[(int)eInfo.Name]; } }
			public string FontName { get { return (string)info[(int)eInfo.FontName]; } }
			public int FontSize { get { return (int)info[(int)eInfo.FontSize]; } }
			public Color PrimaryColour { get { return (Color)info[(int)eInfo.PrimaryColour]; } }
			public Color SecondaryColour { get { return (Color)info[(int)eInfo.SecondaryColour]; } }
			public Color BackColour { get { return (Color)info[(int)eInfo.BackColour]; } }
			public bool Bold { get { return (bool)info[(int)eInfo.Bold]; } }
			public bool Italic { get { return (bool)info[(int)eInfo.Italic]; } }
			public eBorderStyle BorderStyle { get { return (eBorderStyle)info[(int)eInfo.BorderStyle]; } }
			public int Outline { get { return (Color)info[(int)eInfo.Outline]; } }
			public int Shadow { get { return (int)info[(int)eInfo.Shadow]; } }
			public int MarginL { get { return (int)info[(int)eInfo.MarginL]; } }
			public int MarginR { get { return (int)info[(int)eInfo.MarginR]; } }
			public int MarginV { get { return (int)info[(int)eInfo.MarginV]; } }
			public string Encoding { get { return (string)info[(int)eInfo.Encoding]; } }*/
		}
		public class Style_SSA : Style {
			/*public Color TertiaryColour { get { return (Color)info[(int)eInfo.TertiaryColour]; } }
			public eAlignment Alignment { get { return (eAlignment)info[(int)eInfo.Alignment]; } }
			public int AlphaLevel { get { return (int)info[(int)eInfo.AlphaLevel]; } }*/

			public Style_SSA(string[] styleData) : base(styleData) { }
			public enum eAlignment { Left = 1, Center = 2, Right = 3, Top = 1 << 3, Middle = 1 << 4 }
		}
		public class Style_ASS : Style {
			/*public Color OutlineColor { get { return (Color)info[(int)eInfo.OutlineColor]; } }
			public bool Underline { get { return (bool)info[(int)eInfo.Underline]; } }
			public bool StrikeOut { get { return (bool)info[(int)eInfo.StrikeOut]; } }
			public double ScaleX { get { return (double)info[(int)eInfo.ScaleX]; } }
			public double ScaleY { get { return (double)info[(int)eInfo.ScaleY]; } }
			public int Spacing { get { return (int)info[(int)eInfo.Spacing]; } }
			public double Angle { get { return (double)info[(int)eInfo.Angle]; } }
			public eAlignment Alignment { get { return (eAlignment)info[(int)eInfo.Alignment]; } }*/

			public Style_ASS(string[] styleData) : base(styleData) { }
			public enum eAlignment { BLeft = 1, BCenter = 2, BRight = 3, MLeft = 4, MCenter = 5, MRight = 6, TLeft = 7, TCenter = 8, TRight = 9 }
		}

		public class ScriptInfo {
			string[] scriptInfo;

			#region Properties
			public string Title { get; private set; }
			public string OriginalScript { get; private set; }
			public string OriginalTranslation { get; private set; }
			public string OriginalEditing { get; private set; }
			public string OriginalTiming { get; private set; }
			public string SynchPoint { get; private set; }
			public string ScriptUpdatedBy { get; private set; }
			public string UpdateDetails { get; private set; }
			public eVersion ScriptType { get; private set; }
			public eCollisionType Collisisions { get; private set; }
			public int PlayResX { get; private set; }
			public int PlayResY { get; private set; }
			public int PlayResDepth { get; private set; }
			public double Timer { get; private set; }
			#endregion

			internal ScriptInfo(string scriptInfoSource) {
				Parse(scriptInfoSource);
			}

			private void Parse(string source) {
				if(!source.StartsWith("[Script Info]")) throw new Exception();

				string regexStr = "(^[^;][^:]*):([^\r\n]+)";
				Regex regexParser = new Regex(regexStr, RegexOptions.Compiled | RegexOptions.ECMAScript | RegexOptions.Multiline);
				MatchCollection matches = regexParser.Matches(source, 0);

				string key;
				Match match;
				Type eScriptInfoType = typeof(eScriptInfo);
				scriptInfo = new string[Enum.GetNames(typeof(eScriptInfo)).Length];
				for(int i = 0;i < matches.Count;i++) {
					match = matches[i];
					key = match.Groups[1].Value.Replace(" ", "");

					if(Enum.IsDefined(eScriptInfoType, key)) {
						scriptInfo[(int)Enum.Parse(eScriptInfoType, key)] = match.Groups[2].Value;
					}
				}
			}

		}
		public class ScriptInfo_SSA : ScriptInfo {
			public ScriptInfo_SSA(string scriptInfoSource) : base(scriptInfoSource) { }
		}
		public class ScriptInfo_ASS : ScriptInfo {
			public ScriptInfo_ASS(string scriptInfoSource) : base(scriptInfoSource) { }
			public eWrapStyle WrapStyle { get; private set; }
		}


		public enum eVersion {
			v4 = 4,

			SSA = 1 << 31,
			ASS = 1 << 32,

			v4SSA = v4 | SSA,
			v4ASS = v4 | ASS
		}
		public enum eCollisionType { Normal, Reverse }
		public enum eWrapStyle { Smart, EndOfLine, NoWordWrap, SmartWider }
		public enum eBorderStyle { OutlineDropShadow = 1, Opaque = 3 };
		public enum eEventFormat { Prefix, Marked, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text }
		public enum eStyleFormat : int { Prefix, Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, TertiaryColour, OutlineColour, BackColour, Bold, Italic, BorderStyle, Outline, Underline, StrikeOut, Shadow, Alignment, ScaleX, ScaleY, Spacing, Angle, MarginL, MarginR, MarginV, AlphaLevel, Encoding }
		public enum eScriptInfo { Title, OriginalScript, OriginalTranslation, OriginalEditing, OriginalTiming, SynchPoint, ScriptUpdatedBy, UpdateDetails, ScriptType, Collisisions, PlayResX, PlayResY, PlayResDepth, Timer, WrapStyle }
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;
using System.Globalization;
using AVDump2Lib.Misc;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class MediaInfoProvider : InfoProviderBase {
		public MediaInfo MIL { get; private set; }

		public MediaInfoProvider(string filePath) {
			infos = new InfoCollection();

			Func<string, string, string> nonEmpty = (a, b) => string.IsNullOrEmpty(a) ? b : a;

			MIL = new MediaInfo();
			MIL.Option("Internet", "No");
			MIL.Open(filePath);

			Add(EntryKey.Size, Get("FileSize"), "byte");
			Add(EntryKey.Duration, Get("Duration", str => (double.Parse(str, CultureInfo.InvariantCulture) / 1000).ToString("0.000", CultureInfo.InvariantCulture)), "s");
			Add(EntryKey.FileExtension, Get("FileExtension") != null ? Get("FileExtension").ToLower() : null, null);
			Add(EntryKey.WritingApp, Get("Encoded_Application"), null);
			Add(EntryKey.MuxingApp, Get("Encoded_Library"), null);

			StreamType st;
			int streamCount;
			int[] indeces = new int[3];
			Converter<eStreamType, StreamType> streamConverter = a => a == eStreamType.Video ? StreamType.Video : (a == eStreamType.Audio ? StreamType.Audio : (a == eStreamType.Text ? StreamType.Text : (StreamType)(-1)));
			foreach(eStreamType streamKind in Enum.GetValues(typeof(eStreamType))) {
				streamCount = MIL.Count_Get(streamKind);
				st = streamConverter(streamKind);
				if((int)st == -1) continue;
				for(int i = 0;i < streamCount;i++) {
					Add(st, i, EntryKey.Size, Get(streamKind, i, "StreamSize"), "byte");
					Add(st, i, EntryKey.Title, Get(streamKind, i, "Title"), null);
					Add(st, i, EntryKey.Id, Get(streamKind, i, "UniqueID"), null);
					Add(st, i, EntryKey.Language, Get(streamKind, i, "Language"), null);
					Add(st, i, EntryKey.Duration, Get(streamKind, i, "Duration", str => (double.Parse(str.Split('/')[0], CultureInfo.InvariantCulture) / 1000).ToString("0.000", CultureInfo.InvariantCulture)), "s");
					Add(st, i, EntryKey.Bitrate, Get(streamKind, i, "BitRate"), null);
					Add(st, i, EntryKey.CodecId, ((Get(streamKind, i, "Format") + " -- " + nonEmpty(Get(streamKind, i, "Format_Version"), Get(streamKind, i, "CodecID"))).Trim() + " -- " + Get(streamKind, i, "Format_Profile")).Trim(), null);
					Add(st, i, EntryKey.CodecIdAlt, Get(streamKind, i, "CodecID"), null);
					Add(st, i, EntryKey.EncodeSettings, Get(streamKind, i, "Encoded_Library_Settings"), null);
					Add(st, i, EntryKey.EncodeLibrary, Get(streamKind, i, "Encoded_Library"), null);
					Add(st, i, EntryKey.BitrateMode, Get(streamKind, i, "BitRate_Mode"), null);

					switch(streamKind) {
						case eStreamType.Video:
							Add(st, i, EntryKey.FrameRate, Get(streamKind, i, "FrameRate"), null);
							Add(st, i, EntryKey.MaxFrameRate, Get(streamKind, i, "FrameRate_Maximum"), null);
							Add(st, i, EntryKey.MinFrameRate, Get(streamKind, i, "FrameRate_Minimum"), null);
							Add(st, i, EntryKey.VFR, Get(streamKind, i, "FrameRate_Mode").Contains("VFR") ? Get(streamKind, i, "FrameRate") : null, null);
							Add(st, i, EntryKey.FrameCount, Get(streamKind, i, "FrameCount"), null);
							Add(st, i, EntryKey.Width, Get(streamKind, i, "Width"), null);
							Add(st, i, EntryKey.Height, Get(streamKind, i, "Height"), null);
							Add(st, i, EntryKey.DAR, Get(streamKind, i, "DisplayAspectRatio"), null);
							indeces[0]++;
							break;
						case eStreamType.Audio:
							Add(st, i, EntryKey.SamplingRate, Get(streamKind, i, "SamplingRate"), null);
							Add(st, i, EntryKey.SampleCount, Get(streamKind, i, "SamplingCount"), null);
							Add(st, i, EntryKey.ChannelCount, Get(streamKind, i, "Channel(s)"), null);
							indeces[1]++;
							break;
						case eStreamType.Text:
							indeces[2]++;
							break;
					}
				}
			}

			string milInfo = Get("Format/Extensions") != null ? Get("Format/Extensions").ToLower() : "";
			string fileExt = System.IO.Path.GetExtension(filePath).Substring(1).ToLower();
			if(milInfo.Contains("asf") && milInfo.Contains("wmv") && milInfo.Contains("wma")) {
				if(indeces[0] == 0 && indeces[1] != 0 && indeces[2] == 0) {
					Add(EntryKey.Extension, "wma", null);
				} else {
					Add(EntryKey.Extension, "wmv", null);
				}
			} else if(milInfo.Contains("ts") && milInfo.Contains("m2t")) {
				if(System.IO.Path.GetExtension(filePath).Equals(".ts")) Add(EntryKey.Extension, "ts", null); //Blame worf

			} else if(milInfo.Contains("mpeg") && milInfo.Contains("mpg")) {
				if(indeces[0] == 0 || indeces[1] == 0 && indeces[1] != 0) {
					Add(EntryKey.Extension, "sub", null);
				} else {
					Add(EntryKey.Extension, fileExt.Equals("mpeg") ? "mpeg" : "mpg", null);
				}
			} else if((milInfo.Contains("mp1") && milInfo.Contains("mp2") && milInfo.Contains("mp3")) || milInfo.Contains("wav")) {
				switch(Get(eStreamType.Audio, 0, "Format_Profile")) {
					case "Layer 1": Add(EntryKey.Extension, "mp1", null); break;
					case "Layer 2": Add(EntryKey.Extension, "mp2", null); break;
					case "Layer 3": Add(EntryKey.Extension, "mp3", null); break;
				}

			} else if(milInfo.Contains("mp4") && milInfo.Contains("m4a") && milInfo.Contains("m4v")) {
				if(indeces[2] != 0 || (indeces[0] != 0 && indeces[1] != 0)) {
					Add(EntryKey.Extension, "mp4", null);
				} else if(indeces[0] != 0 && indeces[1] == 0) {
					Add(EntryKey.Extension, "m4v", null);
				} else if(indeces[0] == 0 && indeces[1] != 0) {
					Add(EntryKey.Extension, "m4a", null);
				}
			}

			if(this[StreamType.General, 0, EntryKey.Extension] == null) {
				if(milInfo.Contains(fileExt)) {
					Add(EntryKey.Extension, fileExt, null);
				} else {
					Add(EntryKey.Extension, milInfo, null);
				}
			}
		}


		private string Get(eStreamType type, int index, string key, Converter<string, string> converter = null) {
			string value = MIL.Get(type, index, key);
			if(string.IsNullOrEmpty(value)) return "";
			return converter != null ? converter(value) : value;
		}
		private string Get(string key, Converter<string, string> converter = null) { return Get(eStreamType.General, 0, key, converter); }

		protected override void Add(StreamType type, int index, EntryKey entry, string value, string unit) {
			if(value == null) return;

			bool isValid = true;
			foreach(var character in value) {
				isValid = SafeXmlWriter.IsLegalXmlChar(character);
				if(!isValid) break;
			}

			if(isValid) base.Add(type, index, entry, value, unit);
		}


		public override void Dispose() { MIL.Close(); }
	}
}

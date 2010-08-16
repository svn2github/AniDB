using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;
using System.Globalization;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class MediaInfoProvider : InfoProviderBase {
		public MediaInfo MIL { get; private set; }

		public MediaInfoProvider(string filePath) {
			infos = new InfoCollection();

			Func<string, string, string> nonEmpty = (a, b) => { return string.IsNullOrEmpty(a) ? b : a; };

			MIL = new MediaInfo();
			MIL.Option("Internet", "No");
			MIL.Open(filePath);

			Add(EntryKey.Size, Get("FileSize"), "byte");
			Add(EntryKey.Duration, Get("Duration", (str) => { return (double.Parse(str, CultureInfo.InvariantCulture) / 1000).ToString("0.000", CultureInfo.InvariantCulture); }), "s");
			//Add(EntryKey.Extension, Get("Format/Extensions"), null);
			Add(EntryKey.Extension, Get("FileExtension"), null);
			Add(EntryKey.WritingApp, Get("Encoded_Application"), null);
			Add(EntryKey.MuxingApp, Get("Encoded_Library"), null);

			StreamType st;
			int streamCount;
			Converter<eStreamType, StreamType> streamConverter = (a) => { return a == eStreamType.Video ? StreamType.Video : (a == eStreamType.Audio ? StreamType.Audio : (a == eStreamType.Text ? StreamType.Text : (StreamType)(-1))); };
			foreach(eStreamType streamKind in Enum.GetValues(typeof(eStreamType))) {
				streamCount = MIL.Count_Get(streamKind);
				st = streamConverter(streamKind);
				if((int)st == -1) continue;
				for(int i = 0;i < streamCount;i++) {
					Add(st, i, EntryKey.Size, Get(streamKind, i, "StreamSize"), "byte");
					Add(st, i, EntryKey.Title, Get(streamKind, i, "Title"), null);
					Add(st, i, EntryKey.Id, Get(streamKind, i, "UniqueID"), null);
					Add(st, i, EntryKey.Language, Get(streamKind, i, "Language"), null);
					Add(st, i, EntryKey.Duration, Get(streamKind, i, "Duration", (str) => { return (double.Parse(str, CultureInfo.InvariantCulture) / 1000).ToString("0.000", CultureInfo.InvariantCulture); }), "s");
					Add(st, i, EntryKey.Bitrate, Get(streamKind, i, "BitRate"), null);
					Add(st, i, EntryKey.FourCC, nonEmpty(Get(streamKind, i, "Codec/CC"), Get(streamKind, i, "CodecID")), null);
					Add(st, i, EntryKey.EncodeSettings, Get(streamKind, i, "Encoded_Library_Settings"), null);
					Add(st, i, EntryKey.EncodeLibrary, Get(streamKind, i, "Encoded_Library"), null);
					Add(st, i, EntryKey.BitrateMode, Get(streamKind, i, "BitRate_Mode"), null);

					switch(streamKind) {
						case eStreamType.Video:
							Add(st, i, EntryKey.FrameRate, Get(streamKind, i, "FrameRate"), null);
							Add(st, i, EntryKey.FrameCount, Get(streamKind, i, "FrameCount"), null);
							Add(st, i, EntryKey.Width, Get(streamKind, i, "Width"), null);
							Add(st, i, EntryKey.Height, Get(streamKind, i, "Height"), null);
							Add(st, i, EntryKey.DAR, Get(streamKind, i, "DisplayAspectRatio"), null);
							break;
						case eStreamType.Audio:
							Add(st, i, EntryKey.SamplingRate, Get(streamKind, i, "SamplingRate"), null);
							Add(st, i, EntryKey.SampleCount, Get(streamKind, i, "SamplingCount"), null);
							Add(st, i, EntryKey.ChannelCount, Get(streamKind, i, "Channel(s)"), null);
							break;
						case eStreamType.Text:
							break;
					}
				}
			}
		}

		private string Get(eStreamType type, int index, string key, Converter<string, string> converter = null) {
			string value = MIL.Get(type, index, key);
			if(string.IsNullOrEmpty(value)) return null;
			return converter != null ? converter(value) : value;
		}
		private string Get(string key, Converter<string, string> converter = null) { return Get(eStreamType.General, 0, key, converter); }

		public override void Dispose() { MIL.Close(); }
	}
}

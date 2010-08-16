using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2Lib.InfoGathering.InfoProvider.Tools {
	public abstract class InfoProviderBase : IDisposable, IEnumerable<InfoEntry> {
		protected InfoCollection infos;

		public InfoProviderBase() { }

		protected void Add(StreamType type, int index, EntryKey entry, string value, string unit) {
			if(string.IsNullOrEmpty(value)) return;
			infos.Add(new InfoEntry(new StreamTypeEntryPair(type, index, entry), value, unit, this));
		}
		protected void Add(EntryKey entry, string value, string unit) { Add(StreamType.General, 0, entry, value, unit); }
		protected void Add(EntryKey entry, Func<string> value, string unit) { Add(StreamType.General, 0, entry, value, unit); }
		protected void Add(StreamType type, int index, EntryKey entry, Func<string> value, string unit) { try { Add(type, index, entry, value(), unit); } catch(Exception) { } }

		public virtual InfoEntry this[StreamType type, int index, EntryKey key] {
			get {
				InfoEntry entry;
				return infos.TryGetValue(new StreamTypeEntryPair(type, index, key), out entry) ? entry : null;
			}
		}

		public abstract void Dispose();

		public virtual IEnumerator<InfoEntry> GetEnumerator() { return infos.Values.GetEnumerator(); }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return GetEnumerator(); }
	}

	public enum StreamType { General, Video, Audio, Text, Chapter, Hash }
	public enum EntryKey { None, Index, Size, Bitrate, Date, Duration, Title, Language, CodecName, CodecId, FourCC, Id, FrameCount, FrameRate, Width, Height, DAR, Flags, SampleCount, SamplingRate, ChannelCount, WritingApp, MuxingApp, Extension, EncodeSettings, EncodeLibrary, BitrateMode }

	public struct StreamTypeEntryPair {
		public readonly StreamType Type;
		public readonly int Index;
		public readonly EntryKey Entry;

		public StreamTypeEntryPair(StreamType type, int index, EntryKey entry) { Type = type; Index = index; Entry = entry; }

		public override bool Equals(object obj) {
			if(obj is StreamTypeEntryPair) {
				var pair = (StreamTypeEntryPair)obj;
				return Type == pair.Type && Index == pair.Index && Entry == pair.Entry;
			}
			return false;
		}
		public override int GetHashCode() { return Type.GetHashCode() ^ Index.GetHashCode() ^ Entry.GetHashCode(); }
		public override string ToString() { return Type + "/" + Index + "/" + Entry; }
	}
	public class InfoEntry {
		public StreamTypeEntryPair Key { get; private set; }
		public string Value { get; private set; }
		public string Unit { get; private set; }
		public InfoProviderBase Provider { get; private set; }

		public InfoEntry(StreamTypeEntryPair key, string value, string unit, InfoProviderBase provider) { Key = key; Value = value; Unit = unit; Provider = provider; }

		public override string ToString() { return "(" + Key.ToString() + "|" + Value + "|" + Unit + "|" + Provider.ToString() + ")"; }
	}

}

﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml;
using System.Diagnostics;

namespace AVDump2Lib.InfoGathering.InfoProvider.Tools {
	public enum StreamType { General, Video, Audio, Text, Chapter, Hash, Unkown }
	public enum EntryKey { None, Index, Size, Overhead, Bitrate, Date, Duration, Title, ColorBitDepth, Language, CodecName, CodecId, CodecIdAlt, TwoCC, FourCC, Id, FrameCount, FrameRate, VFR, MaxFrameRate, MinFrameRate, Width, Height, PAR, DAR, Flags, SampleCount, SamplingRate, ChannelCount, WritingApp, MuxingApp, Extension, FileExtension, EncodeSettings, EncodeLibrary, BitrateMode, TrackNumber }

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

	public abstract class InfoProviderBase : IDisposable, IEnumerable<InfoEntry> {
		protected InfoCollection infos;

		public InfoProviderBase() { }

		protected virtual void Add(StreamType type, int index, EntryKey entry, string value, string unit) {
			if(string.IsNullOrEmpty(value)) return;
			infos.Add(new InfoEntry(new StreamTypeEntryPair(type, index, entry), value.Trim(), unit, this));
		}
		protected void Add(EntryKey entry, string value, string unit) { Add(StreamType.General, 0, entry, value, unit); }
		protected void Add(EntryKey entry, Func<string> value, string unit) { Add(StreamType.General, 0, entry, value, unit); }
		protected void Add(StreamType type, int index, EntryKey entry, string value) { Add(type, index, entry, value, null); }
		protected void Add(StreamType type, int index, EntryKey entry, Func<string> value, string unit) { try { Add(type, index, entry, value(), unit); } catch(Exception ex) { Debug.Print(entry.ToString()); } }

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
}

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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Xml;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.HashAlgorithms;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;
using AVDump2Lib.Misc;
using System.Collections.ObjectModel;
using System.Reflection;

namespace AVDump2Lib.InfoGathering {

	public class Info {
		public static XmlDocument CreateAVDumpLog(string path, IEnumerable<IBlockConsumer> blockConsumers) {
			MediaInfo mediaInfo;
			MatroskaFileInfo matroskaFileInfo;

			XmlDocument xmlDoc = new XmlDocument();
			XmlNode node, subNode;
			int streamCount;
			string milInfo;

			node = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
			AppendLeaf(xmlDoc, node, "size", (new FileInfo(path)).Length.ToString(), null);

			if(blockConsumers != null) {
				foreach(IBlockConsumer blockConsumer in blockConsumers) {
					if(blockConsumer is HashCalculator) {
						HashCalculator hashExecute = (HashCalculator)blockConsumer;
						eBaseOption baseOption = (hashExecute.HashObj is TreeHash || hashExecute.HashObj is TigerThex ? eBaseOption.Base32 : eBaseOption.Heximal) | eBaseOption.Pad | eBaseOption.Reverse;
						AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower(), BaseConverter.ToString(hashExecute.HashObj.Hash, baseOption).ToLower(), null);

						if(hashExecute.HashObj is Ed2k) {
							Ed2k ed2k = (Ed2k)hashExecute.HashObj;
							if(!ed2k.BlueIsRed()) {
								baseOption = eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse;
								AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower() + "_alt", BaseConverter.ToString(ed2k.BlueHash, baseOption).ToLower(), null);
							}
						}
					} else if(blockConsumer is MatroskaFileInfo) {
						matroskaFileInfo = (MatroskaFileInfo)blockConsumer;
					}
				}
			}

			mediaInfo = CreateMediaInfoInstance();
			mediaInfo.Open(path);

			node = node.AppendChild(xmlDoc.CreateElement("avmf"));

			milInfo = mediaInfo.Get(eStreamType.General, 0, "Duration");
			if(!String.IsNullOrEmpty(milInfo)) {
				AppendLeaf(xmlDoc, node, "duration", (double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000d).ToString(CultureInfo.InvariantCulture), null);
				AppendLeaf(xmlDoc, node, "duration2", TimeSpan.FromSeconds(((long)double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000)).ToString(), null);
			}

			AppendLeaf(xmlDoc, node, "extension", mediaInfo.Get(eStreamType.General, 0, "FileExtension"), null);
			//AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
			AppendLeaf(xmlDoc, node, "chapters", mediaInfo.Get(eStreamType.General, 0, "ChaptersCount"), null);
			AppendLeaf(xmlDoc, node, "app", mediaInfo.Get(eStreamType.General, 0, "Encoded_Application"), null);
			AppendLeaf(xmlDoc, node, "lib", mediaInfo.Get(eStreamType.General, 0, "Encoded_Library"), null);

			streamCount = mediaInfo.Count_Get(eStreamType.Video);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("video"));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Video, i, "Title"), null);
				var asdf = mediaInfo.Get(eStreamType.Video, i, "StreamSize");
				if(!String.IsNullOrEmpty(asdf)) {
					asdf = "";
				}
				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamType.Video, i, "StreamSize"), null);


				milInfo = mediaInfo.Get(eStreamType.Video, i, "Duration");
				if(!String.IsNullOrEmpty(milInfo)) {
					AppendLeaf(xmlDoc, subNode, "duration", (double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000d).ToString(CultureInfo.InvariantCulture), null);
					AppendLeaf(xmlDoc, subNode, "duration2", TimeSpan.FromSeconds(((long)double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000)).ToString(), null);
				}

				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
				milInfo = mediaInfo.Get(eStreamType.Video, i, "BitRate");
				if(!String.IsNullOrEmpty(milInfo)) AppendLeaf(xmlDoc, subNode, "bitrate2", ((long)double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000).ToString(), new String[,] { { "unit", "kbit/s" } });
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamType.Video, i, "BitRate"), null);
				AppendLeaf(xmlDoc, subNode, "fourcc", mediaInfo.Get(eStreamType.Video, i, "CodecID"), null); //Codec/CC
				AppendLeaf(xmlDoc, subNode, "fps", mediaInfo.Get(eStreamType.Video, i, "FrameRate"), null);
				AppendLeaf(xmlDoc, subNode, "encoder", mediaInfo.Get(eStreamType.Video, i, "Encoded_Library"), null);
				AppendLeaf(xmlDoc, subNode, "setting", mediaInfo.Get(eStreamType.Video, i, "Encoded_Library_Settings"), null);
				//AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamType.Video, i, "FrameCount"), null);
				AppendLeaf(xmlDoc, subNode, "chroma", mediaInfo.Get(eStreamType.Video, i, "Colorimetry"), null);
				AppendLeaf(xmlDoc, subNode, "structure", mediaInfo.Get(eStreamType.Video, i, "Interlacement"), null);

				AppendLeaf(xmlDoc, subNode, "res_p", null, new String[,] { { "width", mediaInfo.Get(eStreamType.Video, i, "Width") },
				                                                           { "height", mediaInfo.Get(eStreamType.Video, i, "Height") }});

				//AppendLeaf(xmlDoc, subNode, "dar", mediaInfo.Get(eStreamKind.Video, i, "DisplayAspectRatio"), null);
				string res_d = mediaInfo.Get(eStreamType.Video, i, "DisplayAspectRatio/String");
				if(res_d.Contains(":")) AppendLeaf(xmlDoc, subNode, "dar", null, new String[,] { { "width", res_d.Split(':')[0].ToString() }, { "height", res_d.Split(':')[1].ToString() } });
			}


			streamCount = mediaInfo.Count_Get(eStreamType.Audio);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("audio"));

				milInfo = mediaInfo.Get(eStreamType.Audio, i, "Duration");
				if(!String.IsNullOrEmpty(milInfo)) {
					AppendLeaf(xmlDoc, subNode, "duration", (double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000d).ToString(CultureInfo.InvariantCulture), null);
					AppendLeaf(xmlDoc, subNode, "duration2", TimeSpan.FromSeconds(((long)double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000)).ToString(), null);
				}


				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamType.Audio, i, "StreamSize"), null);
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Audio, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamType.Audio, i, "BitRate"), null);
				milInfo = mediaInfo.Get(eStreamType.Audio, i, "BitRate");
				if(!String.IsNullOrEmpty(milInfo)) AppendLeaf(xmlDoc, subNode, "bitrate2", ((long)double.Parse(milInfo, CultureInfo.InvariantCulture) / 1000).ToString(), new String[,] { { "unit", "kbit/s" } });
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamType.Audio, i, "Language"), null);
				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
				AppendLeaf(xmlDoc, subNode, "twocc", mediaInfo.Get(eStreamType.Audio, i, "Codec"), null);
				AppendLeaf(xmlDoc, subNode, "channels", mediaInfo.Get(eStreamType.Audio, i, "Channel(s)"), null);
				AppendLeaf(xmlDoc, subNode, "sampling_rate", mediaInfo.Get(eStreamType.Audio, i, "SamplingRate"), null);
				AppendLeaf(xmlDoc, subNode, "mode", mediaInfo.Get(eStreamType.Audio, i, "BitRate_Mode"), null);
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamType.Audio, i, "SamplingCount"), null);
			}

			streamCount = mediaInfo.Count_Get(eStreamType.Text);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
				//AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Text, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamType.Text, i, "Language"), null);
				AppendLeaf(xmlDoc, subNode, "identifier", mediaInfo.Get(eStreamType.Text, i, "Codec"), null);
			}
			mediaInfo.Close();

			return xmlDoc;
		}
		public static XmlDocument CreateAVDumpLog(InfoProvider p) {
			XmlDocument xmlDoc = new XmlDocument(); ;
			Func<string, string, string, string, XmlNode> createNode = (name, value, unit, provider) => {
				XmlNode n = xmlDoc.CreateElement(name);
				if(!string.IsNullOrEmpty(unit)) n.Attributes.Append(xmlDoc.CreateAttribute("Unit")).Value = unit;
				if(!string.IsNullOrEmpty(provider) && false) n.Attributes.Append(xmlDoc.CreateAttribute("Provider")).Value = provider;

				n.AppendChild(xmlDoc.CreateTextNode(value));
				return n;
			};

			var aCreqNode = xmlDoc.AppendChild(xmlDoc.CreateElement("Creq"));
			var fileNode = aCreqNode.AppendChild(xmlDoc.CreateElement("File"));
			var hashNode = fileNode.AppendChild(xmlDoc.CreateElement("Hashes"));
			var generalNode = fileNode.AppendChild(xmlDoc.CreateElement("General"));
			var videoNode = fileNode.AppendChild(xmlDoc.CreateElement("VideoStreams"));
			var audioNode = fileNode.AppendChild(xmlDoc.CreateElement("AudioStreams"));
			var subtitleNode = fileNode.AppendChild(xmlDoc.CreateElement("SubtitleStreams"));

			var lookUp = new Dictionary<KeyValuePair<StreamType, int>, XmlNode>();
			XmlNode node;
			foreach(var entry in p) {
				var pair = new KeyValuePair<StreamType, int>(entry.Key.Type, entry.Key.Index);
				if(!lookUp.TryGetValue(pair, out node)) {
					switch(pair.Key) {
						case StreamType.General: node = generalNode; break;
						case StreamType.Video: lookUp[pair] = node = xmlDoc.CreateElement("VideoStream"); videoNode.AppendChild(node); break;
						case StreamType.Audio: lookUp[pair] = node = xmlDoc.CreateElement("AudioStream"); audioNode.AppendChild(node); break;
						case StreamType.Text: lookUp[pair] = node = xmlDoc.CreateElement("SubtitleStream"); subtitleNode.AppendChild(node); break;
						case StreamType.Hash: lookUp[pair] = node = hashNode; break;
						case StreamType.Chapter: break;
						default: break;
					}
				}

				if(entry.Key.Type == StreamType.Hash) {
					node.AppendChild(createNode(entry.Unit, entry.Value, null, null));
				} else {
					node.AppendChild(createNode(entry.Key.Entry.ToString(), entry.Value, entry.Unit, entry.Provider.ToString().Substring(entry.Provider.ToString().LastIndexOf('.') + 1)));
				}
			}

			return xmlDoc;
		}
		public static string CreateMediaInfoDump(string filePath) {
			MediaInfo mi = CreateMediaInfoInstance();

			mi.Open(filePath);
			mi.Option("Complete", "1");

			string dump = mi.Inform();

			mi.Close();

			return dump;
		}
		public static XmlDocument CreateMediaInfoXMLLog(string filePath, IEnumerable<IBlockConsumer> blockConsumers) {
			XmlDocument xmlDoc = new XmlDocument();
			XmlNode node, subNode;

			MediaInfo mediaInfo = CreateMediaInfoInstance();
			mediaInfo.Open(filePath);

			node = xmlDoc.AppendChild(xmlDoc.CreateElement("File"));
			subNode = node.AppendChild(xmlDoc.CreateElement("Hashes"));
			AppendLeaf(xmlDoc, subNode, "Size", (new FileInfo(filePath)).Length.ToString(), null);
			if(blockConsumers != null) {

				foreach(HashCalculator hashExecute in blockConsumers.Where(blockConsumer => { return blockConsumer is HashCalculator; })) {
					AppendLeaf(xmlDoc, subNode, hashExecute.Name, BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);

					if(hashExecute.HashObj is Ed2k) {
						Ed2k ed2k = (Ed2k)hashExecute.HashObj;
						if(!ed2k.BlueIsRed()) AppendLeaf(xmlDoc, subNode, "Ed2k_Alt", BaseConverter.ToString(ed2k.BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);
					}
				}
			}

			int streamCount, entryCount;
			string name, text, measure;
			foreach(eStreamType streamKind in Enum.GetValues(typeof(eStreamType))) {
				streamCount = mediaInfo.Count_Get(streamKind);

				for(int i = 0;i < streamCount;i++) {
					entryCount = mediaInfo.Count_Get(streamKind, i);
					subNode = node.AppendChild(xmlDoc.CreateElement(streamKind.ToString()));

					for(int j = 0;j < entryCount;j++) {
						name = mediaInfo.Get(streamKind, i, j, eInfoType.Name).Replace("/", "-").Replace("(", "").Replace(")", "");
						if(name.Equals("Chapters_Pos_End") || name.Equals("Chapters_Pos_Begin") || name.Contains("-String")) continue;
						if(name.Equals("Bits-Pixel*Frame")) name = "BitsPerPixel";

						text = mediaInfo.Get(streamKind, i, j, eInfoType.Text);
						measure = mediaInfo.Get(streamKind, i, j, eInfoType.Measure).Trim();

						if(name.IndexOfAny(new char[] { ')', ':' }) < 0 && !String.IsNullOrEmpty(text)) {
							AppendLeaf(xmlDoc, subNode, name, text, String.IsNullOrEmpty(measure) ? null : new string[,] { { "Unit", measure } });
						} else {
							//Debug.Print(name + " " + text + " " + measure);
						}
					}
					if(streamKind == eStreamType.Menu) {
						int indexStart;
						int indexEnd;
						XmlNode chapterNode;

						if(int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_Begin"), out indexStart) && int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_End"), out indexEnd)) {
							chapterNode = subNode.AppendChild(xmlDoc.CreateElement("Chapters"));
							for(;indexStart < indexEnd;indexStart++) {
								AppendLeaf(xmlDoc, chapterNode, "Chapter", mediaInfo.Get(streamKind, i, indexStart, eInfoType.Text), new string[,] { { "TimeStamp", mediaInfo.Get(streamKind, i, indexStart, eInfoType.Name) } });
							}
						}
					}
				}
			}

			mediaInfo.Close();

			return xmlDoc;
		}

		private static XmlNode AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, string value, string[,] attributes = null) {
			if((attributes == null || attributes.Length == 0) && string.IsNullOrEmpty(value)) return null;

			XmlNode leaf = xmlDoc.CreateElement(name);
			if(!string.IsNullOrEmpty(value)) leaf.AppendChild(xmlDoc.CreateTextNode(value));

			if(attributes != null) {
				for(int i = 0;i < attributes.GetLength(0);i++) {
					leaf.Attributes.Append(xmlDoc.CreateAttribute(attributes[i, 0])).Value = attributes[i, 1];
				}
			}

			return node.AppendChild(leaf);
		}
		private static MediaInfo CreateMediaInfoInstance() {
			MediaInfo mi = new MediaInfo();
			mi.Option("Internet", "No");
			return mi;
		}
	}

	public enum StreamType { General, Video, Audio, Text, Chapter, Hash }
	public enum EntryKey { None, Index, Size, Bitrate, Date, Duration, Title, Language, CodecName, Id, FrameCount, FrameRate, Width, Height, DAR, Flags, SampleCount, SamplingRate, ChannelCount, WritingApp, MuxingApp, Extension, EncodeSettings, EncodeLibrary, BitrateMode }

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
		public InfoProvider Provider { get; private set; }

		public InfoEntry(StreamTypeEntryPair key, string value, string unit, InfoProvider provider) { Key = key; Value = value; Unit = unit; Provider = provider; }

		public override string ToString() { return "(" + Key.ToString() + "|" + Value + "|" + Unit + "|" + Provider.ToString() + ")"; }
	}

	#region DO NOT UNFOLD - You have been warned
	public abstract class InfoProvider : IDisposable, IEnumerable<InfoEntry> {
		protected InfoCollection infos;

		public InfoProvider() { }

		protected void Add(StreamType type, int index, EntryKey entry, string value, string unit) {
			if(string.IsNullOrEmpty(value)) return;
			infos.Add(new InfoEntry(new StreamTypeEntryPair(type, index, entry), value, unit, this));
		}
		protected void Add(EntryKey entry, string value, string unit) { Add(StreamType.General, 0, entry, value, unit); }

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
	public class MatroskaProvider : InfoProvider {
		public MatroskaFileInfo MFI { get; private set; }
		public MatroskaProvider(MatroskaFileInfo mfi) {
			infos = new InfoCollection();
			MFI = mfi;

			Add(EntryKey.Size, MFI.ProcessedBytes.ToString(), "byte");
			Add(EntryKey.Date, MFI.Segment.SegmentInfo.ProductionDate.HasValue ? MFI.Segment.SegmentInfo.ProductionDate.Value.ToString() : null, null);
			Add(EntryKey.Duration, (MFI.Segment.SegmentInfo.Duration.Value * (MFI.Segment.SegmentInfo.TimecodeScale / 1000000000d)).ToString("0.000", CultureInfo.InvariantCulture), "s");
			Add(EntryKey.WritingApp, MFI.Segment.SegmentInfo.WritingApp.ToString(), null);
			Add(EntryKey.MuxingApp, MFI.Segment.SegmentInfo.MuxingApp.ToString(), null);

			int[] indeces = new int[3];
			foreach(var track in MFI.Segment.Tracks.Items) {
				if(!track.TrackType.HasValue) throw new Exception("TrackType missing");
				switch(track.TrackType.Value) {
					case TrackEntrySection.Types.Video: AddStreamInfo(this, track, StreamType.Video, indeces[0]++); break;
					case TrackEntrySection.Types.Audio: AddStreamInfo(this, track, StreamType.Audio, indeces[1]++); break;
					case TrackEntrySection.Types.Subtitle: AddStreamInfo(this, track, StreamType.Text, indeces[2]++); break;
					default: break;
				}
			}
		}

		private void AddStreamInfo(MatroskaProvider p, TrackEntrySection trackEntry, StreamType type, int index) {
			var trackInfo = MFI.Segment.Cluster.Tracks[(int)trackEntry.TrackNumber.Value - 1].CalcTrackInfo(
				MFI.Segment.SegmentInfo.TimecodeScale * trackEntry.TrackTimecodeScale.GetValueOrDefault(1)
			);

			Add(type, index, EntryKey.Index, index.ToString(), null);
			Add(type, index, EntryKey.Size, trackInfo.TrackSize.ToString(), "byte");
			Add(type, index, EntryKey.Bitrate, trackInfo.AverageBitrate.ToString("0", CultureInfo.InvariantCulture), "bit/s");
			Add(type, index, EntryKey.Duration, trackInfo.TrackLength.TotalSeconds.ToString(CultureInfo.InvariantCulture), "s");
			Add(type, index, EntryKey.Title, trackEntry.Name, null);
			Add(type, index, EntryKey.Language, trackEntry.Language, null);
			Add(type, index, EntryKey.CodecName, trackEntry.CodecId, null);
			Add(type, index, EntryKey.Id, trackEntry.TrackUId.ToString(), null);

			switch(type) {
				case StreamType.Video:
					Add(type, index, EntryKey.FrameCount, trackInfo.LaceCount.ToString(), null);
					Add(type, index, EntryKey.FrameRate, (trackEntry.DefaultDuration.HasValue ? 1000000000d / trackEntry.DefaultDuration.Value : trackInfo.AverageLaceRate).ToString("0.000", CultureInfo.InvariantCulture), "fps");
					Add(type, index, EntryKey.Width, trackEntry.Video.PixelWidth.ToString(), "px");
					Add(type, index, EntryKey.Height, trackEntry.Video.PixelHeight.ToString(), "px");
					Add(type, index, EntryKey.DAR, (trackEntry.Video.DisplayWidth / (double)trackEntry.Video.DisplayHeight).ToString("0.000", CultureInfo.InvariantCulture), null);
					break;
				case StreamType.Audio:
					Add(type, index, EntryKey.SampleCount, (trackInfo.TrackLength.TotalSeconds * trackEntry.Audio.SamplingFrequency).ToString(), null);
					Add(type, index, EntryKey.SamplingRate, trackEntry.Audio.SamplingFrequency.ToString(CultureInfo.InvariantCulture), null);
					Add(type, index, EntryKey.ChannelCount, trackEntry.Audio.ChannelCount.ToString(), null);
					break;
				case StreamType.Text: break;
			}
		}

		public override void Dispose() { }

	}
	public class MediaInfoProvider : InfoProvider {
		public MediaInfo MIL { get; private set; }

		public MediaInfoProvider(string filePath) {
			infos = new InfoCollection();

			MIL = new MediaInfo();
			MIL.Option("Internet", "No");
			MIL.Open(filePath);

			Add(EntryKey.Size, Get("Size"), "byte");
			Add(EntryKey.Duration, Get("Duration", (str) => { return (double.Parse(str, CultureInfo.InvariantCulture) / 1000).ToString("0.000", CultureInfo.InvariantCulture); }), "s");
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
					Add(st, i, EntryKey.CodecName, Get(streamKind, i, "CodecID"), null);
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
	public class HashInfoProvider : InfoProvider {

		public HashInfoProvider(IEnumerable<HashCalculator> hashCalculators) {
			infos = new InfoCollection();

			int i = 0;
			foreach(var hashCalculator in hashCalculators) {
				eBaseOption baseOption = (hashCalculator.HashObj is TreeHash || hashCalculator.HashObj is TigerThex ? eBaseOption.Base32 : eBaseOption.Heximal) | eBaseOption.Pad | eBaseOption.Reverse;
				Add(StreamType.Hash, i++, EntryKey.None, BaseConverter.ToString(hashCalculator.HashObj.Hash, baseOption).ToLower(), hashCalculator.Name.ToLower());

				if(hashCalculator.HashObj is Ed2k) {
					Ed2k ed2k = (Ed2k)hashCalculator.HashObj;
					if(!ed2k.BlueIsRed()) {
						baseOption = eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse;
						Add(StreamType.Hash, i++, EntryKey.None, BaseConverter.ToString(ed2k.BlueHash, baseOption).ToLower(), hashCalculator.Name.ToLower() + "_alt");
					}
				}
			}
		}

		public override void Dispose() { }
	}
	public class CompositeInfoProvider : InfoProvider {
		private IEnumerable<InfoProvider> providers;

		public CompositeInfoProvider(IEnumerable<InfoProvider> providers) { this.providers = providers; }
		public override void Dispose() { foreach(var provider in providers) provider.Dispose(); }

		public override InfoEntry this[StreamType type, int index, EntryKey key] {
			get {
				InfoEntry entry;
				foreach(var provider in providers) if((entry = provider[type, index, key]) != null) return entry;
				return null;
			}
		}

		public override IEnumerator<InfoEntry> GetEnumerator() { return EnumEntries().GetEnumerator(); }
		public IEnumerable<InfoEntry> EnumEntries() {
			InfoEntry entry;


			for(int index = 0;index < 5;index++) {
				entry = this[StreamType.Hash, index, EntryKey.None];
				if(entry != null) yield return entry;
			}

			foreach(EntryKey entryKey in Enum.GetValues(typeof(EntryKey))) {
				entry = this[StreamType.General, 0, entryKey];
				if(entry != null) yield return entry;
			}

			for(int index = 0;index < 6;index++) {
				foreach(EntryKey entryKey in Enum.GetValues(typeof(EntryKey))) {
					entry = this[StreamType.Video, index, entryKey];
					if(entry != null) yield return entry;
				}
			}
			for(int index = 0;index < 6;index++) {
				foreach(EntryKey entryKey in Enum.GetValues(typeof(EntryKey))) {
					entry = this[StreamType.Audio, index, entryKey];
					if(entry != null) yield return entry;
				}
			}
			for(int index = 0;index < 6;index++) {
				foreach(EntryKey entryKey in Enum.GetValues(typeof(EntryKey))) {
					entry = this[StreamType.Text, index, entryKey];
					if(entry != null) yield return entry;
				}
			}
		}
	}
	#endregion

	public class InfoCollection : Dictionary<StreamTypeEntryPair, InfoEntry> { public void Add(InfoEntry item) { Add(item.Key, item); } }
}

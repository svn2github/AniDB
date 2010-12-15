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
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.InfoProvider;

namespace AVDump2Lib.InfoGathering {

	public class Info {
		public static XmlDocument CreateOldAVDumpLog(string path, InfoProviderBase p) {
			MediaInfo mediaInfo;
			MatroskaFile matroskaFileInfo;

			XmlDocument xmlDoc = new XmlDocument();
			XmlNode node, subNode;
			int streamCount;
			string milInfo;

			node = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
			AppendLeaf(xmlDoc, node, "size", (new FileInfo(path)).Length.ToString(), null);

			//if(blockConsumers != null) {
			//    foreach(IBlockConsumer blockConsumer in blockConsumers) {
			//        if(blockConsumer is HashCalculator) {
			//            HashCalculator hashExecute = (HashCalculator)blockConsumer;
			//            BaseOption baseOption = (hashExecute.HashObj is TreeHash || hashExecute.HashObj is TigerThex ? BaseOption.Base32 : BaseOption.Heximal) | BaseOption.Pad | BaseOption.Reverse;
			//            AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower(), BaseConverter.ToString(hashExecute.HashObj.Hash, baseOption).ToLower(), null);

			//            if(hashExecute.HashObj is Ed2k) {
			//                Ed2k ed2k = (Ed2k)hashExecute.HashObj;
			//                if(!ed2k.BlueIsRed) {
			//                    baseOption = BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse;
			//                    AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower() + "_alt", BaseConverter.ToString(ed2k.RedHash, baseOption).ToLower(), null);
			//                }
			//            }
			//        } else if(blockConsumer is MatroskaParser) {
			//            matroskaFileInfo = (MatroskaParser)blockConsumer;
			//        }
			//    }
			//}

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
		public static XmlDocument CreateAVDumpLog(InfoProviderBase p) {
			XmlDocument xmlDoc = new XmlDocument();
			Func<InfoProviderBase, string> getName = (provider) => {
				string name;
				if(provider is MatroskaProvider) {
					name = "mkv";
				} else if(provider is MediaInfoProvider) {
					name = "mil";
				} else if(provider is OgmOggProvider) {
					name = "ogm";
				} else if(provider is HashInfoProvider) {
					name = "hash";
				} else if(provider is FileExtensionProvider) {
					name = "ext";
				} else {
					name = "";
				}
				return name;
			};
			Func<string, string, string, XmlNode> createNode = (name, value, pName) => {
				var n = xmlDoc.CreateElement(name);
				if(value != null) n.AppendChild(xmlDoc.CreateTextNode(value));
				//if(!string.IsNullOrEmpty(pName)) n.Attributes.Append(xmlDoc.CreateAttribute("p")).Value = pName;
				return n;
			};
			Action<XmlNode, string, string> addAttribute = (n, name, value) => { n.Attributes.Append(xmlDoc.CreateAttribute(name)).Value = value; };
			Func<StreamType, TrackEntrySection.Types> convertEnum = t => t == StreamType.Audio ? TrackEntrySection.Types.Audio : (t == StreamType.Video ? TrackEntrySection.Types.Video : (t == StreamType.Text ? TrackEntrySection.Types.Subtitle : 0));

			var toAcreqName = new Dictionary<EntryKey, string>();
			toAcreqName[EntryKey.MuxingApp] = "app";
			toAcreqName[EntryKey.EncodeLibrary] = "encoder";
			toAcreqName[EntryKey.WritingApp] = "lib";
			toAcreqName[EntryKey.CodecId] = "identifier";
			toAcreqName[EntryKey.CodecIdAlt] = "identifier2";
			toAcreqName[EntryKey.FrameRate] = "fps";
			toAcreqName[EntryKey.FrameCount] = "sample_count";
			toAcreqName[EntryKey.SampleCount] = "sample_count";
			toAcreqName[EntryKey.EncodeSettings] = "settings";
			toAcreqName[EntryKey.ChannelCount] = "channels";
			toAcreqName[EntryKey.SamplingRate] = "sampling_rate";
			toAcreqName[EntryKey.FileExtension] = "file_extension";
			toAcreqName[EntryKey.Extension] = "extension";
			toAcreqName[EntryKey.CodecName] = "codec_name";
			toAcreqName[EntryKey.Overhead] = "extra_size";
			toAcreqName[EntryKey.BitrateMode] = "mode";
			toAcreqName[EntryKey.DAR] = "ar";
			toAcreqName[EntryKey.VFR] = null;
			toAcreqName[EntryKey.MaxFrameRate] = null;
			toAcreqName[EntryKey.MinFrameRate] = null;

			var fileNode = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
			var avmfNode = xmlDoc.CreateElement("avmf");
			var lookUp = new Dictionary<KeyValuePair<StreamType, int>, XmlNode>();

			XmlNode node;
			CompositeInfoProvider comp = (CompositeInfoProvider)p;
			foreach(var entry in p) {
				var pair = new KeyValuePair<StreamType, int>(entry.Key.Type, entry.Key.Index);

				if(pair.Key == StreamType.Text && (entry.Key.Entry == EntryKey.Bitrate || entry.Key.Entry == EntryKey.Duration)) continue;

				if(!lookUp.TryGetValue(pair, out node)) {
					switch(pair.Key) {
						case StreamType.General: node = entry.Key.Entry == EntryKey.Size ? fileNode : avmfNode; break;
						case StreamType.Video:
							lookUp[pair] = node = xmlDoc.CreateElement("video"); avmfNode.AppendChild(node);

							if(comp.GetProvider<MatroskaProvider>() != null) {
								var matroskaFile = comp.GetProvider<MatroskaProvider>().MFI;
								var subNode = node.AppendChild(createNode("res_d", null, "mkv"));
								addAttribute(subNode, "width", matroskaFile.Segment.Tracks[TrackEntrySection.Types.Video, entry.Key.Index].Video.DisplayWidth.ToString());
								addAttribute(subNode, "height", matroskaFile.Segment.Tracks[TrackEntrySection.Types.Video, entry.Key.Index].Video.DisplayHeight.ToString());
							}
							break;
						case StreamType.Audio: lookUp[pair] = node = xmlDoc.CreateElement("audio"); avmfNode.AppendChild(node); break;
						case StreamType.Text: lookUp[pair] = node = xmlDoc.CreateElement("subtitles"); avmfNode.AppendChild(node); break;
						case StreamType.Unkown: lookUp[pair] = node = xmlDoc.CreateElement("unkownstream"); avmfNode.AppendChild(node); break;
						case StreamType.Hash: lookUp[pair] = node = fileNode; break;
						case StreamType.Chapter: break;
						default: break;
					}

					if(comp.GetProvider<MatroskaProvider>() != null) {
						var matroskaFile = comp.GetProvider<MatroskaProvider>().MFI;

						if(pair.Key == StreamType.Audio || pair.Key == StreamType.Video || pair.Key == StreamType.Text) {
							if((matroskaFile.Segment.Tracks[convertEnum(pair.Key), entry.Key.Index].TrackFlags & TrackEntrySection.Options.Default) != 0) addAttribute(node, "default", "1");

						}
					}
				}


				string name, value;
				if(!toAcreqName.TryGetValue(entry.Key.Entry, out name)) name = entry.Key.Entry.ToString().ToLower(); else if(name == null) continue;
				if(entry.Key.Entry == EntryKey.Date) {
					try {
						value = Convert.ToString((int)(DateTime.ParseExact(entry.Value, "yyyy.MM.dd HH.mm.ss.ffff", CultureInfo.InvariantCulture) - new DateTime(1970, 1, 1)).TotalSeconds, 16);
					} catch(Exception) {
						continue;
					}
				} else {
					value = entry.Value;
				}

				if(entry.Key.Type == StreamType.Hash) {
					node.AppendChild(createNode(entry.Unit.ToLower(), entry.Value.ToLower(), "hash"));

				} else if(entry.Key.Entry == EntryKey.FrameRate && pair.Key == StreamType.Video) {
					var vfrInfo = p[StreamType.Video, entry.Key.Index, EntryKey.VFR];
					var maxFpsInfo = p[StreamType.Video, entry.Key.Index, EntryKey.MaxFrameRate];
					var minFpsInfo = p[StreamType.Video, entry.Key.Index, EntryKey.MinFrameRate];

					var fpsNode = node.AppendChild(createNode(name, vfrInfo != null ? "0" : value.Equals("") ? null : value, getName(entry.Provider)));
					if(vfrInfo != null) addAttribute(fpsNode, "vfr", vfrInfo.Value);
					if(maxFpsInfo != null) addAttribute(fpsNode, "max", maxFpsInfo.Value);
					if(minFpsInfo != null) addAttribute(fpsNode, "min", minFpsInfo.Value);

				} else if((entry.Key.Entry == EntryKey.Width || entry.Key.Entry == EntryKey.Height) && pair.Key == StreamType.Video) {
					if(node["res_p"] == null) {
						var subNode = node["res_d"] == null ? node["ar"] : node["res_d"];
						if(subNode != null) node.InsertBefore(xmlDoc.CreateElement("res_p"), subNode); else node.AppendChild(xmlDoc.CreateElement("res_p"));
					}
					node["res_p"].Attributes.Append(xmlDoc.CreateAttribute(entry.Key.Entry == EntryKey.Width ? "width" : "height")).Value = entry.Value;

				} else {
					//if(name.Equals("idenfitier") && value.Equals("-- --") || value.Equals("")) {
					//    return null;
					//}
					node.AppendChild(createNode(name, value, getName(entry.Provider)));
				}
			}
			fileNode.AppendChild(avmfNode);

			XmlNode chaptersNode;
			if(comp.GetProvider<MatroskaProvider>() != null) {
				var matroskaFile = comp.GetProvider<MatroskaProvider>().MFI;

				if(matroskaFile.Segment.Attachments != null) fileNode.AppendChild(createNode("extra_size", matroskaFile.Segment.Attachments.SectionSize.Value.ToString(), "mkv"));
				if(matroskaFile.Segment.SegmentInfo.PreviousUId != null) avmfNode.AppendChild(createNode("previous_uid", new Guid(matroskaFile.Segment.SegmentInfo.PreviousUId).ToString(), "mkv"));
				if(matroskaFile.Segment.SegmentInfo.NextUId != null) avmfNode.AppendChild(createNode("next_uid", new Guid(matroskaFile.Segment.SegmentInfo.NextUId).ToString(), "mkv"));
				if(matroskaFile.Segment.SegmentInfo.PreviousFilename != null) avmfNode.AppendChild(createNode("previous_filename", matroskaFile.Segment.SegmentInfo.PreviousFilename, "mkv"));
				if(matroskaFile.Segment.SegmentInfo.NextFilename != null) avmfNode.AppendChild(createNode("next_filename", matroskaFile.Segment.SegmentInfo.NextFilename, "mkv"));
				if(matroskaFile.Segment.SegmentInfo.SegmentUId != null) avmfNode.AppendChild(createNode("segment_uid", new Guid(matroskaFile.Segment.SegmentInfo.SegmentUId).ToString(), "mkv"));
				if(matroskaFile.Segment.Chapters != null) {
					foreach(var chapter in matroskaFile.Segment.Chapters) {
						chaptersNode = xmlDoc.CreateElement("Chapters");
						CreateChaptersNode(xmlDoc, chaptersNode, (EditionEntrySection)chapter.Value);
						if(chaptersNode.HasChildNodes) avmfNode.AppendChild(chaptersNode);
					}
				}

			} else if(comp.GetProvider<MediaInfoProvider>() != null) {
				var mil = comp.GetProvider<MediaInfoProvider>().MIL;

				chaptersNode = xmlDoc.CreateElement("Chapters");
				try { CreateChaptersNode(xmlDoc, chaptersNode, mil); } catch(Exception) { chaptersNode.RemoveAll(); }
				if(chaptersNode.HasChildNodes) avmfNode.AppendChild(chaptersNode);
			}

			return xmlDoc;
		}
		public static XmlDocument CreateNewAVDumpLog(InfoProviderBase provider) {
			XmlDocument xmlDoc = new XmlDocument(); ;
			Func<string, string, string, string, XmlNode> createNode = (name, value, unit, p) => {
				var n = xmlDoc.CreateElement(name);
				if(!string.IsNullOrEmpty(unit)) n.Attributes.Append(xmlDoc.CreateAttribute("u")).Value = unit;
				//if(!string.IsNullOrEmpty(p)) n.Attributes.Append(xmlDoc.CreateAttribute("p")).Value = p;

				n.AppendChild(xmlDoc.CreateTextNode(value));
				return n;
			};
			Func<InfoProviderBase, string> getName = (p) => { return p.ToString().Substring(p.ToString().LastIndexOf('.') + 1); };

			var aCreqNode = xmlDoc.AppendChild(xmlDoc.CreateElement("Creq"));
			var fileNode = aCreqNode.AppendChild(xmlDoc.CreateElement("File"));
			var hashNode = fileNode.AppendChild(xmlDoc.CreateElement("Hashes"));
			var generalNode = fileNode.AppendChild(xmlDoc.CreateElement("General"));
			var videoNode = fileNode.AppendChild(xmlDoc.CreateElement("VideoStreams"));
			var audioNode = fileNode.AppendChild(xmlDoc.CreateElement("AudioStreams"));
			var subtitleNode = fileNode.AppendChild(xmlDoc.CreateElement("SubtitleStreams"));
			var chaptersNode = fileNode.AppendChild(xmlDoc.CreateElement("Chapters"));

			XmlNode node;
			var lookUp = new Dictionary<KeyValuePair<StreamType, int>, XmlNode>();
			foreach(var entry in provider) {
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
					node.AppendChild(createNode(entry.Key.Entry.ToString(), entry.Value, entry.Unit, getName(entry.Provider)));
				}
			}

			var comp = (CompositeInfoProvider)provider;
			if(comp.GetProvider<MatroskaProvider>() != null) {
				var mkvProvider = comp.GetProvider<MatroskaProvider>();
				var matroskaFile = mkvProvider.MFI;

				if(matroskaFile.Segment.SegmentInfo.PreviousUId != null) generalNode.AppendChild(createNode("PreviousUId", new Guid(matroskaFile.Segment.SegmentInfo.PreviousUId).ToString(), null, getName(mkvProvider)));
				if(matroskaFile.Segment.SegmentInfo.NextUId != null) generalNode.AppendChild(createNode("NextUId", new Guid(matroskaFile.Segment.SegmentInfo.NextUId).ToString(), null, getName(mkvProvider)));
				if(matroskaFile.Segment.SegmentInfo.PreviousFilename != null) generalNode.AppendChild(createNode("PreviousFilename", matroskaFile.Segment.SegmentInfo.PreviousFilename, null, getName(mkvProvider)));
				if(matroskaFile.Segment.SegmentInfo.NextFilename != null) generalNode.AppendChild(createNode("NextFilename", matroskaFile.Segment.SegmentInfo.NextFilename, null, getName(mkvProvider)));
				if(matroskaFile.Segment.SegmentInfo.SegmentUId != null) generalNode.AppendChild(createNode("SegmentUId", new Guid(matroskaFile.Segment.SegmentInfo.SegmentUId).ToString(), null, getName(mkvProvider)));
				if(matroskaFile.Segment.Chapters != null) CreateEBMLTree(xmlDoc, chaptersNode, matroskaFile.Segment.Chapters);

			} else {
				try { CreateChaptersMIL(xmlDoc, chaptersNode, comp.GetProvider<MediaInfoProvider>().MIL); } catch(Exception) { }
			}

			return xmlDoc;
		}
		private static void CreateEBMLTree(XmlDocument xmlDoc, XmlNode n, IEnumerable<KeyValuePair<string, object>> c) {
			foreach(var item in c) {
				if(item.Value is Section) {
					CreateEBMLTree(xmlDoc, n.AppendChild(xmlDoc.CreateElement(item.Key)), (IEnumerable<KeyValuePair<string, object>>)item.Value);
				} else if(item.Value != null && item.Value is byte[]) {
					n.AppendChild(xmlDoc.CreateElement(item.Key)).AppendChild(xmlDoc.CreateTextNode(new Guid((byte[])item.Value).ToString()));
				} else if(item.Value != null) {
					n.AppendChild(xmlDoc.CreateElement(item.Key)).AppendChild(xmlDoc.CreateTextNode(item.Value.ToString()));
				}
			}
		}
		private static void CreateChaptersNode(XmlDocument xmlDoc, XmlNode n, EditionEntrySection editionEntry) {
			n.Attributes.Append(xmlDoc.CreateAttribute("flags")).Value = editionEntry.EditionFlags.ToString();

			XmlNode entryNode, subNode;
			foreach(var chapterAtom in editionEntry.ChapterAtoms) {
				entryNode = n.AppendChild(xmlDoc.CreateElement("Entry"));
				entryNode.Attributes.Append(xmlDoc.CreateAttribute("flags")).Value = chapterAtom.ChapterFlags.ToString();

				if(chapterAtom.ChapterSegmentUId != null) entryNode.AppendChild(xmlDoc.CreateElement("SegId")).AppendChild(xmlDoc.CreateTextNode(new Guid((byte[])chapterAtom.ChapterSegmentUId).ToString()));

				subNode = entryNode.AppendChild(xmlDoc.CreateElement("Pos"));
				if(chapterAtom.ChapterTimeStart.HasValue) subNode.Attributes.Append(xmlDoc.CreateAttribute("start")).Value = (chapterAtom.ChapterTimeStart.Value / 1000000000d).ToString("0.###");
				if(chapterAtom.ChapterTimeEnd.HasValue) subNode.Attributes.Append(xmlDoc.CreateAttribute("end")).Value = (chapterAtom.ChapterTimeEnd.Value / 1000000000d).ToString("0.###");

				foreach(var chapterDisplay in chapterAtom.ChapterDisplays) {
					subNode = entryNode.AppendChild(xmlDoc.CreateElement("Title"));
					subNode.AppendChild(xmlDoc.CreateTextNode(chapterDisplay.ChapterString));
					if(chapterDisplay.ChapterLanguages.Count != 0) subNode.Attributes.Append(xmlDoc.CreateAttribute("lang")).Value = chapterDisplay.ChapterLanguages.Aggregate((acc, l) => acc + " " + l);
				}
			}
		}
		private static void CreateChaptersNode(XmlDocument xmlDoc, XmlNode n, MediaInfo mediaInfo) {
			Func<string, ulong> conv = (str) => {
				var timeParts = str.Split(new char[] { ':', '.' }).Select(s => ulong.Parse(s.Trim())).ToArray();
				return (((timeParts[0] * 60 + timeParts[1]) * 60 + timeParts[2]) * 1000 + timeParts[3]) * 1000000;
			};

			int streamCount, entryCount;
			eStreamType streamKind = eStreamType.Menu;
			XmlNode entryNode, subNode;
			string languageTitle, language, title;

			streamCount = mediaInfo.Count_Get(streamKind);
			for(int i = 0;i < streamCount;i++) {
				entryCount = mediaInfo.Count_Get(streamKind, i);
				int indexStart, indexEnd;
				if(int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_Begin"), out indexStart) && int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_End"), out indexEnd)) {
					for(;indexStart < indexEnd;indexStart++) {
						entryNode = n.AppendChild(xmlDoc.CreateElement("Entry"));

						var timeStamp = conv(mediaInfo.Get(streamKind, i, indexStart, eInfoType.Name).Split('-')[0].Trim());
						subNode = entryNode.AppendChild(xmlDoc.CreateElement("Pos"));
						subNode.Attributes.Append(xmlDoc.CreateAttribute("start")).Value = (timeStamp / 1000).ToString("0.###");

						languageTitle = mediaInfo.Get(streamKind, i, indexStart, eInfoType.Text);
						language = languageTitle.Contains(':') ? languageTitle.Substring(0, languageTitle.IndexOf(':')) : null;
						title = languageTitle.Substring(language == null ? 0 : language.Length + 1);

						subNode = entryNode.AppendChild(xmlDoc.CreateElement("Title"));
						subNode.AppendChild(xmlDoc.CreateTextNode(title));
						if(language != null) subNode.Attributes.Append(xmlDoc.CreateAttribute("lang")).Value = language;
					}
				}
			}
		}

		private static void CreateChaptersMIL(XmlDocument xmlDoc, XmlNode chaptersNode, MediaInfo mediaInfo) {
			Func<string, string, XmlNode> createNode = (name, value) => {
				var n = xmlDoc.CreateElement(name);
				if(value != null) n.AppendChild(xmlDoc.CreateTextNode(value));
				return n;
			};

			XmlNode editionEntryNode, chapterAtomNode, chapterDisplayNode;
			eStreamType streamKind = eStreamType.Menu;
			int streamCount, entryCount;
			streamCount = mediaInfo.Count_Get(streamKind);

			for(int i = 0;i < streamCount;i++) {
				entryCount = mediaInfo.Count_Get(streamKind, i);
				editionEntryNode = chaptersNode.AppendChild(xmlDoc.CreateElement("EditionEntry"));

				int indexStart, indexEnd;
				if(int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_Begin"), out indexStart) && int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_End"), out indexEnd)) {
					chapterAtomNode = editionEntryNode.AppendChild(xmlDoc.CreateElement("ChapterAtom"));
					for(;indexStart < indexEnd;indexStart++) {

						Func<string, ulong> conv = (str) => {
							var timeParts = str.Split(new char[] { ':', '.' }).Select(s => ulong.Parse(s.Trim())).ToArray();
							return (((timeParts[0] * 60 + timeParts[1]) * 60 + timeParts[2]) * 1000 + timeParts[3]) * 1000000;
						};

						var timeStamps = mediaInfo.Get(streamKind, i, indexStart, eInfoType.Name).Split('-');
						ulong timestampStart = conv(timeStamps[0].Trim());
						ulong timestampEnd;
						if(timeStamps.Length > 1) timestampEnd = conv(timeStamps[1].Trim());


						chapterAtomNode.AppendChild(createNode("ChapterTimeStart", timestampStart.ToString()));
						chapterDisplayNode = chapterAtomNode.AppendChild(xmlDoc.CreateElement("ChapterDisplay"));

						string languageTitle, language, title;
						languageTitle = mediaInfo.Get(streamKind, i, indexStart, eInfoType.Text);
						language = languageTitle.Contains(':') ? languageTitle.Substring(0, languageTitle.IndexOf(':')) : null;
						title = languageTitle.Substring(language == null ? 0 : language.Length + 1);
						chapterDisplayNode.AppendChild(createNode("ChapterString", title));
						if(language != null) chapterDisplayNode.AppendChild(createNode("ChapterLanguage", language.Equals("en") ? "eng" : language));
					}
				}
			}
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
					AppendLeaf(xmlDoc, subNode, hashExecute.Name, BaseConverter.ToString(hashExecute.HashObj.Hash), null);

					if(hashExecute.HashObj is Ed2k) {
						Ed2k ed2k = (Ed2k)hashExecute.HashObj;
						if(!ed2k.BlueIsRed) AppendLeaf(xmlDoc, subNode, "Ed2k_Alt", BaseConverter.ToString(ed2k.BlueHash), null);
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

	public class InfoCollection : Dictionary<StreamTypeEntryPair, InfoEntry> { public void Add(InfoEntry item) { Add(item.Key, item); } }
}

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
using System.Xml;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.InfoGathering.Parser.CSEBMLLib;
using CSEBMLLib;
using CSEBMLLib.DocTypes;
using CSEBMLLib.DocTypes.Elements;
using CSEBMLLib.DocTypes.Matroska;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;

namespace AVDump2Lib.BlockConsumers {
	public class MatroskaParser : BlockConsumerBase {
		private FileSource dataSrc;
		private EBMLReader reader;
		private XmlDocument xmlDoc;

		public MatroskaParser(string name) : base(name) { xmlDoc = new XmlDocument(); }

		protected override void DoWork() {
			dataSrc = new FileSource(b, consumerId);
			reader = new EBMLReader(dataSrc, new DocTypeEBML(new IDocTypeExtension[] { new DocTypeMatroskaV2() }));

			xmlDoc.AppendChild(xmlDoc.CreateElement("AVD2Entry".ToLower()));
		}

		/*public MatroskaParser(string name) : base(name) { xmlDoc = new XmlDocument(); }

		protected override void DoWork() {
			dataSrc = new FileSource(b, consumerId);
			reader = new EBMLReader(dataSrc, new DocTypeEBML(new IDocTypeExtension[] { new DocTypeMatroskaV2() }));

			xmlDoc.AppendChild(xmlDoc.CreateElement("Ebml"));
			Recurse(xmlDoc.FirstChild);
		}

		private void Recurse(XmlNode node) {
			ElementInfo elementInfo;

			XmlNode subNode;
			while((elementInfo = reader.NextElementInfo()) != null) {
				#region Xml Node creation
				subNode = xmlDoc.CreateElement(elementInfo.ElementType.Name);
				subNode.Attributes.Append(xmlDoc.CreateAttribute("Id")).Value = Convert.ToString(elementInfo.ElementType.Id, 16);
				subNode.Attributes.Append(xmlDoc.CreateAttribute("Pos")).Value = Convert.ToString(elementInfo.Position, 16);
				subNode.Attributes.Append(xmlDoc.CreateAttribute("Length")).Value = elementInfo.Length.ToString();
				subNode.Attributes.Append(xmlDoc.CreateAttribute("Type")).Value = elementInfo.ElementType.Type.ToString();
				#endregion

				if(elementInfo.ElementType.Type == ElementType.eType.Master) {
					reader.EnterMasterElement();
					Recurse(subNode);
					reader.LeaveMasterElement();

				} else {
					if(elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.Block || elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.SimpleBlock) {
						#region Matroska Block
						MatroskaBlock matroskaBlock = (MatroskaBlock)reader.RetrieveValue();

						subNode.InnerText = "TN(" + matroskaBlock.TrackNumber + ")";

						if(matroskaBlock.Flags != 0) {
							MatroskaBlock.BlockFlag f = matroskaBlock.Flags;
							subNode.InnerText += " Flags(" + ((f & MatroskaBlock.BlockFlag.Discardable) != 0 ? "D" : "") +
															 ((f & MatroskaBlock.BlockFlag.Invisible) != 0 ? "I" : "") +
															 ((f & MatroskaBlock.BlockFlag.Keyframe) != 0 ? "K" : "");

							MatroskaBlock.LaceType l = matroskaBlock.LacingType;
							subNode.InnerText += (l == MatroskaBlock.LaceType.Ebml ? "E" : (l == MatroskaBlock.LaceType.Fixed ? "F" : (l == MatroskaBlock.LaceType.Xiph ? "X" : ""))) + ")";


							if(l != MatroskaBlock.LaceType.None) subNode.InnerText += " FC(" + matroskaBlock.FrameCount + ")";
						}
						subNode.InnerText += " DL(" + matroskaBlock.DataLength + ")";
						#endregion

					} else if(elementInfo.ElementType.Type != ElementType.eType.Binary) {
						object value = reader.RetrieveValue();
						subNode.InnerText = value.ToString();
					}
				}
				node.AppendChild(subNode);
				ProcessedBytes = dataSrc.Position;
			}
		}*/
	}

	public class MatroskaFileInfo : BlockConsumerBase {
		private FileSource dataSrc;
		private EBMLReader reader;

		public EbmlHeaderSection EbmlHeader { get; private set; }
		public SegmentSection Segment { get; private set; }

		public MatroskaFileInfo(string name) : base(name) { }

		protected override void DoWork() {
			dataSrc = new FileSource(b, consumerId);
			reader = new EBMLReader(dataSrc, new DocTypeEBML(new IDocTypeExtension[] { new DocTypeMatroskaV2() }));

			Root();
		}

		private void Root() {
			ElementInfo elementInfo = reader.NextElementInfo();
			if(elementInfo.ElementType.Id == (int)DocTypeEBML.eId.EBMLHeader) {
				EbmlHeader = new EbmlHeaderSection();
				EbmlHeader.Read(reader);
			} else {
				//Todo: dispose reader
				while(b.EndOfStream(consumerId)) {
					while(!b.CanRead(consumerId)) Thread.Sleep(20);
					b.Advance(consumerId);
				}
			}

			while((elementInfo = reader.NextElementInfo()) != null && Section.IsGlobalElement(elementInfo)) ;
			if(elementInfo == null || elementInfo.ElementType.Id != (int)DocTypeMatroskaV2.eId.Segment) {
				//error
			} else {
				Segment = new SegmentSection();
			}
		}

		public class EbmlHeaderSection : Section {
			public ulong EbmlVersion { get; private set; }
			public ulong EbmlReadVersion { get; private set; }
			public ulong EbmlMaxIdLength { get; private set; }
			public ulong EbmlMaxSizeLength { get; private set; }
			public string DocType { get; private set; }
			public ulong DocTypeVersion { get; private set; }
			public ulong DocTypeReadVersion { get; private set; }

			public EbmlHeaderSection() {
				EbmlVersion = 1;
				EbmlReadVersion = 1;
				EbmlMaxIdLength = 4;
				EbmlMaxSizeLength = 8;
				DocType = "matroska";
				DocTypeVersion = 1;
				DocTypeReadVersion = 1;
			}

			protected sealed override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
				switch((DocTypeEBML.eId)elementInfo.ElementType.Id) {
					case DocTypeEBML.eId.DocType:
						DocType = (string)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.DocTypeReadVersion:
						DocTypeReadVersion = (ulong)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.DocTypeVersion:
						DocTypeVersion = (ulong)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.EBMLMaxIDLength:
						EbmlMaxIdLength = (ulong)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.EBMLMaxSizeLength:
						EbmlMaxSizeLength = (ulong)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.EBMLReadVersion:
						EbmlReadVersion = (ulong)reader.RetrieveValue(); break;
					case DocTypeEBML.eId.EBMLVersion:
						EbmlVersion = (ulong)reader.RetrieveValue(); break;
					default:
						return false;
				}
				return true;
			}
		}
		public class SegmentSection : Section {
			public SegmentInfoSection SegmentInfo { get; private set; }
			public SeekHeadSection SeekHead { get; private set; }

			protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
				switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
					case DocTypeMatroskaV2.eId.Cluster:
						break;
					case DocTypeMatroskaV2.eId.SeekHead:
						break;
					case DocTypeMatroskaV2.eId.Tracks:
						break;
					case DocTypeMatroskaV2.eId.Info:
						SegmentInfo = new SegmentInfoSection();
						SegmentInfo.Read(reader);
						break;
					case DocTypeMatroskaV2.eId.Chapters:
						break;
					case DocTypeMatroskaV2.eId.Attachments:
						break;
					case DocTypeMatroskaV2.eId.Tags:
						break;

					default:
						return false;
				}
				return true;
			}

			//Cues

			public class SegmentInfoSection : Section {
				private byte[] segmentUId, prevUId, nextUId, segmentFamily;

				public byte[] SegmentUId { get { return segmentUId == null ? null : (byte[])segmentUId.Clone(); } }
				public string SegmentFilename { get; private set; }
				public byte[] PreviousUId { get { return prevUId == null ? null : (byte[])prevUId.Clone(); } }
				public string PreviousFilename { get; private set; }
				public byte[] NextUId { get { return nextUId == null ? null : (byte[])nextUId.Clone(); } }
				public string NextFilename { get; private set; }
				public byte[] SegmentFamily { get { return segmentFamily == null ? null : (byte[])segmentFamily.Clone(); } }
				public ulong? TimecodeScale { get; private set; }
				public double? Duration { get; private set; }
				public string Title { get; private set; }
				public string MuxingApp { get; private set; }
				public string WritingApp { get; private set; }
				public DateTime? ProductionDate { get; private set; }

				public EbmlEnumerable<ChapterTranslateSection> ChapterTranslate { get; private set; }

				public SegmentInfoSection() {
					ChapterTranslate = new EbmlEnumerable<ChapterTranslateSection>();
				}

				protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
					switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
						case DocTypeMatroskaV2.eId.SegmentUID:
							segmentUId = (byte[])reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.SegmentFilename:
							SegmentFilename = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.PrevUID:
							prevUId = (byte[])reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.PrevFilename:
							PreviousFilename = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.NextUID:
							nextUId = (byte[])reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.NextFilename:
							NextFilename = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.SegmentFamily:
							segmentFamily = (byte[])reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.ChapterTranslate:
							Section.CreateReadAdd(new ChapterTranslateSection(), reader, ChapterTranslate); break;
						case DocTypeMatroskaV2.eId.TimecodeScale:
							TimecodeScale = (ulong)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.Duration:
							Duration = (double)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.Title:
							Title = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.MuxingApp:
							MuxingApp = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.WritingApp:
							WritingApp = (string)reader.RetrieveValue(); break;
						case DocTypeMatroskaV2.eId.DateUTC:
							ProductionDate = (DateTime)reader.RetrieveValue(); break;
						default:
							return false;
					}
					return true;
				}

				public class ChapterTranslateSection : Section {
					public ulong EditionUId { get; private set; }
					public ulong Codec { get; private set; }
					//Id (Binary)

					protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
						throw new NotImplementedException();
					}
				}
			}

			public class ClusterSection : Section {
				private List<TrackInfo> tracks = new List<TrackInfo>();

				protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
					switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
						case DocTypeMatroskaV2.eId.SimpleBlock:
						case DocTypeMatroskaV2.eId.Block:
							MatroskaBlock matroskaBlock = (MatroskaBlock)reader.RetrieveValue();
							if(matroskaBlock.TrackNumber > tracks.Count) for(int i = tracks.Count;i < matroskaBlock.TrackNumber;i++) tracks.Add(new TrackInfo(i));
							tracks[matroskaBlock.TrackNumber - 1].ProcessBlock(matroskaBlock);
							break;

						case DocTypeMatroskaV2.eId.BlockGroup:
							reader.EnterMasterElement();
							ProcessElement(reader, reader.NextElementInfo());
							reader.LeaveMasterElement();
							break;

						case DocTypeMatroskaV2.eId.Timecode:
							foreach(var track in tracks) track.EndOfCluster((long)reader.RetrieveValue());
							break;

						default:
							return false;
					}
					return true;
				}

				public class TrackInfo {
					public int Index { get; private set; }

					private long trackSize;

					private uint frames;
					private long minStamp;
					private long highStamp;

					private Collection<ClusterInfo> clusters;

					private uint clusterFrames;
					private uint clusterSize;
					private long clusterStamp;

					public TrackInfo(int index) {
						this.Index = index;
						clusters = new Collection<ClusterInfo>();
					}

					public void EndOfCluster(long clusterStamp) {
						clusters.Add(new ClusterInfo((ulong)this.clusterStamp, clusterFrames, clusterSize));

						clusterSize = clusterFrames = 0;
						this.clusterStamp = clusterStamp;
					}

					public void ProcessBlock(MatroskaBlock block) {
						clusterSize += block.DataLength;
						trackSize += block.DataLength;

						clusterFrames += block.FrameCount;
						frames += block.FrameCount;

						if(highStamp < block.TimeCode + clusterStamp) highStamp = block.TimeCode + clusterStamp;
						if(minStamp > block.TimeCode + clusterStamp) minStamp = block.TimeCode + clusterStamp;
					}

					public struct ClusterInfo {
						public ulong clusterStamp;
						public uint frames;
						public uint size;

						public ClusterInfo(ulong clusterStamp, uint frames, uint size) {
							this.frames = frames; this.size = size; this.clusterStamp = clusterStamp;
						}
					}
				}
			}

			public class SeekHeadSection {
				public class SeekSection {
					public ulong Id { get; private set; }
					public ulong Position { get; private set; }
				}
			}
			public class TracksSection : Section {
				public EbmlEnumerable<TrackEntrySection> Tracks { get; private set; }

				protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
					if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.TrackEntry) {
						Section.CreateReadAdd(new TrackEntrySection(), reader, tracks);
						return true;
					}
					return false;
				}

				public class TrackEntrySection : Section {
					public ulong? TrackNumber { get; private set; } //Not 0; Mandatory
					public ulong? TrackUId { get; private set; } //Not 0
					public Types TrackType { get; private set; } //Mandatory
					public Options TrackFlags { get; private set; } //Set: Default, Enabled
					public ulong? MinCache { get; private set; } //Default: 0
					public ulong? MaxCache { get; private set; }
					public ulong? DefaultDuration { get; private set; }
					public double? TrackTimecodeScale { get; private set; }
					public string Name { get; private set; }
					public string Language { get; private set; } //Default: 'eng'
					public string CodecId { get; private set; } //Mandatory
					//CodecPrivate
					public string CodecName { get; private set; }
					//AttachmentLink #>=1

					public VideoSection Video { get; private set; }
					public AudioSection Audio { get; private set; }
					public ContentEncodingsSection ContentEncodings { get; private set; }

					protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
						switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
							case DocTypeMatroskaV2.eId.TrackNumber: break;
							case DocTypeMatroskaV2.eId.TrackUID: break;
							case DocTypeMatroskaV2.eId.TrackType: break;
							case DocTypeMatroskaV2.eId.MinCache: break;
							case DocTypeMatroskaV2.eId.MaxCache: break;
							case DocTypeMatroskaV2.eId.DefaultDuration: break;
							case DocTypeMatroskaV2.eId.TrackTimecodeScale: break;
							case DocTypeMatroskaV2.eId.Name: break;
							case DocTypeMatroskaV2.eId.Language: break;
							case DocTypeMatroskaV2.eId.CodecID: break;
							case DocTypeMatroskaV2.eId.CodecName: break;
							case DocTypeMatroskaV2.eId.Video:
								Video = new VideoSection();
								Video.Read(reader);
								break;
							case DocTypeMatroskaV2.eId.Audio:
								Audio = new AudioSection();
								Audio.Read(reader);
								break;
							case DocTypeMatroskaV2.eId.ContentEncodings:
								ContentEncodings = new ContentEncodingsSection();
								ContentEncodings.Read(reader);
								break;
							default: return false;
						}
						return true;
					}

					[Flags]
					public enum Options { None = 0, Enabled = 1, Default = 2, Forced = 4, Lacing = 8 }
					public enum Types { Video = 0x1, Audio = 0x2, Complex = 0x3, Logo = 0x10, Subtitle = 0x11, Button = 0x12, Control = 0x20 }

					public class VideoSection : Section {
						public ulong? PixelWidth { get; private set; }
						public ulong? PixelHeight { get; private set; }
						public ulong? PixelCropBottom { get; private set; } //Default: 0
						public ulong? PixelCropTop { get; private set; } //Default: 0
						public ulong? PixelCropLeft { get; private set; } //Default: 0
						public ulong? PixelCropRight { get; private set; } //Default: 0
						public ulong? DisplayWidth { get; private set; } //Default: $PixelWidth
						public ulong? DisplayHeight { get; private set; } //Default: $PixelHeight
						public Unit DisplayUnit { get; private set; } //Default: Pixels (0)

						protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
							throw new NotImplementedException();
						}

						public enum Unit { Pixels, Centimeters, Inches }
					}
					public class AudioSection : Section {
						public ulong? SamplingFrequency { get; private set; } //Default: 8000
						public ulong? OutputSamplingFrequency { get; private set; }
						public ulong? ChannelCount { get; private set; } //Default: 1
						public ulong? BitDepth { get; private set; }

						protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
							throw new NotImplementedException();
						}
					}
					public class ContentEncodingsSection : Section {
						public class ContentEncodingSection {
							public ulong? ContentEncodingOrder { get; private set; } //Default: 0
							public CETypes ContentEncodingScope { get; private set; } //Default: 1
							public CETypes ContentEncodingType { get; private set; } //Default: 0

							protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
								throw new NotImplementedException();
							}

							[Flags]
							public enum CEScopes { AllFrames = 1, CodecPrivate = 2, ContentCompression = 4 }
							public enum CETypes { Compression, Encryption }
							public class ContentCompressionSection {
								public CompAlgos ContentCompAlgo { get; private set; }
								//ContentCompSetting

								public enum CompAlgos { zlib, bzlib, lzo1x, HeaderScripting }
							}
						}
					}
				}

			}
			public class AttachmentsSection {
				public class AttachedFileSection {
					public string FileDescription { get; private set; }
					public string FileName { get; private set; }
					public string FileMimeType { get; private set; }
					public ulong FileUId { get; private set; }
				}
			}
			public class ChaptersSection {

				public class EditionEntry {
					public ulong? EditionUId { get; private set; }
					public Options EditionFlags { get; private set; }
					public IEnumerable<ChapterAtomSection> ChapterAtom { get; private set; }

					[Flags]
					public enum Options { Hidden = 1, Default = 2, Ordered = 4 }

					public class ChapterAtomSection {
						private byte[] chapterSegmentUId;

						public ulong? ChapterUId { get; private set; }
						public ulong? ChapterTimeStart { get; private set; } //Def: 0
						public ulong? ChapterTimeEnd { get; private set; }
						public Options ChapterFlags { get; private set; }
						public byte[] ChapterSegmentUId { get { return chapterSegmentUId == null ? null : (byte[])chapterSegmentUId.Clone(); } }
						public ulong? ChapterSegmentEditionUId { get; private set; }

						public class ChapterTracksSection {
							public ulong ChapterTrackNumber { get; private set; }
						}
						public class ChapterDisplaySection {
							public string ChapterString { get; private set; }
							public string ChapterLanguage { get; private set; } //Def: eng
							public string ChapterCountry { get; private set; }
						}

						[Flags]
						public enum Options { Hidden = 1, Enabled = 2 }
					}
				}
			}
			public class TagsSection {
				public class TagSection {
					public class TargetsSection {
						public ulong? TargetTypeValue { get; private set; } //Def: 50
						public string TargetType { get; private set; }
						public ulong? TrackUId { get; private set; }
						public ulong? EditionUId { get; private set; }
						public ulong? ChapterUId { get; private set; }
						public ulong? AttachmentUId { get; private set; }
					}
					public class SimpleTagSection {
						public string TagName { get; private set; }
						public string TagLanguage { get; private set; } //Def: und
						public bool TagOriginal { get; private set; } //Def: True
						public string TagString { get; private set; }
						//TagBinary
					}
				}
			}
		}

		public abstract class Section {
			//public uint SectionId { get; private set; }

			internal void Read(EBMLReader reader) {
				reader.EnterMasterElement();

				ElementInfo elementInfo;
				while((elementInfo = reader.NextElementInfo()) != null) {
					if(!ProcessElement(reader, elementInfo) && !IsGlobalElement(elementInfo)) {
						//error
					}
				}

				reader.LeaveMasterElement();
			}

			internal static bool IsGlobalElement(ElementInfo elementInfo) {
				DocTypeEBML.eId id = (DocTypeEBML.eId)elementInfo.ElementType.Id;
				return id == DocTypeEBML.eId.CRC32 || id == DocTypeEBML.eId.Void;
			}
			internal static void CreateReadAdd<T>(T section, EBMLReader reader, EbmlEnumerable<T> lst) where T : Section {
				section.Read(reader);
				lst.Add(section);
			}

			protected abstract bool ProcessElement(EBMLReader reader, ElementInfo elementInfo);
		}
		public class EbmlEnumerable<T> : IEnumerable<T> {
			protected Collection<T> items;

			public EbmlEnumerable() { items = new Collection<T>(); }

			protected internal void Add(T item) { items.Add(item); }

			public IEnumerator<T> GetEnumerator() { return items.GetEnumerator(); }
			System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return items.GetEnumerator(); }
		}
	}
}

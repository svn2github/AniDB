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
using System.Linq;
using AVDump2Lib.BlockConsumers.Tools;
using AVDump2Lib.InfoGathering.Parser.CSEBMLLib;
using CSEBMLLib;
using CSEBMLLib.DocTypes;
using CSEBMLLib.DocTypes.Elements;
using CSEBMLLib.DocTypes.Matroska;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Threading;
using System.Diagnostics;

namespace AVDump2Lib.BlockConsumers {
	//public class MatroskaParser : BlockConsumerBase {
	//    private FileSource dataSrc;
	//    private EBMLReader reader;
	//    private XmlDocument xmlDoc;

	//    public MatroskaParser(string name) : base(name) { xmlDoc = new XmlDocument(); }

	//    protected override void DoWork() {
	//        dataSrc = new FileSource(b, consumerId);
	//        reader = new EBMLReader(dataSrc, new DocTypeEBML(new DocTypeMatroskaV2()));

	//        xmlDoc.AppendChild(xmlDoc.CreateElement("AVD2Entry".ToLower()));
	//    }

	//    /*public MatroskaParser(string name) : base(name) { xmlDoc = new XmlDocument(); }

	//    protected override void DoWork() {
	//        dataSrc = new FileSource(b, consumerId);
	//        reader = new EBMLReader(dataSrc, new DocTypeEBML(new IDocTypeExtension[] { new DocTypeMatroskaV2() }));

	//        xmlDoc.AppendChild(xmlDoc.CreateElement("Ebml"));
	//        Recurse(xmlDoc.FirstChild);
	//    }

	//    private void Recurse(XmlNode node) {
	//        ElementInfo elementInfo;

	//        XmlNode subNode;
	//        while((elementInfo = reader.NextElementInfo()) != null) {
	//            #region Xml Node creation
	//            subNode = xmlDoc.CreateElement(elementInfo.ElementType.Name);
	//            subNode.Attributes.Append(xmlDoc.CreateAttribute("Id")).Value = Convert.ToString(elementInfo.ElementType.Id, 16);
	//            subNode.Attributes.Append(xmlDoc.CreateAttribute("Pos")).Value = Convert.ToString(elementInfo.Position, 16);
	//            subNode.Attributes.Append(xmlDoc.CreateAttribute("Length")).Value = elementInfo.Length.ToString();
	//            subNode.Attributes.Append(xmlDoc.CreateAttribute("Type")).Value = elementInfo.ElementType.Type.ToString();
	//            #endregion

	//            if(elementInfo.ElementType.Type == ElementType.eType.Master) {
	//                reader.EnterMasterElement();
	//                Recurse(subNode);
	//                reader.LeaveMasterElement();

	//            } else {
	//                if(elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.Block || elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.SimpleBlock) {
	//                    #region Matroska Block
	//                    MatroskaBlock matroskaBlock = (MatroskaBlock)reader.RetrieveValue();

	//                    subNode.InnerText = "TN(" + matroskaBlock.TrackNumber + ")";

	//                    if(matroskaBlock.Flags != 0) {
	//                        MatroskaBlock.BlockFlag f = matroskaBlock.Flags;
	//                        subNode.InnerText += " Flags(" + ((f & MatroskaBlock.BlockFlag.Discardable) != 0 ? "D" : "") +
	//                                                         ((f & MatroskaBlock.BlockFlag.Invisible) != 0 ? "I" : "") +
	//                                                         ((f & MatroskaBlock.BlockFlag.Keyframe) != 0 ? "K" : "");

	//                        MatroskaBlock.LaceType l = matroskaBlock.LacingType;
	//                        subNode.InnerText += (l == MatroskaBlock.LaceType.Ebml ? "E" : (l == MatroskaBlock.LaceType.Fixed ? "F" : (l == MatroskaBlock.LaceType.Xiph ? "X" : ""))) + ")";


	//                        if(l != MatroskaBlock.LaceType.None) subNode.InnerText += " FC(" + matroskaBlock.FrameCount + ")";
	//                    }
	//                    subNode.InnerText += " DL(" + matroskaBlock.DataLength + ")";
	//                    #endregion

	//                } else if(elementInfo.ElementType.Type != ElementType.eType.Binary) {
	//                    object value = reader.RetrieveValue();
	//                    subNode.InnerText = value.ToString();
	//                }
	//            }
	//            node.AppendChild(subNode);
	//            ProcessedBytes = dataSrc.Position;
	//        }
	//    }*/

	//}

	public class MatroskaFileInfo : BlockConsumerBase {
		private FileSource dataSrc;
		private EBMLReader reader;

		public long FileSize { get; private set; }
		public EbmlHeaderSection EbmlHeader { get; private set; }
		public SegmentSection Segment { get; private set; }

		public MatroskaFileInfo(string name) : base(name) { }

		protected override void DoWork() {
			dataSrc = new FileSource(b, consumerId);
			reader = new EBMLReader(dataSrc, new DocTypeEBML(new DocTypeMatroskaV2()));

			FileSize = dataSrc.Length;

			Thread t = new Thread(ReadBytesUpdater);
			t.Start();

			Root();
		}

		private void Root() {
			ElementInfo elementInfo = reader.NextElementInfo();
			if(elementInfo.ElementType.Id == (int)DocTypeEBML.eId.EBMLHeader) {
				EbmlHeader = Section.CreateRead(new EbmlHeaderSection(), reader);
			} else {
				//Todo: dispose reader
				DummyRead();
			}

			while((elementInfo = reader.NextElementInfo()) != null && Section.IsGlobalElement(elementInfo)) ;

			if(elementInfo != null && elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.Segment) {
				Segment = Section.CreateRead(new SegmentSection(), reader);
			} else {
				//error
				DummyRead();
			}
		}
		private void DummyRead() {
			while(b.EndOfStream(consumerId)) {
				while(!b.CanRead(consumerId)) Thread.Sleep(20);
				b.Advance(consumerId);
			}
		}
		private void ReadBytesUpdater() {
			while(!HasFinished) {
				Thread.Sleep(20);
				ProcessedBytes = dataSrc.Position;
			}
			ProcessedBytes = dataSrc.Length;
		}


	}
	public class EbmlHeaderSection : Section {
		#region Fields & Properties
		private ulong? ebmlVersion;
		private ulong? ebmlReadVersion;
		private ulong? ebmlMaxIdLength;
		private ulong? ebmlMaxSizeLength;
		private string docType;
		private ulong? docTypeVersion;
		private ulong? docTypeReadVersion;

		public ulong EbmlVersion { get { return ebmlVersion.HasValue ? ebmlVersion.Value : 1; } }
		public ulong EbmlReadVersion { get { return ebmlReadVersion.HasValue ? ebmlReadVersion.Value : 1; } }
		public ulong EbmlMaxIdLength { get { return ebmlMaxIdLength.HasValue ? ebmlMaxIdLength.Value : 4; } }
		public ulong EbmlMaxSizeLength { get { return ebmlMaxSizeLength.HasValue ? ebmlMaxSizeLength.Value : 8; } }
		public ulong DocTypeReadVersion { get { return docTypeReadVersion.HasValue ? docTypeReadVersion.Value : 1; } }
		public ulong DocTypeVersion { get { return docTypeVersion.HasValue ? docTypeVersion.Value : 1; } }
		public string DocType { get { return docType != null ? docType : "matroska"; } }
		#endregion

		protected sealed override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeEBML.eId)elementInfo.ElementType.Id) {
				case DocTypeEBML.eId.DocType:
					docType = (string)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.DocTypeReadVersion:
					docTypeReadVersion = (ulong)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.DocTypeVersion:
					docTypeVersion = (ulong)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.EBMLMaxIDLength:
					ebmlMaxIdLength = (ulong)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.EBMLMaxSizeLength:
					ebmlMaxSizeLength = (ulong)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.EBMLReadVersion:
					ebmlReadVersion = (ulong)reader.RetrieveValue(); break;
				case DocTypeEBML.eId.EBMLVersion:
					ebmlVersion = (ulong)reader.RetrieveValue(); break;
				default:
					return false;
			}
			return true;
		}
		protected sealed override void Validate() { }
	}
	public class SegmentSection : Section {
		#region Fields & Properties
		public SegmentInfoSection SegmentInfo { get; private set; }
		public AttachmentsSection Attachments { get; private set; }
		public ChaptersSection Chapters { get; private set; }
		public ClusterSection Cluster { get; private set; }
		public TracksSection Tracks { get; private set; }
		//public SeekHeadSection SeekHead { get; private set; }
		#endregion

		public SegmentSection() { Cluster = new ClusterSection(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.Cluster:
					Cluster.Read(reader); break;
				case DocTypeMatroskaV2.eId.Tracks:
					Tracks = Section.CreateRead(new TracksSection(), reader); Cluster.SetTrackCount(Tracks.Items.Count); break;
				case DocTypeMatroskaV2.eId.Info:
					SegmentInfo = Section.CreateRead(new SegmentInfoSection(), reader); break;
				case DocTypeMatroskaV2.eId.Chapters:
					Chapters = Section.CreateRead(new ChaptersSection(), reader); break;
				case DocTypeMatroskaV2.eId.Attachments:
					Attachments = Section.CreateRead(new AttachmentsSection(), reader); break;
				case DocTypeMatroskaV2.eId.Tags:
					break;
				/*case DocTypeMatroskaV2.eId.SeekHead:
					break;
				case DocTypeMatroskaV2.eId.Cues:
					break;*/
				default:
					return false;
			}
			return true;
		}
		protected override void Validate() { }

	}
	public class SegmentInfoSection : Section {
		#region Fields & Properties
		private ulong? timecodeScale;
		private EbmlList<byte[]> segmentFamily;
		private byte[] segmentUId, prevUId, nextUId;

		public byte[] SegmentUId { get { return segmentUId == null ? null : (byte[])segmentUId.Clone(); } }
		public byte[] PreviousUId { get { return prevUId == null ? null : (byte[])prevUId.Clone(); } }
		public byte[] NextUId { get { return nextUId == null ? null : (byte[])nextUId.Clone(); } }
		public string SegmentFilename { get; private set; }
		public string PreviousFilename { get; private set; }
		public string NextFilename { get; private set; }
		public EbmlList<byte[]> SegmentFamily { get { return segmentFamily.DeepClone(item => { return (byte[])item.Clone(); }); } }
		public ulong TimecodeScale { get { return timecodeScale.HasValue ? timecodeScale.Value : 1000000; } }
		public double? Duration { get; private set; }
		public string Title { get; private set; }
		public string MuxingApp { get; private set; }
		public string WritingApp { get; private set; }
		public DateTime? ProductionDate { get; private set; }

		public EbmlList<ChapterTranslateSection> ChapterTranslate { get; private set; }
		#endregion

		public SegmentInfoSection() {
			segmentFamily = new EbmlList<byte[]>();
			ChapterTranslate = new EbmlList<ChapterTranslateSection>();
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
					segmentFamily.Add((byte[])reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.ChapterTranslate:
					Section.CreateReadAdd(new ChapterTranslateSection(), reader, ChapterTranslate); break;
				case DocTypeMatroskaV2.eId.TimecodeScale:
					timecodeScale = (ulong)reader.RetrieveValue(); break;
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
		protected override void Validate() { }
	}
	public class ChapterTranslateSection : Section {
		private byte[] id;

		public EbmlList<ulong> EditionUId { get; private set; }
		public ulong? Codec { get; private set; }
		public byte[] Id { get { return id != null ? (byte[])id.Clone() : null; } }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterTranslateEditionUID:
					EditionUId.Add((ulong)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.ChapterTranslateCodec:
					Codec = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapterTranslateID:
					id = (byte[])reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}

	public class ClusterSection : Section {
		public List<Track> Tracks { get; private set; }

		public ClusterSection() { }

		public void SetTrackCount(int count) {
			Tracks = new List<Track>();
			for(int i = 0;i < count;i++) Tracks.Add(new Track(i));
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.SimpleBlock:
				case DocTypeMatroskaV2.eId.Block:
					MatroskaBlock matroskaBlock = (MatroskaBlock)reader.RetrieveValue();
					Tracks[matroskaBlock.TrackNumber - 1].ProcessBlock(matroskaBlock);
					break;

				case DocTypeMatroskaV2.eId.BlockDuration: break;
				case DocTypeMatroskaV2.eId.ReferenceBlock: break;
				case DocTypeMatroskaV2.eId.BlockGroup:
					Read(reader); break;
				case DocTypeMatroskaV2.eId.Timecode:
					long timeCode = (long)(ulong)reader.RetrieveValue();
					foreach(var track in Tracks) track.BeginOfCluster(timeCode);
					break;

				default: return false;
			}
			return true;
		}
		protected override void Validate() { foreach(var track in Tracks) track.EndOfCluster(); }

		public class Track {
			public int Index { get; private set; }

			private long trackSize;

			private uint laces;
			private long minStamp;
			private long highStamp;

			private Collection<ClusterInfo> clusters;

			private uint clusterLaces;
			private uint clusterSize;
			private long timeCode;

			internal Track(int index) {
				this.Index = index;
				clusters = new Collection<ClusterInfo>();
			}

			internal void EndOfCluster() { clusters.Add(new ClusterInfo((ulong)timeCode, clusterLaces, clusterSize)); }
			internal void BeginOfCluster(long timeCode) {
				this.timeCode = timeCode;
				clusterSize = clusterLaces = 0;
			}
			internal void ProcessBlock(MatroskaBlock block) {
				clusterSize += block.DataLength;
				trackSize += block.DataLength;

				clusterLaces += block.FrameCount;
				laces += block.FrameCount;

				if(highStamp < block.TimeCode + timeCode) highStamp = block.TimeCode + timeCode;
				if(minStamp > block.TimeCode + timeCode) minStamp = block.TimeCode + timeCode;
			}

			public TrackInfo CalcTrackInfo(double mult) {
				var trackLength = TimeSpan.FromMilliseconds(highStamp * (mult / 1000000));
				return new TrackInfo(
					minBitrate: 0,
					maxBitrate: 0,
					averageBitrate: trackSize * 8 / trackLength.TotalSeconds,
					minLaceRate: 0,
					maxLaceRate: 0,
					averageLaceRate: laces / trackLength.TotalSeconds,
					trackLength: trackLength,
					trackSize: trackSize,
					laceCount: (int)laces
				);

			}


		}
		public class TrackInfo {
			public double MinBitrate { get; private set; }
			public double MaxBitrate { get; private set; }
			public double AverageBitrate { get; private set; }

			public double MinLaceRate { get; private set; }
			public double MaxLaceRate { get; private set; }
			public double AverageLaceRate { get; private set; }

			public TimeSpan TrackLength { get; private set; }
			public long TrackSize { get; private set; }
			public int LaceCount { get; private set; }

			public TrackInfo(double minBitrate, double maxBitrate, double averageBitrate, double minLaceRate, double maxLaceRate, double averageLaceRate, TimeSpan trackLength, long trackSize, int laceCount) {
				MinBitrate = minBitrate; MaxBitrate = maxBitrate; AverageBitrate = averageBitrate;
				MinLaceRate = minLaceRate; MaxLaceRate = maxLaceRate; AverageLaceRate = averageLaceRate;
				TrackLength = trackLength; TrackSize = trackSize; LaceCount = laceCount;
			}
		}
		struct ClusterInfo {
			public ulong clusterStamp;
			public uint frames;
			public uint size;

			public ClusterInfo(ulong clusterStamp, uint frames, uint size) {
				this.frames = frames; this.size = size; this.clusterStamp = clusterStamp;
			}
		}
	}
	public class TracksSection : Section {
		#region Fields & Properties
		public EbmlList<TrackEntrySection> Items { get; private set; }

		public TrackEntrySection this[TrackEntrySection.Types type, int index] {
			get {
				for(int i = 0;i < Items.Count;i++) if(Items[i].TrackType == type && index-- == 0) return Items[i];
				throw new Exception("Index out of range");
			}
		}
		public int Count(TrackEntrySection.Types type) { return Items.Aggregate<TrackEntrySection, int>(0, (acc, item) => { return acc + item.TrackType == type ? 1 : 0; }); }
		#endregion

		public TracksSection() { Items = new EbmlList<TrackEntrySection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.TrackEntry) {
				Section.CreateReadAdd(new TrackEntrySection(), reader, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }
	}
	public class TrackEntrySection : Section {
		#region Fields & Properties
		private bool? enabled, def, forced, lacing;
		private string language;
		private ulong? minCache;
		private ulong? maxBlockAdditionID;

		public ulong? TrackNumber { get; private set; } //Not 0; Mandatory
		public ulong? TrackUId { get; private set; } //Not 0
		public EbmlList<ulong> TrackOverlay { get; private set; }
		public Types? TrackType { get; private set; } //Mandatory
		public Options TrackFlags {
			get { //Set: Default, Enabled
				return (!enabled.HasValue || enabled.Value ? Options.Enabled : Options.None) |
					   (forced.HasValue && forced.Value ? Options.Forced : Options.None) |
					   (!def.HasValue || def.Value ? Options.Default : Options.None) |
					   (lacing.HasValue && lacing.Value ? Options.Lacing : Options.None);
			}
		}
		public ulong MinCache { get { return minCache.HasValue ? minCache.Value : 0; } } //Default: 0
		public ulong? MaxCache { get; private set; }
		public ulong MaxBlockAdditionID { get { return maxBlockAdditionID.HasValue ? maxBlockAdditionID.Value : 0; } } //Default: 0
		public ulong? DefaultDuration { get; private set; }
		public double? TrackTimecodeScale { get; private set; }
		public string Name { get; private set; }
		public string Language { get { return language != null ? language : "eng"; } } //Default: 'eng'
		public string CodecId { get; private set; } //Mandatory
		//CodecPrivate
		public string CodecName { get; private set; }
		public string AttachmentLink { get; private set; }

		public VideoSection Video { get; private set; }
		public AudioSection Audio { get; private set; }
		public ContentEncodingsSection ContentEncodings { get; private set; }
		#endregion

		public TrackEntrySection() { TrackOverlay = new EbmlList<ulong>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.TrackNumber:
					TrackNumber = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TrackUID:
					TrackUId = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TrackOverlay:
					TrackOverlay.Add((ulong)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.TrackType:
					TrackType = (Types)(ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.MinCache:
					minCache = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.MaxCache:
					MaxCache = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.MaxBlockAdditionID:
					maxBlockAdditionID = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.DefaultDuration:
					DefaultDuration = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TrackTimecodeScale:
					TrackTimecodeScale = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.Name:
					Name = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.Language:
					language = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.CodecID:
					CodecId = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.CodecName:
					CodecName = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.AttachmentLink:
					AttachmentLink = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FlagEnabled:
					enabled = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.FlagDefault:
					def = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.FlagForced:
					forced = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.FlagLacing:
					lacing = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.Video:
					Video = Section.CreateRead(new VideoSection(), reader); break;
				case DocTypeMatroskaV2.eId.Audio:
					Audio = Section.CreateRead(new AudioSection(), reader); break;
				case DocTypeMatroskaV2.eId.ContentEncodings:
					ContentEncodings = Section.CreateRead(new ContentEncodingsSection(), reader); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		[Flags]
		public enum Options { None = 0, Enabled = 1, Default = 2, Forced = 4, Lacing = 8 }
		public enum Types { Video = 0x1, Audio = 0x2, Complex = 0x3, Logo = 0x10, Subtitle = 0x11, Button = 0x12, Control = 0x20 }
	}
	public class VideoSection : Section {
		#region Fields & Properties
		public ulong? pixelCropBottom;
		public ulong? pixelCropTop;
		public ulong? pixelCropLeft;
		public ulong? pixelCropRight;
		public ulong? displayWidth;
		public ulong? displayHeight;
		public bool? interlaced;
		public byte[] colorSpace;
		public ARType? aspectRatioType;
		public Unit? displayUnit;

		public double? FrameRate { get; private set; }
		public byte[] ColorSpace { get { return colorSpace != null ? (byte[])colorSpace.Clone() : null; } }
		public ulong PixelWidth { get; private set; }
		public ulong PixelHeight { get; private set; }
		public ARType AspectRatioType { get { return aspectRatioType.HasValue ? aspectRatioType.Value : ARType.FreeResizing; } } //Default: FreeResizing (0)
		public bool Interlaced { get { return interlaced.HasValue ? interlaced.Value : false; } } //Default: FreeResizing (0)
		public ulong PixelCropBottom { get { return pixelCropBottom.HasValue ? pixelCropBottom.Value : 0; } } //Default: 0
		public ulong PixelCropTop { get { return pixelCropTop.HasValue ? pixelCropTop.Value : 0; } } //Default: 0
		public ulong PixelCropLeft { get { return pixelCropLeft.HasValue ? pixelCropLeft.Value : 0; } } //Default: 0
		public ulong PixelCropRight { get { return pixelCropRight.HasValue ? pixelCropRight.Value : 0; } } //Default: 0
		public ulong DisplayWidth { get { return displayWidth.HasValue ? displayWidth.Value : PixelWidth; } } //Default: $PixelWidth
		public ulong DisplayHeight { get { return displayHeight.HasValue ? displayHeight.Value : PixelHeight; } } //Default: $PixelHeight
		public Unit DisplayUnit { get { return displayUnit.HasValue ? displayUnit.Value : Unit.Pixels; } } //Default: Pixels (0)
		#endregion

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.PixelWidth:
					PixelWidth = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.PixelHeight:
					PixelHeight = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.PixelCropBottom:
					pixelCropBottom = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.PixelCropTop:
					pixelCropTop = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.PixelCropLeft:
					pixelCropLeft = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.PixelCropRight:
					pixelCropRight = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.DisplayWidth:
					displayWidth = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.DisplayHeight:
					displayHeight = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.DisplayUnit:
					displayUnit = (Unit)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FrameRate:
					FrameRate = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FlagInterlaced:
					interlaced = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.ColourSpace:
					colorSpace = (byte[])reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public enum Unit { Pixels, Centimeters, Inches }
		public enum ARType { FreeResizing, KeepAR, Fixed }
	}
	public class AudioSection : Section {
		#region Fields & Properties
		private double? samplingFrequency;
		private ulong? channelCount;

		public double SamplingFrequency { get { return samplingFrequency.HasValue ? samplingFrequency.Value : 8000d; } } //Default: 8000
		public double? OutputSamplingFrequency { get; private set; }
		public ulong ChannelCount { get { return channelCount.HasValue ? channelCount.Value : 1; } } //Default: 1
		public ulong? BitDepth { get; private set; }
		#endregion

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.SamplingFrequency:
					samplingFrequency = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.OutputSamplingFrequency:
					OutputSamplingFrequency = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.Channels:
					channelCount = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.BitDepth:
					BitDepth = (ulong)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}
	public class ContentEncodingsSection : Section {
		public EbmlList<ContentEncodingSection> Encodings { get; private set; }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.ContentEncodings) {
				Section.CreateReadAdd(new ContentEncodingSection(), reader, Encodings);
				return true;
			}
			return false;
		}
		protected override void Validate() { }
	}
	public class ContentEncodingSection : Section {
		#region Fields & Properties
		private ulong? contentEncodingOrder;
		private CEScopes? contentEncodingScope;
		private CETypes? contentEncodingType;

		public ulong? ContentEncodingOrder { get { return contentEncodingOrder.HasValue ? contentEncodingOrder.Value : 0; } } //Default: 0
		public CEScopes ContentEncodingScope { get { return contentEncodingScope.HasValue ? contentEncodingScope.Value : CEScopes.AllFrames; } } //Default: 1
		public CETypes ContentEncodingType { get { return contentEncodingType.HasValue ? contentEncodingType.Value : CETypes.Compression; } } //Default: 0

		public ContentCompressionSection ContentCompression { get; private set; }
		#endregion

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ContentEncodingOrder:
					contentEncodingOrder = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentEncodingScope:
					contentEncodingScope = (CEScopes)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentEncodingType:
					contentEncodingType = (CETypes)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentCompression:
					ContentCompression = Section.CreateRead(new ContentCompressionSection(), reader); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		[Flags]
		public enum CEScopes { AllFrames = 1, CodecPrivate = 2, ContentCompression = 4 }
		public enum CETypes { Compression = 0, Encryption = 1 }
	}
	public class ContentCompressionSection : Section {
		#region Fields & Properties
		private CompAlgos? contentCompAlgo;

		public CompAlgos ContentCompAlgo { get { return contentCompAlgo.HasValue ? contentCompAlgo.Value : CompAlgos.zlib; } }
		//ContentCompSetting
		#endregion

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ContentCompAlgo:
					contentCompAlgo = (CompAlgos)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public enum CompAlgos { zlib = 0, bzlib = 1, lzo1x = 2, HeaderScripting = 3 }
	}
	public class ContentEncryptionSection : Section {
		#region Fields & Properties
		private EncAlgos? contentEncAlgo;
		private SigAlgos? contentSigAlgo;
		private SigHashAlgos? contentSigHashAlgo;
		private byte[] contentEncKeyId, contentSignature, contentSigKeyId;

		public EncAlgos ContentEncAlgo { get { return contentEncAlgo.HasValue ? contentEncAlgo.Value : EncAlgos.SignedOnly; } }
		public SigAlgos ContentSigAlgo { get { return contentSigAlgo.HasValue ? contentSigAlgo.Value : SigAlgos.EncryptionOnly; } }
		public SigHashAlgos ContentSigHashAlgo { get { return contentSigHashAlgo.HasValue ? contentSigHashAlgo.Value : SigHashAlgos.EncryptionOnly; } }
		public byte[] ContentEncKeyId { get { return (byte[])contentEncKeyId.Clone(); } }
		public byte[] ContentSignature { get { return (byte[])contentSignature.Clone(); } }
		public byte[] ContentSigKeyId { get { return (byte[])contentSigKeyId.Clone(); } }
		#endregion

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ContentEncAlgo:
					contentEncAlgo = (EncAlgos)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentSigAlgo:
					contentSigAlgo = (SigAlgos)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentSigHashAlgo:
					contentSigHashAlgo = (SigHashAlgos)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentEncKeyID:
					contentEncKeyId = (byte[])reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentSignature:
					contentSignature = (byte[])reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ContentSigKeyID:
					contentSigKeyId = (byte[])reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public enum EncAlgos { SignedOnly = 0, DES = 1, TrippleDES = 2, TwoFish = 3, BlowFish = 4, AES = 5 }
		public enum SigAlgos { EncryptionOnly = 0, RSA = 1 }
		public enum SigHashAlgos { EncryptionOnly = 0, SHA1_160 = 1, MD5 = 2 }
	}
	public class AttachmentsSection : Section {
		public EbmlList<AttachedFileSection> Items { get; private set; }

		public AttachmentsSection() { Items = new EbmlList<AttachedFileSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.AttachedFile) {
				Section.CreateReadAdd(new AttachedFileSection(), reader, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

	}
	public class AttachedFileSection : Section {
		public string FileDescription { get; private set; }
		public string FileName { get; private set; }
		public string FileMimeType { get; private set; }
		public ulong? FileUId { get; private set; }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.FileDescription:
					FileDescription = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FileName:
					FileName = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FileMimeType:
					FileMimeType = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FileUID:
					FileUId = (ulong)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}
	public class ChaptersSection : Section {
		public EbmlList<EditionEntrySection> Items { get; private set; }

		public ChaptersSection() { Items = new EbmlList<EditionEntrySection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.EditionEntry) {
				Section.CreateReadAdd(new EditionEntrySection(), reader, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

	}
	public class EditionEntrySection : Section {
		#region Fields & Properties
		private bool? hidden, ordered, def;

		public ulong? EditionUId { get; private set; }
		public Options EditionFlags {
			get {
				return (hidden.HasValue && hidden.Value ? Options.Hidden : Options.None) |
					   (ordered.HasValue && ordered.Value ? Options.Ordered : Options.None) |
					   (def.HasValue && def.Value ? Options.Default : Options.None);
			}
		}
		public EbmlList<ChapterAtomSection> ChapterAtom { get; private set; }
		#endregion

		public EditionEntrySection() { ChapterAtom = new EbmlList<ChapterAtomSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterAtom:
					Section.CreateReadAdd(new ChapterAtomSection(), reader, ChapterAtom); break;
				case DocTypeMatroskaV2.eId.EditionUID:
					EditionUId = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.EditionFlagHidden:
					hidden = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.EditionFlagDefault:
					def = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.EditionFlagOrdered:
					ordered = (ulong)reader.RetrieveValue() == 1; break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		[Flags]
		public enum Options { None = 0, Hidden = 1, Default = 2, Ordered = 4 }

	}
	public class ChapterAtomSection : Section {
		#region Fields & Properties
		private byte[] chapterSegmentUId;
		private bool? enabled, hidden;

		public ulong? ChapterUId { get; private set; }
		public ulong? ChapterTimeStart { get; private set; } //Def: 0?
		public ulong? ChapterTimeEnd { get; private set; }
		public Options ChapterFlags { get { return (hidden.HasValue && hidden.Value ? Options.Hidden : Options.None) | (!enabled.HasValue || enabled.Value ? Options.Enabled : Options.None); } }
		public byte[] ChapterSegmentUId { get { return chapterSegmentUId == null ? null : (byte[])chapterSegmentUId.Clone(); } }
		public ulong? ChapterSegmentEditionUId { get; private set; }
		public ChapterTrackSection ChapterTrack { get; private set; }
		public EbmlList<ChapterAtomSection> ChapterAtom { get; private set; }
		public EbmlList<ChapterDisplaySection> ChapterDisplay { get; private set; }
		public EbmlList<ChapterProcessSection> ChapterProcess { get; private set; }
		#endregion

		public ChapterAtomSection() {
			ChapterAtom = new EbmlList<ChapterAtomSection>();
			ChapterDisplay = new EbmlList<ChapterDisplaySection>();
			ChapterProcess = new EbmlList<ChapterProcessSection>();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterTrack:
					ChapterTrack = Section.CreateRead(new ChapterTrackSection(), reader); break;
				case DocTypeMatroskaV2.eId.ChapterAtom:
					Section.CreateReadAdd(new ChapterAtomSection(), reader, ChapterAtom); break;
				case DocTypeMatroskaV2.eId.ChapterDisplay:
					Section.CreateReadAdd(new ChapterDisplaySection(), reader, ChapterDisplay); break;
				case DocTypeMatroskaV2.eId.ChapProcess:
					Section.CreateReadAdd(new ChapterProcessSection(), reader, ChapterProcess); break;
				case DocTypeMatroskaV2.eId.ChapterUID:
					ChapterUId = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapterTimeStart:
					ChapterTimeStart = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapterTimeEnd:
					ChapterTimeEnd = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapterFlagEnabled:
					enabled = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.ChapterFlagHidden:
					hidden = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.ChapterSegmentUID:
					chapterSegmentUId = (byte[])reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapterSegmentEditionUID:
					ChapterSegmentEditionUId = (ulong)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		[Flags]
		public enum Options { None = 0, Hidden = 1, Enabled = 2 }

	}
	public class ChapterTrackSection : Section {
		public EbmlList<ulong> ChapterTrackNumber { get; private set; }

		public ChapterTrackSection() { ChapterTrackNumber = new EbmlList<ulong>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterTrackNumber:
					ChapterTrackNumber.Add((ulong)reader.RetrieveValue()); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}
	public class ChapterDisplaySection : Section {
		public string ChapterString { get; private set; }
		public EbmlList<string> ChapterLanguage { get; private set; } //Def: eng
		public EbmlList<string> ChapterCountry { get; private set; }

		public ChapterDisplaySection() {
			ChapterLanguage = new EbmlList<string>();
			ChapterCountry = new EbmlList<string>();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapString:
					ChapterString = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapLanguage:
					ChapterLanguage.Add((string)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.ChapCountry:
					ChapterCountry.Add((string)reader.RetrieveValue()); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}
	public class ChapterProcessSection : Section {
		private ulong? chapterProcessCodecId;

		public ulong ChapterProcessCodecId { get { return chapterProcessCodecId.HasValue ? chapterProcessCodecId.Value : 0; } }
		public EbmlList<ChapterProcessCommandSection> ChapterProcessCommand { get; private set; }
		//ChapProcessPrivate

		public ChapterProcessSection() { ChapterProcessCommand = new EbmlList<ChapterProcessCommandSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapProcessCommand:
					Section.CreateReadAdd(new ChapterProcessCommandSection(), reader, ChapterProcessCommand); break;
				case DocTypeMatroskaV2.eId.ChapProcessCodecID:
					chapterProcessCodecId = (ulong)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public class ChapterProcessCommandSection : Section {
			public ProcessTime? ChapterProcessTime { get; private set; }
			//ChapProcessData

			protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
				switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
					case DocTypeMatroskaV2.eId.ChapProcessTime:
						ChapterProcessTime = (ProcessTime)reader.RetrieveValue(); break;
					default: return false;
				}
				return true;
			}
			protected override void Validate() { }

			public enum ProcessTime { During, Before, After }
		}
	}
	public class TagsSection : Section {
		public EbmlList<TagSection> Items { get; private set; }

		public TagsSection() { Items = new EbmlList<TagSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.Tags) {
				Section.CreateReadAdd(new TagSection(), reader, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }


	}
	public class TagSection : Section {
		public TargetsSection Targets { get; private set; }
		public EbmlList<SimpleTagSection> SimpleTag { get; private set; }

		public TagSection() { SimpleTag = new EbmlList<SimpleTagSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.Targets:
					Targets = Section.CreateRead(new TargetsSection(), reader); break;
				case DocTypeMatroskaV2.eId.SimpleTag:
					Section.CreateReadAdd(new SimpleTagSection(), reader, SimpleTag); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

	}
	public class TargetsSection : Section {
		private ulong? targetTypeValue;
		private EbmlList<ulong> trackUId, editionUId, chapterUId, attachmentUId;

		public ulong TargetTypeValue { get { return targetTypeValue.HasValue ? targetTypeValue.Value : 50; } } //Def: 50
		public string TargetType { get; private set; }
		public EbmlList<ulong> TrackUId { get { return trackUId.Count != 0 ? trackUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> EditionUId { get { return editionUId.Count != 0 ? editionUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> ChapterUId { get { return chapterUId.Count != 0 ? chapterUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> AttachmentUId { get { return attachmentUId.Count != 0 ? attachmentUId : new EbmlList<ulong>(new ulong[] { 0 }); } }

		public TargetsSection() {
			trackUId = new EbmlList<ulong>();
			editionUId = new EbmlList<ulong>();
			chapterUId = new EbmlList<ulong>();
			attachmentUId = new EbmlList<ulong>();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.TargetTypeValue:
					targetTypeValue = (ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TargetType:
					TargetType = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TargetTrackUID:
					trackUId.Add((ulong)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.TargetEditionUID:
					editionUId.Add((ulong)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.TargetChapterUID:
					chapterUId.Add((ulong)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.TargetAttachmentUID:
					attachmentUId.Add((ulong)reader.RetrieveValue()); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}
	public class SimpleTagSection : Section {
		private string tagLanguage;
		private bool? tagOriginal;

		public EbmlList<SimpleTagSection> SimpleTag { get; private set; }


		public string TagName { get; private set; }
		public string TagLanguage { get { return tagLanguage != null ? tagLanguage : "und"; } } //Def: und
		public string TagString { get; private set; }
		public bool TagOriginal { get { return tagOriginal.HasValue ? tagOriginal.Value : true; } } //Def: True
		//TagBinary

		public SimpleTagSection() { SimpleTag = new EbmlList<SimpleTagSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.TagName:
					TagName = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TagLanguage:
					tagLanguage = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TagString:
					TagString = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.TagDefault:
					tagOriginal = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.SimpleTag:
					Section.CreateReadAdd(new SimpleTagSection(), reader, SimpleTag); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
	}

	public class SeekHeadSection {
		public class SeekSection {
			public ulong Id { get; private set; }
			public ulong Position { get; private set; }
		}
	}

	public abstract class Section {
		internal void Read(EBMLReader reader) {
			reader.EnterMasterElement();

			ElementInfo elementInfo;
			while((elementInfo = reader.NextElementInfo()) != null) {
				if(!ProcessElement(reader, elementInfo) && !IsGlobalElement(elementInfo)) {
					//if(elementInfo.ElementType.Type != ElementType.eType.Master) reader.RetrieveValue();
					Debug.Print("Unprocessed Item: " + reader.ParentElements.Aggregate<ElementInfo, string>("File", (acc, item) => { return acc + "." + item.ElementType.Name; }) + "." + elementInfo.ElementType.Name);
				}
			}

			Validate();
			reader.LeaveMasterElement();
		}

		internal static bool IsGlobalElement(ElementInfo elementInfo) {
			DocTypeEBML.eId id = (DocTypeEBML.eId)elementInfo.ElementType.Id;
			return id == DocTypeEBML.eId.CRC32 || id == DocTypeEBML.eId.Void;
		}
		internal static void CreateReadAdd<T>(T section, EBMLReader reader, EbmlList<T> lst) where T : Section {
			section.Read(reader);
			lst.Add(section);
		}
		internal static T CreateRead<T>(T section, EBMLReader reader) where T : Section {
			section.Read(reader);
			return section;
		}

		protected abstract bool ProcessElement(EBMLReader reader, ElementInfo elementInfo);
		protected abstract void Validate();
	}
	public class EbmlList<T> : IList<T> {
		protected List<T> items;

		public EbmlList() : this(new T[0]) { }
		public EbmlList(T[] items) {
			this.items = new List<T>();
			foreach(var item in items) this.items.Add(item);
		}

		protected internal void Add(T item) { items.Add(item); }
		protected internal EbmlList<T> DeepClone(Func<T, T> cloneFunc) {
			EbmlList<T> clone = new EbmlList<T>();
			foreach(var item in items) clone.Add(cloneFunc(item));
			return clone;
		}

		public int Count { get { return items.Count; } }
		public IEnumerator<T> GetEnumerator() { return items.GetEnumerator(); }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return items.GetEnumerator(); }
		public bool Contains(T item) { return items.Contains(item); }
		public void CopyTo(T[] array, int arrayIndex) { items.CopyTo(array, arrayIndex); }
		public bool IsReadOnly { get { return true; } }
		public int IndexOf(T item) { return items.IndexOf(item); }
		public T this[int index] { get { return items[index]; } set { throw new NotSupportedException(); } }
		public void Insert(int index, T item) { throw new NotSupportedException(); }
		public bool Remove(T item) { throw new NotSupportedException(); }
		void ICollection<T>.Add(T item) { throw new NotSupportedException(); }
		public void Clear() { throw new NotSupportedException(); }
		public void RemoveAt(int index) { throw new NotSupportedException(); }


	}
}

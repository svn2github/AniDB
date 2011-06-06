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
using System.Runtime.InteropServices;
using System.IO;
using AVDump2Lib.BlockBuffer;
using System.Collections;

namespace AVDump2Lib.BlockConsumers {
	public class MatroskaParser : BlockConsumerBase {
		public MatroskaFile MatroskaFileObj { get; private set; }

		private FileSource dataSrc;
		private EBMLReader reader;
		private Thread progressUpdater;
		private Thread progressingThread;
		private IDocType matroskaDocType = new DocTypeEBML(new DocTypeMatroskaV2());

		public MatroskaParser(string name) : base(name) { }

		protected override void DoWork() {
			progressUpdater.Start();
			progressingThread = Thread.CurrentThread;
			Root();
		}

		private void Root() {
			var matroskaFile = new MatroskaFile(dataSrc.Length);
			matroskaFile.Parse(reader);
			MatroskaFileObj = matroskaFile;
		}

		private void ReadBytesUpdater() {
			int timerRes = 40;
			int ttl = 10000;
			int ticks = ttl / timerRes;

			while(!HasFinished) {
				Thread.Sleep(timerRes);
				ticks--;

				if(ProcessedBytes == dataSrc.Position) {
					if(ticks < 0) {
						progressingThread.Abort();
						break;
					}
				} else {
					ProcessedBytes = dataSrc.Position;
					ticks = ttl / timerRes;
				}
			}
			ProcessedBytes = dataSrc.Length;
		}

		public static bool IsMatroskaFile(Stream fileStream) {
			var blockSource = new ByteStreamToBlock(1024 * 1024); blockSource.Initialize(fileStream);
			var circb = new CircularBuffer<byte[]>(2);
			var buffer = new RefillBuffer<byte[]>(circb, blockSource, 1);

			buffer.Start();


			var src = new FileSource(buffer, 0);
			var docType = new DocTypeEBML(new DocTypeMatroskaV2());
			var reader = new EBMLReader(src, docType);


			bool isMatroskaFile;
			try {
				ElementInfo elementInfo = reader.NextElementInfo();
				if(elementInfo.ElementType.Id == (int)DocTypeEBML.eId.EBMLHeader) {
					Section.CreateRead(new EbmlHeaderSection(), reader, elementInfo.Length);
					isMatroskaFile = true;
				} else {
					isMatroskaFile = false;
				}
			} catch(Exception) { isMatroskaFile = false; }

			buffer.Stop();

			fileStream.Position = 0;
			return isMatroskaFile;
		}

		public override string ToString() { return ""; }

		protected override void InitInternal() {
			MatroskaFileObj = null;

			dataSrc = new FileSource(b, consumerId);
			reader = new EBMLReader(dataSrc, matroskaDocType);

			progressUpdater = new Thread(ReadBytesUpdater);
			progressUpdater.Name = "MKVParser ProgressUpdater";
		}
	}

	public class MatroskaFile : Section {
		public EbmlHeaderSection EbmlHeader { get; private set; }
		public SegmentSection Segment { get; private set; }

		public MatroskaFile(long fileSize) { SectionSize = fileSize; }

		internal void Parse(EBMLReader reader) {
			ElementInfo elementInfo = reader.NextElementInfo();
			if(elementInfo.ElementType.Id == (int)DocTypeEBML.eId.EBMLHeader) {
				EbmlHeader = Section.CreateRead(new EbmlHeaderSection(), reader, elementInfo.Length.Value);
			} else {
				//Todo: dispose reader / add warning
				return;
			}

			while((elementInfo = reader.NextElementInfo()) != null && Section.IsGlobalElement(elementInfo)) ;

			if(elementInfo != null && elementInfo.ElementType.Id == (int)DocTypeMatroskaV2.eId.Segment) {
				Segment = Section.CreateRead(new SegmentSection(), reader, elementInfo.Length);
			} else {
				//Todo: dispose reader / add warning
				return;
			}

			Validate();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) { throw new NotSupportedException(); }

		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("EbmlHeader", EbmlHeader);
			yield return CreatePair("Segment", Segment);
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("EbmlVersion", EbmlVersion);
			yield return CreatePair("EbmlReadVersion", EbmlReadVersion);
			yield return CreatePair("EbmlMaxIdLength", EbmlMaxIdLength);
			yield return CreatePair("EbmlMaxSizeLength", EbmlMaxSizeLength);
			yield return CreatePair("DocTypeReadVersion", DocTypeReadVersion);
			yield return CreatePair("DocTypeVersion", DocTypeVersion);
			yield return CreatePair("DocType", DocType);
		}
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
					Cluster.Read(reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.Tracks:
					if(Tracks != null) break;
					Tracks = Section.CreateRead(new TracksSection(), reader, elementInfo.Length);
					Cluster.AddTracks(Tracks.Items); break; //Must be set and != 0 (add warning)
				case DocTypeMatroskaV2.eId.Info:
					SegmentInfo = Section.CreateRead(new SegmentInfoSection(), reader, elementInfo.Length);
					Cluster.timecodeScale = SegmentInfo.TimecodeScale; break;
				case DocTypeMatroskaV2.eId.Chapters:
					Chapters = Section.CreateRead(new ChaptersSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.Attachments:
					Attachments = Section.CreateRead(new AttachmentsSection(), reader, elementInfo.Length); break;
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("SegmentInfo", SegmentInfo);
			yield return CreatePair("Attachments", Attachments);
			yield return CreatePair("Chapters", Chapters);
			yield return CreatePair("Cluster", Cluster);
			yield return CreatePair("Tracks", Tracks);
		}
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
					Section.CreateReadAdd(new ChapterTranslateSection(), reader, elementInfo.Length, ChapterTranslate); break;
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("SegmentUId", SegmentUId);
			yield return CreatePair("PreviousUId", PreviousUId);
			yield return CreatePair("NextUId", NextUId);
			yield return CreatePair("SegmentFilename", SegmentFilename);
			yield return CreatePair("PreviousFilename", PreviousFilename);
			yield return CreatePair("NextFilename", NextFilename);
			yield return CreatePair("Duration", Duration);
			yield return CreatePair("Title", Title);
			yield return CreatePair("MuxingApp", MuxingApp);
			yield return CreatePair("WritingApp", WritingApp);
			yield return CreatePair("ProductionDate", ProductionDate);
		}
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("EditionUId", EditionUId);
			yield return CreatePair("Codec", Codec);
			yield return CreatePair("Id", Id);
		}
	}

	public class ClusterSection : Section {
		public Dictionary<int, Track> Tracks { get; private set; }

		public ClusterSection() { Tracks = new Dictionary<int, Track>(); }
		private long timecode;
		public ulong timecodeScale;

		public void AddTracks(EbmlList<TrackEntrySection> tracks) {
			foreach(var track in tracks) Tracks.Add((int)track.TrackNumber, new Track { trackNumber = (int)track.TrackNumber.Value, timecodeScale = track.TrackTimecodeScale });
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.SimpleBlock:
				case DocTypeMatroskaV2.eId.Block:
					MatroskaBlock matroskaBlock = (MatroskaBlock)reader.RetrieveValue();
					Track track;
					if(Tracks.TryGetValue(matroskaBlock.TrackNumber, out track)) {
						track.timecodes.Add(new TrackTimecode((ulong)((matroskaBlock.TimeCode + timecode) * track.timecodeScale * timecodeScale), matroskaBlock.FrameCount, matroskaBlock.DataLength));
					} else {
						throw new Exception("Invalid track index (" + matroskaBlock.TrackNumber + ") in matroska block");
					}
					break;

				case DocTypeMatroskaV2.eId.BlockDuration: break;
				case DocTypeMatroskaV2.eId.ReferenceBlock: break;
				case DocTypeMatroskaV2.eId.BlockGroup:
					Read(reader, elementInfo.Length, false); break;
				case DocTypeMatroskaV2.eId.Timecode:
					timecode = (long)(ulong)reader.RetrieveValue();
					break;

				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() { yield break; }


		public class Track {
			public int trackNumber;
			public double timecodeScale;

			public List<TrackTimecode> timecodes;

			public Track() {
				timecodes = new List<TrackTimecode>();
			}

			private TrackInfo trackInfo;
			public TrackInfo TrackInfo { get { return (trackInfo == null) ? trackInfo = CalcTrackInfo() : trackInfo; } }

			private TrackInfo CalcTrackInfo() {
				if(timecodes.Count == 0) return new ClusterSection.TrackInfo(null, null, null, null, null, null, null, new TimeSpan(0), 0, 0);

				timecodes.Sort();

				var trackLength = TimeSpan.FromMilliseconds((timecodes.Last().timeCode - timecodes.First().timeCode) / 1000000);

				double[] fps = new double[3];
				double? minfps = null, maxfps = null;

				var oldTC = timecodes.First();

				int pos = 0, prevPos = 0, prevprevPos;
				double maxDiff;

				int frames = oldTC.frames;
				long trackSize = oldTC.size;

				var fpsTable = new Dictionary<double, int>();

				foreach(var timecode in timecodes.Skip(1)) {
					//fps[pos] = 1d / ((timecode.timeCode - oldTC.timeCode) / (double)oldTC.frames / 1000000000d);
					fps[pos] = (double)(1000000000d * oldTC.frames) / (double)(timecode.timeCode - oldTC.timeCode);

					if(!double.IsInfinity(fps[pos]) && !double.IsNaN(fps[pos])) {
						if(!fpsTable.ContainsKey(fps[pos])) fpsTable[fps[pos]] = 0;
						fpsTable[fps[pos]]++;
					}

					oldTC = timecode;
					prevprevPos = prevPos;
					prevPos = pos;
					pos = (pos + 1) % 3;

					trackSize += timecode.size;
					frames += timecode.frames;

					maxDiff = (fps[prevprevPos] + fps[pos] / 2) * 0.1;
					if(Math.Abs(fps[prevPos] - fps[prevprevPos]) < maxDiff && Math.Abs(fps[prevPos] - fps[pos]) < maxDiff) {
						if(!minfps.HasValue || minfps.Value > fps[prevPos]) minfps = fps[prevPos];
						if(!maxfps.HasValue || maxfps.Value < fps[prevPos]) maxfps = fps[prevPos];
					}
				}

				return new TrackInfo(
					fpsTable: fpsTable,

					minBitrate: 0,
					maxBitrate: 0,
					averageBitrate: (trackSize != 0 && trackLength.Ticks != 0) ? trackSize * 8 / trackLength.TotalSeconds : (double?)null,
					minLaceRate: minfps,
					maxLaceRate: maxfps,
					averageLaceRate: (frames != 0 && trackLength.Ticks != 0) ? frames / trackLength.TotalSeconds : (double?)null,
					trackLength: trackLength,
					trackSize: trackSize,
					laceCount: (int)frames
				);
			}
		}

		public class TrackInfo {
			public double? MinBitrate { get; private set; }
			public double? MaxBitrate { get; private set; }
			public double? AverageBitrate { get; private set; }

			public double? MinLaceRate { get; private set; }
			public double? MaxLaceRate { get; private set; }
			public double? AverageLaceRate { get; private set; }

			Dictionary<double, int> fpsTable;
			public IEnumerable<KeyValuePair<double, int>> FPSTable { get { return fpsTable; } }

			public TimeSpan TrackLength { get; private set; }
			public long TrackSize { get; private set; }
			public int LaceCount { get; private set; }

			public TrackInfo(Dictionary<double, int> fpsTable, double? minBitrate, double? maxBitrate, double? averageBitrate, double? minLaceRate, double? maxLaceRate, double? averageLaceRate, TimeSpan trackLength, long trackSize, int laceCount) {
				MinBitrate = minBitrate; MaxBitrate = maxBitrate; AverageBitrate = averageBitrate;
				MinLaceRate = minLaceRate; MaxLaceRate = maxLaceRate; AverageLaceRate = averageLaceRate;
				TrackLength = trackLength; TrackSize = trackSize; LaceCount = laceCount;
				this.fpsTable = fpsTable;
			}
		}
		public struct TrackTimecode : IComparable<TrackTimecode> {
			public ulong timeCode;
			public byte frames;
			public uint size;

			public TrackTimecode(ulong timeCode, byte frames, uint size) {
				this.frames = frames; this.size = size; this.timeCode = timeCode;
			}

			public int CompareTo(TrackTimecode other) {
				return timeCode.CompareTo(other.timeCode);
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
				Section.CreateReadAdd(new TrackEntrySection(), reader, elementInfo.Length, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			foreach(var item in Items) yield return CreatePair("Track", item);
		}
	}
	public class TrackEntrySection : Section {
		#region Fields & Properties
		private bool? enabled, def, forced, lacing;
		private string language;
		private ulong? minCache;
		private ulong? maxBlockAdditionID;
		private double? trackTimecodeScale;

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
		public double TrackTimecodeScale { get { return trackTimecodeScale.HasValue ? trackTimecodeScale.Value : 1d; } }
		public string Name { get; private set; }
		public string Language { get { return language != null ? language : "eng"; } } //Default: 'eng'
		public string CodecId { get; private set; } //Mandatory
		public byte[] CodecPrivate { get; private set; }
		public string CodecName { get; private set; }
		public string AttachmentLink { get; private set; }

		public VideoSection Video { get; private set; }
		public AudioSection Audio { get; private set; }
		public ContentEncodingsSection ContentEncodings { get; private set; }
		public TrackOperationSection TrackOperation { get; private set; }
		#endregion


		public BitmapInfoHeader? GetBitMapInfoHeader() {
			if(!CodecId.Equals("V_MS/VFW/FOURCC")) return null;
			byte[] b = new byte[40];
			Buffer.BlockCopy(CodecPrivate, 0, b, 0, 40);

			return BitmapInfoHeader.Create(b);
		}
		[StructLayout(LayoutKind.Sequential)]
		public struct BitmapInfoHeader {
			public uint biSize;
			public int biWidth;
			public int biHeight;
			public ushort biPlanes;
			public ushort biBitCount;
			public uint biCompression;
			public uint biSizeImage;
			public int biXPelsPerMeter;
			public int biYPelsPerMeter;
			public uint biClrUsed;
			public uint biClrImportant;

			public string FourCC {
				get {
					var bytes = new byte[4];
					for(int i = 0;i < 4;i++) bytes[i] = (byte)((biCompression >> (i << 3)) & 0xFF);
					//bytes = BitConverter.GetBytes(biCompression);


					string fourCC = "";
					foreach(var b in bytes) fourCC += (char)b;
					return fourCC;
				}
			}

			public static BitmapInfoHeader Create(byte[] b) {
				GCHandle hDataIn = GCHandle.Alloc(b, GCHandleType.Pinned);
				BitmapInfoHeader bitmapInfoHeader = (BitmapInfoHeader)Marshal.PtrToStructure(hDataIn.AddrOfPinnedObject(), typeof(BitmapInfoHeader));
				hDataIn.Free();
				return bitmapInfoHeader;
			}
		}

		public VMpeg4IsoAvcHeader GetVMpeg4IsoAvcHeader() {
			if(!CodecId.Equals("V_MPEG4/ISO/AVC")) return null;
			return VMpeg4IsoAvcHeader.Create(CodecPrivate);
		}
		public class VMpeg4IsoAvcHeader {
			public byte ReservedA { get; private set; }
			public byte Profile { get; private set; }
			public byte ReservedB { get; private set; }
			public byte Level { get; private set; }
			public byte ReservedC { get; private set; }
			public byte NALULengthSizeMinus1 { get; private set; }
			public byte ReservedD { get; private set; }
			public IEnumerable<string> SequenceParameterSets { get; private set; }
			public IEnumerable<string> PictureParameterSets { get; private set; }

			public static VMpeg4IsoAvcHeader Create(byte[] b) {
				var header = new VMpeg4IsoAvcHeader();

				header.ReservedA = b[0];
				header.Profile = b[1];
				header.ReservedB = b[2];
				header.Level = b[3];
				header.ReservedC = (byte)((b[4] & 0xFC) >> 2);
				header.NALULengthSizeMinus1 = (byte)(b[4] & 0x3);
				header.ReservedB = (byte)((b[5] & 0xE0) >> 5);

				int pos = 6;
				byte count = (byte)(b[5] & 0x1F);
				header.SequenceParameterSets = GetSet(b, count, ref pos);
				count = b[pos++];
				header.PictureParameterSets = GetSet(b, count, ref pos);

				return header;
			}
			private static IEnumerable<string> GetSet(byte[] b, byte count, ref int pos) {
				Int16 size;

				var sets = new Collection<string>();
				for(int i = 0;i < count;i++) {
					size = (Int16)((b[pos] << 8) + b[pos + 1]); pos += 2;

					sets.Add(System.Text.Encoding.ASCII.GetString(b, pos, size));
					pos += size;
				}

				return sets;
			}
		}

		public WaveFormatEx? GetWaveFormatEx() {
			if(!CodecId.Equals("A_MS/ACM")) return null;
			byte[] b = new byte[18];
			Buffer.BlockCopy(CodecPrivate, 0, b, 0, 18);

			return WaveFormatEx.Create(b);
		}
		public struct WaveFormatEx {
			public ushort wFormatTag;
			public ushort nChannels;
			public uint nSamplesPerSec;
			public uint nAvgBytesPerSec;
			public ushort nBlockAlign;
			public ushort wBitsPerSample;
			public ushort cbSize;

			public string TwoCC { get { return Convert.ToString(wFormatTag, 16); } }

			public static WaveFormatEx Create(byte[] b) {
				GCHandle hDataIn = GCHandle.Alloc(b, GCHandleType.Pinned);
				WaveFormatEx waveFormatEx = (WaveFormatEx)Marshal.PtrToStructure(hDataIn.AddrOfPinnedObject(), typeof(WaveFormatEx));
				hDataIn.Free();
				return waveFormatEx;
			}
		}


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
					trackTimecodeScale = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.Name:
					Name = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.Language:
					language = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.CodecID:
					CodecId = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.CodecName:
					CodecName = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.CodecPrivate:
					CodecPrivate = (byte[])reader.RetrieveValue(); break;
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
					Video = Section.CreateRead(new VideoSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.Audio:
					Audio = Section.CreateRead(new AudioSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.TrackOperation:
					TrackOperation = Section.CreateRead(new TrackOperationSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.ContentEncodings:
					ContentEncodings = Section.CreateRead(new ContentEncodingsSection(), reader, elementInfo.Length); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("TrackNumber", TrackNumber);
			yield return CreatePair("TrackUId", TrackUId);
			yield return CreatePair("TrackOverlay", TrackOverlay);
			yield return CreatePair("TrackType", TrackType);
			yield return CreatePair("TrackFlags", TrackFlags);
			yield return CreatePair("MinCache", MinCache);
			yield return CreatePair("MaxCache", MaxCache);
			yield return CreatePair("MaxBlockAdditionID", MaxBlockAdditionID);
			yield return CreatePair("DefaultDuration", DefaultDuration);
			yield return CreatePair("TrackTimecodeScale", TrackTimecodeScale);
			yield return CreatePair("Name", Name);
			yield return CreatePair("Language", Language);
			yield return CreatePair("CodecId", CodecId);
			yield return CreatePair("CodecPrivate", CodecPrivate);
			yield return CreatePair("CodecName", CodecName);
			yield return CreatePair("AttachmentLink", AttachmentLink);
			yield return CreatePair("Video", Video);
			yield return CreatePair("Audio", Audio);
			yield return CreatePair("ContentEncodings", ContentEncodings);
		}


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
		public DisplayMode? stereoMode;
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
		public DisplayMode StereoMode { get { return stereoMode.HasValue ? stereoMode.Value : 0; } } //Default: 0
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
					displayUnit = (Unit)(ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FrameRate:
					FrameRate = (double)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.FlagInterlaced:
					interlaced = (ulong)reader.RetrieveValue() == 1; break;
				case DocTypeMatroskaV2.eId.StereoMode:
					stereoMode = (DisplayMode)(ulong)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ColourSpace:
					colorSpace = (byte[])reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("FrameRate", FrameRate);
			yield return CreatePair("ColorSpace", ColorSpace);
			yield return CreatePair("PixelWidth", PixelWidth);
			yield return CreatePair("PixelHeight", PixelHeight);
			yield return CreatePair("AspectRatioType", AspectRatioType);
			yield return CreatePair("Interlaced", Interlaced);
			yield return CreatePair("PixelCropBottom", PixelCropBottom);
			yield return CreatePair("PixelCropTop", PixelCropTop);
			yield return CreatePair("PixelCropLeft", PixelCropLeft);
			yield return CreatePair("PixelCropRight", PixelCropRight);
			yield return CreatePair("DisplayWidth", DisplayWidth);
			yield return CreatePair("DisplayHeight", DisplayHeight);
			yield return CreatePair("DisplayUnit", DisplayUnit);
		}


		public enum Unit { Pixels, Centimeters, Inches }
		public enum ARType { FreeResizing, KeepAR, Fixed }
		public enum DisplayMode { Mono, SxS_LR, TB_RL, TB_LR, CheckBoard_RL, CheckBoard_LR, RowInterleaved_RL, RowInterleaved_LR, ColumnInterleaved_RL, ColumnInterleaved_LR, Anaglyph_CyanRed, SxS_RL, Anaglyph_GreenMagenta, LacedInOneBlock_LR, LacedInOneBlock_RL }
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
		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("SamplingFrequency", SamplingFrequency);
			yield return CreatePair("OutputSamplingFrequency", OutputSamplingFrequency);
			yield return CreatePair("ChannelCount", ChannelCount);
			yield return CreatePair("BitDepth", BitDepth);
		}
	}
	public class TrackOperationSection : Section {
		public bool Is3d { get; private set; } //TODO

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.TrackCombinePlanes: Is3d = true; break;
				default: if(elementInfo.ElementType.Type == ElementType.eType.Master) Read(reader, elementInfo.Length, false); break;
			}
			return false;
		}

		protected override void Validate() {
		}

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield break;
		}
	}

	public class ContentEncodingsSection : Section {
		public EbmlList<ContentEncodingSection> Encodings { get; private set; }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.ContentEncodings) {
				Section.CreateReadAdd(new ContentEncodingSection(), reader, elementInfo.Length, Encodings);
				return true;
			}
			return false;
		}
		protected override void Validate() { }
		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			foreach(var encoding in Encodings) yield return CreatePair("ContentEncoding", encoding);
		}
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
					ContentCompression = Section.CreateRead(new ContentCompressionSection(), reader, elementInfo.Length); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("ContentEncodingOrder", ContentEncodingOrder);
			yield return CreatePair("ContentEncodingScope", ContentEncodingScope);
			yield return CreatePair("ContentEncodingType", ContentEncodingType);
			yield return CreatePair("ContentCompression", ContentCompression);
		}


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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("ContentCompAlgo", ContentCompAlgo);
		}

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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("ContentEncAlgo", ContentEncAlgo);
			yield return CreatePair("ContentSigAlgo", ContentSigAlgo);
			yield return CreatePair("ContentSigHashAlgo", ContentSigHashAlgo);
			yield return CreatePair("ContentEncKeyId", ContentEncKeyId);
			yield return CreatePair("ContentSignature", ContentSignature);
			yield return CreatePair("ContentSigKeyId", ContentSigKeyId);
		}


		public enum EncAlgos { SignedOnly = 0, DES = 1, TrippleDES = 2, TwoFish = 3, BlowFish = 4, AES = 5 }
		public enum SigAlgos { EncryptionOnly = 0, RSA = 1 }
		public enum SigHashAlgos { EncryptionOnly = 0, SHA1_160 = 1, MD5 = 2 }
	}
	public class AttachmentsSection : Section {
		public EbmlList<AttachedFileSection> Items { get; private set; }

		public AttachmentsSection() { Items = new EbmlList<AttachedFileSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.AttachedFile) {
				Section.CreateReadAdd(new AttachedFileSection(), reader, elementInfo.Length, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			foreach(var attachedFile in Items) yield return CreatePair("AttachedFile", attachedFile);
		}
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("FileDescription", FileDescription);
			yield return CreatePair("FileName", FileName);
			yield return CreatePair("FileMimeType", FileMimeType);
			yield return CreatePair("FileUId", FileUId);
		}
	}
	public class ChaptersSection : Section {
		public EbmlList<EditionEntrySection> Items { get; private set; }

		public ChaptersSection() { Items = new EbmlList<EditionEntrySection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.EditionEntry) {
				Section.CreateReadAdd(new EditionEntrySection(), reader, elementInfo.Length, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() { foreach(var item in Items) yield return CreatePair("EditionEntry", item); }
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
		public EbmlList<ChapterAtomSection> ChapterAtoms { get; private set; }
		#endregion

		public EditionEntrySection() { ChapterAtoms = new EbmlList<ChapterAtomSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterAtom:
					Section.CreateReadAdd(new ChapterAtomSection(), reader, elementInfo.Length, ChapterAtoms); break;
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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("EditionUId", EditionUId);
			yield return CreatePair("EditionFlags", EditionFlags);
			foreach(var chapterAtom in ChapterAtoms) yield return CreatePair("ChapterAtom", chapterAtom);
		}
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
		public EbmlList<ChapterAtomSection> ChapterAtoms { get; private set; }
		public EbmlList<ChapterDisplaySection> ChapterDisplays { get; private set; }
		public EbmlList<ChapterProcessSection> ChapterProcesses { get; private set; }
		#endregion

		public ChapterAtomSection() {
			ChapterAtoms = new EbmlList<ChapterAtomSection>();
			ChapterDisplays = new EbmlList<ChapterDisplaySection>();
			ChapterProcesses = new EbmlList<ChapterProcessSection>();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterTrack:
					ChapterTrack = Section.CreateRead(new ChapterTrackSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.ChapterAtom:
					Section.CreateReadAdd(new ChapterAtomSection(), reader, elementInfo.Length, ChapterAtoms); break;
				case DocTypeMatroskaV2.eId.ChapterDisplay:
					Section.CreateReadAdd(new ChapterDisplaySection(), reader, elementInfo.Length, ChapterDisplays); break;
				case DocTypeMatroskaV2.eId.ChapProcess:
					Section.CreateReadAdd(new ChapterProcessSection(), reader, elementInfo.Length, ChapterProcesses); break;
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


		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return new KeyValuePair<string, object>("ChapterTrack", ChapterTrack);
			foreach(var chapterAtoms in ChapterAtoms) yield return CreatePair("ChapterAtom", chapterAtoms);
			foreach(var chapterDisplay in ChapterDisplays) yield return CreatePair("ChapterDisplay", chapterDisplay);
			foreach(var chapterProcess in ChapterProcesses) yield return CreatePair("ChapterProcess", chapterProcess);
			yield return CreatePair("ChapterUId", ChapterUId);
			yield return CreatePair("ChapterTimeStart", ChapterTimeStart);
			yield return CreatePair("ChapterTimeEnd", ChapterTimeEnd);
			yield return CreatePair("ChapterFlags", ChapterFlags);
			yield return CreatePair("ChapterSegmentUId", ChapterSegmentUId);
			yield return CreatePair("ChapterSegmentEditionUId", ChapterSegmentEditionUId);
		}
	}
	public class ChapterTrackSection : Section {
		public EbmlList<ulong> ChapterTrackNumbers { get; private set; }

		public ChapterTrackSection() { ChapterTrackNumbers = new EbmlList<ulong>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapterTrackNumber:
					ChapterTrackNumbers.Add((ulong)reader.RetrieveValue()); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() { foreach(var chapterTrackNumber in ChapterTrackNumbers) yield return new KeyValuePair<string, object>("ChapterTrackNumber", chapterTrackNumber); }
	}
	public class ChapterDisplaySection : Section {
		public string ChapterString { get; private set; }
		public EbmlList<string> ChapterLanguages { get; private set; } //Def: eng
		public EbmlList<string> ChapterCountries { get; private set; }

		public ChapterDisplaySection() {
			ChapterLanguages = new EbmlList<string>();
			ChapterCountries = new EbmlList<string>();
		}

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapString:
					ChapterString = (string)reader.RetrieveValue(); break;
				case DocTypeMatroskaV2.eId.ChapLanguage:
					ChapterLanguages.Add((string)reader.RetrieveValue()); break;
				case DocTypeMatroskaV2.eId.ChapCountry:
					ChapterCountries.Add((string)reader.RetrieveValue()); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			foreach(var chapterLanguage in ChapterLanguages) yield return CreatePair("ChapterLanguage", chapterLanguage);
			foreach(var chapterCountry in ChapterCountries) yield return CreatePair("ChapterCountry", chapterCountry);
			yield return new KeyValuePair<string, object>("ChapterString", ChapterString);
		}
	}
	public class ChapterProcessSection : Section {
		private ulong? chapterProcessCodecId;

		public ulong ChapterProcessCodecId { get { return chapterProcessCodecId.HasValue ? chapterProcessCodecId.Value : 0; } }
		public EbmlList<ChapterProcessCommandSection> ChapterProcessCommands { get; private set; }
		//ChapProcessPrivate

		public ChapterProcessSection() { ChapterProcessCommands = new EbmlList<ChapterProcessCommandSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.ChapProcessCommand:
					Section.CreateReadAdd(new ChapterProcessCommandSection(), reader, elementInfo.Length, ChapterProcessCommands); break;
				case DocTypeMatroskaV2.eId.ChapProcessCodecID:
					chapterProcessCodecId = (ulong)reader.RetrieveValue(); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("ChapterProcessCodecId", ChapterProcessCodecId);
			foreach(var chapterProcessCommand in ChapterProcessCommands) yield return CreatePair("ChapterProcessCommand", chapterProcessCommand);
		}


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

			public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
				yield return CreatePair("ChapterProcessTime", ChapterProcessTime);
			}

			public enum ProcessTime { During, Before, After }
		}
	}
	public class TagsSection : Section {
		public EbmlList<TagSection> Items { get; private set; }

		public TagsSection() { Items = new EbmlList<TagSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			if((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id == DocTypeMatroskaV2.eId.Tags) {
				Section.CreateReadAdd(new TagSection(), reader, elementInfo.Length, Items);
				return true;
			}
			return false;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			foreach(var tag in Items) yield return CreatePair("Tag", tag);
		}


	}
	public class TagSection : Section {
		public TargetsSection Targets { get; private set; }
		public EbmlList<SimpleTagSection> SimpleTags { get; private set; }

		public TagSection() { SimpleTags = new EbmlList<SimpleTagSection>(); }

		protected override bool ProcessElement(EBMLReader reader, ElementInfo elementInfo) {
			switch((DocTypeMatroskaV2.eId)elementInfo.ElementType.Id) {
				case DocTypeMatroskaV2.eId.Targets:
					Targets = Section.CreateRead(new TargetsSection(), reader, elementInfo.Length); break;
				case DocTypeMatroskaV2.eId.SimpleTag:
					Section.CreateReadAdd(new SimpleTagSection(), reader, elementInfo.Length, SimpleTags); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }
		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return new KeyValuePair<string, object>("Targets", Targets);
			foreach(var simpleTag in SimpleTags) yield return CreatePair("SimpleTag", simpleTag);
		}

	}
	public class TargetsSection : Section {
		private ulong? targetTypeValue;
		private EbmlList<ulong> trackUId, editionUId, chapterUId, attachmentUId;

		public ulong TargetTypeValue { get { return targetTypeValue.HasValue ? targetTypeValue.Value : 50; } } //Def: 50
		public string TargetType { get; private set; }
		public EbmlList<ulong> TrackUIds { get { return trackUId.Count != 0 ? trackUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> EditionUIds { get { return editionUId.Count != 0 ? editionUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> ChapterUIds { get { return chapterUId.Count != 0 ? chapterUId : new EbmlList<ulong>(new ulong[] { 0 }); } }
		public EbmlList<ulong> AttachmentUIds { get { return attachmentUId.Count != 0 ? attachmentUId : new EbmlList<ulong>(new ulong[] { 0 }); } }

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

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("TargetTypeValue", TargetTypeValue);
			yield return CreatePair("TargetType", TargetType);
			foreach(var trackUId in TrackUIds) yield return CreatePair("TrackUId", trackUId);
			foreach(var editionUId in EditionUIds) yield return CreatePair("EditionUId", editionUId);
			foreach(var chapterUId in ChapterUIds) yield return CreatePair("ChapterUId", chapterUId);
			foreach(var attachmentUId in AttachmentUIds) yield return CreatePair("AttachmentUId", attachmentUId);
		}

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
					Section.CreateReadAdd(new SimpleTagSection(), reader, elementInfo.Length, SimpleTag); break;
				default: return false;
			}
			return true;
		}
		protected override void Validate() { }

		public override IEnumerator<KeyValuePair<string, object>> GetEnumerator() {
			yield return CreatePair("TagName", TagName);
			yield return CreatePair("TagLanguage", TagLanguage);
			yield return CreatePair("TagString", TagString);
			yield return CreatePair("TagOriginal", TagOriginal);
		}

	}
	public class SeekHeadSection {
		public class SeekSection {
			public ulong Id { get; private set; }
			public ulong Position { get; private set; }
		}
	}

	public abstract class Section : IEnumerable<KeyValuePair<string, object>> {
		public long? SectionSize { get; protected set; }

		internal void Read(EBMLReader reader, long? size, bool validate = true) {
			SectionSize = size;

			reader.EnterMasterElement();
			ElementInfo elementInfo;
			try {
				while((elementInfo = reader.NextElementInfo()) != null) {
					try {
						if(!ProcessElement(reader, elementInfo) && !IsGlobalElement(elementInfo)) {
							Debug.Print("Unprocessed Item: " + reader.ParentElements.Aggregate<ElementInfo, string>("File", (acc, item) => { return acc + "." + item.ElementType.Name; }) + "." + elementInfo.ElementType.Name);
						}
					} catch(Exception) { }
				}
				if(validate) Validate();
			} catch(Exception) { }

			reader.LeaveMasterElement();
		}

		internal static bool IsGlobalElement(ElementInfo elementInfo) {
			DocTypeEBML.eId id = (DocTypeEBML.eId)elementInfo.ElementType.Id;
			return id == DocTypeEBML.eId.CRC32 || id == DocTypeEBML.eId.Void;
		}
		internal static void CreateReadAdd<T>(T section, EBMLReader reader, long? size, EbmlList<T> lst) where T : Section {
			section.Read(reader, size);
			lst.Add(section);
		}
		internal static T CreateRead<T>(T section, EBMLReader reader, long? size) where T : Section {
			section.Read(reader, size);
			return section;
		}

		protected abstract bool ProcessElement(EBMLReader reader, ElementInfo elementInfo);
		protected abstract void Validate();

		IEnumerator IEnumerable.GetEnumerator() { return GetEnumerator(); }
		public abstract IEnumerator<KeyValuePair<string, object>> GetEnumerator();

		protected KeyValuePair<string, object> CreatePair(string key, object value) { return new KeyValuePair<string, object>(key, value); }

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
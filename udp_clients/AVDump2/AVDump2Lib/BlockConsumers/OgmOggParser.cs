using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.BlockConsumers.Tools;
using System.Threading;
using System.Diagnostics;
using System.IO;
using AVDump2Lib.InfoGathering.Parser.CSEBMLLib;
using AVDump2Lib.HashAlgorithms;
using CSEBMLLib.Sources;
using System.Collections.ObjectModel;
using System.Runtime.InteropServices;
using AVDump2Lib.BlockBuffer;
using AVDump2Lib.Misc.Conversion;

namespace AVDump2Lib.BlockConsumers {
	public class OgmOggParser : BlockConsumerBase {
		private FileSource dataSrc;
		private Thread progressUpdater;
		private Thread processingThread;

		public OgmOggFile OgmOggFileObj { get; private set; }

		public OgmOggParser(string name) : base(name) { }

		private void ReadBytesUpdater() {
			int timerRes = 40;
			int ttl = 10000;
			int ticks = ttl / timerRes;

			while(!HasFinished) {
				Thread.Sleep(timerRes);
				ticks--;

				if(ProcessedBytes == dataSrc.Position) {
					if(ticks < 0) {		
						processingThread.Abort();
						break;
					}
				} else {
					ProcessedBytes = dataSrc.Position;
					ticks = ttl / timerRes;
				}
			}
			ProcessedBytes = dataSrc.Length;
		}

		protected override void DoWork() {
			progressUpdater.Start();
			processingThread = Thread.CurrentThread;

			var ogmOggFile = new OgmOggFile(dataSrc.Length);
			ogmOggFile.Parse(dataSrc);
			OgmOggFileObj = ogmOggFile;
		}

		protected override void InitInternal() {
			OgmOggFileObj = null;

			dataSrc = new FileSource(b, consumerId);

			progressUpdater = new Thread(ReadBytesUpdater);
			progressUpdater.Name = "Ogg/Ogm ProgressUpdater";
		}

		public static bool IsOgmOggFile(Stream fileStream) {
			int offset = 0;
			var b = new byte[fileStream.Length > 128 * 1024 ? 128 * 1024 : fileStream.Length];
			fileStream.Read(b, 0, b.Length);

			bool isOgmOggFile;
			try {

				int magicPos = 0;
				byte[] OggS = { (byte)'O', (byte)'g', (byte)'g', (byte)'S' };
				while(magicPos != 4 && offset != b.Length) if(b[offset++] == OggS[magicPos]) magicPos++; else /*magicPos = 0*/ break;
				isOgmOggFile = magicPos == 4;
				offset -= 4;

				if(isOgmOggFile) {
					int segmentCount = b[offset + 26];
					int dataLength = 0;
					for(int i = 0;i < segmentCount;i++) dataLength += b[offset + 27 + i];

					byte[] crcField = new byte[4];
					Buffer.BlockCopy(b, offset + 22, crcField, 0, 4);
					b[offset + 22] = b[offset + 23] = b[offset + 24] = b[offset + 25] = 0;

					var hash = (new Crc32()).ComputeHash(b, offset, 27 + segmentCount + dataLength);
					//isOgmOggFile = crcField.SequenceEqual(hash);
				}
			} catch(Exception) { isOgmOggFile = false; }

			fileStream.Position = 0;
			return isOgmOggFile;
		}

		public override string ToString() { return ""; }
	}
	public class OgmOggFile {
		public TrackCollection Tracks { get; private set; }
		public long FileSize { get; private set; }
		public long Overhead { get; private set; }

		public OgmOggFile(long fileSize) { FileSize = fileSize; Tracks = new TrackCollection(); }

		internal void Parse(FileSource dataSrc) {
			var pageHeader = new PageHeader();

			while(!dataSrc.EOF()) {
				do {
					PageHeader.ReadMagicBytes(dataSrc);
				} while(!dataSrc.EOF() && !pageHeader.Read(dataSrc));

				if(dataSrc.EOF()) break;

				Overhead += 27 + pageHeader.SegmentCount;

				if(Tracks.Contains(pageHeader.StreamId)) {
					Tracks[pageHeader.StreamId].ProcessPage(dataSrc, pageHeader);

				} else if((pageHeader.Flags & PageHeader.HeaderFlags.BOS) != 0) {
					Track track; object codecInfo;
					var header = PageHeader.ReadPacket(dataSrc, pageHeader.DataLength);

					Overhead += header.Length;

					if((codecInfo = VideoTrack.GetCodecInfo(header)) != null) {
						track = new VideoTrack(pageHeader.StreamId, codecInfo);
					} else if((codecInfo = AudioTrack.GetCodecInfo(header)) != null) {
						track = new AudioTrack(pageHeader.StreamId, codecInfo);
					} else if((codecInfo = SubtitleTrack.GetCodecInfo(header)) != null) {
						track = new SubtitleTrack(pageHeader.StreamId, codecInfo);
					} else {
						var path = Path.Combine(System.IO.Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), "OgmHeader");
						if(!Directory.Exists(path)) Directory.CreateDirectory(path);
						File.WriteAllBytes(Path.Combine(path ,DateTime.Now.ToString("yyyyMMdd HH.mm.ss.ffff") + " " + pageHeader.StreamId + ".bin"), header);

						track = new UnkownTrack(pageHeader.StreamId);
					}
					Tracks.Add(track);

					if(track is AudioTrack && ((AudioTrack)track).ChannelCount == 0) {
					}

				} else {
					Overhead += pageHeader.DataLength;
					dataSrc.Skip(pageHeader.DataLength);
				}
			}

			foreach(var track in Tracks) track.Finish();
		}

		public class TrackCollection : KeyedCollection<uint, Track> { protected override uint GetKeyForItem(Track item) { return item.Id; } }
	}
	public class PageHeader {
		private static byte[] OggS = { (byte)'O', (byte)'g', (byte)'g', (byte)'S' };
		private static byte[] nullArray = new byte[4];
		private static byte[] emptyArray = new byte[0];

		public Byte Version { get; private set; }
		public HeaderFlags Flags { get; private set; }
		public Int64 GranulePosition { get; private set; }
		public UInt32 StreamId { get; private set; }
		public UInt32 Index { get; private set; }
		public byte[] CRC32 { get; private set; }
		public ReadOnlyCollection<PacketLength> PacketLengths { get; private set; } private List<PacketLength> packetLengths;
		public int DataLength { get; private set; }
		public byte SegmentCount { get; private set; }

		public PageHeader() {
			//crc = new Crc32();
			packetLengths = new List<PacketLength>();
			PacketLengths = packetLengths.AsReadOnly();
		}

		[Flags]
		public enum HeaderFlags : byte { None = 0, ContinuedPacket = 1, BOS = 2, EOS = 4 }

		public static bool ReadMagicBytes(FileSource dataSrc) { return dataSrc.ProcessData(ReadMagicBytes); }
		private static bool ReadMagicBytes(DataInfo dataInfo) {
			int magicPos = 0;

			bool syncIssue = false;

			var block = dataInfo.GetBlock();
			while(magicPos != 4 && dataInfo.GetLength() != dataInfo.GetPosition()) {
				if(dataInfo.Offset == block.Length) { dataInfo.Advance(); block = dataInfo.GetBlock(); }
				if(block[dataInfo.Offset++] == OggS[magicPos]) magicPos++; else { if(!syncIssue) { Debug.Print("MagicPos Reset " + magicPos + " " + Convert.ToString(dataInfo.GetPosition(), 16)); syncIssue = true; } magicPos = 0; }
			}

			return magicPos == 4;
		}

		private static byte ReadByte(DataInfo datainfo) {
			//crc.TransformBlock(new byte[] { datainfo.GetBlock()[datainfo.Offset] }, 0, 1, null, 0);
			return datainfo.GetBlock()[datainfo.Offset++];
		}
		private static Int64 ReadInt64(DataInfo datainfo) {
			var block = datainfo.GetBlock();
			if(datainfo.GetPosition() + 8 > datainfo.GetLength()) {
				datainfo.Offset = block.Length;
				return 0;
			}

			Int64 value;
			if(datainfo.Offset + 8 > block.Length) {
				var b = new byte[8];
				var length = block.Length - (int)datainfo.Offset;
				//crc.TransformBlock(block, (int)datainfo.Offset, length, null, 0);
				Buffer.BlockCopy(block, (int)datainfo.Offset, b, 0, length);
				datainfo.Advance(); block = datainfo.GetBlock();
				//crc.TransformBlock(block, 0, 8 - length, null, 0);
				Buffer.BlockCopy(block, 0, b, length, 8 - length);
				datainfo.Offset += 8 - length;

				value = (b[0] & 0x80) != 0 ? -1 : 0;
				for(Int32 i = 0;i < 8;i++) { value <<= 8; value |= b[8 - i - 1]; }
				//value = BitConverter.ToInt64(b, 0);

			} else {
				//crc.TransformBlock(block, (int)datainfo.Offset, 8, null, 0);
				value = (block[(int)datainfo.Offset] & 0x80) != 0 ? -1 : 0;
				for(Int32 i = 0;i < 8;i++) { value <<= 8; value |= block[(int)datainfo.Offset + 8 - i - 1]; }
				//var value2 = BitConverter.ToInt64(block, (int)datainfo.Offset);

				datainfo.Offset += 8;
			}
			return value;
		}
		private static UInt32 ReadUInt32(DataInfo datainfo) {
			var block = datainfo.GetBlock();
			if(datainfo.GetPosition() + 4 > datainfo.GetLength()) {
				datainfo.Offset = block.Length;
				return 0;
			}

			UInt32 value;
			if(datainfo.Offset + 4 > block.Length) {
				var b = new byte[4];
				var length = block.Length - (int)datainfo.Offset;
				//crc.TransformBlock(block, (int)datainfo.Offset, length, null, 0);
				Buffer.BlockCopy(block, (int)datainfo.Offset, b, 0, length);
				datainfo.Advance(); block = datainfo.GetBlock();
				//crc.TransformBlock(block, 0, 4 - length, null, 0);
				Buffer.BlockCopy(block, 0, b, length, 4 - length);
				datainfo.Offset += 4 - length;

				value = BitConverter.ToUInt32(b, 0);
			} else {
				//crc.TransformBlock(block, (int)datainfo.Offset, 4, null, 0);
				value = BitConverter.ToUInt32(block, (int)datainfo.Offset);
				datainfo.Offset += 4;
			}
			return value;
		}
		private static byte[] Read4Bytes(DataInfo datainfo) {
			var block = datainfo.GetBlock();
			if(datainfo.GetPosition() + 4 > datainfo.GetLength()) {
				datainfo.Offset = block.Length;
				return new byte[0];
			}

			byte[] value = new byte[4];
			//crc.TransformBlock(emptyArray, 0, 4, null, 0);
			if(datainfo.Offset + 4 > block.Length) {
				var length = block.Length - (int)datainfo.Offset;
				Buffer.BlockCopy(block, (int)datainfo.Offset, value, 0, length);
				datainfo.Advance(); block = datainfo.GetBlock();
				Buffer.BlockCopy(block, 0, value, length, 4 - length);
				datainfo.Offset += 4 - length;

			} else {
				Buffer.BlockCopy(block, (int)datainfo.Offset, value, 0, 4);
				datainfo.Offset += 4;
			}
			return value;
		}
		private static byte[] ReadSegmentArray(DataInfo datainfo) {
			var block = datainfo.GetBlock();
			var segments = block[datainfo.Offset++];

			if(datainfo.GetPosition() + segments > datainfo.GetLength()) {
				datainfo.Offset = block.Length;
				return null;
			}

			byte[] value = new byte[segments];
			//crc.TransformBlock(emptyArray, 0, count, null, 0);
			if(datainfo.Offset + segments > block.Length) {
				var length = block.Length - (int)datainfo.Offset;
				Buffer.BlockCopy(block, (int)datainfo.Offset, value, 0, length);
				datainfo.Advance(); block = datainfo.GetBlock();
				Buffer.BlockCopy(block, 0, value, length, segments - length);
				datainfo.Offset += segments - length;

			} else {
				Buffer.BlockCopy(block, (int)datainfo.Offset, value, 0, segments);
				datainfo.Offset += segments;
			}
			return value;
		}
		public static byte[] ReadPacket(FileSource dataSrc, int packetLength) {
			Func<DataInfo, byte[]> readPacket = (dataInfo) => {
				var block = dataInfo.GetBlock();
				var dataLength = packetLength;

				if(dataInfo.GetPosition() + dataLength > dataInfo.GetLength()) {
					dataInfo.Offset = block.Length;
					return new byte[0];
				}

				byte[] value = new byte[dataLength];
				if(dataInfo.Offset + dataLength > block.Length) {
					var length = block.Length - (int)dataInfo.Offset;
					Buffer.BlockCopy(block, (int)dataInfo.Offset, value, 0, length);
					dataInfo.Advance(); block = dataInfo.GetBlock();
					Buffer.BlockCopy(block, 0, value, length, dataLength - length);
					dataInfo.Offset += dataLength - length;

				} else {
					Buffer.BlockCopy(block, (int)dataInfo.Offset, value, 0, dataLength);
					dataInfo.Offset += dataLength;
				}
				return value;
			};
			return dataSrc.ProcessData(readPacket);
		}

		public bool Read(FileSource dataSrc) {
			packetLengths.Clear();

			//crc.Initialize();
			//crc.TransformBlock(OggS, 0, 4, null, 0);

			if(dataSrc.Length - dataSrc.Position < 27) return false;

			Version = dataSrc.ProcessData(ReadByte);
			Flags = (HeaderFlags)dataSrc.ProcessData(ReadByte);
			GranulePosition = dataSrc.ProcessData(ReadInt64);
			StreamId = dataSrc.ProcessData(ReadUInt32);
			Index = dataSrc.ProcessData(ReadUInt32);
			CRC32 = dataSrc.ProcessData(Read4Bytes);


			var segmentArray = dataSrc.ProcessData(ReadSegmentArray);

			if(segmentArray == null) return false;
			SegmentCount = (byte)segmentArray.Length;

			DataLength = 0;
			bool isFinished = false;
			int i = 0, packetLength;
			while(i != segmentArray.Length) {
				packetLength = 0;
				while(i < segmentArray.Length && segmentArray[i] == 255) { packetLength += 255; i++; }
				if(i != segmentArray.Length) { packetLength += segmentArray[i++]; isFinished = true; }
				packetLengths.Add(new PacketLength { IsFinished = isFinished, Length = packetLength });
				DataLength += packetLength;
			}

			return true;
		}

		public struct PacketLength {
			public bool IsFinished;
			public int Length;
		}
	}

	public abstract class Track {
		protected object codecInfo;

		public uint Id { get; private set; }
		public long Length { get; private set; }
		public long EncodedDuration { get; private set; }
		public CommentHeader Comments { get; private set; } private List<byte[]> commentState = new List<byte[]>();
		public abstract string CodecName { get; }
		public abstract string CodecVersion { get; }
		public abstract double? Bitrate { get; }
		public abstract TimeSpan? Duration { get; }

		public Track(uint id, object codecInfo) { this.Id = id; this.codecInfo = codecInfo; }

		internal void ProcessPage(FileSource dataSrc, PageHeader pageHeader) {
			//EncodedDuration = pageHeader.GranulePosition != -1 && pageHeader.GranulePosition != 0 ? pageHeader.GranulePosition : EncodedDuration;
			EncodedDuration = pageHeader.GranulePosition > EncodedDuration ? pageHeader.GranulePosition : EncodedDuration;
			Length += pageHeader.DataLength;

			bool skipPacket = true;
			if(Comments == null && pageHeader.Index - 1 == commentState.Count) {
				var packet = PageHeader.ReadPacket(dataSrc, pageHeader.PacketLengths[0].Length);
				dataSrc.Skip(pageHeader.DataLength - pageHeader.PacketLengths[0].Length);

				skipPacket = false;

				if(ByteCompare(packet, new byte[] { 0x03, (byte)'v', (byte)'o', (byte)'r', (byte)'b', (byte)'i', (byte)'s' }) || commentState.Count != 0) {
					commentState.Add(packet);
					if(pageHeader.PacketLengths[0].IsFinished) {
						int offset = 0;

						byte[] b = new byte[commentState.Sum(item => item.Length) - 7];
						Buffer.BlockCopy(commentState[0], 7, b, offset, commentState[0].Length - 7);
						for(int i = 1;i < commentState.Count;i++) {
							Buffer.BlockCopy(commentState[i], 0, b, offset, commentState[i].Length);
							offset += commentState[i].Length;
						}

						Comments = CommentHeader.Parse(b);
					}
				}
			}

			//if(skipPacket) dataSrc.Skip(pageHeader.DataLength - InternalProcessPage(dataSrc, pageHeader));
			if(skipPacket) dataSrc.Skip(pageHeader.DataLength);
		}

		protected abstract int InternalProcessPage(FileSource dataSrc, PageHeader pageHeader);
		internal abstract void Finish();

		protected static bool ByteCompare(byte[] b, byte[] pattern) {
			if(b.Length < pattern.Length) return false;
			for(int i = 0;i < pattern.Length;i++) if(b[i] != pattern[i]) return false;
			return true;
		}

		protected static T GetStruct<T>(byte[] b, int offset, int length) {
			var structBytes = new byte[length];
			Buffer.BlockCopy(b, offset, structBytes, 0, length);

			GCHandle hDataIn = GCHandle.Alloc(structBytes, GCHandleType.Pinned);
			T structure = (T)Marshal.PtrToStructure(hDataIn.AddrOfPinnedObject(), typeof(T));
			hDataIn.Free();
			return structure;
		}


		public class CommentHeader {
			public string Vendor { get; private set; }
			public CommentCollection Comments { get; set; }

			private CommentHeader() { Comments = new CommentCollection(); }

			public static CommentHeader Parse(byte[] b) {
				var ch = new CommentHeader(); int offset = 0, length, count;

				length = EndianBitConverter.Little.ToInt32(b, offset); offset += 4; //TODO Remove EndianBitConverter dependency
				ch.Vendor = System.Text.Encoding.UTF8.GetString(b, offset, length); offset += length;
				count = EndianBitConverter.Little.ToInt32(b, offset); offset += 4;

				for(int i = 0;i < count;i++) {
					length = EndianBitConverter.Little.ToInt32(b, offset); offset += 4;

					var comment = Comment.Parse(System.Text.Encoding.UTF8.GetString(b, offset, length));
					if(ch.Comments.Contains(comment.Key)) {
						ch.Comments[comment.Key].Value.Add(comment.Value[0]);
					} else {
						ch.Comments.Add(comment); offset += length;
					}
				}

				return ch;
			}

			public class CommentCollection : KeyedCollection<string, Comment> { protected override string GetKeyForItem(Comment item) { return item.Key; } }
		}
		public class Comment {
			public string Key { get; private set; }
			public Collection<string> Value { get; private set; }

			private Comment() { Value = new Collection<string>(); }

			public static Comment Parse(string commentStr) {
				int pos = commentStr.IndexOf('=');
				Comment comment;
				if(pos < 0) {
					comment = new Comment { Key = "undefined" };
					comment.Value.Add(commentStr);
				} else {
					comment = new Comment { Key = commentStr.Substring(0, pos).ToLower() };
					comment.Value.Add(commentStr.Substring(pos + 1));
				}
				return comment;
			}
		}
	}
	public class VideoTrack : Track {
		public override string CodecName { get { return codecName; } } private string codecName;
		public override string CodecVersion { get { return codecVersion; } } private string codecVersion;
		public override double? Bitrate { get { return bitrate; } } private double? bitrate;
		public override TimeSpan? Duration { get { return duration; } } private TimeSpan? duration;

		public string FourCC { get; private set; }
		public long FrameCount { get { return (long)EncodedDuration; } }
		public double FrameRate { get; private set; }
		public int Width { get; private set; }
		public int Height { get; private set; }

		public VideoTrack(uint streamId, object codecInfo)
			: base(streamId, codecInfo) {

			if(codecInfo is OGMVideo) {
				var info = (OGMVideo)codecInfo;
				FourCC = new string(info.SubType);
				FrameRate = 10000000d / info.TimeUnit;
				Width = info.Width;
				Height = info.Height;
			}
		}

		protected override int InternalProcessPage(FileSource dataSrc, PageHeader pageHeader) { return 0; }
		internal override void Finish() {
			if(codecInfo is OGMVideo) {
				duration = TimeSpan.FromSeconds(FrameCount / FrameRate);
				bitrate = Length / duration.Value.TotalSeconds;
			}
		}

		public static object GetCodecInfo(byte[] header) {
			object codecInfo = null;

			if(header.Length == 0x39 && System.Text.Encoding.ASCII.GetString(header, 1, 5).Equals("video")) {
				codecInfo = GetStruct<OGMVideo>(header, 1, 0x38);
			}

			return codecInfo;
		}

		[StructLayout(LayoutKind.Sequential, Size = 52)]
		public struct OGMVideo {
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
			public char[] StreamType;
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
			public char[] SubType;
			public int Size;
			public long TimeUnit;
			public long SamplesPerUnit;
			public int DefaultLength;
			public int BufferSize;
			public short BitsPerSample;
			public int Width;
			public int Height;
		}
	}
	public class AudioTrack : Track {
		public override string CodecName { get { return codecName; } } private string codecName;
		public override string CodecVersion { get { return codecVersion; } } private string codecVersion;
		public override double? Bitrate { get { return bitrate; } } private double? bitrate;
		public override TimeSpan? Duration { get { return duration; } } private TimeSpan? duration;

		public long SampleCount { get { return (long)EncodedDuration; } }
		public double SampleRate { get; private set; }
		public int ChannelCount { get; private set; }

		public AudioTrack(uint streamId, object codecInfo)
			: base(streamId, codecInfo) {
			if(codecInfo is VorbisIdentHeader) {
				var info = (VorbisIdentHeader)codecInfo;
				codecName = "Vorbis";
				ChannelCount = info.ChannelCount;
				SampleRate = info.SampleRate;
				codecVersion = info.Version.ToString();
			} else if(codecInfo is OGMAudio) {
				var info = (OGMAudio)codecInfo;
				codecName = new string(info.SubType);
				ChannelCount = info.ChannelCount;
				SampleRate = info.SamplesPerUnit;
			//} else if(codecInfo is OGMAudio_00FF_0055) {
			//    var info = (OGMAudio_00FF_0055)codecInfo;
			//    codecName = new string(info.SubType);
			//    ChannelCount = info.ChannelCount;
			//    SampleRate = info.SamplesPerUnit;
			}
		}

		protected override int InternalProcessPage(FileSource dataSrc, PageHeader pageHeader) { return 0; }
		internal override void Finish() {
			if(codecInfo is OGMAudio || codecInfo is VorbisIdentHeader) {
				duration = TimeSpan.FromSeconds(SampleCount / SampleRate);
				bitrate = Length / duration.Value.TotalSeconds;
			}
		}


		private static byte[] VorbisIdentBytes = { 0x01, (byte)'v', (byte)'o', (byte)'r', (byte)'b', (byte)'i', (byte)'s' };
		public static object GetCodecInfo(byte[] header) {
			object codecInfo = null;

			if(ByteCompare(header, VorbisIdentBytes) && header.Length == 30) {
				codecInfo = GetStruct<VorbisIdentHeader>(header, VorbisIdentBytes.Length, 23);
			} else if(header.Length >= 46 && System.Text.Encoding.ASCII.GetString(header, 1, 5).Equals("audio")) {
				codecInfo = GetStruct<OGMAudio>(header, 1, 0x38);
			//} else if((header.Length == 0x3E || header.Length == 0x45) && System.Text.Encoding.ASCII.GetString(header, 1, 5).Equals("audio")) {
			//    codecInfo = GetStruct<OGMAudio_00FF_0055>(header, 1, header.Length - 1);
			}
			return codecInfo;
		}

		[StructLayout(LayoutKind.Sequential, Pack = 1)]
		public struct VorbisIdentHeader {
			public UInt32 Version;
			public Byte ChannelCount;
			public UInt32 SampleRate;
			public Int32 MaxBitrate;
			public Int32 NomBitrate;
			public Int32 MinBitrate;
			public Byte BlockSizes;
			public Boolean Framing;
		}

		//[StructLayout(LayoutKind.Sequential)]
		//public struct OGMAudio {
		//    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
		//    public Char[] StreamType;
		//    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
		//    public Char[] SubType;
		//    public Int32 Size;
		//    public Int64 TimeUnit;
		//    public Int64 SamplesPerUnit;
		//    public Int32 DefaultLength;
		//    public Int32 BufferSize;
		//    public Int16 BitsPerSample;
		//    public Int16 ChannelCount;
		//    public Int16 BlockAlign;
		//    public Int32 Byterate;
		//}

		[StructLayout(LayoutKind.Explicit)]
		public struct OGMAudio {
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8), FieldOffset(0)]
			public Char[] StreamType;
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4), FieldOffset(8)]
			public Char[] SubType;
			[FieldOffset(12)]
			public Int32 Size;

			[FieldOffset(24)]
			public Int64 SamplesPerUnit;

			[FieldOffset(44)]
			public Int16 ChannelCount;
		}
	}
	public class SubtitleTrack : Track {
		public override string CodecName { get { return codecName; } } private string codecName;
		public override string CodecVersion { get { return null; } }
		public override double? Bitrate { get { return null; } }
		public override TimeSpan? Duration { get { return duration; } } private TimeSpan? duration;

		public SubtitleTrack(uint streamId, object codecInfo) : base(streamId, codecInfo) { }

		protected override int InternalProcessPage(FileSource dataSrc, PageHeader pageHeader) { return 0; }
		internal override void Finish() { }

		public static object GetCodecInfo(byte[] header) {
			object codecInfo = null;
			if(header.Length == 0x39 && System.Text.Encoding.ASCII.GetString(header, 1, 4).Equals("text")) {
				codecInfo = GetStruct<OGMText>(header, 1, 0x38);
			}
			return codecInfo;
		}

		[StructLayout(LayoutKind.Sequential, Size = 52)]
		public struct OGMText {
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
			public Char[] StreamType;
			[MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
			public Char[] SubType;
			public Int32 Size;
			public Int64 TimeUnit;
			public Int64 SamplesPerUnit;
			public Int32 DefaultLength;
			public Int32 BufferSize;
			public Int16 BitsPerSample;
			public Int64 Unused;
		}
	}
	public class UnkownTrack : Track {
		public override string CodecName { get { throw new NotImplementedException(); } }
		public override string CodecVersion { get { throw new NotImplementedException(); } }
		public override double? Bitrate { get { throw new NotImplementedException(); } }
		public override TimeSpan? Duration { get { throw new NotImplementedException(); } }

		public UnkownTrack(uint streamId) : base(streamId, null) { }

		protected override int InternalProcessPage(FileSource dataSrc, PageHeader pageHeader) { return 0; }

		internal override void Finish() { }
	}
}

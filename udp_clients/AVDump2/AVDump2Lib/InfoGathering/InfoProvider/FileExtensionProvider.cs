using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;
using System.IO;
using System.Text.RegularExpressions;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class FileExtensionProvider : InfoProviderBase {
		private static byte[] Conv(string str) { return Encoding.ASCII.GetBytes(str); }


		public FileExtensionProvider(string path) {
			infos = new InfoCollection();
			using(Stream stream = File.Open(path, FileMode.Open, FileAccess.Read)) {

			}
		}


		public override void Dispose() { }
	}

	public class MpegAudio : FileType {
		public MpegAudio() : base(new byte[][] { new byte[] { 0xff }, new byte[] { (byte)'I', (byte)'D', (byte)'3' } }) { }

		private static int[,] bitRateTable = {
			{-1, -1 ,-1, -1, -1},
			{32,32,32,32,8},
			{64,48,40,48,16},
			{96,56,48,56,24},
			{128,64,56,64,32},
			{160,80,64,80,40},
			{192,96,80,96,48},
			{224,112,96,112,56},
			{256,128,112,128,64},
			{288,160,128,144,80},
			{320,192,160,160,96},
			{352,224,192,176,112},
			{384,256,224,192,128},
			{416,320,256,224,144},
			{448,384,320,256,160},
			{-1, -1 ,-1, -1, -1},
		};
		private static int[,] samplerateTable = {
			{44100,22050,11025},
			{48000,24000,12000},
			{32000,16000,8000},
			{-1,-1,-1},
		};

		public override void ElaborateCheck(Stream stream) {
			if(stream.Length > 4 * 1024 * 1024) IsCandidate = false;
			if(!IsCandidate) return;

			int oldByte = 0, newByte;
			while((newByte = stream.ReadByte()) != -1) {
				if(oldByte == 0xFF && (~newByte & 0xE0) == 0) break;
				oldByte = newByte;
			}

			MpegVer mpegVer = (MpegVer)newByte & MpegVer.MASK;
			Layer layer = (Layer)newByte & Layer.MASK;
			if((newByte = stream.ReadByte()) == -1) { IsCandidate = false; return; }

			int rowIndex = newByte & 0xF0;
			int columnIndex = mpegVer == MpegVer.Mpeg1? (layer == Layer.Layer1 ? 0 : (layer == Layer.Layer2 ? 1 : 2)) : ((mpegVer == MpegVer.Mpeg2 || mpegVer == MpegVer.Mpeg2_5) ? ((layer == Layer.Layer1) ? 3 : 4) : -1);
			if(columnIndex == -1) { IsCandidate = false; return; }

			int bitrate = bitRateTable[rowIndex, columnIndex];
			if(bitrate == -1){ IsCandidate = false; return;}


			rowIndex = newByte & 0x0C;
			columnIndex = mpegVer == MpegVer.Mpeg1? 0 : (mpegVer == MpegVer.Mpeg2 ? 1 : 2);
			if(columnIndex == -1) { IsCandidate = false; return; }

			int sampleRate = samplerateTable[rowIndex, columnIndex];
			bool padding = (newByte & 0x02) != 0;

			int frameLength = layer == Layer.Layer1 ? (12 * bitrate / sampleRate + (padding ? 1 : 0)) * 4 : 144 * bitrate / sampleRate + (padding ? 1 : 0);

			if((newByte = stream.ReadByte()) == -1 || newByte != 0xFF) { IsCandidate = false; return; }
			if((newByte = stream.ReadByte()) == -1 || (~newByte & 0xE0) != 0) { IsCandidate = false; return; }

			PossibleExtensions = new string[] { (layer == Layer.Layer1) ? "mp1" : ((layer == Layer.Layer2) ? "mp2" : "mp3") };
		}

		private enum MpegVer { Mpeg1 = 24, Mpeg2 = 16, Mpeg2_5 = 0, MASK = 0x18 }
		private enum Layer { Layer1 = 6, Layer2 = 4, Layer3 = 2, MASK = 0x06 }
	}
	public class SrtFileType : FileType {
		public SrtFileType() : base("") { PossibleExtensions = new string[] { "srt" }; }

		private static Regex regexParse = new Regex(@"(?<start>\d{2}\:\d{2}\:\d{2},\d{3}) --\> (?<end>\d{2}\:\d{2}\:\d{2},\d{3})", RegexOptions.Compiled | RegexOptions.ECMAScript);

		public override void ElaborateCheck(Stream stream) {
			if(stream.Length > 10 * 1024 * 1024) IsCandidate = false;
			if(!IsCandidate) return;

			string line;
			int lineType = 0, dummy;
			StreamReader sr = new StreamReader(stream);
			while(IsCandidate && lineType != 2 && (line = sr.ReadLine()) != null) {
				if((line = line.Trim()).Equals("")) continue;

				switch(lineType) {
					case 0: IsCandidate = int.TryParse(line, out dummy); break;
					case 1: IsCandidate = regexParse.IsMatch(line); break;
				}
				lineType++;
			}
		}
	}
	public class AssSsaFileType : FileType {
		public AssSsaFileType() : base("[Script Info]") { PossibleExtensions = new string[] { "ssa", "ass" }; }
		public override void ElaborateCheck(Stream stream) {
			if(!IsCandidate) return;
			IsCandidate = FindSequence(stream, "ScriptType: v4.00", 2048);

			if(IsCandidate && stream.Length != stream.Position) {
				PossibleExtensions = new string[] { stream.ReadByte() == '+' ? "ass" : "ssa" };
			} else {
				IsCandidate = false;
			}
		}
	}
	public class IdxFileType : FileType {
		public IdxFileType() : base("") { PossibleExtensions = new string[] { "idx" }; }
		public override void ElaborateCheck(Stream stream) {
			if(stream.Length > 10 * 1024 * 1024) IsCandidate = false;
			if(!IsCandidate) return;

			var sr = new StreamReader(stream);
			var line = sr.ReadLine();
			IsCandidate = line.Contains("VobSub index file, v");
		}
	}
	public class ZipFileType : FileType { public ZipFileType() : base(new byte[] { 0x50, 0x4b, 0x03, 0x04 }) { PossibleExtensions = new string[] { "zip" }; } public override void ElaborateCheck(Stream stream) { } }
	public class RarFileType : FileType { public RarFileType() : base(new byte[] { 0x52, 0x61, 0x72, 0x21, 0x1a, 0x07, 0x00 }) { PossibleExtensions = new string[] { "rar" }; } public override void ElaborateCheck(Stream stream) { } }
	public class RaFileType : FileType { public RaFileType() : base(".ra" + (char)0xfd) { PossibleExtensions = new string[] { "ra" }; } public override void ElaborateCheck(Stream stream) { } }
	public class FlacFileType : FileType { public FlacFileType() : base("fLaC") { PossibleExtensions = new string[] { "flac" }; } public override void ElaborateCheck(Stream stream) { } }
	public class AviFileType : FileType { public AviFileType() : base("RIFF") { PossibleExtensions = new string[] { "avi" }; } public override void ElaborateCheck(Stream stream) { IsCandidate &= Check(stream, 8, "AVI LIST"); } }
	public class WavFileType : FileType { public WavFileType() : base(new string[] { "RIFX", "RIFF" }) { PossibleExtensions = new string[] { "wav" }; } public override void ElaborateCheck(Stream stream) { IsCandidate &= Check(stream, 8, "WAVE"); } }

	public abstract class FileType : IFileType {
		private byte[][] magicBytesLst; private int[] magicBytesPos; private int offset;

		public bool NeedsMoreMagicBytes { get; private set; }
		public bool IsCandidate { get; protected set; }
		public string[] PossibleExtensions { get; protected set; }

		public FileType(string magicString, int offset = 0) : this(new string[] { magicString }, offset) { }
		public FileType(byte[] magicBytes, int offset = 0) : this(new byte[][] { magicBytes }, offset) { }
		public FileType(string[] magicStringLst, int offset = 0) : this(magicStringLst.Select(magicString => Encoding.ASCII.GetBytes(magicString)).ToArray(), offset) { }
		public FileType(byte[][] magicBytesLst, int offset = 0) {
			this.magicBytesLst = magicBytesLst;
			this.offset = offset;

			magicBytesPos = new int[magicBytesLst.Length];

			IsCandidate = true;

			foreach(var magicBytes in magicBytesLst) NeedsMoreMagicBytes |= magicBytes.Length != 0;
		}

		public void CheckMagicByte(byte b) {
			if(!NeedsMoreMagicBytes && offset-- > 0) return;

			bool needsMoreMagicBytes = false, isCandidate = false;
			for(int i = 0;i < magicBytesLst.Length;i++) {
				needsMoreMagicBytes |= magicBytesLst[i].Length != magicBytesPos[i] && (isCandidate |= magicBytesLst[i][magicBytesPos[i]++] == b);
			}

			NeedsMoreMagicBytes = needsMoreMagicBytes;
			IsCandidate = isCandidate;
		}

		public abstract void ElaborateCheck(Stream stream);

		protected static bool Check(Stream stream, int totalOffset, string str) { return Check(stream, totalOffset, Encoding.ASCII.GetBytes(str)); }
		protected static bool Check(Stream stream, int totalOffset, byte[] b) {
			bool isValid = true;

			stream.Position = totalOffset;
			foreach(var item in b) isValid &= item == stream.ReadByte();
			return isValid;
		}

		protected static bool FindSequence(Stream stream, string sequenceStr, int bytesToCheck) { return FindSequence(stream, Encoding.ASCII.GetBytes(sequenceStr), 2048); }
		protected static bool FindSequence(Stream stream, byte[] sequence, int bytesToCheck) {
			int sequencePos = 0;
			while(stream.Position != stream.Length && bytesToCheck-- != 0 && sequence.Length != sequencePos) if(sequence[sequencePos++] != stream.ReadByte()) sequencePos = 0;
			return sequence.Length == sequencePos;
		}
	}

	public interface IFileType {
		bool IsCandidate { get; }
		bool NeedsMoreMagicBytes { get; }
		string[] PossibleExtensions { get; }



		void ElaborateCheck(Stream stream);
		void CheckMagicByte(byte b);
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.BlockConsumers;
using System.Globalization;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class MatroskaProvider : InfoProviderBase {
		public MatroskaFile MFI { get; private set; }
		public MatroskaProvider(MatroskaFile mfi) {
			infos = new InfoCollection();
			MFI = mfi;

			Add(EntryKey.Size, MFI.SectionSize.ToString(), "byte");
			Add(EntryKey.Date, MFI.Segment.SegmentInfo.ProductionDate.HasValue ? MFI.Segment.SegmentInfo.ProductionDate.Value.ToString("yyyy.MM.dd HH.mm.ss.ffff") : null, null);
			Add(EntryKey.Duration, MFI.Segment.SegmentInfo.Duration.HasValue ? (MFI.Segment.SegmentInfo.Duration.Value * (MFI.Segment.SegmentInfo.TimecodeScale / 1000000000d)).ToString("0.000", CultureInfo.InvariantCulture) : null, "s");
			Add(EntryKey.WritingApp, MFI.Segment.SegmentInfo.WritingApp, null);
			Add(EntryKey.MuxingApp, MFI.Segment.SegmentInfo.MuxingApp, null);


			int[] indeces = new int[3];
			bool hasVideo, hasAudio, hasSubtitle;
			hasVideo = hasAudio = hasSubtitle = false;
			foreach(var track in MFI.Segment.Tracks.Items) {
				if(!track.TrackType.HasValue) throw new Exception("TrackType missing");
				switch(track.TrackType.Value) {
					case TrackEntrySection.Types.Video: AddStreamInfo(track, StreamType.Video, indeces[0]++); hasVideo = true; break;
					case TrackEntrySection.Types.Audio: AddStreamInfo(track, StreamType.Audio, indeces[1]++); hasAudio = true; break;
					case TrackEntrySection.Types.Subtitle: AddStreamInfo(track, StreamType.Text, indeces[2]++); hasSubtitle = true; break;
					default: break;
				}
			}

			if(hasVideo) {
				Add(EntryKey.Extension, "mkv", null);
			} else if(hasAudio) {
				Add(EntryKey.Extension, "mka", null);
			} else if(hasSubtitle) {
				Add(EntryKey.Extension, "mks", null);
			}
		}

		private void AddStreamInfo(TrackEntrySection trackEntry, StreamType type, int index) {
			ClusterSection.TrackInfo trackInfo = null;
			try {
				trackInfo = MFI.Segment.Cluster.Tracks[(int)trackEntry.TrackNumber.Value].CalcTrackInfo();
			} catch(Exception) { }

			Add(type, index, EntryKey.Index, () => index.ToString(), null);
			Add(type, index, EntryKey.Size, () => trackInfo.TrackSize.ToString(), "byte");
			Add(type, index, EntryKey.Bitrate, () => trackInfo.AverageBitrate.HasValue ? trackInfo.AverageBitrate.Value.ToString("0", CultureInfo.InvariantCulture) : null, "bit/s");
			Add(type, index, EntryKey.Duration, () => trackInfo.TrackLength.TotalSeconds.ToString(CultureInfo.InvariantCulture), "s");
			Add(type, index, EntryKey.Title, () => trackEntry.Name, null);
			Add(type, index, EntryKey.Language, () => trackEntry.Language, null);
			Add(type, index, EntryKey.CodecName, () => trackEntry.CodecName, null);
			Add(type, index, EntryKey.CodecId, () => trackEntry.CodecId, null);
			Add(type, index, EntryKey.FourCC, () => string.Equals(trackEntry.CodecId, "V_MS/VFW/FOURCC") && trackEntry.GetBitMapInfoHeader().HasValue ? trackEntry.GetBitMapInfoHeader().Value.FourCC : null, null);
			Add(type, index, EntryKey.TwoCC, () => string.Equals(trackEntry.CodecId, "A_MS/ACM") && trackEntry.GetWaveFormatEx().HasValue ? trackEntry.GetWaveFormatEx().Value.TwoCC : null, null);
			Add(type, index, EntryKey.Id, () => trackEntry.TrackUId.ToString(), null);

			switch(type) {
				case StreamType.Video:
					Add(type, index, EntryKey.FrameCount, () => trackInfo.LaceCount.ToString(), null);
					Add(type, index, EntryKey.FrameRate, () => (trackEntry.DefaultDuration.HasValue ? 1000000000d / trackEntry.DefaultDuration.Value : 0).ToString("0.000", CultureInfo.InvariantCulture), "fps");

					//if(trackInfo.MinLaceRate + 2 < trackInfo.MaxLaceRate) {
					if(trackInfo.AverageLaceRate.HasValue) Add(type, index, EntryKey.VFR, () => trackInfo.AverageLaceRate.Value.ToString("0.000", CultureInfo.InvariantCulture), "fps");
					if(trackInfo.MinLaceRate.HasValue) Add(type, index, EntryKey.MinFrameRate, () => trackInfo.MinLaceRate.Value.ToString("0.000", CultureInfo.InvariantCulture), "fps");
					if(trackInfo.MaxLaceRate.HasValue) Add(type, index, EntryKey.MaxFrameRate, () => trackInfo.MaxLaceRate.Value.ToString("0.000", CultureInfo.InvariantCulture), "fps");
					//}

					Add(type, index, EntryKey.Width, () => trackEntry.Video.PixelWidth.ToString(), "px");
					Add(type, index, EntryKey.Height, () => trackEntry.Video.PixelHeight.ToString(), "px");
					Add(type, index, EntryKey.DAR, () => (trackEntry.Video.DisplayWidth / (double)trackEntry.Video.DisplayHeight).ToString("0.000", CultureInfo.InvariantCulture), null);
					break;
				case StreamType.Audio:
					Add(type, index, EntryKey.SampleCount, () => ((int)(trackInfo.TrackLength.TotalSeconds * trackEntry.Audio.SamplingFrequency)).ToString(), null);
					Add(type, index, EntryKey.SamplingRate, () => trackEntry.Audio.SamplingFrequency.ToString(CultureInfo.InvariantCulture), null);
					Add(type, index, EntryKey.ChannelCount, () => trackEntry.Audio.ChannelCount.ToString(), null);
					break;
				case StreamType.Text: break;
			}
		}

		public override void Dispose() { }

	}
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.BlockConsumers;
using System.Globalization;
using System.Collections.ObjectModel;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class OgmOggProvider : InfoProviderBase {
		public OgmOggFile provider { get; private set; }

		public OgmOggProvider(OgmOggFile provider) {
			infos = new InfoCollection();
			this.provider = provider;

			Add(EntryKey.Size, provider.FileSize.ToString(), "byte");
			Add(EntryKey.Overhead, provider.Overhead.ToString(), "byte");

			Add(EntryKey.Duration, provider.Tracks.Max(t => !(t is UnkownTrack) && t.Duration.HasValue ? t.Duration.Value.TotalSeconds : 0).ToString("0.000", CultureInfo.InvariantCulture), "s");

			int[] indeces = new int[4];
			foreach(var track in provider.Tracks) {
				if(track is VideoTrack) {
					AddStreamInfo(track, StreamType.Video, indeces[0]++);
				} else if(track is AudioTrack) {
					AddStreamInfo(track, StreamType.Audio, indeces[1]++);
				} else if(track is SubtitleTrack) {
					AddStreamInfo(track, StreamType.Text, indeces[2]++);
				} else if(track is UnkownTrack) {
					Add(StreamType.Unkown, indeces[3], EntryKey.Id, () => track.Id.ToString(), null);
					Add(StreamType.Unkown, indeces[3], EntryKey.Size, () => track.Length.ToString(), "byte");
					indeces[3]++;
				}
			}

			if(provider.Tracks.Count == 1 && ((Collection<Track>)provider.Tracks)[0].CodecName.Equals("Vorbis")) {
				Add(EntryKey.Extension, "ogg", null);
			} else {
				Add(EntryKey.Extension, "ogm", null);
			}
		}

		private void AddStreamInfo(Track track, StreamType type, int index) {
			Add(type, index, EntryKey.Index, () => index.ToString(), null);
			Add(type, index, EntryKey.Id, () => track.Id.ToString(), null);
			Add(type, index, EntryKey.Size, () => track.Length.ToString(), "byte");
			Add(type, index, EntryKey.Bitrate, () => track.Bitrate.HasValue? track.Bitrate.Value.ToString("0", CultureInfo.InvariantCulture) : null, "bit/s");
			Add(type, index, EntryKey.Duration, () => track.Duration.HasValue ? track.Duration.Value.TotalSeconds.ToString(CultureInfo.InvariantCulture) : null, "s");

			if(track.Comments != null) {
				if(track.Comments.Comments.Contains("title")) Add(type, index, EntryKey.Title, () => track.Comments.Comments["title"].Value.Aggregate((acc, str) => acc + "," + str), null);
				if(track.Comments.Comments.Contains("language")) Add(type, index, EntryKey.Language, () => track.Comments.Comments["language"].Value.Aggregate((acc, str) => acc + "," + str), null);
			}

			Add(type, index, EntryKey.CodecId, () => track.CodecName, null);

			switch(type) {
				case StreamType.Video: var vTrack = (VideoTrack)track;
					Add(type, index, EntryKey.FourCC, () => vTrack.FourCC, null);
					Add(type, index, EntryKey.FrameCount, () => vTrack.FrameCount.ToString(), null);
					Add(type, index, EntryKey.FrameRate, () => vTrack.FrameRate.ToString("0.000", CultureInfo.InvariantCulture), "fps");
					Add(type, index, EntryKey.Width, () => vTrack.Width.ToString(), "px");
					Add(type, index, EntryKey.Height, () => vTrack.Height.ToString(), "px");
					break;
				case StreamType.Audio: var aTrack = (AudioTrack)track;
					Add(type, index, EntryKey.SampleCount, () => aTrack.SampleCount.ToString(), null);
					Add(type, index, EntryKey.SamplingRate, () => aTrack.SampleRate.ToString(CultureInfo.InvariantCulture), null);
					Add(type, index, EntryKey.ChannelCount, () => aTrack.ChannelCount.ToString(), null);
					break;
				case StreamType.Text: break;
			}
		}

		public override void Dispose() { }
	}
}

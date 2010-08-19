using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.InfoGathering.Parser.MediaInfoLib;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class FileExtensionProvider : InfoProviderBase {

		public FileExtensionProvider(MediaInfoProvider mil) {
			infos = new InfoCollection();

			bool hasVideo, hasAudio, hasSubtitle;
			hasVideo = hasAudio = hasSubtitle = false;

			string format = mil.MIL.Get(eStreamType.General, 0, "Format");
			if(string.IsNullOrEmpty(format)) return;

			switch(format.ToUpper()) {
				case "MPEG-4": Add(EntryKey.Extension, "mp4", null); break;
				case "BDMV":
				case "BDAV": Add(EntryKey.Extension, "m2ts", null); break;
				case "WINDOWS MEDIA":
					hasVideo = mil.MIL.Count_Get(eStreamType.Video) != 0;
					hasAudio = mil.MIL.Count_Get(eStreamType.Audio) != 0;
					hasSubtitle = mil.MIL.Count_Get(eStreamType.Text) != 0;

					if(hasVideo || (!hasAudio && !hasVideo)) {
						Add(EntryKey.Extension, "wmv", null);
					} else if(hasAudio) {
						Add(EntryKey.Extension, "wma", null);
					}
					break;
				case "AVI": Add(EntryKey.Extension, "AVI", null); break;
			}
		}

		public override void Dispose() { }
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2Lib.InfoGathering.InfoProvider.Tools {
	public class CompositeInfoProvider : InfoProviderBase {
		private IEnumerable<InfoProviderBase> providers;

		public CompositeInfoProvider(IEnumerable<InfoProviderBase> providers) { this.providers = providers; }
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
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVDump2Lib.InfoGathering.InfoProvider.Tools;
using AVDump2Lib.Misc;
using AVDump2Lib.BlockConsumers;
using AVDump2Lib.HashAlgorithms;

namespace AVDump2Lib.InfoGathering.InfoProvider {
	public class HashInfoProvider : InfoProviderBase {
		public HashInfoProvider(IEnumerable<HashCalculator> hashCalculators) {
			infos = new InfoCollection();

			int i = 0;
			var hash = hashCalculators.First().HashObj.Hash;
			foreach(var hashCalculator in hashCalculators) {
				BaseOption baseOption = ((hashCalculator.HashObj is TTH || hashCalculator.HashObj is TigerForTTH) ? BaseOption.Base32 : BaseOption.Heximal) | BaseOption.Pad | BaseOption.Reverse;
				Add(StreamType.Hash, i++, EntryKey.None, BaseConverter.ToString(hashCalculator.HashObj.Hash, baseOption).ToLower(), hashCalculator.Name.ToLower());

				if(hashCalculator.HashObj is Ed2k) {
					Ed2k ed2k = (Ed2k)hashCalculator.HashObj;
					if(!ed2k.BlueIsRed) {
						baseOption = BaseOption.Heximal | BaseOption.Pad | BaseOption.Reverse;
						Add(StreamType.Hash, i++, EntryKey.None, BaseConverter.ToString(ed2k.RedHash, baseOption).ToLower(), hashCalculator.Name.ToLower() + "_alt");
					}
				}
			}
		}

		public override void Dispose() { }
	}
}

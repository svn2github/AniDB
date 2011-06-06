using System;
namespace AVDump2Lib.InfoGathering.Parser.MediaInfoLib {
	public interface IMediaInfo {
		void Close();
		int Count_Get(eStreamType StreamKind);
		int Count_Get(eStreamType StreamKind, int StreamNumber);
		string Get(eStreamType StreamKind, int StreamNumber, int Parameter);
		string Get(eStreamType StreamKind, int StreamNumber, int Parameter, eInfoType KindOfInfo);
		string Get(eStreamType StreamKind, int StreamNumber, string Parameter);
		string Get(eStreamType StreamKind, int StreamNumber, string Parameter, eInfoType KindOfInfo);
		string Get(eStreamType StreamKind, int StreamNumber, string Parameter, eInfoType KindOfInfo, eInfoType KindOfSearch);
		string Inform();
		int Open(string FileName);
		string Option(string Option, string Value);
		string Option(string Option_);
		int State_Get();
	}
}

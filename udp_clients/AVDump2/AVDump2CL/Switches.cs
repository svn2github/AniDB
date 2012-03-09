using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AVDump2CL {
	[Flags]
	enum eSwitches : long {
		None = 0L,

		//Output

		CreqXmlFormat = 1L << 3,
		MediaInfoOutPut = 1L << 4,
		MediaInfoXMLOutPut = 1L << 5,
		NewCreqXmlFormat = 1L << 6,
		TxtFormat = 1L << 7,
		HashOutput = 1L << 8,

		//Control
		ExcludeSubFolders = 1L << 16,
		UseCWD = 1L << 17,
		MonitorFolder = 1L << 18,
		PauseWhenDone = 1L << 20,
		PauseWhenFileDone = 1L << 21,
		RandomFileOrder = 1L << 22,
		PrintTimeUsedPerFile = 1L << 23,
		DeleteFileWhenDone = 1L << 24,
		WaitForDumpReply = 1L << 25,
		SupressProgress = 1L << 27,
		PrintTotalTimeUsed = 1L << 28,
		NoErrorReporting = 1L << 29,
		HashingSpeedTest = 1L << 30,
		DumpAllExtensions = 1L << 31,
		UseUTF8OutputStream = 1L << 32,
		DoneLogFilenameOnly = 1L << 33,

		DefAudioExtensions = 1L << 34,
		DefVideoExtensions = 1L << 35,
		DefSubtitleExtensions = 1L << 36,



		//Hash
		Crc = 1L << 39,
		Ed2k = 1L << 40,
		Md4 = 1L << 41,
		Md5 = 1L << 42,
		Sha1 = 1L << 43,
		Tiger = 1L << 44,
		Tth = 1L << 45,
		Aich = 1L << 46,
		UseAllHashes = Crc | Ed2k | Md5 | Sha1 | Tth | Tiger | Aich | Md4,
		UseACreqHashes = Crc | Ed2k | Md5 | Sha1 | Tth | Aich,
		PrintElapsedHashingTime = 1L << 52,
		PrintEd2kLink = 1L << 60,
		PrintAniDBLink = 1L << 61,
		OpenInBrowser = 1L << 62,
	}
}

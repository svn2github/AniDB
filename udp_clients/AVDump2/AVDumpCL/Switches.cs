// Copyright (C) 2009 DvdKhl 
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.using System;

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

		//Control
		ExcludeSubFolders = 1L << 16,
		MonitorFolder = 1L << 18,
		PauseWhenDone = 1L << 20,
		PauseWhenFileDone = 1L << 21,
		RandomFileOrder = 1L << 22,
		PrintTimeUsedPerFile = 1L << 23,
		DeleteFileWhenDone = 1L << 24,
		WaitForDumpReply = 1L << 25,
		SupressProgress = 1L << 27,
		PrintTotalTimeUsed = 1L << 28,

		DefAudioExtensions = 1L << 32,
		DefVideoExtensions = 1L << 33,
		DefSubtitleExtensions = 1L << 34,

		//Hash
		Crc32 = 1L << 40,
		Ed2k = 1L << 41,
		Md5 = 1L << 42,
		Sha1 = 1L << 43,
		Tth = 1L << 44,
		Tiger = 1L << 45,
		Aich = 1L << 46,
		UseAllHashes = Crc32 | Ed2k | Md5 | Sha1 | Tth | Tiger | Aich,
		PrintElapsedHashingTime = 1L << 52,
		PrintEd2kLink = 1L << 60,
		PrintAniDBLink = 1L << 61,
		OpenInBrowser = 1L << 62,
	}
}

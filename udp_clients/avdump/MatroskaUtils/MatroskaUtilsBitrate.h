/**
	*  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
	*
	*  MatroskaUtilsBitrate.h
	*
	*  Copyright (C) Jory Stone - June 2003
	*
	*
	*  The TCMP Matroska CDL is free software; you can redistribute it and/or modify
	*  it under the terms of the GNU General Public License as published by
	*  the Free Software Foundation; either version 2, or (at your option)
	*  any later version.
	*
	*  The TCMP Matroska CDL is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	*  GNU General Public License for more details.
	*
	*  You should have received a copy of the GNU General Public License
	*  along with The TCMP Matroska CDL; see the file COPYING.  If not, write to
	*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
	*
*/

/*!
    \file MatroskaUtilsBitrate.h
		\version \$Id: MatroskaUtilsBitrate.h,v 1.3 2004/03/08 07:56:51 jcsston Exp $
    \brief Some code to get bitrate infomation about a Matroska file
		\author Jory Stone     <jcsston @ toughguy.net>		
		
*/

#ifndef _MATROSKA_UTILS_BITRATE_H_
#define _MATROSKA_UTILS_BITRATE_H_

//#include "MatroskaUtilsDebug.h"
#include "MatroskaUtilsReader.h"
#include "JString.h"
//#include "JCThread.h"
#include "MatroskaUtils.h"

//standard includes
// We use Win32 API to directly draw our graph
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <malloc.h>

// libebml includes
#include "ebml/StdIOCallback.h"
#include "ebml/EbmlTypes.h"
#include "ebml/EbmlHead.h"
#include "ebml/EbmlVoid.h"
#include "ebml/EbmlCrc32.h"
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlStream.h"
#include "ebml/EbmlBinary.h"
#include "ebml/EbmlString.h"
#include "ebml/EbmlUnicodeString.h"
#include "ebml/EbmlContexts.h"
#include "ebml/EbmlVersion.h"

// libmatroska includes
#include "matroska/KaxConfig.h"
#include "matroska/KaxBlock.h"
#include "matroska/KaxSegment.h"
#include "matroska/KaxContexts.h"
#include "matroska/KaxCluster.h"
#include "matroska/KaxClusterData.h"
#include "matroska/KaxBlock.h"
#include "matroska/KaxBlockData.h"
#include "matroska/KaxVersion.h"

#define TIMECODE_SCALE  1000000

//namespace MatroskaUtilsNamespace {

class MatroskaBitrateInfoTrack {
public:
	MatroskaBitrateInfoTrack();
	//void ProcessBitrateVector();

	uint64 uid;
	/*float minBitrate;
	float avgBitrate;
	float maxBitrate;
	std::vector<float> bitrates;
	float bitrateLength;*/
	double duration;
};

class MatroskaBitrateInfo;

class MatroskaBitrateInfoReader { //! : public JCThread {
public:
	MatroskaBitrateInfoReader(MatroskaBitrateInfo *info);
	~MatroskaBitrateInfoReader();

	bool Init();
	bool Loop();
	bool Clean();

	float timecode;
	bool corrupt;
	
protected:
	void Register(KaxInternalBlock &DataBlock);
	MatroskaBitrateInfo *m_Info;
	//MatroskaUilsFileReader *m_Reader;
	std::basic_string<TCHAR> &m_TextOutput;

	///
	int UpperElementLevel;
	bool bAllowDummy;
	EbmlElement *ElementLevel0;
	EbmlElement *ElementLevel1;
	EbmlElement *ElementLevel2;
	EbmlElement *ElementLevel3;
	EbmlElement *ElementLevel4;

	MatroskaUilsFileReader *reader;
	EbmlStream *inputStream;

	std::vector<float> *prevTimecodes;
	std::vector<float> *currentSize;
};

#ifndef MatroskaInfoParser
class MatroskaInfoParser;
#endif

class MatroskaBitrateInfo {
public:
	MatroskaBitrateInfo();
	~MatroskaBitrateInfo();

	int Open(MatroskaInfoParser *parser);
	/*
	int Save();
	int Draw(HWND hWnd, DWORD x1, DWORD y1, DWORD x2, DWORD y2);
	void SetReportTextCtrl(HWND txtCtrl);
	const std::basic_string<TCHAR> &GetReport(){return m_TextOutput;};
	uint64 GetFilePos();*/
	float GetCurrentPercent();
	//void Stop();
	
	//std::vector<double> accSize;
	MatroskaBitrateInfoReader *m_Reader;
	MatroskaInfoParser *m_Parser;
protected:
	HWND m_TargetTextCtrl;
	JString m_SourceFilename;
	std::vector<MatroskaBitrateInfoTrack> m_TrackList;
#ifdef USING_JCT
	JCCritSec m_FilePosCritSec;
#endif
	uint64 m_FilePos;
	uint64 m_FileSize;
	int hr;
	
	std::basic_string<TCHAR> m_TextOutput;
	friend MatroskaBitrateInfoReader;
};

//};

#endif // _MATROSKA_UTILS_BITRATE

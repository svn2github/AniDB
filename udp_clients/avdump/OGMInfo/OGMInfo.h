/*********************************************************************
 * OGMInfo : an OGM (Ogg based multimedia files) information tool
 *
 * Main functions
 *
 * Copyright (C) 2003 Julien Coloos.  All rights reserved.
 *
 * This file is part of OGMInfo.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE.QPL included in the packaging of this file.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * Licensees holding an other license may use this file in accordance with 
 * the Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See http://www.opensource.org/licenses/qtpl.php for QPL licensing information.
 * See http://www.opensource.org/licenses/gpl-license.html for GPL licensing information.
 *
 * Contact cyrius@corecodec.org if any conditions of this licensing
 * are not clear to you.
 *
 ********************************************************************/

#ifndef __OGMINFO_EXE_H__
#define __OGMINFO_EXE_H__

#include <Common/common.h>
#include <Common/IOStream_File.h>
#include <Common/Fraction.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <Common/OGM.h>


//#define OGMINFO_VERSION	L"2.0b4"

// Buffer size for oggpack_buffer (see Ogg Vorbis SDK for more details)
// oggpack_buffer is an handled buffer proposed by the SDK to help
// decoding file contents
#define BLOCK_SIZE	(32*1024)


typedef enum {
	kTrackUnknown		= 0x00000000,

	kTrackOld			= 0x00000001,
	kTrackVideo			= 0x00000002,
	kTrackAudio			= 0x00000004,
	kTrackText			= 0x00000008,
	kTrackTypeMask		= 0x000000FF,

	kTrackSubtypeVorbis	= 0x00000100,
	kTrackSubtypeMask	= 0x0000FF00,

	kTrackVorbis		= kTrackAudio | kTrackSubtypeVorbis,

	kTrackInvalid		= 0x80000000
} eTrackType;

class OggTrack {
public:
	int					m_iSerial;
	uint32				m_iType;
	int					m_iIndex;
	ogg_stream_state	m_StreamState;

	int					m_iLastPage;
	int					m_iPagesProcessed;
	int					m_iPagesSkipped;

	sint64				m_iPackets;
	sint64				m_iPacketsProcessed;
	sint64				m_iPackets_Data;
	sint64				m_iPacketsSkipped;
	sint64				m_iLastPacket;

	sint64				m_iSamplesProcessed;

	sint64				m_iCurrentGranulepos;
	bool				m_bGranuleposZeroBased;
	bool				m_bGranuleposRepresentsEnd;
	sint64				m_iGranuleposZeroOffset;

	sint64				m_iDurationUS;

	sint64				m_iBytes_PageHeader;
	sint64				m_iBytes_PageBody;
	sint64				m_iBytes_Packets;
	sint64				m_iBytes_Data;

	bool				m_bEOSReached;

	vorbis_info			m_VorbisInfo;
	vorbis_comment		m_VorbisComment;

	bool				m_bHeader;
	std::vector<uint8>	m_StreamHeader;
	stream_header		*m_pStreamHeader;

	Fraction			m_Samplerate;

	int					m_iPrevW;

	OggTrack(int iSerial);
	virtual ~OggTrack(void);

	bool	InitializeStreamState(void);
	void	InitializeStreamHeader(int iSize=0, void *pHeader=NULL);
	void	InitializeGranulepos(bool bGranuleposZeroBased, bool bGranuleposRepresentsEnd);
};
class MyOGM{
	sint64	g_iLastTimeStartUS;
	int	g_iTracks_Unknown;
	int	g_iTracks_Old;
	int	g_iTracks_Video;
	int	g_iTracks_Audio;
	int	g_iTracks_Text;	

	IOFile *input;
	
	sint64 iPagesRead;
	sint64 iPagesReadProgress;
	ogg_sync_state	sync;
	ogg_page		page;

	std::list<OggTrack *>	g_Tracks;	
public:
	std::string vcodec;
	MyOGM(wchar_t *path);
	~MyOGM();
	float Parse();
	float GetDuration();
void	FreeGlobalVars(void);
//void	DisplayCopyright(ToolSystem& toolSystem);
//bool	ProcessCommandLine(IOFile& input, int iNbArgs, char **pArgs);
//void	DisplaySyntax(const char *szExeName);

int		GetNextPage(IOStream& input, ogg_sync_state *pSync, ogg_page *pPage);
OggTrack	*GetTrack(int iSerial, bool bCreate);
OggTrack	*GetTrack(int nr);
int		GetNumberOfTracks();

//+void	ProcessInput(IOFile& input);
void	ProcessPage(ogg_page& page, bool bBOS);
void	ProcessPage_Position(ogg_page& page, OggTrack *pTrack, std::wstring& strInfo);
void	ProcessPacket(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo, bool bHeader);
//+void	AppendTrackNumber(OggTrack *pTrack, std::wstring& strInfo);
//+void	ProcessPacket_Generic(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo, bool bHeader);
//+void	ProcessPacket_Header(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo);
void	ProcessPacket_Comment(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo);
void	ProcessPacket_Data(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo);
void	ProcessPacket_Vorbis(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo);
/*
void	DumpTracks(void);
void	DumpTrack_Serial(OggTrack *pTrack, std::wstring& strInfo, const wchar_t *szType);
void	DumpTrack_Video(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Audio(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Text(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Unknown(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Header(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Comments(OggTrack *pTrack, std::wstring& strInfo);
void	DumpTrack_Statistic(OggTrack *pTrack, std::wstring& strInfo);
*/
long  GetBlocksize(ogg_packet& packet, OggTrack *pTrack);
};
void	MicrosToTime(sint64 iTimeUS, sint32& iHour, sint32& iMinute, sint32& iSecond, sint32& iMicros);
#endif // __OGMINFO_EXE_H__

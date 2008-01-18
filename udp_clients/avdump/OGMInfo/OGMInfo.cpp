/*********************************************************************
 * OGMInfo : an OGM (Ogg based multimedia files) information tool
 *
 * Main functions
 *
 * Copyright (C) 2004 Julien Coloos.  All rights reserved.
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
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "OGMInfo.h"
//#include <Common/common.h>
//#include <Common/IOStream_File.h>
//#include <stdio.h>
//#include <stdarg.h>
//#include <string>
//#include <locale.h>


//+char			*g_pLogFile = NULL;
//+IOStream		*g_pLog = NULL;
//+ToolSystem		g_ToolSystem;
//+ToolSystem		g_ConsoleToolSystem;
//+int				g_iVerbosity = 0;
bool			g_bShowProgress = false;//
bool			g_bHeadersOnly = false;//
MyOGM::MyOGM(wchar_t *path){
	//_crtBreakAlloc = 312;
	g_iLastTimeStartUS = 0;
	g_iTracks_Unknown = 0;
	g_iTracks_Old = 0;
	g_iTracks_Video = 0;
	g_iTracks_Audio = 0;
	g_iTracks_Text = 0;

	ToolSystem g_ToolSystem;
	input = new IOFile(g_ToolSystem);
	input->Open(path, nsFile::kRead | nsFile::kOpenExisting | nsFile::kSequential);
	
	iPagesRead = iPagesReadProgress = 0;
	// With an average of 4kb / Page, estimate the number of
	// Pages to read that represent 1% of the file
	iPagesReadProgress = input->GetSize() / (100 * 4096);
	if(iPagesReadProgress <= 0)
		iPagesReadProgress = 1;
	//ProcessInput((*input));
	ogg_sync_init(&sync);
}
float MyOGM::Parse(){
	int read = GetNextPage(*input, &sync, &page);
	ProcessPage(page, ogg_page_bos(&page));		
	iPagesRead++;
	if(read<=0) return -1.0f;
	return (float)(100. * double(input->GetPosition())) / double(input->GetSize());
}
MyOGM::~MyOGM() {
/*+	if(g_pLog) {
		g_ToolSystem.setPrintFunction(DefaultPrint, NULL);
		delete g_pLog;
		g_pLog = NULL;
	}+*/
	//if(input)
	ogg_sync_clear(&sync);
	delete input;
	//LIST_CLEAR(OggTrack, g_Tracks);
	std::list<OggTrack *>::iterator trackIt;
	
	for(trackIt=g_Tracks.begin() ; trackIt!=g_Tracks.end() ; trackIt++) {
		OggTrack *pTrack = (*trackIt);
		//g_Tracks.remove(pTrack);
		
		_ogg_free(pTrack->m_StreamState.lacing_vals);
		_ogg_free(pTrack->m_StreamState.granule_vals);
		_ogg_free(pTrack->m_StreamState.body_data);
		//vorbis_comment_clear(&pTrack->m_VorbisComment);
		//delete pTrack->m_VorbisComment;
		//pTrack->m_StreamHeader.clear();

		delete pTrack;
	}
}
int MyOGM::GetNumberOfTracks(){
	return (int)g_Tracks.size();
}
float MyOGM::GetDuration(){
	if(GetNumberOfTracks()>0)
		return GetTrack(0)->m_iDurationUS/1000000.0f;
	return 0;
}
OggTrack *MyOGM::GetTrack(int nr) {
	std::list<OggTrack *>::iterator trackIt;
	int itr = 0;
	for(trackIt=g_Tracks.begin() ; trackIt!=g_Tracks.end() ; trackIt++) {
		OggTrack *pTrack = (*trackIt);
		if(itr==nr)
			return pTrack;
		itr++;
	}
	return NULL;
}/*+
void SetLogFile(IOStream *pOutput) {
	if(g_pLog) {
		g_ToolSystem.setPrintFunction(DefaultPrint, NULL);
		delete g_pLog;
		g_pLog = NULL;
	}
	g_pLog = pOutput;
	if(g_pLog && g_pLog->IsOpened()) {
		g_pLog->setOutputSystem(true);
		//if(g_bUseUnicodeWinAPI) {
		//	WriteBOM(*g_pLog);
		//	g_ToolSystem.setPrintFunction(IOStreamPrintW, g_pLog);
		//} else
			g_ToolSystem.setPrintFunction(IOStreamPrintA, g_pLog);
		//+DisplayCopyright(g_ToolSystem);
	} else if(g_pLog) {
		delete g_pLog;
		g_pLog = NULL;
	}
}+*/
/*+int main(int argc, char **argv) {
	setlocale(LC_ALL, ""); //".ACP"

	DisplayCopyright(g_ToolSystem);

	IOFile input(g_ToolSystem);

	if(ProcessCommandLine(input, argc, argv)) {
		if(g_pLogFile) {
			IOFile *pFileOutput = new IOFile(g_ToolSystem, g_pLogFile, nsFile::kWrite | nsFile::kCreateAlways | nsFile::kSequential);
			SetLogFile(pFileOutput);
		}
		ProcessInput(input);
	} else {
		DisplaySyntax(argv[0]);
	}

	DumpTracks();

	FreeGlobalVars();

	return 0;
}+*/
/*+
void DisplayCopyright(ToolSystem& toolSystem) {
	toolSystem.PrintMessage(kMsg,
		L"OGMInfo v%ls - an OGM (Ogg based multimedia files) information tool\n"	\
		L"Copyright (C) 2002-2004  Cyrius\n\n"
		, OGMINFO_VERSION);
}+*/
/*+bool ProcessCommandLine(IOFile& input, int iNbArgs, char **pArgs) {
	bool bResult = false;
	if(iNbArgs > 1) {
		int iArg = 0;
		if(iNbArgs == 2) {
			g_bShowProgress = true;
		}
		while((++iArg) < iNbArgs) {
			if(!strcasecmp(pArgs[iArg], "--help") || !strcasecmp(pArgs[iArg], "-h")) {
				DisplaySyntax(pArgs[0]);
			} else if(!strcasecmp(pArgs[iArg], "--verbose")) {
				g_iVerbosity++;
			} else if(!strncasecmp(pArgs[iArg], "-v", 2)) {
				const char *szChar = pArgs[iArg];
				while(*++szChar) {
					if((*szChar) == 'v')
						g_iVerbosity++;
				}
			} else if(!strcasecmp(pArgs[iArg], "--progress") || !strcasecmp(pArgs[iArg], "-p")) {
				g_bShowProgress = true;
			} else if(!strcasecmp(pArgs[iArg], "--headers")) {
				g_bHeadersOnly = true;
			} else if((!strcasecmp(pArgs[iArg], "--log") || !strcasecmp(pArgs[iArg], "-l")) && ((++iArg)<iNbArgs)) {
				g_pLogFile = pArgs[iArg];
			} else {
				input.Open(pArgs[iArg], nsFile::kRead | nsFile::kOpenExisting | nsFile::kSequential);
				bResult = input.IsOpened();
			}
		}
	}
		
	return bResult;
}+*/
/*+
void DisplaySyntax(const char *szExeName) {
	fprintf(stdout, "Syntax : %s [options] <file>\n", GetFilename(szExeName).c_str());
	fprintf(stdout, "\n");
	fprintf(stdout, "  <file>           Ogg/OGM file to process\n");
	fprintf(stdout, "\n");
	fprintf(stdout, "Options :\n");
	fprintf(stdout, "  --help           Show this help\n");
	fprintf(stdout, "  -l/--log <file>  Log to <file>\n");
	fprintf(stdout, "  --verbose        Increase the verbosity level\n");
	fprintf(stdout, "  -v[v[...]]       Increase the verbosity level (by the number of 'v')\n");
	fprintf(stdout, "                   Level 0 : (Default) print general stream information\n");
	fprintf(stdout, "                   Level 1 : + Page information\n");
	fprintf(stdout, "                   Level 2 : + Packet information (except content)\n");
	fprintf(stdout, "                   Level 3 : + Packet content\n");
	fprintf(stdout, "  -p/--progress    Show progress\n");
	fprintf(stdout, "  --headers        Process only the stream headers\n");
	fprintf(stdout, "\n");
}+*/
int MyOGM::GetNextPage(IOStream& input, ogg_sync_state *pSync, ogg_page *pPage) {
	int iPageSize, iBytesRead;
	char *pBuffer;
	do {
		iPageSize = ogg_sync_pageseek(pSync, pPage);
		if(iPageSize > 0) {
			break;
		} else if(iPageSize < 0) {
//-			g_ToolSystem.PrintMessage(kMsgWarning, L"ogg_sync_pageseek failed, %d bytes skipped\n", -iPageSize);
		} else if(iPageSize == 0) {
			pBuffer = ogg_sync_buffer(pSync, BLOCK_SIZE);
			if(!pBuffer) {
//-				g_ToolSystem.PrintMessage(kMsgError, L"ogg_sync_buffer failed.\n");
				break;
			}
			if((iBytesRead = input.Read(pBuffer, BLOCK_SIZE)) <= 0) {
				//+if(g_iVerbosity > 0)
				//+	g_ToolSystem.PrintMessage(kMsg, L"END OF FILE\n");
				break;
			}

			////////////////////////////
if(input.GetPosition()<100000 && vcodec.length()<1){//check only first 100 k
	int num=0;
	char * temp = pBuffer;
	//sint64 p = input->GetPosition();
	int ssz = iBytesRead;//input->Read(temp, 1024);
	//input->Seek(p, SEEK_SET);
	char cstr[1024];
	int cpos = 0;
	int lpos;
	for(int x=0; x<ssz-4;){
		if(temp[x]==0 && temp[x+1]==0 && temp[x+2]==1 && temp[x+3]==0xFFFFFFB2){
			int y = x + 4;
			lpos = cpos;
			if(num>0) cstr[cpos++] = ' ';
			num++;
			for(; y<ssz-1; y++){
				if(temp[y]==0 && temp[y+1]==0) break;
				if(temp[y]<32 || temp[y]>126){
					if(cpos-lpos<3){
						cpos = lpos;
						num--;
					}
					break;
				}
				cstr[cpos++] = (char)temp[y];
			}
			if(temp[y]>31 && temp[y]<127)
				cstr[cpos++] = (char)temp[y];
			x = y;
		} else x++;
	}
	/*printf("CPOS: %d\n", cpos);
	for(size_t x=0; x<cpos/16; x++){
		printf("%06d ", x*16);
		for(size_t y=0; y<16; y++)
			printf("%c", cstr[x*16+y]);
		printf("\n");
	}*/
	if(cpos>0){
		cstr[cpos] = '\0';
		vcodec = cstr;
		//printf("%s @ %d\n", cstr, input.GetPosition());
	}
}
///////////////////////

			ogg_sync_wrote(pSync, iBytesRead);
		}
	} while(true);
	/*for(int z=0; z<iPageSize; z++){
		if(z>0&&z%16==0) printf("\n");
		printf("%02x ", pBuffer[z]);
	}
	printf("\n");
	for(int z=0; z<iPageSize; z++){
		if(z>0&&z%32==0) printf("\n");
		printf("%c", pBuffer[z]);
	}
	printf("\n\n");
	Sleep(1000);*/
	//exit(0);
	return iPageSize;
}
/*+
void MyOGM::ProcessInput(IOFile& input) {
	sint64 iPagesRead = 0;
	sint64 iPagesReadProgress = 0;
	if(g_bShowProgress) {
		// With an average of 4kb / Page, estimate the number of
		// Pages to read that represent 1% of the file
		iPagesReadProgress = input.GetSize() / (100 * 4096);
		if(iPagesReadProgress <= 0)
			iPagesReadProgress = 1;
	}

	ogg_sync_state sync;
	ogg_page       page;

	ogg_sync_init(&sync);
	while(GetNextPage(input, &sync, &page) > 0) {
		if(g_bShowProgress && (iPagesRead % iPagesReadProgress == 0))
			fprintf(stderr, "Progress : %6.2f%%\r", (100. * double(input.GetPosition())) / double(input.GetSize()));

		if(ogg_page_bos(&page))
			ProcessPage(page, true);
		else {
			if(g_bHeadersOnly)
				break;
			ProcessPage(page, false);
		}
		
		iPagesRead++;
	}
	ogg_sync_clear(&sync);
}+*/
/*+
void AppendTrackNumber(OggTrack *pTrack, std::wstring& strInfo) {
	strInfo += g_ToolSystem.FormatString(L"%3d", pTrack->m_iIndex);
	switch(pTrack->m_iType & kTrackTypeMask) {
	case kTrackUnknown:
		strInfo += L'U';
		break;
	case kTrackOld:
		strInfo += L'O';
		break;
	case kTrackVideo:
		strInfo += L'V';
		break;
	case kTrackAudio:
		if((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis)
			strInfo += L'v';
		else
			strInfo += L'A';
		break;
	case kTrackText:
		strInfo += L'T';
		break;
	default:
		strInfo += L'I';
		break;
	}
}+*/
void MyOGM::ProcessPage(ogg_page& page, bool bBOS) {
	int iSerial = ogg_page_serialno(&page);
	OggTrack *pTrack = GetTrack(iSerial, bBOS);

	if(!pTrack) {
//-		g_ToolSystem.PrintMessage(kMsgWarning, L"Couldn't retrieve entry for stream %d.\n", iSerial);
		return;
	}

	pTrack->m_iPagesProcessed++;
	int iNbPackets = ogg_page_packets(&page);
	pTrack->m_iPackets += iNbPackets;
	pTrack->m_iBytes_PageHeader += page.header_len;
	pTrack->m_iBytes_PageBody += page.body_len;
	int iPage = ogg_page_pageno(&page);

	std::wstring strPageInfo_1;
	std::wstring strPageInfo_2;
	std::wstring strPageInfo_3;

/*	if(g_iVerbosity > 0) {
		AppendTrackNumber(pTrack, strPageInfo_1);
		strPageInfo_2 += g_ToolSystem.FormatString(L" Page%8d (%02X)%3d+%5d b (%ls,%ls,%3d p)"
			, iPage, ogg_page_version(&page), page.header_len, page.body_len
			, (bBOS ? L"INIT" : ogg_page_eos(&page) ? L"LAST" : L"    ")
			, (ogg_page_continued(&page) ? L" CONT": L"!CONT")
			, iNbPackets
			);
		ProcessPage_Position(page, pTrack, strPageInfo_3);
		strPageInfo_3 += L'\n';
	}*/

	if(bBOS) {
		if(iPage != 0) {
			// First Page isn't Page 0 ?
//-			g_ToolSystem.PrintMessage(kMsgWarning, L"Stream %d starts at Page %d.\n", iSerial, iPage);
		}
	} else {
		if(pTrack->m_iLastPage + 1 < iPage) {
			// Pages skipped
//-			g_ToolSystem.PrintMessage(kMsgWarning, L"%d Pages skipped.\n", iPage - pTrack->m_iLastPage - 1);
			pTrack->m_iPagesSkipped += iPage - pTrack->m_iLastPage - 1;
		} /*-else if(iPage < pTrack->m_iLastPage) {
			// Pages out-of-order ?
			g_ToolSystem.PrintMessage(kMsgWarning, L"Page %d preceeds Page %d.\n"
				, pTrack->m_iLastPage, iPage);
		} else if(iPage == pTrack->m_iLastPage) {
			// Duplicated Page ?
			g_ToolSystem.PrintMessage(kMsgWarning, L"Page %d found again.\n", iPage);
		}-*/
	}
	pTrack->m_iLastPage = iPage;

	if(ogg_stream_pagein(&pTrack->m_StreamState, &page) != 0) {
//-		g_ToolSystem.PrintMessage(kMsgWarning, L"ogg_stream_pagein failed for page %d of stream %d\n", iPage, iSerial);
	} else {
		ogg_packet	packet;

		int iResult = 0;
		int iPacketInPage = 0;
		bool bHeader;
		while((iResult = ogg_stream_packetout(&pTrack->m_StreamState, &packet)) != 0) {
			if(iResult == -1) {
				// Generally this means there is a gap (we are out-of-sync)
				/*if(g_iVerbosity > 0) {
					g_ToolSystem.PrintMessage(kMsgWarning, L"ogg_stream_packetout returned -1 for Page %d (Packet %d) of stream %d.\n"
						, iPage, iPacketInPage, iSerial);
				}*/
				// The -1 code is to warn the caller, the library didn't hand over a packet
				continue;
			}
			iPacketInPage++;
			bHeader = bBOS && (iPacketInPage==1);
			if(bHeader) {
				// First packet of the stream _should_ be the header
				if((packet.bytes >= 7) && !strncasecmp((const char *)packet.packet + 1, "vorbis", 6)) {
					// Vorbis stream
					pTrack->m_iType = kTrackVorbis;
					pTrack->InitializeGranulepos(false, true);
					pTrack->InitializeStreamHeader(sizeof(stream_header));
					pTrack->m_pStreamHeader->time_unit = _LL(10000000);
					if(vorbis_synthesis_headerin(&pTrack->m_VorbisInfo, &pTrack->m_VorbisComment, &packet) < 0) {
//-						g_ToolSystem.PrintMessage(kMsgWarning, L"vorbis_synthesis_headerin for Vorbis track.\n");
						// Keep the track in our list to prevent future warning messages
						pTrack->m_iType |= kTrackInvalid;
					} else {
						pTrack->m_pStreamHeader->samples_per_unit = pTrack->m_VorbisInfo.rate;
						pTrack->m_pStreamHeader->default_len = 1;
						pTrack->m_pStreamHeader->buffersize = -1;
						pTrack->m_pStreamHeader->bits_per_sample = pTrack->m_VorbisInfo.bitrate_nominal / pTrack->m_VorbisInfo.rate;
						pTrack->m_pStreamHeader->sh.audio.channels = pTrack->m_VorbisInfo.channels;
						pTrack->m_pStreamHeader->sh.audio.blockalign = (pTrack->m_VorbisInfo.bitrate_nominal * pTrack->m_VorbisInfo.channels) / (8 * pTrack->m_VorbisInfo.rate);
						pTrack->m_pStreamHeader->sh.audio.avgbytespersec = pTrack->m_VorbisInfo.bitrate_nominal / 8;
						pTrack->m_Samplerate = Fraction(pTrack->m_VorbisInfo.rate, 1);
					}
					g_iTracks_Audio++;
				} else if( (packet.bytes >= 142)
			    && !strncasecmp((const char *)packet.packet + 1,"Direct Show Samples embedded in Ogg", 35) ) {
					// Old header, not supported :(
//-					g_ToolSystem.PrintMessage(kMsgWarning, L"Stream %d uses an old (unsupported) stream structure.\n"
//-						, iSerial);
					// Keep the track in our list, in order to prevent future warning messages
					pTrack->m_iType = kTrackOld;
					g_iTracks_Old++;
				} else if(((*packet.packet & PACKET_TYPE_BITS ) == PACKET_TYPE_HEADER)
				&& (packet.bytes >= sizeof(stream_header) + 1) ) {
					pTrack->InitializeStreamHeader(packet.bytes - 1, packet.packet + 1);
					FIX_ENDIAN_FROM_OGM_STREAMHEADER(pTrack->m_pStreamHeader);
					pTrack->m_bHeader = true;

					if(pTrack->m_pStreamHeader->time_unit)
						pTrack->m_Samplerate = Fraction::reduce(_LL(10000000) * pTrack->m_pStreamHeader->samples_per_unit, pTrack->m_pStreamHeader->time_unit);
					// What type of stream ?
					if(!strncasecmp((const char *)packet.packet + 1, "audio", 5)) {
						pTrack->m_iType = kTrackAudio;
						FIX_ENDIAN_FROM_OGM_STREAMHEADER_AUDIO(&pTrack->m_pStreamHeader->sh.audio);
						pTrack->InitializeGranulepos(false, true);
						g_iTracks_Audio++;
					} else if(!strncasecmp((const char *)packet.packet + 1, "video", 5)) {
						pTrack->m_iType = kTrackVideo;
						FIX_ENDIAN_FROM_OGM_STREAMHEADER_VIDEO(&pTrack->m_pStreamHeader->sh.video);
						pTrack->InitializeGranulepos(true, true);
						g_iTracks_Video++;
					} else if(!strncasecmp((const char *)packet.packet + 1, "text", 4)) {
						pTrack->m_iType = kTrackText;
						pTrack->InitializeGranulepos(true, false);
						g_iTracks_Text++;
					} else {
						pTrack->m_iType = kTrackUnknown;
						pTrack->InitializeGranulepos(false, true);
						g_iTracks_Unknown++;
					}
				} else {
//-					g_ToolSystem.PrintMessage(kMsgWarning, L"Stream %d doesn't begin with a valid header.\n"
//-						, iSerial);
					// Keep the track in our list to prevent future warning messages
					pTrack->m_iType |= kTrackInvalid;
					g_iTracks_Unknown++;
				}

				/*if(g_iVerbosity > 0) {
					strPageInfo_1 = L"";
					AppendTrackNumber(pTrack, strPageInfo_1);
					strPageInfo_3 = L"";
					ProcessPage_Position(page, pTrack, strPageInfo_3);
					strPageInfo_3 += L'\n';
				}*/
			}
			if(pTrack->m_iLastPacket + 1 < packet.packetno) {
				// Packets skipped
//-				g_ToolSystem.PrintMessage(kMsgWarning, L"%lld packets skipped.\n", packet.packetno - pTrack->m_iLastPacket - 1);
				pTrack->m_iPacketsSkipped += packet.packetno - pTrack->m_iLastPacket - 1;
			}
			// Packet number is 'rebuilt' by the library, no need to verify the other
			// possible problems of ordering
			pTrack->m_iLastPacket = packet.packetno;
			pTrack->m_iPacketsProcessed++;
			pTrack->m_iBytes_Packets += packet.bytes;

			ProcessPacket(packet, pTrack, strPageInfo_3, bHeader);
		}
	}

	/*if(g_iVerbosity > 0) {
		strPageInfo_1 += strPageInfo_2;
		strPageInfo_1 += strPageInfo_3;
		g_ToolSystem.PrintMessage(kMsg, L"%ls", strPageInfo_1.c_str());
	}*/
}

void MyOGM::ProcessPage_Position(ogg_page& page, OggTrack *pTrack, std::wstring& strInfo) {
	sint64 iPageGranulepos = ogg_page_granulepos(&page);

	//+if(g_iVerbosity > 0)
	//+	strInfo += g_ToolSystem.FormatString(L", pos %10lld", iPageGranulepos);

	if(pTrack->m_Samplerate.round64r() > 0) {
		sint64 iTimeEndUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (iPageGranulepos + pTrack->m_iGranuleposZeroOffset));

		sint64 iTimeStartUS = 0;
		if(pTrack->m_bGranuleposRepresentsEnd)
			iTimeStartUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (pTrack->m_iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset));
		else
			iTimeStartUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (iPageGranulepos + pTrack->m_iGranuleposZeroOffset));

/*		if(g_iVerbosity > 0) {
			sint32 iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros;
			sint32 iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros;
			MicrosToTime(iTimeStartUS, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros);
			MicrosToTime(iTimeEndUS, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros);

			if(pTrack->m_bGranuleposRepresentsEnd) {
				strInfo += g_ToolSystem.FormatString(L", %02d:%02d:%02d.%06d, %02d:%02d:%02d.%06d %ls"
					, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros
					, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros
					, ((iTimeStartUS < g_iLastTimeStartUS) ? L"!SYNC" : L" SYNC")
					);
			} else {
				strInfo += g_ToolSystem.FormatString(L", %02d:%02d:%02d.%06d,                 %ls"
					, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros
					, ((iTimeStartUS < g_iLastTimeStartUS) ? L"!SYNC" : L" SYNC")
					);
			}
		}*/

		if(iTimeEndUS > pTrack->m_iDurationUS)
			pTrack->m_iDurationUS = iTimeEndUS;
		g_iLastTimeStartUS = iTimeStartUS;
	}
}
void MyOGM::ProcessPacket(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo, bool bHeader) {
	assert(pTrack);

	if(packet.e_o_s) {
/*+		if(pTrack->m_bEOSReached) {
			// EOS already reached :/
			g_ToolSystem.PrintMessage(kMsgWarning, L"EOS reached again for stream %d.\n"
				, pTrack->m_iSerial);
		}+*/

		pTrack->m_bEOSReached = true;
	}

/*+	if(g_iVerbosity > 1) {
		AppendTrackNumber(pTrack, strInfo);
		strInfo += L' ';
	}+*/

/*+	if(bHeader) {
		if(g_iVerbosity > 1)
			strInfo += L'H';
		ProcessPacket_Generic(packet, pTrack, strInfo, true);
		ProcessPacket_Header(packet, pTrack, strInfo);
	} else if(packet.bytes <= 0) {
		if(g_iVerbosity>1)
			strInfo += L' ';
		ProcessPacket_Generic(packet, pTrack, strInfo, false);
		if(g_iVerbosity > 2)
			strInfo += L", void Packet";
	} else+*/ if((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis) {
//+		if(g_iVerbosity > 1)
//+			strInfo += L'v';
//+		ProcessPacket_Generic(packet, pTrack, strInfo, false);
		ProcessPacket_Vorbis(packet, pTrack, strInfo);
		if((packet.packetno==1) && ((*packet.packet & PACKET_TYPE_BITS) == PACKET_TYPE_COMMENT))
			ProcessPacket_Comment(packet, pTrack, strInfo);
	} else if((*packet.packet & PACKET_TYPE_BITS) == PACKET_TYPE_COMMENT) {
//+		if(g_iVerbosity > 1)
//+			strInfo += L'C';
//+		ProcessPacket_Generic(packet, pTrack, strInfo, false);
		ProcessPacket_Comment(packet, pTrack, strInfo);
	} else if((*packet.packet & 0x01 ) == PACKET_TYPE_DATA) {
//+		if(g_iVerbosity > 1)
//+			strInfo += L'D';
//+		ProcessPacket_Generic(packet, pTrack, strInfo, false);
		ProcessPacket_Data(packet, pTrack, strInfo);
	} 
//+	else {
//+		if(g_iVerbosity > 1)
//+			strInfo += L'?';
//+		ProcessPacket_Generic(packet, pTrack, strInfo, false);
//+	}

//+	if(g_iVerbosity > 1)
//+		strInfo += L'\n';
}

/*+void MyOGM::ProcessPacket_Generic(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo, bool bHeader) {
	if(g_iVerbosity < 2)
		return;

	strInfo += g_ToolSystem.FormatString(L" Packet%8lld :%8d b"
		, packet.packetno, packet.bytes
		);
}+*/

/*+void MyOGM::ProcessPacket_Header(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo) {
	if(g_iVerbosity < 3)
		return;

	strInfo += L'\n';
	AppendTrackNumber(pTrack, strInfo);
	strInfo += L" H ";

	if((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis) {
		pTrack->m_iBytes_Data += packet.bytes;
		strInfo += g_ToolSystem.FormatString(L"Vorbis Header : v%d, %d/%d/%d bps (window: %d), %d channels, %ld Hz"
			, pTrack->m_VorbisInfo.version
			, pTrack->m_VorbisInfo.bitrate_lower, pTrack->m_VorbisInfo.bitrate_nominal, pTrack->m_VorbisInfo.bitrate_upper
			, pTrack->m_VorbisInfo.bitrate_window
			, pTrack->m_VorbisInfo.channels, pTrack->m_VorbisInfo.rate
			);
	} else {
		switch(pTrack->m_iType & kTrackTypeMask) {
		case kTrackVideo:
			{
				strInfo += g_ToolSystem.FormatString(L"Video Header : [%-4s], %dx%d, %.3f fps"
					, TextAToW(pTrack->m_pStreamHeader->subtype, 4).c_str()
					, pTrack->m_pStreamHeader->sh.video.width, pTrack->m_pStreamHeader->sh.video.height
					, double(pTrack->m_Samplerate)
					);
			}
			break;
		case kTrackAudio:
			{
				std::string codec(pTrack->m_pStreamHeader->subtype, 4);
				uint32 wFormatTag = strtoul(codec.c_str(), NULL, 16);
				std::wstring codecName(L"");
				switch(wFormatTag) {
				case 0x0001:	//WAVE_FORMAT_PCM
					codecName = L" (PCM)";
					break;
				case 0x0050:
					codecName = L" (Mpeg Audio Layer-1/2)";
					break;
				case 0x0055:
					codecName = L" (Mpeg Audio Layer-3)";
					break;
				case 0x2000:
					codecName = L" (ATSC/A-52 - Dolby AC3)";
					break;
				case 0x2001:
					codecName = L" (DTS)";
					break;
				case 0xFFFE:	//WAVE_FORMAT_EXTENSIBLE
					codecName = L" (Extensible)";
					break;
				}
				if(!(pTrack->m_Samplerate.getHi() % pTrack->m_Samplerate.getLo())) {
					strInfo += g_ToolSystem.FormatString(L"Audio Header : [%-4s]%ls, %d bps, %d channels, %lld Hz"
						, TextAToW(codec).c_str(), codecName.c_str()
						, 8 * pTrack->m_pStreamHeader->sh.audio.avgbytespersec
						, pTrack->m_pStreamHeader->sh.audio.channels
						, pTrack->m_Samplerate.round64r());
				} else {
					strInfo += g_ToolSystem.FormatString(L"Audio Header : [%-4s]%ls, %d bps, %d channels, %.3f Hz"
						, TextAToW(codec).c_str(), codecName.c_str()
						, 8 * pTrack->m_pStreamHeader->sh.audio.avgbytespersec
						, pTrack->m_pStreamHeader->sh.audio.channels
						, double(pTrack->m_Samplerate));
				}
			}
			break;
		case kTrackText:
			strInfo += L"Text Header";
			break;
		case kTrackOld:
			strInfo += L"Old Stream Header";
			break;
		case kTrackUnknown:
		default:
			strInfo += L"Unknown Header";
			break;
		}
	}
	if(pTrack->m_iType & kTrackInvalid)
		strInfo += L" (!!INVALID!!)";
}+*/
void MyOGM::ProcessPacket_Comment(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo) {
//+	if(g_iVerbosity > 2)
//+		strInfo += L", Comment Packet";

	// Let's fool around with libvorbis :>
	vorbis_info vi;
	vorbis_info_init(&vi);
	vi.channels = 2;
	vi.rate = 48000;
	vorbis_comment_clear(&pTrack->m_VorbisComment);
	if(vorbis_synthesis_headerin(&vi, &pTrack->m_VorbisComment, &packet) != 0) {
//-		g_ToolSystem.PrintMessage(kMsgWarning, L"vorbis_synthesis_headerin failed on comment Packet for stream %d.\n", pTrack->m_iSerial);
		vorbis_info_clear(&vi);
		return;
	}

/*+	if(g_iVerbosity < 3)
		return;

	strInfo += g_ToolSystem.FormatString(L"\nVendor     : [%ls]", TextAToW(pTrack->m_VorbisComment.vendor).c_str());
	int i;
	for(i=0 ; i<pTrack->m_VorbisComment.comments ; i++) {
		bool bValid;
		std::wstring strComment = TextU8ToW(pTrack->m_VorbisComment.user_comments[i], bValid);
		if(!bValid)
			strComment = TextAToW(pTrack->m_VorbisComment.user_comments[i]);
		strInfo += g_ToolSystem.FormatString(L"\nComment %2d : [%ls]", i+1, strComment.c_str());
	}
	vorbis_info_clear(&vi);+*/
}

void MyOGM::ProcessPacket_Data(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo) {
	int iSamplesEncoding, iCount;
	sint64 iSamples = pTrack->m_pStreamHeader->default_len;

	iCount = iSamplesEncoding = ((*packet.packet & PACKET_LEN_BITS01) >> 6)
		| ((*packet.packet & PACKET_LEN_BITS2) << 1);

	if(iSamplesEncoding) {
		iSamples = 0;
		uint8 *pSamplesEncoding = (uint8 *)packet.packet + iSamplesEncoding;
		while(iCount--) {
			iSamples <<= 8;
			iSamples += *pSamplesEncoding;
			pSamplesEncoding--;
		}
	}

	sint64 iRealSamples = (iSamples == 0 ? 1 : iSamples);
	pTrack->m_iPackets_Data++;
	pTrack->m_iBytes_Data += packet.bytes - iSamplesEncoding - 1;
	pTrack->m_iSamplesProcessed += iRealSamples;

/*+	if(g_iVerbosity > 2) {
		strInfo += g_ToolSystem.FormatString(L", %ls,%7lld samp"
			, ((*packet.packet & PACKET_IS_SYNCPOINT) ? L" KEY" : L"!KEY")
			, iSamples
			);
	}+*/

	sint64 iCurrentGranulepos = pTrack->m_iCurrentGranulepos;
	// Determines the granulepos of this packet (i.e. position of the last sample)
	// update this value according to the packet granulepos when possible
	if(packet.granulepos != -1) {
		if(pTrack->m_bGranuleposRepresentsEnd)
			pTrack->m_iCurrentGranulepos = packet.granulepos;
		else
			pTrack->m_iCurrentGranulepos = packet.granulepos + iRealSamples;
	} else
		pTrack->m_iCurrentGranulepos += iRealSamples;

	if(pTrack->m_Samplerate.round64r() > 0) {
		sint64 iTimeEndUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset + iRealSamples));

		if(iTimeEndUS > pTrack->m_iDurationUS)
			pTrack->m_iDurationUS = iTimeEndUS;
		

/*+		if(g_iVerbosity > 2) {
			sint64 iTimeStartUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset));
			sint32 iBegin_Hour, iBegin_Minute, iBegin_Second, iBegin_Micros;
			sint32 iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros;
			MicrosToTime(iTimeStartUS, iBegin_Hour, iBegin_Minute, iBegin_Second, iBegin_Micros);
			MicrosToTime(iTimeEndUS, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros);

			if(packet.granulepos != -1) {
				strInfo += g_ToolSystem.FormatString(L", pos %10lld, %02d:%02d:%02d.%06d->%02d:%02d:%02d.%06d"
					, packet.granulepos
					, iBegin_Hour, iBegin_Minute, iBegin_Second, iBegin_Micros
					, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros
					);
			} else {
				strInfo += g_ToolSystem.FormatString(L", POS %10lld, %02d:%02d:%02d.%06d->%02d:%02d:%02d.%06d"
					, pTrack->m_iCurrentGranulepos
					, iBegin_Hour, iBegin_Minute, iBegin_Second, iBegin_Micros
					, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros
					);
			}
		}+*/

/*+		if(((pTrack->m_iType & kTrackTypeMask) == kTrackText) && (g_iVerbosity > 2)) {
			strInfo += L'\n';
			// TextAToW also include the 0x00 chars in the translated string (happens only
			// if we give the number of chars to translate and when one of those chars is 0x00)
			// the + operator still keep those, so better concatenate with a string pointer which
			// will get rid of those
			strInfo += TextAToW((char *)packet.packet + iSamplesEncoding + 1, packet.bytes - iSamplesEncoding - 1).c_str();
		}+*/
	}
}

long MyOGM::GetBlocksize(ogg_packet& packet, OggTrack *pTrack) {
	int iThisW = vorbis_packet_blocksize(&pTrack->m_VorbisInfo, &packet);
	// Found in vcut (part of vorbis-tools : see www.vorbis.org)
	int iRet = (iThisW + pTrack->m_iPrevW) / 4;

	pTrack->m_iPrevW = iThisW;
	return iRet;
}

void MyOGM::ProcessPacket_Vorbis(ogg_packet& packet, OggTrack *pTrack, std::wstring& strInfo) {
	if(pTrack->m_iType & kTrackInvalid)
		return;
	
	pTrack->m_iBytes_Data += packet.bytes;

//	printf("\n %d -\n", packet.packetno);
//	printf("\n %d: %d\n", pTrack->m_iSerial, pTrack->m_VorbisInfo.rate);

	// First Packet = Header
	// Second Packet = Comments
	// Third Packet = Setup
	if(packet.packetno < 3 /*&& pTrack->m_VorbisInfo.rate<1*/) {
		if(vorbis_synthesis_headerin(&pTrack->m_VorbisInfo, &pTrack->m_VorbisComment, &packet) != 0) {
//-			g_ToolSystem.PrintMessage(kMsgWarning, L"vorbis_synthesis_headerin failed for vorbis packet %lld of stream %d.\n"
//-				, packet.packetno, pTrack->m_iSerial);
			//pTrack->m_iType |= kTrackInvalid;
		}
		return;
	}

	pTrack->m_iPackets_Data++;
	long iRealSamples = GetBlocksize(packet, pTrack);
	pTrack->m_iSamplesProcessed += iRealSamples;

/*+	if(g_iVerbosity > 2) {
		strInfo += g_ToolSystem.FormatString(L",      %7ld samp"
			, iRealSamples
			);
	}+*/
	
	sint64 iTimeStartUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (pTrack->m_iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset));
	sint64 iTimeEndUS = pTrack->m_Samplerate.scale64ir(_LL(1000000) * (pTrack->m_iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset + iRealSamples));

	if(iTimeEndUS > pTrack->m_iDurationUS)
		pTrack->m_iDurationUS = iTimeEndUS;

	sint32 iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros;
	sint32 iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros;
	MicrosToTime(iTimeStartUS, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros);
	MicrosToTime(iTimeEndUS, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros);

	// Determines the granulepos of this packet (i.e. position of the last sample)
	// update this value according to the packet granulepos when possible
	if(packet.granulepos != -1)
		pTrack->m_iCurrentGranulepos = packet.granulepos;
	else
		pTrack->m_iCurrentGranulepos += iRealSamples;

/*+	if(g_iVerbosity > 2) {
		if(packet.granulepos != -1) {
			strInfo += g_ToolSystem.FormatString(L", pos %10lld, %02d:%02d:%02d.%06d->%02d:%02d:%02d.%06d"
				, packet.granulepos
				, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros
				, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros
				);
		} else {
			strInfo += g_ToolSystem.FormatString(L", POS %10lld, %02d:%02d:%02d.%06d->%02d:%02d:%02d.%06d"
				, pTrack->m_iCurrentGranulepos
				, iStart_Hour, iStart_Minute, iStart_Second, iStart_Micros
				, iEnd_Hour, iEnd_Minute, iEnd_Second, iEnd_Micros
				);
		}
	}+*/
}

//////////////////////////////////////////////////////////////////////
/*
void DumpTracks(void) {
	sint64 iBytes_PageHeader = 0;
	sint64 iBytes_PageBody = 0;
	sint64 iBytes_PageTotal = 0;
	std::wstring strInfo;

	strInfo += g_ToolSystem.FormatString(L"\n\n%d streams : %d video, %d audio, %d text, %d old, %d unknown\n\n"
		, g_iTracks_Video+g_iTracks_Audio+g_iTracks_Text+g_iTracks_Old+g_iTracks_Unknown
		, g_iTracks_Video, g_iTracks_Audio, g_iTracks_Text, g_iTracks_Old, g_iTracks_Unknown
		);

	std::list<OggTrack *>::iterator trackIt;
	for(trackIt=g_Tracks.begin() ; trackIt!=g_Tracks.end() ; trackIt++) {
		OggTrack *pTrack = (*trackIt);
		
		iBytes_PageHeader += pTrack->m_iBytes_PageHeader;
		iBytes_PageBody += pTrack->m_iBytes_PageBody;

		//if((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis) {
		//} else {
			switch(pTrack->m_iType & kTrackTypeMask) {
			case kTrackVideo:
				DumpTrack_Serial(pTrack, strInfo, L"VIDEO");
				DumpTrack_Video(pTrack, strInfo);
				break;
			case kTrackAudio:
				DumpTrack_Serial(pTrack, strInfo
					, (((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis) ? L"VORBIS" : L"AUDIO"));
				DumpTrack_Audio(pTrack, strInfo);
				break;
			case kTrackText:
				DumpTrack_Serial(pTrack, strInfo, L"TEXT");
				DumpTrack_Text(pTrack, strInfo);
				break;
			case kTrackOld:
				DumpTrack_Serial(pTrack, strInfo, L"OLD");
				DumpTrack_Unknown(pTrack, strInfo);
				break;
			case kTrackUnknown:
			default:
				DumpTrack_Serial(pTrack, strInfo, L"UNKNOWN");
				DumpTrack_Unknown(pTrack, strInfo);
				break;
			}
		//}
		DumpTrack_Statistic(pTrack, strInfo);
		strInfo += L'\n';
	}

	if(!g_bHeadersOnly) {
		iBytes_PageTotal = iBytes_PageHeader + iBytes_PageBody;
		strInfo += g_ToolSystem.FormatString(
			L"Total bytes : %lld (page headers) + %lld (page bodies) = %lld (%.2fMB)\n"
			, iBytes_PageHeader, iBytes_PageBody, iBytes_PageTotal, double(iBytes_PageTotal) / 1024. / 1024.
			);
		if(iBytes_PageTotal)
			strInfo += g_ToolSystem.FormatString(L"  %.3f%% for headers + %.3f%% for data\n"
				, 100. * double(iBytes_PageHeader) / double(iBytes_PageTotal)
				, 100. * double(iBytes_PageBody) / double(iBytes_PageTotal)
				);
	}

	g_ToolSystem.PrintMessage(kMsg, L"%ls", strInfo.c_str());
}

void DumpTrack_Serial(OggTrack *pTrack, std::wstring& strInfo, const wchar_t *szType) {
	strInfo += g_ToolSystem.FormatString(
		L"=== STREAM %5d (%3d %ls) %ls===\n"
		, pTrack->m_iSerial, pTrack->m_iIndex
		, szType
		, ((pTrack->m_iType & kTrackInvalid) ? L"!!Invalid!!" : L"")
		);
}

void DumpTrack_Video(OggTrack *pTrack, std::wstring& strInfo) {
	DumpTrack_Header(pTrack, strInfo);
	DumpTrack_Comments(pTrack, strInfo);
}

void DumpTrack_Audio(OggTrack *pTrack, std::wstring& strInfo) {
	DumpTrack_Header(pTrack, strInfo);
	if((pTrack->m_iType & kTrackSubtypeMask) != kTrackSubtypeVorbis) {
		uint32 wFormatTag = strtoul(std::string(pTrack->m_pStreamHeader->subtype, 4).c_str(), NULL, 16);
		switch(wFormatTag) {
		case 0x0001:	//WAVE_FORMAT_PCM
			strInfo += L"  Audio format : PCM\n";
			break;
		case 0x0050:
			strInfo += L"  Audio format : Mpeg Audio Layer-1/2\n";
			break;
		case 0x0055:
			strInfo += L"  Audio format : Mpeg Audio Layer-3\n";
			break;
		case 0x2000:
			strInfo += L"  Audio format : ATSC/A-52 (Dolby AC3)\n";
			break;
		case 0x2001:
			strInfo += L"  Audio format : DTS\n";
			break;
		case 0xFFFE:	//WAVE_FORMAT_EXTENSIBLE
			strInfo += L"  Audio format : Extensible\n";
			break;
		}
	}
	DumpTrack_Comments(pTrack, strInfo);
}

void DumpTrack_Text(OggTrack *pTrack, std::wstring& strInfo) {
	DumpTrack_Header(pTrack, strInfo);
	DumpTrack_Comments(pTrack, strInfo);
}

void DumpTrack_Unknown(OggTrack *pTrack, std::wstring& strInfo) {
	if(pTrack->m_bHeader)
		DumpTrack_Header(pTrack, strInfo);
	else
		strInfo += L"No known stream header found.\n";
	DumpTrack_Comments(pTrack, strInfo);
}

void DumpTrack_Header(OggTrack *pTrack, std::wstring& strInfo) {
	strInfo += L"Stream Header :\n";
	if(pTrack->m_bHeader) {
		strInfo += g_ToolSystem.FormatString(
			L"  Type : [%-8ls]\n"									\
			L"  SubType : [%-4ls]\n"								\
			L"  Size : %d\n"										\
			L"  Time Unit : %lld (in reference time)\n"				\
			L"  Samples per Unit : %lld\n"
			, TextAToW(pTrack->m_pStreamHeader->streamtype, 8).c_str()
			, TextAToW(pTrack->m_pStreamHeader->subtype, 4).c_str()
			, pTrack->m_pStreamHeader->size
			, pTrack->m_pStreamHeader->time_unit
			, pTrack->m_pStreamHeader->samples_per_unit);
		if(pTrack->m_Samplerate.round64t() > 0)
			strInfo += g_ToolSystem.FormatString(L"  Sample Rate : %.3f\n", double(pTrack->m_Samplerate));
		strInfo += g_ToolSystem.FormatString(
			L"  Default Len : %d (in media unit)\n"						\
			L"  Buffer Size : %d\n"										\
			L"  Bits per Sample : %d\n"
			, pTrack->m_pStreamHeader->default_len
			, pTrack->m_pStreamHeader->buffersize
			, (int)pTrack->m_pStreamHeader->bits_per_sample);
		switch(pTrack->m_iType & kTrackTypeMask) {
			case kTrackAudio:
				strInfo += g_ToolSystem.FormatString(
					L"  Number of Channels : %d\n"
					, (int)pTrack->m_pStreamHeader->sh.audio.channels);
				strInfo += g_ToolSystem.FormatString(
					L"  BlockAlign : %d\n"
					, (int)pTrack->m_pStreamHeader->sh.audio.blockalign);
				strInfo += g_ToolSystem.FormatString(
					L"  Average Bytes per Second : %d (%.1f kbps)\n"
					, pTrack->m_pStreamHeader->sh.audio.avgbytespersec, (double)8*(double)pTrack->m_pStreamHeader->sh.audio.avgbytespersec/(double)1000);
				break;
			case kTrackVideo:
				strInfo += g_ToolSystem.FormatString(
					L"  Width : %d\n"									\
					L"  Height : %d\n"
					, pTrack->m_pStreamHeader->sh.video.width
					, pTrack->m_pStreamHeader->sh.video.height);
				break;
			default:
				break;
		}
	}
	if((pTrack->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis) {
		strInfo += g_ToolSystem.FormatString(
			L"  Vorbis version : %d\n"									\
			L"  Lower bitrate : %d\n"									\
			L"  Nominal bitrate : %d\n"									\
			L"  Upper bitrate : %d\n"									\
			L"  Window : %d\n"											\
			L"  Channels : %d\n"										\
			L"  Samplerate : %ld Hz\n"
			, pTrack->m_VorbisInfo.version
			, pTrack->m_VorbisInfo.bitrate_lower, pTrack->m_VorbisInfo.bitrate_nominal, pTrack->m_VorbisInfo.bitrate_upper
			, pTrack->m_VorbisInfo.bitrate_window
			, pTrack->m_VorbisInfo.channels, pTrack->m_VorbisInfo.rate
			);
	}
}

void DumpTrack_Comments(OggTrack *pTrack, std::wstring& strInfo) {
	if(!pTrack->m_VorbisComment.comments && !pTrack->m_VorbisComment.vendor)
		return;

	strInfo += L"--=== COMMENTS ===--\n";
	strInfo += g_ToolSystem.FormatString(L"Vendor     : [%ls]\n", TextAToW(pTrack->m_VorbisComment.vendor).c_str());
	int i;
	for(i=0 ; i<pTrack->m_VorbisComment.comments ; i++) {
		bool bValid;
		std::wstring strComment = TextU8ToW(pTrack->m_VorbisComment.user_comments[i], bValid);
		if(!bValid)
			strComment = TextAToW(pTrack->m_VorbisComment.user_comments[i]);
		strInfo += g_ToolSystem.FormatString(L"Comment %2d : [%ls]\n", i+1, strComment.c_str());
	}
	strInfo += L"--=== COMMENTS ===--\n";
}

void DumpTrack_Statistic(OggTrack *pTrack, std::wstring& strInfo) {
	if(g_bHeadersOnly)
		return;

	sint64 iBytes_PageHeader = pTrack->m_iBytes_PageHeader;
	sint64 iBytes_PageBody = pTrack->m_iBytes_PageBody;
	sint64 iBytes_PageTotal = iBytes_PageHeader + iBytes_PageBody;
	sint64 iBytes_PacketTotal = pTrack->m_iBytes_Packets;
	sint64 iBytes_PacketData = pTrack->m_iBytes_Data;
	sint64 iBytes_PacketHeader = iBytes_PacketTotal - iBytes_PacketData;

	strInfo += g_ToolSystem.FormatString(
		L"End Of Stream reached : %ls\n"															\
		L"Pages : %d (%d skipped)\n"															\
		L"Packets : %lld announced / %lld found (%lld skipped), %lld containing data\n"		\
		L"Samples : %lld announced / %lld found\n"
		, (pTrack->m_bEOSReached ? L"yes" : L"no")
		, pTrack->m_iPagesProcessed, pTrack->m_iPagesSkipped
		, pTrack->m_iPackets,  pTrack->m_iPacketsProcessed, pTrack->m_iPacketsSkipped, pTrack->m_iPackets_Data
		, pTrack->m_iCurrentGranulepos + pTrack->m_iGranuleposZeroOffset, pTrack->m_iSamplesProcessed
		);

	if(pTrack->m_iPackets_Data)
		strInfo += g_ToolSystem.FormatString(L"Samples per Packet : %.1f\n"
			, double(pTrack->m_iSamplesProcessed) / double(pTrack->m_iPackets_Data)
			);
	if(pTrack->m_iPagesProcessed)
		strInfo += g_ToolSystem.FormatString(L"Packets per Page : %.1f\n"
			, double(pTrack->m_iPacketsProcessed) / double(pTrack->m_iPagesProcessed)
			);
	strInfo += g_ToolSystem.FormatString(
		L"Bytes : %lld (headers) + %lld (bodies) = %lld (%.2fMB)\n"
		, iBytes_PageHeader, iBytes_PageBody, iBytes_PageTotal, double(iBytes_PageTotal) / 1024. / 1024.
		);
	if(iBytes_PageTotal)
		strInfo += g_ToolSystem.FormatString(L"  %.3f%% for headers + %.3f%% for data\n"
			, 100. * double(iBytes_PageHeader) / double(iBytes_PageTotal)
			, 100. * double(iBytes_PageBody) / double(iBytes_PageTotal)
			);
	strInfo += g_ToolSystem.FormatString(L"Packet bytes : %lld (headers) + %lld (data) = %lld (%.2fMB)\n"
		, iBytes_PacketHeader, iBytes_PacketData, iBytes_PacketTotal, double(iBytes_PacketTotal) / 1024. / 1024.
		);
	if(iBytes_PacketTotal)
		strInfo += g_ToolSystem.FormatString(L"  %.3f%% for headers + %.3f%% for data\n"
			, 100. * double(iBytes_PacketHeader) / double(iBytes_PacketTotal)
			, 100. * double(iBytes_PacketData) / double(iBytes_PacketTotal)
			);
	if(pTrack->m_iSamplesProcessed && (pTrack->m_Samplerate.round64r()>0))
		strInfo += g_ToolSystem.FormatString(L"Estimated bitrate : %.1f kbps\n"
			, 8. * double(iBytes_PacketData) / double(pTrack->m_Samplerate.scale64ir(_LL(1000) * pTrack->m_iSamplesProcessed))
			);
	if(pTrack->m_iPagesProcessed)
		strInfo += g_ToolSystem.FormatString(L"Bytes per Page : %ld (header) + %ld (body) = %ld\n"
			, RoundToLong(double(iBytes_PageHeader) / double(pTrack->m_iPagesProcessed))
			, RoundToLong(double(iBytes_PageBody) / double(pTrack->m_iPagesProcessed))
			, RoundToLong(double(iBytes_PageTotal) / double(pTrack->m_iPagesProcessed))
			);
	if(pTrack->m_iPacketsProcessed)
		strInfo += g_ToolSystem.FormatString(L"Bytes per Packet : %ld\n"
			, RoundToLong(double(iBytes_PacketTotal) / double(pTrack->m_iPacketsProcessed))
			);
	sint32 iHour, iMinute, iSecond, iMicros;
	MicrosToTime(pTrack->m_iDurationUS, iHour, iMinute, iSecond, iMicros);
	strInfo += g_ToolSystem.FormatString(L"Duration : %02d:%02d:%02d.%06d\n"
		, iHour, iMinute, iSecond, iMicros);
}
*/
//////////////////////////////////////////////////////////////////////

OggTrack::OggTrack(int iSerial)
: m_iSerial(iSerial)
, m_iType(kTrackUnknown)
, m_iIndex(0)
, m_iLastPage(0)
, m_iPagesProcessed(0)
, m_iPagesSkipped(0)
, m_iPackets(0)
, m_iPacketsProcessed(0)
, m_iPackets_Data(0)
, m_iPacketsSkipped(0)
, m_iLastPacket(0)
, m_iSamplesProcessed(0)
// m_iCurrentGranulepos is the current granule position (end of the data processed so far)
// we consider a 1-based granule position (first sample would have granule position 1)
, m_iCurrentGranulepos(0)
, m_bGranuleposZeroBased(false)
, m_bGranuleposRepresentsEnd(true)
, m_iGranuleposZeroOffset(0)
, m_iDurationUS(0)
, m_iBytes_PageHeader(0)
, m_iBytes_PageBody(0)
, m_iBytes_Packets(0)
, m_iBytes_Data(0)
, m_bEOSReached(false)
, m_bHeader(false)
, m_Samplerate(sint64(0))
, m_iPrevW(0)
{
	vorbis_comment_init(&m_VorbisComment);
	vorbis_info_init(&m_VorbisInfo);
	InitializeStreamHeader(sizeof(stream_header));
}

OggTrack::~OggTrack(void) {
	vorbis_comment_clear(&m_VorbisComment);
	vorbis_info_clear(&m_VorbisInfo);
}

void OggTrack::InitializeGranulepos(bool bGranuleposZeroBased, bool bGranuleposRepresentsEnd) {
	m_bGranuleposZeroBased = bGranuleposZeroBased;
	m_bGranuleposRepresentsEnd = bGranuleposRepresentsEnd;

	// Video granule position is based on 0 (first frame would have granule position 0)
	// Audio granule position is based on 1 (as per Ogg Vorbis specs)
	// Text granule position is based on 0, but its granule position represents the start
	// of the data (and not the end, as per Ogg specs)
	m_iCurrentGranulepos = 0;

	if(bGranuleposZeroBased)
		m_iCurrentGranulepos--;
	if(!bGranuleposRepresentsEnd)
		m_iCurrentGranulepos++;

	m_iGranuleposZeroOffset = -m_iCurrentGranulepos;
}

bool OggTrack::InitializeStreamState(void) {
	ogg_stream_reset(&m_StreamState);
	if(ogg_stream_init(&m_StreamState, m_iSerial) != 0) {
//-		g_ToolSystem.PrintMessage(kMsgError, L"ogg_stream_init failed for stream %d\n", m_iSerial);
		return false;
	}
	return true;
}

void OggTrack::InitializeStreamHeader(int iSize, void *pHeader) {
	assert(iSize >= sizeof(stream_header));

	int iRealSize = iSize;
	if(iSize < sizeof(stream_header))
		iSize = sizeof(stream_header);

	m_StreamHeader.resize(iSize);

	assert(m_StreamHeader.size() == iSize);

	m_pStreamHeader = (stream_header *)&m_StreamHeader.front();

	memset(m_pStreamHeader, 0, m_StreamHeader.size());

	if(pHeader)
		memcpy(m_pStreamHeader, pHeader, std::min<int>(iRealSize, m_StreamHeader.size()));
}
OggTrack *MyOGM::GetTrack(int iSerial, bool bCreate) {
	std::list<OggTrack *>::iterator trackIt;
	for(trackIt=g_Tracks.begin() ; trackIt!=g_Tracks.end() ; trackIt++) {
		OggTrack *pTrack = (*trackIt);
		if(pTrack->m_iSerial == iSerial)
			return pTrack;
	}
	if(bCreate) {
		OggTrack *pTrack = new OggTrack(iSerial);
		if(pTrack) {
			if(pTrack->InitializeStreamState()) {
				pTrack->m_iIndex = g_Tracks.size();
				g_Tracks.push_back(pTrack);
			} else {
				delete pTrack;
				pTrack = NULL;
			}
		}
		return pTrack;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////

void MicrosToTime(sint64 iTimeUS, sint32& iHour, sint32& iMinute, sint32& iSecond, sint32& iMicros) {
	iMicros = (iTimeUS % 1000000);
	iSecond = (iTimeUS / 1000000) % 60;
	iMinute = (iTimeUS / 1000000 / 60) % 60;
	iHour = (iTimeUS / 1000000 / 60 / 60);
}

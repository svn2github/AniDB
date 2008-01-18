/**
	*  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
	*
	*  MatroskaUtilsBitrate.cpp
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
    \file MatroskaUtilsBitrate.cpp
		\version \$Id: MatroskaUtilsBitrate.cpp,v 1.7 2004/03/20 05:08:31 jcsston Exp $
    \brief Some code to get bitrate infomation about a Matroska file
		\author Jory Stone     <jcsston @ toughguy.net>		
		
*/

#include "MatroskaUtilsBitrate.h"

#include <iostream>
using namespace std;
#define FMT_TIMECODEN "%02d:%02d:%02d.%09d"
#define ARG_TIMECODENINT(t) (int32)((t) / 60 / 60 / 1000000000), \
                            (int32)(((t) / 60 / 1000000000) % 60), \
                            (int32)(((t) / 1000000000) % 60), \
                            (int32)((t) % 1000000000)
#define ARG_TIMECODEN(t) ARG_TIMECODENINT((int64)(t))

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

MatroskaBitrateInfoTrack::MatroskaBitrateInfoTrack()
{
	duration = 0;
}

MatroskaBitrateInfoReader::MatroskaBitrateInfoReader(MatroskaBitrateInfo *info) 
	: m_TextOutput(info->m_TextOutput)
{
	m_Info = info;	
	//m_TimecodeScale = TIMECODE_SCALE;
	prevTimecodes = new std::vector<float>();
	currentSize = new std::vector<float>();
	corrupt = false;
};

MatroskaBitrateInfoReader::~MatroskaBitrateInfoReader() {
	delete prevTimecodes;
	delete currentSize;
};
bool MatroskaBitrateInfoReader::Init(){
	UpperElementLevel = 0;
	timecode = 0;
	bAllowDummy = false;
	ElementLevel0 = NULL;
	ElementLevel1 = NULL;
	ElementLevel2 = NULL;
	ElementLevel3 = NULL;
	ElementLevel4 = NULL;
	reader = new MatroskaUilsFileReader(m_Info->m_SourceFilename.c_str(), MODE_READ);
	inputStream = new EbmlStream((*reader));

	// Find the EbmlHead element. Must be the first one.
	ElementLevel0 = inputStream->FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
	if (ElementLevel0 == NULL) {
		m_Info->hr = 2;
		return false;
	}
	//We must have found the EBML head :)
	ElementLevel0->SkipData((*inputStream), ElementLevel0->Generic().Context);
	delete ElementLevel0;

	// Next element must be a segment
	ElementLevel0 = inputStream->FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
	if (ElementLevel0 == NULL) {
		//No segment/level 0 element found.
		m_Info->hr = 2;
		return false;
	}
	if (!(EbmlId(*ElementLevel0) == KaxSegment::ClassInfos.GlobalId)) {
		delete ElementLevel0;
		m_Info->hr = 2;
		return false;
	}
	
	currentSize->resize(m_Info->m_TrackList.size());
	prevTimecodes->resize(m_Info->m_TrackList.size());
	UpperElementLevel = 0;
	
	//m_Info->accSize.empty();//+
	
	//m_Info->accSize.resize(m_Info->m_TrackList.size());//+
	
	// We've got our segment, so let's find the tracks
	ElementLevel1 = inputStream->FindNextElement(ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	return true;
}
//#include <windows.h>
bool MatroskaBitrateInfoReader::Clean(){
	//std::cout << "last timecode=" << timecode << std::endl;
	//MessageBoxA(NULL, "TEST", "TEST", MB_OK);
	delete ElementLevel0;
	delete inputStream;
	delete reader;
	m_Info->m_FilePos = m_Info->m_FileSize;
	return true;
}
/*void dump(){
	binary *buf = bf.Buffer();
	int left=(read/16)%16;
	printf("\nTRACK: %d, BUFS %d - %d\n", trackNum, read, left);
	for(size_t x=0; x<read/16/16; x++){
	printf("%06d ", x*16);
	for(size_t y=0; y<16; y++)
		printf("%02X ", buf[x*16+y]);
	printf("\n");
	}
	printf("       ");
	for(size_t y=0; y<left; y++)
	printf("%02X ", buf[read-left+y]);
	printf("\n");
}*/
void MatroskaBitrateInfoReader::Register(KaxInternalBlock &DataBlock){
	#ifdef TRACK_SIZE_FAST
	//uint64 t = inputStream->I_O().getFilePointer();
	DataBlock.ReadInternalHead(inputStream->I_O());
	//uint64 s = inputStream->I_O().getFilePointer();
	uint16 trackNum = DataBlock.TrackNum();
	//printf("Header size, track #%d: %d\n", trackNum, (s-t));	

/*printf("   Block (track number %u, %d frame(s), "
       "timecode %.3fs = " FMT_TIMECODEN ")\n",
       DataBlock.TrackNum(), DataBlock.NumberFrames(),
       (float)(DataBlock.GlobalTimecode() / 1000000),
       ARG_TIMECODEN(DataBlock.GlobalTimecode()));*/

	MatroskaTrackInfo *mti = m_Info->m_Parser->GetTrackInfo1(trackNum);
	if(mti){
		mti->m_timecodes->push_back(DataBlock.GlobalTimecode());
		if(/*mti->GetTrackType()==track_audio || */mti->m_wbs<TRACK_BUF_SIZE){
			inputStream->I_O().setFilePointer(inputStream->I_O().getFilePointer()-4);
			DataBlock.ReadData(inputStream->I_O() );

			size_t read;
			for(size_t i=0; i<DataBlock.NumberFrames(); i++){
				//read = (size_t)DataBlock.GetFrameSize(i);
				//mti->m_bytes += read;
				DataBuffer &bf = DataBlock.GetBuffer(i);
				read = bf.Size();

				mti->m_bytes += read;
				if(mti->m_wbs<TRACK_BUF_SIZE){
					read = min(read, TRACK_BUF_SIZE-mti->m_wbs);
					memcpy(mti->m_wbb+mti->m_wbs, bf.Buffer(), read);
					mti->m_wbs += read;
				}
			}
		}
		//Slightly wrong for audio... but won't bother with xK new dumps. also need to check why
		//else if(mti->GetTrackType()==track_audio)
		//	mti->m_bytes += DataBlock.GetSize()-5;
		else
			mti->m_bytes += DataBlock.GetSize()-4;
	}else{
		if(!corrupt) printf("\n");
		printf("WARNING: Invalid track number: %d @ %.2f ms.\n", trackNum, (double)DataBlock.GlobalTimecode() / m_Info->m_Parser->m_segmentInfo.m_infoTimecodeScale);
		corrupt = true;									
	}
#else
	DataBlock.ReadData(inputStream->I_O());

	MatroskaTrackInfo *mti = m_Info->m_Parser->GetTrackInfo1(DataBlock.TrackNum());
	if(mti){
		mti->m_timecodes->push_back(DataBlock.GlobalTimecode());
		for(size_t i=0; i<DataBlock.NumberFrames(); i++){
			mti->m_bytes += DataBlock.GetFrameSize(i);
		}
	}
#endif
}
bool MatroskaBitrateInfoReader::Loop() {
	if (UpperElementLevel > 0) {
		return false;//break;
	}
	if (UpperElementLevel < 0) {
		UpperElementLevel = 0;
	}

	if (EbmlId(*ElementLevel1) == KaxInfo::ClassInfos.GlobalId) {
		ElementLevel2 = inputStream->FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		while (ElementLevel2 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}

			/*if (EbmlId(*ElementLevel2) == KaxTimecodeScale::ClassInfos.GlobalId) {
				KaxTimecodeScale &TimeScale = *static_cast<KaxTimecodeScale *>(ElementLevel2);
				TimeScale.ReadData(inputStream->I_O());

				m_Info->m_Parser->m_segmentInfo.m_infoTimecodeScale = uint64(TimeScale);
				printf("\nLOL %I64d\n", m_Info->m_Parser->m_segmentInfo.m_infoTimecodeScale);
			}*/

			if (UpperElementLevel > 0) {	// we're coming from l3
				UpperElementLevel--;
				delete ElementLevel2;
				ElementLevel2 = ElementLevel3;
				if (UpperElementLevel > 0)
					break;
			} else {
				ElementLevel2->SkipData((*inputStream), ElementLevel2->Generic().Context);
				delete ElementLevel2;
				ElementLevel2 = inputStream->FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		}
		
	} else if (EbmlId(*ElementLevel1) == KaxCluster::ClassInfos.GlobalId) {
		KaxCluster *SegmentCluster = static_cast<KaxCluster *>(ElementLevel1);
		uint32 ClusterTimecode = 0;

		// read blocks and discard the ones we don't care about
		ElementLevel2 = inputStream->FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
		while (ElementLevel2 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}
			if (EbmlId(*ElementLevel2) == KaxClusterTimecode::ClassInfos.GlobalId) {						
				KaxClusterTimecode & ClusterTime = *static_cast<KaxClusterTimecode*>(ElementLevel2);
				ClusterTime.ReadData(inputStream->I_O());
				ClusterTimecode = uint32(ClusterTime);
				SegmentCluster->InitTimecode(ClusterTimecode, m_Info->m_Parser->m_segmentInfo.m_infoTimecodeScale);

			} else if (EbmlId(*ElementLevel2) == KaxSimpleBlock::ClassInfos.GlobalId) {
				KaxSimpleBlock &DataBlock = *static_cast<KaxSimpleBlock *>(ElementLevel2);
				DataBlock.SetParent(*SegmentCluster);
				Register(DataBlock);

			} else if (EbmlId(*ElementLevel2) == KaxBlockGroup::ClassInfos.GlobalId) {
				KaxBlockGroup & aBlockGroup = *static_cast<KaxBlockGroup*>(ElementLevel2);
				ElementLevel3 = inputStream->FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
				while (ElementLevel3 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}
					/*if (EbmlId(*ElementLevel3) == KaxTrackUID::ClassInfos.GlobalId) {
						KaxTrackUID &track_uid = *static_cast<KaxTrackUID *>(ElementLevel3);
						TRACE1("UID = %08x\n", uint64(track_uid));
					}*/
					if (EbmlId(*ElementLevel3) == KaxBlock::ClassInfos.GlobalId) {
						KaxBlock & DataBlock = *static_cast<KaxBlock*>(ElementLevel3);
						DataBlock.SetParent(*SegmentCluster);
						Register(DataBlock);
					}
					if (UpperElementLevel > 0) {
						UpperElementLevel--;
						delete ElementLevel3;
						ElementLevel3 = ElementLevel4;
						if (UpperElementLevel > 0)
							break;
					} else {
						ElementLevel3->SkipData((*inputStream), ElementLevel3->Generic().Context);
						delete ElementLevel3;
						ElementLevel3 = NULL;
						ElementLevel3 = inputStream->FindNextElement(ElementLevel2->Generic().Context, UpperElementLevel, ElementLevel2->ElementSize(), bAllowDummy);
					}							
				}
			}
			if (UpperElementLevel > 0) {
				UpperElementLevel--;
				delete ElementLevel2;
				ElementLevel2 = ElementLevel3;
				if (UpperElementLevel > 0)
					break;
			} else {
				ElementLevel2->SkipData((*inputStream), ElementLevel2->Generic().Context);
				delete ElementLevel2;								
				ElementLevel2 = NULL;
				ElementLevel2 = inputStream->FindNextElement(ElementLevel1->Generic().Context, UpperElementLevel, ElementLevel1->ElementSize(), bAllowDummy);
			}
		}
	}
	// Update current pos
	m_Info->m_FilePos = reader->getFilePointer();

	if (UpperElementLevel > 0) {		// we're coming from ElementLevel2
		UpperElementLevel--;
		delete ElementLevel1;
		ElementLevel1 = ElementLevel2;
		if (UpperElementLevel > 0)
			return false;//break;
	} else {
		ElementLevel1->SkipData((*inputStream), ElementLevel1->Generic().Context);
		delete ElementLevel1;
		ElementLevel1 = NULL;
		ElementLevel1 = inputStream->FindNextElement(ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	}
	m_Info->hr = 0;//+
	return ElementLevel1 != NULL;
}
MatroskaBitrateInfo::MatroskaBitrateInfo() {
	m_FileSize = 0;
	m_FilePos = 0;
	hr = -255;
	m_Reader = NULL;
	m_TargetTextCtrl = NULL;
}
MatroskaBitrateInfo::~MatroskaBitrateInfo() {
	if(m_Parser){
		m_Reader->Clean();	
		delete m_Reader;
	}
#ifdef INFO_READER
	if ((hr == -255) && (m_Reader != NULL))
		m_Reader->Stop();	
#endif
}
int MatroskaBitrateInfo::Open(MatroskaInfoParser *parser) {
	assert(parser != NULL);
	m_Parser = parser;
	if (parser->trackInfos.size() == 0)
		// No Tracks
		return -1;

	m_SourceFilename = parser->GetFilename();
	m_FileSize = parser->file_size;
	
	float duration = parser->m_segmentInfo.GetDuration();
	m_TrackList.resize(parser->trackInfos.size());
	for (size_t t = 0; t < parser->trackInfos.size(); t++) {
		MatroskaTrackInfo *track = parser->trackInfos.at(t);
		MatroskaBitrateInfoTrack &localTrack = m_TrackList.at(t);
		localTrack.duration = duration;
		//localTrack.bitrateLength = 1;
		localTrack.uid = track->m_trackUID;
		//localTrack.bitrates.reserve((size_t)(localTrack.duration / localTrack.bitrateLength));
	}
	m_Reader = new MatroskaBitrateInfoReader(this);
	m_Reader->Init();
	return 0;
}

float MatroskaBitrateInfo::GetCurrentPercent() {
	if (m_FileSize == 0)
		return 100;
	return (float)((100.0*m_FilePos)/(1.0*m_FileSize));
}

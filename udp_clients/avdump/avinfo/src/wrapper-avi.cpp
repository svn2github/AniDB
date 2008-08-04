// Copyright (C) 2006 epoximator
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#ifdef MEDIA_INFO
#include "wrappers.h"
#include <MediaInfo/MediaInfo.h>
#include <ZenLib/ztring.h>
#include <math.h>
#include "aviparse.h"

#include <MACLib/All.h>
#include <MACLib/MACLib.h>
#include <MACLib/APETag.h>

using namespace ZenLib;
using namespace MediaInfoLib;

//#define PRINT_SPECIAL
//#define MEDIA_INFO_DUMP

#define MYBUFS 128
#define MAXTRACKS 10

#define GETS(x, y, z) m_mi->Get(x, y, _T(z))
#define GETG(x) GETS(Stream_General, 0, x)
#define GETVN(x, y) GETS(Stream_Video, x, y)
#define GETV(x) GETVN(0,x)
#define GETA(x, y) GETS(Stream_Audio, x, y)
#define GETT(x, y) GETS(Stream_Text, x, y)
#define TOI(x) _wtoi(x.c_str())
#define TOF(x) _wtof(x.c_str())
#define TO8(x) (A = x).To_UTF8()
#define TOFI(x) (int)(0.5f+TOF(x))

AVI::AVI(wchar_t *path){ 
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_CrtSetBreakAlloc(442266);
#endif
	m_mi = new MediaInfo();
	valid = m_mi->Open(path)==1;
	m_ap = new AviP(path);
}
void AVI::clean(){
#ifdef PRINT_SPECIAL
	//wprintf(L"%s - %s - %s - %s\n", GETG("CompleteName").c_str(), GETV("StreamSize").c_str(), GETV("PlayTime").c_str(), GETV("BitRate").c_str());
	wprintf(L"%s - %s - %s - %s - %s\n", GETG("CompleteName").c_str(), GETA(0,"Codec").c_str(), GETA(0,"StreamSize").c_str(), GETA(0,"PlayTime").c_str(), GETA(0,"BitRate").c_str());
#endif
#ifdef MEDIA_INFO_DUMP
	for (int StreamKind=Stream_General; StreamKind<Stream_Max; StreamKind++){
        for (int StreamPos=0; StreamPos<m_mi->Count_Get((stream_t)StreamKind); StreamPos++){
            Ztring A=m_mi->Get((stream_t)StreamKind, StreamPos, _T("StreamKind"));
            Ztring B=m_mi->Get((stream_t)StreamKind, StreamPos, _T("StreamKindID"));
            if (B!=_T("")){
                A+=_T(" #");
                A+=B;
            }
			wprintf(L"%s\n", A.c_str());
			for (int Champ_Pos=0; Champ_Pos<m_mi->Count_Get((stream_t)StreamKind, StreamPos); Champ_Pos++){
				Ztring C=m_mi->Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Measure_Text); // TODO Bug sinon? voir Ztring
				if ((1 || m_mi->Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==_T('Y')) && m_mi->Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text)!=_T("")){
					Ztring Nom=m_mi->Get((stream_t)StreamKind, 0, Champ_Pos, Info_Name_Text);
					Ztring Valeur=m_mi->Get((stream_t)StreamKind, StreamPos, Champ_Pos, Info_Text);
					wprintf(L"%d\t %s : %s\n", Champ_Pos, Nom.c_str(), Valeur.c_str());
				}
			}
        }
    }
#endif
	delete m_mi;
	delete m_ap;
}
int AVI::code(){
	return m_ap->m_code;
}
float AVI::parse(){
	return m_ap->progress();
}

int AVI::mkAVMF(avmf::Container *cont){
	cont->duration = TOF(GETG("PlayTime"))/1000;
	//cont->timecodescale = info->GetTimecodeScale();
	Ztring A;
	cont->title = TO8(GETG("Title"));
	cont->writtenby = TO8(GETG("Encoded_Application"));
	cont->muxedby = TO8(GETG("Encoded_Library"));

	//cont->misc_size = m_att_size;

	int j = 0;
	for(int i=0; i<TOI(GETG("VideoCount")); i++){
		avmf::TrackVideo *tv = new avmf::TrackVideo();
		tv->image.dw = 0;
		tv->image.dh = 0;
		tv->image.pw = TOI(GETVN(i,"Width"));
		tv->image.ph = TOI(GETVN(i,"Height"));
		tv->image.ar = TOF(GETVN(i,"AspectRatio"));
		float diff = abs(tv->image.ar-(1.0f*tv->image.pw/tv->image.ph));
		//printf("AR %f\n", diff);
		if(diff<0.05)
			tv->image.ar = 0;
		tv->image.resolution = TOI(GETVN(i,"Resolution"));
		tv->image.chroma = TO8(GETVN(i,"Chroma"));
		tv->image.structure = TO8(GETVN(i,"Interlacement"));
		
		tv->sample.count = TOI(GETVN(i,"FrameCount"));
		tv->sample.rate = TOF(GETVN(i,"FrameRate"));
		
		{ // use riff fps
			float old = TOF(GETVN(i,"FrameRate/RIFF"));
			if( old>0 && old!=tv->sample.rate )
				tv->sample.rate = old;
		}

		if(TOI(GETVN(i,"VFR")))
			tv->sample.rate = VIDEO_MODE_VFR;
		
		tv->fourcc = TO8(GETVN(i,"Codec"));
		tv->encoder = TO8(GETVN(i,"Encoded_Library"));
		tv->settings = TO8(GETVN(i,"Encoded_Library_Settings"));

		tv->no = j++;
		tv->uid = TOI(GETVN(i,"UniqueID"));
		tv->title = TO8(GETVN(i,"Title"));
		tv->language = TO8(GETVN(i,"Language"));
		tv->duration = TOF(GETVN(i,"PlayTime"))/1000;
		tv->flag_default = 0;

		//if(GETG("FileExtension")==_T("mp4"))	
		//	tv->size = TOI(GETVN(i, "Size"));
		//else
		if(GETG("FileExtension")==_T("avi") && m_ap->getSize()>0)
			tv->size = m_ap->getSize(tv->no);
		else
			tv->bitrate = TOFI(GETVN(i, "BitRate"));
		
		if(GETG("FileExtension")!=_T("avi")){
			float min = TOF(GETVN(i,"FrameRate_Min"));
			float max = TOF(GETVN(i,"FrameRate_Max"));
			if(max-min>1.0f){
				tv->sample.rate = -1.0f;
				char out[128];
				sprintf(out, "fps(max=%0.3f,min=%0.3f)", max, min);
				tv->private_vector.push_back(out);
			}
		}

		cont->tracks.push_back(tv);
	}
	for(int i=0; i<TOI(GETG("AudioCount")); i++){
		avmf::TrackAudio *ta = new avmf::TrackAudio();
		ta->twocc = TO8(GETA(i,"Codec"));
		ta->encoder = TO8(GETA(i,"Encoded_Library"));
		
		ta->channels = TOI(GETA(i,"Channel(s)"));
		ta->sample.count = TOI(GETA(i,"SamplingCount"));
		ta->sample.rate = TOF(GETA(i,"SamplingRate"));
		ta->mode = TO8(GETA(i,"Bitrate_mode"));

		ta->no = j++;
		if(m_ap->isOK() && isVbr(m_ap->getBuffer(ta->no), AUDIO_BUF_SIZE))
			ta->mode = "VBR";
		if(ta->mode=="CBR")
			ta->mode = "";

		ta->uid = TOI(GETA(i,"UniqueID"));
		ta->title = TO8(GETA(i,"Title"));
		ta->language = TO8(GETA(i,"Language")); //A = GETG("ILNG");
		ta->duration = TOI(GETA(i, "PlayTime"))/1000.0f;
		ta->flag_default = 0;

		//if(GETG("FileExtension")==_T("mp4"))
		//	ta->size = TOI(GETA(i, "Size"));
		//else
		if(GETG("FileExtension")==_T("avi") && m_ap->getSize()>0)
			ta->size = m_ap->getSize(ta->no);
		else
			ta->bitrate = TOFI(GETA(i, "BitRate"));
		cont->tracks.push_back(ta);
	}
	for(int i=0; i<TOI(GETG("TextCount")); i++){
		avmf::TrackSubtitle *ts = new avmf::TrackSubtitle();
		
		ts->codecid = TO8(GETT(i, "Codec"));
		ts->no = j++;
		ts->title = TO8(GETT(i,"Title"));
		ts->language = TO8(GETT(i,"Language"));
		ts->duration = TOI(GETT(i, "PlayTime"))/1000.0f;
		ts->flag_default = 0;

		if(GETG("FileExtension")==_T("mp4"))	
			ts->size = TOI(GETT(i, "Size"));
		else
		if(GETG("FileExtension")==_T("avi") && m_ap->getSize()>0)
			ts->size = m_ap->getSize(ts->no);
		else
			ts->size = 0;

		cont->tracks.push_back(ts);
	}
	cont->chapters = TOI(GETG("ChaptersCount"));

	cont->meta.title = TO8(GETG("Track"));
	cont->meta.album = TO8(GETG("Album"));
	cont->meta.artist = TO8(GETG("Performer"));
	cont->meta.composer = TO8(GETG("Composer"));
	cont->meta.genre = TO8(GETG("Genre"));
	cont->meta.year = TO8(GETG("Recorded_Date"));
	cont->meta.track_number = TO8(GETG("Track/Position"));
	cont->meta.total_track_number = TO8(GETG("Track/Position_Total"));
	cont->meta.disk_number = TO8(GETG("Part/Position"));
	cont->meta.total_disk_number = TO8(GETG("Part/Position_Total"));
	cont->meta.comment = TO8(GETG("Comment"));
	cont->meta.copyright = TO8(GETG("Copyright"));
	cont->meta.encoded_by = TO8(GETG("Encoded_Library"));

/*	std::cout << "title=" << cont->meta.title << std::endl;
	std::cout << "album=" << cont->meta.album << std::endl;
	std::cout << "artist=" << cont->meta.artist << std::endl;
	std::cout << "composer=" << cont->meta.composer << std::endl;
	std::cout << "genre=" << cont->meta.genre << std::endl;
	std::cout << "year=" << cont->meta.year << std::endl;
	std::cout << "track_number=" << cont->meta.track_number << std::endl;
	std::cout << "total_track_number=" << cont->meta.total_track_number << std::endl;
	std::cout << "disk_number=" << cont->meta.disk_number << std::endl;
	std::cout << "total_disk_number=" << cont->meta.total_disk_number << std::endl;
	std::cout << "comment=" << cont->meta.comment << std::endl;
	std::cout << "copyright=" << cont->meta.copyright << std::endl;
	std::cout << "encoded_by=" << cont->meta.encoded_by << std::endl;*/

	//FIX title->track, BufferC+125->BufferC[125] in File_Mpega
	if(GETG("FileExtension")==_T("mp3")){
		CAPETag monkey(GETG("CompleteName").c_str());
		if(monkey.GetHasAPETag()){
			CAPETagField *pTagField;
			for(int i=0; 0!=(pTagField=monkey.GetTagField(i)); i++)
			{
				if(!wcscmp(pTagField->GetFieldName(), L"Title"))
					cont->meta.title = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Album"))
					cont->meta.album = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Artist"))
					cont->meta.artist = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Composer"))
					cont->meta.composer = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Performer"))
					cont->meta.original_artist = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Track"))
					cont->meta.track_number = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Disc"))
					cont->meta.disk_number = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Year"))
					cont->meta.year = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Genre"))
					cont->meta.genre = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Comment"))
					cont->meta.comment = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Copyright"))
					cont->meta.copyright = pTagField->GetFieldValue();
				else if(!wcscmp(pTagField->GetFieldName(), L"Encoder"))
					cont->meta.encoded_by = pTagField->GetFieldValue();
				else{
					printf("APE Tag not used: %ls=%s\n", pTagField->GetFieldName(), pTagField->GetFieldValue());
				}
			}
		}
	}
	return 0;
}

#endif
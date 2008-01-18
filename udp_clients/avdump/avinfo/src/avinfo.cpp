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

#include "wrappers.h"

#ifdef _DEBUG
#include <stdio.h>
#endif
#include <windows.h>

//#define MB_DEBUG
#define MB_PRINTF(x) printf(x)

const char* checkFile(wchar_t* path, mylong &size){
	HANDLE Handle=CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (Handle!=INVALID_HANDLE_VALUE){
        DWORD High;DWORD Low=GetFileSize(Handle,&High);
        size = 0x100000000ULL*High+Low;

		char buffer[16];
		DWORD read;
		ReadFile(Handle, buffer, 16, &read, NULL);
        CloseHandle(Handle); Handle=NULL;

		wchar_t* ext = (wchar_t*)(path+wcslen(path)-4);

		if(read==16){
			/*for(int i=0; i<16;i++)
				printf("%02X ", 0x0FF&buffer[i]);
			printf("\n");*/
			if(!memcmp(buffer, "RIFF", 4)&&!memcmp(buffer+8, "AVI", 3))
				return "avi";
			if(!memcmp(buffer+8, "matroska", 8))
				return "mkv";
			if(!memcmp(buffer, "OggS", 4))
				return "ogm";
			if(!memcmp(buffer+4, "moov", 4)||!memcmp(buffer+4, "mdat", 4)||!memcmp(buffer+4, "free", 4)/*||!memcmp(buffer+4, "ftypqt", 6)*/)
				return "mov";
			if(!memcmp(buffer+4, "wide", 4)&&!memcmp(buffer+12, "mdat", 4))
				return "mov";
			if(!memcmp(buffer+4, "ftyp", 4))
				return "mp4";
			if(buffer[0]==0 && buffer[1]==0 && buffer[2]==1 && buffer[3]==0xFFFFFFBA && buffer[4]==0x21 && buffer[5]==0)
				return "mpg";
			if(!memcmp(buffer, "RIFF", 4)&&!memcmp(buffer+8, "CDXA", 3))
				return "mpg";
			if(!memcmp(buffer, ".RMF", 4))
				return "rm";
			unsigned int* ibuf = (unsigned int*)buffer;
			if(ibuf[0]==0xA3DF451A)
				return "mkv";
			if(ibuf[0]==0x75B22630 && ibuf[1]==0x11CF668E && ibuf[2]==0xAA00D9A6 && ibuf[3]==0x6CCE6200)
				return "wmv";
			//MessageBoxW(NULL, path, L"Unknown ext:", MB_OK);
			//wprintf(L"Unknown extension: %s\n", path);
			return "...";
		}else printf("failed to read\n");
	}
	return NULL;
}
inline MediaContainer* getFile(addr_t id){
	return (MediaContainer*) id;
}
#include <iostream>
addr_t avinfo_file_open(wchar_t* path){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_open\n");
	//MessageBoxW(NULL, path, L"Open file:", MB_OK);
#endif
	//printf("avinfo_file_open\n");
	mylong size = 0;
	const char* ext = checkFile(path, size);
	if(size<1||ext==NULL) return 0;
	MediaContainer *id = 0;
	
	if(!memcmp(ext,"mkv",3))
#ifdef HAALI_MATROSKA_PARSER
			id = new MKV2(path);
#else
			id = new MKV(path);
#endif
	else if(!memcmp(ext,"ogm",3))
			id = new OGM(path);
#ifdef MYMP4
	else if(!memcmp(ext,"mp4",3))
			id = new MP4(path);
#endif
#ifdef MEDIA_INFO
	else id = new AVI(path);
#else
	else return -2;
#endif
	if(id->valid){
		id->m_bytes = size;//calcSize(path);
		id->m_ext = ext;
		return (addr_t)id;
	}
	return 0;
}
void avinfo_file_close(addr_t id){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_close\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc){
		mc->clean();
		delete mc;
	}
}
float avinfo_file_parse(addr_t id){
#ifdef MB_DEBUG
	//MB_PRINTF("avinfo_file_parse\n");
#endif
	//printf("HI %d\n", id);
	MediaContainer *mc = getFile(id);
	if(mc) return mc->parse();
	return -1.0f;
}
/*float avinfo_file_duration(addr_t id){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_duration\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc) return mc->GetDuration();
	return -1;
}
int avinfo_track_count(addr_t id){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_track_count\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc) return mc->GetTrackCount();
	return -1;
}*/
int avinfo_file_formatted(addr_t id, char* buffer, unsigned int size, int format){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_formatted\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc) return mc->toString(buffer, size, format);
	return -1;
}
int avinfo_file_tinyxml(addr_t id, addr_t tx){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_tinyxml\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc) return mc->buildXML(tx);
	return -1;
}
int avinfo_file_tinyxml_ost(addr_t id, addr_t tx){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_file_tinyxml\n");
#endif
	MediaContainer *mc = getFile(id);
	if(mc) return mc->buildXMLost(tx);
	return -1;
}
/*GenericTrack avinfo_track_generic(addr_t id, int nr){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_track_generic\n");
#endif
	GenericTrack t;
	MediaContainer *mc = getFile(id);
	if(mc) return mc->GetGenericTrack(nr);
	return t;
}
VideoTrack avinfo_track_video(addr_t id, int nr){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_track_video\n");
#endif
	VideoTrack t;
	MediaContainer *mc = getFile(id);
	if(mc) return mc->GetVideoTrack(nr);
	return t;
}
AudioTrack avinfo_track_audio(addr_t id, int nr){
#ifdef MB_DEBUG
	MB_PRINTF("avinfo_track_audio\n");
#endif
	AudioTrack t;
	MediaContainer *mc = getFile(id);
	if(mc) return mc->GetAudioTrack(nr);
	return t;
}*/
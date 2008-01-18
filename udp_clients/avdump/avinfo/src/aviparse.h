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

#pragma once

//#define AUDIO_BUF_SIZE 1048576
//#define AUDIO_BUF_SIZE 32768
#define AUDIO_BUF_SIZE 262144//131072
#define AVI_MAX_TRACKS 10

typedef __int64 size_f;

class AviAudio{
public:
	size_t m_wbs;
	unsigned char m_wbb[AUDIO_BUF_SIZE];
	AviAudio(){
		m_wbs = 0;
	}
	void* getPointer(){
		return m_wbb+m_wbs;
	}
};
class AviP{
private:
	size_f m_size, m_left, m_fsiz, m_avix;
	char m_data[32];
	unsigned __int64 m_ts[AVI_MAX_TRACKS];
	AviAudio* m_audio[AVI_MAX_TRACKS];
	//fio
	FILE *m_file;
	size_f afpos();
	bool aread(size_t n);
	bool aseek(size_t n);
	//avi
	size_t find_movi();
	size_f parse_stream(size_f t_size);
	size_f parse_streams(int max);
	AviAudio* getAudio(size_t id);
public:
	int m_code;
	AviP(wchar_t *path);
	~AviP();
	float progress();
	
	unsigned char* getBuffer(size_t id){
		if(m_audio[id]) return m_audio[id]->m_wbb;
		return NULL;
	}
	unsigned __int64 getSize(size_t id){
		return m_ts[id];
	}
	size_f getSize(){
		return m_size;
	}
	bool isOK(){
		return m_size>0 && m_left<1;
	}
};
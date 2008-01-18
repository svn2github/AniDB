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

#include <iostream>
#include <io.h>
#include <tchar.h>

#include "aviparse.h"

#define MYERROR(x,y) \
{ \
	m_size = 0; \
	m_code = -x; \
	printf("%s\n", y); \
	return 0; \
}

inline size_t CC4(const char* C) {return C[3]<<0 | C[2]<<8 | C[1]<<16 | C[0]<<24;}
inline size_t CC2(const char* C) {return C[1]<<0 | C[0]<<8;}
size_t lestr2uint(const char* c){
    size_t r = (unsigned char)c[3];
    r = (r<<8)|(unsigned char)c[2];
    r = (r<<8)|(unsigned char)c[1];
    r = (r<<8)|(unsigned char)c[0];
    return r;
}
inline bool check_riff(const char* C) {
	return (C[0]=='d'||C[0]=='w'||C[0]=='s') && (C[1]=='b'||C[1]=='c');}
#define PAD_EVEN(x) ( ((x)+1) & ~1 )

AviP::AviP(wchar_t *path){
	for(size_t i=0; i<AVI_MAX_TRACKS; i++){
		m_ts[i] = 0;
		m_audio[i] = NULL;
	}
	m_file = _tfsopen(path, L"rbS", _SH_DENYNO);
	m_fsiz = _filelengthi64(m_file->_file);
	m_avix = 0;
	m_left = m_size = find_movi();
	if(m_avix>0){
		while(progress()>0);		
		aseek(m_avix-afpos());		
		m_left = m_size = find_movi();		
	}
	m_code = 0;
}
AviP::~AviP(){
	fclose(m_file);
	for(size_t i=0; i<AVI_MAX_TRACKS; i++)
		if(m_audio[i]!=NULL)
			delete m_audio[i];
}
AviAudio* AviP::getAudio(size_t id){
	if(m_audio[id]==NULL)
		m_audio[id] = new AviAudio();
	return m_audio[id];
};

bool AviP::aread(size_t n){
	return fread(m_data, 1, n, m_file)!=n;
}
bool AviP::aseek(size_t n){
	return fseek(m_file, (long)n, SEEK_CUR)!=0;
}
float AviP::progress(){
	if(m_size<1 || m_left<1)
		return -1;
	parse_streams(160);
	return 100*(float)(m_size-m_left)/m_size;
}
size_f AviP::afpos(){
	fpos_t fp;
	fgetpos(m_file, &fp);
	return fp;
}
size_t AviP::find_movi(){
	//printf("find_movi: %X\n", afpos());
	if(aread(12)) MYERROR(1,"\nfailure: read start");	
	if(CC4(m_data)!=CC4("RIFF")) return 0; //MYERROR("\nfailure: not riff")
	size_t typ = CC4(m_data+8);
	size_t r_size = lestr2uint((const char*)m_data+4), a_size;
	if(typ==CC4("menu")){
		aseek(r_size-4);
		return find_movi();
	}
	if(typ!=CC4("AVI ") && typ!=CC4("AVIX")){
		if(typ==CC4("CDXA")) return 0;
		MYERROR(2,"\nfailure: not avi(x)")
	}
	if(m_avix==0 && r_size<m_fsiz/3)
		m_avix = afpos()+r_size-4;

	for(int z=0; ;z++){
		if(aread(8)) MYERROR(3,"\nfailure: read list")
		a_size = lestr2uint((const char*)m_data+4);
					
		if(aread(4)) MYERROR(4,"\nfailure: read type")
			a_size -= 4;

		if(CC4(m_data)!=CC4("movi")){
			if(aseek(a_size)) MYERROR(5,"\nfailure: read aseek movi")
			continue;
		}else{
			//printf(" movi @ %X\n", afpos()-4);
			return a_size;
		}
	}
	return 0;
}
size_f AviP::parse_stream(size_f t_size){
	if(aread(8)) MYERROR(6,"\nfailure: read stream")
	size_t cc4 = CC4(m_data);

	if(cc4==CC4("LIST")) return t_size-8;
	if(cc4==CC4("rec ")){
		aseek(-4);
		return t_size-4;
	}
	if(CC2(m_data)==CC2("ix")){
		size_t s = lestr2uint((const char*)m_data+4);
		aseek(s);
		return t_size-s-8;
	}
	/*if(CC2(m_data+2)==CC2("tx")){
		size_t s = lestr2uint((const char*)m_data+4);
		aseek(s);
		return t_size-s-8;
	}*/
	
	//MAYBE check for dc/db/pc/wb
	size_t r_size = lestr2uint((const char*)m_data+4);
	if(r_size>m_left)
		MYERROR(12,"\nfailure: chunk size out of range")
	size_t s_size = PAD_EVEN(r_size);
	m_data[4]='\0';
	//printf("%08X : ", afpos());
	//printf("%s: %#5d", m_data, s_size);

	bool audio = false;
	int tn = 0;
	if(m_data[0]!='0'){
		if(check_riff(m_data) && m_data[2]=='0'){
			tn = atoi(m_data+2);
			if(m_data[0]=='w')
				audio = true;
		}else if( !strcmp("JUNK", m_data)
			//|| !strcmp("idx1", m_data)
			){
			tn = -1;
		}else{
			printf(" @ %08X :", afpos());
			printf(" %s (%d)", m_data, s_size);
			printf(" [%02X %02X %02X %02X]", m_data[0], m_data[1], m_data[2], m_data[3]);
			MYERROR(7,"\nfailure: stream not found")
		}
	}else{
		if(m_data[2]=='w'&&m_data[3]=='b')
			audio = true;
		m_data[2]='\0';
		tn = atoi(m_data);
	}
	
	t_size -= s_size+8;
	if(tn>=0){
		m_ts[tn] += r_size;
		//printf(" : %#9d - %#9d\n", m_ts[tn], m_left);
	}
	
	if(audio/* && (m_size-m_left)>AUDIO_BUF_SIZE*/){
		AviAudio *aa = getAudio(tn);
		if(aa->m_wbs<AUDIO_BUF_SIZE){
			size_t read = AUDIO_BUF_SIZE-aa->m_wbs;
			if(r_size<read) read = r_size;
			if(read!=fread(aa->getPointer(), 1, read, m_file)) MYERROR(8,"\nfailure: read audio")
			aa->m_wbs += read;
			//printf("id=%d, m_wbs=%X, read=%X (%d)\n", aa->m_id, aa->m_wbs, read, read);
			if(read<s_size) if(aseek(s_size-read)) MYERROR(9,"\nfailure: skip audio")
		}else if(aseek(s_size)) MYERROR(10,"\nfailure: skip stream")
	}else if(aseek(s_size)) MYERROR(11,"\nfailure: skip stream")
	return t_size;
}
size_f AviP::parse_streams(int max){
	for(int i=0; i<max && m_left>0; i++)
		m_left = parse_stream(m_left);
	return m_left;
}
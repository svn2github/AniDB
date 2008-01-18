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

#ifndef __WRAPPERS_H
#define __WRAPPERS_H

bool eq(const char* a, const char* b);
int msfourcc(const char* a);
int userdata(int fourcc, const char* a);
int mkvcodecid(const char* a, const char* b);
int language(const char* b);
const char* str_lang(int x);
const char* str_codec(int x);
const char* str_ar(int x);
const char* str_chan(int x);
const char* str_sub(int x);

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include <algorithm>
#include <vector>

#include "avmf.h"
#include "avinfo.h"

class MediaContainer{
public:
	virtual int code(){return 0;};
	virtual void clean()=0;
	virtual float parse(){return 0;}

	virtual int mkAVMF(avmf::Container *cont){return -1;};

	bool valid;

	int toString(char* buf, unsigned int size, int format);
	int buildXML(addr_t tx);
	int buildXMLost(addr_t tx);

	mylong m_bytes;
	const char* m_ext;
protected:
	static bool isVbr(unsigned char * buffer, int len);
};

class MatroskaInfoParser;
class MatroskaBitrateInfo;

class MKV : public MediaContainer{
	MatroskaInfoParser *m_p;
	MatroskaBitrateInfo *m_b;
	int m_att_size;
public:
	MKV(wchar_t* path);
	
	int code();
	void clean();
	float parse();
	int mkAVMF(avmf::Container *cont);
};

class MyOGM;
class OGM : public MediaContainer{
private:
	MyOGM *m_p;
public:	
	OGM(wchar_t* path);
	
	void clean();
	float parse();
	int mkAVMF(avmf::Container *cont);
};

namespace MediaInfoLib{
	class MediaInfo;
}
class AviP;
class AVI : public MediaContainer{
private:
	MediaInfoLib::MediaInfo* m_mi;
	AviP* m_ap;

public:	
	AVI(wchar_t* path);
	
	int code();
	void clean();
	float parse();

	int mkAVMF(avmf::Container *cont);
};

#endif //__WRAPPERS_H
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

#ifdef ENABLE_HASH

#ifndef __AVHASH_H
#define __AVHASH_H

#include "hash.h"
#include <windows.h>


#define HT_HASH_MASK (HASH_CRC|HASH_ED2K|HASH_MD5|HASH_SHA1|HASH_TTH|HASH_AICH)
#define HT_CREQ_HASH (HASH_CRC|HASH_ED2K|HASH_MD5|HASH_SHA1|HASH_TTH)
#define HT_FULL_HASH (HT_CREQ_HASH|HASH_AICH)

#define HT_JUMP 128
#define HT_LINK 256
#define HT_ELINK 512
#define HT_INDENT 1024
#define HT_HTIME 2048
#define HT_NOCARE 4096
#define HT_SILENT 8192

#define HT_OTHER_MASK (HT_JUMP|HT_LINK|HT_ELINK|HT_INDENT|HT_HTIME)

struct hashc{
	char ed2k[33], ed2k_blue[33], crc[9], sha1[41], md5[33], tth[40], aich[33];
};


//#define RB_DEBUG
#define RB_SLEEP_READ 1
#define RB_SLEEP_HASH 1

struct RingBuffer {
	int parts, partsize, write, read, read2, used, used2, active;
	char** buffer;
	CRITICAL_SECTION cs;
};


void hash_setExportFile(void* link);
void hash_copy(hashc* chk, int id, int typ);
void hash_reset(hashc* chk);
void rb_init(RingBuffer* rb);
void rb_destroy(RingBuffer* rb);
void hash_file(hashc* chk, int mode, FILE* file, unsigned __int64 file_size, unsigned int buffer_size);
void hash_file(hashc* chk, int mode, FILE* file, unsigned __int64 file_size, RingBuffer* rb);
int hash_mp3(wstring path, FILE* file, unsigned __int64 file_size, char* sum, int &duration);


#define BC(x,y) ((x&y)==y)
#define HASH_COPY(id,mode,type) if((mode&type)==type) hash_copy(chk,id,type);


#endif //__AVHASH_H

#endif
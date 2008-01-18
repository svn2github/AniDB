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

#include <fstream>

#include <sstream>
#include <vector>

using namespace std;

#include "avhash.h"

void hash_reset(hashc* chk){
	chk->ed2k[0] =
	chk->ed2k_blue[0] =
	chk->crc[0] =
	chk->sha1[0] =
	chk->md5[0] =
	chk->tth[0] =
	chk->aich[0] = '\0';
}

void hash_copy(hashc* chk, int id, int type){
	char sum[128];
	int i = hash_digest(id, type, (char*)&sum, 128);
	sum[i] = '\0';
	_strlwr_s(sum, 128);

	switch(type){
	case HASH_ED2K: {
		memcpy(chk->ed2k, sum, 32);
		memcpy(chk->ed2k_blue, sum+32, 32);
		chk->ed2k[32] =
		chk->ed2k_blue[32] =
		sum[32] = '\0';
	} break;
	case HASH_CRC: 
		strcpy_s(chk->crc, 9, sum);
		break;
	case HASH_SHA1: 
		strcpy_s(chk->sha1, 41, sum);
		break;
	case HASH_MD5: 
		strcpy_s(chk->md5, 33, sum);
		break;
	case HASH_TTH: 
		strcpy_s(chk->tth, 40, sum);
		break;
	case HASH_AICH: 
		strcpy_s(chk->aich, 33, sum);
		break;
	}
}

void hash_file(hashc* chk, int mode, FILE* file, unsigned __int64 file_size, unsigned int buffer_size){

	char* databuffer = new char[buffer_size];
	unsigned __int64 read = 0;
	int rt;

	int hash = hash_create(mode&HT_HASH_MASK, file_size);
	
	unsigned int t0 = GetTickCount();
	unsigned int td0 = t0, td1, num=0;
	int prog = !BC(mode, HT_SILENT);
	int nocare = BC(mode, HT_NOCARE);

	fseek(file, 0, SEEK_SET);
	while(read<file_size){
		read += buffer_size;
		if(read>file_size)
			rt = buffer_size-(read-file_size);
		else rt = buffer_size;
		fread((char*)databuffer, 1, rt, file);
		hash_update(hash, databuffer, rt);
		
		num++;
		if(prog)
		{
			if(nocare)
				printf("\n");
			else
				printf("\r");
			printf("H  %05.2f%%", 100.0*read/file_size);
			//printf("\rH %02d%%", (int)(100.0*read/file_size));
			//if(num%2==0)
			//{
			td1 = GetTickCount();
			printf("  @ %6.2f MB/s", 1000.0*rt/(td1-td0)/1024/1024);
			printf("  / %6.2f MB/s", 1000.0*read/(td1-t0)/1024/1024);
			td0 = td1;
		}
		//}
	}
	if(nocare)
		printf("\n");
	else
		printf("\r                                          \r");

	HASH_COPY(hash, mode, HASH_CRC)
	HASH_COPY(hash, mode, HASH_ED2K)
	HASH_COPY(hash, mode, HASH_MD5)
	HASH_COPY(hash, mode, HASH_SHA1)
	HASH_COPY(hash, mode, HASH_AICH)
	HASH_COPY(hash, mode, HASH_TTH)

	hash_destroy(hash);
	
	delete databuffer;
}


/*const char base32Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
string& toBase32(const unsigned char* src, size_t len, string& dst) {
	// Code snagged from the bitzi bitcollider
	size_t i, index;
	unsigned char word;
	dst.reserve(((len * 8) / 5) + 1);

	for(i = 0, index = 0; i < len;) {
		// Is the current word going to span a byte boundary?
		if (index > 3) {
			word = (unsigned char)(src[i] & (0xFF >> index));
			index = (index + 5) % 8;
			word <<= index;
			if ((i + 1) < len)
				word |= src[i + 1] >> (8 - index);

			i++;
		} else {
			word = (unsigned char)(src[i] >> (8 - (index + 5))) & 0x1F;
			index = (index + 5) % 8;
			if (index == 0)
				i++;
		}

		dst += base32Alphabet[word];
	}
	return dst;
}*/

#endif

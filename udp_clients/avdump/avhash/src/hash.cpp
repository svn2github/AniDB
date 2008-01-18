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

//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "hash.h"
#include "hash_wrapper.h"
#include <stdio.h>

struct Digests{
	int len;
	unsigned int* arr;
};
unsigned int hash_create(int type, unsigned __int64 size){
#define COUNT(x,y) if((x&y)==y) len++
#define CREATE(x,y,z,zz) if((x&y)==y){z *obj = new z(zz); /*if( !obj->selfTest() ){ printf("Self test failed!\n"); return 0;}*/ arr[i++] = (unsigned int) obj;}


	int len=0;
	COUNT(type, HASH_TTH);
	COUNT(type, HASH_SHA1);
	COUNT(type, HASH_AICH);
	COUNT(type, HASH_ED2K);
	COUNT(type, HASH_CRC);
	COUNT(type, HASH_MD5);

	if(len<1)
		return 0;
	unsigned int* arr = new unsigned int[len];
	int i=0;
	CREATE(type, HASH_TTH, DigestTTH);
	CREATE(type, HASH_SHA1, DigestSHA);
	CREATE(type, HASH_AICH, DigestAICH, size);
	CREATE(type, HASH_ED2K, DigestED2K);
	CREATE(type, HASH_CRC, DigestCRC);
	CREATE(type, HASH_MD5, DigestMD5);

	Digests* dig = new Digests;
	dig->len = len;
	dig->arr = arr;
	return (unsigned int)dig;
/*	if(type==HASH_TTH)
		return (unsigned int)new DigestTTH();
	if(type==HASH_SHA1)
		return (unsigned int)new DigestSHA();
	if(type==HASH_AICH)
		return (unsigned int)new DigestAICH(size);
	if(type==HASH_ED2K)
		return (unsigned int)new DigestED2K();
	if(type==HASH_CRC)
		return (unsigned int)new DigestCRC();
	if(type==HASH_MD5)
		return (unsigned int)new DigestMD5();
	return 0;*/
}
int hash_update(unsigned int addr, char* buf, int len){
	if(addr==NULL) return -1;
	//Digest* d = (Digest*) addr;
	//return d->update(buf,len);
	//unsigned int arr* = (unsigned int*)addr;
	Digests *d = (Digests*)addr;
	for(int i=0; i<d->len; i++)
		((Digest*)d->arr[i])->update(buf, len);
}
int hash_digest(unsigned int addr, int type, char* sum, int len){
	if(addr==NULL) return -1;

	Digest* dig = NULL;

	Digests *d = (Digests*)addr;
	for(int i=0; i<d->len; i++)
		if(((Digest*)d->arr[i])->type==type)
			dig = (Digest*)d->arr[i];
	if(!dig){
		printf("NO SUCH TYPE: %d\n", type);
		return 0;
	}

	int x = 0;
	if(sum!=NULL && len>0)
		x = dig->digest(sum, len);
	dig->clean();
	return x;
}
void hash_destroy(unsigned int addr){
	Digests *d = (Digests*)addr;
	for(int i=0; i<d->len; i++)
		delete (Digest*)d->arr[i];
	delete d;
}
/*int main(int argc, char *argv[]){
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	//_crtBreakAlloc = 54;

	int len = 1024;
	char* testbuf = new char[1024];

	unsigned int addr = hash_create(HASH_TTH,0);
	hash_update(addr, testbuf, len);
	hash_update(addr, testbuf, len);
	hash_update(addr, testbuf, len);
	hash_digest(addr,0, 0);

	addr = hash_create(HASH_SHA,0);
	hash_update(addr, testbuf, len);
	hash_digest(addr,0,0);

	addr = hash_create(HASH_AICH,0);
	hash_update(addr, testbuf, len);
	hash_digest(addr,0,0);

	delete testbuf;

	system("pause");
	return 0;
}*/
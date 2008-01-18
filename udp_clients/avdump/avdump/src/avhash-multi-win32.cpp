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
#include <process.h>

#include <sstream>
#include <vector>

#include <windows.h>

using namespace std;

#include "avhash.h"

void rb_reset(RingBuffer* rb){	
	rb->write = rb->read = rb->used = 0;
	rb->read2 = rb->used2 = 0;
	rb->active = 1;
}
void rb_init(RingBuffer* rb){
	rb->buffer = (char**) malloc(rb->parts*rb->partsize);
	InitializeCriticalSection(&(rb->cs));
	rb_reset(rb);
}
char* rb_get_write_buffer(RingBuffer* rb){
	if(rb->active){
		while( (rb->parts-rb->used) < 2 || (rb->parts-rb->used2) < 2 )
			Sleep(RB_SLEEP_READ);

		EnterCriticalSection(&(rb->cs));
		rb->used++;
		rb->used2++;
		LeaveCriticalSection(&(rb->cs));

		int tmp = rb->write;
		rb->write = (rb->write+1)%rb->parts;
		return (char*)rb->buffer+(tmp*rb->partsize);
	}
	return 0;
}
char* rb_get_read_buffer(RingBuffer* rb){
	
	while(rb->used < 2)
		Sleep(RB_SLEEP_HASH);

	EnterCriticalSection(&(rb->cs));
	rb->used--;
	LeaveCriticalSection(&(rb->cs));

	int tmp = rb->read;
	rb->read = (rb->read+1)%rb->parts;
	return (char*)rb->buffer+(tmp*rb->partsize);
}
char* rb_get_read_buffer2(RingBuffer* rb){
	
	while(rb->used2 < 2)
		Sleep(RB_SLEEP_HASH);

	EnterCriticalSection(&(rb->cs));
	rb->used2--;
	LeaveCriticalSection(&(rb->cs));

	int tmp = rb->read2;
	rb->read2 = (rb->read2+1)%rb->parts;
	return (char*)rb->buffer+(tmp*rb->partsize);
}
void rb_finish(RingBuffer* rb){
	rb_get_write_buffer(rb);
	rb->active = 0;
}
void rb_destroy(RingBuffer* rb){
	if( rb->buffer )
	{
		free(rb->buffer);
		DeleteCriticalSection(&(rb->cs));
	}
}

typedef struct _MyData {
	RingBuffer *rb;
    FILE* file;
	unsigned __int64 file_size;
	unsigned int hash, hash2, mode;
} MYDATA, *PMYDATA;

DWORD WINAPI ThreadProcRead( LPVOID lpParam ) 
{ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

	PMYDATA pData = (PMYDATA)lpParam;

	fseek(pData->file, 0, SEEK_SET);
	
	unsigned __int64 read = 0, size = pData->file_size;
	int partsize = pData->rb->partsize;
	int rt;

	char * buf;
	unsigned int t0 = GetTickCount();
	unsigned int /*td0 = t0, td1,*/ num=0;

   	while(read < size){
		read += partsize;
		if(read>size)
			rt = partsize-(read-size);
		else rt = partsize;
		buf = rb_get_write_buffer(pData->rb);
		fread(buf, 1, rt, pData->file);
#ifdef RB_DEBUG
		printf("READ : %I64d\t%d\t%x\t%08X\n", read, rt, buf, buf[0]);
#endif
	}
	rb_finish(pData->rb);
#ifdef RB_DEBUG
			printf("READ : DONE\n");
#endif
    return 0; 
}

DWORD WINAPI ThreadProcHash( LPVOID lpParam ) 
{ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

	PMYDATA pData = (PMYDATA)lpParam;

	unsigned int hash = pData->hash;

	if(hash)
	{
		RingBuffer *rb = pData->rb;
		unsigned __int64 read = 0, size = pData->file_size;
		int rt, partsize = pData->rb->partsize;

		char * buf;
		unsigned int t0 = GetTickCount();
		unsigned int td0 = t0, td1, num=0;

		int prog = !BC(pData->mode, HT_SILENT);
		int nocare = BC(pData->mode, HT_NOCARE);
   		while(read < size){
			read += partsize;
			if(read>size)
				rt = partsize-(read-size);
			else rt = partsize;
			buf = rb_get_read_buffer(rb);
#ifdef RB_DEBUG
			printf("HASH1: %I64d\t%d\t%x\t%08X\n", read, rt, buf, buf[0]);
#endif
			hash_update(hash, buf, rt);
#ifndef RB_DEBUG
			if(prog){
				num++;
				if(num%2==0)
				{
					if(nocare)
						printf("\n");
					else
						printf("\r");
					printf("H  %05.2f%%", 100.0*read/pData->file_size);
					td1 = GetTickCount();
					printf("  @ %6.2f MB/s", 2000.0*rt/(td1-td0)/1024/1024);
					printf("  / %6.2f MB/s", 1000.0*read/(td1-t0)/1024/1024);
					td0 = td1;
				}
			}
#endif
		}
		if(nocare)
			printf("\n");
		else
			printf("\r                                          \r");
	}
#ifdef RB_DEBUG
			printf("HASH1: DONE\n");
#endif
    return 0; 
}

DWORD WINAPI ThreadProcHash2( LPVOID lpParam ) 
{ 
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;

	PMYDATA pData = (PMYDATA)lpParam;

	unsigned int hash = pData->hash2;
	unsigned __int64 read = 0, size = pData->file_size;
	RingBuffer *rb = pData->rb;
	int rt, partsize = pData->rb->partsize;

	if(hash){
		char * buf;
		unsigned int t0 = GetTickCount();
		unsigned int td0 = t0, num=0;

   		while(read < size){
			read += partsize;
			if(read>size)
				rt = partsize-(read-size);
			else rt = partsize;
			buf = rb_get_read_buffer2(rb);
#ifdef RB_DEBUG
			printf("HASH2: %I64d\t%d\t%x\t%08X\n", read, rt, buf, buf[0]);
#endif
			hash_update(hash, buf, rt);
		}
	}else{
		while(read < size){
			read += partsize;
			rb_get_read_buffer2(rb);
		}
	}
#ifdef RB_DEBUG
			printf("HASH2: DONE\n");
#endif
    return 0; 
}

void hash_file(hashc* chk, int mode, FILE* file, unsigned __int64 file_size, RingBuffer* rb){

	int m1=0, m2=0;
{
	int m = mode&HT_HASH_MASK;
	int num = 0;
	for(int i=0; i<8; i++)
		if( ((m>>i)&1)==1 )
			num++;
	if(num<1){
		printf("No hash alg selected.\n");
		return;
	}
	int *arr = new int[num];
	int i = 0;
#define SELIF(x) if((m&x)==x) arr[i++] = x;
	SELIF(HASH_TTH)
	SELIF(HASH_ED2K)
	SELIF(HASH_CRC)
	SELIF(HASH_MD5)
	SELIF(HASH_SHA1)
	SELIF(HASH_AICH)

	for(i=0;i<num/2;i++)
		m2 |= arr[i];
	for(;i<num;i++)
		m1 |= arr[i];
	delete arr;
}
//printf("THREADS: %d %d\n", m1, m2);
	unsigned int hash = hash_create(m1, file_size);
	unsigned int hash2 = hash_create(m2, file_size);
	
	PMYDATA pData;
	pData = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
	pData->file = file;
	pData->file_size = file_size;
	pData->hash = hash;
	pData->hash2 = hash2;
	pData->rb = rb;
	pData->mode = mode;
	
{
	#define MAX_THREADS 3
	DWORD dwThreadId[MAX_THREADS];
    HANDLE hThread[MAX_THREADS];
	
	hThread[0] = CreateThread( 
            NULL,              // default security attributes
            0,                 // use default stack size  
            ThreadProcRead,        // thread function 
            pData,             // argument to thread function 
            0,                 // use default creation flags 
            &dwThreadId[0]);   // returns the thread identifier 
 
	if (hThread[0] == NULL) 
	{
		printf("FAILED TO CREATE READ THREAD\n");
		return;
	}

	hThread[1] = CreateThread( 
            NULL,              // default security attributes
            0,                 // use default stack size  
            ThreadProcHash,        // thread function 
            pData,             // argument to thread function 
            0,                 // use default creation flags 
            &dwThreadId[1]);   // returns the thread identifier 
 
	if (hThread[1] == NULL) 
	{
		printf("FAILED TO CREATE HASH THREAD\n");
		return;
	}

	hThread[2] = CreateThread( 
            NULL,              // default security attributes
            0,                 // use default stack size  
            ThreadProcHash2,        // thread function 
            pData,             // argument to thread function 
            0,                 // use default creation flags 
            &dwThreadId[2]);   // returns the thread identifier 
 
	if (hThread[2] == NULL) 
	{
		printf("FAILED TO CREATE HASH THREAD\n");
		return;
	}

    WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);
#ifdef RB_DEBUG
		printf("ALL THREADS: DONE\n");
#endif
    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThread[i]);
    }
#ifdef RB_DEBUG
		printf("ALL THREADS: CLEANED\n");
#endif
	HeapFree(GetProcessHeap(), 0, pData);
}
	rb_reset(rb);
#ifdef RB_DEBUG
		printf("MULTIHASH: RESET\n");
#endif
	m1 |= mode&HT_OTHER_MASK;
	m2 |= mode&HT_OTHER_MASK;

	HASH_COPY(hash, m1, HASH_CRC)
	HASH_COPY(hash, m1, HASH_ED2K)
	HASH_COPY(hash, m1, HASH_MD5)
	HASH_COPY(hash, m1, HASH_SHA1)
	HASH_COPY(hash, m1, HASH_TTH)
	HASH_COPY(hash, m1, HASH_AICH)
	
	HASH_COPY(hash2, m2, HASH_CRC)
	HASH_COPY(hash2, m2, HASH_ED2K)
	HASH_COPY(hash2, m2, HASH_MD5)
	HASH_COPY(hash2, m2, HASH_SHA1)
	HASH_COPY(hash2, m2, HASH_TTH)
	HASH_COPY(hash2, m2, HASH_AICH)
#ifdef RB_DEBUG
		printf("MULTIHASH: COPIED\n");
#endif

	if(hash) hash_destroy(hash);
	if(hash2) hash_destroy(hash2);
#ifdef RB_DEBUG
		printf("MULTIHASH: DONE\n");
#endif
}

#endif

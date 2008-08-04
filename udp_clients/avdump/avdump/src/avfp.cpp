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

#ifdef ENABLE_FP

#include "avfp.h"
#include "libfooid/fooid.h"
#include "mpg123/mpg123.h"

#include <iostream>

using namespace std;

char *fingerprint;
int foosize = 0;

t_fooid * fid = 0;
int duration;

void avfp_init(){
	mpg123_init();
}
void avfp_destroy(){
	mpg123_clean();
}

int avfp_decode_mp3(const wchar_t* path, char* fp, int dur){
	duration = dur;
	if(duration==0){
		printf("Zero duration!\n");
		return 0;
	}
	foosize = 0;
	memset(fp, 0, FOOSIZE);
	
	fingerprint = fp;
	decode_file((wchar_t*)path);
	if(foosize==FOOSIZE){
		return FOOSIZE;
	}
	printf("\n-----------------------------------------\nGeneration of fingerprint failed! (%d)\nIt might be too short! (%d vs min 9000)\n-----------------------------------------\n\n", foosize, duration);
	if(fid){
		printf("decoder did not call close\n");
	}
	return 0;
}

int audio_open(struct audio_info_struct *ai)
{
	//printf("audio_open\n");
	if(ai->rate == -1){
		//printf("foosic dummy init\n");
       return(0);
	}
	if(!fid){
		fid = fp_init(ai->rate, ai->channels);
		printf("fingerprint created (%X): %d Hz, %d channels\n", (unsigned int)fid, ai->rate, ai->channels);
		if(!fid)
		return -1;
	}else{
		printf("decoder aborted, free(%X)\n", (unsigned int)fid);
		fp_free(fid);
		fid = NULL;
	}
	return 0;
}

int audio_get_formats(struct audio_info_struct *ai)
{
  return AUDIO_FORMAT_SIGNED_16;
}

int audio_play_samples(struct audio_info_struct *ai,unsigned char *buf,int len)
{
	//printf("audio_play_samples\n");
	if(fid==NULL){
		printf("foosic not inited\n");
		return -1;
	}
 // printf("Play %d bytes\n", len);
  
  int i = fp_feed_short(fid, (short*)buf, len/ai->channels);
  
  //fprintf(stderr,"feed returned -> %d\n", i);
  if(i==0){
	  foosize = fp_getsize(fid);
	  if(foosize==FOOSIZE){
		  //fingerprint = new unsigned char[foosize];//malloc(sizeof(char)*x);
		  //printf("DUR: %d\n", duration);
		  fp_calculate(fid, duration, (unsigned char*)fingerprint);

		  /*for(int x=0; x<424/16;x++){
			  for(int y=0; y<16;y++)
				  printf("%02X ", 0xFF&fingerprint[x*16+y]);
			  printf("\n");
		  }*/
		  //free(buff);
		  //delete fingerprint;

		  printf("fingerprint ok, free(%X)\n", (unsigned int)fid);
		  fp_free(fid);
		  fid = NULL;
		  return -1;
	  }
	  //exit(0);
  }
  return 0;
}

int audio_close(struct audio_info_struct *ai)
{
	//printf("audio_close\n");
	if(fid){
		printf("song too short, free(%X)\n", (unsigned int)fid);
		fp_free(fid);
		fid = NULL;
	}
	return 0;
}

void audio_queueflush(struct audio_info_struct *ai)
{
}
#endif
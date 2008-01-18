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

#include <windows.h>
#include "MPAHeaderInfo/MPAHeader.h"
#include <tchar.h>
#include "MPAHeaderInfo/MPAException.h"

#include "wrappers.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;

#define bitcmp(x,y) (x&y)==y
#define MAX_OVERHEAD_P 0.015
#define MAX_OVERHEAD_B 4.0
bool printOverhead(double a, double b, stringstream &sst){
	b = abs(a-b);
	if(b>a*MAX_OVERHEAD_P){
		sst << " WARNING: Much overhead! (" << setiosflags(ios::fixed) << setprecision(2) << (100*b/a) << "%)" << endl;
		return true;
	}
	if((b/1048576)>MAX_OVERHEAD_B){
		sst << " WARNING: Much overhead! (" << setiosflags(ios::fixed) << setprecision(2) << (b/1048576) << " MB)" << endl;
		return true;
	}
	return false;
}
bool MediaContainer::isVbr(unsigned char * buffer, int len){
	if(!buffer) return false;
	/*printf("BUFS %d\n", len);
	for(size_t x=0; x<len/16/16; x++){
		printf("%06d ", x*16);
		for(size_t y=0; y<16; y++)
			printf("%02X ", buffer[x*16+y]);
		printf("\n");
	}*/
	int frame_size, last_size = 0;
	boolean cbr = true;
	for(int x=0; x<len-1; x++)
		if(buffer[x]==0xFF && (buffer[x+1]&0xE0)==0xE0){
			try{
				//printf("MP3 Frame %06X of %06X\n", x, len);
				CMPAHeader head((BYTE*)(buffer+x));
				frame_size = head.CalcFrameSize();
				if(frame_size<2) continue;
				if(x+frame_size+1<len)//check that it actually was a frame by jumping to the next
					if(buffer[frame_size+x]!=0xFF || (buffer[frame_size+x+1]&0xE0)!=0xE0)
						continue;

				if(last_size>0) cbr &= abs((int)(last_size-frame_size))<2;
				last_size = frame_size;
				//printf("MP3 Frame %06X: %d bytes\n", x, frame_size);
				x += frame_size-1;
			}catch(CMPAException ex){
				//printf("MP3 Frame Exception! (%d @ %06X %06d %02X)\n", ex, x, x, buffer[x]);
			}
		}//else printf("MP3 Frame MISS! (%06X %06d %02X)\n", x, x, buffer[x]);
	return !cbr;
};
int lang(const char* a, const char *b){
	int x = language(a);
	if(x>1) return x;
	return language(b);
}
#define wrong_ar(x)\
{\
	f |= VIDEO_FLAG_WRONGAR;\
	return x;\
}
int getAR(int w, int h, int &f){
	/*float r = 1.0f*w/h;
	if(r<1.25) return GENERAL_OTHER; //if(r<130) wrong_ar(VIDEO_AR_133)
	if(r<1.50) return VIDEO_AR_133; //if(r<147) wrong_ar(VIDEO_AR_133)
	if(r<1.65) return GENERAL_OTHER;
	if(r<1.68) return VIDEO_AR_166;
	if(r<1.70) return GENERAL_OTHER; //if(r<176) wrong_ar(VIDEO_AR_177)
	if(r<1.84) return VIDEO_AR_177; //if(r<184) wrong_ar(VIDEO_AR_177)
	if(r<1.86) return VIDEO_AR_185;
	if(r<1.99) return GENERAL_OTHER;
	if(r<2.01) return VIDEO_AR_200;
	if(r<2.20) return GENERAL_OTHER;
	if(r<2.22) return VIDEO_AR_221;
	if(r<2.34) return GENERAL_OTHER;
	if(r<2.36) return VIDEO_AR_235;
	return GENERAL_OTHER;*/
	if(	(w==4 && h==3) ||
		(w==320 && h==240) ||
		(w==352 && h==240) || //(non-square pixels), native NTSC Video CD - common (nwa)
		(w==352 && h==288) || //(non-square pixels), native PAL Video CD - common (nwa)
		(w==384 && h==288) || //(pelican)
		(w==512 && h==368) ||
		(w==512 && h==384) ||
		(w==576 && h==432) ||
		(w==640 && h==480) )
		return VIDEO_AR_133;
	if(	(w==16 && h==9) ||
		(w==640 && h==352) ||
		(w==640 && h==360) ||
		(w==640 && h==368) || //(pelican)
		(w==704 && h==384) ||
		(w==704 && h==392) ||
		(w==704 && h==396) ||
		(w==704 && h==400) ||
		(w==720 && h==400) || //(nwa: a bit off from actual 16:9, probably kept for mod16)
		(w==960 && h==540) || //(nwa: a bit off from actual 16:9, probably kept for mod16)
		(w==1024 && h==576) || //(common under the tards)
		(w==1280 && h==720) ||
		(w==1920 && h==1080) ||
		(w==1920 && h==1088) )
		return VIDEO_AR_177;
	return GENERAL_UNKNOWN;
}
inline bool isAnamorphic(int pw, int ph, int dw, int dh){
	if(dw>0||dh>0){
		return 1000*pw/ph!=1000*dw/dh;
	}
	return false;
}
inline int getChanid(int ch){
	switch(ch){
		case 1: return AUDIO_CHAN_MONO;
		case 2: return AUDIO_CHAN_STEREO;
		case 4: return AUDIO_CHAN_DOLBY;
		case 5:
		case 6: return AUDIO_CHAN_5_1_S;
		case 7: return AUDIO_CHAN_6_1_S;
		case 8: return AUDIO_CHAN_7_1_S;
		default: return GENERAL_UNKNOWN; //unknown - 100->other
	}
}
void arAndFlags(avmf::TrackVideo *tv, int &ar, int &flags){
	int w = tv->image.pw, h = tv->image.ph;
	if(isAnamorphic(w,h, tv->image.dw,tv->image.dh))
		flags |= VIDEO_FLAG_ANAMORPHIC;
	else if(tv->image.ar>0){
		int w2 = h * tv->image.ar;
		if(w != w2){
			flags |= VIDEO_FLAG_ANAMORPHIC;
			w = w2;
		}
	}
	if(tv->image.dw>0 && tv->image.dh>0){
		w = tv->image.dw;
		h = tv->image.dh;
	}
	ar = getAR(w,h,flags);
	if(tv->sample.rate<1) flags |= VIDEO_FLAG_VFR;
}
void printTime(stringstream &sst, float duration, const char *str, bool endnl=true, bool print_sec=true){
	int min = (int)(duration+0.5);
	int sec = (min%60);
	int hou = min/3600;
	min = min/60 - hou*60;
	sst << setfill('0');	
	sst << str << setw(2) << hou << ":" << setw(2) << min << ":" << std::setw(2) << sec;
	if(print_sec) sst << " (" << duration << ")";
	if(endnl) sst << endl;
	sst << setfill(' ');
}
void printTimeXml(stringstream &sst, double dur){
	sst << "<dur value=\"" << dur;
	printTime(sst, (float)dur, "\">", false, false);
	sst << "</dur>" << endl;
}
static char *BYTETB[] ={" B"," KB"," MB"," GB"," TB"," PB"," EB"};
void printSize(stringstream &sst, double d, const char *str, bool endnl=true, int p=0, double orig=0){
	if(p==0) orig = d;
	if( (d>=0 && d<1000) || (d<=0 && d>-1000) ){
		sst << str << setiosflags(ios::fixed) << setprecision(2) << d << BYTETB[p] << " (" << (__int64)orig << ")";
		if(endnl) sst << endl;
	}
	else printSize(sst, d/1024, str, endnl, p+1, orig);
}

int getCid(avmf::Track *t){
	switch(t->track_type()){
		case TRACK_TYPE_VIDEO:{
			avmf::TrackVideo *tv = (avmf::TrackVideo*)t;
			/*if(tv->codecid.length()>0)
				return mkvcodecid(tv->codecid.c_str(), tv->encoder.length()>0?tv->encoder.c_str():tv->fourcc.c_str());
			return userdata(msfourcc(tv->fourcc.c_str()), tv->encoder.c_str());*/
			if(tv->fourcc.length()>0)
				return userdata(msfourcc(tv->fourcc.c_str()), tv->encoder.c_str());
			return mkvcodecid(tv->codecid.c_str(), tv->encoder.c_str());
							  }break;
		case TRACK_TYPE_AUDIO:{
			avmf::TrackAudio *ta = (avmf::TrackAudio*)t;
			int cid;
			if(ta->codecid.length()>0)
				cid = mkvcodecid(ta->codecid.c_str(), ta->twocc.c_str());
			else cid = msfourcc(ta->twocc.c_str());
			if(cid==A_MP3_CBR && ta->mode=="VBR")
				cid = A_MP3_VBR;
			return cid;
							  }break;
		case TRACK_TYPE_SUBTITLE:{
			avmf::TrackSubtitle *ts = (avmf::TrackSubtitle*)t;
			return mkvcodecid(ts->codecid.c_str(), ts->codecid.c_str());
								 }break;
	}
	return 1;
}
void getRate(avmf::TrackTimed *t, float &rate, unsigned __int64 &size, float dur){
	rate = t->bitrate;
	size = t->size;

	if(t->duration>0)
		dur = t->duration;

	if(size<1){
		rate = rate/1000.0f;
	}else
		rate = 8*t->size/1000/dur;
}
void xmlVideoTrack(stringstream &sst, avmf::TrackVideo *tv, float dur){
	int ar=0,flags=0;
	arAndFlags(tv, ar, flags);
	
	int cid = getCid(tv);
	
	float rate; unsigned __int64 size;
	getRate(tv, rate, size, dur);
	
	sst << "\t<vid>" << endl;
	if(tv->title.length()>0)
		sst << "\t\t<nam>" << tv->title << "</nam>" << endl;
	sst << "\t\t<byt>" << (unsigned int)size << "</byt>" << endl;
    sst << "\t\t";
	printTimeXml(sst, tv->duration);
	sst << "\t\t<bit>" << rate <<"</bit>" << endl;
	sst << "\t\t<cod id=\""<< cid <<"\">" << str_codec(cid) << "</cod>" << endl;
	sst << "\t\t<fcc>" << tv->fourcc << "</fcc>" << endl;
	if(tv->encoder.length()>0)
		sst << "\t\t<pri>" << tv->encoder << "</pri>" << endl;
	sst << "\t\t<var id=\"" << ar << "\">" << str_ar(ar) << "</var>" << endl;
	sst << "\t\t<pre>" << tv->image.pw << "x" << tv->image.ph << "</pre>" << endl;
	sst << "\t\t<dre>" << tv->image.dw << "x" << tv->image.dh << "</dre>" << endl;
	sst << "\t\t<fps>" << setprecision(3) << tv->sample.rate << "</fps>" << endl;
	sst << "\t\t<fla id=\"" << flags << "\">";
	if(bitcmp(flags,VIDEO_FLAG_WRONGAR)){
		sst << "wrong ar";
		if(bitcmp(flags,VIDEO_FLAG_ANAMORPHIC)) sst << ", anamorphic";
		if(bitcmp(flags,VIDEO_FLAG_VFR)) sst << ", vfr";
	}else if(bitcmp(flags,VIDEO_FLAG_ANAMORPHIC)){
		sst << "anamorphic";
		if(bitcmp(flags,VIDEO_FLAG_VFR)) sst << ", vfr";
	}else if(bitcmp(flags,VIDEO_FLAG_VFR)) sst << "vfr";
	sst << "</fla>" << endl;
	sst << "\t</vid>" << endl;
}
void xmlAudioTrack(stringstream &sst, avmf::TrackAudio *ta, float dur){
	int li = lang(ta->language.c_str(), ta->title.c_str());
	int ci = getChanid(ta->channels);

	int cid = getCid(ta);

	float rate; unsigned __int64 size;
	getRate(ta, rate, size, dur);

	sst << "\t<aud default=\"" << ta->flag_default << "\">" << endl;
	if(ta->title.length()>0)
		sst << "\t\t<nam>" << ta->title << "</nam>" << endl;
	sst << "\t\t<lan id=\"" << li << "\">" << str_lang(li) << "</lan>" << endl;
	sst << "\t\t<byt>" << (unsigned int)size << "</byt>" << endl;
	sst << "\t\t";
	printTimeXml(sst, ta->duration);
	sst << "\t\t<bit>" << rate <<"</bit>" << endl;
	sst << "\t\t<cod id=\"" << (cid&0xFF) << "\">" << str_codec(cid) << "</cod>" << endl;
	sst << "\t\t<tcc>" << ta->twocc << "</tcc>" << endl;
	sst << "\t\t<cha id=\"" << ci << "\" value=\"" << ta->channels << "\">" << str_chan(ci) << "</cha>" << endl;
	sst << "\t\t<sam>" << (int)ta->sample.rate << "</sam>" << endl;
	sst << "\t</aud>" << endl;
}
void xmlSubtitleTrack(stringstream &sst, avmf::TrackSubtitle *ts){
	int li = lang(ts->language.c_str(), ts->title.c_str());
	int flags = 0;
	
	int cid = getCid(ts);

	if(cid>S_USF) flags = 1<<7;
	else if(cid>S_SRT) flags = 1<<6;
	else if(cid>S_NONEX) flags = 1<<8;
	sst << "\t<sub default=\"" << ts->flag_default << "\">" << endl;
	if(ts->title.length()>0)
		sst << "\t\t<nam>" << ts->title << "</nam>" << endl;
	sst << "\t\t<lan id=\"" << li  << "\">" << str_lang(li) << "</lan>" << endl;
	sst << "\t\t<typ>" << ts->codecid << "</typ>" << endl;
	sst << "\t\t<byt>" << ts->size << "</byt>" << endl;
	sst << "\t\t<fla id=\"" << flags << "\">" << str_sub(cid) << "</fla>" << endl;
	sst << "\t</sub>" << endl;
}
float dumpVideoTrack(stringstream &sst, avmf::TrackVideo *tv, float dur, unsigned __int64 &size){
	int ar=0,flags=0;
	arAndFlags(tv, ar, flags);
	int cid = getCid(tv);

	sst << " video";
	if(tv->title.length()>0) sst << " (" << tv->title << ")";
	sst << endl;
	
	float rate;
	getRate(tv, rate, size, dur);
	
	if(tv->language.length()>0)
		sst << "  lang: " << tv->language << " (" << language(tv->language.c_str()) << ")" << endl;
	else sst << "  lang: Unknown (1)" << endl;

	sst << "  codc: ";
	if(tv->codecid.length()>0)
	{
		sst << tv->codecid;
		if(tv->fourcc.length()>0)
			sst << "/" << tv->fourcc;
	}
	else
		sst << tv->fourcc;

	if(tv->encoder.length()>0)
		sst << " (" << tv->encoder << ")";
	sst << " -> " << str_codec(cid) << " (" << (cid&0xFF) << ")" << endl;
	sst << "  reso: " << tv->image.pw << "x" << tv->image.ph;
	if( tv->image.dw>0 || tv->image.dh)
	{
		sst << " (" << tv->image.dw << "x" << tv->image.dh << ")";
	}
	else if(tv->image.ar>0)
	{
		sst << " (" << tv->image.ar << ")";
	}
	sst << " -> " << str_ar(ar);
	if(bitcmp(flags, VIDEO_FLAG_ANAMORPHIC)) //(flags&VIDEO_FLAG_ANAMORPHIC)==VIDEO_FLAG_ANAMORPHIC)
		sst << " anamorphic";
	if((flags&VIDEO_FLAG_WRONGAR)==VIDEO_FLAG_WRONGAR)
		sst << " WRONG AR";
	sst << endl;
	sst << "  fram: ";
	if(tv->sample.rate==VIDEO_MODE_HYBRID)
		sst << "hybrid (vfr)" << endl;
	else if(tv->sample.rate==VIDEO_MODE_VFR)
		sst << "vfr" << endl;
	else
		sst << setprecision(6) << tv->sample.rate << " fps" << endl;
		
	sst << "  rate: " << (int)(rate+0.5f) << " kbps (" << rate << ")" << endl;
	//sst << "  dura: " << tv->dur << " sec" << endl;
	printTime(sst, tv->duration, "  dura: ");
	printSize(sst, size, "  size: ");
	//sst << "  size: " << (unsigned int)tv->size << " B (" << tv->size/1024/1024 << " MB)" << endl;
	return rate;
}
float dumpAudioTrack(stringstream &sst, avmf::TrackAudio *ta, float dur, unsigned __int64 &size){
	int cid = getCid(ta);

	sst << " audio";
	if(ta->title.length()>0) sst << " (" << ta->title << ")";
	sst << endl;
	
	float rate;
	getRate(ta, rate, size, dur);
	
	if(ta->language.length()>0){
		int lid = language(ta->language.c_str());
		sst << "  lang: " << ta->language << " -> " << str_lang(lid) << " (" << lid << ")"  << endl;
	}
	else sst << "  lang: Unknown (1)" << endl;

	sst << "  codc: ";
	if(ta->codecid.length()>0)
		sst << ta->codecid;
	else sst << ta->twocc;

	sst << " -> " << str_codec(cid) << " (" << (cid&0xFF) << ")" << endl;
	sst << "  chan: " << ta->channels << " -> " << str_chan(getChanid(ta->channels));
	if((ta->channels==5 || ta->channels==6) && rate<100)
		sst << " - WARNING: This is most likely stereo (99% certainty). Please check before creqing!";
	sst << endl;
	sst << "  samp: " << (int)ta->sample.rate << " Hz" << endl;
	sst << "  rate: " << (int)(rate+0.5f) << " kbps (" << rate << ")" << endl;
	//sst << "  dura: " << ta->duration << " sec" << endl;
	printTime(sst, ta->duration, "  dura: ");
	printSize(sst, size, "  size: ");
	//sst << "  size: " << (unsigned int)ta->size << " B (" << ta->size/1024/1024 << " MB)" << endl;
	return rate;
}
void dumpSubtitleTrack(stringstream &sst, avmf::TrackSubtitle *ts){
	int cid = getCid(ts);

	sst << " subtitles";
	if(ts->title.length()>0) sst << " (" << ts->title << ")";
	sst << endl;

	if(ts->language.length()>0){
		int lid = language(ts->language.c_str());
		sst << "  lang: " << ts->language << " -> " << str_lang(lid) << " (" << lid << ")"  << endl;
	}
	else sst << "  lang: Unknown (1)" << endl;

	//sst << "  lang: " << ts->language << " (" <<  << ")" << endl;
	sst << "  type: " << ts->codecid << " -> " << str_codec(cid) << " (" << (cid&0xFF) << ")" << endl;
	//sst << "  size: " << (unsigned int)ts->size << " B (" << ts->size/1024 << " KB)" << endl;
	printSize(sst, ts->size, "  size: ");
}
void toXML(MediaContainer *mc, stringstream &sst){
	avmf::Container c;
	mc->mkAVMF(&c);

	int tracks = c.tracks.size();
	sst << "<avinfo version=\"2\" code=\""<< mc->code() << "\">" << endl;
	sst << "\t<ext>" << mc->m_ext << "</ext>" << endl;
	sst << "\t";
	printTimeXml(sst, c.duration);	
	
	for(int i=0; i<c.tracks.size(); i++){
		avmf::Track *t = c.tracks.at(i);
		switch(t->track_type()){
			case TRACK_TYPE_VIDEO: xmlVideoTrack(sst, (avmf::TrackVideo*)t, c.duration); break;
			case TRACK_TYPE_AUDIO: xmlAudioTrack(sst, (avmf::TrackAudio*)t, c.duration); break;
			case TRACK_TYPE_SUBTITLE: xmlSubtitleTrack(sst, (avmf::TrackSubtitle*)t); break;
		}
	}
	sst << "</avinfo>" << endl;
}
bool printWarning(int code, stringstream &sst){
	if(code==0) return false;
	if(code<0)
		sst << " WARNING: This file is most likely corrupt. The numbers cannot be trusted. (" << code << ")" << endl;
	else 
		sst << " WARNING: It's something funny with this file. Please check the numbers before creqing. (" << code << ")" << endl;
	return true;
}

void toOld(MediaContainer *mc, stringstream &sst){
	avmf::Container c;
	mc->mkAVMF(&c);

	int tracks = c.tracks.size();
	float duration = c.duration;
	for(int i=0; i<tracks; i++)
		if(c.tracks.at(i)->track_type()==TRACK_TYPE_VIDEO && c.tracks.at(i)->duration>0)
			duration = c.tracks.at(i)->duration;
	
	printTime(sst, c.duration, " Duration: ", false);
	if(abs(duration-c.duration)>0.5)
		sst << " - WARNING: Differs from video duration! ("  << setiosflags(ios::fixed) << setprecision(2) << abs(duration-c.duration) <<" sec)";
	sst << endl;

	mylong accbytes = 0;
	unsigned __int64 size;
	float rate = 0;
	for(int i=0; i<tracks; i++){
		avmf::Track *t = c.tracks.at(i);
		
		sst << " Track #" << (i+1) << ":";
		if(t->flag_default) sst << " default";
					
		switch(t->track_type()){
			case TRACK_TYPE_VIDEO: rate += dumpVideoTrack(sst, (avmf::TrackVideo*)t, c.duration, size); break;
			case TRACK_TYPE_AUDIO: rate += dumpAudioTrack(sst, (avmf::TrackAudio*)t, c.duration, size); break;
			case TRACK_TYPE_SUBTITLE: dumpSubtitleTrack(sst, (avmf::TrackSubtitle*)t); break;
		}
		accbytes += (size_t)t->size; //or size
	}

	sst << " Sizes: (check sanity)" << endl;
	printSize(sst, mc->m_bytes, "  disk: ");
	
	if(c.misc_size>0)
		printSize(sst, c.misc_size, "  atts: ", true);
	
	double rcs = rate*duration/8*1000;
	printSize(sst, accbytes, "  trac: ", false);
	sst << " [based on track size]" << endl;	
	printSize(sst, rcs, "  bitr: ", false);
	sst << " [based on bitrate]" << endl;
	
	double tdiff = (double)(mc->m_bytes-accbytes-c.misc_size);
	double bdiff = (double)(mc->m_bytes-rcs-c.misc_size);
	printSize(sst, tdiff, "  tdif: ", false);
	sst << " " << ((__int64)(10000*tdiff/mc->m_bytes))/100.0f << "%" <<  endl;
	printSize(sst, bdiff, "  bdif: ", false);
	sst << " " << ((__int64)(10000*bdiff/mc->m_bytes))/100.0f << "%" << endl;
	printWarning(mc->code(), sst);
}
void toShort(MediaContainer *mc, stringstream &sst){
	//size_t accbytes = 0;
	avmf::Container c;
	mc->mkAVMF(&c);

	int tracks = c.tracks.size();//must be called before GetDuration
	float accrate = 0, duration = c.duration;
	for(int i=0; i<tracks; i++)
		if(c.tracks.at(i)->track_type()==TRACK_TYPE_VIDEO && c.tracks.at(i)->duration>0)
			duration = c.tracks.at(i)->duration;

	if(tracks>9)
		printTime(sst, duration, "  0.D: ");
	else printTime(sst, duration, " 0.D: ");

	for(int i=0; i<tracks; i++){
		avmf::Track *t = c.tracks.at(i);
		//accbytes += (size_t)t->size;
		
		if(tracks>9)
			sst << " " << setw(2) << (i+1);
		else sst << " " << (i+1);

		int cid = getCid(t);
		switch(t->track_type()){
			case TRACK_TYPE_VIDEO:{

				avmf::TrackVideo *tv = (avmf::TrackVideo*)t;
				float rate; unsigned __int64 size;
				getRate(tv, rate, size, duration);

				int ar=0,flags=0;
				arAndFlags(tv, ar, flags);
				sst << ".V: " << tv->image.pw << "x" << tv->image.ph << " (" << str_ar(ar);
				if(bitcmp(flags,VIDEO_FLAG_ANAMORPHIC)) sst << " anamorphic";
				sst << ") " << str_codec(cid) << " ";
				sst << rate << " kbps ";
				if(tv->sample.rate<VIDEO_MODE_UNKNOWN) sst << "VFR";
				else sst << setprecision(6) << tv->sample.rate << " fps";
				if(tv->encoder.length()>0)
					sst << "  [" << tv->encoder << "]";
				else
					sst << "  [" << tv->fourcc << "]";
				if(tv->title.length()>0)
					sst << "  <" << tv->title << ">";
				sst << endl;
				accrate += rate;
							}break;
			case TRACK_TYPE_AUDIO:{
				avmf::TrackAudio *ta = (avmf::TrackAudio*)t;
				float rate; unsigned __int64 size;
				getRate(ta, rate, size, duration);

				sst << ".A: " << str_lang(lang(ta->language.c_str(), ta->title.c_str())) << " " << str_codec(cid) << " ";
				sst << rate << " kbps " << str_chan(getChanid(ta->channels));
				if((ta->channels==5 || ta->channels==6) && rate<100)
					sst << " FAKE? PLEASE CHECK!";
				sst << "  [" << ta->twocc << "]";
				if(ta->title.length()>0)
					sst << "  <" << ta->title << ">";
				sst << endl;
				accrate += rate;
							}break;
			case TRACK_TYPE_SUBTITLE:{
				avmf::TrackSubtitle *ts = (avmf::TrackSubtitle*)t;
				sst << ".S: " << str_lang(lang(ts->language.c_str(), ts->title.c_str())) << " " << str_sub(cid) << " subs";// << " (" << (int)(ts->size/1024+0.5) << " KB)";
				if(ts->title.length()>0) sst << "  <" << ts->title << ">";
				sst << endl;
							}break;
			default:
				sst << ".U: Unknown/Error" << endl;
		}
	}
	int misc_size = c.misc_size;
	//if(!printOverhead(mc->m_bytes-misc_size,accbytes,sst))
	//printf("%f %f\n", accrate, duration);
		printOverhead(mc->m_bytes-misc_size,((1000*accrate/8)*duration),sst);
	printWarning(mc->code(), sst);
}
void toList(MediaContainer *mc, stringstream &sst){
	/*for(int i=0; i<mc->GetTrackCount(); i++){
		GenericTrack g = mc->GetGenericTrack(i);
		//if(ts->type_id==TRACK_VIDEO&&strcmp(ts->codec,"V_MS/VFW/FOURCC")==0)
			sst << "\t" << ts->codec;
			if(ts->type_id==TRACK_VIDEO){
				VideoTrack v = mc->GetVideoTrack(i);
				if(strlen(v.private_frame)>0)
					sst<< " (" << v.private_frame << ")";
			}
	}*/
	avmf::Container c;
	mc->mkAVMF(&c);
	//sst << c.muxing_application;
	for(size_t i=0; i<c.tracks.size(); i++)
		if(c.tracks.at(i)->track_type()==TRACK_TYPE_VIDEO){
			//sst << ((avmf::TrackVideo*)c.tracks.at(i))->private_data << "\t" << ((avmf::TrackVideo*)c.tracks.at(i))->sample.rate;
			avmf::TrackVideo *v = (avmf::TrackVideo*)c.tracks.at(i);
			sst << v->codecid;
			if(v->fourcc.length()>0)
				sst << "/" << v->fourcc;
			sst << "\t" << v->image.pw << "x" << v->image.ph;
			sst << "\t" << v->image.dw << "x" << v->image.dh;
			//if(v->encoder.length()>0)
				sst << "\t" << v->encoder;
			sst << "\t" << v->settings;
		}
		/*if(c.tracks.at(i)->track_type()==TRACK_TYPE_AUDIO){
			sst << ((avmf::TrackAudio*)c.tracks.at(i))->codecid;
			string two = ((avmf::TrackAudio*)c.tracks.at(i))->twocc;
			if(two.length()>0)
				sst << "/" << two;
			sst << "\t";
		}*/
		//sst << c.timecodescale << "\t";
	//sst << endl;
}
int MediaContainer::toString(char* buf, unsigned int size, int format){
	stringstream sst;
	switch(format){
		case FORMAT_OLD: toOld(this, sst); break;
		case FORMAT_XML: toXML(this, sst); break;
		case FORMAT_SHORT: toShort(this, sst); break;
		case FORMAT_LIST: toList(this, sst); break;
		default: return -3;
	}
	string s = sst.str();
	int len = min(size-1,s.length());
	memcpy(buf, s.c_str(), len);
	return len;
}
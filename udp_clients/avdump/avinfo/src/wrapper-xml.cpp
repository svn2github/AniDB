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

#include "tinyxml/tinyxml.h"
#include "wrappers.h"

#include <iostream>

using namespace std;

#define XML_AVMF_RETURN_CODE "rcode"
#define XML_AVMF_MAIN_TAG "avmf"
#define XML_FILE_MISC_SIZE "extra_size"
#define XML_FILE_EXTENSION "extension"
#define XML_FILE_DURATION "duration"
#define XML_FILE_CHAPTERS "chapters"
#define XML_FILE_WRITING_APP "app"
#define XML_FILE_MUXING_APP "lib"
#define XML_FILE_DATE "date"
#define XML_FILE_TITLE "title"
#define XML_TRACK_ENCODER_SETTINGS "settings"
#define XML_TRACK_SAMPLES "sample_count"
#define XML_TRACK_PRIVATE_DATA "private"
#define XML_TRACK_DEFAULT_FLAG "default"
#define XML_TRACK_CODECID "identifier"
#define XML_TRACK_DURATION "duration"
#define XML_TRACK_LANGUAGE "language"
#define XML_TRACK_BITRATE "bitrate"
#define XML_TRACK_ENCODER "encoder"
#define XML_TRACK_TITLE "title"
#define XML_TRACK_SIZE "size"
#define XML_TRACK_UID "uid"
#define XML_TRACK_TYPE_SUB "subtitles"
#define XML_TRACK_TYPE_VIDEO "video"
#define XML_TRACK_TYPE_AUDIO "audio"
#define XML_VIDEO_DISPLAY_RESOLUTION "res_d"
#define XML_VIDEO_PIXEL_RESOLUTION "res_p"
#define XML_VIDEO_STRUCTURE "structure"
#define XML_VIDEO_FOURCC "fourcc"
#define XML_VIDEO_CHROMA "chroma"
#define XML_VIDEO_FPS "fps"
#define XML_VIDEO_AR "ar"
#define XML_AUDIO_SAMPLING_RATE "sampling_rate"
#define XML_AUDIO_CHANNELS "channels"
#define XML_AUDIO_BITRATE_MODE "mode"
#define XML_AUDIO_TWOCC "twocc"
#define XML_GENERAL_CMT "li"

string clean(string& old){
	string str; char c;
	str.begin();
	for(size_t i=0; i<old.length(); i++){
		c = old.at(i);
		switch(c){
			//xml
			case '&': str.append("&amp;");
				break;
			case '<': str.append("&lt;");
				break;
			case '>': str.append("&gt;");
				break;
			//anidb api
			case '|': str.append("&#124;");
				break;
			case'\r':
				break;
			case'\n': str.append(" ");
				break;
			default: str += c;
		}
	}
	return str;
}
void tinyAdd(TiXmlElement *e, const char* tag, string value, bool attr=false, bool cdata=false){
	if(value.length()>0 && value.length()<1024){
		if(attr){
			e->SetAttribute(tag, value.c_str());
		}else{
			TiXmlElement * tmp = new TiXmlElement( tag );
			if(cdata){
				char buffer[2048];
				sprintf(buffer, "<![CDATA[%s]]>", value.c_str());
				tmp->LinkEndChild(new TiXmlText(buffer));
			}else{
				tmp->LinkEndChild(new TiXmlText(clean(value).c_str()));
			}
			e->LinkEndChild(tmp);
		}
	}
}
void tinyAdd(TiXmlElement *e, const char* tag, int value, bool attr=false){
	if(value!=0){
		if(attr){
			e->SetAttribute(tag, value);
		}else{
			char buffer[255];
			tinyAdd(e, tag, itoa(value, buffer, 10));
		}
	}
}
void tinyAdd(TiXmlElement *e, const char* tag, unsigned __int64 value, bool attr=false){
	if(value!=0){
		//if(attr){
		//	e->SetAttribute(tag, value);
		//}else{
			char buffer[255];
			sprintf(buffer, "%I64d", value);
			tinyAdd(e, tag, buffer);
		//}
	}
}
void tinyAdd(TiXmlElement *e, const char* tag, unsigned int value, bool attr=false){
	if(value!=0){
		if(attr){
			e->SetAttribute(tag, value);
		}else{
			char buffer[255];
			sprintf(buffer, "%x", value);
			tinyAdd(e, tag, buffer);
		}
	}
}
void tinyAdd(TiXmlElement *e, const char* tag, float value, bool attr=false){
	if(value!=0){
		if(attr){
			e->SetDoubleAttribute(tag, value);
		}else{
			char buffer[255];
			sprintf(buffer, "%0.3f", value);
			tinyAdd(e, tag, buffer);
		}
	}
	//tinyAdd(e, tag, (int)(1000*value));
}
void tinyAdd(TiXmlElement *e, const char* tag, vector<string> &t){
	if(t.size()<1)
		return;
	if(t.size()<2)
		tinyAdd(e, XML_TRACK_PRIVATE_DATA, t.at(0));
	else{
		TiXmlElement * f = new TiXmlElement( tag );
		for(int i=0; i<t.size(); i++){
			tinyAdd(f, XML_GENERAL_CMT, t.at(i));
		}
		e->LinkEndChild(f);
	}
}
void tinyAddRes(TiXmlElement *e, const char* tag, int w, int h){
	if(w>0 || h>0){
		TiXmlElement * r = new TiXmlElement( tag );
		r->SetAttribute("width", w);
		r->SetAttribute("height", h);
		e->LinkEndChild(r);
	}
}
void avmfVideoTrack(TiXmlElement *e, avmf::TrackVideo *v){
	TiXmlElement * e_vid = new TiXmlElement( XML_TRACK_TYPE_VIDEO );

	tinyAdd(e_vid, XML_TRACK_TITLE, v->title);	
	tinyAdd(e_vid, XML_TRACK_SIZE, v->size);
	tinyAdd(e_vid, XML_TRACK_DURATION, v->duration);
	tinyAdd(e_vid, XML_TRACK_BITRATE, v->bitrate);
	tinyAdd(e_vid, XML_TRACK_CODECID, v->codecid);
	tinyAdd(e_vid, XML_VIDEO_FOURCC, v->fourcc);
	tinyAdd(e_vid, XML_VIDEO_FPS, v->sample.rate);
	tinyAdd(e_vid, XML_TRACK_ENCODER, v->encoder);
	tinyAdd(e_vid, XML_TRACK_ENCODER_SETTINGS, v->settings);
	//tinyAdd(e_vid, XML_TRACK_PRIVATE_DATA, v->private_data);
	tinyAdd(e_vid, XML_TRACK_PRIVATE_DATA, v->private_vector);
	tinyAdd(e_vid, XML_TRACK_SAMPLES, v->sample.count);
	tinyAdd(e_vid, XML_VIDEO_CHROMA, v->image.chroma);
	tinyAdd(e_vid, XML_VIDEO_STRUCTURE, v->image.structure);
	tinyAdd(e_vid, XML_VIDEO_AR, v->image.ar);
	
	tinyAddRes(e_vid, XML_VIDEO_PIXEL_RESOLUTION, v->image.pw, v->image.ph);
	tinyAddRes(e_vid, XML_VIDEO_DISPLAY_RESOLUTION, v->image.dw, v->image.dh);

//	tinyAdd(e_vid, XML_TRACK_UID, v->uid, true);
	
	e->LinkEndChild(e_vid);
}
void avmfAudioTrack(TiXmlElement *e, avmf::TrackAudio *a){
	TiXmlElement * e_aud = new TiXmlElement( XML_TRACK_TYPE_AUDIO );

	tinyAdd(e_aud, XML_TRACK_SIZE, a->size);
	tinyAdd(e_aud, XML_TRACK_DURATION, a->duration);
	tinyAdd(e_aud, XML_TRACK_BITRATE, a->bitrate);
	tinyAdd(e_aud, XML_TRACK_TITLE, a->title);
	tinyAdd(e_aud, XML_TRACK_LANGUAGE, a->language);
	tinyAdd(e_aud, XML_TRACK_PRIVATE_DATA, a->private_vector);
	/*if(a->private_vector.size()==1)
		tinyAdd(e_aud, XML_TRACK_PRIVATE_DATA, a->private_data);
	else if(a->private_vector.size()>1){
		for(int i=0; i<a->private_vector.size(); i++)
			
	}*/
	tinyAdd(e_aud, XML_TRACK_CODECID, a->codecid);	
	tinyAdd(e_aud, XML_AUDIO_TWOCC, a->twocc);
	tinyAdd(e_aud, XML_AUDIO_CHANNELS, a->channels);
	tinyAdd(e_aud, XML_AUDIO_SAMPLING_RATE, a->sample.rate);
	tinyAdd(e_aud, XML_AUDIO_BITRATE_MODE, a->mode);
	tinyAdd(e_aud, XML_TRACK_SAMPLES, a->sample.count);
	
//	tinyAdd(e_aud, XML_TRACK_UID, a->uid, true);
	tinyAdd(e_aud, XML_TRACK_DEFAULT_FLAG, a->flag_default, true);
	
	e->LinkEndChild(e_aud);
}
void avmfSubtitleTrack(TiXmlElement *e, avmf::TrackSubtitle *s){
	TiXmlElement * e_sub = new TiXmlElement( XML_TRACK_TYPE_SUB );

	tinyAdd(e_sub, XML_TRACK_SIZE, s->size);	
	tinyAdd(e_sub, XML_TRACK_TITLE, s->title);
	tinyAdd(e_sub, XML_TRACK_LANGUAGE, s->language);
	//tinyAdd(e_sub, XML_TRACK_PRIVATE_DATA, s->private_data);
	tinyAdd(e_sub, XML_TRACK_PRIVATE_DATA, s->private_vector);
	tinyAdd(e_sub, XML_TRACK_CODECID, s->codecid);

//	tinyAdd(e_sub, XML_TRACK_UID, s->uid, true);
	tinyAdd(e_sub, XML_TRACK_DEFAULT_FLAG, s->flag_default, true);

	e->LinkEndChild(e_sub);
}
int MediaContainer::buildXML(addr_t tx){
	if(tx==NULL) return -1;
	
//	if( !memcmp(m_ext, "mp3", 3))
//		return buildXMLost(tx);

	TiXmlElement *e = (TiXmlElement *)tx;
	avmf::Container c;
	mkAVMF(&c);
	
	TiXmlElement * e_main = new TiXmlElement( XML_AVMF_MAIN_TAG );
	
	tinyAdd(e_main, XML_AVMF_RETURN_CODE, code(), true);
	tinyAdd(e_main, XML_FILE_MISC_SIZE, c.misc_size);
	tinyAdd(e_main, XML_FILE_DURATION, c.duration);
	
	tinyAdd(e_main, XML_FILE_EXTENSION, m_ext);
	tinyAdd(e_main, XML_FILE_DATE, c.date);
	tinyAdd(e_main, XML_FILE_WRITING_APP, c.writtenby);
	tinyAdd(e_main, XML_FILE_MUXING_APP, c.muxedby);	
	tinyAdd(e_main, XML_FILE_CHAPTERS, c.chapters);
	
	for(int i=0; i<c.tracks.size(); i++){
		avmf::Track *t = c.tracks.at(i);
		switch(t->track_type()){
			case TRACK_TYPE_VIDEO: avmfVideoTrack(e_main, (avmf::TrackVideo*)t); break;
			case TRACK_TYPE_AUDIO: avmfAudioTrack(e_main, (avmf::TrackAudio*)t); break;
			case TRACK_TYPE_SUBTITLE: avmfSubtitleTrack(e_main, (avmf::TrackSubtitle*)t); break;
		}
	}

	e->LinkEndChild(e_main);


	/*if(!memcmp(m_ext, "...", 3))
		return 1;
	return 0;*/
	if(
		!memcmp(m_ext, "mkv", 3) ||
		!memcmp(m_ext, "ogm", 3) ||
		!memcmp(m_ext, "avi", 3) ||
		!memcmp(m_ext, "mp4", 3) ||
		!memcmp(m_ext, "mov", 3) ||
		!memcmp(m_ext, "mpg", 3) ||
		!memcmp(m_ext, "wmv", 3) ||
		!memcmp(m_ext, "rm", 2) )
		return 0;
	return 1;
}

int MediaContainer::buildXMLost(addr_t tx){
	if(tx==NULL) return -1;
	TiXmlElement *e_main = (TiXmlElement *)tx;
	avmf::Container c;
	mkAVMF(&c);
	
//	TiXmlElement * e_main = new TiXmlElement( "idx" );

	tinyAdd(e_main, XML_FILE_DURATION, c.duration);

	for(int i=0; i<c.tracks.size(); i++){
		avmf::Track *t = c.tracks.at(i);
		switch(t->track_type()){
			case TRACK_TYPE_AUDIO:{
				avmf::TrackAudio *a = (avmf::TrackAudio*)t;
				tinyAdd(e_main, XML_TRACK_BITRATE, a->bitrate);
				tinyAdd(e_main, XML_AUDIO_TWOCC, a->twocc);
				tinyAdd(e_main, XML_AUDIO_CHANNELS, a->channels);
				tinyAdd(e_main, XML_AUDIO_SAMPLING_RATE, a->sample.rate);
				tinyAdd(e_main, XML_AUDIO_BITRATE_MODE, a->mode);	
								  }break;
		}
	}

	tinyAdd(e_main, "song_title", c.meta.title, false, true);
	tinyAdd(e_main, "album", c.meta.album, false, true);
	tinyAdd(e_main, "artist", c.meta.artist, false, true);
	tinyAdd(e_main, "composer", c.meta.composer, false, true);
	tinyAdd(e_main, "genre", c.meta.genre, false, true);
	tinyAdd(e_main, "year", c.meta.year, false, true);
	tinyAdd(e_main, "track_number", c.meta.track_number, false, true);
	tinyAdd(e_main, "total_track_number", c.meta.total_track_number, false, true);
	tinyAdd(e_main, "disk_number", c.meta.disk_number, false, true);
	tinyAdd(e_main, "total_disk_number", c.meta.total_disk_number, false, true);
	tinyAdd(e_main, "comment", c.meta.comment, false, true);
	tinyAdd(e_main, "copyright", c.meta.copyright, false, true);
	tinyAdd(e_main, "encoded_by", c.meta.encoded_by, false, true);

//	e->LinkEndChild(e_main);

	return 0;
}
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

#include "wrappers.h"
#include "OGMInfo.h"

OGM::OGM(wchar_t *path){
	m_p = new MyOGM(path);
	valid = true;
}
void OGM::clean(){
	delete m_p;
}
float OGM::parse(){
	float last;
	for(int i=0; i<80; i++){
		last = m_p->Parse();
		if(last<0) return -1.0f;
	}
#ifdef DONT_PARSE
	return -1;
#else
	return last;
#endif
}
/*
float OGM::GetDuration(bool video){
	if(GetTrackCount()<1) return 0;
	if(video){
		OggTrack *track;
		for(int i=0; i<GetTrackCount(); i++){
			track = m_p->GetTrack(i);
			if((track->m_iType&kTrackTypeMask)==kTrackVideo)
				return track->m_iDurationUS/1000000.0f;
		}
	}
	return m_p->GetDuration();
}
int OGM::GetTrackCount(){
	return m_p->GetNumberOfTracks();
}
GenericTrack OGM::GetGenericTrack(int nr){
	GenericTrack gt;
	OggTrack *track = m_p->GetTrack(nr);
	gt.codec = "NULL";
	//gt.codecExtra = "";
	switch(track->m_iType & kTrackTypeMask) {
		case kTrackVideo:{
			gt.type_id = 1;
			track->m_pStreamHeader->subtype[4]='\0';
			gt.codec = track->m_pStreamHeader->subtype;
			//gt.codec[4]='\0';
			gt.codec_id = msfourcc(gt.codec);//userdata(msfourcc(gt.codec), v.private_frame);
			}break;
		case kTrackAudio:{
			gt.type_id = 2;
			
			std::string strTag(track->m_pStreamHeader->subtype, 4);
			switch(strtoul(strTag.c_str(), NULL, 16)){
				case 0x0000: gt.codec = "Vorbis"; gt.codec_id = A_VORBIS; break;
				case 0x0001: gt.codec = "PCM"; gt.codec_id = A_PCM; break;
				case 0x0002: gt.codec = "ADPCM"; gt.codec_id = A_PCM; break;
				case 0x0050: gt.codec = "MP2"; gt.codec_id = A_MP2; break;
				case 0x0055: gt.codec = "MP3"; gt.codec_id = A_MP3_CBR; break;
				case 0x00FF: gt.codec = "AAC"; gt.codec_id = A_AAC; break;
				case 0x2000: gt.codec = "AC3"; gt.codec_id = A_AC3; break;
				case 0x2001: gt.codec = "DTS"; gt.codec_id = A_DTS; break;
				default: gt.codec = "Unknown"; gt.codec_id = A_UNKNOWN;
			}
						 }break;
		case kTrackText:
			gt.type_id = 17;
			gt.codec = "S_TEXT";
			gt.codec_id = S_SRT;
			break;
		case kTrackOld:
			gt.type_id = 4;
			break;
		case kTrackUnknown:
		default:
			gt.type_id = 5;
			break;
	}
	//est bitr: 8. * double(track->m_iBytes_Data) / double(track->m_Samplerate.scale64ir(_LL(1000) * track->m_iSamplesProcessed))
	//gt.size = (float)double(track->m_iBytes_PageHeader+track->m_iBytes_PageBody);
	//gt.size = (float)double(track->m_iBytes_Packets);
	//printf("data: %d\n", track->m_iBytes_Data);
	//printf("pack: %d\n", track->m_iBytes_Packets);
	//printf("body: %d\n", track->m_iBytes_PageBody);
	//printf("head: %d\n", track->m_iBytes_PageHeader);
	if((track->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis)
		gt.size = (float)double(track->m_iBytes_Packets-track->m_iBytes_Data);
	else gt.size = (float)double(track->m_iBytes_Data);
	gt.dur = track->m_iDurationUS/1000000.0f;
	if(gt.dur==0) gt.dur = GetDuration();
	gt.def = false;
	gt.lang = "Unknown";
	gt.name = "";
	//printf("AI\n");
	for(int i=0 ; i<track->m_VorbisComment.comments ; i++) {
		//printf("%s\n",track->m_VorbisComment.user_comments[i]);
		if(strncmp(track->m_VorbisComment.user_comments[i], "LANGUAGE=", 9)==0){
			gt.lang = (char *)(track->m_VorbisComment.user_comments[i]+9);
			break;
		}
	}
	return gt;
}
VideoTrack OGM::GetVideoTrack(int nr){
	VideoTrack vt;
	OggTrack *track = m_p->GetTrack(nr);
	if((track->m_iType & kTrackTypeMask)==kTrackVideo){
		vt.display_width = vt.pixel_width = track->m_pStreamHeader->sh.video.width;
		vt.display_height = vt.pixel_height = track->m_pStreamHeader->sh.video.height;
		vt.fps = (int)(1000*double(track->m_Samplerate));
		vt.private_frame = (char *)m_p->vcodec.c_str();			
	}
	return vt;
}
AudioTrack OGM::GetAudioTrack(int nr){
	AudioTrack at;
	OggTrack *track = m_p->GetTrack(nr);
	//printf("STREAMSIZ=%d\n", track->m_pStreamHeader->size);
	//if(strlen(track->m_pStreamHeader->subtype)<1){
	//	at.channels = (int)track->m_VorbisInfo.channels;
	//	at.samplerate = (int)track->m_VorbisInfo.rate;
	//}
	//else{
		at.channels = (int)track->m_pStreamHeader->sh.audio.channels;
		at.samplerate = (int)double(track->m_Samplerate);
	//}
	return at;
}
*/
int OGM::mkAVMF(avmf::Container *cont){
	cont->duration = m_p->GetDuration();
	cont->timecodescale = -1;
	//cont->title = 
	//cont->writing_application = info->m_infoWritingApp.GetUTF8();
	//cont->muxing_application = info->m_infoMuxingApp.GetUTF8();

	OggTrack *ogm_track;
	for(int i=0; i<m_p->GetNumberOfTracks(); i++){
		ogm_track =  m_p->GetTrack(i);
		avmf::Track *avmf_track;
		switch(ogm_track->m_iType & kTrackTypeMask){
			case kTrackVideo:{
				avmf::TrackVideo *tv = new avmf::TrackVideo();
				avmf_track = tv;
				
				tv->image.dw = 0;
				tv->image.dh = 0;
				tv->image.pw = ogm_track->m_pStreamHeader->sh.video.width;
				tv->image.ph = ogm_track->m_pStreamHeader->sh.video.height;
				tv->image.resolution = ogm_track->m_pStreamHeader->bits_per_sample;
				tv->sample.rate = (float)double(ogm_track->m_Samplerate);
				tv->fourcc = std::string(ogm_track->m_pStreamHeader->subtype, 4);
				tv->encoder = m_p->vcodec;
			}break;
			case kTrackAudio:{
				avmf::TrackAudio *ta = new avmf::TrackAudio();
				avmf_track = ta;
				ta->channels = (int)ogm_track->m_pStreamHeader->sh.audio.channels;
				ta->sample.rate = (float)double(ogm_track->m_Samplerate);
				ta->twocc = std::string(ogm_track->m_pStreamHeader->subtype, 4);
				if(strtoul(ta->twocc.c_str(), NULL, 16)==0)
					ta->twocc = "674f";
				//printf(
				
			}break;
			case kTrackText:{
				avmf::TrackSubtitle *ts = new avmf::TrackSubtitle();
				avmf_track = ts;
				ts->codecid = "S_TEXT/UTF8";

			}break;
			default: continue;
		}
		avmf_track->no = i;
		avmf_track->uid = ogm_track->m_iSerial;
		avmf_track->duration = ogm_track->m_iDurationUS/1000000.0f;
		//printf("\nTIME: %I64d\n", ogm_track->m_iDurationUS);
		//ogm_track->m_pStreamHeader->time_unit

		//if((ogm_track->m_iType & kTrackSubtypeMask) == kTrackSubtypeVorbis)
		//	avmf_track->size = ogm_track->m_iBytes_Packets-ogm_track->m_iBytes_Data;
		//else 
			avmf_track->size = ogm_track->m_iBytes_Data;
		
		if(ogm_track->m_VorbisComment.vendor && cont->muxedby.length()<1)
			cont->muxedby.append(ogm_track->m_VorbisComment.vendor);
		
		for(int i=0 ; i<ogm_track->m_VorbisComment.comments ; i++) {
			char * str = ogm_track->m_VorbisComment.user_comments[i];
			//if(avmf_track->private_data.length()>0)
			int len = strlen(str);

			if(len>9 && strncmp(str, "LANGUAGE=", 9)==0){
				avmf_track->language = (char *)(str+9);
			}else if(len>6 && strncmp(str, "TITLE=", 6)==0){
				avmf_track->title = (char *)(str+6);
			}else if(len>8 && strncmp(str+len-8, "Japanese", 8)==0){
				//hack for fid:58058
				avmf_track->language = "Japanese";
			}else{
				/*avmf_track->private_data += "<cmt>";
				avmf_track->private_data += str;
				avmf_track->private_data += "</cmt>";*/
				avmf_track->private_vector.push_back(str);
				/*for(int x=0; x<strlen(str); x++)
					printf("%d: %02d %c\n", x, (byte)str[x], str[x]);*/
			}/*else if(strncmp(str, "CHAPTER", 7)==0){
				str[9] = '\0';
				int nr = atoi(str+7);
				if(nr>cont->chapters)
					cont->chapters = nr;
			}*/
		}

		cont->tracks.push_back(avmf_track);
	}
	return 0;
}

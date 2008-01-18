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

#include "MatroskaUtils.h"
#include "MatroskaUtilsBitrate.h"

#include "ZenLib/BitStream.h"

#define TIMECODESTAT
using namespace avmf;
using namespace ZenLib;

MKV::MKV(wchar_t *path){
	m_p = new MatroskaInfoParser(path);
	m_p->m_parseSeekHead = false;
	m_p->m_parseAttachments = true;
	m_p->m_parseTags = false;
	m_p->m_parseWholeFile = false;
	
	if (!m_p->IsValid() || m_p->ParseFile() != 0){
		delete m_p;
		//MessageBox(NULL, "Not Matroska", "AHA!", MB_OK);
		MessageBoxW(NULL, path, L"Not Matroska", MB_OK);
		valid = false;
		return;
	}
	m_b = new MatroskaBitrateInfo();	
	m_b->Open(m_p);
	valid = true;
	m_att_size = 0;
}
void MKV::clean(){
	delete m_b;
	delete m_p;	
}
int MKV::code(){
#ifdef DONT_PARSE
	return 0;
#else
	if(m_b->m_Reader->corrupt)
		return -1;
	return 0;
#endif
}
float MKV::parse(){
#ifdef DONT_PARSE
	return -1;
#else
	if(m_b->m_Reader->corrupt)
		return -1;
	if(m_b->m_Reader->Loop()){		
		return m_b->GetCurrentPercent();
	}
	if(m_att_size==0){
		for(size_t i=0; i<m_p->fileAttachments.GetNumberOfAttachments(); i++)
			m_att_size += m_p->fileAttachments.GetAttachment(i)->GetAttachmentSize();
	}
	MatroskaTrackInfo *track;
	for(int i=0; i<m_p->GetNumberOfTracks(); i++){
		track = m_p->GetTrackInfo0(i);
		//printf("Track %d: size=%d, first=%f, last=%f\n", i, track->m_timecodes->size(), (float)(1.0*track->m_timecodes->front()/1000000), (float)(1.0*track->m_timecodes->back()/1000000));
		sort(track->m_timecodes->begin(), track->m_timecodes->end());
		//printf("Track %d: size=%d, first=%f, last=%f\n\n", i, track->m_timecodes->size(), (float)(1.0*track->m_timecodes->front()/1000000), (float)(1.0*track->m_timecodes->back()/1000000));
	}
	return -1.0f;
#endif
}

int print_buffer(binary* buf, int len){
	printf("BUFS %d\n", len);
	for(int x=0; x<len/16; x++){
		printf("%06d ", x*16);
		for(size_t y=0; y<16; y++)
			printf("%02X ", buf[x*16+y]);
		printf("\n");
	}
	int extra = len%16;
	if(extra>0)
	{
		int x = len-extra;
		printf("%06d ", x);
		for(int y=0; y<extra; y++)
			printf("%02X ", buf[x+y]);
		printf("\n");
	}
	return 0;
}
/**************AVC*****************/
enum avc_profile_e
{
    PROFILE_BASELINE = 66,
    PROFILE_MAIN     = 77,
    PROFILE_EXTENTED = 88,
    PROFILE_HIGH    = 100,
    PROFILE_HIGH10  = 110,
    PROFILE_HIGH422 = 122,
    PROFILE_HIGH444 = 144
};
static inline int bs_read_ue( BitStream &BS )
{
    int i = 0;

	while( BS.Get( 1 ) == 0 && BS.Remain()>0 && i < 32 )
    {
        i++;
    }
    return( ( 1 << i) - 1 + BS.Get( i ) );
}
static inline int bs_read_se( BitStream &BS )
{
    int val = bs_read_ue( BS );

    return val&0x01 ? (val+1)/2 : -(val/2);
}
void avc_sps_read( BitStream &BS, int &w, int &h )
{
    int i_profile_idc = BS.Get(8); // i_profile_idc
    BS.Get(1); // b_constraint_set0
    BS.Get(1); // b_constraint_set1
    BS.Get(1); // b_constraint_set2

    BS.Get(5); // 0 -> reserved

    BS.Get(8); // i_level_idc

    bs_read_ue(BS);//, sps->i_id );
	
    if( i_profile_idc >= PROFILE_HIGH )
    {
        bs_read_ue(BS);// 1 -> chroma_format_idc = 4:2:0
        bs_read_ue(BS);// 0 -> bit_depth_luma_minus8
        bs_read_ue(BS);// 0 -> bit_depth_chroma_minus8
		BS.Get(3);
        BS.Get(1); // b_qpprime_y_zero_transform_bypass
        BS.Get(1); // 0 -> seq_scaling_matrix_present_flag
    }

	bs_read_ue(BS);// i_log2_max_frame_num - 4
    int i_poc_type = bs_read_ue(BS);// sps->i_poc_type
    if( i_poc_type == 0 )
    {
        bs_read_ue(BS);// i_log2_max_poc_lsb - 4
    }
    else if( i_poc_type == 1 )
    {
        int i;

        BS.Get(1); // b_delta_pic_order_always_zero
        bs_read_se(BS);// i_offset_for_non_ref_pic
        bs_read_se(BS);// i_offset_for_top_to_bottom_field
        int i_num_ref_frames_in_poc_cycle = bs_read_ue(BS);// i_num_ref_frames_in_poc_cycle

        for( i = 0; i < i_num_ref_frames_in_poc_cycle; i++ )
        {
            bs_read_se(BS);// i_offset_for_ref_frame[i]
        }
    }
	bs_read_ue(BS);// i_num_ref_frames
	BS.Get(1); // b_gaps_in_frame_num_value_allowed
    bs_read_ue(BS);// i_mb_width - 1
	/*if (b_frame_mbs_only)
    {
        bs_read_ue(BS);// i_mb_height - 1
    }
    else // interlaced
    {
        bs_read_ue(BS);// i_mb_height/2 - 1
    }*/
	bs_read_ue(BS);

    int b_frame_mbs_only = BS.Get(1); // b_frame_mbs_only
    if( !b_frame_mbs_only )
    {
        BS.Get(1);// b_mb_adaptive_frame_field
    }
    BS.Get(1); // b_direct8x8_inference

    int b_crop = BS.Get(1); // b_crop
    if( b_crop )
    {
        bs_read_ue(BS);// crop.i_left   / 2
        bs_read_ue(BS);// crop.i_right  / 2
        bs_read_ue(BS);// crop.i_top    / 2
        bs_read_ue(BS);// crop.i_bottom / 2
    }

    int b_vui = BS.Get(1);//b_vui
    if( b_vui )
    {
        int b_aspect_ratio_info_present = BS.Get(1);//b_aspect_ratio_info_present
        if( b_aspect_ratio_info_present )
        {
            static const struct{ int sar; int w, h; } sar[] =
			{
				{0,0,0},
				{1,1,1},   {2,12,11},  {3,10,11},  {4,16,11},//4
				{5,40,33}, {6,24,11},  {7,20,11},  {8,32,11}, //8
				{9,80,33}, {10,18,11}, {11,15,11}, {12,64,33}, //12
				{13,160,99},
			};
			int sarid = BS.Get(8);
			
			if(sarid==255)
			{
				w = BS.Get(16); // i_sar_width
                h = BS.Get(16); // i_sar_height
			}
			else
			{
				w = sar[sarid].w;
				h = sar[sarid].h;
			}
        }

        int b_overscan_info_present = BS.Get(1);// b_overscan_info_present
        if( b_overscan_info_present )
            BS.Get(1);// b_overscan_info

        int b_signal_type_present = BS.Get(1);// b_signal_type_present
        if( b_signal_type_present )
        {
            BS.Get(3); // i_vidformat );
            BS.Get(1);// b_fullrange );
            int b_color_description_present = BS.Get(1);// b_color_description_present
            if( b_color_description_present )
            {
                BS.Get(8); // i_colorprim
                BS.Get(8); // i_transfer
                BS.Get(8); // i_colmatrix
            }
        }

        int b_chroma_loc_info_present = BS.Get(1);// b_chroma_loc_info_present
        if( b_chroma_loc_info_present )
        {
            bs_read_ue(BS);// i_chroma_loc_top
            bs_read_ue(BS);// i_chroma_loc_bottom
        }

        int b_timing_info_present = BS.Get(1);// b_timing_info_present
        if( b_timing_info_present )
        {
            BS.Get(32); // i_num_units_in_tick
            BS.Get(32); // i_time_scale
            bs_read_ue(BS);// b_fixed_frame_rate
        }

        BS.Get(1);//0 -> nal_hrd_parameters_present_flag
        BS.Get(1);//0 -> vcl_hrd_parameters_present_flag
        BS.Get(1);//0 -> pic_struct_present_flag
        int b_bitstream_restriction = BS.Get(1);// b_bitstream_restriction
        if( b_bitstream_restriction )
        {
            BS.Get(1);// b_motion_vectors_over_pic_boundaries
            bs_read_ue(BS);// i_max_bytes_per_pic_denom
            bs_read_ue(BS);// i_max_bits_per_mb_denom
            bs_read_ue(BS);// i_log2_max_mv_length_horizontal
            bs_read_ue(BS);// i_log2_max_mv_length_vertical
            bs_read_ue(BS);// i_num_reorder_frames
            bs_read_ue(BS);// i_max_dec_frame_buffering
        }
    }
	BS.Byte_Align();
}
std::string avc_sei_read( BitStream &BS ){
	/*const unsigned char uuid_ateme[16] = {
        0xfb, 0x57, 0x4a, 0x60, 0xac, 0x92, 0x4e, 0x68,
        0xa2, 0x01, 0x30, 0x28, 0x3c, 0x85, 0x32, 0xc9
    };
	const unsigned char uuid_x264[16] = {
        0xdc, 0x45, 0xe9, 0xbd, 0xe6, 0xd9, 0x48, 0xb7,
        0x96, 0x2c, 0xd8, 0x20, 0xd9, 0x23, 0xee, 0xef
    };*/

	int32u payload_byte = 0;
	int32u payload_type = 0;
	int32u payload_size = 0;
	while( (payload_byte=BS.Get(8))==0xFF )
		payload_type += 0xFF;
	payload_type += payload_byte;
	while( (payload_byte=BS.Get(8))==0xFF )
		payload_size += 0xFF;
	payload_size += payload_byte;

	if ( payload_size<17 )
		return "";

	//printf("uuid: ");
	//for(size_t y=0; y<16; y++)
	//	printf("%02X ", BS.Get(8));
	//printf("\n");
	for(size_t y=0; y<16; y++)
		BS.Skip(8);

	payload_size-=16;

	if(payload_type != 5){
		for( size_t i=0; i<payload_size; i++ )
			BS.Skip(8);
		BS.Skip(8);
		BS.Byte_Align();
		return "";
	}

	char *str = new char[payload_size];
	for( size_t i=0; i<payload_size; i++ ){
		str[i] = (char)BS.Get(8);
	}
	BS.Skip(8);
	BS.Byte_Align();
	
	str[payload_size-1] = 0;//should be

	for(int32u i=0; i<payload_size-1; i++)
		if(str[i]<32 || str[i]>126){
			str[i] = 0; //cut if crap
			break;
		}

	std::string settings(str);
	delete str;

	return settings;
}
void avc_decode( binary *buf, int len, int &w, int &h, std::string &str)
{
	BitStream BS(buf, len);
	int32u code = BS.Get(32), forbidden_zero, nal_ref_idc, nal_unit_type;
	while( BS.Remain()>7 && code == 0x00000001 ){
		forbidden_zero = BS.Get(1);
		if ( forbidden_zero )
			return;
		nal_ref_idc = BS.Get(2);
		nal_unit_type = BS.Get(5);

		//printf("nal z/ref/type % 8X: %02x %02x %02x\n", BS.Offset_Get(), forbidden_zero, nal_ref_idc, nal_unit_type);
		
		switch(nal_unit_type){
			case 6:
				str = avc_sei_read(BS);
				break;
			case 7:
				avc_sps_read(BS, w, h);
				break;
		}
		/*code = 0;
		do{
			code = (code<<8) | BS.Get(8);
		}while( code != 1 && BS.Remain()>7 );*/
		code = BS.Get(32);
	}
}
int mkv_avc_decode(binary *dst, binary *src, int len, int nal_size_size) {
	static const binary start_code[4] = { 0x00, 0x00, 0x00, 0x01 };
	int pos_d = 0;
	int pos_s = 0;
	int frames = 0;
	while (len > pos_s){
		int i;
		int nal_size = 0;

		for (i = 0; i < nal_size_size; ++i)
			nal_size = (nal_size << 8) | src[pos_s++];

		if ((pos_s + nal_size) > len){
			break;
		}

		if (pos_d+4+nal_size>len)
			return len;

		memcpy(dst+pos_d, start_code, 4);
		pos_d += 4;
		memcpy(dst+pos_d, src+pos_s, nal_size);
		pos_d += nal_size;
		pos_s += nal_size;
		frames++;
	}
	return pos_d;
}
void getVideoSettings(MatroskaTrackInfo *track, TrackVideo *tv){
	binary *src = track->m_wbb;
	int len = track->m_wbs;
	
	int w = 0;
	int h = 0;
	std::string str;
	//not much point in this after all. native -> no sps pps
	if ( !track->m_codecID.compare("V_MPEG4/ISO/AVC") )
	{
		if(track->m_cps<6)
			return;
		int nal_size_size = 1 + (track->m_codec_private[4] & 3);
		binary *dst = new binary[len+100*(4-nal_size_size)];
		len = mkv_avc_decode(dst, src, len, nal_size_size);
		avc_decode(dst, len, w, h, str);
		delete dst;		
	}else{
		avc_decode(src, len, w, h, str);
	}
	if(str.length()>0)
	{
		tv->settings = str;
	}
	if( w || h ){
		//printf("SAR: %dx%d\n", w, h);
		int dh = (tv->image.ph * h)/w;
		if( tv->image.dw && tv->image.dh )
		{
			if( tv->image.dh != dh )
			{
				char out[128];
				sprintf(out, "sar(%dx%d)", w, h);
				printf("\tMISMATCH! %dx%d - %dx%d sar(%dx%d)\n", tv->image.pw, tv->image.ph, tv->image.dw, tv->image.dh, w, h);
				tv->private_vector.push_back(out);
			}
			//else -> all is good
		}
		else
		{
			tv->image.dw = tv->image.pw;
			tv->image.dh = dh;
		}
	}
	
	/*const unsigned char uuid_ateme[16] = {
        0xfb, 0x57, 0x4a, 0x60, 0xac, 0x92, 0x4e, 0x68,
        0xa2, 0x01, 0x30, 0x28, 0x3c, 0x85, 0x32, 0xc9
    };
	const unsigned char uuid_x264[16] = {
        0xdc, 0x45, 0xe9, 0xbd, 0xe6, 0xd9, 0x48, 0xb7,
        0x96, 0x2c, 0xd8, 0x20, 0xd9, 0x23, 0xee, 0xef
    };
	int i, a=-1;
	for(i=0; i<len-16; i++){
		if(a<0){
			if(!memcmp(uuid_x264, buf+i, 16) || !memcmp(uuid_ateme, buf+i, 16))
				a = i+16;
		}else if(buf[i]>0)
			break;
	}
	if(a>0 && i>0 && i<len-2){
		std::string s((char*)(buf+a));
		return s;
	}*/
}
void addstr(std::vector<std::string> &list, binary* buf, int len){
	if(len<1)
		return;

	/*for(int k=0; k<len/16; k++){
		printf("%04X: ", k*16);
		for(int l=0; l<16; l++)
			printf("%02X ", 0x0FF & buf[k*16+l]);
		for(int l=0; l<16; l++)
			printf("%c ", 0x0FF & buf[k*16+l]);
		printf("\n");
	}*/

	size_t i;
	buf[len] = 0;
	std::string str((char*)buf);
	//printf("%s\n", buf);
	
	for(i=0; i<list.size(); i++)
		if(list.at(i) == str)
			break;
	if(list.size()==i)
		list.push_back(str);
}
std::string getEncoder(binary *buf, size_t len){
	int num = 0;
	int pos = 0;
	binary * priv = new binary[1024];

	std::vector<std::string> list;
	for(size_t x=0; x<len-4;){
		if(buf[x]==0 && buf[x+1]==0 && buf[x+2]==1 && buf[x+3]==0xB2){
			size_t y = x + 4;
			if(num>0){
				addstr(list, priv, pos);
				pos = 0;
			}
			num++;
			for(; y<len-1; y++){
				if(buf[y]==0 && buf[y+1]==0) break;
				if(buf[y]<32 || buf[y]>126){
					pos = 0;
					num--;
					break;
				}
				priv[pos++] = (char)buf[y];
			}
			if(buf[y]>31 && buf[y]<127)
				priv[pos++] = (char)buf[y];
			x = y;
		} else x++;
	}
	addstr(list, priv, pos);
	delete priv;
	if(list.size()>0){
		std::string res = list.at(0);
		for(size_t i=1; i<list.size(); i++){
			res += " ";
			res += list.at(i);
		}
		return res;
	}
	return "";
}
void checkforvfr(MatroskaTrackInfo *track, avmf::TrackVideo *tv, unsigned __int64 tcs){
		
	std::vector<uint64> *v = track->m_timecodes;

	tcs = 1000000;

	if(v->size()>1){
		sort(v->begin(), v->end());
		
		uint64 last = v->at(0), curr;
		size_t frames = v->size();
		int min=2000, max=0, delta;

#ifdef TIMECODESTAT
		uint64 acc = last;
		const int asize = 2048;
		int arr[asize];
		for(int i=0; i<asize; i++)
			arr[i] = 0;
#endif
		for(size_t i=1; i<frames;i++){
			curr = v->at(i);
			delta = (int)((curr - last)/tcs);
			last = curr;
			if(delta>max) max = delta;
			if(delta<min) min = delta;
#ifdef TIMECODESTAT
			acc += delta;
			if(delta>=0&&delta<asize)
				arr[(int)delta]++;
#endif
		}
		if((max-min)>2){
			tv->sample.rate = VIDEO_MODE_VFR;
		}
#ifdef TIMECODESTAT
		int cidx=0, cval=0, unique=0;
		for(int i=0; i<asize; i++){
			if(arr[i]>0){
				if(arr[i]>1) unique++;
				if(arr[i]>cval){
					cval = arr[i];
					cidx = i;
				}
			}
		}

		double fps = 1000.0*frames/(cidx+((v->back()-v->front())/tcs));
		if(tv->sample.rate > 1000.0f){
			tv->sample.rate = (float)fps;
		}

		float avg = (float)acc/frames;
		float std = 0;
		for(size_t i=1; i<v->size(); i++)
			std += pow(((float)((v->at(i)-v->at(i-1))/tcs)-avg), 2);
		std = sqrt(std/(frames-1));

		if(unique>6)
			tv->sample.rate = VIDEO_MODE_VFR;
		else if(unique>3) // or 3
			tv->sample.rate = VIDEO_MODE_HYBRID;
		else if(tv->sample.rate==0)
			tv->sample.rate = (float)fps;

		char out[1024];
		sprintf(out, "tcs(max=%d,min=%d,mod=%d,avg=%.2f,std=%.3f,unq=%d)", max, min, cidx, avg, std, unique);
		tv->private_vector.push_back(out);
#endif
	}else tv->sample.rate = VIDEO_MODE_UNKNOWN;
}

int MKV::mkAVMF(avmf::Container *cont){
	MatroskaSegmentInfo *info = m_p->GetSegmentInfo();
	cont->duration = info->GetDuration();
	cont->timecodescale = info->GetTimecodeScale();
	cont->title = info->m_infoTitle.GetUTF8();
	cont->writtenby = info->m_infoWritingApp.GetUTF8();
	cont->muxedby = info->m_infoMuxingApp.GetUTF8();
	cont->misc_size = m_att_size;
	cont->date = info->m_infoDateUnix;

	MatroskaTrackInfo *mkv_track;
	for(int i=0; i<m_p->GetNumberOfTracks(); i++){
		mkv_track = m_p->GetTrackInfo0(i);
		Track *avmf_track;
		/*printf("Type: , Size: %d\n", mkv_track->GetTrackType(), AUDIO_BUF_SIZE);
		for(size_t x=0; x<AUDIO_BUF_SIZE/16/16; x++){
			printf("%06d ", x*16);
			for(size_t y=0; y<16; y++)
				printf("%02X ", mkv_track->m_wbb[x*16+y]);
			printf("\n");
		}*/
		switch(mkv_track->GetTrackType()){
			case track_video:{
				TrackVideo *tv = new TrackVideo();
				avmf_track = tv;
				MatroskaVideoTrackInfo *video = mkv_track->GetVideoInfo();
				tv->image.dw = video->video_DisplayWidth;
				tv->image.dh = video->video_DisplayHeight;
				tv->image.pw = video->video_Width;
				tv->image.ph = video->video_Height;
				if(tv->image.dw==tv->image.pw && tv->image.dh==tv->image.ph){
					tv->image.dw = 0;
					tv->image.dh = 0;
				}
				tv->image.resolution = video->video_ColorDepth;
				tv->sample.count = mkv_track->m_timecodes->size();
				tv->sample.rate = video->video_FrameRate;
				tv->fourcc = mkv_track->m_CodecOldID;
				tv->encoder = getEncoder(mkv_track->m_wbb, mkv_track->m_wbs);
				if(tv->encoder.length()<1)
					tv->encoder = getEncoder(mkv_track->m_codec_private, mkv_track->m_cps);
				checkforvfr(mkv_track, tv, cont->timecodescale);
				getVideoSettings(mkv_track, tv);
			}break;
			case track_audio:{
				TrackAudio *ta = new TrackAudio();
				avmf_track = ta;
				MatroskaAudioTrackInfo *audio = mkv_track->GetAudioInfo();
				ta->channels = audio->audio_Channels;
				ta->sample.rate = audio->audio_SampleRate;
				ta->sample.count = mkv_track->m_timecodes->size();
				if(mkv_track->m_codecID=="A_MPEG/L3" && isVbr(mkv_track->m_wbb, TRACK_BUF_SIZE))
					ta->mode = "VBR";
				ta->twocc = mkv_track->m_CodecOldID;

				/*if(i==3 || i==2)
				{
					std::vector<uint64> *v = mkv_track->m_timecodes;
					//sort(v->begin(), v->end());
					printf("%d: %d - %f\n", i, v->size(), (float)(1.0*v->at(v->size()-1)/cont->timecodescale) );
					//for(int k=0; k<v->size(); k++)
					//printf("-> %f\n", (float)(1.0*v->at(k)/cont->timecodescale) );

				}*/

			}break;
			case track_subtitle:{
				TrackSubtitle *ts = new TrackSubtitle();
				avmf_track = ts;
			}break;
		}
		avmf_track->no = i;
		avmf_track->uid = mkv_track->m_trackUID;
		avmf_track->title = mkv_track->m_trackName;
		avmf_track->duration = mkv_track->GetDuration()/1000.0f;
		avmf_track->language = mkv_track->m_trackLanguage;
		avmf_track->size = mkv_track->m_bytes;
		avmf_track->codecid = mkv_track->m_codecID;
		avmf_track->flag_default = mkv_track->m_default_flag;
		cont->tracks.push_back(avmf_track);
	}
	return 0;
}

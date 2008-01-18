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

#pragma once
//#include <stddef.h>

typedef unsigned int addr;
typedef __int64 mylong;

#define FILE_MKV 0
#define FILE_OGM 1
#define FILE_MP4 2 //use VAR
#define FILE_VAR 3 //mediainfo

#define TRACK_AUDIO 2
#define TRACK_VIDEO 1
#define TRACK_SUBTITLE 17

#define UNKNOWN -1

#define FORMAT_OLD 0
#define FORMAT_XML 1
#define FORMAT_SHORT 2

/*#define ERROR_COULD_NOT_OPEN 0
#define ERROR_NOT_SUPPORTED -1
#define ERROR_MEDIAINFO_DISABLED -2*/

struct GenericTrack {
	int type_id;
	int codec_id;
    char* codec;
	char* codecExtra;
	char* lang;
	char* name;
    double size;
	float dur;
	bool def;
};
struct VideoTrack {
	int pixel_width, pixel_height, display_width, display_height, fps;
	bool vfr;
};
struct AudioTrack {
	int channels, samplerate, dummy;
};

addr avinfo_file_open(wchar_t* path);
float avinfo_file_parse(addr id);
void avinfo_file_close(addr id);
int avinfo_file_formatted(addr id, char* buffer, unsigned int size, int format);

float avinfo_file_duration(addr id);
int avinfo_track_count(addr id);
GenericTrack avinfo_track_generic(addr id, int nr);
VideoTrack avinfo_track_video(addr id, int nr);
AudioTrack avinfo_track_audio(addr id, int nr);


//ANIDB DEF
#define V_UNKNOWNX	1
#define V_DIVXU 	2
#define V_DIVX3 	3
#define V_DIVX4 	5
#define V_DIVX5 	7
#define V_MPEG1		9
#define V_MPEG2		10
#define V_ASP		11
#define V_OTHER		12
//#define V_MOV		13
#define V_RVO		14
//#define V_VIVO	15
//#define V_DVD		16
#define V_XVID		17
#define V_MS_MP4X 	18
#define V_WMV9		19
#define V_RV9		20
//#define V_IV5		21
#define V_H264AVC 	22
//#define V_NONEX	23

#define	A_UNKNOWN	0x101
#define	A_AC3		0x102
#define	A_WMA		0x103
#define	A_MP3_CBR 	0x105
#define	A_MP3_VBR 	0x106
#define	A_MSAUDIO 	0x107
#define	A_VORBIS	0x108
#define	A_AAC 		0x109
#define	A_PCM 		0x10A //10
#define	A_MP2 		0x10B //11
#define	A_DTS 		0x10D //13
#define	A_OTHER		0x10F //15

#define S_UNKNOWN	0x201
#define S_NONEX		0x202
#define S_SRT		0x203
#define S_SSA		0x204
#define S_ASS		0x205
#define S_USF		0x206
#define S_BMP		0x207
#define S_VOBSUB	0x208
#define S_XSUB		0x209

#define VIDEO_FLAG_ANAMORPHIC 1
#define VIDEO_FLAG_WRONGAR 2
#define VIDEO_FLAG_VFR 4

#define GENERAL_OTHER 100
#define GENERAL_UNKNOWN 200

#define VIDEO_AR_133 10
#define VIDEO_AR_177 20
#define VIDEO_AR_166 30
#define VIDEO_AR_185 40
#define VIDEO_AR_200 50
#define VIDEO_AR_221 60
#define VIDEO_AR_235 70

#define AUDIO_CHAN_MONO 10
#define AUDIO_CHAN_STEREO 20
#define AUDIO_CHAN_DOLBY 30
#define AUDIO_CHAN_5_1_S 40
#define AUDIO_CHAN_6_1_S 50
#define AUDIO_CHAN_7_1_S 60

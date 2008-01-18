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

#ifndef __AVMF_H
#define __AVMF_H

#define TRACK_TYPE_VIDEO 1
#define TRACK_TYPE_AUDIO 2
#define TRACK_TYPE_SUBTITLE 4
//#define TRACK_TYPE_ATTACHMENT 8
//#define TRACK_TYPE_CHAPTER 16

namespace avmf{
	class Track {
	public:
		unsigned __int64 no, uid, size;
		float duration;
		//int duration;
		bool flag_default;
		//int cid;
		std::string title, language, codecid;
		std::vector<std::string> private_vector;
		virtual int track_type(){return 0;}
		Track();
	};
	/*class Dimension {
	public:
		int width, height;
	};*/
	class Image {
	public:
		//Dimension dim_pixel, dim_display;
		int pw,ph,dw,dh;
		int resolution;
		int count;
		int mode;
		float ar;
		std::string chroma, structure;
		Image();
	};
	class Sample {
	public:
		int mode_type, count;
		float rate;
		Sample();
	};
	class TrackTimed : public Track {
	public:
		Sample sample;
		int bitrate;
		std::string encoder;
		TrackTimed();
	};
	class TrackVideo : public TrackTimed {
	public:
		Image image;
		std::string fourcc, settings;
		int track_type(){return TRACK_TYPE_VIDEO;}
	};
	class TrackAudio : public TrackTimed {
	public:
		int channels;
		std::string twocc, mode;
		int track_type(){return TRACK_TYPE_AUDIO;}
	};
	class TrackSubtitle : public Track {
	public:
		//std::string format;
		int track_type(){return TRACK_TYPE_SUBTITLE;}
	};
	class Meta {
	public:
		std::string title, artist, album, disk_number, total_disk_number, track_number, total_track_number,
			year, genre, comment, composer, original_artist, copyright, url, encoded_by;
	};
	class Container {
	public:
		unsigned __int64 timecodescale;
		float duration;
		int chapters, misc_size;
		unsigned int date;
		std::string title, writtenby, muxedby;
		std::vector<Track*> tracks;
		Meta meta;
		Container();
		~Container();
	};
};

#endif // __AVMF_H
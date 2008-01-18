#include "wrappers.h"

using namespace avmf;
namespace avmf{
	Track::Track()
		:no(0)
		,uid(0)
		,size(0)
		,duration(0)
		,flag_default(0)
	{}
	Image::Image()
		:pw(0),ph(0),dw(0),dh(),resolution(0),count(0),mode(),ar(0)
	{}
	Sample::Sample()
		:mode_type(),count(0),rate(0)
	{}
	TrackTimed::TrackTimed()
		:bitrate(0)
	{}
	Container::Container(){
		timecodescale = 0;
		duration = 0;
		chapters = 0 ;
		misc_size = 0;
		date = 0;
	}
	Container::~Container(){
		Track *track;
		for(size_t i=0; i<tracks.size(); i++){
			track = tracks[i];
			switch(track->track_type()){
				case TRACK_TYPE_VIDEO:
					delete (TrackVideo*)track;
					break;
				case TRACK_TYPE_AUDIO:
					delete (TrackAudio*)track;
					break;
				case TRACK_TYPE_SUBTITLE:
					delete (TrackSubtitle*)track;
					break;
			}
		}

	}
};
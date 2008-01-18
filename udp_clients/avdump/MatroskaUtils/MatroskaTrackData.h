/*
 *  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
 *
 *  MatroskaTrackData.h
 *
 *  Copyright (C) Jory Stone - June 2003
 *
 *  The TCMP Matroska CDL is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  The TCMP Matroska CDL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The TCMP Matroska CDL; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*!
    \file MatroskaTrackData.h
		\version \$Id: MatroskaTrackData.h,v 1.19 2004/02/18 06:52:18 jcsston Exp $
    \brief Some general code to make working with Matroska easier
		\author Jory Stone     <jcsston @ toughguy.net>
		
		Inspired by ;) John Cannon (spyder482@yahoo.com) (c) 2003
*/

#ifndef _MATROSKA_TRACK_DATA_H_
#define _MATROSKA_TRACK_DATA_H_

#ifdef WIN32
#include <windows.h>
#endif //WIN32
#include <vector>

#ifdef USING_CXIMAGE
#include "CxImage/ximage.h"
#endif // USING_CXIMAGE

#include "MatroskaUtilsDebug.h"
#include "MatroskaUtilsReader.h"

// libebml includes
#include "ebml/StdIOCallback.h"
#include "ebml/EbmlTypes.h"
#include "ebml/EbmlHead.h"
#include "ebml/EbmlVoid.h"
#include "ebml/EbmlCrc32.h"
#include "ebml/EbmlSubHead.h"
#include "ebml/EbmlStream.h"
#include "ebml/EbmlBinary.h"
#include "ebml/EbmlString.h"
#include "ebml/EbmlUnicodeString.h"
#include "ebml/EbmlContexts.h"
#include "ebml/EbmlVersion.h"

// libmatroska includes
#include "matroska/KaxConfig.h"
#include "matroska/KaxBlock.h"
#include "matroska/KaxSegment.h"
#include "matroska/KaxContexts.h"
#include "matroska/KaxSeekHead.h"
#include "matroska/KaxTracks.h"
#include "matroska/KaxInfo.h"
#include "matroska/KaxInfoData.h"
#include "matroska/KaxTags.h"
#include "matroska/KaxTag.h"
#include "matroska/KaxTagMulti.h"
#include "matroska/KaxCluster.h"
#include "matroska/KaxClusterData.h"
#include "matroska/KaxTrackAudio.h"
#include "matroska/KaxTrackVideo.h"
#include "matroska/KaxAttachments.h"
#include "matroska/KaxAttached.h"
#include "matroska/KaxVersion.h"

#define TIMECODE_SCALE  1000000

#define TRACK_SIZE_FAST
#define TRACK_BUF_SIZE 262144
#define TRACK_PRI_SIZE 1024

#ifdef USING_TAG_DATA
#include "MatroskaTagData.h"
#endif
//namespace MatroskaUtilsNamespace {

class JString;
extern char *iso639_2B_codes[];

class MatroskaSegmentInfo {
public:
	MatroskaSegmentInfo();
	virtual ~MatroskaSegmentInfo();

	virtual void SetDuration(float the_duration);
	virtual float GetDuration();
	/// Return a Nicely formated Duration
//	virtual const char *GetNiceDuration();
//	virtual UTFstring GetNiceDurationW();
	virtual void SetTimecodeScale(uint64 timecodeScale);
	virtual uint64 GetTimecodeScale();

	float m_infoDuration;
	uint64 m_infoTimecodeScale;
	uint32 m_infoDateUnix;
	std::string m_infoNiceDuration;
	std::string m_infoDate;
	UTFstring m_infoFilename;
	UTFstring m_infoWritingApp;
	UTFstring m_infoMuxingApp;
	UTFstring m_infoTitle;
};

typedef struct {
    binary wFormatTag[2];
    uint16 nChannels;
    uint32 nSamplesPerSec;
    uint32 nAvgBytesPerSec;
    uint16 nBlockAlign;
    uint16 wBitsPerSample;
    uint16 cbSize;
} acm_WAVEFORMATEX;

class MatroskaAudioTrackInfo {
	public:
		MatroskaAudioTrackInfo();
		~MatroskaAudioTrackInfo();
		static const char *GetWaveFormatName(uint16 formatTag);
		//Audio Track Info
		float audio_SampleRate;
		float audio_OutputSampleRate;
		uint8 audio_Channels;
		uint8 audio_BitDepth;
};

typedef struct {
  uint32  bi_size;
  uint32  bi_width;
  uint32  bi_height;
  uint16  bi_planes;
  uint16  bi_bit_count;
  uint32  bi_compression;
  uint32  bi_size_image;
  uint32  bi_x_pels_per_meter;
  uint32  bi_y_pels_per_meter;
  uint32  bi_clr_used;
  uint32  bi_clr_important;
} vfw_BITMAPINFOHEADER;

class MatroskaVideoTrackInfo {
	public:
		MatroskaVideoTrackInfo();
		~MatroskaVideoTrackInfo();
		static const char *GetFourCCName(const char *fourcc);
		//Video Track Info
		uint16 video_Width;
		uint16 video_Height;
		uint16 video_DisplayWidth;
		uint16 video_DisplayHeight;
		float video_FrameRate;
		uint32 video_ColorDepth;
		uint8 video_Interlaced;	
		//uint8 video_VFR;
};
class MatroskaTrackInfo {
	public:
		/// Initializes the class
		MatroskaTrackInfo(MatroskaSegmentInfo *segment);
		/// Initializes the class with a UID
		/// \param trackUID The UID to use for the new MatroskaTrackInfo
		MatroskaTrackInfo(MatroskaSegmentInfo *segment, uint64 trackUID);
		/// Destroys the class
		virtual ~MatroskaTrackInfo();
		
    /// Returns the track number of this track, please note that using UID's is highly recommend
		/// \return The number of this track
		virtual uint16 GetTrackNumber();
		/// Sets the track number of this track, please note that using UID's is highly recommend
		/// \param trackNumber The track number to set for this track
		virtual void SetTrackNumber(uint16 trackNumber);
		/// Returns the UID of this track
		/// \return The UID of this track
		virtual uint64 GetTrackUID();
		/// Sets the UID of this track
		/// \param trackUID The UID to change this track to
		virtual void SetTrackUID(uint64 trackUID);
		/// Returns the track type of this track
		
		virtual uint8 GetTrackType();
		/// Returns the track type of this track as char *
		virtual const TCHAR* GetTrackTypeStr();
		/// Sets the track type of this track
		virtual void SetTrackType(uint8 trackType);
		/// Returns the codec ID of this track
		virtual const char* GetCodecID();
		/// Sets the codec ID of this track
		virtual void SetCodecID(const char* codecID);

		static const char *GetLanguageNameFromISO639(const char *iso639_code);
		virtual const char *GetLanguageName();		
		/// Returns a pointer to the video information for this track or NULL if it is not a video track.
		virtual MatroskaVideoTrackInfo* GetVideoInfo();
		/// Sets the video information for this track.
		virtual void SetVideoInfo(MatroskaVideoTrackInfo* videoInfo);
		/// Returns a pointer to the audio information for this track or NULL if it is not a audio track.
		virtual MatroskaAudioTrackInfo* GetAudioInfo();
		/// Sets the audio information for this track.
		virtual void SetAudioInfo(MatroskaAudioTrackInfo* audioInfo);
		/// Returns a pointer to the Tag info for this track or NULL if there is not a Tag segment.
		//MatroskaTagInfo* GetTagInfo();
		/// Sets the Tag information for this track.
		//void SetTagInfo(MatroskaTagInfo* tagInfo);
		/// Merge another track info data, leaving this track left
		//void MergeWithThisTrackInfo(MatroskaTrackInfo *source_track);
			
		uint16 m_trackNumber;
		uint64 m_trackUID;
		uint8 m_trackType;

		unsigned __int64 m_bytes;
		uint32 GetDuration(){
			if(m_timecodes->empty())
				return 0;
			return (uint32)((m_timecodes->back()-m_timecodes->front())/(m_timecode_scale*TIMECODE_SCALE));
			//return (uint32)((m_timecodes->back()-m_timecodes->front())/(m_timecode_scale*m_segmentInfo->m_infoTimecodeScale));
		}

		double m_timecode_scale;
		virtual void SetTrackTimecodeScale(double timecode_scale){
			m_timecode_scale = timecode_scale;
		}

		bool /*m_user_data, */m_check_mp3_vbr;
		unsigned char m_wbb[TRACK_BUF_SIZE];
		binary m_codec_private[TRACK_PRI_SIZE];
		size_t m_wbs, m_cps;

		std::string m_codecID;
		/// Old style FourCC or ACM ID
		std::string m_CodecOldID;//, m_codec_private;
		uint32 m_min_cache;
		uint32 m_max_cache;
		uint32 m_lacing_flag;
		uint32 m_default_flag;

		std::vector<uint64> *m_timecodes;

		//Lanuage of track
		std::string m_trackLanguage;
		std::string m_trackName;

		MatroskaSegmentInfo *m_segmentInfo;
	//protected:
		MatroskaVideoTrackInfo* m_videoInfo;
		MatroskaAudioTrackInfo* m_audioInfo;
		//MatroskaTagInfo* m_tagInfo;
};
class MatroskaTrackInfoList : public std::vector<MatroskaTrackInfo *> {
public:
	MatroskaTrackInfoList();
	~MatroskaTrackInfoList();
	void AddItem(MatroskaTrackInfo *new_item);
	//MatroskaTrackInfo *FindTrackListItemWithType(int type_id);
	size_type GetListCount() { return size(); };
	MatroskaTrackInfo *operator[] (uint16 requested_index) { return at(requested_index); };
protected:

};

#ifdef USING_ATT
class MatroskaAttachmentItem {
public:
	MatroskaAttachmentItem();
	~MatroskaAttachmentItem();
	bool IsExt(const wchar_t *ext);

	const wchar_t *GetMatroskaFilename();
	const char *GetMatroskaFilename_ASCII();
	int SetMatroskaFilename(const wchar_t *new_filename);	
	int SetMatroskaFilename(const char *new_filename);

	/// Get the filename of the attached file without the path
	/// Example The filename is "C:\videos\cover_The Matrix.jpg", This function will return "cover_The Matrix.jpg"
	/// \warning Returned string is freed by the MatroskaAttachmentItem class	
	const wchar_t *GetAttachmentFilenameOnly();
	/// Get the filename of the attached file without the path (ASCII version)
	/// Example The filename is "C:\videos\cover_The Matrix.jpg", This function will return "cover_The Matrix.jpg"
	/// \warning Returned string is freed by the MatroskaAttachmentItem class	
	const char *GetAttachmentFilenameOnly_ASCII();
	
	/// Get the filename of the attached file
	/// \warning Returned string is freed by the MatroskaAttachmentItem class
	const wchar_t *GetAttachmentFilename();
	/// Get the filename of the attached file (ASCII version)
	/// \warning Returned string is freed by the MatroskaAttachmentItem class
	const char *GetAttachmentFilename_ASCII();
	int SetAttachmentFilename(const wchar_t *new_filename);	
	int SetAttachmentFilename(const char *new_filename);

	/// Read the actual data of the attached file,
	/// \warning This reads all the data into memory before returning, so be careful with large attachments
	binary *GetAttachmentData();
	uint32 GetAttachmentSize();

	/// Save the attachment to a file
	int SaveTo(JString saveFilename);
	/// Get the filesize in a nice format
	/// For example, The attached file is 100,043 bytes, This function will return "100.04KB"
	/// \warning Returned string is freed by the ~MatroskaAttachmentItem
	const char *GetNiceSize();
	bool IsCover();
#ifdef USING_CXIMAGE
	CxImage *GetCxImage();
	bool bResized;
	uint32 GetAttachmentImageType();
	std::string GetImageInfo();
#endif // USING_CXIMAGE

#ifdef WIN32
	/// Open this attachment with the correct program (Open)
	/// \param hwndDlg Handle to a dialog to use for displaying MessageBox can be NULL
	/// \retval 0 Everything A ok
	/// \retval 1 Failed to run shell program
	/// \retval 3 Unable to open output file
	/// \retval -1 General Error
	int OpenViaShell(HWND hwndDlg);
#endif

	uint64 attachmentUID;
	std::string attachmentMimeType;
	UTFstring attachmentDescription;

	uint64 attachmentMatroskaFileDataStart;
	uint32 attachmentMatroskaFileDataLength;

	MatroskaAttachmentItem *next_item;
protected:

#ifdef USING_CXIMAGE
	CxImage *m_AttachmentImage;
#endif // USING_CXIMAGE

	UTFstring attach_size;
	//Filename of attached file
	UTFstring m_AttachFilename;	
	//Filename only of attached file (no path)
	UTFstring m_AttachFilenameOnly;	
	//Filename of matroska that hass the attached file
	UTFstring matroska_filename;

	binary *buffer;
};

class MatroskaAttachmentsList : public std::vector<MatroskaAttachmentItem *> {
public:
	MatroskaAttachmentsList();
	~MatroskaAttachmentsList();

	/// Add a Attachment item to the list
	void AddItem(MatroskaAttachmentItem *new_item) { push_back(new_item); };
	/// Returns the number of attachments found in the file.
	uint32 GetNumberOfAttachments() { return size(); };
	/// Returns a pointer to the specified Attachment
	/// This function is simply an alias for the overloaded operator[]
	/// \param requested_index The 0 based index of the Attachment
	/// \return A pointer to the requested MatroskaAttachmentItem or NULL if the index is out of range
	MatroskaAttachmentItem *GetAttachment(uint32 requested_index) { return at(requested_index); };
	/// Returns a pointer to the specified Attachment
	/// \param requested_index The 0 based index of the Attachment
	/// \return A pointer to the requested MatroskaAttachmentItem or NULL if the index is out of range
	MatroskaAttachmentItem *operator[] (uint32 requested_index) { return at(requested_index); };
};
#endif // USING ATT
//}; //End namespace MatroskaUtilsNamespace

#endif // _MATROSKA_TRACK_DATA_H_


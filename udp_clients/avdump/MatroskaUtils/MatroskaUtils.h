/**
	*  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
	*
	*  MatroskaUtils.h
	*
	*  Copyright (C) Jory Stone - June 2003
	*  Copyright (C) John Cannon - 2003
	*
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
    \file MatroskaUtils.h
		\version \$Id: MatroskaUtils.h,v 1.38 2004/03/20 05:08:31 jcsston Exp $
    \brief Some general code to make working with Matroska easier
		\author Jory Stone     <jcsston @ toughguy.net>
		\author John Cannon    <spyder482 @ yahoo.com>
		
		Inspired by ;) John Cannon (spyder482@yahoo.com) (c) 2003
*/

#ifndef _MATROSKA_UTILS_H_
#define _MATROSKA_UTILS_H_

#include "MatroskaUtilsDebug.h"
#include "MatroskaUtilsReader.h"
#ifdef USING_JCT
#include "JCThread.h"
#endif
//!#ifdef USING_BITRATE_INFO
//#include "MatroskaUtilsBitrate.h"
//!#endif


//standard includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <time.h>
#include <malloc.h>
#include <process.h>

#ifdef USING_TINYXML
#include "tinyxml.h"
#endif

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
#include "matroska/KaxChapters.h"
#include "matroska/KaxVersion.h"

#include "MatroskaTrackData.h"
//#ifdef USING_CHAPTER_DATA
#include "MatroskaChapterData.h"
//#endif
//#include "JList.h"
#include "JString.h"

//namespace MatroskaUtilsNamespace {

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

/// The working horse of the CDL, Shell Ext.
/// /par Memory Usage test 08-27-2003
/// For file "mew_mew_attachment.mkv" 19 tracks
/// 20 parsers, 175,886 bytes
/// For file "Copy (2) of Copy of tag_test_mkvmerge.mkv" video + lots of tags
/// 20 parsers, 102,244 bytese
/// For file "Voyager - Good Shepherd.mkv" Xvid video+Vorbis
/// 20 parsers, 81,082 bytes
/// For file "pink2.mka" MP3 + Some tags
/// 10 Parsers, 47,025 bytes
/// 20 parsers, 89,503 bytes
class MatroskaInfoParser {
	public:
		/// Initializes the class
		/// \param MatroskaFilename The filename of a Matroska file to parse
		MatroskaInfoParser(const wchar_t *MatroskaFilename);
		MatroskaInfoParser(const char *MatroskaFilename);		
		/// Reset values to defaults (not the filename)
		virtual int Initialize();
		/// Call this to check if the file is valid
		bool IsValid() { TRACE("MatroskaInfoParser::IsValid()"); return m_IsValid;}
		/// Destroys the class
		virtual ~MatroskaInfoParser();
    /// Detects if a file is a Matroska file or not
    /// \param file_to_check The filename of the Matroska file to detect
    /// \return True if the file is a Matroska file, false otherwise
   // static bool CheckForMatroska(char* file_to_check);
		virtual bool IsMatroska(JString file_to_check);
		//bool IsMatroska(const wchar_t* file_to_check);
		/// Controls if we try to read the tags or not.
		/// If we do read the tags and the file doesn't have a SeekEntry for the Tags element
		/// or doesn't even have a Tags element it will take a long time depending on the file size
		/// \param passed_readTags If set to true we will parse the tags
		//void SetReadTags(bool passed_readTags = true);
		/// Controls if we parse the whole file
		/// This will need to be set to true in order to scan for Attachments
		/// \param passed_parse_whole_file If set to true we will parse the whole file
		//void SetParseWholeFile(bool passed_parse_whole_file = true);		
		/// Parses the file's contents to retrieve the data needed
		/// \retval 0 if successful
		/// \retval 1 if the file isn't a valid Matroska file
		/// \retval 5 if the file is a EBML file but the DocType isn't Matroska
		virtual int ParseFile();
		/// NON-WORKING
		/// \param output_filename The file to copy the current matroska file to
		virtual int CopyFile(char *output_filename);
#ifdef USING_TAG_DATA
		/// Write out the tags from memory to the opened Matroska file
		virtual int WriteTags();
#endif // USING_TAG_DATA
#ifdef USING_WRITES
		/// Updates the Display Size or AR  in the currently opened Matroska file
		/// \param track_no The track number of the video track to update
		/// \param new_display_size_width The display width to write
		/// \param new_display_size_height The display height to write
		/// \retval 0 if the new Display Size was written successfully
		/// \retval 1 if the file wasn't a Matroska file or couldn't be opened
		/// \retval 2 if the file couldn't be opened for writing
		/// \retval 3 if the file didn't have DisplaySize elements
		virtual int WriteDisplaySize(uint8 track_no, uint16 new_display_size_width, uint16 new_display_size_height);
		/// Updates the Language of a track in the currently opened Matroska file
		/// \param track_no The track number of the video track to update
		/// \param new_lang The Language Code to write
		/// \retval 0 if the new Display Size was written successfully
		/// \retval 1 if the file wasn't a Matroska file or couldn't be opened
		/// \retval 2 if the file couldn't be opened for writing
		/// \retval 3 if the file didn't have the Language of a track already written
		/// \retval 4 if the Language of a track already written was too small to put the new value
		virtual int WriteLanguage(uint8 track_no, std::string new_lang);
#endif // USING_WRITES
		/// Get number of tracks found in the file
		/// \return The number of tracks found in the currently opened Matroska file
		virtual int GetNumberOfTracks();
		/// Lookup a track by it's UID and returns a pointer to the track's information
		/// \param trackUID The UID of the track you wish to find
		/// \return A MatroskaTrackInfo pointer to the found track or NULL if the track couldn't be found		
		virtual MatroskaTrackInfo *GetTrackInfoByUID(uint64 trackUID);
		/// Lookup a track by it's number and returns a pointer to the track's information
		/// This isn't the best way to find a specfic track, use GetTrackInfoByUID() instead
		/// but this is a good way to go through each track, say if you were listing all the track found
		/// \param trackNumber The number of the track you wish to find, the valid range is the return value of GetNumberOfTracks() to 1
		/// \return A pointer to a MatroskaTrackInfo of the found track or NULL if the track couldn't be found		
		virtual MatroskaTrackInfo *GetTrackInfo0(int trackNumber);
		virtual MatroskaTrackInfo *GetTrackInfo1(int trackNumber);
		/// Get the first Video track
		virtual MatroskaTrackInfo *GetFirstVideoTrack();
		/// Get the first Audio track
		virtual MatroskaTrackInfo *GetFirstAudioTrack();
		virtual MatroskaTrackInfo *GetFirstSubtitleTrack();
		/// Return a pointer to the global track info, (includes the Info segment and Tag segments without a TrackUID)
		/// If a global track doesn't exist it will create one
		/// \return A MatroskaTrackInfo pointer to the 'Global' track
		//MatroskaTrackInfo *GetGlobalTrackInfo();
		/// Get the filename of the current Matroska file
		/// \return A char pointer to the open Matroska file's name including path
		/// \warning Do not free the memory used by the returned wchar_t *
		virtual const wchar_t *GetFilename();
		virtual const char *GetFilename_ASCII();
		/// Get the filesize in a nice format
		/// For example The file is 100,756,003 bytes, This function will return "100.75MB"
		/// \return A char pointer to the a nicely formated size, Do not delete		
//		virtual const char *GetNiceFileSize();

		/// Get the Muxing App of the file
		/// \return A char pointer to the Muxing App tag
		virtual const char *GetMuxingApp();
		/// Get the Writing App of the file
		/// \return A char pointer to the Writing App tag
		virtual const char *GetWritingApp();

#ifdef USING_TAG_DATA
		/// Returns the title for the file found in the Infos element
		virtual UTFstring &GetTitle();

		virtual UTFstring GetTagArtist(MatroskaTagInfo *tagTrack = NULL);
		virtual UTFstring GetTagTitle(MatroskaTagInfo *tagTrack = NULL);
		virtual UTFstring GetTagAlbum(MatroskaTagInfo *tagTrack = NULL);
		virtual uint32 GetTagDiscTrack(MatroskaTagInfo *tagTrack = NULL);		
		virtual uint32 GetTagYear(MatroskaTagInfo *tagTrack = NULL);
		virtual std::string GetTagGenre(MatroskaTagInfo *tagTrack = NULL);
		virtual UTFstring GetTagComment(MatroskaTagInfo *tagTrack = NULL);
		virtual int32 GetTagRating(MatroskaTagInfo *tagTrack = NULL);
		virtual double GetTagBPM(MatroskaTagInfo *tagTrack = NULL);

		virtual int SetTagArtist(UTFstring artist, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagTitle(UTFstring title, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagAlbum(UTFstring album, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagDiscTrack(uint32 discTrack, MatroskaTagInfo *tagTrack = NULL);		
		virtual int SetTagYear(uint32 year, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagGenre(std::string genre, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagComment(UTFstring comment, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagRating(int32 rating, MatroskaTagInfo *tagTrack = NULL);
		virtual int SetTagBPM(double bpm, MatroskaTagInfo *tagTrack = NULL);
#endif // USING_TAG_DATA

		/// Create a text report of the file, inlcuding the track list
		/// \return A reference to the std::string report
//		virtual std::string &GetTextReport();

		virtual uint32 GetMetaSeekSize() { return m_MetaSeekSize; };
		virtual uint32 GetCuesSize() { return m_CuesSize; };
		/// Convert a ASCII string to Unicode
		/// \param ansi_filename Input ASCII string
		/// \return Unicode string
		/// \warning You have to call delete on the returned string when done with it
		static wchar_t *ConvertCharToWchar_t(const char *ansi_filename);
		/// Multi-threaded safe
		virtual uint8 GetPercentComplete();

    virtual MatroskaSegmentInfo *GetSegmentInfo();
#ifdef USING_TINYXML
    /// Save the tags in a XML file, readable by mkvmerge
    virtual int SaveTagsToXMLFile(JString &xmlFilename);
    virtual int SaveChaptersToXMLFile(JString &xmlFilename);
#endif

		MatroskaTrackInfoList trackInfos;
#ifdef USING_ATT
		MatroskaAttachmentsList fileAttachments;
#endif
#ifdef USING_TAG_DATA
		MatroskaTagList tags;
#endif
#ifdef USING_CHAPTER_DATA
		MatroskaChapterList chapters;
#endif // USING_CHAPTER_DATA
		MatroskaSegmentInfo m_segmentInfo;

		uint64 file_size;

		uint64 currentFilePos;		
		bool keepProcessing;

		//Used when parsing the a Matroska file
		bool really_found_tags;
		bool found_tags;
		bool found_tracks;
		bool found_info;
		bool found_attachments;

		bool m_parseSeekHead;
		bool m_parseAttachments;
		bool m_parseTags;
		bool m_parseWholeFile;

		uint64 m_tagElementPos;
		uint64 m_tagElementSize;

		uint32 cluster_count;
		MatroskaTrackInfo *lastSelectedTrack;
protected:				
		/// An function used by ParseFile() to gather data from a Infomation element
		/// \param inputfile_stream Pointer to a EbmlStream of the opened Matroska file
		/// \param infomation_element Pointer to a EbmlElement of the Info Element		
		/// \retval 0 if successful
		int Parse_InfomationSegment(EbmlStream *inputfile_stream, EbmlElement *infomation_element);
		/// An function used by ParseFile() to gather data from the Tracks element
		/// \param inputfile_stream Pointer to a EbmlStream of the opened Matroska file
		/// \param tracks_element Pointer to a EbmlElement of the Tracks Element
		/// \retval 0 if successful
		int Parse_Tracks(EbmlStream *inputfile_stream, EbmlElement *tracks_element);
#ifdef USING_TAG_DATA
		/// An function used by ParseFile() to gather data from the Tags element
		/// \param inputfile_stream Pointer to a EbmlStream of the opened Matroska file
		/// \param tags_element Pointer to a EbmlElement of the Tags Element
		/// \return The number of Tag(s) element found and parsed
		int Parse_Tags(EbmlStream *inputfile_stream, EbmlElement *tags_element);
#endif // USING_TAG_DATA

		/// An function used by ParseFile() to gather data from the Attachments element
		/// \param inputfile_stream Pointer to a EbmlStream of the opened Matroska file
		/// \param attachments_element Pointer to a EbmlElement of the Attachment Element
		/// \return The number of Attachments found and parsed
#ifdef USING_ATT
		int Parse_Attachments(EbmlStream *inputfile_stream, EbmlElement *attachments_element);
#endif // USING_ATT
#ifdef USING_CHAPTER_DATA
		int Parse_Chapters(EbmlStream *inputfile_stream, EbmlElement *chapter_element);
		int Parse_ChapterAtom(KaxChapterAtom &ChapterAtom, MatroskaChapterEntry **entry);
#endif // USING_CHAPTER_DATA
		 
		 /// Updates the currentPercent locking the critcal section to be multi-threaded safe
		inline void UpdatePercentComplete(uint8 newPercent);

		bool m_IsValid;

		std::string m_fileReport;		
		UTFstring m_niceFileSize;

		UTFstring m_filename;				
		
		//I've never used this
		//UTFstring m_fileTitle;

		//uint64 m_TimecodeScale;
		uint32 m_MetaSeekSize;
		uint32 m_TrackSize;
		uint32 m_CuesSize;

		//Multi-Threaded stuff
#ifdef USING_JCT
		JCCritSec m_CurrentPercent_cs;
#endif
		uint8 m_CurrentPercent;
};

#ifdef _DEBUG			
//Memory Debugging
int YourAllocHook(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber);
#endif

//};

#endif // _MATROSKA_UTILS_H_



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
		\version \$Id: MatroskaChapterData.h,v 1.4 2004/03/20 05:08:30 jcsston Exp $
    \brief Some general code to make working with Matroska easier
		\author Jory Stone     <jcsston @ toughguy.net>
		
		Inspired by ;) John Cannon (spyder482@yahoo.com) (c) 2003
*/

#ifndef _MATROSKA_CHAPTER_DATA_H_
#define _MATROSKA_CHAPTER_DATA_H_

#ifdef WIN32
#include <windows.h>
#endif //WIN32
#include <vector>

#include "MatroskaUtilsDebug.h"

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
#include "matroska/KaxSegment.h"
#include "matroska/KaxContexts.h"
#include "matroska/KaxChapters.h"
#include "matroska/KaxVersion.h"

#ifdef USING_TINYXML
#include "tinyxml/tinyxml.h"
#endif

//namespace MatroskaUtilsNamespace {

class MatroskaChapterEntry;

class MatroskaChapterList : public std::vector<MatroskaChapterEntry *> {
public:	
	void Add(MatroskaChapterEntry *chapter);
	virtual ~MatroskaChapterList();

#ifdef USING_TINYXML
	void ListAsXML(TiXmlElement &baseElement);
#endif
};

class MatroskaChapterDisplayInfo {
public:
	MatroskaChapterDisplayInfo();
	virtual ~MatroskaChapterDisplayInfo();

	UTFstring string;
	std::string lang;
	std::string country;
};

class MatroskaChapterEntry {
public:
	MatroskaChapterEntry();
	MatroskaChapterEntry(MatroskaChapterEntry *_parent);
	virtual ~MatroskaChapterEntry();

	virtual UTFstring &GetTimeStartStr();
	virtual UTFstring &GetTimeEndStr();

#ifdef USING_TINYXML
	virtual void ListAsXML(TiXmlElement &baseElement);
#endif

	uint64 chapterUID;
	uint64 timeStart;	
	uint64 timeEnd;
	
	/// Vector of all the tracks this chapter applies to
	/// if it's empty then this chapter applies to all tracks
	std::vector<uint64> tracks;
	/// Vector of strings we can display for chapter
	std::vector<MatroskaChapterDisplayInfo> display;

	// The parent of this entry, NULL if this is the root chapter
	MatroskaChapterEntry *parent;
	/// Children of this entry
	MatroskaChapterList children;
protected:
	UTFstring m_timeStartStr;
	UTFstring m_timeEndStr;
};

//}; // namespace MatroskaUtilsNamespace {

#endif // _MATROSKA_CHAPTER_DATA_H_

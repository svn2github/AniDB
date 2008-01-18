/*
 *  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
 *
 *  MatroskaChapterData.cpp
 *
 *  Copyright (C) Jory Stone - February 2004
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
    \file MatroskaChapterData.cpp
		\version \$Id: MatroskaChapterData.cpp,v 1.4 2004/03/20 05:08:30 jcsston Exp $
    \brief Code to handle Matroska Chapters
		\author Jory Stone     <jcsston @ toughguy.net>
		
*/

#include "MatroskaChapterData.h"
//#include <Shlwapi.h>

//using namespace MatroskaUtilsNamespace;
//namespace MatroskaUtilsNamespace {

void MatroskaChapterList::Add(MatroskaChapterEntry *chapter)
{
	assert(chapter != NULL);
	for (size_t c = 0; c < size(); c++)
		if (at(c)->chapterUID == chapter->chapterUID)
			return;
	
	push_back(chapter);
}
MatroskaChapterList::~MatroskaChapterList()
{
	for (size_t c = 0; c < size(); c++)
		delete at(c);
}

#ifdef USING_TINYXML
void MatroskaChapterList::ListAsXML(TiXmlElement &baseElement)
{
	size_t i;

	for (i = 0; i < size(); i++) {
		at(i)->ListAsXML(baseElement);
	}
}
#endif

MatroskaChapterDisplayInfo::MatroskaChapterDisplayInfo() 
{
	string = L" ";
	lang = "eng";
};

MatroskaChapterDisplayInfo::~MatroskaChapterDisplayInfo() 
{
	
};

MatroskaChapterEntry::MatroskaChapterEntry() 
{
	chapterUID = 0;
	timeStart = 0;
	timeEnd = 0;
	parent = NULL;
}

MatroskaChapterEntry::MatroskaChapterEntry(MatroskaChapterEntry *_parent)
{
	assert(_parent != NULL);
	MatroskaChapterEntry();
	parent = _parent;
}

MatroskaChapterEntry::~MatroskaChapterEntry() 
{

}

UTFstring &MatroskaChapterEntry::GetTimeStartStr()
{
	if (m_timeStartStr.length() == 0) {
		wchar_t buffer[256];
		
#ifdef WIN322
		StrFromTimeIntervalW(buffer, 255, (DWORD)timeStart, 5);

		m_timeStartStr = buffer;
#else
		m_timeStartStr = L"";
#endif		
	}

	return m_timeStartStr;
}

UTFstring &MatroskaChapterEntry::GetTimeEndStr()
{
	if (m_timeEndStr.length() == 0) {
		wchar_t buffer[256];
		
#ifdef WIN322
		StrFromTimeIntervalW(buffer, 255, (DWORD)timeEnd, 5);

		m_timeEndStr = buffer;
#else
		m_timeEndStr = L"";
#endif		
	}

	return m_timeEndStr;
}

#ifdef USING_TINYXML
void MatroskaChapterEntry::ListAsXML(TiXmlElement &baseElement)
{
	size_t i;
	uint32 HH, MM, SS, mm;
	char buffer[65];

	TiXmlElement chapterElement("ChapterAtom");

#if 0
	TiXmlElement chapterUIDElement("ChapterUID");
	snprintf(buffer, 64, "%u", chapterUID);
	TiXmlText chapterUIDText(buffer);

	chapterUIDElement.InsertEndChild(chapterUIDText);
	chapterElement.InsertEndChild(chapterUIDElement);
#endif

	TiXmlElement chapterTimeStartElement("ChapterTimeStart");
	HH = timeStart / 1000 / 60 / 60;
	MM = (timeStart / 1000 / 60) % 60;
	SS = (timeStart / 1000) % 60;
	mm = timeStart % 1000;
	snprintf(buffer, 64, "%02u:%02u:%02u.%03u", HH, MM, SS, mm);
	TiXmlText chapterTimeStartText(buffer);

	chapterTimeStartElement.InsertEndChild(chapterTimeStartText);
	chapterElement.InsertEndChild(chapterTimeStartElement);

	TiXmlElement chapterTimeEndElement("ChapterTimeEnd");
	HH = timeEnd / 1000 / 60 / 60;
	MM = (timeEnd / 1000 / 60) % 60;
	SS = (timeEnd / 1000) % 60;
	mm = timeEnd % 1000;
	snprintf(buffer, 64, "%02u:%02u:%02u.%03u", HH, MM, SS, mm);
	TiXmlText chapterTimeEndText(buffer);

	chapterTimeEndElement.InsertEndChild(chapterTimeEndText);
	chapterElement.InsertEndChild(chapterTimeEndElement);

	for (i = 0; i < display.size(); i++) {
		MatroskaChapterDisplayInfo &displayInfo = display.at(i);

		TiXmlElement chapterDisplayElement("ChapterDisplay");

		//if (displayInfo.string.length() > 0) {
			TiXmlElement chapterStringElement("ChapterString");
			TiXmlText chapterStringText(displayInfo.string.GetUTF8().c_str());

			chapterStringElement.InsertEndChild(chapterStringText);
			chapterDisplayElement.InsertEndChild(chapterStringElement);
		//}

		//if (displayInfo.lang.length() > 0) {
			TiXmlElement chapterLanguageElement("ChapterLanguage");
			TiXmlText chapterLanguageText(displayInfo.lang.c_str());

			chapterLanguageElement.InsertEndChild(chapterLanguageText);
			chapterDisplayElement.InsertEndChild(chapterLanguageElement);
		//}

		if (displayInfo.country.length() > 0) {
			TiXmlElement chapterTimeEndElement("ChapterCountry");
			TiXmlText chapterTimeEndText(displayInfo.country.c_str());

			chapterTimeEndElement.InsertEndChild(chapterTimeEndText);
			chapterDisplayElement.InsertEndChild(chapterTimeEndElement);
		}

		chapterElement.InsertEndChild(chapterDisplayElement);
	}

	for (i = 0; i < tracks.size(); i++) {		
		TiXmlElement chapterTrackNumberElement("ChapterTrackNumber");
		snprintf(buffer, 255, "%u", (uint32)tracks.at(i));
		TiXmlText chapterTrackNumberText(buffer);

		chapterTrackNumberElement.InsertEndChild(chapterTrackNumberText);
		chapterElement.InsertEndChild(chapterTrackNumberElement);
	}
	
	children.ListAsXML(chapterElement);

	baseElement.InsertEndChild(chapterElement);
}
#endif // USING_TINYXML

//}; // namespace MatroskaUtilsNamespace
/**
	*  Part of The Matroska Shell Extension and The TCMP Matroska CDL
	*
	*  MatroskaUtils.cpp
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
	\file MatroskaUtils.cpp
	\version \$Id: MatroskaUtils.cpp,v 1.74 2004/04/28 04:22:43 jcsston Exp $
	\brief Some general code to make working with Matroska easier
	\author Jory Stone     <jcsston @ toughguy.net>
	\author John Cannon    <spyder482 @ yahoo.com>

	\note Inspired by ;) John Cannon (spyder482@yahoo.com) 2003
	
	
	\todo [16:55] <Goldenear> jcsston: the bugs : 1) you can't delete a tag, the delete button won't work :-(
  \todo [16:56] <Goldenear>  2) if you add a tag and if there is an existing tag with the same name, the existing tag will be updated and no new tag is added :-(
	\todo [17:00] <Goldenear> I also have a new feature to request: still about tags, could you add support for an EditionUID target? (according to the latest matroska specs it's now possible to add tags to an EditionEntry) :-D
	\todo <ZarG> i've got an mkv file with a muxed SSA
        <ZarG> when i attach a picture to make a thumbnail, the thumb isn't displayed
        <ZarG> the bug seems to appear only with mkv with muxed ssa (or a third track, i don't know)
        <ZarG> then when i switch tu thumbnail mode in explorer, either the thumb isn't displayed, or sometimes that makes explorer crash
        <ZarG> at the moment, the shell extension crashes when i try to right-click on the mkv file
        (has ffdshow)
        <ZarG> no, i've got the last koepi build before 1.0 (june 2003)  
	\todo <Haali> jcsston: did you take a look at the "Dimensions" column?, <Haali> jcsston: {6444048F-4C8B-11D1-8B70-080036B11A03}/13
	\todo Add more column's to the handler	
	\todo Add option to dis/enable caching of decoded attached images in the Attachment Tab	
	\todo also, i think a copy-file-info-to-clip-board feature would be very helpful for you	
	\todo I have to bug Gabest about this. Report by Animaniac, the other clip with the 0 fps was made by using mkvmerge to take the avi and make an mkv with the right AR, then i muxed the mkv with 6 audio and 1 subtitle with gabests DSF		
	\todo Copy Matroska file, read each level 1 element and render to a new file
	\todo FourCC changer?
	\todo DShow, Enum through open pins in graph
	\todo DONE Add bitrate display mode
	\todo FIXED. <SLider3> jcsston what i really like to understande is why the track 2 is the last of the list (have 10 tracks total)  <SLider3> when the items are overlap the track 2 is the 2nd of the list but when the icons appear correct it goes to the last position (but still named track 2)
	\todo DONE. <SLider3> why didnt you put something like a remove button?
	\todo DONE Display bytes sizes of MetaSeek and Cue elements, prehaps display which tracks have cues.
	\todo DONE WM_DESTORY free PageData
	\todo DONE. Fix up Simple Tag Tab
	\todo DONE. Add FirstTime reg entry, run ConfigureOptions if FirstTime	
	\todo DONE. Add Tag's ID3v1 style, simply append to the end of the file with 8 byte alignment. On reading scan the last 1MB? of the file for the start of a tag element, then detect if the size of tag element will not pass the the end of the file
	\todo DONE. Remove the attachment text dialog and use a auto-hiding edit box instead, Replace the attached text file dialog with a dynamic text box inside the attachment page, (support Unicode and ASCII instead of forcing ASCII)	
	\todo DONE. Display a warning dialog when user clicks 'View Attachment'
	\todo DONE. Add re-reg button to shortcut list
	\todo DONE. Right-click Context Menu Muxing?
	\todo DONE. by Animaniac, instead of displaying each track, maybe something like, x Video Streams. y Audio Streams. z Text Streams. to be more consistent with other tooltops, Video Streams: x, Audio Streams: x	
	\todo DONE. Add track language changer
	\todo DONE. (for manual re-loading of attachments) Add progress bar for parsing file
	\todo FIXED. Unable to reproduce. Report by Pamel, Any clip created with VDMod requires me to click "Load Attachments".  Any clip created with mkvmerge just works.
	\todo DONE. Add option to enable/disable Context Menu's (menu's crash under Win98)	

	\par Change Log
	- 03-19-2004
		- Added Export Chapters as XML support
		- Fixed YUY2 thumbnails, removed YV12 support for thumbnails (it didn't work)
	- 03-07-2004
		- Fixed a b0rk in the attachment opening code, attachments with non 3 letter exts failed to open
		- Fixed a bug in the Bitrate scanner, it would crash on files without any tracks
		- Updated the Bitrate scanner for track indexing starting at 0 (was 1)
		- Fixed a bug in the JCThread class, sometimes the thread would exit too quickly
	- 03-06-2004
		- Fixed a bug in the attachment code for calling an exernal app to open a attachment, the '.' between the filename and ext was missing
		- The Chapters Tab didn't clear when tags were reloaded, so you ended up with dup chapters
	- 02-19-2004
    - MatroskaProp v2.6 Build 1934 Released
		- Context Menu, Column, and Property Page Shell Extensions work for Matroska files without the .mkv ext
		- Parser Cache now uses a small, lighter-weight, and (hopefully) faster class for management
	- 02-18-2004
    - Fixed a bug in the Tag display code, it was starting the indexes at 1 instead of 0
    - Can write multiple target UID(s) for Tags now
    - Added Better Handling of missing or invalid dll's
    - Changed some of the path/filename stripping code to use Win32 API for Win32 compiles
	- 02-17-2004
		- Finshed Chapter support, supports nested chapters, multiple display strings, and track UID assignments
		- Added Chapter Property Page, and Chapters to the Context Menu
		- No more Global Info track, now the info is stored in the segment infomation class
		- Changed more of the File Size and Time formating code to use Win32 API for Win32 compiles
	- 02-16-2004
		- Moved a lot of the GUI tag code into new smaller functions
		- Added a UID add dialog, to assign more UID's to a tag
		- Started some Chapter support
	- 02-15-2004
		- Now using TinyXml as a dll
		- Added the offical SimpleTag names and desc, however it crashes when I try to add them to the combo box
		- It now adds the Attachment tab only if there are attachments in the file
	- 02-14-2004
		- Added Output Sampling Rate reading
		- Moved the language table into MatroskaTrackData.cpp
		- Moved MatroskaInfoParser::Parse_Tags() into MatroskaUtilsTag.cpp
		- The tags are now track indendepent, and also can have more than one UID assigned to each
		- Changed the MatroskaInfoParser::GetTag... and MatroskaInfoParser::SetTag... methods to use Simple Tags
	- 02-13-2004
		- Moved the DShow related code into a .dll
		- Moved the different Shell Exts into their own .cpp files, CThumbnail.cpp, CTooltip.cpp, etc.
		- Started work on reworking internal tagging system
	- 02-10-2004
		- MatroskaProp v2.5 Build 1750 Released
		- Moved all the MatroskaInfoParser::WriteDisplaySize() and MatroskaInfoParser::WriteLanguage() error messages into thrown exception's. It's much cleaner now
	- 02-09-2004
		- Added an Auto-Reload option for Tags, if enabled tag will be reloaded from the file after writing
		- Moved all the MatroskaInfoParser::WriteTags() error messages into thrown exception's. It's much cleaner now
		- Changed the SimpleTag reading code to use EbmlMaster::Read() cleaner and doesn't use hacks to avoid incorrect levels
		- Added MatroskaTagUID class, for future support of tags with multiple assignments of UID's
		- Updates Segment Size when writing tags
		- Now writes less tag junk, empty base elements (Targets is an exception) are not written		
	- 02-07-2004
		- Changed the CxImage lib back to a .dll link, having it divided into modules can help with memory usage, which with a ShellExt is very important (it's always loaded)
		- Added ID3 Tag support back, also using a .dll link
		- Added SimpleTag support to the XML export function
		- Use of CxImage is not required for most features. The about dialog and attachment image decoding with gracefully fail if cximagecrt.dll cannot be loaded
	- 01-24-2004
		- MatroskaProp v2.4 Build 1690 Released
		- Fixed two bugs in the context menu Track list, instead of displaying "Video Track 1" it was displaying "T Track 1". The problem was that I had previously used char * for the track types. However, I've switched to wchar_t * so that the translations come out correctly. So the whar_t * was being treated as a char *, ending at the first NULL. The other problem was that I have a static buffer/cache for the translations, which means that the return translation is only good until the next translation request/lookup. I was calling two translations in the same snprintf() which caused them to be the same.
		- Switched the "CodecID:" text in the context menu to use the translated "Codec ID:"
		- Changed a number of the MatroskaTrackInfo elements from char * to std::string's
		- Added SimpleTag support
		- Added a bitrate usage display module
		- Added YV12 support to the DShow thumbnail generator
	- 12-21-2003
    - Fixed a bug in the attachment cover dection code, if the keyword "cover" started at 0 I would incorrectly mark it as not the cover
    - Now compiled with the VC7.1 compiler
	- 12-10-2003
    - Improved the NSIS installer considerably
		- The crash catcher is now disabled by default, can be enabled via the configuration dialog
		- Moved the CxImage lib back to a static link, I had problems getting it to find the cximagecrt.dll on a system.
	- 11-16-2003
		- Added Delete button to the tag tab
		- Updated the import to use the MP3/ID3 tag importer as well as the AVI tag importer
	- 11-15-2003
		- Split the tag code into new files
	- 11-13-2003
		- Fixed the filename filtering to ignore case
	- 11-08-2003
		- Instead of filtering out all filenames that didn't end with .mkv, .mks, or .mka. I now search the whole filename for the ext.
	- 10-29-2003
		- MatroskaProp v2.3 Build 1470 Released
	- 10-26-2003
		- Fixed a bug, if you had an attachment without a description would crash when it tried to read it ;)
	- 10-24-2003
		- Added three new MKV icons by Luke Filewalker
	- 10-23-2003
		- Fixed a bug that the Tag track list would be doubled if you went to the Tag tab, Attachment tab, and back to the Tag tab
		- The configuration dialog now updates the text when the translation is changed
		- Made a little app to display a list of all the processes that have MatroskaProp.dll loaded, along with a handy "Kill Process" button
	- 10-22-2003
		- Fixed the track sorting problem reported by SLider3, I manually sort the tracks now
	- 10-21-2003
		- MatroskaProp v2.2 Build 1333 Released
		- Added gettext() support
		- Added an option to change how long the thumbnail generator waits before giving up
		- Reworked a lot of the tagging code to use Unicode more
		- Removed some old tagging code
		- Fixed a crash in the Attachment tab when a attachment didn't have a description
	- 10-20-2003
		- Removed the Logging class, it was causing the failed load under Windows 98
		- Fixed the bug of thumbnail generation causing Matroska files to be locked (no moving/deleting/renaming)
		- The thumbnail handler now waits at the most 5 minutes when trying to generate a thumbnail via DShow before aborting
	- 10-17-2003
		- Added Zen's changes for the MatroskaInfoParser::Parse_Tags() function to compile in Borland C++ Builder
		- Fixed a bug in the XCD reader, it didn't seek back to the beginning of the file after scanning for the XCD header. Which caused my scan for a Matroska header to fail.
		- Fixed MatroskaInfoParser::WriteTags() to not create empty Multi-* elements, it was also updated to the new track access method
		- Fixed MatroskaInfoParser::SaveTagsToXMLFile(), it was not updated to the new track access method
	- 10-16-2003
		- Added a XCD parser IOCallback class, supports XCD's now
	- 10-13-2003
    - Changed some defines and functions to compile with Borland C++ CL Tools 5
	- 10-11-2003
		- Fixed the bug of the Tags sticking at 71%, I was manually scanning 3 bytes at a time for a Tags element. mkvmerge 0.7.1 doesn't write a Tags element by default and the last entry in the SeekHead was the attachments, which resulted in me scanning about 99.99% of the file
		- Fixed a bug of the Reload Tags button in the Tags tab not loading the Attachments correctly. Which caused a crash when you picked a attachment.
		- Added a Logging class that writes to a file, the log file is included in the crash/dump .zip file. In Debug build the Log class outputs to OutputDebugString too
		- Fixed the column handler crashing, The Get*Tag() methods were out-dated
		- I now handle std::exception's in the crash dialog, displaying the .what() value.
		- Fixed the track list icons being too close together
		- The Cancel button in the loading progress dialog doesn't close the dialog right away but only when the parser has really been stopped
		- The Track changer now works with the List View Track list
	- 10-09-2003
		- Attachment display in the context menu is loaded on-the-fly, when you first try to look at the image it is loaded, decoded, and resized
		- Fixed a bug in the thumbnail handler of it crashing if Windows supplied a NULL filename
		- Changed the thumbnail handler to use the cache
	- 10-08-2003
    - Reenabled the Context menu
    - Fixed the context menu of crashing under Win9x
    - Added attachment display to the context menu
	- 10-07-2003
    - Fixed a bug that would keep files from opening under non-Unicode OS (Win9x)
    - Removed some old unused Matroska file checking code
    - First track is now selected in the Main Track Tab automaticly
	- 10-07-2003
		- Crash dialog now lets the user enter some text, and saves all the crash infomation into a single compressed .zip
		- Added a crash button to the Main Tracks tab to allow testing the Crash Dialog
	- 10-06-2003
		- Improved/Added a Crash dialog, with buttons to save a crash report, try and ignore the crash, and a close to button to let the crash go through uneffected
		- Crash dialog also generates a stack trace
	- 10-05-2003
		- Added a AVI Tag Importer, support the extended INFO tags and MovieID tags
		- Fixed? a bug that would keep Release builds from parsing the file
		- Changed the Main track list to a list-view control, different icons for video, audio, and subtitle tracks
		- Moved the CxImage code back into a external .dll
	- 10-04-2003
    - Added a progress dialog to the Attachment and Tag tabs
    - Moved some of the code into new files
	- 09-05-2003
    - The build number is now store in a global long instead of having to look it up every time it was needed
		- Works with 4GB+ files
		- Fixed a bug in the Thumbnail handler where it would crash when searching for attachments
		- Add a Save Crash Dump feature
	- 08-26-2003
		- Did some memory usage tests for MatroskaInfoParser, it amazing only uses 4KB-10KB
		- Added Audio/Video columns to the column handler
		- Added Track Language updater
		- Added Icon chooser for the Shell Ext (change the icon displayed for Matroska files)
	- 08-24-2003
		- Fixed the limits of the spin control for the Rating in the Simple Tag Tab to -10 and 10
	- 08-23-2003
		- If the file has attachments the Attachment Tab is displayed in the CDL now
		- Added Total (support for more than one element) MetaSeek and Cues Size display
		- The About dialog now has a cheesy Flame/Fire effect
		- Killed another memory leak. In my JList if the list only had one item it wasn't deleted
	- 08-22-2003
		- Finished Tag XML Export function
		- Changed the MatroskaPages to do Auto-Reference counting, (deletes itself when no longer needed, instead of being stored in a list and deleted on dll unload)
		- Fixed a memory leak in the Attachment Tab, if the attachment count is zero it will re-parse the file to make sure no attachments were skipped. I forgot to delete the existing parser before creating a new one
		- Killed another memory leak (72 bytes) when trying to find a tag element that doesn't have a seek entry. I read the last element in the SeekHead and I forgot to delete the element pointer after I was done with it. This slight memory leak only happened on Matroska files that had some empty space after the last Seek Entry or had Tags I added.
	- 08-21-2003
		- Changed the Track list and Attachment to using std::vector instead my own single linked list
	- 08-20-2003
		- Started Tag XML Export function
	- 08-18-2003
		- Added Simple Tag Editor
	- 08-11-2003
		- Added a Save As button for the Attachments (for extracting them)
		- Moved my MatroskaUilsFileReader wrapper into it's own .h (so I can use it in the attachment class)
	- 08-10-2003
		- Alexnoe's YUV2->RGB asm routines added to the Internal Thumbnail Render
		- Adjusted the Thumbnail handler to take input other than Matroska files (because the normal Avi Thumbnail handler is SO buggy)
		- Fix a bug that happened with a file muxed by an early version of Gabest's MatroskaMuxer, I was assuming that the byte after the FOURCC in the BITMAPINFOHEADER (in the CodecPrivate) was 0, and so my FOURCC lookup tables couldn't find 'XVID@c(' and returned NULL, which I then accessed causing the catch handlers to catch it and failing to add any tracks to the list (happened on the first track a video track).
		- The tag writing now double-checks to make sure it doesn't write at the beginning of the Matroska file (overwriting the EBML headers)
	- 08-09-2003
		- ID3v1 style tag writing now used if the file doesn't have an existing Tags element
  - 08-08-2003
    - Fixed a bug where the Attachments would be parsed twice. Once from the SeekHead and another time after the Tracks, resulting in double entries
	- 08-06-2003
		- Column Shell Extension starting to look good :)
	- 08-05-2003
		- Fixed two small memory leaks in MatroskaAttachmentItem::OpenViaShell() the filenames were not freed		
	- 08-04-2003
		- Fixed a little bug that caused the Tag tab to disappear when clicked under Win98SE, I had neglected to call InitCommonControlsEx() for the Time/Date Picker control
	- 08-02-2003
		- CHIP Release
		- Added an internal DShow Video Render filter with the help of Gabest, instead of using buggy VFW it now has a chance to use ffdshow
		- Images are now correctly resized, Rewrote the SmartResize() function. It was too b0rked to fix :P
		- Fixed a memory leak in the tag editing
		- Changed the Dialogs from WS_POPUP to WS_CHILD, so that a minimized TCMP wouldn't grab focus when changing the songs
	- 07-31-2003 
		- Finshed support for Multi-Dates
		- Looks up the FourCC and ACM Tag in a list for nice names
	- 07-30-2003
		- Tags where not always seeking properly, fixed
	- 07-29-2003
		- YAHOO, Tags are working r/w
		- Added some functions to easily get the artist, title, album, genre, etc.
	- 07-28-2003
		- Linking CxImage staticly now (no dll's :D)
		- Fixed the memory leaks on prop pages
		- Fixed a bug of the advanced track infomation would not show completely if it had been on advanced previously
		- Tags are starting to work :)
	- 07-27-2003
		- Reads Desc for attachments now :)
		- Fixed a slew of bugs effecting Win9x (non-Unicode)
	- 07-26-2003
		- Add configuration options to enable/disable Context Menu's (they crash on my Win98 test machine), added an option for the thumbnails that show up on the task view of a folder
	- 07-24-2003
		- The Tooltip Handler is now _free_ of memory leaks ;) I fixed 24 leaks totaling 1942 bytes, (all of them were in the CheckForMatroska() and Parse() functions)
		- Added code to change the Language of a track, Also exported in Exports.cpp
		- Now the Display Size button only appears if the selected track is a video track
		- The Context Menu muxer now supports more than one input filename internally
	- 07-23-2003
		- Tags _should_ work now, (I have some file access issues to solve to be sure)
		- Started adding Context Menu Matroska Muxing (using DirectShow)
	- 07-22-2003		
		- Improved the Display Size dialog, it now changes the size on the selected track (checking if the track is a video one), Now I fill the dialog with the previous display size and have added spin buttons
		- Added support for Unicode Filenames
		- Now compiled as a pure Unicode build (my previous dynamic Unicode detection didn't work with the Shell Extension only the CDL displayed Unicode correctly), using Unicows for Win9x systems
		- Did some more debugging of the tagging
	- 07-21-2003
		- Updated some of the tag reading code
	- 07-20-2003
		- Now if there isn't a attached picture the Thumbnail generator will take a frame from the video stream and use it. The frame that will be selected is configurable.
	- 07-19-2003
		- Added option for long or short Tooltips
		- For the Attachment tab you can now Double-click to view a attached file instead of clicking the View Attachment button
		- Moved all the MatroskaUtils code into a namespace
	- 07-18-2003
		- In the Matroska Info tab, When changing the selected track the Basic or Advanced Infomation selection is remebered
		- Progress dialog added for when you click 'Load Attachments'
		- Uses the attached file named cover*.* for the Thumbnails, else it will use the last attached picture
	- 07-17-2003
		- Added Attachment Tab
		- Added Configuration Dialog Box and Reg settings
	- 07-16-2003
		- Added Thumbnail Shell Ext		
		- Add bitrate display, I do an avg bitrate of the whole file using the Duration and the Total File Size
	- 07-15-2003
		- Added ToolTip		
		- Improved the speed of parsing. Once I hit the first cluster I exit the parsing
	- 05-02-2003 to 07-14-2003
		- I didn't keep a change log.
		- John sent me the Matroska CDL sources on 05-02-2003
		- I started the Matroska Shell Ext around 05-26-2003
	
	
	\note [23:32] Gabest: if you want to make sure this renderer gets connected, 
	use IGraphBuilder::AddSourceFilter to load the source filter and 
	call IGraphBuilder::Connect(..) on the output of that and the renderer's input

	\remarks Info about adding the Shell Extensions
	ms-help://MS.VSCC/MS.MSDNVS/shellcc/platform/Shell/Shell_Int/shell_ext.htm
	Reg under HKEY_CLASSES_ROOT\\MatroskaFile\\ShellEx\\ ,
	Thumbnail image handler {BB2E617C-0920-11d1-9A0B-00C04FC2D6C1},
	Infotip handler {00021500-0000-0000-C000-000000000046} 

	The Summary Information Property Set
	ms-help://MS.VSCC/MS.MSDNVS/com/stgasstg_7agk.htm

*/

#include "MatroskaUtils.h"
//#ifdef WIN32
//#include <Shlwapi.h>
//#endif

//using namespace MatroskaUtilsNamespace;
//namespace MatroskaUtilsNamespace {

using namespace LIBEBML_NAMESPACE;
using namespace LIBMATROSKA_NAMESPACE;

/*******************************************
*  Begin MatroskaInfoParser class.        *
*******************************************/

//Initializes the class
MatroskaInfoParser::MatroskaInfoParser(const wchar_t* MatroskaFilename)
{
	TRACE1("MatroskaInfoParser::MatroskaInfoParser(wchar_t *MatroskaFilename = %S)", MatroskaFilename);

	m_IsValid = IsMatroska(MatroskaFilename);
	TRACE1("IS VALID %d", m_IsValid);

	//Copy the filename
	m_filename = MatroskaFilename;

	Initialize();
}

MatroskaInfoParser::MatroskaInfoParser(const char* MatroskaFilename)
{
	TRACE1("MatroskaInfoParser::MatroskaInfoParser(char *MatroskaFilename = %s)", MatroskaFilename);
	
	m_IsValid = IsMatroska(MatroskaFilename);
	
	//Copy the filename
	m_filename = JString(MatroskaFilename).c_str();

	Initialize();
}

int MatroskaInfoParser::Initialize()
{
	ODS("MatroskaInfoParser::Initialize()");

	m_MetaSeekSize = 0;
	m_TrackSize = 0;
	m_CuesSize = 0;	

	cluster_count = 0;
	lastSelectedTrack = NULL;
	m_tagElementPos = 0;
	m_tagElementSize = 0;

	file_size = 0;
	m_CurrentPercent = 0;

	currentFilePos = 0;	
	keepProcessing = true;

	really_found_tags = false;
	found_tags = false;
	found_tracks = false;
	found_info = false;
	found_attachments = false;

	m_parseSeekHead = true;
	//By default we read tags & attachments
	m_parseTags = true;
	m_parseAttachments = true;
	//And don't parse the whole file
	m_parseWholeFile = false;

	return 0;
};

//Destroys the class
MatroskaInfoParser::~MatroskaInfoParser()
{
	ODS("MatroskaInfoParser::~MatroskaInfoParser()");

}

bool MatroskaInfoParser::IsMatroska(JString file_to_check)
{
	TRACE1("MatroskaInfoParser::IsMatroska(file_to_check = %S)", file_to_check.c_str());
	try {
		if (file_to_check.length() == 0)
			//Invalid filename string
			return false;

		//TRACE("Checking this file: %S", file_to_check);

		if (file_to_check.length() < 5)
			return false;

		TRACE("Passed filename length check");

		/*long index = file_to_check.Find(L"mkv");
		index += file_to_check.Find(L"mka");
		index += file_to_check.Find(L"mks");
		if (index == -3)
			//mkv, mks, mka not found in the filename
			return false;*/
		
		{	
			TRACE("Passed filename ext check");

			MatroskaUilsFileReader input_File(file_to_check.c_str(), MODE_READ);
			if (!input_File.IsOk())
				return false;

			binary ebmlHeader[4] = { 0, 0, 0, 0 };
			input_File.read(ebmlHeader, 4);
			if (ebmlHeader[0] == 0x1A && ebmlHeader[1] == 0x45 && ebmlHeader[2] == 0xDF && ebmlHeader[3] == 0xA3)
				return true;
			
#ifdef MATROSKA_EXTRA_FILE_CHECK
			TRACE("Checking it even more???");

			int UpperElementLevel = 0;
			bool bMatroskaFile = false;
			bool bDocTypeFound = false;
			//Open an ebml stream
			EbmlStream aStream(input_File);
			//define our two levels of elements
			EbmlElement *ElementLevel0;
			EbmlElement *ElementLevel1;
			//find the ebml head
			ElementLevel0 = aStream.FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
			if(ElementLevel0 != NULL){
				if (EbmlId(*ElementLevel0) == EbmlHead::ClassInfos.GlobalId) {
					//Find the next Level 1 element and check to see if it is our doctype
					ElementLevel1 = aStream.FindNextElement(ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false);
					while(ElementLevel1 != NULL) {
						if(EbmlId(*ElementLevel1) == EDocType::ClassInfos.GlobalId){
							EDocType & elem = *static_cast<EDocType*>(ElementLevel1);
							elem.ReadData(aStream.I_O());
							bDocTypeFound = true;
							std::string docType = *static_cast<EbmlString*>(&elem);
							if(docType == "matroska"){
								//This is a matroska file, return true.
								bMatroskaFile = true;
							}
						}
						delete ElementLevel1;
						ElementLevel1 = NULL;
						if (!bDocTypeFound)
							ElementLevel1 = aStream.FindNextElement(ElementLevel0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, false);
					}
				}		
				delete ElementLevel0;
			}
			input_File.close();
			return bMatroskaFile;
#endif // MATROSKA_EXTRA_FILE_CHECK
		}
	}catch (...) {
		//An error
	}
	return false;
}
int MatroskaInfoParser::ParseFile(){
	TRACE("MatroskaInfoParser::ParseFile()");

	int UpperElementLevel = 0;
	//bool vfw_codec = false;
	really_found_tags = false;
	found_tags = false;
	found_tracks = false;
	found_info = false;
	found_attachments = false;
	keepProcessing = true;
	bool clusters_hit = false;	
	// Elements for different levels
	EbmlElement *l0 = NULL;
	EbmlElement *l1 = NULL;
	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	//EbmlElement *l4 = NULL;
	//EbmlElement *l5 = NULL;
	EbmlStream *es = NULL;

	//Reset some of the class members
	cluster_count = 0;

	try {
		MatroskaUilsFileReader inputFile(m_filename, MODE_READ);
		if (!inputFile.IsOk())
			return 1;

		//Find the total size of the file
		inputFile.setFilePointer(0, seek_end);
		file_size = inputFile.getFilePointer();
		inputFile.setFilePointer(0, seek_beginning);

		es = new EbmlStream(inputFile);
		if (es == NULL) {
			UpdatePercentComplete(100);
			return 1;
		}

		// Find the EbmlHead element. Must be the first one.
		l0 = es->FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			inputFile.close();
			UpdatePercentComplete(100);
			return 1;
		}
		//We must have found the EBML head :)
		l0->SkipData(static_cast<EbmlStream &>(*es), l0->Generic().Context);
		delete l0;
		
		// Next element must be a segment
		l0 = es->FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			//No segment/level 0 element found.
			UpdatePercentComplete(100);
			inputFile.close();
			return 1;
		}
		if (!(EbmlId(*l0) == KaxSegment::ClassInfos.GlobalId)) {
			delete l0;
			inputFile.close();
			UpdatePercentComplete(100);
			return 1;
		}

		UpperElementLevel = 0;
		// We've got our segment, so let's find the tracks
		l1 = es->FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		while (l1 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}

			if (EbmlId(*l1) == KaxSeekHead::ClassInfos.GlobalId) {
				this->m_MetaSeekSize += (uint32)l1->GetSize();
				// Seekheads smaller than 16KB shouldn't take any time to parse
				if ((l1->GetSize() < 1024 * 16) || m_parseSeekHead) {
					uint64 lastSeekPos = 0;
					uint64 endSeekPos = 0;
#ifdef USING_ATT
					KaxSeekHead *SeekHead = static_cast<KaxSeekHead *>(l1);
					SeekHead->Read(static_cast<EbmlStream &>(*es), KaxSeekHead::ClassInfos.Context, UpperElementLevel, l2, false);

					EbmlId *id = NULL;
					unsigned int Index0;
					for (Index0 = 0; Index0 < SeekHead->ListSize(); Index0++) {
						if ((*SeekHead)[Index0]->Generic().GlobalId == KaxSeek::ClassInfos.GlobalId) {

							KaxSeek &SeekEntry = *static_cast<KaxSeek *>((*SeekHead)[Index0]);
							unsigned int Index1;
							for (Index1 = 0; Index1 < SeekEntry.ListSize(); Index1++) {
								if (SeekEntry[Index1]->Generic().GlobalId == KaxSeekID::ClassInfos.GlobalId) {
									binary *b = NULL;
									unsigned int s = 0;
									KaxSeekID &seek_id = *static_cast<KaxSeekID *>(SeekEntry[Index1]);
									b = seek_id.GetBuffer();
									s = seek_id.GetSize();
									if(id!=NULL){
										delete id;
										id = NULL;
									}
									id = new EbmlId(b, s);
									if (*id == KaxCluster::ClassInfos.GlobalId) {									
										//We found a cluster seek entry, so add to the cluster count
										this->cluster_count++;
									}

								} else if (SeekEntry[Index1]->Generic().GlobalId == KaxSeekPosition::ClassInfos.GlobalId) {
									KaxSeekPosition &seek_pos = *static_cast<KaxSeekPosition *>(SeekEntry[Index1]);									
									lastSeekPos = uint64(seek_pos);
									if(id){
#ifdef USING_TAG_DATA
									if ((*id == KaxTags::ClassInfos.GlobalId) && m_parseTags) {
										ODS("Found Tags Seek Entry Postion\n");
										uint64 orig_pos = inputFile.getFilePointer();									
										if (lastSeekPos > file_size) {
											ODS("Tags Past End of File! Corrupt File Suspected\n");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
																					

											found_tags = true;
											EbmlElement *levelUnknown = es->FindNextID(KaxTags::ClassInfos, 0xFFFFFFFFFFFFFFFFL);										
											this->Parse_Tags(es, levelUnknown);
											delete levelUnknown;

										}
										inputFile.setFilePointer(orig_pos);
									} else 
#endif
									if ((*id == KaxAttachments::ClassInfos.GlobalId) && m_parseAttachments) {
										ODS("Found Attachments Seek Entry Postion\n");
										uint64 orig_pos = inputFile.getFilePointer();
										if (lastSeekPos > file_size) {
											ODS("Attachments Past End of File! Corrupt File Suspected\n");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
											EbmlElement *levelUnknown = es->FindNextID(KaxAttachments::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
											found_attachments = true;
											this->Parse_Attachments(es, levelUnknown);
											delete levelUnknown;
										}
											inputFile.setFilePointer(orig_pos);
									}/* else if ((*id == KaxCues::ClassInfos.GlobalId) && m_parseAttachments) {
										ODS("Found Cues Seek Entry Postion\n");
										uint64 orig_pos = inputFile.getFilePointer();
										if (lastSeekPos > file_size) {
											ODS("Cues Past End of File! Corrupt File Suspected\n");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
											EbmlElement *levelUnknown = es->FindNextID(KaxCues::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
											this->m_CuesSize += levelUnknown->GetSize();
											delete levelUnknown;
										}
										inputFile.setFilePointer(orig_pos);
									}*/
									// epox 16.01.2007:
									// fails (currupt heap) for http://anidb.info/f222427 (corrupt version)
									// don't see any usage of it, so.. removed for now
									}
								}
							}
						}
					}
					if(id!=NULL){
						delete id;
						id = NULL;
					}
#endif //USING_ATT
					l2 = es->FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					while (l2 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}
						UpdatePercentComplete((uint8)((float)90 / (float)(int64)m_MetaSeekSize * (float)((int64)l2->GetElementPosition() - (int64)l1->GetElementPosition())));

						if (EbmlId(*l2) == KaxSeek::ClassInfos.GlobalId) {
							//Wow we found the SeekEntries, time to speed up reading ;)
							l3 = es->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);

							EbmlId *id = NULL;
							uint64 seekPos = 0;
							while (l3 != NULL) {
								if (UpperElementLevel > 0) {
									break;
								}
								if (UpperElementLevel < 0) {
									UpperElementLevel = 0;
								}

								if (EbmlId(*l3) == KaxSeekID::ClassInfos.GlobalId) {
									binary *b = NULL;
									uint16 s = 0;
									KaxSeekID &seek_id = static_cast<KaxSeekID &>(*l3);
									seek_id.ReadData(es->I_O(), SCOPE_ALL_DATA);
									b = seek_id.GetBuffer();
									s = (uint16)seek_id.GetSize();
									delete id;
									id = NULL;
									id = new EbmlId(b, s);

								} else if (EbmlId(*l3) == KaxSeekPosition::ClassInfos.GlobalId) {
									KaxSeekPosition &seek_pos = static_cast<KaxSeekPosition &>(*l3);
									seek_pos.ReadData(es->I_O());				
									seekPos = uint64(seek_pos);
									lastSeekPos = seekPos;
									if (endSeekPos < lastSeekPos)
										endSeekPos = seekPos;
								} else {
									
								}

								if ((id != NULL) && (seekPos != 0)) {
#ifdef USING_TAG_DATA
									if ((*id == KaxTags::ClassInfos.GlobalId) && m_parseTags) {
										ODS("Found Tags Seek Entry Postion");
										uint64 orig_pos = inputFile.getFilePointer();									
										if (lastSeekPos > file_size) {
											ODS("Tags Past End of File! Corrupt File Suspected");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));																																
											EbmlElement *levelUnknown = es->FindNextID(KaxTags::ClassInfos, 0xFFFFFFFFFFFFFFFFL);										
											if (levelUnknown != NULL) {
												found_tags = true;
												this->Parse_Tags(es, levelUnknown);
												delete levelUnknown;
											} else {
												ODS("Failed to seek to the Tags Seek Entry Postion");
											}
										}
										inputFile.setFilePointer(orig_pos);
									} else
#endif // USING_TAG_DATA
#ifdef USING_ATT
									if ((*id == KaxAttachments::ClassInfos.GlobalId) && m_parseAttachments) {
										ODS("Found Attachments Seek Entry Postion");
										uint64 orig_pos = inputFile.getFilePointer();
										if (lastSeekPos > file_size) {
											ODS("Attachments Past End of File! Corrupt File Suspected");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
											EbmlElement *levelUnknown = es->FindNextID(KaxAttachments::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
											if (levelUnknown != NULL) {
												found_attachments = true;
												this->Parse_Attachments(es, levelUnknown);
												delete levelUnknown;
											} else {
												ODS("Failed to seek to the Attachments Seek Entry Postion");
											}
										}
											inputFile.setFilePointer(orig_pos);
									} else
#endif // USING_ATT
									if ((*id == KaxCues::ClassInfos.GlobalId) && m_parseAttachments) {
										ODS("Found Cues Seek Entry Postion");
										uint64 orig_pos = inputFile.getFilePointer();
										if (lastSeekPos > file_size) {
											ODS("Cues Past End of File! Corrupt File Suspected\n");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
											EbmlElement *levelUnknown = es->FindNextID(KaxCues::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
											if (levelUnknown != NULL) {
												this->m_CuesSize += (uint32)levelUnknown->GetSize();
												delete levelUnknown;
											} else {
												ODS("Failed to seek to Cues");
											}
										}
										inputFile.setFilePointer(orig_pos);

									} else

#ifdef USING_CHAPTER_DATA
									if (*id == KaxChapters::ClassInfos.GlobalId) {
										ODS("Found Chapters Seek Entry Postion");
										uint64 orig_pos = inputFile.getFilePointer();
										if (lastSeekPos > file_size) {
											ODS("Cues Past End of File! Corrupt File Suspected\n");
										}else {
											inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(lastSeekPos));
											EbmlElement *levelUnknown = es->FindNextID(KaxChapters::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
											if (levelUnknown != NULL) {
												this->Parse_Chapters(es, levelUnknown);
												delete levelUnknown;
											} else {
												ODS("Failed to seek to KaxChapters");
											}
										}
										inputFile.setFilePointer(orig_pos);
									} else 
#endif // USING_CHAPTER_DATA
									if (*id == KaxCluster::ClassInfos.GlobalId) {									
										//We found a cluster seek entry, so add to the cluster count
										this->cluster_count++;
									} else {
										//
									}


									// Reset the seek entry
									seekPos = 0;
									assert(id != NULL);
									delete id;
									id = NULL;
								}

								l3->SkipData(static_cast<EbmlStream &>(*es), l3->Generic().Context);
								delete l3;
								l3 = NULL;
								if (keepProcessing)
									l3 = es->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							} // while (l3 != NULL)
							delete id;
							id = NULL;
						} else {

						}

						if (UpperElementLevel > 0) {    // we're coming from l3
							UpperElementLevel--;
							delete l2;
							l2 = l3;
							if (UpperElementLevel > 0)
								break;

						} else {
							l2->SkipData(static_cast<EbmlStream &>(*es), l2->Generic().Context);
							delete l2;
							l2 = NULL;
							if (keepProcessing)
								l2 = es->FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						}
					} // while (l2 != NULL)
#ifdef USING_TAG_DATA
					if (!found_tags && m_parseTags)
					{
						//Tag element never found, start search for ID3v1 style tags
						//First find the last element
						uint64 orig_pos = inputFile.getFilePointer();									
						if (endSeekPos > file_size) {
							ODS("Last Seek entry is Past End of File! Corrupt File Suspected");
						} else {						
							inputFile.setFilePointer(static_cast<KaxSegment *>(l0)->GetGlobalPosition(endSeekPos));
							EbmlElement *levelUnknown = es->FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
							if (levelUnknown != NULL) {
								inputFile.setFilePointer(levelUnknown->GetSize(), seek_current);
								delete levelUnknown;
								levelUnknown = NULL;
							} else {
								ODS("Failed to seek to the Last Seek entry");
							}
							uint64 scanSize = file_size - inputFile.getFilePointer();		
							// A Tags entry larger than 1MB? I doubt it
							if (scanSize > 1024 * 1024) {
								scanSize = 1024 * 1024;
								inputFile.setFilePointer((int64)scanSize * -1, seek_end);
							}
							if (scanSize > 0)
							{
								binary *Buffer = new binary[3];
								while (inputFile.read(Buffer, 3) >= 3)
								{//0x18
									if ((Buffer[0] == 0x54) && (Buffer[1] == 0xc3) && (Buffer[2] == 0x67))
									{
										uint64 startPos = inputFile.getFilePointer();

										//seek back 3 bytes, so libmatroska can find the Tags element Ebml ID
										inputFile.setFilePointer(-4, seek_current);

										EbmlElement *levelUnknown = es->FindNextID(KaxTags::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
										if ((levelUnknown != NULL) && (file_size >= startPos + levelUnknown->GetSize()) && (EbmlId(*levelUnknown) == KaxTags::ClassInfos.GlobalId))
										{
											found_tags = true;
											this->Parse_Tags(es, levelUnknown);
											break;
										}
										delete levelUnknown;

										//Restore the file pos
										inputFile.setFilePointer(startPos);
									}
									//seek back 2 bytes
									inputFile.setFilePointer(-2, seek_current);
								}
								//Free the scanning buffer
								delete [] Buffer;							
							} else {
								m_tagElementPos = file_size;
							}
							inputFile.setFilePointer(orig_pos);
						}
					}
#endif // USING_TAG_DATA
					}
			}else if (EbmlId(*l1) == KaxInfo::ClassInfos.GlobalId) {
				// General info about this Matroska file
				found_info = true;
				this->Parse_InfomationSegment(es, l1);

			}
#ifdef USING_TAG_DATA
			else if ((EbmlId(*l1) == KaxTags::ClassInfos.GlobalId) && m_parseTags && !found_tags) {
				found_tags = true;
				this->m_TrackSize = (uint32)l1->GetSize();
				int count_of_tag_found = this->Parse_Tags(es, l1);

			}
#endif // USING_TAG_DATA
			else if (EbmlId(*l1) == KaxTracks::ClassInfos.GlobalId) {
				// Yep, we've found our KaxTracks element. Now find all tracks
				// contained in this segment. 
				found_tracks = true;
				this->Parse_Tracks(es, l1);

			} else if (EbmlId(*l1) == KaxCluster::ClassInfos.GlobalId) {
				clusters_hit = true;

			}
#ifdef USING_ATT
			else if ((EbmlId(*l1) == KaxAttachments::ClassInfos.GlobalId) && m_parseAttachments && !found_attachments) {
				// Yep, we've found our KaxAttachment element. Now find all attached files
				// contained in this segment. 				
				this->Parse_Attachments(es, l1);	
			}
#endif // USING_ATT
#ifdef USING_CHAPTER_DATA
			else if (EbmlId(*l1) == KaxChapters::ClassInfos.GlobalId) {
				this->Parse_Chapters(es, l1);
			}
#endif // USING_CHAPTER_DATA
			// Update current pos
			currentFilePos = inputFile.getFilePointer();
			UpdatePercentComplete((uint8)((float)100 / (float)(int64)file_size * (float)(int64)currentFilePos));

			if (UpperElementLevel > 0) {		// we're coming from l2
				UpperElementLevel--;
				delete l1;
				l1 = l2;
				if (UpperElementLevel > 0)
					break;
			} else {
				l1->SkipData(static_cast<EbmlStream &>(*es), l1->Generic().Context);
				delete l1;
				l1 = NULL;
				
				if (keepProcessing && (!clusters_hit && !m_parseWholeFile))
					l1 = es->FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		} // while (l1 != NULL)
		delete l0;
		delete es;
	} catch (std::exception &ex) {
		std::string errorMsg;
		errorMsg = _("Error Parsing Matroska File");
		errorMsg += "\n";
		errorMsg += _("Exception: ");
		errorMsg += ex.what();	
		
		throw std::runtime_error(errorMsg);		
	} catch (...) {
		UpdatePercentComplete(100);		
#ifndef DEBUG
		// Release
		throw std::runtime_error(_("Error Parsing Matroska File"));
#else
		// Debug
		throw;
#endif
	}
	UpdatePercentComplete(100);
	return 0;
};

int MatroskaInfoParser::Parse_InfomationSegment(EbmlStream *inputfile_stream, EbmlElement *infomation_element) {
	ODS("MatroskaInfoParser::Parse_InfomationSegment()");	

	CRASH_PROTECT_START;

	if ((inputfile_stream == NULL) || (infomation_element == NULL))
		return -1;

	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	int UpperElementLevel = 0;

	// Store the Info Segement data in a segment info class

	l2 = inputfile_stream->FindNextElement(infomation_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	while (l2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}

		if (EbmlId(*l2) == KaxTimecodeScale::ClassInfos.GlobalId) {
			KaxTimecodeScale &TimeScale = *static_cast<KaxTimecodeScale *>(l2);
			TimeScale.ReadData(inputfile_stream->I_O());

			m_segmentInfo.m_infoTimecodeScale = uint64(TimeScale);

		} else if (EbmlId(*l2) == KaxDuration::ClassInfos.GlobalId) {
			KaxDuration &duration = *static_cast<KaxDuration *>(l2);
			duration.ReadData(inputfile_stream->I_O());

			//m_segmentInfo.SetDuration(float(duration) * (float)m_segmentInfo.m_infoTimecodeScale / (float)1000000000.0);
			m_segmentInfo.SetDuration(float(duration));
		} else if (EbmlId(*l2) == KaxDateUTC::ClassInfos.GlobalId) {
			time_t time = 0;
			char *ctimeStr = NULL;
			KaxDateUTC & DateUTC = *static_cast<KaxDateUTC *>(l2);
			DateUTC.ReadData(inputfile_stream->I_O(), SCOPE_ALL_DATA);

			m_segmentInfo.m_infoDateUnix = DateUTC.GetEpochDate();
			time = m_segmentInfo.m_infoDateUnix;
			ctimeStr = ctime(&time);
			if (ctimeStr != NULL)
				m_segmentInfo.m_infoDate = ctimeStr;

		} else if (EbmlId(*l2) == KaxSegmentFilename::ClassInfos.GlobalId) {
			KaxSegmentFilename &tag_SegmentFilename = *static_cast<KaxSegmentFilename *>(l2);
			tag_SegmentFilename.ReadData(inputfile_stream->I_O());

			m_segmentInfo.m_infoFilename = *static_cast<EbmlUnicodeString *>(&tag_SegmentFilename);

		} else if (EbmlId(*l2) == KaxMuxingApp::ClassInfos.GlobalId)	{
			KaxMuxingApp &tag_MuxingApp = *static_cast<KaxMuxingApp *>(l2);
			tag_MuxingApp.ReadData(inputfile_stream->I_O());

			TRACE("Found MuxingApp Tag");						
			m_segmentInfo.m_infoMuxingApp = *static_cast<EbmlUnicodeString *>(&tag_MuxingApp);

		} else if (EbmlId(*l2) == KaxWritingApp::ClassInfos.GlobalId) {
			KaxWritingApp &tag_WritingApp = *static_cast<KaxWritingApp *>(l2);
			tag_WritingApp.ReadData(inputfile_stream->I_O());

			TRACE("Found WritingApp Tag");
			m_segmentInfo.m_infoWritingApp  = *static_cast<EbmlUnicodeString *>(&tag_WritingApp);

		} else if (EbmlId(*l2) == KaxTitle::ClassInfos.GlobalId) {
			KaxTitle *Title = static_cast<KaxTitle*>(l2);
			Title->ReadData(inputfile_stream->I_O());
			m_segmentInfo.m_infoTitle = UTFstring(*Title);
		}

		if (UpperElementLevel > 0) {	// we're coming from l3
			UpperElementLevel--;
			delete l2;
			l2 = l3;
			if (UpperElementLevel > 0)
				break;
		} else {
			l2->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l2->Generic().Context);
			delete l2;
			l2 = inputfile_stream->FindNextElement(infomation_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		}
	}
	delete l2;

	CRASH_PROTECT_END;

	return 0;
}
int MatroskaInfoParser::Parse_Tracks(EbmlStream *inputfile_stream, EbmlElement *tracks_element) {
	ODS("MatroskaInfoParser::Parse_Tracks()");
	
	CRASH_PROTECT_START;

	if ((inputfile_stream == NULL) || (tracks_element == NULL))
		return -1;

	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	EbmlElement *l4 = NULL;
	//EbmlElement *l5 = NULL;
	int UpperElementLevel = 0;
	bool vfw_codec = false;

	l2 = inputfile_stream->FindNextElement(tracks_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	while (l2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}

		if (EbmlId(*l2) == KaxTrackEntry::ClassInfos.GlobalId) {
			// We actually found a track entry :) We're happy now.
			MatroskaTrackInfo* matroskaTrack = new MatroskaTrackInfo(&m_segmentInfo, 1);

			l3 = inputfile_stream->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			while (l3 != NULL) {
										
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}


				// Now evaluate the data belonging to this track
				if (EbmlId(*l3) == KaxTrackNumber::ClassInfos.GlobalId) {
					KaxTrackNumber &tnum = *static_cast<KaxTrackNumber *>(l3);
					tnum.ReadData(inputfile_stream->I_O());
					matroskaTrack->SetTrackNumber(uint32(tnum));

				} else if (EbmlId(*l3) == KaxTrackUID::ClassInfos.GlobalId) {
					KaxTrackUID &track_uid = *static_cast<KaxTrackUID *>(l3);
					track_uid.ReadData(inputfile_stream->I_O());

					matroskaTrack->SetTrackUID(uint64(track_uid));
					if (GetTrackInfoByUID(uint64(track_uid)) != NULL)
					{
						//Um..., this track is already in, Let's move any stuff we're already put in the current track to the correct track
						throw std::runtime_error(_("More than one track found with the same UID, aborting..."));
						/* OLD CODE
						MatroskaTrackInfo *temp = matroskaTrack;
						matroskaTrack = GetTrackInfoByUID(uint64(track_uid));
						matroskaTrack->MergeWithThisTrackInfo(temp);
						*/
					}

				} else if (EbmlId(*l3) == KaxTrackTimecodeScale::ClassInfos.GlobalId) {
					KaxTrackTimecodeScale &tts = *static_cast<KaxTrackTimecodeScale *>(l3);
					tts.ReadData(inputfile_stream->I_O());

					matroskaTrack->SetTrackTimecodeScale(double(tts));

				} else if (EbmlId(*l3) == KaxTrackType::ClassInfos.GlobalId) {
					KaxTrackType &ttype = *static_cast<KaxTrackType *>(l3);
					ttype.ReadData(inputfile_stream->I_O());

					matroskaTrack->SetTrackType(uint8(ttype));

				} else if (EbmlId(*l3) == KaxTrackAudio::ClassInfos.GlobalId) {
					MatroskaAudioTrackInfo *audioTrack_info = new MatroskaAudioTrackInfo;

					l4 = inputfile_stream->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					while (l4 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}

						if (EbmlId(*l4) == KaxAudioSamplingFreq::ClassInfos.GlobalId) {
							KaxAudioSamplingFreq &freq = *static_cast<KaxAudioSamplingFreq*>(l4);
							freq.ReadData(inputfile_stream->I_O());
							audioTrack_info->audio_SampleRate = float(freq);
						
						} else if (EbmlId(*l4) == KaxAudioOutputSamplingFreq::ClassInfos.GlobalId) {
							KaxAudioOutputSamplingFreq &freq = *static_cast<KaxAudioOutputSamplingFreq*>(l4);
							freq.ReadData(inputfile_stream->I_O());
							audioTrack_info->audio_OutputSampleRate = float(freq);

						} else if (EbmlId(*l4) == KaxAudioChannels::ClassInfos.GlobalId) {
							KaxAudioChannels &channels = *static_cast<KaxAudioChannels*>(l4);
							channels.ReadData(inputfile_stream->I_O());
							audioTrack_info->audio_Channels = uint8(channels);

						} else if (EbmlId(*l4) == KaxAudioBitDepth::ClassInfos.GlobalId) {
							KaxAudioBitDepth &bps = *static_cast<KaxAudioBitDepth*>(l4);
							bps.ReadData(inputfile_stream->I_O());
							audioTrack_info->audio_BitDepth = uint8(bps);

						}

						if (UpperElementLevel > 0) {
							assert(1 == 0);	// this should never happen
						} else {
							l4->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l4->Generic().Context);
							delete l4;
							l4 = inputfile_stream->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						}
					} // while (l4 != NULL)
					matroskaTrack->SetAudioInfo(audioTrack_info);
				} else if (EbmlId(*l3) == KaxTrackVideo::ClassInfos.GlobalId) {
					MatroskaVideoTrackInfo *videoTrack_info = matroskaTrack->GetVideoInfo();
					if (videoTrack_info == NULL)
					{
						//Hmm, lets make a new video track entry
						videoTrack_info = new MatroskaVideoTrackInfo();
						matroskaTrack->SetVideoInfo(videoTrack_info);
					}

					l4 = inputfile_stream->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					while (l4 != NULL) {
						if (UpperElementLevel > 0) {
							break;
						}
						if (UpperElementLevel < 0) {
							UpperElementLevel = 0;
						}

						if (EbmlId(*l4) == KaxVideoPixelWidth::ClassInfos.GlobalId) {
							KaxVideoPixelWidth &width = *static_cast<KaxVideoPixelWidth *>(l4);
							width.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_Width =	uint16(width);

						} else if (EbmlId(*l4) == KaxVideoPixelHeight::ClassInfos.GlobalId) {
							KaxVideoPixelHeight &height = *static_cast<KaxVideoPixelHeight *>(l4);
							height.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_Height =	uint16(height);

						} else if (EbmlId(*l4) == KaxVideoDisplayWidth::ClassInfos.GlobalId) {
							KaxVideoDisplayWidth &Displaywidth = *static_cast<KaxVideoDisplayWidth *>(l4);
							Displaywidth.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_DisplayWidth = uint16(Displaywidth);

						} else if (EbmlId(*l4) == KaxVideoDisplayHeight::ClassInfos.GlobalId) {
							KaxVideoDisplayHeight &Displayheight = *static_cast<KaxVideoDisplayHeight *>(l4);
							Displayheight.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_DisplayHeight =	uint16(Displayheight);

						} else if (EbmlId(*l4) == KaxVideoFrameRate::ClassInfos.GlobalId) {
							KaxVideoFrameRate &framerate = *static_cast<KaxVideoFrameRate *>(l4);
							framerate.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_FrameRate = float(framerate);
							//printf("FPS %f\n", float(framerate));
#if 0
						} else if (EbmlId(*l4) == KaxVideoFlagInterlaced::ClassInfos.GlobalId) {
							KaxVideoFlagInterlaced &flagInterlaced = *static_cast<KaxVideoFlagInterlaced *>(l4);
							flagInterlaced.ReadData(inputfile_stream->I_O());
							videoTrack_info->video_Interlaced = uint8(flagInterlaced);
#endif
						} else if (EbmlId(*l4) == KaxVideoColourSpace::ClassInfos.GlobalId) {
							KaxVideoColourSpace &colorSpace = *static_cast<KaxVideoColourSpace *>(l4);
							colorSpace.ReadData(inputfile_stream->I_O(), SCOPE_ALL_DATA);
							videoTrack_info->video_ColorDepth =	uint32(colorSpace);

						}

						if (UpperElementLevel > 0) {
							assert(1 == 0);	// this should never happen
						} else {
							l4->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l4->Generic().Context);
							delete l4;
							l4 = inputfile_stream->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						}
					} // while (l4 != NULL)					
				} else if (EbmlId(*l3) == KaxCodecID::ClassInfos.GlobalId) {
					KaxCodecID &CodecID = *static_cast<KaxCodecID*>(l3);
					CodecID.ReadData(inputfile_stream->I_O());

					matroskaTrack->SetCodecID(std::string(CodecID).c_str()); // strings for the moment (example)
				} else if (EbmlId(*l3) == KaxCodecPrivate::ClassInfos.GlobalId) {
					KaxCodecPrivate &c_priv = *static_cast<KaxCodecPrivate*>(l3);
					c_priv.ReadData(inputfile_stream->I_O(), SCOPE_ALL_DATA);

					matroskaTrack->m_cps = c_priv.GetSize();
					if ( matroskaTrack->m_cps >= TRACK_PRI_SIZE )
					{
						matroskaTrack->m_cps = TRACK_PRI_SIZE;
					}
					memcpy(matroskaTrack->m_codec_private, c_priv.GetBuffer(), matroskaTrack->m_cps);

					if ( c_priv.GetSize() >= sizeof(vfw_BITMAPINFOHEADER) ) {
						vfw_BITMAPINFOHEADER *bih = (vfw_BITMAPINFOHEADER *)c_priv.GetBuffer();						
						JString fccCode = JString::Format("%c%c%c%c", ((char *)&bih->bi_compression)[0], ((char *)&bih->bi_compression)[1], ((char *)&bih->bi_compression)[2], ((char *)&bih->bi_compression)[3]);
						matroskaTrack->m_CodecOldID = fccCode.mb_str();

					}else if ( c_priv.GetSize() >= sizeof(acm_WAVEFORMATEX) ) {
						acm_WAVEFORMATEX *bih = (acm_WAVEFORMATEX *)c_priv.GetBuffer();
						uint16 tag = *((uint16 *)bih->wFormatTag);
						const char *acm_name = MatroskaAudioTrackInfo::GetWaveFormatName(tag);
						JString acm_wav;
						if (acm_name != NULL) {
							acm_wav = acm_name;
						//	acm_wav += " ";
						}
						//acm_wav += JString::Format("(0x%04X)", *((uint16 *)bih->wFormatTag));
						//matroskaTrack->m_codecID.append(JString::Format(" (0x%04X)", tag).mb_str());
						//matroskaTrack->m_CodecOldID = matroskaTrack->m_codecID;

						//if(acm_wav.mb_str())
						//	matroskaTrack->m_CodecOldID = acm_wav.mb_str();
						//else matroskaTrack->m_CodecOldID = "NULL";
						matroskaTrack->m_CodecOldID.append(JString::Format("%04X", tag).mb_str());
					}
				} else if (EbmlId(*l3) ==  KaxTrackMinCache::ClassInfos.GlobalId) {
					KaxTrackMinCache &min_cache = *static_cast<KaxTrackMinCache*>(l3);
					min_cache.ReadData(inputfile_stream->I_O());
					matroskaTrack->m_min_cache = uint32(min_cache);

				} else if (EbmlId(*l3) == KaxTrackMaxCache::ClassInfos.GlobalId) {
					KaxTrackMaxCache &max_cache = *static_cast<KaxTrackMaxCache*>(l3);
					max_cache.ReadData(inputfile_stream->I_O());
					matroskaTrack->m_max_cache = uint32(max_cache);

				} else if (EbmlId(*l3) == KaxTrackFlagLacing::ClassInfos.GlobalId) {
					KaxTrackFlagLacing &f_lacing = *static_cast<KaxTrackFlagLacing *>(l3);
					f_lacing.ReadData(inputfile_stream->I_O());

					matroskaTrack->m_lacing_flag = uint32(f_lacing); 

				} else if (EbmlId(*l3) == KaxTrackFlagDefault::ClassInfos.GlobalId) {
					KaxTrackFlagDefault &f_default = *static_cast<KaxTrackFlagDefault *>(l3);
					f_default.ReadData(inputfile_stream->I_O());
					matroskaTrack->m_default_flag = uint32(f_default); 

				} else if (EbmlId(*l3) == KaxTrackDefaultDuration::ClassInfos.GlobalId) {
					KaxTrackDefaultDuration &defaultDuration = *static_cast<KaxTrackDefaultDuration *>(l3);
					defaultDuration.ReadData(inputfile_stream->I_O());
					//printf("DUR: %d\n", uint32(defaultDuration));

					if (matroskaTrack->GetTrackType() == track_video)
					{
						//We got a video track
						MatroskaVideoTrackInfo *videoTrack_info = matroskaTrack->GetVideoInfo();
						// If this is an video track AND we didn't fill in the framerate field with the old type element
						if (videoTrack_info == NULL)
						{
							//Hmm, lets make a video track entry anyway
							videoTrack_info = new MatroskaVideoTrackInfo();
							matroskaTrack->SetVideoInfo(videoTrack_info);
						}
						if ((videoTrack_info->video_FrameRate == 0) && (uint32(defaultDuration) != 0))
						{
							videoTrack_info->video_FrameRate = 100000 / (float)uint32(defaultDuration) * 10000;
						}
					}

				} else if (EbmlId(*l3) == KaxTrackName::ClassInfos.GlobalId) {
					KaxTrackName &name = *static_cast<KaxTrackName *>(l3);
					name.ReadData(inputfile_stream->I_O());
					matroskaTrack->m_trackName = static_cast<const UTFstring&>(name).GetUTF8();
				}else if (EbmlId(*l3) == KaxTrackLanguage::ClassInfos.GlobalId) {
					KaxTrackLanguage &language = *static_cast<KaxTrackLanguage *>(l3);
					language.ReadData(inputfile_stream->I_O());
					matroskaTrack->m_trackLanguage = std::string(language);
				}
				if (UpperElementLevel > 0) {	// we're coming from l4
					UpperElementLevel--;
					delete l3;
					l3 = l4;
					if (UpperElementLevel > 0)
						break;
				} else {
					l3->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l3->Generic().Context);
					delete l3;
					l3 = inputfile_stream->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				}
			} // while (l3 != NULL)

			if (//matroskaTrack->m_codecID.compare("V_MPEG4/ISO/ASP") &&
				matroskaTrack->m_codecID.compare("V_MS/VFW/FOURCC") &&
				matroskaTrack->m_codecID.compare("A_MS/ACM") )
				matroskaTrack->m_CodecOldID = "";
			trackInfos.AddItem(matroskaTrack);
		}

		if (UpperElementLevel > 0) {	// we're coming from l3
			UpperElementLevel--;
			delete l2;
			l2 = l3;
			if (UpperElementLevel > 0)
				break;
		} else {
			l2->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l2->Generic().Context);
			delete l2;
			l2 = inputfile_stream->FindNextElement(tracks_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		}
	} // while (l2 != NULL)
	delete l2;
	
	CRASH_PROTECT_END;

	return 0;
}
#ifdef USING_ATT
int MatroskaInfoParser::Parse_Attachments(EbmlStream *inputfile_stream, EbmlElement *attachments_element)
{
	ODS("MatroskaInfoParser::Parse_Attachments()");	
	
	if ((inputfile_stream == NULL) || (attachments_element == NULL))
		return -1;

	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	EbmlElement *l4 = NULL;
	int UpperElementLevel = 0;
	int attachment_count = 0;

	CRASH_PROTECT_START;

	l2 = inputfile_stream->FindNextElement(attachments_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
	while (l2 != NULL) {
		if (UpperElementLevel > 0) {
			break;
		}
		if (UpperElementLevel < 0) {
			UpperElementLevel = 0;
		}
		if (EbmlId(*l2) == KaxAttached::ClassInfos.GlobalId) {
			// We actually found a attached file entry :) We're happy now.            
			MatroskaAttachmentItem *the_attached_info = new MatroskaAttachmentItem();
			attachment_count++;
			//std::string mime_type;
			//UTFstring the_filename;

			l3 = inputfile_stream->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			while (l3 != NULL) {
				if (UpperElementLevel > 0) {
					break;
				}
				if (UpperElementLevel < 0) {
					UpperElementLevel = 0;
				}

				// Now evaluate the data belonging to this track
				if (EbmlId(*l3) == KaxFileName::ClassInfos.GlobalId) {
					KaxFileName &attached_filename = *static_cast<KaxFileName *>(l3);
					attached_filename.ReadData(inputfile_stream->I_O());
					the_attached_info->SetAttachmentFilename(UTFstring(attached_filename).c_str());

				} else if (EbmlId(*l3) == KaxMimeType::ClassInfos.GlobalId) {
					KaxMimeType &attached_mime_type = *static_cast<KaxMimeType *>(l3);
					attached_mime_type.ReadData(inputfile_stream->I_O());
					the_attached_info->attachmentMimeType = attached_mime_type;

				} else if (EbmlId(*l3) == KaxFileDescription::ClassInfos.GlobalId) {
					KaxFileDescription &attached_description = *static_cast<KaxFileDescription *>(l3);
					attached_description.ReadData(inputfile_stream->I_O());
					the_attached_info->attachmentDescription = attached_description;

				} else if (EbmlId(*l3) == KaxFileData::ClassInfos.GlobalId) {
					KaxFileData &attached_data = *static_cast<KaxFileData *>(l3);

					//We don't what to read the data into memory because it could be very large
					//attached_data.ReadData(inputfile_stream->I_O());

					//Instead we store the Matroska filename, the start of the data and the length, so we can read it
					//later at the users request. IMHO This will save a lot of memory
					the_attached_info->SetMatroskaFilename(m_filename);
					the_attached_info->attachmentMatroskaFileDataStart = attached_data.GetElementPosition() + attached_data.HeadSize();
					the_attached_info->attachmentMatroskaFileDataLength = (uint32)attached_data.GetSize();
				}

				if (UpperElementLevel > 0) {	// we're coming from l4
					UpperElementLevel--;
					delete l3;
					l3 = l4;
					if (UpperElementLevel > 0)
						break;
				} else {
					l3->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l3->Generic().Context);
					delete l3;
					l3 = inputfile_stream->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				}					
			} // while (l3 != NULL)
			this->fileAttachments.AddItem(the_attached_info);
		}

		if (UpperElementLevel > 0) {	// we're coming from l3
			UpperElementLevel--;
			delete l2;
			l2 = l3;
			if (UpperElementLevel > 0)
				break;
		} else {
			l2->SkipData(static_cast<EbmlStream &>(*inputfile_stream), l2->Generic().Context);
			delete l2;
			l2 = inputfile_stream->FindNextElement(attachments_element->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		}
	} // while (l2 != NULL)
	delete l2;

	CRASH_PROTECT_END;	

	return attachment_count;
};
#endif // USING_ATT
#ifdef USING_CHAPTER_DATA
int MatroskaInfoParser::Parse_Chapters(EbmlStream *inputfile_stream, EbmlElement *chapter_element)
{
	ODS("MatroskaInfoParser::Parse_Chapters()");
		
	if ((inputfile_stream == NULL) || (chapter_element == NULL))
		return -1;
	
	KaxChapters *chapters_element = static_cast<KaxChapters *>(chapter_element);
	EbmlElement *ElementLevel2 = NULL;
	int UpperElementLevel = 0;
	int chapter_count = 0;

	CRASH_PROTECT_START;

	chapters_element->Read(static_cast<EbmlStream &>(*inputfile_stream), KaxChapters::ClassInfos.Context, UpperElementLevel, ElementLevel2, false);

	unsigned int Index0;
	for (Index0 = 0; Index0 < chapters_element->ListSize(); Index0++) {
		if ((*chapters_element)[Index0]->Generic().GlobalId == KaxEditionEntry::ClassInfos.GlobalId) {

			KaxEditionEntry &EditionEntry = *static_cast<KaxEditionEntry *>((*chapters_element)[Index0]);
			unsigned int Index1;
			for (Index1 = 0; Index1 < EditionEntry.ListSize(); Index1++) {
				if (EditionEntry[Index1]->Generic().GlobalId == KaxChapterAtom::ClassInfos.GlobalId) {
					MatroskaChapterEntry *entry = NULL;
					chapter_count += Parse_ChapterAtom(*static_cast<KaxChapterAtom *>(EditionEntry[Index1]), &entry);
					if (entry != NULL) 
						chapters.Add(entry);
				}
			}
		}
	}

	CRASH_PROTECT_END;

	return chapter_count;
}

int MatroskaInfoParser::Parse_ChapterAtom(KaxChapterAtom &ChapterAtom, MatroskaChapterEntry **entry)
{
	ODS("MatroskaInfoParser::Parse_ChapterAtom()");

	int chapter_count = 1;
	MatroskaChapterEntry *newChapter = NULL;
	
	CRASH_PROTECT_START;

	newChapter = new MatroskaChapterEntry();	

	unsigned int Index0;
	for (Index0 = 0; Index0 < ChapterAtom.ListSize(); Index0++) {
		if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterUID::ClassInfos.GlobalId) {
			KaxChapterUID &chapterUID = *static_cast<KaxChapterUID *>(ChapterAtom[Index0]);

			newChapter->chapterUID = uint64(chapterUID);

		} else if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterTimeStart::ClassInfos.GlobalId) {							
			KaxChapterTimeStart &timeStart = *static_cast<KaxChapterTimeStart *>(ChapterAtom[Index0]);			

			newChapter->timeStart = uint64(timeStart) / m_segmentInfo.m_infoTimecodeScale;

		} else if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterTimeEnd::ClassInfos.GlobalId) {							
			KaxChapterTimeEnd &timeEnd = *static_cast<KaxChapterTimeEnd *>(ChapterAtom[Index0]);

			newChapter->timeEnd = uint64(timeEnd) / m_segmentInfo.m_infoTimecodeScale;

		} else if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterTrack::ClassInfos.GlobalId) {							
			KaxChapterTrack &ChapterTrack = *static_cast<KaxChapterTrack *>(ChapterAtom[Index0]);

			unsigned int Index1;
			for (Index1 = 0; Index1 < ChapterTrack.ListSize(); Index1++) {
				if (ChapterTrack[Index1]->Generic().GlobalId == KaxChapterTrackNumber::ClassInfos.GlobalId) {							
					KaxChapterTrackNumber &chapTrackNo = *static_cast<KaxChapterTrackNumber *>(ChapterTrack[Index1]);

					newChapter->tracks.push_back(uint64(chapTrackNo));
				}

			}

		} else if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterDisplay::ClassInfos.GlobalId) {							
			KaxChapterDisplay &ChapterDisplay = *static_cast<KaxChapterDisplay *>(ChapterAtom[Index0]);

			// A new chapter display string+lang+country
			MatroskaChapterDisplayInfo newChapterDisplay;
			unsigned int Index1;
			for (Index1 = 0; Index1 < ChapterDisplay.ListSize(); Index1++) {
				if (ChapterDisplay[Index1]->Generic().GlobalId == KaxChapterString::ClassInfos.GlobalId) {							
					KaxChapterString &chapString = *static_cast<KaxChapterString *>(ChapterDisplay[Index1]);

					newChapterDisplay.string = UTFstring(chapString);

				} else if (ChapterDisplay[Index1]->Generic().GlobalId == KaxChapterLanguage::ClassInfos.GlobalId) {							
					KaxChapterLanguage &chapLang = *static_cast<KaxChapterLanguage *>(ChapterDisplay[Index1]);

					newChapterDisplay.lang = std::string(chapLang);

				} else if (ChapterDisplay[Index1]->Generic().GlobalId == KaxChapterCountry::ClassInfos.GlobalId) {							
					KaxChapterCountry &chapCountry = *static_cast<KaxChapterCountry *>(ChapterDisplay[Index1]);

					newChapterDisplay.country = std::string(chapCountry);
				}
			}
			// A emtpy string in a chapter display string is usless
			if (newChapterDisplay.string.length() > 0)
				newChapter->display.push_back(newChapterDisplay);

		} else if (ChapterAtom[Index0]->Generic().GlobalId == KaxChapterAtom::ClassInfos.GlobalId) {
			MatroskaChapterEntry *entry = NULL;
			chapter_count += Parse_ChapterAtom(*static_cast<KaxChapterAtom *>(ChapterAtom[Index0]), &entry);
			if (entry != NULL) 
				newChapter->children.push_back(entry);
		}

	}

	if (newChapter->chapterUID != 0) {
		*entry = newChapter;
	} else {
		delete newChapter;
		return 0;
	}

	CRASH_PROTECT_END;

	return chapter_count;
};

#endif // USING_CHAPTER_DATA
int MatroskaInfoParser::CopyFile(char *output_filename)
{
#ifdef MATROSKA_INFO_PARSER_COPY_FILE
	TRACE("MatroskaInfoParser::CopyFile()");
	int UpperElementLevel;
	bool updated_display_size = false;
	// Elements for different levels
	EbmlElement *l0 = NULL;
	EbmlElement *l1 = NULL;
	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	EbmlElement *l4 = NULL;
	EbmlElement *l5 = NULL;
	try {
		MatroskaUilsFileReader inout_file(m_filename, MODE_READ);		

		EbmlStream *es = new EbmlStream(inout_file);
		if (es == NULL)
			return 1;

		// Find the EbmlHead element. Must be the first one.
		l0 = es->FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			return 1;
		}
		// Don't verify its data for now.
		l0->SkipData(static_cast<EbmlStream &>(*es), l0->Generic().Context);
		delete l0;
		//We must have found the EBML head :)

		// Next element must be a segment
		l0 = es->FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			//No segment/level 0 element found.
			return 1;
		}
		if (!(EbmlId(*l0) == KaxSegment::ClassInfos.GlobalId)) {
			return 1;
		}

		//Setup the output Matroska file
		MatroskaUilsFileReader out_file(output_filename, MODE_WRITE);		

		EbmlHead FileHead;	  
		
		EDocType & MyDocType = GetChild<EDocType>(FileHead);
		*static_cast<EbmlString *>(&MyDocType) = "matroska";	  
		
		EDocTypeVersion & MyDocTypeVer = GetChild<EDocTypeVersion>(FileHead);
		*(static_cast<EbmlUInteger *>(&MyDocTypeVer)) = 1;	  
		
		EDocTypeReadVersion & MyDocTypeReadVer = GetChild<EDocTypeReadVersion>(FileHead);
		*(static_cast<EbmlUInteger *>(&MyDocTypeReadVer)) = 1;	  
		
		FileHead.Render(out_file);
		// size is unknown and will always be, we can render it right away
		KaxSegment FileSegment;
		uint64 SegmentSize = FileSegment.WriteHead(out_file, 5);


		//Start copying
		UpperElementLevel = 0;
		// We've got our segment, so let's find the tracks
		l1 = es->FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL,
			true, 1);
		while (l1 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}
			if (EbmlId(*l1) == KaxAttachments::ClassInfos.GlobalId) {

			}else if (EbmlId(*l1) == KaxSeekHead::ClassInfos.GlobalId) {
				//Copy this element over
				EbmlMaster *seekHead = static_cast<EbmlMaster *>(l1);

				seekHead->Read(static_cast<EbmlStream &>(*es), KaxSeekHead::ClassInfos.Context, UpperElementLevel, l2, true);
				if (seekHead->CheckMandatory()) {
					printf("  * All mandatory elements found *\n");
				} else {
					printf("  * Some mandatory elements ar missing !!! *\n");
				}
					
				// display the elements read
				unsigned int Index0;
				for (Index0 = 0; Index0 < seekHead->ListSize() ;Index0++) {
					printf(" - found %s\n", (*seekHead)[Index0]->Generic().DebugName);
				}

				l1->Render(out_file, true);
			}
			/*
			if (EbmlId(*l1) == KaxTracks::ClassInfos.GlobalId) {
				// Yep, we've found our KaxTracks element. Now find all tracks
				// contained in this segment. 				

				l2 = es->FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				while (l2 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}

					if (EbmlId(*l2) == KaxTrackEntry::ClassInfos.GlobalId) {
						// We actually found a track entry :) We're happy now.

						l3 = es->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						while (l3 != NULL) {
							if (UpperElementLevel > 0) {
								break;
							}
							if (UpperElementLevel < 0) {
								UpperElementLevel = 0;
							}

							//Now evaluate the data belonging to this track              
							if (EbmlId(*l3) == KaxTrackVideo::ClassInfos.GlobalId) {
								MatroskaVideoTrackInfo *videoTrack_info = new MatroskaVideoTrackInfo;

								l4 = es->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
								while (l4 != NULL) {
									if (UpperElementLevel > 0) {
										break;
									}
									if (UpperElementLevel < 0) {
										UpperElementLevel = 0;
									}

									if (EbmlId(*l4) == KaxVideoDisplayWidth::ClassInfos.GlobalId) {
										KaxVideoDisplayWidth &Displaywidth = *static_cast<KaxVideoDisplayWidth *>(l4);

										//Now update the copy in memory with the new value
										*static_cast<EbmlUInteger *>(&Displaywidth) = new_display_size_width;
										//Ok good, now store the current postion in the file
										uint64 currentFilePos = inout_file.getFilePointer();
										//Seek to the beginning of this element
										inout_file.setFilePointer(Displaywidth.GetElementPosition(), seek_beginning);
										//Now write it or Render
										Displaywidth.Render(inout_file);
										//Restore the previous location in the file
										inout_file.setFilePointer(currentFilePos, seek_beginning);										
										updated_display_size = true;

									} else if (EbmlId(*l4) == KaxVideoDisplayHeight::ClassInfos.GlobalId) {
										KaxVideoDisplayHeight &Displayheight = *static_cast<KaxVideoDisplayHeight *>(l4);

										//Now update the copy in memory with the new value
										*static_cast<EbmlUInteger *>(&Displayheight) = new_display_size_height;
										//Ok good, now store the current postion in the file
										uint64 currentFilePos = inout_file.getFilePointer();
										//Seek to the beginning of this element
										inout_file.setFilePointer(Displayheight.GetElementPosition(), seek_beginning);
										//Now write it or Render
										Displayheight.Render(inout_file);
										//Restore the previous location in the file
										inout_file.setFilePointer(currentFilePos, seek_beginning);

										updated_display_size = true;
									}

									if (UpperElementLevel > 0) {
										assert(1 == 0);	// this should never happen
									} else {
										l4->SkipData(static_cast<EbmlStream &>(*es), l4->Generic().Context);
										delete l4;
										l4 = es->FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
									}
								} // while (l4 != NULL)								
							}

							if (UpperElementLevel > 0) {	// we're coming from l4
								UpperElementLevel--;
								delete l3;
								l3 = l4;
								if (UpperElementLevel > 0)
									break;
							} else {
								l3->SkipData(static_cast<EbmlStream &>(*es), l3->Generic().Context);
								delete l3;
								l3 = es->FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							}
						} // while (l3 != NULL)
					}

					if (UpperElementLevel > 0) {	// we're coming from l3
						UpperElementLevel--;
						delete l2;
						l2 = l3;
						if (UpperElementLevel > 0)
							break;
					} else {
						l2->SkipData(static_cast<EbmlStream &>(*es), l2->Generic().Context);
						delete l2;
						l2 = es->FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					}
				} // while (l2 != NULL)
			}

			if (updated_display_size)
				return 0;
			*/
			if (UpperElementLevel > 0) {		// we're coming from l2
				UpperElementLevel--;
				delete l1;
				l1 = l2;
				if (UpperElementLevel > 0)
					break;
			} else {
				l1->SkipData(static_cast<EbmlStream &>(*es), l1->Generic().Context);
				delete l1;
				l1 = es->FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		} // while (l1 != NULL)
	} catch (std::exception &ex) {  
		const char *error = ex.what();
		return 2;
	}
#endif
	return 3;	
};
#ifdef USING_WRITES
int MatroskaInfoParser::WriteDisplaySize(uint8 track_no, uint16 new_display_size_width, uint16 new_display_size_height) {
	TRACE("MatroskaInfoParser::WriteDisplaySize()");	
	bool updated_display_size = false;
	int UpperElementLevel = 0;
	// Elements for different levels
	EbmlElement *l0 = NULL;
	EbmlElement *l1 = NULL;
	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	EbmlElement *l4 = NULL;
	//EbmlElement *l5 = NULL;
	try {
		MatroskaUilsFileReader inout_file(m_filename, MODE_SAFE);
		if (!inout_file.IsOk())
			throw std::runtime_error(_("Unable open file. Please check that the file isn't open in another program."));

		EbmlStream es(inout_file);

		// Find the EbmlHead element. Must be the first one.
		l0 = es.FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			throw std::runtime_error(_("No EBML header was found."));
		}
		
		if (!(EbmlId(*l0) == EbmlHead::ClassInfos.GlobalId)) {
			throw std::runtime_error(_("No EBML header was found."));
		}
		l0->SkipData(es, l0->Generic().Context);
		delete l0;
		//We must have found the EBML head :)

		// Next element must be a segment
		l0 = es.FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			//No segment/level 0 element found.
			throw std::runtime_error(_("No Segment/Level 0 Element was found."));
		}
		if (!(EbmlId(*l0) == KaxSegment::ClassInfos.GlobalId)) {
			throw std::runtime_error(_("First Level 0 Element was not a Segment."));
		}

		UpperElementLevel = 0;
		// We've got our segment, so let's find the tracks
		l1 = es.FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL,
			true, 1);
		while (l1 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}

			if (EbmlId(*l1) == KaxTracks::ClassInfos.GlobalId) {
				// Yep, we've found our KaxTracks element. Now find all tracks
				// contained in this segment. 				

				l2 = es.FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				while (l2 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}

					if (EbmlId(*l2) == KaxTrackEntry::ClassInfos.GlobalId) {
						// We actually found a track entry :) We're happy now.

						l3 = es.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						while (l3 != NULL) {
							if (UpperElementLevel > 0) {
								break;
							}
							if (UpperElementLevel < 0) {
								UpperElementLevel = 0;
							}

							//Now evaluate the data belonging to this track       
							if (EbmlId(*l3) == KaxTrackNumber::ClassInfos.GlobalId) {
								KaxTrackNumber &tNumber = *static_cast<KaxTrackNumber *>(l3);
								tNumber.ReadData(es.I_O());
								if (uint16(tNumber) != track_no)
									//Not our track
									break;

							}else	if (EbmlId(*l3) == KaxTrackVideo::ClassInfos.GlobalId) {
								//MatroskaVideoTrackInfo *videoTrack_info = new MatroskaVideoTrackInfo;

								l4 = es.FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
								while (l4 != NULL) {
									if (UpperElementLevel > 0) {
										break;
									}
									if (UpperElementLevel < 0) {
										UpperElementLevel = 0;
									}

									if (EbmlId(*l4) == KaxVideoDisplayWidth::ClassInfos.GlobalId) {
										KaxVideoDisplayWidth &Displaywidth = *static_cast<KaxVideoDisplayWidth *>(l4);

										//Now update the copy in memory with the new value
										*static_cast<EbmlUInteger *>(&Displaywidth) = new_display_size_width;
										//Ok good, now store the current postion in the file
										uint64 currentFilePos = inout_file.getFilePointer();
										//Seek to the beginning of this element
										inout_file.setFilePointer(Displaywidth.GetElementPosition(), seek_beginning);
										//Now write it or Render
										Displaywidth.Render(inout_file);
										//Restore the previous location in the file
										inout_file.setFilePointer(currentFilePos, seek_beginning);										
										updated_display_size = true;

									} else if (EbmlId(*l4) == KaxVideoDisplayHeight::ClassInfos.GlobalId) {
										KaxVideoDisplayHeight &Displayheight = *static_cast<KaxVideoDisplayHeight *>(l4);

										//Now update the copy in memory with the new value
										*static_cast<EbmlUInteger *>(&Displayheight) = new_display_size_height;
										//Ok good, now store the current postion in the file
										uint64 currentFilePos = inout_file.getFilePointer();
										//Seek to the beginning of this element
										inout_file.setFilePointer(Displayheight.GetElementPosition(), seek_beginning);
										//Now write it or Render
										Displayheight.Render(inout_file);
										//Restore the previous location in the file
										inout_file.setFilePointer(currentFilePos, seek_beginning);

										updated_display_size = true;
									}

									if (UpperElementLevel > 0) {
										assert(1 == 0);	// this should never happen
									} else {
										l4->SkipData(es, l4->Generic().Context);
										delete l4;
										l4 = es.FindNextElement(l3->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
									}
								} // while (l4 != NULL)								
							}

							if (UpperElementLevel > 0) {	// we're coming from l4
								UpperElementLevel--;
								delete l3;
								l3 = l4;
								if (UpperElementLevel > 0)
									break;
							} else {
								l3->SkipData(es, l3->Generic().Context);
								delete l3;
								l3 = es.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							}
						} // while (l3 != NULL)
					}

					if (UpperElementLevel > 0) {	// we're coming from l3
						UpperElementLevel--;
						delete l2;
						l2 = l3;
						if (UpperElementLevel > 0)
							break;
					} else {
						l2->SkipData(es, l2->Generic().Context);
						delete l2;
						l2 = es.FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					}
				} // while (l2 != NULL)
			}

			if (updated_display_size) {
				delete l1;
				delete l0;
				return 0;
			}

			if (UpperElementLevel > 0) {		// we're coming from l2
				UpperElementLevel--;
				delete l1;
				l1 = l2;
				if (UpperElementLevel > 0)
					break;
			} else {
				l1->SkipData(es, l1->Generic().Context);
				delete l1;
				l1 = es.FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		} // while (l1 != NULL)
		
		throw std::runtime_error(_("No Track Display Size found to update"));

	} catch (std::exception &ex) {
		std::string errorMsg;
		errorMsg = _("Failed to update Track Display Size.");
		errorMsg += "\n";
		errorMsg += _("Exception: ");
		errorMsg += ex.what();	
		
		throw std::runtime_error(errorMsg);		
	}
};

int MatroskaInfoParser::WriteLanguage(uint8 track_no, std::string new_lang) {
	TRACE("MatroskaInfoParser::WriteLanguage()");
	bool updated_lang = false;
	int UpperElementLevel = 0;
	// Elements for different levels
	EbmlElement *l0 = NULL;
	EbmlElement *l1 = NULL;
	EbmlElement *l2 = NULL;
	EbmlElement *l3 = NULL;
	EbmlElement *l4 = NULL;
	try {
		MatroskaUilsFileReader inout_file(m_filename, MODE_SAFE);
		if (!inout_file.IsOk())
			throw std::runtime_error(_("Unable open file. Please check that the file isn't open in another program."));

		EbmlStream es(inout_file);
		
		// Find the EbmlHead element. Must be the first one.
		l0 = es.FindNextID(EbmlHead::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			throw std::runtime_error(_("No EBML header was found."));
		}

		// Check if it's really an ebml header
		if (!(EbmlId(*l0) == EbmlHead::ClassInfos.GlobalId)) {
			throw std::runtime_error(_("No EBML header was found."));
		}
		l0->SkipData(es, l0->Generic().Context);
		delete l0;
		

		// Next element must be a segment
		l0 = es.FindNextID(KaxSegment::ClassInfos, 0xFFFFFFFFFFFFFFFFL);
		if (l0 == NULL) {
			throw std::runtime_error(_("No Segment/Level 0 Element was found."));
		}
		if (!(EbmlId(*l0) == KaxSegment::ClassInfos.GlobalId)) {
			throw std::runtime_error(_("First Level 0 Element was not a Segment."));
		}

		UpperElementLevel = 0;
		// We've got our segment, so let's find the tracks
		l1 = es.FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
		while (l1 != NULL) {
			if (UpperElementLevel > 0) {
				break;
			}
			if (UpperElementLevel < 0) {
				UpperElementLevel = 0;
			}

			if (EbmlId(*l1) == KaxTracks::ClassInfos.GlobalId) {
				// Yep, we've found our KaxTracks element. Now find all tracks
				// contained in this segment. 				

				l2 = es.FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
				while (l2 != NULL) {
					if (UpperElementLevel > 0) {
						break;
					}
					if (UpperElementLevel < 0) {
						UpperElementLevel = 0;
					}

					if (EbmlId(*l2) == KaxTrackEntry::ClassInfos.GlobalId) {
						// We actually found a track entry :) We're happy now.

						l3 = es.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
						while (l3 != NULL) {
							if (UpperElementLevel > 0) {
								break;
							}
							if (UpperElementLevel < 0) {
								UpperElementLevel = 0;
							}

							//Now evaluate the data belonging to this track       
							if (EbmlId(*l3) == KaxTrackNumber::ClassInfos.GlobalId) {
								KaxTrackNumber &tNumber = *static_cast<KaxTrackNumber *>(l3);
								tNumber.ReadData(es.I_O());
								if (uint16(tNumber) != track_no) {
									//Not our track
									delete l3;
									l3 = NULL;
									break;
								}

							}else	if (EbmlId(*l3) == KaxTrackLanguage::ClassInfos.GlobalId) {
								KaxTrackLanguage &tLang = *static_cast<KaxTrackLanguage *>(l3);
								uint64 origSize = tLang.GetSize();								
								//Now update the copy in memory with the new value
								*static_cast<EbmlString *>(&tLang) = new_lang;
								if (tLang.GetSize() > origSize) {
									//Too large
									delete l3;
									delete l2;
									delete l1;
									delete l0;

									throw std::runtime_error(_("New Track Language is longer than the previous."));
								}
								
								//Ok good, now store the current postion in the file
								uint64 currentFilePos = inout_file.getFilePointer();
								//Seek to the beginning of this element
								inout_file.setFilePointer(tLang.GetElementPosition(), seek_beginning);
								//Now write it or Render
								tLang.Render(inout_file);
								//Restore the previous location in the file
								inout_file.setFilePointer(currentFilePos, seek_beginning);										
								updated_lang = true;
							}

							if (UpperElementLevel > 0) {	// we're coming from l4
								UpperElementLevel--;
								delete l3;
								l3 = l4;
								if (UpperElementLevel > 0)
									break;
							} else {
								l3->SkipData(es, l3->Generic().Context);
								delete l3;
								l3 = es.FindNextElement(l2->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
							}
						} // while (l3 != NULL)
					}

					if (UpperElementLevel > 0) {	// we're coming from l3
						UpperElementLevel--;
						delete l2;
						l2 = l3;
						if (UpperElementLevel > 0)
							break;
					} else {
						l2->SkipData(es, l2->Generic().Context);
						delete l2;
						l2 = es.FindNextElement(l1->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
					}
				} // while (l2 != NULL)
			}

			if (updated_lang) {
				delete l1;
				delete l0;
				return 0;
			}

			if (UpperElementLevel > 0) {		// we're coming from l2
				UpperElementLevel--;
				delete l1;
				l1 = l2;
				if (UpperElementLevel > 0)
					break;
			} else {
				l1->SkipData(es, l1->Generic().Context);
				delete l1;
				l1 = es.FindNextElement(l0->Generic().Context, UpperElementLevel, 0xFFFFFFFFFFFFFFFFL, true, 1);
			}
		} // while (l1 != NULL)
		delete l0;

		throw std::runtime_error(_("No Track Language found to update"));

	} catch (std::exception &ex) {
		std::string errorMsg;
		errorMsg = _("Failed to update Track Language.");
		errorMsg += "\n";
		errorMsg += _("Exception: ");
		errorMsg += ex.what();	
		
		throw std::runtime_error(errorMsg);		
	}
};

#endif // USING_WRITES
//returns the number of tracks found in the file.
int MatroskaInfoParser::GetNumberOfTracks(){
	TRACE("MatroskaInfoParser::GetNumberOfTracks()");
	return trackInfos.GetListCount();
}

//Lookup a track by it's UID and Returns a pointer to the track's information
MatroskaTrackInfo *MatroskaInfoParser::GetTrackInfoByUID(uint64 trackUID)
{
	TRACE("MatroskaInfoParser::GetTrackInfoByUID()");
	uint16 total_item_count = trackInfos.GetListCount();
	uint16 current_item_count = 0;

	for(current_item_count = 0; current_item_count < total_item_count; current_item_count++)
	{
		if (trackInfos[current_item_count]->GetTrackUID() == trackUID)
			return trackInfos[current_item_count];
	}
	return NULL;
};

//Returns a pointer to the specified track's information
MatroskaTrackInfo *MatroskaInfoParser::GetTrackInfo0(int trackNumber){
	return trackInfos[trackNumber];
}

MatroskaTrackInfo *MatroskaInfoParser::GetTrackInfo1(int trackNumber){
	//printf("track num %d of %d\n", trackNumber, trackInfos.size());
	for(int i=0; i<trackInfos.size(); i++)
		if(trackInfos[i]->GetTrackNumber()==trackNumber)
			return trackInfos[i];
	return NULL;
}

MatroskaTrackInfo *MatroskaInfoParser::GetFirstVideoTrack() {
	uint16 totalIndexCount = trackInfos.GetListCount();

	for(uint16 index = 0; index < totalIndexCount; index++)
		if ((trackInfos[index]->GetTrackType() == track_video) && (trackInfos[index]->GetVideoInfo() != NULL))
			return trackInfos[index];
	
	return NULL;
};

MatroskaTrackInfo *MatroskaInfoParser::GetFirstAudioTrack() {
	uint16 totalIndexCount = trackInfos.GetListCount();

	for(uint16 index = 0; index < totalIndexCount; index++)
		if ((trackInfos[index]->GetTrackType() == track_audio) && (trackInfos[index]->GetAudioInfo() != NULL))
			return trackInfos[index];
	
	return NULL;
};

MatroskaTrackInfo *MatroskaInfoParser::GetFirstSubtitleTrack() {
	uint16 totalIndexCount = trackInfos.GetListCount();

	for(uint16 index = 0; index < totalIndexCount; index++)
		if (trackInfos[index]->GetTrackType() == track_subtitle)
			return trackInfos[index];
	
	return NULL;
};

/*MatroskaTrackInfo *MatroskaInfoParser::GetGlobalTrackInfo()
{
	TRACE("MatroskaInfoParser::GetGlobalTrackInfo()");
	MatroskaTrackInfo* matroskaGlobalTrack = this->GetTrackInfoByUID(0);
	if (matroskaGlobalTrack == NULL) {
		matroskaGlobalTrack = new MatroskaTrackInfo(0);
		//Go ahead and pre-create the Tags, this is supposed to be the only reason you use a Global Track Info
		matroskaGlobalTrack->SetTagInfo(new MatroskaTagInfo());
		//Add it to the list
		trackInfos.AddItem(matroskaGlobalTrack);
	}
	return matroskaGlobalTrack;
}*/
/*
const char *MatroskaInfoParser::GetNiceFileSize()
{
	TRACE("MatroskaInfoParser::GetNiceFileSize()");
	if (m_niceFileSize.length() != 0) {
		return m_niceFileSize.GetUTF8().c_str();
	} else {
#ifdef WIN32
		wchar_t buffer[256];
		StrFormatByteSizeW(file_size, buffer, 255);
		m_niceFileSize = buffer;
#else
		if (file_size < 1024) {
			m_niceFileSize = JString::Format("%u bytes", file_size);

		}	else if (file_size < 1024 * 1024) {
			m_niceFileSize = JString::Format("%.2fKB", (float)(int64)file_size / 1024).c_str();

		}	else if (file_size < 1024 * 1024 * 1024) {
			m_niceFileSize = JString::Format("%.2fMB", (float)(int64)file_size / 1024 / 1024).c_str();

		}	else {
			m_niceFileSize = JString::Format("%.2fGB", (float)(int64)file_size / 1024 / 1024 / 1024).c_str();
		}
#endif
		return m_niceFileSize.GetUTF8().c_str();
	};
};

std::string &MatroskaInfoParser::GetTextReport()
{
	ODS("MatroskaInfoParser::GetTextReport()");
	m_fileReport = "";

	for (int i = 0; i < GetNumberOfTracks(); i++)
	{
		if (i > 0)
			//After the first track we insert line breaks before each track line
			m_fileReport += "\n";

		MatroskaTrackInfo *current_track = GetTrackInfo(i);			
		
		if (current_track != NULL)
		{
			char tag_txt[128];
			_snprintf(tag_txt, 128, "Track %u: %s %s", current_track->GetTrackNumber(), current_track->GetTrackTypeStr(), current_track->GetCodecID());
			m_fileReport += tag_txt;
			if (current_track->m_CodecOldID.length() > 1)
			{
				m_fileReport += " ";
				m_fileReport += current_track->m_CodecOldID.c_str();
			}
			if ((current_track->GetLanguageName() != NULL) && (strlen(current_track->GetLanguageName()) > 1))
			{
				m_fileReport += " Language: ";
				m_fileReport += current_track->GetLanguageName();
			}
			m_fileReport += ",";
		}
	}
	if (file_size != 0)
	{
		char *size_txt = new char[128];
		m_fileReport += "\nFilesize: ";
		m_fileReport += GetNiceFileSize();			
		if (m_segmentInfo.GetDuration() != 0)
		{				
			_snprintf(size_txt, 128, " Avg Bitrate: %.2fKb/s", (double)(int64)file_size / 1024 / m_segmentInfo.GetDuration() * 8);
			m_fileReport += size_txt;				
		}
		m_fileReport += " Length: ";
		m_fileReport += m_segmentInfo.GetNiceDuration();
		delete size_txt;
	}

	return m_fileReport;
};*/

const char *MatroskaInfoParser::GetMuxingApp() {
	return m_segmentInfo.m_infoMuxingApp.GetUTF8().c_str();
};

const char *MatroskaInfoParser::GetWritingApp() {
	return m_segmentInfo.m_infoWritingApp.GetUTF8().c_str();
};

const wchar_t *MatroskaInfoParser::GetFilename() {
	return m_filename.c_str();
}

const char *MatroskaInfoParser::GetFilename_ASCII() {
	return m_filename.GetUTF8().c_str();
}

#ifdef USING_TINYXML
int MatroskaInfoParser::SaveChaptersToXMLFile(JString &xmlFilename)
{
	TiXmlDocument chapterXml;

	TiXmlDeclaration baseLevel("1.0", "UTF-8", "yes");
	chapterXml.InsertEndChild(baseLevel);

	TiXmlElement rootLevel("Chapters");

	TiXmlElement editionLevel("EditionEntry");

	chapters.ListAsXML(editionLevel);

	rootLevel.InsertEndChild(editionLevel);

	chapterXml.InsertEndChild(rootLevel);
	chapterXml.SaveFile(xmlFilename.mb_str());

	return 0;
}
#endif // USING_TINYXML

wchar_t *MatroskaInfoParser::ConvertCharToWchar_t(const char *ansi_filename)
{
	UTFstring temp;
	temp.SetUTF8(ansi_filename);
	
	wchar_t *unicode_filename = new wchar_t[temp.length() + 3];	
	wcscpy(unicode_filename, temp.c_str());

	return unicode_filename;
};

uint8 MatroskaInfoParser::GetPercentComplete() {
#ifdef USING_JCT
	JCAutoLockCritSec lockCurrentPercent(&m_CurrentPercent_cs);
#endif
	return m_CurrentPercent;
};

void MatroskaInfoParser::UpdatePercentComplete(uint8 newPercent) {
#ifdef USING_JCT
	JCAutoLockCritSec lockCurrentPercent(&m_CurrentPercent_cs);
#endif
	m_CurrentPercent = newPercent;
};

MatroskaSegmentInfo *MatroskaInfoParser::GetSegmentInfo()
{
  return &m_segmentInfo;
}

#ifdef _DEBUG
int YourAllocHook(int allocType, void *userData, size_t size, int blockType, long requestNumber, const unsigned char *filename, int lineNumber)
{
	static int64 totalSize;
	if (allocType == 1)
		totalSize += size;
	else if (allocType == 3)
		totalSize -= _msize(userData);
	else if (allocType == 500)
		totalSize = 0;
	else if (allocType == 1000)
		*(int64 *)userData = totalSize;
	//I can use this to find exactly where a leak started
	if (size == 168 && requestNumber == 841)
	{
		printf("cool");
	}
	return TRUE;
};
#endif

//};// End namespace MatroskaUtilsNamespace



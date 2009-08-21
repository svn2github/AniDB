// File__Tags - Info for all kind of framed tags tagged files
// Copyright (C) 2007-2007 Jerome Martinez, Zen@MediaArea.net
//
// This library is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Information about all kind of framed tags tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File__TagsH
#define MediaInfo_File__TagsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#if defined(MEDIAINFO_APETAG_YES)
    #include "MediaInfo/Tag/File_ApeTag.h"
#else //MEDIAINFO_APETAG_YES
    #define File_ApeTag File__Base
#endif //MEDIAINFO_APETAG_YES
#if defined(MEDIAINFO_ID3_YES)
    #include "MediaInfo/Tag/File_Id3.h"
#else //MEDIAINFO_ID3_YES
    #define File_Id3v2 File__Base
#endif //MEDIAINFO_ID3_YES
#if defined(MEDIAINFO_ID3V2_YES)
    #include "MediaInfo/Tag/File_Id3v2.h"
#else //MEDIAINFO_ID3V2_YES
    #define File_Id3v2 File__Base
#endif //MEDIAINFO_ID3V2_YES
#if defined(MEDIAINFO_LYRICS3_YES)
    #include "MediaInfo/Tag/File_Lyrics3.h"
#else //MEDIAINFO_LYRICS3_YES
    #define File_Lyrics3 File__Base
#endif //MEDIAINFO_LYRICS3V2_YES
#if defined(MEDIAINFO_LYRICS3V2_YES)
    #include "MediaInfo/Tag/File_Lyrics3v2.h"
#else //MEDIAINFO_LYRICS3V2_YES
    #define File_Lyrics3v2 File__Base
#endif //MEDIAINFO_LYRICS3V2_YES
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Tags_Helper
//***************************************************************************

#if !defined(MEDIAINFO_TAG_NO)
class File__Tags_Helper : public File_Id3_Helper, public File_Id3v2_Helper, public File_Lyrics3_Helper, public File_Lyrics3v2_Helper, public File_ApeTag_Helper
{
public :
    File__Tags_Helper(File__Base* Base_);
    ~File__Tags_Helper();

protected :
    //From elsewhere
    bool DetectBeginOfEndTags();
    bool DetectBeginOfEndTags_Test();
    bool Tags_Read_Buffer_Continue ();
    void Tags_Read_Buffer_Finalize ();

    //Data
    File__Base* Base;
    int64u File_BeginTagSize; //Id3v2
    int64u File_EndTagSize; //Others
    int32u Id3v1_Size;
    int32u Lyrics3_Size;
    int32u Lyrics3v2_Size;
    int32u ApeTag_Size;
    bool TagSizeIsFinal;
};

#else //!defined(MEDIAINFO_TAG_NO)
class File__Tags_Helper
{
public :
    File__Tags_Helper(File__Base* Base_);
    ~File__Tags_Helper() {};

protected :
    //From elsewhere
    bool DetectBeginOfEndTags() {return false;};
    bool DetectBeginOfEndTags_Test() {return false;};
    bool Tags_Read_Buffer_Continue () {return true;};
    void Tags_Read_Buffer_Finalize () {};

    //Data
    int64u File_BeginTagSize; //Id3v2
    int64u File_EndTagSize; //Others
    int32u Id3v1_Size;
    int32u Lyrics3_Size;
    int32u Lyrics3v2_Size;
    int32u ApeTag_Size;
    bool TagSizeIsFinal;
};
#endif //!defined(MEDIAINFO_TAG_NO)

} //NameSpace

#endif

// File_Id3 - Info for ID3 tagged files
// Copyright (C) 2005-2007 Jerome Martinez, Zen@MediaArea.net
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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_ID3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Id3.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
void Mpega_ID3v1_KeepOutSpaces(Ztring &Tag)
{
    //Removing trailing spaces
    while (!Tag.empty() && Tag[Tag.size()-1]==_T(' '))
        Tag.resize(Tag.size()-1);
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Id3::Read_Buffer_Init()
{
    //Temp - ID3v1
    Id3v1_Title.clear();
    Id3v1_Artist.clear();
    Id3v1_Album.clear();
    Id3v1_Year.clear();
    Id3v1_Comment.clear();
    Id3v1_Track.clear();
    Id3v1_Genre.clear();
}

//---------------------------------------------------------------------------
void File_Id3::Read_Buffer_Continue()
{
    if (Buffer_Size<3)
        return;

    if (CC3(Buffer+Buffer_Offset)!=CC3("TAG"))
        FINNISHED();

    if (Buffer_Size<128)
        return;

    ELEMENT(0, "Tag", 128);
    Stream_Prepare(Stream_General);
    Stream_Prepare(Stream_Audio);

    //Parsing
    Element_Size=128;
    int32u Track=0, Genre;
    CC_BEGIN();
    SKIP_LOCAL(3,                                           ID);
    GET_LOCAL(30, Id3v1_Title,                              Title);
    GET_LOCAL(30, Id3v1_Artist,                             Artist);
    GET_LOCAL(30, Id3v1_Album,                              Album);
    GET_LOCAL( 4, Id3v1_Year,                               Year);
    GET_LOCAL(30, Id3v1_Comment,                            Comment);
    if (Id3v1_Comment.size()<29) //Id3v1.1 specifications : Track number addition
    {
        Stream_Pos-=2;
        int32u Zero;
        PEEK_B1(Zero);
        if (Zero==0)
        {
            SKIP_B1(                                        Zero);
            GET_B1 (Track,                                  Track);
        }
    }
    GET_B1 (Genre,                                          Genre);
    CC_END();
    FLUSH();

    //Filling
    Mpega_ID3v1_KeepOutSpaces(Id3v1_Title);
    Mpega_ID3v1_KeepOutSpaces(Id3v1_Artist);
    Mpega_ID3v1_KeepOutSpaces(Id3v1_Album);
    Mpega_ID3v1_KeepOutSpaces(Id3v1_Year);
    Mpega_ID3v1_KeepOutSpaces(Id3v1_Comment);
    if (Track!=0)
        Id3v1_Track.From_Number(Track);
    else
        Id3v1_Track.clear();
    if (Genre!=0 && Genre!=0xFF)
        Id3v1_Genre.From_Number(Genre);
    else
        Id3v1_Genre.clear();
    if (!Id3v1_Album.empty())
        Fill(Stream_General, 0, "Album", Id3v1_Album);
    if (!Id3v1_Title.empty())
        Fill(Stream_General, 0, "Track", Id3v1_Title);
    if (!Id3v1_Artist.empty())
        Fill(Stream_General, 0, "Performer", Id3v1_Artist);
    if (!Id3v1_Comment.empty())
        Fill(Stream_General, 0, "Comment", Id3v1_Comment);
    if (!Id3v1_Year.empty())
        Fill(Stream_General, 0, "Recorded_Date", Id3v1_Year);
    if (!Id3v1_Genre.empty())
        Fill(Stream_General, 0, "Genre", Id3v1_Genre);
    if (!Id3v1_Track.empty())
        Fill(Stream_General, 0, "Track/Position", Id3v1_Track);

    Buffer_Offset+=128;
}

//***************************************************************************
// External Helper
//***************************************************************************

File_Id3_Helper::File_Id3_Helper(File__Base* Base_)
{
    #if defined(MEDIAINFO_ID3_YES)
        Base=Base_;
        ID3=NULL;
    #endif //ID3
}

File_Id3_Helper::~File_Id3_Helper()
{
    #if defined(MEDIAINFO_ID3_YES)
        delete ID3; //ID3=NULL;
    #endif //ID3
}

bool File_Id3_Helper::Id3_Read_Buffer_Continue()
{
    if (Base->Buffer_Offset+3>Base->Buffer_Size)
        return false;

    //First bytes
    if (CC3(Base->Buffer+Base->Buffer_Offset)==CC3("TAG"))
    {
        //ID3v2, must skip it here
        if (Base->Buffer_Offset+128>Base->Buffer_Size)
            return false;
    }
    else
        return true; //Nothing

    //Details
    Base->ELEMENT(0, "ID3v1 tags", 128);
    Base->FLUSH();

    //If we continue the Id3 tag
    #if defined(MEDIAINFO_ID3_YES)
        if (ID3==NULL)
        {
            ID3=new File_Id3;
            Base->Open_Buffer_Init(ID3);
        }

        Base->Open_Buffer_Continue(ID3, Base->Buffer+Base->Buffer_Offset, 128);
        Base->Buffer_Offset+=128;
        Base->Open_Buffer_Finalize(ID3);
    #else
        Base->Buffer_Offset+=128;
    #endif //ID3V2

    return true;
}

void File_Id3_Helper::Id3_Read_Buffer_Finalize()
{
    #if defined(MEDIAINFO_ID3_YES)
    if (ID3!=NULL)
    {
        if (ID3->Count_Get(Stream_General)>0)
            Base->Merge(*ID3, Stream_General, 0, 0);
        delete ID3; ID3=NULL;
    }
    #endif //ID3
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Id3::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("Album", "R");
            Fill_HowTo("Part/Position", "R");
            Fill_HowTo("Part/Position_Total", "R");
            Fill_HowTo("Track", "R");
            Fill_HowTo("Track/Position", "R");
            Fill_HowTo("Track/Position_Total", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES


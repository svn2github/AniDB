// File_Lyrics3 - Info for Lyrics3 tagged files
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
#if defined(MEDIAINFO_LYRICS3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Lyrics3.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::Read_Buffer_Init()
{
}

//---------------------------------------------------------------------------
void File_Lyrics3::Read_Buffer_Continue()
{
    if (Buffer_Size<20)
        FINNISHED();

    Stream_Prepare(Stream_General);

    Header();
    Lyrics();
    Footer();

    FINNISHED();
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::Header()
{
    ELEMENT(0, "Header", 11);

    //Parsing
    Element_Size=11;
    CC_BEGIN();
    SKIP_LOCAL(11,                                              Signature);
    CC_END();
    FLUSH();

    //Positionning
    Buffer_Offset+=11;
}

//---------------------------------------------------------------------------
void File_Lyrics3::Footer()
{
    ELEMENT(0, "Footer", 9);

    //Parsing
    Element_Size=9;
    CC_BEGIN();
    SKIP_LOCAL(9,                                               Signature);
    CC_END();
    FLUSH();

    //Positionning
    Buffer_Offset+=9;
}

//---------------------------------------------------------------------------
void File_Lyrics3::Lyrics()
{
    Element_Size=Buffer_Size-20;
    ELEMENT(0, "Lyrics", Element_Size);

    //Parsing
    //CC_BEGIN();
    //SKIP_LOCAL(Element_Size,                                    Value);
    //CC_END();
    FLUSH();

    //Filling
    Stream_Prepare(Stream_Text);
    Fill("Name", "Lyrics");
    Fill("Codec", "Lyrics3");

    //Positionning
    Buffer_Offset+=Element_Size;
}

//***************************************************************************
// External Helper
//***************************************************************************

File_Lyrics3_Helper::File_Lyrics3_Helper(File__Base* Base_)
{
    #if defined(MEDIAINFO_LYRICS3_YES)
        Base=Base_;
        Lyrics3=NULL;
        Lyrics3_Size=0;
    #endif //LYRICS3
}

File_Lyrics3_Helper::~File_Lyrics3_Helper()
{
    #if defined(MEDIAINFO_LYRICS3_YES)
        delete Lyrics3; //Lyrics3=NULL;
    #endif //LYRICS3
}

bool File_Lyrics3_Helper::Lyrics3_Read_Buffer_Continue()
{
    if (Base->Buffer_Offset+11>Base->Buffer_Size)
        return false;

    //First bytes
    if (CC8(Base->Buffer+Base->Buffer_Offset)!=CC8("LYRICSBE") || CC3(Base->Buffer+Base->Buffer_Offset+8)!=CC3("GIN"))
        return true; //Nothing

    //Element size
    std::string Buf((const char*)(Base->Buffer+Base->Buffer_Offset), Base->Buffer_Size-Base->Buffer_Offset);
    size_t Pos=Buf.find("LYRICSEND");
    if (Pos==std::string::npos)
        return false;
    Lyrics3_Size=(int32u)Pos+9;

    //Details
    Base->ELEMENT(0, "Lyrics3 tags", Lyrics3_Size);
    Base->FLUSH();

    //If we continue the Lyrics3 tag
    #if defined(MEDIAINFO_LYRICS3_YES)
        if (Lyrics3==NULL)
        {
            Lyrics3=new File_Lyrics3;
            Base->Open_Buffer_Init(Lyrics3);
        }

        Base->Open_Buffer_Continue(Lyrics3, Base->Buffer+Base->Buffer_Offset, Lyrics3_Size);
        Base->Buffer_Offset+=Lyrics3_Size;
        Base->Open_Buffer_Finalize(Lyrics3);
    #else
        Base->Buffer_Offset+=Lyrics3_Size;
    #endif //LYRICS3V2

    return true;
}

void File_Lyrics3_Helper::Lyrics3_Read_Buffer_Finalize()
{
    #if defined(MEDIAINFO_LYRICS3_YES)
    if (Lyrics3!=NULL)
    {
        if (Lyrics3->Count_Get(Stream_Text)>0)
            Base->Merge(*Lyrics3, Stream_Text, 0, 0);
        delete Lyrics3; Lyrics3=NULL;
    }
    #endif //LYRICS3
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3::HowTo(stream_t StreamKind)
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


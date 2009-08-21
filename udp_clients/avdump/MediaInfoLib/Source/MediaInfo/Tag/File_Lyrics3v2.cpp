// File_Lyrics3v2 - Info for Lyrics3v2 tagged files
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
#if defined(MEDIAINFO_LYRICS3V2_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_Lyrics3v2.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::Read_Buffer_Init()
{
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::Read_Buffer_Continue()
{
    if (Buffer_Size<26)
        FINNISHED();

    Stream_Prepare(Stream_General);

    Header();
    while(Buffer_Parse());
    Footer();

    FINNISHED();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Lyrics3v2::Buffer_Parse()
{
    if (Buffer_Offset+8>Buffer_Size-9)
        return false; //Not enough buffer

    //Element name
    Element_Name=CC3(Buffer+Buffer_Offset);

    //Element size
    Element_Size=Ztring((const char*)(Buffer+Buffer_Offset+3), 5).To_int32u();
    if (Buffer_Offset+8+Element_Size>Buffer_Size)
        return false; //Not enough bytes
    Element_Next=File_Offset+Buffer_Offset+8+Element_Size;

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);

    return true;
}

//---------------------------------------------------------------------------
bool File_Lyrics3v2::Element_Parse()
{
    ELEMENT(0, Element_Name<<8, Element_Size);

    //Positionning
    Buffer_Offset+=8;

    #define ELEMENT_CASE(_NAME) \
        case Lyrics3v2::_NAME : _NAME(); break;

    //Parsing
    switch (Element_Name)
    {
        ELEMENT_CASE(AUT);
        ELEMENT_CASE(CRC);
        ELEMENT_CASE(EAL);
        ELEMENT_CASE(EAR);
        ELEMENT_CASE(ETT);
        ELEMENT_CASE(IMG);
        ELEMENT_CASE(IND);
        ELEMENT_CASE(INF);
        ELEMENT_CASE(LYR);
        default : ;
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::Header()
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
void File_Lyrics3v2::Footer()
{
    ELEMENT(0, "Footer", 15);

    //Parsing
    Element_Size=15;
    CC_BEGIN();
    SKIP_LOCAL(6,                                               Size);
    SKIP_LOCAL(9,                                               Signature);
    CC_END();
    FLUSH();

    //Positionning
    Buffer_Offset+=9;
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::AUT()
{
    NAME("Lyrics Author Name");

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL(Element_Size,                                    Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::CRC()
{
    NAME("CRC");

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL(Element_Size,                                    Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::EAL()
{
    NAME("Extended Album name");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Album", Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::EAR()
{
    NAME("Extended Artist name");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Performer", Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::ETT()
{
    NAME("Extended Track Title");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Title", Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::IMG()
{
    NAME("Image location");

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL(Element_Size,                                    Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::IND()
{
    NAME("Indications field");

    //Parsing
    CC_BEGIN();
    if (Element_Size>=1)
        SKIP_LOCAL(1,                                           lyrics present);
    if (Element_Size>=2)
        SKIP_LOCAL(1,                                           timestamp in lyrics);
    if (Element_Size>=3)
        SKIP_LOCAL(1,                                           inhibits tracks for random selection);
    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::INF()
{
    NAME("Additional information");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Comment", Value);
}

//---------------------------------------------------------------------------
void File_Lyrics3v2::LYR()
{
    NAME("Lyrics");

    //Parsing
    //CC_BEGIN();
    //SKIP_LOCAL(Element_Size,                                    Value);
    //CC_END();

    //Filling
    Stream_Prepare(Stream_Text);
    Fill("Name", "Lyrics");
    Fill("Codec", "Lyrics3v2");
}

//***************************************************************************
// External Helper
//***************************************************************************

File_Lyrics3v2_Helper::File_Lyrics3v2_Helper(File__Base* Base_)
{
    #if defined(MEDIAINFO_LYRICS3V2_YES)
        Base=Base_;
        Lyrics3v2=NULL;
        Lyrics3v2_Size=0;
    #endif //LYRICS3V2
}

File_Lyrics3v2_Helper::~File_Lyrics3v2_Helper()
{
    #if defined(MEDIAINFO_LYRICS3V2_YES)
        delete Lyrics3v2; //Lyrics3v2=NULL;
    #endif //LYRICS3V2
}

bool File_Lyrics3v2_Helper::Lyrics3v2_Read_Buffer_Continue()
{
    if (Base->Buffer_Offset+11>Base->Buffer_Size)
        return false;

    //First bytes
    if (CC8(Base->Buffer+Base->Buffer_Offset)!=CC8("LYRICSBE") || CC3(Base->Buffer+Base->Buffer_Offset+8)!=CC3("GIN"))
        return true; //Nothing

    //Element size
    std::string Buf((const char*)(Base->Buffer+Base->Buffer_Offset), Base->Buffer_Size-Base->Buffer_Offset);
    size_t Pos=Buf.find("LYRICS200");
    if (Pos==std::string::npos)
        return false;
    Lyrics3v2_Size=(int32u)Pos+9;

    //Details
    Base->ELEMENT(0, "Lyrics3v2 tags", Lyrics3v2_Size);
    Base->FLUSH();

    //If we continue the Lyrics3v2 tag
    #if defined(MEDIAINFO_LYRICS3V2_YES)
        if (Lyrics3v2==NULL)
        {
            Lyrics3v2=new File_Lyrics3v2;
            Base->Open_Buffer_Init(Lyrics3v2);
        }

        Base->Open_Buffer_Continue(Lyrics3v2, Base->Buffer+Base->Buffer_Offset, Lyrics3v2_Size);
        Base->Buffer_Offset+=Lyrics3v2_Size;
        Base->Open_Buffer_Finalize(Lyrics3v2);
    #else
        Base->Buffer_Offset+=Lyrics3v2_Size;
    #endif //LYRICS3V2V2

    return true;
}

void File_Lyrics3v2_Helper::Lyrics3v2_Read_Buffer_Finalize()
{
    #if defined(MEDIAINFO_LYRICS3V2_YES)
    if (Lyrics3v2!=NULL)
    {
        if (Lyrics3v2->Count_Get(Stream_General)>0)
            Base->Merge(*Lyrics3v2, Stream_General, 0, 0);
        if (Lyrics3v2->Count_Get(Stream_Text)>0)
            Base->Merge(*Lyrics3v2, Stream_Text, 0, 0);
        delete Lyrics3v2; Lyrics3v2=NULL;
    }
    #endif //LYRICS3V2
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Lyrics3v2::HowTo(stream_t StreamKind)
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


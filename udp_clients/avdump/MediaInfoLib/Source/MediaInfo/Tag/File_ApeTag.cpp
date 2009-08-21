// File_ApeTag - Info for ApeTag tagged files
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
#if defined(MEDIAINFO_APETAG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_ApeTag.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_ApeTag::Read_Buffer_Init()
{
}

//---------------------------------------------------------------------------
void File_ApeTag::Read_Buffer_Continue()
{
    //ApeTagv2
    if (CC8(Buffer+Buffer_Offset)==CC8("APETAGEX"))
    {
        v2();
        return;
    }

    //ApeTagv1
    if (Buffer_Size>=0x20 && CC8(Buffer+Buffer_Size-0x20)==CC8("APETAGEX"))
    {
        Element_Size=Buffer_Size;
        Common();
        return;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Stream_Prepare(Stream_Audio);

    FLUSH();

    FINNISHED();
}

//---------------------------------------------------------------------------
void File_ApeTag::v2()
{
    ELEMENT(0, "Header", 0x20);

    //Parsing
    Element_Size=Buffer_Size;
    int32u Flags;
    CC_BEGIN()
    SKIP_C8(                                                    Preamble);
    SKIP_L4(                                                    Version);
    SKIP_L4(                                                    Size);
    SKIP_L4(                                                    Count);
    GET_L4 (Flags,                                              Flags);
        SKIP_FLAGS(Flags,  0,                                   Read Only);
        SKIP_FLAGS(Flags,  1,                                   Binary);
        SKIP_FLAGS(Flags,  2,                                   Locator of external stored information);
        SKIP_FLAGS(Flags, 29,                                   Is the header);
        SKIP_FLAGS(Flags, 30,                                   Contains a footer);
        SKIP_FLAGS(Flags, 31,                                   Contains a header);
    SKIP_L8(                                                    Reserved);
    CC_END_CANBEMORE();
    FLUSH();

    Buffer_Offset+=0x20;
    Element_Size-=0x20;
    Common();
}

//---------------------------------------------------------------------------
void File_ApeTag::Common()
{
    //Parsing
    std::string Key;
    Ztring Value;
    int32u Flags, Length;
    CC_BEGIN()
    while (Stream_Pos<Element_Size-0x20)
    {
        GET_L4 (Length,                                         Length);
        GET_L4 (Flags,                                          Flags);
            SKIP_FLAGS(Flags,  0,                               Read Only);
            SKIP_FLAGS(Flags,  1,                               Binary);
            SKIP_FLAGS(Flags,  2,                               Locator of external stored information);
            SKIP_FLAGS(Flags, 29,                               Is the header);
            SKIP_FLAGS(Flags, 30,                               Contains a footer);
            SKIP_FLAGS(Flags, 31,                               Contains a header);
        int32u Stream_Pos_Save=Stream_Pos;
        GET_STRING(Element_Size-Stream_Pos, Key,                Key);
        Stream_Pos=Stream_Pos_Save+Key.size()+1;
        GET_UTF8(Length, Value,                                 Value);
        ELEMENT(0, "Tag", Length);

        //Filling
             if (Key=="ALBUM")          Fill(Stream_General, 0, "Album", Value);
        else if (Key=="ARTIST")         Fill(Stream_General, 0, "Performer", Value);
        else if (Key=="AUTHOR")         Fill(Stream_General, 0, "Performer", Value);
        else if (Key=="COMMENTS")       Fill(Stream_General, 0, "Comment", Value);
        else if (Key=="COMPOSER")       Fill(Stream_General, 0, "WrittentBy", Value);
        else if (Key=="YEAR")           Fill(Stream_General, 0, "Recorded_Date", Value);
        else if (Key=="TRACK")          Fill(Stream_General, 0, "Track/Position", Value);
        else if (Key=="TITLE")          Fill(Stream_General, 0, "Title", Value);
        else                            Fill(Stream_General, 0, Key.c_str(), Value);

        FLUSH();
    }
    ELEMENT(0, "Footer", 0x20);
    SKIP_C8(                                                    Preamble);
    SKIP_L4(                                                    Version);
    SKIP_L4(                                                    Size);
    SKIP_L4(                                                    Count);
    GET_L4 (Flags,                                              Flags);
        SKIP_FLAGS(Flags,  0,                                   Read Only);
        SKIP_FLAGS(Flags,  1,                                   Binary);
        SKIP_FLAGS(Flags,  2,                                   Locator of external stored information);
        SKIP_FLAGS(Flags, 29,                                   Is the header);
        SKIP_FLAGS(Flags, 30,                                   Contains a footer);
        SKIP_FLAGS(Flags, 31,                                   Contains a header);
    SKIP_L8(                                                    Reserved);
    CC_END()
    FLUSH();
}

//***************************************************************************
// External Helper
//***************************************************************************

File_ApeTag_Helper::File_ApeTag_Helper(File__Base* Base_)
{
    #if defined(MEDIAINFO_APETAG_YES)
        Base=Base_;
        ApeTag=NULL;
    #endif //APETAG
}

File_ApeTag_Helper::~File_ApeTag_Helper()
{
    #if defined(MEDIAINFO_APETAG_YES)
        delete ApeTag; //APETAG=NULL;
    #endif //APETAG
}

bool File_ApeTag_Helper::ApeTag_Read_Buffer_Continue()
{
    if (Base->Buffer_Offset+8>Base->Buffer_Size)
        return false;

    //First bytes
    if (CC8(Base->Buffer+Base->Buffer_Offset)!=CC8("APETAGEX"))
        return true; //Nothing

    //Element size
    if (Base->Buffer_Offset+12+4>Base->Buffer_Size)
       return false;
    int32u ApeTagSize=LittleEndian2int32u(Base->Buffer+Base->Buffer_Offset+12)+32;
    if (Base->Buffer_Offset+ApeTagSize>Base->Buffer_Size)
        return false;

    //Details
    Base->ELEMENT(0, "ApeTag tags", ApeTagSize);
    Base->FLUSH();

    //If we continue the ApeTag tag
    #if defined(MEDIAINFO_APETAG_YES)
        if (ApeTag==NULL)
        {
            ApeTag=new File_ApeTag;
            Base->Open_Buffer_Init(ApeTag);
        }

        Base->Open_Buffer_Continue(ApeTag, Base->Buffer+Base->Buffer_Offset, ApeTagSize);
        Base->Buffer_Offset+=ApeTagSize;
        Base->Open_Buffer_Finalize(ApeTag);
    #else
        Base->Buffer_Offset+=ApeTagSize;
    #endif //APETAGV2

    return true;
}

void File_ApeTag_Helper::ApeTag_Read_Buffer_Finalize()
{
    #if defined(MEDIAINFO_APETAG_YES)
    if (ApeTag!=NULL)
    {
        if (ApeTag->Count_Get(Stream_General)>0)
            Base->Merge(*ApeTag, Stream_General, 0, 0);
        delete ApeTag; ApeTag=NULL;
    }
    #endif //APETAG
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_ApeTag::HowTo(stream_t StreamKind)
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


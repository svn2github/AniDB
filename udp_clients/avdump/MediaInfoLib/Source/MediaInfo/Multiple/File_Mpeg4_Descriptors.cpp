// File_Mpeg4 - Info for MPEG-4 files
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
//
// Descriptors part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_MPEG4_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg4.h"
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_AAC_YES)
    #include "MediaInfo/Audio/File_Aac.h"
#endif
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_MPEG4V_YES
    const char* Mpeg4v_Profile_Level(int32u Profile_Level);
#endif //MEDIAINFO_MPEG4V_YES
//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

#define LIST_SET(OFFSET) \
    Buffer_Offset+=OFFSET; \
    Element_Size-=OFFSET; \

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptors()
{
    while (File_Offset+Buffer_Offset<Element_Next[Element_Level])
    {
        FLUSH();

        //Integrity
        if (Element_Level>=MPEG4_ELEMENT_LEVEL_MAX)
        {
            File_Offset=File_Size;
            return;
        }

        //Parsing
        ELEMENT_BEGIN();
        CC_BEGIN();
        int32u Size=0, Size_Bytes=0;
        int8u Descriptor_Type, Size_ToAdd;
        GET_B1(Descriptor_Type,                                 Descriptor_Type);
        do
        {
            GET_B1(Size_ToAdd,                                  Size element);
            Size=(Size<<7) | (Size_ToAdd&0x7F);
            Size_Bytes++;
        }
        while ((Size_ToAdd&0x80) && Size_Bytes<4);
        CC_END_CANBEMORE();

        //Corrupted sizes
        if (Size>Element_Size-Stream_Pos)
            Size=(int32u)Element_Size-Stream_Pos;

        Buffer_Offset+=Stream_Pos;
        Element_Level++;
        Element_Next[Element_Level]=File_Offset+Buffer_Offset+Size;
        Element_Size=Size;

        //Parsing
        ELEMENT(Element_Level-1, Descriptor_Type, Size);
        switch (Descriptor_Type)
        {
            case  0x3 : Descriptor_ES          (); break;
            case  0x4 : Descriptor_DecConfig   (); break;
            case  0x5 : Descriptor_DecSpecific (); break;
            case  0x6 : Descriptor_SLConfig    (); break;
            case 0x10 : Descriptor_Profiles    (); break;
        }
        FLUSH();

        //Next element
        Buffer_Offset=(size_t)(Element_Next[Element_Level]-File_Offset);
        Element_Level--;
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptor_DecConfig()
{
    NAME("DecConfig");

    //Parsing
    int32u MaxBitrate, AvgBitrate;
    int8u ObjectTypeId;
    CC_BEGIN();
    GET_B1 (ObjectTypeId,                                       ObjectTypeId);
    SKIP_B1(                                                    Flags);
    SKIP_B3(                                                    BufferSizeDB);
    GET_B4 (MaxBitrate,                                         MaxBitrate);
    GET_B4 (AvgBitrate,                                         AvgBitrate);
    CC_END_CANBEMORE();

    //Filling
    switch (ObjectTypeId)
    {
        case 0x01 : Fill("Codec", "System", Error, false, true); break;
        case 0x02 : Fill("Codec", "System Core", Error, false, true); break;
        case 0x20 : Fill("Codec", "MPEG-4V", Error, false, true); break;
        case 0x21 : Fill("Codec", "H264", Error, false, true); break;
        case 0x40 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-4 AAC
        case 0x60 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Simple
        case 0x61 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Main
        case 0x62 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V SNR
        case 0x63 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V Spatial
        case 0x64 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V High
        case 0x65 : Fill("Codec", "MPEG-2V", Error, false, true); break; //MPEG-2V 4:2:2
        case 0x66 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC Main
        case 0x67 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC LC
        case 0x68 : Fill("Codec", "AAC", Error, false, true); break; //MPEG-2 AAC SSR
        case 0x69 : Fill("Codec", "MPEG-2A L3", Error, false, true); break;
        case 0x6A : Fill("Codec", "MPEG-1V", Error, false, true); break;
        case 0x6B : Fill("Codec", "MPEG-1A", Error, false, true); break;
        case 0x6C : Fill("Codec", "JPEG", Error, false, true); break;
        case 0xA0 : Fill("Codec", "EVRC", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true); break;
        case 0xA1 : Fill("Codec", "SMV", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
        case 0xD1 : Fill("Codec", "EVRC", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
        case 0xD3 : Fill("Codec", "AC3", Error, false, true); break;
        case 0xD4 : Fill("Codec", "DTS", Error, false, true); break;
        case 0xDD : Fill("Codec", "Ogg", Error, false, true); break;
        case 0xDE : Fill("Codec", "Ogg", Error, false, true); break;
        case 0xE1 : Fill("Codec", "QCELP", Error, false, true); Fill("SamplingRate", "8000"); Fill("Channel(s)", "1", 10, true);  break;
        default: ;
    }
    Fill("Codec/CC", ObjectTypeId, 16, true);

    //Exception, TODO: find a better way to detect ALS
    if ((AvgBitrate>640000) && (ObjectTypeId==0x40))
        Fill("Codec", "ALS", Error, false, true);

    //Bitrate mode
    if (AvgBitrate>0)
    {
        Fill("BitRate", AvgBitrate);
        if (MaxBitrate<=AvgBitrate*1.1)
            Fill("BitRate_Mode", "CBR");
        else
            Fill("BitRate_Mode", "VBR");
    }

    //Other descriptors
    LIST_SET(13);
    Descriptors();
}

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptor_DecSpecific()
{
    NAME("DecSpecific");

    //Creating the parser
    File__Base* MI=NULL;
         if (0);
    #if defined(MEDIAINFO_MPEG4V_YES)
    else if (Get(StreamKind_Last, StreamPos_Last, _T("Codec"))==_T("MPEG-4V"))
        MI=new File_Mpeg4v;
    #endif
    #if defined(MEDIAINFO_AAC_YES)
    else if (Get(StreamKind_Last, StreamPos_Last, _T("Codec"))==_T("AAC"))
        MI=new File_Aac;
    #endif
    else
        return;

    //Parsing
    Open_Buffer_Init(MI, File_Offset+Buffer_Offset+Element_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(MI, Buffer+Buffer_Offset, (size_t)Element_Size);
    Open_Buffer_Finalize(MI);

    //Filling
    Merge(*MI, StreamKind_Last, 0, StreamPos_Last);
    delete MI; //MI=NULL;
}

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptor_ES()
{
    NAME("ES");

    //Parsing
    CC_BEGIN()
    int8u Flags;
    bool DependsOnESID_Flag, URL_Flag, OCRESID_Flag;
    SKIP_B2(                                                    ESID);
    GET_B1 (Flags,                                              Flags);
        SKIP_FLAGS(Flags, 0,                                    StreamPriority); //5 bits
        SKIP_FLAGS(Flags, 1,                                    StreamPriority);
        SKIP_FLAGS(Flags, 2,                                    StreamPriority);
        SKIP_FLAGS(Flags, 3,                                    StreamPriority);
        SKIP_FLAGS(Flags, 4,                                    StreamPriority);
        GET_FLAGS (Flags, 5, DependsOnESID_Flag,                DependsOnESID);
        GET_FLAGS (Flags, 6, URL_Flag,                          URL);
        GET_FLAGS (Flags, 7, OCRESID_Flag,                      OCRESID);
    if (DependsOnESID_Flag)
        SKIP_B2(                                                DependsOnESID);
    if (URL_Flag)
    {
        int8u Size;
        GET_B1(Size,                                            URL size);
        SKIP_LOCAL(Size,                                        URL);
    }
    if (OCRESID_Flag)
        SKIP_B2(                                                OCRESID);
    CC_END_CANBEMORE();

    //Other descriptors
    LIST_SET(Stream_Pos);
    Descriptors();
}

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptor_SLConfig()
{
    NAME("SLConfig");

    //Reading
    CC_BEGIN();
    SKIP_B1(                                                    Predefined);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::Descriptor_Profiles()
{
    NAME("Profiles");

    //Reading
    CC_BEGIN();
    SKIP_B1(                                                    Unknown);
    SKIP_B1(                                                    ID);
    SKIP_B1(                                                    ODProfileLevel);
    SKIP_B1(                                                    SceneProfileLevel);
    SKIP_B1(                                                    AudioProfileLevel);
    INFO_B1(VideoProfileLevel,                                  VideoProfileLevel);
        #ifdef MEDIAINFO_MPEG4V_YES
            INFO(Mpeg4v_Profile_Level(VideoProfileLevel));
        #endif //MEDIAINFO_MPEG4V_YES
    SKIP_B1(                                                    GraphicsProfileLevel);
    CC_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES

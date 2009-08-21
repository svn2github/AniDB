// File_Ogg_SubElement - Info for OGG files
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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_OGG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Ogg_SubElement.h"
#include "MediaInfo/Audio/File_Vorbis.h"
#include "MediaInfo/Tag/File_VorbisCom.h"
#include <ZenLib/ZtringListList.h>
#include <ZenLib/BitStream.h>
#include <ZenLib/Utils.h>
#include <math.h>
#ifdef __BORLANDC__
    #include <mem.h>
#endif //__BORLANDC__
#include <stdlib.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Ogg_SubElement::File_Ogg_SubElement()
: File__Analyze()
{
    SetupFinnished=false;
}

//---------------------------------------------------------------------------
File_Ogg_SubElement::~File_Ogg_SubElement()
{
    delete Setup_Vorbis; //Setup_Vorbis=NULL;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Read_Buffer_Init()
{
    OldSize=0;
    Setup_Vorbis=NULL;
    absolute_granule_position_Resolution=0;
    StreamKind=Stream_Max;

    File__Analyze::Read_Buffer_Init();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ogg_SubElement::Header_Begin()
{
    //Already parsed (there is only one pass)
    if (Buffer_Offset!=0)
        return false;

    //We are waiting for the end of the stream, signaled by a empty buffer adding
    if (Buffer_Size!=OldSize)
    {
        OldSize=Buffer_Size;
        return false;
    }
    else
    {
        OldSize=0;
        return true;
    }
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Header_Parse()
{
    //Parsing
    int64u SamplesCount;
    int32u SamplesCount4;
    int16u SamplesCount2;
    int8u  SamplesCount1, Type;
    bool lenbytes0, lenbytes1, lenbytes2;
    GET_L1 (Type,                                               "Type");
    if ((Type&0x01)==0 && Type!=0x80) //TODO : find a better algo
    {
        SKIP_FLAGS(Type, 0,                                     "Indicates data packet");
        GET_FLAGS (Type, 1, lenbytes2,                          "Bit 2 of lenbytes");
        SKIP_FLAGS(Type, 2,                                     "unused");
        SKIP_FLAGS(Type, 3,                                     "Keyframe");
        SKIP_FLAGS(Type, 4,                                     "unused");
        SKIP_FLAGS(Type, 5,                                     "unused");
        GET_FLAGS (Type, 6, lenbytes0,                          "Bit 0 of lenbytes");
        GET_FLAGS (Type, 7, lenbytes1,                          "Bit 1 of lenbytes");
        if (lenbytes2)
        {
            if (lenbytes1)
            {
                if (lenbytes0)
                    //GET_L7 (SamplesCount,                   "SamplesCount");
                    Element_Pos+=7;
                else
                    //GET_L6 (SamplesCount,                   "SamplesCount");
                    Element_Pos+=6;
            }
            else
            {
                if (lenbytes0)
                    //GET_L5 (SamplesCount,                   "SamplesCount");
                    Element_Pos+=5;
                else
                {
                    GET_L4 (SamplesCount4,                  "SamplesCount");
                    SamplesCount=SamplesCount4;
                }
            }
        }
        else
        {
            if (lenbytes1)
            {
                if (lenbytes0)
                {
                    GET_L3 (SamplesCount4,                  "SamplesCount");
                    SamplesCount=SamplesCount4;
                }
                else
                {
                    GET_L2 (SamplesCount2,                  "SamplesCount");
                    SamplesCount=SamplesCount2;
                }
            }
            else
            {
                if (lenbytes0)
                {
                    GET_L1 (SamplesCount1,                  "SamplesCount");
                    SamplesCount=SamplesCount1;
                }
                //else
                //    GET_L0 (SamplesCount,                   "SamplesCount");
            }
        }
    }

    //Filling
    Header_Fill_Code(Type, Ztring::ToZtring(Type, 16));
    Header_Fill_Size(Element_Size);

    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);
}

//---------------------------------------------------------------------------
bool File_Ogg_SubElement::Data_Parse()
{
    //Parsing
    switch (Element[Element_Level].Code)
    {
        case 0x01 : Identification(); break;
        case 0x03 : Comment(); break;
        case 0x05 : Setup(); break;
        case 0x80 : Identification(); break;
        case 0x81 : Comment(); break;
        case 0x82 : Setup(); break;
        default   : SKIP_XX(Element_Size,                       "Data");
                    SetupFinnished=true;
    }

    return true;
}

//---------------------------------------------------------------------------
// Identification chunk, video
// StreamKind                       8 bytes, Pos=0
// Datas                            X bytes, Pos=4
//
void File_Ogg_SubElement::Identification()
{
    Element_Name("Identification");

    //Parsing
    GET_C6 (ID_Identification,                                  "ID");

    //Filling
    #undef ELEMENT_CASE
    #ifdef __BORLANDC__ //Borland converts int64u to int32u
        #define ELEMENT_CASE(_NAME) \
            else if (ID_Identification==Ogg::_NAME##1*0x100000000LL+Ogg::_NAME##2) Identification_##_NAME();

    #else //__BORLANDC__
        #define ELEMENT_CASE(_NAME) \
            else if (ID_Identification==Ogg::_NAME) Identification_##_NAME();

    #endif //__BORLANDC__

    if (0) ;
    ELEMENT_CASE(vorbis)
    ELEMENT_CASE(theora)
    ELEMENT_CASE(video)
    ELEMENT_CASE(audio)
    ELEMENT_CASE(text)
    else
    {
        SetupFinnished=true;
        return;
    }

    //Filling
    StreamKind=StreamKind_Last;
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Identification_vorbis()
{
    Element_Info("Vorbis");

    //Parsing
    int32u Version, SamplingRate, BitRate_Maximum, BitRate_Nominal, BitRate_Minimum;
    int8u Channels;
    GET_L4 (Version,                                            "Version");
    if (Version>0)
        return; //Not supported
    GET_L1 (Channels,                                           "Channels");
    GET_L4 (SamplingRate,                                       "SamplingRate");
    GET_L4 (BitRate_Maximum,                                    "BitRate_Maximum");
    GET_L4 (BitRate_Nominal,                                    "BitRate_Nominal");
    GET_L4 (BitRate_Minimum,                                    "BitRate_Minimum");
    CC_BS_BEGIN();
    SKIP_BS(4,                                                  "BlockSize_0"); //2^Value
    SKIP_BS(4,                                                  "BlockSize_1"); //2^Value
    CC_BS_END();
    SKIP_L1(                                                    "Framing");

    //Filling
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "Vorbis");
    if (BitRate_Maximum!=0 && BitRate_Maximum<0x80000000) //This is a signed value, and negative values are not OK
        Fill("BitRate/Maximum", BitRate_Maximum);
    if (BitRate_Nominal!=0 && BitRate_Nominal<0x80000000) //This is a signed value, and negative values are not OK
        Fill("BitRate", BitRate_Nominal);
    if (BitRate_Minimum!=0 && BitRate_Minimum<0x80000000) //This is a signed value, and negative values are not OK
        Fill("BitRate/Minimum", BitRate_Minimum);
    Fill("Channel(s)", Channels);
    Fill("SamplingRate", SamplingRate);
    absolute_granule_position_Resolution=SamplingRate;
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Identification_theora()
{
    Element_Info("Theora");

    //Parsing
    int32u Version, PICW=0, PICH=0, FRN=0, FRD=0, PARN=0, PARD=0, NOMBR=0;
    GET_B3 (Version,                                        "Version");
    if (Version==0x030200) //Version 3.2.x
    {
        SKIP_B2(                                            "FMBW");
        SKIP_B2(                                            "FMBH");
        GET_B3 (PICW,                                       "PICW");
        GET_B3 (PICH,                                       "PICH");
        SKIP_B1(                                            "PICX");
        SKIP_B1(                                            "PICY");
        GET_B4 (FRN,                                        "FRN");
        GET_B4 (FRD,                                        "FRD");
        GET_B3 (PARN,                                       "PARN");
        GET_B3 (PARD,                                       "PARD");
        SKIP_B1(                                            "CS"); // //0=4:2:0, 2=4:2:2, 3=4:4:4
        GET_B3 (NOMBR,                                      "NOMBR"); //The nominal bitrate of the stream
        CC_BS_BEGIN();
        SKIP_BS( 6,                                         "QUAL"); //The quality hint.
        SKIP_BS( 5,                                         "KFGSHIFT");
        SKIP_BS( 2,                                         "PF"); //The Pixel Format
        SKIP_BS( 3,                                         "Reserved");
        CC_BS_END();
    }

    //Filling
    Stream_Prepare(Stream_Video);
    Fill("Codec", "Theora");
    if (Version!=0x030200) //Version 3.2.x
        return;
    Fill("FrameRate", ((float)FRN)/FRD, 3);
    float PixelRatio=1;
    if (PARN && PARN)
        PixelRatio=((float)PARN)/(float)PARN;
    Fill("Width", PICW);
    Fill("Height", PICH);
    Fill("AspectRatio", ((float)PICW)/((float)PICH)*PixelRatio, 3);
    if (NOMBR)
        Fill("BitRate_Nominal", NOMBR);
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Identification_video()
{
    Element_Info("Video");

    //Parsing
    int64u TimeUnit;
    int32u fccHandler, Width, Height;
    SKIP_L2(                                                    "Reserved");
    GET_C4 (fccHandler,                                         "fccHandler");
    SKIP_L4(                                                    "SizeOfStructure");
    GET_L8 (TimeUnit,                                           "TimeUnit"); //10000000/TimeUnit is stream tick rate in ticks/sec
    SKIP_L4(                                                    "SamplesPerUnit");
    SKIP_L8(                                                    "DefaultLengh"); //in media time
    SKIP_L4(                                                    "BufferSize");
    SKIP_L2(                                                    "BitsPerSample");
    SKIP_L2(                                                    "Reserved");
    GET_L4 (Width,                                              "Width");
    GET_L4 (Height,                                             "Height");

    //Filling
    Stream_Prepare(Stream_Video);
    Fill("Codec", Ztring().From_CC4(fccHandler));
    Fill("FrameRate", (float)10000000/(float)TimeUnit, 3);
    Fill("Width", Width);
    Fill("Height", Height);
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Identification_audio()
{
    Element_Info("Audio");

    //Parsing
    int64u TimeUnit, SamplesPerUnit;
    int32u fccHandler, AvgBytesPerSec;
    int16u Channels;
    SKIP_L2(                                                    "Reserved");
    GET_C4 (fccHandler,                                         "fccHandler");
    SKIP_L4(                                                    "SizeOfStructure");
    GET_L8 (TimeUnit,                                           "TimeUnit"); //10000000/TimeUnit is stream tick rate in ticks/sec
    GET_L8 (SamplesPerUnit,                                     "SamplesPerUnit");
    SKIP_L4(                                                    "DefaultLengh"); //in media time
    SKIP_L4(                                                    "BufferSize");
    SKIP_L2(                                                    "BitsPerSample");
    SKIP_L2(                                                    "Reserved");
    GET_L2 (Channels,                                           "Channels");
    SKIP_L2(                                                    "BlockAlign");
    GET_L4 (AvgBytesPerSec,                                     "AvgBytesPerSec");

    //Filling
    Stream_Prepare(Stream_Audio);
    Fill("Codec", Ztring().From_CC4(fccHandler));
    if (AvgBytesPerSec<0x80000000) //This is a signed value, and negative values are not OK
        Fill("BitRate", AvgBytesPerSec*8);
    Fill("Channel(s)", Channels==5?6:Channels); //5 channels are 5.1
    Fill("SamplingRate", SamplesPerUnit);
    absolute_granule_position_Resolution=SamplesPerUnit;
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Identification_text()
{
    Element_Info("Text");

    //Parsing
    SKIP_L2(                                                    "Reserved");

    //Filling
    Stream_Prepare(Stream_Text);
    Fill("Codec", "Subrip");
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment()
{
    Element_Name("Comment");

    //Parsing
    int64u ID;
    GET_C6 (ID,                                                 "ID");

    //Filling
    #undef ELEMENT_CASE
    #ifdef __BORLANDC__ //Borland converts int64u to int32u
        #define ELEMENT_CASE(_NAME) \
            else if (ID==Ogg::_NAME##1*0x100000000LL+Ogg::_NAME##2) Comment_##_NAME();

    #else //__BORLANDC__
        #define ELEMENT_CASE(_NAME) \
            else if (ID==Ogg::_NAME) Comment_##_NAME();

    #endif //__BORLANDC__

    if (0) ;
    ELEMENT_CASE(vorbis)
    ELEMENT_CASE(theora)
    ELEMENT_CASE(video)
    ELEMENT_CASE(audio)
    ELEMENT_CASE(text)
    else
        SetupFinnished=true;

    //Must finnish?
    #undef ELEMENT_CASE
    #ifdef __BORLANDC__ //Borland converts int64u to int32u
        #define ELEMENT_CASE(_NAME) \
            else if (ID_Identification==Ogg::_NAME##1*0x100000000LL+Ogg::_NAME##2) SetupFinnished=true;

    #else //__BORLANDC__
        #define ELEMENT_CASE(_NAME) \
            else if (ID_Identification==Ogg::_NAME) SetupFinnished=true;

    #endif //__BORLANDC__

    if (0) ;
    ELEMENT_CASE(video)
    ELEMENT_CASE(audio)
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment_vorbis()
{
    Element_Info("Vorbis");

    //Integrity
    if (Element_Size<6)
        return;

    File_VorbisCom* Vorbis=new File_VorbisCom;
    Vorbis->StreamKind=StreamKind;

    //Open
    Open_Buffer_Init(Vorbis, File_Size, File_Offset+Buffer_Offset+6);
    Open_Buffer_Continue(Vorbis, Buffer+Buffer_Offset+6, Element_Size-6);
    Merge(*Vorbis, Stream_General, 0, 0);
    Merge(*Vorbis, StreamKind, 0, 0);
    Merge(*Vorbis, Stream_Chapters, 0, 0);

    //int32u Element_Pos=Vorbis->Comment_Size;
    delete Vorbis; //Vorbis=NULL;
    /*if (CC1(Buffer+Buffer_Offset+Element_Pos)==0x01) //End bit
        Element_Pos++;

    if (Element_Pos==Element_Size)
        File_Offset=File_Size;

    //Parsing Setup
    while (CC1(Buffer+Buffer_Offset+Element_Pos)==0x00)
        Element_Pos++;

    Buffer_Offset+=Element_Pos;
    Element_Size-=Element_Pos;
    Buffer_Parse();*/
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment_theora()
{
    Comment_vorbis();
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment_video()
{
    Comment_vorbis();
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment_audio()
{
    Comment_vorbis();
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Comment_text()
{
    Comment_vorbis();
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Setup()
{
    Element_Name("Setup");

    //Parsing
    int64u ID;
    GET_C6 (ID,                                                 "ID");

    //Filling
    #undef ELEMENT_CASE
    #ifdef __BORLANDC__ //Borland converts int64u to int32u
        #define ELEMENT_CASE(_NAME) \
            else if (ID==Ogg::_NAME##1*0x100000000LL+Ogg::_NAME##2) Setup_##_NAME();

    #else //__BORLANDC__
        #define ELEMENT_CASE(_NAME) \
            else if (ID==Ogg::_NAME) Setup_##_NAME();

    #endif //__BORLANDC__

    if (0) ;
    ELEMENT_CASE(vorbis)
    //ELEMENT_CASE(theora)
    //ELEMENT_CASE(video)
    //ELEMENT_CASE(audio)
    //ELEMENT_CASE(text)
    SetupFinnished=true;
}

//---------------------------------------------------------------------------
void File_Ogg_SubElement::Setup_vorbis()
{
    Element_Info("Vorbis");

    //Integrity
    if (Element_Size<6)
        return;

    if (Setup_Vorbis==NULL)
        Setup_Vorbis=new File_Vorbis;

    //Open
    Open_Buffer_Init(Setup_Vorbis, File_Size, File_Offset+Buffer_Offset+6);
    Open_Buffer_Continue(Setup_Vorbis, Buffer+Buffer_Offset+6, Element_Size-6);
    Merge(*Setup_Vorbis, Stream_Audio, 0, 0);
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_OGG_YES

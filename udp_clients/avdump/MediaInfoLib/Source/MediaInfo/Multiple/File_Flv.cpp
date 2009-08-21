// File_Flv - Info for Flash files
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
//
// Examples:
// http://samples.mplayerhq.hu/FLV/
//
// Reverse engineering
// http://osflash.org/documentation/amf/astypes
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_FLV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Flv.h"
#include "ZenLib/Utils.h"
#include "ZenLib/BitStream.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const int16u  Flv_Channels[]=
{
    1,
    2,
};

const int16u  Flv_Resolution[]=
{
    8,
    16,
};

const int16u Flv_SamplingRate[]=
{
    5500,
    11025,
    22050,
    44100,
    8000, //Special case for Nellymoser 8kHz mono
};

const char* Flv_Codec_Audio[]=
{
    "Uncompressed",
    "ADPCM",
    "MPEG-1A L3",
    "",
    "",
    "Nellymoser 8kHz mono",
    "Nellymoser",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Flv_Codec_Video[]=
{
    "",
    "",
    "Sorenson H263",
    "Screen video",
    "On2 VP6",
    "On2 VP6 with alpha channel",
    "Screen video 2",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Flv_H263_PictureSize[]=
{
    "custom, 1 byte",
    "custom, 2 bytes",
    "CIF (352x288)",
    "QCIF (176x144)",
    "SQCIF (128x96)",
    "320x240",
    "160x120",
    "",
};

const int32u Flv_H263_WidthHeight[8][2]=
{
    {  0,   0},
    {  0,   0},
    {352, 288},
    {176, 144},
    {128, 96},
    {320, 240},
    {160, 120},
    {0, 0},
};

const char* Flv_H263_PictureType[]=
{
    "IntraFrame",
    "InterFrame",
    "InterFrame (Disposable)",
    "",
};
const char* Flv_VP6_FrameMode[]=
{
    "IntraFrame",
    "",
};

const char* Flv_VP6_Marker[]=
{
    "VP6.1/6.2",
    "VP6.0",
};

const char* Flv_VP6_Version[]=
{
    "",
    "",
    "",
    "",
    "",
    "",
    "VP6.0/6.1",
    "VP6.0 (Electronic Arts)",
    "VP6.2",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Flv_VP6_Version2[]=
{
    "VP6.0",
    "",
    "",
    "VP6.1/6.2",
};

const char* Flv_FrameType[]=
{
    "",
    "KeyFrame",
    "InterFrame",
    "InterFrame (Disposable)",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Flv_TagType[]=
{
    "DOUBLE",
    "UI8",
    "SCRIPTDATASTRING",
    "SCRIPTDATAOBJECT[n]",
    "SCRIPTDATASTRING defining the MovieClip path",
    "Null",
    "Undefined",
    "UI16",
    "SCRIPTDATAVARIABLE[ECMAArrayLength]",
    "EndOfObject",
    "SCRIPTDATAVARIABLE[n]",
    "SCRIPTDATADATE",
    "SCRIPTDATALONGSTRING",
    "Unsupported",
    "Recordset",
    "XML",
    "TypedObject",
    "AMF3 data",
};

const char* Flv_Amf3Type[]=
{
    "Undefined",
    "Null",
    "Boolean-false",
    "Boolean-true",
    "Integer",
    "Number",
    "String",
    "XML",
    "Data",
    "Array",
    "Object",
    "XML String",
    "ByteArray",
};

//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flv::Read_Buffer_Continue()
{
    if (File_Offset==0)
    {
        if (Buffer_Size<9 || CC3(Buffer)!=CC3("FLV"))
        {
            File_Offset=File_Size;
            return;
        }

        header();
        Buffer_Offset+=Element_Size;
    }

    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Flv::Read_Buffer_Finalize()
{
    if (Video.size()>0 && Get(Stream_Video, 0, _T("Codec")).empty())
        Video[0].clear(); //Said there is video, but there is not!
    if (Audio.size()>0 && Get(Stream_Audio, 0, _T("Codec")).empty())
        Audio[0].clear(); //Said there is audio, but there is not!
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Flv::Buffer_Parse()
{
    if (Buffer_Offset+15>Buffer_Size)
        return false; //Not enough buffer
    Element_Size=15;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    PreviousTagSize);
    GET_B1 (Element_Name,                                       Type); //INFO(Element_Name<19?Flv_Type[Element_Name]:_T("Unknown"));
    GET_B3 (Element_Size,                                       BodyLength);
    SKIP_B3(                                                    Timestamp_Base); //in ms
    SKIP_B1(                                                    Timestamp_Extended); //TimeStamp = Timestamp_Extended*0x01000000+Timestamp_Base
    SKIP_B3(                                                    StreamID);
    CC_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    Element_Next=File_Offset+Buffer_Offset+15+Element_Size;

    //Enough data?
    if (Element_Next>File_Offset+Buffer_Size)
        return false; //Wait for more data

    //Parsing
    Element_Parse();

    //If no need of more
    if (Flags==0x00)
        FINNISHED_BOOL();

    Buffer_Offset=(size_t)(Element_Next-File_Offset);

    return true;
}

//---------------------------------------------------------------------------
bool File_Flv::Element_Parse()
{
    ELEMENT(0, Element_Name, Element_Size);

    //Positionning
    Buffer_Offset+=15;

    switch (Element_Name)
    {
        case 0x08 : audio(); break;
        case 0x09 : video(); break;
        case 0x12 : meta(); break;
        default : break;
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flv::header()
{
    ELEMENT(0, "FLV header", 9);

    if (Buffer_Offset+9>Buffer_Size)
        return; //Not enough buffer
    Element_Size=9;

    //Parsing
    bool Flag_Video, Flag_Audio;
    int32u Version;
    CC_BEGIN();
    SKIP_LOCAL(3,                                               Signature);
    GET_B1 (Version,                                            Version);
    GET_B1 (Flags,                                              Flags);
        GET_FLAGS (Flags, 0, Flag_Video,                        Video);
        GET_FLAGS (Flags, 2, Flag_Audio,                        Audio);
    GET_B4 (Element_Size,                                       Size);
    CC_END();

    //Integrity
    if (Version==0 || Version>1)
        return;
    if (Element_Size<9)
        return;

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "FLV");
    if (Flag_Video) Stream_Prepare(Stream_Video);
    if (Flag_Audio) Stream_Prepare(Stream_Audio);
    FLUSH();
}

//---------------------------------------------------------------------------
void File_Flv::video()
{
    NAME("Video");

    //Needed?
    if (!(Flags&0x01))
        return; //No more need of Audio stream

    //Parsing
    int32u Codec, FrameType;
    BS_BEGIN();
    GET_BS (4, FrameType,                                       frameType); INFO(Flv_FrameType[FrameType]);
    GET_BS (4, Codec,                                           codecID); INFO(Flv_Codec_Video[Codec]);

    if (FrameType!=1) //Not a KeyFrame
        return;

    //Filling
    if (Get(Stream_Video, 0, _T("Codec")).empty())
    {
        //Filling
        if (Count_Get(Stream_Video)==0)
            Stream_Prepare(Stream_Video);
        if (Codec<6)
            Fill(Stream_Video, 0, "Codec", Flv_Codec_Video[Codec]);
        Flags&=0xFE; //No more need of Video stream

        //Parsing video data
        Buffer_Offset+=1;
        Element_Size+=1;
        switch (Codec)
        {
            case 2 : video_H263(); break;
            case 3 : video_ScreenVideo(); break;
            case 4 : video_VP6(false); break;
            case 5 : video_VP6(true); break;
            case 6 : video_ScreenVideo(); break;
        }
    }
}

//---------------------------------------------------------------------------
void File_Flv::video_H263()
{
    //Parsing
    int32u Version, PictureSize, Width, Height, PictureType;
    BS_BEGIN();
    SKIP_BS(17,                                                 PictureStartCode)
    GET_BS (5, Version,                                         Version)
    if (Version>1)
        return;
    SKIP_BS(8,                                                  TemporalReference)
    GET_BS (3, PictureSize,                                     PictureSize) INFO(Flv_H263_PictureSize[PictureSize]);
    switch (PictureSize)
    {
        case 0 :
            GET_BS ( 8, Width,                                  Width)
            GET_BS ( 8, Height,                                 Height)
            break;
        case 1 :
            GET_BS (16, Width,                                  Width)
            GET_BS (16, Height,                                 Height)
            break;
        default :
            Width=Flv_H263_WidthHeight[PictureSize][0];
            Height=Flv_H263_WidthHeight[PictureSize][1];
    }
    GET_BS (2, PictureType,                                     PictureSize) INFO(Flv_H263_PictureType[PictureType]);
    SKIP_BS(1,                                                  DeblockingFlag)
    SKIP_BS(5,                                                  Quantizer)

    //Filling
    Fill(Stream_Video, 0, "Width", Width, 10, true);
    Fill(Stream_Video, 0, "Height", Height, 10, true);
}

//---------------------------------------------------------------------------
void File_Flv::video_ScreenVideo()
{
    //Parsing
    int32u Width, Height;
    BS_BEGIN();
    SKIP_BS(4,                                                  PixelWidth)
    GET_BS (12, Width,                                          Width)
    SKIP_BS(4,                                                  PixelHeight)
    GET_BS (12, Height,                                         Height)

    //Filling
    //Fill(Stream_Video, 0, "Width", Width&0x3FFF, 10, true);
    //Fill(Stream_Video, 0, "Height", Height&0x3FFF, 10, true);
}

//---------------------------------------------------------------------------
// From: http://wiki.multimedia.cx/index.php?title=On2_VP6
//
void File_Flv::video_VP6(bool WithAlpha)
{
    //Parsing
    int32u HorizontalAdjustment, VerticalAdjustment, Version, Version2, FrameMode, Marker, Width=0, Height=0;
    BS_BEGIN();
    GET_BS ( 4, HorizontalAdjustment,                           HorizontalAdjustment) INFO(Ztring(_T("Remove "))+Ztring::ToZtring(HorizontalAdjustment)+_T(" pixels"));
    GET_BS ( 4, VerticalAdjustment,                             VerticalAdjustment) INFO(Ztring(_T("Remove "))+Ztring::ToZtring(VerticalAdjustment)+_T(" pixels"));
    if (WithAlpha)
        SKIP_BS(24,                                             OffsetToAlpha)
    GET_BS ( 1, FrameMode,                                      FrameMode) INFO(Flv_VP6_FrameMode[FrameMode]);
    SKIP_BS( 6,                                                 Quantization)
    GET_BS ( 1, Marker,                                         Marker) INFO(Flv_VP6_Marker[Marker]);
    if (FrameMode==0)
    {
        GET_BS ( 5, Version,                                    Version)
        GET_BS ( 2, Version2,                                   Version2)
        SKIP_BS( 1,                                             Interlace)
        if (Marker==1 || Version2==0)
            SKIP_BS(16,                                         Offset)
        SKIP_BS( 8,                                             MacroBlock_Height)
        SKIP_BS( 8,                                             MacroBlock_Width)
        GET_BS ( 8, Height,                                     Height) INFO(Ztring::ToZtring(Height*16)+_T(" pixels"));
        GET_BS ( 8, Width,                                      Width) INFO(Ztring::ToZtring(Width*16)+_T(" pixels"));
    }
    else
    {
        if (Marker==1)
            SKIP_BS( 16,                                        Offset)
    }

    //Filling
    if (Width && Height)
    {
        Fill(Stream_Video, 0, "Width",  Width*16-HorizontalAdjustment, 10, true);
        Fill(Stream_Video, 0, "Height", Height*16-VerticalAdjustment, 10, true);
    }
}

//---------------------------------------------------------------------------
void File_Flv::audio()
{
    NAME("Audio");

    //Needed?
    if (!(Flags&0x04))
        return; //No more need of Aufio stream

    //Parsing
    int32u Channels, Resolution, SamplingRate, Codec;
    BS_BEGIN();
    GET_BS (4, Codec,                                           codec) INFO(Flv_Codec_Audio[Codec]);
    GET_BS (2, SamplingRate,                                    sampling_rate) INFO(Ztring::ToZtring(Flv_SamplingRate[SamplingRate])+_T(" Hz"));
    GET_BS (1, Resolution,                                      is_16bit) INFO(Ztring::ToZtring(Flv_Resolution[Resolution])+_T(" bits"));
    GET_BS (1, Channels,                                        is_stereo) INFO(Ztring::ToZtring(Flv_Channels[Channels])+_T(" channel(s)"));

    //Special case
    if (Codec==5) //Nellymoser 8kHz mono
    {
        SamplingRate=5; //8000 Hz forced
        Channels=0; //Mono forced
    }

    //Filling
    if (Get(Stream_Audio, 0, _T("Codec")).empty())
    {
        //Filling
        if (Count_Get(Stream_Audio)==0)
            Stream_Prepare(Stream_Audio);
        Fill(Stream_Audio, 0, "Channel(s)", Flv_Channels[Channels], 10, true);
        Fill(Stream_Audio, 0, "Resolution", Flv_Resolution[Resolution], 10, true);
        Fill(Stream_Audio, 0, "SamplingRate", Flv_SamplingRate[SamplingRate], 10, true);
        if (Codec<6)
            Fill(Stream_Audio, 0, "Codec", Flv_Codec_Audio[Codec]);
        Flags&=0xFB; //No more need of Audio stream
    }
}

//---------------------------------------------------------------------------
void File_Flv::meta()
{
    NAME("Meta");

    //Parsing
    CC_BEGIN();
    meta_Level=0;
    meta_SCRIPTDATAOBJECT(Stream_Pos);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Flv::meta_SCRIPTDATAOBJECT(int32u &Stream_Pos)
{
    //Parsing Value
    std::string StringData;
    meta_SCRIPTDATAVALUE(Stream_Pos, StringData);
    meta_SCRIPTDATAVALUE(Stream_Pos, StringData);
}

//---------------------------------------------------------------------------
void File_Flv::meta_SCRIPTDATAVARIABLE(int32u &Stream_Pos)
{
    std::string StringData;
    int16u StringLength;
    FLUSH();
    ELEMENT_BEGIN();
    GET_B2 (StringLength,                                       StringLength)
    GET_STRING(StringLength, StringData,                        StringData)
    ELEMENT(Details_Level_Last, StringData.c_str(), 0);

    //Parsing Value
    meta_SCRIPTDATAVALUE(Stream_Pos, StringData);
    ELEMENT_END();
    FLUSH();
}

//---------------------------------------------------------------------------
void File_Flv::meta_SCRIPTDATAVALUE(int32u &Stream_Pos, const std::string &StringData)
{
    //Parsing
    int8u Type;
    GET_B1 (Type,                                               Type) if (Type<0x12) INFO(Flv_TagType[Type]);
    switch (Type)
    {
        case 0x00 : //DOUBLE --> 64 bits Big endian float
            {
                Ztring Value;
                double Value_Double;
                stream_t StreamKind=Stream_General;
                int64u Double;
                GET_B8(Double,                                  Double);
                Value_Double=*((double*)(&Double));
                INFO(Ztring::ToZtring(Value_Double));
                std::string ToFill;
                     if (0) ;
                else if (StringData=="width") {if (Count_Get(Stream_Video)==0) Stream_Prepare(Stream_Video); ToFill="Width"; StreamKind=Stream_Video; Value.From_Number(Value_Double, 0);}
                else if (StringData=="height") {if (Count_Get(Stream_Video)==0) Stream_Prepare(Stream_Video); ToFill="Height"; StreamKind=Stream_Video; Value.From_Number(Value_Double, 0);}
                else if (StringData=="duration") {ToFill="PlayTime"; Value.From_Number(Value_Double*1000, 0);}
                else if (StringData=="audiodatarate") {if (Count_Get(Stream_Audio)==0) Stream_Prepare(Stream_Audio); ToFill="BitRate"; StreamKind=Stream_Audio; Value.From_Number(Value_Double*1000, 0);}
                else if (StringData=="framerate") {if (Count_Get(Stream_Video)==0) Stream_Prepare(Stream_Video); ToFill="FrameRate"; StreamKind=Stream_Video; Value.From_Number(Value_Double, 3);}
                else if (StringData=="datasize") {}
                else if (StringData=="lasttimestamp") {}
                else if (StringData=="audiosize") {if (Count_Get(Stream_Audio)==0) Stream_Prepare(Stream_Audio); ToFill="StreamSize"; StreamKind=Stream_Audio; Value.From_Number(Value_Double, 0);}
                else if (StringData=="videosize") {if (Count_Get(Stream_Video)==0) Stream_Prepare(Stream_Video); ToFill="StreamSize"; StreamKind=Stream_Video; Value.From_Number(Value_Double, 0);}
                else if (StringData=="videodatarate") {if (Count_Get(Stream_Video)==0) Stream_Prepare(Stream_Video); ToFill="BitRate"; StreamKind=Stream_Video; Value.From_Number(Value_Double*1000, 0);}
                else if (StringData=="lastkeyframetimestamp") {}
                else if (StringData=="lastkeyframelocation") {}
                else if (StringData=="videocodecid") {}
                else if (StringData=="audiocodecid") {}
                else if (StringData=="audiodelay") {if (Count_Get(Stream_Audio)==0) Stream_Prepare(Stream_Audio); ToFill="Delay"; StreamKind=Stream_Audio; if (Value_Double>0) Value.From_Number(Value_Double*1000, 0);}
                else if (StringData=="canSeekToEnd") {}
                else if (StringData=="keyframes_times") {}
                else if (StringData=="keyframes_filepositions") {}
                else if (StringData=="audiosamplerate") {if (Count_Get(Stream_Audio)==0) Stream_Prepare(Stream_Audio); ToFill="SamplingRate"; StreamKind=Stream_Audio; if (Value_Double>0) Value.From_Number(Value_Double, 0);}
                else if (StringData=="audiosamplesize") {if (Count_Get(Stream_Audio)==0) Stream_Prepare(Stream_Audio); ToFill="Resolution"; StreamKind=Stream_Audio; if (Value_Double>0) Value.From_Number(Value_Double, 0);}
                else {StreamKind=Stream_General; ToFill=StringData; Value.From_Number(Value_Double);}
                Fill(StreamKind, 0, ToFill.c_str(), Value);
            }
            break;
        case 0x01 : //UI8
            {
                int8u Value;
                GET_B1 (Value,                                  Value)
                std::string ToFill;
                     if (0) ;
                else if (StringData=="haskeyframes") {}
                else if (StringData=="hasKeyframes") {}
                else if (StringData=="hasVideo") {}
                else if (StringData=="stereo") {}
                else if (StringData=="canSeekToEnd") {}
                else if (StringData=="hasAudio") {}
                else if (StringData=="hasmetadata") {}
                else if (StringData=="hasMetadata") {}
                else if (StringData=="hasCuePoints") {}
                else {ToFill=StringData;}
                Fill(Stream_General, 0, ToFill.c_str(), Value?"Yes":"No");
            }
            break;
        case 0x02 : //SCRIPTDATASTRING
             {
                int32u Value_Size;
                GET_B2 (Value_Size,                             Value_Size)
                if (Value_Size)
                {
                    Ztring Value;
                    GET_UTF8(Value_Size, Value,                 Value)
                    std::string ToFill;
                         if (0) ;
                    else if (StringData=="creator") {ToFill="Encoded_Application";}
                    else if (StringData=="metadatacreator") {ToFill="Tagged_Application";}
                    else if (StringData=="creationdate") {ToFill="Encoded_Date"; Value.Date_From_String(Value.To_UTF8().c_str());}
                    else {ToFill=StringData;}
                    Fill(Stream_General, 0, ToFill.c_str(), Value);
                }
            }
            break;
        case 0x03 : //SCRIPTDATAOBJECT[n]
        case 0x10 : //Typed object - SCRIPTDATAOBJECT[n]
            {
                std::string StringData2;
                int16u StringLength2;
                meta_Level++;
                meta_LevelFinnished[meta_Level]=false;
                while (!meta_LevelFinnished[meta_Level])
                {
                    if (Stream_Pos>=Element_Size)
                        break;
                    FLUSH();
                    ELEMENT_BEGIN();
                    GET_B2 (StringLength2,                          StringLength2);
                    GET_STRING(StringLength2, StringData2,          StringData2);
                    ELEMENT(Details_Level_Last, StringData2.empty()?"EndOfObject":StringData2.c_str(), 0);
                    meta_SCRIPTDATAVALUE(Stream_Pos, StringData+'_'+StringData2);
                    ELEMENT_END();
                }
                meta_Level--;
            }
            break;
        case 0x04 : //SCRIPTDATASTRING defining the MovieClip path
            {
                int32u Value_Size;
                GET_B2 (Value_Size,                             Value_Size)
                if (Value_Size)
                {
                    Ztring Value;
                    GET_LOCAL(Value_Size, Value,                Value)
                    Fill(Stream_General, 0, StringData.c_str(), Value);
                }
            }
            break;
        case 0x05 : //NULL
        case 0x06 : //Undefined - NULL
        case 0x0D : //Unsupported - NULL
            break;
        case 0x07 : //UI16
            {
                int32u Value;
                GET_B2 (Value,                                  Value)
                Fill(Stream_General, 0, StringData.c_str(), Value);
            }
            break;
        case 0x08 : //SCRIPTDATAVARIABLE[ECMAArrayLength]
            {
                int32u ECMAArrayLength;
                GET_B4 (ECMAArrayLength,                        ECMAArrayLength)
                NAME (Ztring::ToZtring(ECMAArrayLength)+_T(" elements"));
                for (int32u Pos=0; Pos<ECMAArrayLength; Pos++)
                {
                    meta_SCRIPTDATAVARIABLE(Stream_Pos);
                }
            }
            break;
        case 0x09 :
            SKIP_LOCAL(0,                                       EndOfObject);
            meta_LevelFinnished[meta_Level]=true;
            break;
        case 0x0A : //SCRIPTDATAVARIABLE[n]
        case 0x0E : //RecordSet - SCRIPTDATAVARIABLE[n]
            {
                int32u Count;
                GET_B4 (Count,                                  Count);
                for (int32u Pos=0; Pos<Count; Pos++)
                    meta_SCRIPTDATAVALUE(Stream_Pos, StringData);
            }
            break;
        case 0x0B : //SCRIPTDATADATE
            {
                Ztring Value;
                double Value_Double;
                int8u Double0, Double1, Double2, Double3, Double4, Double5, Double6, Double7;
                GET_B1 (Double0,                                DoublePart1)
                GET_B1 (Double1,                                DoublePart2)
                GET_B1 (Double2,                                DoublePart3)
                GET_B1 (Double3,                                DoublePart4)
                GET_B1 (Double4,                                DoublePart5)
                GET_B1 (Double5,                                DoublePart6)
                GET_B1 (Double6,                                DoublePart7)
                GET_B1 (Double7,                                DoublePart8)
                int8u* Double=(int8u*)&Value_Double;
                Double[0]=Double7;
                Double[1]=Double6;
                Double[2]=Double5;
                Double[3]=Double4;
                Double[4]=Double3;
                Double[5]=Double2;
                Double[6]=Double1;
                Double[7]=Double0;
                Value.Date_From_Seconds_1970((int32u)Value_Double);
                INFO(Value);
                SKIP_B2(                                        Local_Offset_Minutes)
                std::string ToFill;
                     if (0) ;
                else if (StringData=="metadatadate") {ToFill="Tagged_Date";}
                else {ToFill=StringData;}
                Fill(Stream_General, 0, ToFill.c_str(), Value);
            }
            break;
        case 0x0C : //SCRIPTDATALONGSTRING
        case 0x0F : //XML - SCRIPTDATALONGSTRING
            {
                int32u Value_Size;
                GET_B4 (Value_Size,                             Value_Size)
                if (Value_Size)
                {
                    Ztring Value;
                    SKIP_UTF16B(Value_Size,                     Value)
                    std::string ToFill;
                         if (0) ;
                    else if (StringData=="creator") {ToFill="Encoded_Application";}
                    else if (StringData=="metadatacreator") {ToFill="Tagged_Application";}
                    else if (StringData=="creationdate") {ToFill="Encoded_Date"; Value.Date_From_String(Value.To_UTF8().c_str());}
                    else {ToFill=StringData;}
                    Fill(Stream_General, 0, ToFill.c_str(), Value);
                }
            }
            break;
        case 0x11 : //AMF3 data
            {
                int32u TypeCode;
                GET_B4 (TypeCode,                               AMF3 type code) if (TypeCode<0x0D) INFO(Flv_Amf3Type[TypeCode]);
                switch (TypeCode)
                {
                    case 0x00 : //undefined
                    case 0x01 : //null
                    case 0x02 : //boolean-false
                    case 0x03 : //boolean-true
                        break;
                    default : //Not implemented or unknown
                        Stream_Pos=Element_Size;
                }
            }
            break;
        default : //Unknown
            Stream_Pos=Element_Size; //Forcing the end of parsing
    }
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flv::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
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

} //NameSpace

#endif //MEDIAINFO_FLV_YES

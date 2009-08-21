// File_Swf - Info for SWF files
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
#if defined(MEDIAINFO_SWF_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Swf.h"
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
#include <zlib/zlib.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
int8u  Swf_Channels[]=
{
    1,
    2,
};

int8u  Swf_Resolution[]=
{
    8,
    16,
};

int16u Swf_SamplingRate[]=
{
    5500,
    11025,
    22050,
    44100,
};

const char* Swf_Codec_Audio[]=
{
    "Uncompressed",
    "ADPCM",
    "MPEG-1L3",
    "Uncompressed Little Endian",
    "",
    "",
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

const char* Swf_Codec_Video[]=
{
    "",
    "",
    "H263",
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

//***************************************************************************
// Functions
//***************************************************************************

//---------------------------------------------------------------------------
void File_Swf::Read_Buffer_Continue()
{
    //SWF buffer
    Buffer=File__Base::Buffer;
    Buffer_Size=File__Base::Buffer_Size;
    File_Offset=File__Base::File_Offset;

    //Header handling
    if (File_Offset==0)
    {
        if (Buffer_Size<20 || (CC3(Buffer)!=CC3("FWS") && CC3(Buffer)!=CC3("CWS")))
        {
            File__Base::File_Offset=File_Size;
            return;
        }
        //Compressed file handling
        if (CC3(Buffer)==CC3("CWS"))
        {
            if (!Decompress())
                return;
        }
        else
            Header();
    }
    if (Count_Get(Stream_General)==0)
        FINNISHED();

    //Parsing
    while (Buffer_Parse());

    //SWF buffer
    if (Buffer!=File__Base::Buffer)
        delete Buffer; //Buffer=NULL;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Swf::Buffer_Parse()
{
    if (Buffer_Offset+2>Buffer_Size)
        return false; //Not enough buffer

    //Element name
    Element_HeaderSize=2;
    Element_Size=4; //Maximum
    int16u TagCodeAndLength;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    CC_BEGIN();
    GET_L2 (TagCodeAndLength,                                   TagCodeAndLength);
    Element_Name=(TagCodeAndLength&0xFFC0)>>6; INFO(Element_Name);

    //Element size
    Element_Size=TagCodeAndLength&0x003F; INFO(Element_Size, " bytes");
    if (Element_Size==0x3F) //All 1
    {
        Element_HeaderSize=6;
        if (Buffer_Offset+6>Buffer_Size)
            return false; //Not enough buffer
        GET_L4(Element_Size,                                    Length); INFO(Element_Size, " bytes");
    }
    Element_Next=File_Offset+Buffer_Offset+Element_HeaderSize+Element_Size;

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);

    return true;
}

//---------------------------------------------------------------------------
bool File_Swf::Element_Parse()
{
    //Details
    ELEMENT(0, Element_Name, Element_Size);

    //Positionning
    Buffer_Offset+=Element_HeaderSize;

    //Parsing
    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case Swf::_NAME : NAME(_DETAIL); _NAME(); break;

    switch (Element_Name)
    {
        ELEMENT_CASE(End,                       "End");
        ELEMENT_CASE(ShowFrame,                 "ShowFrame");
        ELEMENT_CASE(DefineShape,               "DefineShape");
        ELEMENT_CASE(PlaceObject,               "PlaceObject");
        ELEMENT_CASE(RemoveObject,              "RemoveObject");
        ELEMENT_CASE(DefineBits,                "DefineBits");
        ELEMENT_CASE(DefineButton,              "DefineButton");
        ELEMENT_CASE(JPEGTables,                "JPEGTables");
        ELEMENT_CASE(SetBackgroundColor,        "SetBackgroundColor");
        ELEMENT_CASE(DefineFont,                "DefineFont");
        ELEMENT_CASE(DefineText,                "DefineText");
        ELEMENT_CASE(DoAction,                  "DoAction");
        ELEMENT_CASE(DefineFontInfo,            "DefineFontInfo");
        ELEMENT_CASE(DefineSound,               "DefineSound");
        ELEMENT_CASE(StartSound,                "StartSound");
        ELEMENT_CASE(DefineButtonSound,         "DefineButtonSound");
        ELEMENT_CASE(SoundStreamHead,           "SoundStreamHead");
        ELEMENT_CASE(SoundStreamBlock,          "SoundStreamBlock");
        ELEMENT_CASE(DefineBitsLossless,        "DefineBitsLossless");
        ELEMENT_CASE(DefineBitsJPEG2,           "DefineBitsJPEG2");
        ELEMENT_CASE(DefineShape2,              "DefineShape2");
        ELEMENT_CASE(DefineCxform,              "DefineCxform");
        ELEMENT_CASE(Protected,                 "Protected");
        ELEMENT_CASE(PlaceObject2,              "PlaceObject2");
        ELEMENT_CASE(RemoveObject2,             "RemoveObject2");
        ELEMENT_CASE(DefineShape3,              "DefineShape3");
        ELEMENT_CASE(DefineText2,               "DefineText2");
        ELEMENT_CASE(DefineButton2,             "DefineButton2");
        ELEMENT_CASE(DefineBitsJPEG3,           "DefineBitsJPEG3");
        ELEMENT_CASE(DefineBitsLossless2,       "DefineBitsLossless2");
        ELEMENT_CASE(DefineEditText,            "DefineEditText");
        ELEMENT_CASE(DefineSprite,              "DefineSprite");
        ELEMENT_CASE(FrameLabel,                "FrameLabel");
        ELEMENT_CASE(DefineMorphShape,          "DefineMorphShape");
        ELEMENT_CASE(SoundStreamHead2,          "SoundStreamHead2");
        ELEMENT_CASE(DefineFont2,               "DefineFont2");
        ELEMENT_CASE(ExportAssets,              "ExportAssets");
        ELEMENT_CASE(ImportAssets,              "ImportAssets");
        ELEMENT_CASE(EnableDebugger,            "EnableDebugger");
        ELEMENT_CASE(DoInitAction,              "DoInitAction");
        ELEMENT_CASE(DefineVideoStream,         "DefineVideoStream");
        ELEMENT_CASE(DefineVideoFrame,          "DefineVideoFrame");
        ELEMENT_CASE(DefineFontInfo2,           "DefineFontInfo2");
        ELEMENT_CASE(EnableDebugger2,           "EnableDebugger2");
        ELEMENT_CASE(ScriptLimits,              "ScriptLimits");
        ELEMENT_CASE(SetTabIndex,               "SetTabIndex");
        ELEMENT_CASE(FileAttributes,            "FileAttributes");
        ELEMENT_CASE(PlaceObject3,              "PlaceObject3");
        ELEMENT_CASE(ImportAssets2,             "ImportAssets2");
        ELEMENT_CASE(DefineFontAlignZones,      "DefineFontAlignZones");
        ELEMENT_CASE(CSMTextSettings,           "CSMTextSettings");
        ELEMENT_CASE(DefineFont3,               "DefineFont3");
        ELEMENT_CASE(Metadata,                  "Metadata");
        ELEMENT_CASE(DefineScalingGrid,         "DefineScalingGrid");
        ELEMENT_CASE(DefineShape4,              "DefineShape4");
        ELEMENT_CASE(DefineMorphShape2,         "DefineMorphShape2");
        default : ;
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Swf::Header()
{
    //Parsing
    Element_Size=8;
    CC_BEGIN();
    SKIP_C3(                                                    Signature);
    GET_L1 (Version,                                            Version);
    GET_L4 (FileLength,                                         FileLength);
    CC_END();

    //The rest of the header
    Buffer_Offset+=8;
    Header_Continue();

    ELEMENT(0, "SWF header", Element_Size);
    FLUSH();
}

//---------------------------------------------------------------------------
void File_Swf::Header_Continue()
{
    //If we are here, we are sure this is SWF
    if (Count_Get(Stream_General)==0) //May be done in Decompress()
        Stream_Prepare(Stream_General);
    Fill("Format", "SWF");
    Stream_Prepare(Stream_Video);

    //Parsing - BitStream
    Element_Size=(int32u)(File_Size-8); //Impossible to know the size before parsing
    BS_BEGIN();
    int32u Nbits, Xmin, Xmax, Ymin, Ymax, FrameRate, FrameCount;
    GET_BS (5, Nbits,                                           Nbits);
    GET_BS (Nbits, Xmin,                                        Xmin);
    GET_BS (Nbits, Xmax,                                        Xmax); INFO((Xmax-Xmin)/20, " pixels");
    GET_BS (Nbits, Ymin,                                        Ymin);
    GET_BS (Nbits, Ymax,                                        Ymax); INFO((Ymax-Ymin)/20, " pixels");

    //Parsing - Integers
    if (File_Size<8+BS.Offset_Get()+4)
        return;
    Buffer_Offset+=BS.Offset_Get();
    Element_Size=4;
    CC_BEGIN();
    SKIP_L1(                                                    Ignored);
    GET_L1 (FrameRate,                                          FrameRate);
    GET_L2 (FrameCount,                                         FrameCount);
    CC_END();

    //Filling
    Fill("Width", (Xmax-Xmin)/20);
    Fill("Height", (Ymax-Ymin)/20);
    Fill("FrameRate", FrameRate);
    //Fill("FrameCount", FrameCount);

    //Settings
    Buffer_Offset+=4;
    Element_Size=8+BS.Offset_Get()+4;
}

//---------------------------------------------------------------------------
void File_Swf::DefineSound()
{
    //Parsing
    int32u ID, Channels, Resolution, SamplingRate, Codec;
    CC_BEGIN();
    GET_L2 (ID,                                                 SoundId);
    CC_BS_BEGIN();
    GET_BS (4, Codec,                                           SoundFormat); INFO(Swf_Codec_Audio[Codec]);
    GET_BS (2, SamplingRate,                                    SoundRate); INFO(Swf_SamplingRate[SamplingRate], " Hz");
    GET_BS (1, Resolution,                                      SoundSize); INFO(Swf_Resolution[Resolution], " bits");
    GET_BS (1, Channels,                                        SoundType); INFO(Swf_Channels[Channels], " channel(s)");
    CC_BS_END();
    SKIP_L4(                                                    SoundSampleCount);
    SKIP_XX(Element_Size-Stream_Pos,                            SoundData);
    CC_END();

    Stream_Prepare(Stream_Audio);
    Fill("ID", ID);
    Fill("Channel(s)", Swf_Channels[Channels]);
    Fill("Resolution", Swf_Resolution[Resolution]);
    Fill("SamplingRate", Swf_SamplingRate[SamplingRate]);
    Fill("Codec", Swf_Codec_Audio[Codec]);
}

//---------------------------------------------------------------------------
void File_Swf::SoundStreamHead()
{
    //Parsing
    int32u Channels, Resolution, SamplingRate, Codec;
    CC_BEGIN();
    CC_BS_BEGIN();
    SKIP_BS(4,                                                  Reserved);
    GET_BS (2, SamplingRate,                                    PlaybackSoundRate); INFO(Swf_SamplingRate[SamplingRate], " Hz");
    GET_BS (1, Resolution,                                      PlaybackSoundSize); INFO(Swf_Resolution[Resolution], " bits");
    GET_BS (1, Channels,                                        PlaybackSoundType); INFO(Swf_Channels[Channels], " channel(s)");
    GET_BS (4, Codec,                                           StreamSoundCompression); INFO(Swf_Codec_Audio[Codec]);
    GET_BS (2, SamplingRate,                                    StreamSoundRate); INFO(Swf_SamplingRate[SamplingRate], " Hz");
    GET_BS (1, Resolution,                                      StreamSoundSize); INFO(Swf_Resolution[Resolution], " bits");
    GET_BS (1, Channels,                                        StreamSoundType); INFO(Swf_Channels[Channels], " channel(s)");
    CC_BS_END();
    SKIP_L2(                                                    StreamSoundSampleCount);
    if (Codec==2)
        SKIP_L2(                                                LatencySeek);
    CC_END();

    Stream_Prepare(Stream_Audio);
    Fill("Channel(s)", Swf_Channels[Channels]);
    Fill("Resolution", Swf_Resolution[Resolution]);
    Fill("SamplingRate", Swf_SamplingRate[SamplingRate]);
    Fill("Codec", Swf_Codec_Audio[Codec]);
}

//---------------------------------------------------------------------------
void File_Swf::SoundStreamHead2()
{
    SoundStreamHead();
}

//---------------------------------------------------------------------------
void File_Swf::DefineVideoStream()
{
    //Parsing
    int32u ID, Width, Height, Codec;
    CC_BEGIN();
    GET_L2 (ID,                                                 CharacterID);
    SKIP_L2(                                                    NumFrames);
    GET_L2 (Width,                                              Width);
    GET_L2 (Height,                                             Height);
    CC_BS_BEGIN();
    SKIP_BS(4,                                                  VideoFlagsReserved);
    SKIP_BS(3,                                                  VideoFlagsDeblocking);
    SKIP_BS(1,                                                  VideoFlagsSmoothing);
    CC_BS_END();
    GET_L1 (Codec,                                              CodecID);
    if (ID>=16)
        ID=0; //Should never happen (FLV is only 4-bit sized)
    CC_END();

    Stream_Prepare(Stream_Video);
    Fill("ID", ID);
    Fill("Width", Width);
    Fill("Height", Height);
    Fill("Codec", Swf_Codec_Video[Codec]);
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Swf::Decompress()
{
    if (Buffer_Size!=File_Size)
    {
        //We must have the complete file in memory
        Stream_Prepare(Stream_General); //Say "OK, this is SWF, need more bytes"
        return false;
    }

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    Element_Size=8;
    CC_BEGIN();
    SKIP_C3(                                                    Signature);
    GET_L1 (Version,                                            Version);
    GET_L4 (FileLength,                                         FileLength);
    CC_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Sizes
    unsigned long Source_Size=(unsigned long)(File_Size-8);
    unsigned long Dest_Size=(unsigned long)(FileLength-8);

    //Uncompressing
    int8u* Dest=new int8u[Dest_Size];
    if (uncompress((Bytef*)Dest, &Dest_Size, (const Bytef*)Buffer+Buffer_Offset+8, Source_Size)<0)
    {
        delete[] Dest; //Dest=NULL
        TRUSTED_ISNOT("Error while decompressing");
        FLUSH();
        File__Base::File_Offset=File_Size;
        return false;
    }

    //Configuring
    Buffer=Dest;
    Buffer_Size=(size_t)Dest_Size;
    File_Offset=8;

    //The rest of the header
    Header_Continue();

    //Filling
    ELEMENT(0, "Compressed SWF header", Element_Size);
    FLUSH();

    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Swf::HowTo(stream_t StreamKind)
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

#endif //MEDIAINFO_AAC_*


// File_Dts - Info for DTS files
// Copyright (C) 2004-2007 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_DTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Dts.h"
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  DTS_FrameType[]=
{
    "Normal",
    "Termination",
};

const size_t DTS_SamplingRate[]=
{
        0,  8000,  16000,  32000,      0,      0,  11025,  22050,
    44100,     0,      0,  12000,  24000,  48000,  96000, 192000,
};

const size_t DTS_BitRate[]=
{
      32000,   56000,   64000,   96000,  112000,  128000,  192000,  224000,
     256000,  320000,  384000,  448000,  512000,  576000,  640000,  768000,
     960000, 1024000, 1152000, 1280000, 1344000, 1408000, 1411200, 1472000,
    1536000, 1920000, 2048000, 3072000, 3840000,       0,       0,       0,
};

const size_t DTS_Channels[]=
{
    1, 2, 2, 2, 2, 3, 3, 4,
    4, 5, 6, 6, 6, 7, 8, 8,
};

const size_t DTS_Resolution[]=
{16, 20, 24, 24};

const char*  DTS_ChannelPositions[]=
{
    "Mono",
    "Dual mono",
    "L R",
    "L R",
    "L R",
    "L C R",
    "Front: L R, Surround: C",
    "Front: L C R, Surround: C",
    "Front: L R, Surround: L R",
    "Front: L C R, Surround: L R",
    "Front: L C C R, Surround: L R",
    "Front: L C R, Rear: L R",
    "Front: L R, Middle: L R, Rear: L R",
    "Front: L C R, Middle: L R, surround: L R",
    "Front: L R, Middle: L R, Surround: L C C R",
    "Front: L C R, Middle: L R, Surround: L C R",
};

const char*  DTS_ExtensionAudioDescriptor[]=
{
    "Channel Extension",
    "Reserved",
    "Frequency Extension",
    "Channel Extension + Frequency Extension",
    "",
    "",
    "",
    "",
    "",
    "",
};
//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dts::Read_Buffer_Init()
{
    //Configuration
    File_MaximumOffset=64*1024;
    Frame_Count_Valid=8;

    //External info
    Delay=0;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
    Word=true;
    BigEndian=true;

    //Temp - Unknown
    Unknown_Size=Error;
}

//---------------------------------------------------------------------------
void File_Dts::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonDTS())
    {
        File_Offset=File_Size;
        return;
    }

    //Trusted limitation
    Trusted=Buffer_Size>2*(8*1024)?Buffer_Size/(8*1024):2; //Never less than 2 acceptable errors

    //Buffer
    Element_Next=File_Offset;

    //Parsing
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dts::Buffer_Parse()
{
    //Search for header
    if (!NextFrame())
        return false; //Header not found

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
bool File_Dts::Element_Parse()
{
    //Configuring
    if (Frame_Count==0)
    {
        if (0) ;
        else if (BigEndian2int16u   (Buffer+Buffer_Offset)==0x7FFE) //16 bits and big    endian bitstream
        {
        }
        else if (LittleEndian2int16u(Buffer+Buffer_Offset)==0x7FFE) //16 bits and little endian bitstream
        {
            BigEndian=false;
        }
        else if (BigEndian2int16u   (Buffer+Buffer_Offset)==0x1FFF) //14 bits and big    endian bitstream
        {
            Word=false;
        }
        else if (LittleEndian2int16u(Buffer+Buffer_Offset)==0x1FFF) //14 bits and little endian bitstream
        {
            Word=false;
            BigEndian=false;
        }

        //14-bits and Little endian are not yet supported
        if (!Word || !BigEndian)
        {
            File_Offset=File_Size;
            return false;
        }
    }

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    if (Element_Size<20 && File_Offset+Buffer_Size==File_Size) //Uncompleted frame management
    {
        ELEMENT(0, "Frame (uncomplete)", Element_Size);
        if (Count_Get(Stream_Audio)==0)
            Frame_Fill();
        FLUSH();
    }
    int32u crc_present, EncoderSoftwareRevision;
    BS_BEGIN();
    SKIP_BS(32,                                                 Sync);
    INFO_BS( 1, FrameType,                                      Frame Type); INFO(DTS_FrameType[FrameType]);
    SKIP_BS( 5,                                                 Deficit Sample Count);
    GET_BS ( 1, crc_present,                                    CRC Present);
    SKIP_BS( 7,                                                 Number of PCM Sample Blocks);
    GET_BS (14, Element_Size,                                   Primary Frame Byte Size minus 1);
    Element_Size+=1;
    if (!Word) Element_Size=Element_Size*8/14*2; INFO(Ztring::ToZtring(Element_Size)+_T(" bytes")); //Word is on 14 bits!
    GET_BS ( 6, channel_arrangement,                            Audio Channel Arrangement); INFO(Ztring::ToZtring(DTS_Channels[channel_arrangement])+_T(" channels"));
    GET_BS ( 4, sample_frequency,                               Core Audio Sampling Frequency); INFO(Ztring::ToZtring(DTS_SamplingRate[sample_frequency])+_T(" Hz"));
    GET_BS ( 5, bit_rate,                                       Transmission Bit Rate); INFO(Ztring::ToZtring(DTS_BitRate[bit_rate])+_T(" bps"));
    SKIP_BS( 1,                                                 Embedded Down Mix Enabled);
    SKIP_BS( 1,                                                 Embedded Dynamic Range);
    SKIP_BS( 1,                                                 Embedded Time Stamp);
    SKIP_BS( 1,                                                 Auxiliary Data);
    SKIP_BS( 1,                                                 HDCD);
    GET_BS ( 3, ExtensionAudioDescriptor,                       Extension Audio Descriptor); INFO(DTS_ExtensionAudioDescriptor[ExtensionAudioDescriptor]);
    GET_BS ( 1, ExtendedCoding,                                 Extended Coding);
    SKIP_BS( 1,                                                 Audio Sync Word Insertion);
    GET_BS ( 2, lfe_effects,                                    Low Frequency Effects);
    SKIP_BS( 1,                                                 Predictor History);
    if (crc_present)
        SKIP_BS(16,                                             Header CRC Check);
    SKIP_BS( 1,                                                 Multirate Interpolator);
    GET_BS ( 4, EncoderSoftwareRevision,                        Encoder Software Revision);
    SKIP_BS( 2,                                                 Copy History);
    GET_BS ( 2, bits_per_sample,                                Source PCM Resolution); INFO(Ztring::ToZtring(DTS_Resolution[bits_per_sample])+_T(" bits"));
    SKIP_BS( 1,                                                 ES);
    SKIP_BS( 1,                                                 Front Sum/Difference);
    SKIP_BS( 1,                                                 Surrounds Sum/Difference);
    SKIP_BS( 4,                                                 Dialog Normalisation Parameter);
    switch (EncoderSoftwareRevision)
    {
        case 6 :
                SKIP_BS( 4,                                     Dialog Normalisation Parameter);
                break;
        case 7 :
                SKIP_BS( 4,                                     Dialog Normalisation Parameter);
                break;
        default :
                SKIP_BS( 4,                                     Unspecified);
                break;
    }

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Element size
    Element_HeaderSize=BS.Offset_Get();
    if (!Element_Size_Get())
        return false;

    //Element
    ELEMENT(0, "Frame", Element_Size);

    //Positioning
    Buffer_Offset+=BS.Offset_Get();

    //Parsing
    Frame();

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dts::Frame()
{
    //Counting
    if (Element_Next>=File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //Parsing
    CC_BEGIN()
    SKIP_XX(Element_Size,                                       Data);
    CC_END();

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Frame_Fill();
}

//---------------------------------------------------------------------------
void File_Dts::Frame_Fill()
{
    Stream_Prepare(Stream_General);
    Fill("Format", "DTS");
    Stream_Prepare(Stream_Audio);

    if (Unknown_Size==Error) //TODO : Find a better way
        Fill("Codec", "DTS");
    else
        Fill("Codec", "DTS-HD");

    if (ExtendedCoding && (ExtensionAudioDescriptor==2 || ExtensionAudioDescriptor==3))
        Fill("SamplingRate", 96000);
    else
        Fill("SamplingRate", DTS_SamplingRate[sample_frequency]);
    if (bit_rate<29)
        Fill("BitRate", DTS_BitRate[bit_rate]);
    else if (bit_rate==29)
        Fill("BitRate", "Open");
    else if (bit_rate==30)
        Fill("BitRate", "Variable");
    else if (bit_rate==31)
        Fill("BitRate", "LossLess");
    if (ExtendedCoding && (ExtensionAudioDescriptor==0 || ExtensionAudioDescriptor==3))
    {
    }
    else
    {
        int8u Channels;
        Ztring ChannelPositions;
        if (channel_arrangement<16)
        {
            Channels=DTS_Channels[channel_arrangement]+(lfe_effects?1:0);
            ChannelPositions.From_Local(DTS_ChannelPositions[channel_arrangement]);
        }
        else
        {
            Channels=8;
            ChannelPositions.From_Local("User defined");
        }
        if (lfe_effects)
            ChannelPositions+=_T(", Subwoofer");
        Fill("Channel(s)", Channels);
        Fill("ChannelPositions", ChannelPositions);
    }
    Fill("Resolution", DTS_Resolution[bits_per_sample]);

    if (File_Offset+Buffer_Size<File_Size)
    {
        //Details
        Buffer_Offset=(size_t)(Element_Next-File_Offset);
        if (Config.Details_Get())
        {
            Details_Add_Error("---------------------------------------");
            Details_Add_Error("---   DTS, Jumping to end of file   ---");
            Details_Add_Error("---------------------------------------");
        }
        //No more need data
        File_Offset=File_Size;
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Dts::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+6<=Buffer_Size
      && !(BigEndian2int16u   (Buffer+Buffer_Offset_Temp+0)==0x7FFE && BigEndian2int16u   (Buffer+Buffer_Offset_Temp+2)==0x8001) //16 bits and big    endian bitstream
      && !(LittleEndian2int16u(Buffer+Buffer_Offset_Temp+0)==0x7FFE && LittleEndian2int16u(Buffer+Buffer_Offset_Temp+2)==0x8001) //16 bits and little endian bitstream
      && !(BigEndian2int16u   (Buffer+Buffer_Offset_Temp+0)==0x1FFF && BigEndian2int16u   (Buffer+Buffer_Offset_Temp+2)==0xE800 && (BigEndian2int16u   (Buffer+Buffer_Offset_Temp+4)&0xFFF0)==0x07F0) //14 bits and big    endian bitstream
      && !(LittleEndian2int16u(Buffer+Buffer_Offset_Temp+0)==0x1FFF && LittleEndian2int16u(Buffer+Buffer_Offset_Temp+2)==0xE800 && (LittleEndian2int16u(Buffer+Buffer_Offset_Temp+4)&0xFFF0)==0x07F0) //14 bits and little endian bitstream
      )
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+6>Buffer_Size)
    {
        if (Buffer_Offset_Temp+5==Buffer_Size)
        {
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;

        //Delay handling
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+6>Buffer_Size)
        return false;

    //Error in stream?
    if (Buffer_Offset_Temp-Buffer_Offset>0)
    {
        //Some strange data was seen on 1 DTS stream expected to be 6.1 DTS-HD Master Lossless Audio
        if (Synched && (Unknown_Size==Error || Unknown_Size==Buffer_Offset_Temp-Buffer_Offset) && CC4(Buffer+Buffer_Offset)==0x64582025)
        {
            Synched=false;
            Unknown_Size=Buffer_Offset_Temp-Buffer_Offset;
        }

        //Synch
        if (Synched)
            TRUSTED_ISNOT("Sync error", 0, Buffer_Offset_Temp-Buffer_Offset);
        else
        {
            ELEMENT(0, "Synchronization", Buffer_Offset_Temp-Buffer_Offset);
            FLUSH();
        }
    }

    //OK, we continue
    Buffer_Offset=Buffer_Offset_Temp;
    Buffer_Offset_Temp=0;
    Synched=true;
    if (Frame_Count==0)
        Delay+=Buffer_Offset;
    return true;
}

//---------------------------------------------------------------------------
bool File_Dts::Element_Size_Get()
{
    //Uncompleted frame management
    if (Buffer_Offset+Element_Size>Buffer_Size)
    {
        if (File_Offset+Buffer_Size==File_Size)
        {
            ELEMENT(0, "Frame (Uncomplete)", Element_Size-Element_HeaderSize);
            if (Count_Get(Stream_Audio)==0)
                Frame_Fill();
            FLUSH();
        }
        return false;
    }

    //OK, we continue
    Element_Next=File_Offset+Buffer_Offset+Element_Size;
    Element_Size-=Element_HeaderSize;
    return true;
}

//---------------------------------------------------------------------------
bool File_Dts::Detect_NonDTS ()
{
    //Element_Size
    if (Buffer_Size<4)
    {
        File_Offset=File_Size;
        return true;
    }

    //Detect WAV files, the parser can't detect it easily, there is only 70 bytes of begining for sayint WAV
    if (CC4(Buffer)==CC4("RIFF"))
    {
        File_Offset=File_Size;
        return true;
    }

    //Seems OK
    return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dts::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("ChannelPositions", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Resolution", "N");
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

#endif //MEDIAINFO_DTS_YES


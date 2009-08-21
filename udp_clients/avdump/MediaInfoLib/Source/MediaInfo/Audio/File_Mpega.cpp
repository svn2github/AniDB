// File_Mpega - Info for MPEG Audio files
// Copyright (C) 2002-2007 Jerome Martinez, Zen@MediaArea.net
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
// A good start : http://www.codeproject.com/audio/MPEGAudioInfo.asp
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGA_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
const char* Mpega_Version[4]=
{
    "MPA2.5",
    "",
    "MPA2",
    "MPA1"
};

const char* Mpega_Version_String[4]=
{
    "MPEG-2.5 Audio",
    "",
    "MPEG-2 Audio",
    "MPEG-1 Audio",
};

const char* Mpega_Layer[4]=
{
    "",
    "L3",
    "L2",
    "L1",
};

const char* Mpega_Layer_String[4]=
{
    "",
    " layer 3",
    " layer 2",
    " layer 1",
};

const int16u Mpega_BitRate[4][4][16]=
{
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 2.5 layer X
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2.5 layer 3
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2.5 layer 2
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256,   0}}, //MPEG Audio 2.5 layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer X
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer 3
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio X layer 2
     {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}}, //MPEG Audio X layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 2 layer X
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2 layer 3
     {0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160,   0},  //MPEG Audio 2 layer 2
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256,   0}}, //MPEG Audio 2 layer 1
    {{0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},  //MPEG Audio 1 layer X
     {0,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320,   0},  //MPEG Audio 1 layer 3
     {0,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384,   0},  //MPEG Audio 1 layer 2
     {0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448,   0}}, //MPEG Audio 1 layer 1
};

const int16u Mpega_SamplingRate[4][4]=
{
    {11025, 12000,  8000, 0}, //MPEG Audio 2.5
    {    0,     0,     0, 0}, //MPEG Audio X
    {22050, 24000, 16000, 0}, //MPEG Audio 2
    {44100, 48000, 32000, 0}, //MPEG Audio 1
};

const int16u Mpega_Channels[4]=
{
    2,
    2,
    2,
    1,
};

const char* Mpega_Codec_Profile[4]=
{
    "",
    "Joint stereo",
    "Dual mono",
    "",
};

const char* Mpega_Codec_Profile_Extension[]=
{
    "",
    "Intensity Stereo",
    "MS Stereo",
    "Intensity Stereo + MS Stereo",
};

const char* Mpega_Emphasis[]=
{
    "",
    "50/15ms",
    "Reserved",
    "CCITT",
};

const char* Lame_BitRate_Mode[]=
{
    "",
    "CBR",
    "VBR",
    "VBR",
    "VBR",
    "VBR",
    "VBR",
    "",
    "CBR",
    "VBR",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char* Lame_Method[]=
{
    "",
    "CBR",
    "ABR",
    "VBR (rh)",
    "VBR (mtrh)",
    "VBR (rh)",
    "VBR",
    "",
    "CBR (2-pass)",
    "ABR (2-pass)",
    "",
    "",
    "",
    "",
    "",
    "",
};


const int8u Mpega_Coefficient[4][4] = //Samples per Frame / 8
{
    {  0,  72, 144,  12}, //MPEG Audio 2.5
    {  0,   0,   0,   0}, //MPEG Audio X
    {  0,  72, 144,  12}, //MPEG Audio 2
    {  0, 144, 144,  12}, //MPEG Audio 1
};

const int8u Mpega_SlotSize[4]= //A frame is coposed of slots
{
    0, // Layer X
    1, // Layer3
    1, // Layer2
    4, // Layer1
};                       
//---------------------------------------------------------------------------
} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//***************************************************************************
//
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Mpega.h"
#include <ZenLib/BitStream.h>
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpega::File_Mpega()
: File__Tags_Helper(this)
{
    //Configuration
    File_MaximumOffset=64*1024;
    Frame_Count_Valid=8;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpega::Read_Buffer_Init()
{
    //External info
    Delay=0;

    //Buffer
    Element_Size=0;

    //Temp - Global
    Frame_Count=0;

    //Temp - BitStream info
    Block_Count[0]=0;
    Block_Count[1]=0;
    Block_Count[2]=0;
    Channels_Count[0]=0;
    Channels_Count[1]=0;
    Channels_Count[2]=0;
    Channels_Count[3]=0;
    Extension_Count[0]=0;
    Extension_Count[1]=0;
    Extension_Count[2]=0;
    Extension_Count[3]=0;
    Emphasis_Count[0]=0;
    Emphasis_Count[1]=0;
    Emphasis_Count[2]=0;
    Emphasis_Count[3]=0;
    Scfsi=0;
    Scalefac=0;
    Reservoir=0;
    Reservoir_Max=0;

    //Temp - VBR handling
    VBR_Frames=0;
    VBR_FileSize=0;
    BitRate_Mode.clear();

    //Temp - Tags in a frame
    Encoded_Library.clear();
    Encoded_Library_Settings.clear();
}

//---------------------------------------------------------------------------
void File_Mpega::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEGA())
        FINNISHED();

    //Tags
    if (!TagSizeIsFinal && !DetectBeginOfEndTags())
        return;

    //Trusted limitation
    Trusted=Buffer_Size>2*(16*1024)?Buffer_Size/(16*1024):2; //Never less than 2 acceptable errors

    //Buffer
    Element_Next=File_Offset;

    //Parsing
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Mpega::Read_Buffer_Finalize()
{
    if (Count_Get(Stream_Audio)==0)
        return;

    //VBR detection without header
    if (VBR_Frames==0)
    {
        //How much kinds of bitrates?
        if (BitRate_Count.size()>1)
            BitRate_Mode=_T("VBR");
    }

    //VBR header
    if (VBR_Frames==0 && BitRate_Mode!=_T("VBR"))
    {
        BitRate=Mpega_BitRate[Mpeg][Layer][BitRate]*1000;
        BitRate_Mode=_T("CBR");
    }
    else if (VBR_Frames>0)
    {
        float32 FrameLength=(VBR_FileSize?VBR_FileSize:File_Size-File_EndTagSize-File_BeginTagSize)/VBR_Frames;
        size_t Divider;
        if (Mpeg==3 && Layer==3) //MPEG 1 Layer 1
             Divider=384/8;
        else if ((Mpeg==2 || Mpeg==0) && Layer==1) //MPEG 2 or 2.5 Layer 3
            Divider=576/8;
        else
            Divider=1152/8;
        BitRate=(size_t)(FrameLength*Mpega_SamplingRate[Mpeg][SamplingRate]/Divider);
        BitRate_Mode=_T("VBR");
    }
    else
        BitRate=0;

    //Bitrate VBR
    if (BitRate_Count.size()>1)
    {
        Ztring BitRate_VBR;
        if (!BitRate_VBR.empty())
            BitRate_VBR+=_T(' ');
        BitRate_VBR+=Ztring::ToZtring(8);
        BitRate_VBR+=_T(':');
        BitRate_VBR+=Ztring::ToZtring(BitRate_Count[8]);
        //Fill("BitRate_VBR", Ztring::ToZtring(BitRate_Count[8]));
    }

    //Reservoir
    //Fill("Reservoir_Avg", Reservoir/Frame_Count);
    //Fill("Reservoir_Max", Reservoir_Max);
    //size_t Granules=(Mpeg==3?2:1);
    //size_t Ch=Mpega_Channels[Channels];
    //Fill("Scalefactors", Ztring::ToZtring(Scalefac*100/(Granules*Ch*Frame_Count))+_T('%'));

    //Filling
    if (File_Size!=0 && VBR_FileSize==0)
    {
        //We calculate VBR_FileSize from File_Size
        VBR_FileSize=File_Size;
        VBR_FileSize-=File_BeginTagSize;
        VBR_FileSize-=File_EndTagSize;
    }
    if (BitRate>0 && !File_Name.empty())
        Fill(Stream_General, 0, "PlayTime", VBR_FileSize*8*1000/BitRate);
    Fill(Stream_General, 0, "Encoded_Library", Encoded_Library);
    if (BitRate>0)
        Fill(Stream_Audio, 0, "BitRate", BitRate);
    Fill(Stream_Audio, 0, "BitRate_Mode", BitRate_Mode);
    if (Encoded_Library.empty())
        Encoded_Library_Guess();
    Fill(Stream_Audio, 0, "Encoded_Library", Encoded_Library);
    Fill(Stream_Audio, 0, "Encoded_Library_Settings", Encoded_Library_Settings);

    //Tags
    Tags_Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpega::Buffer_Parse()
{
    //Tags management
    if (!Tags_Read_Buffer_Continue())
        return false;

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
bool File_Mpega::Element_Parse()
{
    //Parsing the header
    if (!Element_Parse_Header())
        return false;

    //Element size verification
    if (Element_Size==0)
    {
        //Error in header, look for next header
        if (Synched && Frame_Count>1)
        {
            ELEMENT(0, "Frame - Sync error", Buffer_Offset_Temp-Buffer_Offset);
            TRUSTED_ISNOT("Sync error", 0, Buffer_Offset_Temp-Buffer_Offset);
        }
        else
        {
            ELEMENT(0, "Frame - Bad Synchro", 1);
            FLUSH();
        }
        Synched=false;
        Element_Next=File_Offset+Buffer_Offset+1;
        Buffer_Offset_Temp=Buffer_Offset+1;
        return true;
    }
    Synched=true;

    //Element
    Element_Size-=4;
    ELEMENT(0, "Frame", Element_Size);

    //Positionning
    Buffer_Offset+=4; //Header

    //Parsing
    Frame();
    FLUSH();

    return true;
}

//---------------------------------------------------------------------------
bool File_Mpega::Element_Parse_Header()
{
    //Enough buffer?
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=4;
    ELEMENT_END(); //Details_Level_Last=0; //Will be setted later
    BS_BEGIN();
    SKIP_BS(11,                                                 syncword);
    GET_BS (2, Mpeg,                                            ID); INFO(Mpega_Version[Mpeg]);
    GET_BS (2, Layer,                                           layer); INFO(Mpega_Layer[Layer]);
    GET_BS (1, Protection,                                      protection_bit);
    GET_BS (4, BitRate,                                         bitrate_index); INFO(Mpega_BitRate[Mpeg][Layer][BitRate], " Kbps");
    GET_BS (2, SamplingRate,                                    sampling_frequency); INFO(Mpega_SamplingRate[Mpeg][SamplingRate], " Hz");
    GET_BS (1, Padding,                                         padding_bit);
    GET_BS (1, Private,                                         private_bit);
    GET_BS (2, Channels,                                        mode); INFO(Mpega_Channels[Channels], " channels"); INFO(Mpega_Codec_Profile[Channels]);
    GET_BS (2, Extension,                                       mode_extension); INFO(Mpega_Codec_Profile_Extension[Extension]);
    GET_BS (1, Copyright,                                       copyright);
    GET_BS (1, Original,                                        original);
    GET_BS (2, Emphasis,                                        emphasis); INFO(Mpega_Emphasis[Emphasis]);
    BS_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Getting size
    if (!Element_Size_Get())
        return false;

    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpega::Frame()
{
    //VBR and library headers
    if (Frame_Count<3) //No need to do it too much
    {
        if (Header_Xing()) return;
        if (Header_VBRI()) return;
    }
    //Counting
    if (Element_Next==File_Size || Element_Next==File_Size-128)
        Frame_Count_Valid=Frame_Count; //Finalize MPEG Audio frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //LAME
    if (Encoded_Library.empty() && Frame_Count<Frame_Count_Valid) //Can be elsewhere...
        Header_Encoders();

    //Parsing
    BitRate_Count[Mpega_BitRate[Mpeg][Layer][BitRate]]++;
    Channels_Count[Channels]++;
    Extension_Count[Extension]++;
    Emphasis_Count[Emphasis]++;
    BS_BEGIN();
    if (Protection==0)
        SKIP_BS(16,                                             crc_check);
    int32u main_data_end;
    if (Mpeg==3) //MPEG-1
        GET_BS (9, main_data_end,                               main_data_end)
    else
        GET_BS (8, main_data_end,                               main_data_end);
    if (main_data_end>Reservoir_Max)
        Reservoir_Max=main_data_end;
    Reservoir+=main_data_end;
    if (Mpeg==3) //MPEG-1
    {
        if (Channels==3) //Mono
            SKIP_BS(5,                                          private_bits)
        else
            SKIP_BS(3,                                          private_bits);
    }
    else
    {
        if (Channels==3) //Mono
            SKIP_BS(1,                                          private_bits)
        else
            SKIP_BS(2,                                          private_bits);
    }
    if (Mpeg==3) //MPEG-1
    {
        for(int8u ch=0; ch<Mpega_Channels[Channels]; ch++)
            for(int8u scfsi_band=0; scfsi_band<4; scfsi_band++)
            {
                int32u scfsi;
                GET_BS (1, scfsi,                               scfsi);
                if (scfsi)
                    Scfsi++;
            }
    }
    for(int8u gr=0; gr<(Mpeg==3?2:1); gr++)
    {
        for(int8u ch=0; ch<Mpega_Channels[Channels]; ch++)
        {
            SKIP_BS(12,                                         part2_3_length);
            SKIP_BS(9,                                          big_values);
            SKIP_BS(8,                                          global_gain);
            if (Mpeg==3) //MPEG-1
                SKIP_BS(4,                                      scalefac_compress)
            else
                SKIP_BS(9,                                      scalefac_compress)
            int32u blocksplit_flag;
            GET_BS (1, blocksplit_flag,                         blocksplit_flag);
            if (blocksplit_flag==1)
            {
                int32u block_type, mixed_block_flag;
                GET_BS (2, block_type,                          block_type)
                GET_BS (1, mixed_block_flag,                    mixed_block_flag)
                for (int8u region=0; region<2; region++)
                    SKIP_BS(5,                                  table_select)
                for (int8u window=0; window<3; window++)
                    SKIP_BS(3,                                  subblock_gain)
                if (block_type == 2)
                {
                    if (mixed_block_flag==1)
                    {
                        INFO("Mixed");
                        Block_Count[2]++; //Mixed
                    }
                    else
                    {
                        INFO("Short");
                        Block_Count[1]++; //Short
                    }
                }
                else
                {
                    INFO("Long");
                    Block_Count[0]++; //Long
                }
            }
            else
            {
                for (int8u region=0; region<3; region++)
                    SKIP_BS(5,                                  table_select)
                SKIP_BS(4,                                      region0_count);
                SKIP_BS(3,                                      region1_count);
                INFO("Long");
                Block_Count[0]++; //Long
            }
            if (Mpeg==3) //MPEG-1
                SKIP_BS(1,                                      preflag);
            int32u scalefac;
            GET_BS (1, scalefac,                                scalefac_scale);
            if (scalefac==1)
                Scalefac++;
            SKIP_BS(1,                                          count1table_select);
        } //channel
    } //granules

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Frame_Fill();
}

//---------------------------------------------------------------------------
void File_Mpega::Frame_Fill()
{
    Stream_Prepare(Stream_General);
    Fill("Format", Ztring(Mpega_Version[Mpeg])+Ztring(Mpega_Layer[Layer]));
    Stream_Prepare(Stream_Audio);
    Fill("Codec", Ztring(Mpega_Version[Mpeg])+Ztring(Mpega_Layer[Layer]));
    Fill("Codec/String", Ztring(Mpega_Version_String[Mpeg])+Ztring(Mpega_Layer_String[Layer]));
    Fill("SamplingRate", Mpega_SamplingRate[Mpeg][SamplingRate]);
    Fill("Channel(s)", Mpega_Channels[Channels]);
    Fill("Codec_Profile", Mpega_Codec_Profile[Channels]);
    Fill("Resolution", 16);

    if (File_Size>File_EndTagSize+4*1024 && File_Offset+Buffer_Size<File_Size)
    {
        //Details
        Buffer_Offset=(size_t)(Element_Next-File_Offset);
        if (Config.Details_Get())
        {
            Details_Add_Error("------------------------------------------");
            Details_Add_Error("---   MPEG-A, Jumping to end of file   ---");
            Details_Add_Error("------------------------------------------");
        }
        //No more need data
        if (File_Name.empty())
            File_Offset=File_Size; //Finnished
        else if (File_Size>File_EndTagSize+8*1024)
            File_GoTo=File_Size-File_EndTagSize-8*1024; //Looking for end frames and tags

        //Reseting Frame_Count
        Frame_Count=0;
    }
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_Xing()
{
    int32u Xing_Header_Offset;
    if (Mpeg==3) //MPEG-1
        if (Channels==3) //Mono
            Xing_Header_Offset=21-4;
        else
            Xing_Header_Offset=36-4;
    else //MPEG-2 or 2.5
        if (Channels==3) //Mono
            Xing_Header_Offset=13-4;
        else
            Xing_Header_Offset=21-4;
    if (Buffer_Offset+Xing_Header_Offset+128<Buffer_Size)
    {
        const int8u* Xing_Header=Buffer+Buffer_Offset+Xing_Header_Offset;
        if (CC4(Xing_Header)==CC4("Xing") || CC4(Xing_Header)==CC4("Info"))
        {
            //This is a "tag"
            File_BeginTagSize+=4+Element_Size;

            //Configure defines : must return false
            #undef INTEGRITY_RETURNVALUE
            #define INTEGRITY_RETURNVALUE false

            NAME("Xing header");
            FLUSH();

            //Parsing
            ELEMENT_BEGIN(); //Details_Level_Last=1; //Will be setted later
            Buffer_Offset+=Xing_Header_Offset;
            int32u Flags;
            bool FrameCount, FileSize, TOC, Scale, Lame;
            CC_BEGIN();
            SKIP_C4(                                            Xing);
            GET_B4 (Flags,                                      Flags);
                GET_FLAGS(Flags, 0, FrameCount,                 FrameCount);
                GET_FLAGS(Flags, 1, FileSize,                   FileSize);
                GET_FLAGS(Flags, 2, TOC,                        TOC);
                GET_FLAGS(Flags, 3, Scale,                      Scale);
                GET_FLAGS(Flags, 4, Lame,                       Lame);
            int32u Xing_Header_Size=8
                                   +(FrameCount?  4:0)    //FrameCount
                                   +(FileSize?    4:0)    //FileSize
                                   +(TOC?       100:0)    //TOC
                                   +(Scale?       4:0)    //Scale
                                   +(Lame?      348:0);   //Lame
            //Element size
            if (Xing_Header_Size>Element_Size-Xing_Header_Offset)
                return false; //Error tag size
            //Element_Size=Xing_Header_Size;
            ELEMENT(1, "Xing", Element_Size);

            //Parsing
            if (FrameCount)
                GET_B4 (VBR_Frames,                             FrameCount); //FrameCount exclude this frame
            if (FileSize)
            {
                GET_B4 (VBR_FileSize,                           FileSize);
                VBR_FileSize-=4+Element_Size; //FileSize include the Xing element
            }
            if (TOC)
                SKIP_XX(100,                                    TOC);
            if (Scale)
                SKIP_B4(                                        Scale);
            Ztring Lib;
            PEEK_LOCAL(4, Lib);
            if (Lame || Lib==_T("LAME") || Lib==_T("GOGO"))
            {
                GET_LOCAL(9, Encoded_Library,                   Encoded_Library);
                if (Encoded_Library>=_T("LAME3.90"))
                {
                    int32u Flags;
                    GET_B1 (Flags,                              Flags);
                    if ((Flags&0xF0)<=0x10) //Rev. 0 or 1, http://gabriel.mp3-tech.org/mp3infotag.html
                    {
                        INFO(Lame_Method[Flags&0x0F]);
                        BitRate_Mode=Lame_BitRate_Mode[Flags&0x0F];
                        Encoded_Library_Settings.From_Local(Lame_Method[Flags&0x0F]);
                        if ((Flags&0x0F)==1 || (Flags&0x0F)==8) //2 possible values for CBR
                            VBR_Frames=0;
                    }
                    INFO_B1(lowpass,                            Lowpass filter value); INFO(lowpass*100, " Hz");
                }
                else
                {
                    Stream_Pos-=9; //Replace it at the beginning
                    GET_LOCAL(20, Encoded_Library,              Encoded_Library_Long); //Long tag version, if version<3.90
                }
            }

            if (CC4(Xing_Header)==CC4("Info"))
                VBR_Frames=0; //This is not a VBR file

            //UnConfigure defines : must return nothing
            #undef INTEGRITY_RETURNVALUE
            #define INTEGRITY_RETURNVALUE false

            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_VBRI()
{
    const size_t Fraunhofer_Header_Offset=36-4;
    if (Buffer_Offset+Fraunhofer_Header_Offset+32<Buffer_Size)
    {
        const int8u* Fraunhofer_Header=Buffer+Buffer_Offset+Fraunhofer_Header_Offset;
        if (CC4(Fraunhofer_Header)==CC4("VBRI") && CC2(Fraunhofer_Header+4)==0x0001) //VBRI v1 only
        {
            //This is a "tag"
            File_BeginTagSize+=4+Element_Size;

            //Configure defines : must return false
            #undef INTEGRITY_RETURNVALUE
            #define INTEGRITY_RETURNVALUE false

            NAME("VBRI header");
            FLUSH();

            //Parsing
            ELEMENT_BEGIN(); //Details_Level_Last=1; //Will be setted later
            Buffer_Offset+=Fraunhofer_Header_Offset;
            CC_BEGIN();
            SKIP_C4(                                            VBRI);
            SKIP_B2(                                            Version);
            SKIP_B4(                                            Unknown);
            GET_B4 (VBR_FileSize,                               FileSize);
            GET_B4 (VBR_Frames,                                 Frames);
            ELEMENT(1, "VBRI", 18);

            //UnConfigure defines : must return nothing
            #undef INTEGRITY_RETURNVALUE
            #define INTEGRITY_RETURNVALUE

            return true;
        }
    }
    return false;
}

//---------------------------------------------------------------------------
bool File_Mpega::Header_Encoders()
{
    std::string BufferS((const char*)(Buffer+Buffer_Offset), Element_Size);
    size_t Buffer_Pos;

    //Lame
    Buffer_Pos=BufferS.find("LAME");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<Element_Size-11)
    {
        NAME("Lame");
        Ztring Lib="Lame ";

        size_t Buffer_Pos_End=BufferS.find("LAME", Buffer_Pos);
        if (Buffer_Pos_End==std::string::npos)
            Buffer_Pos_End=Buffer_Pos+4;
        Lib+=Ztring((const char*)(Buffer+Buffer_Offset+4), Buffer_Pos_End-Buffer_Pos);

        Encoded_Library+=Lib;
        return true;
    }

    //RCA
    Buffer_Pos=BufferS.find("RCA mp3PRO Encoder");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<Element_Size-23)
    {
        NAME("RCA");
        Ztring Lib="RCA ";
        Lib+=Ztring((const char*)(Buffer+Buffer_Offset+18), 5);

        Encoded_Library+=Lib;
        return true;
    }

    //Thomson
    Buffer_Pos=BufferS.find("THOMSON mp3PRO Encoder");
    if (Buffer_Pos!=std::string::npos && Buffer_Pos<Element_Size-29)
    {
        NAME("Thomson");
        Ztring Lib="Thomson ";
        Lib+=Ztring((const char*)(Buffer+Buffer_Offset+22), 6);

        Encoded_Library+=Lib;
        return true;
    }

    //Gogo (old)
    Buffer_Pos=BufferS.find("MPGE");
    if (Buffer_Pos!=std::string::npos)
    {
        NAME("Gogo");
        Ztring Lib="Gogo <3.0";

        Encoded_Library+=Lib;
        return true;
    }

    //Gogo (new)
    Buffer_Pos=BufferS.find("GOGO");
    if (Buffer_Pos!=std::string::npos)
    {
        NAME("Gogo");
        Ztring Lib="Gogo >3.0";

        Encoded_Library+=Lib;
        return true;
    }

    return false;
}

void File_Mpega::Encoded_Library_Guess()
{
    if (Block_Count[1]==0) //No short blocks
    {
        if (Channels==2) //Dual Mono
        {
            if (Scfsi>0) //scfsi used
                {}
            else //no scfsi
            {
                if (Scalefac>0) //scalefacors used
                    {}
                else //scalefacors not used
                    Encoded_Library="Shine";
            }
        }
        else //Other than dual mono
        {
            if (Extension_Count[1]>0 || Extension_Count[3]>0) //Intensity Stereo
                Encoded_Library="Xing (very old)";
            else //No Intensity Stereo
            {
                if (Scfsi>0) //Scfsi used
                    Encoded_Library="Xing (new)";
                else //Scsfi not used
                {
                    if (Channels_Count[2]>0) //Joint Stereo
                    {
                        if (Channels_Count[0]>0) //also includes no Joint Stereo frames
                        {
                            if (Padding) //Padding
                            {
                                if (Original)
                                    Encoded_Library="FhG (l3enc)";
                                else
                                    Encoded_Library="FhG (fastenc or mp3enc)";
                            }
                            else //No padding
                                Encoded_Library="FhG (ACM or producer pro)";
                        }
                        else //No stereo frames: joint stereo was forced
                        {
                            if (Padding && !Original && !Copyright)
                                Encoded_Library="QDesign (fast mode)";
                        }
                    }
                    else
                    {
                        if (Channels_Count[0]>0 && Scalefac==0 && !Original) //Stereo
                            Encoded_Library="Plugger";
                        else
                            Encoded_Library="Xing (old)";
                    }
                }
            }
        }
    }
    else //Short blocks
    {
        if (Scfsi)  //scfsi used
        {
            if (Scalefac>0) //Scalefactor used
                Encoded_Library="Gogo (after 3.0)"; //Could be lame, but with a label, detected elsewhere before
            else
                Encoded_Library="Lame (old) or m3e";
        }
        else //Scfsi not used
        {
            if (Scalefac>0) //Scalefactor used
            {
                if (Padding)
                {
                    if (Original)
                    {
                       /*
                            //10 last bytes
                            int sum = get_final_sum(data);
                            if (sum==0)
                                return guess = _T("FhG (fastenc, low quality mode)");
                            else if (sum==10 * 0xFF)
                                return guess = _T("FhG (l3enc)");
                            else if (sum==5 * 0x20)
                                return guess = _T("FhG (fastenc, medium or high quality mode)");
                            else
                                return guess = _T("FhG (l3enc or fastenc)");
                        */
                    }
                    else
                    {
                        if (Channels_Count[1]>0 && Extension_Count[1]>0)        //Joint Stereo and some Intensity Stereo
                            Encoded_Library="Thomson mp3PRO Encoder";
                        else
                            Encoded_Library="FhG (fastenc or mp3enc)";
                    }
                }
                else //No padding
                {
                    if (BitRate_Mode.find(_T("VBR"))==0) //VBR
                        Encoded_Library="FhG (fastenc)";
                    else
                        Encoded_Library="FhG (ACM or producer pro)";
                }
            }
            else //scalefactors not used
            {
                if (Channels_Count[1]>0) //Joint Stereo
                {
                    if (Padding && !Original && !Copyright)
                        Encoded_Library="QDesign";
                }
                else //Joint Stereo not used
                {
                    if (BitRate_Mode.find(_T("VBR"))==0) //VBR
                        Encoded_Library="Lame (old)";
                    else //CBR
                    {

                        if (Channels==2) //Dual Mono
                        {
                            if (Padding)
                                Encoded_Library="Blade";
                            else
                                Encoded_Library="dist10 encoder or other encoder";
                        }
                        else //Stereo or Mono
                        {
                            /*
                            if (data.av_reservoir < 40 && !data.vbr) //ISO based encoders are unable to properly use bit reservoir... average reservoir usage is about 10
                            {
                                if (data.padding)
                                    return guess = _T("Blade");
                                else
                                    return guess = _T("dist10 encoder or other encoder");
                            }
                            else
                                return guess = _T("Gogo (before 3.0)");
                            */
                        }
                    }
                }
            }
        }
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpega::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size
        && (!((CC2(Buffer+Buffer_Offset_Temp)&0xFFE0)==0xFFE0 && (Buffer[Buffer_Offset_Temp+1]&0x18)!=0x08 && (Buffer[Buffer_Offset_Temp+1]&0x06)!=0x00)
            || CC2(Buffer+Buffer_Offset_Temp)==0xFFFF))
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (Buffer_Offset_Temp+3==Buffer_Size)
        {
            if (!((CC2(Buffer+Buffer_Offset_Temp)&0xFFE0)==0xFFE0 && (Buffer[Buffer_Offset_Temp+1]&0x18)!=0x08 && (Buffer[Buffer_Offset_Temp+1]&0x06)!=0x00))
            {
                Buffer_Offset_Temp++;
                if (!((CC2(Buffer+Buffer_Offset_Temp)&0xFFE0)==0xFFE0 && (Buffer[Buffer_Offset_Temp+1]&0x18)!=0x08 && (Buffer[Buffer_Offset_Temp+1]&0x06)!=0x00))
                {
                    Buffer_Offset_Temp++;
                    if (!((CC1(Buffer+Buffer_Offset_Temp)&0xFF)==0xFF))
                        Buffer_Offset_Temp++;
                }
            }
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;

        //Delay handling
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        //Tag management
        if (File_Offset+Buffer_Offset>=File_Size-File_EndTagSize)
        {
            Buffer_Offset=File_Size-File_EndTagSize-File_Offset;
            Tags_Read_Buffer_Continue();
        }

        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
        return false;

    //Error in stream?
    if (Buffer_Offset_Temp-Buffer_Offset>0)
    {
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
    //Synched=true; //Should be done here, but more test is done with Element_Size_Get()
    //if (Frame_Count==0)
    //    Delay+=Buffer_Offset_Temp;
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpega::Element_Size_Get()
{
    //Calculating
    if (Mpega_SamplingRate[Mpeg][SamplingRate]==0)
    {
        Element_Size=0;
        return true; //Will be detected in Element_Parse()
    }

    Element_Size=(Mpega_Coefficient[Mpeg][Layer]*Mpega_BitRate[Mpeg][Layer][BitRate]*1000/Mpega_SamplingRate[Mpeg][SamplingRate]+Padding)*Mpega_SlotSize[Layer];

/*
    if (Layer==3) //Layer 1
    {
        if (Mpeg==3) //MPEG-1
            Element_Size=(12*Mpega_BitRate[Mpeg][Layer][BitRate]*1000/Mpega_SamplingRate[Mpeg][SamplingRate]+Padding)*4;
        else //MPEG-2 and 2.5
            Element_Size=(6*Mpega_BitRate[Mpeg][Layer][BitRate]*1000/Mpega_SamplingRate[Mpeg][SamplingRate]+Padding)*4;
    }
    else //Layer 2 and 3
    {
        if (Mpeg==3) //MPEG-1
            Element_Size=144*Mpega_BitRate[Mpeg][Layer][BitRate]*1000/Mpega_SamplingRate[Mpeg][SamplingRate]+Padding;
        else //MPEG-2 and 2.5
            Element_Size=72*Mpega_BitRate[Mpeg][Layer][BitRate]*1000/Mpega_SamplingRate[Mpeg][SamplingRate]+Padding;
    }
*/

    //Coherancy
    if (Element_Size<4)
    {
        Element_Size=0;
        return true; //Will be detected in Element_Parse()
    }

    //Uncompleted frame management
    if (Buffer_Offset+Element_Size>Buffer_Size || File_Offset+Buffer_Offset+Element_Size>File_Size-File_EndTagSize)
    {
        if (File_Offset+Buffer_Offset+Element_Size>File_Size-File_EndTagSize)
        {
            ELEMENT(0, "Frame - Uncomplete", Element_Size-4);
            if (Count_Get(Stream_Audio)==0)
                Frame_Fill();
            FLUSH();
            Buffer_Offset=(size_t)(File_Size-File_EndTagSize-File_Offset);
            Tags_Read_Buffer_Continue();
        }
        return false;
    }

    //Some file have the padding bit set but there is no padding
    //if ((CC2(Buffer+Buffer_Offset+Element_Size)&0xFFE0)!=0xFFE0)// && (Buffer[Buffer_Offset+1]&0x18)!=0x08 && (Buffer[Buffer_Offset+1]&0x06)!=0x00)
    //    Element_Size--;

    //Trying do dectect false starters
    if (File_Offset+Buffer_Offset+Element_Size!=File_Size-File_EndTagSize) //Not at the end
    {
        if (Buffer_Offset+Element_Size+2>Buffer_Size) //Need 2 bytes
            return false;
        if (!((CC2(Buffer+Buffer_Offset+Element_Size)&0xFFE0)==0xFFE0 && (Buffer[Buffer_Offset+Element_Size+1]&0x18)!=0x08 && (Buffer[Buffer_Offset+Element_Size+1]&0x06)!=0x00))
        {
            Element_Size=0;
            return true; //Will be detected in Element_Parse()
        }
    }

    //OK, we continue
    Element_Next=File_Offset+Buffer_Offset+Element_Size;
    Element_Size-=4;
    if (Frame_Count==0)
        Delay+=Buffer_Offset;
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpega::Detect_NonMPEGA ()
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
void File_Mpega::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("BitRate"), Info_HowTo)=_T("R");
        General[0](_T("PlayTime"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Audio)
    {
        Audio[0](_T("Codec"), Info_HowTo)=_T("R");
        Audio[0](_T("BitRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Channel(s)"), Info_HowTo)=_T("R");
        Audio[0](_T("ChannelPositions"), Info_HowTo)=_T("R");
        Audio[0](_T("SamplingRate"), Info_HowTo)=_T("R");
    }
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES


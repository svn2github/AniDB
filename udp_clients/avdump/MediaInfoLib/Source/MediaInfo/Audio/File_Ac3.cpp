// File_Ac3 - Info for AC3 files
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

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AC3_YES) || defined(MEDIAINFO_DVDV_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const int32u AC3_SamplingRate[]=
{ 48000,  44100,  32000,      0,};

//---------------------------------------------------------------------------
extern const int16u AC3_BitRate[]=
{

     32,
     40,
     48,
     56,
     64,
     80,
     96,
    112,
    128,
    160,
    192,
    224,
    256,
    320,
    384,
    448,
    512,
    576,
    640,
};

//---------------------------------------------------------------------------
const char*  AC3_ChannelPositions[]=
{
    "L R",
    "C",
    "L R",
    "L C R",
    "Front: L R,   Rear: C",
    "Front: L C R, Rear: C",
    "Front: L R,   Rear: L R",
    "Front: L C R, Rear: L R",
};

//---------------------------------------------------------------------------
extern const int16u AC3_Channels[]=
{2, 1, 2, 3, 3, 4, 4, 5};

//---------------------------------------------------------------------------
const int16u AC3_FrameSize[27][4]=
{
    { 128,  138,  192,    0},
    { 160,  174,  240,    0},
    { 192,  208,  288,    0},
    { 224,  242,  336,    0},
    { 256,  278,  384,    0},
    { 320,  348,  480,    0},
    { 384,  416,  576,    0},
    { 448,  486,  672,    0},
    { 512,  556,  768,    0},
    { 640,  696,  960,    0},
    { 768,  834, 1152,    0},
    { 896,  974, 1344,    0},
    {1024, 1114, 1536,    0},
    {1280, 1392, 1920,    0},
    {1536, 1670, 2304,    0},
    {1792, 1950, 2688,    0},
    {2048, 2228, 3072,    0},
    {2304, 2506, 3456,    0},
    {2560, 2786, 3840,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    {   0,    0,    0,    0},
    { 768,    0,    0,    0},
};

//---------------------------------------------------------------------------
int16u AC3_FrameSize_Get(int32u frmsizecod, int32u fscod)
{
    bool Padding=(bool)(frmsizecod%2);
    int32u frame_size_id=frmsizecod/2;

    if (frame_size_id>26 || fscod>3)
        return 0;

    int32u FrameSize=AC3_FrameSize[frame_size_id][fscod];
    if (fscod==1 && Padding)
        FrameSize+=2; // frame lengths are padded by 1 word (16 bits) at 44100 Hz
    return FrameSize;
}

//---------------------------------------------------------------------------
const char*  AC3_Mode[]=
{
    "complete main (CM)",
    "music and effects (ME)",
    "visually impaired (VI)",
    "hearing impaired (HI)",
    "dialogue (D)",
    "commentary (C)",
    "emergency (E)",
    "voice over (VO)",
};

//---------------------------------------------------------------------------
const char*  AC3_Surround[]=
{
    "",
    "(No surround)",
    "(Surround)",
    "",
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
#if defined(MEDIAINFO_AC3_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Ac3.h"
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ac3::Read_Buffer_Init()
{
    //Configuration
    File_MaximumOffset=64*1024;
    if (File_Name.empty())
        Frame_Count_Valid=4;
    else
        Frame_Count_Valid=32;

    //External info
    Delay=0;

    //Buffer
    Element_Size_Save=0;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
    id=0;
    bitstream_mode=0;
    channels_mode=0;
    central=0;
    back=0;
    dolby=0;
    subwoofer=0;
}

//---------------------------------------------------------------------------
void File_Ac3::Read_Buffer_Continue()
{
    //Trusted limitation
    Trusted=Buffer_Size>2*(8*1024)?Buffer_Size/(8*1024):2; //Never less than 2 acceptable errors

    //Buffer
    Element_Next=File_Offset;
    Buffer_Offset_Temp=0;

    //Parsing
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::Buffer_Parse()
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
bool File_Ac3::Element_Parse()
{
    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    if (Element_Size<7 && File_Offset+Buffer_Size==File_Size) //Uncompleted frame management
    {
        ELEMENT(0, "Frame (uncomplete)", Element_Size);
        if (Count_Get(Stream_Audio)==0)
            Frame_Fill();
        FLUSH();
    }
    BS_BEGIN();
    SKIP_BS(16,                                                 syncword);
    SKIP_BS(16,                                                 crc1);
    GET_BS ( 2, fscod,                                          fscod); INFO(AC3_SamplingRate[fscod], " Hz");
    GET_BS ( 6, frmsizecod,                                     frmsizecod); if (frmsizecod/2<19) {INFO(AC3_FrameSize_Get(frmsizecod/2, fscod)*1000, " Bps");}
    GET_BS ( 5, id,                                             id);
    if (id<=0x08)
    {
        GET_BS (3, bitstream_mode,                              bitstream mode); INFO(AC3_Mode[bitstream_mode]);
        GET_BS (3, channels_mode,                               channel mode); INFO(AC3_ChannelPositions[channels_mode]);
        if (channels_mode&1) //central present
            GET_BS (2, central,                                 central amplification);
        if (channels_mode&4) //back present
            GET_BS (2, back,                                    back amplification);
        if (channels_mode==2)
            GET_BS (2, dolby,                                   dolby);
        GET_BS (1, subwoofer,                                   Sub woofer present);
    }
    BS_END_CANBEMORE();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Element size
    if (!Element_Size_Get())
        return false;
    if (Element_Size==0)
    {
        //Error in header, look for next header
        if (Frame_Count>0 && File_Offset<File_MaximumOffset)
            TRUSTED_ISNOT("Frame - Bad Synchro", 0, 1);
        else
        {
            ELEMENT(0, "Frame - Bad first Synchro", 1);
            FLUSH();
        }
        Synched=false;
        Element_Next=File_Offset+Buffer_Offset+1;
        Buffer_Offset_Temp=Buffer_Offset+1;
        return true;
    }

    //Element
    ELEMENT(0, "Frame", Element_Size);

    //Positioning
    Buffer_Offset+=7;

    //Parsing
    Frame();

    FLUSH();
    return true;
}

//---------------------------------------------------------------------------
void File_Ac3::Frame()
{
    //Counting
    if (Element_Next==File_Size)
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
void File_Ac3::Frame_Fill()
{
    Stream_Prepare(Stream_General);
    Fill("Format", "AC3");
    Stream_Prepare(Stream_Audio);
    Fill("SamplingRate", AC3_SamplingRate[fscod]);
    if (frmsizecod/2<19)
    {
        Fill("BitRate", AC3_BitRate[frmsizecod/2]*1000);
        if (Delay>100 && File_Name.empty())
            Fill("Delay", (float)Delay*1000/(AC3_BitRate[frmsizecod/2]*1000/8), 0);
    }

    if (id<=0x08)
    {
        Fill("Codec", "AC3");
        if (channels_mode==0)
            Fill("Codec_Profile", "Dual Mono");
        int8u Channels=AC3_Channels[channels_mode];
        Ztring ChannelPositions; ChannelPositions.From_Local(AC3_ChannelPositions[channels_mode]);
        if (subwoofer)
        {
            Channels+=1;
            ChannelPositions+=_T(", Subwoofer");
        }
        Fill("Channel(s)", Channels);
        Fill("ChannelPositions", ChannelPositions);
        if (dolby)
            Fill("Codec_Profile", "Dolby Digital");
        Fill("BitRate_Mode", "CBR");
    }

    if (id==0x10)
    {
        Fill("Codec", "AC3+");
        Fill("BitRate_Mode", "CBR");
    }

    if (File_Offset+Buffer_Size<File_Size)
    {
        //Details
        Buffer_Offset=(size_t)(Element_Next-File_Offset);
        if (Config.Details_Get())
        {
            Details_Add_Error("----------------------------------------");
            Details_Add_Error("---   AC-3, Jumping to end of file   ---");
            Details_Add_Error("----------------------------------------");
        }
        //No more need data
        File_Offset=File_Size;
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Ac3::NextFrame()
{
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+2<=Buffer_Size && CC2(Buffer+Buffer_Offset_Temp)!=0x0B77)
        Buffer_Offset_Temp++;

    //Test if synch is lost
    if (Synched && Buffer_Offset_Temp>3840)
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("----------------------------");
            Details_Add_Error("---   AC-3, Synch lost   ---");
            Details_Add_Error("----------------------------");
        }
        //No more need data
        Synched=false;
    }

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+2>Buffer_Size)
    {
        if (Buffer_Offset_Temp+1==Buffer_Size)
        {
            if (CC1(Buffer+Buffer_Offset_Temp)!=0x0B)
                Buffer_Offset_Temp++;
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;

        //Delay handling
        if (Frame_Count==0)
            Delay+=Buffer_Offset;

        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+2>Buffer_Size)
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
    Synched=true;
    if (Frame_Count==0)
        Delay+=Buffer_Offset;
    return true;
}

//---------------------------------------------------------------------------
bool File_Ac3::Element_Size_Get()
{
    //Calculating
    Element_Size=AC3_FrameSize_Get(frmsizecod, fscod);
    if (Element_Size==0)
        return true; //Will be detected later

    //Uncompleted frame management
    if (Buffer_Offset+Element_Size>Buffer_Size)
    {
        if (File_Offset+Buffer_Size==File_Size)
        {
            ELEMENT(0, "Frame - Uncomplete", Element_Size-7);
            Frame_Fill();
            FLUSH();
        }
        return false;
    }

    //OK, we continue
    Element_Next=File_Offset+Buffer_Offset+Element_Size;
    Element_Size-=7;
    return true;
}

//---------------------------------------------------------------------------
void File_Ac3::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OveralBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("BitRate_Mode", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("ChannelPositions", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Codec", "R");
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

#endif //MEDIAINFO_AC3_YES


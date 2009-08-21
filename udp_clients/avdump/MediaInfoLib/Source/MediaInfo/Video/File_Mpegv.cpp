// File_Mpegv - Info for MPEG Video files
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
#if defined(MEDIAINFO_MPEGV_YES) || defined(MEDIAINFO_MPEGTS_YES) || defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

#include "ZenLib/Conf.h"
using namespace ZenLib;

//---------------------------------------------------------------------------
extern const float Mpegv_FrameRate[]=
{
    0,
    23.976,
    24,
    25,
    29.97,
    30,
    50,
    59.94,
    60,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const char* Mpegv_Chroma[]=
{
    "",
    "4:2:0",
    "4:2:2",
    "4:4:4",
};

//---------------------------------------------------------------------------
const char* Mpegv_Profile[]=
{
    "",
    "High",
    "Spatial",
    "SNR",
    "Main",
    "Simple",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_Level[]=
{
    "",
    "",
    "",
    "",
    "High",
    "",
    "High-1440",
    "",
    "Main",
    "",
    "Low",
    "",
    "",
    "",
    "",
    "",
};

} //NameSpace

//---------------------------------------------------------------------------
#endif //...
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Mpegv.h"
#include <ZenLib/BitStream.h>
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace MediaInfoLib
{
//---------------------------------------------------------------------------

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const float Mpegv_Ratio1[]=
{
    0,
    1,
    0.6735,
    0.7031, //16/9 PAL
    0.7615,
    0.8055,
    0.8437, //16/9 NTSC
    0.8935,
    0.9157, //4/3 PAL
    0.9815,
    1.0255,
    1.0695,
    1.0950, //4/3 NTSC
    1.1575,
    1.2015,
    0,
};

//---------------------------------------------------------------------------
const float Mpegv_Ratio2[]=
{
    0,
    1,
    (float)4/3,
    (float)16/9,
    2.21,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

//---------------------------------------------------------------------------
const char* Mpegv_Standard[]=
{
    "Composite",
    "PAL",
    "NTSC",
    "SECAM",
    "MAC",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Mpegv_Interlacement[]=
{
    "Interlaced",
    "TFF",
    "BFF",
    "PPF",
};

//---------------------------------------------------------------------------
const char* Mpegv_FrameType[]=
{
    "",
    "I",
    "P",
    "B",
    "D",
    "",
    "",
    "",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpegv::File_Mpegv()
{
    //Configuration
    MPEG_Version=2;
    Frame_Count_Valid=2;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Init()
{
    //Elements
    Element_Size=0;
    Frame_Count=0;

    //Temp
    Time_Begin_Seconds=Error;
    Time_Begin_Frames=Error;
    Time_End_Seconds=Error;
    Time_End_Frames=Error;
    Time_End_NeedComplete=false;
    Width=0;
    Height=0;
    RatioValue=0;
    FrameRate=0;
    BitRate=0;

    //Error controls
    User_Start_Count=0;
    Synched=false;
}

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEGV())
    {
        File_Offset=File_Size;
        return;
    }

    //Buffer
    Element_Next=File_Offset;

    //Parse
    Trusted=Buffer_Size/512; //Pb with Test China 1.mpg if /1024
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Mpegv::Read_Buffer_Finalize()
{
    //Tags with multiple location
    //-AspectRatio
    float AspectRatio=0;
    if (MPEG_Version==2)
    {
        if (RatioValue==1 && Height!=0)
            AspectRatio=Width/Height;
        else
            AspectRatio=Mpegv_Ratio2[RatioValue];
    }
    else
    {
        if (Height!=0 && Mpegv_Ratio1[RatioValue]!=0)
            AspectRatio=(float)Width/Height/Mpegv_Ratio1[RatioValue];
    }

    //-Version
    if (MPEG_Version==2)
    {
        Fill(Stream_General, 0, "Format", "MPEG-2V");
        Fill("Codec", "MPEG-2V");
        Fill("Codec/String", "MPEG-2 Video");
    }
    else
    {
        Fill(Stream_General, 0, "Format", "MPEG-1V");
        Fill("Codec", "MPEG-1V");
        Fill("Codec/String", "MPEG-1 Video");
        Fill("Interlacement", "PPF");
    }
    Fill("Width", Width);
    Fill("Height", Height);
    Fill("AspectRatio", AspectRatio);
    Fill("FrameRate", FrameRate);

    if (BitRate==0x3FFFF)
        Fill("BitRate_Mode", "VBR");
    else
    {
        Fill("BitRate_Mode", "CBR");
        Fill("BitRate", BitRate*400);
    }

    if (Library.size()>=8)
    {
        Fill("Encoded_Library", Library);
        Fill(Stream_General, 0, "Encoded_Library", Library);
    }

    //Calculate InitTime with Framerate
    if (Time_End_NeedComplete && Config.ParseSpeed_Get()!=1)
        Time_End_Seconds=Error;
    if (Time_End_Seconds!=Error)
    {
        size_t Time_Begin=Time_Begin_Seconds*1000;
        size_t Time_End =Time_End_Seconds*1000;
        Time_End_Frames++; //+1 to count the last frame
        if (FrameRate)
        {
            Time_Begin+=(size_t)(Time_Begin_Frames*1000/FrameRate);
            Time_End  +=(size_t)(Time_End_Frames  *1000/FrameRate);
        }
        if (!Video.empty() && Time_End>Time_Begin)
            Video[0](_T("PlayTime")).From_Number(Time_End-Time_Begin);
    }
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Buffer_Parse()
{
    //Next frame
    if (!NextFrame())
        return false;

    //Element name
    Element_Name=BigEndian2int8u(Buffer+Buffer_Offset+3);

    //Element size
    if (!Element_Size_Get())
        return false;

    //Is OK?
    if (Element_Name_IsOK())
        Element_Parse();

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next frame
    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
// Element parse
//
bool File_Mpegv::Element_Parse()
{
    //Details
    if (Config.Details_Get())
    {
        Details_Add_Element(0, Element_Name, Element_Size-4);
    }

    //Header
    Buffer_Offset+=4;
    Element_Size -=4;

    //Parse
    switch (Element_Name)
    {
        case 0x00: Picture_Start(); break;
        case 0xB2: User_Start(); break;
        case 0xB3: Sequence_Header(); break;
        case 0xB4: Sequence_Error(); break;
        case 0xB5: Extension_Start(); break;
        case 0xB7: Sequence_End(); break;
        case 0xB8: Group_Start(); break;
        default:
            if (Element_Name>=0x01
             && Element_Name<=0xAF) Slice_Start();
            else
                TRUSTED_ISNOT("Unattended element!");
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

#define NEXTFRAME_TEST() \
    { \
        if (!Frame_ShouldBe.empty()) \
        { \
            bool OK=false; \
            for (size_t Pos=0; Pos<Frame_ShouldBe.size(); Pos++) \
                if (Frame_ShouldBe[Pos]==Element_Name) \
                    OK=true; \
            if (!OK) \
                TRUSTED_ISNOT("Frames are not in the right order"); \
        } \
    } \

#define NEXTFRAME_CLEAR() \
    { \
        Frame_ShouldBe.clear(); \
    } \

#define NEXTFRAME_ADD(_Frame) \
    { \
        Frame_ShouldBe.push_back(_Frame); \
    } \

//---------------------------------------------------------------------------
// Packet "00"
void File_Mpegv::Picture_Start()
{
    DETAILLEVEL_SET(0);
    NAME("Picture_Start");
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x01);
    NEXTFRAME_ADD(0xB2);
    NEXTFRAME_ADD(0xB5);

    //Counting
    if (Element_Next==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //Parsing
    int32u FrameType;
    BS_BEGIN();
    SKIP_BS(10,                                                 temporal_reference)
    GET_BS ( 3, FrameType,                                      picture_coding_type)

    //Time
    if (Time_End_Seconds!=Error)
        Time_End_Frames++;

    //Details
    if (Config.Details_Get())
    {
        Details_Add_Info(Mpegv_FrameType[FrameType]);
        if (Time_End_Seconds!=Error)
        {
            size_t Time_Begin=Time_Begin_Seconds*1000;
            size_t Time_End =Time_End_Seconds*1000;
            if (FrameRate)
            {
                Time_Begin+=(size_t)(Time_Begin_Frames*1000/FrameRate);
                Time_End  +=(size_t)(Time_End_Frames  *1000/FrameRate);
            }
            Details_Add_Info(Error, "Info - Time", Time_End-Time_Begin);
        }
    }
}

//---------------------------------------------------------------------------
// Packet "01" --> "AF"
void File_Mpegv::Slice_Start()
{
    DETAILLEVEL_SET(0.7);
    NAME("Slice_Start");
    INTEGRITY_GENERAL(0);
    NEXTFRAME_TEST();
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x00);
    for (int32u Element_Name_Next=Element_Name; Element_Name_Next<=0x1F; Element_Name_Next++)
        NEXTFRAME_ADD(Element_Name_Next);
    NEXTFRAME_ADD(0xB3);
    NEXTFRAME_ADD(0xB8);

    //Detection is finnished
    if (Count_Get(Stream_Video)>0 && Frame_Count>=Frame_Count_Valid)
        Slice_Start_Fill();
}

//---------------------------------------------------------------------------
void File_Mpegv::Slice_Start_Fill()
{
    if (File_Size>4*1024*1024 && File_Offset+Buffer_Size<File_Size-4*1024*1024 && File_Size!=(int64u)-1 && Config.ParseSpeed_Get()<=0.01)
    {
        DETAILLEVEL_SET(0);
        //Details
        Buffer_Offset=(size_t)(Element_Next-File_Offset);
        if (Config.Details_Get())
        {
            Details_Add_Error("------------------------------------------");
            Details_Add_Error("---   MPEG-V, Jumping to end of file   ---");
            Details_Add_Error("------------------------------------------");
        }

        //Jumping
        File_GoTo=File_Size-4*1024*1024;
        Synched=false;
        NEXTFRAME_CLEAR();
        Time_End_Seconds=Error;
        Time_End_Frames=Error;
    }
}

//---------------------------------------------------------------------------
// Packet "B2"
void File_Mpegv::User_Start()
{
    DETAILLEVEL_SET(0);
    NAME("User_Start");

    //In case of User start is not used for library name
    if (User_Start_Count>2)
        return;
    User_Start_Count++;

    //Reading
    size_t Library_Offset=0;
    size_t Library_Size=Element_Size;

    //-Reject junk after the name
    while (Library_Size>0 && (Buffer[Buffer_Offset+Library_Offset+Library_Size-1]<0x20 || Buffer[Buffer_Offset+Library_Offset+Library_Size-1]>0x7D))
        Library_Size--;
    if (Library_Size==0)
        return;

    //-Reject junk before the name
    size_t Library_Offset_Final=Library_Offset+Library_Size-1;
    while (Buffer_Offset+Library_Offset_Final>=Buffer_Offset+Library_Offset && Buffer[Buffer_Offset+Library_Offset_Final]>=0x20 && Buffer[Buffer_Offset+Library_Offset_Final]<=0x7D)
        Library_Offset_Final--;
    if (Buffer_Offset+Library_Offset_Final>=Buffer_Offset+Library_Offset)
    {
        Library_Size -=Library_Offset_Final-Library_Offset;
        Library_Offset=Library_Offset_Final;
    }

    //Filling
    Library.From_Local((const char*)Buffer+Buffer_Offset+Library_Offset, Library_Size);

    //Details
    if (Config.Details_Get())
    {
        Details_Add_Info(Library_Offset, "Datas", Library);
    }
}

//---------------------------------------------------------------------------
// Packet "B3"
void File_Mpegv::Sequence_Header()
{
    DETAILLEVEL_SET(0);
    NAME("Sequence_Header");
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0xB2);
    NEXTFRAME_ADD(0xB5);
    NEXTFRAME_ADD(0xB8);
    NOT_NEEDED(!General.empty());

    //Reading
    int32u FrameRate_Code, Intra, Intra_No;
    BS_BEGIN();
    GET_BS (12, Width,                                          horizontal_size_value)
    GET_BS (12, Height,                                         vertical_size_value)
    GET_BS ( 4, RatioValue,                                     aspect_ratio_information)
    GET_BS ( 4, FrameRate_Code,                                 frame_rate_code)
    GET_BS (18, BitRate,                                        bit_rate_value)
    SKIP_BS( 1,                                                 marker_bit)
    SKIP_BS(10,                                                 vbv_buffer_size_value)
    SKIP_BS( 1,                                                 constrained_parameters_flag)
    GET_BS ( 1, Intra,                                          load_intra_quantiser_matrix)
    if (Intra)
    {
        for (size_t Pos=0; Pos<64; Pos++)
            SKIP_BS(8,                                          intra_quantiser_matrix)
    }
    GET_BS ( 1, Intra_No,                                       load_intra_quantiser_matrix)
    if (Intra_No)
    {
        for (size_t Pos=0; Pos<64; Pos++)
            SKIP_BS(8,                                          non_intra_quantiser_matrix)
    }
    BS_END();

    //Calculations
    FrameRate=Mpegv_FrameRate[FrameRate_Code];

    //Filling
    Stream_Prepare(Stream_General);
    Stream_Prepare(Stream_Video);
    if (Intra || Intra_No)
    {
        Fill("Codec_Settings", "CustomMatrix");
        Fill("Codec_Settings/Matrix", "Custom");
    }
    else
        Fill("Codec_Settings/Matrix", "Standard");
}

//---------------------------------------------------------------------------
// Packet "B4"
void File_Mpegv::Sequence_Error()
{
    DETAILLEVEL_SET(0);
    NAME("Sequence_Error");
    INTEGRITY_GENERAL(0);
}

//---------------------------------------------------------------------------
// Packet "B5"
void File_Mpegv::Extension_Start()
{
    DETAILLEVEL_SET(0);
    NAME("Extension_Start");
    INTEGRITY_GENERAL(0);
    MPEG_Version=2; //Extension_Start only exists in MPEG-2 specs

    //Reading
    int32u ID;
    BS_BEGIN();
    GET_BS ( 4, ID,                                             extension_start_code_identifier)

    //sequence_extension
         if (ID==1)
    {
        INFO("Sequence Extension");

        //Reading
        int32u Profile, Level, Progressive, Chroma, Width_Ext, Height_Ext,
               BitRate_Ext, FrameRate_N, FrameRate_D;
        SKIP_BS( 1,                                             profile_and_level_indication_escape)
        GET_BS ( 3, Profile,                                    profile_and_level_indication_profile) INFO(Mpegv_Profile[Profile]);
        GET_BS ( 4, Level,                                      profile_and_level_indication_level) INFO(Mpegv_Level[Level]);
        GET_BS ( 1, Progressive,                                progressive_sequence)
        GET_BS ( 2, Chroma,                                     chroma_format) INFO(Mpegv_Chroma[Chroma]);
        GET_BS ( 2, Width_Ext,                                  horizontal_size_extension)
        GET_BS ( 2, Height_Ext,                                 vertical_size_extension)
        GET_BS (12, BitRate_Ext,                                bit_rate_extension)
        SKIP_BS( 1,                                             marker_bit)
        SKIP_BS( 8,                                             vbv_buffer_size_extension)
        SKIP_BS( 1,                                             low_delay)
        GET_BS ( 2, FrameRate_N,                                frame_rate_extension_n)
        GET_BS ( 5, FrameRate_D,                                frame_rate_extension_d)
        BS_END();

        //Filling
        Width+=0x1000*Width_Ext;
        Height+=0x1000*Height_Ext;
        BitRate+=0x4000*BitRate_Ext;
        if (FrameRate_D!=0)
            FrameRate=FrameRate_N/FrameRate_D;
        if (Progressive && Get(Stream_Video, 0, _T("Interlacement")).empty())
            Fill("Interlacement", Mpegv_Interlacement[3]);
        if (Get(Stream_Video, 0, _T("Chroma")).empty())
            Fill("Chroma", Mpegv_Chroma[Chroma]);
        if (Profile<8 && Level<16 && Get(Stream_Video, 0, _T("Codec_Profile")).empty())
            Fill("Codec_Profile", Ztring().From_Local(Mpegv_Profile[Profile])+_T("@")+Ztring().From_Local(Mpegv_Level[Level]));
    }

    //sequence_display_extension
    else if (ID==2)
    {
        INFO("Sequence Display Extension");

        //Reading
        int32u Standard;

        GET_BS ( 3, Standard,                                   video_format) INFO(Mpegv_Standard[Standard]);
        TEST_BS_SKIP( 1,                                        load_intra_quantiser_matrix)
            SKIP_BS( 8,                                         colour_primaries)
            SKIP_BS( 8,                                         transfer_characteristics)
            SKIP_BS( 8,                                         matrix_coefficients)
        TEST_BS_END();
        SKIP_BS(14,                                             display_horizontal_size)
        SKIP_BS( 1,                                             marker_bit)
        SKIP_BS(14,                                             display_vertical_size)
        BS_END();

        //Filling
        if (Standard<8 && Get(Stream_Video, 0, _T("Standard")).empty())
            Fill("Standard", Mpegv_Standard[Standard]);
    }
    //
    else if (ID==3)
    {
        INFO("Quant Matrix Extension");
    }
    //
    else if (ID==4)
    {
        INFO("Copyright");
    }
    //
    else if (ID==5)
    {
        INFO("Sequence Scalable");
    }
    //
    else if (ID==7)
    {
        INFO("Picture Display");
    }
    //
    else if (ID==8)
    {
        INFO("Picture Coding");
        //Reading
        int32u Standard, PictureStructure, TopField;
        GET_BS ( 4, Standard,                                   video_format)
        SKIP_BS( 4,                                             f_code_forward_horizontal)
        SKIP_BS( 4,                                             f_code_forward_vertical)
        SKIP_BS( 4,                                             f_code_backward_horizontal)
        SKIP_BS( 4,                                             f_code_backward_vertical)
        SKIP_BS( 2,                                             intra_dc_precision)
        GET_BS ( 2, PictureStructure,                           picture_structure)
        GET_BS ( 1, TopField,                                   top_field_first)
        SKIP_BS( 1,                                             frame_pred_frame_dct)
        SKIP_BS( 1,                                             concealment_motion_vectors)
        SKIP_BS( 1,                                             q_scale_type)
        SKIP_BS( 1,                                             intra_vlc_format)
        SKIP_BS( 1,                                             alternate_scan)
        SKIP_BS( 1,                                             repeat_first_field)
        SKIP_BS( 1,                                             chroma_420_type)
        SKIP_BS( 1,                                             progressive_frame)
        TEST_BS_SKIP( 1,                                        composite_display_flag)
            SKIP_BS( 1,                                         v_axis)
            SKIP_BS( 3,                                         field_sequence)
            SKIP_BS( 1,                                         sub_carrier)
            SKIP_BS( 7,                                         burst_amplitude)
            SKIP_BS( 8,                                         sub_carrier_phase)
        TEST_BS_END();
        BS_END();

        //Filling
        size_t Interlacement;
        if (PictureStructure==3)
        {
            if (TopField)
                Interlacement=1; //TopField
            else
                Interlacement=2; //BottomField
        }
        else
            Interlacement=PictureStructure; //Depend of 1st field found
        if (Get(Stream_Video, 0, _T("Interlacement")).empty())
            Fill("Interlacement", Mpegv_Interlacement[Interlacement]);
    }
    //
    else if (ID==9)
    {
        INFO("Picture Spatial Scalable");
    }
    //
    else if (ID==10)
    {
        INFO("Picture Temporal Scalable");
    }
    //
    else
    {
        INFO("Reserved");
    }
}

//---------------------------------------------------------------------------
// Packet "B7"
void File_Mpegv::Sequence_End()
{
    DETAILLEVEL_SET(0);
    NAME("Sequence_End");
    INTEGRITY_GENERAL(0);
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpegv::Group_Start()
{
    DETAILLEVEL_SET(0);
    NAME("Group_Start");
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x00);
    NEXTFRAME_ADD(0xB2);
    NEXTFRAME_ADD(0xB5);
    INTEGRITY_GENERAL(0);

    //Reading
    BitStream BS(Buffer+Buffer_Offset, Element_Size);
    int32u Hours, Minutes, Seconds, Frames;
    SKIP_BS( 1,                                                 time_code_drop_frame_flag)
    GET_BS ( 5, Hours,                                          time_code_time_code_hours)
    GET_BS ( 6, Minutes,                                        time_code_time_code_minutes)
    SKIP_BS( 1,                                                 time_code_marker_bit)
    GET_BS ( 6, Seconds,                                        time_code_time_code_seconds)
    GET_BS ( 6, Frames,                                         time_code_time_code_pictures)
    SKIP_BS( 1,                                                 closed_gop)
    SKIP_BS( 1,                                                 broken_link)
    BS_END();

    //Calculating
    if (Time_Begin_Seconds==Error)
    {
        //Save BS_BEGIN(); time before
        Time_Begin_Seconds=60*60*Hours+60*Minutes+Seconds;
        Time_Begin_Frames =Frames;
    }
    else
    {
        //Is it always the same?
        if (60*60*Hours+60*Minutes+Seconds==Time_Begin_Seconds && Frames==Time_Begin_Frames)
            Time_End_NeedComplete=true; //Will always be empty...
    }
    if (!Time_End_NeedComplete)
    {
        Time_End_Seconds=60*60*Hours+60*Minutes+Seconds;
        Time_End_Frames =Frames-1; //We must not count the next picture
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpegv::Detect_NonMPEGV ()
{
    //Element_Size
    if (Buffer_Size<8)
    {
        File_Offset=File_Size;
        return true;
    }

    //Detect DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        File_Offset=File_Size;
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    if (Buffer_Size>=188*4)
    {
        //Look for first Sync word
        while (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset)!=0x47)
            Buffer_Offset++;
        if (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset+188)==0x47 && CC1(Buffer+Buffer_Offset+188*2)==0x47 && CC1(Buffer+Buffer_Offset+188*3)==0x47)
        {
            File_Offset=File_Size;
            return true;
        }
        Buffer_Offset=0;
    }

    //Seems OK
    return false;
}

//---------------------------------------------------------------------------
bool File_Mpegv::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first bytes
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset_Temp)==0x000001 && CC1(Buffer+Buffer_Offset_Temp+3)<=0xB8)) //0xB8 is the upper value for MPEG-V
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (Buffer_Offset_Temp+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
            {
                Buffer_Offset_Temp++;
                if (CC2(Buffer+Buffer_Offset_Temp)!=0x0000)
                {
                    Buffer_Offset_Temp++;
                    if (CC1(Buffer+Buffer_Offset_Temp)!=0x00)
                        Buffer_Offset_Temp++;
                }
            }
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;
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
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpegv::Element_Size_Get()
{
    //Element with known size
    if (Element_Name==0xB7) //BS_END();
    {
        Element_Size=0;
        Element_Next=Buffer_Offset+4;
        return true;
    }

    //Searching Element_Next
    Element_Next=Buffer_Offset+4;
    while(Element_Next+3<=Buffer_Size && !(CC3(Buffer+Element_Next)==0x000001 && CC1(Buffer+Element_Next+3)<=0xB8)) //0xB8 is the upper value for MPEG-V
        Element_Next++;
    if (Element_Next+3>Buffer_Size)
    {
        //Not found
        //Maybe next byte is not good too
        if (CC2(Buffer+Element_Next)!=0x0000)
        {
            Element_Next++;
            //Maybe next byte is not good too
            if (CC1(Buffer+Element_Next)!=0x00)
                Element_Next++;
        }

        //Testing
        if (Element_Next-Buffer_Offset>=128*1024) //False positive, no element should have more than this size
        {
            TRUSTED_ISNOT("Next element is too big, not normal");
            File_Offset=File_Size;
        }
        Element_Next-=Buffer_Offset; //Because buffer will be moved by File__Base
        return false;
    }

    //Filling
    Element_Size=Element_Next-Buffer_Offset;

    //Testing
    if (Element_Size==0) //Not normal
        TRUSTED_ISNOT("Next element is too small, not normal");

    return true;
}

bool File_Mpegv::Element_Name_IsOK()
{
    //Is it the start of a MPEG-V?
    if (Count_Get(Stream_General)==0
     && !(Element_Name==0xB3)
    )
    {
        //MPEG V didn't BS_BEGIN();, SKIP_BSping
        //Details
        if (Config.Details_Get()>0.9)
        {
            Details_Add_Element(0, Element_Name, Element_Size-4);
            Details_Add_Element("Has not began, SKIP_BSping");
            FLUSH();
        }
        return false;
    }

    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpegv::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("Encoded_Application", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("AspectRatio", "R");
            Fill_HowTo("FrameRate", "R");
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

#endif //MEDIAINFO_MPEGV_YES


// File_Avc - Info for AVC Video files
// Copyright (C) 2006-2007 Jerome Martinez, Zen@MediaArea.net
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
#if defined(MEDIAINFO_AVC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Avc.h"
#include <ZenLib/BitStream.h>
#include <ZenLib/Utils.h>
#include <math.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const size_t Avc_Errors_MaxCount=32;

const int8u Avc_PixelAspectRatio_Size=14;
const float Avc_PixelAspectRatio[]=
{
    1.0, //Reserved
    1.0,
    12.0/11.0,
    10.0/11.0,
    16.0/11.0,
    40.0/33.0,
    24.0/11.0,
    20.0/11.0,
    32.0/11.0,
    80.0/33.0,
    18.0/11.0,
    15.0/11.0,
    64.0/33.0,
    160.0/99.0,
};

const char* Avc_PicType[]=
{
    "I",
    "I, P",
    "I, P, B",
    "SI",
    "SI, SP",
    "I, SI",
    "I, SI, P, SP",
    "I, SI, P, SP, B",
};

const char* Avc_SliceType[]=
{
    "P",
    "B",
    "I",
    "SP",
    "SI",
    "P",
    "B",
    "I",
    "SP",
    "SI",
};
//---------------------------------------------------------------------------

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Avc::File_Avc()
{
    //Configuration
    Frame_Count_Valid=16;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
    frame_mbs_only_flag=0;
    log2_max_frame_num_minus4=0;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::Read_Buffer_Init()
{
    //Temp - Global
    Frame_Count=0;
}

//---------------------------------------------------------------------------
void File_Avc::Read_Buffer_Continue()
{
    //Buffer
    Element_Next=File_Offset;

    //Parse with PES parser
    Trusted=Buffer_Size>4096?(Buffer_Size/2048):2;
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Avc::Buffer_Parse()
{
    //Next frame
    if (!NextFrame())
        return false;

    //Element name
    if (!Element_Name_Get())
        return false;

    //Element size
    if (!Element_Size_Get())
        return false;

    //Parse
    if (!Element_Parse())
        return false;

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next Frame
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
//
bool File_Avc::Element_Parse()
{
    //Details
    if (Config.Details_Get())
    {
        Details_Add_Element(0, Element_Name, Element_Size);
    }

    //Header
    Buffer_Offset+=4;

    //Parsing
    switch (Element_Name)
    {
        case 0x01 : slice_layer_without_partitioning_non_IDR(); break;
        case 0x02 : ; break;
        case 0x03 : ; break;
        case 0x04 : ; break;
        case 0x05 : slice_layer_without_partitioning_IDR(); break;
        case 0x06 : sei(); break;
        case 0x07 : seq_parameter_set(); break;
        case 0x08 : pic_parameter_set(); break;
        case 0x09 : access_unit_delimiter(); break;
        case 0x10 : ; break;
        case 0x11 : ; break;
        case 0x12 : ; break;
        default :
            TRUSTED_ISNOT("Unattended element!");
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

#define SKIP_U(_NAME) \
    { \
        if (BS.Remain()==0) \
            return; \
        int LeadingZeroBits=0; \
        while(BS.Remain()>0 && BS.Get(1)==0) \
            LeadingZeroBits++; \
        if (Config.Details_Get()) \
        { \
            Details_Add_Info(BS.Offset_Get()-1, #_NAME, (int32u)pow(2, (float)LeadingZeroBits)-1+BS.Get(LeadingZeroBits)); \
        } \
        else \
            BS.Skip(LeadingZeroBits); \
    } \

#define SKIP_S(_NAME) \
    { \
        if (BS.Remain()==0) \
            return; \
        int LeadingZeroBits=0; \
        while(BS.Remain()>0 && BS.Get(1)==0) \
            LeadingZeroBits++; \
        if (Config.Details_Get()) \
        { \
            double Temp=pow(2, (float)LeadingZeroBits)-1+BS.Get(LeadingZeroBits); \
            Details_Add_Info(BS.Offset_Get()-1, #_NAME, (int32u)pow(-1, Temp+1)*ceil(Temp/2)); \
        } \
        else \
            BS.Skip(LeadingZeroBits); \
    } \

#define GET_U(_INFO, _NAME) \
    { \
        if (BS.Remain()==0) \
            return; \
        int LeadingZeroBits=0; \
        while(BS.Remain()>0 && BS.Get(1)==0) \
            LeadingZeroBits++; \
        _INFO=(int32u)pow(2, (float)LeadingZeroBits)-1+BS.Get(LeadingZeroBits); \
        if (Config.Details_Get()) \
        { \
            Details_Add_Info(BS.Offset_Get()-1, #_NAME, _INFO, 16); \
        } \
    }

#define GET_S(_INFO, _NAME) \
    { \
        if (BS.Remain()==0) \
            return; \
        int LeadingZeroBits=0; \
        while(BS.Remain()>0 && BS.Get(1)==0) \
            LeadingZeroBits++; \
        double Temp=pow(2, (float)LeadingZeroBits)-1+BS.Get(LeadingZeroBits); \
        _INFO=(int32u)pow(-1, Temp+1)*ceil(Temp/2); \
        if (Config.Details_Get()) \
        { \
            Details_Add_Info(BS.Offset_Get()-1, #_NAME, _INFO); \
        } \
    }

//---------------------------------------------------------------------------
//
void File_Avc::slice_layer_without_partitioning_non_IDR()
{
    NAME("slice_layer_without_partitioning (non-IDR)");

    slice_header();
}

//---------------------------------------------------------------------------
//
void File_Avc::slice_layer_without_partitioning_IDR()
{
    NAME("slice_layer_without_partitioning (IDR)");

    slice_header();
}

//---------------------------------------------------------------------------
//
void File_Avc::slice_header()
{
    //Counting
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //Parsing
    int32u SliceType;
    BS_BEGIN();
    SKIP_U (                                                    first_mb_in_slice);
    GET_U  (SliceType,                                          slice_type); if (SliceType<9) INFO(Avc_SliceType[SliceType]);
    SKIP_U (                                                    pic_parameter_set_id);
    SKIP_BS(log2_max_frame_num_minus4,                          frame_num);
    if (!frame_mbs_only_flag)
    {
        TEST_BS_SKIP(1,                                         field_pic_flag);
            SKIP_BS(1,                                          bottom_field_flag);
        TEST_BS_END();
    }

    if (Count_Get(Stream_Video)!=0 && Frame_Count>=Frame_Count_Valid)
    {
        if (File_Offset+Buffer_Size<File_Size)
        {
            //Details
            Buffer_Offset=(size_t)(Element_Next-File_Offset);
            if (Config.Details_Get())
            {
                Details_Add_Error("---------------------------------------");
                Details_Add_Error("---   AVC, Jumping to end of file   ---");
                Details_Add_Error("---------------------------------------");
            }
            //No more need data
            File_Offset=File_Size;
        }
    }
}

//---------------------------------------------------------------------------
//
void File_Avc::seq_parameter_set()
{
    NAME("seq_parameter_set");

    float PixelAspectRatio=1;

    int32u Profile, Level, Width, Height;
    int32u pic_order_cnt_type, frame_cropping_flag, vui_parameters_present_flag;
    BS_BEGIN();
    GET_BS ( 8, Profile,                                        profile_idc);
    SKIP_BS( 1,                                                 constraint_set0_flag);
    SKIP_BS( 1,                                                 constraint_set1_flag);
    SKIP_BS( 1,                                                 constraint_set2_flag);
    SKIP_BS( 1,                                                 constraint_set3_flag);
    MARK_0 (                                                    reserved_zero_4bits);
    MARK_0 (                                                    reserved_zero_4bits);
    MARK_0 (                                                    reserved_zero_4bits);
    MARK_0 (                                                    reserved_zero_4bits);
    GET_BS ( 8, Level,                                          level_idc);
    SKIP_U (                                                    seq_parameter_set_id);
    if (Profile==0x64
     || Profile==0x6E
     || Profile==0x7A
     || Profile==0x90) //High profiles
    {
        int32u chroma_format_idc, seq_scaling_matrix_present_flag;
        GET_U  (chroma_format_idc,                              chroma_format_idc)
        if (chroma_format_idc==3)
            SKIP_U  (                                           residual_colour_transform_flag)
        SKIP_U  (                                               bit_depth_luma_minus8)
        SKIP_U  (                                               bit_depth_chroma_minus8)
        SKIP_BS( 1,                                             qpprime_y_zero_transform_bypass_flag)
        GET_BS ( 1, seq_scaling_matrix_present_flag,            seq_scaling_matrix_present_flag)
        if (seq_scaling_matrix_present_flag)
        {
            int32u Temp;
            for (int32u Pos=0; Pos<8; Pos++)
            {
                GET_BS (1, Temp,                                seq_scaling_list_present_flag)
                if (Temp)
                    scaling_list(BS, Pos<6?16:64); //Never tested!
            }
        }
    }
    GET_U  (log2_max_frame_num_minus4,                          log2_max_frame_num_minus4)
    GET_U  (pic_order_cnt_type,                                 pic_order_cnt_type)
    if (pic_order_cnt_type==0)
    {
        SKIP_U (                                                log2_max_pic_order_cnt_lsb_minus4)
    }
    else if (pic_order_cnt_type==1)
    {
        int32u num_ref_frames_in_pic_order_cnt_cycle;
        SKIP_BS(1,                                              delta_pic_order_always_zero_flag)
        SKIP_S (                                                offset_for_non_ref_pic)
        SKIP_S (                                                offset_for_top_to_bottom_field)
        GET_U  (num_ref_frames_in_pic_order_cnt_cycle,          num_ref_frames_in_pic_order_cnt_cycle)
        for(int32u Pos=0; Pos<num_ref_frames_in_pic_order_cnt_cycle; Pos++)
        {
            SKIP_S (                                            offset_for_ref_frame)
        }
    }
    SKIP_U (                                                    num_ref_frames)
    SKIP_BS( 1,                                                 gaps_in_frame_num_value_allowed_flag)
    GET_U  (Width,                                              pic_width_in_mbs_minus1)
    GET_U  (Height,                                             pic_height_in_map_units_minus1)
    GET_BS ( 1, frame_mbs_only_flag,                            frame_mbs_only_flag)
    if (!frame_mbs_only_flag)
    {
        SKIP_BS( 1,                                             mb_adaptive_frame_field_flag)
    }
    SKIP_BS( 1,                                                 direct_8x8_inference_flag)
    GET_BS ( 1, frame_cropping_flag,                            frame_cropping_flag)
    if (frame_cropping_flag)
    {
        SKIP_U (                                                frame_crop_left_offset)
        SKIP_U (                                                frame_crop_right_offset)
        SKIP_U (                                                frame_crop_top_offset)
        SKIP_U (                                                frame_crop_bottom_offset)
    }
    GET_BS ( 1, vui_parameters_present_flag,                    vui_parameters_present_flag)
    if (vui_parameters_present_flag)
        vui_parameters(BS, PixelAspectRatio);

    //Profile
    Ztring ProfileS;
    switch (Profile)
    {
        case 0x42 : ProfileS=_T("Baseline"); break;
        case 0x4D : ProfileS=_T("Main"); break;
        case 0x58 : ProfileS=_T("Extended"); break;
        case 0x64 : ProfileS=_T("High"); break;
        case 0x6E : ProfileS=_T("High 10"); break;
        case 0x7A : ProfileS=_T("High 4:2:2"); break;
        case 0x90 : ProfileS=_T("High 4:4:4"); break;
        default   : ProfileS.From_Number(Profile); break;
    }

    //Continue only if not already done
    if (Count_Get(Stream_Video)>0)
        return;

    //Level
    Ztring LevelS;
    LevelS.From_Number(((float)Level)/10, 1); //Level is Value*10, can have one digit example 5.1

    //Calculating
    Width=(Width+1)*16;
    Height=(Height+1)*(2-frame_mbs_only_flag)*16;

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "AVC");
    Stream_Prepare(Stream_Video);
    Fill("Codec", "AVC");

    Fill("Codec_Profile", ProfileS+_T("@L")+LevelS);
    Fill("Width", Width);
    Fill("Height", Height);
    Fill("PixelAspectRatio", PixelAspectRatio);
    if (Height!=0)
    {
        Fill("DisplayAspectRatio", ((float)Width)/Height*PixelAspectRatio);
        Fill("AspectRatio", ((float)Width)/Height*PixelAspectRatio); //Old version
    }
}

//---------------------------------------------------------------------------
//
void File_Avc::pic_parameter_set()
{
    NAME("pic_parameter_set");
}

//---------------------------------------------------------------------------
//
void File_Avc::sei()
{
    NAME("sei");
}

//---------------------------------------------------------------------------
//
void File_Avc::access_unit_delimiter()
{
    NAME("access_unit_delimiter");

    int32u PicType;
    BS_BEGIN();
    GET_BS ( 3, PicType,                                        primary_pic_type); INFO(Avc_PicType[PicType]);
}

//***************************************************************************
// SubElements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::scaling_list(BitStream &BS, int32u ScalingList_Size)
{
    //From http://mpeg4ip.cvs.sourceforge.net/mpeg4ip/mpeg4ip/util/h264/main.cpp?revision=1.17&view=markup, never tested
    double Scale_Delta;
    int32u Scale_Last=8, Scale_Next=8;
    for (int32u Pos=0; Pos<ScalingList_Size; Pos++)
    {
        if (Scale_Next!=0)
        {
            GET_S  (Scale_Delta,                                Scale_Delta_NeverTested)
            Scale_Next=(Scale_Last+(int32u)(Scale_Delta)+256)%256;
            Scale_Last=Scale_Next;
        }
    }
}

//---------------------------------------------------------------------------
void File_Avc::vui_parameters(BitStream &BS, float &PAR)
{
    TEST_BS_SKIP(1,                                             aspect_ratio_info_present_flag)
        int32u aspect_ratio_idc;
        GET_BS  (8, aspect_ratio_idc,                           aspect_ratio_idc)
        if (aspect_ratio_idc==0xFF)
        {
            int32u sar_width, sar_height;
            GET_BS  (16, sar_width,                             sar_width)
            GET_BS  (16, sar_height,                            sar_height)
        }
        else if (aspect_ratio_idc<Avc_PixelAspectRatio_Size)
            PAR=Avc_PixelAspectRatio[aspect_ratio_idc];
        else
            PAR=1; //Unknown
    TEST_BS_END();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Avc::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first bytes
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset_Temp)==0x000001 && (CC1(Buffer+Buffer_Offset_Temp+3)&0x1F)<=0x12)) //5 bits only, 0x12 is the upper value for AVC
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
bool File_Avc::Element_Name_Get()
{
    //Enough data?
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=4;
    BS_BEGIN();
    SKIP_BS(24,                                                 sync);
    MARK_0 (                                                    forbidden_zero_bit);
    SKIP_BS( 2,                                                 nal_ref_idc);
    GET_BS ( 5, Element_Name,                                   nal_unit_type);
    BS_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    return true;
}

//---------------------------------------------------------------------------
bool File_Avc::Element_Size_Get()
{
    size_t Buffer_Next=Buffer_Offset+4;
    while(Buffer_Next+3<Buffer_Size && CC3(Buffer+Buffer_Next)!=0x000001)
        Buffer_Next++;
    if (Buffer_Next+3>=Buffer_Size)
    {
        //No next element
        if (File_Offset+Buffer_Next+3>=File_Size)
            Buffer_Next=Buffer_Size; //This is the last frame
        else if (Buffer_Next-Buffer_Offset>=65536) //False positive, no element should have more than this size
        {
            TRUSTED_ISNOT("Next element is too big, not normal");
            File_Offset=File_Size;
            return false;
        }
        else
            return false; //Other byte will come
    }
    Element_Size=Buffer_Next-Buffer_Offset-4;
    Element_Next=File_Offset+Buffer_Next;
    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Avc::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
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

#endif //MEDIAINFO_AVC_YES

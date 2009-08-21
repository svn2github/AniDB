// File_Avc - Info for AVC Visual files
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
#if defined(MEDIAINFO_MPEG4V_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Mpeg4v.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char* Mpeg4v_PictureStructure[]=
{
    "Interlaced",
    "TFF",
    "BFF",
    "PPF",
};

const char* Mpeg4v_Chroma[]=
{
    "",
    "4:2:0",
    "",
    "",
};

const char* Mpeg4v_visual_object_type[]=
{
    "",
    "video",
    "still texture",
    "mesh",
    "FBA",
    "3D mesh",
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

const char* Mpeg4v_verid[]=
{
    "",
    "ISO/IEC 14496-2",
    "ISO/IEC 14496-2 AMD 1",
    "ISO/IEC 14496-2 AMD 2",
    "ISO/IEC 14496-2 AMD 3 (Studio)",
    "ISO/IEC 14496-2 AMD 4 (SVP)",
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

const char* Mpeg4v_vop_coding_type[]=
{
    "I",
    "P",
    "B",
    "S",
};

const char* Mpeg4v_Profile_Level(int32u Profile_Level)
{
    switch (Profile_Level)
    {
        case B8(00000001) : return "Simple Profile/Level 1";
        case B8(00000010) : return "Simple Profile/Level 2";
        case B8(00000011) : return "Simple Profile/Level 3";
        case B8(00010001) : return "Simple Scalable Profile/Level 1";
        case B8(00010010) : return "Simple Scalable Profile/Level 2";
        case B8(00100001) : return "Core Profile/Level 1";
        case B8(00100010) : return "Core Profile/Level 2";
        case B8(00110010) : return "Main Profile/Level 2";
        case B8(00110011) : return "Main Profile/Level 3";
        case B8(00110100) : return "Main Profile/Level 4";
        case B8(01000010) : return "N-bit Profile/Level 2";
        case B8(01010001) : return "Scalable Texture Profile/Level 1";
        case B8(01100001) : return "Simple Face Animation Profile/Level 1";
        case B8(01100010) : return "Simple Face Animation Profile/Level 2";
        case B8(01100011) : return "Simple FBA Profile/Level 1";
        case B8(01100100) : return "Simple FBA Profile/Level 2";
        case B8(01110001) : return "Basic Animated Texture Profile/Level 1";
        case B8(01110010) : return "Basic Animated Texture Profile/Level 2";
        case B8(10000001) : return "Hybrid Profile/Level 1";
        case B8(10000010) : return "Hybrid Profile/Level 2";
        case B8(10010001) : return "Advanced Real Time Simple Profile/Level 1";
        case B8(10010010) : return "Advanced Real Time Simple Profile/Level 2";
        case B8(10010011) : return "Advanced Real Time Simple Profile/Level 3";
        case B8(10010100) : return "Advanced Real Time Simple Profile/Level 4";
        case B8(10100001) : return "Core Scalable Profile/Level 1";
        case B8(10100010) : return "Core Scalable Profile/Level 2";
        case B8(10100011) : return "Core Scalable Profile/Level 3";
        case B8(10110001) : return "Advanced Coding Efficiency Profile/Level 1";
        case B8(10110010) : return "Advanced Coding Efficiency Profile/Level 2";
        case B8(10110011) : return "Advanced Coding Efficiency Profile/Level 3";
        case B8(10110100) : return "Advanced Coding Efficiency Profile/Level 4";
        case B8(11000001) : return "Advanced Core Profile/Level 1";
        case B8(11000010) : return "Advanced Core Profile/Level 2";
        case B8(11010001) : return "Advanced Scalable Texture/Level 1";
        case B8(11010010) : return "Advanced Scalable Texture/Level 2";
        case B8(11010011) : return "Advanced Scalable Texture/Level 3";
        case B8(11100001) : return "Simple Studio Profile/Level 1";
        case B8(11100010) : return "Simple Studio Profile/Level 2";
        case B8(11100011) : return "Simple Studio Profile/Level 3";
        case B8(11100100) : return "Simple Studio Profile/Level 4";
        case B8(11100101) : return "Core Studio Profile/Level 1";
        case B8(11100110) : return "Core Studio Profile/Level 2";
        case B8(11100111) : return "Core Studio Profile/Level 3";
        case B8(11101000) : return "Core Studio Profile/Level 4";
        case B8(11110001) : return "Simple Streaming Video Profile/Level 1";
        case B8(11110010) : return "Simple Streaming Video Profile/Level 2";
        case B8(11110011) : return "Simple Streaming Video Profile/Level 3";
        case B8(11110100) : return "Simple Streaming Video Profile/Level 4";
        case B8(11110101) : return "Streaming Video Profile/Level 1";
        case B8(11110110) : return "Streaming Video Profile/Level 2";
        case B8(11110111) : return "Streaming Video Profile/Level 3";
        case B8(11111000) : return "Streaming Video Profile/Level 4";
        case B8(11111111) : return "None";
        default :           return "Unknown";
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
Ztring Mpeg4v_Quant_Mat_Read(BitStream &BS);
//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4v::Read_Buffer_Init()
{
    //Configuration
    Frame_Count_Valid=16;

    IVOP_Count=0;
    PVOP_Count=0;
    BVOP_Count=0;
    SVOP_Count=0;
    NVOP_Count=0;
    Chroma=Error;
    Standard=Error;
    PictureStructure=Error;
    interlaced=Error;
    newpred_enable=0;
    time_size=0;
    reduced_resolution_vop_enable=0;
    shape=Error;
    sprite_enable=0;
    scalability=0;
    enhancement_type=0;
    complexity_estimation_disable=0;
    vop_time_increment_resolution=0;
    RIFF_VOP_Count=0;
    RIFF_VOP_Count_Max=0;

    //Temp - Global
    Frame_Count=0;
}

//---------------------------------------------------------------------------
void File_Mpeg4v::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEG4V())
    {
        File_Offset=File_Size;
        return;
    }

    //Look for first Sync word
    while (Buffer_Offset+3<=Buffer_Size && CC3(Buffer+Buffer_Offset)!=0x000001)
        Buffer_Offset++;
    if (Buffer_Offset+3>Buffer_Size)
        return;

    //We want to count the number of VOP in this bufffer
    RIFF_VOP_Count=0;

    //Parse
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Mpeg4v::Read_Buffer_Finalize()
{
    //Libray
    if (!Library.empty())
        Fill("Encoded_Library", Library);
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg4v::Buffer_Parse()
{
    //Enough data?
    if (Buffer_Offset+4>Buffer_Size)
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

    //Next Frame
    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
// Element parse
//
bool File_Mpeg4v::Element_Parse()
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
        case 0xB0: visual_object_sequence_start(); break;
        case 0xB1: visual_object_sequence_end(); break;
        case 0xB2: user_data_start(); break;
        case 0xB3: group_of_vop_start(); break;
        case 0xB4: video_session_error(); break;
        case 0xB5: visual_object_start(); break;
        case 0xB6: vop_start(); break;
        case 0xB7: slice_start(); break;
        case 0xB8: extension_start(); break;
        case 0xB9: fgs_vop_start(); break;
        case 0xBA: fba_object_start(); break;
        case 0xBB: fba_object_plane_start(); break;
        case 0xBC: mesh_object_start(); break;
        case 0xBD: mesh_object_plane_start(); break;
        case 0xBE: still_texture_object_start(); break;
        case 0xBF: texture_spatial_layer_start(); break;
        case 0xC0: texture_snr_layer_start(); break;
        case 0xC1: texture_tile_start(); break;
        case 0xC2: texture_shape_layer_start(); break;
        default:
                 if (Element_Name<=0x1F) video_object_start();
            else if (Element_Name>=0x20
                  && Element_Name<=0x2F) video_object_layer_start();
            else if (Element_Name>=0x40
                  && Element_Name<=0x4F) fgs_bp_start();
            else if (Element_Name>=0xC6) system_start();
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
void File_Mpeg4v::video_object_start()
{
    NAME("video_object_start");
    NEXTFRAME_CLEAR();

    //Parsing
    BS_BEGIN();
    BS_END();

    //Filling
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);
}

//---------------------------------------------------------------------------
// Packet "20"
void File_Mpeg4v::video_object_layer_start()
{
    NAME("video_object_layer_start");
    NEXTFRAME_CLEAR();

    //Parsing
    int32u Width=0;
    int32u Height=0;
    int32u PixelAspectRatio_Width=0;
    int32u PixelAspectRatio_Height=0;
    int32u QPel=0;
    int32u GMC=0;
    int32u BVOP=0;
    int32u load_intra_quant_mat=0;
    int32u load_nonintra_quant_mat=0;
    int32u load_intra_quant_mat_grayscale=0;
    int32u load_nonintra_quant_mat_grayscale=0;
    int32u verid=1;
    int32u PixelAspectRatio;
    int32u shape_extension=0;
    int32u aux_comp_count=0;
    int32u Resolution=8;
    int32u fixed_vop_time=0;
    BS_BEGIN();
    SKIP_BS(1,                                                  random_accessible_vol);
    SKIP_BS(8,                                                  video_object_type_indication);
    TEST_BS_SKIP(1,                                             is_object_layer_identifier);
        GET_BS (4, verid,                                       video_object_layer_verid); INFO(Mpeg4v_verid[verid]);
        SKIP_BS(3,                                              video_object_layer_priority);
    TEST_BS_END();
    GET_BS(4, PixelAspectRatio,                                 aspect_ratio_info);
    if (PixelAspectRatio==0x0F)
    {
        GET_BS (8, PixelAspectRatio_Width,                      par_width);
        GET_BS (8, PixelAspectRatio_Height,                     par_height);
    }
    TEST_BS_SKIP(1,                                             vol_control_parameters);
        GET_BS (2, Chroma,                                      chroma_format);
        GET_BS(1, BVOP,                                         low_delay);
        BVOP=(BVOP==1?0:1);
        TEST_BS_SKIP(1,                                         vbv_parameters)
            SKIP_BS(15,                                         first_half_bit_rate);
            MARK_1 (                                            marker_bit);
            SKIP_BS(15,                                         latter_half_bit_rate);
            MARK_1 (                                            marker_bit);
            SKIP_BS(15,                                         first_half_vbv_Element_Size);
            MARK_1 (                                            marker_bit);
            SKIP_BS( 3,                                         latter_half_vbv_Element_Size);
            SKIP_BS(11,                                         first_half_vbv_occupancy);
            MARK_1 (                                            marker_bit);
            SKIP_BS(15,                                         latter_half_vbv_occupancy);
            MARK_1 (                                            marker_bit);
        TEST_BS_END();
    TEST_BS_END();
    GET_BS (2, shape,                                           video_object_layer_shape);
    if (shape==3 && verid!=1) //Shape=GrayScale
        GET_BS (4, shape_extension,                             video_object_layer_shape_extension);
    if (shape_extension==0 && shape_extension==1 && shape_extension==5 && shape_extension==7 && shape_extension==8) aux_comp_count=1;
    if (shape_extension==2 && shape_extension==3 && shape_extension==6 && shape_extension==9 && shape_extension==11) aux_comp_count=2;
    if (shape_extension==4 && shape_extension==10 && shape_extension==12) aux_comp_count=3;
    MARK_1 (                                                    marker_bit);
    GET_BS (16, vop_time_increment_resolution,                  vop_time_increment_resolution);
    int32u PowerOf2=1;
    for (time_size=0; time_size<=16; time_size++)
    {
        if (vop_time_increment_resolution<PowerOf2)
            break;
        PowerOf2<<=1;
    }
    MARK_1 (                                                    marker_bit);
    TEST_BS_SKIP(1,                                             fixed_vop_rate);
        GET_BS (time_size, fixed_vop_time,                      fixed_vop_time); INFO(fixed_vop_time*1000/vop_time_increment_resolution, " ms");
    TEST_BS_END();
    if (shape!=2) //Shape!=BinaryOnly
    {
        if (shape==0) //Shape=Rectangular
        {
            MARK_1 (                                            marker_bit);
            GET_BS (13, Width,                                  object_layer_width);
            MARK_1 (                                            marker_bit);
            GET_BS (13, Height,                                 object_layer_height);
            MARK_1 (                                            marker_bit);
        }
        GET_BS (1, interlaced,                                  interlaced);
        if (interlaced)
            PictureStructure=0;
        else
            PictureStructure=3;
        SKIP_BS(1,                                              obmc_disable);
        if (verid==1)
            GET_BS (1, sprite_enable,                           sprite_enable)
        else
            GET_BS (2, sprite_enable,                           sprite_enable)
        if (sprite_enable==1 || sprite_enable==2 )  //static or GMC
        {
            if (sprite_enable!=2) //No GMC
            {
                SKIP_BS(13,                                     sprite_width);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     sprite_height);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     sprite_top_coordinate);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     sprite_left_coordinate);
                MARK_1 (                                        marker_bit);
            }
            GET_BS (6, GMC,                                     no_of_sprite_warping_points);
            SKIP_BS(2,                                          sprite_warping_accuracy);
            SKIP_BS(1,                                          sprite_brightness_change);
            if (sprite_enable!=2) //No GMC
                SKIP_BS(1,                                      low_latency_sprite_enable);
        }
        if (verid==1 && shape!=0) //Shape!=Rectangular
            SKIP_BS(1,                                          sadct_disable);
        TEST_BS_SKIP(1,                                         bits_per_pixel_not_8_bit)
            SKIP_BS(4,                                          quant_precision);
            GET_BS (4, Resolution,                              bits_per_pixel);
        TEST_BS_END();
        if (shape==3) //Shape=GrayScale
        {
            SKIP_BS(1,                                          no_gray_quant_update);
            SKIP_BS(1,                                          composition_method);
            SKIP_BS(1,                                          linear_composition);
        }
        TEST_BS_SKIP(1,                                         quant_type);
            GET_BS(1, load_intra_quant_mat,                     load_intra_quant_mat);
            if(load_intra_quant_mat)
                for (int16u Pos=0; Pos<64; Pos++)
                {
                    int32u intra_quant_mat;
                    GET_BS(8, intra_quant_mat,                  intra_quant_mat);
                    if (intra_quant_mat)
                        Pos=64;
                }
            GET_BS(1, load_nonintra_quant_mat,                  load_nonintra_quant_mat);
            if(load_nonintra_quant_mat)
                for (int16u Pos=0; Pos<64; Pos++)
                {
                    int32u nonintra_quant_mat;
                    GET_BS(8, nonintra_quant_mat,               nonintra_quant_mat);
                    if (nonintra_quant_mat)
                        Pos=64;
                }
            if(shape==3) //Shape=GrayScale
            {
                for(size_t Pos=0; Pos<aux_comp_count; Pos++)
                {
                    GET_BS(1, load_intra_quant_mat_grayscale,   load_intra_quant_mat_grayscale);
                    if(load_intra_quant_mat_grayscale)
                        for (int16u Pos=0; Pos<64; Pos++)
                        {
                            int32u intra_quant_mat_grayscale;
                            GET_BS(8, intra_quant_mat_grayscale, intra_quant_mat_grayscale);
                            if (intra_quant_mat_grayscale)
                                Pos=64;
                        }
                    GET_BS(1, load_nonintra_quant_mat_grayscale, load_nonintra_quant_mat_grayscale);
                    if(load_nonintra_quant_mat_grayscale)
                        for (int16u Pos=0; Pos<64; Pos++)
                        {
                            int32u nonintra_quant_mat_grayscale;
                            GET_BS(8, nonintra_quant_mat_grayscale, nonintra_quant_mat_grayscale);
                            if (nonintra_quant_mat_grayscale)
                                Pos=64;
                        }
                }
            }
        TEST_BS_END();
        if (verid!=1)
        {
            GET_BS (1, QPel,                                    quarter_sample);
        }
        GET_BS (1, complexity_estimation_disable,               complexity_estimation_disable);
        if (!complexity_estimation_disable)
        {
            int32u estimation_method;
            GET_BS (2, estimation_method,                       estimation_method);
            if (estimation_method==0 || estimation_method==1)
            {
                TEST_BS_SKIP(1,                                 shape_complexity_estimation_disable)
                    SKIP_BS(1,                                  opaque);
                    SKIP_BS(1,                                  transparent);
                    SKIP_BS(1,                                  intra_cae);
                    SKIP_BS(1,                                  inter_cae);
                    SKIP_BS(1,                                  no_update);
                    SKIP_BS(1,                                  upsampling);
                TEST_BS_END();
                TEST_BS_SKIP(1,                                 texture_complexity_estimation_set_1_disable)
                    SKIP_BS(1,                                  intra_blocks);
                    SKIP_BS(1,                                  inter_blocks);
                    SKIP_BS(1,                                  inter4v_blocks);
                    SKIP_BS(1,                                  not_coded_blocks);
                TEST_BS_END();
				SKIP_BS(1,                                  	marker_bit); //MARK_1 (                                        marker_bit);
                TEST_BS_SKIP(1,                                 texture_complexity_estimation_set_2_disable)
                    SKIP_BS(1,                                  dct_coefs);
                    SKIP_BS(1,                                  dct_lines);
                    SKIP_BS(1,                                  vlc_symbols);
                    SKIP_BS(1,                                  vlc_bits);
                TEST_BS_END();
                TEST_BS_SKIP(1,                                 motion_compensation_complexity_disable)
                    SKIP_BS(1,                                  apm);
                    SKIP_BS(1,                                  npm);
                    SKIP_BS(1,                                  interpolate_mc_q);
                    SKIP_BS(1,                                  forw_back_mc_q);
                    SKIP_BS(1,                                  halfpel2);
                    SKIP_BS(1,                                  halfpel4);
                TEST_BS_END();
				SKIP_BS(1,                                  	marker_bit); //MARK_1 (                                        marker_bit);
                if (estimation_method==1)
                {
                    TEST_BS_SKIP(1,                             version2_complexity_estimation_disable)
                        SKIP_BS(1,                              sadct);
                        SKIP_BS(1,                              quarterpel);
                    TEST_BS_END();
                }
            }
        }
        SKIP_BS(1,                                              resync_marker_disable);
        TEST_BS_SKIP(1,                                         data_partitioned)
            SKIP_BS(1,                                          reversible_vlc);
        TEST_BS_END();
        if (verid!=1)
        {
            GET_BS (1, newpred_enable,                          newpred_enable);
            if (newpred_enable)
            {
                SKIP_BS(2,                                      requested_upstream_message_type);
                SKIP_BS(1,                                      newpred_segment_type);
            }
            GET_BS (1, reduced_resolution_vop_enable,           reduced_resolution_vop_enable);
        }
        GET_BS (1, scalability,                                 scalability);
        if (scalability==1)
        {
            int32u Hierarchy;
            GET_BS (1, Hierarchy,                               hierarchy_type);
            SKIP_BS(4,                                          ref_layer_id);
            SKIP_BS(1,                                          ref_layer_sampling_direc);
            SKIP_BS(5,                                          hor_sampling_factor_n);
            SKIP_BS(5,                                          hor_sampling_factor_m);
            SKIP_BS(5,                                          vert_sampling_factor_n);
            SKIP_BS(5,                                          vert_sampling_factor_m);
            GET_BS (1, enhancement_type,                        enhancement_type);
            if (shape==1 && Hierarchy==0) //Shape=Binary
            {
                SKIP_BS(1,                                      use_ref_shape);
                SKIP_BS(1,                                      use_ref_texture);
                SKIP_BS(5,                                      shape_hor_sampling_factor_n);
                SKIP_BS(5,                                      shape_hor_sampling_factor_m);
                SKIP_BS(5,                                      shape_vert_sampling_factor_n);
                SKIP_BS(5,                                      shape_vert_sampling_factor_m);
            }
        }
    }
    else
    {
        if (verid!=1)
        {
            TEST_BS_SKIP(1,                                     scalability);
                SKIP_BS(4,                                      ref_layer_id);
                SKIP_BS(5,                                      shape_hor_sampling_factor_n);
                SKIP_BS(5,                                      shape_hor_sampling_factor_m);
                SKIP_BS(5,                                      shape_vert_sampling_factor_n);
                SKIP_BS(5,                                      shape_vert_sampling_factor_m);
            TEST_BS_END();
        }
        SKIP_BS(1,                                              resync_marker_disable);
        //TODO: ...
    }

    //BS_END(); //Not possible because all stream analysis is not done

    //Coherancy
    if (Width==0 || Height==0 || Width/Height<0.2 || Width/Height>5)
        return;

    //Continue only if not already done
    if (Count_Get(Stream_Video)>0)
        return;

    //Filling
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);
    Fill("Codec", "MPEG-4V");
    Stream_Prepare(Stream_Video);
    Fill("Codec", "MPEG-4V");

    if (fixed_vop_time && vop_time_increment_resolution)
    {
        if (vop_time_increment_resolution==0xFFFF && fixed_vop_time==2733)
            Fill("FrameRate", 23.976); //Rounding with this kind of values is not precise
        else if (vop_time_increment_resolution==0xFFFF && fixed_vop_time==2186)
            Fill("FrameRate", 29.97); //Rounding with this kind of values is not precise
        else
            Fill("FrameRate", ((float)vop_time_increment_resolution)/fixed_vop_time);
    }
    if (Height)
    {
        Fill("Width", Width);
        Fill("Height", Height);
        float PixelAspectRatio_Value=1;
             if (PixelAspectRatio==0x01) PixelAspectRatio_Value=1;
             if (PixelAspectRatio==0x02) PixelAspectRatio_Value=12.0/11.0;
        else if (PixelAspectRatio==0x03) PixelAspectRatio_Value=10.0/11.0;
        else if (PixelAspectRatio==0x04) PixelAspectRatio_Value=16.0/11.0;
        else if (PixelAspectRatio==0x05) PixelAspectRatio_Value=40.0/13.0;
        else if (PixelAspectRatio==0x0F && PixelAspectRatio_Height) PixelAspectRatio_Value=((float)PixelAspectRatio_Width)/PixelAspectRatio_Height;
        Fill("AspectRatio", ((float)Width)/Height*PixelAspectRatio_Value);
    }
    Fill("Resolution", Resolution);
    if (Chroma<4)
        Fill("Chroma", Mpeg4v_Chroma[Chroma]);
    if (PictureStructure<4)
        Fill("Interlacement", Mpeg4v_PictureStructure[PictureStructure]);
    if (BVOP)
    {
        Fill("Codec_Settings", "BVOP");
        Fill("Codec_Settings/BVOP", "Yes");
    }
    else
        Fill("Codec_Settings/BVOP", "No");
    if (GMC)
    {
        Fill("Codec_Settings", Ztring(_T("GMC"))+Ztring::ToZtring(GMC));
        Fill("Codec_Settings/GMC", GMC);
    }
    else
        Fill("Codec_Settings/GMC", 0);
    if (QPel)
    {
        Fill("Codec_Settings", "QPel");
        Fill("Codec_Settings/QPel", "Yes");
    }
    else
        Fill("Codec_Settings/QPel", "No");
    if (load_intra_quant_mat_grayscale || load_nonintra_quant_mat_grayscale)
    {
        Fill("Codec_Settings", "Custom Matrix (Gray)");
        Fill("Codec_Settings/Matrix", "Custom (Gray)");
    }
    else if (load_intra_quant_mat || load_nonintra_quant_mat)
    {
        Fill("Codec_Settings", "Custom Matrix");
        Fill("Codec_Settings/Matrix", "Custom");
    }
    else
        Fill("Codec_Settings/Matrix", "Default");
}

//---------------------------------------------------------------------------
// Packet "40"
//
void File_Mpeg4v::fgs_bp_start()
{
    NAME("fgs_bp_start")
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "B0"
void File_Mpeg4v::visual_object_sequence_start()
{
    NAME("visual_object_sequence_start");
    NEXTFRAME_CLEAR();

    //Reading
    int32u Profile_Level;
    BS_BEGIN();
    GET_BS (8, Profile_Level,                                   profile_and_level_indication) INFO(Mpeg4v_Profile_Level(Profile_Level));
    BS_END();
}

//---------------------------------------------------------------------------
// Packet "B1"
void File_Mpeg4v::visual_object_sequence_end()
{
    NAME("visual_object_sequence_end");
    NEXTFRAME_CLEAR();
    INTEGRITY_GENERAL(0);
}

//---------------------------------------------------------------------------
// Packet "B2", User defined size, this is often used of library name
void File_Mpeg4v::user_data_start()
{
    NAME("user_data_start");
    NEXTFRAME_CLEAR();
    INTEGRITY_GENERAL(0);

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
    if (CC4(Buffer+Buffer_Offset)==CC4("XviD"))
        Library_Offset=0; //Exception, should do better
    else if (Buffer_Offset+Library_Offset_Final>=Buffer_Offset+Library_Offset)
    {
        Library_Size -=Library_Offset_Final-Library_Offset;
        Library_Offset=Library_Offset_Final;
    }

    //Filling
    Ztring Temp;
    Temp.From_Local((const char*)Buffer+Buffer_Offset+Library_Offset, 0, Library_Size);

    //Special cases
         if (Temp.find(_T("build"))==0) Library+=Ztring(_T(" "))+Temp;
    else Library=Temp;

    //Details
    if (Config.Details_Get())
    {
        Details_Add_Info(Buffer_Offset, "Library", Library);
    }

}

//---------------------------------------------------------------------------
// Packet "B3"
void File_Mpeg4v::group_of_vop_start()
{
    NAME("group_of_vop_start");
    NEXTFRAME_CLEAR();
    INTEGRITY_GENERAL(0);
}

//---------------------------------------------------------------------------
// Packet "B4"
void File_Mpeg4v::video_session_error()
{
    NAME("video_session_error");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "B5"
void File_Mpeg4v::visual_object_start()
{
    NAME("visual_object_start");
    NEXTFRAME_CLEAR();

    //Reading
    int32u FrameType, VersionID;
    BS_BEGIN();
    TEST_BS_SKIP( 1,                                            is_visual_object_identifier)
        GET_BS ( 4, VersionID,                                  visual_object_verid)  INFO(Mpeg4v_verid[VersionID]);
        SKIP_BS( 3,                                             visual_object_priority)
    TEST_BS_END();
    GET_BS ( 4, FrameType,                                      visual_object_type) INFO(Mpeg4v_visual_object_type[FrameType]);
    if (FrameType==1)
    {
        TEST_BS_SKIP(1,                                         video_signal_type)
            SKIP_BS(3,                                          video_format)
            SKIP_BS(1,                                          video_range)
            TEST_BS_SKIP(1,                                     colour_description)
                SKIP_BS(8,                                      colour_primaries)
                SKIP_BS(8,                                      transfer_characteristics)
                SKIP_BS(8,                                      matrix_coefficients)
            TEST_BS_END();
        TEST_BS_END();
        BS_END(); //Other FrameType need some work
    }
}

//---------------------------------------------------------------------------
// Packet "B6"
void File_Mpeg4v::vop_start()
{
    //Counting
    Frame_Count++;

    //Name
    NAME("vop_start");
    NAME(Ztring(_T("Frame ")+Ztring::ToZtring(Frame_Count)));
    NEXTFRAME_CLEAR();
    INTEGRITY_GENERAL(0);

    //Parsing
    int32u vop_coding_type, vop_coded;
    BS_BEGIN();
    GET_BS (2, vop_coding_type,                                 vop_coding_type);
    NAME(Mpeg4v_vop_coding_type[vop_coding_type]);
    int32u modulo_time_base_Continue;
    int32u modulo_time_base=0;
    do
    {
        GET_BS (1, modulo_time_base_Continue,                   modulo_time_base);
        modulo_time_base++;
    }
    while (modulo_time_base_Continue);
    MARK_1 (                                                    marker_bit);
    INFO_BS(time_size, vop_time_increment,                      vop_time_increment); if (vop_time_increment_resolution) INFO(vop_time_increment*1000/vop_time_increment_resolution, " ms");
    MARK_1 (                                                    marker_bit);
    GET_BS (1, vop_coded,                                       vop_coded);
    if (vop_coded)
    {
        if (newpred_enable)
        {
            SKIP_BS(time_size+3<15?time_size+3:15,              vop_id);
            TEST_BS_SKIP(1,                                     vop_id_for_prediction_indication);
                SKIP_BS(time_size+3<15?time_size+3:15,          vop_id_for_prediction);
            TEST_BS_END();
            MARK_1 (                                            marker_bit);
        }

        if (shape!=2              //Shape!=BinaryOnly
         && (vop_coding_type==1   //Type=P
          || (vop_coding_type==3  //Type=S
           && sprite_enable==2))) //Sprite_Enable=GMC
            SKIP_BS(1,                                          vop_rounding_type);
        if (reduced_resolution_vop_enable==1
         && shape==0                         //Shape=Rectangular
         && (vop_coding_type==1              //Type=P
          || vop_coding_type==0))            //Type=I
            SKIP_BS(1,                                          vop_reduced_resolution);
        if (shape!=0) //Shape!=Rectangular
        {
            if (sprite_enable==1    //Sprite_Enable=Static
             && vop_coding_type==0) //Type=I
            {
                SKIP_BS(13,                                     vop_width);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     vop_height);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     vop_horizontal_mc_spatial_ref);
                MARK_1 (                                        marker_bit);
                SKIP_BS(13,                                     vop_vertical_mc_spatial_ref);
                MARK_1 (                                        marker_bit);
            }
            if (shape!=2             //Shape=BinaryOnly
             && scalability==1
             && enhancement_type==1)
                SKIP_BS(1,                                      background_composition);
            SKIP_BS(1,                                          change_conv_ratio_disable);
            TEST_BS_SKIP(1,                                     vop_constant_alpha);
                SKIP_BS(8,                                      vop_constant_alpha_value);
            TEST_BS_END();
        }

        if (shape!=2) //Shape=BinaryOnly
            if (complexity_estimation_disable==0)
                return; //TODO: parse it, but this value is not common
        if (shape!=2) //Shape=BinaryOnly
        {
            SKIP_BS(3,                                          intra_dc_vlc_thr);
            if (interlaced)
            {
                int32u top_field_first;
                GET_BS (1, top_field_first,                     top_field_first)
                if (top_field_first)
                    PictureStructure=1; //Top first
                else
                    PictureStructure=2; //Bottom first
                SKIP_BS(1,                                      alternate_vertical_scan_flag);
            }
        }
        //...
    }

    if (!vop_coded)              NVOP_Count++; //VOP with no data
    else if (vop_coding_type==0) IVOP_Count++; //Type I
    else if (vop_coding_type==1) PVOP_Count++; //Type P
    else if (vop_coding_type==2) BVOP_Count++; //Type B
    else if (vop_coding_type==3) SVOP_Count++; //Type S

    //Specific
    if (File_Name.empty())
    {
        RIFF_VOP_Count++;
        if (RIFF_VOP_Count>RIFF_VOP_Count_Max)
            RIFF_VOP_Count_Max=RIFF_VOP_Count;
    }

    //Jumping to the end of the file
    if (Count_Get(Stream_Video)!=0 && Frame_Count>=Frame_Count_Valid)
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("-------------------------------------------");
            Details_Add_Error("---   MPEG-4V, Jumping to end of file   ---");
            Details_Add_Error("-------------------------------------------");
        }

        //Jumping
        File_GoTo=File_Size;
    }
}

//---------------------------------------------------------------------------
// Packet "B7"
void File_Mpeg4v::slice_start()
{
    NAME("slice_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "B8"
void File_Mpeg4v::extension_start()
{
    NAME("extension_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "B9"
void File_Mpeg4v::fgs_vop_start()
{
    NAME("fgs_vop_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BA"
void File_Mpeg4v::fba_object_start()
{
    NAME("fba_object_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BB"
void File_Mpeg4v::fba_object_plane_start()
{
    NAME("fba_object_plane_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BC"
void File_Mpeg4v::mesh_object_start()
{
    NAME("mesh_object_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BD"
void File_Mpeg4v::mesh_object_plane_start()
{
    NAME("mesh_object_plane_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BE"
void File_Mpeg4v::still_texture_object_start()
{
    NAME("still_texture_object_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "BF"
void File_Mpeg4v::texture_spatial_layer_start()
{
    NAME("texture_spatial_layer_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "C0"
void File_Mpeg4v::texture_snr_layer_start()
{
    NAME("texture_snr_layer_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "C1"
void File_Mpeg4v::texture_tile_start()
{
    NAME("texture_tile_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "C2"
void File_Mpeg4v::texture_shape_layer_start()
{
    NAME("texture_shape_layer_start");
    NEXTFRAME_CLEAR();
}

//---------------------------------------------------------------------------
// Packet "C3"
void File_Mpeg4v::system_start()
{
    NAME("system_start");
    NEXTFRAME_CLEAR();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg4v::Detect_NonMPEG4V ()
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
bool File_Mpeg4v::Element_Size_Get()
{
    Element_Next=Buffer_Offset+4;
    while(Element_Next+3<Buffer_Size && CC3(Buffer+Element_Next)!=0x000001)
        Element_Next++;
    if (Element_Next+3>=Buffer_Size)
    {
        //No next element
        if (File_Offset+Element_Next+3>=File_Size)
            Element_Next=Buffer_Size; //This is the last frame
        else if (Element_Next-Buffer_Offset>=65536) //False positive, no element should have more than this size
        {
            //Details
            if (Config.Details_Get())
            {
                ELEMENT_END();
                Details_Add_Info(Error, "Next element not found!", Element_Next-Buffer_Offset);
                ELEMENT_BEGIN();
            }

            File_Offset=File_Size;
            return false;
        }
        else
            return false; //Other byte will come
    }
    //Filling
    Element_Size=Element_Next-Buffer_Offset-4;

    return true;
}

//---------------------------------------------------------------------------
bool File_Mpeg4v::Element_Name_IsOK()
{
    //Is it the start of a MPEG-4V?
    if (Count_Get(Stream_General)==0
     && !(Element_Name==0x00
       || Element_Name==0xB0
       || Element_Name==0xB5)
    )
    {
        //MPEG-4V didn't begin, skipping
        //Details
        if (Config.Details_Get()>0.9)
        {
            Details_Add_Element(0, Element_Name, Element_Size-4);
            Details_Add_Element("Has not began, skipping");
        }
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
//"xxx_quant_mat" reading
Ztring Mpeg4v_Quant_Mat_Read(BitStream &BS)
{
    Ztring ToReturn; ToReturn.reserve(64);
    int32u Value;
    size_t Pos=0;

    //Reading from BitStream
    do
    {
        Value=BS.Get(8);
        ToReturn+=Ztring::ToZtring(Value, 16);
        Pos++;
    }
    while (Pos<64);

    //Completing with automatic values

    //Returning
    ToReturn.MakeUpperCase();
    return ToReturn;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg4v::HowTo(stream_t StreamKind)
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4V_*

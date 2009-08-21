// File_Mpeg_Descriptors - Info for MPEG files
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
#if defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg_Descriptors.h"
#include "ZenLib/BitStream.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* Mpeg_Descriptors_audio_type(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "Undefined";
        case 0x01 : return "Clean effects";
        case 0x02 : return "Hearing impaired";
        case 0x03 : return "Visual impaired commentary";
        default   : return "Reserved";
    }
}

const char* Mpeg_Descriptors_alignment_type(int8u alignment_type)
{
    switch (alignment_type)
    {
        case 0x01 : return "Slice or video access unit (Viedo), or sync word (Audio)";
        case 0x02 : return "Video access unit";
        case 0x03 : return "GOP, or SEQ";
        case 0x04 : return "SEQ";
        default   : return "Reserved";
    }
}

const char* Mpeg_Descriptors_teletext_type(int8u teletext_type)
{
    switch (teletext_type)
    {
        case 0x01 : return "initial Teletext page";
        case 0x02 : return "Teletext subtitle page";
        case 0x03 : return "additional information page";
        case 0x04 : return "programme schedule page";
        case 0x05 : return "Teletext subtitle page for hearing impaired people";
        default   : return "reserved for future use";
    }
}

const char* Mpeg_Descriptors_component_type_O3(int8u component_type)
{
    switch (component_type)
    {
        case 0x01 : return "EBU Teletext subtitles";
        case 0x02 : return "associated EBU Teletext";
        case 0x03 : return "VBI data";
        case 0x10 : return "DVB subtitles (normal) with no monitor aspect ratio criticality";
        case 0x11 : return "DVB subtitles (normal) for display on 4:3 aspect ratio monitor";
        case 0x12 : return "DVB subtitles (normal) for display on 16:9 aspect ratio monitor";
        case 0x13 : return "DVB subtitles (normal) for display on 2.21:1 aspect ratio monitor";
        case 0x20 : return "DVB subtitles (for the hard of hearing) with no monitor aspect ratio criticality";
        case 0x21 : return "DVB subtitles (for the hard of hearing) for display on 4:3 aspect ratio monitor";
        case 0x22 : return "DVB subtitles (for the hard of hearing) for display on 16:9 aspect ratio monitor";
        case 0x23 : return "DVB subtitles (for the hard of hearing) for display on 2.21:1 aspect ratio monitor";
        default   :
            if (component_type>=0xB0 && component_type<=0xFE)
                    return "user defined";
            else
                    return "reserved for future use";
    }
}

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGV_YES)
    extern const float Mpegv_FrameRate[]; //In Video/File_Mpegv.cpp
    extern const char* Mpegv_Chroma[]; //In Video/File_Mpegv.cpp
    extern const char* Mpegv_Profile[]; //In Video/File_Mpegv.cpp
    extern const char* Mpegv_Level[]; //In Video/File_Mpegv.cpp
#else //MEDIAINFO_MPEGV_YES
    const float Mpegv_FrameRate[]=
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

    const char* Mpegv_Chroma[]=
    {
        "",
        "4:2:0",
        "4:2:2",
        "4:4:4",
    };

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
#endif //MEDIAINFO_MPEGV_YES

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPEGA_YES)
    extern const char* Mpega_Version[]; //In Audio/File_Mpega.cpp
    extern const char* Mpega_Layer[]; //In Audio/File_Mpega.cpp
#else //MEDIAINFO_MPEGA_YES
    const char* Mpega_Version[4]=
    {
        "MPA2.5",
        "",
        "MPA2",
        "MPA1"
    };

    const char* Mpega_Layer[4]=
    {
        "",
        "L3",
        "L2",
        "L1",
    };
#endif //MEDIAINFO_MPEGA_YES

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AC3_YES)
    extern const int32u AC3_SamplingRate[]; //In Audio/File_Ac3.cpp
    extern const int16u AC3_BitRate[]; //In Audio/File_Ac3.cpp
    extern const char*  AC3_ChannelPositions[]; //In Audio/File_Ac3.cpp
    extern const int16u AC3_Channels[]; //In Audio/File_Ac3.cpp
    extern const char*  AC3_Mode[]; //In Audio/File_Ac3.cpp
    extern const char*  AC3_Surround[]; //In Audio/File_Ac3.cpp
#else //MEDIAINFO_AC3_YES
    const int32u AC3_SamplingRate[]=
    { 48000,  44100,  32000,      0,};

    const int16u AC3_BitRate[]=
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

    extern const int16u AC3_Channels[]=
    {2, 1, 2, 3, 3, 4, 4, 5};

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

    const char*  AC3_Surround[]=
    {
        "",
        "(No surround)",
        "(Surround)",
        "",
    };
#endif //MEDIAINFO_AC3_YES

const char* Mpeg_Descriptors_AC3_Channels[]=
{
    "1",
    "2",
    "2",
    "2",
    "3+",
    "6+",
    "",
    "",
};

const char* Mpeg_Descriptors_AC3_Priority[]=
{
    "",
    "Primary Audio",
    "Other Audio",
    "",
};

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg_Descriptors::File_Mpeg_Descriptors()
{
    KindOfStream=Stream_Max;
    format_identifier=0x00000000;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Read_Buffer_Continue()
{
    while (Buffer_Parse());

    FINNISHED();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg_Descriptors::Buffer_Parse()
{
    //Parsing
    if (!Element_Parse())
        return false;

    //Positioning
    Buffer_Offset+=Element_Size;

    return true;
}

//---------------------------------------------------------------------------
bool File_Mpeg_Descriptors::Element_Parse()
{
    if (Buffer_Offset+2>Buffer_Size)
        return false; //Finnished

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=2;
    CC_BEGIN();
    GET_B1 (Element_Name,                                       descriptor_tag);
    GET_B1 (Element_Size,                                       descriptor_length);
    CC_END();
    ELEMENT(0, Ztring().From_CC2(Element_Name), Element_Size);

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Positionning
    Buffer_Offset+=2;

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case 0x##_NAME : NAME(_DETAIL); Descriptor_##_NAME(); break;

    switch (Element_Name)
    {
        ELEMENT_CASE(00, "Reserved");
        ELEMENT_CASE(01, "Reserved");
        ELEMENT_CASE(02, "video_stream");
        ELEMENT_CASE(03, "audio_stream");
        ELEMENT_CASE(04, "hierarchy");
        ELEMENT_CASE(05, "registration");
        ELEMENT_CASE(06, "data_stream_alignment");
        ELEMENT_CASE(07, "target_background_grid");
        ELEMENT_CASE(08, "Video_window");
        ELEMENT_CASE(09, "CA");
        ELEMENT_CASE(0A, "ISO_639_language");
        ELEMENT_CASE(0B, "System_clock");
        ELEMENT_CASE(0C, "Multiplex_buffer_utilization");
        ELEMENT_CASE(0D, "Copyright");
        ELEMENT_CASE(0E, "Maximum_bitrate");
        ELEMENT_CASE(0F, "Private_data_indicator");
        ELEMENT_CASE(10, "Smoothing_buffer");
        ELEMENT_CASE(11, "STD");
        ELEMENT_CASE(12, "IBP");
        ELEMENT_CASE(13, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(14, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(15, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(16, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(17, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(18, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(19, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(1A, "Defined in ISO/IEC 13818-6");
        ELEMENT_CASE(1B, "MPEG-4_video");
        ELEMENT_CASE(1C, "MPEG-4_audio");
        ELEMENT_CASE(1D, "IOD");
        ELEMENT_CASE(1E, "SL");
        ELEMENT_CASE(1F, "FMC");
        ELEMENT_CASE(20, "External_ES_ID");
        ELEMENT_CASE(21, "MuxCode");
        ELEMENT_CASE(22, "FmxBufferSize");
        ELEMENT_CASE(23, "MultiplexBuffer");
        ELEMENT_CASE(40, "DVB - network_name_descriptor");
        ELEMENT_CASE(41, "DVB - service_list_descriptor");
        ELEMENT_CASE(42, "DVB - stuffing_descriptor");
        ELEMENT_CASE(43, "DVB - satellite_delivery_system_descriptor");
        ELEMENT_CASE(44, "DVB - cable_delivery_system_descriptor");
        ELEMENT_CASE(45, "DVB - VBI_data_descriptor");
        ELEMENT_CASE(46, "DVB - VBI_teletext_descriptor");
        ELEMENT_CASE(47, "DVB - bouquet_name_descriptor");
        ELEMENT_CASE(48, "DVB - service_descriptor");
        ELEMENT_CASE(49, "DVB - country_availability_descriptor");
        ELEMENT_CASE(4A, "DVB - linkage_descriptor");
        ELEMENT_CASE(4B, "DVB - NVOD_reference_descriptor");
        ELEMENT_CASE(4C, "DVB - time_shifted_service_descriptor");
        ELEMENT_CASE(4D, "DVB - short_event_descriptor");
        ELEMENT_CASE(4E, "DVB - extended_event_descriptor");
        ELEMENT_CASE(4F, "DVB - time_shifted_event_descriptor");
        ELEMENT_CASE(50, "DVB - component_descriptor");
        ELEMENT_CASE(51, "DVB - mosaic_descriptor");
        ELEMENT_CASE(52, "DVB - stream_identifier_descriptor");
        ELEMENT_CASE(53, "DVB - CA_identifier_descriptor");
        ELEMENT_CASE(54, "DVB - content_descriptor");
        ELEMENT_CASE(55, "DVB - parental_rating_descriptor");
        ELEMENT_CASE(56, "DVB - teletext_descriptor");
        ELEMENT_CASE(57, "DVB - telephone_descriptor");
        ELEMENT_CASE(58, "DVB - local_time_offset_descriptor");
        ELEMENT_CASE(59, "DVB - subtitling_descriptor");
        ELEMENT_CASE(5A, "DVB - terrestrial_delivery_system_descriptor");
        ELEMENT_CASE(5B, "DVB - multilingual_network_name_descriptor");
        ELEMENT_CASE(5C, "DVB - multilingual_bouquet_name_descriptor");
        ELEMENT_CASE(5D, "DVB - multilingual_service_name_descriptor");
        ELEMENT_CASE(5E, "DVB - multilingual_component_descriptor");
        ELEMENT_CASE(5F, "DVB - private_data_specifier_descriptor");
        ELEMENT_CASE(60, "DVB - service_move_descriptor");
        ELEMENT_CASE(61, "DVB - short_smoothing_buffer_descriptor");
        ELEMENT_CASE(62, "DVB - frequency_list_descriptor");
        ELEMENT_CASE(63, "DVB - partial_transport_stream_descriptor");
        ELEMENT_CASE(64, "DVB - data_broadcast_descriptor");
        ELEMENT_CASE(65, "DVB - scrambling_descriptor");
        ELEMENT_CASE(66, "DVB - data_broadcast_id_descriptor");
        ELEMENT_CASE(67, "DVB - transport_stream_descriptor");
        ELEMENT_CASE(68, "DVB - DSNG_descriptor");
        ELEMENT_CASE(69, "DVB - PDC_descriptor");
        ELEMENT_CASE(6A, "DVB - AC-3_descriptor");
        ELEMENT_CASE(6B, "DVB - ancillary_data_descriptor");
        ELEMENT_CASE(6C, "DVB - cell_list_descriptor");
        ELEMENT_CASE(6D, "DVB - cell_frequency_link_descriptor");
        ELEMENT_CASE(6E, "DVB - announcement_support_descriptor");
        ELEMENT_CASE(6F, "DVB - application_signalling_descriptor");
        ELEMENT_CASE(70, "DVB - adaptation_field_data_descriptor");
        ELEMENT_CASE(71, "DVB - service_identifier_descriptor");
        ELEMENT_CASE(72, "DVB - service_availability_descriptor");
        ELEMENT_CASE(73, "DVB - default_authority_descriptor");
        ELEMENT_CASE(74, "DVB - related_content_descriptor");
        ELEMENT_CASE(75, "DVB - TVA_id_descriptor");
        ELEMENT_CASE(76, "DVB - content_identifier_descriptor");
        ELEMENT_CASE(77, "DVB - time_slice_fec_identifier_descriptor");
        ELEMENT_CASE(78, "DVB - ECM_repetition_rate_descriptor");
        ELEMENT_CASE(79, "DVB - S2_satellite_delivery_system_descriptor");
        ELEMENT_CASE(7A, "DVB - enhanced_AC-3_descriptor");
        ELEMENT_CASE(7B, "DVB - DTS descriptor");
        ELEMENT_CASE(7C, "DVB - AAC descriptor");
        ELEMENT_CASE(7D, "DVB - reserved for future use");
        ELEMENT_CASE(7E, "DVB - reserved for future use");
        ELEMENT_CASE(7F, "DVB - extension descriptor");
        ELEMENT_CASE(80, "ATSC - stuffing");
        ELEMENT_CASE(81, "ATSC - AC-3 audio");
        ELEMENT_CASE(86, "ATSC - caption service");
        ELEMENT_CASE(87, "ATSC - content advisory");
        ELEMENT_CASE(A0, "ATSC - extended channel name");
        ELEMENT_CASE(A1, "ATSC - service location");
        ELEMENT_CASE(A2, "ATSC - time-shifted service");
        ELEMENT_CASE(A3, "ATSC - component name");
        ELEMENT_CASE(A8, "ATSC - DCC Departing Request");
        ELEMENT_CASE(A9, "ATSC - DCC Arriving Request");
        ELEMENT_CASE(AA, "ATSC - Redistribution Control");
        ELEMENT_CASE(AB, "ATSC - DCC Location Code");
        default: if (Element_Name>=64)
                    NAME("user private")
                 else
                    NAME("unknown");
                 break;
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_02()
{
    //Parsing
    int32u multiple_frame_rate_flag, frame_rate_code, MPEG_1_only_flag;
    int32u profile_and_level_indication_profile=4, profile_and_level_indication_level=10, chroma_format=1, frame_rate_extension_flag=0;
    BS_BEGIN();
    GET_BS (1, multiple_frame_rate_flag,                        multiple_frame_rate_flag);
    GET_BS (4, frame_rate_code,                                 frame_rate_code); INFO(Mpegv_FrameRate[frame_rate_code]);
    GET_BS (1, MPEG_1_only_flag,                                MPEG_1_only_flag);
    SKIP_BS(1,                                                  constrained_parameter_flag);
    SKIP_BS(1,                                                  still_picture_flag);
    if (MPEG_1_only_flag==0)
    {
        SKIP_BS(1,                                              profile_and_level_indication_escape);
        GET_BS (3, profile_and_level_indication_profile,        profile_and_level_indication_profile); INFO(Mpegv_Profile[profile_and_level_indication_profile]);
        GET_BS (4, profile_and_level_indication_level,          profile_and_level_indication_level); INFO(Mpegv_Level[profile_and_level_indication_level]);
        GET_BS (2, chroma_format,                               chroma_format); INFO(Mpegv_Chroma[chroma_format]);
        GET_BS (1, frame_rate_extension_flag,                   frame_rate_extension_flag);
        SKIP_BS(5,                                              reserved);
    }
    BS_END();

    //Filling
    if (!multiple_frame_rate_flag && !frame_rate_extension_flag)
        Info[_T("FrameRate")]=Ztring::ToZtring(Mpegv_FrameRate[frame_rate_code]);
    Info[_T("Chroma")]=Mpegv_Chroma[chroma_format];
    Info[_T("Codec_Profile")]=Ztring().From_Local(Mpegv_Profile[profile_and_level_indication_profile])+_T("@")+Ztring().From_Local(Mpegv_Level[profile_and_level_indication_level]);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_03()
{
    //Parsing
    int32u ID, layer, variable_rate_audio_indicator;
    BS_BEGIN();
    SKIP_BS(1,                                                  free_format_flag);
    GET_BS (1, ID,                                              ID); INFO(Mpega_Version[2+ID]); //Mpega_Version is with MPEG2.5 hack
    GET_BS (2, layer,                                           layer);  INFO(Mpega_Layer[layer]);
    GET_BS (1, variable_rate_audio_indicator,                   variable_rate_audio_indicator);
    SKIP_BS(3,                                                  reserved);
    BS_END();

    //Filling
    Info[_T("BitRate/Mode")]=variable_rate_audio_indicator?_T("VBR"):_T("CBR");
    Info[_T("Codec")]=Ztring(Mpega_Version[ID])+Ztring(Mpega_Layer[layer]);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_05()
{
    //Parsing
    CC_BEGIN();
    GET_B4 (format_identifier,                                  format_identifier);
    CC_END_CANBEMORE();

    //Filling
    switch (format_identifier)
    {
        case Mpeg_Descriptors::AC_3 : INFO(_T("AC3")); KindOfStream=Stream_Audio; Info[_T("Codec")]=_T("AC3"); break;
        case Mpeg_Descriptors::DTS1 : INFO(_T("DTS")); KindOfStream=Stream_Audio; Info[_T("Codec")]=_T("DTS"); break;
        case Mpeg_Descriptors::GA94 : INFO(_T("ATSC"));
        case Mpeg_Descriptors::HDMV : INFO(_T("Blu-ray"));
        default : ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_06()
{
    //Parsing
    CC_BEGIN();
    INFO_B1(alignment_type,                                     alignment_type); INFO(Mpeg_Descriptors_alignment_type(alignment_type));
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_09()
{
    //Parsing
    int32u CA_PID;
    CC_BEGIN()
    SKIP_B2(                                                    CA_system_ID);
    CC_BS_BEGIN()
    SKIP_BS( 3,                                                 reserved);
    GET_BS (13, CA_PID,                                         CA_PID);
    CC_BS_END();
    SKIP_XX(Element_Size-Stream_Pos,                            private_data_byte);
    CC_END();

    //Filling
    Kind[CA_PID]=File_MpegTs::conditional_access_table; //TODO: DOES NOT WORK
    //Info[_T("CA_PID")]=Ztring::ToZtring(CA_PID);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0A()
{
    //Parsing
    Ztring ISO_639_language_code;
    int32u audio_type;
    CC_BEGIN();
    GET_LOCAL(3, ISO_639_language_code,                         ISO_639_language_code);
    GET_B1 (audio_type,                                         audio_type); INFO(Mpeg_Descriptors_audio_type(audio_type));
    CC_END();

    //Filling
    Info[_T("Language")]=ISO_639_language_code;
    if (audio_type)
        Info[_T("Language_More")]=Mpeg_Descriptors_audio_type(audio_type);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0E()
{
    //Parsing
    int32u maximum_bitrate;
    BS_BEGIN();
    SKIP_BS( 2,                                                 reserved);
    GET_BS (22, maximum_bitrate,                                maximum_bitrate);
    BS_END();

    //Filling
    Info[_T("BitRate/Max")]=Ztring::ToZtring(maximum_bitrate);
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_0F()
{
    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    private_data_indicator);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_10()
{
    //Parsing
    BS_BEGIN()
    SKIP_BS( 2,                                                 reserved);
    INFO_BS(22, sb_leak_rate,                                   sb_leak_rate); INFO(sb_leak_rate*400, " bps");
    SKIP_BS( 2,                                                 reserved);
    INFO_BS(22, sb_size,                                        sb_size); INFO(sb_size, " bytes");
    BS_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_48()
{
    //Parsing
    int8u service_type, service_provider_name_length, service_name_length;
    CC_BEGIN();
    GET_B1 (service_type,                                       service_type);
    GET_B1 (service_provider_name_length,                       service_provider_name_length);
    SKIP_LOCAL(service_provider_name_length,                    service_provider_name);
    GET_B1 (service_name_length,                                service_name_length);
    SKIP_LOCAL(service_name_length,                             service_name);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_4D()
{
    //Parsing
    int8u event_name_length, text_length;
    CC_BEGIN();
    SKIP_LOCAL(3,                                               ISO_639_language_code);
    GET_B1 (event_name_length,                                  event_name_length);
    SKIP_LOCAL(event_name_length,                               event_name);
    GET_B1 (text_length,                                        text_length);
    SKIP_LOCAL(text_length,                                     text);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_50()
{
    //Parsing
    Ztring ISO_639_language_code;
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(4,                                                  reserved_future_use);
    SKIP_BS(4,                                                  stream_content);
    CC_BS_END()
    SKIP_B1(                                                    component_type);
    SKIP_B1(                                                    component_tag);
    GET_LOCAL(3, ISO_639_language_code,                         ISO_639_language_code);
    SKIP_LOCAL(Element_Size-Stream_Pos,                         text);
    CC_END();

    //Filling
    Info[_T("Language")]=ISO_639_language_code;
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_52()
{
    //Parsing
    CC_BEGIN();
    SKIP_B1(                                                    component_tag);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_54()
{
    //Parsing
    BS_BEGIN();
    while (BS.Offset_Get()<Element_Size)
    {
        SKIP_BS(4,                                              content_nibble_level_1);
        SKIP_BS(4,                                              content_nibble_level_2);
        SKIP_BS(4,                                              user_nibble);
        SKIP_BS(4,                                              user_nibble);
    }
    BS_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_56()
{
    //Parsing
    Ztring ISO_639_language_code;
    CC_BEGIN();
    while (Stream_Pos<Element_Size)
    {
        GET_LOCAL(3, ISO_639_language_code,                         ISO_639_language_code);
        CC_BS_BEGIN()
        INFO_BS(5, teletext_type,                                   teletext_type); INFO(Mpeg_Descriptors_teletext_type(teletext_type));
        SKIP_BS(3,                                                  teletext_magazine_number);
        SKIP_BS(4,                                                  teletext_page_number_1);
        SKIP_BS(4,                                                  teletext_page_number_2);
        CC_BS_END()

        //Filling
        Info[_T("Language")]=ISO_639_language_code;
        //TODO: this stream is teletext. Be careful, multiple stream in a PID
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_59()
{
    //Parsing
    Ztring ISO_639_language_code;
    CC_BEGIN();
    while (Stream_Pos<Element_Size)
    {
        GET_LOCAL(3, ISO_639_language_code,                         ISO_639_language_code);
        INFO_B1(subtitling_type,                                    subtitling_type); INFO(Mpeg_Descriptors_component_type_O3(subtitling_type));
        SKIP_B2(                                                    composition_page_id);
        SKIP_B2(                                                    ancillary_page_id);

        //Filling
        Info[_T("Language")]=ISO_639_language_code;
        //TODO: this stream is subtitle. Be careful, multiple stream in a PID
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_6A()
{
    //Parsing
    BS_BEGIN()
    int32u component_type_flag, bsid_flag, mainid_flag, asvc_flag;
    GET_BS (1, component_type_flag,                             component_type_flag);
    GET_BS (1, bsid_flag,                                       bsid_flag);
    GET_BS (1, mainid_flag,                                     mainid_flag);
    GET_BS (1, asvc_flag,                                       asvc_flag);
    SKIP_BS(1,                                                  reserved_flag);
    SKIP_BS(1,                                                  reserved_flag);
    SKIP_BS(1,                                                  reserved_flag);
    SKIP_BS(1,                                                  reserved_flag);
    if (component_type_flag)
    {
        int32u service_type, number_of_channels;
        SKIP_BS(1, enhanced_ac3                                 enhanced AC-3);
        SKIP_BS(1,                                              full_service);
        GET_BS (3, service_type,                                service_type); INFO(AC3_Mode[service_type]);
        GET_BS (3, number_of_channels,                          number_of_channels); INFO(Mpeg_Descriptors_AC3_Channels[number_of_channels], " channels");
        Info[_T("Channel(s)")]=Ztring().From_Local(Mpeg_Descriptors_AC3_Channels[number_of_channels]);
    }
    if (bsid_flag)
    {
        SKIP_BS(3,                                              zero);
        SKIP_BS(5,                                              bsid);
    }
    if (mainid_flag)
    {
        SKIP_BS(8,                                              mainid);
    }
    if (asvc_flag)
    {
        SKIP_BS(8,                                              asvc);
    }
    BS_END_CANBEMORE();

    //Filling
    KindOfStream=Stream_Audio;
    Info[_T("Codec")]=_T("AC3");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7A()
{
    //Parsing
    BS_BEGIN()
    int32u component_type_flag, bsid_flag, mainid_flag, asvc_flag, mixinfoexists, substream1_flag, substream2_flag, substream3_flag, enhanced_ac3=0;
    GET_BS (1, component_type_flag,                             component_type_flag);
    GET_BS (1, bsid_flag,                                       bsid_flag);
    GET_BS (1, mainid_flag,                                     mainid_flag);
    GET_BS (1, asvc_flag,                                       asvc_flag);
    GET_BS (1, mixinfoexists,                                   mixinfoexists);
    GET_BS (1, substream1_flag,                                 substream1_flag);
    GET_BS (1, substream2_flag,                                 substream2_flag);
    GET_BS (1, substream3_flag,                                 substream3_flag);
    if (component_type_flag)
    {
        int32u service_type, number_of_channels;
        GET_BS (1, enhanced_ac3,                                enhanced AC-3);
        SKIP_BS(1,                                              full_service);
        GET_BS (3, service_type,                                service_type); INFO(AC3_Mode[service_type]);
        GET_BS (3, number_of_channels,                          number_of_channels); INFO(Mpeg_Descriptors_AC3_Channels[number_of_channels], " channels");
        Info[_T("Channel(s)")]=Ztring().From_Local(Mpeg_Descriptors_AC3_Channels[number_of_channels]);
    }
    if (bsid_flag)
    {
        SKIP_BS(3,                                              zero);
        SKIP_BS(5,                                              bsid);
    }
    if (mainid_flag)
    {
        SKIP_BS(8,                                              mainid);
    }
    if (asvc_flag)
    {
        SKIP_BS(8,                                              asvc);
    }
    if (substream1_flag)
    {
        SKIP_BS(8,                                              substream1);
    }
    if (substream2_flag)
    {
        SKIP_BS(8,                                              substream2);
    }
    if (substream3_flag)
    {
        SKIP_BS(8,                                              substream3);
    }
    BS_END_CANBEMORE();

    //Filling
    KindOfStream=Stream_Audio;
    Info[_T("Codec")]=enhanced_ac3?_T("AC3+"):_T("AC3");
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7B()
{
    //Parsing
    BS_BEGIN()
    SKIP_BS(6,                                                  bit_rate_code);
    SKIP_BS(7,                                                  nblks);
    SKIP_BS(14,                                                 fsize);
    SKIP_BS(6,                                                  surround_mode);
    SKIP_BS(1,                                                  lfe_flag);
    SKIP_BS(2,                                                  extended_surround_flag);
    BS_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_7C()
{
    //Parsing
    int32u AAC_type_flag;
    BS_BEGIN();
    SKIP_BS(8,                                                  Profile_and_level);
    GET_BS (1, AAC_type_flag,                                   AAC_type_flag);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    SKIP_BS(1,                                                  reserved);
    if (AAC_type_flag)
    {
        SKIP_BS(8,                                              AAC_type);
    }
    BS_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_81()
{
    //if (format_identifier!=Mpeg_Descriptors::GA94);
    //    return;

    //Parsing
    std::string Text, Language1, Language2;
    int32u sample_rate_code, bit_rate_code, surround_mode, bsmod, num_channels, langcod, textlen, text_code, language_flag, language_flag_2;
    BS_BEGIN();
    GET_BS (3, sample_rate_code,                                sample_rate_code); if (sample_rate_code<4) {INFO(AC3_SamplingRate[sample_rate_code], " Hz");}
    SKIP_BS(5,                                                  bsid);
    GET_BS (6, bit_rate_code,                                   bit_rate_code); INFO(AC3_BitRate[bit_rate_code]*1000, " Kbps");
    GET_BS (2, surround_mode,                                   surround_mode); INFO(AC3_Surround[surround_mode]);
    GET_BS (3, bsmod,                                           bsmod);
    GET_BS (4, num_channels,                                    num_channels); if (num_channels<8) {INFO(AC3_Channels[num_channels], " channels");}
    SKIP_BS(1,                                                  full_svc);

    //Filling
    if (sample_rate_code<4)
        Info[_T("SamplingRate")]=Ztring::ToZtring(AC3_SamplingRate[sample_rate_code]);
    Info[_T("BitRate")]=Ztring::ToZtring(AC3_BitRate[bit_rate_code]*1000);
    if (num_channels<8)
        Info[_T("Channel(s)")]=Ztring::ToZtring(AC3_Channels[num_channels]);

    //Parsing
    if (BS.Remain()==0) return;
    GET_BS (8, langcod,                                         langcod);

    //Parsing
    if (BS.Remain()==0) return;
    if (num_channels==0)
    {
        SKIP_BS(8,                                              langcod2);
    }

    //Parsing
    if (BS.Remain()==0) return;
    if (bsmod<2)
    {
        SKIP_BS(3,                                              mainid);
        INFO_BS(2, priority,                                    priority); INFO(Mpeg_Descriptors_AC3_Priority[priority]);
        SKIP_BS(3,                                              reserved);
    }
    else
    {
        SKIP_BS(8,                                              asvcflags);
    }

    //Parsing
    if (BS.Remain()==0) return;
    GET_BS (7, textlen,                                         textlen);
    GET_BS (1, text_code,                                       text_code); if (text_code) INFO("Unicode");
    for (size_t Pos=0; Pos<textlen; Pos++)
    {
        int32u text;
        GET_BS (8, text,                                        text); INFO((char)text);
        Text+=(char)text;
    }

    //Parsing
    if (BS.Remain()==0) return;
    GET_BS (1, language_flag,                                   language_flag);
    GET_BS (1, language_flag_2,                                 language_flag_2);
    SKIP_BS(6,                                                  all is 1);

    //Parsing
    if (BS.Remain()==0) return;
    if (language_flag)
    {
        int32u language1, language2, language3;
        GET_BS (8, language1,                                   language); INFO((char)language1);
        GET_BS (8, language2,                                   language); INFO((char)language2);
        GET_BS (8, language3,                                   language); INFO((char)language3);
        Language1+=(char)language1;
        Language1+=(char)language2;
        Language1+=(char)language3;
    }

    //Parsing
    if (BS.Remain()==0) return;
    if (language_flag_2)
    {
        int32u language1, language2, language3;
        GET_BS (8, language1,                                   language); INFO((char)language1);
        GET_BS (8, language2,                                   language); INFO((char)language2);
        GET_BS (8, language3,                                   language); INFO((char)language3);
        Language2+=(char)language1;
        Language2+=(char)language2;
        Language2+=(char)language3;
    }
    BS_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A0()
{
    //Parsing
    Ztring Value;
    ATSC_multiple_string_structure(Value);

    //Filling
    if (!Value.empty())
        Info[_T("Extended Channel Name")]=Value;
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A1()
{
    //Parsing
    int32u number_elements;
    CC_BEGIN();
    CC_BS_BEGIN();
    SKIP_BS( 3,                                                 reserved);
    SKIP_BS(13,                                                 PCR_PID);
    GET_BS ( 8, number_elements,                                number_elements);
    CC_BS_END();
    for (int32u Pos=0; Pos<number_elements; Pos++)
    {
        FLUSH();
        ELEMENT_BEGIN();
        int32u elementary_PID;
        CC_BS_BEGIN();
        SKIP_BS( 8,                                             stream_type);
        SKIP_BS( 3,                                             reserved);
        GET_BS (13, elementary_PID,                             elementary_PID);
        CC_BS_END();
        SKIP_LOCAL(3,                                           ISO_639_language_code);
        ELEMENT(1, elementary_PID, 6);
        ELEMENT_END();
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::Descriptor_A3()
{
    //Parsing
    Ztring Value;
    ATSC_multiple_string_structure(Value);

    //Filling
    if (!Value.empty())
        Info[_T("Name")]=Value;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Descriptors::ATSC_multiple_string_structure(Ztring &Value)
{
    //Parsing
    Ztring compressed_string_byte;
    int32u number_strings, number_segments, compression_type, mode, number_bytes;
    CC_BEGIN();
    GET_B1(number_strings,                                      number_strings);
    for (int32u Pos=0; Pos<number_strings; Pos++)
    {
        FLUSH();
        ELEMENT(Details_Level_Last+1, "String", 0);
        SKIP_LOCAL(3,                                           ISO_639_language_code);
        GET_B1(number_segments,                                 number_segments);
        for (int32u Pos=0; Pos<number_segments; Pos++)
        {
            FLUSH();
            ELEMENT(Details_Level_Last+1, "Segment", 0);
            GET_B1 (compression_type,                           compression_type);
            GET_B1 (mode,                                       mode);
            GET_B1 (number_bytes,                               number_bytes);
            if (compression_type==0)
            {
                GET_LOCAL(number_bytes, compressed_string_byte, compressed_string_byte);
            }
            else
                SKIP_XX(number_bytes,                           compressed_string_byte);
            ELEMENT_END();
        }
        ELEMENT_END();
    }
    CC_END();

    //Filling
    Value=compressed_string_byte;
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES

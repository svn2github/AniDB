// File_Mpeg_Psi - Info for MPEG Stream files
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
#if defined(MEDIAINFO_MPEGPS_YES) || defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
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
const char* Mpeg_Psi_table_id(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "program_association_section";
        case 0x01 : return "conditional_access_section";
        case 0x02 : return "program_map_section";
        case 0x03 : return "description_section";
        case 0x04 : return "ISO_IEC_14496_scene_description_section";
        case 0x05 : return "ISO_IEC_14496_object_descriptor_section";
        case 0x06 : return "Metadata";
        case 0x38 : return "ISO/IEC 13818-6 reserved";
        case 0x39 : return "DSM-CC addressable section";
        case 0x3A : return "DSM-CC, MPE";
        case 0x3B : return "DSM-CC, U-N messages, except DDM";
        case 0x3C : return "DSM-CC, DDM";
        case 0x3D : return "DSM-CC, stream descriptors";
        case 0x3E : return "DSM-CC, private data, IP-Datagram";
        case 0x3F : return "DSM-CC addressable section";
        case 0x40 : return "DVB - NIT – actual network";
        case 0x41 : return "DVB - NIT – other network";
        case 0x42 : return "DVB - SDT – actual TS";
        case 0x46 : return "DVB - SDT – other TS";
        case 0x4A : return "DVB - BAT";
        case 0x4B : return "DVB - UNT";
        case 0x4C : return "DVB - INT";
        case 0x4E : return "DVB - EIT – actual TS";
        case 0x4F : return "DVB - EIT – other TS";
        case 0x70 : return "DVB - TDT";
        case 0x71 : return "DVB - RST";
        case 0x72 : return "DVB - ST";
        case 0x73 : return "DVB - TOT";
        case 0x74 : return "DVB - AIT";
        case 0x78 : return "DVB - MPE-FEC";
        case 0x79 : return "DVB - RNT";
        case 0x7E : return "DVB - DIT";
        case 0x7F : return "DVB - SIT";
        case 0xC0 : return "ATSC - Program Information Message";
        case 0xC1 : return "ATSC - Program Name Message";
        case 0xC2 : return "ATSC/SCTE - Network Information Message";
        case 0xC3 : return "ATSC/SCTE - Network Text Table (NTT)";
        case 0xC4 : return "ATSC/SCTE - Short Form Virtual Channel Table (S-VCT)";
        case 0xC5 : return "ATSC/SCTE - System Time Table (STT)";
        case 0xC6 : return "ATSC/SCTE - Subtitle Message";
        case 0xC7 : return "ATSC - Master Guide Table (MGT)";
        case 0xC8 : return "ATSC - Terrestrial Virtual Channel Table (TVCT)";
        case 0xC9 : return "ATSC - Cable Virtual Channel Table (CVCT) / Long-form Virtual Channel Table (L-VCT)";
        case 0xCA : return "ATSC - Rating Region Table (RRT)";
        case 0xCB : return "ATSC - Event Information Table (EIT)";
        case 0xCC : return "ATSC - Extended Text Table (ETT)";
        case 0xCD : return "ATSC - System Time Table (STT)";
        case 0xCE : return "ATSC - Data Event Table (DET) ";
        case 0xCF : return "ATSC - Data Service Table (DST)";
        case 0xD0 : return "ATSC - Program Identifier Table (PIT)";
        case 0xD1 : return "ATSC - Network Resource Table (NRT)";
        case 0xD2 : return "ATSC - Long-term Service Table (L-TST)";
        case 0xD3 : return "ATSC - Directed Channel Change Table (DCCT)";
        case 0xD4 : return "ATSC - DCC Selection Code Table (DCCSCT)";
        case 0xD5 : return "ATSC - Selection Information Table (SIT)";
        case 0xD6 : return "ATSC - Aggregate Event Information Table (AEIT)";
        case 0xD7 : return "ATSC - Aggregate Extended Text Table (AETT)";
        case 0xD8 : return "ATSC - Cable Emergency Alert";
        case 0xD9 : return "ATSC - Aggregate Data Event Table";
        case 0xDA : return "ATSC - Satellite VCT";
        default :
            if (ID<=0x37) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved";
            if (ID>=0x50
             && ID<=0x5F) return "DVB - EITS – actual TS, Schedule";
            if (ID>=0x60
             && ID<=0x6F) return "DVB - EITS – other TS, Schedule";
            if (ID>=0x40
             && ID<=0x7F) return "DVB - reserved";
            if (ID>=0x80
             && ID<=0x8F) return "CA message, EMM, ECM";
            if (ID>=0xC0
             && ID<=0xDF) return "ATSC/SCTE - reserved";
            if (ID<=0xFE) return "User Private";
            return "forbidden";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_ATSC_table_type(int16u ID)
{
    switch (ID)
    {
        case 0x0000 : return "Terrestrial VCT with current_next_indicator=1";
        case 0x0001 : return "Terrestrial VCT with current_next_indicator=0";
        case 0x0002 : return "Cable VCT with current_next_indicator=1";
        case 0x0003 : return "Cable VCT with current_next_indicator==0";
        case 0x0004 : return "Channel ETT";
        case 0x0005 : return "DCCSCT";
        case 0x0010 : return "Short-form Virtual Channel Table—VCM Subtyp";
        case 0x0011 : return "Short-form Virtual Channel Table—DCM Subtyp";
        case 0x0012 : return "Short-form Virtual Channel Table—ICM Subtyp";
        case 0x0020 : return "Network Information Table—CDS Table Subtype";
        case 0x0021 : return "Network Information Table—MMS Table Subtype";
        case 0x0030 : return "Network Text Tabl e—SNS Subtype";
        default :
            if (ID>=0x0100
             && ID<=0x017F) return "Event Information Table (EIT)";
            if (ID>=0x0200
             && ID<=0x027F) return "Event Extended Text Table (EETT)";
            if (ID>=0x301
             && ID<=0x03FF) return "Rating Region Table (RRT)";
            if (ID>=0x0400
             && ID<=0x0FFF) return "User private";
            if (ID>=0x1000
             && ID<=0x10FF) return "Aggregate Event Information Table (AEIT)";
            if (ID>=0x1100
             && ID<=0x11FF) return "Aggregate Extended Text Table (AETT)";
            if (ID>=0x1400
             && ID<=0x14FF) return "DCCT";
            return "Reserved";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_stream_type(int32u ID)
{
    switch (ID)
    {
        case 0x00 : return "ITU-T | ISO/IEC Reserved";
        case 0x01 : return "ISO/IEC 11172 Video";
        case 0x02 : return "ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream";
        case 0x03 : return "ISO/IEC 11172 Audio";
        case 0x04 : return "ISO/IEC 13818-3 Audio";
        case 0x05 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections";
        case 0x06 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data";
        case 0x07 : return "ISO/IEC 13522 MHEG";
        case 0x08 : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC";
        case 0x09 : return "ITU-T Rec. H.222.1";
        case 0x0A : return "ISO/IEC 13818-6 type A";
        case 0x0B : return "ISO/IEC 13818-6 type B";
        case 0x0C : return "ISO/IEC 13818-6 type C";
        case 0x0D : return "ISO/IEC 13818-6 type D";
        case 0x0E : return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary";
        case 0x0F : return "ISO/IEC 13818-7 Audio with ADTS transport syntax";
        case 0x10 : return "ISO/IEC 14496-2 Visual";
        case 0x11 : return "ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1";
        case 0x12 : return "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets";
        case 0x13 : return "ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.";
        case 0x14 : return "ISO/IEC 13818-6 Synchronized Download Protocol";
        case 0x1B : return "H.264";
        case 0x80 : return "DigiCipher® II video";
        case 0x81 : return "ATSC - AC-3";
        case 0x82 : return "SCTE Standard Subtitle";
        case 0x83 : return "SCTE Isochronous Data";
        case 0x84 : return "ATSC Reserved";
        case 0x85 : return "ATSC Program Identifier";
        case 0x86 : return "SCTE 35 splice_information_table";
        case 0x87 : return "E-AC-3";
        case 0x88 : return "VC-9";
        case 0x90 : return "DVB - stream_type value for Time Slicing / MPE-FEC";
        case 0x95 : return "ATSC - Data Service Table, Network Resources Table";
        default :
            if (ID<=0x7F) return "ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved";
            if (ID<=0xFF) return "User Private";
            return "forbidden";
    }
}

//---------------------------------------------------------------------------
const char* Mpeg_Psi_stream_Codec(int32u ID)
{
    switch (ID)
    {
        case 0x01 : return "MPEG-1V";
        case 0x02 : return "MPEG-2V";
        case 0x03 : return "MPEG-1A";
        case 0x04 : return "MPEG-2A";
        case 0x0F : return "AAC";
        case 0x10 : return "MPEG-4V";
        case 0x11 : return "AAC";
        case 0x1B : return "AVC";
        default   : return "";
    }
}

//---------------------------------------------------------------------------
stream_t Mpeg_Psi_stream_Kind(int32u ID)
{
    switch (ID)
    {
        case 0x01 : return Stream_Video;
        case 0x02 : return Stream_Video;
        case 0x03 : return Stream_Audio;
        case 0x04 : return Stream_Audio;
        case 0x0F : return Stream_Audio;
        case 0x10 : return Stream_Video;
        case 0x11 : return Stream_Audio;
        case 0x1B : return Stream_Video;
        default   : return Stream_Max;
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Mpeg_Psi::File_Mpeg_Psi()
{
    //Configuration
    pid=0;
    payload_unit_start_indicator=0;
    From_TS=true; //Default is from TS

    //About programs
    program_number=0;

    //About streams
    elementary_PID=0;

    table_id=0xFF;
    Descriptor_format_identifier=0x00000000;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Read_Buffer_Init()
{
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Read_Buffer_Continue()
{
    //Payload
    if (payload_unit_start_indicator && !PayLoad_Parse())
        return;

    //From Program stream
    if (!From_TS)
    {
        program_stream_map();
        return;
    }

    //Header
    if (table_id==0xFF)
    {
        if (Buffer_Size==0)
            return;
        if (!table_id_Parse())
            return;
    }

    //Loop
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg_Psi::Buffer_Parse()
{
    //Element size
    Element_Size=Buffer_Size-Buffer_Offset;

    //Parsing
    if (!Element_Parse())
        return false;

    //The end
    Footer();
    FINNISHED_BOOL();
}

//---------------------------------------------------------------------------
bool File_Mpeg_Psi::Element_Parse()
{
    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    if (Element_Size<2)
        return false; //Must wait for more data
    BS_BEGIN();
    MARK_1(                                                     section_syntax_indicator);
    SKIP_BS( 1,                                                 reserved_future_use);
    SKIP_BS( 2,                                                 reserved);
    GET_BS (12, section_length,                                 section_length);
    if (Element_Size<2+section_length)
        return false; //Must wait for more data
    Element_Size=section_length-4; //CRC
    ELEMENT(0, table_id, Element_Size);

    //Positionning
    Buffer_Offset+=BS.Offset_Get();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    #define ELEMENT_CASE(_NAME, _DETAIL) \
        case 0x##_NAME : NAME(_DETAIL); Table_##_NAME(); break;

    switch (table_id)
    {
        ELEMENT_CASE(00, "program_association_table (PAT)");
        ELEMENT_CASE(01, "conditional_access_table (CAT)");
        ELEMENT_CASE(02, "TS_program_map_table (PMT)");
        ELEMENT_CASE(03, "TS_description_table");
        ELEMENT_CASE(04, "ISO_IEC_14496_scene_description_table");
        ELEMENT_CASE(05, "ISO_IEC_14496_object_descriptor_table");
        ELEMENT_CASE(42, "DVB - service_description_section_actual");
        ELEMENT_CASE(46, "DVB - service_description_section_other");
        ELEMENT_CASE(4E, "DVB - event_information_section_actual_following");
        ELEMENT_CASE(4F, "DVB - event_information_section_other_following");
        case 0x50 :
        case 0x51 :
        case 0x52 :
        case 0x53 :
        case 0x54 :
        case 0x55 :
        case 0x56 :
        case 0x57 :
        case 0x58 :
        case 0x59 :
        case 0x5A :
        case 0x5B :
        case 0x5C :
        case 0x5E :
        ELEMENT_CASE(5F, "DVB - event_information_section_actual_schedule");
        case 0x60 :
        case 0x61 :
        case 0x62 :
        case 0x63 :
        case 0x64 :
        case 0x65 :
        case 0x66 :
        case 0x67 :
        case 0x68 :
        case 0x69 :
        case 0x6A :
        case 0x6B :
        case 0x6C :
        case 0x6D :
        case 0x6E :
        ELEMENT_CASE(6F, "DVB - event_information_section_other_schedule");
        ELEMENT_CASE(C7, "ATSC - master_guide_table (MGT)");
        ELEMENT_CASE(C8, "ATSC - terrestrial_virtual_channel_table (TVCT)");
        ELEMENT_CASE(C9, "ATSC - cable_virtual_channel_table (CVCT)");
        ELEMENT_CASE(CA, "ATSC - rating_region_table (RRT)");
        ELEMENT_CASE(CB, "ATSC - event_information_region_table (RRT)");
        ELEMENT_CASE(CC, "ATSC - extended_text_table (ETT)");
        ELEMENT_CASE(CD, "ATSC - system_time_table (STT)");
        ELEMENT_CASE(D3, "ATSC - directed_channel_change_table (DCCT)");
        ELEMENT_CASE(D4, "ATSC - directed_channel_change_selection_code_table (DCCSCT)");
        default :
            if (table_id>=0x06
             && table_id<=0x37) reserved();
            if (table_id>=0x38
             && table_id<=0x39) iso13818_6();
            if (table_id>=0x40
             && table_id<=0xFE) user_private();
            if (table_id==0xFF) forbidden();
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Mpeg_Psi::PayLoad_Parse()
{
    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=Buffer_Size;
    int8u Jump=0;
    CC_BEGIN();
    GET_B1 (Jump,                                               pointer_field);
    if (Jump)
        SKIP_XX(Jump,                                           payload);
    CC_END_CANBEMORE();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    ELEMENT(0, "PayLoad", Stream_Pos);

    //Positionning
    Buffer_Offset+=Stream_Pos;

    FLUSH();
    return true;
}

//---------------------------------------------------------------------------
bool File_Mpeg_Psi::table_id_Parse()
{
    ELEMENT(0, "table_id", 1);

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=1;
    CC_BEGIN();
    GET_B1 (table_id,                                           table_id); INFO(Mpeg_Psi_table_id(table_id));
    CC_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Positionning
    Buffer_Offset+=1;

    FLUSH();
    return true;
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Footer()
{
    FLUSH();
    ELEMENT(0, "CRC", Buffer_Size-Buffer_Offset);

    //Parsing
    Element_Size=Buffer_Size-Buffer_Offset;
    BS_BEGIN();
    SKIP_BS(32,                                                 CRC_32);
    BS_END_FF();

    FLUSH();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::reserved()
{
    NAME("Reserved");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::iso13818_6()
{
    NAME("Defined in ISO/IEC 13818-6");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::user_private()
{
    NAME("user_private");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::forbidden()
{
    NAME("forbidden");
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::program_stream_map()
{
    Element_Size=Buffer_Size;
    ELEMENT(0, "program_stream_map", Element_Size);

    //Parsing
    int16u program_stream_info_length, elementary_stream_map_length;
    int8u  stream_type;
    CC_BEGIN()
    CC_BS_BEGIN()
    SKIP_BS( 1,                                                 current_next_indicator)
    SKIP_BS( 2,                                                 reserved)
    SKIP_BS( 5,                                                 program_stream_map_version)
    SKIP_BS( 7,                                                 reserved)
    MARK_1 (                                                    marker_bit)
    CC_BS_END();
    GET_B2 (program_stream_info_length,                         program_stream_info_length)
    //Descriptors and Positionning
    Descriptors_Length=program_stream_info_length;
    if (Descriptors_Length>0)
    {
        Descriptors(Stream_Pos);
        Stream_Pos+=Descriptors_Length;
    }
    GET_B2 (elementary_stream_map_length,                       elementary_stream_map_length)
    while (Stream_Pos<(int32u)(4+program_stream_info_length+2+elementary_stream_map_length))
    {
        FLUSH();
        ELEMENT_BEGIN();
        GET_B1 (stream_type,                                    stream_type) INFO(Mpeg_Psi_stream_type(stream_type));
        GET_B1 (elementary_PID,                                 elementary_stream_id)
        GET_B2 (Descriptors_Length,                             ES_info_length)
        ELEMENT(Details_Level_Last, (int16u)elementary_PID, 4+Descriptors_Length);
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }
        ELEMENT_END();

        //Filling
        program_map_Values[elementary_PID].stream_type=stream_type;
    }
    SKIP_B4(                                                    CRC_32)
    CC_END();

    //Finnishing
    FLUSH();
    FINNISHED();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_00()
{
    //Parsing
    BS_BEGIN();
    SKIP_BS(16,                                                 transport_stream_id);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);

    int32u ProgNumber;
    while (BS.Offset_Get()<Element_Size)
    {
        FLUSH();
        ELEMENT_BEGIN();
        GET_BS (16, ProgNumber,                                 program_number);
        ELEMENT(1, (int16u)ProgNumber, 4);
        SKIP_BS( 3,                                             reserved);
        if (ProgNumber==0)
        {
            int32u Network_PID;
            GET_BS ( 13, Network_PID,                           network_PID); NAME(Ztring().From_CC2(Network_PID));
            Programs[Network_PID].Kind=File_MpegTs::ts_network_section;
        }
        else
        {
            int32u program_map_PID;
            GET_BS ( 13, program_map_PID,                       program_map_PID); NAME(Ztring().From_CC2(program_map_PID));
            Programs[program_map_PID].Kind=File_MpegTs::ts_program_map_section;
        }
        program_association_section_ProgramNumber.push_back(ProgNumber);
        ELEMENT_END();
    }
    BS_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_01()
{
    //Parsing
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(18,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    CC_BS_END()

    //Descriptors and Positionning
    if (Stream_Pos<Element_Size)
    {
        Descriptors_Length=Element_Size-4-Stream_Pos;
        Descriptors(Stream_Pos);
        Stream_Pos+=Descriptors_Length;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_02()
{
    //Parsing
    CC_BEGIN();
    CC_BS_BEGIN()
    GET_BS (16, program_number,                                 program_number);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    SKIP_BS( 3,                                                 reserved);
    SKIP_BS(13,                                                 PCR_PID);
    SKIP_BS( 4,                                                 reserved);
    GET_BS (12, Descriptors_Length,                             program_info_length);
    CC_BS_END()

    //Descriptors and Positionning
    if (Descriptors_Length>0)
    {
        Descriptors(Stream_Pos);
        Stream_Pos+=Descriptors_Length;
    }

    //Parsing
    while (Stream_Pos<Element_Size)
    {
        FLUSH();
        ELEMENT_BEGIN();
        int32u stream_type;
        CC_BS_BEGIN();
        GET_BS ( 8, stream_type,                                stream_type); INFO(Mpeg_Psi_stream_type(stream_type));
        SKIP_BS( 3,                                             reserved);
        GET_BS (13, elementary_PID,                             elementary_PID);
        SKIP_BS( 4,                                             reserved);
        GET_BS (12, Descriptors_Length,                         ES_info_length);
        CC_BS_END();
        ELEMENT(1, (int16u)elementary_PID, 5+Descriptors_Length); NAME(Mpeg_Psi_stream_type(stream_type));

        //Filling
        program_map_Values[elementary_PID].stream_type=stream_type;

        //Descriptors
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }

        ELEMENT_END();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_03()
{
    //Parsing
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(18,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    CC_BS_END()

    //Descriptors and Positionning
    if (Stream_Pos<Element_Size)
    {
        Descriptors_Length=Element_Size-Stream_Pos;
        Descriptors(Stream_Pos);
        Stream_Pos+=Descriptors_Length;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_42()
{
    //Parsing
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(16,                                                 transport_stream_id);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    SKIP_BS(16,                                                 original_network_id);
    SKIP_BS( 8,                                                 reserved_future_use);
    CC_BS_END()
    while (Stream_Pos<Element_Size-4)
    {
        FLUSH();
        ELEMENT_BEGIN();
        int32u service_id;
        CC_BS_BEGIN();
        GET_BS (16, service_id,                                 service_id);
        SKIP_BS( 6,                                             reserved_future_use);
        SKIP_BS( 1,                                             EIT_schedule_flag);
        SKIP_BS( 1,                                             EIT_present_following_flag);
        SKIP_BS( 3,                                             running_status);
        SKIP_BS( 1,                                             free_CA_mode);
        GET_BS (12, Descriptors_Length,                         ES_info_length);
        CC_BS_END();
        ELEMENT(1, (int16u)service_id, 5+Descriptors_Length);

        //Descriptors
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }

        ELEMENT_END();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_46()
{
    Table_42();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_4E()
{
    //Parsing
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(16,                                                 service_id);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    SKIP_BS(16,                                                 transport_stream_id);
    SKIP_BS(16,                                                 original_network_id);
    SKIP_BS( 8,                                                 segment_last_section_number);
    SKIP_BS( 8,                                                 last_table_id);
    CC_BS_END()
    while (Stream_Pos<Element_Size)
    {
        FLUSH();
        ELEMENT_BEGIN();
        CC_BS_BEGIN();
        SKIP_BS(16,                                             event_id);
        SKIP_BS( 8,                                             start_time);
        SKIP_BS(32,                                             start_time);
        SKIP_BS(24,                                             duration);
        SKIP_BS( 3,                                             running_status);
        SKIP_BS( 1,                                             free_CA_mode);
        GET_BS (12, Descriptors_Length,                         descriptors_loop_length);
        CC_BS_END();
        ELEMENT(1, "?", 11+Descriptors_Length);

        //Descriptors
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }

        ELEMENT_END();
    }
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_4F()
{
    Table_4E();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_5F()
{
    Table_4E();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_6F()
{
    Table_4E();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_C7()
{
    //Parsing
    int32u tables_defined;
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(16,                                                 transport_stream_id);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    SKIP_BS( 8,                                                 protocol_version);
    GET_BS (16, tables_defined,                                 tables_defined);
    CC_BS_END()
    for (int32u Pos=0; Pos<tables_defined; Pos++)
    {
        int32u table_type, table_type_PID;
        FLUSH();
        ELEMENT_BEGIN();
        CC_BS_BEGIN();
        GET_BS (16, table_type,                                 table_type); INFO(Mpeg_Psi_ATSC_table_type(table_type));
        SKIP_BS( 3,                                             reserved);
        GET_BS (13, table_type_PID,                             table_type_PID);
        SKIP_BS( 3,                                             reserved);
        SKIP_BS( 5,                                             table_type_version_number);
        SKIP_BS(32,                                             number_bytes);
        SKIP_BS( 4,                                             reserved);
        GET_BS (12, Descriptors_Length,                         table_type_descriptors_length);
        CC_BS_END();
        ELEMENT(1, (int16u)table_type_PID, 11+Descriptors_Length); NAME(Mpeg_Psi_ATSC_table_type(table_type));

        //Descriptors
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }

        ELEMENT_END();
    }
    CC_BS_BEGIN()
    SKIP_BS( 4,                                                 reserved);
    GET_BS (12, Descriptors_Length,                             descriptors_length);
    CC_BS_END();

    //Descriptors
    if (Descriptors_Length>0)
    {
        Descriptors(Stream_Pos);
        Stream_Pos+=Descriptors_Length;
    }

    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Table_C8()
{
    //Parsing
    Ztring short_name;
    int32u num_channels_in_section;
    CC_BEGIN();
    CC_BS_BEGIN()
    SKIP_BS(16,                                                 transport_stream_id);
    SKIP_BS( 2,                                                 reserved);
    SKIP_BS( 5,                                                 version_number);
    SKIP_BS( 1,                                                 current_next_indicator);
    SKIP_BS( 8,                                                 section_number);
    SKIP_BS( 8,                                                 last_section_number);
    SKIP_BS( 8,                                                 protocol_version);
    GET_BS ( 8, num_channels_in_section,                        num_channels_in_section);
    CC_BS_END()
    for (int32u Pos=0; Pos<num_channels_in_section; Pos++)
    {
        int32u program_number, major_channel_number;
        FLUSH();
        ELEMENT_BEGIN();
        //GET_UTF16B(14, short_name,                              short_name);
        SKIP_XX(14,                                             short_name);
        CC_BS_BEGIN();
        SKIP_BS( 4,                                             reserved);
        GET_BS (10, major_channel_number,                       major_channel_number);
        SKIP_BS(10,                                             minor_channel_number);
        SKIP_BS( 8,                                             modulation_mode);
        SKIP_BS(32,                                             carrier_frequency);
        SKIP_BS(16,                                             channel_TSID);
        GET_BS (16, program_number,                             program_number);
        SKIP_BS( 2,                                             ETM_location);
        SKIP_BS( 1,                                             access_controlled);
        SKIP_BS( 1,                                             hidden);
        SKIP_BS( 2,                                             reserved);
        SKIP_BS( 1,                                             hide_guide);
        SKIP_BS( 3,                                             reserved);
        SKIP_BS( 6,                                             service_type);
        SKIP_BS(16,                                             source_id);
        SKIP_BS( 6,                                             reserved);
        GET_BS (10, Descriptors_Length,                         descriptors_length);
        CC_BS_END();
        ELEMENT(1, (int16u)program_number, 18+Descriptors_Length);

        //Filling
        Programs[program_number].Info[_T("major_channel_number")]=Ztring::ToZtring(major_channel_number);

        //Descriptors
        if (Descriptors_Length>0)
        {
            Descriptors(Stream_Pos);
            Stream_Pos+=Descriptors_Length;
        }

        ELEMENT_END();
    }
    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg_Psi::Descriptors(int32u &Stream_Pos)
{
    FLUSH();
    ELEMENT_BEGIN();
    ELEMENT("Descriptors", Descriptors_Length);

    //Parsing
    File_Mpeg_Descriptors* Descriptors=new File_Mpeg_Descriptors();
    Descriptors->format_identifier=Descriptor_format_identifier;
    Buffer_Offset+=Stream_Pos; //Positionning
    Open_Buffer_Init(Descriptors, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Descriptors, Buffer+Buffer_Offset, Descriptors_Length);
    Buffer_Offset-=Stream_Pos; //Positionning

    //Filling
    if (elementary_PID!=0)
    {
        program_map_Values[elementary_PID].KindOfStream=Descriptors->KindOfStream;
        program_map_Values[elementary_PID].Info=Descriptors->Info;
    }
    else
        Programs[program_number].Info=Descriptors->Info;
    //Kind=Descriptors->Kind;

    Descriptor_format_identifier=Descriptors->format_identifier;

    delete Descriptors; //Descriptors=NULL;

    ELEMENT_END();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGTS_YES

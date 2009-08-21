// File_Aac - Info for AAC files
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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_AAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Aac.h"
#include "ZenLib/Utils.h"
#include "ZenLib/BitStream.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
inline static size_t Aac_SynchSafeInt (int8u* B)   {return ((B[0]<<21)
                                                          | (B[1]<<14)
                                                          | (B[2]<< 7)
                                                          | (B[3]<< 0));}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
const int32u ADTS_SamplingRate[]=
{96000, 88200, 64000, 48000, 44100, 32000, 24000, 22050,
 16000, 12000, 11025,  8000, 7350, 0, 0, 0,};

const char* ADTS_ID[]=
{
    "MPEG-4",
    "MPEG-2",
};

const char* ADTS_Profile[]=
{
    "A_AAC/MPEG4/MAIN",
    "A_AAC/MPEG4/LC",
    "A_AAC/MPEG4/SSR",
    "A_AAC/MPEG4/LTP",
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

const char* MP4_Profile[]=
{
    "Reserved",
    "A_AAC/MPEG4/MAIN",
    "A_AAC/MPEG4/LC",
    "A_AAC/MPEG4/SSR",
    "A_AAC/MPEG4/LTP",
    "AAC SBR",
    "AAC Scalable",
    "TwinVQ",
    "CELP",
    "HVXC",
    "Reserved",
    "Reserved"
    "TTSI",
    "Main synthetic",
    "Wavetable synthesis",
    "General MIDI",
    "Algorithmic Synthesis and Audio FX",
    "ER AAC LC",
    "Reserved",
    "ER AAC LTP",
    "ER AAC scalable",
    "ER TwinVQ",
    "ER BSAC",
    "ER AAC LD",
    "ER CELP",
    "ER HVXC",
    "ER HILN",
    "ER Parametric",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};
//---------------------------------------------------------------------------

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Aac::Read_Buffer_Init ()
{
    //Configuration
    Frame_Count_Valid=16;

    //Temp - Technical info
    KindOfFormat=Kind_Unknown;

    //Temp - Global
    Frame_Count=0;
}

//---------------------------------------------------------------------------
void File_Aac::Read_Buffer_Continue ()
{
    //Id3v2 management
    if (!File_Name.empty() && !Id3v2_Read_Buffer_Continue())
        return;

    //Trusted limitation
    Trusted=Buffer_Size>2*(8*1024)?Buffer_Size/(8*1024):2; //Never less than 2 acceptable errors

    //Parsing headers
    if (KindOfFormat==Kind_Unknown)
    {
             if (Buffer_Size<=8)                             KindOfFormat=Kind_ES;   //AAC in ES (.mp4)
        else if ((CC2(Buffer+Buffer_Offset)&0xFFF6)==0xFFF0) KindOfFormat=Kind_ADTS; //ADTS header (12 bits + ID + 2 bits)
        else if (CC4(Buffer+Buffer_Offset)==CC4("ADIF"))     KindOfFormat=Kind_ADIF; //ADIF header
        else if (CC3(Buffer+Buffer_Offset)==CC3("AAC"))      KindOfFormat=Kind_AAC;  //AAC header
    }

    //Parsing
    switch (KindOfFormat)
    {
        case Kind_AAC  : AAC();  break; //AAC header
        case Kind_ADTS : ADTS(); break; //ADTS header (12 bits + ID + 2 bits)
        case Kind_ADIF : ADIF(); break; //ADIF header
        case Kind_ES   : ES();   break; //AAC in ES (.MP4)
        default        : File_Offset=File_Size; return; //Error
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Aac::AAC ()
{
    ELEMENT(0, "AAC", File_Size);
    FLUSH();

    //Filling
    Stream_Prepare(Stream_General);
    Fill ("Format", "AAC");
    Stream_Prepare(Stream_Audio);
    Fill ("Codec", "AAC");
    Fill("Resolution", 16);

    //Finnished
    FINNISHED();
}

//---------------------------------------------------------------------------
void File_Aac::ADIF ()
{
    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    int32u bitstream_type, bitrate, num_program_config_elements;
    CC_BEGIN();
    SKIP_C4(                                                    adif_id);
    CC_BS_BEGIN();
    TEST_BS_SKIP(1,                                             copyright_id_present)
        SKIP_BS(32,                                             copyright_id);
        SKIP_BS(32,                                             copyright_id);
        SKIP_BS( 8,                                             copyright_id);
    TEST_BS_END()
    SKIP_BS( 1,                                                 original_copy);
    SKIP_BS( 1,                                                 home);
    GET_BS ( 1, bitstream_type,                                 bitstream_type); INFO(bitstream_type?"VBR":"CBR");
    GET_BS (23, bitrate,                                        bitrate);
    GET_BS ( 4, num_program_config_elements,                    num_program_config_elements);
    for (int32u Pos=0; Pos<num_program_config_elements; Pos++)
    {
        int32u sampling_frequency_index, num_front_channel_elements, num_side_channel_elements, num_back_channel_elements, num_lfe_channel_elements, num_assoc_data_elements, num_valid_cc_elements, Comment_Size;
        if (bitstream_type==0)
            SKIP_BS(20,                                         adif_buffer_fullness);
        SKIP_BS(20,                                             buffer_fullness);
        SKIP_BS(4,                                              element_instance_tag);
        SKIP_BS(2,                                              object_type);
        GET_BS (4, sampling_frequency_index,                    sampling_frequency_index);
        GET_BS (4, num_front_channel_elements,                  num_front_channel_elements);
        GET_BS (4, num_side_channel_elements,                   num_side_channel_elements);
        GET_BS (4, num_back_channel_elements,                   num_back_channel_elements);
        GET_BS (2, num_lfe_channel_elements,                    num_lfe_channel_elements);
        GET_BS (3, num_assoc_data_elements,                     num_assoc_data_elements);
        GET_BS (4, num_valid_cc_elements,                       num_valid_cc_elements);
        TEST_BS_SKIP(1,                                         mono_mixdown_present);
            SKIP_BS(4,                                          mono_mixdown_element_number);
        TEST_BS_END();
        TEST_BS_SKIP(1,                                         stereo_mixdown_present);
            SKIP_BS(4,                                          stereo_mixdown_element_number);
        TEST_BS_END();
        TEST_BS_SKIP(1,                                         matrix_mixdown_idx_present);
            SKIP_BS(2,                                          matrix_mixdown_idx);
            SKIP_BS(2,                                          pseudo_surround_enable);
        TEST_BS_END();
        for (int32u Pos2=0; Pos2<num_front_channel_elements; Pos2++)
        {
            SKIP_BS(1,                                          front_element_is_cpe);
            SKIP_BS(4,                                          front_element_tag_select);
        }
        for (int32u Pos2=0; Pos2<num_side_channel_elements; Pos2++)
        {
            SKIP_BS(1,                                          back_element_is_cpe);
            SKIP_BS(4,                                          back_element_tag_select);
        }
        for (int32u Pos2=0; Pos2<num_back_channel_elements; Pos2++)
        {
            SKIP_BS(1,                                          back_element_is_cpe);
            SKIP_BS(4,                                          back_element_tag_select);
        }
        for (int32u Pos2=0; Pos2<num_lfe_channel_elements; Pos2++)
            SKIP_BS(4,                                          lfe_element_tag_select);
        for (int32u Pos2=0; Pos2<num_assoc_data_elements; Pos2++)
            SKIP_BS(4,                                          assoc_data_element_tag_select);
        for (int32u Pos2=0; Pos2<num_valid_cc_elements; Pos2++)
        {
            SKIP_BS(1,                                          cc_element_is_ind_sw);
            SKIP_BS(4,                                          valid_cc_element_tag_select);
        }
        BS.Byte_Align();
        GET_BS (8, Comment_Size,                                comment_field_bytes);
        for (int32u Pos2=0; Pos2<Comment_Size; Pos2++)
        {
            SKIP_BS(8,                                          comment_field_data);
        }
    }
    CC_BS_END();
    CC_END_CANBEMORE();

    //Element
    ELEMENT(0, "Audio Data Interchange Format", Stream_Pos);
    FLUSH();

    //Filling
    Stream_Prepare(Stream_General);
    Fill ("Format", "AAC");
    Stream_Prepare(Stream_Audio);
    Fill ("Codec", "AAC");
    Fill("BitRate_Mode", bitstream_type?"VBR":"CBR");
    if (bitrate>0) Fill(bitstream_type?"BitRate_Max":"BitRate", bitrate);
    Fill("Resolution", 16);

    //Finnished
    FINNISHED();
}

//---------------------------------------------------------------------------
void File_Aac::ADTS ()
{
    Trusted=1000;
    while (ADTS_Buffer_Parse());
}

//---------------------------------------------------------------------------
bool File_Aac::ADTS_Buffer_Parse ()
{
    //Search for header
    if (!ADTS_NextFrame())
        return false;

    //Parsing
    if (!ADTS_Element_Parse())
        return false; //Not enough bytes

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
bool File_Aac::ADTS_Element_Parse ()
{
    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    int32u id, protection_absent;
    BS_BEGIN();
    SKIP_BS(12,                                                 syncword);
    GET_BS ( 1, id,                                             id); INFO(ADTS_ID[id]);
    SKIP_BS( 2,                                                 layer);
    GET_BS ( 1, protection_absent,                              protection_absent);
    GET_BS ( 2, profile,                                        profile); INFO(ADTS_Profile[profile]);
    GET_BS ( 4, sampling_frequency_index,                       sampling_frequency_index); INFO(ADTS_SamplingRate[sampling_frequency_index], " Hz");
    SKIP_BS( 1,                                                 private);
    GET_BS ( 3, channel_configuration,                          channel_configuration);
    SKIP_BS( 1,                                                 original);
    SKIP_BS( 1,                                                 home);
    SKIP_BS( 1,                                                 copyright_id);
    SKIP_BS( 1,                                                 copyright_id_start);
    GET_BS (13, aac_frame_length,                               aac_frame_length);
    GET_BS (11, adts_buffer_fullness,                           adts_buffer_fullness); INFO(adts_buffer_fullness==0x7FF?"VBR":"CBR");
    SKIP_BS( 2,                                                 num_raw_data_blocks);
    if (!protection_absent)
        SKIP_BS(16,                                             CRC_check);
    BS_END_CANBEMORE();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Element size
    if (Buffer_Offset+aac_frame_length>Buffer_Size)
    {
        PURGE();
        return false;
    }
    Element_Size=aac_frame_length-BS.Offset_Get();
    Element_Next=File_Offset+Buffer_Offset+aac_frame_length;

    //Element
    ELEMENT(0, "Audio Data Transport Stream", Element_Size);

    //Positioning
    Buffer_Offset+=aac_frame_length;

    //Parsing
    ADTS_Frame();

    FLUSH();
    return true;
}

//---------------------------------------------------------------------------
void File_Aac::ADTS_Frame ()
{
    //Counting
    if (Element_Next==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //Parsing
    CC_BEGIN();
    SKIP_XX(Element_Size,                                       Data);
    CC_END();

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
    {
        //Filling
        int32u BitRate=(ADTS_SamplingRate[sampling_frequency_index]/1024)*aac_frame_length*8;
        Stream_Prepare(Stream_General);
        Fill("Format", "AAC");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", ADTS_Profile[profile]);
        Fill("SamplingRate", ADTS_SamplingRate[sampling_frequency_index]);
        Fill("Channel(s)", channel_configuration);
        if (adts_buffer_fullness==0x7FF)
            Fill("BitRate_Mode", "VBR");
        else
        {
            Fill("BitRate_Mode", "CBR");
            Fill("BitRate", BitRate);
        }
        Fill("Resolution", 16);

        if (File_Offset+Buffer_Size<File_Size)
        {
            //Details
            Buffer_Offset=(size_t)(Element_Next-File_Offset);
            if (Config.Details_Get())
            {
                Details_Add_Error("----------------------------------------");
                Details_Add_Error("---   ADTS, Jumping to end of file   ---");
                Details_Add_Error("----------------------------------------");
            }
            //No more need data
            File_Offset=File_Size;
        }
    }
}

//---------------------------------------------------------------------------
bool File_Aac::ADTS_NextFrame()
{
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+2<=Buffer_Size && (CC2(Buffer+Buffer_Offset_Temp)&0xFFF6)!=0xFFF0)
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+2>Buffer_Size)
    {
        Buffer_Offset=Buffer_Offset_Temp+1; //keep only 1 byte
        Buffer_Offset_Temp=0;
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
    return true;
}

//---------------------------------------------------------------------------
// AAC in ES, 2+ bytes
void File_Aac::ES ()
{
    Element_Size=Buffer_Size;
    ELEMENT(0, "ES", Element_Size);

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    int32u AudioObjectType, SamplingRate, ChannelConfiguration;
    BS_BEGIN();
    GET_BS (5, AudioObjectType,                                 AudioObjectType);
    GET_BS (4, SamplingRate,                                    SamplingRateIndex); INFO(ADTS_SamplingRate[SamplingRate]);
    if (SamplingRate==0xF)
    {
        GET_BS (24, SamplingRate,                               SamplingRate);
    }
    else
        SamplingRate=ADTS_SamplingRate[SamplingRate];
    GET_BS (4, ChannelConfiguration,                            ChannelConfiguration);
    INFO_BS(1, FrameLengthFlag,                                 FrameLengthFlag); INFO(FrameLengthFlag==0?1024:960, " bytes");
    SKIP_BS(1,                                                  DependsOnCoreCoder);
    SKIP_BS(1,                                                  ExtensionFlag);
    if (AudioObjectType==5)
    {
        //Parsing SBR stuff with AAC Scalable
        GET_BS (4, SamplingRate,                                SamplingRateIndex - SBR); INFO(ADTS_SamplingRate[SamplingRate]);
        if (SamplingRate==0xF)
        {
            GET_BS (24, SamplingRate,                           SamplingRate - SBR);
        }
        else
            SamplingRate=ADTS_SamplingRate[SamplingRate];
    }
    BS_END_CANBEMORE();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "MPEG-4 AAC");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", MP4_Profile[AudioObjectType]);
    Fill("SamplingRate", SamplingRate);
    if (ChannelConfiguration)
        Fill("Channel(s)", ChannelConfiguration);
    Fill("Resolution", 16);
    FLUSH();

    //Next element
    Buffer_Offset+=BS.Offset_Get();

    //SBR stuff
    if (Buffer_Size-Buffer_Offset>=3)
        ES_SBR();
}

//---------------------------------------------------------------------------
// AAC in ES, SBR part, 3+ bytes
void File_Aac::ES_SBR ()
{
    Element_Size=Buffer_Size-Buffer_Offset;
    ELEMENT(1, "SBR", Element_Size);

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    int32u Sync, HasSBR, AudioObjectType, SamplingRate;
    BS_BEGIN();
    GET_BS (11, Sync,                                           Sync);
    if (Sync!=0x2B7)
        return;
    GET_BS (5, AudioObjectType,                                 AudioObjectType);
    GET_BS (1, HasSBR,                                          HasSBR);
    if (HasSBR)
    {
        //Parsing SBR stuff with AAC Scalable
        GET_BS (4, SamplingRate,                                SamplingRateIndex - SBR); INFO(ADTS_SamplingRate[SamplingRate]);
        if (SamplingRate==0xF)
        {
            GET_BS (24, SamplingRate,                           SamplingRate - SBR);
        }
        else
            SamplingRate=ADTS_SamplingRate[SamplingRate];
    }
    BS_END_CANBEMORE();

    //Filling
    if (HasSBR)
    {
        (Stream[StreamKind_Last]->at(StreamPos_Last))(_T("Codec")).append(_T("/SBR"));
        Fill("SamplingRate", SamplingRate, 10, true);
    }
    FLUSH();

    //Next element
    Buffer_Offset+=BS.Offset_Get();

    //SBR-PS stuff
    if (Buffer_Size-Buffer_Offset>=2)
        ES_SBR_PS();
}

//---------------------------------------------------------------------------
// AAC in ES, SBR part, 2 bytes?
// Format is unknown
void File_Aac::ES_SBR_PS ()
{
    Element_Size=Buffer_Size-Buffer_Offset;
    ELEMENT(2, "PS", Element_Size);

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    BS_BEGIN();
    SKIP_BS (Element_Size*8,                                     Unknown);
    BS_END_CANBEMORE();

    //Possible only if 1 Channel
    if (Get(Stream_Audio, 0, _T("Channel(s)"))==_T("1"))
    {
        Fill("Channel(s)", 2, 10, true);
        (Stream[StreamKind_Last]->at(StreamPos_Last))(_T("Codec")).append(_T("/PS"));
    }
    FLUSH();

    //Next element
    Buffer_Offset+=2;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Aac::HowTo(stream_t StreamKind)
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
        Audio[0](_T("SamplingRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Resolution"), Info_HowTo)=_T("R");
    }
}

} //NameSpace

#endif //MEDIAINFO_AAC_YES


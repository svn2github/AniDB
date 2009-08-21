// File_Mpegts - Info for MPEG Transport Stream files
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
#if defined(MEDIAINFO_MPEGTS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_MpegTs.h"
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#include "MediaInfo/Multiple/File_MpegPs.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

extern const char* Mpeg_Psi_stream_Codec(int32u ID);
extern stream_t Mpeg_Psi_stream_Kind(int32u ID);

//---------------------------------------------------------------------------
const char* Mpeg_Psi_kind(File_MpegTs::ts_kind ID)
{
    switch (ID)
    {
        case File_MpegTs::program_association_table          : return "Program Association Table";
        case File_MpegTs::conditional_access_table           : return "Conditional Access Table";
        case File_MpegTs::transport_stream_description_table : return "Transport Stream Description Table";
        case File_MpegTs::reserved                           : return "reserved";
        case File_MpegTs::ts_program_map_section             : return "TS Program Map Section";
        case File_MpegTs::ts_network_section                 : return "TS Network Section";
        case File_MpegTs::pes                                : return "Data";
        case File_MpegTs::null                               : return "Null";
        case File_MpegTs::dvb_nit_st                         : return "DVB - NIT, ST";
        case File_MpegTs::dvb_sdt_bat_st                     : return "DVB - SDT, BAT, ST";
        case File_MpegTs::dvb_eit                            : return "DVB - EIT";
        case File_MpegTs::dvb_rst_st                         : return "DVB - RST, ST";
        case File_MpegTs::dvb_tdt_tot_st                     : return "DVB - TDT, TOT, ST";
        case File_MpegTs::dvb_mip                            : return "DVB - MIP (no table_id)";
        case File_MpegTs::dvb_reserved                       : return "DVB - reserved";
        case File_MpegTs::dvb_inband                         : return "DVB - Inband Signalling";
        case File_MpegTs::dvb_measurement                    : return "DVB - Measurement";
        case File_MpegTs::dvb_dit                            : return "DVB - DIT";
        case File_MpegTs::dvb_sit                            : return "DVB - SIT";
        case File_MpegTs::arib                               : return "ARIB";
        case File_MpegTs::cea_osd                            : return "CEA OSD";
        case File_MpegTs::atsc_pate                          : return "ATSC - PAT-E";
        case File_MpegTs::atsc_stt_pide                      : return "ATSC - STT, PID-E";
        case File_MpegTs::atsc_op                            : return "ATSC - operational and management packets";
        case File_MpegTs::atsc_psip                          : return "ATSC - PSIP";
        case File_MpegTs::atsc_scte                          : return "ATSC - SCTE Network/System Information Base";
        case File_MpegTs::atsc_reserved                      : return "ATSC - reserved";
        case File_MpegTs::docsis                             : return "DOCSIS";
        default : return "";
    }
}

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpegTs::~File_MpegTs()
{
    std::map<int32u, ts_stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        delete Temp->second.Parser; //Temp->second=NULL;
        Temp++;
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Init()
{
    Buffer_MinimumSize=32768*4;
    Streams_Count=Error;

    //Default values
    Stream[0x00].TS_Kind=program_association_table;
    Stream[0x01].TS_Kind=conditional_access_table;
    Stream[0x02].TS_Kind=transport_stream_description_table;
    for (int32u Pos=0x03; Pos<0x10; Pos++)
        Stream[Pos].TS_Kind=reserved;
    for (int32u Pos=0x00; Pos<0x10; Pos++)
        Stream[Pos].TS_Needed=true;
}

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Unsynched()
{
    if (Config.Details_Get())
    {
        Details_Add_Error("MPEG-TS, Will resync...");
    }

    Synched=false;
}

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Continue()
{
    //Look for first Sync word
    if (!Synched && !Synchronize())
        return;

    //Fill buffers with demuxed streams
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_MpegTs::Read_Buffer_Finalize()
{
    std::map<int32u, ts_stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        if (Temp->second.TS_Kind==pes)
        {
            //By the parser
            StreamKind_Last=Stream_Max;
            if (Temp->second.Parser)
            {
                Temp->second.Parser->Open_Buffer_Finalize();
                Merge (*Temp->second.Parser);
            }
            //By the descriptors
            if (StreamKind_Last==Stream_Max && Temp->second.KindOfStream!=Stream_Max)
            {
                Stream_Prepare(Temp->second.KindOfStream);
            }
            //By the stream_type
            if (StreamKind_Last==Stream_Max)
            {
                //No parser or the parser failed
                if (Mpeg_Psi_stream_Kind(Temp->second.stream_type)!=Stream_Max)
                {
                    StreamKind_Last=Mpeg_Psi_stream_Kind(Temp->second.stream_type);
                    Stream_Prepare(StreamKind_Last);
                }
            }

            //More info
            if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
            {
                //Common
                Fill("ID", Temp->first, 16);
                if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty())
                    Fill("Codec", Mpeg_Psi_stream_Codec(Temp->second.stream_type));
                //TimeStamp
                if (Temp->second.TimeStamp_End<Temp->second.TimeStamp_Start)
                    Temp->second.TimeStamp_End+=0x200000000LL; //33 bits, cyclic
                int64u PlayTime=Temp->second.TimeStamp_End-Temp->second.TimeStamp_Start;
                if (PlayTime!=0 && PlayTime!=(int64u)-1)
                    Fill("PlayTime", PlayTime/90, 10, true);
                else
                    Fill("PlayTime", "", 0, true, true); //Clear it
                //Encryption
                if (Temp->second.Scrambled)
                    Fill("Encryption", "Encrypted");

                //TS info
                if (!Temp->second.Info.empty())
                {
                    std::map<ZenLib::Ztring, ZenLib::Ztring>::iterator Info_Temp=Temp->second.Info.begin();
                    while (Info_Temp!=Temp->second.Info.end())
                    {
                        if (Get(StreamKind_Last, StreamPos_Last, Info_Temp->first).empty())
                            Fill(Info_Temp->first.To_Local().c_str(), Info_Temp->second);
                        Info_Temp++;
                    }
                }
            }
        }
        delete Temp->second.Parser; Temp->second.Parser=NULL;
        Temp++;
    }

    //Fill General
    if (General.empty())
        Stream_Prepare(Stream_General);
         if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-2V"))
        Fill(Stream_General, 0, "Format", "MPEG-2TS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-4V"))
        Fill(Stream_General, 0, "Format", "MPEG-4TS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("AVC"))
        Fill(Stream_General, 0, "Format", "MPEG-4TS");
    else
        Fill(Stream_General, 0, "Format", "MPEG-1TS");
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Buffer_Parse()
{
    //Element size
    Element_Next=Buffer_Offset+188;

    //Parsing
    if (!Element_Parse())
        return false;

    //Jumping to the end of the file if needed
    if (Detect_EOF_Needed())
        return false;

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Element_Parse()
{
    //Parsing the header
    if (!Element_Parse_Header())
        return false;

    //Element size verification
    if (Element_Size==0)
        return true;

    //Element
    ELEMENT(0, Element_Name);

    Buffer_Offset+=Element_HeaderSize;

    //PIDs
    if (Element_HeaderSize<188)
    {
        switch (Stream[Element_Name].TS_Kind)
        {
            case pes :
                PES();
                break;
            default:
                PSI();
        }
    }
    FLUSH();

    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Element_Parse_Header()
{
    //Enough data?
    if (Buffer_Offset+188>Buffer_Size)
        return false;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=188;
    int32u Adaptation, Scrambling;
    BS_BEGIN();
    SKIP_BS( 8,                                                 sync_byte);
    SKIP_BS( 1,                                                 transport_error_indicator);
    GET_BS ( 1, payload_unit_start_indicator,                   payload_unit_start_indicator);
    SKIP_BS( 1,                                                 transport_priority);
    GET_BS (13, Element_Name,                                   pid);
    if (!Stream[Element_Name].TS_Needed)
    {
        //We don't need this element (TS level)
        PURGE();
        Element_Size=0;
        return true;
    }
    GET_BS ( 2, Scrambling,                                     transport_scrambling_control);
    GET_BS ( 2, Adaptation,                                     adaptation_field_control);
    SKIP_BS( 4,                                                 continuity_counter);

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Adaptation
    if (Adaptation&0x2)
        AdaptationField();
    else
    {
        Element_Size=184;
        Element_HeaderSize=4;
    }

    //Needed?
    if (Stream[Element_Name].TS_Kind==pes && !Stream[Element_Name].PES_Needed)
    {
        //We don't need this element (PES level)
        PURGE();
        Element_Size=0;
        return true;
    }

    //Encryption
    if (Scrambling>0)
    {
        //Encrypted
        PURGE();
        Stream[Element_Name].PES_Needed=false;
        Stream[Element_Name].Scrambled=true;
        Element_Size=0;
        return true;
    }

    //Parsing
    if (Adaptation&0x1==0x00)
    {
        //No data
        FLUSH();
        Element_Size=0;
        return true;
    }

    return true;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegTs::Synchronize()
{
    while (           Buffer_Offset+188*4<Buffer_Size
      && !(CC1(Buffer+Buffer_Offset+188*0)==0x47
        && CC1(Buffer+Buffer_Offset+188*1)==0x47
        && CC1(Buffer+Buffer_Offset+188*2)==0x47
        && CC1(Buffer+Buffer_Offset+188*3)==0x47))
        Buffer_Offset++;
    if (Buffer_Offset+188*4>=Buffer_Size)
    {
        if (Synched)
        {
            //Sync lost
            if (Config.Details_Get())
            {
                Details_Add_Error("-------------------------------------------");
                Details_Add_Error("---   MPEG-TS, Synchronisation lost     ---");
                Details_Add_Error("-------------------------------------------");
            }
            Synched=false;
        }

        //Managing first Synch attempt
        if (Buffer_Size>188*4 && Count_Get(Stream_General)==0)
            File_Offset=File_Size; //End

        return false;
    }

    //There is no start code, so Stream_General is filled here
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);

    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegTs::Detect_EOF_Needed()
{
    //Jump to the end of the file
    if (File_Size>=File_Offset+Buffer_Size+1*1024*1024 && File_Size!=(int64u)-1 && (
       (File_Offset>=2*1024*1024)
    || (Streams_Count==0)
    ))
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("-------------------------------------------");
            Details_Add_Error("---   MPEG-TS, Jumping to end of file   ---");
            Details_Add_Error("-------------------------------------------");
        }

        File_GoTo=File_Size-1*1024*1024;
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
//
void File_MpegTs::AdaptationField()
{
    //Go where Adapation field is
    Buffer_Offset+=4;
    Element_Size=188-4;

    int32u Adaptation_Size, PCR, OPCR, Splicing, Transport, Adapatation_Extension;
    BS_BEGIN();
    GET_BS ( 8, Adaptation_Size,                                adaptation_field_length);
    if (Adaptation_Size>0)
    {
        SKIP_BS( 1,                                             discontinuity_indicator);
        SKIP_BS( 1,                                             random_access_indicator);
        SKIP_BS( 1,                                             elementary_stream_priority_indicator);
        GET_BS ( 1, PCR,                                        PCR_flag);
        GET_BS ( 1, OPCR,                                       OPCR_flag);
        GET_BS ( 1, Splicing,                                   splicing_point_flag);
        GET_BS ( 1, Transport,                                  transport_private_data_flag);
        GET_BS ( 1, Adapatation_Extension,                      adaptation_field_extension_flag);
        if (PCR)
        {
            int64u TimeStamp, TimeStamp1, TimeStamp2;
            GET_BS ( 1, TimeStamp1,                             program_clock_reference_base);
            GET_BS (32, TimeStamp2,                             program_clock_reference_base);
            TimeStamp=TimeStamp1*0x100000000LL+TimeStamp2;
            if (Stream[Element_Name].TimeStamp_Start==(int64u)-1 && Stream[Element_Name].TimeStamp_End==(int64u)-1) //This is the first PCR
            {
                Stream[Element_Name].TimeStamp_Start=TimeStamp;
                Stream[Element_Name].TimeStamp_End=(int64u)-1; //To be able to indicate that Start is OK even if it is 0
            }
            else
                Stream[Element_Name].TimeStamp_End=TimeStamp;
            SKIP_BS( 6,                                         reserved);
            SKIP_BS( 9,                                         program_clock_reference_extension);
        }
        if (OPCR)
        {
            SKIP_BS( 1,                                         original_program_clock_reference_base);
            SKIP_BS(32,                                         original_program_clock_reference_base);
            SKIP_BS( 6,                                         reserved);
            SKIP_BS( 9,                                         original_program_clock_reference_extension);
        }
        if (Splicing)
        {
            SKIP_BS(8,                                          splice_countdown);
        }
        if (Transport)
        {
            int32u Transport_Size;
            GET_BS ( 8, Transport_Size,                         transport_private_data_length);
            for (int32u Pos=0; Pos<=Transport_Size; Pos++)
                BS.Skip(8);
        }
        if (Adapatation_Extension)
        {
            int32u Adapatation_Extension_Size;
            GET_BS ( 8, Adapatation_Extension_Size,             adaptation_field_extension_length);
            for (int32u Pos=0; Pos<=Adapatation_Extension_Size; Pos++)
                BS.Skip(8);
        }
    }

    //Jump after the adaptation field
    Buffer_Offset-=4;
    Element_HeaderSize=4+1+Adaptation_Size;
    Element_Size=188-Element_HeaderSize;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegTs::PSI()
{
    NAME("Program Specific Information");
    NAME(Mpeg_Psi_kind(Stream[Element_Name].TS_Kind));

    if (Stream[Element_Name].Parser==NULL)
        Stream[Element_Name].Parser=new File_Mpeg_Psi;
    Open_Buffer_Init(Stream[Element_Name].Parser, File_Size, File_Offset+Buffer_Offset);
    ((File_Mpeg_Psi*)Stream[Element_Name].Parser)->pid=Element_Name;
    ((File_Mpeg_Psi*)Stream[Element_Name].Parser)->payload_unit_start_indicator=payload_unit_start_indicator;
    Open_Buffer_Continue(Stream[Element_Name].Parser, Buffer+Buffer_Offset, Element_Size);

    if (Stream[Element_Name].Parser->File_Offset==File_Size)
    {
        //Finnished, we can fill data
        File_Mpeg_Psi* Parser=(File_Mpeg_Psi*)Stream[Element_Name].Parser;
        switch (Stream[Element_Name].TS_Kind)
        {
            case program_association_table :
                    {
                        std::map<int32u, File_Mpeg_Psi::Program>::iterator Program_Temp=Parser->Programs.begin();
                        while (Program_Temp!=Parser->Programs.end())
                        {
                            //Enabling what we know parsing
                            Stream[Program_Temp->first].TS_Kind=Program_Temp->second.Kind;
                            Stream[Program_Temp->first].TS_Needed=true;

                            //Menus
                            //ProgramNumber2StreamNumber[Parser->program_association_section_ProgramNumber[Pos]]=elementary_PID;

                            Program_Temp++;
                        }
                    }
                    break;

            case ts_program_map_section :
                   {
                        std::map<int32u, File_Mpeg_Psi::program_map_Value>::iterator program_map_Values_Temp=Parser->program_map_Values.begin();
                        while (program_map_Values_Temp!=Parser->program_map_Values.end())
                        {
                            //Retrieving info
                            int32u elementary_PID=program_map_Values_Temp->first;

                            //Enabling what we know parsing
                            Stream[elementary_PID].stream_type=program_map_Values_Temp->second.stream_type;
                            Stream[elementary_PID].KindOfStream=program_map_Values_Temp->second.KindOfStream;
                            Stream[elementary_PID].Info=program_map_Values_Temp->second.Info;
                            if (Mpeg_Psi_stream_Kind(Stream[elementary_PID].stream_type)!=Stream_Max  //Known
                             || Stream[elementary_PID].KindOfStream!=Stream_Max
                             || (Stream[elementary_PID].stream_type>=0x80 && Stream[elementary_PID].stream_type<=0xFF) //Private
                               )
                            {
                                Stream[elementary_PID].TS_Kind=pes;
                                Stream[elementary_PID].TS_Needed=true;
                                Stream[elementary_PID].PES_Needed=true;
                            }

                            //Not precised PID handling
                            for (size_t Pos=0x10; Pos<0x1FFF; Pos++)
                            {
                                if (Stream.find(Pos)==Stream.end() || Stream[Pos].TS_Kind==not_needed)
                                {
                                    Stream[Pos].TS_Needed=true;
                                    ts_kind Kind;
                                    switch (Pos)
                                    {
                                        case 0x0010 : Kind=dvb_nit_st; break;
                                        case 0x0011 : Kind=dvb_sdt_bat_st; break;
                                        case 0x0012 : Kind=dvb_eit; break;
                                        case 0x0013 : Kind=dvb_rst_st; break;
                                        case 0x0014 : Kind=dvb_tdt_tot_st; break;
                                        case 0x0015 : Kind=dvb_mip; break;
                                        case 0x0016 :
                                        case 0x0017 :
                                        case 0x0018 :
                                        case 0x0019 :
                                        case 0x001A :
                                        case 0x001B : Kind=dvb_reserved; break;
                                        case 0x001C : Kind=dvb_inband; break;
                                        case 0x001D : Kind=dvb_measurement; break;
                                        case 0x001E : Kind=dvb_dit; break;
                                        case 0x001F : Kind=dvb_sit; break;
                                        case 0x0020 :
                                        case 0x0021 :
                                        case 0x0022 :
                                        case 0x0023 :
                                        case 0x0024 :
                                        case 0x0025 :
                                        case 0x0026 :
                                        case 0x0027 :
                                        case 0x0028 :
                                        case 0x0029 :
                                        case 0x002A :
                                        case 0x002B :
                                        case 0x002C :
                                        case 0x002D :
                                        case 0x002E :
                                        case 0x002F : Kind=arib; break;
                                        case 0x1ABC : Kind=cea_osd; break;
                                        case 0x1FF7 : Kind=atsc_pate; break;
                                        case 0x1FF8 : Kind=atsc_stt_pide; break;
                                        case 0x1FF9 : Kind=atsc_reserved; break;
                                        case 0x1FFA : Kind=atsc_op; break;
                                        case 0x1FFB : Kind=atsc_psip; break;
                                        case 0x1FFC : Kind=atsc_scte; break;
                                        case 0x1FFD : Kind=atsc_reserved; break;
                                        case 0x1FFE : Kind=docsis; break;
                                        default     : Kind=reserved;
                                    }
                                    Stream[Pos].TS_Kind=Kind;
                                }

                                if (Pos==0x002F)
                                    Pos=0x1ABB; //Skipping normal data
                                if (Pos==0x01ABC)
                                    Pos=0x1FF6; //Skipping normal data
                            }

                            program_map_Values_Temp++;
                        }
                   }
                   break;
            default : ;
        }

        /*std::map<int32u, File_Mpeg_Psi::Program>::iterator Programs_Temp=Parser->Programs.begin();
        while (Programs_Temp!=Parser->Programs.end())
        {
            //General information
            if (!Programs_Temp->second.Info.empty())
            {
                //Menu location
                size_t StreamNumber;
                if (Stream[Element_Name].TS_Kind!=atsc)
                    StreamNumber=Element_Name;
                else
                    StreamNumber=ProgramNumber2StreamNumber[Programs_Temp->first];
                if (StreamNumber2MenuPos.find(ProgramNumber2StreamNumber[StreamNumber])==StreamNumber2MenuPos.end())
                {
                    Stream_Prepare(Stream_Menu);
                    StreamNumber2MenuPos[ProgramNumber2StreamNumber[StreamNumber]]=StreamPos_Last;
                }

                //Filling
                std::map<ZenLib::Ztring, ZenLib::Ztring>::iterator Info_Temp=Programs_Temp->second.Info.begin();
                while (Info_Temp!=Programs_Temp->second.Info.end())
                {
                    Fill(Stream_Menu, StreamPos_Last, Info_Temp->first.To_Local().c_str(), Info_Temp->second, true);
                    Info_Temp++;
                }
            }
            if (!Parser->Kind.empty())
            {
                std::map<int32u, ts_kind>::iterator Kind_Temp=Parser->Kind.begin();
                while (Kind_Temp!=Parser->Kind.end())
                {
                    //Fill(Stream_General, 0, "AAAA", "AAAA");
                    Kind_Temp++;
                }
            }
        Programs_Temp++;
        }*/

        //Deleting
        delete Stream[Element_Name].Parser; Stream[Element_Name].Parser=NULL;
    }
}

//---------------------------------------------------------------------------
void File_MpegTs::Reserved()
{
    NAME("Reserved");
}

//---------------------------------------------------------------------------
void File_MpegTs::PES()
{
    NAME("Packet Stream");

    //Is it the start of a PES?
    NOT_NEEDED(Stream[Element_Name].Parser==NULL && !payload_unit_start_indicator)

    //Allocating an handle if needed
    if (Stream[Element_Name].Parser==NULL)
    {
            #if defined(MEDIAINFO_MPEGPS_YES)
                Stream[Element_Name].Parser=new File_MpegPs;
                ((File_MpegPs*)Stream[Element_Name].Parser)->FromTS=true;
                ((File_MpegPs*)Stream[Element_Name].Parser)->stream_type_FromTS=Stream[Element_Name].stream_type;
                ((File_MpegPs*)Stream[Element_Name].Parser)->MPEG_Version=2;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_General);
                Stream[Element_Name].Parser->Fill("Codec", "MPEG-PS");
                Stream[Element_Name].PES_Needed=false;
            #endif

    }

    //Parsing is needed?
    NOT_NEEDED(!Stream[Element_Name].PES_Needed)

    //Details
    size_t Details_Info_Temp_Size_Before=Details_Info_Temp.size();

    //Open MPEG-PS (PES)
    if (Stream[Element_Name].Parser && (Stream[Element_Name].Parser->File_GoTo==(int64u)-1 || Stream[Element_Name].Parser->File_GoTo<File_Offset+Buffer_Offset) && Stream[Element_Name].Parser->File_Offset!=Stream[Element_Name].Parser->File_Size)
    {
        Open_Buffer_Init(Stream[Element_Name].Parser, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Stream[Element_Name].Parser, Buffer+Buffer_Offset, Element_Size);

        //Need anymore?
        if (Stream[Element_Name].Parser->File_GoTo!=(int64u)-1 || Stream[Element_Name].Parser->File_Offset==Stream[Element_Name].Parser->File_Size)
        {
            Stream[Element_Name].PES_Needed=false;
            Streams_Count--;
        }
    }

    //Details
    if (Details_Info_Temp.size()==Details_Info_Temp_Size_Before)
        PURGE(); //Nothing has changed

    //Demux
    DEMUX(Buffer+Buffer_Offset, Element_Size, Ztring::ToZtring(Element_Name, 16)+_T(".mpg"));
}

//---------------------------------------------------------------------------
void File_MpegTs::Null()
{
    NAME("Null");
}

//---------------------------------------------------------------------------
void File_MpegTs::HowTo(stream_t StreamKind)
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

#endif //MEDIAINFO_MPEGTS_YES

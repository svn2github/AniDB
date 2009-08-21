// File_MpegPs - Info for MPEG files
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
#if defined(MEDIAINFO_MPEGPS_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_MpegPs.h"
#include "MediaInfo/Multiple/File_Mpeg_Psi.h"
#if defined(MEDIAINFO_AVC_YES)
    #include "MediaInfo/Video/File_Avc.h"
#endif
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGV_YES)
    #include "MediaInfo/Video/File_Mpegv.h"
#endif
#if defined(MEDIAINFO_VC1_YES)
    #include "MediaInfo/Video/File_Vc1.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_PCM_YES)
    #include "MediaInfo/Audio/File_Pcm.h"
#endif
#if defined(MEDIAINFO_RLE_YES)
    #include "MediaInfo/Image/File_Rle.h"
#endif
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* MpegPs_System_Fixed[]=
{
    "CBR",
    "VBR",
};

//---------------------------------------------------------------------------
const char* MpegPs_stream_id(int8u Element_Name)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return "MPEG Audio";
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return "MPEG Video";
    else if (Element_Name==0xB8) return "For all MPEG Audio streams";
    else if (Element_Name==0xB9) return "For all MPEG Video streams";
    else if (Element_Name==0xBD) return "Private 1";
    else if (Element_Name==0xBF) return "Private 2";
    else if (Element_Name==0xFD) return "Private HD";
    else                         return "";
}

//---------------------------------------------------------------------------
const char* MpegPs_Codec(int8u Element_Name)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return "MPEG-A";
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return "MPEG-V";
    else                         return "";
}

//---------------------------------------------------------------------------
int32u MpegPs_Default_stream_type(int8u Element_Name, int8u Mpeg_Version)
{
         if (Element_Name>=0xC0
          && Element_Name<=0xDF) return Mpeg_Version==0x02?0x04:0x03;
    else if (Element_Name>=0xE0
          && Element_Name<=0xEF) return Mpeg_Version==0x02?0x02:0x01;
    else                         return 0x00;
}

//---------------------------------------------------------------------------
extern const char* Mpeg_Psi_stream_type(int32u ID);
extern const char* Mpeg_Psi_stream_Codec(int32u ID);
extern stream_t    Mpeg_Psi_stream_Kind(int32u ID);

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_MpegPs::File_MpegPs()
{
    MPEG_Version=0;
    FromTS=false;
    stream_type_FromTS=0; //No info
}

//---------------------------------------------------------------------------
File_MpegPs::~File_MpegPs()
{
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Init()
{
    Video_Unlimited=false;
    Synched=false;
    Video_Count=(int8u)-1;
    Audio_Count=(int8u)-1;
    Private1_Count=true;
    Private2_Count=true;
    TimeStamp_Start=(int64u)-1;
    TimeStamp_End=(int64u)-1;
    File_Offset_LastTimeStamp=0;
    SizeToAnalyse=2*1024*1024;

    Stream[0xBA].PS_Needed=true;
    if (FromTS)
    {
        for (int8u Pos=0xB9; Pos!=0x00; Pos++)
            Stream[Pos].PS_Needed=true;
        Stream[0xBE].PS_Needed=false; //We need not parse padding_stream
    }

    //TS specific
    if (FromTS)
    {
        Video_Count=0;
        Audio_Count=0;
        Private1_Count=false;
        Private2_Count=false;
        if (Mpeg_Psi_stream_Kind(stream_type_FromTS)==Stream_Video)
            Video_Count=1;
        if (Mpeg_Psi_stream_Kind(stream_type_FromTS)==Stream_Audio)
        {
            if (stream_type_FromTS==0x03 || stream_type_FromTS==0x04) //MPEG Audio
                Audio_Count=0;
            else
                Private1_Count=true;
        }
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Unsynched()
{
    if (Config.Details_Get())
    {
        Details_Add_Error("Will resync...");
    }

    Synched=false;
    Video_Unlimited=false;
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonMPEGPS())
    {
        File_Offset=File_Size;
        return;
    }

    //Video unlimited specific, we didn't wait for the end (because this is... unlimited)
    if (Video_Unlimited)
    {
        Element_Size_Get_Unlimited();
        Element_Parse();
        Buffer_Offset+=Element_Size;

        //Jumping to the end of the file if needed
        if (Detect_EOF_Needed())
            return;
    }

    //Buffer
    Element_Next=File_Offset;

    //Parse with PES parser
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_MpegPs::Read_Buffer_Finalize()
{
    //General
    int64u PlayTime=TimeStamp_End-TimeStamp_Start;
    if (PlayTime)
    {
        if (program_mux_rate)
            PlayTime+=(File_Size-File_Offset_LastTimeStamp)*90000/(program_mux_rate*50); //The last bytes...
        Fill("PlayTime", PlayTime/90, 10, true);
    }

    //For each stream
    std::map<int8u, ps_stream>::iterator Temp=Stream.begin();
    while (Temp!=Stream.end())
    {
        //By the parser
        StreamKind_Last=Stream_Max;
        if (Temp->second.Parser)
        {
            Temp->second.Parser->Open_Buffer_Finalize();
            Merge(*Temp->second.Parser);
        }
        //By the stream_type
        if (StreamKind_Last==Stream_Max)
        {
            //No parser or the parser failed
            if (Temp->second.stream_type!=0)
                Stream_Prepare(Mpeg_Psi_stream_Kind(Temp->second.stream_type));
        }

        //More info
        if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
        {
            //Common
            Fill("ID", Temp->first, 16);
            if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty() && Temp->second.stream_type!=0)
                Fill("Codec", Mpeg_Psi_stream_Codec(Temp->second.stream_type));

            //TimeStamp
            if (Temp->second.TimeStamp_End.PTS<Temp->second.TimeStamp_Start.PTS)
                Temp->second.TimeStamp_End.PTS+=0x200000000LL; //33 bits, cyclic
            int64u PlayTime=Temp->second.TimeStamp_End.PTS-Temp->second.TimeStamp_Start.PTS;
            if (PlayTime)
            {
                Fill("PlayTime", PlayTime/90, 10, true);
                Fill("Delay", Temp->second.TimeStamp_Start.PTS/90, 10, true);
            }
        }

        Temp++;
    }

    //For each stream
    Temp=Stream_Private1.begin();
    while (Temp!=Stream_Private1.end())
    {
        //By the parser
        StreamKind_Last=Stream_Max;
        if (Temp->second.Parser)
        {
            Temp->second.Parser->Open_Buffer_Finalize();
            Merge(*Temp->second.Parser);
        }
        //By the stream_type
        if (StreamKind_Last==Stream_Max)
        {
            //No parser or the parser failed
            if (Temp->second.stream_type!=0)
                Stream_Prepare(Mpeg_Psi_stream_Kind(Temp->second.stream_type));
        }

        //More info
        if (StreamKind_Last!=Stream_Max) //Found by the Parser or stream_type
        {
            //Common
            Fill("ID", Temp->first, 16);
            if (Get(StreamKind_Last, StreamPos_Last, _T("Codec")).empty() && Temp->second.stream_type!=0)
                Fill("Codec", Mpeg_Psi_stream_Codec(Temp->second.stream_type));

            //TimeStamp
            if (Temp->second.TimeStamp_End.PTS<Temp->second.TimeStamp_Start.PTS)
                Temp->second.TimeStamp_End.PTS+=0x200000000LL; //33 bits, cyclic
            int64u PlayTime=Temp->second.TimeStamp_End.PTS-Temp->second.TimeStamp_Start.PTS;
            if (PlayTime)
            {
                Fill("PlayTime", PlayTime/90, 10, true);
                Fill("Delay", Temp->second.TimeStamp_Start.PTS/90, 10, true);
            }
        }

        Temp++;
    }

    //Tags in MPEG Video
    if (Count_Get(Stream_Video)>0)
        Fill(Stream_General, 0, "Encoded_Library", Get(Stream_Video, 0, _T("Encoded_Library")));

    //Fill General
         if (!Video.empty() && Video[0](_T("Codec"))==_T("AVC"))
        Fill(Stream_General, 0, "Format", "MPEG-4PS");
    else if (!Video.empty() && Video[0](_T("Codec"))==_T("MPEG-4V"))
        Fill(Stream_General, 0, "Format", "MPEG-4PS");
    else if (MPEG_Version==2)
        Fill(Stream_General, 0, "Format", "MPEG-2PS");
    else if (MPEG_Version==1)
        Fill(Stream_General, 0, "Format", "MPEG-1PS");
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Buffer_Parse()
{
    //Next Frame
    if (!NextFrame())
        return false;

    //Element name
    if (!Element_Name_Get())
        return false;

    //Element size
    if (!Element_Size_Get())
        return false;

    if (Stream[Element_Name].PS_Needed) //Only if we need of it
    {
        //Is OK?
        if (Element_Name_IsOK() && !Element_Parse())
            return false;

        //Jumping to the end of the file if needed
        if (Detect_EOF_Needed())
            return false;

        //If no need of more
        if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
            return false;
    }

    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
//
bool File_MpegPs::Element_Parse()
{
    ELEMENT(0, Element_Name, Element_Size);

    //Positionning
    Buffer_Offset+=Element_HeaderSize;

    //Parsing
    switch (Element_Name)
    {
        case 0xB9 : MPEG_program_end(); break;
        case 0xBA : pack_start(); break;
        case 0xBB : system_header_start(); break;
        case 0xBC : program_stream_map(); break;
        case 0xBD : private_stream_1(); break;
        case 0xBE : padding_stream(); break;
        case 0xBF : private_stream_2(); break;
        case 0xFD : if (FromTS)
                        private_stream_1();
                    else
                        video_stream();
                    break;
        default:
                 if (Element_Name>=0xC0
                  && Element_Name<=0xDF) audio_stream();
            else if (Element_Name>=0xE0
                  && Element_Name<=0xEF) video_stream();
            else
                TRUSTED_ISNOT("Unattended element!");
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
// Packet "B9"
void File_MpegPs::MPEG_program_end()
{
    NAME("MPEG_program_end");

    //Parsing
    INTEGRITY_SIZE(Element_Size!=0, 0);
}

//---------------------------------------------------------------------------
// Packet "BA"
void File_MpegPs::pack_start()
{
    NAME("pack_start");

    //Reading
    int32u Version, SysClock32, SysClock29, SysClock14, Padding;
    BS_BEGIN()
    PEEK_BS( 2, Version,                                        );
    if (Version==1)
    {
        //MPEG-2
        MPEG_Version=2;
        SKIP_BS( 2,                                             01 - MPEG2);
        GET_BS ( 3, SysClock32,                                 system_clock_reference_base32)
        MARK_1(                                                 marker_bit)
        GET_BS (15, SysClock29,                                 system_clock_reference_base29)
        MARK_1(                                                 marker_bit)
        GET_BS (15, SysClock14,                                 system_clock_reference_base14)
        TimeStamp_End=SysClock32<<30
                    | SysClock29<<15
                    | SysClock14<<00;
        INFO(TimeStamp_End/90);
        MARK_1(                                                 marker_bit)
        SKIP_BS( 9,                                             system_clock_reference_extension)
        MARK_1(                                                 marker_bit)
        GET_BS (22, program_mux_rate,                           program_mux_rate) INFO(program_mux_rate*50, " Bps");
        MARK_1(                                                 marker_bit)
        MARK_1(                                                 marker_bit)
        SKIP_BS( 5,                                             reserved)
        GET_BS ( 3, Padding,                                    pack_stuffing_length)
        SKIP_BS(8*Padding,                                      padding)
        BS_END();
    }
    else
    {
        //MPEG-1
        MPEG_Version=1;
        SKIP_BS( 4,                                             0010 - MPEG2);
        GET_BS ( 3, SysClock32,                                 system_clock_reference_base32)
        MARK_1(                                                 marker_bit)
        GET_BS (15, SysClock29,                                 system_clock_reference_base29)
        MARK_1(                                                 marker_bit)
        GET_BS (15, SysClock14,                                 system_clock_reference_base14)
        TimeStamp_End=SysClock32<<30
                    | SysClock29<<15
                    | SysClock14<<00;
        INFO(TimeStamp_End/90);
        MARK_1(                                                 marker_bit)
        MARK_1(                                                 marker_bit)
        GET_BS(22, program_mux_rate,                            mux_rate)
        MARK_1(                                                 marker_bit)
        BS_END()
    }

    //Autorisation of other streams
    if (General.empty())
    {
        for (int8u Pos=0xB9; Pos!=0x00; Pos++)
            Stream[Pos].PS_Needed=true;
        Stream[0xBE].PS_Needed=false; //We need not parse padding_stream
    }
    else
        //Details
        PURGE();

    //Filling
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);
    if (TimeStamp_Start==(int64u)-1)
        TimeStamp_Start=TimeStamp_End;
    File_Offset_LastTimeStamp=File_Offset+Buffer_Offset;
}

//---------------------------------------------------------------------------
// Packet "BB"
void File_MpegPs::system_header_start()
{
    NAME("system_header_start");
    INTEGRITY_GENERAL(0);

    //If there is system_header_start, default value for private sections are false
    Private1_Count=false;
    Private2_Count=false;

    //Parsing
    int32u rate_bound, audio_bound, video_bound;
    BS_BEGIN()
    MARK_1(                                                     marker_bit)
    GET_BS (22, rate_bound,                                     rate_bound) INFO(rate_bound*(MPEG_Version==1?1:50), " Bps");
    MARK_1(                                                     marker_bit)
    GET_BS ( 6, audio_bound,                                    audio_bound)
    INFO_BS( 1, fixed_flag,                                     fixed_flag); INFO(MpegPs_System_Fixed[fixed_flag]);
    SKIP_BS( 1,                                                 CSPS_flag)
    SKIP_BS( 1,                                                 system_audio_lock_flag)
    SKIP_BS( 1,                                                 system_video_lock_flag)
    MARK_1(                                                     marker_bit)
    GET_BS ( 5, video_bound,                                    video_bound)
    SKIP_BS( 1,                                                 packet_rate_restriction_flag)
    SKIP_BS( 7,                                                 reserved_byte)
    while (BS.Peek(1)==1)
    {
        FLUSH();
        ELEMENT_BEGIN();
        int32u stream_id, Scale, Size;
        GET_BS ( 8, stream_id,                                  stream_id)
        ELEMENT(1, (int16u)stream_id, 3);
        NAME(MpegPs_stream_id((int16u)stream_id));
        INFO(MpegPs_stream_id((int16u)stream_id));
        MARK_1(                                                 marker_bit)
        MARK_1(                                                 marker_bit)
        GET_BS ( 1, Scale,                                      STD_buffer_bound_scale)
        GET_BS (13, Size,                                       STD_buffer_size_bound) INFO(Ztring::ToZtring(Size*(Scale?1024:128)) + _T(" bytes"));
        ELEMENT_END();

        //Filling
        if (MpegPs_Default_stream_type((int8u)stream_id, MPEG_Version)!=0x00)
            Stream[stream_id].stream_type=MpegPs_Default_stream_type((int8u)stream_id, MPEG_Version);
        if (stream_id==0xBD)
            Private1_Count=true;
        if (stream_id==0xBF)
            Private2_Count=true;
    }
    BS_END();

    //Filling
    if (Audio_Count==(int8u)-1) //0xBB may be multipart
        Audio_Count=0;
    Audio_Count+=audio_bound;
    if (Video_Count==(int8u)-1) //0xBB may be multipart
        Video_Count=0;
    Video_Count+=video_bound;
    SizeToAnalyse=rate_bound*50; //standart delay between TimeStamps is 0.7s, we try 1s to be sure
    if (Private1_Count)
        SizeToAnalyse*=16; //If there is a private section, this may be DVD, with late data --> 10s minimum

    //Autorisation of other streams
    if (Audio_Count!=0 && Video_Count!=0) //0xBB may be multipart
        Stream[0xBB].PS_Needed=false;
}

//---------------------------------------------------------------------------
// Packet "BC"
void File_MpegPs::program_stream_map()
{
    NAME("program_stream_map");
    MPEG_Version=2; //program_stream_map does NOT exist in MPEG-1 specs

    File_Mpeg_Psi Parser;
    Parser.From_TS=false;
    Open_Buffer_Init(&Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(&Parser, Buffer+Buffer_Offset, Element_Size);

    if (Parser.File_Offset==File_Size)
    {
        //Finnished, we can fill data
        std::map<int32u, File_Mpeg_Psi::program_map_Value>::iterator Temp=Parser.program_map_Values.begin();
        while (Temp!=Parser.program_map_Values.end())
        {
            Stream[Temp->first].stream_type=Temp->second.stream_type;
            Temp++;
        }
    }
}

//---------------------------------------------------------------------------
// Packet "BD"
void File_MpegPs::private_stream_1()
{
    NAME("private_stream_1");

    //For TS streams, which does not have Start chunk
    if (FromTS && General.empty())
    {
        Stream_Prepare(Stream_General);
        Video_Count=0;
        Audio_Count=1;
        Stream[Element_Name].stream_type=stream_type_FromTS;
    }

    //If this is a file, we must have a start chunk
    if (!File_Name.empty())
        INTEGRITY_GENERAL(0);

    //Testing DVD
    if (!FromTS)
    {
        Private1_ChooseID();
        if (Private1_ID)
            NAME(Ztring::ToZtring(Private1_ID, 16));
    }
    else
    {
        Private1_ID=Stream[Element_Name].stream_type;
        Private1_Offset=0;
    }

    //New stream if needed
    if (Stream_Private1[Private1_ID].Parser==NULL)
    {
        Stream_Private1[Private1_ID].Parser=Private1_ChooseParser();
        Stream_Private1[Private1_ID].TimeStamp_Start=Stream[0xBD].TimeStamp_End; //TimeStamp_End is the last info
    }
    Stream_Private1[Private1_ID].TimeStamp_End=Stream[0xBD].TimeStamp_End; //TimeStamp_End is the last info

    //Always useful?
    if (Stream_Private1[Private1_ID].Parser->File_Offset==Stream_Private1[Private1_ID].Parser->File_Size
     && (Stream_Private1[Private1_ID].Parser2==NULL || Stream_Private1[Private1_ID].Parser2->File_Offset==Stream_Private1[Private1_ID].Parser->File_Size)
     && (Stream_Private1[Private1_ID].Parser3==NULL || Stream_Private1[Private1_ID].Parser3->File_Offset==Stream_Private1[Private1_ID].Parser->File_Size))
    {
        PURGE();
        return;
    }

    //Parsing
    Open_Buffer_Init(Stream_Private1[Private1_ID].Parser, File_Size, File_Offset+Buffer_Offset+Private1_Offset);
    Open_Buffer_Continue(Stream_Private1[Private1_ID].Parser, Buffer+Buffer_Offset+Private1_Offset, Element_Size-Private1_Offset);

    //Testing other parsers in case of need
    if (!Stream_Private1[Private1_ID].Parser->Synched)
    {
        FLUSH();
        ELEMENT(1, "Testing AC3...", Element_Size);
        if (Stream_Private1[Private1_ID].Parser3==NULL)
            Stream_Private1[Private1_ID].Parser3=new File_Ac3;
        Open_Buffer_Init(Stream_Private1[Private1_ID].Parser3, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Stream_Private1[Private1_ID].Parser3, Buffer+Buffer_Offset, Element_Size);

        if (Stream_Private1[Private1_ID].Parser3->Count_Get(Stream_General)>0)
        {
            FLUSH();
            ELEMENT(1, "AC3 found, changing default parser", 0);
            delete Stream_Private1[Private1_ID].Parser; Stream_Private1[Private1_ID].Parser=Stream_Private1[Private1_ID].Parser3; Stream_Private1[Private1_ID].Parser3=NULL;
        }
        else
        {
            FLUSH();
            ELEMENT(1, "Testing DTS...", Element_Size);
            if (Stream_Private1[Private1_ID].Parser2==NULL)
                Stream_Private1[Private1_ID].Parser2=new File_Dts;
            Open_Buffer_Init(Stream_Private1[Private1_ID].Parser2, File_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Stream_Private1[Private1_ID].Parser2, Buffer+Buffer_Offset, Element_Size);

            if (Stream_Private1[Private1_ID].Parser2->Count_Get(Stream_General)>0)
            {
                FLUSH();
                ELEMENT(1, "DTS found, changing default parser", 0);
                delete Stream_Private1[Private1_ID].Parser; Stream_Private1[Private1_ID].Parser=Stream_Private1[Private1_ID].Parser2; Stream_Private1[Private1_ID].Parser2=NULL;
            }
        }
    }

    //Disabling this stream
    if (FromTS && Audio_Count>0 && Stream_Private1[Private1_ID].Parser->File_GoTo!=(int64u)-1 && Stream_Private1[Private1_ID].Parser->Count_Get(Stream_Audio)>0)
    {
        Stream[Element_Name].PS_Needed=false;
        Audio_Count--;
    }

    //Demux
    DEMUX(Buffer+Buffer_Offset+Private1_Offset, Element_Size-Private1_Offset, Ztring::ToZtring(Element_Name, 16)+_T(".")+Ztring::ToZtring(Private1_ID, 16)+Private1_ChooseExtension());
}

//---------------------------------------------------------------------------
void File_MpegPs::Private1_ChooseID()
{
    //Init
    Private1_Offset=0;

    //From TS
    if (stream_type_FromTS>0)
    {
        Private1_ID=stream_type_FromTS;
        return;
    }

    //Trying VOB system
    Private1_ID=0;
    Private1_ChooseID_VOB();
    if (Private1_ID)
        return;
}

//---------------------------------------------------------------------------
void File_MpegPs::Private1_ChooseID_VOB()
{
    //Parsing
    int32u CodecID, Count, Next;
    CC_BEGIN();
    GET_B1 (CodecID,                                        CodecID);
    GET_B1 (Count,                                          Count of next frame headers);
    GET_B2 (Next,                                           Next frame offset minus 1);
    CC_END_CANBEMORE();

    //Coherancy
    if (Next+1+4>Element_Size && !(CodecID>=0x20 && CodecID<=0x3F)) //Seems to not work with subtitles, to be confirmed
        return; //Should not!

    //Testing
    if (Count>0)
    {
        //Subtitles
             if (CodecID>=0x20 && CodecID<=0x3F)
            ;
        //AC3
        else if (CodecID>=0x80 && CodecID<=0x87)
        {
            if (CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77)
                return;
        }
        //DTS
        else if (CodecID>=0x88 && CodecID<=0x8F)
        {
            if (CC4(Buffer+Buffer_Offset+3+Next)!=0x7FFE8001)
                return;
        }
        //PCM
        else if (CodecID>=0xA0 && CodecID<=0xA8)
            ;
        //AC3+
        else if (CodecID>=0xC0 && CodecID<=0xC7)
        {
            if (CC2(Buffer+Buffer_Offset+3+Next)!=0x0B77)
                return;
        }
        //Unknown
        else
        {
            Private1_ID=0;
            return;
        }
    }

    //Filling
    Private1_ID=(int8u)CodecID;

    //Positionning
    Private1_Offset=4;
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser()
{
    //Subtitles
         if (Private1_ID>=0x20 && Private1_ID<=0x3F)
        return Private1_ChooseParser_RLE();
    //AC3
    else if (Private1_ID>=0x80 && Private1_ID<=0x87)
        return Private1_ChooseParser_AC3();
    //DTS
    else if (Private1_ID>=0x88 && Private1_ID<=0x8F)
        return Private1_ChooseParser_DTS();
    //PCM
    else if (Private1_ID>=0xA0 && Private1_ID<=0xA8)
        return Private1_ChooseParser_PCM();
    //AC3+
    else if (Private1_ID>=0xC0 && Private1_ID<=0xC7)
        return Private1_ChooseParser_AC3();
    else
        return Private1_ChooseParser_NULL();
}

//---------------------------------------------------------------------------
ZenLib::Char* File_MpegPs::Private1_ChooseExtension()
{
    //Subtitles
         if (Private1_ID>=0x20 && Private1_ID<=0x3F)
        return _T(".sub");
    //AC3
    else if (Private1_ID>=0x80 && Private1_ID<=0x87)
        return _T(".ac3");
    //DTS
    else if (Private1_ID>=0x88 && Private1_ID<=0x8F)
        return _T(".dts");
    //PCM
    else if (Private1_ID>=0xA0 && Private1_ID<=0xA8)
        return _T(".pcm");
    //AC3+
    else if (Private1_ID>=0xC0 && Private1_ID<=0xC7)
        return _T(".ac3");
    else
        return _T("");
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser_RLE()
{
    NAME("RLE");

    //Filling
    #if defined(MEDIAINFO_RLE_YES)
        return new File_Rle();
    #else
        //Filling
        File__Base* Handle=new File__Base();
        Handle->Stream_Prepare(Stream_Text);
        Handle->Fill("Codec", "RLE");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser_AC3()
{
    NAME("AC3");

    //Filling
    #if defined(MEDIAINFO_AC3_YES)
        return new File_Ac3();
    #else
        //Filling
        File__Base* Handle=new File__Base();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "AC3");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser_DTS()
{
    NAME("DTS");

    //Filling
    #if defined(MEDIAINFO_DTS_YES)
        return new File_Dts();
    #else
        //Filling
        File__Base* Handle=new File__Base();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "DTS");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser_PCM()
{
    NAME("PCM");

    //Filling
    #if defined(MEDIAINFO_PCM_YES)
        return new File_Pcm();
    #else
        //Filling
        File__Base* Handle=new File__Base();
        Handle->Stream_Prepare(Stream_Audio);
        Handle->Fill("Codec", "PCM");
        return Handle;
    #endif
}

//---------------------------------------------------------------------------
File__Base* File_MpegPs::Private1_ChooseParser_NULL()
{
    //Filling
    File__Base* Handle=new File__Base();
    return Handle;
}

//---------------------------------------------------------------------------
// Packet "BE"
void File_MpegPs::padding_stream()
{
    NAME("padding_stream");
}

//---------------------------------------------------------------------------
// Packet "BF"
void File_MpegPs::private_stream_2()
{
    NAME("private_stream_2");

    //Disabling this stream
    if (Private2_Count)
    {
        Stream[Element_Name].PS_Needed=false;
        Private2_Count=false;
    }
}

//---------------------------------------------------------------------------
void File_MpegPs::audio_stream()
{
    NAME("Audio");

    //For TS streams, which does not have Start chunk
    if (FromTS && General.empty())
    {
        Stream_Prepare(Stream_General);
        Video_Count=0;
        Audio_Count=1;
        Stream[Element_Name].stream_type=stream_type_FromTS;
    }

    //If this is a file, we must have a start chunk
    if (!File_Name.empty())
        INTEGRITY_GENERAL(0);

    //If we have no stream map --> Registering the stream as MPEG Video
    if (Stream[Element_Name].stream_type==0)
    {
        if (MPEG_Version==2)
            Stream[Element_Name].stream_type=0x04; //MPEG-2 Audio
        else
            Stream[Element_Name].stream_type=0x03; //MPEG-1 Audio
    }

    //New stream if needed
    if (Stream[Element_Name].Parser==NULL)
    {
             if (Stream[Element_Name].stream_type==0x03 || Stream[Element_Name].stream_type==0x04)
        {
            #if defined(MEDIAINFO_MPEGA_YES)
                Stream[Element_Name].Parser=new File_Mpega;
                ((File_Mpega*)Stream[Element_Name].Parser)->Frame_Count_Valid=1;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_Audio);
                if (Stream[Element_Name].stream_type==1)
                    Stream[Element_Name].Parser->Fill("Codec", "MPEG-1 Audio");
                if (Stream[Element_Name].stream_type==2)
                    Stream[Element_Name].Parser->Fill("Codec", "MPEG-2 Audio");
            #endif
        }
        else
        {
            Stream[Element_Name].Parser=new File__Base();
            Stream[Element_Name].Parser->Stream_Prepare(Stream_Audio);
        }
    }

    //Parsing
    Open_Buffer_Init(Stream[Element_Name].Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Stream[Element_Name].Parser, Buffer+Buffer_Offset, Element_Size);

    //Disabling this stream
    if (Audio_Count>0 && Stream[Element_Name].Parser->File_GoTo!=(int64u)-1)
    {
        Stream[Element_Name].PS_Needed=false;
        Audio_Count--;
    }

    //Demux
    DEMUX(Buffer+Buffer_Offset, Element_Size, Ztring::ToZtring(Element_Name, 16)+_T(".mpa"));
}

//---------------------------------------------------------------------------
void File_MpegPs::video_stream()
{
    NAME ("Video");

    //For TS streams, which does not have Start chunk
    if (FromTS && General.empty())
    {
        Stream_Prepare(Stream_General);
        Video_Count=1;
        Audio_Count=0;
        Stream[Element_Name].stream_type=stream_type_FromTS;
    }

    //If this is a file, we must have a start chunk
    if (!File_Name.empty())
        INTEGRITY_GENERAL(0);

    //If we have no stream map --> Registering the stream as MPEG Video
    if (Stream[Element_Name].stream_type==0)
    {
        if (Element_Name!=0xFD)
        {
            if (MPEG_Version==2)
                Stream[Element_Name].stream_type=0x02; //MPEG-2 Video
            else
                Stream[Element_Name].stream_type=0x01; //MPEG-1 Video
        }
    }

    //New stream if needed
    if (Stream[Element_Name].Parser==NULL)
    {
        if (Element_Name==0xFD)
        {
            #if defined(MEDIAINFO_VC1_YES)
                Stream[Element_Name].Parser=new File_Vc1;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Name].Parser->Fill("Codec", "VC-1");
            #endif
        }
        else if (Stream[Element_Name].stream_type==0x01 || Stream[Element_Name].stream_type==0x02)
        {
            #if defined(MEDIAINFO_MPEGV_YES)
                Stream[Element_Name].Parser=new File_Mpegv;
                ((File_Mpegv*)Stream[Element_Name].Parser)->MPEG_Version=MPEG_Version;
                ((File_Mpegv*)Stream[Element_Name].Parser)->Frame_Count_Valid=1;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_Video);
                if (Stream[Element_Name].stream_type==0x01)
                    Stream[Element_Name].Parser->Fill("Codec", "MPEG-1 Video");
                if (Stream[Element_Name].stream_type==0x02)
                    Stream[Element_Name].Parser->Fill("Codec", "MPEG-2 Video");
            #endif
        }
        else if (Stream[Element_Name].stream_type==0x10)
        {
            #if defined(MEDIAINFO_MPEG4V_YES)
                Stream[Element_Name].Parser=new File_Mpeg4v;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Name].Parser->Fill("Codec", "MPEG-4V");
            #endif
        }
        else if (Stream[Element_Name].stream_type==0x1B)
        {
            #if defined(MEDIAINFO_AVC_YES)
                Stream[Element_Name].Parser=new File_Avc;
                ((File_Avc*)Stream[Element_Name].Parser)->Frame_Count_Valid=1;
            #else
                //Filling
                Stream[Element_Name].Parser=new File__Base();
                Stream[Element_Name].Parser->Stream_Prepare(Stream_Video);
                Stream[Element_Name].Parser->Fill("Codec", "AVC");
            #endif
        }
        else
        {
            Stream[Element_Name].Parser=new File__Base();
            Stream[Element_Name].Parser->Stream_Prepare(Stream_Video);
        }
    }

    //Parsing
    Open_Buffer_Init(Stream[Element_Name].Parser, File_Size, File_Offset+Buffer_Offset);
    Open_Buffer_Continue(Stream[Element_Name].Parser, Buffer+Buffer_Offset, Element_Size);

    //Testing other parsers in case of need
    if (Stream[Element_Name].stream_type!=0x02 && !Stream[Element_Name].Parser->Synched)
    {
        FLUSH();
        ELEMENT(1, "Testing AVC...", Element_Size);
        if (Stream[Element_Name].Parser3==NULL)
            Stream[Element_Name].Parser3=new File_Avc;
        Open_Buffer_Init(Stream[Element_Name].Parser3, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Stream[Element_Name].Parser3, Buffer+Buffer_Offset, Element_Size);

        if (Stream[Element_Name].Parser3->Count_Get(Stream_General)>0)
        {
            FLUSH();
            ELEMENT(1, "AVC found, changing default parser", 0);
            delete Stream[Element_Name].Parser; Stream[Element_Name].Parser=Stream[Element_Name].Parser3; Stream[Element_Name].Parser3=NULL;
        }
        else
        {
            FLUSH();
            ELEMENT(1, "Testing MPEG-4 Video...", Element_Size);
            if (Stream[Element_Name].Parser2==NULL)
                Stream[Element_Name].Parser2=new File_Mpeg4v;
            Open_Buffer_Init(Stream[Element_Name].Parser2, File_Size, File_Offset+Buffer_Offset);
            Open_Buffer_Continue(Stream[Element_Name].Parser2, Buffer+Buffer_Offset, Element_Size);

            if (Stream[Element_Name].Parser2->Count_Get(Stream_General)>0)
            {
                FLUSH();
                ELEMENT(1, "MPEG-4 Video found, changing default parser", 0);
                delete Stream[Element_Name].Parser; Stream[Element_Name].Parser=Stream[Element_Name].Parser2; Stream[Element_Name].Parser2=NULL;
            }
        }
    }

    //Disabling this stream
    if (Video_Count>0 && Stream[Element_Name].Parser->File_GoTo!=(int64u)-1 && Stream[Element_Name].Parser->Count_Get(Stream_Video)>0)
    {
        Stream[Element_Name].PS_Needed=false;
        Video_Count--;
    }

    //Demux
    DEMUX(Buffer+Buffer_Offset, Element_Size, Ztring::ToZtring(Element_Name, 16)+_T(".mpv"));
}

//---------------------------------------------------------------------------
void File_MpegPs::audioHD_stream()
{
    NAME ("Audio HD");

    //Demux
    DEMUX(Buffer+Buffer_Offset, Element_Size, Ztring::ToZtring(Element_Name, 16)+_T(".xxx"));
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_MpegPs::Detect_NonMPEGPS ()
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

    //Detect MPEG Version
    if (MPEG_Version==0)
        Detect_MPEG_Version();

    //Seems OK
    return false;
}

//---------------------------------------------------------------------------
void File_MpegPs::Detect_MPEG_Version ()
{
    /*
    size_t B5_Offset=Buffer_Offset;
    while (B5_Offset+4<=Buffer_Size && !(CC3(Buffer+B5_Offset)==0x000001 && CC1(Buffer+B5_Offset+3)==0xB5))
        B5_Offset++;
    if (B5_Offset+4>Buffer_Size)
        MPEG_Version=1;
    else
    */ //Currently default is MPEG-2, if Header says MPEG-1, this will be MPEG-1, no more.
        MPEG_Version=2;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Detect_EOF_Needed ()
{
    //Jumping to the end of the file
    if (File_Size>=File_Offset+Buffer_Size+SizeToAnalyse && File_Size!=(int64u)-1 && (
       (File_Offset>=SizeToAnalyse)
    || (Synched && Video_Count==0 && Audio_Count==0 && Private1_Count==false && Private1_Count==false)
    ))
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("-------------------------------------------");
            Details_Add_Error("---   MPEG-PS, Jumping to end of file   ---");
            Details_Add_Error("-------------------------------------------");
        }

        //Jumping
        File_GoTo=File_Size-SizeToAnalyse;
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
bool File_MpegPs::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first bytes
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset_Temp)==0x000001 && CC1(Buffer+Buffer_Offset_Temp+3)>=0xB9)) //0xB9 is the lower value for MPEG-PS
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
        //There may have unuseful 0x00 in the stream
        if (Synched)
        {
            size_t Check=0;
            for (size_t Pos=Buffer_Offset; Pos<=Buffer_Offset_Temp; Pos++)
                Check+=Buffer[Pos];
            if (Check==0)
                Synched=false; //all zeros, this is synch
        }

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
bool File_MpegPs::Element_Name_Get()
{
    //Enough data?
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Element name
    Element_Name=BigEndian2int8u(Buffer+Buffer_Offset+3);

    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Element_Size_Get()
{
    switch (Element_Name)
    {
        //Fixed size
        case 0xB9 : //MPEG_program_end
            Element_HeaderSize=4; //MPEG start code + Element name
            Element_Size=0;
            Element_Next=Buffer_Offset+4; //MPEG start code + Element name
            break;

        //Not possible to know the size directly
        case 0xBA : //pack_start
            //Searching Element_Next
            Element_Next=Buffer_Offset+4;
            if (!Element_Size_Get_Element_Next())
                return false;
            //Filling
            Element_HeaderSize=4; //MPEG start code + Element name
            Element_Size=Element_Next-Buffer_Offset-4;
            break;

        //Header is only Size
        case 0xBB : //system_header_start
        case 0xBC : //program_stream_map
        case 0xBE : //padding_stream
        case 0xBF : //private_stream_2
        case 0xF0 : //ECM
        case 0xF1 : //EMM
        case 0xF2 : //DSMCC stream
        case 0xF8 : //ITU-T Rec. H .222.1 type E
        case 0xFF : //Program stream directory
            //Enough data?
            if (Buffer_Offset+6>Buffer_Size)
                return false;
            //Filling
            Element_HeaderSize=6; //MPEG start code + Element name + Element size
            Element_Size=BigEndian2int16u(Buffer+Buffer_Offset+4);
            Element_Next=Buffer_Offset+6+Element_Size; //MPEG start code + Element name + Element size
            break;

        //Element with PES Header (with Size in Header)
        default :
            if (!Buffer_Parse_Header()) //Header
                return false;
    }

    //Testing
    if (Element_Size==0&& Element_Name!=0xB9) //Not normal
        TRUSTED_ISNOT("Next element is too small, not normal");
    if (Element_Next+2<=Buffer_Size && CC2(Buffer+Element_Next)!=0x0000)
        TRUSTED_ISNOT("Next Synch is not found");

    //Does we have enough buffer?
    if (Buffer_Offset+Element_HeaderSize+Element_Size>Buffer_Size)
        return false;

    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Element_Size_Get_Unlimited()
{
    //Searching Element_Next
    Element_Next=Buffer_Offset;
    if (!Element_Size_Get_Element_Next())
        Element_Next=Buffer_Size; //The whole buffer

    Element_HeaderSize=0;
    Element_Size=Element_Next-Buffer_Offset;
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Element_Size_Get_Element_Next()
{
    //Integrity
    if (Element_Next<Buffer_Offset)
        Element_Next=Buffer_Size;
    if (Element_Next==Buffer_Size && CC3(Buffer+Element_Next)==0x000001 && CC1(Buffer+Element_Next+3)>=0xB9) //0xB9 is the lower value for MPEG-PS
        Element_Next+=4; //Not normal, risk of infinite loop

    //Searching Element_Next
    while (Element_Next+4<=Buffer_Size && !(CC3(Buffer+Element_Next)==0x000001 && CC1(Buffer+Element_Next+3)>=0xB9)) //0xB9 is the lower value for MPEG-PS
        Element_Next++;
    if (Element_Next+4>Buffer_Size)
    {
        if (File_Offset+Element_Next+4<=File_Size)
            return false; //This is not the end of file, so we must wait for more data

        //Managing special cases
        Element_Next=Buffer_Size; //End of file
    }

    return true;
}

//---------------------------------------------------------------------------
bool File_MpegPs::Element_Name_IsOK()
{
    //Is it the start of a MPEG PS?
    if (Count_Get(Stream_General)==0
     &&   Element_Name!=0xBA
     && !(Element_Name>=0xBD && Element_Name<=0xEF)
     &&   Element_Name!=0xFD //HD-DVD
     )
    {
        //MPEG PS didn't begin, skipping
        //Details
        if (Config.Details_Get()>0.9)
        {
            Details_Add_Element(0, Element_Name, Element_Size-4);
            Details_Add_Element("Has not began, skipping");
        }
        Buffer_Offset+=Element_HeaderSize+Element_Size;
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
// Packet header
bool File_MpegPs::Buffer_Parse_Header()
{
    //There is a PES Header with size and long packet header
    if (Buffer_Offset+6>=Buffer_Size)
        return false; //No enough bytes

    //Element size
    Element_Size=BigEndian2int16u(Buffer+Buffer_Offset+4); //Element size
    Element_HeaderSize=6;

    //Element_Next
    if (Element_Size==0)
    {
        //Video stream in Transport Stream, Element_Size may be unlimited
        Video_Unlimited=true;
        Element_Next=Buffer_Offset+6; ////MPEG start code + Element name + Element size
        if (!Element_Size_Get_Element_Next())
            Element_Next=Buffer_Size; //The whole buffer
        if (Element_Next>Buffer_Offset && Element_Next-Buffer_Offset>Element_HeaderSize)
            Element_Size=Element_Next-Buffer_Offset-Element_HeaderSize;
        else
            Element_Size=0; //There is a problem...
    }
    else
    {
        Element_Next=Buffer_Offset+6+Element_Size;

        //Testing end of the file before the end of the PES
        if (File_Offset+Element_Next>=File_Size) //This is the end of the file
            Element_Next=File_Size;

        //Testing if we have enough data
        if (Element_Next>Buffer_Size)
            return false;
    }

    //Parsing header data
    if (!Buffer_Parse_Header_Data()) //Header
        return false;

    return true;
}

//---------------------------------------------------------------------------
// Packet header data
bool File_MpegPs::Buffer_Parse_Header_Data()
{
    //MPEG-2
    switch (MPEG_Version)
    {
        case 1  : if (!Buffer_Parse_Header_Data_MPEG1()) return false; break;
        case 2  : if (!Buffer_Parse_Header_Data_MPEG2()) return false; break;
        default : return true; //Change nothing
    }

    //TimeStamp start
    if (!Stream[Element_Name].TimeStamp_Start.PTS_Is_Valid && Stream[Element_Name].TimeStamp_End.PTS_Is_Valid)
        Stream[Element_Name].TimeStamp_Start=Stream[Element_Name].TimeStamp_End;

    Element_Size=Element_Next-Buffer_Offset-Element_HeaderSize;
    return true;
}

//---------------------------------------------------------------------------
// Packet header data - MPEG-1
bool File_MpegPs::Buffer_Parse_Header_Data_MPEG1()
{
    //Keep out firsts 0xFF
    while (Buffer_Offset+Element_HeaderSize<Element_Next && Buffer[Buffer_Offset+Element_HeaderSize]==0xFF)
        Element_HeaderSize++;
    if (Buffer_Offset+Element_HeaderSize>=Element_Next)
        return true;

    //Buffer scale and size
    if ((Buffer[Buffer_Offset+Element_HeaderSize]&0xC0)==0x40)
    {
        Element_HeaderSize+=2;
    }

    //With PTS only
    if ((Buffer[Buffer_Offset+Element_HeaderSize]&0xF0)==0x20)
    {
        if (Element_Size<5)
            return false;
        BitStream BS(Buffer+Buffer_Offset+Element_HeaderSize, Element_Size);
        Stream[Element_Name].TimeStamp_End.PTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.PTS=MPEG_TimeStamp_Read(BS);
        Element_HeaderSize+=5;
    }

    //With PTS and DTS
    else if ((Buffer[Buffer_Offset+Element_HeaderSize]&0xF0)==0x30)
    {
        if (Element_Size<10)
            return false;
        BitStream BS(Buffer+Buffer_Offset+Element_HeaderSize, Element_Size);
        Stream[Element_Name].TimeStamp_End.PTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.DTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.PTS=MPEG_TimeStamp_Read(BS);
        Stream[Element_Name].TimeStamp_End.DTS=MPEG_TimeStamp_Read(BS);
        Element_HeaderSize+=10;
    }

    return true;
}

//---------------------------------------------------------------------------
// Packet header data - MPEG-2
bool File_MpegPs::Buffer_Parse_Header_Data_MPEG2()
{
    if (Element_Size<3)
        return false;

    //Reading
    BitStream BS(Buffer+Buffer_Offset+6, Element_Size-6);
    int32u Flags, Size;
    BS.Skip(2);                                 //01
    BS.Skip(2);                                 //PES_scrambling_control
    BS.Skip(1);                                 //PES_priority
    BS.Skip(1);                                 //data_alignment_indicator
    BS.Skip(1);                                 //copyright
    BS.Skip(1);                                 //original_or_copy
    Flags=BS.Get(2);                            //PTS_DTS_flags
    BS.Skip(1);                                 //ESCR_flag
    BS.Skip(1);                                 //ES_rate_flag
    BS.Skip(1);                                 //DSM_trick_mode_flag
    BS.Skip(1);                                 //additional_copy_info_flag
    BS.Skip(1);                                 //PES_CRC_flag
    BS.Skip(1);                                 //PES_extension_flag
    Size=BS.Get(8);                             //PES_header_data_length

    Element_HeaderSize+=3;
    if (3+Size>Element_Size)
        return false;

    //Options
    if (Flags==0x2)
    {
        Stream[Element_Name].TimeStamp_End.PTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.PTS=MPEG_TimeStamp_Read(BS);
    }
    if (Flags==0x3)
    {
        Stream[Element_Name].TimeStamp_End.PTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.DTS_Is_Valid=true;
        Stream[Element_Name].TimeStamp_End.PTS=MPEG_TimeStamp_Read(BS);
        Stream[Element_Name].TimeStamp_End.DTS=MPEG_TimeStamp_Read(BS);
    }

    //Positioning Packet data start
    Element_HeaderSize+=Size;
    return true;
}

//---------------------------------------------------------------------------
int64u File_MpegPs::MPEG_TimeStamp_Read (BitStream &BS)
{
    int64u TimeStamp;

    BS.Skip(4);
    TimeStamp=BS.Get(3);
    BS.Skip(1);
    TimeStamp<<=15;
    TimeStamp|=BS.Get(15);
    BS.Skip(1);
    TimeStamp<<=15;
    TimeStamp|=BS.Get(15);
    BS.Skip(1);
    return TimeStamp;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_MpegPs::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OveralBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            Fill_HowTo("Encoded_Application", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("AspectRatio", "R");
            Fill_HowTo("FrameRate", "R");
            Fill_HowTo("Delay", "R");
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Resolution", "R");
            Fill_HowTo("Delay", "R");
            break;
        case (Stream_Text) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("Delay", "R");
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

#endif //MEDIAINFO_MPEGPS_YES


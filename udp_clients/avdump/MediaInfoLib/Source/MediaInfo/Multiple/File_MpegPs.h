// File_Mpeg - Info for MPEG files
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
// Information about MPEG files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegH
#define MediaInfo_MpegH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "ZenLib/BitStream.h"
#include <ZenLib/Conf.h>
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
struct Mpeg_TimeStamp
{
  int64u PTS;
  int64u DTS;
  bool PTS_Is_Valid;
  bool DTS_Is_Valid;

  Mpeg_TimeStamp ()
  {
    PTS=0;
    DTS=0;
    PTS_Is_Valid=false;
    DTS_Is_Valid=false;
  }
};

//---------------------------------------------------------------------------

//***************************************************************************
// Class File_Mpeg
//***************************************************************************

class File_MpegPs : public File__Base
{
protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Unsynched ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_MpegPs();
    ~File_MpegPs();

public :
    //Where this file comes from?
    bool FromTS; //Indicate if stream comes from TS
    int32u stream_type_FromTS; //ID
    int8u MPEG_Version;

private :
    //Buffer
    bool Buffer_Parse ();
    bool Buffer_Parse_Header ();
    bool Buffer_Parse_Header_Data ();
    bool Buffer_Parse_Header_Data_MPEG1 ();
    bool Buffer_Parse_Header_Data_MPEG2 ();
    bool Element_Parse();
    size_t Element_Size;
    size_t Element_HeaderSize;
    int8u  Element_Name;
    size_t Element_Next;

    //Packet
    void MPEG_program_end();    //0xB9
    void pack_start();          //0xBA
    void system_header_start(); //0xBB
    void program_stream_map();  //0xBC
    void private_stream_1();    //0xBD
    void padding_stream();      //0xBE
    void private_stream_2();    //0xBF
    void audio_stream();        //0xC0 --> 0xDF
    void video_stream();        //0xE0 --> 0xEF, 0xFD (if !FromTS)
    void audioHD_stream();      //0xFD (if FromTS)
    int8u Video_Count;
    int8u Audio_Count;
    bool Private1_Count;
    bool Private2_Count;

    //Helpers
    void Private1_ChooseID();
    void Private1_ChooseID_VOB();
    int8u Private1_ID;
    size_t Private1_Offset;
    File__Base* Private1_ChooseParser();
    File__Base* Private1_ChooseParser_RLE();
    File__Base* Private1_ChooseParser_AC3();
    File__Base* Private1_ChooseParser_DTS();
    File__Base* Private1_ChooseParser_PCM();
    File__Base* Private1_ChooseParser_NULL();
    ZenLib::Char* Private1_ChooseExtension();

    bool   Detect_NonMPEGPS();
    void   Detect_MPEG_Version();
    bool   Detect_EOF_Needed();
    bool   NextFrame();
    bool   Element_Name_Get();
    bool   Element_Size_Get();
    bool   Element_Size_Get_Unlimited();
    bool   Element_Size_Get_Element_Next();
    bool   Element_Name_IsOK();


    size_t MPEG_PES_Data   ();
    size_t MPEG_Stream     ();
    int64u MPEG_TimeStamp_Read (BitStream &BS);

    //PS
    struct ps_stream
    {
        int32u stream_type;
        File__Base* Parser;
        File__Base* Parser2; //Sometimes, we need to do parallel tests
        File__Base* Parser3; //Sometimes, we need to do parallel tests
        Mpeg_TimeStamp TimeStamp_Start;
        Mpeg_TimeStamp TimeStamp_End;
        bool PS_Needed;

        ps_stream()
        {
            stream_type=0;
            Parser=NULL;
            Parser2=NULL;
            Parser3=NULL;
            PS_Needed=false;
        }

        ~ps_stream()
        {
            stream_type=0;
            delete Parser; //Parser=NULL;
            delete Parser2; //Parser2=NULL;
            delete Parser3; //Parser3=NULL;
            PS_Needed=false;
        }
    };
    std::map<int8u, ps_stream> Stream;
    std::map<int8u, ps_stream> Stream_Private1; //there can have multiple streams in one private stream

    //Container
    int64u TimeStamp_Start;
    int64u TimeStamp_End;
    int64u File_Offset_LastTimeStamp;
    int32u program_mux_rate;
    int64u SizeToAnalyse; //Total size of a chunk to analyse, it may be lowered by the system

    //Temp
    /*
    std::vector<File__Base*>    Video_Handle;
    std::vector<File__Base*>    VideoMpeg4v_Handle;
    std::vector<File__Base*>    VideoAvc_Handle;
    std::vector<File__Base*>    Audio_Handle;
    std::vector<File__Base*>    Private1_Handle;
    std::vector<File__Base*>    Private1Ac3_Handle;
    std::vector<File__Base*>    Private1Dts_Handle;
    std::vector<Mpeg_TimeStamp> Video_TimeStamp;
    std::vector<Mpeg_TimeStamp> Video_TimeStamp_End;
    std::vector<Mpeg_TimeStamp> Audio_TimeStamp;
    std::vector<Mpeg_TimeStamp> Audio_TimeStamp_End;
    std::vector<Mpeg_TimeStamp> Private1_TimeStamp;
    std::vector<Mpeg_TimeStamp> Private1_TimeStamp_End;
    */
    bool Video_Unlimited;

    //Error controls
    //std::vector<int8u> StreamKind;
};

} //NameSpace

#endif

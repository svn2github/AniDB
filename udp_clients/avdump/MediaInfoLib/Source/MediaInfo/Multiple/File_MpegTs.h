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
//
// Information about MPEG Transport Stream files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_MpegTsH
#define MediaInfo_MpegTsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Multiple/File_MpegPs.h"
#include <vector>
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_MpegTs
//***************************************************************************

class File_MpegTs : public File__Base
{
protected :
    //Formats
    void Read_Buffer_Init ();
    void Read_Buffer_Unsynched ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    ~File_MpegTs();

    enum ts_kind
    {
        //MPEG
        not_needed,
        program_association_table,
        conditional_access_table,
        transport_stream_description_table,
        reserved,
        ts_network_section,
        ts_program_map_section,
        pes,
        null,

        //Other
        dvb_nit_st,
        dvb_sdt_bat_st,
        dvb_eit,
        dvb_rst_st,
        dvb_tdt_tot_st,
        dvb_mip,
        dvb_reserved,
        dvb_inband,
        dvb_measurement,
        dvb_dit,
        dvb_sit,
        arib,
        cea_osd,
        atsc_pate,
        atsc_stt_pide,
        atsc_op,
        atsc_psip,
        atsc_scte,
        atsc_reserved,
        docsis
    };

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    bool Element_Parse_Header();
    int16u Element_Name;
    size_t Element_Size;
    size_t Element_HeaderSize;
    size_t Element_Next;

    //TS
    struct ts_stream
    {
        int32u stream_type;
        stream_t KindOfStream;
        File__Base* Parser;
        ts_kind TS_Kind;
        int64u TimeStamp_Start;
        int64u TimeStamp_End;
        std::map<ZenLib::Ztring, ZenLib::Ztring> Info;
        bool Scrambled;
        bool TS_Needed;
        bool PES_Needed;

        ts_stream()
        {
            stream_type=0;
            KindOfStream=Stream_Max;
            Parser=NULL;
            TS_Kind=not_needed;
            TimeStamp_Start=(int64u)-1;
            TimeStamp_End=(int64u)-1;
            Scrambled=false;
            TS_Needed=false;
            PES_Needed=false;
        }
    };
    std::map<int32u, ts_stream> Stream;
    std::map<int32u, int16u> ProgramNumber2StreamNumber;
    std::map<int32u, size_t> StreamNumber2MenuPos;
    int32u payload_unit_start_indicator;
    size_t Streams_Count;

    //Elements
    void PSI();
    void Reserved();
    void PES();
    void Null();

    //Helpers
    bool   Synchronize();
    bool   Detect_EOF_Needed();
    void   AdaptationField();
};

} //NameSpace

#endif

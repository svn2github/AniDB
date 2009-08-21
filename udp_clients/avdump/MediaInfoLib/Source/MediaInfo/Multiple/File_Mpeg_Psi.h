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
//
// Information about MPEG files, Program Map Section
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Mpeg_PsiH
#define MediaInfo_Mpeg_PsiH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Multiple/File_MpegTs.h"
#include <map>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpeg_Psi
//***************************************************************************

class File_Mpeg_Psi : public File__Base
{
public :
    //Configuration
    int32u pid;
    int32u payload_unit_start_indicator;
    bool   From_TS;

    //About programs
    struct Program
    {
        std::map<ZenLib::Ztring, ZenLib::Ztring> Info;
        File_MpegTs::ts_kind Kind;

        Program()
        {
            Kind=File_MpegTs::not_needed;
        }
    };
    std::map<int32u, Program> Programs;
    int32u program_number; //Current program number

    //About streams
    struct program_map_Value
    {
        int32u stream_type;
        stream_t KindOfStream;
        std::map<ZenLib::Ztring, ZenLib::Ztring> Info;
        File_MpegTs::ts_kind Kind;

        program_map_Value()
        {
            stream_type=0;
            KindOfStream=Stream_Max;
        }
    };
    std::map<int32u, program_map_Value>         program_map_Values;
    int32u elementary_PID;


    std::vector<int32u>                         program_association_section_ProgramNumber;
    int32u                                      Descriptor_format_identifier;
    //std::map<int32u, File_MpegTs::ts_kind> Kind;

protected :
    //Formats
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

public :
    File_Mpeg_Psi();

private :
    bool Buffer_Parse();
    bool Element_Parse();
    size_t Element_Size;

    //Elements
    bool PayLoad_Parse();
    bool table_id_Parse();
    void Footer();

    //Elements - Base
    void reserved();
    void iso13818_6();
    void user_private();
    void forbidden();

    //Elements - MPEG
    void program_stream_map(); //From PS
    void Table_00();
    void Table_01();
    void Table_02();
    void Table_03();
    void Table_04() {};
    void Table_05() {};
    //Elements - DVB
    void Table_42();
    void Table_46();
    void Table_4E();
    void Table_4F();
    void Table_5F(); //50 --> 5F
    void Table_6F(); //60 --> 6F
    //Elements - ASTC
    void Table_C7();
    void Table_C8();
    void Table_C9() {};
    void Table_CA() {};
    void Table_CB() {};
    void Table_CC() {};
    void Table_CD() {};
    void Table_D3() {};
    void Table_D4() {};

    //Descriptors
    void Descriptors(int32u &Stream_Pos);

    //Temp
    int8u table_id;
    int32u section_length;
    int32u Descriptors_Length;
};

} //NameSpace

#endif

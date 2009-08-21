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
#ifndef MediaInfo_AvcH
#define MediaInfo_AvcH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "ZenLib/BitStream.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Avc
//***************************************************************************

class File_Avc : public File__Base
{
public :
    //Configuration
    size_t Frame_Count_Valid;

protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Avc();

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int32u Element_Size;
    int8u  Element_Name;
    int64u Element_Next;

    //Elements
    void slice_layer_without_partitioning_IDR();
    void slice_layer_without_partitioning_non_IDR();
    void slice_header();
    void seq_parameter_set();
    void pic_parameter_set();
    void sei();
    void access_unit_delimiter();

    //SubElements
    void scaling_list(ZenLib::BitStream &BS, int32u ScalingList_Size);
    void vui_parameters(ZenLib::BitStream &BS, float &PAR);

    //Helpers
    bool NextFrame();
    bool Element_Name_Get();
    bool Element_Size_Get();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    int32u frame_mbs_only_flag;
    int32u log2_max_frame_num_minus4;
};

} //NameSpace

#endif

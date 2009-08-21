// File_Vc1 - Info for MPEG Video files
// Copyright (C) 2004-2007 Jerome Martinez, Zen@MediaArea.net
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
// Information about MPEG Video files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Vc1H
#define MediaInfo_Vc1H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Vc1
//***************************************************************************

class File_Vc1 : public File__Base
{
public :
    //Configuration
    size_t Frame_Count_Valid;

protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    //Public parameters
    int MPEG_Version;

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int8u  Element_Name;
    size_t Element_Size;
    size_t Element_Next;

    //Elements
    void EndOfSequence();
    void Slice();
    void Field();
    void FrameHeader();
    void EntryPointHeader();
    void SequenceHeader();
    void UserDefinedSlice();
    void UserDefinedField();
    void UserDefinedFrameHeader();
    void UserDefinedEntryPointHeader();
    void UserDefinedSequenceHeader();
    size_t Frame_Count;

    //Error controls
    std::vector<int8u> Frame_ShouldBe;

    //Temp
    size_t Width;
    size_t Height;
    size_t RatioValue;
    float32 FrameRate;
    size_t BitRate;

    //Temp - Sequence Header
    int32u interlace;
    int32u tfcntrflag;

    //Helpers
    bool Detect_NonVC1();
    bool Element_Size_Get();
    bool Element_Name_IsOK();
    bool NextFrame();
};

} //NameSpace

#endif

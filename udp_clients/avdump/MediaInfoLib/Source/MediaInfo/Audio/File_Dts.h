// File_Dts - Info for DTS files
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

//---------------------------------------------------------------------------
#ifndef MediaInfo_DtsH
#define MediaInfo_DtsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Dts
//***************************************************************************

class File_Dts : public File__Base
{
public :
    //Configuration
    size_t Frame_Count_Valid;

    //External info
    size_t Delay;

protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int64u Element_Next;
    int32u Element_Size;
    int32u Element_HeaderSize;

    //Element
    void Frame();
    void Frame_Fill();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    bool   Word;
    int32u BigEndian;
    int32u channel_arrangement;
    int32u sample_frequency;
    int32u bit_rate;
    int32u lfe_effects;
    int32u bits_per_sample;
    int32u ExtensionAudioDescriptor;
    int32u ExtendedCoding;

    //Temp - Unknown
    size_t Unknown_Size;

    //Helpers
    bool NextFrame();
    bool Element_Size_Get();
    bool Detect_NonDTS();
};

} //NameSpace

#endif

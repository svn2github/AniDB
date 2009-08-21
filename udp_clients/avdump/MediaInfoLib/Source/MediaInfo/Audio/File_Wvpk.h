// File_Wvpk - Info for WavePack files
// Copyright (C) 2007-2007 Jerome Martinez, Zen@MediaArea.net
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
// Information about WavePack files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_WvpkH
#define MediaInfo_File_WvpkH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#if !defined(MEDIAINFO_TAG_NO)
    #include "MediaInfo/Tag/File__Tags.h"
#else //MEDIAINFO_TAG_NO
    #define File__Tags File__Base
#endif //MEDIAINFO_TAG_NO
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Wvpk
//***************************************************************************

class File_Wvpk : public File__Base, public File__Tags_Helper
{
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
    File_Wvpk() : File__Tags_Helper(this) {};

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    bool Element_Parse_Header();
    int32u Element_Size;
    int64u Element_Next;

    //Element
    void Block();
    void Block_Fill();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    int32u total_samples_FirstFrame;
    int32u block_index_FirstFrame;
    int32u block_index_LastFrame;
    int32u block_samples_LastFrame;
    bool   resolution0;
    bool   resolution1;
    bool   mono;
    bool   hybrid;
    bool   joint_stereo;
    bool   cross_channel_decorrelation;
    int32u SamplingRate;

    //Helpers
    bool NextFrame();
};

} //NameSpace

#endif

// File_Mpega - Info for MPEG Audio files
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
#ifndef MediaInfo_File_MpegaH
#define MediaInfo_File_MpegaH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Tag/File__Tags.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpega
//***************************************************************************

class File_Mpega : public File__Base, public File__Tags_Helper
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
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Mpega();

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    bool Element_Parse_Header();
    int32u Element_Size;
    int64u Element_Next;

    //Element
    void Frame();
    void Frame_Fill();
    bool Header_Xing();
    bool Header_VBRI();
    bool Header_Encoders();
    void Encoded_Library_Guess();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    int32u Mpeg;
    int32u Layer;
    int32u Protection;
    int32u BitRate;
    int32u SamplingRate;
    int32u Padding;
    int32u Private;
    int32u Channels;
    int32u Extension;
    int32u Copyright;
    int32u Original;
    int32u Emphasis;

    //Temp - BitStream info
    std::map<int16u, size_t> BitRate_Count;
    size_t Block_Count[3]; //long, short, mixed
    size_t Channels_Count[4]; //Stereo, Join Stereo, Dual mono, mono
    size_t Extension_Count[4]; //No, IS, MS, IS+MS
    size_t Emphasis_Count[4]; //No, 50/15ms, Reserved, CCITT
    size_t Scfsi; //Total
    size_t Scalefac; //Total
    size_t Reservoir; //Total
    int32u Reservoir_Max;

    //Temp - VBR handling
    int32u VBR_Frames;
    int64u VBR_FileSize;
    Ztring BitRate_Mode;

    //Temp - Tags in a frame
    Ztring Encoded_Library;
    Ztring Encoded_Library_Settings;

    //Helpers
    bool NextFrame();
    bool Element_Size_Get();
    bool Detect_NonMPEGA();
};

} //NameSpace

#endif

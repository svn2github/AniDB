// File_Aac - Info for AAC Audio files
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
#ifndef MediaInfo_File_AacH
#define MediaInfo_File_AacH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#if defined(MEDIAINFO_ID3V2_YES)
    #include "MediaInfo/Tag/File_Id3v2.h"
#else //MEDIAINFO_ID3V2_YES
    #define File_Id3v2 File__Base
#endif //MEDIAINFO_ID3V2_YES
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

class File_Id3;

//***************************************************************************
// Class File_Aac
//***************************************************************************

class File_Aac : public File__Base, public File_Id3v2_Helper
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
    File_Aac() : File_Id3v2_Helper(this) {};

private :
    //Buffer
    bool ADTS_Buffer_Parse();
    bool ADTS_Element_Parse();
    int64u Element_Next;
    int32u Element_Size;

    //Elements
    void AAC();
    void ADIF();
    void ADTS();
    void ADTS_Frame();
    void ES();
    void ES_SBR();
    void ES_SBR_PS();

    //Temp - Global
    size_t Frame_Count;

    //Temp - Technical info
    enum aac_kindofaac
    {
        Kind_Unknown,
        Kind_AAC,
        Kind_ADIF,
        Kind_ADTS,
        Kind_ES
    };
    aac_kindofaac KindOfFormat;
    int32u profile;
    int32u sampling_frequency_index;
    int32u channel_configuration;
    int32u adts_buffer_fullness;
    int32u aac_frame_length;

    //Helpers
    bool ADTS_NextFrame();
};

} //NameSpace

#endif

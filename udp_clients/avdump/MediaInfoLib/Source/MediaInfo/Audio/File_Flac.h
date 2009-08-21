// File_Flac - Info for Flac Audio files
// Copyright (C) 2003-2007 Jasper van de Gronde, th.v.d.gronde@hccnet.nl
// Copyright (C) 2003-2007 Jerome Martinez, zen@mediaarea.net
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
// Information about Flac files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_FlacH
#define MediaInfo_File_FlacH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Flac
//***************************************************************************

class File_Flac : public File__Base
{
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
    int32u Element_Size;
    int16u Element_Name;
    int64u Element_Next;

    //Elements
    void Header();
    void STREAMINFO();
    void PADDING();
    void APPLICATION();
    void SEEKTABLE();
    void VORBIS_COMMENT();
    void CUESHEET();
    void PICTURE();

    //Temp
    bool LastMetaData;

    //Helpers
    bool Element_Name_Get();
    bool Element_Size_Get();
};

//***************************************************************************
// Const
//***************************************************************************

namespace Flac
{
    const int16u STREAMINFO         =0x00;
    const int16u PADDING            =0x01;
    const int16u APPLICATION        =0x02;
    const int16u SEEKTABLE          =0x03;
    const int16u VORBIS_COMMENT     =0x04;
    const int16u CUESHEET           =0x05;
    const int16u PICTURE            =0x06;
}

} //NameSpace

#endif

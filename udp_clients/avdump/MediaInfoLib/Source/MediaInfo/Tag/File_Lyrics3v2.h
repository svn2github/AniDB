// File_Lyrics3v2 - Info for Lyrics3v2 tagged files
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
// Information about Lyrics3v2 tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_Lyrics3v2H
#define MediaInfo_File_Lyrics3v2H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Lyrics3v2
//***************************************************************************

class File_Lyrics3v2 : public File__Base
{
protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    size_t Element_Size;
    int32u Element_Name;
    int64u Element_Next;

    //Elements
    void Header();
    void Footer();
    void AUT();
    void CRC();
    void EAL();
    void EAR();
    void ETT();
    void IMG();
    void IND();
    void INF();
    void LYR();

protected :
    //Information
    void HowTo (stream_t StreamKind);
};

//***************************************************************************
// Const
//***************************************************************************

namespace Lyrics3v2
{
    const int32u AUT=0x415554;
    const int32u CRC=0x435243;
    const int32u EAL=0x45414C;
    const int32u EAR=0x454152;
    const int32u ETT=0x455454;
    const int32u IMG=0x494D47;
    const int32u IND=0x494E44;
    const int32u INF=0x494E46;
    const int32u LYR=0x4C5952;
}

//***************************************************************************
// Class File_Lyrics3v2_Helper
//***************************************************************************

class File_Lyrics3v2_Helper
{
public :
    File_Lyrics3v2_Helper(File__Base* Base_);
    ~File_Lyrics3v2_Helper();

protected :
    //Temp - LYRICS3V2
    File_Lyrics3v2* Lyrics3v2;
    int32u Lyrics3v2_Size;

    //From elsewhere
    bool Lyrics3v2_Read_Buffer_Continue ();
    void Lyrics3v2_Read_Buffer_Finalize ();

    //Data
    File__Base* Base;
};

} //NameSpace

#endif

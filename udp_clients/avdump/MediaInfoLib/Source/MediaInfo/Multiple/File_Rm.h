// File_Rm - Info for Real Media files
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
//
// Information about Real Media files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_RmH
#define MediaInfo_File_RmH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Classe File_Rm
//***************************************************************************

class File_Rm : public File__Base
{
protected :
    //Formats
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    size_t Element_HeaderSize;
    size_t Element_Size;
    int32u Element_Name;
    int64u Element_Next;

    //Elements
    void  RMF();
    void CONT();
    void DATA();
    void INDX();
    void MDPR();
    void MDPR_realvideo();
    void MDPR_realaudio();
    void MDPR_mp3();
    void MDPR_ralf();
    void MDPR_fileinfo();
    void PROP();
    void RJMD();
    void RJMD_property(std::string Name);
    void RMJE();
    void RMMD();
    void TAG ();

    //Temp
    bool MDPR_IsStream;

    //Helpers
    bool Element_Name_Get();
    bool Element_Size_Get();
};

namespace Rm
{
    const int32u  RMF=0x2E524D46;
    const int32u CONT=0x434F4E54;
    const int32u DATA=0x44415441;
    const int32u INDX=0x494E4458;
    const int32u MDPR=0x4D445052;
    const int32u PROP=0x50524F50;
    const int32u RJMD=0x524A4D44;
    const int32u RMJE=0x524D4A45;
    const int32u RMMD=0x524D4D44;
    const int32u TAG =0x54414700;
}

} //NameSpace

#endif

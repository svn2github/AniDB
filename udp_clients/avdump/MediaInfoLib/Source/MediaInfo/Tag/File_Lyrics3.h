// File_Lyrics3 - Info for Lyrics3 tagged files
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
// Information about Lyrics3 tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_Lyrics3H
#define MediaInfo_File_Lyrics3H
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Lyrics3
//***************************************************************************

class File_Lyrics3 : public File__Base
{
protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

private :
    //Buffer
    size_t Element_Size;

    //Elements
    void Header();
    void Footer();
    void Lyrics();

protected :
    //Information
    void HowTo (stream_t StreamKind);
};

//***************************************************************************
// Class File_Lyrics3_Helper
//***************************************************************************

class File_Lyrics3_Helper
{
public :
    File_Lyrics3_Helper(File__Base* Base_);
    ~File_Lyrics3_Helper();

protected :
    //Temp - LYRICS3V2
    File_Lyrics3* Lyrics3;
    int32u Lyrics3_Size;

    //From elsewhere
    bool Lyrics3_Read_Buffer_Continue ();
    void Lyrics3_Read_Buffer_Finalize ();

    //Data
    File__Base* Base;
};

} //NameSpace

#endif

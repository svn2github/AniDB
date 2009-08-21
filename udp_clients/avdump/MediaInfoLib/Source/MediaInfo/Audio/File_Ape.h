// File_Ape - Info for Ape Audio files
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
// Information about Ape files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_ApeH
#define MediaInfo_File_ApeH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Ape
//***************************************************************************

class File_Ape : public File__Base
{
protected :
    //Format
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Buffer
    size_t Element_Size;

    //Elements
    void Header();
    void Footer();
    void APETag();
    void APETagv1();
    void APETagv2();
    void APETag_Common();
    void Id3v1();

    //Temp
    int64u UncompressedSize;
};

} //NameSpace

#endif


// File_ApeTag - Info for APETAG tagged files
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
// Information about APETAG tagged files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_ApeTagH
#define MediaInfo_File_ApeTagH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_ApeTag
//***************************************************************************

class File_ApeTag : public File__Base
{
protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();

private :
    //Buffer
    size_t Element_Size;

    //Elements
    void v2();
    void Common();

protected :
    //Information
    void HowTo (stream_t StreamKind);
};

//***************************************************************************
// Class File_ApeTag_Helper
//***************************************************************************

class File_ApeTag_Helper
{
public :
    File_ApeTag_Helper(File__Base* Base_);
    ~File_ApeTag_Helper();

protected :
    //Temp - APETAG
    File_ApeTag* ApeTag;

    //From elsewhere
    bool ApeTag_Read_Buffer_Continue ();
    void ApeTag_Read_Buffer_Finalize ();

    //Data
    File__Base* Base;
};

} //NameSpace

#endif

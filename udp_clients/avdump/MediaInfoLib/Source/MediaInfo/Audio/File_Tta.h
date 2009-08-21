// File_Tta - Info for TTA files
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
// Information about TTA files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_TtaH
#define MediaInfo_File_TtaH
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
// Class File_Tta
//***************************************************************************

class File_Tta : public File__Base, public File__Tags_Helper
{
protected :
    //Format
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Tta() : File__Tags_Helper(this) {};

private :
    //Buffer
    bool Buffer_Parse();
    int32u Element_Size;

    //Element
    void Header();
};

} //NameSpace

#endif

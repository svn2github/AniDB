// File_Bdav - Info for BluRay Audio/Video files
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
// Information about MPEG Transport Stream files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_BdavH
#define MediaInfo_BdavH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Multiple/File_MpegTs.h"
#include <vector>
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Bdav
//***************************************************************************

class File_Bdav : public File__Base
{
protected :
    //Formats
    void Read_Buffer_Init ();
    void Read_Buffer_Unsynched ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    File_Bdav();
    ~File_Bdav();

private :
    bool Buffer_Parse();
    bool Frame_Parse();
    bool Element_Parse();
    int16u Element_Name;
    size_t Element_Size;
    size_t Element_HeaderSize;
    size_t Element_Next;

    //Elements
    void PES();

    //Temp - Ts
    File_MpegTs* TS_Handle;

    //Helpers
    bool   Synchronize();
    bool   Detect_EOF_Needed();
    void   AdaptationField();
    void   Frame_Parse_Scrambled();
};

} //NameSpace

#endif

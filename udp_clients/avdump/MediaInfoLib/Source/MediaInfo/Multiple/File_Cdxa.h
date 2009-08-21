// File_Cdxa - Info for CDXA files
// Copyright (C) 2004-2007 Jerome Martinez, Zen@MediaArea.net
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
// Information about CDXA files
// (like Video-CD...)
// CDXA are read by MS-Windows with CRC bytes
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_CdxaH
#define MediaInfo_File_CdxaH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

class MediaInfo;

//***************************************************************************
// Class File_Cdxa
//***************************************************************************

class File_Cdxa : public File__Base
{
public :
    //Configuration
    size_t Frame_Count_Valid;

protected :
    //Format
    void Read_Buffer_Init     ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

public :
    //Constructor/Destructor
    File_Cdxa();
    ~File_Cdxa();

private :
    //Buffer
    bool Buffer_Parse ();
    bool Element_Parse();
    size_t Element_Size;
    size_t Element_Next;

    //Element
    void Header();
    void Frame();

    //Temp - Global
    size_t Frame_Count;

    //Helpers
    bool NextFrame();
    bool Element_Size_Get();

    bool IsHeader();
    bool Detect_EOF_Needed();

    //Temp
    MediaInfo* MI;
};

} //NameSpace

#endif

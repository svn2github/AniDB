// File_Wm - Info for Windows Media files
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
// Main part
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_WM_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Wm.h"
#if defined(MEDIAINFO_MPEGPS_YES)
    #include "MediaInfo/Multiple/File_MpegPs.h"
#endif
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wm::Read_Buffer_Init()
{
    //Buffer
    Element_Level=0;
    Element_Name[0][0]=0;
    Element_Name[0][1]=0;
    Element_Name[0][2]=0;
    Element_Name[0][3]=0;
    Element_Next[0]=File_Size;
}

//---------------------------------------------------------------------------
void File_Wm::Read_Buffer_Continue()
{
    //Normal parsing
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Wm::Read_Buffer_Finalize()
{
    //Encryption management
    const Ztring& Encryption=Get(Stream_General, 0, "Encryption");
    if (!Encryption.empty())
    {
        for (size_t StreamKind=Stream_General+1; StreamKind<Stream_Max; StreamKind++)
            for (size_t Pos=0; Pos<(*Stream[StreamKind]).size(); Pos++)
                Fill ((stream_t)StreamKind, 0, "Encryption", Encryption);
    }

}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
//
bool File_Wm::Buffer_Parse()
{
    //Element Level
    if (!Element_Level_Get())
        return false;

    //Going in a lower level
    Element_Level++;

    //Element Name
    if (!Element_Name_Get())
    {
        Element_Level--;
        return false;
    }

    //Element Size
    if (!Element_Size_Get())
    {
        Element_Level--;
        return false;
    }

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes, Element_Level-- is done in Element_Parse() if needed

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    if (Element_Next[Element_Level]-File_Offset>0x100000000LL)
    {
        File_GoTo=Element_Next[Element_Level]-File_Offset;
        return false;
    }
    Buffer_Offset=(size_t)(Element_Next[Element_Level]-File_Offset);
    Element_Name[Element_Level][0]=0;
    Element_Name[Element_Level][1]=0;
    Element_Name[Element_Level][2]=0;
    Element_Name[Element_Level][3]=0;
    Element_Next[Element_Level]=0;
    Element_Level--;
    return true;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Wm::Element_Level_Get()
{
    //Integrity
    if (Element_Level>=WM_ELEMENT_LEVEL_MAX)
    {
        File_Offset=File_Size;
        return false;
    }

    //End of this level?
    while (File_Offset+Buffer_Offset>=Element_Next[Element_Level])
    {
        if (Element_Level==0)
        {
            File_Offset=File_Size;
            return false;
        }
        Element_Level--; //To next element
    }

    return true;
}

//---------------------------------------------------------------------------
bool File_Wm::Element_Name_Get()
{
    //Enough data?
    if (Buffer_Offset+24>Buffer_Size)
        return false;

    //Element name
    Element_Name[Element_Level][0]=LittleEndian2int32u(Buffer+Buffer_Offset+0);
    Element_Name[Element_Level][1]=LittleEndian2int32u(Buffer+Buffer_Offset+4);
    Element_Name[Element_Level][2]=CC4(Buffer+Buffer_Offset+8);
    Element_Name[Element_Level][3]=CC4(Buffer+Buffer_Offset+12);

    return true;
}

//---------------------------------------------------------------------------
bool File_Wm::Element_Size_Get()
{
    //Enough data?
    if (Buffer_Offset+24>Buffer_Size)
        return false;
    if (Element_Next[Element_Level-1]-(File_Offset+Buffer_Offset)<4)
    {
        //Corrupted
        TRUSTED_ISNOT("Not enough place to have a size");
        if (Element_Level>0)
            Element_Size=Element_Next[Element_Level-1]-(File_Offset+Buffer_Offset); //Maximum limit
        else
        {
            //Impossible to know where we are
            File_Offset=File_Size;
            return false;
        }
    }
    //Element size
    Element_Size=LittleEndian2int64u(Buffer+Buffer_Offset+16);
    if (Element_Size<24)
    {
        //Not in specs!
        Element_Size=24;
    }

    //Element Next
    Element_Next[Element_Level]=File_Offset+Buffer_Offset+Element_Size;

    //Integrity of element
    if (Element_Next[Element_Level]>Element_Next[Element_Level-1])
    {
        TRUSTED_ISNOT("Atom is too big, resizing...", Element_Level-1);
        Element_Next[Element_Level]=Element_Next[Element_Level-1];
        if (Element_Next[Element_Level]>(File_Offset+Buffer_Offset))
            Element_Size=Element_Next[Element_Level]-(File_Offset+Buffer_Offset);
        else
        {
            TRUSTED_ISNOT("Not enough place to have an atom", Element_Level-1);
            Element_Size=0; //Not enough place to have an atom
        }
    }

    Element_Size-=24;
    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wm::HowTo(stream_t UNUSED(StreamKind))
{
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES

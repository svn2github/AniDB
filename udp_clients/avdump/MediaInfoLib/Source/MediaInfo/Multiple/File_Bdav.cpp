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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_BDAV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Bdav.h"
#include "MediaInfo/Multiple/File_MpegTs.h"
#include "ZenLib/BitStream.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Bdav::File_Bdav()
{
    TS_Handle=NULL;
}

//---------------------------------------------------------------------------
File_Bdav::~File_Bdav()
{
    delete TS_Handle; //TS_Handle=NULL
}

//***************************************************************************
// Base
//***************************************************************************

//---------------------------------------------------------------------------
void File_Bdav::Read_Buffer_Init()
{
    Synched=false;

    TS_Handle=NULL;
}

//---------------------------------------------------------------------------
void File_Bdav::Read_Buffer_Unsynched()
{
    if (Config.Details_Get())
    {
        Details_Add_Error("BDAV, Will resync...");
    }

    Synched=false;
}

//---------------------------------------------------------------------------
void File_Bdav::Read_Buffer_Continue()
{
    //Look for first Sync word
    if (!Synched && !Synchronize())
        return;

    //Fill buffers with demuxed streams
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Bdav::Read_Buffer_Finalize()
{
    //Streams
    if (TS_Handle)
    {
        TS_Handle->Open_Buffer_Finalize();
        Merge (*TS_Handle);
    }

    //Fill General
    Fill(Stream_General, 0, "Format", "BDAV", Unlimited, false, true);

    return;
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Bdav::Buffer_Parse()
{
    //Element size
    Element_Next=Buffer_Offset+192;

    //Parsing
    if (!Frame_Parse())
        return false;

    //Jumping to the end of the file if needed
    if (Detect_EOF_Needed())
        return false;

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
bool File_Bdav::Frame_Parse()
{
    //Enough data?
    if (Buffer_Offset+192>Buffer_Size)
        return false;

    //Header
    Element_Size=192;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    ELEMENT_END();
    BS_BEGIN();
    SKIP_BS(32,                                                 BDAV);

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Parsing
    Element_Parse();

    return true;
}

//---------------------------------------------------------------------------
bool File_Bdav::Element_Parse()
{
    ELEMENT(0, "Block", 192);

    Buffer_Offset+=4;
    Element_Size=192-4;

    PES();

    FLUSH();

    return true;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Bdav::Synchronize()
{
    while (           Buffer_Offset+192*4<Buffer_Size
      && !(CC1(Buffer+Buffer_Offset+192*0+4)==0x47
        && CC1(Buffer+Buffer_Offset+192*1+4)==0x47
        && CC1(Buffer+Buffer_Offset+192*2+4)==0x47
        && CC1(Buffer+Buffer_Offset+192*3+4)==0x47))
        Buffer_Offset++;
    if (Buffer_Offset+192*4>=Buffer_Size)
    {
        if (Synched)
        {
            //Sync lost
            if (Config.Details_Get())
            {
                Details_Add_Error("-------------------------------------------");
                Details_Add_Error("---   BDAV, Synchronisation lost     ---");
                Details_Add_Error("-------------------------------------------");
            }
            Synched=false;
        }

        //Managing first Synch attempt
        if (Count_Get(Stream_General)==0)
            File_Offset=File_Size; //End

        return false;
    }

    //There is no start code, so Stream_General is filled here
    if (Count_Get(Stream_General)==0)
        Stream_Prepare(Stream_General);

    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Bdav::Detect_EOF_Needed()
{
    //Jump to the end of the file
    if (File_Size>=File_Offset+Buffer_Size+65536+0*1024*1024 && File_Size!=(int64u)-1 && (
       (File_Offset>=2*1024*1024)
    ))
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("-------------------------------------------");
            Details_Add_Error("---   BDAV, Jumping to end of file   ---");
            Details_Add_Error("-------------------------------------------");
        }

        File_GoTo=File_Size-0*1024*1024;
        return true;
    }

    return false;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Bdav::PES()
{
    //FLUSH();
    //ELEMENT(1, "TS", 188);

    //Allocating an handle if needed
    if (TS_Handle==NULL)
        TS_Handle=new File_MpegTs;

    //Details
    size_t Details_Info_Temp_Size_Before=Details_Info_Temp.size();

    //Open MPEG-TS
    if (TS_Handle && (TS_Handle->File_GoTo==(int64u)-1 || TS_Handle->File_GoTo<File_Offset+Buffer_Offset) && TS_Handle->File_Offset!=TS_Handle->File_Size)
    {
        Open_Buffer_Init(TS_Handle, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(TS_Handle, Buffer+Buffer_Offset, Element_Size);

        //Need anymore?

        if (TS_Handle->File_GoTo!=(int64u)-1 || TS_Handle->File_Offset==TS_Handle->File_Size)
            File_GoTo=TS_Handle->File_GoTo;
    }

    //Details
    if (Details_Info_Temp.size()==Details_Info_Temp_Size_Before)
        PURGE(); //Nothing has changed

    //Demux
    DEMUX(Buffer+Buffer_Offset, 188, _T("RealTS.ts"));
}

//---------------------------------------------------------------------------
void File_Bdav::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            break;
        case (Stream_Text) :
            break;
        case (Stream_Chapters) :
            break;
        case (Stream_Image) :
            break;
        case (Stream_Menu) :
            break;
        case (Stream_Max) :
            break;
    }
}

} //NameSpace

#endif //MEDIAINFO_BDAV_YES

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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_CDXA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Cdxa.h"
#include "ZenLib/Utils.h"
#include "MediaInfo/MediaInfo.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Format
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// CDXA = RIFF header + Raw sectors
// Riff header size = 44
// Raw sector size = 2352
//
// Raw sector :
// Sync             12 bytes (00 FF .. FF 00)
// Header           4 bytes
// SubHeader        8 bytes
// Datas            2324 bytes
// EDC (CRC)        4 bytes
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
File_Cdxa::File_Cdxa()
{
    MI=NULL;
}

//---------------------------------------------------------------------------
File_Cdxa::~File_Cdxa()
{
    delete MI; //MI=NULL;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Cdxa::Read_Buffer_Init ()
{
    //Configuration
    File_MaximumOffset=64*1024;
    Frame_Count_Valid=16;

    //Buffer
    Element_Size=0;

    //Temp - Global
    Frame_Count=0;

    //Pointers
    delete MI; MI=NULL;
}

//---------------------------------------------------------------------------
void File_Cdxa::Read_Buffer_Continue ()
{
    //Trusted limitation
    Trusted=100;

    //Can not be included in another container
    if (MI==NULL && File_Offset!=0)
    {
        File_Offset=File_Size;
        return;
    }

    //Coherancy
    if (File_Offset==0)
    {
        //Integrity
        if (Buffer_Size<44)
            return;

        //Looking for header
        while (Buffer_Offset<Buffer_Size-44 && !IsHeader())
            Buffer_Offset++;
        if (Buffer_Offset>=Buffer_Size-44)
        {
            File_Offset=File_Size;
            return;
        }

        Header();
    }

    //Parsing
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Cdxa::Read_Buffer_Finalize ()
{
    //If nothing
    if (MI->Info==NULL)
    {
        General[0](_T("Format"))=Ztring(_T("CDXA"));
        General[0](_T("Format/String"))=Ztring(_T("CD Mode 2"));
        return;
    }

    //General
    MI->Open_Buffer_Finalize();
    Merge(*(MI->Info));
    Merge(*(MI->Info), Stream_General, 0, 0);
    General[0](_T("Format"))=Ztring(_T("CDXA/"))+General[0](_T("Format"));
    General[0](_T("Format/String"))=Ztring(_T("CD Mode 2 / "))+General[0](_T("Format/String"));
    Fill(Stream_General, 0, "PlayTime", "");
    Fill(Stream_Video, 0, "PlayTime", "");
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Cdxa::Buffer_Parse()
{
    //Search for header
    if (!NextFrame())
        return false; //Header not found

    //Parsing
    if (!Element_Parse())
        return false;

    //If no need of more
    if (File_Offset+Buffer_Offset>1024*1024)
        FINNISHED_BOOL();

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
// Chunk, 2352 bytes
// Sync                                 12 bytes
// Header                               4 bytes
// SubHeader                            8 bytes
// Data                                 2324 bytes
// CRC                                  4 bytes
//
bool File_Cdxa::Element_Parse()
{
    //Enough data?
    if (Buffer_Offset+24>Buffer_Size)
        return false;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=24;
    CC_BEGIN();
    SKIP_B4(                                                    Sync1);
    SKIP_B4(                                                    Sync2);
    SKIP_B4(                                                    Sync3);
    SKIP_B4(                                                    Header);
    SKIP_B8(                                                    SubHeader);
    CC_END_CANBEMORE();

    //Element size
    if (!Element_Size_Get())
        return false;

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    //Element
    ELEMENT(0, "Chunk", Element_Size);

    //Positioning
    Buffer_Offset+=24;

    //Parsing
    Frame();

    //Parsing
    if (Element_Size==2324)
    {
        //Configure defines : must return false
        #undef INTEGRITY_RETURNVALUE
        #define INTEGRITY_RETURNVALUE false

        Buffer_Offset+=2324;
        Element_Size=4;
        CC_BEGIN();
        SKIP_B4(                                                CRC);
        CC_END();

        //UnConfigure defines : must return nothing
        #undef INTEGRITY_RETURNVALUE
        #define INTEGRITY_RETURNVALUE
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Cdxa::Header()
{
    ELEMENT(0, "CDXA Header", 44);

    //Parsing
    Element_Size=44;
    CC_BEGIN();
    SKIP_C4(                                                    RIFF header);
    SKIP_L4(                                                    RIFF data size);
    SKIP_C4(                                                    CDXA);
    SKIP_C4(                                                    fmt header);
    SKIP_L4(                                                    fmt size);
    SKIP_L2(                                                    user_id);
    SKIP_L2(                                                    group_id);
    SKIP_L2(                                                    attributes);
    SKIP_C2(                                                    xa_signature);
    SKIP_L4(                                                    xa_track_number);
    SKIP_L4(                                                    reserved);
    SKIP_C4(                                                    data header);
    SKIP_L4(                                                    data size);
    CC_END();

    FLUSH();

    //Skip RIFF Header
    Buffer_Offset+=44;

    if (MI==NULL)
    {
        MI=new MediaInfo;
        Stream_Prepare(Stream_General);
    }
}

//---------------------------------------------------------------------------
void File_Cdxa::Frame()
{
    //Counting
    if (Element_Next==File_Size)
        Frame_Count_Valid=Frame_Count; //Finalize frames in case of there are less than Frame_Count_Valid frames
    Frame_Count++;

    //Name
    NAME(Ztring::ToZtring(Frame_Count));

    //Parsing
    CC_BEGIN()
    SKIP_XX(Element_Size,                                       Data);
    CC_END();

    MI->Open_Buffer_Init(File_Size, File_Offset+Buffer_Offset);
    MI->Open_Buffer_Continue(Buffer+Buffer_Offset, Element_Size);

    //Details
    #ifndef MEDIAINFO_MINIMIZESIZE
    if (Config.Details_Get())
    {
        if (!MI->Inform().empty())
        {
            FLUSH();
            if (Details.find(EOL)!=std::string::npos)
                Details+=EOL;
            Details+=MI->Inform();
        }
    }
    #endif //MEDIAINFO_MINIMIZESIZE
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Cdxa::NextFrame()
{
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+2352*2+12<=Buffer_Size && !( //3 frames
        CC8(Buffer+Buffer_Offset_Temp+2352*0)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset_Temp+2352*0+8)==0xFFFFFF00
     && CC8(Buffer+Buffer_Offset_Temp+2352*1)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset_Temp+2352*1+8)==0xFFFFFF00
     && CC8(Buffer+Buffer_Offset_Temp+2352*2)==0x00FFFFFFFFFFFFFFLL && CC4(Buffer+Buffer_Offset_Temp+2352*2+8)==0xFFFFFF00
    ))
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+2352*2+12>Buffer_Size)
    {
        if (Buffer_Offset_Temp+2352*2+12-1==Buffer_Size)
        {
            Buffer_Offset_Temp++;
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;
        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+2352*2+12>Buffer_Size)
        return false;

    //Error in stream?
    if (Buffer_Offset_Temp-Buffer_Offset>0)
    {
        if (Synched)
            TRUSTED_ISNOT("Sync error", 0, Buffer_Offset_Temp-Buffer_Offset);
        else
        {
            ELEMENT(0, "Synchronization", Buffer_Offset_Temp-Buffer_Offset);
            FLUSH();
        }
    }

    //OK, we continue
    Buffer_Offset=Buffer_Offset_Temp;
    Buffer_Offset_Temp=0;
    Synched=true;
    return true;
}

//---------------------------------------------------------------------------
bool File_Cdxa::Element_Size_Get()
{
    //Calculating
    if (Buffer_Offset+2352>Buffer_Size)
    {
        //Uncompleted frame management
        if (File_Offset+Buffer_Size>=File_Size)
        {
            //End of file
            if (Buffer_Offset+24>Buffer_Size)
                Element_Size=Buffer_Size-(Buffer_Offset+24);
            else
                return false; //Not enough place
        }
        else
            return false; //Wait for data
    }
    else
        //Complete frame
        Element_Size=2352-24-4;

    //OK, we continue
    Element_Next=File_Offset+Buffer_Offset+24+Element_Size+4;
    return true;
}

//---------------------------------------------------------------------------
bool File_Cdxa::Detect_EOF_Needed()
{
    //Jump to the end of the file
    if (File_Size!=(int64u)-1 && File_Size>File_Offset+Buffer_Size+1*1024*1024 && (
       (File_Offset>1*1024*1024)
    || (MI!=NULL && MI->Open_Buffer_Continue_GoTo_Get()!=(int64u)-1)
    ))
    {
        Details_Add_Error("----------------------------------------");
        Details_Add_Error("---   CDXA, Jumping to end of file   ---");
        Details_Add_Error("----------------------------------------");
        File_GoTo=File_Size-1*1024*1024;
        return true;
    }

    return false;
}

//---------------------------------------------------------------------------
// RIFF Header, 44 bytes
// RIFF header                      4 bytes, Pos=0
// RIFF data size                   4 bytes, Pos=4
// Format (CDXA)                    4 bytes, Pos=8
// Format Header                    4 bytes, Pos=12
// Format Size                      4 bytes, Pos=16
// Format user_id                   2 bytes, Pos=20
// Format group_id                  2 bytes, Pos=22
// Format attributes                2 bytes, Pos=24
// Format xa_signature              2 bytes, Pos=26 ("XA")
// Format xa_track_number           4 bytes, Pos=28
// Format Reserved                  4 bytes, Pos=32
// Data Header                      4 bytes, Pos=36
// Data Size                        4 bytes, Pos=40
//
// Attributes (big endian):
// 15 Directory
// 14 CDDA
// 13 Interleaved
// 12 Mode2Form2 --> 2324 bytes/block
// 11 Mode2Form1 --> 2048 bytes/block
// 10 Exec_Other
// 09 Reserved
// 08 Read_Other
// 07 Reserved
// 06 Exec_Group
// 05 Reserved
// 04 Read_Group
// 03 Reserved
// 02 Exec_User
// 01 Reserved
// 00 Read_User
//
bool File_Cdxa::IsHeader()
{
    if (                           Buffer_Offset<Buffer_Size-0x2C &&
                        CC4(Buffer+Buffer_Offset+0x00)==CC4("RIFF")
     && LittleEndian2int32u(Buffer+Buffer_Offset+0x04)==LittleEndian2int32u(Buffer+Buffer_Offset+0x28)+0x24 //Sizes of chunks
     &&                 CC4(Buffer+Buffer_Offset+0x08)==CC4("CDXA")
     &&                 CC4(Buffer+Buffer_Offset+0x0C)==CC4("fmt ")
     && LittleEndian2int32u(Buffer+Buffer_Offset+0x10)==0x10
     &&                 CC4(Buffer+Buffer_Offset+0x24)==CC4("data")
       )
        return true;
    else
        return false;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Cdxa::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R|Overall BitRate - Audio BitRates");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("AspectRatio", "R");
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

#endif //MEDIAINFO_CDXA_YES


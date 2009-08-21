// File__Analysze - Base for analyze files
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
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Analyze.h"
#include "MediaInfo/MediaInfo_Config.h"
#include "ZenLib/File.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
extern MediaInfo_Config Config;
//---------------------------------------------------------------------------

//Integrity test
#define INTEGRITY(TOVALIDATE, ERRORTEXT, OFFSET) \
    if (!(TOVALIDATE)) \
    { \
        Trusted_IsNot(ERRORTEXT); \
        Element_Pos=Element_Size; \
        return; \
    } \

#define INTEGRITY_INT(TOVALIDATE, ERRORTEXT, OFFSET) \
    if (!(TOVALIDATE)) \
    { \
        Trusted_IsNot(ERRORTEXT); \
        Element_Pos=Element_Size; \
        Info=0; \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST(_BYTES) \
    if (Buffer_Offset+Element_Pos+_BYTES>Buffer_Size) \
    { \
        Trusted_IsNot("Size is wrong"); \
        Element_Pos=Element_Size; \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST_STRING(_BYTES) \
    if (Buffer_Offset+Element_Pos+_BYTES>Buffer_Size) \
    { \
        Trusted_IsNot("Size is wrong"); \
        Element_Pos=Element_Size; \
        Info.clear(); \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST_INT(_BYTES) \
    if (Buffer_Offset+Element_Pos+_BYTES>Buffer_Size) \
    { \
        Trusted_IsNot("Size is wrong"); \
        Element_Pos=Element_Size; \
        Info=0; \
        return; \
    } \


//***************************************************************************
// Constructor/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::ToTS(int16u PID, int16u Value)
{
    //To change... Gestion du PID 0
    if (PID==0)
    {
        Config.File_Duplicate_Get(0);
        std::map<int16u, MediaInfo_Config::file_duplicate*>::iterator Temp=Config.File_Duplicate_16.begin();
        while (Temp!=Config.File_Duplicate_16.end())
        {
            ToTS(1, Temp->first);
            Temp++;
        }
        return;
    }

    if (!Config.File_Duplicate_Get(Value))
        return;

    switch (Config.File_Duplicate_16[Value]->Method)
    {
        case MediaInfo_Config::file_duplicate::method_filename :
        case MediaInfo_Config::file_duplicate::method_filename_ID :
            if (ToTS_File_16[Value]==NULL)
            {
                ToTS_File_16[Value]=new File;
                Ztring Name;
                if (Config.File_Duplicate_16[Value]->Method==MediaInfo_Config::file_duplicate::method_filename)
                    Name=Config.File_Duplicate_16[Value]->FileName;
                else
                    Name=File_Name+_T('.')+Ztring::ToZtring(Value);
                ((File*)ToTS_File_16[Value])->Open(Name, File::Access_Write_Append);
            }
            ((File*)ToTS_File_16[Value])->Write(Buffer+Buffer_Offset-(size_t)HeaderSize, Element_Size+HeaderSize);
            break;
        case MediaInfo_Config::file_duplicate::method_buffer :
        case MediaInfo_Config::file_duplicate::method_buffer_nodelete :
            Fill_Output_Buffer(Value, Buffer+Buffer_Offset-(size_t)HeaderSize, Element_Size+HeaderSize);
            break;
        default : ;
    }
}

//---------------------------------------------------------------------------
File__Analyze::File__Analyze ()
:File__Base()
{
    //Synchro
    Synched=false;

    //Header
    HeaderSize=0;

    //Element
    Element_Pos=0;
    Element_Size=0;

    //Elements
    Element.resize(64);
    Element_Level_Base=0;
    Element_Level=0;
    Element_Info_Level_=0;

    //BitStream
    BS=new BitStream;

    //File_Duplicate_Get
    //ToTS_File=NULL;
}

//---------------------------------------------------------------------------
File__Analyze::~File__Analyze ()
{
    //File_Duplicate
    std::map<int16u, void*>::iterator Temp=ToTS_File_16.begin();
    while (Temp!=ToTS_File_16.end())
    {
        delete (File*)Temp->second; //Temp=NULL;
        Temp++;
    }

    //BitStream
    delete BS; //BS=NULL;
}

//***************************************************************************
// Subs
//***************************************************************************

void File__Analyze::Open_Buffer_Continue (File__Analyze* Sub, const int8u* ToAdd, size_t ToAdd_Size)
{
    //Sub
    Sub->Element_Level_Base=Element_Level_Base+Element_Level;
    File__Base::Open_Buffer_Continue(Sub, ToAdd, ToAdd_Size);

    //ToShow
    if (!Sub->Element[0].ToShow.Details.empty())
    {
        //Line separator
        if (!Element[Element_Level].ToShow.Details.empty())
            Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

        //From Sub
        Element[Element_Level].ToShow.Details+=Sub->Element[0].ToShow.Details;
        Sub->Element[0].ToShow.Details.clear();
    }
    else
        Element[Element_Level].ToShow.NoShow=true; //We don't want to show this item because there is no info
}


//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Init ()
{
    //Elements
    Element[0].Code=0;
    Element[0].Next=File_Size;
    Element[0].WaitForMoreData=false;
    Element[0].UnTrusted=false;
    Element[0].IsComplete=false;
    Element[0].InLoop=false;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[0].ToShow.Name.clear();
        Element[0].ToShow.Info.clear();
        Element[0].ToShow.Details.clear();
        Element[0].ToShow.NoShow=false;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Continue ()
{
    //Parsing
    while (Buffer_Parse());

    //Jumping to the end of the file if needed
    Detect_EOF();

    //ToShow
    if (File_GoTo==File_Size || File_Offset==File_Size || File_Offset+Buffer_Offset==File_Size)
    {
        //Flushing if needed
        while (Element_Level>0)
            Element_End(); //This is a buffer restart, must sync to Element level
    }
    Details=Element[0].ToShow.Details;
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Unsynched()
{
    Synched=false;
}

//---------------------------------------------------------------------------
void File__Analyze::Read_Buffer_Finalize ()
{
    //File_Duplicate
    std::map<int16u, void*>::iterator Temp=ToTS_File_16.begin();
    while (Temp!=ToTS_File_16.end())
    {
        delete (File*)Temp->second; //Temp=NULL;
        Temp++;
    }
    ToTS_File_16.clear();
}

//---------------------------------------------------------------------------
bool File__Analyze::Buffer_Parse()
{
    //End of this level?
    if (File_Offset+Buffer_Offset>=Element[Element_Level].Next)
    {
        if (Element[Element_Level].InLoop)
            return false; //To next element
        else
        {
            //There is no loop handler, so we make the level down here
            while (Element_Level>0 && File_Offset+Buffer_Offset>=Element[Element_Level].Next)
                Element_End(); //This is a buffer restart, must sync to Element level
            if (File_Offset+Buffer_Offset==File_Size)
                return false; //End of file
        }
    }

    //Header
    if (!Header_Manage())
        return false; //Wait for more data

    //Data
    if (!Data_Manage())
        return false; //Wait for more data

    return true;
}

//***************************************************************************
// Init
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::BS_Begin()
{
    size_t BS_Size;
    if (Buffer_Offset+Element_Size<=Buffer_Size)
        BS_Size=Element_Size-Element_Pos;
    else
        BS_Size=Buffer_Size-(Buffer_Offset+Element_Pos);
    BS->Attach(Buffer+Buffer_Offset+Element_Pos, BS_Size);
}

//---------------------------------------------------------------------------
void File__Analyze::BS_End()
{
    BS->Byte_Align();
    Element_Pos+=BS->Offset_Get();
    BS->Attach(NULL, 0);
}

//***************************************************************************
// Header
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Header_Manage()
{
    //From the parser
    if (!Header_Begin())
        return false; //Wait for more data

    //Going in a lower level
    Element_Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Pos+BS->Offset_Get());
    if (Buffer_Offset+Element_Size>Buffer_Size)
    {
        Element_Size=Buffer_Size-Buffer_Offset;
        Element[Element_Level].IsComplete=false;
    }
    else
        Element[Element_Level].IsComplete=true;
    if (Element_Size==0)
        return false;
    Element_Pos=0;
    Element_Begin(); //Element
    Element_Begin("Header"); //Header

    //From the parser
    Header_Parse();

    //Testing the parser result
    if (Element_WaitForMoreData())
    {
        //The header is not complete, need more data
        Element_End(); //Header
        Element_End(); //Element
        return false;
    }

    //From the parser
    if (!Header_End())
    {
        //The header has a problem, skip it
        Element_End(); //Header
        Element_End(); //Element
        return false;
    }

    //Filling
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=false;
    Element[Element_Level].IsComplete=true;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element_Pos;
        Element[Element_Level].ToShow.HeaderSize=0;
        Element[Element_Level-1].ToShow.HeaderSize=HeaderSize;
    }

    //Positionning
    Element_Size=Element[Element_Level-1].Next-(File_Offset+Buffer_Offset+Element_Pos+BS->Offset_Get());
    HeaderSize=Element_Pos;
    Buffer_Offset+=HeaderSize;
    Element_Pos=0;

    if (Buffer_Offset+Element_Size>Buffer_Size)
        Element[Element_Level].IsComplete=false;

    Element_End(); //Header
    return true;
}

//---------------------------------------------------------------------------
void File__Analyze::Header_Fill_Code(int64u Code, const Ztring &Name)
{
    //Filling
    Element[Element_Level-1].Code=Code;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Name=Name;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Header_Fill_Size(int64u Size)
{
    if (Element[Element_Level].UnTrusted)
        return;

    //Filling
    Element[Element_Level-1].Next=File_Offset+Buffer_Offset+Size;
    if (Element[Element_Level-1].Next>Element[Element_Level-2].Next)
    {
        Element[Element_Level-1].Next=Element[Element_Level-2].Next;
        Element[Element_Level-1].IsComplete=false;
    }
    else
        Element[Element_Level-1].IsComplete=true;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level-1].ToShow.Pos=File_Offset+Buffer_Offset;
        Element[Element_Level-1].ToShow.Size=Element[Element_Level-1].Next-(File_Offset+Buffer_Offset);
    }
}

//***************************************************************************
// Data
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Data_Manage()
{
    if (!Data_Parse())
    {
        Buffer_Offset-=HeaderSize;
        Element[Element_Level].IsComplete=false;
        Element_End(); //Element
        return false;
    }
    Element[Element_Level].IsComplete=true;

    //Next element
    Element_Size=0;
    Element_Pos=0;
    Buffer_Offset=(size_t)(Element[Element_Level].Next-File_Offset);
    if (Buffer_Offset>Buffer_Size)
    {
        Element[Element_Level].WaitForMoreData=true;
        if (File_Offset!=File_Size)
            File_GoTo=File_Offset+Buffer_Offset;
    }

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false; //Element_End will be done because of false

    Element_End(); //Element
    return true;
}

//***************************************************************************
// Element
//***************************************************************************

//---------------------------------------------------------------------------
Ztring Log_Offset (int64u OffsetToShow)
{
    if (OffsetToShow==(int64u)-1)
        return _T("         ");
    int64u Offset=OffsetToShow%0x100000000ULL; //Only 32 bits
    Ztring Pos1; Pos1.From_Number(Offset, 16);
    Ztring Pos2;
    Pos2.resize(8-Pos1.size(), _T('0'));
    Pos2+=Pos1;
    Pos2.MakeUpperCase();
    Pos2+=_T(' ');
    return Pos2;
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Begin()
{
    //Level
    Element_Level++;
    HeaderSize=0;

    //Element
    Element[Element_Level].Code=0;
    Element[Element_Level].Next=Element[Element_Level-1].Next;
    Element[Element_Level].WaitForMoreData=Element[Element_Level-1].WaitForMoreData;
    Element[Element_Level].UnTrusted=Element[Element_Level-1].UnTrusted;
    Element[Element_Level].IsComplete=Element[Element_Level-1].IsComplete;
    Element[Element_Level].InLoop=Element[Element_Level-1].InLoop;

    //ToShow
    Element[Element_Level].ToShow.Pos=File_Offset+Buffer_Offset+Element_Pos+BS->Offset_Get(); //TODO: change this, used in Element_End()
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Pos);
        Element[Element_Level].ToShow.HeaderSize=0;
        Element[Element_Level].ToShow.Name.clear();
        Element[Element_Level].ToShow.Info.clear();
        Element[Element_Level].ToShow.Details.clear();
        Element[Element_Level].ToShow.NoShow=false;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Begin(const Ztring &Name, int64u Size)
{
    //Level
    Element_Level++;
    HeaderSize=0;

    //Element
    Element[Element_Level].Code=0;
    if (Size==(int64u)-1)
        Element[Element_Level].Next=Element[Element_Level-1].Next;
    else
    {
        Element[Element_Level].Next=File_Offset+Buffer_Offset+Element_Pos+Size;
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }
    Element[Element_Level].WaitForMoreData=false;
    Element[Element_Level].UnTrusted=Element[Element_Level-1].UnTrusted;
    Element[Element_Level].IsComplete=Element[Element_Level-1].IsComplete;
    Element[Element_Level].InLoop=false;

    //ToShow
    Element[Element_Level].ToShow.Pos=File_Offset+Buffer_Offset+Element_Pos+BS->Offset_Get(); //TODO: change this, used in Element_End()
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-(File_Offset+Buffer_Offset+Element_Pos);
        Element[Element_Level].ToShow.HeaderSize=0;
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
        Element[Element_Level].ToShow.Info.clear();
        Element[Element_Level].ToShow.Details.clear();
        Element[Element_Level].ToShow.NoShow=false;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Name(const Ztring &Name)
{
    //ToShow
    if (Config.Details_Get()!=0)
    {
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Element_Info(const Ztring &Parameter)
{
    //Coherancy
    if (Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (Config.Details_Get()<=0.7)
        return;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level-Element_Info_Level_].ToShow.Info+=Ztring(_T(" - "))+Parameter;
    }
}

//---------------------------------------------------------------------------
void File__Analyze::Element_End(const Ztring &Name, int64u Size)
{
    //Element
    if (Size!=(int64u)-1)
    {
        Element[Element_Level].Next=Element[Element_Level].ToShow.Pos+Size; //TODO: change this
        if (Element[Element_Level].Next>Element[Element_Level-1].Next)
            Element[Element_Level].Next=Element[Element_Level-1].Next;
    }

    //ToShow
    if (Config.Details_Get()!=0)
    {
        Element[Element_Level].ToShow.Size=Element[Element_Level].Next-Element[Element_Level].ToShow.Pos;
        if (!Name.empty())
            Element[Element_Level].ToShow.Name=Name;
    }

    Element_End_Common_Flush();
}

//---------------------------------------------------------------------------
void File__Analyze::Element_End()
{
    Element_End_Common_Flush();
}


//***************************************************************************
// Element - Common
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Element_End_Common_Flush()
{
    //Level
    if (Element_Level==0)
        return;

    //Element level
    Element_Level--;

    //Element
    Element[Element_Level].UnTrusted=Element[Element_Level+1].UnTrusted;
    Element[Element_Level].WaitForMoreData=Element[Element_Level+1].WaitForMoreData;
    Element[Element_Level].ToShow.NoShow=Element[Element_Level+1].ToShow.NoShow;

    //ToShow
    if (Config.Details_Get()!=0)
    {
        if (Element[Element_Level+1].IsComplete || !Element[Element_Level+1].UnTrusted)// && !Element[Element_Level].ToShow.NoShow && Config.Details_Get()!=0 && Element[Element_Level].ToShow.Details.size()<=64*1024*1024)
        {
            //Element
            if (!Element[Element_Level].ToShow.Details.empty())
                Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
            Element[Element_Level].ToShow.Details+=Element_End_Common_Flush_Build();

            //Info
            if (!Element[Element_Level+1].ToShow.Details.empty())
            {
                if (!Element[Element_Level].ToShow.Details.empty())
                    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
                Element[Element_Level].ToShow.Details+=Element[Element_Level+1].ToShow.Details;
                Element[Element_Level+1].ToShow.Details.clear();
            }
        }
    }
}

//---------------------------------------------------------------------------
Ztring File__Analyze::Element_End_Common_Flush_Build()
{
    Ztring ToReturn;

    //Show Offset
    if (Config.Details_Get()>0.7)
    {
        ToReturn+=Log_Offset(Element[Element_Level+1].ToShow.Pos);
    }

    //Name
    ToReturn.resize(ToReturn.size()+Element_Level_Base+Element_Level, _T(' '));
    if (Element[Element_Level+1].ToShow.Name.empty())
        Element[Element_Level+1].ToShow.Name=_T("Unknown");
    ToReturn+=Element[Element_Level+1].ToShow.Name;

    //Info
    ToReturn+=Element[Element_Level+1].ToShow.Info;
    Element[Element_Level+1].ToShow.Info.clear();

    //Size
    if (Config.Details_Get()>0.3)
    {
        ToReturn+=_T(" (");
        ToReturn+=Ztring::ToZtring(Element[Element_Level+1].ToShow.Size);
        if (Element[Element_Level+1].ToShow.HeaderSize>0)
        {
            ToReturn+=_T("/");
            ToReturn+=Ztring::ToZtring(Element[Element_Level+1].ToShow.Size-Element[Element_Level+1].ToShow.HeaderSize);
        }
        ToReturn+=_T(" bytes)");
    }

    return ToReturn;
}

//***************************************************************************
// Param
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Param(int64u Pos, const Ztring& Parameter, const Ztring& Value)
{
    if (Config.Details_Get()==0)
        return;

    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;

    const size_t Padding_Value=40;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

    //Show Offset
    if (Config.Details_Get()>0.7)
    {
        Element[Element_Level].ToShow.Details+=Log_Offset(Pos==(int64u)-1?Pos:(File_Offset+Buffer_Offset+Pos));
    }

    //Show Parameter
    Ztring Param; Param=Parameter;
    if (Param.size()>Padding_Value) Param.resize(Padding_Value);
    Element[Element_Level].ToShow.Details.resize(Element[Element_Level].ToShow.Details.size()+Element_Level_Base+Element_Level, _T(' '));
    Element[Element_Level].ToShow.Details+=Param;

    //Show Value
    if (!Value.empty())
    {
        Element[Element_Level].ToShow.Details+=_T(": ");
        Element[Element_Level].ToShow.Details.resize(Element[Element_Level].ToShow.Details.size()+Padding_Value-Param.size()-Element_Level+1, _T(' '));
        Element[Element_Level].ToShow.Details+=Value;
    }
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Info(const Ztring& Value)
{
    if (Config.Details_Get()==0)
        return;

    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;

    //Line separator
    if (!Element[Element_Level].ToShow.Details.empty())
        Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();

    //Preparing
    Ztring ToShow=_T("---   ");
    ToShow+=Value;
    ToShow+=_T("   ---");
    Ztring Separator; Separator.resize(ToShow.size(), _T('-'));

    //Show Offset
    Ztring Offset;
    if (Config.Details_Get()>0.7)
        Offset=Log_Offset(File_Offset+Buffer_Offset);

    //Show Value
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=Separator;
    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=ToShow;
    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
    Element[Element_Level].ToShow.Details+=Offset;
    Element[Element_Level].ToShow.Details+=Separator;
    Element[Element_Level].ToShow.Details+=Config.LineSeparator_Get();
}

//---------------------------------------------------------------------------
void File__Analyze::Param_Info (const Ztring &Text)
{
    //Coherancy
    if (Element[Element_Level].UnTrusted)
        return;
    if (Config.Details_Get()==0 || Element[Element_Level].ToShow.Details.size()>64*1024*1024)
        return;

    //Needed?
    if (Config.Details_Get()<=0.7)
        return;

    //Filling
    Element[Element_Level].ToShow.Details+=_T(" - ")+Text;
}

//***************************************************************************
// Big Endian
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_B1(int8u  &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Info=BigEndian2int8u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=1;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B2(int16u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Info=BigEndian2int16u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=2;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B3(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Info=BigEndian2int24u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B4(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=BigEndian2int32u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B5(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(5);
    Info=BigEndian2int40u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=5;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B6(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(6);
    Info=BigEndian2int48u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=6;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B7(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(7);
    Info=BigEndian2int56u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=7;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B8(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=BigEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=8;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_B16(int128u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    //Info=BigEndian2int128u(Buffer+Buffer_Offset+Element_Pos);
    Info.hi=BigEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
    Info.lo=BigEndian2int64u(Buffer+Buffer_Offset+Element_Pos+8);
    Param(Element_Pos, Name, Info);
    Element_Pos+=16;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_BF4(float32 &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=BigEndian2float32(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_BF8(float64 &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=BigEndian2float64(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=8;
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B1(int8u  &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Info=BigEndian2int8u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B2(int16u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Info=BigEndian2int16u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B3(int32u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Info=BigEndian2int24u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B4(int32u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=BigEndian2int32u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B5(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(5);
    Info=BigEndian2int40u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B6(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(6);
    Info=BigEndian2int48u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B7(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(7);
    Info=BigEndian2int56u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_B8(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=BigEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B1(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(1);
    Param(Element_Pos, Name, BigEndian2int8u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=1;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B2(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(2);
    Param(Element_Pos, Name, BigEndian2int16u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=2;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B3(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(3);
    Param(Element_Pos, Name, BigEndian2int24u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B4(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(4);
    Param(Element_Pos, Name, BigEndian2int32u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B5(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(5);
    Param(Element_Pos, Name, BigEndian2int40u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=5;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B6(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(6);
    Param(Element_Pos, Name, BigEndian2int48u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=6;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B7(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(7);
    Param(Element_Pos, Name, BigEndian2int56u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=7;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_B8(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(8);
    Param(Element_Pos, Name, BigEndian2int64u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=8;
}

//***************************************************************************
// Little Endian
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_L1(int8u  &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Info=LittleEndian2int8u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=1;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L2(int16u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Info=LittleEndian2int16u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=2;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L3(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Info=LittleEndian2int24u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L4(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=LittleEndian2int32u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L5(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(5);
    Info=LittleEndian2int40u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=5;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L6(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(6);
    Info=LittleEndian2int48u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=6;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L7(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(7);
    Info=LittleEndian2int56u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=7;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L8(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=LittleEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=8;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_L16(int128u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(16);
    //Info=LittleEndian2int128u(Buffer+Buffer_Offset+Element_Pos);
    Info.hi=LittleEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
    Info.lo=LittleEndian2int64u(Buffer+Buffer_Offset+Element_Pos+8);
    Param(Element_Pos, Name, Info);
    Element_Pos+=16;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_LF4(float32 &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=LittleEndian2float32(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_LF8(float64 &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=LittleEndian2float64(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Info);
    Element_Pos+=8;
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L1(int8u  &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(1);
    Info=LittleEndian2int8u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L2(int16u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(2);
    Info=LittleEndian2int16u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L3(int32u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(3);
    Info=LittleEndian2int24u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L4(int32u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(4);
    Info=LittleEndian2int32u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L5(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(5);
    Info=LittleEndian2int40u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L6(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(6);
    Info=LittleEndian2int48u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L7(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(7);
    Info=LittleEndian2int56u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_L8(int64u &Info)
{
    INTEGRITY_SIZE_ATLEAST_INT(8);
    Info=LittleEndian2int64u(Buffer+Buffer_Offset+Element_Pos);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L1(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(1);
    Param(Element_Pos, Name, LittleEndian2int8u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=1;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L2(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(2);
    Param(Element_Pos, Name, LittleEndian2int16u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=2;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L3(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(3);
    Param(Element_Pos, Name, LittleEndian2int24u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L4(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(4);
    Param(Element_Pos, Name, LittleEndian2int32u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L5(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(5);
    Param(Element_Pos, Name, LittleEndian2int40u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=5;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L6(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(6);
    Param(Element_Pos, Name, LittleEndian2int48u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=6;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L7(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(7);
    Param(Element_Pos, Name, LittleEndian2int56u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=7;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_L8(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(8);
    Param(Element_Pos, Name, LittleEndian2int64u(Buffer+Buffer_Offset+Element_Pos));
    Element_Pos+=8;
}

//***************************************************************************
// EBML
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_EB(int64u &Info, const char* Name)
{
    //Element size
    INTEGRITY_SIZE_ATLEAST_INT(1);
    int32u Size=0;
    int32u Size_Mark=0;
    BS_Begin();
    while (Size_Mark==0 && BS->Remain() && Size<=8)
    {
        Size++;
        PEEK_BS(Size, Size_Mark);
    }

    //Integrity
    if (!BS->Remain() || Size>8)
    {
        if (Size>8)
        {
            //Element[Element_Level].IsComplete=true; //If it is in a header
            Trusted_IsNot("EBML integer parsing error");
        }
        Info=0;
        return;
    }
    BS_End();
    if (File_Offset+Buffer_Offset+Element_Pos>=Element[Element_Level].Next)
    {
        //Element[Element_Level].IsComplete=true; //If it is in a header
        Trusted_IsNot("Not enough place to have an EBML");
        Info=0;
        return; //Not enough space
    }
    INTEGRITY_SIZE_ATLEAST_INT(Size);

    //Element Name
    switch (Size)
    {
        case 1 : {
                    int8u Element_Name;
                    PEEK_B1 (Element_Name);
                    Info=Element_Name&0x7F; //Keep out first bit
                 }
                 break;
        case 2 : {
                    int16u Element_Name;
                    PEEK_B2(Element_Name);
                    Info=Element_Name&0x3FFF; //Keep out first bits
                 }
                 break;
        case 3 : {
                    int32u Element_Name;
                    PEEK_B3(Element_Name);
                    Info=Element_Name&0x1FFFFF; //Keep out first bits
                 }
                 break;
        case 4 : {
                    int32u Element_Name;
                    PEEK_B4(Element_Name);
                    Info=Element_Name&0x0FFFFFFF; //Keep out first bits
                 }
                 break;
        case 5 : {
                    int64u Element_Name;
                    PEEK_B5(Element_Name);
                    Info=Element_Name&0x07FFFFFFFFLL; //Keep out first bits
                 }
                 break;
        case 6 : {
                    int64u Element_Name;
                    PEEK_B6(Element_Name);
                    Info=Element_Name&0x03FFFFFFFFFFLL; //Keep out first bits
                 }
                 break;
        case 7 : {
                    int64u Element_Name;
                    PEEK_B7(Element_Name);
                    Info=Element_Name&0x01FFFFFFFFFFFFLL; //Keep out first bits
                 }
                 break;
        case 8 : {
                    int64u Element_Name;
                    PEEK_B8(Element_Name);
                    Info=Element_Name&0x00FFFFFFFFFFFFFFLL; //Keep out first bits
                 }
                 break;
    }

    Param(Element_Pos, Name, Info);
    Element_Pos+=Size;
}

//***************************************************************************
// Characters
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_C3(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(3);
    Info=CC3(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 3);
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_C3(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(3);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 3);
    Element_Pos+=3;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_C4(int32u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(4);
    Info=CC4(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 4);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_C4(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(4);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 4);
    Element_Pos+=4;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_C6(int64u &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(6);
    Info=CC6(Buffer+Buffer_Offset+Element_Pos);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 6);
    Element_Pos+=6;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_C6(const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(6);
    Param(Element_Pos, Name, Buffer+Buffer_Offset+Element_Pos, 6);
    Element_Pos+=6;
}

//***************************************************************************
// Text
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_Local(int64u Bytes, Ztring &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_STRING(Bytes);
    Info.From_Local((const char*)(Buffer+Buffer_Offset+Element_Pos), Bytes);
    Param(Element_Pos, Name, Info);
    Element_Pos+=Bytes;
}

//---------------------------------------------------------------------------
void File__Analyze::Get_UTF8(int64u Bytes, Ztring &Info, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST_STRING(Bytes);
    Info.From_UTF8((const char*)(Buffer+Buffer_Offset+Element_Pos), Bytes);
    Param(Element_Pos, Name, Info);
    Element_Pos+=Bytes;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_Local(int64u Bytes, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(Bytes);
    Param(Element_Pos, Name, Ztring().From_Local((const char*)(Buffer+Buffer_Offset+Element_Pos), Bytes));
    Element_Pos+=Bytes;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_UTF8(int64u Bytes, const char* Name)
{
    INTEGRITY_SIZE_ATLEAST(Bytes);
    Param(Element_Pos, Name, Ztring().From_UTF8((const char*)(Buffer+Buffer_Offset+Element_Pos), Bytes));
    Element_Pos+=Bytes;
}

//***************************************************************************
// Unknown
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Skip_XX(int64u Bytes, const char* Name)
{
    Param(Element_Pos, Name, Ztring("(")+Ztring::ToZtring(Bytes)+Ztring(" bytes)"));
    Element_Pos+=Bytes;
}

//***************************************************************************
// Flags
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_Flags (int64u Flags, size_t Order, bool &Info, const char* Name)
{
    if (Flags&(1<<Order))
        Info=true;
    else
        Info=false;

    //Element_Level++;
    if (Info)
        Param(Element_Pos+BS->Offset_Get(), Name, "Yes");
    else
        Param(Element_Pos+BS->Offset_Get(), Name, "No");
    //Element_Level--;
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_Flags(int64u Flags, size_t Order, const char* Name)
{
    //Element_Level++;
    if (Flags&(1<<Order))
        Param(Element_Pos+BS->Offset_Get(), Name, "Yes");
    else
        Param(Element_Pos+BS->Offset_Get(), Name, "No");
    //Element_Level--;
}

//***************************************************************************
// BitStream
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Get_BS(size_t Bits, int32u &Info, const char* Name)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Get(Bits);
    Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Get_SB(             bool &Info, const char* Name)
{
    INTEGRITY_INT(1<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->GetB();
    Param(Element_Pos+BS->Offset_Get()-1/8-(1%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Get_S1(size_t Bits, int8u &Info, const char* Name)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Get1(Bits);
    Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Get_S2(size_t Bits, int16u &Info, const char* Name)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Get2(Bits);
    Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Get_S4(size_t Bits, int32u &Info, const char* Name)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Get4(Bits);
    Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Get_S8(size_t Bits, int64u &Info, const char* Name)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Get8(Bits);
    Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, Info);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_BS(size_t Bits, int32u &Info)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Peek(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_SB(              bool &Info)
{
    INTEGRITY_INT(1<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->PeekB();
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_S1(size_t Bits, int8u &Info)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Peek1(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_S2(size_t Bits, int16u &Info)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Peek2(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Peek_S4(size_t Bits, int32u &Info)
{
    INTEGRITY_INT(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    Info=BS->Peek4(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_BS(size_t Bits, const char* Name)
{
    INTEGRITY(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, BS->Get(Bits));
    else
        BS->Skip(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_SB(              const char* Name)
{
    INTEGRITY(1<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-1/8-(1%8==0?0:1), Name, BS->GetB());
    else
        BS->SkipB();
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_S1(size_t Bits, const char* Name)
{
    INTEGRITY(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, BS->Get1(Bits));
    else
        BS->Skip1(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_S2(size_t Bits, const char* Name)
{
    INTEGRITY(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, BS->Get2(Bits));
    else
        BS->Skip2(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_S4(size_t Bits, const char* Name)
{
    INTEGRITY(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, BS->Get4(Bits));
    else
        BS->Skip4(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Skip_S8(size_t Bits, const char* Name)
{
    INTEGRITY(Bits<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    if (Config.Details_Get())
        Param(Element_Pos+BS->Offset_Get()-Bits/8-(Bits%8==0?0:1), Name, BS->Get8(Bits));
    else
        BS->Skip8(Bits);
}

//---------------------------------------------------------------------------
void File__Analyze::Mark_0(const char* Name)
{
    INTEGRITY(1<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    bool Info=BS->GetB();
    Param(Element_Pos+BS->Offset_Get()-1/8-(1%8==0?0:1), Name, Info);
    INTEGRITY_INT(Info==0, "Mark bit is wrong", BS->Offset_Get())
}

//---------------------------------------------------------------------------
void File__Analyze::Mark_1(const char* Name)
{
    INTEGRITY(1<=BS->Remain(), "Size is wrong", BS->Offset_Get())
    bool Info=BS->GetB();
    Param(Element_Pos+BS->Offset_Get()-1/8-(1%8==0?0:1), Name, Info);
    INTEGRITY_INT(Info==1, "Mark bit is wrong", BS->Offset_Get())
}

//***************************************************************************
// Element trusting
//***************************************************************************

//---------------------------------------------------------------------------
void File__Analyze::Trusted_IsNot (const char* Reason, int64u Size)
{
    if (!Element[Element_Level].UnTrusted)
    {
        Param(Element_Pos, Reason, 0);

        //Enough data?
        if (!Element[Element_Level].IsComplete)
        {
            Element_Size=0;
            Element_Pos=0;
            Element[Element_Level].WaitForMoreData=true;
            return;
        }

        Element[Element_Level].UnTrusted=true;
        Trusted--;
    }

    if (Trusted==0)
    {
        Clear();
        File_Offset=File_Size;
    }
}

//***************************************************************************
// Filling
//***************************************************************************

//---------------------------------------------------------------------------
bool File__Analyze::Element_IsOK ()
{
    return !Element[Element_Level].WaitForMoreData && !Element[Element_Level].UnTrusted;
}

//---------------------------------------------------------------------------
bool File__Analyze::Element_WaitForMoreData ()
{
    return Element[Element_Level].WaitForMoreData;
}

} //NameSpace


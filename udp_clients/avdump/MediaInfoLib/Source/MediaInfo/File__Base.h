// File__Base - Base for other files
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
// Give common methods for all file types
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo__BaseH
#define MediaInfo__BaseH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/MediaInfo_Config.h"
#include <ZenLib/ZtringListList.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Base
//***************************************************************************

class File__Base
{
public :
    //Constructor/Destructor
    File__Base();
    virtual ~File__Base();

    //Analyze
    int     Open_File (const Ztring &File_Name); //1 if succeed, 0 if problem with format
    void    Open_Buffer_Init (int64u File_Size=0, int64u File_Offset=0);
    void    Open_Buffer_Init (File__Base* Sub);
    void    Open_Buffer_Init (File__Base* Sub, int64u File_Size, int64u File_Offset=0);
    void    Open_Buffer_Continue (const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Continue (File__Base* Sub, const int8u* Buffer, size_t Buffer_Size);
    void    Open_Buffer_Finalize ();
    void    Open_Buffer_Finalize (File__Base* Sub);
    int     Save ();

    //Get information
    Ztring  Inform ();
    Ztring  Inform (stream_t StreamKind, size_t StreamNumber=0); //All about only a specific stream
    const Ztring &Get (stream_t StreamKind, size_t StreamNumber, size_t Parameter, info_t KindOfInfo=Info_Text);
    const Ztring &Get (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, info_t KindOfInfo=Info_Text, info_t KindOfSearch=Info_Name);

    //Set information
    int           Set (stream_t StreamKind, size_t StreamNumber, size_t Parameter, const Ztring &ToSet, const Ztring &OldValue=_T(""));
    int           Set (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, const Ztring &ToSet, const Ztring &OldValue=_T(""));

    //Output buffer
    size_t        Output_Buffer_Get (const Ztring &Value, char** Output_Buffer=NULL);

    //Options
    size_t Count_Get (stream_t StreamKind, size_t Pos=Error) const;
    void   Language_Set (); //Update language for an existing File__Base

    //Infos
           ZtringListList Info_Capacities ();
    static ZtringListList Info_Capacities_Parameters ();

    //Before filling the stream, the stream must be prepared
    size_t Stream_Prepare   (stream_t KindOfStream);
    void   General_Fill     (); //Special : pre-fill General with some important information

    //Fill with datas
    void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value.c_str(), Value.size())); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value.c_str(), Value.size()), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char*    Value, size_t Value_Size=Unlimited, bool Utf8=true, bool Replace=false) {if (Utf8) Fill(StreamKind, StreamPos, Parameter, Ztring().From_UTF8(Value, Value_Size)); else Fill(StreamKind, StreamPos, Parameter, Ztring().From_Local(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, const wchar_t* Value, size_t Value_Size=Unlimited, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring().From_Unicode(Value, Value_Size), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8u          Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int8s          Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int16s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int32s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, int64s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, float32        Value, intu AfterComma=3, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, AfterComma), Replace);}
    #ifdef NEED_SIZET
    inline void Fill (stream_t StreamKind, size_t StreamPos, const char* Parameter, size_t         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind, StreamPos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase(), Replace);}
    #endif //NEED_SIZET
    inline void Fill (const char* Parameter, const Ztring  &Value, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Replace);} //With the last set
    inline void Fill (const char* Parameter, const std::string &Value, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const char*    Value, size_t ValueSize=Unlimited, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, ValueSize, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const int8u*   Value, size_t ValueSize=Unlimited, bool Utf8=true, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, (const char*)Value, ValueSize, Utf8, Replace);} //With the last set
    inline void Fill (const char* Parameter, const wchar_t* Value, size_t ValueSize=Unlimited, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, ValueSize, Replace);} //With the last set
    inline void Fill (const char* Parameter, int8u          Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int8s          Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int16u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int16s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int32u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int32s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int64u         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, int64s         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    inline void Fill (const char* Parameter, float32        Value, intu AfterComma=3, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, AfterComma, Replace);} //With the last set
    inline void Fill (const char* Parameter, double         Value, intu AfterComma=3, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, (float32)Value, AfterComma, Replace);} //With the last set
    #ifdef NEED_SIZET
    inline void Fill (const char* Parameter, size_t         Value, intu Radix=10, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Radix, Replace);} //With the last set
    #endif //NEED_SIZET
    void Fill (stream_t StreamKind, size_t StreamPos, const int32u Parameter, const Ztring  &Value, bool Replace=false);
    inline void Fill (int32u Parameter, const Ztring  &Value, bool Replace=false) {Fill(StreamKind_Last, StreamPos_Last, Parameter, Value, Replace);} //With the last set
    void Fill_HowTo (stream_t StreamKind, size_t StreamPos, const char* Parameter, const char* Value);
    inline void Fill_HowTo (const char* Parameter, const char* Value) {Fill_HowTo(StreamKind_Last, StreamPos_Last, Parameter, Value);} //With the last set
    ZtringListList Fill_Temp;
    void Fill_Flush ();

protected :
    //Read
    virtual void Read_File            (); //To overload
    virtual void Read_Buffer_Init     (); //To overload
    virtual void Read_Buffer_Unsynched(); //To overload
    virtual void Read_Buffer_Continue (); //To overload
    virtual void Read_Buffer_Finalize (); //To overload

    //Write
    virtual int Write       (stream_t StreamKind, size_t StreamNumber, const Ztring &Parameter, const Ztring &ToSet, const Ztring &OldValue); //Write the value in memory
    virtual int WriteToDisk (); //Write modified tags on disk

    //Output buffer
    void          Fill_Output_Buffer (int16u Value, const int8u* Output_Buffer, size_t Output_Buffer_Size);

    //How to
    virtual void HowTo (stream_t StreamType);

    //Arrays
    std::vector<ZtringListList> General;
    std::vector<ZtringListList> Video;
    std::vector<ZtringListList> Audio;
    std::vector<ZtringListList> Text;
    std::vector<ZtringListList> Chapters;
    std::vector<ZtringListList> Image;
    std::vector<ZtringListList> Menu;
    std::vector<ZtringListList>* Stream[Stream_Max]; //pointer to others listed streams

    //Debug
    #ifndef MEDIAINFO_MINIMIZESIZE
    Ztring Details;
    void Details_Add_Element      (int8u Level, const Ztring &Text, int64u Size=(int64u)(-1)) {Details_Add_Element_Generic(Level, Text, Size);} //Element
    void Details_Add_Element      (int8u Level, const char*   Text, int64u Size=(int64u)(-1)) {Details_Add_Element(Level, Ztring().From_UTF8(Text), Size);}
    void Details_Add_Element      (int8u Level, int8u         Text, int64u Size=(int64u)(-1));
    void Details_Add_Element      (int8u Level, int16u        Text, int64u Size=(int64u)(-1));
    void Details_Add_Element      (int8u Level, int32u        Text, int64u Size=(int64u)(-1));
    void Details_Add_Element      (int8u Level, int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)(-1));
    void Details_Add_Element      (             const Ztring &Text                   ); //More text about element
    void Details_Add_Element      (             const char*   Text                   ) {Details_Add_Element(Ztring().From_UTF8(Text));}
    void Details_Add_Element_Flush();
    void Details_Add_Element_Purge();
    void Details_Add_Info (size_t Pos, const Ztring &Parameter, const Ztring& Value) {Details_Add_Info(Pos, Parameter.To_Local().c_str(), Value);};
    void Details_Add_Info (size_t Pos, const char*   Parameter, const Ztring& Value);
    void Details_Add_Info (size_t Pos, const char*   Parameter, const std::string& Value) {Details_Add_Info(Pos, Parameter, Ztring().From_Local(Value.c_str()));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const char* Value, size_t Value_Size=Unlimited, bool Utf8=true) {if (Utf8) Details_Add_Info(Pos, Parameter, Ztring().From_UTF8(Value, Value_Size)); else Details_Add_Info(Pos, Parameter, Ztring().From_Local(Value, Value_Size));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const int8u* Value, size_t Value_Size=Unlimited, bool Utf8=true) {Details_Add_Info(Pos, Parameter, (const char*)Value, Value_Size, Utf8);}
    void Details_Add_Info (size_t Pos, const char*   Parameter, bool   Value) {if (Value) Details_Add_Info(Pos, Parameter, "Yes"); else Details_Add_Info(Pos, Parameter, "No");}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int8u  Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int8s  Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int16u Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int16s Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int32u Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int32s Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int64u Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int64s Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    void Details_Add_Info (size_t Pos, const char*   Parameter, float32 Value, intu AfterComma=3) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, AfterComma));}
    #ifdef NEED_SIZET
    void Details_Add_Info (size_t Pos, const char*   Parameter, size_t Value, intu Radix=16) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    #endif //NEED_SIZET
    void Details_Add_Info (size_t Pos, const char*   Parameter, double  Value, intu AfterComma=3) {Details_Add_Info(Pos, Parameter, Ztring::ToZtring(Value, AfterComma));}
    void Details_Add_Info (size_t Pos, const int32u  Parameter, const Ztring& Value) {Details_Add_Info(Pos, Ztring().From_CC4(Parameter), Value);};
    void Details_Add_Info (const Ztring &Parameter);
    void Details_Add_Info (const char*   Parameter) {Details_Add_Info(Ztring().From_UTF8(Parameter));}
    void Details_Add_Info (const char*   Parameter, const char*   Measure)      {Details_Add_Info(Ztring().From_UTF8(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int64u        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int64s        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int32u        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int32s        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int16u        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int16s        Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int8u         Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (int8s         Parameter, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Details_Add_Info (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {Details_Add_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    void Details_Add_Error(const char*   Parameter, int8u Element_Level=(int8u)(-1), int64u Size=(int64u)(-1));
    void Details_Remove_Info ();
    void Details_Add_Element_Generic (int8u Level, const Ztring &Text, int64u Size);
    void Details_Add_Info_Generic (size_t Pos, const Ztring &Parameter, const Ztring& Value);
    void Demux (const int8u* Buffer, size_t Buffer_Size, const Ztring& StreamName, bool Final=true);
    int8u Details_Level_Base; //Element level
    int8u Details_Level_Last; //Element level
    float32 Details_Level_Min; //Minimum value for showing details
    Ztring Details_Element_Temp;
    Ztring Details_Info_Temp;

    //Some quick helpers
public :
    inline void DETAILLEVEL_SET (float32 Value)     {Details_Level_Min=Value;}
    inline void ELEMENT_BEGIN () {Details_Level_Last++;}
    inline void ELEMENT_END () {Details_Level_Last--;}
    inline void ELEMENT (             const Ztring &Text, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text, Size);}
    inline void ELEMENT (             const char*   Text, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text, Size);}
    inline void ELEMENT (             int8u         Text, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text, Size);}
    inline void ELEMENT (             int16u        Text, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text, Size);}
    inline void ELEMENT (             int32u        Text, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text, Size);}
    inline void ELEMENT (             int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)-1) {ELEMENT(Details_Level_Last, Text1, Text2, Text3, Text4, Size);}
    inline void ELEMENT (int8u Level, const Ztring &Text, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text, Size);}
    inline void ELEMENT (int8u Level, const char*   Text, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text, Size);}
    inline void ELEMENT (int8u Level, int8u         Text, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text, Size);}
    inline void ELEMENT (int8u Level, int16u        Text, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text, Size);}
    inline void ELEMENT (int8u Level, int32u        Text, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text, Size);}
    inline void ELEMENT (int8u Level, int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)-1) {DETAILLEVEL_SET(0.0); Details_Add_Element(Level, Text1, Text2, Text3, Text4, Size);}
    inline void NAME (const char* Parameter)        {if (Config.Details_Get()>=Details_Level_Min) {Details_Add_Element(Parameter); FLUSH();}}
    inline void INFO (const Ztring &Parameter)      {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter);}
    inline void INFO (const char*   Parameter)      {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter);}
    inline void INFO (const char*   Parameter, const char*   Measure)      {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int64u        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int64s        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int32u        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int32s        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int16u        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int16s        Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int8u         Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (int8s         Parameter, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, Measure);}
    inline void INFO (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Info(Parameter, AfterComma, Measure);}
    inline void NOT_NEEDED ()                       {if (Config.Details_Get()>=Details_Level_Min) PURGE();} //Details_Add_Element("(Not needed)");}
    inline void FLUSH ()                            {if (Config.Details_Get()) Details_Add_Element_Flush();}
    inline void PURGE ()                            {if (Config.Details_Get()) Details_Add_Element_Purge();}
    inline void TRUSTED_ISNOT (const char* Reason, int8u Element_Level=(int8u)(-1), int64u Size=(int64u)(-1)) {if (Config.Details_Get()>=Details_Level_Min) Details_Add_Error(Reason, Element_Level, Size);
                                                     Trusted--; if (Trusted==0) {Clear(); File_Offset=File_Size; }};
    inline void DEMUX (const int8u* Buffer, size_t Buffer_Size, const Ztring& StreamName, bool Final=true) {if (Config.Demux_Get()) Demux(Buffer, Buffer_Size, StreamName, Final);}
    #else //MEDIAINFO_MINIMIZESIZE
    void Details_Add_Element      (int8u Level, const Ztring &Text, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (int8u Level, const char*   Text, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (int8u Level, int8u         Text, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (int8u Level, int16u        Text, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (int8u Level, int32u        Text, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (int8u Level, int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)(-1)) {}
    void Details_Add_Element      (             const Ztring &Text                   ) {}
    void Details_Add_Element      (             const char*   Text                   ) {}
    void Details_Add_Element_Flush() {}
    void Details_Add_Element_Purge() {}
    void Details_Add_Info (size_t Pos, const Ztring &Parameter, const Ztring& Value) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const Ztring& Value) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const std::string& Value) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const char* Value, size_t Value_Size=Unlimited, bool Utf8=true) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, const int8u* Value, size_t Value_Size=Unlimited, bool Utf8=true) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, bool   Value) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int8u  Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int8s  Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int16u Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int16s Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int32u Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int32s Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int64u Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, int64s Value, intu Radix=16) {}
    void Details_Add_Info (size_t Pos, const char*   Parameter, float32 Value, intu AfterComma=3) {}
    #ifdef NEED_SIZET
    void Details_Add_Info (size_t Pos, const char*   Parameter, size_t Value, intu Radix=16) {}
    #endif //NEED_SIZET
    void Details_Add_Info (size_t Pos, const char*   Parameter, double  Value, intu AfterComma=3) {}
    void Details_Add_Info (size_t Pos, const int32u  Parameter, const Ztring& Value) {}
    void Details_Add_Info (const Ztring &Parameter) {}
    void Details_Add_Info (const char*   Parameter) {}
    void Details_Add_Info (const char*   Parameter, const char*   Measure)      {}
    void Details_Add_Info (int64u        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int64s        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int32u        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int32s        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int16u        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int16s        Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int8u         Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (int8s         Parameter, const char*   Measure=NULL) {}
    void Details_Add_Info (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {}
    void Details_Add_Error(const char*   Parameter, int8u Element_Level=(int8u)(-1), int64u Size=(int64u)(-1)) {}
    void Details_Remove_Info () {}
    void Details_Add_Element_Generic (int8u Level, const Ztring &Text, int64u Size) {}
    void Details_Add_Info_Generic (size_t Pos, const Ztring &Parameter, const Ztring& Value) {}

    //Some quick helpers
public :
    inline void DETAILLEVEL_SET (float32 Value)     {}
    inline void ELEMENT_BEGIN () {}
    inline void ELEMENT_END () {}
    inline void ELEMENT (             const Ztring &Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (             const char*   Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (             int8u         Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (             int16u        Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (             int32u        Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (             int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, const Ztring &Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, const char*   Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, int8u         Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, int16u        Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, int32u        Text, int64u Size=(int64u)-1) {}
    inline void ELEMENT (int8u Level, int32u        Text1, int32u Text2, int32u Text3, int32u Text4, int64u Size=(int64u)-1) {}
    inline void NAME (const char* Parameter)        {}
    inline void INFO (const Ztring &Parameter)      {}
    inline void INFO (const char*   Parameter)      {}
    inline void INFO (const char*   Parameter, const char*   Measure)      {}
    inline void INFO (int64u        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int64s        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int32u        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int32s        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int16u        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int16s        Parameter, const char*   Measure=NULL) {}
    inline void INFO (int8u         Parameter, const char*   Measure=NULL) {}
    inline void INFO (int8s         Parameter, const char*   Measure=NULL) {}
    inline void INFO (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {}
    inline void NOT_NEEDED ()                       {}
    inline void FLUSH ()                            {}
    inline void PURGE ()                            {}
    inline void TRUSTED_ISNOT (const char* Reason, int8u Element_Level=(int8u)(-1), int64u Size=(int64u)(-1)) {}
    #endif //MEDIAINFO_MINIMIZESIZE

    //Debug
    bool   Synched;
    size_t Trusted;

protected :
    std::vector<bool> Optimized[Stream_Max]; //If true, Arrays are not fully filled
    stream_t StreamKind_Last;
    size_t   StreamPos_Last;

    //Utils
public :
    size_t Merge(const File__Base &ToAdd); //Merge 2 File_Base
    size_t Merge(File__Base &ToAdd, stream_t StreamKind, size_t StreamPos_From, size_t StreamPos_To); //Merge 2 streams
protected :

    //Utils - Finalize
    void PlayTime_PlayTime123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void FileSize_FileSize123   (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Kilo_Kilo123           (const Ztring &Value, stream_t StreamKind, size_t StreamPos);
    void Value_Value123         (const Ztring &Value, stream_t StreamKind, size_t StreamPos);

    //Divers
    void Clear();

public :
    //Buffer
    const int8u* Buffer;
    int8u* Buffer_Temp;
    size_t Buffer_Size;
    size_t Buffer_Size_Max;
    size_t Buffer_Offset; //Temporary usage in this parser
    size_t Buffer_Offset_Temp; //Temporary usage in this parser
    size_t Buffer_MinimumSize;
    size_t Buffer_MaximumSize;
    bool   Buffer_Init_Done;
    void   Buffer_Clear();
    void   Details_Clear();
    void   Details_Add(const char* Parameter);

    //Output Buffer
    struct output
    {
        int8u* Buffer;
        size_t Buffer_Size;
        size_t Buffer_Size_Max;
        size_t Buffer_MaximumSize;
        output()
        {
            Buffer=NULL;
            Buffer_Size=0;
            Buffer_Size_Max=0;
            Buffer_MaximumSize=1024*1024;
        };
    };
    std::map<Ztring, output >   Output;
    std::map<int16u, output*>   Output_16;

    //File
    Ztring File_Name;
    int64u File_Size;
    int64u File_Offset;
    int64u File_GoTo; //How many byte to skip?
    int64u File_MaximumOffset;

    //End
    void Finalize();
    void Finalize__All      (stream_t StreamKind);
    void Finalize__All      (stream_t StreamKind, size_t Pos, Ztring &Codec_List, Ztring &Language_List);
    void Finalize_General   (size_t Pos);
    void Finalize_Video     (size_t Pos);
    void Finalize_Audio     (size_t Pos);
    void Finalize_Text      (size_t Pos);
    void Finalize_Chapters  (size_t Pos);
    void Finalize_Image     (size_t Pos);
    void Finalize_Menu      (size_t Pos);
    void Finalize_Tags      ();

public :  //A virer
    void Traiter(Ztring &C); //enleve les $if...
    friend class File__Analyze;
};

//***************************************************************************
// Some helpers for having more readable code
//***************************************************************************

#ifndef MediaInfo_File__AnalyzeH

//Finnished
#define FINNISHED() \
    { \
        File__Base::File_Offset=File_Size; \
        return; \
    } \

#define FINNISHED_BOOL() \
    { \
        File__Base::File_Offset=File_Size; \
        return false; \
    } \

//Element name
#ifndef MEDIAINFO_MINIMIZESIZE
    #define NAME(ELEMENT_NAME) \
        { \
            Details_Add_Element(ELEMENT_NAME); \
        } \

#else //MEDIAINFO_MINIMIZESIZE
    #define NAME(ELEMENT_NAME) \

#endif //MEDIAINFO_MINIMIZESIZE

//define the return value if there is a problem
#define INTEGRITY_RETURNVALUE

//Integrity test
#define INTEGRITY(TOVALIDATE, ERRORTEXT, OFFSET) \
    if (!(TOVALIDATE)) \
    { \
        Buffer_Offset+=OFFSET; \
        TRUSTED_ISNOT(ERRORTEXT); \
        return INTEGRITY_RETURNVALUE; \
    } \

//A starting chunk is not yet found
#define INTEGRITY_GENERAL(OFFSET) \
    if (Count_Get(Stream_General)==0) \
    { \
        Buffer_Offset+=OFFSET; \
        Details_Add_Error("Waiting for starting element"); \
        return INTEGRITY_RETURNVALUE; \
    } \

//Exact element size
#define INTEGRITY_SIZE(ELEMENT_SIZE, OFFSET) \
    if (Element_Size!=ELEMENT_SIZE) \
    { \
        Buffer_Offset+=OFFSET; \
        TRUSTED_ISNOT("Size is wrong"); \
        return INTEGRITY_RETURNVALUE; \
    } \

//At least this size
#define INTEGRITY_SIZE_ATLEAST(ELEMENT_SIZE, OFFSET) \
    if (Element_Size<ELEMENT_SIZE) \
    { \
        Buffer_Offset+=OFFSET; \
        TRUSTED_ISNOT("Size is wrong"); \
        return INTEGRITY_RETURNVALUE; \
    } \

//Element is no more needed
#define NOT_NEEDED(_TEST) \
    if (_TEST) \
    { \
        NOT_NEEDED(); \
        return INTEGRITY_RETURNVALUE; \
    } \

//Begin of a stream
#define CC_BEGIN() \
    int32u Stream_Pos=0; \

#define BS_BEGIN() \
    BitStream BS(Buffer+Buffer_Offset, Element_Size); \

#define CC_BS_BEGIN() \
    { \
        Buffer_Offset+=Stream_Pos; \
        BS_BEGIN(); \

//End of a stream
#define CC_END() \
    { \
    } \

#define CC_END_CANBEMORE() \
    { \
    } \

#define BS_END() \
    { \
        BS.Byte_Align(); \
        int32u BS_Value=0x00; \
        while(BS.Remain()>0 && BS_Value==0x00) \
            BS_Value=BS.Get(8); \
        if (BS_Value!=0x00) \
            INTEGRITY_SIZE(BS.Offset_Get()-1, BS.Offset_Get()) \
        else \
            INTEGRITY_SIZE(BS.Offset_Get(), BS.Offset_Get()) \
    } \

#define BS_END_FF() \
    { \
        BS.Byte_Align(); \
        int32u BS_Value=0xFF; \
        while(BS.Remain()>0 && BS_Value==0x00) \
            BS_Value=BS.Get(8); \
        if (BS_Value!=0xFF) \
            INTEGRITY_SIZE(BS.Offset_Get()-1, BS.Offset_Get()) \
        else \
            INTEGRITY_SIZE(BS.Offset_Get(), BS.Offset_Get()) \
    } \

#define BS_END_CANBEMORE() \
    { \
    } \

#define CC_BS_END() \
        Buffer_Offset-=Stream_Pos; \
        Stream_Pos+=BS.Offset_Get(); \
    } \

//Get
#define GET_B1(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        _INFO=BigEndian2int8u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=1; \
    } \

#define GET_B2(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        _INFO=BigEndian2int16u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=2; \
    } \

#define GET_B3(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        _INFO=BigEndian2int24u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=3; \
    } \

#define GET_B4(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        _INFO=BigEndian2int32u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=4; \
    } \

#define GET_B5(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        _INFO=BigEndian2int40u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=5; \
    } \

#define GET_B6(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        _INFO=BigEndian2int48u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=6; \
    } \

#define GET_B7(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        _INFO=BigEndian2int56u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=7; \
    } \

#define GET_B8(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        _INFO=BigEndian2int64u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=8; \
    } \

#define GET_L1(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        _INFO=LittleEndian2int8u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=1; \
    } \

#define GET_L2(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        _INFO=LittleEndian2int16u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=2; \
    } \

#define GET_L3(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        _INFO=LittleEndian2int24u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=3; \
    } \

#define GET_L4(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        _INFO=LittleEndian2int32u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=4; \
    } \

#define GET_L5(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        _INFO=LittleEndian2int40u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=5; \
    } \

#define GET_L6(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        _INFO=LittleEndian2int48u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=6; \
    } \

#define GET_L7(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        _INFO=LittleEndian2int56u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=7; \
    } \

#define GET_L8(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        _INFO=LittleEndian2int64u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=8; \
    } \

#define GET_C1(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        _INFO=CC1(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 1); \
        Stream_Pos+=1; \
    } \

#define GET_C2(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        _INFO=CC2(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 2); \
        Stream_Pos+=2; \
    } \

#define GET_C3(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        _INFO=CC3(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 3); \
        Stream_Pos+=3; \
    } \

#define GET_C4(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        _INFO=CC4(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 4); \
        Stream_Pos+=4; \
    } \

#define GET_C5(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        _INFO=CC5(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 5); \
        Stream_Pos+=5; \
    } \

#define GET_C6(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        _INFO=CC6(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 6); \
        Stream_Pos+=6; \
    } \

#define GET_C7(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        _INFO=CC7(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 7); \
        Stream_Pos+=7; \
    } \

#define GET_C8(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        _INFO=CC8(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 8); \
        Stream_Pos+=8; \
    } \

#define GET_GUID(_INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+16, Stream_Pos) \
        _INFO=LittleEndian2int32u(Buffer+Buffer_Offset+Stream_Pos); \
        Details_Add_Info(Stream_Pos, #_NAME, "GUID"); \
        Stream_Pos+=16; \
    } \

#define GET_STRING(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.assign((const char*)(Buffer+Buffer_Offset+Stream_Pos), 0, _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_LOCAL(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_Local((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_UTF8(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_UTF8((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_UTF16(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_UTF16((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_UTF16B(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_UTF16BE((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_UTF16L(_BYTES, _INFO, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_UTF16LE((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
        Details_Add_Info(Stream_Pos, #_NAME, _INFO); \
        Stream_Pos+=_BYTES; \
    } \

#define GET_BS(_BITS, _INFO, _NAME) \
    { \
        INTEGRITY(_BITS<=BS.Remain(), "Size is wrong", BS.Offset_Get()) \
        _INFO=BS.Get(_BITS); \
        Details_Add_Info(BS.Offset_Get()-_BITS/8-(_BITS%8==0?0:1), #_NAME, _INFO); \
    } \

//Skip
#define SKIP_XX(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring("(")+Ztring::ToZtring(_BYTES)+Ztring(" bytes)")); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_B1(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int8u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=1; \
    } \

#define SKIP_B2(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int16u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=2; \
    } \

#define SKIP_B3(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int24u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=3; \
    } \

#define SKIP_B4(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int32u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=4; \
    } \

#define SKIP_B5(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int40u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=5; \
    } \

#define SKIP_B6(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int48u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=6; \
    } \

#define SKIP_B7(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int56u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=7; \
    } \

#define SKIP_B8(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, BigEndian2int64u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=8; \
    } \

#define SKIP_L1(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int8u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=1; \
    } \

#define SKIP_L2(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int16u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=2; \
    } \

#define SKIP_L3(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int24u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=3; \
    } \

#define SKIP_L4(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int32u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=4; \
    } \

#define SKIP_L5(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int40u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=5; \
    } \

#define SKIP_L6(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int48u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=6; \
    } \

#define SKIP_L7(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int56u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=7; \
    } \

#define SKIP_L8(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, LittleEndian2int64u(Buffer+Buffer_Offset+Stream_Pos)); \
        Stream_Pos+=8; \
    } \

#define SKIP_C1(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 1); \
        Stream_Pos+=1; \
    } \

#define SKIP_C2(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 2); \
        Stream_Pos+=2; \
    } \

#define SKIP_C3(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 3); \
        Stream_Pos+=3; \
    } \

#define SKIP_C4(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 4); \
        Stream_Pos+=4; \
    } \

#define SKIP_C5(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 5); \
        Stream_Pos+=5; \
    } \

#define SKIP_C6(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 6); \
        Stream_Pos+=6; \
    } \

#define SKIP_C7(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 7); \
        Stream_Pos+=7; \
    } \

#define SKIP_C8(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Buffer+Buffer_Offset+Stream_Pos, 8); \
        Stream_Pos+=8; \
    } \

#define SKIP_GUID(_NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+16, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, "GUID"); \
        Stream_Pos+=16; \
    } \

#define SKIP_LOCAL(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring().From_Local((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES)); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_UTF8(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring().From_UTF8((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES)); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_UTF16(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring().From_UTF16((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES)); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_UTF16B(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring().From_UTF16BE((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES)); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_UTF16L(_BYTES, _NAME) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        Details_Add_Info(Stream_Pos, #_NAME, Ztring().From_UTF16LE((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES)); \
        Stream_Pos+=_BYTES; \
    } \

#define SKIP_BS(_BITS, _NAME) \
    { \
        INTEGRITY(_BITS<=BS.Remain(), "Size is wrong", BS.Offset_Get()) \
        if (Config.Details_Get()) \
            Details_Add_Info(BS.Offset_Get()-_BITS/8-(_BITS%8==0?0:1), #_NAME, BS.Get(_BITS)); \
        else \
            BS.Skip(_BITS); \
    } \

//Info, used only if INFO() is the only planned function for this indicator
#define INFO_B1(_INFO, _NAME) \
    int8u _INFO; \
    GET_B1(_INFO, _NAME) \

#define INFO_B2(_INFO, _NAME) \
    int16u _INFO; \
    GET_B2(_INFO, _NAME) \

#define INFO_B3(_INFO, _NAME) \
    int32u _INFO; \
    GET_B3(_INFO, _NAME) \

#define INFO_B4(_INFO, _NAME) \
    int32u _INFO; \
    GET_B4(_INFO, _NAME) \

#define INFO_B5(_INFO, _NAME) \
    int32u _INFO; \
    GET_B5(_INFO, _NAME) \

#define INFO_B6(_INFO, _NAME) \
    int32u _INFO; \
    GET_B6(_INFO, _NAME) \

#define INFO_B7(_INFO, _NAME) \
    int32u _INFO; \
    GET_B7(_INFO, _NAME) \

#define INFO_B8(_INFO, _NAME) \
    int64u _INFO; \
    GET_B8(_INFO, _NAME) \

#define INFO_BS(_BITS, _INFO, _NAME) \
    int32u _INFO; \
    GET_BS(_BITS, _INFO, _NAME) \

//Peek
#define PEEK_B1(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        _INFO=BigEndian2int8u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B2(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        _INFO=BigEndian2int16u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B3(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        _INFO=BigEndian2int24u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B4(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        _INFO=BigEndian2int32u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B5(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        _INFO=BigEndian2int40u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B6(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        _INFO=BigEndian2int48u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B7(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        _INFO=BigEndian2int56u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_B8(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        _INFO=BigEndian2int64u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L1(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+1, Stream_Pos) \
        _INFO=LittleEndian2int8u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L2(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+2, Stream_Pos) \
        _INFO=LittleEndian2int16u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L3(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+3, Stream_Pos) \
        _INFO=LittleEndian2int24u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L4(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+4, Stream_Pos) \
        _INFO=LittleEndian2int32u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L5(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+5, Stream_Pos) \
        _INFO=LittleEndian2int40u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L6(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+6, Stream_Pos) \
        _INFO=LittleEndian2int48u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L7(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+7, Stream_Pos) \
        _INFO=LittleEndian2int56u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_L8(_INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+8, Stream_Pos) \
        _INFO=LittleEndian2int64u(Buffer+Buffer_Offset+Stream_Pos); \
    }

#define PEEK_STRING(_BYTES, _INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.assign((const char*)(Buffer+Buffer_Offset+Stream_Pos), 0, _BYTES); \
    }

#define PEEK_LOCAL(_BYTES, _INFO) \
    { \
        INTEGRITY_SIZE_ATLEAST(Stream_Pos+_BYTES, Stream_Pos) \
        _INFO.From_Local((const char*)(Buffer+Buffer_Offset+Stream_Pos), _BYTES); \
    }

#define PEEK_BS(_BITS, _INFO, _NAME) \
    { \
        INTEGRITY(_BITS<=BS.Remain(), "Size is wrong", BS.Offset_Get()) \
        _INFO=BS.Peek(_BITS); \
    } \

//Must be 1
#define MARK_1(_NAME) \
    { \
        int32u Temp=BS.Get(1); \
        Details_Add_Info(BS.Offset_Get()-1, #_NAME, Temp); \
        if (Temp==0) \
        { \
            Details_Add_Info(BS.Offset_Get()-1, "Marker bit", "Problem"); \
            return INTEGRITY_RETURNVALUE; \
        } \
    }

//Must be 1
#define MARK_0(_NAME) \
    { \
        int32u Temp=BS.Get(1); \
        Details_Add_Info(BS.Offset_Get()-1, #_NAME, Temp); \
        if (Temp==1) \
        { \
            Details_Add_Info(BS.Offset_Get()-1, "Marker bit", "Problem"); \
            return INTEGRITY_RETURNVALUE; \
        } \
    }

//Flags management
#define GET_FLAGS(_FLAGS, _ORDER, _INFO, _NAME) \
    { \
        if (_FLAGS&(1<<_ORDER)) \
            _INFO=true; \
        else \
            _INFO=false; \
        ELEMENT_BEGIN(); \
        if (_INFO) \
            Details_Add_Info(Error, #_NAME, "Yes"); \
        else \
            Details_Add_Info(Error, #_NAME, "No"); \
        ELEMENT_END(); \
    } \

#define SKIP_FLAGS(_FLAGS, _ORDER, _NAME) \
    { \
        ELEMENT_BEGIN(); \
        if (_FLAGS&(1<<_ORDER)) \
            Details_Add_Info(Error, #_NAME, "Yes"); \
        else \
            Details_Add_Info(Error, #_NAME, "No"); \
        ELEMENT_END(); \
    } \

//Test, Get
#define TEST_BS(_BITS, _INFO, _TEST, _NAME) \
    { \
        GET (_BITS, _INFO, _NAME); \
        if (_TEST) \
        { \

//Test, Skip
#define TEST_BS_SKIP(_BITS, _NAME) \
    { \
        int32u _NAME=BS.Get(_BITS); \
        Details_Add_Info(BS.Offset_Get()-_BITS/8-(_BITS%8==0?0:1), #_NAME, _NAME); \
        if (_NAME) \
        { \

//Test, end
#define TEST_BS_END() \
        } \
    } \

#endif //MediaInfo_File__AnalyzeH

} //NameSpace

#endif

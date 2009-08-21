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
#ifndef MediaInfo_File__AnalyzeH
#define MediaInfo_File__AnalyzeH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include <ZenLib/BitStream.h>
#include <ZenLib/int128u.h>
#include <ZenLib/ZtringListList.h>
#include <vector>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File__Base
//***************************************************************************

class File__Analyze : public File__Base
{
public :
    //***************************************************************************
    // Constructor/Destructor
    //***************************************************************************

    File__Analyze();
    virtual ~File__Analyze();

    //***************************************************************************
    // Subs
    //***************************************************************************

    void    Open_Buffer_Continue (File__Analyze* Sub, const int8u* Buffer, size_t Buffer_Size);

    //***************************************************************************
    // Buffer
    //***************************************************************************

    //Buffer
    virtual void Read_Buffer_Init (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Continue (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Unsynched (); //Temp, should be in File__Base caller
    virtual void Read_Buffer_Finalize (); //Temp, should be in File__Base caller
    bool Buffer_Parse();

    //***************************************************************************
    // Init
    //***************************************************************************

    void BS_Begin();
    #define BS_BEGIN() BS_Begin()
    #define CC_BS_BEGIN() BS_Begin()

    //void BS_End();
    void BS_End();
    #define BS_END() BS_End()
    #define CC_BS_END() BS_End()

    //***************************************************************************
    // Header
    //***************************************************************************

    //Header - Management
    bool Header_Manage ();

    //Header - Begin
    virtual bool Header_Begin () {return true;};

    //Header - Parse
    virtual void Header_Parse () {};

    //Header - End
    virtual bool Header_End () {return true;};

    //Header - Info
    void Header_Fill_Code (int64u Code, const Ztring &Name);
    void Header_Fill_Size (int64u Size);

    //***************************************************************************
    // Data
    //***************************************************************************

    //Header - Management
    bool Data_Manage ();

    //Data - Parse
    virtual bool Data_Parse () {return false;};

    //Data - Detect EOF
    virtual bool Detect_EOF () {return false;};

    //***************************************************************************
    // Elements
    //***************************************************************************

    //Elements - Begin
    void Element_Begin ();
    void Element_Begin (const Ztring &Name, int64u Size=(int64u)-1);
    void Element_Begin (int64u Size) {Element_Begin(Ztring(), Size);}
    void Element_Begin (const char *Name, int64u Size=(int64u)-1) {Element_Begin(Ztring().From_UTF8(Name), Size);}
    #define ELEMENT_BEGIN Element_Begin

    //Elements - Name
    void Element_Name (const Ztring &Name);
    void Element_Name (const char*   Name) {Element_Name(Ztring().From_UTF8(Name));}

    //Elements - Info
    void Element_Info (const Ztring &Parameter);
    void Element_Info (const char*   Parameter) {Element_Info(Ztring().From_UTF8(Parameter));}
    void Element_Info (const char*   Parameter, const char*   Measure)      {Element_Info(Ztring().From_UTF8(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int8s         Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int8u         Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int16s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int16u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int32s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int32u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int64s        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int64u        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Element_Info (int128u       Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #ifdef NEED_SIZET
    void Element_Info (size_t        Parameter, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #endif //NEED_SIZET
    void Element_Info (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {Element_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    void Element_Info_Level (size_t  Level) {Element_Info_Level_=Level;};

    //Elements - End
    void Element_End ();
    void Element_End (const Ztring &Name, int64u Size=(int64u)-1);
    void Element_End (int64u Size) {Element_End(Ztring(), Size);}
    void Element_End (const char *Name, int64u Size=(int64u)-1) {Element_End(Ztring().From_UTF8(Name), Size);}
    #define ELEMENT_END Element_End

    //Elements - Message
    void Element_Message (const Ztring &Message) {Param(0, Message, Ztring());};

protected :
    //Element - Common
    void   Element_End_Common_Flush();
    Ztring Element_End_Common_Flush_Build();
public :

    //***************************************************************************
    // Param
    //***************************************************************************

    //TODO: put this in Ztring()
    Ztring ToZtring(const char* Value, size_t Value_Size=Unlimited, bool Utf8=true)
    {
        if (Utf8)
            return Ztring().From_UTF8(Value, Value_Size);
        else
            return Ztring().From_Local(Value, Value_Size);
    }
    #define VALUE(Value) \
        Ztring::ToZtring(Value, 16).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")")

    //Param - Main
    void Param (int64u Pos, const Ztring &Parameter, const Ztring& Value);
    void Param (int64u Pos, const char*   Parameter, const Ztring& Value) {Param(Pos, Ztring().From_Local(Parameter), Value);};
    void Param (int64u Pos, const char*   Parameter, const std::string& Value) {Param(Pos, Parameter, Ztring().From_Local(Value.c_str()));}
    void Param (int64u Pos, const char*   Parameter, const char*   Value, size_t Value_Size=Unlimited, bool Utf8=true) {Param(Pos, Parameter, ToZtring(Value, Value_Size, Utf8));}
    void Param (int64u Pos, const char*   Parameter, const int8u*  Value, size_t Value_Size=Unlimited, bool Utf8=true) {Param(Pos, Parameter, (const char*)Value, Value_Size, Utf8);}
    void Param (int64u Pos, const char*   Parameter, bool   Value) {if (Value) Param(Pos, Parameter, "Yes"); else Param(Pos, Parameter, "No");}
    void Param (int64u Pos, const char*   Parameter, int8u  Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int8s  Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int16u Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int16s Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int32u Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int32s Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int64u Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int64s Value) {Param(Pos, Parameter, VALUE(Value));}
    void Param (int64u Pos, const char*   Parameter, int128u Value){Param(Pos, Parameter, VALUE(Value));}
    #ifdef NEED_SIZET
    void Param (int64u Pos, const char*   Parameter, size_t Value, intu Radix=16) {Param(Pos, Parameter, Ztring::ToZtring(Value, Radix).MakeUpperCase()+_T(" (")+Ztring::ToZtring(Value, 10).MakeUpperCase()+_T(")"));}
    #endif //NEED_SIZET
    void Param (int64u Pos, const char*   Parameter, float32 Value, intu AfterComma=3) {Param(Pos, Parameter, Ztring::ToZtring(Value, AfterComma));}
    void Param (int64u Pos, const char*   Parameter, float64 Value, intu AfterComma=3) {Param(Pos, Parameter, Ztring::ToZtring(Value, AfterComma));}
    void Param (int64u Pos, const int32u  Parameter, const Ztring& Value) {Param(Pos, Ztring().From_CC4(Parameter), Value);};
    void Param (int64u Pos, const int16u  Parameter, const Ztring& Value) {Param(Pos, Ztring().From_CC2(Parameter), Value);};
    #define Details_Add_Info Param

    //Param - Info
    void Param_Info (const Ztring &Parameter);
    void Param_Info (const char*   Parameter) {Param_Info(Ztring().From_UTF8(Parameter));}
    void Param_Info (const char*   Parameter, const char*   Measure)      {Param_Info(Ztring().From_UTF8(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int64u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int64s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int32u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int32s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int16u        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int16s        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int8u         Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (int8s         Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    void Param_Info (float32       Parameter, size_t AfterComma=3, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter, AfterComma)+Ztring().From_UTF8(Measure));}
    #ifdef NEED_SIZET
    void Param_Info (size_t        Parameter, const char*   Measure=NULL) {Param_Info(Ztring::ToZtring(Parameter)+Ztring().From_UTF8(Measure));}
    #endif //NEED_SIZET
    #define INFO Param_Info

    //***************************************************************************
    // Information
    //***************************************************************************

    void Info (const Ztring& Value);

    //***************************************************************************
    // Big Endian
    //***************************************************************************

    void Get_B1  (int8u   &Info, const char* Name);
    void Get_B2  (int16u  &Info, const char* Name);
    void Get_B3  (int32u  &Info, const char* Name);
    void Get_B4  (int32u  &Info, const char* Name);
    void Get_B5  (int64u  &Info, const char* Name);
    void Get_B6  (int64u  &Info, const char* Name);
    void Get_B7  (int64u  &Info, const char* Name);
    void Get_B8  (int64u  &Info, const char* Name);
    void Get_B16 (int128u &Info, const char* Name);
    void Get_BF4 (float32 &Info, const char* Name);
    void Get_BF8 (float64 &Info, const char* Name);
    void Peek_B1 (int8u   &Info);
    void Peek_B2 (int16u  &Info);
    void Peek_B3 (int32u  &Info);
    void Peek_B4 (int32u  &Info);
    void Peek_B5 (int64u  &Info);
    void Peek_B6 (int64u  &Info);
    void Peek_B7 (int64u  &Info);
    void Peek_B8 (int64u  &Info);
    void Peek_B16(int128u &Info);
    void Peek_BF4(float32 &Info);
    void Peek_BF8(float64 &Info);
    void Skip_B1 (               const char* Name);
    void Skip_B2 (               const char* Name);
    void Skip_B3 (               const char* Name);
    void Skip_B4 (               const char* Name);
    void Skip_B5 (               const char* Name);
    void Skip_B6 (               const char* Name);
    void Skip_B7 (               const char* Name);
    void Skip_B8 (               const char* Name);
    void Skip_BF4(               const char* Name);
    void Skip_BF8(               const char* Name);
    void Skip_B16(               const char* Name);
    #define GET_B1(_INFO, _NAME)  Get_B1(_INFO, _NAME)
    #define GET_B2(_INFO, _NAME)  Get_B2(_INFO, _NAME)
    #define GET_B3(_INFO, _NAME)  Get_B3(_INFO, _NAME)
    #define GET_B4(_INFO, _NAME)  Get_B4(_INFO, _NAME)
    #define GET_B5(_INFO, _NAME)  Get_B5(_INFO, _NAME)
    #define GET_B6(_INFO, _NAME)  Get_B6(_INFO, _NAME)
    #define GET_B7(_INFO, _NAME)  Get_B7(_INFO, _NAME)
    #define GET_B8(_INFO, _NAME)  Get_B8(_INFO, _NAME)
    #define GET_B16(_INFO, _NAME) Get_B16(_INFO, _NAME)
    #define GET_BF4(_INFO, _NAME) Get_BF4(_INFO, _NAME)
    #define GET_BF8(_INFO, _NAME) Get_BF8(_INFO, _NAME)
    #define PEEK_B1(_INFO)  Peek_B1 (_INFO)
    #define PEEK_B2(_INFO)  Peek_B2 (_INFO)
    #define PEEK_B3(_INFO)  Peek_B3 (_INFO)
    #define PEEK_B4(_INFO)  Peek_B4 (_INFO)
    #define PEEK_B5(_INFO)  Peek_B5 (_INFO)
    #define PEEK_B6(_INFO)  Peek_B6 (_INFO)
    #define PEEK_B7(_INFO)  Peek_B7 (_INFO)
    #define PEEK_B8(_INFO)  Peek_B8 (_INFO)
    #define PEEK_B16(_INFO) Peek_B16(_INFO)
    #define PEEK_BF4(_INFO) Peek_BF4(_INFO)
    #define PEEK_BF8(_INFO) Peek_BF8(_INFO)
    #define SKIP_B1(_NAME)  Skip_B1 (_NAME)
    #define SKIP_B2(_NAME)  Skip_B2 (_NAME)
    #define SKIP_B3(_NAME)  Skip_B3 (_NAME)
    #define SKIP_B4(_NAME)  Skip_B4 (_NAME)
    #define SKIP_B5(_NAME)  Skip_B5 (_NAME)
    #define SKIP_B6(_NAME)  Skip_B6 (_NAME)
    #define SKIP_B7(_NAME)  Skip_B7 (_NAME)
    #define SKIP_B8(_NAME)  Skip_B8 (_NAME)
    #define SKIP_B16(_NAME) Skip_B16(_NAME)
    #define SKIP_BF4(_NAME) Skip_BF4(_NAME)
    #define SKIP_BF8(_NAME) Skip_BF8(_NAME)
    #define INFO_B1(_INFO, _NAME)  int8u   _INFO; Get_B1 (_INFO, _NAME)
    #define INFO_B2(_INFO, _NAME)  int16u  _INFO; Get_B2 (_INFO, _NAME)
    #define INFO_B3(_INFO, _NAME)  int32u  _INFO; Get_B3 (_INFO, _NAME)
    #define INFO_B4(_INFO, _NAME)  int32u  _INFO; Get_B4 (_INFO, _NAME)
    #define INFO_B5(_INFO, _NAME)  int64u  _INFO; Get_B5 (_INFO, _NAME)
    #define INFO_B6(_INFO, _NAME)  int64u  _INFO; Get_B6 (_INFO, _NAME)
    #define INFO_B7(_INFO, _NAME)  int64u  _INFO; Get_B7 (_INFO, _NAME)
    #define INFO_B8(_INFO, _NAME)  int64u  _INFO; Get_B8 (_INFO, _NAME)
    #define INFO_B16(_INFO, _NAME) int128u _INFO; Get_B16(_INFO, _NAME)
    #define INFO_BF4(_INFO, _NAME) float32 _INFO; Get_BF4(_INFO, _NAME)
    #define INFO_BF8(_INFO, _NAME) float64 _INFO; Get_BF8(_INFO, _NAME)

    //***************************************************************************
    // Little Endian
    //***************************************************************************

    void Get_L1  (int8u   &Info, const char* Name);
    void Get_L2  (int16u  &Info, const char* Name);
    void Get_L3  (int32u  &Info, const char* Name);
    void Get_L4  (int32u  &Info, const char* Name);
    void Get_L5  (int64u  &Info, const char* Name);
    void Get_L6  (int64u  &Info, const char* Name);
    void Get_L7  (int64u  &Info, const char* Name);
    void Get_L8  (int64u  &Info, const char* Name);
    void Get_L16 (int128u &Info, const char* Name);
    void Get_LF4 (float32 &Info, const char* Name);
    void Get_LF8 (float64 &Info, const char* Name);
    void Peek_L1 (int8u   &Info);
    void Peek_L2 (int16u  &Info);
    void Peek_L3 (int32u  &Info);
    void Peek_L4 (int32u  &Info);
    void Peek_L5 (int64u  &Info);
    void Peek_L6 (int64u  &Info);
    void Peek_L7 (int64u  &Info);
    void Peek_L8 (int64u  &Info);
    void Peek_L16(int128u &Info);
    void Peek_LF4(float32 &Info);
    void Peek_LF8(float64 &Info);
    void Skip_L1 (               const char* Name);
    void Skip_L2 (               const char* Name);
    void Skip_L3 (               const char* Name);
    void Skip_L4 (               const char* Name);
    void Skip_L5 (               const char* Name);
    void Skip_L6 (               const char* Name);
    void Skip_L7 (               const char* Name);
    void Skip_L8 (               const char* Name);
    void Skip_LF4(               const char* Name);
    void Skip_LF8(               const char* Name);
    void Skip_L16(               const char* Name);
    #define GET_L1(_INFO, _NAME)  Get_L1(_INFO, _NAME)
    #define GET_L2(_INFO, _NAME)  Get_L2(_INFO, _NAME)
    #define GET_L3(_INFO, _NAME)  Get_L3(_INFO, _NAME)
    #define GET_L4(_INFO, _NAME)  Get_L4(_INFO, _NAME)
    #define GET_L5(_INFO, _NAME)  Get_L5(_INFO, _NAME)
    #define GET_L6(_INFO, _NAME)  Get_L6(_INFO, _NAME)
    #define GET_L7(_INFO, _NAME)  Get_L7(_INFO, _NAME)
    #define GET_L8(_INFO, _NAME)  Get_L8(_INFO, _NAME)
    #define GET_L16(_INFO, _NAME) Get_L16(_INFO, _NAME)
    #define GET_LF4(_INFO, _NAME) Get_LF4(_INFO, _NAME)
    #define GET_LF8(_INFO, _NAME) Get_LF8(_INFO, _NAME)
    #define PEEK_L1(_INFO)  Peek_L1 (_INFO)
    #define PEEK_L2(_INFO)  Peek_L2 (_INFO)
    #define PEEK_L3(_INFO)  Peek_L3 (_INFO)
    #define PEEK_L4(_INFO)  Peek_L4 (_INFO)
    #define PEEK_L5(_INFO)  Peek_L5 (_INFO)
    #define PEEK_L6(_INFO)  Peek_L6 (_INFO)
    #define PEEK_L7(_INFO)  Peek_L7 (_INFO)
    #define PEEK_L8(_INFO)  Peek_L8 (_INFO)
    #define PEEK_L16(_INFO) Peek_L16(_INFO)
    #define PEEK_LF4(_INFO) Peek_LF4(_INFO)
    #define PEEK_LF8(_INFO) Peek_LF8(_INFO)
    #define SKIP_L1(_NAME)  Skip_L1 (_NAME)
    #define SKIP_L2(_NAME)  Skip_L2 (_NAME)
    #define SKIP_L3(_NAME)  Skip_L3 (_NAME)
    #define SKIP_L4(_NAME)  Skip_L4 (_NAME)
    #define SKIP_L5(_NAME)  Skip_L5 (_NAME)
    #define SKIP_L6(_NAME)  Skip_L6 (_NAME)
    #define SKIP_L7(_NAME)  Skip_L7 (_NAME)
    #define SKIP_L8(_NAME)  Skip_L8 (_NAME)
    #define SKIP_L16(_NAME) Skip_L16(_NAME)
    #define SKIP_LF4(_NAME) Skip_LF4(_NAME)
    #define SKIP_LF8(_NAME) Skip_LF8(_NAME)
    #define INFO_L1(_INFO, _NAME)  int8u   _INFO; Get_L1 (_INFO, _NAME)
    #define INFO_L2(_INFO, _NAME)  int16u  _INFO; Get_L2 (_INFO, _NAME)
    #define INFO_L3(_INFO, _NAME)  int32u  _INFO; Get_L3 (_INFO, _NAME)
    #define INFO_L4(_INFO, _NAME)  int32u  _INFO; Get_L4 (_INFO, _NAME)
    #define INFO_L5(_INFO, _NAME)  int64u  _INFO; Get_L5 (_INFO, _NAME)
    #define INFO_L6(_INFO, _NAME)  int64u  _INFO; Get_L6 (_INFO, _NAME)
    #define INFO_L7(_INFO, _NAME)  int64u  _INFO; Get_L7 (_INFO, _NAME)
    #define INFO_L8(_INFO, _NAME)  int64u  _INFO; Get_L8 (_INFO, _NAME)
    #define INFO_L16(_INFO, _NAME) int128u _INFO; Get_L16(_INFO, _NAME)
    #define INFO_LF4(_INFO, _NAME) float32 _INFO; Get_LF4(_INFO, _NAME)
    #define INFO_LF8(_INFO, _NAME) float64 _INFO; Get_LF8(_INFO, _NAME)

    //***************************************************************************
    // EBML
    //***************************************************************************

    void Get_EB (int64u &Info, const char* Name);
    void Skip_EB(              const char* Name);
    #define GET_EB(_INFO, _NAME) Get_EB(_INFO, _NAME)
    #define SKIP_EB(_NAME) Skip_EB(_NAME)
    #define INFO_EB(_INFO, _NAME) int64u _INFO; Get_EB(_INFO, _NAME)

    //***************************************************************************
    // Characters
    //***************************************************************************

    void Get_C3 (int32u &Info, const char* Name);
    void Get_C4 (int32u &Info, const char* Name);
    void Get_C6 (int64u &Info, const char* Name);
    void Skip_C3(              const char* Name);
    void Skip_C4(              const char* Name);
    void Skip_C6(              const char* Name);
    #define GET_C3(_INFO, _NAME) Get_C3(_INFO, _NAME)
    #define GET_C4(_INFO, _NAME) Get_C4(_INFO, _NAME)
    #define GET_C6(_INFO, _NAME) Get_C6(_INFO, _NAME)
    #define SKIP_C3(_NAME) Skip_C3(_NAME)
    #define SKIP_C4(_NAME) Skip_C4(_NAME)
    #define INFO_C3(_INFO, _NAME) Ztring _INFO; Get_C3(_INFO, _NAME)
    #define INFO_C4(_INFO, _NAME) Ztring _INFO; Get_C4(_INFO, _NAME)

    //***************************************************************************
    // Text
    //***************************************************************************

    void Get_Local (int64u Bytes, Ztring &Info, const char* Name);
    void Get_UTF8 (int64u Bytes, Ztring &Info, const char* Name);
    void Skip_Local(int64u Bytes, const char* Name);
    void Skip_UTF8(int64u Bytes, const char* Name);
    #define GET_LOCAL(_BYTES, _INFO, _NAME) Get_Local(_BYTES, _INFO, _NAME)
    #define GET_UTF8(_BYTES, _INFO, _NAME)  Get_UTF8 (_BYTES, _INFO, _NAME)
    #define SKIP_LOCAL(_BYTES, _NAME) Skip_Local(_BYTES, _NAME)
    #define SKIP_UTF8(_BYTES, _NAME)  Skip_UTF8 (_BYTES, _NAME)
    #define INFO_LOCAL(_BYTES, _INFO, _NAME) Ztring _INFO; Get_Local(_BYTES, _INFO, _NAME)
    #define INFO_UTF8(_BYTES, _INFO, _NAME)  Ztring _INFO; Get_UTF8 (_BYTES, _INFO, _NAME)

    //***************************************************************************
    // Unknown
    //***************************************************************************

    void Skip_XX(int64u Bytes, const char* Name);
    #define SKIP_XX(_BYTES, _NAME) Skip_XX(_BYTES, _NAME)

    //***************************************************************************
    // Flags
    //***************************************************************************

    void Get_Flags (int64u Flags, size_t Order, bool &Info, const char* Name);
    void Skip_Flags(int64u Flags, size_t Order,             const char* Name);
    #define GET_FLAGS(_FLAGS, _ORDER, _INFO, _NAME) Get_Flags(_FLAGS, _ORDER, _INFO, _NAME)
    #define SKIP_FLAGS(_FLAGS, _ORDER, _NAME) Skip_Flags(_FLAGS, _ORDER, _NAME)

    //***************************************************************************
    // BitStream
    //***************************************************************************

    void Get_BS (size_t Bits, int32u  &Info, const char* Name);
    void Get_SB (             bool    &Info, const char* Name);
    void Get_S1 (size_t Bits, int8u   &Info, const char* Name);
    void Get_S2 (size_t Bits, int16u  &Info, const char* Name);
    void Get_S4 (size_t Bits, int32u  &Info, const char* Name);
    void Get_S8 (size_t Bits, int64u  &Info, const char* Name);
    void Peek_BS(size_t Bits, int32u  &Info);
    void Peek_SB(              bool    &Info);
    void Peek_S1(size_t Bits, int8u   &Info);
    void Peek_S2(size_t Bits, int16u  &Info);
    void Peek_S4(size_t Bits, int32u  &Info);
    void Skip_BS(size_t Bits,                const char* Name);
    void Skip_SB(                            const char* Name);
    void Skip_S1(size_t Bits,                const char* Name);
    void Skip_S2(size_t Bits,                const char* Name);
    void Skip_S4(size_t Bits,                const char* Name);
    void Skip_S8(size_t Bits,                const char* Name);
    void Mark_0 (                            const char* Name);
    void Mark_1 (                            const char* Name);
    #define GET_BS(_BITS, _INFO, _NAME) Get_BS(_BITS, _INFO, _NAME)
    #define GET_SB(_INFO, _NAME)        Get_SB(       _INFO, _NAME)
    #define GET_S1(_BITS, _INFO, _NAME) Get_S1(_BITS, _INFO, _NAME)
    #define GET_S2(_BITS, _INFO, _NAME) Get_S2(_BITS, _INFO, _NAME)
    #define GET_S3(_BITS, _INFO, _NAME) Get_S4(_BITS, _INFO, _NAME)
    #define GET_S4(_BITS, _INFO, _NAME) Get_S4(_BITS, _INFO, _NAME)
    #define GET_S8(_BITS, _INFO, _NAME) Get_S8(_BITS, _INFO, _NAME)
    #define PEEK_BS(_BITS, _INFO) Peek_BS(_BITS, _INFO)
    #define PEEK_SB(_INFO)        Peek_SB(       _INFO)
    #define PEEK_S1(_BITS, _INFO) Peek_S1(_BITS, _INFO)
    #define PEEK_S2(_BITS, _INFO) Peek_S2(_BITS, _INFO)
    #define PEEK_S3(_BITS, _INFO) Peek_S4(_BITS, _INFO)
    #define PEEK_S4(_BITS, _INFO) Peek_S4(_BITS, _INFO)
    #define SKIP_BS(_BITS, _NAME) Skip_BS(_BITS, _NAME)
    #define SKIP_SB(_NAME)        Skip_SB(       _NAME)
    #define SKIP_S1(_BITS, _NAME) Skip_S1(_BITS, _NAME)
    #define SKIP_S2(_BITS, _NAME) Skip_S2(_BITS, _NAME)
    #define SKIP_S3(_BITS, _NAME) Skip_S4(_BITS, _NAME)
    #define SKIP_S4(_BITS, _NAME) Skip_S4(_BITS, _NAME)
    #define SKIP_S8(_BITS, _NAME) Skip_S8(_BITS, _NAME)
    #define MARK_0(_NAME)         Mark_0 (       _NAME)
    #define MARK_1(_NAME)         Mark_1 (       _NAME)
    #define INFO_BS(_BITS, _INFO, _NAME) int32u  _INFO; Get_BS(_BITS, _INFO, _NAME)
    #define INFO_SB(_INFO, _NAME)        bool    _INFO; Get_SB(       _INFO, _NAME)
    #define INFO_S1(_BITS, _INFO, _NAME) int8u   _INFO; Get_S1(_BITS, _INFO, _NAME)
    #define INFO_S2(_BITS, _INFO, _NAME) int16u  _INFO; Get_S2(_BITS, _INFO, _NAME)
    #define INFO_S3(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define INFO_S4(_BITS, _INFO, _NAME) int32u  _INFO; Get_S4(_BITS, _INFO, _NAME)
    #define INFO_S8(_BITS, _INFO, _NAME) int64u  _INFO; Get_S8(_BITS, _INFO, _NAME)

    //***************************************************************************
    // Element trusting
    //***************************************************************************

    void Trusted_IsNot (const char* Reason, int64u Size=(int64u)(-1));

    //***************************************************************************
    // Filling
    //***************************************************************************

    //Is OK?
    bool Element_IsOK ();
    bool Element_WaitForMoreData ();

    //Begin
    #define FILLING_BEGIN() if (Element_IsOK()) {

    //End
    #define FILLING_END() }

    //***************************************************************************
    //
    //***************************************************************************

protected :
    //Synchro
    bool Synched;               //Data is synched

    //Header
    int64u HeaderSize;          //Size of the header of the current element
    //bool   IsParsingHeader;     //Header is being parsed
    //bool   IsNotSure;     //Header is being parsed

    //Element
    size_t Element_Pos;         //Position in the Element
    int64u Element_Size;        //Size of the Element
    BitStream* BS;              //From the Element (must be called by BS_BEGIN())

    //Elements
    size_t Element_Level_Base;  //From other parsers
    size_t Element_Level;       //Current level
    size_t Element_Info_Level_; //Specify which Element is being informed (ToShow part)
    struct element_details
    {
        struct to_show
        {
            int64u Pos;             //Position of the element in the file
            int64u Size;            //Size of the element (including header and sub-elements)
            int64u HeaderSize;      //Size of the header of the element
            Ztring Name;            //Name planned for this element
            Ztring Info;            //More info about the element
            Ztring Details;         //The main text
            bool   NoShow;          //Don't show this element
        };

        int64u  Code;            //Code filled in the file
        int64u  Next;            //
        bool    WaitForMoreData; //This element is not complete, we need more data
        bool    UnTrusted;       //This element has a problem
        bool    IsComplete;      //This element is fully buffered, no need of more
        bool    InLoop;          //This element is in a parsing loop
        to_show ToShow;
    };
    std::vector<element_details> Element;

    //File_Duplicate_Get
    void  ToTS(int16u PID, int16u Value);
    std::map <int16u, void*>    ToTS_File_16;
};

} //NameSpace



/*
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
*/

#define ATOM_BEGIN \
    if (Level!=Element_Level) \
    { \
        Level++; \
        switch (Element[Level].Code) \
        { \

#define ATOM(_ATOM) \
            case Elements::_ATOM : \
                    if (Level==Element_Level) \
                    { \
                        if (Element[Element_Level].IsComplete==true) \
                        { \
                            Element[Element_Level].WaitForMoreData=false; \
                            _ATOM(); \
                        } \
                        else \
                        { \
                            Element[Element_Level].WaitForMoreData=true; \
                            IsNotSure=true; \
                            return false; \
                        } \
                    } \
                    break; \

#define ATOM_END \
            default : \
            Element[Element_Level].WaitForMoreData=false; \
            Element[Element_Level].IsComplete=true; \
            SKIP_XX(Element[Element_Level].Next-(File_Offset+Buffer_Offset), "Unknown"); \
        } \
    } \
    break; \


#define LIST(_ATOM) \
    case Elements::_ATOM : \
            if (Level==Element_Level) \
            { \
                IsNotSure=true; \
                _ATOM(); \
                if (Element[Element_Level].WaitForMoreData) \
                    return false; \
                IsNotSure=false; \
                Buffer_Offset+=Element_Pos; \
                List=true; \
            } \

#define DATA_BEGIN \
    size_t Level=0; \
    bool List=false; \
    ATOM_BEGIN \

#define DATA_END \
        default : ; \
    }} \
     \
    if (List) \
    { \
        size_t Element_Level_Save=Element_Level; \
        Element[Element_Level].InLoop=true; \
        while (Buffer_Parse()); \
        while (Element_Level_Save<=Element_Level) \
        { \
            Element[Element_Level_Save].InLoop=false; \
            Element_Level_Save++; \
        } \
        if (Element[Element_Level].WaitForMoreData) \
            return false; \
    } \
     \
    return true; \

#endif

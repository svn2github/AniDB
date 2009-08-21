// File_Mpeg_Descriptors - Info for MPEG files
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
// Information about MPEG files, Descriptors
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_Mpeg_DescriptorsH
#define MediaInfo_Mpeg_DescriptorsH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include "MediaInfo/Multiple/File_MpegTs.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Mpeg_Descriptors
//***************************************************************************

class File_Mpeg_Descriptors : public File__Base
{
public :
    int32u format_identifier; //Must be filled by the caller
    stream_t KindOfStream;
    std::map<ZenLib::Ztring, ZenLib::Ztring> Info;
    std::map<int32u, File_MpegTs::ts_kind> Kind;

protected :
    //Format
    void Read_Buffer_Continue ();

public :
    File_Mpeg_Descriptors();

private :
    //Buffer
    bool Buffer_Parse ();
    bool Element_Parse();
    int32u Element_Size;
    int8u  Element_Name;

    //Elements
    void Descriptors();
    void Descriptor();
    void Descriptor_00() {};
    void Descriptor_01() {};
    void Descriptor_02();
    void Descriptor_03();
    void Descriptor_04() {};
    void Descriptor_05();
    void Descriptor_06();
    void Descriptor_07() {};
    void Descriptor_08() {};
    void Descriptor_09();
    void Descriptor_0A();
    void Descriptor_0B() {};
    void Descriptor_0C() {};
    void Descriptor_0D() {};
    void Descriptor_0E();
    void Descriptor_0F();
    void Descriptor_10();
    void Descriptor_11() {};
    void Descriptor_12() {};
    void Descriptor_13() {};
    void Descriptor_14() {};
    void Descriptor_15() {};
    void Descriptor_16() {};
    void Descriptor_17() {};
    void Descriptor_18() {};
    void Descriptor_19() {};
    void Descriptor_1A() {};
    void Descriptor_1B() {};
    void Descriptor_1C() {};
    void Descriptor_1D() {};
    void Descriptor_1E() {};
    void Descriptor_1F() {};
    void Descriptor_20() {};
    void Descriptor_21() {};
    void Descriptor_22() {};
    void Descriptor_23() {};
    void Descriptor_40() {};
    void Descriptor_41() {};
    void Descriptor_42() {};
    void Descriptor_43() {};
    void Descriptor_44() {};
    void Descriptor_45() {};
    void Descriptor_46() {};
    void Descriptor_47() {};
    void Descriptor_48();
    void Descriptor_49() {};
    void Descriptor_4A() {};
    void Descriptor_4B() {};
    void Descriptor_4C() {};
    void Descriptor_4D();
    void Descriptor_4E() {};
    void Descriptor_4F() {};
    void Descriptor_50();
    void Descriptor_51() {};
    void Descriptor_52();
    void Descriptor_53() {};
    void Descriptor_54();
    void Descriptor_55() {};
    void Descriptor_56();
    void Descriptor_57() {};
    void Descriptor_58() {};
    void Descriptor_59();
    void Descriptor_5A() {};
    void Descriptor_5B() {};
    void Descriptor_5C() {};
    void Descriptor_5D() {};
    void Descriptor_5E() {};
    void Descriptor_5F() {};
    void Descriptor_60() {};
    void Descriptor_61() {};
    void Descriptor_62() {};
    void Descriptor_63() {};
    void Descriptor_64() {};
    void Descriptor_65() {};
    void Descriptor_66() {};
    void Descriptor_67() {};
    void Descriptor_68() {};
    void Descriptor_69() {};
    void Descriptor_6A();
    void Descriptor_6B() {};
    void Descriptor_6C() {};
    void Descriptor_6D() {};
    void Descriptor_6E() {};
    void Descriptor_6F() {};
    void Descriptor_70() {};
    void Descriptor_71() {};
    void Descriptor_72() {};
    void Descriptor_73() {};
    void Descriptor_74() {};
    void Descriptor_75() {};
    void Descriptor_76() {};
    void Descriptor_77() {};
    void Descriptor_78() {};
    void Descriptor_79() {};
    void Descriptor_7A();
    void Descriptor_7B();
    void Descriptor_7C();
    void Descriptor_7D() {};
    void Descriptor_7E() {};
    void Descriptor_7F() {};
    void Descriptor_80() {};
    void Descriptor_81();
    void Descriptor_86() {};
    void Descriptor_87() {};
    void Descriptor_A0();
    void Descriptor_A1();
    void Descriptor_A2() {};
    void Descriptor_A3();
    void Descriptor_A8() {};
    void Descriptor_A9() {};
    void Descriptor_AA() {};
    void Descriptor_AB() {};

    //Helpers
    void ATSC_multiple_string_structure(Ztring &Value);
};

//***************************************************************************
// Const
//***************************************************************************

namespace Mpeg_Descriptors
{
    const int32u AC_3=0x41432D33; //Exactly AC-3
    const int32u DTS1=0x44545331; //DTS
    const int32u GA94=0x47413934; //ATSC
    const int32u HDMV=0x48444D56; //BluRay

    const int32u DVB =0x00000001; //Forced value, does not exist is stream
}

} //NameSpace

#endif

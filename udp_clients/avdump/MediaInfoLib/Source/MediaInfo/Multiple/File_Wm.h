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

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_WmH
#define MediaInfo_File_WmH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
#include <map>
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
#define WM_ELEMENT_LEVEL_MAX 20
//---------------------------------------------------------------------------

//***************************************************************************
// Class File_Wm
//***************************************************************************

class File_Wm : public File__Base
{
protected :
    //Format
    void Read_Buffer_Init ();
    void Read_Buffer_Continue ();
    void Read_Buffer_Finalize ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int8u  Element_Level;
    int64u Element_Size;
    int32u Element_Name[WM_ELEMENT_LEVEL_MAX][4]; //GUID part 1
    int64u Element_Next[WM_ELEMENT_LEVEL_MAX];

    //Elements
    void Header();
    void Header_FileProperties();
    void Header_StreamProperties();
    void Header_StreamProperties_Audio();
    void Header_StreamProperties_Video();
    void Header_HeaderExtension();
    void Header_HeaderExtension_ExtendedStreamProperties();
    void Header_HeaderExtension_AdvancedMutualExclusion();
    void Header_HeaderExtension_GroupMutualExclusion();
    void Header_HeaderExtension_StreamPrioritization();
    void Header_HeaderExtension_BandwidthSharing();
    void Header_HeaderExtension_LanguageList();
    void Header_HeaderExtension_Metadata();
    void Header_HeaderExtension_MetadataLibrary();
    void Header_HeaderExtension_IndexParameters();
    void Header_HeaderExtension_MediaIndexParameters();
    void Header_HeaderExtension_TimecodeIndexParameters();
    void Header_HeaderExtension_Compatibility();
    void Header_HeaderExtension_AdvancedContentEncryption();
    void Header_CodecList();
    void Header_ScriptCommand();
    void Header_Marker();
    void Header_BitRateMutualExclusion();
    void Header_ErrorCorrection();
    void Header_ContentDescription();
    void Header_ExtendedContentDescription();
    void Header_StreamBitRate();
    void Header_ContentBranding();
    void Header_ContentEncryption();
    void Header_ExtendedContentEncryption();
    void Header_DigitalSignature();
    void Header_Padding();
    void Data();
    void SimpleIndex();
    void Index();
    void MediaIndex();
    void TimecodeIndex();

    //Helpers
    bool Element_Level_Get();
    bool Element_Name_Get();
    bool Element_Size_Get();

    //Temp
    bool List;
    std::map<int8u, stream_t> CurrentStream_Kind;
    std::map<int8u, size_t>   CurrentStream_Pos;
    std::vector <int32u>      CurrentBitRate;
};

//***************************************************************************
// Const
//***************************************************************************

#define GUID(NAME, PART1, PART2, PART3, PART4, PART5) \
    const int32u NAME =0x##PART1; \
    const int32u NAME##1=0x##PART1; \
    const int32u NAME##2=(0x##PART3<<16)&0x##PART2; \
    const int32u NAME##3=(0x##PART4<<16)&(0x##PART5##LL>>16);\
    const int32u NAME##4=0x##PART5##LL&0x00FFFF; \

namespace Wm
{
    GUID(Header,                                            75B22630, 668E, 11CF, A6D9, 00AA0062CE6C)
    GUID(Header_FileProperties,                             8CABDCA1, A947, 11CF, 8EE4, 00C00C205365)
    GUID(Header_StreamProperties,                           B7DC0791, A9B7, 11CF, 8EE6, 00C00C205365)
    GUID(Header_StreamProperties_Audio,                     F8699E40, 5B4D, 11CF, A8FD, 00805F5C442B)
    GUID(Header_StreamProperties_Video,                     BC19EFC0, 5B4D, 11CF, A8FD, 00805F5C442B)
    GUID(Header_StreamProperties_Command,                   59DACFC0, 59E6, 11D0, A3AC, 00A0C90348F6)
    GUID(Header_StreamProperties_JFIF,                      B61BE100, 5B4E, 11CF, A8FD, 00805F5C442B)
    GUID(Header_StreamProperties_DegradableJPEG,            35907DE0, E415, 11CF, A917, 00805F5C442B)
    GUID(Header_StreamProperties_FileTransfer,              91BD222C, F21C, 497A, 8B6D, 5AA86BFC0185)
    GUID(Header_StreamProperties_Binary,                    3AFB65E2, 47EF, 40F2, AC2C, 70A90D71D343)
    GUID(Header_HeaderExtension,                            5FBF03B5, A92E, 11CF, 8EE3, 00C00C205365)
    GUID(Header_HeaderExtension_ExtendedStreamProperties,   14E6A5CB, C672, 4332, 8399, A96952065B5A)
    GUID(Header_HeaderExtension_AdvancedMutualExclusion,    A08649CF, 4775, 4670, 8A16, 6E35357566CD)
    GUID(Header_HeaderExtension_GroupMutualExclusion,       D1465A40, 5A79, 4338, B71B, E36B8FD6C249)
    GUID(Header_HeaderExtension_StreamPrioritization,       D4FED15B, 88D3, 454F, 81F0, ED5C45999E24)
    GUID(Header_HeaderExtension_BandwidthSharing,           A69609E6, 517B, 11D2, B6AF, 00C04FD908E9)
    GUID(Header_HeaderExtension_LanguageList,               7C4346A9, EFE0, 4BFC, B229, 393EDE415C85)
    GUID(Header_HeaderExtension_Metadata,                   C5F8CBEA, 5BAF, 4877, 8467, AA8C44FA4CCA)
    GUID(Header_HeaderExtension_MetadataLibrary,            44231C94, 9498, 49D1, A141, 1D134E457054)
    GUID(Header_HeaderExtension_IndexParameters,            D6E229DF, 35DA, 11D1, 9034, 00A0C90349BE)
    GUID(Header_HeaderExtension_MediaIndexParameters,       6B203BAD, 3F11, 48E4, ACA8, D7613DE2CFA7)
    GUID(Header_HeaderExtension_TimecodeIndexParameters,    F55E496D, 9797, 4B5D, 8C8B, 604DFE9BFB24)
    GUID(Header_HeaderExtension_Compatibility,              75B22630, 668E, 11CF, A6D9, 00AA0062CE6C)
    GUID(Header_HeaderExtension_AdvancedContentEncryption,  43058533, 6981, 49E6, 9B74, AD12CB86D58C)
    GUID(Header_CodecList,                                  86D15240, 311D, 11D0, A3A4, 00ACC90348F6)
    GUID(Header_ScriptCommand,                              1EFB1A30, 0B62, 11D0, A39B, 00A0C90348F6)
    GUID(Header_Marker,                                     F487CD01, A951, 11CF, 8EE6, 00C00C205365)
    GUID(Header_BitRateMutualExclusion,                     D6E229DC, 35DA, 11D1, 9034, 00A0C90349BE)
    GUID(Header_ErrorCorrection,                            75B22635, 668E, 11CF, A6D9, 00AA0062CE6C)
    GUID(Header_ContentDescription,                         75B22633, 668E, 11CF, A6D9, 00AA0062CE6C)
    GUID(Header_ExtendedContentDescription,                 D2D0A440, E307, 11D2, 97F0, 00A0C95EA850)
    GUID(Header_StreamBitRate,                              7BF875CE, 468D, 11D1, 8D82, 006097C9A2B2)
    GUID(Header_ContentBranding,                            2211B3FA, BD23, 11D2, B4B7, 00A0C955FC6E)
    GUID(Header_ContentEncryption,                          2211B3FB, BD23, 11D2, B4B7, 00A0C955FC6E)
    GUID(Header_ExtendedContentEncryption,                  298AE614, 2622, 4C17, B935, DAE07EE9289C)
    GUID(Header_DigitalSignature,                           2211B3FC, BD23, 11D2, B4B7, 00A0C955FC6E)
    GUID(Header_Padding,                                    1806D474, CADF, 4509, A4BA, 9AABCB96AAE8)
    GUID(Data,                                              75B22636, 668E, 11CF, A6D9, 00AA0062CE6C)
    GUID(SimpleIndex,                                       33000890, E5B1, 11CF, 89F4, 00A0C90349CB)
    GUID(Index,                                             D6E229D3, 35DA, 11D1, 9034, 00A0C90349BE)
    GUID(MediaIndex,                                        FEB103F8, 12AD, 4C64, 840F, 2A1D2F7AD48C)
    GUID(TimecodeIndex,                                     3CB73FD0, 0C4A, 4803, 953D, EDF7B6228F0C)
}

} //NameSpace

#endif

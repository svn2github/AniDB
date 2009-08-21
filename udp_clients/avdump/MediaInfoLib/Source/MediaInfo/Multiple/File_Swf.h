// File_Swf - Info for SWF Audio files
// Copyright (C) 2005-2007 Jerome Martinez, Zen@MediaArea.net
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
// Information about SWF files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_SwfH
#define MediaInfo_File_SwfH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Swf
//***************************************************************************

class File_Swf : public File__Base
{
protected :
    //Format
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Replacement of File__Base
    const int8u* Buffer;
    size_t Buffer_Size;
    int64u File_Offset;

    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int32u Element_HeaderSize;
    int32u Element_Size;
    int16u Element_Name;
    int64u Element_Next;

    //Elements
    void Header();
    void Header_Continue();
    void End()                      {};
    void ShowFrame()                {};
    void DefineShape()              {};
    void PlaceObject()              {};
    void RemoveObject()             {};
    void DefineBits()               {};
    void DefineButton()             {};
    void JPEGTables()               {};
    void SetBackgroundColor()       {};
    void DefineFont()               {};
    void DefineText()               {};
    void DoAction()                 {};
    void DefineFontInfo()           {};
    void DefineSound();
    void StartSound()               {};
    void DefineButtonSound()        {};
    void SoundStreamHead();
    void SoundStreamBlock()         {};
    void DefineBitsLossless()       {};
    void DefineBitsJPEG2()          {};
    void DefineShape2()             {};
    void DefineCxform()             {};
    void Protected()                {};
    void PlaceObject2()             {};
    void RemoveObject2()            {};
    void DefineShape3()             {};
    void DefineText2()              {};
    void DefineButton2()            {};
    void DefineBitsJPEG3()          {};
    void DefineBitsLossless2()      {};
    void DefineEditText()           {};
    void DefineSprite()             {};
    void FrameLabel()               {};
    void DefineMorphShape()         {};
    void SoundStreamHead2();
    void DefineFont2()              {};
    void ExportAssets()             {};
    void ImportAssets()             {};
    void EnableDebugger()           {};
    void DoInitAction()             {};
    void DefineVideoStream();
    void DefineVideoFrame()         {};
    void DefineFontInfo2()          {};
    void EnableDebugger2()          {};
    void ScriptLimits()             {};
    void SetTabIndex()              {};
    void FileAttributes()           {};
    void PlaceObject3()             {};
    void ImportAssets2()            {};
    void DefineFontAlignZones()     {};
    void CSMTextSettings()          {};
    void DefineFont3()              {};
    void Metadata()                 {};
    void DefineScalingGrid()        {};
    void DefineShape4()             {};
    void DefineMorphShape2()        {};

    //Temp
    int8u  Version;
    int32u FileLength;

    //Helpers
    bool Decompress();
};

//***************************************************************************
// Const
//***************************************************************************

namespace Swf
{
    const int16u End                        =0x0000; //V1+
    const int16u ShowFrame                  =0x0001; //V1+
    const int16u DefineShape                =0x0002; //V1+
    const int16u PlaceObject                =0x0004; //V1-V2
    const int16u RemoveObject               =0x0005; //V1+
    const int16u DefineBits                 =0x0006; //V1+
    const int16u DefineButton               =0x0007; //V1+
    const int16u JPEGTables                 =0x0008; //V1+
    const int16u SetBackgroundColor         =0x0009; //V1+
    const int16u DefineFont                 =0x000A; //V1+
    const int16u DefineText                 =0x000B; //V1+
    const int16u DoAction                   =0x000C; //V3+
    const int16u DefineFontInfo             =0x000D; //V1+
    const int16u DefineSound                =0x000E; //V1+
    const int16u StartSound                 =0x000F; //V1+
    const int16u DefineButtonSound          =0x0011; //V2+
    const int16u SoundStreamHead            =0x0012; //V1+
    const int16u SoundStreamBlock           =0x0013; //V1+
    const int16u DefineBitsLossless         =0x0014; //V2+
    const int16u DefineBitsJPEG2            =0x0015; //V2+
    const int16u DefineShape2               =0x0016; //V2+
    const int16u DefineCxform               =0x0017; //V2+
    const int16u Protected                  =0x0018; //V2+
    const int16u PlaceObject2               =0x001A; //V3+
    const int16u RemoveObject2              =0x001C; //V3+
    const int16u DefineShape3               =0x0020; //V3+
    const int16u DefineText2                =0x0021; //V3+
    const int16u DefineButton2              =0x0022; //V3+
    const int16u DefineBitsJPEG3            =0x0023; //V3+
    const int16u DefineBitsLossless2        =0x0024; //V3+
    const int16u DefineEditText             =0x0025; //V4+
    const int16u DefineSprite               =0x0027; //V3+
    const int16u FrameLabel                 =0x002B; //V3+
    const int16u DefineMorphShape           =0x002E; //V3+
    const int16u SoundStreamHead2           =0x002D; //V3+
    const int16u DefineFont2                =0x0030; //V3+
    const int16u ExportAssets               =0x0038; //V5+
    const int16u ImportAssets               =0x0039; //V5+
    const int16u EnableDebugger             =0x003A; //V5
    const int16u DoInitAction               =0x003B; //V6+
    const int16u DefineVideoStream          =0x003C; //V6+
    const int16u DefineVideoFrame           =0x003D; //V6+
    const int16u DefineFontInfo2            =0x003E; //V6+
    const int16u EnableDebugger2            =0x0040; //V6+
    const int16u ScriptLimits               =0x0041; //V6+
    const int16u SetTabIndex                =0x0042; //V7+
    const int16u FileAttributes             =0x0045; //V1+
    const int16u PlaceObject3               =0x0046; //V8+
    const int16u ImportAssets2              =0x0047; //V8+
    const int16u DefineFontAlignZones       =0x0049; //V8+
    const int16u CSMTextSettings            =0x004A; //V8+
    const int16u DefineFont3                =0x004B; //V8+
    const int16u Metadata                   =0x004D; //V1+
    const int16u DefineScalingGrid          =0x004E; //V8+
    const int16u DefineShape4               =0x0053; //V8+
    const int16u DefineMorphShape2          =0x0054; //V8+
}

} //NameSpace

#endif

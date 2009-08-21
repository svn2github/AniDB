// File_Dvdv - Info for DVD objects (IFO) files
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
// Information about DVD objects
// (.ifo files on DVD-Video)
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Mainly from http://dvd.sourceforge.net/dvdinfo/ifo.html
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
#if defined(MEDIAINFO_DVDV_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Dvdv.h"
#include <ZenLib/Utils.h>
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char*  IFO_VTS_Category[]=
{
    "Normal",
    "Karaoke",
};

const char*  IFO_CodecV[]=
{
    "MPEG1V",
    "MPEG2V",
    "",
    "",
};

const char*  IFO_Standard[]=
{
    "NTSC",
    "PAL",
    "",
    "",
};

const char*  IFO_AspectRatio[]=
{
    "1.333",
    "",
    "",
    "1.778",
};

const char*  IFO_BitRate_Mode[]=
{
    "VBR",
    "CBR",
};

const size_t IFO_Width[]=
{720, 704, 352, 352,   0,   0,   0,   0};

const size_t IFO_Height[4][8]=
{{480, 480, 480, 240,   0,   0,   0,   0}, //NTSC
 {576, 576, 576, 288,   0,   0,   0,   0}, //PAL
 {  0,   0,   0,   0,   0,   0,   0,   0}, //Unknown
 {  0,   0,   0,   0,   0,   0,   0,   0}, //Unknown
 };

const size_t IFO_FrameRate[]=
{25, 30};

const char*  IFO_CodecA[]=
{
    "AC3",
    "",
    "MPEG1A",
    "MPEG2A",
    "LPCM",
    "",
    "DTS",
    "SDDS",
};

const char*  IFO_ModeA[]=
{
    "",
    "Karaoke",
    "Surround",
    "",
};

const char*  IFO_ResolutionA[]=
{
    "16",
    "20",
    "24",
    "DRC",
};

const int16u IFO_SamplingRate[]=
{48000, 0, 0, 0, 0, 0, 0, 0};

const char*  IFO_Language_MoreA[]=
{
    "",
    "",
    "For visually impaired",
    "Director's comments",
    "Director's comments",
    "",
    "",
    "",
};

const char*  IFO_CodecT[]=
{
    "2-bit RLE",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

const char*  IFO_Language_MoreT[]=
{
    "",
    "",
    "Large",
    "Children",
    "",
    "",
    "Large",
    "Children",
    "",
    "Forced",
    "",
    "",
    "",
    "Director comments",
    "Director comments large",
    "Director comments children",
};

const size_t IFO_PlaybackTime_FrameRate[]=
{1, 25, 1, 30};

const char*  IFO_MenuType[]=
{
    "",
    "",
    "",
    "root",
    "sub-picture",
    "audio",
    "angle",
    "PTT (chapter)",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
extern const char*  AC3_ChannelPositions[];

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dvdv::Read_Buffer_Init()
{
    //Temp
    VTS_Attributes_AreHere=false;
    Program_Pos=0;
    Time_Pos=0;

    File__Analyze::Read_Buffer_Init();
}

//---------------------------------------------------------------------------
void File_Dvdv::Read_Buffer_Continue()
{
    //Allways at least 1 sector
    if (Buffer_Size<2048)
        return;

    //Sector 0
    if (File_Offset==0)
    {
        if (CC8(Buffer)!=CC8("DVDVIDEO"))
        {
            File_Offset=File_Size;
            return;
        }

        Element_Begin();
        Element[Element_Level].IsComplete=true;
        switch (CC4(Buffer+8))
        {
            case Dvdv::VMG : VMG(); break;
            case Dvdv::VTS : VTS(); break;
            default         :
                        File_Offset=File_Size;
                        return;
        }
        Element_End("Header", 2048);
        Buffer_Offset+=2048; //Size of first block
    }

    //We must have the whole file
    if (Sectors.size()<File_Size/2048+(File_Size%2048==0?0:1))
    {
        File_Offset==File_Size;
        return; //Finnished
    }
    if (File_Offset+Buffer_Size<File_Size)
        return;

    File__Analyze::Read_Buffer_Continue();
}

//***************************************************************************
// Elements
//***************************************************************************

#define SUBELEMENT(_BYTES, _ELEMENT) \
    { \
        Element_Begin(#_ELEMENT, _BYTES); \
        _ELEMENT(); \
        Element_End(); \
    } \

//---------------------------------------------------------------------------
void File_Dvdv::VMG()
{
    //Filling
    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        General[0](_T("Format"))=_T("DVD Video");
        General[0](_T("Format/String"))=_T("DVD Video (Menu)");
        General[0](_T("Format/Extensions"))=_T("IFO");
    FILLING_END();

    //Parsing
    if (Buffer_Size<0x200)
        return;
    Element_Size=0x200;
    int32u Sector_Pointer_LastSector, Sector_Pointer_TT_SRPT, Sector_Pointer_VMGM_PGCI_UT, Sector_Pointer_VMG_PTL_MAIT, Sector_Pointer_VMG_VTS_ATRT, Sector_Pointer_VMG_TXTDT_MG, Sector_Pointer_VMGM_C_ADT, Sector_Pointer_VMGM_VOBU_ADMAP;
    int16u Version, Audio_Count, Text_Count;
    Element_Begin("Header", 0x100);
        SKIP_LOCAL(12,                                          "Header");
        INFO_B4(LastSector,                                     "Last sector of VMG set (last sector of BUP)"); Param_Info((LastSector+1)*2048, " bytes");
        SKIP_XX(12,                                             "Unknown");
        GET_B4 (Sector_Pointer_LastSector,                      "last sector of IFO");
        GET_B2 (Version,                                        "version number"); Param_Info(Ztring::ToZtring((Version&0x00F0)>>4)+_T(".")+Ztring::ToZtring(Version&0x000F));
        if (Version>0x001F)
            return;
        INFO_B4(Category,                                       "VMG category");
        SKIP_B2(                                                "number of volumes");
        SKIP_B2(                                                "volume number");
        SKIP_B1(                                                "side ID");
        SKIP_XX(19,                                             "Unknown");
        SKIP_B2(                                                "number of title sets");
        SKIP_LOCAL(32,                                          "Provider ID");
        SKIP_B8(                                                "VMG POS");
        SKIP_XX(24,                                             "Unknown");
        SKIP_B4(                                                "end byte address of VMGI_MAT");
        SKIP_B4(                                                "start address of FP_PGC (First Play program chain)");
        SKIP_XX(56,                                             "Unknown");
        INFO_B4(Sector_Pointer_Menu,                            "start sector of Menu VOB");
        GET_B4 (Sector_Pointer_TT_SRPT,                         "sector pointer to TT_SRPT (table of titles)");
        GET_B4 (Sector_Pointer_VMGM_PGCI_UT,                    "sector pointer to VMGM_PGCI_UT (Menu Program Chain table)");
        GET_B4 (Sector_Pointer_VMG_PTL_MAIT,                    "sector pointer to VMG_PTL_MAIT (Parental Management masks)");
        GET_B4 (Sector_Pointer_VMG_VTS_ATRT,                    "sector pointer to VMG_VTS_ATRT (copies of VTS audio/sub-picture attributes)");
        GET_B4 (Sector_Pointer_VMG_TXTDT_MG,                    "sector pointer to VMG_TXTDT_MG (text data)");
        GET_B4 (Sector_Pointer_VMGM_C_ADT,                      "sector pointer to VMGM_C_ADT (menu cell address table)");
        GET_B4 (Sector_Pointer_VMGM_VOBU_ADMAP,                 "sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map)");
        SKIP_XX(32,                                             "Unknown");
    Element_End();

    //-VTSM
    VTS_Attributes_AreHere=true; //now we have what we are looking for
    Element_Begin("VMGM (VMG for Menu)", 0x100);
        Element_Begin("Video streams", 1*2);
            Element_Info(1, " streams");
            SUBELEMENT(2, Video)
        Element_End();
        Element_Begin("Audio streams", 8*8);
            GET_B2 (Audio_Count,                                "number of audio streams in VMGM_VOBS");
            Element_Info(Audio_Count, " streams");
            for (int16u Pos=0; Pos<8; Pos++)
            {
                if (Pos<Audio_Count)
                    SUBELEMENT(8, Audio)
                else
                    SKIP_XX(8,                                  "Reserved for Audio");
            }
            SKIP_XX(16,                                         "Unknown");
        Element_End();
        Element_Begin("Text streams", 1*6);
            GET_B2 (Text_Count,                                 "number of subpicture streams in VMGM_VOBS");
            Element_Info(Text_Count, " streams");
            for (int16u Pos=0; Pos<1; Pos++)
            {
                if (Pos<Text_Count)
                    SUBELEMENT(6, Text)
                else
                    SKIP_XX(6,                                  "Reserved for Text");
            }
            SKIP_XX(164,                                        "Unknown");
        Element_End();
    Element_End();

    Element_Info("DVD Video - VMG");
    Element_Size=Element_Pos;

    //Filling
    FILLING_BEGIN();
        Sectors.resize(Sector_Pointer_LastSector+1);
        if (Sector_Pointer_TT_SRPT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_TT_SRPT]=Sector_TT_SRPT;
        if (Sector_Pointer_VMGM_PGCI_UT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMGM_PGCI_UT]=Sector_VMGM_PGCI_UT;
        if (Sector_Pointer_VMG_PTL_MAIT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMG_PTL_MAIT]=Sector_VMG_PTL_MAIT;
        if (Sector_Pointer_VMG_VTS_ATRT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMG_VTS_ATRT]=Sector_VMG_VTS_ATRT;
        if (Sector_Pointer_VMG_TXTDT_MG<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMG_TXTDT_MG]=Sector_VMG_TXTDT_MG;
        if (Sector_Pointer_VMGM_C_ADT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMGM_C_ADT]=Sector_VMGM_C_ADT;
        if (Sector_Pointer_VMGM_VOBU_ADMAP<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VMGM_VOBU_ADMAP]=Sector_VMGM_VOBU_ADMAP;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dvdv::VTS()
{
    //Filling
    FILLING_BEGIN();
        Stream_Prepare(Stream_General);
        General[0](_T("Format"))=_T("DVD Video");
        General[0](_T("Format/String"))=_T("DVD Video");
        General[0](_T("Format/Extensions"))=_T("IFO");
    FILLING_END();

    //Parsing
    if (Buffer_Size<0x3D8)
        return;
    //Element_Size=0x3D8;
    int32u Sector_Pointer_LastSector, Sector_Pointer_VTS_PTT_SRPT, Sector_Pointer_VTS_PGCI, Sector_Pointer_VTSM_PGCI_UT, Sector_Pointer_VTS_TMAPTI, Sector_Pointer_VTSM_C_ADT, Sector_Pointer_VTSM_VOBU_ADMAP, Sector_Pointer_VTS_C_ADT, Sector_Pointer_VTS_VOBU_ADMAP;
    int16u Version, Audio_Count, Text_Count, MultiChannel_Count;
    Element_Begin("Header", 0x100);
        SKIP_LOCAL(12,                                          "Header");
        INFO_B4(LastSector,                                     "Last sector of Title set (last sector of BUP)"); Param_Info((LastSector+1)*2048, " bytes");
        SKIP_XX(12,                                             "Unknown");
        GET_B4 (Sector_Pointer_LastSector,                      "last sector of IFO");
        GET_B2 (Version,                                        "version number"); Param_Info(Ztring::ToZtring((Version&0x00F0)>>4)+_T(".")+Ztring::ToZtring(Version&0x000F));
        if (Version>0x001F)
            return;
        INFO_B4(Category,                                       "VTS category"); if (Category<2) Param_Info(IFO_VTS_Category[Category]);
        SKIP_XX(90,                                             "Unknown");
        SKIP_B4(                                                "end byte address of VTS_MAT");
        SKIP_XX(60,                                             "Unknown");
        INFO_B4(StartSector_Menu,                               "start sector of Menu VOB"); Param_Info((StartSector_Menu+1)*2048, " bytes");
        INFO_B4(StartSector_Title,                              "start sector of Title Vob"); Param_Info((StartSector_Title+1)*2048, " bytes");
        GET_B4 (Sector_Pointer_VTS_PTT_SRPT,                    "sector pointer to VTS_PTT_SRPT (Table of Titles and Chapters)");
        GET_B4 (Sector_Pointer_VTS_PGCI,                        "sector pointer to VTS_PGCI (Title Program Chain table)");
        GET_B4 (Sector_Pointer_VTSM_PGCI_UT,                    "sector pointer to VTSM_PGCI_UT (Menu Program Chain table)");
        GET_B4 (Sector_Pointer_VTS_TMAPTI,                      "sector pointer to VTS_TMAPTI (Time map)");
        GET_B4 (Sector_Pointer_VTSM_C_ADT,                      "sector pointer to VTSM_C_ADT (Menu cell address table)");
        GET_B4 (Sector_Pointer_VTSM_VOBU_ADMAP,                 "sector pointer to VTSM_VOBU_ADMAP(menu VOBU address map)");
        GET_B4 (Sector_Pointer_VTS_C_ADT,                       "sector pointer to VTS_C_ADT (Title set cell address table)");
        GET_B4 (Sector_Pointer_VTS_VOBU_ADMAP,                  "sector pointer to VTS_VOBU_ADMAP (Title set VOBU address map)");
        SKIP_XX(24,                                             "Unknown");
    Element_End();

    //-VTSM
    Element_Begin("VTSM (VTS for Menu, Vob 0)", 0x100);
        Element_Begin("Video streams", 1*2);
            Element_Info(1, " streams");
            SUBELEMENT(2, Video)
        Element_End();
        Element_Begin("Audio streams", 8*8);
            GET_B2 (Audio_Count,                                "number of audio streams in VTSM_VOBS");
            Element_Info(Audio_Count, " streams");
            for (int16u Pos=0; Pos<8; Pos++)
            {
                if (Pos<Audio_Count)
                    SUBELEMENT(8, Audio)
                else
                    SKIP_XX(8,                                  "Reserved for Audio");
            }
            SKIP_XX(16,                                         "Unknown");
        Element_End();
        Element_Begin("Text streams", 1*6);
            GET_B2 (Text_Count,                                 "number of subpicture streams in VTSM_VOBS");
            Element_Info(Text_Count, " streams");
            for (int16u Pos=0; Pos<1; Pos++)
            {
                if (Pos<Text_Count)
                    SUBELEMENT(6, Text)
                else
                    SKIP_XX(6,                                  "Reserved for Text");
            }
            SKIP_XX(164,                                        "Unknown");
        Element_End();
    Element_End();

    //-VTS
    FLUSH();
    VTS_Attributes_AreHere=true; //now we have what we are looking for
    Element_Begin("VTS (VTS for movie, Vob 1-9)", 0x1D8);
        Element_Begin("Video streams", 1*2);
            Element_Info(1, " streams");
            SUBELEMENT(2, Video)
        Element_End();
        Element_Begin("Audio streams", 8*8);
            GET_B2 (Audio_Count,                                "number of audio streams in VMGM_VOBS");
            Element_Info(Audio_Count, " streams");
            for (int16u Pos=0; Pos<8; Pos++)
            {
                if (Pos<Audio_Count)
                    SUBELEMENT(8, Audio)
                else
                    SKIP_XX(8,                                  "Reserved for Audio");
            }
            SKIP_XX(16,                                         "Unknown");
        Element_End();
        Element_Begin("Text streams", 32*6);
            GET_B2 (Text_Count,                                 "number of subpicture streams in VMGM_VOBS");
            Element_Info(Text_Count, " streams");
            for (int16u Pos=0; Pos<32; Pos++)
            {
                if (Pos<Text_Count)
                    SUBELEMENT(6, Text)
                else
                    SKIP_XX(6,                                  "Reserved for Text");
            }
            SKIP_XX(2,                                          "Unknown");
        Element_End();
        Element_Begin("MultiChannel Info", 8*24);
            Element_Info(Audio_Count, " streams");
            for (int16u Pos=0; Pos<8; Pos++)
            {
                if (Pos<Audio_Count)
                    SUBELEMENT(24, MultiChannel)
                else
                    SKIP_XX(24,                                 "Reserved for multichannel extension");
            }
        Element_End();
    Element_End();

    Element_Info("DVD Video - VTS (Video Title Set)");
    Element_Size=Element_Pos;

    //Filling
    FILLING_BEGIN();
        Sectors.resize(Sector_Pointer_LastSector+1);
        if (Sector_Pointer_VTS_PTT_SRPT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTS_PTT_SRPT]=Sector_VTS_PTT_SRPT;
        if (Sector_Pointer_VTS_PGCI<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTS_PGCI]=Sector_VTS_PGCI;
        if (Sector_Pointer_VTSM_PGCI_UT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTSM_PGCI_UT]=Sector_VTSM_PGCI_UT;
        if (Sector_Pointer_VTS_TMAPTI<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTS_TMAPTI]=Sector_VTS_TMAPTI;
        if (Sector_Pointer_VTSM_C_ADT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTSM_C_ADT]=Sector_VTSM_C_ADT;
        if (Sector_Pointer_VTSM_VOBU_ADMAP<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTSM_VOBU_ADMAP]=Sector_VTSM_VOBU_ADMAP;
        if (Sector_Pointer_VTS_C_ADT<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTS_C_ADT]=Sector_VTS_C_ADT;
        if (Sector_Pointer_VTS_VOBU_ADMAP<=Sector_Pointer_LastSector)
            Sectors[Sector_Pointer_VTS_VOBU_ADMAP]=Sector_VTS_VOBU_ADMAP;
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dvdv::Video()
{
    //Parsing
    int32u Codec, Standard, AspectRatio, Resolution, BitRate_Mode;
    CC_BS_BEGIN();
    GET_BS (2, Codec,                                           "Coding mode"); Param_Info(IFO_CodecV[Codec]);
    GET_BS (2, Standard,                                        "Standard"); Param_Info(IFO_Standard[Standard]);
    GET_BS (2, AspectRatio,                                     "Aspect ratio"); Param_Info(IFO_AspectRatio[AspectRatio]);
    INFO_BS(1, Pan,                                             "Automatic Pan/Scan"); Param_Info(Pan?"No":"Yes");
    INFO_BS(1, Letter,                                          "Automatic Letterbox"); Param_Info(Letter?"No":"Yes");
    SKIP_BS(1,                                                  "CC for line 21 field 1 in GOP (NTSC only)");
    SKIP_BS(1,                                                  "CC for line 21 field 2 in GOP (NTSC only)");
    GET_BS (3, Resolution,                                      "Resolution"); Param_Info(Ztring::ToZtring(IFO_Width[Resolution])+_T("x")+Ztring::ToZtring(IFO_Height[Standard][Resolution]));
    INFO_BS(1, Letterboxed,                                     "Letterboxed"); Param_Info(Letter?"Yes":"No");
    GET_BS (1, BitRate_Mode,                                    "Bitrate mode"); Param_Info(IFO_BitRate_Mode[BitRate_Mode]);
    INFO_BS(1, Camera,                                          "Camera/Film"); Param_Info(Letter?"Film":"Camera");
    CC_BS_END();

    //Filling
    FILLING_BEGIN();
        if (VTS_Attributes_AreHere)
        {
            Stream_Prepare(Stream_Video);
            Fill("Codec", IFO_CodecV[Codec]);
            Fill("AspectRatio", IFO_AspectRatio[AspectRatio]);
            Fill("Width", IFO_Width[Resolution]);
            Fill("Height", IFO_Height[Standard][Resolution]);
            Fill("FrameRate", IFO_FrameRate[Standard]);
            Fill("BitRate_Mode", IFO_BitRate_Mode[BitRate_Mode]);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dvdv::Audio()
{
    //Parsing
    Ztring Language;
    int32u Codec, LanguageType, Mode, Resolution, SamplingRate, Channels;
    int8u Language_Extension;
    CC_BS_BEGIN();
    GET_BS (3, Codec,                                           "Coding mode"); Param_Info(IFO_CodecA[Codec]);
    INFO_BS(1, MultiChannel,                                    "Multichannel extension present"); Param_Info(MultiChannel?"Yes":"No");
    GET_BS (2, LanguageType,                                    "Language type"); Param_Info(LanguageType==1?"2CC":"Unknown");
    GET_BS (2, Mode,                                            "Application mode"); Param_Info(IFO_ModeA[Mode]);
    GET_BS (2, Resolution,                                      "Resolution"); if (Codec==2 || Codec==3) Param_Info(IFO_ResolutionA[Resolution]); else if (Codec==4) Param_Info(Mode?"DRC":"No DRC");
    GET_BS (2, SamplingRate,                                    "Sampling rate"); Param_Info(Ztring::ToZtring(IFO_SamplingRate[SamplingRate]));
    GET_BS (4, Channels,                                        "Channels"); Param_Info(Channels+1, " channels");
    CC_BS_END();
    GET_LOCAL(3, Language,                                      "Language code");
    GET_B1 (Language_Extension,                                 "Language extension"); if (Language_Extension<8) Param_Info(IFO_Language_MoreA[Language_Extension]);
    SKIP_B1(                                                    "Unknown");
    switch (Mode)
    {
        case 1 : //Karaoke
            {
            CC_BS_BEGIN();
            SKIP_BS(1,                                          "Zero");
            INFO_BS(3, ChannelsK,                               "Channels");
                #ifdef MEDIAINFO_AC3_YES
                    Param_Info(AC3_ChannelPositions[ChannelsK]);
                #endif //MEDIAINFO_AC3_YES
            SKIP_BS(2,                                          "Version");
            INFO_BS(1, MC,                                      "MC intro present"); Param_Info(MC?"Yes":"No");
            INFO_BS(1, Duet,                                    "Duet"); Param_Info(Duet?"Duet":"Solo");
            CC_BS_END();
            }
            break;
        case 2 : //Surround
            {
            CC_BS_BEGIN();
            SKIP_BS(4,                                          "Reserved");
            INFO_BS(1, DolbyDecode,                             "Suitable for Dolby surround decoding"); Param_Info(DolbyDecode?"Yes":"No");
            SKIP_BS(3,                                          "Reserved");
            CC_BS_END();
            }
            break;
    default:
            {
            SKIP_B1(                                            "Reserved");
            }
    }

    //Filling
    FILLING_BEGIN();
        if (VTS_Attributes_AreHere)
        {
            Stream_Prepare(Stream_Audio);
            Fill("Codec", IFO_CodecA[Codec]);
            Fill("SamplingRate", IFO_SamplingRate[SamplingRate]);
            Fill("Channel(s)", Channels+1);
            if (Codec==2 || Codec==3)
                Fill("Resolution", IFO_ResolutionA[Resolution]);
            else if (Codec==4 && Mode)
                Fill("Resolution", "DRC");
            Fill("Language", Language);
            if (Language_Extension<8)
                Fill("Language_More", IFO_Language_MoreA[Language_Extension]);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dvdv::Text()
{
    //Parsing
    Ztring Language;
    int32u Codec, LanguageType;
    int8u Language_Extension;
    CC_BS_BEGIN();
    GET_BS (3, Codec,                                           "Coding mode"); Param_Info(IFO_CodecT[Codec]);
    SKIP_BS(3,                                                  "Reserved");
    GET_BS (2, LanguageType,                                    "Language type"); Param_Info(LanguageType==1?"2CC":"Unknown");
    CC_BS_END();
    SKIP_B1(                                                    "Reserved");
    GET_LOCAL(3, Language,                                      "Language code");
    GET_B1 (Language_Extension,                                 "Language extension"); if (Language_Extension<16) Param_Info(IFO_Language_MoreT[Language_Extension]);

    //Filling
    FILLING_BEGIN();
        if (VTS_Attributes_AreHere)
        {
            Stream_Prepare(Stream_Text);
            Fill("Codec", IFO_CodecT[Codec]);
            Fill("Language", Language);
            if (Language_Extension<16)
                Fill("Language_More", IFO_Language_MoreT[Language_Extension]);
        }
    FILLING_END();
}

//---------------------------------------------------------------------------
void File_Dvdv::MultiChannel()
{
    //Parsing
    CC_BS_BEGIN();
    Element_Begin("ACH0", 1);
        SKIP_BS(7,                                              "Reserved");
        SKIP_BS(1,                                              "ACH0 Guide Melody exists");
    Element_End();
    Element_Begin("ACH1", 1);
        SKIP_BS(7,                                              "Reserved");
        SKIP_BS(1,                                              "ACH1 Guide Melody exists");
    Element_End();
    Element_Begin("ACH2", 1);
        SKIP_BS(4,                                              "Reserved");
        SKIP_BS(1,                                              "ACH2 Guide Vocal 1 exists");
        SKIP_BS(1,                                              "ACH2 Guide Vocal 2 exists");
        SKIP_BS(1,                                              "ACH2 Guide Melody 1 exists");
        SKIP_BS(1,                                              "ACH2 Guide Melody 2 exists");
    Element_End();
    Element_Begin("ACH3", 1);
        SKIP_BS(4,                                              "Reserved");
        SKIP_BS(1,                                              "ACH3 Guide Vocal 1 exists");
        SKIP_BS(1,                                              "ACH3 Guide Vocal 2 exists");
        SKIP_BS(1,                                              "ACH3 Guide Melody A exists");
        SKIP_BS(1,                                              "ACH3 Sound Effect A exists");
    Element_End();
    Element_Begin("ACH4", 1);
        SKIP_BS(4,                                              "Reserved");
        SKIP_BS(1,                                              "ACH4 Guide Vocal 1 exists");
        SKIP_BS(1,                                              "ACH4 Guide Vocal 2 exists");
        SKIP_BS(1,                                              "ACH4 Guide Melody B exists");
        SKIP_BS(1,                                              "ACH4 Sound Effect B exists");
    Element_End();
    CC_BS_END();
    SKIP_XX(19,                                                 "Unknown");
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dvdv::Header_Parse()
{
    //Calculating
    size_t Sector_Pos=(size_t)((File_Offset+Buffer_Offset)/2048);
    size_t Sector_Count=1;
    while (Sector_Pos+Sector_Count<Sectors.size() && Sectors[Sector_Pos+Sector_Count]==Sector_Nothing)
        Sector_Count++;

    //Filling
    Header_Fill_Size(Sector_Count*2048);
}

//---------------------------------------------------------------------------
bool File_Dvdv::Data_Parse()
{
    //Parsing
    size_t Sector_Pos=(size_t)((File_Offset+Buffer_Offset)/2048);
    Element[Element_Level].IsComplete=true;

    //Parsing
    switch(Sectors[Sector_Pos])
    {
        case Sector_VTS_PTT_SRPT    : VTS_PTT_SRPT(); break;
        case Sector_VTS_PGCI        : VTS_PGCI(); break;
        case Sector_VTSM_PGCI_UT    : VTSM_PGCI_UT(); break;
        case Sector_VTS_TMAPTI      : VTS_TMAPTI(); break;
        case Sector_VTSM_C_ADT      : VTSM_C_ADT(); break;
        case Sector_VTSM_VOBU_ADMAP : VTSM_VOBU_ADMAP(); break;
        case Sector_VTS_C_ADT       : VTS_C_ADT(); break;
        case Sector_VTS_VOBU_ADMAP  : VTS_VOBU_ADMAP(); break;
        case Sector_TT_SRPT         : TT_SRPT(); break;
        case Sector_VMGM_PGCI_UT    : VMGM_PGCI_UT(); break;
        case Sector_VMG_PTL_MAIT    : VMG_PTL_MAIT(); break;
        case Sector_VMG_VTS_ATRT    : VMG_VTS_ATRT(); break;
        case Sector_VMG_TXTDT_MG    : VMG_TXTDT_MG(); break;
        case Sector_VMGM_C_ADT      : VMGM_C_ADT(); break;
        case Sector_VMGM_VOBU_ADMAP : VMGM_VOBU_ADMAP(); break;
        default                     : ;
    }

    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dvdv::VTS_PTT_SRPT ()
{
    Element_Name("Table of Titles and Chapters");

    //Parsing
    int32u Element_RealSize;
    Element_Begin("Header", 8);
        SKIP_B2(                                                "Count of elements");
        SKIP_B2(                                                "Unknown");
        GET_B4 (Element_RealSize,                               "End address");
        Element_RealSize++; //Last byte
    Element_End();
    Element_Begin("Extra data");
        int32u Offset;
        GET_B4 (Offset,                                         "Offset of first element");
        int32u Extra_Size=Offset-Element_Pos;
        if (Extra_Size>0)
            SKIP_XX(Extra_Size,                                 "Extra data (Unknown)");
    Element_End(Extra_Size);

    //For each chapter
    while (Element_Pos<Element_RealSize)
    {
        //VTS_PTT
        int16u PGCN, PGN;
        Element_Begin(4);
        GET_B2 (PGCN,                                           "Program Chain (PGCN)");
        GET_B2 (PGN,                                            "Program (PGN)");
        Element_Name("Chapter"); Element_Info(Ztring::ToZtring(PGCN)); Element_Info(Ztring::ToZtring(PGN));
        Element_End();
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTS_PGCI ()
{
    Element_Name("Title Program Chain table");

    //Parsing
    Element_Begin("Header");
    int32u Offset;
    SKIP_B2(                                                    "Number of Program Chains");
    SKIP_B2(                                                    "Reserved");
    int32u Element_Size_;
    GET_B4 (Element_Size_,                                       "End address");
    Element_Size=Element_Size_+1; //Last byte
    Element_Begin("PGC category", 4);
        CC_BS_BEGIN();
        SKIP_BS(1,                                              "entry PGC");
        SKIP_BS(7,                                              "title number");
        CC_BS_END();
        SKIP_B1(                                                "Unknown");
        SKIP_B2(                                                "parental management mask");
    Element_End();
    GET_B4 (Offset,                                             "offset to VTS_PGC - relative to VTS_PGCI");
    if (Offset-16>0)
        SKIP_XX(Offset-16,                                      "Unknown");
    Element_End(Offset);

    //For each Program
    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        PGC(Offset);
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTSM_PGCI_UT ()
{
    Element_Name("Menu Program Chain table");

    //Parsing
    int16u LU_Count;
    Element_Begin("Header");
        int32u Offset;
        int8u Flags;
        GET_B2 (LU_Count,                                       "Number of Language Units");
        SKIP_B2(                                                "Reserved");
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
        SKIP_C3(                                                "Language");
        GET_B1 (Flags,                                          "Menu existence flags");
            SKIP_FLAGS(Flags, 3,                                "PTT");
            SKIP_FLAGS(Flags, 4,                                "angle");
            SKIP_FLAGS(Flags, 5,                                "audio");
            SKIP_FLAGS(Flags, 6,                                "sub-picture");
            SKIP_FLAGS(Flags, 7,                                "root");
        GET_B4 (Offset,                                         "Offset to VTSM_LU relative to VTSM_PGCI_UT");
        if (Offset-16>0)
            SKIP_XX(Offset-16,                                  "Unknown");
    Element_End(Offset);

    for (int16u LU_Pos=0; LU_Pos<LU_Count; LU_Pos++)
    {
        Element_Begin("Language Unit");
        int32u LU_Size;
        int16u PGC_Count;
        Element_Begin("Header");
            GET_B2 (PGC_Count,                                  "Number of Program Chains");
            SKIP_B2(                                            "Reserved");
            GET_B4 (LU_Size,                                    "end address (last byte of last PGC in this LU) relative to VTSM_LU");
            LU_Size++; //Last byte
            Element_Begin("PGC category", 4);
                int32u EntryPGC;
                CC_BS_BEGIN();
                GET_BS (1, EntryPGC,                            "Entry PGC");
                SKIP_BS(3,                                      "Unknown");
                if (EntryPGC)
                {
                    INFO_BS(4, MenuType,                        "menu type"); Param_Info(IFO_MenuType[MenuType]);
                }
                else
                {
                    SKIP_BS(4,                                  "Reserved");
                }
                CC_BS_END();
                SKIP_B1(                                        "Unknown");
                SKIP_B2(                                        "parental management mask");
            Element_End();
            GET_B4 (Offset,                                     "offset to VTSM_PGC relative to VTSM_LU");
            if (Offset-16>0)
                SKIP_XX(Offset-16,                              "Unknown");
        Element_End();
        for (int16u PGC_Pos=0; PGC_Pos<PGC_Count; PGC_Pos++)
            PGC(Element_Pos);    

        Element_End();
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTS_TMAPTI ()
{
    Element_Name("Time map");

    //Parsing
    Element_Begin("Header", 8);
        int32u Offset;
        SKIP_B2(                                                "Number of program chains");
        SKIP_B2(                                                "Reserved");
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
        GET_B4 (Offset,                                         "Offset to VTS_TMAP 1");
        if (Offset-12>0)
            SKIP_XX(Offset-12,                                  "Unknown");
    Element_End();

    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        //VTS_TMAP
        Element_Begin("Time Map");
        //std::vector<size_t> Sector_Times;
        int8u Sector_Times_SecondsPerTime;
        int16u Count;
        GET_B1 (Sector_Times_SecondsPerTime,                    "Time unit (seconds)");
        SKIP_B1(                                                "Unknown");
        GET_B2 (Count,                                          "Number of entries in map");
        //Sector_Times.resize(Count);
        CC_BS_BEGIN();
        for (int16u Pos=0; Pos<Count; Pos++)
        {
            Element_Begin("Sector Offset", 4);
            int32u SectorOffset;
            SKIP_BS( 1,                                         "discontinuous with previous");
            GET_BS (31, SectorOffset,                           "Sector offset within VOBS of nearest VOBU");
            //GET_B4 (Sector_Times[Pos],                          Sector offset within VOBS of nearest VOBU);// Param_Info(Ztring().Duration_From_Milliseconds((Pos+1)*Sectors_Times_SecondsPerTime[Program_Pos]));
            //Sector_Times[Pos]&=0x7FFFFFFF; //bit 31 is set if VOBU time codes are discontinuous with previous
            Element_Info(SectorOffset);
            Element_End();
        }
        CC_BS_END();
        Element_End();

        //Filling
        //Sectors_Times.push_back(Sector_Times);
        //Sectors_Times_SecondsPerTime.push_back(Sector_Times_SecondsPerTime);
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTSM_C_ADT ()
{
    Element_Name("Menu cell address table");

    //Parsing
    Element_Begin("Header", 8);
        SKIP_B2(                                                "Number of cells");
        SKIP_B2(                                                "Reserved");
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
    Element_End();

    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        //ADT
        Element_Begin("Entry", 12);
        SKIP_B2(                                                "VOBidn");
        SKIP_B1(                                                "CELLidn");
        SKIP_B1(                                                "Unknown");
        SKIP_B4(                                                "Starting sector within VOB");
        SKIP_B4(                                                "Ending sector within VOB");
        Element_End();
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTSM_VOBU_ADMAP ()
{
    Element_Name("Menu VOBU address map");

    //Parsing
    Element_Begin("Header", 4);
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
    Element_End();

    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        //ADMAP
        SKIP_B4(                                                "Starting sector within VOB of first VOBU");
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTS_C_ADT ()
{
    Element_Name("Title set cell address table");

    //Parsing
    Element_Begin("Header", 8);
        SKIP_B2(                                                "Number of cells");
        SKIP_B2(                                                "Reserved");
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
    Element_End();

    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        //ADT
        Element_Begin("Entry", 12);
        int32u Start, End;
        int16u VOBidn;
        int8u CELLidn;
        GET_B2 (VOBidn,                                         "VOBidn");
        GET_B1 (CELLidn,                                        "CELLidn");
        SKIP_B1(                                                "Unknown");
        GET_B4 (Start,                                          "Starting sector within VOB"); Param_Info(Time_ADT(Start));
        GET_B4 (End,                                            "Ending sector within VOB"); Param_Info(Time_ADT(End));
        Element_End();

        //Filling
        FILLING_BEGIN();
            if (Count_Get(Stream_Chapters)<(size_t)VOBidn)
                Stream_Prepare(Stream_Chapters);
        FILLING_END();
        //Fill(Ztring::ToZtring(CELLidn).To_Local().c_str(), Time_ADT(Start));
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VTS_VOBU_ADMAP ()
{
    Element_Name("Title set VOBU address map");

    //Parsing
    Element_Begin("Header", 4);
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
    Element_End();

    DETAILLEVEL_SET(1.0);
    while (Element_Pos<Element_Size)
    {
        //ADMAP
        SKIP_B4(                                                "Starting sector within VOB of first VOBU");
    }
    PURGE();
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
Ztring File_Dvdv::Time_ADT(int32u Value)
{
    return Ztring();
    /*
    if (Sectors_Times.empty())
        return Ztring(); //TODO: it can be empty?

    while (Time_Pos<Sectors_Times[Program_Pos].size() && Sectors_Times[Program_Pos][Time_Pos]<Value)
        Time_Pos++;
    if (Time_Pos<Sectors_Times[Program_Pos].size())
    {
        int32u Time=(Time_Pos+1)*Sectors_Times_SecondsPerTime[Program_Pos]*1000;
        float32 Part;
        //True time is between Time and Time+Sectors_Times_SecondsPerTime, finding where...
        int32u Sectors_Count;
        if (Time_Pos==0)
            Sectors_Count=Sectors_Times[Program_Pos][Time_Pos];
        else
            Sectors_Count=Sectors_Times[Program_Pos][Time_Pos]-Sectors_Times[Program_Pos][Time_Pos-1];
        Part=(Sectors_Times[Program_Pos][Time_Pos]-Value); //Count of more sectors after
        Part/=Sectors_Count; //Percentage
        Time=(int32u)((Time_Pos+1-Part)*Sectors_Times_SecondsPerTime[Program_Pos]*1000);

        return Ztring().Duration_From_Milliseconds(Time*1000);
    }
    else
    {
        int32u Time=(Time_Pos+1)*Sectors_Times_SecondsPerTime[Program_Pos]*1000;
        float32 Part;
        //True time is between Time and Time+Sectors_Times_SecondsPerTime, finding where... but with the last offset diffrence
        int32u Sectors_Count=Sectors_Times[Program_Pos][Time_Pos-1]-Sectors_Times[Program_Pos][Time_Pos-2];
        Part=((int32s)Sectors_Times[Program_Pos][Time_Pos-1])-((int32s)Value); //Count of more sectors after
        Part/=Sectors_Count; //Percentage
        Part+=1; //We were one offset less
        Time=(int32u)((Time_Pos+1-Part)*Sectors_Times_SecondsPerTime[Program_Pos]*1000);

        return Ztring().Duration_From_Milliseconds(Time*1000);
    }
    */
}


void File_Dvdv::Time_BCD(const Ztring &Name)
{
    int32u FrameRate, FF;
    int8u HH, MM, SS;
    Element_Begin(Name, 4);
        GET_B1 (HH,                                     "hh (BCD)");
        GET_B1 (MM,                                     "mm (BCD)");
        GET_B1 (SS,                                     "ss (BCD)");
        CC_BS_BEGIN();
        GET_BS (2, FrameRate,                           "framerate (BCD)"); Param_Info(IFO_PlaybackTime_FrameRate[FrameRate], " fps");
        GET_BS (6, FF,                                  "ff (BCD)");
        CC_BS_END();
        Time_String=Ztring::ToZtring(HH, 16)+_T(":") //BCD
                  + Ztring::ToZtring(MM, 16)+_T(":") //BCD
                  + Ztring::ToZtring(SS, 16)+_T(".") //BCD
                  + Ztring::ToZtring(Ztring::ToZtring(FF, 16).To_int32u()*1000/IFO_PlaybackTime_FrameRate[FrameRate], 10); //BCD
        Element_Info(Time_String);
    Element_End();
}

void File_Dvdv::PGC(int32u Offset)
{
        //VTS_PGC
        Element_Begin("PGC");
        int16u commands, program_map, cell_playback, cell_position;
        int8u Program_Count;
        Element_Begin("Header", 236);
            int32u Flags;
            int8u Cells;
            SKIP_B2(                                            "Unknown");
            GET_B1 (Program_Count,                              "number of programs");
            GET_B1 (Cells,                                      "number of cells");
            Time_BCD("playback time");
            GET_B4 (Flags,                                      "prohibited user ops");
                /*SKIP_FLAGS(Flags,  0,                           "Time play or search");
                SKIP_FLAGS(Flags,  1, PTT play or search);
                SKIP_FLAGS(Flags,  2, Title play);
                SKIP_FLAGS(Flags,  3, Stop);
                SKIP_FLAGS(Flags,  4, GoUp);
                SKIP_FLAGS(Flags,  5, Time or PTT search);
                SKIP_FLAGS(Flags,  6, TopPG or PrevPG search);
                SKIP_FLAGS(Flags,  7, NextPG search);
                SKIP_FLAGS(Flags,  8, Forward scan);
                SKIP_FLAGS(Flags,  9, Backward scan);
                SKIP_FLAGS(Flags, 10, Menu call - Title);
                SKIP_FLAGS(Flags, 11, Menu call - Root);
                SKIP_FLAGS(Flags, 12, Menu call - Subpicture);
                SKIP_FLAGS(Flags, 13, Menu call - Audio);
                SKIP_FLAGS(Flags, 14, Menu call - Angle);
                SKIP_FLAGS(Flags, 15, Menu call - PTT);
                SKIP_FLAGS(Flags, 16, Resume);
                SKIP_FLAGS(Flags, 17, Button select or activate);
                SKIP_FLAGS(Flags, 18, Still off);
                SKIP_FLAGS(Flags, 19, Pause on);
                SKIP_FLAGS(Flags, 20, Audio stream change);
                SKIP_FLAGS(Flags, 21, Subpicture stream change);
                SKIP_FLAGS(Flags, 22, Angle change);
                SKIP_FLAGS(Flags, 23, Karaoke audio mix change);
                SKIP_FLAGS(Flags, 24, Video presentation mode change);
                /*
                SKIP_FLAGS(Flags,  0, Video presentation mode change);
                SKIP_FLAGS(Flags,  1, Karaoke audio mix change);
                SKIP_FLAGS(Flags,  2, Angle change);
                SKIP_FLAGS(Flags,  3, Subpicture stream change);
                SKIP_FLAGS(Flags,  4, Audio stream change);
                SKIP_FLAGS(Flags,  5, Pause on);
                SKIP_FLAGS(Flags,  6, Still off);
                SKIP_FLAGS(Flags,  7, Button select or activate);
                SKIP_FLAGS(Flags,  8, Resume);
                SKIP_FLAGS(Flags,  9, Menu call - PTT);
                SKIP_FLAGS(Flags, 10, Menu call - Angle);
                SKIP_FLAGS(Flags, 11, Menu call - Audio);
                SKIP_FLAGS(Flags, 12, Menu call - Subpicture);
                SKIP_FLAGS(Flags, 13, Menu call - Root);
                SKIP_FLAGS(Flags, 14, Menu call - Title);
                SKIP_FLAGS(Flags, 15, Backward scan);
                SKIP_FLAGS(Flags, 16, Forward scan);
                SKIP_FLAGS(Flags, 17, NextPG search);
                SKIP_FLAGS(Flags, 18, TopPG or PrevPG search);
                SKIP_FLAGS(Flags, 19, Time or PTT search);
                SKIP_FLAGS(Flags, 20, GoUp);
                SKIP_FLAGS(Flags, 21, Stop);
                SKIP_FLAGS(Flags, 22, Title play);
                SKIP_FLAGS(Flags, 23, PTT play or search);
                SKIP_FLAGS(Flags, 24,                               Time play or search);
                */
            Element_Begin("Audio Stream Controls", 8*2);
            for (int Pos=0; Pos<8; Pos++)
            {
                Element_Begin("Audio Stream Control", 2);
                Element_Info(Ztring::ToZtring(Pos));
                SKIP_B1(                                        "Audio Stream Control - bits...");
                SKIP_B1(                                        "Audio Stream Control - reserved");
                Element_End();
            }
            Element_End();
            Element_Begin("Subpicture Stream Controls", 32*4);
            for (int Pos=0; Pos<32; Pos++)
            {
                Element_Begin("Subpicture Stream Control", 4);
                Element_Info(Ztring::ToZtring(Pos));
                SKIP_B1(                                        "Subpicture Stream Control - bits...");
                SKIP_B1(                                        "Subpicture Stream Control - bits...");
                SKIP_B1(                                        "Subpicture Stream Control - bits...");
                SKIP_B1(                                        "Subpicture Stream Control - bits...");
                Element_End();
            }
            Element_End();
            SKIP_B2(                                            "next PGCN");
            SKIP_B2(                                            "previous PGCN");
            SKIP_B2(                                            "goup PGCN");
            SKIP_B1(                                            "PGC still time - 255=infinite");
            SKIP_B1(                                            "PG playback mode");
            Element_Begin("palette", 16*4);
            for (int Pos=0; Pos<16; Pos++)
            {
                SKIP_B4(                                        "palette (0 - Y - Cr - Cb)");
            }
            Element_End();
            GET_B2 (commands,                                   "offset within PGC to commands");
            GET_B2 (program_map,                                "offset within PGC to program map");
            GET_B2 (cell_playback,                              "offset within PGC to cell playback information table");
            GET_B2 (cell_position,                              "offset within PGC to cell position information table");
        Element_End();

        //commands
        if (commands>0)
        {
            if (Element_Pos<Offset+commands)
                SKIP_XX(Offset+commands-Element_Pos,             "Unknown");
            Element_Begin("commands");
            int16u PreCommands_Count, PostCommands_Count, CellCommands_Count, EndAdress;
            GET_B2 (PreCommands_Count,                          "Number of pre commands");
            GET_B2 (PostCommands_Count,                         "Number of post commands");
            GET_B2 (CellCommands_Count,                         "Number of cell commands");
            GET_B2 (EndAdress,                                  "End address relative to command table");
            if (PreCommands_Count>0)
            {
                Element_Begin("Pre commands", PreCommands_Count*8);
                    for (int16u Pos=0; Pos<PreCommands_Count; Pos++)
                    {
                        Element_Begin("Pre command", 8);
                        SKIP_XX(8,                              "Pre command");
                        Element_End();
                    }
                Element_End();
            }
            if (PostCommands_Count>0)
            {
                Element_Begin("Post commands", PreCommands_Count*8);
                    for (int16u Pos=0; Pos<PreCommands_Count; Pos++)
                    {
                        Element_Begin("Post command", 8);
                        SKIP_XX(8,                              "Post command");
                        Element_End();
                    }
                Element_End();
            }
            if (CellCommands_Count>0)
            {
                Element_Begin("Cell commands", PreCommands_Count*8);
                    for (int16u Pos=0; Pos<PreCommands_Count; Pos++)
                    {
                        Element_Begin("Cell command", 8);
                        SKIP_XX(8,                              "Cell command");
                        Element_End();
                    }
                Element_End();
            }
            Element_End(EndAdress+1);
        }

        //program map
        if (program_map>0)
        {
            if (Element_Pos<Offset+program_map)
                SKIP_XX(Offset+program_map-Element_Pos,          "Unknown");
            Element_Begin("program map", Program_Count*1);
            for (int8u Pos=0; Pos<Program_Count; Pos++)
            {
                Element_Begin("Entry", 8);
                SKIP_B1(                                        "Entry cell number");
                Element_End();
            }
            Element_End();
        }

        //cell playback
        if (cell_playback>0)
        {
            if (Element_Pos<Offset+cell_playback)
                SKIP_XX(Offset+cell_playback-Element_Pos,        "Unknown");
            Element_Begin("cell playback", Cells*24);
            for (int8u Pos=0; Pos<Cells; Pos++)
            {
                Element_Begin("cell", 24);
                SKIP_XX(4,                                      "ToDo");
                Time_BCD("Time");
                SKIP_B4(                                        "first VOBU start sector");
                SKIP_B4(                                        "first ILVU end sector");
                SKIP_B4(                                        "last VOBU start sector");
                SKIP_B4(                                        "last VOBU end sector");
                Element_Info(Ztring::ToZtring(Pos)); Element_Info(Time_String);
                Element_End();
            }
            Element_End();
        }

        //cell position
        if (cell_position>0)
        {
            if (Element_Pos<Offset+cell_position)
                SKIP_XX(Offset+cell_position-Element_Pos,        "Unknown");
            Element_Begin("cell position", Cells*4);
            for (int8u Pos=0; Pos<Cells; Pos++)
            {
                Element_Begin("cell", 4);
                SKIP_B2(                                        "VOBid");
                SKIP_B1(                                        "reserved");
                SKIP_B1(                                        "Cell id");
                Element_End();
            }
            Element_End();
        }

        Element_End(Element_Pos-Offset);
}

//---------------------------------------------------------------------------
void File_Dvdv::TT_SRPT()
{
    Element_Name("table of titles");
}

//---------------------------------------------------------------------------
void File_Dvdv::VMGM_PGCI_UT()
{
    Element_Name("Menu Program Chain table");
}

//---------------------------------------------------------------------------
void File_Dvdv::VMG_PTL_MAIT()
{
    Element_Name("Parental Management masks");
}

//---------------------------------------------------------------------------
void File_Dvdv::VMG_VTS_ATRT()
{
    Element_Name("copies of VTS audio/sub-picture attributes");

    //Parsing
    Element_Begin("Header");
        int32u Offset;
        SKIP_B4(                                                "Number of title sets");
        int32u Element_Size_;
        GET_B4 (Element_Size_,                                   "End address");
        Element_Size=Element_Size_+1; //Last byte
        GET_B4 (Offset,                                         "Offset to VTSM_LU relative to VTSM_PGCI_UT");
        if (Offset-12>0)
            SKIP_XX(Offset-12,                                  "Unknown");
    Element_End(Offset);

    while (Element_Pos<Element_Size)
    {
        Element_Begin("VTS_ATRT");
            Element_Begin("Header", 4);
                int32u Size;
                GET_B4 (Size,                                   "End adress");
                Size++; //Last byte
            Element_End();
            Element_Begin("Copy of VTS Category", 4);
                SKIP_B4(                                        "VTS Category");
            Element_End();
            Element_Begin("Copy of VTS attributes", Size-8);
                SKIP_XX(Size-8,                                 "VTS attributes");
            Element_End();
        Element_End(Size);
    }
}

//---------------------------------------------------------------------------
void File_Dvdv::VMG_TXTDT_MG()
{
    Element_Name("text data");
}

//---------------------------------------------------------------------------
void File_Dvdv::VMGM_C_ADT()
{
    Element_Name("menu cell address table");
}

//---------------------------------------------------------------------------
void File_Dvdv::VMGM_VOBU_ADMAP()
{
    Element_Name("menu VOBU address map");
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Dvdv::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("AspectRatio", "R");
            Fill_HowTo("FrameRate", "R");
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Resolution", "R");
            Fill_HowTo("Language", "R");
            Fill_HowTo("Language_More", "R");
            Fill_HowTo("Codec", "R");
            Fill_HowTo("Language", "R");
            Fill_HowTo("Language_More", "R");
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

#endif //MEDIAINFO_DVDV_YES


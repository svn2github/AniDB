// File_Mpc - Info for Musepack files
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
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_MPC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Mpc.h"
#include <ZenLib/BitStream.h>
using namespace std;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char* Mpc_Profile[]=
{
    "no profile",
    "Unstable/Experimental",
    "",
    "",
    "",
    "Below Telephone (q=0)",
    "Below Telephone (q=1)",
    "Telephone (q=2)",
    "Thumb (q=3)",
    "Radio (q=4)",
    "Standard (q=5)",
    "Xtreme (q=6)",
    "Insane (q=7)",
    "BrainDead (q=8)",
    "Above BrainDead (q=9)",
    "Above BrainDead (q=10)",
};

const int16u Mpc_SampleFreq[]=
{
    44100, //CD
    48000, //DAT, DVC, ADR
    37800, //CD-ROM-XA
    32000, //DSR, DAT-LP, DVC-LP
};

const char* Mpc_Link[]=
{
    "Starts or ends with a very low level",
    "Ends loudly",
    "Starts loudly",
    "Starts loudly and ends loudly",
};

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpc::Read_Buffer_Continue()
{
    //Header
    if (File_Offset==0)
    {
        if (Buffer_Size>=4 && CC4(Buffer)==CC4("MPCK"))
        {
            SV8();
            return;
        }

        if (Buffer_Size<21 || CC3(Buffer)!=CC3("MP+"))
        {
            File_Offset=File_Size;
            return;
        }

        SV7();
    }
}

void File_Mpc::SV7()
{
    Element_Size=25;

    //Parsing
    Ztring Encoder;
    int32u Version, FrameCount, Profile, Link, SampleFreq, EncoderVersion;
    int16u TitleGain, AlbumGain;
    CC_BEGIN();
    SKIP_C3(                                                    Signature);
    CC_BS_BEGIN();
    SKIP_BS(4,                                                  PNS);
    GET_BS (4, Version,                                         Version);
    if (Version!=7)
    {
        File_Offset=File_Size;
        return;
    }
    CC_BS_END()
    GET_L4 (FrameCount,                                         FrameCount);
    SKIP_L2(                                                    MaxLevel);
    CC_BS_BEGIN();
    GET_BS (4, Profile,                                         Profile); INFO(Mpc_Profile[Profile]);
    GET_BS (2, Link,                                            Link); INFO(Mpc_Link[Link]);
    GET_BS (2, SampleFreq,                                      SampleFreq); INFO(Mpc_SampleFreq[SampleFreq]);
    CC_BS_END()
    SKIP_L2(                                                    TitlePeak);
    GET_L2 (TitleGain,                                          TitleGain); INFO(((float32)TitleGain-0x8000)/1000, 2, " dB");
    SKIP_L2(                                                    AlbumPeak);
    GET_L2 (AlbumGain,                                          AlbumGain); INFO(((float32)TitleGain-0x8000)/1000, 2, " dB");
    CC_BS_BEGIN();
    SKIP_BS(32,                                                 Gap Info);
    CC_BS_END()
    GET_L1 (EncoderVersion,                                     EncoderVersion);
    Encoder.From_Number(((float)EncoderVersion)/100, 2); if (EncoderVersion%10==0); else if (EncoderVersion%2==0) Encoder+=_T(" Beta"); else if (EncoderVersion%2==1) Encoder+=_T(" Alpha"); INFO(Encoder);
    CC_END();
    
    ELEMENT(0, "Header", 21);
    FLUSH();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "MPC");
    Stream_Prepare(Stream_Audio);
    Fill("SamplingRate", Mpc_SampleFreq[SampleFreq]);
    Fill("SamplingCount", FrameCount*1152);
    Fill("Codec", "SV7");
    Fill("Codec_Settings", Mpc_Profile[Profile]);
    Fill("Encoded_Library", Encoder);
    Fill("Resolution", 16); //MPC support only 16 bits
    Fill("PlayTime", ((int64u)FrameCount)*1152*1000/Mpc_SampleFreq[SampleFreq]);
    Fill("BitRate", (File_Size-25)*8*Mpc_SampleFreq[SampleFreq]/FrameCount/1152);

    File_Offset=File_Size;
}

void File_Mpc::SV8()
{
    Element_Size=4;

    //Parsing
    CC_BEGIN();
    SKIP_C4(                                                    Signature);
    CC_END();

    ELEMENT(0, "Header", 4);
    FLUSH();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "MPC");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "SV8");

    File_Offset=File_Size;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Mpc::HowTo(stream_t StreamKind)
{
        if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("OverallBitRate"), Info_HowTo)=_T("R");
        General[0](_T("PlayTime"), Info_HowTo)=_T("R");
        General[0](_T("Encoded_Library"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Audio)
    {
        Audio[0](_T("Codec"), Info_HowTo)=_T("R");
        Audio[0](_T("BitRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Channel(s)"), Info_HowTo)=_T("R");
        Audio[0](_T("SamplingRate"), Info_HowTo)=_T("R");
        Audio[0](_T("SamplingCount"), Info_HowTo)=_T("R");
        Audio[0](_T("Resolution"), Info_HowTo)=_T("R");
    }
}

} //NameSpace

#endif //MEDIAINFO_MPC_YES


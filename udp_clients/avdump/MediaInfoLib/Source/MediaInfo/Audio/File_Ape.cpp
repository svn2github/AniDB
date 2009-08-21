// File_Ape - Info for Ape files
// Copyright (C) 2003-2007 Jasper van de Gronde, th.v.d.gronde@hccnet.nl
// Copyright (C) 2003-2007 Jerome Martinez, zen@mediaarea.net
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
#if defined(MEDIAINFO_APE_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Ape.h"

using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
int32u Ape_SamplesPerFrame(int16u Version, int16u CompressionLevel)
{
    if (Version>=3950)
        return 73728*4;
    else if (Version>=3900)
        return 73728;
    else if (Version>=3800 && CompressionLevel==4000)
        return 73728;
    else
        return 9216;
}

const char* Ape_Codec_Settings(int16u Setting)
{
    switch (Setting)
    {
        case 1000 : return "fast";
        case 2000 : return "normal";
        case 3000 : return "high";
        case 4000 : return "extra-high";
        case 5000 : return "insane";
        default   : return "";
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ape::Read_Buffer_Continue()
{
    //Header
    if (File_Offset==0)
    {
        if (Buffer_Size<4 || CC4(Buffer)!=CC4("MAC "))
        {
            File_Offset=File_Size;
            return;
        }

        Header();
    }

    //Footer
    if (File_Offset+Buffer_Size==File_Size)
    {
        Footer();
    }
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ape::Header()
{
    if (Buffer_Size-Buffer_Offset<0x100)
    {
        //Not enough place
        Element_Size=File_Offset;
        return;
    }

    ELEMENT(0, "APE header", 0x28);

    //Parsing
    Element_Size=0x100;
    int32u SampleRate=0, TotalFrames=0, FinalFrameSamples=0, SamplesPerFrame=0;
    int16u Version, CompressionLevel=0, Flags=0, Channels=0, Resolution=0, SeekElements;
    bool Resolution8=false, Resolution24=false, no_wav_header;
    CC_BEGIN();
    SKIP_C4(                                                    Identifier);
    GET_L2 (Version,                                            Version);
    if (Version<3980) //<3.98
    {
        GET_L2 (CompressionLevel,                               CompressionLevel); INFO(Ape_Codec_Settings(CompressionLevel));
        GET_L2 (Flags,                                          FormatFlags);
            GET_FLAGS (Flags, 0, Resolution8,                   8-bit);
            SKIP_FLAGS(Flags, 1,                                crc-32);
            SKIP_FLAGS(Flags, 2,                                peak_level);
            GET_FLAGS (Flags, 3, Resolution24,                  24-bit);
            SKIP_FLAGS(Flags, 4,                                seek_elements);
            GET_FLAGS (Flags, 5, no_wav_header,                 no_wav_header);
        if (Resolution8)
            Resolution=8;
        else if (Resolution24)
            Resolution=24;
        else
            Resolution=16;
        GET_L2 (Channels,                                       Channels);
        GET_L4 (SampleRate,                                     SampleRate);
        SKIP_L4(                                                WavHeaderDataBytes);
        SKIP_L4(                                                WavTerminatingBytes);
        GET_L4 (TotalFrames,                                    TotalFrames);
        GET_L4 (FinalFrameSamples,                              FinalFrameSamples);
        SamplesPerFrame=Ape_SamplesPerFrame(Version, CompressionLevel);
        SKIP_L4(                                                PeakLevel);
        GET_L4 (SeekElements,                                   SeekElements);
        if (!no_wav_header)
            SKIP_XX(44,                                         RIFF header);
        SKIP_XX(SeekElements*4,                                 Seek table);
    }
    else
    {
        SKIP_L2(                                                Version_High);
        SKIP_L4(                                                DescriptorBytes);
        SKIP_L4(                                                HeaderBytes);
        SKIP_L4(                                                SeekTableBytes);
        SKIP_L4(                                                WavHeaderDataBytes);
        SKIP_L4(                                                APEFrameDataBytes);
        SKIP_L4(                                                APEFrameDataBytesHigh);
        SKIP_L4(                                                WavTerminatingDataBytes);
        SKIP_L4(                                                FileMD5);
        SKIP_L4(                                                FileMD5);
        SKIP_L4(                                                FileMD5);
        SKIP_L4(                                                FileMD5);
        GET_L2 (CompressionLevel,                               CompressionLevel); INFO(Ape_Codec_Settings(CompressionLevel));
        GET_L2 (Flags,                                          FormatFlags);
        GET_L4 (SamplesPerFrame,                                BlocksPerFrame);
        GET_L4 (FinalFrameSamples,                              FinalFrameBlocks);
        GET_L4 (TotalFrames,                                    TotalFrames);
        GET_L2 (Resolution,                                     BitsPerSample);
        GET_L2 (Channels,                                       Channels);
        GET_L4 (SampleRate,                                     SampleRate);
    }

    //Coherancy
    if (SampleRate==0 || Channels==0 || Resolution==0)
        return;

    //Filling
    int32u Samples=(TotalFrames-1)*SamplesPerFrame+FinalFrameSamples;
    int64u PlayTime=((int64u)Samples)*1000/SampleRate;
    if (PlayTime==0)
        return;
    UncompressedSize=Samples*Channels*(Resolution/8);
    if (UncompressedSize==0)
        return;
    float32 CompressionRatio=((float32)File_Size)/UncompressedSize;
    int32u BitRate=(int32u)(Samples*Channels*Resolution*1000/PlayTime*CompressionRatio);

    Stream_Prepare(Stream_General);
    Fill("Format", "APE");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "APE");
    Fill("Resolution", Resolution);
    Fill("Channel(s)", Channels);
    Fill("SamplingRate", SampleRate);
    Fill("PlayTime", PlayTime);
    Fill("CompressionRatio", CompressionRatio);
    Fill("BitRate", BitRate);

    FLUSH();

    File_GoTo=File_Size-32768;
}

//---------------------------------------------------------------------------
void File_Ape::Footer()
{
    //Looking for APETag alone
    if (Buffer_Size>=0x20 && CC8(Buffer+Buffer_Size-0x20)==CC8("APETAGEX"))
    {
        Buffer_Offset=Buffer_Size-0x20;
        APETag();
    }

    //Looking for APETag before Id3v1
    if (Buffer_Size>=0x20+128 && CC8(Buffer+Buffer_Size-0x20-128)==CC8("APETAGEX"))
    {
        Buffer_Offset=Buffer_Size-0x20-128;
        APETag();
    }

    //Looking for APETag alone
    if (Buffer_Size>=128 && CC3(Buffer+Buffer_Size-128)==CC3("TAG"))
    {
        Buffer_Offset=Buffer_Size-128;
        Id3v1();
    }

    FLUSH();
}

//---------------------------------------------------------------------------
void File_Ape::APETag()
{
    int32u Version=LittleEndian2int32u(Buffer+Buffer_Offset+8);
    Element_Size=LittleEndian2int32u(Buffer+Buffer_Offset+12);

    if (Version==1000)
    {
        if (Element_Size>Buffer_Offset+0x20)
            return;
        Buffer_Offset=Buffer_Offset+0x20-Element_Size;
        APETagv1();
    }

    if (Version==2000)
    {
        if (Element_Size>Buffer_Offset)
            return;
        Buffer_Offset=Buffer_Offset-Element_Size;
        APETagv2();
    }
}

//---------------------------------------------------------------------------
void File_Ape::APETagv1()
{
    ELEMENT(0, "APE Tag v1", Element_Size);
    FLUSH();

    APETag_Common();
}

//---------------------------------------------------------------------------
void File_Ape::APETagv2()
{
    ELEMENT(0, "APE Tag v2", Element_Size+0x20);
    FLUSH();

    ELEMENT(1, "Header", 0x20);
    int32u Flags;
    CC_BEGIN()
    SKIP_C8(                                                    Preamble);
    SKIP_L4(                                                    Version);
    SKIP_L4(                                                    Size);
    SKIP_L4(                                                    Count);
    GET_L4 (Flags,                                              Flags);
        SKIP_FLAGS(Flags,  0,                                   Read Only);
        SKIP_FLAGS(Flags,  1,                                   Binary);
        SKIP_FLAGS(Flags,  2,                                   Locator of external stored information);
        SKIP_FLAGS(Flags, 29,                                   Is the header);
        SKIP_FLAGS(Flags, 30,                                   Contains a footer);
        SKIP_FLAGS(Flags, 31,                                   Contains a header);
    SKIP_L8(                                                    Reserved);
    FLUSH();

    Buffer_Offset+=0x20;
    APETag_Common();
}

//---------------------------------------------------------------------------
void File_Ape::APETag_Common()
{
    //Parsing
    std::string Key;
    Ztring Value;
    int32u Flags, Length;
    CC_BEGIN()
    while (Stream_Pos<Element_Size-0x20)
    {
        GET_L4 (Length,                                         Length);
        GET_L4 (Flags,                                          Flags);
            SKIP_FLAGS(Flags,  0,                               Read Only);
            SKIP_FLAGS(Flags,  1,                               Binary);
            SKIP_FLAGS(Flags,  2,                               Locator of external stored information);
            SKIP_FLAGS(Flags, 29,                               Is the header);
            SKIP_FLAGS(Flags, 30,                               Contains a footer);
            SKIP_FLAGS(Flags, 31,                               Contains a header);
        int32u Stream_Pos_Save=Stream_Pos;
        GET_STRING(Element_Size-Stream_Pos, Key,                Key);
        Stream_Pos=Stream_Pos_Save+Key.size()+1;
        GET_UTF8(Length, Value,                                 Value);
        ELEMENT(1, "Tag", Length);

        //Filling
             if (Key=="ALBUM")          Fill(Stream_General, 0, "Album", Value);
        else if (Key=="ARTIST")         Fill(Stream_General, 0, "Performer", Value);
        else if (Key=="AUTHOR")         Fill(Stream_General, 0, "Performer", Value);
        else if (Key=="COMMENTS")       Fill(Stream_General, 0, "Comment", Value);
        else if (Key=="COMPOSER")       Fill(Stream_General, 0, "WrittentBy", Value);
        else if (Key=="YEAR")           Fill(Stream_General, 0, "Recorded_Date", Value);
        else if (Key=="TRACK")          Fill(Stream_General, 0, "Track/Position", Value);
        else if (Key=="TITLE")          Fill(Stream_General, 0, "Title", Value);
        else                            Fill(Stream_General, 0, Key.c_str(), Value);

        FLUSH();
    }
    ELEMENT(1, "Footer", 0x20);
    SKIP_C8(                                                    Preamble);
    SKIP_L4(                                                    Version);
    SKIP_L4(                                                    Size);
    SKIP_L4(                                                    Count);
    GET_L4 (Flags,                                              Flags);
        SKIP_FLAGS(Flags,  0,                                   Read Only);
        SKIP_FLAGS(Flags,  1,                                   Binary);
        SKIP_FLAGS(Flags,  2,                                   Locator of external stored information);
        SKIP_FLAGS(Flags, 29,                                   Is the header);
        SKIP_FLAGS(Flags, 30,                                   Contains a footer);
        SKIP_FLAGS(Flags, 31,                                   Contains a header);
    SKIP_L8(                                                    Reserved);
    CC_END()
    FLUSH();
}

//---------------------------------------------------------------------------
void File_Ape::Id3v1()
{
    ELEMENT(0, "Id3v1", 128);
    FLUSH();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Ape::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("BitRate"), Info_HowTo)=_T("R");
        General[0](_T("PlayTime"), Info_HowTo)=_T("R");
        General[0](_T("Author"), Info_HowTo)=_T("R");
        General[0](_T("Album"), Info_HowTo)=_T("R");
        General[0](_T("Track"), Info_HowTo)=_T("R");
        General[0](_T("Comment"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Audio)
    {
        Audio[0](_T("BitRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Channel(s)"), Info_HowTo)=_T("R");
        Audio[0](_T("SamplingRate"), Info_HowTo)=_T("R");
        Audio[0](_T("Codec"), Info_HowTo)=_T("R");
    }
}

} //NameSpace

#endif //MEDIAINFO_APE_YES


// File_Tta - Info for TTA  files
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
// Source : http://tta.sourceforge.net/codec.format
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
#if defined(MEDIAINFO_TTA_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Tta.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

void File_Tta::Read_Buffer_Continue()
{
    //Tags
    if (!TagSizeIsFinal && !DetectBeginOfEndTags())
        return;

    //Parsing
    Buffer_Parse();
}

//---------------------------------------------------------------------------
void File_Tta::Read_Buffer_Finalize()
{
    //Tags
    Tags_Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Tta::Buffer_Parse()
{
    //Tags management
    if (!Tags_Read_Buffer_Continue())
        return false;

    //Header
    Header();

    //Next element
    File_GoTo=File_Size-File_EndTagSize; //Until the end, but before tags
    return false;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Tta::Header()
{
    ELEMENT(0, "Header", 0);

    //Parsing
    Element_Size=(size_t)(Buffer_Size-Buffer_Offset);
    int32u Signature, SampleRate, Samples, CRC32;
    int16u AudioFormat, Channels, BitsPerSample;
    CC_BEGIN();
    GET_C4 (Signature,                                          Signature);
    if (Signature!=CC4("TTA1"))
        return; //Not True Audio
    GET_L2 (AudioFormat,                                        AudioFormat);
    GET_L2 (Channels,                                           NumChannels);
    GET_L2 (BitsPerSample,                                      BitsPerSample);
    GET_L4 (SampleRate,                                         SampleRate);
    GET_L4 (Samples,                                            DataLength);
    GET_L4 (CRC32,                                              CRC32);

    //Filling
    if (SampleRate==0)
        return;
    int64u PlayTime=((int64u)Samples)*1000/SampleRate;
    if (PlayTime==0)
        return;
    int64u UncompressedSize=Samples*Channels*(BitsPerSample/8);
    if (UncompressedSize==0)
        return;
    float32 CompressionRatio=((float32)File_Size)/UncompressedSize;
    int32u BitRate=(int32u)(Samples*Channels*BitsPerSample*1000/PlayTime*CompressionRatio);

    Stream_Prepare(Stream_General);
    Fill("Format", "TTA");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "TTA ");
    Fill("Resolution", BitsPerSample);
    Fill("Channel(s)", Channels);
    Fill("SamplingRate", SampleRate);
    Fill("PlayTime", PlayTime);
    Fill("CompressionRatio", CompressionRatio);
    Fill("BitRate", BitRate);

    FLUSH();
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Tta::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("OverallBitRate", "R");
            Fill_HowTo("PlayTime", "R");
            Fill_HowTo("Encoded_Library", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("SamplingCount", "R");
            Fill_HowTo("Resolution", "R");
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_TTA_YES

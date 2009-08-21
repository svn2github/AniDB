// File_Wvpk - Info for WavPack files
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
// Source : http://www.wavpack.com/file_format.txt
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
#if defined(MEDIAINFO_WVPK_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Wvpk.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const int16u Wvpk_Resolution[]=
{
     8,
    16,
    24,
    32,
};

//---------------------------------------------------------------------------
const int32u Wvpk_SamplingRate[]=
{
      6000,
      8000,
      9600,
     11025,
     12000,
     16000,
     22050,
     24000,
     32000,
     44100,
     48000,
     64000,
     88200,
     96000,
    192000,
         0,
};

//---------------------------------------------------------------------------
const char* Wvpk_id(int8u ID)
{
    switch (ID)
    {
        case 0x00 : return "could be used to pad WavPack blocks";
        case 0x02 : return "decorrelation terms & deltas";
        case 0x03 : return "initial decorrelation weights";
        case 0x04 : return "decorrelation sample history";
        case 0x05 : return "initial entropy variables";
        case 0x06 : return "entropy variables specific to hybrid mode";
        case 0x07 : return "info needed for hybrid lossless (wvc) mode";
        case 0x08 : return "specific info for floating point decode";
        case 0x09 : return "specific info for decoding integers > 24 bits";
        case 0x0A : return "normal compressed audio bitstream (wv file)";
        case 0x0B : return "correction file bitstream (wvc file)";
        case 0x0C : return "special extended bitstream for floating point data or integers > 24 bit";
        case 0x0D : return "contains channel count and channel_mask";
        case 0x21 : return "RIFF header for .wav files (before audio)";
        case 0x22 : return "RIFF trailer for .wav files (after audio)";
        case 0x25 : return "some encoding details for info purposes";
        case 0x26 : return "16-byte MD5 sum of raw audio data";
        case 0x27 : return "non-standard sampling rate info";
        default:    return "";
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wvpk::Read_Buffer_Init()
{
    //Configuration
    Frame_Count_Valid=2;

    //Temp - Global
    Frame_Count=0;

    //Temp - Technical info
    total_samples_FirstFrame=0xFFFFFFFF;
    block_index_FirstFrame=0;
    block_index_LastFrame=0;
}

//---------------------------------------------------------------------------
void File_Wvpk::Read_Buffer_Continue()
{
    //Tags
    if (!TagSizeIsFinal && !DetectBeginOfEndTags())
        return;

    //Parsing
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Wvpk::Read_Buffer_Finalize()
{
    //PlayTime
    if (SamplingRate<15)
        Fill("PlayTime", ((int64u)(block_index_LastFrame+block_samples_LastFrame-block_index_FirstFrame))*1000/Wvpk_SamplingRate[SamplingRate], 10, true); //Don't forget the last frame with block_samples...

    //Tags
    Tags_Read_Buffer_Finalize();
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Wvpk::Buffer_Parse()
{
    //Tags management
    if (!Tags_Read_Buffer_Continue())
        return false;

    //Search for header
    if (!NextFrame())
        return false; //Header not found

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
bool File_Wvpk::Element_Parse()
{
    //Parsing the header
    if (!Element_Parse_Header())
        return false;

    //Element
    ELEMENT(0, "Block", Element_Size);

    //Positionning
    Buffer_Offset+=8; //Header

    //Parsing
    Block();
    FLUSH();

    return true;
}

//---------------------------------------------------------------------------
bool File_Wvpk::Element_Parse_Header()
{
    //Enough buffer?
    if (Buffer_Offset+8>Buffer_Size)
        return false;

    //Configure defines : must return false
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE false

    //Parsing
    Element_Size=8;
    int32u ckID;
    CC_BEGIN();
    GET_C4 (ckID,                                               ckID);
    if (ckID!=CC4("wvpk"))
        FINNISHED_BOOL(); // Not wavpack 4.xxx files
    GET_L4 (Element_Size,                                       ckSize);
    CC_END();

    //UnConfigure defines : must return nothing
    #undef INTEGRITY_RETURNVALUE
    #define INTEGRITY_RETURNVALUE

    Element_Next=File_Offset+Buffer_Offset+8+Element_Size;
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wvpk::Block()
{
    //Counting
    Frame_Count++;

    //Parsing
    int16u version;
    CC_BEGIN();
    FLUSH();
    ELEMENT(1, "Header", 32);
    GET_L2 (version,                                            version);
    if (version/0x100==0x4)
    {
        int32u total_samples, block_index, block_samples, flags;
        SKIP_L1(                                                track_no);
        SKIP_L1(                                                index_no);
        GET_L4 (total_samples,                                  total_samples);
        GET_L4 (block_index,                                    block_index);
        GET_L4 (block_samples,                                  block_samples);
        if (block_samples!=0) //empty frames have other values empty
        {
            if (block_index==0) //Only the frame with block_index==0
                total_samples_FirstFrame=total_samples; //Note: total_samples is not trustable for a cutted file
            if (Frame_Count==1)
                block_index_FirstFrame=block_index; //Save the block_index of the first block
            block_index_LastFrame=block_index;
            block_samples_LastFrame=block_samples;
            GET_L4 (flags,                                      flags);
                GET_FLAGS (flags,  0, resolution0,              resolution0);
                GET_FLAGS (flags,  1, resolution1,              resolution1);
                GET_FLAGS (flags,  2, mono,                     mono);
                GET_FLAGS (flags,  3, hybrid,                   hybrid);
                GET_FLAGS (flags,  4, joint_stereo,             joint stereo);
                GET_FLAGS (flags,  5, cross_channel_decorrelation, cross-channel decorrelation);
                SKIP_FLAGS(flags,  6,                           hybrid noise shaping);
                SKIP_FLAGS(flags,  7,                           floating point data);
                SKIP_FLAGS(flags,  8,                           extended size integers);
                SKIP_FLAGS(flags,  9,                           hybrid mode parameters control bitrate);
                SKIP_FLAGS(flags, 10,                           hybrid noise balanced between channels);
                SKIP_FLAGS(flags, 11,                           initial block in sequence);
                SKIP_FLAGS(flags, 12,                           final block in sequence);
                SKIP_FLAGS(flags, 13,                           amount of data left-shift after decode);
                SKIP_FLAGS(flags, 14,                           amount of data left-shift after decode);
                SKIP_FLAGS(flags, 15,                           amount of data left-shift after decode);
                SKIP_FLAGS(flags, 16,                           amount of data left-shift after decode);
                SKIP_FLAGS(flags, 17,                           amount of data left-shift after decode);
                SKIP_FLAGS(flags, 18,                           maximum magnitude of decoded data);
                SKIP_FLAGS(flags, 19,                           maximum magnitude of decoded data);
                SKIP_FLAGS(flags, 20,                           maximum magnitude of decoded data);
                SKIP_FLAGS(flags, 21,                           maximum magnitude of decoded data);
                SKIP_FLAGS(flags, 22,                           maximum magnitude of decoded data);
                SKIP_FLAGS(flags, 23,                           sampling rate);
                SKIP_FLAGS(flags, 24,                           sampling rate);
                SKIP_FLAGS(flags, 25,                           sampling rate);
                SKIP_FLAGS(flags, 26,                           sampling rate); SamplingRate=((flags>>23)&0xF); INFO(Wvpk_SamplingRate[SamplingRate]);
                SKIP_FLAGS(flags, 27,                           reserved);
                SKIP_FLAGS(flags, 28,                           reserved);
                SKIP_FLAGS(flags, 29,                           use IIR for negative hybrid noise shaping);
                SKIP_FLAGS(flags, 30,                           false stereo);
                SKIP_FLAGS(flags, 31,                           reserved);
        }
        else
        {
            SKIP_L4(                                            flags_empty);

            //Counting
            Frame_Count--; //This is not a real frame
        }
        SKIP_L4(                                                crc);
        ELEMENT_END();

        //Sub-block
        int32u word_size;
        int8u id;
        while (Stream_Pos<Element_Size)
        {
            FLUSH();
            ELEMENT_BEGIN();
            GET_L1 (id,                                             id); INFO(Wvpk_id(id&0x2F));
            if (id&0x80)
            {
                GET_L3 (word_size,                                  word_size);
            }
            else
            {
                GET_L1 (word_size,                                  word_size);
            }
            ELEMENT(1, id, word_size*2); NAME(Wvpk_id(id&0x2F));
            SKIP_XX(word_size*2,                                    data);
            ELEMENT_END();
        }
    }
    CC_END_CANBEMORE();

    //Filling
    if (Count_Get(Stream_Audio)==0 && Frame_Count>=Frame_Count_Valid)
        Block_Fill();
}

//---------------------------------------------------------------------------
void File_Wvpk::Block_Fill()
{
    //Filling
    if (Count_Get(Stream_General)==0)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "Wvpk");
        Stream_Prepare(Stream_Audio);
        Fill("Codec", "Wavpack");
    }

    Fill("Resolution", Wvpk_Resolution[(resolution1?1:0)*2+(resolution0?1:0)]);
    Fill("Channel(s)", mono?1:2);
    if (SamplingRate<15)
    {
        Fill("SamplingRate", Wvpk_SamplingRate[SamplingRate]);
        if (total_samples_FirstFrame!=0xFFFFFFFF) //--> this is a valid value
            Fill("PlayTime", ((int64u)total_samples_FirstFrame)*1000/Wvpk_SamplingRate[SamplingRate]);
    }
    if (hybrid)
        Fill("Codec_Settings", "hybrid lossy");
            else
        Fill("Codec_Settings", "lossless");

    //Going to end of file
    if (File_Size>File_EndTagSize+1024*1024 && File_Offset+Buffer_Size<File_Size)
    {
        //Details
        Buffer_Offset=(size_t)(Element_Next-File_Offset);
        if (Config.Details_Get())
        {
            Details_Add_Error("-------------------------------------------");
            Details_Add_Error("---   Wavpack, Jumping to end of file   ---");
            Details_Add_Error("-------------------------------------------");
        }
        //No more need data
        if (File_Name.empty())
            File_Offset=File_Size; //Finnished
        else if (File_Size>File_EndTagSize)
            File_GoTo=File_Size-File_EndTagSize-1024*1024; //Looking for end blocks and tags
    }
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Wvpk::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first frames
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size && CC4(Buffer+Buffer_Offset_Temp)!=CC4("wvpk"))
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (Buffer_Offset_Temp+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset_Temp)!=CC3("wvp"))
            {
                Buffer_Offset_Temp++;
                if (CC2(Buffer+Buffer_Offset_Temp)!=CC2("wv"))
                {
                    Buffer_Offset_Temp++;
                    if (CC1(Buffer+Buffer_Offset_Temp)!=CC1("w"))
                        Buffer_Offset_Temp++;
                }
            }
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;

        //Tag management
        if (File_Offset+Buffer_Offset>=File_Size-File_EndTagSize)
        {
            Buffer_Offset=File_Size-File_EndTagSize-File_Offset;
            Tags_Read_Buffer_Continue();
        }

        return false;
    }

    //Must wait more data?
    if (Buffer_Offset_Temp+4>Buffer_Size)
        return false;

    //Error in stream?
    if (Buffer_Offset_Temp-Buffer_Offset>0)
    {
        if (Synched)
            TRUSTED_ISNOT("Sync error", 0, Buffer_Offset_Temp-Buffer_Offset);
        else
        {
            ELEMENT(0, "Synchronization", Buffer_Offset_Temp-Buffer_Offset);
            FLUSH();
        }
    }

    //OK, we continue
    Buffer_Offset=Buffer_Offset_Temp;
    Buffer_Offset_Temp=0;
    Synched=true;
    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Wvpk::HowTo(stream_t StreamKind)
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

#endif //MEDIAINFO_WVPK_YES

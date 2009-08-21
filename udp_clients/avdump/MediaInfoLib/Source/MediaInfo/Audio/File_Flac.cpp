// File_Flac - Info for Flac files
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
#include <ZenLib/BitStream.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_FLAC_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_Flac.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Read_Buffer_Init()
{
    //Temp
    LastMetaData=false;
}

//---------------------------------------------------------------------------
void File_Flac::Read_Buffer_Continue()
{
    //Header
    if (File_Offset==0)
    {
        if (Buffer_Size<4 || CC4(Buffer)!=CC4("fLaC"))
        {
            File_Offset=File_Size;
            return;
        }

        Header();
    }

    //Parsing
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
//
bool File_Flac::Buffer_Parse()
{
    //Element Name
    if (!Element_Name_Get())
        return false;

    //Element Size
    if (!Element_Size_Get())
        return false;

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes

    //If no need of more
    if (LastMetaData)
        File_Offset=File_Size; //MetaData is finnished
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Flac::Element_Parse()
{
    ELEMENT(0, Element_Name, Element_Size);

    #define ELEMENT_CASE(_NAME) \
        case Flac::_NAME : _NAME(); break;

    //Positionning
    Buffer_Offset+=4;

    //Parsing
    switch (Element_Name)
    {
        ELEMENT_CASE(STREAMINFO);
        ELEMENT_CASE(PADDING);
        ELEMENT_CASE(APPLICATION);
        ELEMENT_CASE(SEEKTABLE);
        ELEMENT_CASE(VORBIS_COMMENT);
        ELEMENT_CASE(CUESHEET);
        ELEMENT_CASE(PICTURE);
        default : ;
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::Header()
{
    ELEMENT(0, "Flac header", 4);

    //Parsing
    Element_Size=4;
    CC_BEGIN();
    SKIP_C4(                                                    Signature);
    CC_END();

    FLUSH();

    Buffer_Offset+=4;
}

//---------------------------------------------------------------------------
void File_Flac::STREAMINFO()
{
    NAME("STREAMINFO");

    //Parsing
    int64u Samples, Samples_Temp;
    int32u FrameSize_Min, FrameSize_Max, SampleRate, Channels, BitPerSample;
    BS_BEGIN();
    SKIP_BS(16,                                                 BlockSize_Min); //The minimum block size (in samples) used in the stream.
    SKIP_BS(16,                                                 BlockSize_Max); //The maximum block size (in samples) used in the stream. (Minimum blocksize == maximum blocksize) implies a fixed-blocksize stream.
    GET_BS (24, FrameSize_Min,                                  FrameSize_Min); //The minimum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    GET_BS (24, FrameSize_Max,                                  FrameSize_Max); //The maximum frame size (in bytes) used in the stream. May be 0 to imply the value is not known.
    GET_BS (20, SampleRate,                                     SampleRate); //Sample rate in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 1048570Hz. Also, a value of 0 is invalid.
    GET_BS ( 3, Channels,                                       Channels); INFO(Channels+1, " channels"); //(number of channels)-1. FLAC supports from 1 to 8 channels
    GET_BS ( 5, BitPerSample,                                   BitPerSample); INFO(BitPerSample+1, " bits"); //(bits per sample)-1. FLAC supports from 4 to 32 bits per sample. Currently the reference encoder and decoders only support up to 24 bits per sample.
    GET_BS ( 4, Samples_Temp,                                   Samples  4 bits);
    GET_BS (32, Samples,                                        Samples 32 bits); //Total samples in stream. 'Samples' means inter-channel sample, i.e. one second of 44.1Khz audio will have 44100 samples regardless of the number of channels. A value of zero here means the number of total samples is unknown.
    Samples+=Samples_Temp*0x100000000LL; //BitStream class is only 32 bits
    SKIP_BS(32,                                                 MD5 part 1);
    SKIP_BS(32,                                                 MD5 part 2);
    SKIP_BS(32,                                                 MD5 part 3);
    SKIP_BS(32,                                                 MD5 part 4); //MD5 signature of the unencoded audio data. This allows the decoder to determine if an error exists in the audio data even when the error does not result in an invalid bitstream.
    BS_END();

    //Filling
    if (SampleRate==0)
        return;
    Stream_Prepare(Stream_General);
    Fill("Format", "FLAC");
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "FLAC");
    if (FrameSize_Min==FrameSize_Max && FrameSize_Min!=0 ) // 0 means it is unknown
        Fill("BitRate_Mode", "CBR");
     else
        Fill("BitRate_Mode", "VBR");
    Fill("SamplingRate", SampleRate);
    Fill("Channel(s)", Channels);
    Fill("Resolution", BitPerSample);
    Fill(Stream_General, 0, "PlayTime", Samples*1000/SampleRate);
}

//---------------------------------------------------------------------------
void File_Flac::PADDING()
{
    NAME("PADDING");

    //Parsing
    CC_BEGIN();
    SKIP_XX(Element_Size,                                       Padding);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Flac::APPLICATION()
{
    NAME("APPLICATION");

    //Parsing
    CC_BEGIN();
    SKIP_C4(                                                    Application);
    SKIP_XX(Element_Size-4,                                     (Application specific));
    CC_END();
}

//---------------------------------------------------------------------------
void File_Flac::SEEKTABLE()
{
    NAME("SEEKTABLE");
}

//---------------------------------------------------------------------------
void File_Flac::VORBIS_COMMENT()
{
    NAME("VORBIS_COMMENT");

    //Parsing
    Ztring vendor_string, comment;
    int32u vendor_length, user_comment_list_length, length;
    CC_BEGIN();
    GET_L4 (vendor_length,                                      vendor_length);
    GET_LOCAL(vendor_length, vendor_string,                     vendor_string);
    GET_L4 (user_comment_list_length,                           user_comment_list_length);
    FLUSH();
    for (int32u Pos=0; Pos<user_comment_list_length; Pos++)
    {
        ELEMENT_BEGIN();
        GET_L4 (length,                                         length);
        ELEMENT(1, "Comment", length);
        GET_UTF8(length, comment,                               comment);

        //Filling
        Ztring Key=comment.SubString(_T(""), _T("="));
        Ztring Value=comment.SubString(_T("="), _T(""));

             if (Key==_T("ALBUM"))          Fill(Stream_General, 0, "Album", Value);
        else if (Key==_T("ARTIST"))         Fill(Stream_General, 0, "Performer", Value);
        else if (Key==_T("COPYRIGHT"))      Fill(Stream_General, 0, "Copyright", Value);
        else if (Key==_T("DATE"))           Fill(Stream_General, 0, "Recorded_Date", Value);
        else if (Key==_T("DESCRIPTION"))    Fill(Stream_General, 0, "Comment", Value);
        else if (Key==_T("LOCATION"))       Fill(Stream_General, 0, "RecordingLocation", Value);
        else if (Key==_T("PERFORMER"))      Fill(Stream_General, 0, "Performer", Value);
        else if (Key==_T("TITLE"))          Fill(Stream_General, 0, "Title", Value);
        else if (Key==_T("TRACKNUMBER"))    Fill(Stream_General, 0, "Track", Value);
        else                                Fill(Stream_General, 0, Key.To_Local().c_str(), Value);

        FLUSH();
        ELEMENT_END();
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Flac::CUESHEET()
{
    NAME("CUESHEET");
}

//---------------------------------------------------------------------------
void File_Flac::PICTURE()
{
    NAME("PICTURE");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Flac::Element_Name_Get()
{
    //Enough data?
    if (Buffer_Offset+1>Buffer_Size)
        return false;

    //Element name
    Element_Name=LittleEndian2int8u(Buffer+Buffer_Offset);

    //LastMetaData?
    if (Element_Name&0x80) //First bit
    {
        LastMetaData=true;
        Element_Name=Element_Name&0x7F; //Remove last bit
    }

    return true;
}

//---------------------------------------------------------------------------
bool File_Flac::Element_Size_Get()
{
    //Enough data?
    if (Buffer_Offset+4>Buffer_Size)
        return false;

    //Element size
    Element_Size=BigEndian2int24u(Buffer+Buffer_Offset+1);

    //Element Next
    Element_Next=File_Offset+Buffer_Offset+4+Element_Size;

    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Flac::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
        General[0](_T("BitRate"), Info_HowTo)=_T("R");
        General[0](_T("Title"), Info_HowTo)=_T("R");
        General[0](_T("Title/More"), Info_HowTo)=_T("R");
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

#endif //MEDIAINFO_FLAC_YES


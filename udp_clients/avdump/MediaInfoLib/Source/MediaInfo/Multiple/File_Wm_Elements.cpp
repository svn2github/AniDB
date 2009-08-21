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
//
// Elements part
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
#ifdef MEDIAINFO_WM_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Wm.h"
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

const char* Wm_CodecList_Kind(int32u Kind)
{
    switch (Kind)
    {
        case 0x01 : return "Video";
        case 0x02 : return "Audio";
        default   : return "Unknown";
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
// Element parse
//
bool File_Wm::Element_Parse()
{
    #define ATOM_BEGIN \
        if (Level!=Element_Level) \
        { \
            Level++; \
            switch (Element_Name[Level][0]) \
            {

        #define SKIP(_ATOM) \
                case Wm::_ATOM : \
                            FLUSH(); \
                            return true; \

        #define ATOM(_ATOM) \
                case Wm::_ATOM : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_Next[Element_Level]-File_Offset<=Buffer_Size || (Wm::_ATOM==Wm::Data && Buffer_Offset+26<=Buffer_Size)) \
                            { \
                                _ATOM(); \
                                FLUSH(); \
                                if (!List) \
                                    return true; \
                            } \
                            else \
                            { \
                                Buffer_Offset-=24; \
                                Element_Level--; \
                                PURGE(); \
                                return false; \
                            } \
                        } \

        #define ATOM_DEFAULT(_ATOM) \
                default : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_Next[Element_Level]-File_Offset<=Buffer_Size) \
                            { \
                                _ATOM(); \
                                FLUSH(); \
                                if (!List) \
                                    return true; \
                            } \
                            else \
                            { \
                                Buffer_Offset-=24; \
                                Element_Level--; \
                                PURGE(); \
                                return false; \
                            } \
                        } \

        #define LIST(_ATOM) \
                case Wm::_ATOM : \
                        if (Level==Element_Level) \
                        { \
                            List=true; \
                           _ATOM(); \
                            FLUSH(); \
                            if (!List) \
                                return true; \
                        } \

        #define LIST_DEFAULT(_ATOM) \
                default : \
                        if (Level==Element_Level) \
                        { \
                            List=true; \
                           _ATOM(); \
                            FLUSH(); \
                            if (!List) \
                                return true; \
                        } \

    #define ATOM_END \
            } \
        } \
        else \
        { \
            List=true; \
        } \
        break; \

    //Init
    ELEMENT(Element_Level-1, Element_Name[Element_Level][0], Element_Name[Element_Level][1], Element_Name[Element_Level][2], Element_Name[Element_Level][3], Element_Size);
    Buffer_Offset+=24;
    int8u Level=0;
    List=false;

    //Hierarchy
    ATOM_BEGIN
    LIST(Header)
        ATOM_BEGIN
        ATOM(Header_FileProperties)
        ATOM(Header_StreamProperties)
        LIST(Header_HeaderExtension)
            ATOM_BEGIN
            ATOM(Header_HeaderExtension_ExtendedStreamProperties)
            ATOM(Header_HeaderExtension_AdvancedMutualExclusion)
            ATOM(Header_HeaderExtension_GroupMutualExclusion)
            ATOM(Header_HeaderExtension_StreamPrioritization)
            ATOM(Header_HeaderExtension_BandwidthSharing)
            ATOM(Header_HeaderExtension_LanguageList)
            ATOM(Header_HeaderExtension_Metadata)
            ATOM(Header_HeaderExtension_MetadataLibrary)
            ATOM(Header_HeaderExtension_IndexParameters)
            ATOM(Header_HeaderExtension_MediaIndexParameters)
            ATOM(Header_HeaderExtension_TimecodeIndexParameters)
            ATOM(Header_HeaderExtension_Compatibility)
            ATOM(Header_HeaderExtension_AdvancedContentEncryption)
            ATOM_END
        ATOM(Header_CodecList)
        ATOM(Header_ScriptCommand)
        ATOM(Header_Marker)
        ATOM(Header_BitRateMutualExclusion)
        ATOM(Header_ErrorCorrection)
        ATOM(Header_ContentDescription)
        ATOM(Header_ExtendedContentDescription)
        ATOM(Header_StreamBitRate)
        ATOM(Header_ContentBranding)
        ATOM(Header_ContentEncryption)
        ATOM(Header_ExtendedContentEncryption)
        ATOM(Header_DigitalSignature)
        ATOM(Header_Padding)
        ATOM_END
    ATOM(Data)
    ATOM(SimpleIndex)
    ATOM(Index)
    ATOM(MediaIndex)
    ATOM(TimecodeIndex)
    default : ;
    }}
    FLUSH();

    //List
    if (List)
    {
        while (Buffer_Parse());
        if (Element_Next[Element_Level]-File_Offset>Buffer_Size)
            return false; //Not enough bytes
    }

    return true; //Atom unknown
}

//***************************************************************************
// Elements
//***************************************************************************

#define LIST_SET(OFFSET) \
    Buffer_Offset+=OFFSET; \
    Element_Size-=OFFSET; \
    List=true; \

//---------------------------------------------------------------------------
void File_Wm::Header()
{
    NAME("Header");

    //Parsing
    CC_BEGIN();
    SKIP_L4(                                                    Number of Header Objects);
    SKIP_L1(                                                    Alignment);
    SKIP_L1(                                                    Architecture);
    CC_END_CANBEMORE();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "WM");

    LIST_SET(6);
}

//---------------------------------------------------------------------------
void File_Wm::Header_FileProperties()
{
    NAME("File Properties");

    //Parsing
    int64u CreationDate, PlayDuration, SendDuration, Preroll, PacketCount;
    int32u Flags, MinimumDataPacketSize, MaximumDataPacketSize, Seekable, Broadcast;
    CC_BEGIN();
    SKIP_GUID(                                                  File ID);
    SKIP_L8(                                                    File Size);
    GET_L8 (CreationDate,                                       Creation Date); INFO(Ztring().Date_From_Seconds_1601(CreationDate/10000));
    GET_L8 (PacketCount,                                        Data Packets Count);
    GET_L8 (PlayDuration,                                       Play Duration); INFO(Ztring().Duration_From_Milliseconds(PlayDuration/10000));
    GET_L8 (SendDuration,                                       Send Duration); INFO(Ztring().Duration_From_Milliseconds(SendDuration/10000));
    GET_L8 (Preroll,                                            Preroll); INFO(Ztring().Duration_From_Milliseconds(Preroll));
    GET_L4 (Flags,                                              Flags);
        GET_FLAGS (Flags, 0, Broadcast,                         Broadcast);
        GET_FLAGS (Flags, 1, Seekable,                          Seekable);
        SKIP_FLAGS(Flags, 2,                                    Use Packet Template);
        SKIP_FLAGS(Flags, 3,                                    Live);
        SKIP_FLAGS(Flags, 4,                                    Recordable);
        SKIP_FLAGS(Flags, 5,                                    Unknown Data Size);
    GET_L4 (MinimumDataPacketSize,                              Minimum Data Packet Size);
    GET_L4 (MaximumDataPacketSize,                              Maximum Data Packet Size);
    SKIP_L4(                                                    Maximum Bitrate);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "PacketCount", PacketCount);
    if (MinimumDataPacketSize==MaximumDataPacketSize)
        Fill(Stream_General, 0, "PacketSize", MinimumDataPacketSize);
    if (Broadcast)
        Fill(Stream_General, 0, "Broadcast", "Yes");
    Fill(Stream_General, 0, "Date_Created", Ztring().Date_From_Seconds_1601(CreationDate/10000));
    Fill(Stream_General, 0, "PlayTime", PlayDuration/10000-Preroll);
}

//---------------------------------------------------------------------------
void File_Wm::Header_StreamProperties ()
{
    NAME("Stream Properties");

    //Parsing
    CC_BEGIN();
    int32u  StreamType;
    int32u StreamTypeLength, ErrorCorrectionTypeLength;
    int16u Flags;
    GET_GUID(StreamType,                                        StreamType);
    switch (StreamType) //Info + Filling
    {
        case Wm::Header_StreamProperties_Audio :            INFO("Audio"); break;
        case Wm::Header_StreamProperties_Video :            INFO("Video"); break;
        case Wm::Header_StreamProperties_Command :          INFO("Command"); break;
        case Wm::Header_StreamProperties_JFIF :             INFO("JFIF"); break;
        case Wm::Header_StreamProperties_DegradableJPEG :   INFO("Degradable JPEG"); break;
        case Wm::Header_StreamProperties_FileTransfer :     INFO("File Transfer"); break;
        case Wm::Header_StreamProperties_Binary :           INFO("Binary"); break;
        default :                                           INFO("Unknown"); break;
    }
    SKIP_GUID(                                                  Error Correction Type);
    SKIP_L8(                                                    Time Offset);
    GET_L4 (StreamTypeLength,                                   Type-Specific Data Length);
    GET_L4 (ErrorCorrectionTypeLength,                          Error Correction Data Length);
    GET_L2 (Flags,                                              Flags);
    int8u StreamNumber=(int8u)(Flags&0x007F);
    SKIP_L4(                                                    Reserved);
    switch (StreamType) //Info + Filling
    {
        case Wm::Header_StreamProperties_Audio :            Header_StreamProperties_Audio(); Stream_Pos+=StreamTypeLength; break;
        case Wm::Header_StreamProperties_Video :            Header_StreamProperties_Video(); Stream_Pos+=StreamTypeLength; break;
        default :                                           SKIP_XX(StreamTypeLength, Type-Specific Data); break;
    }

    SKIP_XX(ErrorCorrectionTypeLength,                      Error Correction Data);
    CC_END();

    //Filling
    CurrentStream_Kind[StreamNumber]=StreamKind_Last;
    CurrentStream_Pos [StreamNumber]=StreamPos_Last;
    if ((size_t)StreamNumber<CurrentBitRate.size())
        Fill("BitRate", CurrentBitRate[StreamNumber]);
}

//---------------------------------------------------------------------------
void File_Wm::Header_StreamProperties_Audio ()
{
    int32u Stream_Pos=54; //From Header_StreamProperties()
    int32u CodecID, Channels, SamplingRate, BytesPerSec, Data_Size;
    GET_L2 (CodecID,                                            Codec ID);
    GET_L2 (Channels,                                           Number of Channels);
    GET_L4 (SamplingRate,                                       Samples Per Second);
    GET_L4 (BytesPerSec,                                        Average Number of Bytes Per Second);
    SKIP_L2(                                                    Block Alignment);
    GET_L2 (Data_Size,                                          Codec Specific Data Size);
    SKIP_XX(Data_Size,                                          Codec Specific Data);

    //Filling
    Stream_Prepare(Stream_Audio);
    Ztring Codec; Codec.From_Number(CodecID, 16);
    Codec.MakeUpperCase();
    Fill("Codec", Codec); //May be replaced by codec parser
    Fill("Codec/CC", Codec);
    Fill("Channel(s)", Channels);
    Fill("SamplingRate", SamplingRate);
    Fill("BitRate", BytesPerSec*8);
}

//---------------------------------------------------------------------------
void File_Wm::Header_StreamProperties_Video ()
{
    int32u Stream_Pos=54; //From Header_StreamProperties()
    int32u Data_Size, Width, Height, Resolution, Compression;
    GET_L4 (Width,                                              Width);
    GET_L4 (Height,                                             Height);
    SKIP_L1(                                                    Flags);
    GET_L2 (Data_Size,                                          Format Data Size);
    SKIP_L4(                                                    Size);
    GET_L4 (Width,                                              Width);
    GET_L4 (Height,                                             Height);
    SKIP_L2(                                                    Planes);
    GET_L2 (Resolution,                                         BitCount);
    GET_C4 (Compression,                                        Compression);
    SKIP_L4(                                                    SizeImage);
    SKIP_L4(                                                    XPelsPerMeter);
    SKIP_L4(                                                    YPelsPerMeter);
    SKIP_L4(                                                    ClrUsed);
    SKIP_L4(                                                    ClrImportant);
    if (Data_Size-28>0 && Stream_Pos+Data_Size-28<=Element_Size) //TODO: see "The Mummy_e"
        SKIP_XX(Data_Size-28,                                   Format Data - Extended);

    //Filling
    Stream_Prepare(Stream_Video);
    Fill("Codec", Ztring().From_CC4(Compression)); //May be replaced by codec parser
    Fill("Codec/CC", Ztring().From_CC4(Compression));
    Fill("Width", Width);
    Fill("Height", Height);
    Fill("Resolution", Resolution);
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension()
{
    NAME("Header Extension");

    //Parsing
    CC_BEGIN();
    int32u Size;
    SKIP_GUID(                                                  ClockType);
    SKIP_L2(                                                    ClockSize);
    GET_L4 (Size,                                               Extension Data Size);

    LIST_SET(22);
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_ExtendedStreamProperties()
{
    NAME("Extended Stream Properties");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_AdvancedMutualExclusion()
{
    NAME("Advanced Mutual Exclusion");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_GroupMutualExclusion()
{
    NAME("Group Mutual Exclusion");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_StreamPrioritization()
{
    NAME("Stream Prioritization");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_BandwidthSharing()
{
    NAME("Bandwidth Sharing");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_LanguageList()
{
    NAME("Language List");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_MetadataLibrary()
{
    NAME("Metadata Library");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_Metadata()
{
    NAME("Metadata");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_IndexParameters()
{
    NAME("Index Parameters");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_MediaIndexParameters()
{
    NAME("MediaIndex Parameters");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_TimecodeIndexParameters()
{
    NAME("Timecode Index Parameters");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_Compatibility()
{
    NAME("Compatibility");
}

//---------------------------------------------------------------------------
void File_Wm::Header_HeaderExtension_AdvancedContentEncryption()
{
    NAME("Advanced Content Encryption");
}

//---------------------------------------------------------------------------
void File_Wm::Header_CodecList()
{
    NAME("Codec List");

    //Parsing
    CC_BEGIN();
    int32u Count, Type, CodecNameLength, CodecDescriptionLength, CodecInformationLength;
    Ztring CodecName, CodecDescription, CodecInformation;
    SKIP_GUID(                                                  Reserved);
    GET_L4 (Count,                                              Codec Entries Count);
    for (int32u Pos=0; Pos<Count; Pos++)
    {
        GET_L2 (Type,                                           Type); INFO(Wm_CodecList_Kind(Type));
        GET_L2 (CodecNameLength,                                Codec Name Length);
        GET_UTF16L(CodecNameLength*2, CodecName,                Codec Name);
        GET_L2 (CodecDescriptionLength,                         Codec Description Length);
        GET_UTF16L(CodecDescriptionLength*2, CodecDescription,  Codec Description);
        GET_L2 (CodecInformationLength,                         Codec Information Length);
        GET_LOCAL(CodecInformationLength, CodecInformation,     Codec Information);
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Wm::Header_ScriptCommand()
{
    NAME("Script Command");
}

//---------------------------------------------------------------------------
void File_Wm::Header_Marker()
{
    NAME("Marker");
}

//---------------------------------------------------------------------------
void File_Wm::Header_BitRateMutualExclusion()
{
    NAME("BitRate Mutual Exclusion");
}

//---------------------------------------------------------------------------
void File_Wm::Header_ErrorCorrection()
{
    NAME("Error Correction");
}

//---------------------------------------------------------------------------
void File_Wm::Header_ContentDescription()
{
    NAME("Content Description");

    //Parsing
    CC_BEGIN();
    int32u TitleLength, AuthorLength, CopyrightLength, DescriptionLength, RatingLength;
    Ztring Title, Author, Copyright, Description, Rating;
    GET_L2 (TitleLength,                                        TitleLength);
    GET_L2 (AuthorLength,                                       AuthorLength);
    GET_L2 (CopyrightLength,                                    CopyrightLength);
    GET_L2 (DescriptionLength,                                  DescriptionLength);
    GET_L2 (RatingLength,                                       RatingLength);
    GET_UTF16L(TitleLength, Title,                              Title);
    GET_UTF16L(AuthorLength, Author,                            Author);
    GET_UTF16L(CopyrightLength, Copyright,                      Copyright);
    GET_UTF16L(DescriptionLength, Description,                  Description);
    GET_UTF16L(RatingLength, Rating,                            Rating);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Title", Title);
    Fill(Stream_General, 0, "Performer", Author);
    Fill(Stream_General, 0, "Copyright", Copyright);
    Fill(Stream_General, 0, "Comment", Description);
    Fill(Stream_General, 0, "Rating", Rating);
}

//---------------------------------------------------------------------------
void File_Wm::Header_ExtendedContentDescription()
{
    NAME("Extended Content Description");

    //Parsing
    Ztring Name, Value;
    int64u Value_Int;
    int16u Count, Name_Length, Value_Type, Value_Length;
    CC_BEGIN();
    GET_L2 (Count,                                              Content Descriptors Count);
    for (int16u Pos=0; Pos<Count; Pos++)
    {
        GET_L2 (Name_Length,                                    Name Length);
        GET_UTF16L(Name_Length, Name,                           Name Length);
        GET_L2 (Value_Type,                                     Value Data Type);
        GET_L2 (Value_Length,                                   Value Length);
        Value.clear(); Value_Int=0;
        switch (Value_Type)
        {
            case 0x00 : GET_UTF16L(Value_Length, Value,         Value); break;
            case 0x01 : GET_LOCAL(Value_Length, Value,          Value); break;
            case 0x02 : GET_L4 (Value_Int,                      Value); break;
            case 0x03 : GET_L4 (Value_Int,                      Value); break;
            case 0x04 : GET_L8 (Value_Int,                      Value); break;
            case 0x05 : GET_L2 (Value_Int,                      Value); break;
        default :                                                       break;
        }
        if (Value_Int)
            Value.From_Number(Value_Int);

        //Filling
             if (Name==_T("WM/AlbumTitle"))
            Fill(Stream_General, 0, "Album", Value);
        else if (Name==_T("WM/AlbumArtist"))
        {
            Fill(Stream_General, 0, "Performer", "");
            Fill(Stream_General, 0, "Performer", Value, true); //Clear last value, like Author (Content Description)
        }
        else if (Name==_T("WM/ArtistSortOrder"))
            Fill(Stream_General, 0, "Performer/Sort", Value);
        else if (Name==_T("WM/AuthorURL"))
            Fill(Stream_General, 0, "Performer/Url", Value);
        else if (Name==_T("WM/BeatsPerMinute"))
            Fill(Stream_General, 0, "BPM", Value);
        else if (Name==_T("WM/Binary"))
            Fill(Stream_General, 0, "Cover", "Y");
        else if (Name==_T("WM/Comments"))
            Fill(Stream_General, 0, "Comment", Value, true); //Clear last value
        else if (Name==_T("WM/Composer"))
            Fill(Stream_General, 0, "Composer", Value);
        else if (Name==_T("WM/Conductor"))
            Fill(Stream_General, 0, "Conductor", Value);
        else if (Name==_T("WM/EncodedBy"))
            Fill(Stream_General, 0, "EncodedBy", Value);
        else if (Name==_T("WM/EncoderSettings"))
            Fill(Stream_General, 0, "Encoded_Library_Settings", Value);
        else if (Name==_T("WM/EncodingTime"))
            Fill(Stream_General, 0, "Encoded_Date", Ztring().Date_From_Seconds_1601(Value_Int));
        else if (Name==_T("WM/Genre"))
            Fill(Stream_General, 0, "Genre", Value, true); //Clear last value
        else if (Name==_T("WM/GenreID"))
        {
            if (Get(Stream_General, 0, _T("Genre")).empty())
                Fill(Stream_General, 0, "Genre", Value);
        }
        else if (Name==_T("WM/MediaPrimaryClassID")) {}
        else if (Name==_T("WM/MCDI")) {}
        else if (Name==_T("WM/ModifiedBy"))
            Fill(Stream_General, 0, "RemixedBy", Value);
        else if (Name==_T("WM/OriginalAlbumTitle"))
            Fill(Stream_General, 0, "Original/Album", Value);
        else if (Name==_T("WM/Picture"))
            Fill(Stream_General, 0, "Cover", "Y");
        else if (Name==_T("WM/Provider"))
            Fill(Stream_General, 0, "Provider", Value);
        else if (Name==_T("WM/Publisher"))
            Fill(Stream_General, 0, "Publisher", Value);
        else if (Name==_T("WM/RadioStationName"))
            Fill(Stream_General, 0, "RadioStation", Value);
        else if (Name==_T("WM/RadioStationOwner"))
            Fill(Stream_General, 0, "RadioStation/Owner", Value);
        else if (Name==_T("WM/TrackNumber"))
            Fill(Stream_General, 0, "Track/Position", Value, true); //Clear last value, like WM/Track
        else if (Name==_T("WM/Track"))
        {
            if (Get(Stream_General, 0, _T("Track/Position")).empty())
                Fill(Stream_General, 0, "Track/Position", Value.To_int32u()+1);
        }
        else if (Name==_T("WM/UniqueFileIdentifier"))
        {
            if (Value.empty() || Value[0]!=_T(';')) //Test if there is only the separator
            {
                Value.FindAndReplace(_T(";"), Config.TagSeparator_Get());
                Fill(Stream_General, 0, "UniqueID", Value);
            }
        }
        else if (Name==_T("WM/Writer"))
            Fill(Stream_General, 0, "Lyristic", Value);
        else if (Name==_T("WM/Year"))
            Fill(Stream_General, 0, "Recorded_Date", Value);
        else if (Name==_T("IsVBR")) {}
        else if (Name==_T("WMFSDKVersion")) {}
        else if (Name==_T("WMFSDKNeeded")) {}
        else
            Fill(Stream_General, 0, Name.To_Local().c_str(), Value);
    }

    CC_END();

}

//---------------------------------------------------------------------------
void File_Wm::Header_StreamBitRate()
{
    NAME("Stream Bitrate");

    //Parsing
    int32u BitRate;
    int16u Count, Flags;
    CC_BEGIN();
    GET_L2 (Count,                                              Count);
    for (int16u Pos=0; Pos<Count; Pos++)
    {
        GET_L2 (Flags,                                          Flags);
        int8u StreamNumber=(int8u)(Flags&0x007F);
        GET_L4 (BitRate,                                        Average Bitrate);

        //Filling
        if (CurrentStream_Kind.find(StreamNumber)==CurrentStream_Kind.end()) //Already parsed
            Fill(CurrentStream_Kind[StreamNumber], CurrentStream_Pos[StreamNumber], "BitRate", BitRate);
        else
        {
            if (CurrentBitRate.size()<(size_t)(StreamNumber+1))
                CurrentBitRate.resize(StreamNumber+1);
            CurrentBitRate[StreamNumber]=BitRate;
        }
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Wm::Header_ContentBranding()
{
    NAME("Content Branding");
}

//---------------------------------------------------------------------------
void File_Wm::Header_ContentEncryption()
{
    NAME("Content Encryption");

    //Parsing
    Ztring LicenseURL;
    int32u SecretDataLength, ProtectionTypeLength, KeyIDLength, LicenseURLLength;
    CC_BEGIN();
    GET_L4 (SecretDataLength,                                               Secret Data Length);
    SKIP_XX(SecretDataLength,                                               Secret Data);
    GET_L4 (ProtectionTypeLength,                                           Protection Type Length);
    SKIP_LOCAL(ProtectionTypeLength,                                        Protection Type);
    GET_L4 (KeyIDLength,                                                    Key ID Length);
    SKIP_LOCAL(KeyIDLength,                                                 Key ID Type);
    GET_L4 (LicenseURLLength,                                               License URL Length);
    GET_LOCAL(LicenseURLLength, LicenseURL,                                 License URL);

    //Filling
    Fill(Stream_General, 0, "Encryption", LicenseURL);
}

//---------------------------------------------------------------------------
void File_Wm::Header_ExtendedContentEncryption()
{
    NAME("Extended Content Encryption");

    //Parsing
    int32u DataLength;
    CC_BEGIN();
    GET_L4 (DataLength,                                                     Data Length);
    SKIP_XX(DataLength,                                                     Data);
}

//---------------------------------------------------------------------------
void File_Wm::Header_DigitalSignature()
{
    NAME("Digital Signature");
}

//---------------------------------------------------------------------------
void File_Wm::Header_Padding()
{
    NAME("Padding");
}

//---------------------------------------------------------------------------
void File_Wm::Data()
{
    NAME("Data");

    //Parsing
    CC_BEGIN();
    SKIP_L8(                                                    File ID (part 1));
    SKIP_L8(                                                    File ID (part 2));
    SKIP_L8(                                                    Total Data Packets);
    SKIP_L1(                                                    Alignment);
    SKIP_L1(                                                    Packet Aligment);
    CC_END_CANBEMORE();

    Fill(Stream_General, 0, "HeaderSize", File_Offset+Buffer_Offset-24);
    Fill(Stream_General, 0, "DataSize", Element_Size+24);
}

//---------------------------------------------------------------------------
void File_Wm::SimpleIndex()
{
    NAME("SimpleIndex");
}

//---------------------------------------------------------------------------
void File_Wm::Index()
{
    NAME("Index");
}

//---------------------------------------------------------------------------
void File_Wm::MediaIndex()
{
    NAME("MediaIndex");
}

//---------------------------------------------------------------------------
void File_Wm::TimecodeIndex()
{
    NAME("TimecodeIndex");
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES

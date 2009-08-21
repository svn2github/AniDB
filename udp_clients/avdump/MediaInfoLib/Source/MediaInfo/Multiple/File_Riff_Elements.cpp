// File_Riff - Info for RIFF files
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
#ifdef MEDIAINFO_RIFF_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Riff.h"
#if defined(MEDIAINFO_MPEG4V_YES)
    #include "MediaInfo/Video/File_Mpeg4v.h"
#endif
#if defined(MEDIAINFO_MPEGA_YES)
    #include "MediaInfo/Audio/File_Mpega.h"
#endif
#if defined(MEDIAINFO_AC3_YES)
    #include "MediaInfo/Audio/File_Ac3.h"
#endif
#if defined(MEDIAINFO_DTS_YES)
    #include "MediaInfo/Audio/File_Dts.h"
#endif
#if defined(MEDIAINFO_AAC_YES)
    #include "MediaInfo/Audio/File_Aac.h"
#endif
#include <ZenLib/Utils.h>
#include <ZenLib/Ztring.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Riff::Element_Parse()
{
    #define ATOM_BEGIN \
        if (Level!=Element_Level) \
        { \
            Level++; \
            switch (Element_Name[Level]) \
            {

    #define SKIP(_ATOM) \
                case Riff::_ATOM : \
                           break; \

    #define ATOM(_ATOM) \
                case Riff::_ATOM : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_Next[Element_Level]-File_Offset<=Buffer_Size) \
                                _ATOM(); \
                            else \
                            { \
                                Buffer_Offset-=Element_HeaderSize; \
                                Element_Level--; \
                                PURGE(); \
                                return false; \
                            } \
                        } \
                        break; \

    #define ATOM_DEFAULT(_ATOM) \
                default : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_Next[Element_Level]-File_Offset<=Buffer_Size) \
                                _ATOM(); \
                            else \
                            { \
                                Buffer_Offset-=Element_HeaderSize; \
                                Element_Level--; \
                                PURGE(); \
                                return false; \
                            } \
                        } \
                        break; \

    #define LIST(_ATOM) \
                case Riff::_ATOM : \
                        if (Level==Element_Level) \
                            _ATOM(); \

    #define LIST_DEFAULT(_ATOM) \
                default : \
                        if (Level==Element_Level) \
                            _ATOM(); \

    #define ATOM_END \
            } \
        } \
        break; \

    //Special case : W3DI tags (unknown format!) are at the end of the file
    if (Element_Level==1 && File_Offset+Buffer_Size==File_Size && Buffer_Size>8)
    {
        if (CC4(Buffer+Buffer_Size-4)==Riff::W3DI)
        {
            int32u Size=LittleEndian2int32u(Buffer+Buffer_Size-8);
            if (Size<=Buffer_Size && Buffer_Offset==Buffer_Size-Size)
            {
                W3DI();
                return true;
            }
        }
    }

    //Init
    ELEMENT(Element_Level-1, Element_Name[Element_Level], Element_Size);
    Buffer_Offset+=Element_HeaderSize;
    int8u Level=0;
    List=Element_HeaderSize>8?true:false;

    ATOM_BEGIN
    LIST(AIFC)
    LIST(AVI_)
        ATOM_BEGIN
        ATOM(AVI__cset)
        LIST(AVI__exif)
            ATOM_BEGIN
            ATOM_DEFAULT(AVI__exif_xxxx)
            ATOM_END
        LIST(AVI__goog)
            ATOM_BEGIN
            ATOM(AVI__goog_GDAT)
            ATOM_END
        ATOM(AVI__GMET)
        LIST(AVI__hdlr)
            ATOM_BEGIN
            ATOM(AVI__hdlr_avih)
            ATOM(AVI__hdlr_JUNK)
            LIST(AVI__hdlr_strl)
                ATOM_BEGIN
                ATOM(AVI__hdlr_strl_indx)
                ATOM(AVI__hdlr_strl_JUNK)
                ATOM(AVI__hdlr_strl_strf)
                ATOM(AVI__hdlr_strl_strh)
                ATOM(AVI__hdlr_strl_strn)
                ATOM(AVI__hdlr_strl_vprp)
                ATOM_END
            LIST(AVI__hdlr_odml)
                ATOM_BEGIN
                ATOM(AVI__hdlr_odml_dmlh)
                ATOM_END
            ATOM_DEFAULT(AVI__hdlr_xxxx)
            ATOM_END
        ATOM(AVI__idx1)
        LIST(AVI__INFO)
            ATOM_BEGIN
            ATOM(AVI__INFO_JUNK)
            ATOM_DEFAULT(AVI__INFO_xxxx)
            ATOM_END
        ATOM(AVI__JUNK)
        LIST(AVI__movi)
            ATOM_BEGIN
            LIST(AVI__movi_rec_)
                ATOM_BEGIN
                ATOM_DEFAULT(AVI__movi_xxxx)
                ATOM_END
            ATOM_DEFAULT(AVI__movi_xxxx)
            ATOM_END
        ATOM_END
    LIST(AVIX) //OpenDML
        ATOM_BEGIN
        ATOM(AVIX_idx1)
        LIST(AVIX_movi)
            ATOM_BEGIN
            LIST(AVIX_movi_rec_)
                ATOM_BEGIN
                ATOM_DEFAULT(AVIX_movi_xxxx)
                ATOM_END
            ATOM_DEFAULT(AVIX_movi_xxxx)
            ATOM_END
        ATOM_END
    ATOM(IDVX)
    LIST(menu)
        ATOM_BEGIN
        ATOM_END
    LIST(PAL_)
        ATOM_BEGIN
        ATOM_END
    LIST(RDIB)
        ATOM_BEGIN
        ATOM_END
    LIST(RMID)
        ATOM_BEGIN
        ATOM_END
    LIST(RMMP)
        ATOM_BEGIN
        ATOM_END
    LIST(WAVE)
        ATOM_BEGIN
        ATOM(WAVE_fmt_)
        ATOM(WAVE_fact)
        //This atom is too large, but we need only first bytes
        case Riff::WAVE_data :
                if (Level==Element_Level)
                {
                    if (Buffer_Size-Buffer_Offset>=1000)
                    {
                        WAVE_data();
                        return true;
                    }
                    else
                    {
                        Buffer_Offset-=Element_HeaderSize;
                        Element_Level--;
                        PURGE();
                        return false;
                    }
                }
        ATOM_END
    default : ;
    }}
    FLUSH();

    //List
    if (List)
    {
        Read_Buffer_Continue();
        if (Element_Next[Element_Level]-File_Offset>Buffer_Size)
            return false; //Not enough bytes
    }

    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::AIFC()
{
    NAME("Format: AIFC");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "AIFF");
    File_GoTo=File_Size;
}

//---------------------------------------------------------------------------
void File_Riff::AVI_()
{
    NAME("Format: AVI");

    //Test if there is only one AVI
    if (Count_Get(Stream_General)>0)
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Info(0, "Problem", "2 AVI chunks, this is not normal");
        }

        //Problem, we go to the end
        File_GoTo=File_Size;
        return;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "AVI");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__cset()
{
    NAME ("Regional settings");

    //Parsing
    CC_BEGIN();
    SKIP_L2(                                                    CodePage); //TODO: take a look about IBM/MS RIFF/MCI Specification 1.0
    SKIP_L2(                                                    CountryCode);
    SKIP_L2(                                                    LanguageCode);
    SKIP_L2(                                                    Dialect);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__exif()
{
    NAME ("Exif (Exchangeable Image File Format)");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__exif_xxxx()
{
    NAME ("Value");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    //Filling
    switch (Element_Name[Element_Level])
    {
        case Riff::AVI__exif_ecor : Fill(Stream_General, 0, "Make", Value); break;
        case Riff::AVI__exif_emdl : Fill(Stream_General, 0, "Model", Value); break;
        case Riff::AVI__exif_emnt : Fill(Stream_General, 0, "MakerNotes", Value); break;
        case Riff::AVI__exif_erel : Fill(Stream_General, 0, "RelatedImageFile", Value); break;
        case Riff::AVI__exif_etim : Fill(Stream_General, 0, "Written_Date", Value); break;
        case Riff::AVI__exif_eucm : Fill(Stream_General, 0, "Comment", Value); break;
        case Riff::AVI__exif_ever : break; //Exif version
        default:                    Fill(Stream_General, 0, Element_Name[Element_Level], Value);
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__goog()
{
    NAME ("Google specific");

    //Filling
    Fill("Format", "GVI", Error, false, true);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__goog_GDAT()
{
    NAME ("Google datas");
}

//---------------------------------------------------------------------------
// Google Metadata
//
void File_Riff::AVI__GMET()
{
    NAME ("Google Metadatas");

    //Parsing
    Ztring Value; Value.From_Local((const char*)(Buffer+Buffer_Offset+0), Element_Size);
    ZtringListList List;
    List.Separator_Set(0, _T("\n"));
    List.Separator_Set(1, _T(":"));
    List.Max_Set(1, 2);
    List.Write(Value);

    //Details
    if (Config.Details_Get())
    {
        for (size_t Pos=0; Pos<List.size(); Pos++)
            Details_Add_Info(Pos, List(Pos, 0).To_Local().c_str(), List(Pos, 1));
    }

    //Filling
    for (size_t Pos=0; Pos<List.size(); Pos++)
    {
        if (List(Pos, 0)==_T("gvp_version"));
        if (List(Pos, 0)==_T("duration"));
        if (List(Pos, 0)==_T("title"))          Fill(Stream_General, 0, "Title", List(Pos, 1));
        if (List(Pos, 0)==_T("description"))    Fill(Stream_General, 0, "Title/More", List(Pos, 1));
        if (List(Pos, 0)==_T("url"))            Fill(Stream_General, 0, "Title/Url", List(Pos, 1));
        if (List(Pos, 0)==_T("docid"))          Fill(Stream_General, 0, "UniqueID", List(Pos, 1));
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr()
{
    NAME("Header");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_avih()
{
    NAME("AVI header");

    //Parsing
    int32u MicrosecPerFrame, Flags;
    CC_BEGIN();
    GET_L4 (MicrosecPerFrame,                                   MicrosecPerFrame);
    SKIP_L4(                                                    MaxBytesPerSec);
    SKIP_L4(                                                    PaddingGranularity);
    GET_L4 (Flags,                                              Flags);
        SKIP_FLAGS(Flags,  4,                                   HasIndex);
        SKIP_FLAGS(Flags,  5,                                   MustUseIndex);
        SKIP_FLAGS(Flags,  8,                                   IsInterleaved);
        SKIP_FLAGS(Flags,  9,                                   UseCKTypeToFindKeyFrames);
        SKIP_FLAGS(Flags, 11,                                   TrustCKType);
        SKIP_FLAGS(Flags, 16,                                   WasCaptureFile);
        SKIP_FLAGS(Flags, 17,                                   Copyrighted);
    GET_L4 (avih_TotalFrame,                                    TotalFrames);
    SKIP_L4(                                                    InitialFrames);
    SKIP_L4(                                                    StreamsCount);
    SKIP_L4(                                                    SuggestedBufferSize);
    SKIP_L4(                                                    Width);
    SKIP_L4(                                                    Height);
    SKIP_L4(                                                    Reserved);
    SKIP_L4(                                                    Reserved);
    SKIP_L4(                                                    Reserved);
    SKIP_L4(                                                    Reserved);
    CC_END_CANBEMORE();

    //Filling
    if (MicrosecPerFrame!=0)
        avih_FrameRate=((float32)1000000)/MicrosecPerFrame;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_JUNK()
{
    NAME("Garbage");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_odml()
{
    NAME("OpenDML");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_odml_dmlh()
{
    NAME("Header");

    //Parsing
    CC_BEGIN();
    GET_L4(TotalFrame,                                          GrandFrames);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl()
{
    NAME("Stream info");

    //Reset internal datas
    AVI__hdlr_strl_strh_StreamType=0;
    AVI__hdlr_strl_strh_Scale=0;
    AVI__hdlr_strl_strh_Rate=0;
    AVI__hdlr_strl_strh_Length=0;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_indx()
{
    NAME("Index");

    int16u Entry_Size, Entry_Count; int8u Type, SubType;
    CC_BEGIN();
    GET_L2 (Entry_Size,                                         LongsPerEntry); //Size of each entry in aIndex array
    GET_L1 (SubType,                                            IndexSubType); //Future use. must be 0
        SKIP_FLAGS(SubType, 0,                                  AVI_INDEX_2FIELD);
    GET_L1 (Type,                                               IndexType); //One of AVI_INDEX_* codes
        SKIP_FLAGS(Type, 1,                                     AVI_INDEX_OF_CHUNKS); //0 when each entry in aIndex array points to an index chunk, 1 when each entry in aIndex array points to a chunk in the file
        SKIP_FLAGS(Type, 7,                                     AVI_INDEX_IS_DATA); //When each entry is aIndex is really the data bIndexSubtype codes for INDEX_OF_CHUNKS
    GET_L4 (Entry_Count,                                        EntriesInUse); //Index of first unused member in aIndex array
    SKIP_C4(                                                    ChunkId); //FCC of what is indexed

    //Depends of size of structure...
          if (Entry_Size==2)
    {
        //Index of Indexes
             if (Type==0x00)
        {
            //Standard
                 if (SubType==0x00)
            {
                //Standard Index Chunk
                SKIP_L8 (                                       Offset);
                SKIP_L4 (                                       Reserved2);
                for (int16u Pos=0; Pos<Entry_Count; Pos++)
                {
                    SKIP_L4 (                                   Offset); //BaseOffset + this is absolute file offset
                    SKIP_L4 (                                   Size); //Bit 31 is set if this is NOT a keyframe
                }
            }
        }
        //Index of Chunks
        else if (Type==0x01)
        {
            //Index to Fields
                 if (SubType==0x01)
            {
                //Field Index Chunk
                SKIP_L8 (                                       Offset);
                SKIP_L4 (                                       Reserved2);
                for (int16u Pos=0; Pos<Entry_Count; Pos++)
                {
                    SKIP_L4 (                                   Offset); //BaseOffset + this is absolute file offset
                    SKIP_L4 (                                   Size); //Bit 31 is set if this is NOT a keyframe
                    SKIP_L4 (                                   OffsetField2); //Offset to second field
                }
            }
        }
    }

    //Super Index Chunk
    else if (Entry_Size==4)
    {
        //Index of Indexes
             if (Type==0x00)
        {
            //Standard or Index to Fields
                 if (SubType==0x00 || SubType==0x01)
            {
                SKIP_L4 (                                       Reserved0);
                SKIP_L4 (                                       Reserved1);
                SKIP_L4 (                                       Reserved2);
                for (int16u Pos=0; Pos<Entry_Count; Pos++)
                {
                    SKIP_L8 (                                   Offset);
                    SKIP_L4 (                                   Size); //Size of index chunk at this offset
                    SKIP_L4 (                                   Duration); //time span in stream ticks
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_JUNK()
{
    NAME("Garbage");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strd()
{
    NAME("Stream datas");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf()
{
    NAME("Stream format");

    //Coherancy test
    if (AVI__hdlr_strl_strh_StreamType==0)
        return;

    //Add the codec to the codec list
    Codec_External.push_back(NULL);
    Codec_External_PacketCount.push_back(0);
    Codec_External_StreamSize.push_back(0);
    Element_ID=(int8u)Codec_External_Codec.size();

    //Parse depending of kind of stream
    switch (AVI__hdlr_strl_strh_StreamType)
    {
        case Riff::AVI__hdlr_strl_strh_auds : AVI__hdlr_strl_strf_auds(); break;
        case Riff::AVI__hdlr_strl_strh_iavs : AVI__hdlr_strl_strf_iavs(); break;
        case Riff::AVI__hdlr_strl_strh_mids : AVI__hdlr_strl_strf_mids(); break;
        case Riff::AVI__hdlr_strl_strh_txts : AVI__hdlr_strl_strf_txts(); break;
        case Riff::AVI__hdlr_strl_strh_vids : AVI__hdlr_strl_strf_vids(); break;
        default :;
    }

    //Managing Bitrate if not present in element
    if (AVI__hdlr_strl_strf_auds_AvgBytesPerSec.size()!=Codec_External.size())
        AVI__hdlr_strl_strf_auds_AvgBytesPerSec.push_back(0);

    //Add the codec to the codec list
    Codec_External_Codec.push_back(Get(StreamKind_Last, StreamPos_Last, _T("Codec")));//After because we don't have the codec name
    Fill("ID", Element_ID);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds()
{
    NAME("Audio");

    //Parsing
    int32u FormatTag, Channels, SamplesPerSec, AvgBytesPerSec, BitsPerSample;
    CC_BEGIN();
    GET_L2 (FormatTag,                                          FormatTag);
    GET_L2 (Channels,                                           Channels);
    GET_L4 (SamplesPerSec,                                      SamplesPerSec);
    GET_L4 (AvgBytesPerSec,                                     AvgBytesPerSec);
    SKIP_L2(                                                    BlockAlign);
    GET_L2 (BitsPerSample,                                      BitsPerSample);

    //Filling
    Stream_Prepare(Stream_Audio);
    Ztring Codec; Codec.From_Number(FormatTag, 16);
    Codec.MakeUpperCase();
    Fill("Codec", Codec); //May be replaced by codec parser
    Fill("Codec/CC", Codec);
    Fill("Channel(s)", Channels!=5?Channels:6);
    Fill("SamplingRate", SamplesPerSec);
    Fill("BitRate", AvgBytesPerSec*8);
    if (BitsPerSample) Fill("Resolution", BitsPerSample);
    AVI__hdlr_strl_strf_auds_AvgBytesPerSec.push_back(AvgBytesPerSec); //Saving bitrate for each stream

    //Filling, From stream header
    if (AVI__hdlr_strl_strh_Rate)
    {
        int64u PlayTime=0;
        float32 FrameRate=((float)AVI__hdlr_strl_strh_Rate)/AVI__hdlr_strl_strh_Scale;
        if (FrameRate)
            PlayTime=float32_int64s((1000*(float)AVI__hdlr_strl_strh_Length)/FrameRate);
        if (avih_TotalFrame>0 //avih_TotalFrame is here because some files have a wrong Audio Playtime if TotalFrame==0 (which is a bug, of course!)
        && (avih_FrameRate==0 || PlayTime<((float)avih_TotalFrame)/avih_FrameRate*1000*1.10) //Some file have a nearly perfect header, except that the value is false, trying to detect it (false if 10% more than 1st video)
        && (avih_FrameRate==0 || PlayTime>((float)avih_TotalFrame)/avih_FrameRate*1000*0.90)) //Some file have a nearly perfect header, except that the value is false, trying to detect it (false if 10% less than 1st video)
            Fill("PlayTime", PlayTime);
        else
            Fill("Coherency/PlayTime", PlayTime);
    }

    //Creating the parser
    Codec.MakeUpperCase();
         if (0);
    #if defined(MEDIAINFO_MPEGA_YES)
    else if (Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("MPEG-1"))==0)
        Codec_External[Element_ID]=new File_Mpega;
    #endif
    #if defined(MEDIAINFO_AC3_YES)
    else if (Codec==_T("2000"))
        Codec_External[Element_ID]=new File_Ac3;
    #endif
    #if defined(MEDIAINFO_DTS_YES)
    else if (Codec==_T("2001") || Codec==_T("1")) //Some DTS streams are coded "1"
        Codec_External[Element_ID]=new File_Dts;
    #endif
    #if defined(MEDIAINFO_AAC_YES)
    else if (Codec==_T("AAC") || Codec==_T("FF"))
        Codec_External[Element_ID]=new File_Aac;
    #endif

    //Options
    if (Stream_Pos+2>=Element_Size)
        return;
        
    //Parsing
    int32u Option_Size;
    GET_L2 (Option_Size,                                        cbSize);

    //Filling
    if (Option_Size>0)
    {
        FLUSH();
        int32u Element_Size_Old=Element_Size;
        Element_Size=Option_Size;
        Buffer_Offset+=18;
             if (0);
        else if (Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("MPEG-1"))==0)
            AVI__hdlr_strl_strf_auds_Mpega();
        else if (Codec==_T("AAC") || Codec==_T("FF"))
            AVI__hdlr_strl_strf_auds_Aac();
        Buffer_Offset-=18;
        Element_Size=Element_Size_Old;
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Mpega()
{
    ELEMENT(Element_Level, "", Element_Size);
    NAME ("MPEG Audio options");

    //Parsing
    CC_BEGIN();
    SKIP_L2(                                                    ID);
    SKIP_L4(                                                    Flags);
    SKIP_L2(                                                    BlockSize);
    SKIP_L2(                                                    FramesPerBlock);
    SKIP_L2(                                                    CodecDelay);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_auds_Aac()
{
    ELEMENT(Element_Level, "", Element_Size);
    NAME ("AAC options");

    //Parsing
    #if defined(MEDIAINFO_AAC_YES)
        File_Aac MI;
        Open_Buffer_Init(&MI, File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(&MI, Buffer+Buffer_Offset, Element_Size);
        Open_Buffer_Finalize(&MI);
        Merge(MI, StreamKind_Last, 0, StreamPos_Last);
    #endif

}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_iavs()
{
    //Standard video header before Iavs?
    int32u Iavs_Offset=0;
    if (Element_Size==72)
    {
        AVI__hdlr_strl_strf_vids();
        Iavs_Offset=40;
    }
    else
    {
        NAME("Interleaved Audio/Video")
    }

    //Parsing
    CC_BEGIN();
    Stream_Pos=Iavs_Offset; //Positioning offset at the right place
    SKIP_L4(                                                    DVAAuxSrc);
    SKIP_L4(                                                    DVAAuxCtl);
    SKIP_L4(                                                    DVAAuxSrc1);
    SKIP_L4(                                                    DVAAuxCtl1);
    SKIP_L4(                                                    DVVAuxSrc);
    SKIP_L4(                                                    DVVAuxCtl);
    SKIP_L4(                                                    DVReserved);
    CC_END();

    //Filling - Video
    if (Iavs_Offset==0)
    {
        Stream_Prepare(Stream_Video);
        Fill("Codec", AVI__hdlr_strl_strh_Codec); //May be replaced by codec parser
        Fill("Codec/CC", AVI__hdlr_strl_strh_Codec);
    }

    //FrameRate
    float FrameRate=0;
    if (AVI__hdlr_strl_strh_Scale!=0)
    {
        FrameRate=((float)AVI__hdlr_strl_strh_Rate)/AVI__hdlr_strl_strh_Scale;
        if (avih_FrameRate==0)
            avih_FrameRate=FrameRate; //Keep it in Memory
        Fill("FrameRate", FrameRate, 3);
        if (FrameRate)
            Fill("PlayTime", (1000*(float)AVI__hdlr_strl_strh_Length)/FrameRate);
    }

    //Fill datas depend of the codec
         if (AVI__hdlr_strl_strh_Codec==_T("dvsd") || AVI__hdlr_strl_strh_Codec==_T("dvsl"))
    {
                                    Fill("Width",  720);
             if (FrameRate==25.000) Fill("Height", 576);
        else if (FrameRate==29.970) Fill("Height", 480);
        Fill("AspectRatio", ((float)4)/3);
    }
    else if (AVI__hdlr_strl_strh_Codec==_T("dvhd"))
    {
                                    Fill("Width",  1440);
             if (FrameRate==25.000) Fill("Height", 1152);
        else if (FrameRate==30.000) Fill("Height",  960);
        Fill("AspectRatio", ((float)4)/3);
    }

    //Filling - Audio
    if (Iavs_Offset==0)
    {
        Stream_Prepare(Stream_Audio);
        Fill("Codec", AVI__hdlr_strl_strh_Codec); //FormatTag, may be replaced by codec parser
        Fill("Codec/CC", AVI__hdlr_strl_strh_Codec); //FormatTag
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_mids()
{
    NAME("Midi");

    //Filling
    Stream_Prepare(Stream_Audio);
    Fill("Codec", "Midi");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_txts()
{
    NAME("Text");

    //Filling
    Stream_Prepare(Stream_Text);
    Fill("Codec", "SRT");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strf_vids()
{
    NAME("Video");

    //Parsing
    int32u Resolution, Compression, Width, Height;
    CC_BEGIN();
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
    CC_END_CANBEMORE();

    //Filling
    Ztring Codec;
    if (((Compression&0x000000FF)>=0x00000020 && (Compression&0x000000FF)<=0x0000007E
      && (Compression&0x0000FF00)>=0x00002000 && (Compression&0x0000FF00)<=0x00007E00
      && (Compression&0x00FF0000)>=0x00200000 && (Compression&0x00FF0000)<=0x007E0000
      && (Compression&0xFF000000)>=0x20000000 && (Compression&0xFF000000)<=0x7E000000)
     ||   Compression==0x00000000
       ) //Sometimes this value is wrong, we have to test this
    {
        //Divx.com hack for subtitle, this is a text stream in a DivX container
        if (Compression==CC4("DXSB"))
        {
            Fill(Stream_General, 0, "Format", "");
            Fill(Stream_General, 0, "Format", "DivX");
            Stream_Prepare(Stream_Text);
        }
        else
            Stream_Prepare(Stream_Video);

        //Filling
        Codec.From_UTF8((const char*)(Buffer+Buffer_Offset+16), 4);
        if (Compression==0x00000000)
            Fill("Codec", "RGB"); //Raw RGB, not handled by automatic codec mapping
        else
        {
            Fill("Codec", Codec); //FormatTag, may be replaced by codec parser
            Fill("Codec/CC", Codec); //FormatTag
        }
        Fill("Width", LittleEndian2int32u(Buffer+Buffer_Offset+4)); //Width
        Fill("Height", LittleEndian2int32u(Buffer+Buffer_Offset+8)); //Height
        Fill("Resolution", Resolution);
    }
    else
    {
        //Some Stream headers are broken, must use AVISTREAMINFOA structure instead of AVIFILEINFOA
        Stream_Prepare(Stream_Video);
        Codec=AVI__hdlr_strl_strh_Codec;
        Fill("Codec", AVI__hdlr_strl_strh_Codec); //StreamHeader fcchandler, may be replaced by codec parser
        Fill("Codec/CC", AVI__hdlr_strl_strh_Codec); //StreamHeader fcchandler
        Fill("Width", AVI__hdlr_strl_strh_Width?AVI__hdlr_strl_strh_Width:LittleEndian2int32u(Buffer+Buffer_Offset+4)); //StreamHeader Width if exists
        Fill("Height", AVI__hdlr_strl_strh_Height?AVI__hdlr_strl_strh_Height:LittleEndian2int32u(Buffer+Buffer_Offset+8)); //StreamHeader Height if exists
    }

    //Don't calculate video information for DXSB, this is text
    if (Compression==CC4("DXSB"))
        return;

    //Framerate, Playtime
    if (AVI__hdlr_strl_strh_Scale!=0)
    {
        Fill("FrameCount", AVI__hdlr_strl_strh_Length);
        float32 FrameRate=((float)AVI__hdlr_strl_strh_Rate)/AVI__hdlr_strl_strh_Scale;
        if (avih_FrameRate==0)
            avih_FrameRate=FrameRate; //Keep it in Memory
        Fill("FrameRate", FrameRate, 3);
        if (FrameRate)
            Fill("PlayTime", (int32u)(1000*(float)AVI__hdlr_strl_strh_Length)/FrameRate);
    }

    //Creating the parser
         if (0);
    #if defined(MEDIAINFO_MPEG4V_YES)
    else if (Config.Codec_Get(Codec, InfoCodec_KindofCodec).find(_T("MPEG-4"))==0)
        Codec_External[Element_ID]=new File_Mpeg4v;
    #endif
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strh()
{
    NAME("Stream header");

    //Parsing
    int32u Left, Top, Right, Bottom;
    CC_BEGIN();
    GET_C4 (AVI__hdlr_strl_strh_StreamType,                     fccType);
    GET_LOCAL(4, AVI__hdlr_strl_strh_Codec,                     fccHandler);
    SKIP_L4(                                                    Flags);
    SKIP_L2(                                                    Priority);
    SKIP_L2(                                                    Language);
    SKIP_L4(                                                    InitialFrames);
    GET_L4 (AVI__hdlr_strl_strh_Scale,                          Scale);
    GET_L4 (AVI__hdlr_strl_strh_Rate,                           Rate); //Rate/Scale is stream tick rate in ticks/sec
    SKIP_L4(                                                    Start);
    GET_L4 (AVI__hdlr_strl_strh_Length,                         Length);
    SKIP_L4(                                                    SuggestedBufferSize);
    SKIP_L4(                                                    Quality);
    SKIP_L4(                                                    SampleSize);
    GET_L2 (Left,                                               Frame_Left);
    GET_L2 (Top,                                                Frame_Top);
    GET_L2 (Right,                                              Frame_Right);
    GET_L2 (Bottom,                                             Frame_Bottom);
    CC_END_CANBEMORE();

    //Filling
    AVI__hdlr_strl_strh_Width=Right-Left;
    AVI__hdlr_strl_strh_Height=Bottom-Top;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_strn()
{
    NAME("Stream name");

    //Parsing
    Ztring Title;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Title,                              StreamName);
    CC_END();

    //Filling
    Fill("Title", Title);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_strl_vprp()
{
    NAME("Video properties");

    //Parsing
    int32u FieldPerFrame;
    CC_BEGIN();
    SKIP_L4(                                                    VideoFormatToken);
    SKIP_L4(                                                    VideoStandard);
    SKIP_L4(                                                    VerticalRefreshRate);
    SKIP_L4(                                                    HTotalInT);
    SKIP_L4(                                                    VTotalInLines);
    SKIP_L4(                                                    FrameAspectRatio);
    SKIP_L4(                                                    FrameWidthInPixels);
    SKIP_L4(                                                    FrameHeightInLines);
    GET_L4 (FieldPerFrame,                                      FieldPerFrame);
    for (int32u Pos=0; Pos<FieldPerFrame; Pos++)
    {
        SKIP_L4(                                                CompressedBMHeight);
        SKIP_L4(                                                CompressedBMWidth);
        SKIP_L4(                                                ValidBMHeight);
        SKIP_L4(                                                ValidBMWidth);
        SKIP_L4(                                                ValidBMXOffset);
        SKIP_L4(                                                ValidBMYOffset);
        SKIP_L4(                                                VideoXOffsetInT);
        SKIP_L4(                                                VideoYValidStartLine);
    }
    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__hdlr_xxxx()
{
    AVI__INFO_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__idx1()
{
    NAME("Index (old)");
    DETAILLEVEL_SET(1.0);

    //Parsing
    int8u StreamID1, StreamID2;
    int32u Flags, Size;
    CC_BEGIN();
    while (Stream_Pos<Element_Size)
    {
        GET_C1 (StreamID1,                                      StreamID - part 1);
        GET_C1 (StreamID2,                                      StreamID - part 2);
        SKIP_LOCAL(2,                                           StreamKind);
        GET_L4 (Flags,                                          Flags);
            SKIP_FLAGS(Flags, 0,                                NoTime);
            SKIP_FLAGS(Flags, 1,                                Lastpart);
            SKIP_FLAGS(Flags, 2,                                Firstpart);
            SKIP_FLAGS(Flags, 3,                                Midpart);
            SKIP_FLAGS(Flags, 4,                                KeyFrame);
        SKIP_L4(                                                ChunkOffset);
        GET_L4 (Size,                                           ChunkLength);

        //Filling
        size_t StreamID=(StreamID1-'0')*10+(StreamID2-'0')*1;
        if (StreamID<Codec_External_StreamSize.size() && Size>8)
            Codec_External_StreamSize[StreamID]+=Size-8;
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO()
{
    NAME("Tags");
}

//---------------------------------------------------------------------------
void File_Riff::AVI__INFO_JUNK()
{
    NAME("Garbage");
}

//---------------------------------------------------------------------------
// List of information atoms
// Name                             X bytes, Pos=0
//
void File_Riff::AVI__INFO_xxxx()
{
    NAME("Value");

    //Parsing
    Ztring Value;
    CC_BEGIN();
    GET_LOCAL(Element_Size, Value,                              Value);
    CC_END();

    switch (Element_Name[Element_Level])
    {
        case Riff::AVI__INFO_IARL : Fill(Stream_General, 0, "Archival_Location", Value); break;
        case Riff::AVI__INFO_IART : Fill(Stream_General, 0, "Director", Value); break;
        case Riff::AVI__INFO_ICMS : Fill(Stream_General, 0, "CommissionedBy", Value); break;
        case Riff::AVI__INFO_ICMT : Fill(Stream_General, 0, "Comment", Value); break;
        case Riff::AVI__INFO_ICOP : Fill(Stream_General, 0, "Copyright", Value); break;
        case Riff::AVI__INFO_ICRD : Fill(Stream_General, 0, "Written_Date", Ztring().Date_From_String((const char*)(Buffer+Buffer_Offset), Element_Size)); break;
        case Riff::AVI__INFO_ICRP : Fill(Stream_General, 0, "Cropped", Value); break;
        case Riff::AVI__INFO_IDIM : Fill(Stream_General, 0, "Dimensions", Value); break;
        case Riff::AVI__INFO_IDIT : Fill(Stream_General, 0, "Mastered_Date", Ztring().Date_From_String((const char*)(Buffer+Buffer_Offset), Element_Size)); break;
        case Riff::AVI__INFO_IDPI : Fill(Stream_General, 0, "DotsPerInch", Value); break;
        case Riff::AVI__INFO_IENG : Fill(Stream_General, 0, "Engineer", Value); break;
        case Riff::AVI__INFO_IGNR : Fill(Stream_General, 0, "Genre", Value); break;
        case Riff::AVI__INFO_IKEY : Fill(Stream_General, 0, "Keywords", Value); break;
        case Riff::AVI__INFO_ILGT : Fill(Stream_General, 0, "Ligthness", Value); break;
        case Riff::AVI__INFO_ILNG : Fill(Stream_Audio  , 0, "Language", Value); break;
        case Riff::AVI__INFO_IMED : Fill(Stream_General, 0, "Medium", Value); break;
        case Riff::AVI__INFO_INAM : Fill(Stream_General, 0, "Title", Value); break;
        case Riff::AVI__INFO_IPLT : Fill(Stream_General, 0, "NumColors", Value); break;
        case Riff::AVI__INFO_IPRD : Fill(Stream_General, 0, "Product", Value); break;
        case Riff::AVI__INFO_IRTD : Fill(Stream_General, 0, "LawRating", Value); break;
        case Riff::AVI__INFO_ISBJ : Fill(Stream_General, 0, "Subject", Value); break;
        case Riff::AVI__INFO_ISFT : Fill(Stream_General, 0, "Encoded_Application", Value); break;
        case Riff::AVI__INFO_ISHP : Fill(Stream_General, 0, "Sharpness", Value); break;
        case Riff::AVI__INFO_ISRC : Fill(Stream_General, 0, "Encoded_Original/DistributedBy", Value); break;
        case Riff::AVI__INFO_ISRF : Fill(Stream_General, 0, "Encoded_Original", Value); break;
        case Riff::AVI__INFO_ITCH : Fill(Stream_General, 0, "EncodedBy", Value); break;
        default: Fill(Stream_General, 0, Element_Name[Element_Level], Value);
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__JUNK()
{
    NAME("Garbage"); //library defined size for padding, often used to store library name
    INTEGRITY_GENERAL(0);

    //Detect DivX files
         if (CC5(Buffer+Buffer_Offset)==CC5("DivX "))
    {
        Fill(Stream_General, 0, "Format", "");
        Fill(Stream_General, 0, "Format", "DivX");
    }
    //MPlayer
    else if (CC8(Buffer+Buffer_Offset)==CC8("[= MPlay") && Get(Stream_General, 0, _T("Encoded_Library")).empty())
        Fill(Stream_General, 0, "Encoded_Library", "MPlayer");
    //FFMpeg broken files detection
    else if (CC8(Buffer+Buffer_Offset)==CC8("odmldmlh"))
        TotalFrame=0; //this is not normal to have this string in a JUNK block!!! and in files tested, in this case TotalFrame is broken too
    //VirtualDubMod
    else if (CC8(Buffer+Buffer_Offset)==CC8("INFOISFT"))
    {
        int32u Size=LittleEndian2int32u(Buffer+Buffer_Offset+8);
        if (Size>Element_Size-12)
            Size=Element_Size-12;
        Fill(Stream_General, 0, "Encoded_Library", (const char*)(Buffer+Buffer_Offset+12), Size);
    }
    else if (CC8(Buffer+Buffer_Offset)==CC8("INFOIENG"))
    {
        int32u Size=LittleEndian2int32u(Buffer+Buffer_Offset+8);
        if (Size>Element_Size-12)
            Size=Element_Size-12;
        Fill(Stream_General, 0, "Encoded_Library", (const char*)(Buffer+Buffer_Offset+12), Size);
    }
    //Other libraries?
    else if (CC1(Buffer+Buffer_Offset)>=CC1("A") && CC1(Buffer+Buffer_Offset)<=CC1("z") && Get(Stream_General, 0, _T("Encoded_Library")).empty())
        Fill(Stream_General, 0, "Encoded_Library", (const char*)(Buffer+Buffer_Offset), Element_Size);
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi()
{
    NAME("Datas");

    //Filling
    AVI__movi_Size+=Element_Size;

    //We must parse moov?
    bool Skip=true;
    for (size_t Pos=0; Pos<Codec_External_PacketCount.size(); Pos++)
    {
        if (Codec_External[Pos]!=NULL && Codec_External_PacketCount[Pos]<Error)
            Skip=false;
    }
    if (Skip)
    {
        //Jumping
        File_GoTo=Element_Next[Element_Level-1];
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_rec_()
{
    NAME("Syncronisation");

    Rec_Present=true;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_rec__xxxx()
{
    AVI__movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx()
{
    //ID
    char ID1=(char)((Element_Name[Element_Level]&0xFF000000)>>24);
    char ID2=(char)((Element_Name[Element_Level]&0x00FF0000)>>16);
    if (ID1<'0' || ID1>'9' || ID2<'0' || ID2>'9')
        return;
    Element_ID=(ID1-'0')*10+(ID2-'0');
    if ((size_t)Element_ID>=Codec_External.size())
        return;

    //Details
    if (Config.Details_Get())
    {
        switch (Element_Name[Element_Level]&0x0000FFFF) //2 last bytes
        {
            case Riff::AVI__movi_xxxx___db : NAME("Video"); break;
            case Riff::AVI__movi_xxxx___dc : NAME("Video"); break;
            case Riff::AVI__movi_xxxx___tx : NAME("Text"); break;
            case Riff::AVI__movi_xxxx___wb : NAME("Audio"); break;
            default :                        NAME("Data"); break;
        }
    }

    //Finished?
    if (Codec_External[Element_ID]!=NULL && Codec_External_PacketCount[Element_ID]!=Error)
    {
        Codec_External_PacketCount[Element_ID]++;

        //Details
        if (Config.Details_Get())
        {
            Details_Add_Element(Ztring(_T("Packet number ")+Ztring::ToZtring(Codec_External_PacketCount[Element_ID])));
        }

        //Parsing
        Open_Buffer_Init(Codec_External[Element_ID], File_Size, File_Offset+Buffer_Offset);
        Open_Buffer_Continue(Codec_External[Element_ID], Buffer+Buffer_Offset, Element_Size);

        //Some specific stuff
        switch (Element_Name[Element_Level]&0x0000FFFF) //2 last bytes
        {
            case Riff::AVI__movi_xxxx___db : AVI__movi_xxxx___db(); break;
            case Riff::AVI__movi_xxxx___dc : AVI__movi_xxxx___dc(); break;
            case Riff::AVI__movi_xxxx___tx : AVI__movi_xxxx___tx(); break;
            case Riff::AVI__movi_xxxx___wb : AVI__movi_xxxx___wb(); break;
            default :                        AVI__movi_xxxx___xx(); break;
        }
    }
    else
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Element("Skipped");
        }
    }

    //We must always parse moov?
    bool Skip=true;
    for (size_t Pos=0; Pos<Codec_External_PacketCount.size(); Pos++)
    {
        if (Codec_External[Pos]!=NULL && Codec_External_PacketCount[Pos]<Error)
            Skip=false;
    }
    if (Skip)
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Error("----------------------------------------");
            Details_Add_Error("---   Jumping to end of movi chunk   ---");
            Details_Add_Error("----------------------------------------");
        }

        //Jumping
        if (Rec_Present)
            File_GoTo=Element_Next[Element_Level-2];
        else
            File_GoTo=Element_Next[Element_Level-1];
    }
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___db()
{
    AVI__movi_xxxx___dc();
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___dc()
{
    //Finalize (if requested)
    if (Codec_External[Element_ID]->File_Offset==File_Size || Codec_External_PacketCount[Element_ID]>=300)
        Codec_External_PacketCount[Element_ID]=Error;

    //Specific
    #if defined(MEDIAINFO_MPEG4V_YES)
        if (Config.Codec_Get(Codec_External_Codec[Element_ID], InfoCodec_KindofCodec).find(_T("MPEG-4"))==0 && ((File_Mpeg4v*)Codec_External[Element_ID])->RIFF_VOP_Count>1)
            Codec_External_PacketCount[Element_ID]=Error;
    #endif
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___tx()
{
    NAME("Text");

    //Parsing
    int32u Name_Size;
    Ztring Value;
    CC_BEGIN();
    SKIP_C4(                                                    GAB2);
    SKIP_L1(                                                    Zero);
    SKIP_L2(                                                    CodePage); //2=Unicode
    GET_L4 (Name_Size,                                          Name_Size);
    SKIP_UTF16L(Name_Size,                                      Name);
    SKIP_L2(                                                    Four);
    SKIP_L4(                                                    File_Size);
    //Entire SRT/SS file...
    CC_END_CANBEMORE();

    //Skip it
    Codec_External_PacketCount[Element_ID]=Error;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___wb()
{
    //Finalize (if requested)
    if (Codec_External[Element_ID]->File_Offset==File_Size || Codec_External_PacketCount[Element_ID]>=1000)
        Codec_External_PacketCount[Element_ID]=Error;
}

//---------------------------------------------------------------------------
void File_Riff::AVI__movi_xxxx___xx()
{
    //Skip it
    Codec_External_PacketCount[Element_ID]=true;
}

//---------------------------------------------------------------------------
void File_Riff::AVIX()
{
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_idx1()
{
    AVI__idx1();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi()
{
    AVI__movi();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_rec_()
{
    AVI__movi_rec_();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_rec__xxxx()
{
    AVIX_movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::AVIX_movi_xxxx()
{
    AVI__movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::IDVX()
{
    NAME("Tags");
}

//---------------------------------------------------------------------------
void File_Riff::menu()
{
    NAME("DivX Menu");

    //Filling
    Stream_Prepare(Stream_Menu);
    Fill("Codec", "DivX");
}

//---------------------------------------------------------------------------
void File_Riff::PAL_()
{
    NAME("Format: RIFF Palette");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "RIFF Palette");
}

//---------------------------------------------------------------------------
void File_Riff::RDIB()
{
    NAME("Format: RIFF DIB");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "RIFF DIB");
}

//---------------------------------------------------------------------------
void File_Riff::RMID()
{
    NAME("Format: RIFF MIDI");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "RIFF MIDI");
}

//---------------------------------------------------------------------------
void File_Riff::RMMP()
{
    NAME("Format: RIFF MMP");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "RIFF MMP");
}

//---------------------------------------------------------------------------
void File_Riff::WAVE()
{
    NAME("Format: Wave");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "WAV");
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_data()
{
    NAME("Raw datas");

    //Parsing
    Element_Name[Element_Level]=CC4("00wb");
    Element_Size=(int32u)(Buffer_Size-Buffer_Offset);
    AVI__movi_xxxx();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_fact()
{
    NAME("Sample count");

    //Parsing
    CC_BEGIN();
    SKIP_L2(                                                    SamplesCount);
    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Riff::WAVE_fmt_()
{
    AVI__hdlr_strl_strh_Rate=0;

    //Add the codec to the codec list
    Codec_External.push_back(NULL);
    Codec_External_PacketCount.push_back(0);
    Codec_External_StreamSize.push_back(0);
    Element_ID=(int8u)Codec_External_Codec.size();

    AVI__hdlr_strl_strf_auds();

    //Add the codec to the codec list
    Codec_External_Codec.push_back(Get(StreamKind_Last, StreamPos_Last, _T("Codec")));//After because we don't have the codec name
    Fill("ID", Element_ID);
}

//---------------------------------------------------------------------------
void File_Riff::W3DI()
{
    //Constructing Element
    Element_Size=(int32u)(File_Size-(File_Offset+Buffer_Offset)-8);
    Element_HeaderSize=0;
    Element_Next[Element_Level]=File_Size;
    Details_Add_Element(Element_Level-1, "IDVX tags (Out of specs!)");

    //Parsing
    int32u Size=Element_Size;
    Ztring Title, Artist, Album, Unknown, Genre, Comment;
    int32u TrackPos;
    CC_BEGIN();
    GET_LOCAL(Size, Title,                                      Title);
    Stream_Pos=(int32u)Title.size();
    Size-=(int32u)Title.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_LOCAL(Size, Artist,                                     Artist);
    Stream_Pos=(int32u)Title.size()+1+(int32u)Artist.size();
    Size-=(int32u)Artist.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_LOCAL(Size, Album,                                      Album);
    Stream_Pos=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size();
    Size-=(int32u)Album.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_LOCAL(Size, Unknown,                                    Unknown);
    Stream_Pos=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size();
    Size-=(int32u)Unknown.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_LOCAL(Size, Genre,                                      Genre);
    Stream_Pos=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size()+1+(int32u)Genre.size();
    Size-=(int32u)Genre.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_LOCAL(Size, Comment,                                    Comment);
    Stream_Pos=(int32u)Title.size()+1+(int32u)Artist.size()+1+(int32u)Album.size()+1+(int32u)Unknown.size()+1+(int32u)Genre.size()+1+(int32u)Comment.size();
    Size-=(int32u)Comment.size();
    if (Size==0) return;
    SKIP_L1(                                                    Zero); Size--; //NULL char
    GET_L4 (TrackPos,                                           Track_Position);
    CC_END_CANBEMORE();

    //Filling
    Fill(Stream_General, 0, "Track", Title);
    Fill(Stream_General, 0, "Performer", Artist);
    Fill(Stream_General, 0, "Album", Album);
    Fill(Stream_General, 0, "Unknown", Unknown);
    Fill(Stream_General, 0, "Genre", Genre);
    Fill(Stream_General, 0, "Comment", Comment);
    Fill(Stream_General, 0, "Track/Position", TrackPos);

    FLUSH();
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_RIFF_YES

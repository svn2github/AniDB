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
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
File_Riff::~File_Riff()
{
    for (size_t Pos=0; Pos<Codec_External.size(); Pos++)
        delete Codec_External[Pos];
}

//---------------------------------------------------------------------------
void File_Riff::Read_Buffer_Init()
{
    //Buffer
    Element_Level=0;
    Element_Name[0]=0;
    Element_Next[0]=(int64u)-1; //Unlimited
    Element_ID=0;
    AVI__movi_Size=0;
    Rec_Present=false;
    TotalFrame=0;
    Trusted=10;
}

//---------------------------------------------------------------------------
void File_Riff::Read_Buffer_Continue()
{
    while (Buffer_Parse());
}

void File_Riff::Read_Buffer_Finalize ()
{
    //Finalizing streams
    for (Element_ID=0; (size_t)Element_ID<Codec_External.size(); Element_ID++)
    {
        if (Codec_External[Element_ID]!=NULL)
        {
            //Details
            if (Config.Details_Get())
            {
                Details_Add_Info(0, "Stream", Element_ID);
                ELEMENT_BEGIN();
            }

            //Finalizing and Merging (except Video codec and 120 fps hack)
            Open_Buffer_Finalize(Codec_External[Element_ID]);
            SetLastByID(Element_ID);
            Ztring Codec_Temp;
            Ztring FrameRate_Temp;
            if (StreamKind_Last==Stream_Video)
            {
                Codec_Temp=Get(StreamKind_Last, StreamPos_Last, _T("Codec")); //We want to keep the 4CC of AVI
                FrameRate_Temp=Get(StreamKind_Last, StreamPos_Last, _T("FrameRate")); //We want to keep the FrameRate of AVI 120 fps
            }
            Merge(*Codec_External[Element_ID], StreamKind_Last, 0, StreamPos_Last);
            if (StreamKind_Last==Stream_Video)
            {
                Fill("Codec", Codec_Temp, true);

                //120 fps hack
                int32u FrameRate=FrameRate_Temp.To_int32u();
                if (FrameRate>119 && FrameRate<121)
                {
                    Fill("FrameRate", FrameRate_Temp, true);
                    Fill("FrameRate/String", Config.Language_Get(FrameRate_Temp+_T(" (24/30)"), _T(" fps")));
                    Fill("FrameRate_Min", 24, true);
                    Fill("FrameRate_Max", 30, true);
                }
            }
            if (Codec_External_StreamSize[Element_ID]>0)
                Fill("StreamSize", Codec_External_StreamSize[Element_ID]);
            Codec_External_PacketCount[Element_ID]=Error;

            //Specific
            #if defined(MEDIAINFO_MPEG4V_YES)
                if (StreamKind_Last==Stream_Video && Config.Codec_Get(Codec_External_Codec[Element_ID], InfoCodec_KindofCodec).find(_T("MPEG-4"))==0)
                {
                    if (((File_Mpeg4v*)Codec_External[Element_ID])->RIFF_VOP_Count_Max>1)
                    {
                        Fill("Codec_Settings/PacketBitStream", "Yes");
                        Fill("Codec_Settings", "Packed Bitstream");
                    }
                    else
                        Fill("Codec_Settings/PacketBitStream", "No");
                }
            #endif
            #if defined(MEDIAINFO_MPEGA_YES)
                if (StreamKind_Last==Stream_Audio && Config.Codec_Get(Codec_External_Codec[Element_ID], InfoCodec_KindofCodec).find(_T("MPEG-1"))==0)
                {
                    if (((File_Mpega*)Codec_External[Element_ID])->Delay>100 && AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]!=0)
                        Fill("Delay", ((File_Mpega*)Codec_External[Element_ID])->Delay*1000/AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]);
                }
            #endif
            #if defined(MEDIAINFO_AC3_YES)
                if (StreamKind_Last==Stream_Audio && Codec_External_Codec[Element_ID]==_T("2000"))
                {
                    if (((File_Ac3*)Codec_External[Element_ID])->Delay>100 && AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]!=0)
                        Fill("Delay", ((File_Ac3*)Codec_External[Element_ID])->Delay*1000/AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]);
                }
            #endif
            #if defined(MEDIAINFO_DTS_YES)
                if (StreamKind_Last==Stream_Audio && Codec_External_Codec[Element_ID]==_T("2001") || Codec_External_Codec[Element_ID]==_T("1"))
                {
                    if (((File_Dts*)Codec_External[Element_ID])->Delay>100 && AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]!=0)
                        Fill("Delay", ((File_Dts*)Codec_External[Element_ID])->Delay*1000/AVI__hdlr_strl_strf_auds_AvgBytesPerSec[Element_ID]);
                }
            #endif

            //Details
            if (Config.Details_Get())
                ELEMENT_END();

            //Delete parsers
            delete Codec_External[Element_ID]; Codec_External[Element_ID]=NULL;
        }
    }

    //Some work on the first video stream
    if (!Video.empty())
    {
        //ODML
        if (TotalFrame!=0)
        {
            Fill(Stream_Video, 0, "FrameCount", "");
            Fill(Stream_Video, 0, "FrameCount", TotalFrame);

            float32 FrameRate=Get(Stream_Video, 0, _T("FrameRate")).To_float32();
            if (FrameRate!=0)
            {
                Fill(Stream_Video, 0, "PlayTime", "");
                Fill(Stream_Video, 0, "PlayTime", (int64u)(TotalFrame*1000/FrameRate));
            }
        }

        //Calculation of missing information
        int64u PlayTime=Get(Stream_Video, 0, _T("PlayTime")).To_int64u();
        if (PlayTime>0)
        {
            int32u BitRate=(int32u)(AVI__movi_Size*8*1000/PlayTime);
            Fill(Stream_General, 0, "BitRate", BitRate);
            if (BitRate>100000) //BitRate is > 100 000, to avoid strange behavior
            {
                int32u VideoBitRate=BitRate-5000; //5000 bps because of AVI OverHead (from GordianKnot)
                for (size_t Pos=0; Pos<Audio.size(); Pos++)
                {
                    int OverHead=8000; //Classic OverHead (for MP3) (from GordianKnot)
                    if (Audio[Pos](_T("Codec"))==_T("2000")) //AC3
                        OverHead=4750; //OverHead of AC3 in AVI (from GordianKnot)
                    VideoBitRate-=Audio[Pos](_T("BitRate")).To_int32s()+OverHead;
                }
                Fill(Stream_Video, 0, "BitRate", VideoBitRate);
            }
        }
    }

    //Rec
    if (Rec_Present)
        Fill(Stream_General, 0, "Codec_Settings", "rec");
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Riff::Buffer_Parse()
{
    //Integrity
    if (Element_Level>10)
    {
        File_Offset=File_Size;
        return false;
    }

    //End of this level?
    while (File_Offset+Buffer_Offset>=Element_Next[Element_Level])
        Element_Level--; //To next element

    //Going in a lower level
    Element_Level++;

    //Element Name
    if (!Element_Name_Get())
    {
        Element_Level--;
        return false;
    }

    //Element Size
    if (!Element_Size_Get())
    {
        Element_Level--;
        return false;
    }

    //Parsing
    if (!Element_Parse())
        return false; //Not enough bytes, Element_Level-- is done in Element_Parse() if needed

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next element
    Buffer_Offset=(size_t)(Element_Next[Element_Level]-File_Offset);
    Element_Name[Element_Level]=0;
    Element_Next[Element_Level]=0;
    Element_Level--;
    return true;
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Riff::Element_Name_Get()
{
    //Enough data?
    if (Buffer_Offset+8>Buffer_Size)
        return false;

    //Element name
    Element_Name[Element_Level]=CC4(Buffer+Buffer_Offset);
    Element_HeaderSize=8;

    //Special cases
    if (Element_Name[Element_Level]==Riff::LIST || Element_Name[Element_Level]==Riff::RIFF || Element_Name[Element_Level]==Riff::FORM)
    {
        //Enough data?
        if (Buffer_Offset+12>Buffer_Size)
            return false;

        //Reading real name
        Element_Name[Element_Level]=CC4(Buffer+Buffer_Offset+8);
        Element_HeaderSize=12;
    }

    //Integrity
    if (Element_Name[Element_Level]==0x00000000)
    {
        TRUSTED_ISNOT("Empty name");
        File_Offset=File_Size;
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
bool File_Riff::Element_Size_Get()
{
    //Enough data?
    if (Buffer_Offset+8>Buffer_Size)
        return false;

    //Element size
    Element_Size=LittleEndian2int32u(Buffer+Buffer_Offset+4);

    //Special cases
    if (Element_HeaderSize==12)
    {
        if (Element_Size>=4)
            Element_Size-=4;
        else
        {
            Element_Size=(int32u)-1; //Size is broken
            Element_Next[Element_Level]=(int64u)-1;
            return true;
        }
    }

    //Element Next
    Element_Next[Element_Level]=File_Offset+Buffer_Offset+Element_HeaderSize+Element_Size;
    if (Element_Size%2)
        Element_Next[Element_Level]+=1; //Always 2 byte-aligned

    //Integrity of element
    if (Element_Next[Element_Level]>Element_Next[Element_Level-1])
    {
        TRUSTED_ISNOT("Atom is too big, resizing...", Element_Level-1);
        Element_Next[Element_Level]=Element_Next[Element_Level-1];
        if (Element_Next[Element_Level]>(File_Offset+Buffer_Offset)+Element_HeaderSize)
            Element_Size=(int32u)(Element_Next[Element_Level]-(File_Offset+Buffer_Offset)-Element_HeaderSize);
        else
        {
            TRUSTED_ISNOT("Not enough place to have an atom", Element_Level-1);
            Element_Size=0; //Not enough place to have an atom
        }
    }

    return true;
}

//---------------------------------------------------------------------------
// Set StreamXXXX_Last with the stream corresponding to the ID
void File_Riff::SetLastByID(int8u ID)
{
    for (size_t StreamKind_Temp=(size_t)Stream_General+1; StreamKind_Temp<Stream_Max; StreamKind_Temp++)
    {
        for (size_t StreamPos_Temp=0; StreamPos_Temp<Stream[StreamKind_Temp]->size(); StreamPos_Temp++)
        {
            if (Get((stream_t)StreamKind_Temp, StreamPos_Temp, _T("ID")).To_int32s()==ID)
            {
                //Found
                StreamKind_Last=(stream_t)StreamKind_Temp;
                StreamPos_Last=StreamPos_Temp;
                return;
            }
        }
    }

    //Not found
    StreamKind_Last=Stream_General;
    StreamPos_Last=0;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Riff::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("PlayTime", "R");
            Fill_HowTo("Movie", "R|INAM");
            Fill_HowTo("Track", "N|INAM");
            Fill_HowTo("Track/Position", "N|IPRT");
            Fill_HowTo("Album/Track_Total", "N|IFRM");
            Fill_HowTo("Composer", "R|IMUS");
            Fill_HowTo("WrittenBy", "R|IWRI");
            Fill_HowTo("Director", "R|IART");
            Fill_HowTo("DirectorOfPhotography", "R|ICNM");
            Fill_HowTo("EditedBy", "R|IEDT");
            Fill_HowTo("Producer", "R|IPRO");
            Fill_HowTo("ProductionDesigner", "R|IPDS");
            Fill_HowTo("CostumeDesigner", "R|ICDS");
            Fill_HowTo("MasteredBy", "R|IENG");
            Fill_HowTo("ProductionStudio", "R|ISTD");
            Fill_HowTo("DistributedBy", "R|IDST");
            Fill_HowTo("EncodedBy", "R|ITCH");
            Fill_HowTo("CommissionedBy", "R|ICMS");
            Fill_HowTo("Encoded_Original/DistributedBy", "R|ISRC");
            Fill_HowTo("Subject", "R|ISBJ");
            Fill_HowTo("Keywords", "R|IKEY");
            Fill_HowTo("LawRating", "R|IRTD");
            Fill_HowTo("Language", "R|ILNG");
            Fill_HowTo("Medium", "R|IMED");
            Fill_HowTo("Product", "R|IPRD");
            Fill_HowTo("Country", "R|ICNT");
            Fill_HowTo("Written_Date", "R|ICRD");
            Fill_HowTo("Mastered_Date", "R|IDIT");
            Fill_HowTo("Archival_Location", "R|IARL");
            Fill_HowTo("Genre", "R|IGNR");
            Fill_HowTo("Comment", "R|ICMT");
            Fill_HowTo("Encoded_Application", "R|ISFT");
            Fill_HowTo("Encoded_Original", "R|ISRF");
            Fill_HowTo("Copyright", "R|ICOP");
            break;
        case (Stream_Video) :
            Fill_HowTo("Codec", "R");
            Fill_HowTo("FrameRate", "R");
            Fill_HowTo("FrameCount", "R");
            Fill_HowTo("Width", "R");
            Fill_HowTo("Height", "R");
            Fill_HowTo("AspectRatio", "R");
            Fill_HowTo("BitRate", "R");
            break;
        case (Stream_Audio) :
            Fill_HowTo("BitRate", "R");
            Fill_HowTo("Channel(s)", "R");
            Fill_HowTo("SamplingRate", "R");
            Fill_HowTo("Codec", "R");
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

#endif //MEDIAINFO_RIFF_YES

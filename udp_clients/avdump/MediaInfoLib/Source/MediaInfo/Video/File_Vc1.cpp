// File_Vc1 - Info for VC-1 files
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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#if defined(MEDIAINFO_VC1_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Video/File_Vc1.h"
#include <ZenLib/BitStream.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
namespace MediaInfoLib
{

//***************************************************************************
// Constants
//***************************************************************************

//---------------------------------------------------------------------------
const char* Vc1_Profile[]=
{
    "Simple",
    "Main",
    "Complex",
    "Advanced",
};

//---------------------------------------------------------------------------
const char* Vc1_Level[]=
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "",
    "",
    "",
};

//---------------------------------------------------------------------------
const char* Vc1_ChromaFormat[]=
{
    "",
    "4:2:0",
    "",
    "",
};

//---------------------------------------------------------------------------
const float Vc1_PixelAspectRatio[]=
{
    1.0, //Reserved
    1.0,
    12.0/11.0,
    10.0/11.0,
    16.0/11.0,
    40.0/33.0,
    24.0/11.0,
    20.0/11.0,
    32.0/11.0,
    80.0/33.0,
    18.0/11.0,
    15.0/11.0,
    64.0/33.0,
    160.0/99.0,
    1.0, //Reserved
    1.0, //Custom
};

//---------------------------------------------------------------------------
const float32 Vc1_FrameRate_enr(int32u Code)
{
    switch (Code)
    {
        case 0x01 : return 24000.0;
        case 0x02 : return 25000.0;
        case 0x03 : return 30000.0;
        case 0x04 : return 50000.0;
        case 0x05 : return 60000.0;
        case 0x06 : return 48000.0;
        case 0x07 : return 72000.0;
        default   : return 0;
    }
}

//---------------------------------------------------------------------------
const float32 Vc1_FrameRate_dr(int32u Code)
{
    switch (Code)
    {
        case 0x01 : return 1000.0;
        case 0x02 : return 1001.0;
        default   : return 0;
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Vc1::Read_Buffer_Init()
{
    //Configuration
    Frame_Count_Valid=2;

    //Elements
    Element_Size=0;
    Frame_Count=0;

    //Temp
    Width=0;
    Height=0;
    RatioValue=0;
    FrameRate=0;
    BitRate=0;

    //Error controls
    Synched=false;
}

//---------------------------------------------------------------------------
void File_Vc1::Read_Buffer_Continue()
{
    //Integrity
    if (File_Offset==0 && Detect_NonVC1())
    {
        File_Offset=File_Size;
        return;
    }

    //Buffer
    Element_Next=File_Offset;

    //Parse
    while (Buffer_Parse());
}

//---------------------------------------------------------------------------
void File_Vc1::Read_Buffer_Finalize()
{
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Vc1::Buffer_Parse()
{
    //Next frame
    if (!NextFrame())
        return false;

    //Element name
    Element_Name=BigEndian2int8u(Buffer+Buffer_Offset+3);

    //Element size
    if (!Element_Size_Get())
        return false;

    //Is OK?
    if (Element_Name_IsOK())
        Element_Parse();

    //If no need of more
    if (File_Offset==File_Size || File_GoTo!=(int64u)-1)
        return false;

    //Next frame
    Buffer_Offset=Element_Next;
    return true;
}

//---------------------------------------------------------------------------
// Element parse
//
bool File_Vc1::Element_Parse()
{
    ELEMENT(0, Element_Name, Element_Size-4);

    //Header
    Buffer_Offset+=4;
    Element_Size -=4;

    //Parse
    switch (Element_Name)
    {
        case 0x0A: EndOfSequence(); break;
        case 0x0B: Slice(); break;
        case 0x0C: Field(); break;
        case 0x0D: FrameHeader(); break;
        case 0x0E: EntryPointHeader(); break;
        case 0x0F: SequenceHeader(); break;
        case 0x1B: UserDefinedSlice(); break;
        case 0x1C: UserDefinedField(); break;
        case 0x1D: UserDefinedFrameHeader(); break;
        case 0x1E: UserDefinedEntryPointHeader(); break;
        case 0x1F: UserDefinedSequenceHeader(); break;
        default:
                TRUSTED_ISNOT("Unattended element!");
    }

    FLUSH();
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

#define NEXTFRAME_TEST() \
    { \
        if (!Frame_ShouldBe.empty()) \
        { \
            bool OK=false; \
            for (size_t Pos=0; Pos<Frame_ShouldBe.size(); Pos++) \
                if (Frame_ShouldBe[Pos]==Element_Name) \
                    OK=true; \
            if (!OK) \
                TRUSTED_ISNOT("Frames are not in the right order"); \
        } \
    } \

#define NEXTFRAME_CLEAR() \
    { \
        Frame_ShouldBe.clear(); \
    } \

#define NEXTFRAME_ADD(_Frame) \
    { \
        Frame_ShouldBe.push_back(_Frame); \
    } \

//---------------------------------------------------------------------------
// Packet "0A"
void File_Vc1::EndOfSequence()
{
    NAME("EndOfSequence");
}

//---------------------------------------------------------------------------
// Packet "0B"
void File_Vc1::Slice()
{
    NAME("Slice");
}

//---------------------------------------------------------------------------
// Packet "0C"
void File_Vc1::Field()
{
    NAME("Field");
}

//---------------------------------------------------------------------------
// Packet "0D"
void File_Vc1::FrameHeader()
{
    NAME("FrameHeader");
    NEXTFRAME_TEST();
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x0D);
    NEXTFRAME_ADD(0x0F);

    //We need Interlace value
    if (Count_Get(Stream_Video)==0)
        return;

    //Parsing
    int32u TypeNotP, TypeNotB, TypeNotI, TypeNotBI;
    BS_BEGIN();
    if (interlace)
    {
        int32u Unknown;
        GET_BS (1, Unknown,                                     Unknown_maybe_top_field_first);
        if (Unknown)
            SKIP_BS(1,                                          Unknown);
    }
    GET_BS (1, TypeNotP,                                        TypeNotP);
    if (TypeNotP)
    {
        GET_BS (1, TypeNotB,                                    TypeNotB);
        if (TypeNotB)
        {
            GET_BS (1, TypeNotI,                                TypeNotI);
            if (TypeNotI)
            {
                GET_BS (1, TypeNotBI,                           TypeNotBI);
                if (TypeNotBI)
                {
                    INFO("Skipped");
                }
                else
                {
                    INFO("BI");
                }
            }
            else
            {
                INFO("I");
            }
        }
        else
        {
            INFO("B");
        }
    }
    else
    {
        INFO("P");
    }
    if (tfcntrflag)
        SKIP_BS(8,                                              tfcntrflag related);
    int32u Flags;
    GET_BS (2, Flags,                                           Flags);

    BS_END_CANBEMORE();

    //Filling
    if (Count_Get(Stream_Video)==0)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "VC-1");
        Stream_Prepare(Stream_Video);
        Fill("Codec", "VC-1");
    }
}

//---------------------------------------------------------------------------
// Packet "0E"
void File_Vc1::EntryPointHeader()
{
    NAME("EntryPointHeader");
    NEXTFRAME_TEST();
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x0D);
}

//---------------------------------------------------------------------------
// Packet "0F"
void File_Vc1::SequenceHeader()
{
    NAME("SequenceHeader");
    NEXTFRAME_CLEAR();
    NEXTFRAME_ADD(0x0D);
    NEXTFRAME_ADD(0x0E);

    //Parsing
    int32u profile;
    BS_BEGIN();
    GET_BS ( 2, profile,                                        profile); INFO(Vc1_Profile[profile]);
    if (profile==3) //Advanced
    {
        float PixelAspectRatio=0, framerate=0;
        int32u level, chromaformat, coded_width, coded_height;
        GET_BS ( 3, level,                                      level); INFO(Vc1_Level[level]);
        GET_BS ( 2, chromaformat,                               chromaformat); INFO(Vc1_ChromaFormat[chromaformat]);
        SKIP_BS( 3,                                             frmrtq_postproc);
        SKIP_BS( 5,                                             bitrtq_postproc);
        SKIP_BS( 1,                                             postprocflag);
        GET_BS (12, coded_width,                                coded_width); INFO((coded_width+1)*2, " pixels");
        GET_BS (12, coded_height,                               coded_height); INFO((coded_height+1)*2, " pixels");
        SKIP_BS( 1,                                             pulldown);
        GET_BS ( 1, interlace,                                  interlace);
        GET_BS ( 1, tfcntrflag,                                 tfcntrflag);
        SKIP_BS( 1,                                             finterpflag);
        SKIP_BS( 1,                                             reserved);
        SKIP_BS( 1,                                             Progressive Segmented Frame);
        TEST_BS_SKIP( 1,                                        display_extended_info);
            SKIP_BS(14,                                         Display x);
            SKIP_BS(14,                                         Display y);
            TEST_BS_SKIP( 1,                                    aspectratio_present);
                int32u AspectRatioCode;
                GET_BS ( 4, AspectRatioCode,                    AspectRatio); INFO(Vc1_PixelAspectRatio[AspectRatioCode]);
                if (AspectRatioCode==0x0F)
                {
                    int32u AspectRatioX, AspectRatioY;
                    GET_BS ( 8, AspectRatioX,                   AspectRatioX);
                    GET_BS ( 8, AspectRatioY,                   AspectRatioY);
                    if (AspectRatioY)
                        PixelAspectRatio=((float)AspectRatioX)/((float)AspectRatioY);
                }
                else
                    PixelAspectRatio=Vc1_PixelAspectRatio[AspectRatioCode];
                TEST_BS_SKIP( 1,                                framerate_present);
                    int32u framerate_form;
                    GET_BS ( 1, framerate_form,                 framerate_form);
                    if (framerate_form)
                    {
                        int32u framerateexp;
                        GET_BS (16, framerateexp,               framerateexp); INFO((float32)((framerateexp+1)/32.0), 3, " fps");
                        framerate=(framerateexp+1)/32.0;
                    }
                    else
                    {
                        int32u frameratecode_enr, frameratecode_dr;
                        GET_BS ( 8, frameratecode_enr,          frameratecode_enr); INFO(Vc1_FrameRate_enr(frameratecode_enr));
                        GET_BS ( 8, frameratecode_dr,           frameratecode_dr); INFO(Vc1_FrameRate_dr(frameratecode_dr));
                        if (Vc1_FrameRate_dr(frameratecode_dr))
                            framerate=Vc1_FrameRate_enr(frameratecode_enr)/Vc1_FrameRate_dr(frameratecode_dr);
                    }
                TEST_BS_END();
            TEST_BS_END();
            TEST_BS_SKIP( 1,                                    Unknown);
                SKIP_BS( 8,                                     color_prim);
                SKIP_BS( 8,                                     transfer_char);
                SKIP_BS( 8,                                     matrix_coef);
            TEST_BS_END();
        TEST_BS_END();
        TEST_BS_SKIP( 1,                                        hrd_param_flag);
            int32u hrd_num_leaky_buckets;
            GET_BS ( 5, hrd_num_leaky_buckets,                  hrd_num_leaky_buckets);
            SKIP_BS( 4,                                         bitrate exponent);
            SKIP_BS( 4,                                         buffer size exponent);
            for (int32u Pos=0; Pos<hrd_num_leaky_buckets; Pos++)
            {
                SKIP_BS(16,                                     hrd_rate)
                SKIP_BS(16,                                     hrd_buffer)
            }
        TEST_BS_END();
        //BS_END(); //something is missing, need specs

        //Filling
        if (Count_Get(Stream_Video)==0)
        {
            Stream_Prepare(Stream_General);
            Fill("Format", "VC-1");
            Stream_Prepare(Stream_Video);
            Fill("Codec", "VC-1");
            Fill("Width", (coded_width+1)*2);
            Fill("Height", (coded_height+1)*2);
            if (PixelAspectRatio!=0)
                Fill("PixelAspectRatio", PixelAspectRatio);
        }
    }
}

//---------------------------------------------------------------------------
// Packet "1B"
void File_Vc1::UserDefinedSlice()
{
    NAME("UserDefinedSlice");
}

//---------------------------------------------------------------------------
// Packet "1C"
void File_Vc1::UserDefinedField()
{
    NAME("UserDefinedField");
}

//---------------------------------------------------------------------------
// Packet "1D"
void File_Vc1::UserDefinedFrameHeader()
{
    NAME("UserDefinedFrameHeader");
}

//---------------------------------------------------------------------------
// Packet "1E"
void File_Vc1::UserDefinedEntryPointHeader()
{
    NAME("UserDefinedEntryPointHeader");
}

//---------------------------------------------------------------------------
// Packet "1F"
void File_Vc1::UserDefinedSequenceHeader()
{
    NAME("UserDefinedSequenceHeader");
}

//***************************************************************************
// Helpers
//***************************************************************************

//---------------------------------------------------------------------------
bool File_Vc1::Detect_NonVC1 ()
{
    //Element_Size
    if (Buffer_Size<8)
    {
        File_Offset=File_Size;
        return true;
    }

    //Detect DAT files, and the parser is not enough precise to detect them later
    if (CC4(Buffer)==CC4("RIFF"))
    {
        File_Offset=File_Size;
        return true;
    }

    //Detect TS files, and the parser is not enough precise to detect them later
    if (Buffer_Size>=188*4)
    {
        //Look for first Sync word
        while (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset)!=0x47)
            Buffer_Offset++;
        if (Buffer_Offset<188 && CC1(Buffer+Buffer_Offset+188)==0x47 && CC1(Buffer+Buffer_Offset+188*2)==0x47 && CC1(Buffer+Buffer_Offset+188*3)==0x47)
        {
            File_Offset=File_Size;
            return true;
        }
        Buffer_Offset=0;
    }

    //Detect BDAV files, and the parser is not enough precise to detect them later
    if (Buffer_Size>=192*4)
    {
        //Look for first Sync word
        while (Buffer_Offset<192 && CC1(Buffer+Buffer_Offset+4)!=0x47)
            Buffer_Offset++;
        if (Buffer_Offset<192 && CC1(Buffer+Buffer_Offset+192+4)==0x47 && CC1(Buffer+Buffer_Offset+192*2+4)==0x47 && CC1(Buffer+Buffer_Offset+192*3+4)==0x47)
        {
            File_Offset=File_Size;
            return true;
        }
        Buffer_Offset=0;
    }

    //Seems OK
    return false;
}

//---------------------------------------------------------------------------
bool File_Vc1::NextFrame()
{
    //Look for first Sync word
    if (Buffer_Offset_Temp==0) //Buffer_Offset_Temp is not 0 if NextFrame() has already parsed first bytes
        Buffer_Offset_Temp=Buffer_Offset;
    while (Buffer_Offset_Temp+4<=Buffer_Size && !(CC3(Buffer+Buffer_Offset_Temp)==0x000001 && CC1(Buffer+Buffer_Offset_Temp+3)<=0x1F)) //0x1F is the upper value for VC-1
        Buffer_Offset_Temp++;

    //Not synched case
    if (!Synched && Buffer_Offset_Temp+4>Buffer_Size)
    {
        if (Buffer_Offset_Temp+3==Buffer_Size)
        {
            if (CC3(Buffer+Buffer_Offset_Temp)!=0x000001)
            {
                Buffer_Offset_Temp++;
                if (CC2(Buffer+Buffer_Offset_Temp)!=0x0000)
                {
                    Buffer_Offset_Temp++;
                    if (CC1(Buffer+Buffer_Offset_Temp)!=0x00)
                        Buffer_Offset_Temp++;
                }
            }
        }
        Buffer_Offset=Buffer_Offset_Temp;
        Buffer_Offset_Temp=0;
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

//---------------------------------------------------------------------------
bool File_Vc1::Element_Size_Get()
{
    //Element with known size
    if (Element_Name==0x0A)
    {
        Element_Size=0;
        Element_Next=Buffer_Offset+4;
        return true;
    }

    //Searching Element_Next
    Element_Next=Buffer_Offset+4;
    while(Element_Next+3<=Buffer_Size && !(CC3(Buffer+Element_Next)==0x000001 && CC1(Buffer+Element_Next+3)<=0x1F)) //0x1F is the upper value for VC-1
        Element_Next++;
    if (Element_Next+3>Buffer_Size)
    {
        //Not found
        //Maybe next byte is not good too
        if (CC2(Buffer+Element_Next)!=0x0000)
        {
            Element_Next++;
            //Maybe next byte is not good too
            if (CC1(Buffer+Element_Next)!=0x00)
                Element_Next++;
        }

        //Testing
        if (Element_Next-Buffer_Offset>=128*1024) //False positive, no element should have more than this size
        {
            TRUSTED_ISNOT("Next element is too big, not normal");
            File_Offset=File_Size;
        }
        Element_Next-=Buffer_Offset; //Because buffer will be moved by File__Base
        return false;
    }

    //Filling
    Element_Size=Element_Next-Buffer_Offset;

    //Testing
    if (Element_Size==0) //Not normal
        TRUSTED_ISNOT("Next element is too small, not normal");

    return true;
}

bool File_Vc1::Element_Name_IsOK()
{
    return true;
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Vc1::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
            Fill_HowTo("Format", "R");
            break;
        case (Stream_Video) :
            break;
        case (Stream_Audio) :
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

} //NameSpace

#endif //MEDIAINFO_VC1_YES


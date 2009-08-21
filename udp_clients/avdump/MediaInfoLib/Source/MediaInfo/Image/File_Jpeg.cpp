// File_Jpeg - Info for NewFormat files
// Copyright (C) 2005-2007 Jerome Martinez, Zen@MediaArea.net
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

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Links
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// http://www.fileformat.info/format/jpeg/
// http://park2.wakwak.com/~tsuruzoh/Computer/Digicams/exif-e.html
// http://www.w3.org/Graphics/JPEG/jfif3.pdf
// http://www.sentex.net/~mwandel/jhead/
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
#if defined(MEDIAINFO_JPEG_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Image/File_Jpeg.h"
#include "ZenLib/Utils.h"
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_Jpeg::Read_Buffer_Continue()
{
    if (File_Offset==0)
    {
        //Integrity
        if (Buffer_Size<2)
            return;

        //Header
        if (CC2(Buffer)!=0xFFD8)
        {
            File_Offset=File_Size;
            return;
        }
        ELEMENT(0, "SOI", 0);
        FLUSH();
        Buffer_Offset+=2;
    }

    //Parse
    while (Buffer_Parse());
}

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
// An Element
// Size                             2 bytes, Pos=4
// Code                             5 bytes, Pos=6
//
bool File_Jpeg::Buffer_Parse()
{
    //Enough data?
    if (Buffer_Offset+2>Buffer_Size)
        return false;

    //Element name
    Element_Name=BigEndian2int16u(Buffer+Buffer_Offset);

    //Element size
    if (Element_Name==Jpeg::SOI || Element_Name==Jpeg::EOI)
        Element_Size=2;
    else
    {
        if (Buffer_Offset+4>Buffer_Size)
            return false;
        Element_Size=BigEndian2int16u(Buffer+Buffer_Offset+2);
        if (Element_Size<2) //At least the size of Size
        {
            File_Offset=File_Size;
            return false;
        }
        if (Element_Size+4>Buffer_Size) //Enough buffer?
            return false;
    }
    Element_Next=File_Offset+Buffer_Offset+2+Element_Size;

    //Parse
    Element_Parse();

    //Next element
    if (File_Offset==File_Size)
        return false; //No need of more
    Buffer_Offset=(size_t)(Element_Next-File_Offset);
    return true;
}

//---------------------------------------------------------------------------
// Element parse
//
bool File_Jpeg::Element_Parse()
{
    ELEMENT(0, Element_Name, Element_Size-2);

    //Offset
    Buffer_Offset+=4; //Marker + Size
    Element_Size-=2; //Size

    //Parsing
    switch (Element_Name)
    {
        case Jpeg::SOF0 : SOF0(); break;
        case Jpeg::SOF1 : SOF1(); break;
        case Jpeg::SOF2 : SOF2(); break;
        case Jpeg::SOF3 : SOF3(); break;
        case Jpeg::SOF9 : SOF9(); break;
        case Jpeg::SOFA : SOFA(); break;
        case Jpeg::SOFB : SOFB(); break;
        case Jpeg::DHT  : DHT (); break;
        case Jpeg::SOI  : SOI (); break;
        case Jpeg::EOI  : EOI (); break;
        case Jpeg::DQT  : DQT (); break;
        case Jpeg::SOS  : SOS (); break;
        case Jpeg::APP0 : APP0(); break;
        case Jpeg::APP1 : APP1(); break;
        default: ;
    }
    FLUSH();

	return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_Jpeg::SOF_()
{
    //Parsing
    int32u Resolution, Height, Width;
    int8u Count;
    CC_BEGIN();
    GET_B1 (Resolution,                                         Resolution);
    GET_B2 (Height,                                             Height);
    GET_B2 (Width,                                              Width);
    GET_B1 (Count,                                              Number of image components in frame);
    for (int8u Pos=0; Pos<Count; Pos++)
    {
        SKIP_B1(                                                Identifier);
        SKIP_B1(                                                sampling factor);
        SKIP_B1(                                                Quantization table destination selector);
    }
    CC_END();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "JPEG");
    Stream_Prepare(Stream_Image);
    Fill("Codec", "JPEG");
    Fill("Codec/String", "JPEG"); //To Avoid automatic filling
    Fill("Resolution", Resolution);
    Fill("Height", Height);
    Fill("Width", Width);
}

//---------------------------------------------------------------------------
void File_Jpeg::SOF0()
{
    NAME("Baseline DCT")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOF1()
{
    NAME("Extended sequential DCT, Huffman coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOF2()
{
    NAME("Progressive DCT, Huffman coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOF3()
{
    NAME("Lossless (sequential), Huffman coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOF9()
{
    NAME("Extended sequential DCT, arithmetic coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOFA()
{
    NAME("Progressive DCT, arithmetic coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::SOFB()
{
    NAME("Lossless (sequential), arithmetic coding")
    SOF_();
}

//---------------------------------------------------------------------------
void File_Jpeg::DHT()
{
    NAME("Define Huffman table(s)")
}

//---------------------------------------------------------------------------
void File_Jpeg::SOI()
{
    NAME("Start Of Image")
}

//---------------------------------------------------------------------------
void File_Jpeg::EOI()
{
    NAME("End Of Image")
}

//---------------------------------------------------------------------------
void File_Jpeg::SOS()
{
    NAME("Start Of Scan")

    //Parsing
    CC_BEGIN();
    int8u Count;
    GET_B1 (Count,                                              Number of image components in scan);
    for (int8u Pos=0; Pos<Count; Pos++)
    {
        SKIP_B1(                                                Scan component selector);
        SKIP_B1(                                                Entropy coding table destination selector);
    }
    SKIP_B1(                                                    Start of spectral or predictor selection);
    SKIP_B1(                                                    End of spectral selection);
    SKIP_B1(                                                    Successive approximation bit position);
    CC_END();

    //Filling
    File_Offset=File_Size; //No need of more
}

//---------------------------------------------------------------------------
void File_Jpeg::DQT()
{
    NAME("Define quantization table(s)")
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0()
{
    NAME("Application Segment 0")

    //Parsing
    int64u Name;
    CC_BEGIN();
    GET_C5(Name,                                                Name);

    //Parsing element
    Buffer_Offset+=5;
    Element_Size-=5;

    if (Name==CC5("JFIF\0"))
        APP0_JFIF();
    if (Name==CC5("JFFF\0"))
        APP0_JFXX();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFIF()
{
    NAME("JFIF")

    //Parsing
    int32u Unit, Width, Height, ThumbailX, ThumbailY;
    CC_BEGIN();
    SKIP_B2(                                                    Version);
    GET_B1 (Unit,                                               Unit); //0=Pixels, 1=dpi, 2=dpcm
    GET_B2 (Width,                                              Xdensity);
    GET_B2 (Height,                                             Ydensity);
    GET_B1 (ThumbailX,                                          Xthumbail);
    GET_B1 (ThumbailY,                                          Ythumbail);
    SKIP_XX(3*ThumbailX*ThumbailY,                              RGB Thumbail);
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX()
{
    NAME("Extension");

    CC_BEGIN();
    SKIP_B1(                                                    extension_code); //0x10 Thumbnail coded using JPEG, 0x11 Thumbnail stored using 1 byte/pixel, 0x13 Thumbnail stored using 3 bytes/pixel
    SKIP_XX(Element_Size-1,                                     extension_data);
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_JPEG()
{
    NAME("Thumbail JPEG");

    //Normal JPEG without JFIF...
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_1B()
{
    NAME("Thumbail 1 byte per pixel")

    //Parsing
    int32u ThumbailX, ThumbailY;
    CC_BEGIN();
    GET_B1 (ThumbailX,                                          Xthumbail);
    GET_B1 (ThumbailY,                                          Ythumbail);
    SKIP_XX(768,                                                Palette);
    SKIP_XX(ThumbailX*ThumbailY,                                Thumbail);
}

//---------------------------------------------------------------------------
void File_Jpeg::APP0_JFXX_3B()
{
    NAME("Thumbail 3 bytes per pixel")

    //Parsing
    int32u ThumbailX, ThumbailY;
    CC_BEGIN();
    GET_B1 (ThumbailX,                                          Xthumbail);
    GET_B1 (ThumbailY,                                          Ythumbail);
    SKIP_XX(3*ThumbailX*ThumbailY,                              RGB Thumbail);
}

//---------------------------------------------------------------------------
void File_Jpeg::APP1()
{
    NAME("Application Segment 1")

    //Parsing
    int64u Name;
    CC_BEGIN();
    GET_C6(Name,                                                Name);

    //Parsing element
    Buffer_Offset+=6;
    Element_Size-=6;

    if (Name==CC6("Exif\0\0"))
        APP1_EXIF();
}

//---------------------------------------------------------------------------
void File_Jpeg::APP1_EXIF()
{
    NAME("Exif")

    //Parsing
    int64u Aligment;
    CC_BEGIN();
    GET_C4(Aligment,                                            Aligment);
    if (Aligment!=0x49492A00 && Aligment!=0x4D4D2A00)
        return;
    if (Aligment==0x49492A00)
        SKIP_B4(                                                First_IFD)
    else
        SKIP_L4(                                                First_IFD)
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_Jpeg::HowTo(stream_t StreamKind)
{
         if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Image)
    {
        Video[0](_T("Codec"), Info_HowTo)=_T("Raw");
    }

    return;
}

} //NameSpace

#endif

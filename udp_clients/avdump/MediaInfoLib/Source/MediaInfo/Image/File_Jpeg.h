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
//
// Information about JPEG files
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//---------------------------------------------------------------------------
#ifndef MediaInfo_File_JpegH
#define MediaInfo_File_JpegH
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/File__Base.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Class File_Jpeg
//***************************************************************************

class File_Jpeg : public File__Base
{
protected :
    //Format
    void Read_Buffer_Continue ();

    //Information
    void HowTo (stream_t StreamKind);

private :
    //Buffer
    bool Buffer_Parse();
    bool Element_Parse();
    int16u Element_Name;
    size_t Element_Size;
    int64u Element_Next;

    //Elements
    void SOF_();
    void SOF0();
    void SOF1();
    void SOF2();
    void SOF3();
    void SOF9();
    void SOFA();
    void SOFB();
    void DHT();
    void SOI();
    void EOI();
    void DQT();
    void SOS();
    void APP0();
    void APP0_JFIF();
    void APP0_JFXX();
    void APP0_JFXX_JPEG();
    void APP0_JFXX_1B();
    void APP0_JFXX_3B();
    void APP1();
    void APP1_EXIF();
};

//***************************************************************************
// Const
//***************************************************************************

namespace Jpeg
{
    const int16u SOF0=0xFFC0;
    const int16u SOF1=0xFFC1;
    const int16u SOF2=0xFFC2;
    const int16u SOF3=0xFFC3;
    const int16u SOF9=0xFFC9;
    const int16u SOFA=0xFFCA;
    const int16u SOFB=0xFFCB;
    const int16u DHT =0xFFC4;
    const int16u SOI =0xFFD8;
    const int16u EOI =0xFFD9;
    const int16u DQT =0xFFDB;
    const int16u SOS =0xFFDA;
    const int16u APP0=0xFFE0;
    const int16u APP1=0xFFE1;
}

} //NameSpace

#endif

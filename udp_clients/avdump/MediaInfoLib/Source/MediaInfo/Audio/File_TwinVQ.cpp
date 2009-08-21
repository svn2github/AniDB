// File_TwinVQ - Info for TwiVQ files
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
#if defined(MEDIAINFO_TWINVQ_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Audio/File_TwinVQ.h"
#include <ZenLib/Utils.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
void File_TwinVQ::Read_Buffer_Continue()
{
    //Integrity
    if (Buffer_Size<=16)
        return;

    //Header
    if (!(CC4(Buffer)==CC4("TWIN") && CC8(Buffer+4)==CC8("97012000")))
    {
        File_Offset=File_Size;
        return;
    }

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "TwinVQ");

    Element_Size=BigEndian2int32u(Buffer+12);
    Buffer_Offset+=16;
    Tags();

    Stream_Prepare(Stream_Audio);
    Fill("Codec", "TwinVQ");

    //No need of more
    File_Offset=File_Size;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_TwinVQ::Tags()
{
    //Integrity
    if (Buffer_Size-Buffer_Offset<Element_Size)
        return;

    int32u Size;
    Ztring Value;
    CC_BEGIN();
    while (Stream_Pos<Element_Size)
    {
        Value.clear();
        GET_B4 (Element_Name,                                   Name);
        GET_B4 (Size,                                           Size);
        GET_LOCAL(Size, Value,                                  Value);

        std::string Parameter;
        switch (Element_Name)
        {
            case TwinVQ::_c__ : Parameter="Copyright"; break;
            case TwinVQ::COMM : break;
            case TwinVQ::AUTH : Parameter="Performer"; break;
            case TwinVQ::COMT : Parameter="Performer/Url"; break;
            case TwinVQ::DSIZ : break;
            case TwinVQ::FILE : break;
            case TwinVQ::NAME : Parameter="Title"; break;
            default :
                {
                    Parameter.append(1, (char)((Element_Name&0xFF000000)>>24));
                    Parameter.append(1, (char)((Element_Name&0x00FF0000)>>16));
                    Parameter.append(1, (char)((Element_Name&0x0000FF00)>> 8));
                    Parameter.append(1, (char)((Element_Name&0x000000FF)>> 0));
                }
        }
        if (!Parameter.empty() && !Value.empty())
            Fill(Stream_General, 0, Parameter.c_str(), Value);
    }
    CC_END();


}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_TwinVQ::HowTo(stream_t StreamKind)
{
        if (StreamKind==Stream_General)
    {
        General[0](_T("Format"), Info_HowTo)=_T("R");
    }
    else if (StreamKind==Stream_Audio)
    {
        Audio[0](_T("Codec"), Info_HowTo)=_T("R");
    }
}

} //NameSpace

#endif //MEDIAINFO_TWINVQ_YES


// File_VorbisCom - Info for VorbisComments tagged files
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
#if defined(MEDIAINFO_VORBISCOM_YES)
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File_VorbisCom.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//***************************************************************************
// Buffer
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::Header_Parse()
{
    //Parsing
    Ztring vendor_string;
    GET_L4 (vendor_length,                                      "vendor_length");
    GET_LOCAL(vendor_length, vendor_string,                     "vendor_string");
    GET_L4 (user_comment_list_length,                           "user_comment_list_length");

    //Filling
    Header_Fill_Code(0, "Vorbis");
    Header_Fill_Size(Element_Size);
}

//---------------------------------------------------------------------------
bool File_VorbisCom::Data_Parse()
{
    Stream_Prepare(Stream_General);
    Stream_Prepare(StreamKind);

    //Comments
    for (int32u Pos=0; Pos<user_comment_list_length; Pos++)
        Comment();

    //Positionning
    File_Offset=File_Size;
    return true;
}

//***************************************************************************
// Elements
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::Comment()
{
    //Parsing
    Ztring comment;
    //ELEMENT_BEGIN();
    GET_L4 (user_comment_length,                            "length");
    //ELEMENT(0, "Comment", user_comment_length);
    GET_UTF8(user_comment_length, comment,                  "comment");

    //Fiiling
    Ztring Key=comment.SubString(_T(""), _T("="));
    Key.MakeUpperCase();
    Ztring Value=comment.SubString(_T("="), _T(""));

         if (Key==_T("ALBUM"))          Fill(Stream_General, 0, "Album", Value);
    else if (Key==_T("ARTIST"))         Fill(Stream_General, 0, "Performer", Value);
    else if (Key==_T("AUTHOR"))         Fill(Stream_General, 0, "Performer", Value);
    else if (Key==_T("COMMENT"))        Fill(Stream_General, 0, "Comment", Value);
    else if (Key==_T("COMMENTS"))       Fill(Stream_General, 0, "Comment", Value);
    else if (Key==_T("CONTACT"))        Fill(Stream_General, 0, "Publisher", Value);
    else if (Key==_T("COPYRIGHT"))      Fill(Stream_General, 0, "Copyright", Value);
    else if (Key==_T("DATE"))           Fill(Stream_General, 0, "Recorded_Date", Value);
    else if (Key==_T("DESCRIPTION"))    Fill(Stream_General, 0, "Comment", Value);
    else if (Key==_T("ENCODER"))        Fill(Stream_General, 0, "Encoded_Application", Value);
    else if (Key==_T("ENCODED_USING"))  Fill(Stream_General, 0, "Encoded_Application", Value);
    else if (Key==_T("ENCODER_URL"))    Fill(Stream_General, 0, "Encoded_Application/Url", Value);
    else if (Key==_T("GENRE"))          Fill(Stream_General, 0, "Genre", Value);
    else if (Key==_T("ISRC"))           Fill(Stream_General, 0, "ISRC", Value);
    else if (Key==_T("LANGUAGE"))       Fill(StreamKind,     0, "Language", Value);
    else if (Key==_T("LICENCE"))        Fill(Stream_General, 0, "TermsOfUse", Value);
    else if (Key==_T("LWING_GAIN"))     Fill(StreamKind,     0, "ReplayGain_Gain", Value);
    else if (Key==_T("LOCATION"))       Fill(Stream_General, 0, "Recorded_Location", Value);
    else if (Key==_T("ORGANIZATION"))   Fill(Stream_General, 0, "Producer", Value);
    else if (Key==_T("PERFORMER"))      Fill(Stream_General, 0, "Performer", Value);
    else if (Key==_T("TITLE"))          Fill(Stream_General, 0, "Title", Value);
    else if (Key==_T("TRACKNUMBER"))    Fill(Stream_General, 0, "Track/Position", Value);
    else if (Key==_T("VERSION"))        Fill(Stream_General, 0, "Track/More", Value);
    else if (Key.find(_T("CHAPTER"))==0)
    {
        if (Count_Get(Stream_Chapters)==0)
            Stream_Prepare(Stream_Chapters);
        if (Key.find(_T("NAME"))==Error)
        {
            Chapter_Pos=Key.SubString(_T("CHAPTER"), _T(""));
            Chapter_Time=Value;
        }
        else
        {
            Value.FindAndReplace(_T("\n"), _T(""), Text.size()-1); //Some chapters names have extra characters, not needed
            Value.FindAndReplace(_T("\r"), _T(""), Text.size()-1); //Some chapters names have extra characters, not needed
            Value.FindAndReplace(_T(" "), _T(""), Text.size()-1); //Some chapters names have extra characters, not needed
            Fill(Stream_Chapters, 0, Chapter_Pos.To_Local().c_str(), Chapter_Time+_T(" ")+Value);
        }
    }
    else                                Fill(Stream_General, 0, Key.To_Local().c_str(), Value);
}

//***************************************************************************
// Information
//***************************************************************************

//---------------------------------------------------------------------------
void File_VorbisCom::HowTo(stream_t StreamKind)
{
    switch (StreamKind)
    {
        case (Stream_General) :
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

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEGA_YES

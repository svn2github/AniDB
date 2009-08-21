// File__Tags - Info for all kind of framed tags tagged files
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

//---------------------------------------------------------------------------
// Compilation conditions
#include <MediaInfo/Setup.h>
#ifdef __BORLANDC__
    #pragma hdrstop
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Tag/File__Tags.h"
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
#ifdef MEDIAINFO_TAG_YES
//---------------------------------------------------------------------------

//***************************************************************************
// External Helper
//***************************************************************************

File__Tags_Helper::File__Tags_Helper(File__Base* Base_)
: File_Id3_Helper(Base_), File_Id3v2_Helper(Base_), File_Lyrics3_Helper(Base_), File_Lyrics3v2_Helper(Base_), File_ApeTag_Helper(Base_)
{
    Base=Base_;
    File_BeginTagSize=0;
    File_EndTagSize=0;
    Id3v1_Size=0;
    Lyrics3_Size=0;
    Lyrics3v2_Size=0;
    ApeTag_Size=0;
    TagSizeIsFinal=false;
}

File__Tags_Helper::~File__Tags_Helper()
{
}

bool File__Tags_Helper::DetectBeginOfEndTags()
{
    //If this is a stream, we must not try to go to the end
    if (Base->File_Name.empty())
    {
        TagSizeIsFinal=true;
        return true;
    }

    while (!TagSizeIsFinal && DetectBeginOfEndTags_Test());

    if (TagSizeIsFinal)
        Base->File_GoTo=0; //Something found and this is finnished, returning to the beginning of the file

    return false;
}

bool File__Tags_Helper::DetectBeginOfEndTags_Test()
{
        TagSizeIsFinal=true;

        //Id3v1
        if (Id3v1_Size==0 && File_EndTagSize==0 && Base->File_Size>=128) //Only one, at the end, larger than 128 bytes
        {
            if (Base->File_Offset>Base->File_Size-128) //Must be at the end less 128 bytes
            {
                Base->File_GoTo=Base->File_Size-128-32; //32 to be able to quickly see another tag system
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-125) //Must be at the end less 128 bytes plus 3 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-128)
                    Base->File_GoTo=Base->File_Size-128-32; //32 to be able to quickly see another tag system
                TagSizeIsFinal=false;
                return false;
            }

            if (CC3(Base->Buffer+((size_t)((Base->File_Size-128)-Base->File_Offset)))==CC3("TAG"))
            {
                Id3v1_Size=128;
                File_EndTagSize+=128;
                TagSizeIsFinal=false;
            }
        }

        //Lyrics3
        if (Lyrics3_Size==0 && Base->File_Size>=File_EndTagSize+9) //Only one, larger than File_EndTagSize+15 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-9) //Must be at the end less 15 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-9;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize+9 bytes plus 9 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-9)-Base->File_Offset)))==CC8("LYRICSEN"))
            {
                //Must find the beginning, 5100 bytes before
                if (Base->File_Offset>Base->File_Size-File_EndTagSize-5100) //Must be at the end less 15 bytes
                {
                    Base->File_GoTo=Base->File_Size-File_EndTagSize-5100;
                    TagSizeIsFinal=false;
                    return false;
                }

                if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize
                {
                    if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                        Base->File_GoTo=Base->File_Size-File_EndTagSize;
                    TagSizeIsFinal=false;
                    return false;
                }

                //Searching for "LYRICSBEGIN"
                std::string Buf((const char*)(Base->Buffer+Base->Buffer_Offset), Base->Buffer_Size-Base->Buffer_Offset);
                size_t Pos=Buf.find("LYRICSBEGIN");
                if (Pos!=std::string::npos)
                {
                    Lyrics3_Size=Base->File_Size-File_EndTagSize-(Base->File_Offset+Base->Buffer_Offset+Pos);
                    File_EndTagSize+=Lyrics3_Size;
                    TagSizeIsFinal=false;
                }
            }
        }

        //Lyrics3v2
        if (Lyrics3v2_Size==0 && Base->File_Size>=File_EndTagSize+15) //Only one, larger than File_EndTagSize+15 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-15) //Must be at the end less 15 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-15;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-15+6)-Base->File_Offset)))==CC8("LYRICS20"))
            {
                Lyrics3v2_Size=Ztring((const char*)(Base->Buffer+((size_t)(Base->Buffer_Size-File_EndTagSize-15))), 6).To_int32u()+15;
                File_EndTagSize+=Lyrics3v2_Size;
                TagSizeIsFinal=false;
            }
        }

        //ApeTag
        if (ApeTag_Size==0 && Base->File_Size>=File_EndTagSize+32) //Only one, larger than File_EndTagSize+32 bytes
        {
            if (Base->File_Offset>Base->File_Size-File_EndTagSize-32) //Must be at the end less 32 bytes
            {
                Base->File_GoTo=Base->File_Size-File_EndTagSize-32;
                TagSizeIsFinal=false;
                return false;
            }

            if (Base->File_Offset+Base->Buffer_Size<Base->File_Size-File_EndTagSize) //Must be at the end less File_EndTagSize+15 bytes plus 15 bytes of tags
            {
                if (Base->File_Offset!=Base->File_Size-File_EndTagSize-32)
                    Base->File_GoTo=Base->File_Size-File_EndTagSize-32;
                TagSizeIsFinal=false;
                return false;
            }

            if (CC8(Base->Buffer+((size_t)((Base->File_Size-File_EndTagSize-32)-Base->File_Offset)))==CC8("APETAGEX"))
            {
                ApeTag_Size=LittleEndian2int32u(Base->Buffer+((size_t)(Base->Buffer_Size-File_EndTagSize-32+12)))+32;
                File_EndTagSize+=ApeTag_Size;
                TagSizeIsFinal=false;
            }
        }

        //Finnished
        return true;
}

bool File__Tags_Helper::Tags_Read_Buffer_Continue()
{
    //Id3v2
    if (!Id3v2_Read_Buffer_Continue())
        return false;
    if (Id3v2_Size!=0 && Id3v2_Size_Continue==0)
    {
        File_BeginTagSize+=Id3v2_Size;
        Id3v2_Size=0;
    }

    //Continue only if we are at the end
    if (Base->File_Offset+Base->Buffer_Offset<Base->File_Size-File_EndTagSize)
        return true;

    //ApeTag
    if (!ApeTag_Read_Buffer_Continue())
        return false;

    //Lyrics3
    if (!Lyrics3_Read_Buffer_Continue())
        return false;

    //Lyrics3v2
    if (!Lyrics3v2_Read_Buffer_Continue())
        return false;

    //ApeTag again (order)
    if (!ApeTag_Read_Buffer_Continue())
        return false;

    //Id3v1
    if (!Id3_Read_Buffer_Continue())
        return false;

    return false; //Finnished! Should be end of file
}

void File__Tags_Helper::Tags_Read_Buffer_Finalize()
{
    Id3_Read_Buffer_Finalize();
    Id3v2_Read_Buffer_Finalize();
    Lyrics3_Read_Buffer_Finalize();
    Lyrics3v2_Read_Buffer_Finalize();
    ApeTag_Read_Buffer_Finalize();
}

#else //MEDIAINFO_TAG_YES
File__Tags_Helper::File__Tags_Helper(File__Base*)
{
    File_BeginTagSize=0;
    File_EndTagSize=0;
    Id3v1_Size=0;
    Lyrics3_Size=0;
    Lyrics3v2_Size=0;
    ApeTag_Size=0;
    TagSizeIsFinal=true;
}
#endif //MEDIAINFO_TAG_YES

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace


// File_Mpeg4 - Info for MPEG-4 files
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
#ifdef MEDIAINFO_MPEG4_YES
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#include "MediaInfo/Multiple/File_Mpeg4.h"
#include <ZenLib/Utils.h>
#include <zlib/zlib.h>
using namespace ZenLib;
//---------------------------------------------------------------------------

namespace MediaInfoLib
{

//---------------------------------------------------------------------------
const char* Mpeg4_Meta_Kind(int32u Kind)
{
    switch (Kind)
    {
        case 0x00 : return "Binary";
        case 0x01 : return "UTF8";
        case 0x02 : return "UTF16";
        case 0x03 : return "Mac String";
        case 0x0E : return "Jpeg";
        case 0x15 : return "Signed Integer"; //the size of the integer is derived from the container size
        case 0x16 : return "Float 32";
        case 0x17 : return "Float 64";
        default   : return "Unknown";
    }
}

//***************************************************************************
// Format
//***************************************************************************

//---------------------------------------------------------------------------
// Element parse
//
bool File_Mpeg4::Element_Parse()
{
    #define ATOM_BEGIN \
        if (Level!=Element_Level) \
        { \
            Level++; \
            switch (Element_Name[Level]) \
            {

        #define SKIP(_ATOM) \
                case Mpeg4::_ATOM : \
                            FLUSH(); \
                            return true; \

        #define ATOM(_ATOM) \
                case Mpeg4::_ATOM : \
                        if (Level==Element_Level) \
                        { \
                            if (Element_Next[Element_Level]-File_Offset<=Buffer_Size || Mpeg4::_ATOM==Mpeg4::mdat || Mpeg4::_ATOM==Mpeg4::free) \
                            { \
                                _ATOM(); \
                                FLUSH(); \
                                if (!List) \
                                    return true; \
                            } \
                            else \
                            { \
                                Buffer_Offset-=Element_HeaderSize; \
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
                                Buffer_Offset-=Element_HeaderSize; \
                                Element_Level--; \
                                PURGE(); \
                                return false; \
                            } \
                        } \

        #define LIST(_ATOM) \
                case Mpeg4::_ATOM : \
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
    ELEMENT(Element_Level-1, Element_Name[Element_Level], Element_Size);
    Buffer_Offset+=Element_HeaderSize;
    int8u Level=0;
    List=false;

    //Hierarchy
    ATOM_BEGIN
    ATOM(free)
    ATOM(ftyp)
    ATOM(idat)
    ATOM(idsc)
    ATOM(mdat)
    LIST(moov)
        ATOM_BEGIN
        LIST(moov_cmov)
            ATOM_BEGIN
            ATOM(moov_cmov_dcom)
            ATOM(moov_cmov_cmvd)
            ATOM_END
        ATOM(moov_ctab)
        ATOM(moov_iods)
        LIST(moov_meta)
            ATOM_BEGIN
            ATOM(moov_meta_hdlr)
            ATOM(moov_meta_keys)
                ATOM_BEGIN
                ATOM(moov_meta_keys_mdta)
                ATOM_END
            LIST(moov_meta_ilst)
                ATOM_BEGIN
                LIST_DEFAULT (moov_meta_ilst_xxxx);
                    ATOM_BEGIN
                    ATOM (moov_meta_ilst_xxxx_data);
                    ATOM (moov_meta_ilst_xxxx_mean);
                    ATOM (moov_meta_ilst_xxxx_name);
                    ATOM_END
                ATOM_END
            ATOM_END
        ATOM(moov_mvhd)
        LIST(moov_trak)
            ATOM_BEGIN
            LIST(moov_trak_edts)
                ATOM_BEGIN
                ATOM(moov_trak_edts_elst)
                ATOM_END
            LIST(moov_trak_mdia)
                ATOM_BEGIN
                ATOM(moov_trak_mdia_hdlr)
                ATOM(moov_trak_mdia_mdhd)
                LIST(moov_trak_mdia_minf)
                    ATOM_BEGIN
                    LIST(moov_trak_mdia_minf_dinf)
                        ATOM_BEGIN
                        ATOM(moov_trak_mdia_minf_dinf_url)
                        ATOM(moov_trak_mdia_minf_dinf_urn)
                        LIST(moov_trak_mdia_minf_dinf_dref)
                            ATOM_BEGIN
                            ATOM(moov_trak_mdia_minf_dinf_url)
                            ATOM(moov_trak_mdia_minf_dinf_urn)
                            ATOM_END
                        ATOM_END
                    ATOM(moov_trak_mdia_minf_hmhd)
                    ATOM(moov_trak_mdia_minf_nmhd)
                    ATOM(moov_trak_mdia_minf_smhd)
                    LIST(moov_trak_mdia_minf_stbl)
                        ATOM_BEGIN
                        ATOM(moov_trak_mdia_minf_stbl_ctts)
                        ATOM(moov_trak_mdia_minf_stbl_stco)
                        ATOM(moov_trak_mdia_minf_stbl_stsc)
                        LIST(moov_trak_mdia_minf_stbl_stsd)
                            ATOM_BEGIN
                            ATOM_DEFAULT(moov_trak_mdia_minf_stbl_stsd_xxxx)
                                ATOM_BEGIN
                                ATOM(moov_trak_mdia_minf_stbl_stsd_xxxx_alac)
                                ATOM(moov_trak_mdia_minf_stbl_stsd_xxxx_chan)
                                ATOM(moov_trak_mdia_minf_stbl_stsd_xxxx_esds)
                                LIST(moov_trak_mdia_minf_stbl_stsd_xxxx_wave)
                                    ATOM_BEGIN
                                    ATOM(moov_trak_mdia_minf_stbl_stsd_xxxx_esds)
                                    ATOM(moov_trak_mdia_minf_stbl_stsd_xxxx_wave_frma)
                                    ATOM_END
                                ATOM_END
                            ATOM_END
                        ATOM(moov_trak_mdia_minf_stbl_stsz)
                        ATOM(moov_trak_mdia_minf_stbl_stts)
                        ATOM_END
                    ATOM(moov_trak_mdia_minf_vmhd)
                    ATOM_END
                ATOM_END
            LIST(moov_trak_tapt)
                ATOM_BEGIN
                ATOM(moov_trak_tapt_clef)
                ATOM(moov_trak_tapt_prof)
                ATOM(moov_trak_tapt_enof)
                ATOM_END
            ATOM(moov_trak_tkhd)
            LIST(moov_trak_tref)
                ATOM_BEGIN
                ATOM(moov_trak_tref_tmcd)
                ATOM_END
            ATOM_END
        LIST(moov_udta)
            ATOM_BEGIN
            SKIP(moov_udta_AllF)
            ATOM(moov_udta_chpl)
            ATOM(moov_udta_cprt)
            SKIP(moov_udta_hinf)
            LIST(moov_udta_hnti)
                ATOM_BEGIN
                ATOM(moov_udta_hnti_rtp)
                ATOM_END
            SKIP(moov_udta_LOOP)
            ATOM(moov_udta_MCPS)
            LIST(moov_udta_meta)
                ATOM_BEGIN
                ATOM(moov_udta_meta_hdlr)
                LIST(moov_udta_meta_ilst)
                    ATOM_BEGIN
                    LIST_DEFAULT (moov_udta_meta_ilst_xxxx);
                        ATOM_BEGIN
                        ATOM (moov_udta_meta_ilst_xxxx_data);
                        ATOM (moov_udta_meta_ilst_xxxx_mean);
                        ATOM (moov_udta_meta_ilst_xxxx_name);
                        ATOM_END
                    ATOM_END
                ATOM_END
            ATOM(moov_udta_ptv )
            SKIP(moov_udta_Sel0)
            SKIP(moov_udta_WLOC)
            ATOM_DEFAULT (moov_udta_xxxx); //User data
            ATOM_END
        ATOM_END
    ATOM(pckg)
    ATOM(pnot)
    ATOM(skip)
    ATOM(wide)
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

#define NAME_VERSION_FLAG(ELEMENT_NAME) \
    NAME(ELEMENT_NAME) \
    int32u Version, Flags; \
    { \
        CC_BEGIN(); \
        GET_B1(Version,                                             Version); \
        GET_B3(Flags,                                               Flags); \
    } \
    Buffer_Offset+=4; \
    Element_Size-=4; \

#define INTEGRITY_VERSION(_VERSION) \
    if (Version>_VERSION) \
    { \
        Details_Add_Error("Unknow version"); \
        return; \
    } \

#define INTEGRITY_SIZE1(ELEMENT_SIZE0) \
    INTEGRITY_VERSION(0) \
    if (Element_Size!=ELEMENT_SIZE0+4) \
    { \
        TRUSTED_ISNOT("Size is wrong"); \
        return; \
    } \

#define INTEGRITY_SIZE2(ELEMENT_SIZE0, ELEMENT_SIZE1) \
    INTEGRITY_VERSION(1) \
    if (Version==0 && Element_Size!=ELEMENT_SIZE0 \
     || Version==1 && Element_Size!=ELEMENT_SIZE1) \
    { \
        TRUSTED_ISNOT("Size is wrong"); \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST1(ELEMENT_SIZE0) \
    INTEGRITY_VERSION(0) \
    if (Version==0 && Element_Size<ELEMENT_SIZE0) \
    { \
        TRUSTED_ISNOT("Size is wrong"); \
        return; \
    } \

#define INTEGRITY_SIZE_ATLEAST2(ELEMENT_SIZE0, ELEMENT_SIZE1) \
    INTEGRITY_VERSION(1) \
    if (Version==0 && Element_Size<ELEMENT_SIZE0 \
     || Version==1 && Element_Size<ELEMENT_SIZE1) \
    { \
        TRUSTED_ISNOT("Size is wrong"); \
        return; \
    } \

#define GET_B_DEPENDOFVERSION(_INFO, _NAME) \
    { \
        if (Version==0) \
            GET_B4(_INFO,                                       _NAME) \
        else \
            GET_B8(_INFO,                                       _NAME) \
    } \

#define GET_DATE1904_DEPENDOFVERSION(_INFO, _NAME) \
    { \
        int64u Temp; \
        if (Version==0) \
            GET_B4(Temp,                                        _NAME) \
        else \
            GET_B8(Temp,                                        _NAME) \
        _INFO.Date_From_Seconds_1904(Temp); INFO(_INFO); \
    } \

#define LIST_SET(OFFSET) \
    Buffer_Offset+=OFFSET; \
    Element_Size-=OFFSET; \
    List=true; \

//---------------------------------------------------------------------------
void File_Mpeg4::free()
{
    NAME("Free space")
}

//---------------------------------------------------------------------------
void File_Mpeg4::ftyp()
{
    NAME("File Type");

    //Parsing
    int32u MajorBrand;
    CC_BEGIN();
    GET_C4 (MajorBrand,                                         MajorBrand);
    SKIP_B4(                                                    MajorBrandVersion);
    while (Stream_Pos<Element_Size)
        SKIP_C4(                                                CompatibleBrand);
    CC_END();

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", Ztring().From_CC4(MajorBrand));
}

//---------------------------------------------------------------------------
void File_Mpeg4::idat()
{
    NAME("QTI");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "QTI");
}

//---------------------------------------------------------------------------
void File_Mpeg4::idsc()
{
    NAME("QTI");

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "QTI");
}

//---------------------------------------------------------------------------
void File_Mpeg4::mdat()
{
    NAME("Data");

    //Filling
    if (Count_Get(Stream_General)==0)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "QT"); //if there is no ftyp atom, this is an old Quictime file
    }

    //Parse mdat if needed
    if (moov_trak_mdia_minf_stbl_stco_ID.size()>0)
    {
        //Currently, we can only parse one chunk in one stream (for MPEG-PS in MOV), must be improved
        if (moov_trak_mdia_minf_stbl_stco_ID.size()==1 && moov_trak_mdia_minf_stbl_stco_Map[moov_trak_mdia_minf_stbl_stco_ID[0]]->size()==1)
        {
            mdat_MustParse=true;
            mdat_Parse();
            return;
        }
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov()
{
    NAME("File header");

    //Testing
    if (moov_Done)
    {
        INFO("Moov atom already parsed");
        List=false; //We must NOT parse it
        return;
    }
    moov_Done=true; //this element will never be seen before the real next "moov" atom

    //Filling
    if (Count_Get(Stream_General)==0)
    {
        Stream_Prepare(Stream_General);
        Fill("Format", "QT"); //if there is no ftyp atom, this is an old Quictime file
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_cmov()
{
    NAME("Compressed file header");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_cmov_cmvd()
{
    NAME("Data");

    switch (moov_cmov_dcom_Compressor)
    {
        case Mpeg4::moov_cmov_dcom_zlib :
            {
                //Sizes
                unsigned long Source_Size=(unsigned long)(Element_Size-4);
                unsigned long Dest_Size=(unsigned long)BigEndian2int32u(Buffer+Buffer_Offset);

                //Details
                if (Config.Details_Get())
                {
                    Details_Add_Info(0, "Dest Size", (size_t)Dest_Size);
                }

                //Uncompressing
                int8u* Dest=new int8u[Dest_Size];
                if (uncompress((Bytef*)Dest, &Dest_Size, (const Bytef*)Buffer+Buffer_Offset+4, Source_Size)<0)
                {
                    delete Dest; //Dest=NULL;
                    return;
                }
                File_Mpeg4 MI;
                Open_Buffer_Init(&MI, File_Size, File_Offset+Buffer_Offset+4);
                Open_Buffer_Continue(&MI, Dest, Dest_Size);
                Open_Buffer_Finalize(&MI);
                Merge(MI);
                Merge(MI, Stream_General, 0, 0);
                delete Dest; //Dest=NULL;
            }
            break;
        default: ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_ctab()
{
    NAME("Color Table");

    //Parsing
    int16u Size;
    CC_BEGIN();
    SKIP_B4(                                                    Color table seed);
    SKIP_B2(                                                    Color table flags);
    GET_B2 (Size,                                               Color table size);
    for (int16u Pos=0; Pos<=Size; Pos++)
    {
        SKIP_B2(                                                Zero);
        SKIP_B2(                                                Red);
        SKIP_B2(                                                Green);
        SKIP_B2(                                                Blue);
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_cmov_dcom()
{
    NAME("Data compressor name");

    //Parsing
    CC_BEGIN();
    GET_C4 (moov_cmov_dcom_Compressor,                          Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_iods()
{
    NAME_VERSION_FLAG("Initial object descriptor");
    INTEGRITY_VERSION(0);

    //Filling
    Descriptors();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta()
{
    NAME_VERSION_FLAG("Metadatas");
    INTEGRITY_VERSION(0);

    //Filling
    moov_meta_hdlr_Type=0;
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_bxml()
{
    NAME_VERSION_FLAG("Binary XML"); //ISO/IEC 14496-12
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mp7b, "Bad meta type", 0)

    //Parsing
    CC_BEGIN();
    SKIP_XX((int32u)Element_Size,                               Binary XML dump);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_hdlr()
{
    NAME_VERSION_FLAG("Header"); //ISO/IEC 14496-12 handler
    INTEGRITY_SIZE_ATLEAST1(20)

    //Parsing
    CC_BEGIN();
    SKIP_C4(                                                    Type (Quicktime)); //Filled if Quicktime
    GET_C4 (moov_meta_hdlr_Type,                                Metadata type);
    SKIP_C4(                                                    Manufacturer);
    SKIP_B4(                                                    Component reserved flags); //Filled if Quicktime
    SKIP_B4(                                                    Component reserved flags mask); //Filled if Quicktime
    SKIP_LOCAL((int32u)Element_Size-20,                         Component type name);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_keys()
{
    NAME_VERSION_FLAG("Keys");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdta, "Bad meta type", 0);
    INTEGRITY_SIZE_ATLEAST1(4);

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Count);
    CC_END_CANBEMORE();

    LIST_SET(4);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_keys_mdta()
{
    NAME("Name");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdta, "Bad meta type", 0);

    //Parsing
    std::string Value;
    CC_BEGIN();
    GET_STRING((int32u)Element_Size, Value,                     Value);
    CC_END();

    //Filling
    moov_udta_meta_keys_List.push_back(Value);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_ilst()
{
    NAME("List");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdta || moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdir, "Bad meta type", 0);

    //Filling
    moov_udta_meta_keys_ilst_Pos=0;
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_ilst_xxxx()
{
    NAME("Element");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdta || moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdir, "Bad meta type", 0);

    //Filling
    moov_meta_ilst_xxxx_name_Name.clear(); //useful if metadata type = "mdir"
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_ilst_xxxx_data()
{
    NAME("Data");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdta || moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdir, "Bad meta type", 0);

    //Parsing
    int32u Kind, Language;
    Ztring Value;
    CC_BEGIN();
    GET_B4(Kind,                                                  Kind); INFO(Mpeg4_Meta_Kind(Kind));
    GET_B4(Language,                                              Language);
    switch (Kind)
    {
        case 0x00 : //Binary
                    switch (Element_Name[Element_Level-1])
                    {
                        case Mpeg4::moov_meta__disk :
                            {
                            //Parsing
                            int32u Position, Total;
                            SKIP_B2(                            Reserved);
                            GET_B2(Position,                    Position);
                            GET_B2(Total,                       Total);
                            if (Element_Size==14)
                                SKIP_B2(                        Reserved); //Sometimes there are 2 more bytes, unknown
                            CC_END_CANBEMORE();
                            //Filling
                            Fill(Stream_General, 0, "Part/Position", Position, 10, true);
                            Fill(Stream_General, 0, "Part/Position_Total", Total, 10, true);
                            }
                            return;
                        case Mpeg4::moov_meta__trkn :
                            {
                            //Parsing
                            int32u Position, Total;
                            SKIP_B2(                            Reserved);
                            GET_B2(Position,                    Position);
                            GET_B2(Total,                       Total);
                            if (Element_Size==14)
                                SKIP_B2(                        Reserved); //Sometimes there are 2 more bytes, unknown
                            CC_END_CANBEMORE();
                            //Filling
                            Fill(Stream_General, 0, "Track/Position", Position, 10, true);
                            Fill(Stream_General, 0, "Track/Position_Total", Total, 10, true);
                            }
                            return;
                        default:
                            Value=_T("(Binary)");
                    }
                    break;
        case 0x01 : //UTF-8
                    switch (Element_Name[Element_Level-1])
                    {
                        case Mpeg4::moov_meta___day :
                            {
                            std::string ValueS;
                            GET_STRING((int32u)Element_Size-8, ValueS,  Value);
                            Value.Date_From_String(ValueS.c_str(), ValueS.size());
                            }
                            break;
                        default:
                            GET_UTF8((int32u)Element_Size-8, Value,     Value);
                    }
                     break;
        case 0x02 : //UTF-16
                    Value=_T("UTF-16 encoding not yet supported");
                    break;
        case 0x03 : //Mac String
                    Value=_T("String(Mac) encoding not yet supported");
                    break;
        case 0x0E : //JPEG
                    Value=_T("(Image)");
                    break;
        case 0x15 : //Signed Integer
                    {
                    int64u ValueI;
                    switch (Element_Size)
                    {
                        case 8+1 : {GET_B1(ValueI,                Value);} Value.From_Number((int64s)ValueI); break;
                        case 8+2 : {GET_B2(ValueI,                Value);} Value.From_Number((int64s)ValueI); break;
                        case 8+4 : {GET_B4(ValueI,                Value);} Value.From_Number((int64s)ValueI); break;
                        case 8+8 : {GET_B8(ValueI,                Value);} Value.From_Number((int64s)ValueI); break;
                        default  : Value=_T("Unknown kind of integer value!");
                    }
                    }
                    break;
        case 0x16 : //Float 32
                    Value=_T("Float32 encoding not yet supported");
                    break;
        case 0x17 : //Foloat 64
                    Value=_T("Float64 encoding not yet supported");
                    break;
        default: Value=_T("Unknown kind of value!");
    }
    CC_END();

    //Filling
    switch (moov_meta_hdlr_Type)
    {
        case Mpeg4::moov_meta_hdlr_mdir :
            {
                std::string Parameter;
                if (Element_Name[Element_Level-1]==Mpeg4::moov_meta______)
                    Metadata_Get(Parameter, moov_meta_ilst_xxxx_name_Name);
                else
                    Metadata_Get(Parameter, Element_Name[Element_Level-1]);
                if (!Parameter.empty())
                    Fill(Stream_General, 0, Parameter.c_str(), Value, true);
            }
            break;
        case Mpeg4::moov_meta_hdlr_mdta :
            INTEGRITY(moov_udta_meta_keys_ilst_Pos<moov_udta_meta_keys_List.size(), "Keys atom is not enough large!", 0)
            {
                std::string Parameter;
                Metadata_Get(Parameter, moov_udta_meta_keys_List[moov_udta_meta_keys_ilst_Pos]);
                if (!Parameter.empty())
                    Fill(Stream_General, 0, Parameter.c_str(), Value, true);
                moov_udta_meta_keys_ilst_Pos++;
            }
        default: ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_ilst_xxxx_mean()
{
    NAME("Mean");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdir, "Bad meta type", 0);

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Unknown);
    SKIP_LOCAL((int32u)Element_Size-4,                          Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_ilst_xxxx_name()
{
    NAME("Name");
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mdir, "Bad meta type", 0);

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Unknown);
    GET_STRING((int32u)Element_Size-4, moov_meta_ilst_xxxx_name_Name, Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_meta_xml()
{
    NAME_VERSION_FLAG("XML"); //ISO/IEC 14496-12
    INTEGRITY(moov_meta_hdlr_Type==Mpeg4::moov_meta_hdlr_mp7t, "Bad meta type", 0);

    //Parsing
    CC_BEGIN();
    SKIP_XX((int32u)Element_Size,                               XML dump);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_mvhd()
{
    NAME_VERSION_FLAG("Movie header");
    INTEGRITY_SIZE_ATLEAST2(96, 108);

    //Parsing
    Ztring Date_Created, Date_Modified;
    int64u Duration;
    int32u TimeScale, Rate;
    int16u Volume;
    CC_BEGIN();
    GET_DATE1904_DEPENDOFVERSION(Date_Created,                  Creation time);
    GET_DATE1904_DEPENDOFVERSION(Date_Modified,                 Modification time);
    GET_B4(TimeScale,                                           Time scale); INFO(Ztring::ToZtring(TimeScale)+_T(" Hz"));
    GET_B_DEPENDOFVERSION(Duration,                             Duration); INFO(Ztring::ToZtring(Duration*1000/TimeScale)+_T(" ms"));
    GET_B4 (Rate,                                               Preferred rate); INFO(Ztring::ToZtring(((float32)Rate)/0x10000));
    GET_B2 (Volume,                                             Preferred volume); INFO(Ztring::ToZtring(((float32)Volume)/0x100));
    SKIP_XX(10,                                                 Reserved);
    SKIP_XX(36,                                                 Matrix structure);
    SKIP_B4(                                                    Preview time);
    SKIP_B4(                                                    Preview duration);
    SKIP_B4(                                                    Poster time);
    SKIP_B4(                                                    Selection time);
    SKIP_B4(                                                    Selection duration);
    SKIP_B4(                                                    Current time);
    SKIP_B4(                                                    Next track ID);
    CC_END();

    //Filling
    if (TimeScale)
    {
        int32u PlayTime=(int32u)(((float)Duration)/TimeScale*1000);
        Fill("PlayTime", PlayTime);
    }
    Fill("Encoded_Date", Date_Created);
    Fill("Tagged_Date", Date_Modified);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak()
{
    NAME("Track");

    //Filling
    Fill_Flush();
    moov_trak_tkhd_TrackID=(int32u)-1;
    moov_trak_mdia_minf_stbl_stco_Parse=true;//false;
    Stream_Prepare(Stream_Max); //clear filling
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_edts()
{
    NAME("Edit");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_edts_elst()
{
    NAME_VERSION_FLAG("Edit List");

    //Parsing
    int32u Count;
    CC_BEGIN();
    GET_B4 (Count,                                              Number of entries);
    for (int32u Pos=0; Pos<Count; Pos++)
    {
        FLUSH();
        ELEMENT(Element_Level+1, "Entry", 12);
        SKIP_B4(                                                Track duration);
        SKIP_B4(                                                Media time);
        INFO_B4(MediaRate,                                      Media rate); INFO(((float)MediaRate)/0x10000);
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia()
{
    NAME("Media");

    //Filling
    moov_trak_mdia_mdhd_PlayTime=0;
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_hdlr()
{
    NAME_VERSION_FLAG("Handler Reference");

    //Parsing
    Ztring Manufacturer, Title;
    int32u SubType;
    int8u Size;
    CC_BEGIN();
    SKIP_C4(                                                    Component type);
    GET_C4 (SubType,                                            Component subtype);
    SKIP_C4(                                                    Component manufacturer);
    SKIP_B4(                                                    Component flags);
    SKIP_B4(                                                    Component flags mask);
    PEEK_B1(Size);
    if ((size_t)(20+1+Size)==Element_Size)
    {
        SKIP_B1(                                                Component name size);
        GET_LOCAL(Size, Title,                                  Component name);
    }
    else
    {
        GET_LOCAL((int32u)Element_Size-20, Title,               Component name);
    }
    if (Title.find(_T("Handler"))!=std::string::npos || Title.find(_T("vide"))!=std::string::npos || Title.find(_T("soun"))!=std::string::npos)
        Title.clear(); //This is not a Title
    CC_END();

    //Filling
    if (!Title.empty()) Fill("Title",    Title);
    switch (SubType)
    {
        case Mpeg4::moov_trak_mdia_hdlr_soun :
            if (StreamKind_Last!=Stream_Audio)
            {
                Stream_Prepare(Stream_Audio);
            }
            break;
        case Mpeg4::moov_trak_mdia_hdlr_vide :
            if (StreamKind_Last!=Stream_Video)
            {
                Stream_Prepare(Stream_Video);
            }
            break;
        case Mpeg4::moov_trak_mdia_hdlr_text :
            if (StreamKind_Last!=Stream_Text)
            {
                Stream_Prepare(Stream_Text);
                Fill("Codec", "tx3g");
            }
            break;
        case Mpeg4::moov_trak_mdia_hdlr_subp :
            if (StreamKind_Last!=Stream_Text)
            {
                Stream_Prepare(Stream_Text);
                Fill("Codec", "subp");
            }
            break;
        case Mpeg4::moov_trak_mdia_hdlr_MPEG :
            moov_trak_mdia_minf_stbl_stco_Parse=true; //Data is in MDAT
            break;
        default: ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_mdhd()
{
    NAME_VERSION_FLAG("Media Header");
    INTEGRITY_SIZE2(20, 32);

    //Parsing
    Ztring Date_Created, Date_Modified;
    int64u Duration;
    int16u Language;
    CC_BEGIN();
    GET_DATE1904_DEPENDOFVERSION(Date_Created,                  Creation time);
    GET_DATE1904_DEPENDOFVERSION(Date_Modified,                 Modification time);
    GET_B4(moov_trak_mdia_mdhd_TimeScale,                       Time scale);
    GET_B_DEPENDOFVERSION(Duration,                             Duration);
    GET_B2 (Language,                                           Language); INFO(Language_Get(Language));
    SKIP_B2(                                                    Quality);
    CC_END();

    //Filling
    Fill("Language", Language_Get(Language));
    if (moov_trak_mdia_mdhd_TimeScale)
    {
        moov_trak_mdia_mdhd_PlayTime=(int32u)(((float)Duration)/moov_trak_mdia_mdhd_TimeScale*1000);
        Fill("PlayTime", moov_trak_mdia_mdhd_PlayTime);
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf()
{
    NAME("Media Information");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_dinf()
{
    NAME("Data Information");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_dinf_url()
{
    NAME_VERSION_FLAG("Data Reference");

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL(Element_Size,                                    location);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_dinf_urn()
{
    NAME_VERSION_FLAG("Data Reference");

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL(Element_Size,                                    name TODO location after null string);
    //SKIP_LOCAL(Element_Size,                                    location);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_dinf_dref()
{
    NAME_VERSION_FLAG("Data Reference");

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    entry_count);
    CC_END_CANBEMORE();

    LIST_SET(4);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_hint()
{
    NAME_VERSION_FLAG("Hint");
    INTEGRITY_SIZE1(16);

    //Parsing
    CC_BEGIN();
    SKIP_B2(                                                    Maximum packet delivery unit);
    SKIP_B2(                                                    Average packet delivery unit);
    SKIP_B4(                                                    Maximum bit rate);
    SKIP_B4(                                                    Average bit rate);
    SKIP_B4(                                                    Reserved);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_hmhd()
{
    NAME_VERSION_FLAG("Hint Media Header");

    //Parsing
    CC_BEGIN();
    SKIP_B2(                                                    maxPDUsize);
    SKIP_B2(                                                    avgPDUsize);
    SKIP_B4(                                                    maxbitrate);
    SKIP_B4(                                                    avgbitrate);
    SKIP_B4(                                                    reserved);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_nmhd()
{
    NAME_VERSION_FLAG("Null Media Header");
    INTEGRITY_SIZE(0, 0);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_smhd()
{
    NAME_VERSION_FLAG("Sound Media Header");

    //Parsing
    CC_BEGIN();
    SKIP_B2(                                                    Audio balance);
    SKIP_B2(                                                    Reserved);
    CC_END();

    //Filling
    if (StreamKind_Last!=Stream_Audio)
        Stream_Prepare(Stream_Audio);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_vmhd()
{
    NAME_VERSION_FLAG("Video Media Header")
    INTEGRITY_SIZE2(8, 8)

    //Parsing
    CC_BEGIN();
    SKIP_B2(                                                    Graphic mode);
    SKIP_B2(                                                    Graphic mode color R);
    SKIP_B2(                                                    Graphic mode color G);
    SKIP_B2(                                                    Graphic mode color B);
    CC_END();

    //Filling
    if (StreamKind_Last!=Stream_Video)
        Stream_Prepare(Stream_Video);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl()
{
    NAME("Sample Table");

    //Filling
    Buffer_MaximumSize=16*1024*1024; //If we are here, this is really a MPEG-4 file, and some atoms are very bigs...
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_ctts()
{
    NAME("Composition Time To Sample");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stco()
{
    NAME_VERSION_FLAG("Chunk offset");
    INTEGRITY_SIZE_ATLEAST1(4);

    int32u Count;
    CC_BEGIN();
    GET_B4 (Count,                                              Number of entries)
    if (moov_trak_mdia_minf_stbl_stco_Parse)
    {
        DETAILLEVEL_SET(1);
        moov_trak_mdia_minf_stbl_stco_ID.push_back(moov_trak_tkhd_TrackID);
        if (moov_trak_mdia_minf_stbl_stco_Map[moov_trak_tkhd_TrackID]==NULL)
            moov_trak_mdia_minf_stbl_stco_Map[moov_trak_tkhd_TrackID]=new std::vector<int64u>;
        int32u Offset;
        for (int32u Pos=0; Pos<Count; Pos++)
        {
            GET_B4 (Offset,                                     Offset)
            moov_trak_mdia_minf_stbl_stco_Map[moov_trak_tkhd_TrackID]->push_back(Offset);
            mdat_Pos[Offset].StreamKind=StreamKind_Last;
            mdat_Pos[Offset].StreamPos=StreamPos_Last;
        }
    }
    else
        SKIP_XX(Count,                                          Offsets);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsc()
{
    NAME("Sample To Chunk");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd()
{
    NAME_VERSION_FLAG("Sample Description");
    INTEGRITY_SIZE_ATLEAST1(4);

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Count);
    CC_END_CANBEMORE();

    LIST_SET(4);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx()
{
    switch (StreamKind_Last)
    {
        case Stream_Audio : moov_trak_mdia_minf_stbl_stsd_xxxxSound(); break;
        case Stream_Video : moov_trak_mdia_minf_stbl_stsd_xxxxVideo(); break;
        default : ;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxxSound()
{
    NAME("Audio")

    int32u Version, Channels, SampleSize, ID, TimeScale;
    CC_BEGIN();
    SKIP_B4(                                                    Reserved)
    SKIP_B2(                                                    Reserved)
    SKIP_B2(                                                    Data reference index)
    GET_B2 (Version,                                            Version)
    SKIP_B2(                                                    Revision level)
    SKIP_B4(                                                    Vendor)
    GET_B2 (Channels,                                           Number of channels)
    GET_B2 (SampleSize,                                         Sample size)
    GET_B2 (ID,                                                 Compression ID)
    SKIP_B2(                                                    Packet size)
    if (ID==0)
        {GET_B4 (TimeScale,                                     Sample rate) INFO(Ztring::ToZtring(TimeScale/0x10000)+_T(" Hz"));}
    else
        {GET_B4 (TimeScale,                                     Frames per chunk) INFO(Ztring::ToZtring(TimeScale/0x10000)+_T(" frames"));}
    if (Version>=1)
    {
        SKIP_B4(                                                Samples per packet)
        SKIP_B4(                                                Bytes per packet)
        SKIP_B4(                                                Bytes per frame)
        SKIP_B4(                                                Bytes per sample)
        if (Version>=2)
        {
            SKIP_B4(                                            Unknown)
            SKIP_B4(                                            Unknown)
            SKIP_B4(                                            Unknown)
            SKIP_B4(                                            Unknown)
            SKIP_B4(                                            Unknown)
        }
    }

    //Filling
    std::string Codec;
    Codec.append(1, (char)((Element_Name[Element_Level]&0xFF000000)>>24));
    Codec.append(1, (char)((Element_Name[Element_Level]&0x00FF0000)>>16));
    if (Codec!="ms") //Normal
    {
        Codec.append(1, (char)((Element_Name[Element_Level]&0x0000FF00)>> 8));
        Codec.append(1, (char)((Element_Name[Element_Level]&0x000000FF)>> 0));
        if (Codec!="raw ")
            Fill("Codec", Codec, false, true);
        else
            Fill("Codec", "PCM", Error, false, true);
        Fill("Codec/CC", Codec, false, true);
        if (Codec=="drms")
            Fill("Encryption", "iTunes");
        if (Codec=="enca")
            Fill("Encryption", "Encrypted");
    }
    else //Microsoft 2CC
    {
        int32u Codec= ((Element_Name[Element_Level]&0x0000FF00)>> 8)
                    + ((Element_Name[Element_Level]&0x000000FF)>> 0); //FormatTag
        Fill("Codec",Codec, 16, true);
        Fill("Codec/CC", Codec, 16, true);
    }
    Fill("Channel(s)", Channels, 10, true);
    if (SampleSize!=0)
        Fill("Resolution", SampleSize, 10, true);
    if (ID==0)
        Fill("SamplingRate", TimeScale/0x10000);

    //Sometimes, more Atoms in this atoms
    if (Stream_Pos<Element_Size)
    {
        Buffer_Offset+=Stream_Pos;
        Element_Size-=Stream_Pos;
        List=true;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxxVideo()
{
    NAME("Video")

    int32u Width, Height, CompressorName_Size, ColorTableID;
    CC_BEGIN();
    SKIP_B4(                                                    Reserved)
    SKIP_B2(                                                    Reserved)
    SKIP_B2(                                                    Data reference index)
    SKIP_B2(                                                    Version)
    SKIP_B2(                                                    Revision level)
    SKIP_B4(                                                    Vendor)
    SKIP_B4(                                                    Temporal quality)
    SKIP_B4(                                                    Spatial quality)
    GET_B2 (Width,                                              Width)
    GET_B2 (Height,                                             Height)
    SKIP_B4(                                                    Horizontal resolution)
    SKIP_B4(                                                    Vertical resolution)
    SKIP_B4(                                                    Data size)
    SKIP_B2(                                                    Frame count)
    GET_B1 (CompressorName_Size,                                Compressor name size)
    SKIP_LOCAL(CompressorName_Size,                             Compressor name)
    Stream_Pos+=32-1-CompressorName_Size;
    SKIP_B2(                                                    Depth)
    GET_B2 (ColorTableID,                                       Color table ID)
    if (ColorTableID==0)
        SKIP_XX(32,                                             Color Table);

    //Filling
    std::string Codec;
    Codec.append(1, (char)((Element_Name[Element_Level]&0xFF000000)>>24));
    Codec.append(1, (char)((Element_Name[Element_Level]&0x00FF0000)>>16));
    Codec.append(1, (char)((Element_Name[Element_Level]&0x0000FF00)>> 8));
    Codec.append(1, (char)((Element_Name[Element_Level]&0x000000FF)>> 0));
    if (Codec!="mp4v") //mp4v can contain a lot of different codecs :(
    {
        Fill("Codec", Codec, false, true);
        Fill("Codec/CC", Codec, false, true);
        if (Codec=="drms")
            Fill("Encryption", "iTunes");
        if (Codec=="enca")
            Fill("Encryption", "Encrypted");
    }
    Fill("Width", BigEndian2int16u(Buffer+Buffer_Offset+24), 10, true);
    Fill("Height", BigEndian2int16u(Buffer+Buffer_Offset+26), 10, true);

    //Specific
    if (Codec=="dvc " || Codec=="DVC " || Codec=="dvcp" || Codec=="DVCP" || Codec=="dvpn" || Codec=="DVPN" || Codec=="dvpp" || Codec=="DVPP")
    {
        Fill("AspectRatio", ((float)4)/3, 3, true);
    }

    LIST_SET(Stream_Pos)
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx_alac()
{
    NAME("ALAC")

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Reserved);
    SKIP_B2(                                                    Reserved);
    SKIP_B2(                                                    Count);
    CC_END_CANBEMORE();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx_chan()
{
    NAME("Channels")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx_esds()
{
    NAME_VERSION_FLAG("ES Descriptor")
    INTEGRITY_VERSION(0);

    //Filling
    Descriptors();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx_wave()
{
    NAME("Wave")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsd_xxxx_wave_frma()
{
    NAME("Data format")

    //Parsing
    std::string Codec;
    CC_BEGIN();
    PEEK_STRING(2, Codec);
    if (Codec=="ms") //Microsoft 2CC
    {
        int16u CodecMS;
        SKIP_C2(                                                Codec_MS);
        GET_B2 (CodecMS,                                        CC2);
        //Filling
        Fill("Codec", CodecMS, 16, true);
        Fill("Codec/CC", CodecMS, 16, true);
    }
    else if (CC4(Buffer+Buffer_Offset)!=CC4("mp4a")) //mp4a can contain a lot of different codecs :(
    {
        Ztring Codec;
        GET_LOCAL (4, Codec,                                    Codec);
        //Filling
        Fill("Codec", Codec, true);
        Fill("Codec/CC", Codec, true);
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stss()
{
    NAME("Time To Sample");
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stsz()
{
    NAME_VERSION_FLAG("Sample Size")
    INTEGRITY_SIZE_ATLEAST1(8)

    int32u Size, Count;
    CC_BEGIN();
    GET_B4 (Size,                                               Sample Size)
    GET_B4 (Count,                                              Number of entries)

    int64u Stream_Size=0;

    if (Size>0)
    {
        Stream_Size=Size*Count;
        if (moov_trak_mdia_minf_stbl_stco_Parse)
        {
            if (moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]==NULL)
                moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]=new std::vector<int64u>;
            moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]->push_back(Stream_Size);
        }
    }
    else
    {
        if (moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]==NULL)
            moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]=new std::vector<int64u>;
        int32u Size;
        for (int32u Pos=8; Pos+4<=Element_Size; Pos+=4)
        {
            DETAILLEVEL_SET((float32)1.1);
            GET_B4 (Size,                                     Size)
            Stream_Size+=Size;
            if (moov_trak_mdia_minf_stbl_stco_Parse)
                moov_trak_mdia_minf_stbl_stsz_Map[moov_trak_tkhd_TrackID]->push_back(Size);
        }
    }

    if (Stream_Size>0)
        Fill("StreamSize", Stream_Size);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_mdia_minf_stbl_stts()
{
    NAME_VERSION_FLAG("Time to Sample")
    INTEGRITY_SIZE_ATLEAST1(4)

    //Details
    if (Config.Details_Get())
    {
        Details_Add_Info(0, "Number of entries", BigEndian2int32u(Buffer+Buffer_Offset+0));
    }

    int32u Min=(int32u)-1;
    int32u Max=0;
    int64u FrameCount=0;

    for (int32u Pos=4; Pos+8<=Element_Size; Pos+=8)
    {
        FrameCount+=BigEndian2int32u(Buffer+Buffer_Offset+Pos);
        int32u Value=BigEndian2int32u(Buffer+Buffer_Offset+Pos+4);
        if (Value<Min) Min=Value;
        if (Value>Max) Max=Value;
    }

    //Details
    if (Config.Details_Get())
    {
        Details_Add_Info(8, "Min", Min);
        Details_Add_Info(8, "Max", Max);
    }

    //Filling
    if (StreamKind_Last==Stream_Video)
    {
        if (moov_trak_mdia_mdhd_TimeScale && Min && Max)
        {
            if (Min!=Max)
            {
                Fill("FrameRate_Min", ((float)moov_trak_mdia_mdhd_TimeScale)/Max);
                Fill("FrameRate_Max", ((float)moov_trak_mdia_mdhd_TimeScale)/Min);
                Fill("FrameRate", ((float)FrameCount)/moov_trak_mdia_mdhd_PlayTime*1000, 3, true);
            }
            else
                Fill("FrameRate", ((float)moov_trak_mdia_mdhd_TimeScale)/Max, 3, true);
        }
        Fill("FrameCount", FrameCount);
    }

    //Details
    for (int32u Pos=8; Pos+8<=Element_Size; Pos+=8)
    {
        //Details
        if (Config.Details_Get())
        {
            Details_Add_Info(Pos, "Sample Count", BigEndian2int32u(Buffer+Buffer_Offset+Pos));
            Details_Add_Info(Pos+4, "Sample Duration", BigEndian2int32u(Buffer+Buffer_Offset+Pos+4));
            if (Pos>=40)
            {
                Details_Add_Info(Pos+8, "(...)", "(...)");
                Pos=(int32u)Element_Size;
            }
        }
    }

}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tapt()
{
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tapt_clef()
{
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tapt_prof()
{
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tapt_enof()
{
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tkhd()
{
    NAME_VERSION_FLAG("Header")
    INTEGRITY_SIZE_ATLEAST2(80, 92)

    //Parsing
    Ztring Date_Created, Date_Modified;
    int64u Duration;
    int16u Volume;
    CC_BEGIN();
        SKIP_FLAGS(Flags, 0,                                    Track Enabled);
        SKIP_FLAGS(Flags, 1,                                    Track in Movie);
        SKIP_FLAGS(Flags, 2,                                    Track in Preview);
        SKIP_FLAGS(Flags, 3,                                    Track in Poster);
    GET_DATE1904_DEPENDOFVERSION(Date_Created,                  Creation time);
    GET_DATE1904_DEPENDOFVERSION(Date_Modified,                 Modification time);
    GET_B4 (moov_trak_tkhd_TrackID,                             Track ID);
    SKIP_B4(                                                    Reserved);
    GET_B_DEPENDOFVERSION(Duration,                             Duration);
    SKIP_B4(                                                    Reserved);
    SKIP_B4(                                                    Reserved);
    SKIP_B2(                                                    Layer);
    SKIP_B2(                                                    Alternate group);
    GET_B2 (Volume,                                             Volume); INFO(Ztring::ToZtring(((float)Volume)/256));
    SKIP_B2(                                                    Reserved);
    SKIP_XX(36,                                                 Matrix structure);
    SKIP_B4(                                                    Track width);
    SKIP_B4(                                                    Track height);
    CC_END();

    //Filling
    Fill("Encoded_Date", Date_Created);
    Fill("Tagged_Date", Date_Modified);
    Fill("ID", moov_trak_tkhd_TrackID, 10, true);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tref()
{
    NAME("Track Reference")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_trak_tref_tmcd()
{
    NAME("TimeCode")

    //Parsing
    CC_BEGIN();
    SKIP_B4(                                                    Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta()
{
    NAME("User Data")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_chpl()
{
    NAME("Chapters")

    //Parsing
    Ztring Value;
    std::string ValueS;
    int64u Time;
    int8u Size;
    size_t Pos=0;
    Stream_Prepare(Stream_Chapters);
    CC_BEGIN();
    SKIP_B8(                                                    Unknown);
    SKIP_B1(                                                    Chapter Count);
    while (Stream_Pos<Element_Size)
    {
        GET_B8 (Time,                                           Time);
        GET_B1 (Size,                                           Text size);
        GET_STRING(Size, ValueS,                                Value);
        Value.From_UTF8(ValueS.c_str());
        if (Value.empty())
            Value.From_Local(ValueS.c_str()); //Trying Local...

        //Filling
        Ztring ToAdd;
        ToAdd+=Ztring().Duration_From_Milliseconds(Time/100);
        ToAdd+=_T(" - ");
        ToAdd+=Value;
        Fill(Ztring::ToZtring(Pos+1).To_Local().c_str(), ToAdd);

        //Next
        Pos++;
    }
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_cprt()
{
    NAME_VERSION_FLAG("Copyright")
    INTEGRITY_SIZE_ATLEAST1(2)

    //Parsing
    Ztring Copyright;
    CC_BEGIN();
    SKIP_B2(                                                    Language);
    GET_LOCAL((int32u)Element_Size-2, Copyright,                Copyright);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Copyright", Copyright);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_hnti()
{
    NAME("Hint Info")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_hnti_rtp()
{
    NAME("Real Time")

    //Parsing
    CC_BEGIN();
    SKIP_LOCAL((int32u)Element_Size,                            Value);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_MCPS()
{
    NAME("Mechanical Copyright Protection Society?")

    //Parsing
    Ztring Encoder;
    CC_BEGIN();
    GET_LOCAL((int32u)Element_Size, Encoder,                    Value);
    CC_END();

    //Filling
    Fill(Stream_General, 0, "Encoded_Library", Encoder);
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta()
{
    NAME_VERSION_FLAG("Metadata")
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_hdlr()
{
    moov_meta_hdlr();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_ilst()
{
    moov_meta_ilst();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_ilst_xxxx()
{
    moov_meta_ilst_xxxx();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_ilst_xxxx_data()
{
    moov_meta_ilst_xxxx_data();
}
//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_ilst_xxxx_mean()
{
    moov_meta_ilst_xxxx_mean();
}
//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_meta_ilst_xxxx_name()
{
    moov_meta_ilst_xxxx_name();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_ptv()
{
    NAME("Print To Video")

    //Parsing
    CC_BEGIN();
    SKIP_B2(                                                    Display size);
    SKIP_B2(                                                    Reserved);
    SKIP_B2(                                                    Reserved);
    SKIP_B1(                                                    Slide show);
    SKIP_B1(                                                    Play on open);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::moov_udta_xxxx()
{
    INTEGRITY_GENERAL(0);

    //Getting the method
    std::string Parameter;
    method Method=Metadata_Get(Parameter, Element_Name[Element_Level]);

    switch (Method)
    {
        case Method_None :
            {
                NAME("Unknown");
            }
            break;
        case Method_Binary :
            {
                NAME("Binary");
            }
            break;
        case Method_String :
            {
                NAME("Text");

                //Parsing
                Ztring Value;
                int16u Size, Language;
                CC_BEGIN();
                while(Stream_Pos<Element_Size)
                {
                    GET_B2(Size,                                Size);
                    GET_B2(Language,                            Language); INFO(Language_Get(Language));
                    GET_LOCAL(Size, Value,                      Value);
                    //Filling
                    Fill(Stream_General, 0, Parameter.c_str(), Value);
                }
            }
            break;
        case Method_Integer :
            {
                NAME("Integer");

                //Parsing
                CC_BEGIN();
                SKIP_XX(4,                                      Value);
                CC_END_CANBEMORE();
            }
            break;
    }
}

//---------------------------------------------------------------------------
void File_Mpeg4::pckg()
{
    NAME("QTCA")

    //Filling
    Stream_Prepare(Stream_General);
    Fill("Format", "QTCA");
}

//---------------------------------------------------------------------------
void File_Mpeg4::pnot()
{
    NAME("Preview")

    //Parsing
    int32u Date_Modified;
    CC_BEGIN();
    GET_B4 (Date_Modified,                                      Modification date); INFO(Ztring().Date_From_Seconds_1904(Date_Modified));
    SKIP_B2(                                                    Version number);
    SKIP_C4(                                                    Atom type);
    SKIP_B4(                                                    Atom index);
    CC_END();
}

//---------------------------------------------------------------------------
void File_Mpeg4::skip()
{
    NAME("Skip")
}

//---------------------------------------------------------------------------
void File_Mpeg4::wide()
{
    NAME("Wide")
}

//***************************************************************************
// C++
//***************************************************************************

} //NameSpace

#endif //MEDIAINFO_MPEG4_YES

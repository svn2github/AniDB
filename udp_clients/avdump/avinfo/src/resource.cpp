// Copyright (C) 2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "wrappers.h"
#include <string.h>
#include <stdio.h>

bool eq(const char* a, const char* b){
	//printf("%s<>%s\n",a,b);
	size_t al = strlen(a);
	size_t bl = strlen(b);
	if(al>bl) return false;
	return !memcmp(a, b, al);
}
int msfourcc(const char* b){
	if(strlen(b)==0) return NULL;
	#define BZ 255
	char a[BZ];
	strncpy_s((char*)&a, BZ, b,_TRUNCATE);
	_strupr_s((char*)&a, BZ);

	//printf("msf: %s %d\n", a, strlen(a));
	if(eq("55", a)) return A_MP3_CBR; //55
	if(eq("0055", a)) return A_MP3_CBR; //55
	if(eq("MP3", a)) return A_MP3_CBR; //55
	if(eq("MPEG-2/3", a)) return A_MP3_CBR; //55
	if(eq("MPEG-1/3", a)) return A_MP3_CBR; //55
	if(eq("MPEG-1 L3", a)) return A_MP3_CBR;
	if(eq("MPEG-2 L3", a)) return A_MP3_CBR;
	if(eq("MPEG-1 AUDIO", a)) return A_MP3_CBR;
	if(eq("MPEG-1A", a)) return A_MP3_CBR;
	if(eq("MPEG-1A L1", a)) return A_OTHER;
	if(eq("MPEG-2A L1", a)) return A_OTHER;
	if(eq("MPEG-1A L2", a)) return A_MP2;
	if(eq("MPEG-2A L2", a)) return A_MP2;
	if(eq("MPEG-1A L3", a)) return A_MP3_CBR;
	if(eq("MPEG-2A L3", a)) return A_MP3_CBR;
	
	if(eq("SRT", a)) return S_SRT;
	if(eq("TX3G", a)) return S_TX3G;
	if(eq("XVID", a)) return V_XVID;
	if(eq("DX50", a)) return V_DIVX5;
	if(eq("DIV5", a)) return V_DIVX5;
	if(eq("674F", a)) return A_VORBIS;
	if(eq("VORBIS", a)) return A_VORBIS;
	if(eq("DIV3", a)) return V_DIVX3;
	if(eq("DIV4", a)) return V_DIVX3;
	//MP3 VBR
	if(strstr(a, "A_AAC")>0) return A_AAC;
	if(eq("AAC", a)) return A_AAC;
	if(eq("AC3", a)) return A_AC3;
	if(eq("2000", a)) return A_AC3;
	if(eq("DIVX", a)) return V_DIVX4;
	if(eq("AVC", a)) return V_H264AVC;
	if(eq("AVC1", a)) return V_H264AVC;
	if(eq("H264", a)) return V_H264AVC;
	if(eq("X264", a)) return V_H264AVC;
	if(eq("WMV3", a)) return V_WMV9;
	//none
//	if(eq("MP2", a)) return A_WMA;
	if(eq("160", a)) return A_WMA;
	if(eq("161", a)) return A_WMA;
	if(eq("162", a)) return A_WMA;
	if(eq("163", a)) return A_WMA;
	if(eq("0160", a)) return A_WMA;
	if(eq("0161", a)) return A_WMA;
	if(eq("0162", a)) return A_WMA;
	if(eq("0163", a)) return A_WMA;

	if(eq("MP2", a)) return A_MP2;
	if(eq("DTS", a)) return A_DTS;
	if(eq("2001", a)) return A_DTS;
	if(eq("MP4A", a)) return A_AAC;
	if(eq("FF", a)) return A_AAC;
	if(eq("00FF", a)) return A_AAC;
	if(eq("MP42", a)) return V_MS_MP4X;
	if(eq("MP43", a)) return V_MS_MP4X;
	if(eq("MPEG-4V", a)) return V_ASP;
	if(eq("MP4V", a)) return V_ASP;
	if(eq("FMP4", a)) return V_ASP;
	
	if(eq("MPEG-1", a)) return V_MPEG1;
	if(eq("MPEG-2", a)) return V_MPEG2;
	if(eq("RV40", a)) return V_RV9;
	if(eq("RV", a)) return V_RVO;
	if(eq("WMV", a)) return V_MS_MP4X;
	
	if(eq("DXSB", a)) return S_XSUB;
	
	if(eq("VP6", a)) return V_OTHER;
	if(eq("JPEG", a)) return V_OTHER;
	if(eq("CVID", a)) return V_OTHER; //cinepak
	if(eq("SVQ1", a)) return V_OTHER; //Sorenson Video v1
	if(eq("SVQ3", a)) return V_OTHER; //Sorenson Video v3

	if(eq("IV30", a)) return V_OTHER;
	if(eq("IV32", a)) return V_OTHER;
	if(eq("IV39", a)) return V_OTHER;
	if(eq("IV40", a)) return V_OTHER;
	if(eq("IV49", a)) return V_OTHER;
	if(eq("IV50", a)) return V_OTHER;

	if(eq("COOK", a)) return A_OTHER;
	if(eq("IMA4", a)) return A_OTHER; //Apple QuickTime IMA4 audio codec
	if(eq("QDM2", a)) return A_OTHER; //QDesign Music v2 audio codec.
	if(eq("ULAW", a)) return A_OTHER; //(mu)-Law 2:1
	if(eq("ALAW", a)) return A_OTHER; //A-Law 2:1
	if(eq("TWOS", a)) return A_OTHER; //Twos Complement - Big Endian PCM
	if(eq("SOWT", a)) return A_OTHER; //Twos Complement - Little Endian PCM




	if(eq(".MP3", a)) return A_MP3_CBR; //55
	if(eq("PCM", a)) return A_PCM;
	if(eq("RAW ", a)) return A_PCM;
	if(eq("1", a)) return A_PCM;
	if(eq("2", a)) return A_PCM;
	if(eq("0001", a)) return A_PCM;
	if(eq("0002", a)) return A_PCM;
	return -1;
}
int userdata(int fourcc, const char* a){
	if(strstr(a, "FFmpeg")>0 || strstr(a, "Lavc")>0) return V_ASP;
	if(strstr(a, "XviD")>0) return V_XVID;
	if(strstr(a, "DivX50")>0) return V_DIVX5;
	if(strstr(a, "div3")>0) return V_DIVX3;
	return fourcc;
}
int nnull(const char* a, const char* b){
	if(b!=NULL && strlen(b)>0) return userdata(msfourcc(b), b);
	if(a!=NULL && strlen(a)>0) return userdata(msfourcc(a), a);
	return -1;
}
int mkvcodecid(const char* a, const char* b){
	if(a==NULL || strlen(a)<2) return -1;//msfourcc(b);
	if(a[0]=='A'){
		if(eq("A_AC3", a)) return A_AC3;
		if(eq("A_DTS", a)) return A_DTS;
		if(eq("A_FLAC", a)) return A_OTHER;
		//if(eq("A_MPC", a)) return "MPC";
		if(eq("A_MPEG/L1", a)) return A_OTHER;
		if(eq("A_MPEG/L2", a)) return A_MP2;
		if(eq("A_MPEG/L3", a)) return A_MP3_CBR;
		if(eq("A_MS/ACM", a)) return A_MSAUDIO;
		if(eq("A_PCM", a)) return A_PCM;
		//if(eq("A_PCM/INT/BIG", a)) return "";
		//if(eq("A_PCM/INT/LIT", a)) return "";
		//if(eq("A_QUICKTIME", a)) return "QDM";
		//if(eq("A_QUICKTIME/QDM2", a)) return "";
		if(eq("A_REAL", a)) return A_OTHER;
		//if(eq("A_REAL/14_4", a)) return "";
		//if(eq("A_REAL/28_8", a)) return "";
		//if(eq("A_REAL/ATRC", a)) return "";
		//if(eq("A_REAL/COOK", a)) return "";
		//if(eq("A_REAL/RALF", a)) return "";
		//if(eq("A_REAL/SIPR", a)) return "";
		if(eq("A_TTA1", a)) return A_OTHER;
		if(eq("A_VORBIS", a)) return A_VORBIS;
		if(eq("A_WAVPACK4", a)) return A_OTHER;
		if(eq("A_AAC", a)) return A_AAC;
		//if(eq("A_AAC/MPEG2/LC", a)) return "";
		//if(eq("A_AAC/MPEG2/LC/SBR", a)) return "";
		//if(eq("A_AAC/MPEG2/MAIN", a)) return "";
		//if(eq("A_AAC/MPEG2/SSR", a)) return "";
		//if(eq("A_AAC/MPEG4/LC", a)) return "";
		//if(eq("A_AAC/MPEG4/LC/SBR", a)) return "";
		//if(eq("A_AAC/MPEG4/LTP", a)) return "";
		//if(eq("A_AAC/MPEG4/MAIN", a)) return "";
		//if(eq("A_AAC/MPEG4/SSR", a)) return "";
		return A_UNKNOWN;
	}else if(a[0]=='V'){
		if(eq("V_MPEG1", a)) return V_MPEG1;
		if(eq("V_MPEG2", a)) return V_MPEG2;
		if(eq("V_MPEG4/ISO/AVC", a)) return V_H264AVC;
		if(eq("V_MPEG4/ISO/AP", a)) return userdata(V_OTHER, b);
		if(eq("V_MPEG4/ISO/ASP", a)) return userdata(V_ASP, b);
		if(eq("V_MPEG4/ISO/SP", a)) return userdata(V_DIVX4, b);
		//if(eq("V_MPEG4/ISO", a)) return nnull(a, b);
		if(eq("V_MPEG4/MS/V3", a)) return nnull(a, b);
		if(eq("V_MS/VFW/FOURCC", a)) return nnull(a, b);
		if(eq("V_QUICKTIME", a)) return V_OTHER;
		if(eq("V_REAL/RV40", a)) return V_RV9;
		if(eq("V_REAL", a)) return V_RVO;
		//if(eq("V_REAL/RV10", a)) return "RM";
		//if(eq("V_REAL/RV20", a)) return "RM";
		//if(eq("V_REAL/RV30", a)) return "RM";
		//if(eq("V_THEORA", a)) return "Theora";
		//if(eq("V_UNCOMPRESSED", a)) 
		return V_UNKNOWNX;
	}else if(a[0]=='S'){
		if(eq("S_TEXT/UTF8", a)) return S_SRT;
		if(eq("S_TEXT/SSA", a)) return S_SSA;
		if(eq("S_TEXT/ASS", a)) return S_ASS;
		if(eq("S_TEXT/USF", a)) return S_USF;
		if(eq("S_IMAGE/BMP", a)) return S_BMP;
		if(eq("S_VOBSUB", a)) return S_VOBSUB;
		return S_UNKNOWN;
	}
	return nnull(b,a);
}
const char* fromiso(const char *iso639_code)
{
	if (!_stricmp(iso639_code, "abk") || !_stricmp(iso639_code, "ab")) return "Abkhazian";
	if (!_stricmp(iso639_code, "ace")) return "Achinese";
	if (!_stricmp(iso639_code, "ach")) return "Acoli";
	if (!_stricmp(iso639_code, "ada")) return "Adangme";
	if (!_stricmp(iso639_code, "aar") || !_stricmp(iso639_code, "aa")) return "Afar";
	if (!_stricmp(iso639_code, "afh")) return "Afrihili";
	if (!_stricmp(iso639_code, "afr") || !_stricmp(iso639_code, "af")) return "Afrikaans";
	if (!_stricmp(iso639_code, "afa")) return "Afro-Asiatic (Other)";
	if (!_stricmp(iso639_code, "aka")) return "Akan";
	if (!_stricmp(iso639_code, "akk")) return "Akkadian";
	if (!_stricmp(iso639_code, "alb") || !_stricmp(iso639_code, "sqi") || !_stricmp(iso639_code, "sq")) return "Albanian";
	if (!_stricmp(iso639_code, "ale")) return "Aleut";
	if (!_stricmp(iso639_code, "alg")) return "Algonquian languages";
	if (!_stricmp(iso639_code, "tut")) return "Altaic (Other)";
	if (!_stricmp(iso639_code, "amh") || !_stricmp(iso639_code, "am")) return "Amharic";
	if (!_stricmp(iso639_code, "apa")) return "Apache languages";
	if (!_stricmp(iso639_code, "ara") || !_stricmp(iso639_code, "ar")) return "Arabic";
	if (!_stricmp(iso639_code, "arc")) return "Aramaic";
	if (!_stricmp(iso639_code, "arp")) return "Arapaho";
	if (!_stricmp(iso639_code, "arn")) return "Araucanian";
	if (!_stricmp(iso639_code, "arw")) return "Arawak";
	if (!_stricmp(iso639_code, "arm") || !_stricmp(iso639_code, "hye") || !_stricmp(iso639_code, "hy")) return "Armenian";
	if (!_stricmp(iso639_code, "art")) return "Artificial (Other)";
	if (!_stricmp(iso639_code, "asm") || !_stricmp(iso639_code, "as")) return "Assamese";
	if (!_stricmp(iso639_code, "ath")) return "Athapascan languages";
	if (!_stricmp(iso639_code, "map")) return "Austronesian (Other)";
	if (!_stricmp(iso639_code, "ava")) return "Avaric";
	if (!_stricmp(iso639_code, "ave")) return "Avestan";
	if (!_stricmp(iso639_code, "awa")) return "Awadhi";
	if (!_stricmp(iso639_code, "aym") || !_stricmp(iso639_code, "ay")) return "Aymara";
	if (!_stricmp(iso639_code, "aze") || !_stricmp(iso639_code, "az")) return "Azerbaijani";
	if (!_stricmp(iso639_code, "nah")) return "Aztec";
	if (!_stricmp(iso639_code, "ban")) return "Balinese";
	if (!_stricmp(iso639_code, "bat")) return "Baltic (Other)";
	if (!_stricmp(iso639_code, "bal")) return "Baluchi";
	if (!_stricmp(iso639_code, "bam")) return "Bambara";
	if (!_stricmp(iso639_code, "bai")) return "Bamileke languages";
	if (!_stricmp(iso639_code, "bad")) return "Banda";
	if (!_stricmp(iso639_code, "bnt")) return "Bantu (Other)";
	if (!_stricmp(iso639_code, "bas")) return "Basa";
	if (!_stricmp(iso639_code, "bak") || !_stricmp(iso639_code, "ba")) return "Bashkir";
	if (!_stricmp(iso639_code, "baq") || !_stricmp(iso639_code, "eus") || !_stricmp(iso639_code, "eu")) return "Basque";
	if (!_stricmp(iso639_code, "bej")) return "Beja";
	if (!_stricmp(iso639_code, "bem")) return "Bemba";
	if (!_stricmp(iso639_code, "ben") || !_stricmp(iso639_code, "bn")) return "Bengali";
	if (!_stricmp(iso639_code, "ber")) return "Berber (Other)";
	if (!_stricmp(iso639_code, "bho")) return "Bhojpuri";
	if (!_stricmp(iso639_code, "bih") || !_stricmp(iso639_code, "bh")) return "Bihari";
	if (!_stricmp(iso639_code, "bik")) return "Bikol";
	if (!_stricmp(iso639_code, "bin")) return "Bini";
	if (!_stricmp(iso639_code, "bis") || !_stricmp(iso639_code, "bi")) return "Bislama";
	if (!_stricmp(iso639_code, "bra")) return "Braj";
	if (!_stricmp(iso639_code, "bre") || !_stricmp(iso639_code, "be")) return "Breton";
	if (!_stricmp(iso639_code, "bug")) return "Buginese";
	if (!_stricmp(iso639_code, "bul") || !_stricmp(iso639_code, "bg")) return "Bulgarian";
	if (!_stricmp(iso639_code, "bua")) return "Buriat";
	if (!_stricmp(iso639_code, "bur") || !_stricmp(iso639_code, "mya") || !_stricmp(iso639_code, "my")) return "Burmese";
	if (!_stricmp(iso639_code, "bel") || !_stricmp(iso639_code, "be")) return "Byelorussian";
	if (!_stricmp(iso639_code, "cad")) return "Caddo";
	if (!_stricmp(iso639_code, "car")) return "Carib";
	if (!_stricmp(iso639_code, "cat") || !_stricmp(iso639_code, "ca")) return "Catalan";
	if (!_stricmp(iso639_code, "cau")) return "Caucasian (Other)";
	if (!_stricmp(iso639_code, "ceb")) return "Cebuano";
	if (!_stricmp(iso639_code, "cel")) return "Celtic (Other)";
	if (!_stricmp(iso639_code, "cai")) return "Central American Indian (Other)";
	if (!_stricmp(iso639_code, "chg")) return "Chagatai";
	if (!_stricmp(iso639_code, "cha")) return "Chamorro";
	if (!_stricmp(iso639_code, "che")) return "Chechen";
	if (!_stricmp(iso639_code, "chr")) return "Cherokee";
	if (!_stricmp(iso639_code, "chy")) return "Cheyenne";
	if (!_stricmp(iso639_code, "chb")) return "Chibcha";
	if (!_stricmp(iso639_code, "chi") || !_stricmp(iso639_code, "zho") || !_stricmp(iso639_code, "zh")) return "Chinese";
	if (!_stricmp(iso639_code, "chn")) return "Chinook jargon";
	if (!_stricmp(iso639_code, "cho")) return "Choctaw";
	if (!_stricmp(iso639_code, "chu")) return "Church Slavic";
	if (!_stricmp(iso639_code, "chv")) return "Chuvash";
	if (!_stricmp(iso639_code, "cop")) return "Coptic";
	if (!_stricmp(iso639_code, "cor")) return "Cornish";
	if (!_stricmp(iso639_code, "cos") || !_stricmp(iso639_code, "co")) return "Corsican";
	if (!_stricmp(iso639_code, "cre")) return "Cree";
	if (!_stricmp(iso639_code, "mus")) return "Creek";
	if (!_stricmp(iso639_code, "crp")) return "Creoles and Pidgins (Other)";
	if (!_stricmp(iso639_code, "cpe")) return "Creoles and Pidgins, English-based (Other)";
	if (!_stricmp(iso639_code, "cpf")) return "Creoles and Pidgins, French-based (Other)";
	if (!_stricmp(iso639_code, "cpp")) return "Creoles and Pidgins, Portuguese-based (Other)";
	if (!_stricmp(iso639_code, "cus")) return "Cushitic (Other)";
	if (!_stricmp(iso639_code, "hr")) return "Croatian";
	if (!_stricmp(iso639_code, "ces") || !_stricmp(iso639_code, "cze") || !_stricmp(iso639_code, "cs")) return "Czech";
	if (!_stricmp(iso639_code, "dak")) return "Dakota";
	if (!_stricmp(iso639_code, "dan") || !_stricmp(iso639_code, "da")) return "Danish";
	if (!_stricmp(iso639_code, "del")) return "Delaware";
	if (!_stricmp(iso639_code, "din")) return "Dinka";
	if (!_stricmp(iso639_code, "div")) return "Divehi";
	if (!_stricmp(iso639_code, "doi")) return "Dogri";
	if (!_stricmp(iso639_code, "dra")) return "Dravidian (Other)";
	if (!_stricmp(iso639_code, "dua")) return "Duala";
	if (!_stricmp(iso639_code, "dut") || !_stricmp(iso639_code, "nla") || !_stricmp(iso639_code, "nl")) return "Dutch";
	if (!_stricmp(iso639_code, "dum")) return "Dutch, Middle (ca. 1050-1350)";
	if (!_stricmp(iso639_code, "dyu")) return "Dyula";
	if (!_stricmp(iso639_code, "dzo") || !_stricmp(iso639_code, "dz")) return "Dzongkha";
	if (!_stricmp(iso639_code, "efi")) return "Efik";
	if (!_stricmp(iso639_code, "egy")) return "Egyptian (Ancient)";
	if (!_stricmp(iso639_code, "eka")) return "Ekajuk";
	if (!_stricmp(iso639_code, "elx")) return "Elamite";
	if (!_stricmp(iso639_code, "eng") || !_stricmp(iso639_code, "en")) return "English";
	if (!_stricmp(iso639_code, "enm")) return "English, Middle (ca. 1100-1500)";
	if (!_stricmp(iso639_code, "ang")) return "English, Old (ca. 450-1100)";
	if (!_stricmp(iso639_code, "esk")) return "Eskimo (Other)";
	if (!_stricmp(iso639_code, "epo") || !_stricmp(iso639_code, "eo")) return "Esperanto";
	if (!_stricmp(iso639_code, "est") || !_stricmp(iso639_code, "et")) return "Estonian";
	if (!_stricmp(iso639_code, "ewe")) return "Ewe";
	if (!_stricmp(iso639_code, "ewo")) return "Ewondo";
	if (!_stricmp(iso639_code, "fan")) return "Fang";
	if (!_stricmp(iso639_code, "fat")) return "Fanti";
	if (!_stricmp(iso639_code, "fao") || !_stricmp(iso639_code, "fo")) return "Faroese";
	if (!_stricmp(iso639_code, "fij") || !_stricmp(iso639_code, "fj")) return "Fijian";
	if (!_stricmp(iso639_code, "fin") || !_stricmp(iso639_code, "fi")) return "Finnish";
	if (!_stricmp(iso639_code, "fiu")) return "Finno-Ugrian (Other)";
	if (!_stricmp(iso639_code, "fon")) return "Fon";
	if (!_stricmp(iso639_code, "fra") || !_stricmp(iso639_code, "fre") || !_stricmp(iso639_code, "fr")) return "French";
	if (!_stricmp(iso639_code, "frm")) return "French, Middle (ca. 1400-1600)";
	if (!_stricmp(iso639_code, "fro")) return "French, Old (842- ca. 1400)";
	if (!_stricmp(iso639_code, "fry") || !_stricmp(iso639_code, "fy")) return "Frisian";
	if (!_stricmp(iso639_code, "ful")) return "Fulah";
	if (!_stricmp(iso639_code, "gaa")) return "Ga";
	if (!_stricmp(iso639_code, "gae") || !_stricmp(iso639_code, "gdh")) return "Gaelic (Scots)";
	if (!_stricmp(iso639_code, "glg") || !_stricmp(iso639_code, "gl")) return "Gallegan";
	if (!_stricmp(iso639_code, "lug")) return "Ganda";
	if (!_stricmp(iso639_code, "gay")) return "Gayo";
	if (!_stricmp(iso639_code, "gez")) return "Geez";
	if (!_stricmp(iso639_code, "geo") || !_stricmp(iso639_code, "kat") || !_stricmp(iso639_code, "ka")) return "Georgian";
	if (!_stricmp(iso639_code, "deu") || !_stricmp(iso639_code, "ger") || !_stricmp(iso639_code, "de")) return "German";
	if (!_stricmp(iso639_code, "gmh")) return "German, Middle High (ca. 1050-1500)";
	if (!_stricmp(iso639_code, "goh")) return "German, Old High (ca. 750-1050)";
	if (!_stricmp(iso639_code, "gem")) return "Germanic (Other)";
	if (!_stricmp(iso639_code, "gil")) return "Gilbertese";
	if (!_stricmp(iso639_code, "gon")) return "Gondi";
	if (!_stricmp(iso639_code, "got")) return "Gothic";
	if (!_stricmp(iso639_code, "grb")) return "Grebo";
	if (!_stricmp(iso639_code, "grc")) return "Greek, Ancient (to 1453)";
	if (!_stricmp(iso639_code, "ell") || !_stricmp(iso639_code, "gre") || !_stricmp(iso639_code, "el")) return "Greek, Modern (1453-)";
	if (!_stricmp(iso639_code, "kal") || !_stricmp(iso639_code, "kl")) return "Greenlandic";
	if (!_stricmp(iso639_code, "grn") || !_stricmp(iso639_code, "gn")) return "Guarani";
	if (!_stricmp(iso639_code, "guj") || !_stricmp(iso639_code, "gu")) return "Gujarati";
	if (!_stricmp(iso639_code, "hai")) return "Haida";
	if (!_stricmp(iso639_code, "hau") || !_stricmp(iso639_code, "ha")) return "Hausa";
	if (!_stricmp(iso639_code, "haw")) return "Hawaiian";
	if (!_stricmp(iso639_code, "heb") || !_stricmp(iso639_code, "he")) return "Hebrew";
	if (!_stricmp(iso639_code, "her")) return "Herero";
	if (!_stricmp(iso639_code, "hil")) return "Hiligaynon";
	if (!_stricmp(iso639_code, "him")) return "Himachali";
	if (!_stricmp(iso639_code, "hin") || !_stricmp(iso639_code, "hi")) return "Hindi";
	if (!_stricmp(iso639_code, "hmo")) return "Hiri Motu";
	if (!_stricmp(iso639_code, "hun") || !_stricmp(iso639_code, "hu")) return "Hungarian";
	if (!_stricmp(iso639_code, "hup")) return "Hupa";
	if (!_stricmp(iso639_code, "iba")) return "Iban";
	if (!_stricmp(iso639_code, "ice") || !_stricmp(iso639_code, "isl") || !_stricmp(iso639_code, "is")) return "Icelandic";
	if (!_stricmp(iso639_code, "ibo")) return "Igbo";
	if (!_stricmp(iso639_code, "ijo")) return "Ijo";
	if (!_stricmp(iso639_code, "ilo")) return "Iloko";
	if (!_stricmp(iso639_code, "inc")) return "Indic (Other)";
	if (!_stricmp(iso639_code, "ine")) return "Indo-European (Other)";
	if (!_stricmp(iso639_code, "ind") || !_stricmp(iso639_code, "id")) return "Indonesian";
	if (!_stricmp(iso639_code, "ina") || !_stricmp(iso639_code, "ia")) return "Interlingua (International Auxiliary language Association)";
	if (!_stricmp(iso639_code, "ine")) return "Interlingue";
	if (!_stricmp(iso639_code, "iku") || !_stricmp(iso639_code, "iu")) return "Inuktitut";
	if (!_stricmp(iso639_code, "ipk") || !_stricmp(iso639_code, "ik")) return "Inupiak";
	if (!_stricmp(iso639_code, "ira")) return "Iranian (Other)";
	if (!_stricmp(iso639_code, "gai") || !_stricmp(iso639_code, "iri") || !_stricmp(iso639_code, "ga")) return "Irish";
	if (!_stricmp(iso639_code, "sga")) return "Irish, Old (to 900)";
	if (!_stricmp(iso639_code, "mga")) return "Irish, Middle (900 - 1200)";
	if (!_stricmp(iso639_code, "iro")) return "Iroquoian languages";
	if (!_stricmp(iso639_code, "ita") || !_stricmp(iso639_code, "it")) return "Italian";
	if (!_stricmp(iso639_code, "jpn") || !_stricmp(iso639_code, "ja")) return "Japanese";
	if (!_stricmp(iso639_code, "jav") || !_stricmp(iso639_code, "jaw") || !_stricmp(iso639_code, "jv/jw")) return "Javanese";
	if (!_stricmp(iso639_code, "jrb")) return "Judeo-Arabic";
	if (!_stricmp(iso639_code, "jpr")) return "Judeo-Persian";
	if (!_stricmp(iso639_code, "kab")) return "Kabyle";
	if (!_stricmp(iso639_code, "kac")) return "Kachin";
	if (!_stricmp(iso639_code, "kam")) return "Kamba";
	if (!_stricmp(iso639_code, "kan") || !_stricmp(iso639_code, "kn")) return "Kannada";
	if (!_stricmp(iso639_code, "kau")) return "Kanuri";
	if (!_stricmp(iso639_code, "kaa")) return "Kara-Kalpak";
	if (!_stricmp(iso639_code, "kar")) return "Karen";
	if (!_stricmp(iso639_code, "kas") || !_stricmp(iso639_code, "ks")) return "Kashmiri";
	if (!_stricmp(iso639_code, "kaw")) return "Kawi";
	if (!_stricmp(iso639_code, "kaz") || !_stricmp(iso639_code, "kk")) return "Kazakh";
	if (!_stricmp(iso639_code, "kha")) return "Khasi";
	if (!_stricmp(iso639_code, "khm") || !_stricmp(iso639_code, "km")) return "Khmer";
	if (!_stricmp(iso639_code, "khi")) return "Khoisan (Other)";
	if (!_stricmp(iso639_code, "kho")) return "Khotanese";
	if (!_stricmp(iso639_code, "kik")) return "Kikuyu";
	if (!_stricmp(iso639_code, "kin") || !_stricmp(iso639_code, "rw")) return "Kinyarwanda";
	if (!_stricmp(iso639_code, "kir") || !_stricmp(iso639_code, "ky")) return "Kirghiz";
	if (!_stricmp(iso639_code, "kom")) return "Komi";
	if (!_stricmp(iso639_code, "kon")) return "Kongo";
	if (!_stricmp(iso639_code, "kok")) return "Konkani";
	if (!_stricmp(iso639_code, "kor") || !_stricmp(iso639_code, "ko")) return "Korean";
	if (!_stricmp(iso639_code, "kpe")) return "Kpelle";
	if (!_stricmp(iso639_code, "kro")) return "Kru";
	if (!_stricmp(iso639_code, "kua")) return "Kuanyama";
	if (!_stricmp(iso639_code, "kum")) return "Kumyk";
	if (!_stricmp(iso639_code, "kur") || !_stricmp(iso639_code, "ku")) return "Kurdish";
	if (!_stricmp(iso639_code, "kru")) return "Kurukh";
	if (!_stricmp(iso639_code, "kus")) return "Kusaie";
	if (!_stricmp(iso639_code, "kut")) return "Kutenai";
	if (!_stricmp(iso639_code, "lad")) return "Ladino";
	if (!_stricmp(iso639_code, "lah")) return "Lahnda";
	if (!_stricmp(iso639_code, "lam")) return "Lamba";
	if (!_stricmp(iso639_code, "oci") || !_stricmp(iso639_code, "oc")) return "Langue d'Oc (post 1500)";
	if (!_stricmp(iso639_code, "lao") || !_stricmp(iso639_code, "lo")) return "Lao";
	if (!_stricmp(iso639_code, "lat") || !_stricmp(iso639_code, "la")) return "Latin";
	if (!_stricmp(iso639_code, "lav") || !_stricmp(iso639_code, "lv")) return "Latvian";
	if (!_stricmp(iso639_code, "ltz")) return "Letzeburgesch";
	if (!_stricmp(iso639_code, "lez")) return "Lezghian";
	if (!_stricmp(iso639_code, "lin") || !_stricmp(iso639_code, "ln")) return "Lingala";
	if (!_stricmp(iso639_code, "lit") || !_stricmp(iso639_code, "lt")) return "Lithuanian";
	if (!_stricmp(iso639_code, "loz")) return "Lozi";
	if (!_stricmp(iso639_code, "lub")) return "Luba-Katanga";
	if (!_stricmp(iso639_code, "lui")) return "Luiseno";
	if (!_stricmp(iso639_code, "lun")) return "Lunda";
	if (!_stricmp(iso639_code, "luo")) return "Luo (Kenya and Tanzania)";
	if (!_stricmp(iso639_code, "mac") || !_stricmp(iso639_code, "mak") || !_stricmp(iso639_code, "mk")) return "Macedonian";
	if (!_stricmp(iso639_code, "mad")) return "Madurese";
	if (!_stricmp(iso639_code, "mag")) return "Magahi";
	if (!_stricmp(iso639_code, "mai")) return "Maithili";
	if (!_stricmp(iso639_code, "mak")) return "Makasar";
	if (!_stricmp(iso639_code, "mlg") || !_stricmp(iso639_code, "mg")) return "Malagasy";
	if (!_stricmp(iso639_code, "may") || !_stricmp(iso639_code, "msa") || !_stricmp(iso639_code, "ms")) return "Malay";
	if (!_stricmp(iso639_code, "mal")) return "Malayalam";
	if (!_stricmp(iso639_code, "mlt") || !_stricmp(iso639_code, "ml")) return "Maltese";
	if (!_stricmp(iso639_code, "man")) return "Mandingo";
	if (!_stricmp(iso639_code, "mni")) return "Manipuri";
	if (!_stricmp(iso639_code, "mno")) return "Manobo languages";
	if (!_stricmp(iso639_code, "max")) return "Manx";
	if (!_stricmp(iso639_code, "mao") || !_stricmp(iso639_code, "mri") || !_stricmp(iso639_code, "mi")) return "Maori";
	if (!_stricmp(iso639_code, "mar") || !_stricmp(iso639_code, "mr")) return "Marathi";
	if (!_stricmp(iso639_code, "chm")) return "Mari";
	if (!_stricmp(iso639_code, "mah")) return "Marshall";
	if (!_stricmp(iso639_code, "mwr")) return "Marwari";
	if (!_stricmp(iso639_code, "mas")) return "Masai";
	if (!_stricmp(iso639_code, "myn")) return "Mayan languages";
	if (!_stricmp(iso639_code, "men")) return "Mende";
	if (!_stricmp(iso639_code, "mic")) return "Micmac";
	if (!_stricmp(iso639_code, "min")) return "Minangkabau";
	if (!_stricmp(iso639_code, "mis")) return "Miscellaneous (Other)";
	if (!_stricmp(iso639_code, "moh")) return "Mohawk";
	if (!_stricmp(iso639_code, "mol") || !_stricmp(iso639_code, "mo")) return "Moldavian";
	if (!_stricmp(iso639_code, "mkh")) return "Mon-Kmer (Other)";
	if (!_stricmp(iso639_code, "lol")) return "Mongo";
	if (!_stricmp(iso639_code, "mon") || !_stricmp(iso639_code, "mn")) return "Mongolian";
	if (!_stricmp(iso639_code, "mos")) return "Mossi";
	if (!_stricmp(iso639_code, "mul")) return "Multiple languages";
	if (!_stricmp(iso639_code, "mun")) return "Munda languages";
	if (!_stricmp(iso639_code, "nau") || !_stricmp(iso639_code, "na")) return "Nauru";
	if (!_stricmp(iso639_code, "nav")) return "Navajo";
	if (!_stricmp(iso639_code, "nde")) return "Ndebele, North";
	if (!_stricmp(iso639_code, "nbl")) return "Ndebele, South";
	if (!_stricmp(iso639_code, "ndo")) return "Ndongo";
	if (!_stricmp(iso639_code, "nep") || !_stricmp(iso639_code, "ne")) return "Nepali";
	if (!_stricmp(iso639_code, "new")) return "Newari";
	if (!_stricmp(iso639_code, "nic")) return "Niger-Kordofanian (Other)";
	if (!_stricmp(iso639_code, "ssa")) return "Nilo-Saharan (Other)";
	if (!_stricmp(iso639_code, "niu")) return "Niuean";
	if (!_stricmp(iso639_code, "non")) return "Norse, Old";
	if (!_stricmp(iso639_code, "nai")) return "North American Indian (Other)";
	if (!_stricmp(iso639_code, "nor") || !_stricmp(iso639_code, "no")) return "Norwegian";
	if (!_stricmp(iso639_code, "nno")) return "Norwegian (Nynorsk)";
	if (!_stricmp(iso639_code, "nub")) return "Nubian languages";
	if (!_stricmp(iso639_code, "nym")) return "Nyamwezi";
	if (!_stricmp(iso639_code, "nya")) return "Nyanja";
	if (!_stricmp(iso639_code, "nyn")) return "Nyankole";
	if (!_stricmp(iso639_code, "nyo")) return "Nyoro";
	if (!_stricmp(iso639_code, "nzi")) return "Nzima";
	if (!_stricmp(iso639_code, "oji")) return "Ojibwa";
	if (!_stricmp(iso639_code, "ori") || !_stricmp(iso639_code, "or")) return "Oriya";
	if (!_stricmp(iso639_code, "orm") || !_stricmp(iso639_code, "om")) return "Oromo";
	if (!_stricmp(iso639_code, "osa")) return "Osage";
	if (!_stricmp(iso639_code, "oss")) return "Ossetic";
	if (!_stricmp(iso639_code, "oto")) return "Otomian languages";
	if (!_stricmp(iso639_code, "pal")) return "Pahlavi";
	if (!_stricmp(iso639_code, "pau")) return "Palauan";
	if (!_stricmp(iso639_code, "pli")) return "Pali";
	if (!_stricmp(iso639_code, "pam")) return "Pampanga";
	if (!_stricmp(iso639_code, "pag")) return "Pangasinan";
	if (!_stricmp(iso639_code, "pan") || !_stricmp(iso639_code, "pa")) return "Panjabi";
	if (!_stricmp(iso639_code, "pap")) return "Papiamento";
	if (!_stricmp(iso639_code, "paa")) return "Papuan-Australian (Other)";
	if (!_stricmp(iso639_code, "fas") || !_stricmp(iso639_code, "per") || !_stricmp(iso639_code, "fa")) return "Persian";
	if (!_stricmp(iso639_code, "peo")) return "Persian, Old (ca 600 - 400 B.C.)";
	if (!_stricmp(iso639_code, "phn")) return "Phoenician";
	if (!_stricmp(iso639_code, "pol") || !_stricmp(iso639_code, "pl")) return "Polish";
	if (!_stricmp(iso639_code, "pon")) return "Ponape";
	if (!_stricmp(iso639_code, "por") || !_stricmp(iso639_code, "pt")) return "Portuguese";
	if (!_stricmp(iso639_code, "pra")) return "Prakrit languages";
	if (!_stricmp(iso639_code, "pro")) return "Provencal, Old (to 1500)";
	if (!_stricmp(iso639_code, "pus") || !_stricmp(iso639_code, "ps")) return "Pushto";
	if (!_stricmp(iso639_code, "que") || !_stricmp(iso639_code, "qu")) return "Quechua";
	if (!_stricmp(iso639_code, "roh") || !_stricmp(iso639_code, "rm")) return "Rhaeto-Romance";
	if (!_stricmp(iso639_code, "raj")) return "Rajasthani";
	if (!_stricmp(iso639_code, "rar")) return "Rarotongan";
	if (!_stricmp(iso639_code, "roa")) return "Romance (Other)";
	if (!_stricmp(iso639_code, "ron") || !_stricmp(iso639_code, "rum") || !_stricmp(iso639_code, "ro")) return "Romanian";
	if (!_stricmp(iso639_code, "rom")) return "Romany";
	if (!_stricmp(iso639_code, "run") || !_stricmp(iso639_code, "rn")) return "Rundi";
	if (!_stricmp(iso639_code, "rus") || !_stricmp(iso639_code, "ru")) return "Russian";
	if (!_stricmp(iso639_code, "sal")) return "Salishan languages";
	if (!_stricmp(iso639_code, "sam")) return "Samaritan Aramaic";
	if (!_stricmp(iso639_code, "smi")) return "Sami languages";
	if (!_stricmp(iso639_code, "smo") || !_stricmp(iso639_code, "sm")) return "Samoan";
	if (!_stricmp(iso639_code, "sad")) return "Sandawe";
	if (!_stricmp(iso639_code, "sag") || !_stricmp(iso639_code, "sg")) return "Sango";
	if (!_stricmp(iso639_code, "san") || !_stricmp(iso639_code, "sa")) return "Sanskrit";
	if (!_stricmp(iso639_code, "srd")) return "Sardinian";
	if (!_stricmp(iso639_code, "sco")) return "Scots";
	if (!_stricmp(iso639_code, "sel")) return "Selkup";
	if (!_stricmp(iso639_code, "sem")) return "Semitic (Other)";
	if (!_stricmp(iso639_code, "sr")) return "Serbian";
	if (!_stricmp(iso639_code, "scr") || !_stricmp(iso639_code, "sh")) return "Serbo-Croatian";
	if (!_stricmp(iso639_code, "srr")) return "Serer";
	if (!_stricmp(iso639_code, "shn")) return "Shan";
	if (!_stricmp(iso639_code, "sna") || !_stricmp(iso639_code, "sn")) return "Shona";
	if (!_stricmp(iso639_code, "sid")) return "Sidamo";
	if (!_stricmp(iso639_code, "bla")) return "Siksika";
	if (!_stricmp(iso639_code, "snd") || !_stricmp(iso639_code, "sd")) return "Sindhi";
	if (!_stricmp(iso639_code, "sin") || !_stricmp(iso639_code, "si")) return "Singhalese";
	if (!_stricmp(iso639_code, "sit") || !_stricmp(iso639_code, "-" )) return "Sino-Tibetan (Other)";
	if (!_stricmp(iso639_code, "sio")) return "Siouan languages";
	if (!_stricmp(iso639_code, "sla")) return "Slavic (Other)";
	if (!_stricmp(iso639_code, "ssw") || !_stricmp(iso639_code, "ss")) return "Siswant";
	if (!_stricmp(iso639_code, "slk") || !_stricmp(iso639_code, "slo") || !_stricmp(iso639_code, "sk")) return "Slovak";
	if (!_stricmp(iso639_code, "slv") || !_stricmp(iso639_code, "sl")) return "Slovenian";
	if (!_stricmp(iso639_code, "sog")) return "Sogdian";
	if (!_stricmp(iso639_code, "som") || !_stricmp(iso639_code, "so")) return "Somali";
	if (!_stricmp(iso639_code, "son")) return "Songhai";
	if (!_stricmp(iso639_code, "wen")) return "Sorbian languages";
	if (!_stricmp(iso639_code, "nso")) return "Sotho, Northern ";
	if (!_stricmp(iso639_code, "sot") || !_stricmp(iso639_code, "st")) return "Sotho, Southern";
	if (!_stricmp(iso639_code, "sai")) return "South American Indian (Other)";
	if (!_stricmp(iso639_code, "esl") || !_stricmp(iso639_code, "spa") || !_stricmp(iso639_code, "es")) return "Spanish";
	if (!_stricmp(iso639_code, "suk")) return "Sukuma";
	if (!_stricmp(iso639_code, "sux")) return "Sumerian";
	if (!_stricmp(iso639_code, "sun") || !_stricmp(iso639_code, "su")) return "Sudanese";
	if (!_stricmp(iso639_code, "sus")) return "Susu";
	if (!_stricmp(iso639_code, "swa") || !_stricmp(iso639_code, "sw")) return "Swahili";
	if (!_stricmp(iso639_code, "ssw")) return "Swazi";
	if (!_stricmp(iso639_code, "sve") || !_stricmp(iso639_code, "swe") || !_stricmp(iso639_code, "sv")) return "Swedish";
	if (!_stricmp(iso639_code, "syr")) return "Syriac";
	if (!_stricmp(iso639_code, "tgl") || !_stricmp(iso639_code, "tl")) return "Tagalog";
	if (!_stricmp(iso639_code, "tah")) return "Tahitian";
	if (!_stricmp(iso639_code, "tgk") || !_stricmp(iso639_code, "tg")) return "Tajik";
	if (!_stricmp(iso639_code, "tmh")) return "Tamashek";
	if (!_stricmp(iso639_code, "tam") || !_stricmp(iso639_code, "ta")) return "Tamil";
	if (!_stricmp(iso639_code, "tat") || !_stricmp(iso639_code, "tt")) return "Tatar";
	if (!_stricmp(iso639_code, "tel") || !_stricmp(iso639_code, "te")) return "Telugu";
	if (!_stricmp(iso639_code, "ter")) return "Tereno";
	if (!_stricmp(iso639_code, "tha") || !_stricmp(iso639_code, "th")) return "Thai";
	if (!_stricmp(iso639_code, "bod") || !_stricmp(iso639_code, "tib") || !_stricmp(iso639_code, "bo")) return "Tibetan";
	if (!_stricmp(iso639_code, "tig")) return "Tigre";
	if (!_stricmp(iso639_code, "tir") || !_stricmp(iso639_code, "ti")) return "Tigrinya";
	if (!_stricmp(iso639_code, "tem")) return "Timne";
	if (!_stricmp(iso639_code, "tiv")) return "Tivi";
	if (!_stricmp(iso639_code, "tli")) return "Tlingit";
	if (!_stricmp(iso639_code, "tog") || !_stricmp(iso639_code, "to")) return "Tonga (Nyasa)";
	if (!_stricmp(iso639_code, "ton")) return "Tonga (Tonga Islands)";
	if (!_stricmp(iso639_code, "tru")) return "Truk";
	if (!_stricmp(iso639_code, "tsi")) return "Tsimshian";
	if (!_stricmp(iso639_code, "tso") || !_stricmp(iso639_code, "ts")) return "Tsonga";
	if (!_stricmp(iso639_code, "tsn") || !_stricmp(iso639_code, "tn")) return "Tswana";
	if (!_stricmp(iso639_code, "tum")) return "Tumbuka";
	if (!_stricmp(iso639_code, "tur") || !_stricmp(iso639_code, "tr")) return "Turkish";
	if (!_stricmp(iso639_code, "ota")) return "Turkish, Ottoman (1500 - 1928)";
	if (!_stricmp(iso639_code, "tuk") || !_stricmp(iso639_code, "tk")) return "Turkmen";
	if (!_stricmp(iso639_code, "tyv")) return "Tuvinian";
	if (!_stricmp(iso639_code, "twi") || !_stricmp(iso639_code, "tw")) return "Twi";
	if (!_stricmp(iso639_code, "uga")) return "Ugaritic";
	if (!_stricmp(iso639_code, "uig") || !_stricmp(iso639_code, "ug")) return "Uighur";
	if (!_stricmp(iso639_code, "ukr") || !_stricmp(iso639_code, "uk")) return "Ukrainian";
	if (!_stricmp(iso639_code, "umb")) return "Umbundu";
	if (!_stricmp(iso639_code, "und")) return "Undetermined";
	if (!_stricmp(iso639_code, "urd") || !_stricmp(iso639_code, "ur")) return "Urdu";
	if (!_stricmp(iso639_code, "uzb") || !_stricmp(iso639_code, "uz")) return "Uzbek";
	if (!_stricmp(iso639_code, "vai")) return "Vai";
	if (!_stricmp(iso639_code, "ven")) return "Venda";
	if (!_stricmp(iso639_code, "vie") || !_stricmp(iso639_code, "vi")) return "Vietnamese";
	if (!_stricmp(iso639_code, "vol") || !_stricmp(iso639_code, "vo")) return "Volap&uuml;k";
	if (!_stricmp(iso639_code, "vot")) return "Votic";
	if (!_stricmp(iso639_code, "wak")) return "Wakashan languages";
	if (!_stricmp(iso639_code, "wal")) return "Walamo";
	if (!_stricmp(iso639_code, "war")) return "Waray";
	if (!_stricmp(iso639_code, "was")) return "Washo";
	if (!_stricmp(iso639_code, "cym") || !_stricmp(iso639_code, "wel") || !_stricmp(iso639_code, "cy")) return "Welsh";
	if (!_stricmp(iso639_code, "wol") || !_stricmp(iso639_code, "wo")) return "Wolof";
	if (!_stricmp(iso639_code, "xho") || !_stricmp(iso639_code, "xh")) return "Xhosa";
	if (!_stricmp(iso639_code, "sah")) return "Yakut";
	if (!_stricmp(iso639_code, "yao")) return "Yao";
	if (!_stricmp(iso639_code, "yap")) return "Yap";
	if (!_stricmp(iso639_code, "yid") || !_stricmp(iso639_code, "yi")) return "Yiddish";
	if (!_stricmp(iso639_code, "yor") || !_stricmp(iso639_code, "yo")) return "Yoruba";
	if (!_stricmp(iso639_code, "zap")) return "Zapotec";
	if (!_stricmp(iso639_code, "zen")) return "Zenaga";
	if (!_stricmp(iso639_code, "zha") || !_stricmp(iso639_code, "za")) return "Zhuang";
	if (!_stricmp(iso639_code, "zul") || !_stricmp(iso639_code, "zu")) return "Zulu";
	if (!_stricmp(iso639_code, "zun")) return "Zuni";

	//Not found, just returning the code supplied
	return iso639_code;
};
int language(const char* b){
	char a[255];
	strncpy_s((char*)&a, 255, fromiso(b),_TRUNCATE);
	_strlwr_s((char*)&a, 255);
	
	if(strstr(a, "japanese")>0) return 2;
	if(strstr(a, "english")>0) return 4;
	if(strstr(a, "en-us")>0) return 4;
	if(strstr(a, "en-uk")>0) return 4;
	if(strstr(a, "german")>0) return 5;
	if(strstr(a, "french")>0) return 6;
	if(strstr(a, "italian")>0) return 8;
	if(strstr(a, "spanish")>0) return 9;
	if(strstr(a, "russian")>0) return 15;
	if(strstr(a, "korean")>0) return 17;
	if(strstr(a, "polish")>0) return 18;
	if(strstr(a, "swedish")>0) return 19;
	if(strstr(a, "arabic")>0) return 20;
	if(strstr(a, "portuguese")>0) return 21;
	if(strstr(a, "norwegian")>0) return 23;
	if(strstr(a, "brazilian")>0) return 25;
	if(strstr(a, "catalan")>0) return 26;
	if(strstr(a, "czech")>0) return 27;
	if(strstr(a, "danish")>0) return 28;
	if(strstr(a, "dutch")>0) return 29;
	if(strstr(a, "finnish")>0) return 30;
	if(strstr(a, "greek")>0) return 31;
	if(strstr(a, "hebrew")>0) return 32;
	if(strstr(a, "hungarian")>0) return 33;
	if(strstr(a, "instrumental")>0) return 34;
	if(strstr(a, "lithuanian")>0) return 35;
	if(strstr(a, "thai")>0) return 37;
	if(strstr(a, "turkish")>0) return 38;
	if(strstr(a, "mandarin")>0) return 41;
	if(strstr(a, "cantonese")>0) return 42;
	if(strstr(a, "chinese")>0) return 3;
	if(strstr(a, "taiwanese")>0) return 43;
	if(strstr(a, "other")>0) return 58;
	if(strstr(a, "romanian")>0) return 61;
	if(strstr(a, "malay")>0) return 62;
	if(strstr(a, "slovak")>0) return 63;
	if(strstr(a, "slovenian")>0) return 64;
	if(strstr(a, "tatar")>0) return 65;
	if(strstr(a, "latvian")>0) return 67;
	if(strstr(a, "georgian")>0) return 68;
	return 1;
}
const char* str_lang(int x){
	switch(x){
		case  1: return "Unknown";
		case  2: return "Japanese";
		case  4: return "English";
		case 20: return "Arabic";
		case 25: return "Brazilian";
		case 26: return "Catalan";
		case 27: return "Czech";
		case  3: return "Chinese (unspecified)";
		case 42: return "Chinese (cantonese)";
		case 41: return "Chinese (mandarin)";
		case 28: return "Danish";
		case 29: return "Dutch";
		case 30: return "Finnish";
		case  6: return "French";
		case  5: return "German";
		case 68: return "Georgian";
		case 31: return "Greek";
		case 32: return "Hebrew";
		case 33: return "Hungarian";
		case 34: return "Instrumental";
		case  8: return "Italian";
		case 17: return "Korean";
		case 67: return "Latvian";
		case 35: return "Lithuanian";
		case 62: return "Malay";
		case 23: return "Norwegian";
		case 18: return "Polish";
		case 21: return "Portuguese";
		case 61: return "Romanian";
		case 15: return "Russian";
		case 63: return "Slovak";
		case 64: return "Slovenian";
		case  9: return "Spanish";
		case 19: return "Swedish";
		case 43: return "Taiwanese";
		case 65: return "Tatar";
		case 37: return "Thai";
		case 38: return "Turkish";
		case 58: return "Other";
		default: return "Unknown";
	}
}
const char* str_codec(int x){
	switch(x){
		case NULL: return "Undefined";
		case V_UNKNOWNX:return "Unknown";
		case V_DIVXU: 	return "DivX Unknown";
		case V_DIVX3: 	return "DivX3";
		case V_DIVX4: 	return "DivX4";
		case V_DIVX5: 	return "DivX5/6";
		case V_MPEG1:	return "MPEG-1";
		case V_MPEG2:	return "MPEG-2";
		case V_ASP:		return "ASP Other";
		case V_RVO:		return "RV Other";
		case V_XVID:	return "XviD";
		case V_MS_MP4X:	return "MS MP4x";
		case V_WMV9:	return "WMV9/3";
		case V_RV9:		return "RV9/10(rv40)";
		case V_H264AVC:	return "H264/AVC";
		case V_OTHER:	return "Other(non-ASP)";

		case A_UNKNOWN:	return "Unknown";
		case A_AC3:		return "AC3";
		case A_WMA:		return "WMA (DivX Audio)";
		case A_MP3_CBR:	return "MP3 CBR";
		case A_MP3_VBR: return "MP3 VBR";
		case A_MSAUDIO: return "MS Audio";
		case A_VORBIS:	return "Ogg Vorbis";
		case A_AAC:		return "AAC";
		case A_PCM:		return "PCM";
		case A_MP2:		return "MP2";
		case A_DTS:		return "DTS";
		case A_OTHER:	return "Other";

		case S_UNKNOWN:	return "Unknown";
		case S_NONEX:	return "None";
		case S_SRT:		return "SRT";
		case S_SSA:		return "SSA";
		case S_ASS:		return "ASS";
		case S_USF:		return "USF";
		case S_TX3G:	return "TX3G";
		case S_BMP:		return "BMP";
		case S_XSUB:	return "XSUB";
		case S_VOBSUB:	return "VobSub";
	}
	return "Codec not recognized.";
}
const char* str_ar(int id){
	switch(id){
		case VIDEO_AR_133: return "4:3";
		case VIDEO_AR_177: return "16:9";
		case VIDEO_AR_166: return "1.66:1";
		case VIDEO_AR_185: return "1.85:1";
		case VIDEO_AR_200: return "2.00:1";
		case VIDEO_AR_221: return "2.21:1";
		case VIDEO_AR_235: return "2.35:1";
		case GENERAL_OTHER: return "Other";
		default: return "Unknown (aka not common)";
	}
}
const char* str_chan(int id){
	switch(id){
		case AUDIO_CHAN_MONO: return "Mono";// - Mono";
		case AUDIO_CHAN_STEREO: return "Stereo";// - Stereo";
		case AUDIO_CHAN_DOLBY: return "Dolby Surround";// - Dolby Surround";
		case AUDIO_CHAN_5_1_S: return "5.1 Surround";// - 5.1 Surround";
		case AUDIO_CHAN_6_1_S: return "6.1 Surround";// - 6.1 Surround";
		case AUDIO_CHAN_7_1_S: return "7.1 Surround";// - 7.1 Surround";
		case GENERAL_OTHER: return "Other";
		default: return "Unknown";
	}
}
const char* str_sub(int id){
	switch(id){
		case S_SRT: return "plain soft";
		case S_ASS:
		case S_SSA:
		case S_USF: 
		case S_TX3G:return "styled soft";
		case S_BMP:
		case S_VOBSUB:
		case S_XSUB: return "image soft";
		case S_UNKNOWN : return "unknown";
		default : return "hard";
	}
}
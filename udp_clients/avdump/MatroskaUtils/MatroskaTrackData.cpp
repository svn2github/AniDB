/*
 *  Part of The TCMP Matroska CDL, a plugin to access extra features of Matroska files with TCMP
 *
 *  MatroskaTrackData.cpp
 *
 *  Copyright (C) Jory Stone - June 2003
 *  ISO639 Language List from Mosu's mkvmerge
 *
 *  The TCMP Matroska CDL is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  The TCMP Matroska CDL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The TCMP Matroska CDL; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*!
    \file MatroskaTrackData.cpp
		\version \$Id: MatroskaTrackData.cpp,v 1.38 2004/03/08 07:56:51 jcsston Exp $
    \brief Some general code to make working with Matroska easier
		\author Jory Stone     <jcsston @ toughguy.net>
		
		Inspired by ;) John Cannon (spyder482@yahoo.com) (c) 2003
*/

#include "MatroskaTrackData.h"
#include "JString.h"
#ifdef WIN32
#include <Shlwapi.h>
#endif

//using namespace MatroskaUtilsNamespace;
//namespace MatroskaUtilsNamespace {

char *iso639_2B_codes[] = {
	"abk", "ace", "ach", "ada", "ady", "ady", "aar", "afh",
	"afr", "afa", "aka", "akk", "alb", "ale", "alg", "tut",
	"amh", "apa", "ara", "arg", "arc", "arp", "arn", "arw",
	"arm", "art", "asm", "ast", "ath", "aus", "map", "ava",
	"ave", "awa", "aym", "aze", "ast", "ban", "bat", "bal",
	"bam", "bai", "bad", "bnt", "bas", "bak", "baq", "btk",
	"bej", "bel", "bem", "ben", "ber", "bho", "bih", "bik",
	"bin", "bis", "nob", "bos", "bra", "bre", "bug", "bul",
	"bua", "bur", "cad", "car", "spa", "cat", "cau", "ceb",
	"cel", "cai", "chg", "cmc", "cha", "che", "chr", "nya",
	"chy", "chb", "nya", "chi", "chn", "chp", "cho", "zha",
	"chu", "chu", "chk", "chv", "cop", "cor", "cos", "cre",
	"mus", "crp", "cpe", "cpf", "cpp", "crh", "crh", "scr",
	"cus", "cze", "dak", "dan", "dar", "day", "del", "din",
	"doi", "dgr", "dra", "dua", "dut", "dum", "dyu", "dzo",
	"efi", "egy", "eka", "elx", "eng", "enm", "ang", "myv",
	"epo", "est", "ewe", "ewo", "fan", "fat", "fao", "fij",
	"fin", "fiu", "dut", "fon", "fre", "frm", "fro", "fry",
	"fur", "ful", "gaa", "gla", "glg", "lug", "gay", "gba",
	"gez", "geo", "ger", "nds", "gmh", "goh", "gem", "kik",
	"gil", "gon", "gor", "got", "grb", "grc", "gre", "kal",
	"grn", "guj", "gwi", "hai", "hat", "hat", "hau", "haw",
	"heb", "her", "hil", "him", "hin", "hmo", "hit", "hmn",
	"hun", "hup", "iba", "ice", "ido", "ibo", "ijo", "ilo",
	"smn", "inc", "ine", "ind", "inh", "ina", "ile", "iku",
	"ipk", "ira", "gle", "mga", "sga", "iro", "ita", "jpn",
	"jav", "jrb", "jpr", "kbd", "kab", "kac", "kal", "xal",
	"kam", "kan", "kau", "krc", "kaa", "kar", "kas", "csb",
	"kaw", "kaz", "kha", "khm", "khi", "kho", "kik", "kmb",
	"kin", "kir", "kom", "kon", "kok", "kor", "kos", "kpe",
	"kro", "kua", "kum", "kur", "kru", "kut", "kua", "lad",
	"lah", "lam", "lao", "lat", "lav", "ltz", "lez", "lim",
	"lim", "lim", "lin", "lit", "nds", "nds", "loz", "lub",
	"lua", "lui", "smj", "lun", "luo", "lus", "ltz", "mac",
	"mad", "mag", "mai", "mak", "mlg", "may", "mal", "mlt",
	"mnc", "mdr", "man", "mni", "mno", "glv", "mao", "mar",
	"chm", "mah", "mwr", "mas", "myn", "men", "mic", "min",
	"mis", "moh", "mdf", "mol", "mkh", "lol", "mon", "mos",
	"mul", "mun", "nah", "nau", "nav", "nav", "nde", "nbl",
	"ndo", "nap", "nep", "new", "nia", "nic", "ssa", "niu",
	"nog", "non", "nai", "sme", "nde", "nor", "nob", "nno",
	"nub", "nym", "nya", "nyn", "nno", "nyo", "nzi", "oci",
	"oji", "chu", "chu", "chu", "ori", "orm", "osa", "oss",
	"oss", "oto", "pal", "pau", "pli", "pam", "pag", "pan",
	"pap", "paa", "per", "peo", "phi", "phn", "pon", "pol",
	"por", "pra", "oci", "pro", "pus", "que", "roh", "raj",
	"rap", "rar", "qaa", "roa", "rum", "rom", "run", "rus",
	"sal", "sam", "smi", "smo", "sad", "sag", "san", "sat",
	"srd", "sas", "nds", "sco", "gla", "sel", "sem", "scc",
	"srr", "shn", "sna", "iii", "sid", "sgn", "bla", "snd",
	"sin", "sit", "sio", "sms", "den", "sla", "slo", "slv",
	"sog", "som", "son", "snk", "wen", "nso", "sot", "sai",
	"sma", "nbl", "suk", "sux", "sun", "sus", "swa",
	"ssw", "swe", "syr", "tgl", "tah", "tai", "tgk", "tmh",
	"tam", "tat", "tel", "ter", "tet", "tha", "tib", "tig",
	"tir", "tem", "tiv", "tli", "tpi", "tkl", "tog", "ton",
	"tsi", "tso", "tsn", "tum", "tup", "tur", "ota", "tuk",
	"tvl", "tyv", "twi", "udm", "uga", "uig", "ukr", "umb",
	"und", "urd", "uzb", "vai", "ven", "vie", "vol", "vot",
	"wak", "wal", "wln", "war", "was", "wel", "wol", "xho",
	"sah", "yao", "yap", "yid", "yor", "ypk", "znd", "zap",
	"zen", "zha", "zul", "zun", NULL
};

MatroskaSegmentInfo::MatroskaSegmentInfo(){	
	m_infoDuration = 0.0;
	m_infoTimecodeScale = TIMECODE_SCALE;
	m_infoDateUnix = 0;
}
MatroskaSegmentInfo::~MatroskaSegmentInfo(){

}
void MatroskaSegmentInfo::SetDuration(float the_duration){
	m_infoDuration = the_duration;
}
float MatroskaSegmentInfo::GetDuration(){
	return m_infoDuration * (float) m_infoTimecodeScale / (float)1000000000.0;
}

/*const char *MatroskaSegmentInfo::GetNiceDuration(){
	if (m_infoNiceDuration.length() > 0) {
		return m_infoNiceDuration.c_str();

	}else {
		//The m_infoDuration should be stored in mill-seconds and duration will be in seconds
		TCHAR buffer[256];
		float duration = m_infoDuration;
		
#ifdef WIN32
		StrFromTimeInterval(buffer, 255, static_cast<DWORD>(duration*1000), 5);
		StrTrim(buffer, _T(" "));

		UTFstring utf8;
		utf8 = buffer;
		m_infoNiceDuration = utf8.GetUTF8();
#endif
		return m_infoNiceDuration.c_str();
	}
};

UTFstring MatroskaSegmentInfo::GetNiceDurationW()
{	
	UTFstring utf8;
	GetNiceDuration();
	utf8.SetUTF8(m_infoNiceDuration);

	return utf8;
}*/
void MatroskaSegmentInfo::SetTimecodeScale(uint64 timecodeScale){
	m_infoTimecodeScale = timecodeScale;
}
uint64 MatroskaSegmentInfo::GetTimecodeScale(){
	return m_infoTimecodeScale;
}

/*******************************************
 *  Begin MatroskaTrackInfo class.         *
 *******************************************/
MatroskaTrackInfo::MatroskaTrackInfo(MatroskaSegmentInfo *segment) {
	ODS("MatroskaTrackInfo::MatroskaTrackInfo(MatroskaSegmentInfo *)");

	::MatroskaTrackInfo(segment,1);
}

MatroskaTrackInfo::MatroskaTrackInfo(MatroskaSegmentInfo *segment, uint64 trackUID) {
	ODS("MatroskaTrackInfo::MatroskaTrackInfo(MatroskaSegmentInfo *, uint64)");

	m_trackUID = trackUID;
	m_trackNumber = 0;
	m_trackType = 0;

	m_segmentInfo = segment;
	m_min_cache = 0;
	m_max_cache = 0;	
	m_lacing_flag = 0;
	m_default_flag = 0;

	m_videoInfo = NULL;
	m_audioInfo = NULL;
	//m_tagInfo = NULL;

	m_bytes = 0;
//	m_time = 0;
//	m_time_0 = -1.0;
	m_timecodes = new std::vector<uint64>();
//	m_user_data = true;
	m_check_mp3_vbr = true;
	m_wbs = 0;
	m_cps = 0;
	m_timecode_scale = 1.0;
}

MatroskaTrackInfo::~MatroskaTrackInfo() {
	ODS("MatroskaTrackInfo::~MatroskaTrackInfo()");

	delete m_timecodes;

	if (m_videoInfo != NULL) {
		delete m_videoInfo;
		m_videoInfo = NULL;
	}
	if (m_audioInfo != NULL) {
		delete m_audioInfo;
		m_audioInfo = NULL;
	}
#ifdef USING_TAG_DATA
	if (m_tagInfo != NULL) {
		delete m_tagInfo;
		m_tagInfo = NULL;
	}
#endif
}
//Returns the track number of this track
uint16 MatroskaTrackInfo::GetTrackNumber() {
	return m_trackNumber;
}
//Sets the track number of this track
void MatroskaTrackInfo::SetTrackNumber(uint16 trackNumber) {
	m_trackNumber = trackNumber;
}
//Returns the UID of this track
uint64 MatroskaTrackInfo::GetTrackUID() {
	return m_trackUID;
}
//Sets the track number of this track
void MatroskaTrackInfo::SetTrackUID(uint64 trackUID) {
	m_trackUID = trackUID;
}
//Returns the track type of this track
uint8 MatroskaTrackInfo::GetTrackType() {
	return m_trackType;
}
//Returns the track type of this track as a char *
const TCHAR *MatroskaTrackInfo::GetTrackTypeStr() {
	switch (m_trackType) {
		case track_audio:
			return _W("Audio");
			break;
		case track_video:
			return _W("Video");
			break;
		case track_subtitle:
			return _W("Subtitle");
			break;
		case 0:
			return _W("Internal Tag Track (You shouldn't be seeing this :P)");
			break;
		default:
			return _W("Unknown");
			break;
	}
}
//Sets the track type of this track
void MatroskaTrackInfo::SetTrackType(uint8 trackType) {
	m_trackType = trackType;
}
//Returns the codec ID of this track
const char* MatroskaTrackInfo::GetCodecID() {		
	return m_codecID.c_str();
}
//Sets the codec ID of this track
void MatroskaTrackInfo::SetCodecID(const char* codecID)
{
	m_codecID = codecID;
}
//Returns a pointer to the video information for this track or NULL if it is not a video track.
MatroskaVideoTrackInfo* MatroskaTrackInfo::GetVideoInfo() {
	return m_videoInfo;
}
//Sets the video information for this track.
void MatroskaTrackInfo::SetVideoInfo(MatroskaVideoTrackInfo* videoInfo) {
	m_videoInfo = videoInfo;
}
//Returns a pointer to the audio information for this track or NULL if it is not a audio track.
MatroskaAudioTrackInfo* MatroskaTrackInfo::GetAudioInfo() {
	return m_audioInfo;
}
//Sets the audio information for this track.
void MatroskaTrackInfo::SetAudioInfo(MatroskaAudioTrackInfo* audioInfo){
	m_audioInfo = audioInfo;
}
//Returns a pointer to the Tag information for this track or NULL if there is not a Tag segment.
/*MatroskaTagInfo* MatroskaTrackInfo::GetTagInfo()
{
	return m_tagInfo;
}

//Sets the Tag information for this track.
void MatroskaTrackInfo::SetTagInfo(MatroskaTagInfo* tagInfo)
{
	if (m_tagInfo != tagInfo)
	{
		delete m_tagInfo;
		m_tagInfo = tagInfo;
		
		if (m_tagInfo != NULL) {
			m_tagInfo->Tag_TrackUID = this->GetTrackUID();
		}
	}
}

void MatroskaTrackInfo::MergeWithThisTrackInfo(MatroskaTrackInfo *source_track) 
{
	this->m_audioInfo = source_track->m_audioInfo;
	this->m_codecID = source_track->m_codecID;
	this->m_CodecOldID = source_track->m_CodecOldID;
	this->m_default_flag = source_track->m_default_flag;
	this->m_lacing_flag = source_track->m_lacing_flag;
	this->m_max_cache = source_track->m_max_cache;
	this->m_min_cache = source_track->m_min_cache;
	if ((this->m_tagInfo != NULL) && (source_track->m_tagInfo != NULL))
		this->m_tagInfo->MergeWithThisTagInfo(source_track->m_tagInfo);
	this->m_trackLanguage = source_track->m_trackLanguage;
	this->m_trackNumber = source_track->m_trackNumber;
	this->m_trackType = source_track->m_trackType;
	this->m_trackUID = source_track->m_trackUID;
	this->m_videoInfo = source_track->m_videoInfo;
	delete source_track;
};
*/
const char *MatroskaTrackInfo::GetLanguageName()
{
	if (m_trackLanguage.length() < 1)
		return "English (default)";
	
	return GetLanguageNameFromISO639(m_trackLanguage.c_str());
};

const char *MatroskaTrackInfo::GetLanguageNameFromISO639(const char *iso639_code)
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

MatroskaVideoTrackInfo::MatroskaVideoTrackInfo()
{
	ODS("Matroska Video Track Info Created\n");
	video_Width = 0;
	video_Height = 0;
	video_DisplayWidth = 0;
	video_DisplayHeight = 0;
	video_FrameRate = 0;
	video_ColorDepth = 0;
	video_Interlaced = 0;
	//video_VFR = 0;
};

MatroskaVideoTrackInfo::~MatroskaVideoTrackInfo()
{
	ODS("MatroskaVideoTrackInfo::~MatroskaVideoTrackInfo()");
};

/// Video FourCC Codec list from http://kibus1.narod.ru/frames_eng.htm?sof/abcavi/fourcc.htm
const char *MatroskaVideoTrackInfo::GetFourCCName(const char *fourcc)
{
	if (!_strnicmp(fourcc, "3IV0", 4)) return "MPEG4-based codec 3ivx";
	if (!_strnicmp(fourcc, "3IV1", 4)) return "MPEG4-based codec 3ivx";
	if (!_strnicmp(fourcc, "3IV2", 4)) return "MPEG4-based codec 3ivx";
	if (!_strnicmp(fourcc, "3IVD", 4)) return "FFmpeg DivX ;-) (MS MPEG-4 v3)";
	if (!_strnicmp(fourcc, "3IVX", 4)) return "MPEG4-based codec 3ivx";
	if (!_strnicmp(fourcc, "AAS4", 4)) return "Autodesk Animator codec (RLE)";
	if (!_strnicmp(fourcc, "AASC", 4)) return "Autodesk Animator codec (RLE)";
	if (!_strnicmp(fourcc, "ABYR", 4)) return "Kensington codec";
	if (!_strnicmp(fourcc, "ADV1", 4)) return "Loronix WaveCodec (used in various CCTV products)";
	if (!_strnicmp(fourcc, "ADVJ", 4)) return "Avid M-JPEG Avid Technology Also known as AVRn";
	if (!_strnicmp(fourcc, "AEMI", 4)) return "Array VideoONE MPEG1-I Capture";
	if (!_strnicmp(fourcc, "AFLC", 4)) return "Autodesk Animator FLC (256 color)";
	if (!_strnicmp(fourcc, "AFLI", 4)) return "Autodesk Animator FLI (256 color)";
	if (!_strnicmp(fourcc, "AMPG", 4)) return "Array VideoONE MPEG";
	if (!_strnicmp(fourcc, "ANIM", 4)) return "Intel RDX";
	if (!_strnicmp(fourcc, "AP41", 4)) return "AngelPotion Definitive (hack MS MP43)";
	if (!_strnicmp(fourcc, "ASV1", 4)) return "Asus Video V1";
	if (!_strnicmp(fourcc, "ASV2", 4)) return "Asus Video V2";
	if (!_strnicmp(fourcc, "ASVX", 4)) return "Asus Video 2.0";
	if (!_strnicmp(fourcc, "AUR2", 4)) return "Aura 2 Codec - YUV 422";
	if (!_strnicmp(fourcc, "AURA", 4)) return "Aura 1 Codec - YUV 411";
	if (!_strnicmp(fourcc, "AVDJ", 4)) return "Avid Motion JPEG";
	if (!_strnicmp(fourcc, "AVI1", 4)) return "MainConcept Motion JPEG Codec";
	if (!_strnicmp(fourcc, "AVI2", 4)) return "MainConcept Motion JPEG Codec";
	if (!_strnicmp(fourcc, "AVRN", 4)) return "Avid Motion JPEG";
	if (!_strnicmp(fourcc, "AZPR", 4)) return "Quicktime Apple Video";
	if (!_strnicmp(fourcc, "BGR", 4)) return "Uncompressed BGR32 8:8:8:8";
	if (!_strnicmp(fourcc, "BGR(15)", 4)) return "Uncompressed BGR15 5:5:5";
	if (!_strnicmp(fourcc, "BGR(16)", 4)) return "Uncompressed BGR16 5:6:5";
	if (!_strnicmp(fourcc, "BGR(24)", 4)) return "Uncompressed BGR24 8:8:8";
	if (!_strnicmp(fourcc, "BINK", 4)) return "Bink Video (RAD Game Tools) (256 color)";
	if (!_strnicmp(fourcc, "BITM", 4)) return "Microsoft H.261";
	if (!_strnicmp(fourcc, "BLZ0", 4)) return "FFmpeg MPEG-4";
	if (!_strnicmp(fourcc, "BT20", 4)) return "Conexant (ex Brooktree) ProSummer MediaStream";
	if (!_strnicmp(fourcc, "BTCV", 4)) return "Conexant Composite Video";
	if (!_strnicmp(fourcc, "BTVC", 4)) return "Conexant Composite Video";
	if (!_strnicmp(fourcc, "BW10", 4)) return "Data Translation Broadway MPEG Capture/Compression";
	if (!_strnicmp(fourcc, "CC12", 4)) return "Intel YUV12 Codec";
	if (!_strnicmp(fourcc, "CDVC", 4)) return "Canopus DV Codec";
	if (!_strnicmp(fourcc, "CFCC", 4)) return "Conkrete DPS Perception Motion JPEG";
	if (!_strnicmp(fourcc, "CGDI", 4)) return "Camcorder Video (MS Office 97)";
	if (!_strnicmp(fourcc, "CHAM", 4)) return "WinNow Caviara Champagne";
	if (!_strnicmp(fourcc, "CJPG", 4)) return "Creative Video Blaster Webcam Go JPEG";
	if (!_strnicmp(fourcc, "CLJR", 4)) return "Cirrus Logic YUV 4:1:1";
	if (!_strnicmp(fourcc, "CLPL", 4)) return "Format similar to YV12 but including a level of indirection.";
	if (!_strnicmp(fourcc, "CMYK", 4)) return "Common Data Format in Printing";
	if (!_strnicmp(fourcc, "COL0", 4)) return "FFmpeg DivX ;-) (MS MPEG-4 v3)";
	if (!_strnicmp(fourcc, "COL1", 4)) return "FFmpeg DivX ;-) (MS MPEG-4 v3)";
	if (!_strnicmp(fourcc, "CPLA", 4)) return "Weitek YUV 4:2:0 Planar";
	if (!_strnicmp(fourcc, "CRAM", 4)) return "Microsoft Video 1";
	if (!_strnicmp(fourcc, "CVID", 4)) return "Cinepak by Radius YUV 4:2:2";
	if (!_strnicmp(fourcc, "CWLT", 4)) return "Microsoft Color WLT DIB";
	if (!_strnicmp(fourcc, "CYUV", 4)) return "Creative Labs YUV 4:2:2";
	if (!_strnicmp(fourcc, "CYUY", 4)) return "ATI Technologies YUV";
	if (!_strnicmp(fourcc, "D261", 4)) return "DEC H.261";
	if (!_strnicmp(fourcc, "D263", 4)) return "DEC H.263";
	if (!_strnicmp(fourcc, "DCAP", 4)) return "Pinnacle DV25 Codec";
	if (!_strnicmp(fourcc, "DIB", 4)) return "Full Frames (Uncompressed)";
	if (!_strnicmp(fourcc, "DIV1", 4)) return "FFmpeg-4 V1 (hacked MS MPEG-4 V1)";
	if (!_strnicmp(fourcc, "DIV2", 4)) return "MS MPEG-4 V2";
	if (!_strnicmp(fourcc, "DIV3", 4)) return "Low motion DivX MPEG-4";
	if (!_strnicmp(fourcc, "DIV4", 4)) return "Fast motion DivX MPEG-4";
	if (!_strnicmp(fourcc, "DIV5", 4)) return "DivX MPEG-4";
	if (!_strnicmp(fourcc, "DIV6", 4)) return "DivX MPEG-4";
	if (!_strnicmp(fourcc, "DIVX", 4)) return "OpenDivX (DivX 4.0 and later)";
	if (!_strnicmp(fourcc, "DMB1", 4)) return "Matrox Rainbow Runner hardware compression (Motion JPEG)";
	if (!_strnicmp(fourcc, "DMB2", 4)) return "Motion JPEG codec used by Paradigm";
	if (!_strnicmp(fourcc, "DP02", 4)) return "DynaPel MPEG-4";
	if (!_strnicmp(fourcc, "DPS0", 4)) return "DPS Reality Motion JPEG";
	if (!_strnicmp(fourcc, "DPSC", 4)) return "DPS PAR Motion JPEG";
	if (!_strnicmp(fourcc, "DRWX", 4)) return "Pinnacle DV25 Codec";
	if (!_strnicmp(fourcc, "DSVD", 4)) return "Microsoft DirectShow DV";
	if (!_strnicmp(fourcc, "DUCK", 4)) return "True Motion 1.0";
	if (!_strnicmp(fourcc, "DV25", 4)) return "Matrox DVCPRO codec";
	if (!_strnicmp(fourcc, "DV50", 4)) return "Matrox DVCPRO50 codec";
	if (!_strnicmp(fourcc, "DVC", 4)) return "MainConcept DV Codec";
	if (!_strnicmp(fourcc, "DVCP", 4)) return "Sony Digital Video (DV)";
	if (!_strnicmp(fourcc, "DVCS", 4)) return "MainConcept DV Codec";
	if (!_strnicmp(fourcc, "DVE2", 4)) return "InSoft DVE-2 Videoconferencing Codec";
	if (!_strnicmp(fourcc, "DVHD", 4)) return "DV 1125 lines at 30.00 Hz or 1250 lines at 25.00 Hz";
	if (!_strnicmp(fourcc, "DVMA", 4)) return "Darim Vision DVMPEG (dummy for MPEG compressor)";
	if (!_strnicmp(fourcc, "DVSD", 4)) return "Sony Digital Video (DV) 525 lines at 29.97 Hz or 625 lines at 25.00 Hz";
	if (!_strnicmp(fourcc, "DVSL", 4)) return "DV compressed in SD (SDL)";
	if (!_strnicmp(fourcc, "DVX1", 4)) return "Lucent DVX1000SP Video Decoder";
	if (!_strnicmp(fourcc, "DVX2", 4)) return "Lucent DVX2000S Video Decoder";
	if (!_strnicmp(fourcc, "DVX3", 4)) return "Lucent DVX3000S Video Decoder";
	if (!_strnicmp(fourcc, "DX50", 4)) return "DivX 5.0 codec";
	if (!_strnicmp(fourcc, "DXT1", 4)) return "DirectX Compressed Texture (1bit alpha channel)";
	if (!_strnicmp(fourcc, "DXT2", 4)) return "DirectX Compressed Texture";
	if (!_strnicmp(fourcc, "DXT3", 4)) return "DirectX Compressed Texture (4bit alpha channel)";
	if (!_strnicmp(fourcc, "DXT4", 4)) return "DirectX Compressed Texture";
	if (!_strnicmp(fourcc, "DXT5", 4)) return "DirectX Compressed Texture (3bit alpha channel with interpolation)";
	if (!_strnicmp(fourcc, "DXTC", 4)) return "DirectX Texture Compression";
	if (!_strnicmp(fourcc, "EKQ0", 4)) return "Elsa graphics card quick codec";
	if (!_strnicmp(fourcc, "ELK0", 4)) return "Elsa graphics card codec";
	if (!_strnicmp(fourcc, "EM2V", 4)) return "Etymonix MPEG-2 I-frame";
	if (!_strnicmp(fourcc, "ESCP", 4)) return "Eidos Technologies Escape codec";
	if (!_strnicmp(fourcc, "ETV1", 4)) return "eTreppid Video Codec";
	if (!_strnicmp(fourcc, "ETV2", 4)) return "eTreppid Video Codec";
	if (!_strnicmp(fourcc, "ETVC", 4)) return "eTreppid Video Codec";
	if (!_strnicmp(fourcc, "FLIC", 4)) return "Autodesk FLI/FLC Animation";
	if (!_strnicmp(fourcc, "FLJP", 4)) return "Field Encoded Motion JPEG (Targa emulation)";
	if (!_strnicmp(fourcc, "FRWA", 4)) return "Darim Vision Forward Motion JPEG with Alpha-channel";
	if (!_strnicmp(fourcc, "FRWD", 4)) return "Darim Vision Forward Motion JPEG";
	if (!_strnicmp(fourcc, "FRWT", 4)) return "Darim Vision Forward Motion JPEG";
	if (!_strnicmp(fourcc, "FRWU", 4)) return "Darim Vision Forward Uncompressed";
	if (!_strnicmp(fourcc, "FVF1", 4)) return "Iterated Systems Fractal Video Frame";
	if (!_strnicmp(fourcc, "FVFW", 4)) return "ff MPEG-4 based on XviD codec";
	if (!_strnicmp(fourcc, "GEPJ", 4)) return "White Pine (ex Paradigm Matrix) Motion JPEG Codec";
	if (!_strnicmp(fourcc, "GLZW", 4)) return "Motion LZW by gabest@freemail.hu";
	if (!_strnicmp(fourcc, "GPEG", 4)) return "Motion JPEG by gabest@freemail.hu (with floating point)";
	if (!_strnicmp(fourcc, "GPJM", 4)) return "Pinnacle ReelTime MJPEG Codec";
	if (!_strnicmp(fourcc, "GREY", 4)) return "Apparently a duplicate of Y800";
	if (!_strnicmp(fourcc, "GWLT", 4)) return "Microsoft Greyscale WLT DIB";
	if (!_strnicmp(fourcc, "H260", 4)) return "Intel ITU H.260";
	if (!_strnicmp(fourcc, "H261", 4)) return "Intel ITU H.261";
	if (!_strnicmp(fourcc, "H262", 4)) return "Intel ITU H.262";
	if (!_strnicmp(fourcc, "H263", 4)) return "Intel ITU H.263";
	if (!_strnicmp(fourcc, "H264", 4)) return "Intel ITU H.264";
	if (!_strnicmp(fourcc, "H265", 4)) return "Intel ITU H.265";
	if (!_strnicmp(fourcc, "H266", 4)) return "Intel ITU H.266";
	if (!_strnicmp(fourcc, "H267", 4)) return "Intel ITU H.267";
	if (!_strnicmp(fourcc, "H268", 4)) return "Intel ITU H.268";
	if (!_strnicmp(fourcc, "H269", 4)) return "Intel ITU H.263 for POTS-based videoconferencing";
	if (!_strnicmp(fourcc, "HFYU", 4)) return "Huffman Lossless Codec YUV and RGB formats (with Alpha-channel)";
	if (!_strnicmp(fourcc, "HMCR", 4)) return "Rendition Motion Compensation Format";
	if (!_strnicmp(fourcc, "HMRR", 4)) return "Rendition Motion Compensation Format";
	if (!_strnicmp(fourcc, "I263", 4)) return "Intel ITU H.263";
	if (!_strnicmp(fourcc, "I420", 4)) return "Intel Indeo 4 H.263";
	if (!_strnicmp(fourcc, "IAN", 4)) return "Indeo 4 (RDX) Codec";
	if (!_strnicmp(fourcc, "ICLB", 4)) return "InSoft CellB Videoconferencing Codec";
	if (!_strnicmp(fourcc, "IF09", 4)) return "Microsoft H.261";
	if (!_strnicmp(fourcc, "IGOR", 4)) return "Power DVD";
	if (!_strnicmp(fourcc, "IJPG", 4)) return "Intergraph JPEG";
	if (!_strnicmp(fourcc, "ILVC", 4)) return "Intel Layered Video";
	if (!_strnicmp(fourcc, "ILVR", 4)) return "ITU H.263+ Codec";
	if (!_strnicmp(fourcc, "IMC1", 4)) return "As YV12, except the U and V planes each have the same stride as the Y plane";
	if (!_strnicmp(fourcc, "IMC2", 4)) return "Similar to IMC1, except that the U and V lines are interleaved at half stride boundaries";
	if (!_strnicmp(fourcc, "IMC3", 4)) return "As IMC1, except that U and V are swapped";
	if (!_strnicmp(fourcc, "IMC4", 4)) return "As IMC2, except that U and V are swapped";
	if (!_strnicmp(fourcc, "IPDV", 4)) return "Giga AVI DV Codec";
	if (!_strnicmp(fourcc, "IR21", 4)) return "Intel Indeo 2.1";
	if (!_strnicmp(fourcc, "IRAW", 4)) return "Intel YUV Uncompressed";
	if (!_strnicmp(fourcc, "IUYV", 4)) return "Interlaced version of UYVY (line order 0,2,4 then 1,3,5 etc)";
	if (!_strnicmp(fourcc, "IV30", 4)) return "Intel Indeo Video 3";
	if (!_strnicmp(fourcc, "IV31", 4)) return "Intel Indeo Video 3.1";
	if (!_strnicmp(fourcc, "IV32", 4)) return "Intel Indeo Video 3.2";
	if (!_strnicmp(fourcc, "IV33", 4)) return "Intel Indeo Video 3.3";
	if (!_strnicmp(fourcc, "IV34", 4)) return "Intel Indeo Video 3.4";
	if (!_strnicmp(fourcc, "IV35", 4)) return "Intel Indeo Video 3.5";
	if (!_strnicmp(fourcc, "IV36", 4)) return "Intel Indeo Video 3.6";
	if (!_strnicmp(fourcc, "IV37", 4)) return "Intel Indeo Video 3.7";
	if (!_strnicmp(fourcc, "IV38", 4)) return "Intel Indeo Video 3.8";
	if (!_strnicmp(fourcc, "IV39", 4)) return "Intel Indeo Video 3.9";
	if (!_strnicmp(fourcc, "IV40", 4)) return "Intel Indeo Video 4.0";
	if (!_strnicmp(fourcc, "IV41", 4)) return "Intel Indeo Video 4.1";
	if (!_strnicmp(fourcc, "IV42", 4)) return "Intel Indeo Video 4.2";
	if (!_strnicmp(fourcc, "IV43", 4)) return "Intel Indeo Video 4.3";
	if (!_strnicmp(fourcc, "IV44", 4)) return "Intel Indeo Video 4.4";
	if (!_strnicmp(fourcc, "IV45", 4)) return "Intel Indeo Video 4.5";
	if (!_strnicmp(fourcc, "IV46", 4)) return "Intel Indeo Video 4.6";
	if (!_strnicmp(fourcc, "IV47", 4)) return "Intel Indeo Video 4.7";
	if (!_strnicmp(fourcc, "IV48", 4)) return "Intel Indeo Video 4.8";
	if (!_strnicmp(fourcc, "IV49", 4)) return "Intel Indeo Video 4.9";
	if (!_strnicmp(fourcc, "IV50", 4)) return "Intel Indeo Video 5.0 Wavelet";
	if (!_strnicmp(fourcc, "IY41", 4)) return "Interlaced version of Y41P (line order 0,2,4,...,1,3,5...)";
	if (!_strnicmp(fourcc, "IYU1", 4)) return "12 bit format used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec";
	if (!_strnicmp(fourcc, "IYU2", 4)) return "24 bit format used in mode 2 of the IEEE 1394 Digital Camera 1.04 spec";
	if (!_strnicmp(fourcc, "IYUV", 4)) return "Intel Indeo iYUV 4:2:0";
	if (!_strnicmp(fourcc, "JBYR", 4)) return "Kensington Video Codec";
	if (!_strnicmp(fourcc, "JFIF", 4)) return "Motion JPEG (FFmpeg)";
	if (!_strnicmp(fourcc, "JPEG", 4)) return "Still Image JPEG DIB";
	if (!_strnicmp(fourcc, "JPGL", 4)) return "DIVIO JPEG Light for WebCams (Pegasus Lossless JPEG)";
	if (!_strnicmp(fourcc, "KMVC", 4)) return "Karl Morton Video Codec";
	if (!_strnicmp(fourcc, "L261", 4)) return "Lead Technologies H.261";
	if (!_strnicmp(fourcc, "L263", 4)) return "Lead Technologies H.263";
	if (!_strnicmp(fourcc, "LCMW", 4)) return "Lead Technologies Motion CMW Codec";
	if (!_strnicmp(fourcc, "LEAD", 4)) return "LEAD Video Codec";
	if (!_strnicmp(fourcc, "LGRY", 4)) return "Lead Technologies Grayscale Image";
	if (!_strnicmp(fourcc, "LJPG", 4)) return "LEAD Motion JPEG Codec";
	if (!_strnicmp(fourcc, "LSVM", 4)) return "Vianet Lighting Strike Vmail (Streaming)";
	if (!_strnicmp(fourcc, "LZO1", 4)) return "LZO compressed (lossless codec)";
	if (!_strnicmp(fourcc, "M261", 4)) return "Microsoft H.261";
	if (!_strnicmp(fourcc, "M263", 4)) return "Microsoft H.263";
	if (!_strnicmp(fourcc, "M4S2", 4)) return "Microsoft MPEG-4 (hacked MS MPEG-4)";
	if (!_strnicmp(fourcc, "MC12", 4)) return "ATI Motion Compensation Format";
	if (!_strnicmp(fourcc, "MC24", 4)) return "MainConcept Motion JPEG Codec";
	if (!_strnicmp(fourcc, "MCAM", 4)) return "ATI Motion Compensation Format";
	if (!_strnicmp(fourcc, "MDVD", 4)) return "Alex MicroDVD Video (hacked MS MPEG-4)";
	if (!_strnicmp(fourcc, "MJ2C", 4)) return "Morgan Multimedia JPEG2000 Compression";
	if (!_strnicmp(fourcc, "MJPA", 4)) return "Pinnacle Motion JPEG with Alpha-channel";
	if (!_strnicmp(fourcc, "MJPB", 4)) return "Motion JPEG codec";
	if (!_strnicmp(fourcc, "MJPG", 4)) return "IBM Motion JPEG including Huffman Tables";
	if (!_strnicmp(fourcc, "MMES", 4)) return "Matrox MPEG-2 I-frame";
	if (!_strnicmp(fourcc, "MP2A", 4)) return "Media Excel MPEG-2 Audio";
	if (!_strnicmp(fourcc, "MP2T", 4)) return "Media Excel MPEG-2 Transport Stream";
	if (!_strnicmp(fourcc, "MP2V", 4)) return "Media Excel MPEG-2 Video";
	if (!_strnicmp(fourcc, "MP41", 4)) return "Microsoft MPEG-4 V1 (enhansed H263)";
	if (!_strnicmp(fourcc, "MP42", 4)) return "Microsoft MPEG-4 V2";
	if (!_strnicmp(fourcc, "MP43", 4)) return "Microsoft MPEG-4 V3";
	if (!_strnicmp(fourcc, "MP4A", 4)) return "Media Excel MPEG-4 Audio";
	if (!_strnicmp(fourcc, "MP4S", 4)) return "Microsoft MPEG-4 (Windows Media 7.0)";
	if (!_strnicmp(fourcc, "MP4T", 4)) return "Media Excel MPEG-4 Transport Stream";
	if (!_strnicmp(fourcc, "MP4V", 4)) return "Microsoft MPEG-4 (hacked MS MPEG-4)";
	if (!_strnicmp(fourcc, "MP4V", 4)) return "Media Excel MPEG-4 Video";
	if (!_strnicmp(fourcc, "MPEG", 4)) return "Chromatic MPEG 1 Video I Frame";
	if (!_strnicmp(fourcc, "MPG1", 4)) return "FFmpeg-1";
	if (!_strnicmp(fourcc, "MPG2", 4)) return "FFmpeg-1";
	if (!_strnicmp(fourcc, "MPG3", 4)) return "Same as Low motion DivX MPEG-4";
	if (!_strnicmp(fourcc, "MPG4", 4)) return "Microsoft MPEG-4 V1";
	if (!_strnicmp(fourcc, "MPGI", 4)) return "Sigma Design MPEG-1 I-frame";
	if (!_strnicmp(fourcc, "MPNG", 4)) return "Motion PNG codec";
	if (!_strnicmp(fourcc, "MRCA", 4)) return "FAST Multimedia MR Codec";
	if (!_strnicmp(fourcc, "MRLE", 4)) return "Microsoft Run Length Encoding";
	if (!_strnicmp(fourcc, "MSS1", 4)) return "Windows Screen Video";
	if (!_strnicmp(fourcc, "MSS2", 4)) return "Windows Media 9";
	if (!_strnicmp(fourcc, "MSVC", 4)) return "Microsoft Video 1";
	if (!_strnicmp(fourcc, "MSZH", 4)) return "Lossless codec (ZIP compression)";
	if (!_strnicmp(fourcc, "MTGA", 4)) return "Motion TGA images (24, 32 bpp)";
	if (!_strnicmp(fourcc, "MTX1", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX2", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX3", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX4", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX5", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX6", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX7", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX8", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MTX9", 4)) return "Matrox Motion-JPEG codec";
	if (!_strnicmp(fourcc, "MWV1", 4)) return "Aware Motion Wavelets";
	if (!_strnicmp(fourcc, "NAVI", 4)) return "nAVI video codec (hacked MS MPEG-4)";
	if (!_strnicmp(fourcc, "NHVU", 4)) return "NVidia Texture Format (GEForce 3)";
	if (!_strnicmp(fourcc, "NT00", 4)) return "NewTek LigtWave HDTV YUV with Alpha-channel";
	if (!_strnicmp(fourcc, "NTN1", 4)) return "Nogatech Video Compression 1";
	if (!_strnicmp(fourcc, "NUV1", 4)) return "NuppelVideo";
	if (!_strnicmp(fourcc, "NV12", 4)) return "8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling";
	if (!_strnicmp(fourcc, "NV21", 4)) return "As NV12 with U and V reversed in the interleaved plane";
	if (!_strnicmp(fourcc, "NVDS", 4)) return "nVidia Texture Format";
	if (!_strnicmp(fourcc, "NVHS", 4)) return "NVidia Texture Format (GEForce 3)";
	if (!_strnicmp(fourcc, "NVS0", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVS1", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVS2", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVS3", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVS4", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVS5", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT0", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT1", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT2", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT3", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT4", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "NVT5", 4)) return "nVidia Texture Compression Format";
	if (!_strnicmp(fourcc, "PDVC", 4)) return "Panasonic DV codec";
	if (!_strnicmp(fourcc, "PGVV", 4)) return "Radius Video Vision Telecast (adaptive JPEG)";
	if (!_strnicmp(fourcc, "PHMO", 4)) return "IBM Photomotion";
	if (!_strnicmp(fourcc, "PNG ", 4)) return "CorePNG Lossless Video Codec";
	if (!_strnicmp(fourcc, "PNG1", 4)) return "CorePNG Lossless Video Codec";
	if (!_strnicmp(fourcc, "PIM1", 4)) return "Pegasus Imaging MPEG-1 codec";
	if (!_strnicmp(fourcc, "PIM2", 4)) return "Pegasus Imaging codec";
	if (!_strnicmp(fourcc, "PIMJ", 4)) return "Pegasus Imaging PICvideo Lossless JPEG";
	if (!_strnicmp(fourcc, "PIXL", 4)) return "MiroVideo XL (Motion JPEG)";
	if (!_strnicmp(fourcc, "PVEZ", 4)) return "Horizons Technology PowerEZ codec";
	if (!_strnicmp(fourcc, "PVMM", 4)) return "PacketVideo Corporation MPEG-4";
	if (!_strnicmp(fourcc, "PVW2", 4)) return "Pegasus Imaging Wavelet 2000";
	if (!_strnicmp(fourcc, "Q1.0", 4)) return "Q-Team QPEG 1.0";
	if (!_strnicmp(fourcc, "QPEG", 4)) return "Q-Team QPEG 1.1";
	if (!_strnicmp(fourcc, "QPEQ", 4)) return "Q-Team QPEG 1.1";
	if (!_strnicmp(fourcc, "RAW", 4)) return "Full Frames (Uncompressed)";
	if (!_strnicmp(fourcc, "RGB", 4)) return "Full Frames (Uncompressed)";
	if (!_strnicmp(fourcc, "RGB(15)", 4)) return "Uncompressed RGB15 5:5:5";
	if (!_strnicmp(fourcc, "RGB(16)", 4)) return "Uncompressed RGB16 5:6:5";
	if (!_strnicmp(fourcc, "RGB(24)", 4)) return "Uncompressed RGB24 8:8:8";
	if (!_strnicmp(fourcc, "RGB1", 4)) return "Uncompressed RGB332 3:3:2";
	if (!_strnicmp(fourcc, "RGBA", 4)) return "Raw RGB with alpha";
	if (!_strnicmp(fourcc, "RGBO", 4)) return "Uncompressed RGB555 5:5:5";
	if (!_strnicmp(fourcc, "RGBP", 4)) return "Uncompressed RGB565 5:6:5";
	if (!_strnicmp(fourcc, "RGBQ", 4)) return "Uncompressed RGB555X 5:5:5 BE";
	if (!_strnicmp(fourcc, "RGBR", 4)) return "Uncompressed RGB565X 5:6:5 BE";
	if (!_strnicmp(fourcc, "RGBT", 4)) return "Uncompressed RGB with transparency";
	if (!_strnicmp(fourcc, "RLE4", 4)) return "Run length encoded 4bpp RGB image";
	if (!_strnicmp(fourcc, "RLE8", 4)) return "Run length encoded 8bpp RGB image";
	if (!_strnicmp(fourcc, "RLE", 4)) return "Raw RGB with arbitrary sample packing within a pixel";
	if (!_strnicmp(fourcc, "RMP4", 4)) return "REALmagic MPEG-4 Video Codec (Sigma Design, built on XviD)";
	if (!_strnicmp(fourcc, "ROQV", 4)) return "Id RoQ File Video Decoder";
	if (!_strnicmp(fourcc, "RPZA", 4)) return "Apple Video 16 bit (road pizza)";
	if (!_strnicmp(fourcc, "RT21", 4)) return "Intel Real Time Video 2.1";
	if (!_strnicmp(fourcc, "RTV0", 4)) return "NewTek VideoToaster (uncompressed)";
	if (!_strnicmp(fourcc, "RUD0", 4)) return "Rududu video codec";
	if (!_strnicmp(fourcc, "RV10", 4)) return "RealVideo codec";
	if (!_strnicmp(fourcc, "RV13", 4)) return "RealVideo codec";
	if (!_strnicmp(fourcc, "RV20", 4)) return "RealVideo G2";
	if (!_strnicmp(fourcc, "RV30", 4)) return "RealVideo 8";
	if (!_strnicmp(fourcc, "RV40", 4)) return "RealVideo 9";
	if (!_strnicmp(fourcc, "RVX", 4)) return "Intel RDX";
	if (!_strnicmp(fourcc, "S422", 4)) return "VideoCap C210 YUV Codec";
	if (!_strnicmp(fourcc, "SAN3", 4)) return "MPEG-4 codec (direct copy of DivX 3.11a)";
	if (!_strnicmp(fourcc, "SDCC", 4)) return "Sun Digital Camera Codec";
	if (!_strnicmp(fourcc, "SFMC", 4)) return "Crystal Net SFM (Surface Fitting Method) Codec";
	if (!_strnicmp(fourcc, "SMC", 4)) return "Apple Graphics (SMC) codec (256 color)";
	if (!_strnicmp(fourcc, "SMSC", 4)) return "Radius proprietary codec";
	if (!_strnicmp(fourcc, "SMSD", 4)) return "Radius proprietary codec";
	if (!_strnicmp(fourcc, "SMSV", 4)) return "WorldConnect Wavelet Streaming Video";
	if (!_strnicmp(fourcc, "SP44", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP53", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP54", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP55", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP56", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP57", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SP58", 4)) return "SunPlus Aiptek MegaCam Codec";
	if (!_strnicmp(fourcc, "SPIG", 4)) return "Radius Spigot";
	if (!_strnicmp(fourcc, "SPLC", 4)) return "Splash Studios ACM Audio Codec";
	if (!_strnicmp(fourcc, "SQZ2", 4)) return "Microsoft VXTreme Video Codec V2";
	if (!_strnicmp(fourcc, "STVA", 4)) return "ST CMOS Imager Data (Bayer)";
	if (!_strnicmp(fourcc, "STVB", 4)) return "ST CMOS Imager Data (Nudged Bayer)";
	if (!_strnicmp(fourcc, "STVC", 4)) return "ST CMOS Imager Data (Bunched)";
	if (!_strnicmp(fourcc, "STVX", 4)) return "ST CMOS Imager Data (Extended)";
	if (!_strnicmp(fourcc, "STVY", 4)) return "ST CMOS Imager Data (Extended with Correction Data)";
	if (!_strnicmp(fourcc, "SV10", 4)) return "Sorenson Media Video R1";
	if (!_strnicmp(fourcc, "SVQ1", 4)) return "Sorenson Video (Apple Quicktime 3)";
	if (!_strnicmp(fourcc, "SVQ3", 4)) return "Sorenson Video 3 (Apple Quicktime 5)";
	if (!_strnicmp(fourcc, "SWC1", 4)) return "MainConcept Motion JPEG Codec";
	if (!_strnicmp(fourcc, "T420", 4)) return "Toshiba YUV 4:2:0";
	if (!_strnicmp(fourcc, "TIM2", 4)) return "Pinnacle RAL DVI";
	if (!_strnicmp(fourcc, "TLMS", 4)) return "TeraLogic Motion Infraframe Codec A";
	if (!_strnicmp(fourcc, "TLST", 4)) return "TeraLogic Motion Infraframe Codec B";
	if (!_strnicmp(fourcc, "TM20", 4)) return "Duck TrueMotion 2.0";
	if (!_strnicmp(fourcc, "TM2A", 4)) return "Duck TrueMotion Archiver 2.0";
	if (!_strnicmp(fourcc, "TM2X", 4)) return "Duck TrueMotion 2X";
	if (!_strnicmp(fourcc, "TMIC", 4)) return "TeraLogic Motion Intraframe Codec 2";
	if (!_strnicmp(fourcc, "TMOT", 4)) return "TrueMotion Video Compression";
	if (!_strnicmp(fourcc, "TR20", 4)) return "Duck TrueMotion RT 2.0";
	if (!_strnicmp(fourcc, "TSCC", 4)) return "TechSmith Screen Capture Codec";
	if (!_strnicmp(fourcc, "TV10", 4)) return "Tecomac Low-Bit Rate Codec";
	if (!_strnicmp(fourcc, "TVJP", 4)) return "TrueVision Field Encoded Motion JPEG (Targa emulation)";
	if (!_strnicmp(fourcc, "TVMJ", 4)) return "TrueVision Field Encoded Motion JPEG (Targa emulation)";
	if (!_strnicmp(fourcc, "TY0N", 4)) return "Trident Decompression Driver";
	if (!_strnicmp(fourcc, "TY2C", 4)) return "Trident Decompression Driver";
	if (!_strnicmp(fourcc, "TY2N", 4)) return "Trident Decompression Driver";
	if (!_strnicmp(fourcc, "U263", 4)) return "UB Video StreamForce H.263";
	if (!_strnicmp(fourcc, "UCOD", 4)) return "ClearVideo (fractal compression-based codec)";
	if (!_strnicmp(fourcc, "ULTI", 4)) return "IBM Corp. Ultimotion";
	if (!_strnicmp(fourcc, "UMP4", 4)) return "UB Video MPEG 4";
	if (!_strnicmp(fourcc, "UYNV", 4)) return "A direct copy of UYVY registered by nVidia to work around problems in some old codecs which did not like hardware that offered more than 2 UYVY surfaces";
	if (!_strnicmp(fourcc, "UYVP", 4)) return "YCbCr 4:2:2 extended precision 10-bits per component in U0Y0V0Y1 order";
	if (!_strnicmp(fourcc, "UYVY", 4)) return "YUV 4:2:2 (Y sample at every pixel, U and V sampled at every second pixel horizontally on each line)";
	if (!_strnicmp(fourcc, "V261", 4)) return "Lucent VX2000S";
	if (!_strnicmp(fourcc, "V422", 4)) return "Vitec Multimedia YUV 4:2:2 as for UYVY, but with different component ordering within the u_int32 macropixel";
	if (!_strnicmp(fourcc, "V655", 4)) return "Vitec Multimedia 16 bit YUV 4:2:2 (6:5:5) format";
	if (!_strnicmp(fourcc, "VCR1", 4)) return "ATI VCR 1.0";
	if (!_strnicmp(fourcc, "VCR2", 4)) return "ATI VCR 2.0 (MPEG YV12)";
	if (!_strnicmp(fourcc, "VCR3", 4)) return "ATI VCR 3.0";
	if (!_strnicmp(fourcc, "VCR4", 4)) return "ATI VCR 4.0";
	if (!_strnicmp(fourcc, "VCR5", 4)) return "ATI VCR 5.0";
	if (!_strnicmp(fourcc, "VCR6", 4)) return "ATI VCR 6.0";
	if (!_strnicmp(fourcc, "VCR7", 4)) return "ATI VCR 7.0";
	if (!_strnicmp(fourcc, "VCR8", 4)) return "ATI VCR 8.0";
	if (!_strnicmp(fourcc, "VCR9", 4)) return "ATI VCR 9.0";
	if (!_strnicmp(fourcc, "VDCT", 4)) return "Video Maker Pro DIB";
	if (!_strnicmp(fourcc, "VDOM", 4)) return "VDOnet VDOWave";
	if (!_strnicmp(fourcc, "VDOW", 4)) return "VDOLive (H.263)";
	if (!_strnicmp(fourcc, "VDST", 4)) return "VirtualDub remote frameclient ICM driver";
	if (!_strnicmp(fourcc, "VDTZ", 4)) return "Darim Vision VideoTizer YUV";
	if (!_strnicmp(fourcc, "VGPX", 4)) return "Alaris VideoGramPixel Codec";
	if (!_strnicmp(fourcc, "VIDS", 4)) return "Vitec Multimedia YUV 4:2:2 codec";
	if (!_strnicmp(fourcc, "VIFP", 4)) return "Virtual Frame API codec (VFAPI)";
	if (!_strnicmp(fourcc, "VIV1", 4)) return "Vivo H.263";
	if (!_strnicmp(fourcc, "VIV2", 4)) return "Vivo H.263";
	if (!_strnicmp(fourcc, "VIVO", 4)) return "Vivo H.263";
	if (!_strnicmp(fourcc, "VIXL", 4)) return "MiroVideo XL (Motion JPEG)";
	if (!_strnicmp(fourcc, "VLV1", 4)) return "VideoLogic codec";
	if (!_strnicmp(fourcc, "VP30", 4)) return "On2 (ex Duck TrueMotion) VP3";
	if (!_strnicmp(fourcc, "VP31", 4)) return "On2 (ex Duck TrueMotion) VP3";
	if (!_strnicmp(fourcc, "VSSV", 4)) return "Vanguard Software Solutions Video Codec";
	if (!_strnicmp(fourcc, "VTLP", 4)) return "Alaris VideoGramPixel Codec";
	if (!_strnicmp(fourcc, "VX1K", 4)) return "Lucent VX1000S Video Codec";
	if (!_strnicmp(fourcc, "VX2K", 4)) return "Lucent VX2000S Video Codec";
	if (!_strnicmp(fourcc, "VXSP", 4)) return "Lucent VX1000SP Video Codec";
	if (!_strnicmp(fourcc, "VYU9", 4)) return "ATI Technologies YUV";
	if (!_strnicmp(fourcc, "VYUY", 4)) return "ATI Packed YUV Data";
	if (!_strnicmp(fourcc, "WARP", 4)) return "WARP Video Codec";
	if (!_strnicmp(fourcc, "WBVC", 4)) return "Winbond W9960 codec";
	if (!_strnicmp(fourcc, "WHAM", 4)) return "Microsoft Video 1";
	if (!_strnicmp(fourcc, "WINX", 4)) return "Winnov Software Compression";
	if (!_strnicmp(fourcc, "WJPG", 4)) return "Winbond JPEG (AverMedia USB devices)";
	if (!_strnicmp(fourcc, "WMV1", 4)) return "Windows Media Video V7";
	if (!_strnicmp(fourcc, "WMV2", 4)) return "Windows Media Video V8";
	if (!_strnicmp(fourcc, "WMV3", 4)) return "Windows Media Video V9";
	if (!_strnicmp(fourcc, "WNV1", 4)) return "WinNow Videum Hardware Compression";
	if (!_strnicmp(fourcc, "WNVA", 4)) return "Winnov hw compress";
	if (!_strnicmp(fourcc, "WRPR", 4)) return "VideoTools VideoServer Client Codec (wrapper for AviSynth)";
	if (!_strnicmp(fourcc, "X263", 4)) return "Xirlink H.263";
	if (!_strnicmp(fourcc, "XLV0", 4)) return "NetXL Inc. XL Video Decoder";
	if (!_strnicmp(fourcc, "XMPG", 4)) return "XING MPEG (I frame only)";
	if (!_strnicmp(fourcc, "XVID", 4)) return "XviD MPEG-4 codec";
	if (!_strnicmp(fourcc, "XVIX", 4)) return "Based on XviD MPEG-4 codec";
	if (!_strnicmp(fourcc, "XWV0", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV1", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV2", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV3", 4)) return "XiWave Video Codec (Xi-3 Video)";
	if (!_strnicmp(fourcc, "XWV4", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV5", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV6", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV7", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV8", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XWV9", 4)) return "XiWave Video Codec";
	if (!_strnicmp(fourcc, "XXAN", 4)) return "Origin Video Codec (used in Wing Commander 3 and 4)";
	if (!_strnicmp(fourcc, "Y211", 4)) return "Packed YUV format with Y sampled at every second pixel across each line and U and V sampled at every fourth pixel";
	if (!_strnicmp(fourcc, "Y411", 4)) return "YUV 4:1:1 Packed";
	if (!_strnicmp(fourcc, "Y41B", 4)) return "YUV 4:1:1 Planar";
	if (!_strnicmp(fourcc, "Y41P", 4)) return "Conexant (ex Brooktree) YUV 4:1:1 Raw";
	if (!_strnicmp(fourcc, "Y41T", 4)) return "Format as for Y41P, but the lsb of each Y component is used to signal pixel transparency";
	if (!_strnicmp(fourcc, "Y422", 4)) return "Direct copy of UYVY as used by ADS Technologies Pyro WebCam firewire camera";
	if (!_strnicmp(fourcc, "Y42B", 4)) return "YUV 4:2:2 Planar";
	if (!_strnicmp(fourcc, "Y42T", 4)) return "Format as for UYVY, but the lsb of each Y component is used to signal pixel transparency";
	if (!_strnicmp(fourcc, "Y800", 4)) return "Simple grayscale video";
	if (!_strnicmp(fourcc, "Y8", 4)) return "Simple grayscale video";
	if (!_strnicmp(fourcc, "YC12", 4)) return "Intel YUV12 Codec";
	if (!_strnicmp(fourcc, "YUNV", 4)) return "A direct copy of YUY2 registered by nVidia to work around problems in some old codecs which did not like hardware that offered more than 2 YUY2 surfaces";
	if (!_strnicmp(fourcc, "YUV8", 4)) return "Winnov Caviar YUV8";
	if (!_strnicmp(fourcc, "YUV9", 4)) return "Intel YUV9";
	if (!_strnicmp(fourcc, "YUVP", 4)) return "YCbCr 4:2:2 extended precision 10-bits per component in Y0U0Y1V0 order";
	if (!_strnicmp(fourcc, "YUY2", 4)) return "YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel";
	if (!_strnicmp(fourcc, "YUYV", 4)) return "Canopus YUV format";
	if (!_strnicmp(fourcc, "YV12", 4)) return "ATI YVU12 4:2:0 Planar";
	if (!_strnicmp(fourcc, "YVU9", 4)) return "Brooktree YVU9 Raw (YVU9 Planar)";
	if (!_strnicmp(fourcc, "YVYU", 4)) return "YUV 4:2:2 as for UYVY but with different component ordering within the u_int32 macropixel";
	if (!_strnicmp(fourcc, "ZLIB", 4)) return "Lossless codec (ZIP compression)";
	if (!_strnicmp(fourcc, "ZPEG", 4)) return "Metheus Video Zipper";
	if (!_strnicmp(fourcc, "ZYGO", 4)) return "ZyGo Video Codec";
	return fourcc;
};

MatroskaAudioTrackInfo::MatroskaAudioTrackInfo()
{
	ODS("MatroskaAudioTrackInfo::MatroskaAudioTrackInfo()");
	audio_SampleRate = 0;
	audio_OutputSampleRate = 0;
	audio_Channels = 1;
	audio_BitDepth = 0;
};

MatroskaAudioTrackInfo::~MatroskaAudioTrackInfo()
{
	TRACE("MatroskaAudioTrackInfo::~MatroskaAudioTrackInfo()");
};

/// Wav Format Tag list from http://kibus1.narod.ru/frames_eng.htm?sof/abcavi/twocc.htm
const char *MatroskaAudioTrackInfo::GetWaveFormatName(uint16 formatTag){
	switch (formatTag){
		case 0x0000: return "Microsoft Unknown Wave Format";
		case 0x0001: return "Microsoft PCM Format";
		case 0x0002: return "Microsoft ADPCM Format";
		case 0x0003: return "IEEE Float";
		case 0x0004: return "Compaq Computer VSELP (codec for Windows CE 2.0 devices)";
		case 0x0005: return "IBM CVSD";
		case 0x0006: return "Microsoft ALAW (CCITT A-Law)";
		case 0x0007: return "Microsoft MULAW (CCITT u-Law)";
		case 0x0008: return "Microsoft DTS";
		case 0x000A: return "Windows Media Audio 3 Voice Mono";
		case 0x0010: return "OKI ADPCM";
		case 0x0011: return "Intel DVI ADPCM (IMA ADPCM)";
		case 0x0012: return "Videologic MediaSpace ADPCM";
		case 0x0013: return "Sierra Semiconductor ADPCM";
		case 0x0014: return "Antex Electronics G.723 ADPCM";
		case 0x0015: return "DSP Solutions DIGISTD";
		case 0x0016: return "DSP Solutions DIGIFIX";
		case 0x0017: return "Dialogic OKI ADPCM";
		case 0x0018: return "MediaVision ADPCM";
		case 0x0019: return "HP CU codec";
		case 0x0020: return "Yamaha ADPCM";
		case 0x0021: return "Speech Compression SONARC";
		case 0x0022: return "DSP Group True Speech";
		case 0x0023: return "Echo Speech EchoSC1";
		case 0x0024: return "Audiofile AF36";
		case 0x0025: return "APTX";
		case 0x0026: return "AudioFile AF10";
		case 0x0027: return "Prosody 1612 codec for CTI Speech Card";
		case 0x0028: return "Merging Technologies S.A. LRC";
		case 0x0030: return "Dolby Labs AC2";
		case 0x0031: return "Microsoft GSM 6.10";
		case 0x0032: return "MSNAudio";
		case 0x0033: return "Antex Electronics ADPCME";
		case 0x0034: return "Control Resources VQLPC";
		case 0x0035: return "DSP Solutions Digireal";
		case 0x0036: return "DSP Solutions DigiADPCM";
		case 0x0037: return "Control Resources Ltd CR10";
		case 0x0038: return "Natural MicroSystems VBXADPCM";
		case 0x0039: return "Roland RDAC (Crystal Semiconductor IMA ADPCM)";
		case 0x003A: return "Echo Speech EchoSC3";
		case 0x003B: return "Rockwell ADPCM";
		case 0x003C: return "Rockwell Digit LK";
		case 0x003D: return "Xebec Multimedia Solutions";
		case 0x0040: return "Antex Electronics G.721 ADPCM";
		case 0x0041: return "Antex Electronics G.728 CELP";
		case 0x0042: return "Microsoft MSG723";
		case 0x0043: return "IBM AVC ADPCM";
		case 0x0050: return "Microsoft MPEG-1 layer 1, 2";
		case 0x0052: return "InSoft RT24 (ACM codec is an alternative codec)";
		case 0x0053: return "InSoft PAC";
		case 0x0055: return "MPEG-1 Layer 3 (MP3)";
		case 0x0059: return "Lucent G.723";
		case 0x0060: return "Cirrus Logic";
		case 0x0061: return "ESS Technology ESPCM / Duck DK4 ADPCM";
		case 0x0062: return "Voxware file-mode codec / Duck DK3 ADPCM";
		case 0x0063: return "Canopus Atrac";
		case 0x0064: return "APICOM G.726 ADPCM";
		case 0x0065: return "APICOM G.722 ADPCM";
		case 0x0066: return "Microsoft DSAT";
		case 0x0067: return "Microsoft DSAT Display";
		case 0x0069: return "Voxware Byte Aligned (bitstream-mode codec)";
		case 0x0070: return "Voxware AC8 (Lernout & Hauspie CELP 4.8 kbps)";
		case 0x0071: return "Voxware AC10 (Lernout & Hauspie CBS 8kbps)";
		case 0x0072: return "Voxware AC16 (Lernout & Hauspie CBS 12kbps)";
		case 0x0073: return "Voxware AC20 (Lernout & Hauspie CBS 16kbps)";
		case 0x0074: return "Voxware MetaVoice (file and stream oriented)";
		case 0x0075: return "Voxware MetaSound (file and stream oriented)";
		case 0x0076: return "Voxware RT29HW";
		case 0x0077: return "Voxware VR12";
		case 0x0078: return "Voxware VR18";
		case 0x0079: return "Voxware TQ40";
		case 0x0080: return "Softsound";
		case 0x0081: return "Voxware TQ60";
		case 0x0082: return "Microsoft MSRT24 (ACM codec is an alternative codec)";
		case 0x0083: return "AT&T Labs G.729A";
		case 0x0084: return "Motion Pixels MVI MV12";
		case 0x0085: return "DataFusion Systems G.726";
		case 0x0086: return "DataFusion Systems GSM610";
		case 0x0088: return "Iterated Systems ISIAudio";
		case 0x0089: return "Onlive";
		case 0x0091: return "Siemens Business Communications SBC24";
		case 0x0092: return "Sonic Foundry Dolby AC3 SPDIF";
		case 0x0093: return "MediaSonic G.723";
		case 0x0094: return "Aculab 8KBPS";
		case 0x0097: return "ZyXEL ADPCM";
		case 0x0098: return "Philips LPCBB";
		case 0x0099: return "Studer Professional Audio AG Packed";
		case 0x00A0: return "Malden Electronics PHONYTALK";
		case 0x00ff: return "AAC";
		case 0x0100: return "Rhetorex ADPCM";
		case 0x0101: return "BeCubed Software IRAT";
		case 0x0111: return "Vivo G.723";
		case 0x0112: return "Vivo Siren";
		case 0x0123: return "Digital G.723";
		case 0x0125: return "Sanyo ADPCM";
		case 0x0130: return "Sipro Lab Telecom ACELP.net";
		case 0x0131: return "Sipro Lab Telecom ACELP.4800";
		case 0x0132: return "Sipro Lab Telecom ACELP.8V3";
		case 0x0133: return "Sipro Lab Telecom ACELP.G.729";
		case 0x0134: return "Sipro Lab Telecom ACELP.G.729A";
		case 0x0135: return "Sipro Lab Telecom ACELP.KELVIN";
		case 0x0140: return "Dictaphone G.726 ADPCM";
		case 0x0150: return "Qualcomm PUREVOICE";
		case 0x0151: return "Qualcomm HALFRATE";
		case 0x0155: return "Ring Zero Systems TUBGSM";
		case 0x0160: return "Windows Media Audio 1 / DivX audio (WMA)";
		case 0x0161: return "Windows Media Audio 2 / DivX audio (WMA) / Alex AC3 Audio";
		case 0x0162: return "Windows Media Audio 3";
		case 0x0163: return "Windows Media Audio 3 Lossless";
		case 0x0170: return "Unisys Nap ADPCM";
		case 0x0171: return "Unisys Nap U-law";
		case 0x0172: return "Unisys Nap A-law";
		case 0x0173: return "Unisys Nap 16K";    
		case 0x0200: return "Creative Labs ADPCM";
		case 0x0202: return "Creative Labs FastSpeech8";
		case 0x0203: return "Creative Labs FastSpeech10";
		case 0x0210: return "UHER informatic GmbH ADPCM";
		case 0x0216: return "Ulead DV ACM";
		case 0x0220: return "Quarterdeck";
		case 0x0230: return "I-link Worldwide ILINK VC";
		case 0x0240: return "Aureal Semiconductor RAW SPORT";
		case 0x0250: return "Interactive Products HSX";
		case 0x0251: return "Interactive Products RPELP";
		case 0x0260: return "Consistent Software CS2";
		case 0x0270: return "Sony ATRAC3 (SCX, same as MiniDisk LP2)";
		case 0x0300: return "Fujitsu TOWNS SND";
		case 0x0400: return "BTV Digital (Brooktree digital audio format)";
		case 0x0401: return "Intel Music Coder (IMC)";
		case 0x0450: return "QDesign Music";
		case 0x0680: return "AT&T Labs VME VMPCM";
		case 0x0681: return "AT&T Labs TPC";
		case 0x1000: return "Olivetti GSM";
		case 0x1001: return "Olivetti ADPCM";
		case 0x1002: return "Olivetti CELP";
		case 0x1003: return "Olivetti SBC";
		case 0x1004: return "Olivetti OPR";
		case 0x1100: return "Lernout & Hauspie codec";
		case 0x1101: return "Lernout & Hauspie CELP codec";
		case 0x1102: return "Lernout & Hauspie SBC codec";
		case 0x1103: return "Lernout & Hauspie SBC codec";
		case 0x1104: return "Lernout & Hauspie SBC codec";
		case 0x1400: return "Norris Communication";
		case 0x1401: return "ISIAudio";
		case 0x1500: return "AT&T Labs Soundspace Music Compression";
		case 0x181C: return "VoxWare RT24 speech codec";
		case 0x1FC4: return "NTC ALF2CD ACM";
		case 0x2000: return "FAST Multimedia AG DVM (Dolby AC3)";
		case 0x674f: return "Ogg Vorbis (mode 1)";
		case 0x676f: return "Ogg Vorbis (mode 1+)";
		case 0x6750: return "Ogg Vorbis (mode 2)";
		case 0x6770: return "Ogg Vorbis (mode 2+)";
		case 0x6751: return "Ogg Vorbis (mode 3)";
		case 0x6771: return "Ogg Vorbis (mode 3+)";
		case 0xFFFE: return "Extensible wave format (or Ogg Vorbis)";
		case 0xFFFF: return "In Development / Unregistered";
	}
	return NULL;
};
/*******************************************
 *    Begin MatroskaInfoList class.        *
 *******************************************/

MatroskaTrackInfoList::MatroskaTrackInfoList() {
	TRACE("MatroskaTrackInfoList::MatroskaTrackInfoList()");
	//first_item = NULL;
};

MatroskaTrackInfoList::~MatroskaTrackInfoList() {
	TRACE("MatroskaTrackInfoList::~MatroskaTrackInfoList()");
	//I should free memory here	
	for (uint32 i = 0; i < size(); i++)
		delete (MatroskaTrackInfo *)at(i);	
};

void MatroskaTrackInfoList::AddItem(MatroskaTrackInfo *new_item)
{
	//Check that some idiot didn't pass a NULL pointer
	if (new_item == NULL) {
		//So some idiot tried to pass a NULL pointer :P, That idot couldn't be me ^_^
		ODS("MatroskaTrackInfoList: So some idiot tried to pass a NULL pointer :P");
		return;
	}	
	//Check if this item is already in the vector	
	for (uint32 i = 0; i < size(); i++) {
		if (at(i) == new_item) {
			//Item already in List !!!!!
			ODS("MatroskaTrackInfoList: Item already in List !!!!!");
			return;
		}
	}
	//It passed the checks so lets add it
	push_back(new_item);
};


/*******************************************
 *   Begin MatroskaAttachmentItem class.   *
 *******************************************/
#ifdef USING_ATT
MatroskaAttachmentItem::MatroskaAttachmentItem()
{
	ODS("MatroskaAttachmentItem::MatroskaAttachmentItem()");
	this->next_item = NULL;
	this->buffer = NULL;
	this->attachmentUID = 0;
	this->attachmentMatroskaFileDataStart = 0;
	this->attachmentMatroskaFileDataLength = 0;
#ifdef USING_CXIMAGE
	m_AttachmentImage = NULL;
	bResized = false;
#endif // USING_CXIMAGE
	m_AttachFilename = L"";
	m_AttachFilenameOnly = L"";
	matroska_filename = L"";
};

MatroskaAttachmentItem::~MatroskaAttachmentItem()
{
	ODS("MatroskaAttachmentItem::~MatroskaAttachmentItem()");
#ifdef USING_CXIMAGE
	try {
		if (m_AttachmentImage != NULL) {
			m_AttachmentImage->Destroy();
			delete m_AttachmentImage;
		}
	} catch (...) {
		// The CxImage lib has a bug in the freeing of memory that breaks into the debugger
	};
#endif // USING_CXIMAGE
	
	delete buffer;
};

bool MatroskaAttachmentItem::IsExt(const wchar_t *ext)
{
	if (ext != NULL) {
		if (m_AttachFilename.length() > 4) {
			return !wcsicmp(m_AttachFilename.c_str()+m_AttachFilename.length()-3, ext);
		}
	}
	return false;
};

binary *MatroskaAttachmentItem::GetAttachmentData()
{	
	try {
		if (buffer == NULL)
		{
			//Must be the first time
			buffer = new binary[attachmentMatroskaFileDataLength+3];
			if (buffer == NULL)
			{
				//Failed to alloc memory
				return NULL;
			}
			MatroskaUilsFileReader matroskaFile(GetMatroskaFilename(), MODE_READ);
			matroskaFile.setFilePointer(attachmentMatroskaFileDataStart);
			matroskaFile.read((void *)buffer, attachmentMatroskaFileDataLength);
			matroskaFile.close();
		}
		buffer[attachmentMatroskaFileDataLength] = 0;
		buffer[attachmentMatroskaFileDataLength+1] = 0;
		return buffer;
	} catch (std::exception &ex) {    
		const char *error = ex.what();
		return NULL;
	};
};

uint32 MatroskaAttachmentItem::GetAttachmentSize()
{
	return attachmentMatroskaFileDataLength;
};

const char *MatroskaAttachmentItem::GetNiceSize()
{
	//We format the size differently
	if (attach_size.length() == 0)
	{
#ifdef WIN32
		wchar_t buffer[256];
		StrFormatByteSizeW(attachmentMatroskaFileDataLength, buffer, 255);
		attach_size = buffer;
#else
		if (attachmentMatroskaFileDataLength < 1024) {
			//This is less than one kilobyte so we display it in bytes
			attach_size = JString::Format("%u bytes", attachmentMatroskaFileDataLength).c_str();
		}else if ((attachmentMatroskaFileDataLength > 1024) && (attachmentMatroskaFileDataLength < 1024*1024)) {
			//This is more than one kilobyte and less than one megabyte so we display it in kilobytes
			attach_size = JString::Format("%.1f KB", (float)int64(attachmentMatroskaFileDataLength)/1024).c_str();
		}else if ((attachmentMatroskaFileDataLength > 1024*1024) && (attachmentMatroskaFileDataLength < 1024*1024*1024)) {
			//This is more than one megabyte and less than one gigabyte so we display it in megabytes
			attach_size = JString::Format("%.1f MB", (float)int64(attachmentMatroskaFileDataLength)/1024/1024).c_str();
		}else {
			//This is more than one gigabyte :O
			//so we display it in gigabytes ;)
			attach_size = JString::Format("%.2f GB", (float)int64(attachmentMatroskaFileDataLength)/1024/1024/1024).c_str();
		}
#endif
	}
	return attach_size.GetUTF8().c_str();
};

const wchar_t *MatroskaAttachmentItem::GetMatroskaFilename() {
	/*if (matroska_filename == NULL)
	{
		matroska_filename = new wchar_t[attachmentMatroskaFilename.length()+1];
		wcscpy(matroska_filename, attachmentMatroskaFilename.c_str());
	}*/
	return matroska_filename.c_str();
};

const char *MatroskaAttachmentItem::GetMatroskaFilename_ASCII() {
	return matroska_filename.GetUTF8().c_str();
};

int MatroskaAttachmentItem::SetMatroskaFilename(const wchar_t *new_filename) {
	if (new_filename == NULL)
		return -1;

	matroska_filename = new_filename;

	return 0;
};

int MatroskaAttachmentItem::SetMatroskaFilename(const char *new_filename) {
	if (new_filename == NULL)
		return -1;

	matroska_filename.SetUTF8(new_filename);
	
	return 0;
};

const wchar_t *MatroskaAttachmentItem::GetAttachmentFilenameOnly()
{	
	if (m_AttachFilenameOnly.length() == 0) {
		m_AttachFilenameOnly = m_AttachFilename;
		wchar_t *lastSlash = (wchar_t *)m_AttachFilenameOnly.c_str();		
		
		if (lastSlash != NULL) {
#ifdef WIN32
			PathStripPath(lastSlash);
#else				
			while (wcschr(lastSlash, L'\\') != NULL)
				lastSlash = wcschr(lastSlash, L'\\')+1;
#endif
			// Since I'm messing with the string in place, I need to make a copy before I reassign it
			// Else UTFstring will dealloc it's string and try to assign the pointer
			// I give it, which points to the string it just dealloced
			UTFstring temp = lastSlash;
			m_AttachFilenameOnly = temp;		
		}
	}
	return m_AttachFilenameOnly.c_str();
};

const char *MatroskaAttachmentItem::GetAttachmentFilenameOnly_ASCII()
{
	GetAttachmentFilenameOnly();
	return m_AttachFilenameOnly.GetUTF8().c_str();	
};

int MatroskaAttachmentItem::SetAttachmentFilename(const wchar_t *new_filename) {
	if (new_filename == NULL)
		return -1;

	m_AttachFilename = new_filename;

	return 0;
};

int MatroskaAttachmentItem::SetAttachmentFilename(const char *new_filename) {
	if (new_filename == NULL)
		return -1;

	m_AttachFilename.SetUTF8(new_filename);

	return 0;
};

const wchar_t *MatroskaAttachmentItem::GetAttachmentFilename()
{	
	return m_AttachFilename.c_str();	
};

const char *MatroskaAttachmentItem::GetAttachmentFilename_ASCII()
{
	return m_AttachFilename.GetUTF8().c_str();	
};

bool MatroskaAttachmentItem::IsCover()
{
	if (wcsstr(m_AttachFilename.c_str(), L"cover") != NULL)
		return true;
	
	if ((attachmentDescription.length() > 0) 
		&& (wcsstr(attachmentDescription.c_str(), L"cover") != NULL))
		return true;

	return false;
};

int MatroskaAttachmentItem::SaveTo(JString saveFilename)
{
	try {
		MatroskaUilsFileReader fileMatroskaSource(GetMatroskaFilename(), MODE_READ);
		MatroskaUilsFileReader fileAttachmentOutput(saveFilename, MODE_WRITE);

		//Seek to the correct pos
		fileMatroskaSource.setFilePointer(attachmentMatroskaFileDataStart);
		if(fileMatroskaSource.getFilePointer() != attachmentMatroskaFileDataStart)
			//Seeking was not successful
			return -1;

		binary writeBuffer[256];

		uint32 current_pos = 0;
		uint32 amount_read = 0;
		uint32 amount_to_read = 0;
		while ((current_pos < attachmentMatroskaFileDataLength)) {
			if (current_pos + 255 < attachmentMatroskaFileDataLength)
				amount_to_read = 255;
			else
				amount_to_read = attachmentMatroskaFileDataLength - current_pos;

			amount_read = fileMatroskaSource.read((void *)writeBuffer, amount_to_read);	
			fileAttachmentOutput.write(writeBuffer, amount_read);
			current_pos += amount_read;			
		}
		fileAttachmentOutput.close();
		fileMatroskaSource.close();
	}catch (...) {
		return -1;
	};
	return 0;
}

#ifdef USING_CXIMAGE
uint32 MatroskaAttachmentItem::GetAttachmentImageType()
{
	if (!attachmentMimeType.compare("image/jpeg")
			|| !attachmentMimeType.compare("image/jpg")
			|| IsExt(L"jpg")
			|| IsExt(L"jpeg")) 
	{
		return CXIMAGE_FORMAT_JPG;

	}else if (!attachmentMimeType.compare("image/gif")
						|| IsExt(L"gif")) 
	{
		return CXIMAGE_FORMAT_GIF;

	}else if (!attachmentMimeType.compare("image/tiff")
						|| !attachmentMimeType.compare("image/tif")
						|| IsExt(L"tif")
						|| IsExt(L"tiff")) 
	{
		return CXIMAGE_FORMAT_TIF;

	}else if (!attachmentMimeType.compare("image/png")
						|| IsExt(L"png")) 
	{
		return CXIMAGE_FORMAT_PNG;

	}else if (!attachmentMimeType.compare("image/vnd.wap.wbmp")
						|| IsExt(L"wbmp")) 
	{
		return CXIMAGE_FORMAT_WBMP;

	}else if (!attachmentMimeType.compare("image/jpeg2000")
						|| IsExt(L"jp2")) 
	{
		return CXIMAGE_FORMAT_JP2;

	}else if (IsExt(L"jpc")) {
		return CXIMAGE_FORMAT_JPC;

	}else if (IsExt(L"bmp")) {
		return CXIMAGE_FORMAT_BMP;

	}else if (IsExt(L"pcx")) {
		return CXIMAGE_FORMAT_PCX;

	}else if (IsExt(L"tga")) {
		return CXIMAGE_FORMAT_TGA;

	}else if (IsExt(L"pgx")) {
		return CXIMAGE_FORMAT_PGX;

	}else if (IsExt(L"pnm")) {
		return CXIMAGE_FORMAT_PNM;

	}else if (IsExt(L"ras")) {
		return CXIMAGE_FORMAT_RAS;

	}else if (IsExt(L"wmf")) {
		return CXIMAGE_FORMAT_WMF;
	}

	return CXIMAGE_FORMAT_UNKNOWN;
};

std::string MatroskaAttachmentItem::GetImageInfo() {
	std::string s;	
	if (m_AttachmentImage != NULL) {
		uint32 imageType = m_AttachmentImage->GetType();

		switch (imageType)
		{
			case CXIMAGE_FORMAT_JPG:
				s = "JPEG ";
				break;
			case CXIMAGE_FORMAT_GIF:
				s = "GIF ";
				break;
			case CXIMAGE_FORMAT_TIF:
				s = "TIFF ";
				break;
			case CXIMAGE_FORMAT_PNG:
				s = "PNG ";
				break;
			case CXIMAGE_FORMAT_WBMP:
				s = "WBMP ";
				break;
			case CXIMAGE_FORMAT_JP2:
				s = "JPEG-2000 ";
				break;
			case CXIMAGE_FORMAT_JPC:
				s = "JPC ";
				break;
			case CXIMAGE_FORMAT_BMP:
				s = "BMP ";
				break;
			case CXIMAGE_FORMAT_PCX:
				s = "PCX ";
				break;
			case CXIMAGE_FORMAT_TGA:
				s = "TGA ";
				break;
			case CXIMAGE_FORMAT_PGX:
				s = "PGX ";
				break;
			case CXIMAGE_FORMAT_PNM:
				s = "PNM ";
				break;
			case CXIMAGE_FORMAT_RAS:
				s = "RAS ";
				break;
			case CXIMAGE_FORMAT_WMF:
				s = "WMF ";
				break;
			default:
				break;
		};

		char imageDim[256];
		snprintf(imageDim, 255, "%i x %i %i bpp", m_AttachmentImage->GetWidth(), m_AttachmentImage->GetHeight(), m_AttachmentImage->GetBpp());
		s += imageDim;
	}
	return s;
};

CxImage *MatroskaAttachmentItem::GetCxImage()
{
	uint32 imageType = GetAttachmentImageType();
	if ((m_AttachmentImage == NULL) && (imageType != CXIMAGE_FORMAT_UNKNOWN))
	{
		try {
			m_AttachmentImage = new CxImage(GetAttachmentData(), GetAttachmentSize(), GetAttachmentImageType());
		} catch (...) {
			// Failed for some reason
			return NULL;
		}

		// Since we decoded the image we no longer need the compressed image data in memory
		delete buffer;
		buffer = NULL;
	}
	return m_AttachmentImage;
};
#endif //USING_CXIMAGE

#ifdef WIN32
int MatroskaAttachmentItem::OpenViaShell(HWND hwndDlg)
{
	try {
		char temp_filename[MAX_MATROSKA_PATH] = { 0 };
		long len = 0;
		
		strcpy(temp_filename, m_AttachFilenameOnly.GetUTF8().c_str());
		
		char *ext = strchr(temp_filename, '.');
		if (ext != NULL) {
			len = ext-temp_filename;
			ext[0] = 0;
		}

		const char *temp_dir = _tempnam("C:\\", temp_filename);		
		if ((temp_dir == NULL) || (strlen(temp_dir) == 0))
			throw std::runtime_error(_("Unable to get temporary filename."));

		const char *file_ext = m_AttachFilenameOnly.GetUTF8().c_str() + len;
		if (file_ext[0] != '.')
			file_ext--;

		strncpy(temp_filename, temp_dir, MAX_MATROSKA_PATH);
		strncat(temp_filename, file_ext, MAX_MATROSKA_PATH);

		if (SaveTo(temp_filename) == 0) {
			//Now we can use the data
			int shellResult = (int)ShellExecuteA(hwndDlg, NULL, temp_filename, NULL, "C:\\", SW_SHOWNORMAL);
			if (shellResult < 32) {
				MessageBox(hwndDlg, _W("Failed to open attached file"), _W("Failed to open attached file"), 0);
				return 1;
			}
		}
	} catch (std::exception &ex) {    
		UTFstring errMsg;
		errMsg.SetUTF8(ex.what());

		MessageBox(hwndDlg, errMsg.c_str(), _W("Failed to open attached file"), 0);		
		return -1;
	};
	return 0;
};
#endif

/*******************************************
 *   Begin MatroskaAttachmentsList class   *
 *******************************************/
MatroskaAttachmentsList::MatroskaAttachmentsList() {
	TRACE("Matroska Attachment List Created");
	//this->first_item = NULL;
};

MatroskaAttachmentsList::~MatroskaAttachmentsList() {
	TRACE("Matroska Attachment List Deleted");
	
	//I should free memory here
	for (uint32 i = 0; i < size(); i++)
		delete (MatroskaAttachmentItem *)at(i);	

};
#endif // USING ATT
//}; //end namespace MatroskaUtilsNamespace


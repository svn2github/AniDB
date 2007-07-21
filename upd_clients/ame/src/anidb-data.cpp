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

#include <wx/wx.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

#include "ame-id.h"
#include "ame-db.h"
#include "ame-cache.h"
#include "anidb-base.h"

AdbConReply::AdbConReply(wxString raw){
	//TODO handle broken responses from server; substr dang
	code = atoi(raw.SubString(0,3).ToAscii().data());
	if(code>=600){
		error = wxT("Internal Server Error ")+raw;
		return;
	}
	size_t j;
	switch(code){
		case rLOGIN_ACCEPTED_NEW_VER:			
		case rLOGIN_ACCEPTED:
			j = raw.find(wxT("LOGIN ACCEPTED"));
			data = raw.substr(4, 5);
			message = raw.substr(j);
			break;
		case rFILE:
			message = raw.substr(4, 4);
			data = raw.substr(9);
			break;
		case rMYLIST_ENTRY_EDITED:
			message = raw.substr(4, 19);
			data = raw.substr(24);
			break;
		case rFILE_ALREADY_IN_MYLIST:
			message = raw.substr(4, 22);
			data = raw.substr(27);
			break;
		case rMYLIST_ENTRY_ADDED:
			message = raw.substr(4, 18);
			data = raw.substr(23);
			break;
		case rMYLIST:
			message = raw.substr(4, 6);
			data = raw.substr(12);
			break;
		case rACCESS_DENIED:
			error = wxT("Access Denied!");
			break;
		case rCLIENT_BANNED:
			message = raw.substr(4, 13);
			data = wxT("");
			error = wxT("Client Banned! ")+data;
			break;
		case rCLIENT_VERSION_OUTDATED:
			error = wxT("Client Outdated!");
			break;
		case rILLEGAL_INPUT_OR_ACCESS_DENIED:
			error = wxT("Illegal Input or Access Denied");
			break;
		default:
			message = raw.substr(4);
	}
	//if(error!="") 
	//	println(error);
}
bool AdbFile::inYear(wxString s){
	s.Replace(wxT(" "), wxEmptyString);
	int i = s.Find('-');
	long l;
	if(i>0){ //Range
		if(!s.SubString(0, i-1).ToLong(&l)) return false;
		int start = l;
		if(!s.SubString(i+1, s.Length()).ToLong(&l)) return false;
		int end = l;
		if(start>end){
			int t = end;
			end = start;
			start = t;
		}
		return m_anime->yea>=start && m_anime->yea<=end;
	}
	if(s.ToLong(&l))
		return m_anime->yea == l;
	return false;
}
AdbFile* AdbFile::make(wxString s){
	AdbFile* af = new AdbFile();
	if(af->parse(s))
		return af;
	//println("Unexpected Server AdbConReply.");
	return NULL;
}

bool AdbFile::parse(wxString s){
	//Empty strings before non-empty are counted, but not after. adding ! is a hack
	wxStringTokenizer t(s+wxT("!"), wxT("|"));
	if(t.CountTokens()!=33){
		POPI(t.CountTokens());
		return false;
	}

	long l;
	#define PINT(x) if(t.GetNextToken().ToLong(&l)) x=l;
	PINT(m_fid);
	PINT(m_aid);//2
	PINT(m_eid);
	PINT(m_gid);//4
	PINT(m_lid);
	PINT(m_stt);//6
	PINT(m_siz);

	m_ed2 = t.GetNextToken(); 
	m_md5 = t.GetNextToken(); //8
	m_sha = t.GetNextToken();
	m_crc = t.GetNextToken(); //10

	m_dub = t.GetNextToken();//12
	m_sub = t.GetNextToken();
	m_qua = t.GetNextToken();
	m_rip = t.GetNextToken();
	m_aud = t.GetNextToken();
	m_vid = t.GetNextToken();
	m_res = t.GetNextToken();

	PINT(m_len);

	if(m_gid>0){
		m_group = CACHE_GROUP(m_gid);
		m_group->grn = t.GetNextToken();
		m_group->grp = t.GetNextToken();//20
		m_group->src = 2;
	}else{
		t.GetNextToken();
		t.GetNextToken();
	}

	m_episode = CACHE_EPISODE(m_eid);
	m_episode->enr = t.GetNextToken();
	m_episode->epn = t.GetNextToken();//22
	m_episode->epr = t.GetNextToken();
	m_episode->epk = t.GetNextToken();//24
	m_episode->src = 2;

	m_anime = CACHE_ANIME(m_aid);
	PINT(m_anime->eps);
	PINT(m_anime->lep);//26
	if(t.GetNextToken().SubString(0,3).ToLong(&l)) 
		m_anime->yea=l;
	else m_anime->yea=-1;
	m_anime->typ = t.GetNextToken();//28
	m_anime->ann = t.GetNextToken();
	m_anime->kan = t.GetNextToken();//30
	m_anime->eng = t.GetNextToken();
	m_anime->cat = t.GetNextToken();//32
	m_anime->src = 2;

	//remove hacky !
	m_anime->cat = m_anime->cat.SubString(0, m_anime->cat.Length()-2);

	m_def = wxString::Format(wxT("%s - %s - %s - [%s](%s)%s"), m_anime->ann, m_episode->enr, m_episode->epn, m_gid>0?m_group->grp:wxT("RAW"), m_crc.Length()==8?m_crc:wxT("nil"), m_ext);
	return true;
}
wxString AdbFile::toMds(){
	wxString x;
	if(m_crc.Length()!=8) x+=wxT("c");
	if(	m_md5.Length()!=32 ||
		m_sha.Length()!=40) x+=wxT("h");
	if(m_len<1) x+=wxT("l");
	if(m_dub.Find(wxT("unknown"))>=0) x+=wxT("d");
	if(m_sub.Find(wxT("unknown"))>=0) x+=wxT("s");
	if(m_aud.Find(wxT("unknown"))>=0) x+=wxT("a");
	if(m_vid.Find(wxT("unknown"))>=0) x+=wxT("v");
	if(m_res==wxT("0x0")||m_res==wxT("unknown")) x+=wxT("x");
	return x;
}
wxString AdbFile::toString(){
	return m_def;//(*m_data)[wxT("nam")];
}
wxString AdbFile::formatByFID(){
	return wxString::Format(wxT("edit=0&fid=%d"), m_fid);
}
wxString AdbFile::formatByHash(){
	return wxString::Format(wxT("ed2k=%s&size=%d"),m_ed2.Lower(),m_siz);//TODO lowercase
}
wxString AdbFile::urlEd2k(){
	wxString s;
	s.Append(wxT("ed2k://|file|"));
	s.Append(m_def);
	s.Append(SP1("|%d",m_siz));
	s.Append(wxT("|"));
	s.Append(m_ed2);
	s.Append(wxT("|"));
	return s;
}
wxString url0(wxString str, bool non){
	if(non)
		return wxT("http://anidb.info/perl-bin/animedb.pl?")+str+wxT("&nonav=1");
	return wxT("http://anidb.info/perl-bin/animedb.pl?")+str;
}
wxString url1(wxString str, bool non=true){
	return url0(wxT("show=")+str, non);
}
wxString AdbFile::urlAnime(){
	return url1(SP1("anime&aid=%d",m_aid));
}
wxString AdbFile::urlEpisode(){
	return url1(SP2("ep&aid=%d&eid=%d", m_aid, m_eid));
}
wxString AdbFile::urlFile(){
	return url1(SP3("file&aid=%d&eid=%d&fid=%d", m_aid, m_eid, m_fid));
}
wxString AdbFile::urlExport(){
	return url1(SP1("ed2kexport&h=1&aid=%d", m_aid));
}
wxString AdbFile::urlGroup(){
	return url1(SP1("group&gid=%d", m_gid));
}
wxString AdbFile::urlMylistX(){
	if(m_lid<2) return urlMylist();
	return url1(SP4("mylist&do=add&aid=%d&eid=%d&fid=%d&lid=%d", m_aid, m_eid, m_fid, m_lid));
}
wxString AdbFile::urlMylist(){
	if(m_anime!=NULL&&m_anime->ann.Length()>0)
		return url1(SP3("mylist&expand=%d&char=%c#a%d", m_aid, m_anime->ann.at(0), m_aid), false);
	return url1(wxT("mylist"));
}
wxString AdbFile::urlYear(){
	if(m_anime!=NULL)
		return url0(SP1("do.search=%%20Start%%20Search%%20&show=search&noalias=1&search.anime.year=%d", m_anime->yea),false);	
	return url1(wxT("search"));
}
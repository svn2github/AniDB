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
#include <wx/file.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <wx/regex.h>

#include "anidb-base.h"
#include "ame-id.h"
#include "ame-job.h"
#include "ame-rules.h"

#define SCMP(x,y) m.get(wxT(x)).CmpNoCase(y)==0
#define CONT(x,y) m.get(wxT(x)).Lower().Find(y.Lower())>=0

wxString TRUNC = wxT("TRUNCATE<");
wxString SEP=wxT("\\");
#define COL wxT(":")
#define R1 wxT("\1")
#define R2 wxT("\2")

wxString AmeRules::getRuleName(void){
	wxCriticalSectionLocker locker(m_cs);
	return m_sren;
}
wxString AmeRules::getRulePath(void){
	wxCriticalSectionLocker locker(m_cs);
	return m_smov;
}
wxString replaceOldTags(wxString &s){
	s.Replace(wxT("%year"), wxT("%yea"));
	s.Replace(wxT("%type"), wxT("%typ"));
	s.Replace(wxT("%qual"), wxT("%qua"));
	s.Replace(wxT("%ed2k"), wxT("%ed2"));
	s.Replace(wxT("%HashED2K"), wxT("%ED2"));
	return s;
}
void AmeRules::setRuleName(wxString s){
	wxCriticalSectionLocker locker(m_cs);
	m_sren = replaceOldTags(s);
}
void AmeRules::setRulePath(wxString s){
	wxCriticalSectionLocker locker(m_cs);
	m_smov = replaceOldTags(s);
}
bool AmeRules::addReplaceRule(wxString f, wxString t){
	if(f.Length()<1) return false;
	wxCriticalSectionLocker locker(m_cs);
	for(size_t i=0; i<m_rf.GetCount(); i++)
		if(m_rf[i]==f) return false;
	m_rf.Add(f);
	m_rt.Add(t);
	return true;
}
wxString AmeRules::getEncRepl(void){
	wxCriticalSectionLocker locker(m_cs);
	if(m_rf.GetCount()<1) return wxEmptyString;
	wxString s;// = m_rf[0]+R1+m_rt[0];
	for(size_t i=0; i<m_rf.GetCount(); i++){
		s += m_rf[i]+R1;//+m_rt[i]+R1;
		if(m_rt[i].Length()<1)
			s += wxT("\\0");
		else s += m_rt[i];
		s += R1;
	}
	return s;
}
bool AmeRules::getReplaceRule(size_t i, wxString &f, wxString &t){
	wxCriticalSectionLocker locker(m_cs);
	if(i<0||i>=m_rf.GetCount()) return false;
	f = m_rf[i];
	t = m_rt[i];
	return true;
}
bool AmeRules::removeReplaceRule(size_t i){
	wxCriticalSectionLocker locker(m_cs);
	if(i<0||i>=m_rf.GetCount()) return false;
	m_rf.RemoveAt(i);
	return true;
}
size_t AmeRules::getReplaceCount(void){
	wxCriticalSectionLocker locker(m_cs);
	return m_rf.GetCount();
}
wxString truncate(wxString s){
	wxString t;
	int x = s.Find(TRUNC), y, z, i;
	long l;
	while(x>0){
		y = s.Find('>', x);
		if(y<x) break;
		z = s.Find(',', x);
		if(z<x||z>y) break;
		wxString num = s.SubString(x+TRUNC.Length(),z-1);
		if(num.ToLong(&l))
			i = l;
		else{
			POSTM(MSG_DEB|MSG_POP, SP1("! Could not convert to integer: %s", num));
			i = 64;
		}
		t = s.SubString(z+1,y);
		if(x>i+t.length())
			s = s.SubString(0, i-t.length())+t+s.substr(y+1);
		else s = s.SubString(0, x)+s.substr(y+1);
		x = s.Find(TRUNC);
	}
	return s;
}
wxString AmeRules::fin(AmeJob *j, StringMap &m, wxString s){
	wxString x = s;
	x.Replace(SEP, R1);
	x.Replace(COL, R2);

	s = x; x = wxT(""); wxString u; int i;
	do{
		i = s.Find('%');
		if(i<0){
			x.Append(s);
			break;
		}
		x.Append(s.SubString(0,i-1));
		if(i+3>=(int)s.Length()){
			x.Append(s.substr(i));
			break;
		}
		u = s.substr(i+1,3);
		s = s.substr(i+4);
		x.append(m.get(u));
	}while(i>=0);

	x = truncate(x);

	for(size_t i = 0; i<m_rf.GetCount(); i++)
		x.Replace(m_rf[i], m_rt[i]);
	
	x.Replace(R1, SEP);
	x.Replace(R2, COL);

	if( x.at(x.Length()-1)=='_' ){
		x = x.SubString(0, x.Length()-2);
		x.Replace(wxT(" "), wxT("_"));
	}
	return x;
}

wxString AmeRules::apply(AmeJob *j){
	if(j==NULL||j->getAdbFile()==NULL) return wxEmptyString;

	wxString path = j->getFolderPath();
	wxString name = j->getFileName();

	StringMap m;
	j->makeMap(&m);
	
	//if(j.m_fa.anime!=null && (mImod==M_RULES || mImod==M_FALLB)){
		wxString sren = get(j, m, m_sren);
		wxString smov = get(j, m, m_smov);

		if(smov.Length()==0&&sren.Length()==0)
			return wxEmptyString;

		if(smov.Length()>0)
			path = fin(j, m, smov);
		if(sren.Length()>0)
			name = fin(j, m, sren) + j->getFileExt();
		//}
	//else if(mImod==M_ANIDB || mImod==M_FALLB)
	//	name = replace(j.m_fa.def, mVill) + j.getExtension();
	//else return null;

	//POP(wxString::Format(L"'%s' - '%s'", path, name));
	wxString abs = path+wxT("\\")+name;//TODO PLATFORM
	abs.Replace(SEP+SEP, SEP);
	//if(j->getAbsolutePath()==abs)
	//	return false;
	//j->setNewFilePath(abs);
	return abs;
}
wxString AmeRules::get(AmeJob *j, StringMap &m, wxString s){
	wxArrayString v;
	build(j, m, s, v);
	wxString rule, str;
	for ( size_t n = 0; n < v.GetCount(); n++ ){
		str = v[n];
		int i = str.Find('\'')+1;
		int j = str.Find('\'',TRUE)-1;
		if(i>0&&j>=i)
			str = str.SubString(i,j);
		i = str.Find(wxT("//"))-1;
		if(i>0) str = str.SubString(0,i);
		rule.Append(str);
	}
	return rule;
}
void AmeRules::build(AmeJob *j, StringMap &m, wxString s, wxArrayString &sch){
	wxStringTokenizer st(s, wxT("\r\n"));
	wxString tok, tup;
	
	int i; bool prev=true;
	while(st.HasMoreTokens()){
		tok = st.GetNextToken();
		//POP(wxString::Format(L"token: %s", tok));
	
		tup = tok.Upper();
		if(tup.StartsWith(wxT("#"))) continue;
		if(tup.StartsWith(wxT("DO ")) && handle(tok.substr(3), sch)) break;
		else if(tup.StartsWith(wxT("IF "))&&
				(prev=check_a(j, m, tok.SubString(3, (i=tup.Find(wxT(" DO ")))))) &&
				handle(tok.substr(i+4), sch)) break;
		else if(tup.StartsWith(wxT("ELSE IF "))&&!prev &&
					(prev=check_a(j, m, tok.SubString(8, (i=tup.Find(wxT(" DO ")))))) &&
				handle(tok.substr(i+4), sch)) break;
		else if(tup.StartsWith(wxT("ELSE DO "))&&!prev &&
				handle(tok.substr(8), sch)) break;
	}
}
bool AmeRules::handle(wxString s, wxArrayString &sch){
	//POP(wxString::Format(L"handle a: %s", op));
	wxString z = s.Upper();
	if(z.StartsWith(wxT("ADD "))){
		sch.Add(s.substr(4));
		return false;
	}
	if(z.StartsWith(wxT("SET "))){
		sch.Clear();
		sch.Add(s.substr(4));
		return false;
	}
	if(z.StartsWith(wxT("FAIL"))){
		sch.Clear();
		return true;
	}
	if(z.StartsWith(wxT("FINISH")))
		return true;
	if(z.StartsWith(wxT("FINISH "))){
		sch.Add(s.substr(7));
		return true;
	}
	if(z.StartsWith(wxT("RETURN "))){
		sch.clear();
		sch.Add(s.substr(7));
		return true;
	}
	if(z.StartsWith(TRUNC)){
		int x = z.Find('>');
		int y = z.Find(',');
		if(x>0 && y>0 && y<x)
			sch.Add(s.substr(0,x+1));
		else POSTM(MSG_DEB|MSG_POP, SP1("! Invalid rule element: %s", s));
		return false;
	}
	sch.clear();
	return false;
}

bool AmeRules::check_a(AmeJob *j, StringMap &m, wxString s){
	//POP(wxString::Format(L"check a: %s", s));
	
	wxStringTokenizer st(s, wxT(";"));
	if(st.CountTokens()<1)return false;
	bool b = true;
	while(st.HasMoreTokens() && b){
		b &= check_b(j, m, st.GetNextToken().Trim());
	}
	return b;
}
/*
! x=2, y=14, s=G(Anime-MX,V-A) - OK
! x=2, y=38, s=E(^(Complete Movie|Part \d of \d|OVA)$)
! x=2, y=14, s=E(^[STOCP]+.*$)
! x=2, y=10, s=G(!unknown)
! x=2, y=11, s=P(^[Ee]:.*$)
*/
int findEndPar(wxString &s, int start){
	if(start<0) return start;
	int level = 0; wxChar c;
	for (int i = start; i < s.Length(); i++){
		c = s.GetChar(i);
		if(c==wxT('('))
			level++;
		else if(c==wxT(')')){
			if(level==0)
				return i;
			level--;
		}
	}
	return -1;
}
bool AmeRules::check_b(AmeJob *j, StringMap &m, wxString s){
	//POP(wxString::Format(L"check b: %s", s));
	wxChar c = s.Upper().at(0);
	int x = s.Find('(')+1;
	int y = findEndPar(s, x)-1;
	//POSTM(MSG_DEB, SP4("! x=%d, y=%d, s=%s => %s",x,y,s,s.SubString(x, y)));
	if(x<1||y<x){
		POSTM(MSG_DEB, SP1("! Wrong syntax: %s", s));
		return false;
	}

	wxStringTokenizer st(s.SubString(x, y), wxT(","));
	while(st.HasMoreTokens()){
		wxString test = st.GetNextToken().Trim();
		bool not = test.StartsWith(wxT("!"));
		if(not) test = test.substr(1);
		if(not^check_c(j, m, test, c)) return true;		
	}
	return false;
}
bool AmeRules::check_c(AmeJob *j, StringMap &m, wxString s, wxChar c){
	switch(c){
		case 'A': //Name
			return SCMP("aid",s) || SCMP("ann",s) || SCMP("kan",s) || SCMP("eng",s);
		case 'E':{ //Path
			wxRegEx exp(s);
			return exp.Matches(m.get(wxT("enr"))) || exp.Matches(m.get(wxT("epn")));
		}
		case 'C': //Codec
			return SCMP("vid",s) || SCMP("aud",s);
		case 'Q': //Quality
			return SCMP("qua",s);
		case 'R': //ripSource
			return SCMP("src",s);
		case 'T': //Type
			return SCMP("typ",s);
		case 'G': //Group
			//if(m.get(wxT("gid"))==wxT("0"))
			//	return s.CmpNoCase(wxT("unknown")) || s.CmpNoCase(wxT("0"));
			return SCMP("gid",s) || SCMP("grp",s) || SCMP("grn",s);
		case 'Y': //Year
			return j->getAdbFile()->inYear(s);
		case 'D': //Dub lang
			return CONT("dub",s);
		case 'S': //Sub lang
			return CONT("sub",s);
		case 'P':{ //Path
			wxRegEx exp(s);
			return exp.Matches(j->getAbsolutePath());
		}
		case 'N': //Genre
			return CONT("gen",s);
		case 'I': //Is Defined
			return m.get(s).Length()>0;
		case 'U':{
			wxStringTokenizer st(s, wxT(":"));
			if(st.CountTokens()==2){
				wxString a = m.get(st.GetNextToken());
				wxString b = m.get(st.GetNextToken());
				return a.Length()>0 && b.Length()>0 && a!=b;
			}
			POSTM(MSG_LOG, SP1("ERROR: Invalid data in test: O(%s)", s));
			return false;
		}
		default: return false;
	}
}
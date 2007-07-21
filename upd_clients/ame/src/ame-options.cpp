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
#include <wx/wfstream.h>
#include <wx/tokenzr.h>

#include "ame-options.h"

#define POP(x) MessageBox(NULL, x, wxT("POP"), MB_OK)
#define POPT(x) POP(wxT(x))

#define PATH_SEP wxT("\\")

#define VER wxT("001")
#define SEP1 wxT("\1")
#define SEP2 wxT("\2")


AmeOptions::AmeOptions(){
	m_path = wxGetUserHome();
	//m_path.Append(PATH_SEP);
	m_path.Append(wxT("\\.webaom"));
	defaults();
}
bool AmeOptions::onDisk(void){
	return wxFile::Exists(m_path);
}
wxString AmeOptions::path(void){
	return m_path;
}
bool AmeOptions::equals(AmeOptions o){
	for(int i=0; i<oBLEN; i++)
		if(mBa[i]!=o.mBa[i])
			return false;
	for(int i=0; i<oILEN; i++)
		if(mIa[i]!=o.mIa[i])
			return false;
	for(int i=0; i<oSLEN; i++)
		if(mSa[i]==o.mSa[i])
			return false;
	return true;
}
void AmeOptions::save(void){
	wxFFile file(m_path, wxT("w"));
	file.Write(enc(), wxConvUTF8);
}
bool AmeOptions::load(void){
	if(!onDisk()) return false;
	wxFFile file(m_path);
	wxString s;
	file.ReadAll(&s, wxConvUTF8);
	return dec(s);
}
wxString AmeOptions::enc(void){
	wxString s(VER);
	s.Append(SEP2);
	s.Append(encB());
	s.Append(SEP2);
	s.Append(encI());
	s.Append(SEP2);
	s.Append(encS());
	return s;
}
bool AmeOptions::dec(wxString str){
	wxStringTokenizer st(str, SEP2);
	if(st.CountTokens()>0 && st.GetNextToken() == VER){
		decB(st.GetNextToken());
		decI(st.GetNextToken());
		decS(&st);
		return true;
	}
	return false;
}
bool AmeOptions::getB(int id){
	return mBa[id];
}
void AmeOptions::setB(int id, bool b){
	mBa[id] = b;
}
int AmeOptions::getI(int id){
	return mIa[id];
}
void AmeOptions::setI(int id, int i){
	mIa[id] = i;
}
wxString AmeOptions::getS(int id){
	return mSa[id];
}
void AmeOptions::setS(int id, wxString s){
	mSa[id] = s;
}
wxString AmeOptions::encB(){
	wxString s;
	for(int i=0; i<oBLEN; i++)
		s += mBa[i] ? '1':'0';
	return s;
}
void AmeOptions::decB(wxString str){
	for(int i=0; i<oBLEN; i++)
		mBa[i] = (str.at(i)=='1');
}
wxString AmeOptions::encI(){
	wxString s;
	for(int i=0; i<oILEN; i++){
		s.Append(wxString::Format(wxT("%d"),mIa[i]));
		s.Append(SEP1);
	}
	return s;
}
void AmeOptions::decI(wxString str){
	wxStringTokenizer st(str, SEP1);
	long x;
	for(int i=0; i<oILEN; i++){
		if(st.GetNextToken().ToLong(&x))
			mIa[i] = (int)x;
	}
}
wxString AmeOptions::encS(){
	wxString s = wxEmptyString;
	for(int i=0; i<oSLEN; i++){
		s.Append((mSa[i].Length()==0)?wxT("null"):mSa[i]);
		s.Append(SEP2);
	}
	return s;
}
void AmeOptions::decS(wxStringTokenizer *st){
	wxString tmp;
	for(int i=0; i<oSLEN; i++){
		if(st->HasMoreTokens())
			tmp = st->GetNextToken();
		else tmp = wxEmptyString;
		mSa[i] = (tmp==wxT("null"))?wxEmptyString:tmp;
	}
}
void AmeOptions::defaults(void){
	setB(oBUNUSED0, false);
	setB(oBHASHCRC, false);
	setB(oBHASHMD5, false);
	setB(oBHASHSHA, false);
	setB(oBHASHTTH, false);
	setB(oBADDFILE, true);
	setB(oBWATCHED, false);
	setB(oBUNUSED1, false);
	setB(oBNATKEEP, false);
	setB(oBSTORPSW, false);
	setB(oBALOADDB, false);
	setB(oBAUTOLOG, false);
	setB(oBUNUSED2, false);

	setI(oIRPORT, 9000);
	setI(oILPORT, 45678);
	setI(oISTATE, 1); //file state on mylist add
	setI(oITIMEO, 10); //timeout
	setI(oIDELAY, 4); //datagram delay
	setI(oIUSMOD, 1); //rename mode

	setS(oSUSRNAME, wxT(""));
	setS(oSHOSTURL, wxT("api.anidb.info"));
	setS(oSMYDBURL, wxT(""));
	setS(oSHASHDIR, wxT(""));
	setS(oSBROWSER, wxT(""));
	setS(oSEXTENSN, wxT("avi\1mkv\1ogm\1mp4"));
	setS(oSSOURCEF, wxT(""));
	setS(oSSTORAGE, wxT(""));
	setS(oSOTHERIN, wxT(""));
	setS(oSVRLSREN, wxT("DO FAIL //SET '%ann - %enr [%grp][%CRC]_'"));
	setS(oSVRLSMOV, wxT("DO FAIL"));
	setS(oSREPLSYS, wxT("`\1'\1\"\1\1<\1\1>\1\1|\1\1/\1\1:\1\1\\\1\1?\1\1*\1"));
	setS(oSHTMLCOL, wxT(""));
	setS(oSLOGFILE, wxT(""));
	setS(oSPATHREG, wxT(""));
	setS(oSFONTSTR, wxT(""));
	setS(oSLOGHEAD, wxT(""));
}
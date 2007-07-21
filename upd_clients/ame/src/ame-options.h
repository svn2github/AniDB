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

#ifndef __OPTIONS_H
#define __OPTIONS_H

#define oBUNUSED0 0 //
#define oBHASHCRC 1
#define oBHASHMD5 2
#define oBHASHSHA 3
#define oBHASHTTH 4
#define oBADDFILE 5
#define oBWATCHED 6
#define oBUNUSED1 7 //
#define oBNATKEEP 8
#define oBSTORPSW 9
#define oBALOADDB 10
#define oBAUTOLOG 11
#define oBUNUSED2 12 //
#define oBLEN     13

#define oIRPORT 0
#define oILPORT 1
#define oISTATE 2 //file state on mylist add
#define oITIMEO 3 //timeout
#define oIDELAY 4 //datagram delay
#define oIUSMOD 5 //rename mode
#define oILEN   6

#define oSUSRNAME 0
#define oSHOSTURL 1
#define oSMYDBURL 2
#define oSHASHDIR 3
#define oSBROWSER 4
#define oSEXTENSN 5
#define oSSOURCEF 6
#define oSSTORAGE 7
#define oSOTHERIN 8
#define oSVRLSREN 9
#define oSVRLSMOV 10
#define oSREPLSYS 11
#define oSHTMLCOL 12
#define oSLOGFILE 13
#define oSPATHREG 14
#define oSFONTSTR 15
#define oSLOGHEAD 16
#define oSLEN     17

class wxStringTokenizer;

class AmeOptions{
private:
	wxString m_path;

	bool strcmp(wxString a, wxString b);
	wxString encB();
	void decB(wxString str);
	wxString encI();
	void decI(wxString str);
	wxString encS();
	void decS(wxStringTokenizer *st);
public:
	int mIa[oILEN];
	wxString mSa[oSLEN];
	bool mBa[oBLEN];

	AmeOptions();
	bool onDisk(void);
	wxString path(void);
	wxString enc(void);
	bool equals(AmeOptions o);
	void save(void);
	bool load(void);
	bool dec(wxString str);
	bool getB(int id);
	void setB(int id, bool b);
	int getI(int id);
	void setI(int id, int i);
	wxString getS(int id);
	void setS(int id, wxString s);
	void defaults(void);

};
#endif //__OPTIONS_H
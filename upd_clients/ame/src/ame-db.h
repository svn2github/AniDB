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

#ifndef __ADB_H
#define __ADB_H

#define dA 0
#define dE 1
#define dG 2
#define dF 3
#define dJ 4
#define dD 5
#define dV 6
#define dL 7

class wxFrame;
class wxDbConnectInf;
class wxDb;
class wxDbTable;
class DirHash;
class AmeJob;

class AmeDB{
	friend class AmeThreadDB;
private:
	wxCriticalSection m_cs;
	bool mBallj, mBclean, mBpgre;//, mBinitd;
	wxString mSc, mSp, mSu;

	wxFrame *m_frm;
	wxDbConnectInf *m_dbinf;
	wxDb *m_db;
	wxDbTable *m_ta[7]; //A/E/F/G/J/J2
	DirHash *m_hmd;

///
	long m_did, m_aid, m_eid, m_gid, m_fid, m_lid, m_yea, m_eps, m_lep, m_stt, m_siz, m_len, m_sta;
	 
	char m_path[256], m_ann[256], m_kan[256], m_eng[256], m_typ[17], m_cat[512];
	char m_epn[256], m_epk[256], m_epr[256], m_enr[6];
	char m_grp[256], m_grn[256];
	//wxChar m_ed2j[33];
	char m_ed2j[33], m_md5j[33], m_shaj[41], m_tthj[40], m_crcj[9];
	char m_ed2f[33], m_md5f[33], m_shaf[41], m_crcf[9];
	char m_rip[17], m_qua[17], m_aud[256], m_vid[17], m_res[17], m_sub[256], m_dub[256], m_def[256];
	char m_name[256], m_ori[256], m_avx[4096];
///
	bool parseJDBC(wxString);
	wxDbTable* defA(void);
	wxDbTable* defD(wxDbTable *t=NULL, int *i=NULL);
	wxDbTable* defE(wxDbTable *t=NULL, int *i=NULL);
	wxDbTable* defF(wxDbTable *t=NULL, int *i=NULL);
	wxDbTable* defG(void);
	wxDbTable* defJ(wxDbTable *t=NULL, int *i=NULL);
	wxDbTable* defV(void);

	int getDid(wxString, bool failsafe=false);
	void mkJob(AmeJob*);
	
public:
	AmeDB(wxFrame *frm);
	~AmeDB();
	bool connect(wxString jdbc, bool check=true);
	bool ok(void);
	void out(int i, wxString s);
	
	//bool exec(wxString, bool);
	
	void getJobs(void);
	int getJob(AmeJob* j, bool ed);

	bool update(int type, int id, void *b);
	bool get(int type, int id, void *b, bool ext=true);
};

class AmeThreadDB : public wxThread{
private:
	AmeDB* m_db;
public:
	AmeThreadDB(AmeDB* db);
	virtual void *Entry();
	virtual void OnExit();
};

#endif //__ADB_H
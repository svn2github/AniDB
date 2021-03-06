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

#include <wx/db.h>
#include <wx/dbtable.h>
#include <wx/hashset.h>
#include <wx/filename.h>
#include <wx/thread.h>

#include "ame-id.h"
#include "ame-db.h"
#include "ame-job.h"
#include "anidb-base.h"
#include "ame-cache.h"

WX_DECLARE_STRING_HASH_MAP( int, DirHash );

//#define dPLY 6
#define AL() wxCriticalSectionLocker locker(m_cs)
#define TU8(x) wxString(x,wxConvUTF8)
#define SCPY(x,y) strcpy(x, y.mb_str(wxConvUTF8));
#define RESET(x) strcpy(x, "\0");

AmeThreadDB::AmeThreadDB(AmeDB* db):wxThread(){
	m_db = db;
};
void AmeThreadDB::OnExit(){
	wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, EVT_DB_BYE );
	wxPostEvent( m_db->m_frm, evt );
}
/*void *AmeThreadDB::Entry(){

	return NULL;
}*/

wxDbTable* AmeDB::defA(void){	
/*	+----------+------------------+------+-----+-------------------+-------+
	| Field    | Type             | Null | Key | Default           | Extra |
	+----------+------------------+------+-----+-------------------+-------+
	| aid      | int(10) unsigned | NO   | PRI | 0                 |       |
	| time     | timestamp        | YES  |     | CURRENT_TIMESTAMP |       |
	| romaji   | varchar(255)     | NO   |     |                   |       |
	| kanji    | varchar(255)     | YES  |     | NULL              |       |
	| english  | varchar(255)     | YES  |     | NULL              |       |
	| year     | int(4) unsigned  | NO   |     | 0                 |       |
	| episodes | int(10) unsigned | NO   |     | 0                 |       |
	| last_ep  | int(10) unsigned | NO   |     | 0                 |       |
	| type     | varchar(16)      | NO   |     |                   |       |
	| genre    | varchar(512)     | NO   |     |                   |       |
	| img      | int(10) unsigned | NO   |     | 0                 |       |
	+----------+------------------+------+-----+-------------------+-------+	*/
	int i = 0;
	wxDbTable *t = new wxDbTable(m_db, wxT("atb"), 9);
	t->SetColDefs(i++, wxT("atb.aid"), DB_DATA_TYPE_INTEGER, &m_aid, SQL_C_LONG, 0, true, false);
	t->SetColDefs(i++, wxT("atb.year"), DB_DATA_TYPE_INTEGER, &m_yea, SQL_C_LONG, 0);
	t->SetColDefs(i++, wxT("atb.episodes"), DB_DATA_TYPE_INTEGER, &m_eps, SQL_C_LONG, 0);
	t->SetColDefs(i++, wxT("atb.last_ep"), DB_DATA_TYPE_INTEGER, &m_lep, SQL_C_LONG, 0);
	t->SetColDefs(i++, wxT("atb.romaji"), DB_DATA_TYPE_VARCHAR, m_ann, SQL_C_CHAR, 255);
	t->SetColDefs(i++, wxT("atb.kanji"), DB_DATA_TYPE_VARCHAR, m_kan, SQL_C_CHAR, 255);
	t->SetColDefs(i++, wxT("atb.english"), DB_DATA_TYPE_VARCHAR, m_eng, SQL_C_CHAR, 255);
	t->SetColDefs(i++, wxT("atb.type"), DB_DATA_TYPE_VARCHAR, m_typ, SQL_C_CHAR, 255);
	t->SetColDefs(i++, wxT("atb.genre"), DB_DATA_TYPE_VARCHAR, m_cat, SQL_C_CHAR, 255);
	return t;
}
wxDbTable* AmeDB::defD(wxDbTable *t, int *i){
/*	+-------+------------------+------+-----+---------+----------------+
	| Field | Type             | Null | Key | Default | Extra          |
	+-------+------------------+------+-----+---------+----------------+
	| did   | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
	| name  | varchar(255)     | NO   | UNI |         |                |
	+-------+------------------+------+-----+---------+----------------+	*/
	int x=0;
	if(i==NULL) i = &x;
	if(t==NULL) t = new wxDbTable(m_db, wxT("dtb"), 2);
	t->SetColDefs((*i)++, wxT("dtb.did"), DB_DATA_TYPE_INTEGER, &m_did, SQL_C_LONG, 0, true, false, false);
	t->SetColDefs((*i)++, wxT("dtb.name"), DB_DATA_TYPE_VARCHAR, m_path, SQL_C_CHAR, 255);
	return t;
}
wxDbTable* AmeDB::defE(wxDbTable *t, int *i){
/*	+---------+------------------+------+-----+-------------------+-------+
	| Field   | Type             | Null | Key | Default           | Extra |
	+---------+------------------+------+-----+-------------------+-------+
	| eid     | int(10) unsigned | NO   | PRI | 0                 |       |
	| english | varchar(255)     | NO   |     |                   |       |
	| kanji   | varchar(255)     | YES  |     | NULL              |       |
	| romaji  | varchar(255)     | YES  |     | NULL              |       |
	| number  | varchar(6)       | NO   |     |                   |       |
	| time    | timestamp        | YES  |     | CURRENT_TIMESTAMP |       |
	+---------+------------------+------+-----+-------------------+-------+	*/
	int x=0;
	if(i==NULL) i = &x;
	if(t==NULL) t = new wxDbTable(m_db, wxT("etb"), 5);
	t->SetColDefs((*i)++, wxT("etb.eid"), DB_DATA_TYPE_INTEGER, &m_eid, SQL_C_LONG, 0, true, false);
	t->SetColDefs((*i)++, wxT("etb.english"),DB_DATA_TYPE_VARCHAR, m_epn, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("etb.kanji"),  DB_DATA_TYPE_VARCHAR, m_epk, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("etb.romaji"), DB_DATA_TYPE_VARCHAR, m_epr, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("etb.number"), DB_DATA_TYPE_VARCHAR, m_enr, SQL_C_CHAR, 16);
	
	return t;
}
wxDbTable* AmeDB::defF(wxDbTable *t, int *i){
/*	+------------+------------------+------+-----+-------------------+-------+
	| Field      | Type             | Null | Key | Default           | Extra |
	+------------+------------------+------+-----+-------------------+-------+
	| fid        | int(10) unsigned | NO   | PRI | 0                 |       |
	| aid        | int(10) unsigned | NO   |     | 0                 |       |
	| eid        | int(10) unsigned | NO   |     | 0                 |       |
	| gid        | int(10) unsigned | NO   |     | 0                 |       |
	| state      | int(10) unsigned | NO   |     | 0                 |       |
	| size       | int(10) unsigned | NO   |     | 0                 |       |
	| ed2k       | varchar(32)      | YES  |     | NULL              |       |
	| md5        | varchar(32)      | YES  |     | NULL              |       |
	| sha1       | varchar(40)      | YES  |     | NULL              |       |
	| crc32      | varchar(8)       | YES  |     | NULL              |       |
	| ripsource  | varchar(16)      | YES  |     | NULL              |       |
	| quality    | varchar(16)      | YES  |     | NULL              |       |
	| audio      | varchar(255)     | YES  |     | NULL              |       |
	| video      | varchar(16)      | YES  |     | NULL              |       |
	| resolution | varchar(16)      | YES  |     | NULL              |       |
	| def_name   | varchar(255)     | NO   |     |                   |       |
	| time       | timestamp        | YES  |     | CURRENT_TIMESTAMP |       |
	| sublang    | varchar(255)     | NO   |     |                   |       |
	| dublang    | varchar(255)     | NO   |     |                   |       |
	+------------+------------------+------+-----+-------------------+-------+	*/
	int x=0;
	if(i==NULL) i = &x;
	if(t==NULL) t = new wxDbTable(m_db, wxT("ftb"), 19);

	t->SetColDefs((*i)++, wxT("ftb.fid"), DB_DATA_TYPE_INTEGER, &m_fid, SQL_C_LONG, 0, true, false);
	t->SetColDefs((*i)++, wxT("ftb.aid"), DB_DATA_TYPE_INTEGER, &m_aid, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.eid"), DB_DATA_TYPE_INTEGER, &m_eid, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.gid"), DB_DATA_TYPE_INTEGER, &m_gid, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.state"),DB_DATA_TYPE_INTEGER, &m_stt, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.size"), DB_DATA_TYPE_INTEGER, &m_siz, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.len"), DB_DATA_TYPE_INTEGER, &m_len, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("ftb.ed2k"), DB_DATA_TYPE_VARCHAR, m_ed2f, SQL_C_CHAR, 32);
	t->SetColDefs((*i)++, wxT("ftb.md5"),  DB_DATA_TYPE_VARCHAR, m_md5f, SQL_C_CHAR, 32);
	t->SetColDefs((*i)++, wxT("ftb.sha1"), DB_DATA_TYPE_VARCHAR, m_shaf, SQL_C_CHAR, 40);
	t->SetColDefs((*i)++, wxT("ftb.crc32"),DB_DATA_TYPE_VARCHAR, m_crcf, SQL_C_CHAR, 8);
	t->SetColDefs((*i)++, wxT("ftb.ripsource"), DB_DATA_TYPE_VARCHAR, m_rip, SQL_C_CHAR, 16);
	t->SetColDefs((*i)++, wxT("ftb.quality"),   DB_DATA_TYPE_VARCHAR, m_qua, SQL_C_CHAR, 16);
	t->SetColDefs((*i)++, wxT("ftb.audio"),     DB_DATA_TYPE_VARCHAR, m_aud, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("ftb.video"),     DB_DATA_TYPE_VARCHAR, m_vid, SQL_C_CHAR, 16);
	t->SetColDefs((*i)++, wxT("ftb.resolution"),DB_DATA_TYPE_VARCHAR, m_res, SQL_C_CHAR, 16);
	t->SetColDefs((*i)++, wxT("ftb.def_name"),  DB_DATA_TYPE_VARCHAR, m_def, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("ftb.sublang"),   DB_DATA_TYPE_VARCHAR, m_sub, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("ftb.dublang"),   DB_DATA_TYPE_VARCHAR, m_dub, SQL_C_CHAR, 255);
	return t;
}
wxDbTable* AmeDB::defG(void){
/*	+-------+------------------+------+-----+-------------------+-------+
	| Field | Type             | Null | Key | Default           | Extra |
	+-------+------------------+------+-----+-------------------+-------+
	| gid   | int(10) unsigned | NO   | PRI | 0                 |       |
	| time  | timestamp        | YES  |     | CURRENT_TIMESTAMP |       |
	| name  | varchar(255)     | NO   |     |                   |       |
	| short | varchar(255)     | NO   |     |                   |       |
	+-------+------------------+------+-----+-------------------+-------+	*/
	int i = 0;
	wxDbTable *t = new wxDbTable(m_db, wxT("gtb"), 3);
	t->SetColDefs(i++, wxT("gtb.gid"), DB_DATA_TYPE_INTEGER, &m_gid, SQL_C_LONG, 0, true, false);
	t->SetColDefs(i++, wxT("gtb.name"),  DB_DATA_TYPE_VARCHAR, m_grn, SQL_C_CHAR, 255);
	t->SetColDefs(i++, wxT("gtb.short"), DB_DATA_TYPE_VARCHAR, m_grp, SQL_C_CHAR, 255);	
	return t;
}
wxDbTable* AmeDB::defJ(wxDbTable *t, int *i){
/*	+--------+------------------+------+-----+-------------------+-------+
	| Field  | Type             | Null | Key | Default           | Extra |
	+--------+------------------+------+-----+-------------------+-------+
	| orig   | varchar(255)     | NO   |     |                   |       |
	| name   | varchar(255)     | NO   | MUL |                   |       |
	| did    | int(10) unsigned | NO   | MUL | 0                 |       |
	| fid    | int(10) unsigned | NO   | MUL | 0                 |       |
	| status | int(10) unsigned | NO   |     | 0                 |       |
	| ed2k   | varchar(32)      | NO   | PRI |                   |       |
	| md5    | varchar(32)      | YES  |     | NULL              |       |
	| sha1   | varchar(40)      | YES  |     | NULL              |       |
	| tth    | varchar(39)      | YES  |     | NULL              |       |
	| crc32  | varchar(8)       | YES  |     |                   |       |
	| size   | int(10) unsigned | NO   | PRI | 0                 |       |
	| uid    | int(10) unsigned | NO   | MUL | 0                 |       |
	| lid    | int(10) unsigned | NO   |     | 0                 |       |
	| time   | timestamp        | YES  |     | CURRENT_TIMESTAMP |       |
	| avxml  | text             | YES  |     | NULL              |       |
	+--------+------------------+------+-----+-------------------+-------+	*/
	int x=0;
	if(i==NULL) i = &x;
	if(t==NULL) t = new wxDbTable(m_db, wxT("jtb"), 13);
	t->SetColDefs((*i)++, wxT("jtb.fid"), DB_DATA_TYPE_INTEGER, &m_fid, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("jtb.did"), DB_DATA_TYPE_INTEGER, &m_did, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("jtb.lid"), DB_DATA_TYPE_INTEGER, &m_lid, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("jtb.status"),DB_DATA_TYPE_INTEGER, &m_sta, SQL_C_LONG, 0);
	t->SetColDefs((*i)++, wxT("jtb.size"),  DB_DATA_TYPE_INTEGER, &m_siz, SQL_C_LONG, 0, true, false);
	t->SetColDefs((*i)++, wxT("jtb.ed2k"), DB_DATA_TYPE_VARCHAR, m_ed2j, SQL_C_CHAR, 32, true, false);
	t->SetColDefs((*i)++, wxT("jtb.md5"),  DB_DATA_TYPE_VARCHAR, m_md5j, SQL_C_CHAR, 32);
	t->SetColDefs((*i)++, wxT("jtb.sha1"), DB_DATA_TYPE_VARCHAR, m_shaj, SQL_C_CHAR, 40);
	t->SetColDefs((*i)++, wxT("jtb.tth"), DB_DATA_TYPE_VARCHAR, m_tthj, SQL_C_CHAR, 39);
	t->SetColDefs((*i)++, wxT("jtb.crc32"),DB_DATA_TYPE_VARCHAR, m_crcj, SQL_C_CHAR, 8);
	t->SetColDefs((*i)++, wxT("jtb.name"), DB_DATA_TYPE_VARCHAR, m_name, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("jtb.orig"), DB_DATA_TYPE_VARCHAR, m_ori, SQL_C_CHAR, 255);
	t->SetColDefs((*i)++, wxT("jtb.avxml"),   DB_DATA_TYPE_VARCHAR, m_avx, SQL_C_CHAR, 4095);
	
	return t;
}
wxDbTable* AmeDB::defV(void){
	int i = 0;

	wxDbTable *t = new wxDbTable(m_db, wxT("jtb"), 13+2+18+5, wxT("jtb JOIN dtb ON jtb.did=dtb.did JOIN ftb ON jtb.fid=ftb.fid JOIN etb ON ftb.eid=etb.eid"), wxDB_QUERY_ONLY);
	
	defJ(t, &i);
	defD(t, &i);
	defE(t, &i);
	defF(t, &i);
	//NOTE some data will be stored in same arrays; ed2k from both jtb and ftb
	
	return t;
}
wxString HandleError(wxString errmsg, wxDb *pDb=NULL)
{
    // Retrieve all the error message for the errors that occurred
    wxString allErrors;
    if (!pDb == NULL)
        // Get the database errors and append them to the error message
        allErrors = wxDbLogExtendedErrorMsg(errmsg.c_str(), pDb, 0, 0);
    else
        allErrors = errmsg;

    // Do whatever you wish with the error message here
    // wxLogDebug() is called inside wxDbLogExtendedErrorMsg() so this
    // console program will show the errors in the console window,
    // but these lines will show the errors in RELEASE builds also
    wxFprintf(stderr, wxT("\n%s\n"), allErrors.c_str());
    fflush(stderr);

    return allErrors;
}
bool AmeDB::ok(void){
	AL();
	return m_db!=NULL;
}
void AmeDB::out(int i, wxString s){
	wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, EVT_MESSAGE );
	evt.SetInt(i);
	evt.SetString(s);
	wxPostEvent(m_frm, evt);
}
AmeDB::AmeDB(wxFrame *frm){
	m_frm = frm;
	m_dbinf = new wxDbConnectInf;
	m_db = NULL;
	m_hmd = new DirHash();
}
AmeDB::~AmeDB(){
	if(m_db){
		for(size_t i=0; i<dL; i++)
			delete m_ta[i];
		wxDbFreeConnection(m_db);
		wxDbCloseConnections();
		m_dbinf->FreeHenv();
	}
	delete m_dbinf;
	delete m_hmd;
}
bool AmeDB::connect(wxString jdbc, bool check){
	AL();
	if(check && !parseJDBC(jdbc))
		return false;
	m_dbinf->SetDsn(mSc);
	m_dbinf->SetUserID(mSu);
	m_dbinf->SetPassword(mSp);
	if(!m_dbinf->AllocHenv()){
		out(MSG_POP|MSG_DEB, wxT("! AllocHenv() failed!"));
		return false;
	}
	
	m_db = wxDbGetConnection(m_dbinf);
	if(!m_db){
		m_dbinf->FreeHenv();
		out(MSG_POP|MSG_DEB, wxT("! wxDbGetConnection() failed!\nCheck info tab."));
		return false;
	}

	//TODO update def
	m_db->SetSqlLogging(sqlLogON, wxT("C:\\aame.log"), false);

	m_ta[dA] = defA();
	m_ta[dD] = defD();
	m_ta[dE] = defE();
	m_ta[dF] = defF();
	m_ta[dG] = defG();
	m_ta[dJ] = defJ();
	m_ta[dV] = defV();

	for(size_t i=0; i<dL; i++)
		m_ta[i]->Open();

	return true;	
}
bool AmeDB::parseJDBC(wxString jdbc){
	if(jdbc.Length()<16) return false;
	if(jdbc.at(0)=='!'){
		jdbc = jdbc.substr(1);
		mBallj = true;
	}else mBallj = false;
	if(jdbc.at(0)=='?'){
		jdbc = jdbc.substr(1);
		mBclean = true;
	}else mBclean = false;

	if(!jdbc.StartsWith(wxT("jdbc:")))
		return false;
	jdbc = jdbc.substr(5);
	if(jdbc.StartsWith(wxT("postgresql://"))){
		jdbc = jdbc.substr(13);
		mBpgre = true;
	}else{
		mBpgre = false;
		if(jdbc.StartsWith(wxT("mysql://")))
			jdbc = jdbc.substr(8);
		else{
			out(MSG_POP|MSG_DEB, wxT("Database type not supported!"));
			return false;
		}
	}
	//host
	int x = jdbc.Find(L'/');
	if(x<0) return false;
	//url = jdbc.SubString(0, x-1);
	jdbc = jdbc.substr(x+1);
	//password
	x = jdbc.Find(wxT("&password="));
	if(x>=0){
		mSp = jdbc.substr(x+10);
		jdbc = jdbc.SubString(0, x-1);
	}
	//user
	x = jdbc.Find(wxT("?user="));
	if(x>=0){
		mSu = jdbc.substr(x+6);
		jdbc = jdbc.SubString(0, x-1);
	}else mSu = wxT("root");
	mSc = jdbc;
	return true;
}
/*bool AmeDB::exec(wxString s, bool q){
	if(!q) out(MSG_DEB, s);
	return m_db->ExecSql((wxChar *)(s.GetData()));
}*/
int AmeDB::getDid(wxString pathx, bool failsafe){
	if(!ok()) return -1;
	wxString path = pathx;
	
	path.Replace(wxT("\\"),wxT("\\\\"));
	path.Replace(wxT("'"),wxT("\\'"));
	if(path.Find('\\')+1==path.Length())
		path = path.SubString(0, path.Length()-2);
	
	DirHash::iterator it = m_hmd->find(path);
	if(it!=m_hmd->end())
		return it->second;

	/*SDWORD cb;
    LONG reqQty;
    wxString sqlStmt;
    sqlStmt = SP1("SELECT did FROM dtb WHERE name='%s'",path);

    // Perform the query
    if (!m_db->ExecSql(sqlStmt.c_str()))
    {
        out(MSG_POP, HandleError(wxT("EXEC ERROR: "), m_ta[dD]->GetDb()));
        return(-1);
    }

    // Request the first row of the result set
    if (m_db->GetNext())
    {
        // Read column #1 of the row returned by the call to ::GetNext()
		// and return the value in 'reqQty'
		if (!m_db->GetData(1, SQL_C_LONG, &reqQty, 0, &cb))
		{
			out(MSG_POP, HandleError(wxT("GET DATA ERROR: "), m_ta[dD]->GetDb()));
			return(-1);
		}

		// Check for a NULL result
		if (cb == SQL_NULL_DATA)
			return(-1);
		return reqQty;
    }
	if(failsafe) return -1;
	sqlStmt = SP1("INSERT INTO dtb (name) VALUES ('%s')",path);
	if (!m_db->ExecSql(sqlStmt.c_str())){
        out(MSG_POP, HandleError(wxT("EXEC ERROR: "), m_ta[dD]->GetDb()));
        return(-1);
    }
	return getDid(pathx, true);*/

	//doesn't work for postgresql
	m_ta[dD]->SetWhereClause(SP1("dtb.name='%s'",path));
	//if(!m_ta[dD]->QueryBySqlStmt(SP1("SELECT d.did FROM dtb AS d WHERE name='%s'", path))){
	if(m_ta[dD]->Query()){
		if(m_ta[dD]->GetNext()){
			(*m_hmd)[path] = m_did;
			out(MSG_POP, SP1("did=%d", m_did));
			return m_did;
		}
	}/*else{
		out(MSG_POP, HandleError(wxT("QUERY ERROR: "), m_ta[dD]->GetDb()));
		return -1;
	}*/
	m_ta[dD]->SetWhereClause(wxEmptyString);
	if(failsafe) return -1;
	SCPY(m_path, pathx);
	//wxStrcpy(m_path, pathx.c_str());
	if (!m_ta[dD]->Insert()){
		out(MSG_POP, HandleError(wxT("INSERTION ERROR: "), m_ta[dD]->GetDb()));
		return -1;
	}
	if(m_db->CommitTrans())
		return getDid(pathx, true);
	return -1;
}
//void AmeDB::getJobs(void){
void *AmeThreadDB::Entry(){
	if(!m_db->ok()) return NULL;
	wxCriticalSectionLocker locker(m_db->m_cs);

	wxDbTable *t = m_db->m_ta[dV];
	
	if(m_db->mBallj)
		t->SetWhereClause(wxEmptyString);
	else t->SetWhereClause(SP1("status!=%d",jFINISHED));
	t->SetOrderByClause(wxT("dtb.name,jtb.name"));
	if(!t->Query()){
		m_db->out(MSG_POP, HandleError(wxT("QUERY ERROR: "), t->GetDb()));
		return NULL;
	}
	long number = 1;
	while(t->GetNext() && !TestDestroy()){
		if( (number%100)==0 ) POSTE(EVT_JOB_LIST_LEN);
		number++;

		AmeJob *j = new AmeJob(TU8(m_db->m_path)+wxT("\\")+TU8(m_db->m_name), m_db->m_sta|jH_PAUSED, m_db->m_sta, m_db->m_siz); //m_sta
		m_db->mkJob(j);
		A.m_jl->addJob(j);
	}
	POSTE(EVT_JOB_LIST_LEN);
	return NULL;
}
wxString rs(wxString s){
	s.Replace(wxT("\'"), wxT("\\\'"));
	return s;
}
int AmeDB::getJob(AmeJob* j, bool ed){
	if(!ok()) return 0;
	AL();
	wxDbTable *t = m_ta[dV];
	if(ed)
		t->SetWhereClause( SP2("jtb.size=%d and jtb.ed2k='%s'", j->getSize(), j->getEd2k()) );
	else{
		int did = getDid(j->getFolderPath());
		t->SetWhereClause( SP2("jtb.name='%s' and jtb.did=%d",rs(j->getFileName()),did) );
	}
	t->SetOrderByClause(wxEmptyString);
	if(!t->Query()){
		out(MSG_POP, HandleError(wxT("QUERY ERROR: "), t->GetDb()));
		return NULL;
	}
	if(t->GetNext()){
		j->setNewFilePath(TU8(m_path)+wxT("\\")+TU8(m_name));
		mkJob(j);
		return m_sta;
	}
	return -1;
}
void AmeDB::mkJob(AmeJob* j){
	j->m_new = false;
	j->m_did = m_did;
	j->setEd2k(TU8(m_ed2j));
	j->m_ori = TU8(m_ori);
	j->m_md5 = TU8(m_md5j);
	j->m_sha = TU8(m_shaj);
	j->m_crc = TU8(m_crcj);
	j->m_tth = TU8(m_tthj);
	j->m_avx = TU8(m_avx);

	if(m_fid>0){
		AdbFile *f = new AdbFile(); //not cached: job 1-1 file
		f->m_aid = m_aid;
		f->m_eid = m_eid;
		f->m_fid = m_fid;
		f->m_gid = m_gid;
		f->m_lid = m_lid;
		f->m_stt = m_stt;
		f->m_len = m_len;

		f->m_ed2 = TU8(m_ed2j);
		f->m_md5 = TU8(m_md5f);
		f->m_sha = TU8(m_shaf);
		f->m_crc = TU8(m_crcf);
		f->m_dub = TU8(m_dub);
		f->m_sub = TU8(m_sub);
		f->m_aud = TU8(m_aud);
		f->m_vid = TU8(m_vid);
		f->m_res = TU8(m_res);
		f->m_rip = TU8(m_rip);
		f->m_qua = TU8(m_qua);
		
		if(m_eid>0){
			AdbEpisode *e = CACHE_EPISODE(m_eid);
			e->enr = TU8(m_enr);
			e->epk = TU8(m_epk);
			e->epn = TU8(m_epn);
			e->epr = TU8(m_epr);
			f->m_episode = e;
		}

		if(m_aid>0){
			AdbAnime *a = CACHE_ANIME(m_aid);
			if(a->src==0){
				get(dA, m_aid, a, false);
				a->src = 1;
			}
			f->m_anime = a;
		}

		if(m_gid>0){
			AdbGroup *g = CACHE_GROUP(m_gid);
			if(g->src==0){
				get(dG, m_gid, g, false);
				g->src = 1;
			}
			f->m_group = g;
		}
		j->setAdbFile(f);
	}
}
bool AmeDB::update(int type, int id, void *b){
	if(!ok() || id<1) return false;
	AL();
	
	wxDbTable *t = m_ta[type];
	wxString wherestr;

	switch(type){
	case dA:{
		m_aid = id;
		//t->SetWhereClause(SP1("aid=%s",m_aid));
		
		AdbAnime *a = (AdbAnime*) b;
		m_eps = a->eps;
		m_lep = a->lep;
		m_yea = a->yea;
		SCPY(m_cat, a->cat);
		SCPY(m_eng, a->eng);
		SCPY(m_kan, a->kan);
		SCPY(m_ann, a->ann);
		SCPY(m_typ, a->typ);
		}break;
	case dE:{
		m_eid = id;
		//t->SetWhereClause(SP1("eid=%s",m_eid));
		AdbEpisode *e = (AdbEpisode*) b;
		SCPY(m_enr, e->enr);
		SCPY(m_epn, e->epn);
		SCPY(m_epk, e->epk);
		SCPY(m_epr, e->epr);
		}break;
	case dF:{
		m_fid = id;
		//t->SetWhereClause(SP1("fid=%s",m_fid));
		AdbFile *f = (AdbFile*) b;
		m_aid = f->m_aid;
		m_eid = f->m_eid;
		m_gid = f->m_gid;
		m_stt = f->m_stt;
		m_siz = f->m_siz;
		m_len = f->m_len;
		SCPY(m_ed2f, f->m_ed2);
		SCPY(m_md5f, f->m_md5);
		SCPY(m_shaf, f->m_sha);
		SCPY(m_crcf, f->m_crc);

		SCPY(m_rip, f->m_rip);
		SCPY(m_qua, f->m_qua);
		SCPY(m_aud, f->m_aud);
		SCPY(m_vid, f->m_vid);
		SCPY(m_res, f->m_res);
		SCPY(m_def, f->m_def);
		SCPY(m_sub, f->m_sub);
		SCPY(m_dub, f->m_dub);

		}break;
	case dG:{
		m_gid = id;
		//t->SetWhereClause(SP1("gid=%s",m_gid));
		AdbGroup *g = (AdbGroup*) b;
		SCPY(m_grp, g->grp);
		SCPY(m_grn, g->grn);
		}break;
	case dJ:{
		AmeJob *j = (AmeJob*) b;
		//wxString from;
		wxString ed2k = j->getEd2k();
		if(ed2k==wxEmptyString) return false;
		wherestr.Printf(wxT("jtb.size=%d AND jtb.ed2k='%s'"), j->getSize(), ed2k);
		//t->SetWhereClause(from);
		AdbFile *f = j->getAdbFile();
		if(f){
			m_fid = f->m_fid;
			m_lid = f->m_lid;
		}else{
			m_fid = 0;
			m_lid = 0;
		}
		if(j->m_did<1)
			j->m_did = getDid(j->getFolderPath());
		m_did = j->m_did;
		m_sta = j->getStatus();
		m_siz = j->getSize();
		SCPY(m_ed2j, j->getEd2k());
		//wxStrcpy(m_ed2j, j->getEd2k());
		SCPY(m_md5j, j->m_md5);
		SCPY(m_shaj, j->m_sha);
		SCPY(m_tthj, j->m_tth);
		SCPY(m_crcj, j->m_crc);
		SCPY(m_tthj, j->m_tth);
		SCPY(m_name, j->getFileName());
		SCPY(m_ori, j->m_ori);
		SCPY(m_avx, j->m_avx);
		}break;
	default:
		return false;
	}
	/*if(!m_ta[type]->Update()){
		out(MSG_POP, HandleError(wxT("UPDATE ERROR: "), m_ta[type]->GetDb()));			
		if(!m_ta[type]->Insert()){
			out(MSG_POP, HandleError(wxT("UPDATE/INSERT ERROR: "), m_ta[type]->GetDb()));
			return false;
		}
	}*/
	//no way to check affected rows!?!??!
	if(wherestr.Length()>0){ //unicode issue with wxodbc/mysql
		//t->SetOrderByClause(wxEmptyString);
		//t->SetFromClause(wxEmptyString);
		if(!t->UpdateWhere(wherestr)){
			out(MSG_POP, HandleError(wxT("UPDATE ERROR: "), t->GetDb()));
			return false;
		}
	}else{
		if(!t->Update()){
			out(MSG_POP, HandleError(wxT("UPDATE ERROR: "), t->GetDb()));
			return false;
		}
	}
	if(!t->Insert()){
		out(MSG_POP, HandleError(wxT("INSERT ERROR: "), t->GetDb()));
		return false;
	}
	//POPI(m_db->CommitTrans());
	if(!m_db->CommitTrans()){
		out(MSG_POP, HandleError(wxT("COMMIT ERROR: "), t->GetDb()));
		return false;
	}
	return true;
}
bool AmeDB::get(int type, int id, void *b, bool ext){
	if(ext){
		if(!ok()) return false;
		AL();
	}
	switch(type){
	case dA:{
		//m_aid = id;
		m_ta[type]->SetWhereClause(SP1("aid=%d",id));
		
		//m_eps;
		//m_lep;
		//m_yea;
		RESET(m_cat);
		RESET(m_eng);
		RESET(m_kan);
		RESET(m_ann);
		RESET(m_typ);
		}break;
	case dE:{
		//m_eid = id;
		m_ta[type]->SetWhereClause(SP1("eid=%d",id));
		AdbEpisode *e = (AdbEpisode*) b;
		RESET(m_enr);
		RESET(m_epk);
		RESET(m_epn);
		RESET(m_epr);
		}break;
	case dF:{
		m_ta[type]->SetWhereClause(SP1("fid=%d",id));
		//m_fid = id;
		AdbFile *f = (AdbFile*) b;
		}break;
	case dG:{
		m_ta[type]->SetWhereClause(SP1("gid=%d",id));
		//m_gid = id;
		AdbGroup *g = (AdbGroup*) b;
		RESET(m_grp);
		RESET(m_grn);
		}break;
	case dJ:{
		AmeJob *j = (AmeJob*) b;
		wxString ed2k = j->getEd2k();
		if(ed2k.Length()==32)
			m_ta[type]->SetWhereClause(SP2("size=%d AND ed2k=%s", j->getSize(), ed2k));
		else{
			int did = getDid(j->getFolderPath());
			m_ta[type]->SetWhereClause(SP3("size=%d AND name=%s AND did=%d", j->getSize(), j->getFileName(), did));
		}
		
		RESET(m_ed2j);
		//wxStrcpy(m_ed2j, wxEmptyString);
		}break;
	default:
		return false;
	}

	m_ta[type]->SetOrderByClause(wxEmptyString);
	m_ta[type]->SetFromClause(wxEmptyString);
	if(!m_ta[type]->Query()){
		out(MSG_POP, HandleError(wxT("SELECT ERROR: "), m_ta[type]->GetDb()));
		return false;
	}
	if(!m_ta[type]->GetNext())
		return false;

	switch(type){
	case dA:{
		m_aid = id;
		
		AdbAnime *a = (AdbAnime*) b;
		a->eps = m_eps;
		a->lep = m_lep;
		a->yea = m_yea;
		a->cat = TU8(m_cat);
		a->eng = TU8(m_eng);
		a->kan = TU8(m_kan);
		a->ann = TU8(m_ann);
		a->typ = TU8(m_typ);
		}break;
	case dE:{
		m_eid = id;
		AdbEpisode *e = (AdbEpisode*) b;
		e->enr = TU8(m_enr);
		e->epk = TU8(m_epk);
		e->epn = TU8(m_epn);
		e->epr = TU8(m_epr);
		}break;
	case dF:{
		m_fid = id;
		AdbFile *f = (AdbFile*) b;
		f->m_aid = m_aid;
		f->m_eid = m_eid;
		f->m_fid = m_fid;
		f->m_gid = m_gid;
		//lid
		}break;
	case dG:{
		m_gid = id;
		AdbGroup *g = (AdbGroup*) b;
		g->grp = TU8(m_grp);
		g->grn = TU8(m_grn);
		}break;
	/*case dJ:{
		AmeJob *j = (AmeJob*) b;
		m_siz = j->getSize();
		SCPY(m_ed2, j->getEd2k());
		}break;*/
	default:
		return false;
	}
	return true;
}
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

#ifndef __ANIDB_H
#define __ANIDB_H

typedef __int64 mylong;

#define rLOGIN_ACCEPTED 200
#define rLOGIN_ACCEPTED_NEW_VER 201
#define rLOGGED_OUT 203
#define rMYLIST_ENTRY_ADDED 210
#define rENTRY_DELETED 211
#define rFILE 220
#define rMYLIST 221
#define rNOTIFICATION_ENABLED 270
#define rPUSHACK_CONFIRMED 280
#define rNOTIFICATION 290
#define rNOTIFYLIST 291
#define rSENDMSG_SUCCESSFUL 294

#define rFILE_ALREADY_IN_MYLIST 310
#define rMYLIST_ENTRY_EDITED 311
#define rNO_SUCH_FILE 320
#define rNO_SUCH_ENTRY 321
#define rNOTIFICATION_DISABLED 370
#define rNO_SUCH_PACKET_PENDING 380
#define rNO_SUCH_USER 394

#define rNOT_LOGGED_IN 403
#define rNO_SUCH_MYLIST_FILE 410
#define rNO_SUCH_MYLIST_ENTRY 411

#define rLOGIN_FAILED 500
#define rLOGIN_FIRST 501
#define rACCESS_DENIED 502
#define rCLIENT_VERSION_OUTDATED 503
#define rCLIENT_BANNED 504
#define rILLEGAL_INPUT_OR_ACCESS_DENIED 505
#define rINVALID_SESSION 506

#define rANIDB_OUT_OF_SERVICE 601

#define FILE_CRCOK	1
#define FILE_CRCERR	2
#define FILE_ISV2	4
#define FILE_ISV3	8
#define FILE_ISV4	16
#define FILE_ISV5	32

#define STATE_UNKNOWN	0
#define STATE_ON_HDD	1
#define STATE_ON_CD		2
#define STATE_DELETED	3
#define STATE_SHARED	4
#define STATE_RELEASE	5

#define UDP_BUF_MAX 1400

class wxIPV4address;
class wxDatagramSocket;

class AdbBase{
public:
	unsigned char src;
};
class AdbAnime : public AdbBase{
public:
	int yea, eps, lep;
	wxString typ, ann, kan, eng, cat;
};
class AdbEpisode : public AdbBase{
public:
	wxString enr, epr, epk, epn;
	wxString toString(){
		return wxString::Format(wxT("%s - %s - %s"), epn, enr, epk);
	}
};
class AdbGroup : public AdbBase{
public:
	wxString grp, grn;
};
class AdbFile{
public:
	AdbFile() : m_anime(NULL),m_episode(NULL),m_group(NULL){}
	~AdbFile(){}
	//long m_siz;
	long m_fid, m_aid, m_eid, m_gid, m_lid, m_stt, m_siz, m_len;//, u_state, viewed, viewdate, edit;
	AdbAnime *m_anime;
	AdbEpisode *m_episode;
	AdbGroup *m_group;
	wxString m_ed2, m_md5, m_sha, m_crc, m_def, m_ext;//, storage, source, other, path, myname;
	wxString m_dub, m_sub, m_qua, m_rip, m_aud, m_vid, m_res;
//	StringHashMap* m_data;
	static AdbFile* make(wxString s);
	bool parse(wxString s);
	wxString toString();
	wxString toMds();
	wxString formatByFID();
	wxString formatByHash();
	wxString urlAnime();
	wxString urlEpisode();
	wxString urlFile();
	wxString urlExport();
	wxString urlGroup();
	wxString urlMylist();
	wxString urlMylistX();
	wxString urlYear();
	wxString urlEd2k();
	bool inYear(wxString s);//TODO
};
class AdbConReply{
public:
	AdbConReply(wxString raw);
	int code;
	wxString message;
	wxString data;
	wxString error;
	wxString toString();
};

class AdbConCallBack{
public:
	virtual void print(int t, wxString s){}
	virtual bool abort(void){return true;}
	virtual bool showLogin(void){return false;}
};

class AdbUserPass{
public:
	AdbUserPass(wxString u, wxString p, wxString a):username(u){
		username=u;
		password=p;
		apipass=a;
		rem_att=3;
	}
	bool ok(void){
		return password.Length()>2&&password.Length()<11&&password.Length()>3;
	}
	wxString username, password, apipass;
	int rem_att;
};

class AdbConOptions{
public:
	unsigned short remote_port, local_port;
	unsigned int timeout;
	time_t delay;
	wxString host;
};

class AdbConnection{
private:
	wxString ver;
	int max_timeouts;
	int remaining_auth_attempts;
	bool loggd, old;
	time_t lastTimeUsed, lastTimeStamp;
	wxString lastError;
	wxString m_session;
	wxStopWatch m_sw;

	AdbUserPass *m_up;
	AdbConOptions *m_opt;
	AdbConCallBack *m_cb;
	
	wxIPV4address *m_remoteaddr;
	wxDatagramSocket *m_udp;
	
	//void error(wxString str);
	//void debug(wxString str);

	bool showLoginDialog();
	
	bool retrieveFileData(AdbFile* af, AdbConReply* r);
	
	AdbConReply* send(wxString op, wxString para, bool send_onlym);
	AdbConReply* send_layer1(wxString op, wxString param, bool send_only);
	AdbConReply* send_layer0(wxString s, bool send_only);
	
public:

	AdbConnection(AdbConOptions *opt, AdbUserPass *up, AdbConCallBack *log);

	int ping(void);
	
	bool connect(void);
	bool disconnect(void);
	bool login(void);
	bool logout(bool exit);
	bool retrieveFileData(AdbFile* af);
	bool addFileToMylist(AdbFile* af, wxString me);
	bool removeFileFromMylist(int lid, wxString name);
	
	wxString getLastError(){
		return lastError;
	}
};

#endif // __ANIDB_H

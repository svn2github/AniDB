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

#include <wx/socket.h>
#include <wx/wx.h>
#include <wx/datetime.h>

#include "ame-id.h"
#include "anidb-base.h"

#define ADEBUG(x) m_cb->print(MSG_DEB, x);
#define AERROR(x) m_cb->print(MSG_POP|MSG_LOG, x); lastError=x;
#define ALOG(x) m_cb->print(MSG_LOG, x); lastError=x;

AdbConnection::AdbConnection(AdbConOptions *opt, AdbUserPass *up, AdbConCallBack *cb){
	lastError = wxT("AdbConnection Not Initialized.");
	ver = wxT("&protover=3&client=ameame&clientver=2&enc=utf8");
	
	max_timeouts = 3;
	remaining_auth_attempts=3;
	lastTimeUsed = lastTimeStamp = 0;

	loggd = false;
	old = false;

	m_udp = NULL;

	m_cb = cb;
	m_opt = opt;
	m_up = up;
}
bool AdbConnection::connect(void){
	wxIPV4address m_localaddr;
	m_localaddr.AnyAddress();
    m_localaddr.Service(m_opt->local_port);
	m_udp = new wxDatagramSocket(m_localaddr, wxSOCKET_BLOCK); //, wxSOCKET_NOWAIT
	if(!m_udp->Ok()){
		AERROR(wxT("Could not create datagram socket! Try to change the port."));
		delete m_udp;
		m_udp=NULL;
		return false;
	}
	m_remoteaddr = new wxIPV4address();
	if(!m_remoteaddr->Hostname(m_opt->host)||!m_remoteaddr->Service(m_opt->remote_port)){
		AERROR(wxT("Failed to set remote host or port."));
		delete m_remoteaddr;
		m_remoteaddr = NULL;
		return false;
	}
	lastError = wxT("User Abort.");
	return true;
}
bool AdbConnection::disconnect(void){
	if(m_udp==NULL) return false;

	m_udp->Destroy();
	delete m_remoteaddr;
	return true;
}
AdbConReply* AdbConnection::send(wxString op, wxString param, bool send_only){
	int timeouts = 0;
	AdbConReply* r;
	while(max_timeouts>timeouts++){
		r = send_layer1(op, param, send_only);
		if(m_cb->abort()) break;
		if(r==NULL) continue; // timeout
		if(!op.StartsWith(wxT("LOGOUT"))&&(r->code==rLOGIN_FIRST||r->code==rINVALID_SESSION)){
			if(remaining_auth_attempts<=0){
				AERROR(wxT("Invalid session."));
				return NULL;
			}
			remaining_auth_attempts--;
			if(login())
				return send(op, param, send_only);
			return NULL;
		}
		return r;
	}
	ALOG(getLastError());
	return NULL;
}
AdbConReply* AdbConnection::send_layer1(wxString op, wxString param, bool send_only){
	if(m_session.Length()>0){
		if(param.Length()>0)
			param.Append(wxT("&s="));
		else param = wxT("s=");
		param.Append(m_session);
	}
	return send_layer0(op+wxT(" ")+param, send_only);
}
AdbConReply* AdbConnection::send_layer0(wxString s, bool send_only){
	if(old){
		time_t td = m_sw.Time();

		if(td<m_opt->delay){
			ADEBUG(SP1("| Sleep: %d", m_opt->delay-td));
			wxThread::Sleep(m_opt->delay-td);
		}
		/*if(!send_only && m_cb->abort()){
			logout(true);
			return NULL;
		}*/
	} else old = true;

	m_sw.Start();

	wxString cen = s;
	int i = cen.Find(wxT("pass="));
	if(i>0&&i<=cen.Length()){
		int j = cen.find(wxT("&"), i);
		if(j>0&&j<=cen.Length()) cen = cen.substr(0, i+5)+wxT("xxxxx")+cen.substr(j);
	}

	ADEBUG(wxT("> ") + cen);
	
	//const char * outb = s.mb_str(wxConvUTF8);
	const wxCharBuffer outb = s.mb_str(wxConvUTF8);
	m_udp->SendTo((*m_remoteaddr), /*s.ToAscii().data()*/outb, strlen(outb.data()));//(wxUint32)s.Length()); 
	if(m_udp->Error())
		ADEBUG(wxT("! Failed to send datagram!"));
	
	if(send_only) return NULL;

	char buf[UDP_BUF_MAX];
	wxDateTime t0 = wxDateTime::UNow();
	do{	
		m_udp->RecvFrom((*m_remoteaddr),buf, UDP_BUF_MAX);
		if(m_cb->abort()){
			logout(true);
			return NULL;
		}
		wxTimeSpan delta = wxDateTime::UNow()-t0;
		if(delta.GetMilliseconds()>m_opt->timeout){
			AERROR(wxT("! Timed out!"));
			return NULL;
		}
	}while(m_udp->Error());	

	lastTimeUsed = m_sw.Time();
	m_sw.Start();
	
	buf[m_udp->LastCount()-1] = '\0';

	wxString replymsg(buf,wxConvUTF8);
	
	ADEBUG(wxT("< ")+replymsg);
	
	AdbConReply* r = new AdbConReply(replymsg);
	return r;
}
int AdbConnection::ping(void){
	AdbConReply* r = send_layer0(wxT("PING\r"), false);
	if(r!=NULL){
		delete r;
		return (int)lastTimeUsed;
	}
	return -1;
}
bool AdbConnection::retrieveFileData(AdbFile* af, AdbConReply* r){
	if(!r) return false;
	bool x = false;
	switch(r->code){
		case rFILE:
			x = af->parse(r->data);
			x = true;
			break;
		case rNO_SUCH_FILE:
			ALOG(wxT("The file does not exist in AniDB."));
			break;
		default:
			ADEBUG(r->message);
	}
	delete r;
	return x;
}
/*bool AdbConnection::retrieveFileData(AdbFile* af, int fid){
	return retrieveFileData(af, send("FILE", str_stream() << "fid=" << fid));
}*/
bool AdbConnection::retrieveFileData(AdbFile* af){
	return retrieveFileData(af, send(wxT("FILE"), af->formatByHash()+wxT("&fcode=90128158&acode=75435779"), false));
}
bool AdbConnection::addFileToMylist(AdbFile* af, wxString me){
	AdbConReply* r = send(wxT("MYLISTADD"), af->formatByFID()+me, false);
	if(r==NULL) return false;
	switch(r->code){
		case rFILE_ALREADY_IN_MYLIST:
			ALOG(wxT("The file is already in your mylist."));
		case rMYLIST_ENTRY_EDITED:
		case rMYLIST_ENTRY_ADDED:{
			long l;
			if(r->data.ToLong(&l))
				af->m_lid = l;
			else af->m_lid = 0;
			delete r;
			return true;		
			
			//break;
			}
		case rNO_SUCH_MYLIST_FILE:
			ALOG(wxT("The file was not found in AniDB. Add it! (If not corrupt)"));
		case rNO_SUCH_MYLIST_ENTRY:
		default:
		ALOG(r->message);
	}
	delete r;
	return false;
}
bool AdbConnection::removeFileFromMylist(int lid, wxString name){
	AdbConReply* r = send(wxT("MYLISTDEL"), SP1("lid=%d",lid), false);
	if(r==NULL) return false;
	switch(r->code){
		case rENTRY_DELETED:
			delete r;
			return true;
		case rNO_SUCH_MYLIST_ENTRY:
			delete r;
			return false;
		default:
			ALOG(SP2("%s (%s)", r->message, name));
	}
	delete r;
	return false;
}
bool AdbConnection::login(){
	if(!m_up->ok()){
		ALOG(wxT("User Abort."));
		return false;
	}
	m_session = wxT("");
	AdbConReply* r = send(wxT("AUTH"), wxT("user=")+m_up->username+wxT("&pass=")+m_up->password+ver, false);
	if(r==NULL){
		AERROR(wxT("AdbConReply is NULL!"));
		return false;
	}
	switch(r->code){
		case rLOGIN_ACCEPTED_NEW_VER:
			m_cb->print(MSG_POP, L"New version available!");
		case rLOGIN_ACCEPTED:{
			m_session = r->data;
			loggd = true;
			delete r;
			return true;
		}
		case rLOGIN_FAILED:
			ALOG(wxT("Login Failed"));
			m_up->password = wxT("");
			m_up->rem_att--;
			if(showLoginDialog()){
				delete r;
				return login();
			}
			break;
		default:
			ALOG(r->message);
	}
	delete r;
	return false;
}
bool AdbConnection::logout(bool exit){
	if(!loggd) return true;
	loggd = false;
	AdbConReply* r = send(wxT("LOGOUT"), wxT(""), exit);
	if(r==NULL) return false;
	switch(r->code){
		case rLOGGED_OUT:
			delete r;
			return true;
		case rNOT_LOGGED_IN:
			ALOG(wxT("Not Logged In"));
		default:
			ALOG(r->message);
	}
	delete r;
	return false;
}
bool AdbConnection::showLoginDialog(){
	if(m_up->rem_att<1){
		AERROR(wxT("Access Denied! No more login attempts allowed."));
		return false;
	}
	ALOG(wxString::Format(wxT("Logins left: %d"), m_up->rem_att));
	
	return m_cb->showLogin();
}

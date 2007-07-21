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
#include <wx/thread.h>
#include <wx/filename.h>


#include "ame-id.h"
#include "ame-job.h"
#include "ame-diag-login.h"
#include "ame-thread-netio.h"
#include "anidb-base.h"

class MyCallBack : public AdbConCallBack{
private:
	wxThread *m_thr;
public:
	AdbUserPass *m_up;
	MyCallBack(wxThread *thr, AdbUserPass *up) : AdbConCallBack(){
		m_thr = thr;
		m_up = up;
	};
	bool abort(void){
		return m_thr->TestDestroy();
	}
	void print(int i, wxString s){
		POSTM(i,s);
	}
	bool showLogin(void){
		POSTE(EVT_NET_LOGIN);
		return false;
	}
};

AmeThreadNetIO::AmeThreadNetIO(wxFrame* frame, AdbConOptions *opt, AdbUserPass *up, wxString me):wxThread(){
	m_frame = frame;
	m_opt = opt;
	m_cb = new MyCallBack(this, up);
	m_me = me;
}
AmeThreadNetIO::~AmeThreadNetIO(){
	delete m_opt;
	delete m_cb;
}
void AmeThreadNetIO::OnExit(){
	wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, EVT_NET_BYE );
	wxPostEvent( m_frame, evt );
}

void *AmeThreadNetIO::Entry(){
	AmeJobList *jl = wxGetApp().m_jl;
	AmeJob *j;
	AmeApp *a = &(wxGetApp());
	ac = new AdbConnection(m_opt, m_cb->m_up, m_cb);
	if(ac->connect()){	
		if(ac->ping()>=0){
			if(ac->login()){
				while(!TestDestroy()){
					wxThread::Sleep(200);
					for(size_t i=0; i<jl->count()&&!TestDestroy(); i++){
						j = jl->get(i);
						if(j->check(jH_NORMAL)){
							if(j->check(jIDENTWAIT))
								identify(j);							
							else if(j->check(jADDWAIT))
								add(j);
							else if(j->check(jREMWAIT))
								remove(j);
						}
					}					
				}
				ac->logout(true);
			}// else wxMessageDialog( NULL, wxT("Error"), wxT("Login failed."), wxOK);
		}//else wxMessageDialog( NULL, wxT("Error"), wxT("Server is not reachable."), wxOK);
		ac->disconnect();
	}
	delete ac;
	return NULL;
}
bool AmeThreadNetIO::identify(AmeJob* j){
	AdbFile* f = new AdbFile();
	wxString name = j->getAbsolutePath();
	f->m_siz = j->getSize();
	f->m_ed2 = j->getEd2k();
	f->m_ext = j->getFileExt();
	
	AmeJobMan::updateStatus(j, jIDENTIFYING);
	POSTM(MSG_LOG, SP1("Identifying: %s", name));	

	if(ac->retrieveFileData(f)){
		j->setAdbFile(f);
		AmeJobMan::updateStatus(j, jIDENTIFIED);
		
		POSTM(MSG_LOG, SP1("Found: %s", f->m_def));
		
		if(j->check(jADDWAIT) && !TestDestroy())
			return add(j);
		return true;
	}
	delete f;
	if(TestDestroy())
		AmeJobMan::updateStatus(j, jIDENTWAIT);
	else{
		AmeJobMan::updateStatus(j, jUNKNOWN);
		POSTM(MSG_LOG, SP1("Not found: %s", name));
	}
	return false;	
}
bool AmeThreadNetIO::add(AmeJob* j){
	AdbFile* f;
	wxString name = j->getAbsolutePath();
	f = j->getAdbFile();

	AmeJobMan::updateStatus(j, jADDING);
	POSTM(MSG_LOG, SP1("Adding: %s", name));

	if(f!=NULL && ac->addFileToMylist(f, m_me)){
		AmeJobMan::updateStatus(j, jADDED);
		POSTM(MSG_LOG, SP1("Added: %s", name));
		return true;
	}
	j->setError(ac->getLastError());
	AmeJobMan::updateStatus(j, jFAILED);
	POSTM(MSG_LOG, SP1("Failed to add: %s", name));
	return false;
}
bool AmeThreadNetIO::remove(AmeJob* j){
	AdbFile* f;
	wxString name = j->getAbsolutePath();
	f = j->getAdbFile();

	if(f==NULL || f->m_lid<1){
		AmeJobMan::updateStatus(j, jFAILED);
		j->setError(wxT("Was not in mylist."));
		POSTM(MSG_LOG, SP1("The file '%s' is not in your mylist. (File is unknown or lid is 0)", name));
		return false;
	}

	AmeJobMan::updateStatus(j, jREMING);
	POSTM(MSG_LOG, SP1("Removing: %s", name));

	if(ac->removeFileFromMylist(f->m_lid, j->getFileName())){
		AmeJobMan::updateStatus(j, jFINISHED);
		f->m_lid = 0;
		POSTM(MSG_LOG, SP1("Removed: %s", name));
		return true;
	}
	j->setError(ac->getLastError());
	AmeJobMan::updateStatus(j, jFAILED);
	POSTM(MSG_LOG, SP1("Failed to remove: %s", name));
	return false;
}

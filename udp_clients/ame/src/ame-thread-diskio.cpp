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
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/timer.h>
#include <wx/filefn.h>
#include <wx/thread.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/longlong.h>

#include "ame-id.h"
#include "ame-job.h"
#include "ame-dir-trav.h"
#include "hash-base.h"

#include "ame-thread-diskio.h"

AmeThreadDiskIO::AmeThreadDiskIO(wxFrame* frame, wxString d, wxArrayString e):wxThread(),m_dirs(d),m_exts(e){
	m_frame = frame;
};
void AmeThreadDiskIO::OnExit(){
	wxCommandEvent evt( wxEVT_COMMAND_MENU_SELECTED, EVT_DISK_BYE );
	wxPostEvent( m_frame, evt );
}
void *AmeThreadDiskIO::Entry(){
	AmeJobList *jl = wxGetApp().m_jl;
	AmeJob *j;
	while(true){
		size_t cnt = 0;
		for(size_t i=0; i<jl->count(); i++){
			j = jl->get(i);
			if(j->check(jH_NORMAL|jHASHWAIT)){
				hash(j);
				cnt++;
			}
			if(TestDestroy()) return NULL;
			if(j->check(jH_NORMAL|jMOVEWAIT)){
				move(j);
				cnt++;
			}
			if(TestDestroy()) return NULL;
		}
		if(cnt<1) checkDirs();
		if(TestDestroy()) break;
		wxThread::Sleep(200);
	}
	return NULL;
}
bool AmeThreadDiskIO::hash(AmeJob* j){
	wxString name = j->getFileName();
	wxFile file(j->getAbsolutePath(), wxFile::read);
	wxFileInputStream in(j->getAbsolutePath());

	AmeJobMan::updateStatus(j, jHASHING);
	POSTM(MSG_LOG, SP1("Hashing: %s", name));

	int b_size = 1024*1024*2;
	char *buffer = new char[b_size];
	HashED2K ed;
	wxStopWatch sw;
	for(;;){
		in.Read(buffer, b_size);
		if(in.LastRead()<1) break;
		ed.update(buffer, 0, in.LastRead());
		if(TestDestroy()){
			AmeJobMan::updateStatus(j, jHASHWAIT);
			POSTM(MSG_LOG, wxT("Disk thread aborted."));

			delete buffer;
			return false;
		}
		POSTG(in.TellI()*100/file.Length());
	}
	delete buffer;
	
	char ed2k[33];
	ed.getHexValue((char*)&ed2k);
	ed2k[32] ='\0';
	wxString str(ed2k, wxConvLocal);

	j->setEd2k(str);
	AmeJobMan::updateStatus(j, jHASHED);

	wxString euri = SP2("ed2k://|file|%s|%d|", name, file.Length())+str+wxT("|");
	POSTM(MSG_HSH, euri);
	
	double s = file.Length();
	POSTM(MSG_LOG, SP1("Hashed @ %.2f MB/s: ",(s*1000)/sw.Time()/1048576)+euri);	

	return true;
}
bool AmeThreadDiskIO::move(AmeJob *j){
	wxString name = j->getFileName();
	wxString path = j->getAbsolutePath();
	wxString path_new = j->getNewFilePath();

	if(path==path_new){
		j->setNewFilePath(wxEmptyString);
		AmeJobMan::updateStatus(j, jFINISHED);
		return true;
	}
	
	if(wxFile::Exists(path_new) /* || free<file_in.Length() ||*/){
		j->setError(wxString::Format(wxT("Illegal path or out of space.\n%s\n%s"), path, path_new));
		AmeJobMan::updateStatus(j, jFAILED);
		return false;
	}

	wxStopWatch sw;
	{
		wxFile file_in(path, wxFile::read);
		wxFile file_out(path_new, wxFile::write);
		wxFileInputStream in(file_in);
		wxFileOutputStream out(file_out);
		if(!in.Ok()||!out.Ok()){
			j->setError(wxT("Failed to create streams."));
			AmeJobMan::updateStatus(j, jFAILED);
			return false;
		}
			
		AmeJobMan::updateStatus(j, jMOVING);		
		POSTM(MSG_LOG, SP2("Moving: %s to %s", path, path_new));

		int b_size = 1024*1024*2;
		char *buffer = new char[b_size];
		size_t read;

		for(;;){
			in.Read(buffer, b_size);
			read = in.LastRead();
			if(read<1) break;
			out.Write(buffer, read);
			if(TestDestroy()){
				out.Close();
				delete buffer;
				AmeJobMan::removeFile(path_new);
				AmeJobMan::updateStatus(j, jMOVEWAIT);
				POSTM(MSG_LOG, wxT("Disk thread aborted."));
				return false;
			}
			POSTG(in.TellI()*100/file_in.Length());
		}
		out.Close();
		delete buffer;
	}
	wxString ed2k = check(j);
	if(ed2k.Length()==32){
		if(ed2k.CmpNoCase(j->getEd2k())==0){
			AmeJobMan::updateStatus(j, jMOVED);
			AmeJobMan::removeFile(path);
			j->setAbsolutePath(path_new);
			double s = j->getSize();
			POSTM(MSG_LOG, SP1("Moved @ %.2f MB/s: ",(s*1000)/sw.Time()/1048576)+path_new);
		}else{
			j->setNewFilePath(wxEmptyString);
			AmeJobMan::removeFile(path_new);
			AmeJobMan::updateStatus(j, jFAILED);
			POSTM(MSG_LOG, wxT("Move failed: ")+name);
		}
		return true;
	}
	AmeJobMan::updateStatus(j, jMOVEWAIT);
	return false;
}
wxString AmeThreadDiskIO::check(AmeJob *j){
	wxString path = j->getNewFilePath();
	wxFile file(path, wxFile::read);
	wxFileInputStream in(j->getAbsolutePath());

	AmeJobMan::updateStatus(j, jMOVECHECK);
	POSTM(MSG_LOG, SP1("Checking: %s", path));
	
	int b_size = 1024*1024*2;
	char *buffer = new char[b_size];
	HashED2K ed;
	wxStopWatch sw;
	for(;;){
		in.Read(buffer, b_size);
		if(in.LastRead()<1) break;
		ed.update(buffer, 0, in.LastRead());
		if(TestDestroy()){
			AmeJobMan::updateStatus(j, jHASHWAIT);
			POSTM(MSG_LOG, wxT("Disk thread aborted."));
			delete buffer;
			return wxEmptyString;
		}
		POSTG(in.TellI()*100/file.Length());
	}
	delete buffer;
	char ed2k[33];
	ed.getHexValue((char*)&ed2k);
	ed2k[32]='\0';
	return wxString(ed2k, wxConvLocal);
}
void AmeThreadDiskIO::checkDirs(void){
	wxArrayString as;
	wxDirTraverserSimple traverser(as, m_exts);

	wxStringTokenizer st(m_dirs, wxT(";"));
	while(st.HasMoreTokens()){
		wxString path = st.GetNextToken();
		if(wxDir::Exists(path)){
			wxDir dir(path);
			dir.Traverse(traverser, _T(""),wxDIR_DIRS|wxDIR_FILES);
		}
	}

	wxArrayString::iterator it;
	for(it=as.begin(); it!=as.end(); ++it)
		wxGetApp().m_jl->addFile(*it);

	POSTE(EVT_JOB_LIST_LEN);
}

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

#include <wx/wxprec.h>

#include <wx/listctrl.h>
#include <wx/html/htmlwin.h>

#include <process.h>

#include "ame-id.h"
#include "ame-tools.h"
#include "ame-db.h"
#include "ame-job.h"
#include "ame-main.h"
#include "ame-strc.h"
#include "anidb-base.h"

#define GET(x) wxGetApp().m_jl->get(x)
#define NEXT(x) GetNextItem(x, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)
#define REFRESH(x) RefreshItem((long)x->getID())

#define REFRESH_BLOCK(x)\
	AmeJob *j;\
	long item = NEXT(-1);\
	while(item>=0){\
		j = GET(item);\
		x;\
		REFRESH(j);\
		item = NEXT(item);\
	}
#define UPDATE_STATUS(x) REFRESH_BLOCK(if(x==jHASHED) j->deleteAdbFile(); AmeJobMan::updateStatus(j, x))
//#define FOLLOW_LINK(x){ long item = NEXT(-1); if(item>=0){ AdbFile f* = GET(item)->getAdbFile(); if(f) f->x; } }
AmeJob* AmeCtrlJobList::GetSelectedJob(void){
	long item = NEXT(-1);
	if(item>=0){
		return GET(item);
	}
	return NULL;
}
void AmeCtrlJobList::DoPause(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jH_PAUSED);
}
void AmeCtrlJobList::DoApplyRules(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jIDENTIFIED);
}
void AmeCtrlJobList::DoWatchNow(wxCommandEvent& WXUNUSED(event)){
	AmeJob *j = GetSelectedJob();
	if(j&&!j->check(jH_MISSING)){
		MYEXEC(j->getAbsolutePath());
	}
}
void AmeCtrlJobList::DoExploreFolder(wxCommandEvent& WXUNUSED(event)){
	AmeJob *j = GetSelectedJob();
	if(j&&!j->check(jH_MISSING)){
		MYEXPL(j->getFolderPath());
	}
}
void AmeCtrlJobList::DoReHash(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jHASHWAIT)
}
void AmeCtrlJobList::DoReId(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jHASHED)
}
void AmeCtrlJobList::DoReAdd(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jADDWAIT)
}
void AmeCtrlJobList::DoRemove(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jREMWAIT)
}
void AmeCtrlJobList::DoSetFinished(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jFINISHED)
}
void AmeCtrlJobList::DoRestoreName(wxCommandEvent& WXUNUSED(event)){
	REFRESH_BLOCK(AmeJobMan::restoreName(j));
}
void AmeCtrlJobList::DoSetFolder(wxCommandEvent& WXUNUSED(event)){
	if(NEXT(-1)<0) return;
	wxDirDialog diag(this,_T("Select a directory"),_(""),wxFILE_MUST_EXIST);
	if(diag.ShowModal()==wxID_OK){
		wxString path = diag.GetPath();
		REFRESH_BLOCK(AmeJobMan::setPath(j, path, false));
	}
}
void AmeCtrlJobList::DoSetParentFolder(wxCommandEvent& WXUNUSED(event)){
	if(NEXT(-1)<0) return;
	wxDirDialog diag(this,_T("Select a directory"),_(""),wxFILE_MUST_EXIST);
	if(diag.ShowModal()==wxID_OK){
		wxString path = diag.GetPath();
		REFRESH_BLOCK(AmeJobMan::setPath(j, path, true));
	}
}
void AmeCtrlJobList::DoEditFolderPath(wxCommandEvent& WXUNUSED(event)){
	long i = NEXT(-1);
	if(i<0) return;
	AmeJob *job = GET(i);
	wxTextEntryDialog diag(this, wxT("Change path (for all selected files) to:"), wxT("Edit path"), job->getFolderPath());
	if(diag.ShowModal()==wxID_OK){
		wxString path = diag.GetValue();
		REFRESH_BLOCK(AmeJobMan::setPath(j, path, false));
	}
}
void AmeCtrlJobList::DoEditFileName(wxCommandEvent& WXUNUSED(event)){
	AmeJob *j = GetSelectedJob();
	if(j){
		wxTextEntryDialog diag(this, wxT("Change name to:"), wxT("Edit name"), j->getFileName());
		if(diag.ShowModal()==wxID_OK){
			AmeJobMan::setName(j, diag.GetValue());
			REFRESH(j);
		}
	}
}
void AmeCtrlJobList::DoSetFid(wxCommandEvent& WXUNUSED(event)){
	POPT("Not Implemented");
}
void AmeCtrlJobList::DoParseWithAvinfo(wxCommandEvent& WXUNUSED(event)){
	POPT("Not Implemented");
	//UPDATE_STATUS(jPARSEWAIT)
}
void AmeCtrlJobList::DoRemoveFromDb(wxCommandEvent& WXUNUSED(event)){
	UPDATE_STATUS(jH_DELETED)
}
void showInfo(AmeCtrlJobList *l){
	long item = l->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if(item>=0){
		AmeJob *j = wxGetApp().m_jl->get(item);
		wxString z = wxT(FILE_HTM);
		/*{
			wxFFile file(L"D:\\Visual Pro\\wxtest\\file.htm");
			file.ReadAll(&z, wxConvUTF8);
		}*/
		StringMap sm;
		j->makeMap(&sm);

		wxString u, s; int i;
		do{
			i = z.Find('%');
			if(i<0){
				s.Append(z);
				break;
			}
			s.Append(z.SubString(0,i-1));
			if(i+3>=(int)z.Length()){
				s.Append(z.substr(i));
				break;
			}
			u = z.substr(i+1,3);
			z = z.substr(i+4);
			s.append(sm.get(u));
		}while(i>=0);
		new MyHtmlFrame(j->getFileName(),s);
	}
}
void AmeCtrlJobList::DoShowInfo(wxCommandEvent& WXUNUSED(event)){
	showInfo(this);
}
void AmeCtrlJobList::OnChar(wxKeyEvent& event){
	//POP(SP1("%d", event.GetKeyCode()));
	if(event.GetKeyCode()<128){
		long item = NEXT(-1);
		if(item>-1){
			AmeJob *j = GET(item);
			AdbFile *f = j->getAdbFile();
			if(f){
				switch(event.GetKeyCode()){
					case 'A': MYEXEC(f->urlAnime()); break;
					case 'M': MYEXEC(f->urlMylist()); break;
					case 'N': MYEXEC(f->urlMylistX()); break;
					case 'E': MYEXEC(f->urlEpisode()); break;
					case 'G': MYEXEC(f->urlGroup()); break;
					case 'F': MYEXEC(f->urlFile()); break;
					case 'K': MYEXEC(f->urlExport()); break;
					case 'C': AVDUMP(j->getAbsolutePath()); MYEXEC(f->urlFile()); break;
				}
			}
			switch(event.GetKeyCode()){
				case 'W': MYEXEC(j->getAbsolutePath()); break;
				case 'X': MYEXPL(j->getFolderPath()); break;
				case 'P': {UPDATE_STATUS(jH_PAUSED);} break;
				case 'S': AmeJobMan::updateStatus(j, jIDENTIFIED); break;
				case 'I':
					j->deleteAdbFile();
					AmeJobMan::updateStatus(j, jHASHED); break;
				case ' ':
				case 13: showInfo(this); break;
				//case 39: if(m_tt!=null) m_tt.expandRow(); break;
				//case 37: if(m_tt!=null) m_tt.collapseRow(); break;
				//default: POP(SP1("%d", event.GetKeyCode()));
			}
		}
	}else event.Skip();
}
void AmeCtrlJobList::OnListKeyDown(wxListEvent& event){
	/*long item = NEXT(-1);
	if(item<0) return;
	AmeJob *j = GET(item);
	AdbFile *f = j->getAdbFile();
	//POP(SP2("%d %d", event.GetKeyCode(), 'f'));
	switch(event.GetKeyCode()){
		case 'A': MYEXEC(f->urlAnime()); break;
		case 'M': MYEXEC(f->urlMylist()); break;
		case 'N': MYEXEC(f->urlMylistX()); break;
		case 'E': MYEXEC(f->urlEpisode()); break;
		case 'G': MYEXEC(f->urlGroup()); break;
		case 'F': MYEXEC(f->urlFile()); break;
		case 'K': MYEXEC(f->urlExport()); break;
		case 'W': MYEXEC(j->getAbsolutePath()); break;
		case 'X': MYEXPL(j->getFolderPath()); break;
		case 'C': AVDUMP(SP1("\"%s\"",j->getAbsolutePath())); MYEXEC(f->urlFile()); break;
		case 'P': DoPause(event); break;
		case 'S': DoApplyRules(event); break;
		//case 'i': DoIdentify(NULL); break;
		case ' ':
		case 10: DoShowInfo(event); break;
		//case 39: if(m_tt!=null) m_tt.expandRow(); break;
		//case 37: if(m_tt!=null) m_tt.collapseRow(); break;
		//default: con = false;
	}*/
}
void AmeCtrlJobList::OnMouseEvent(wxMouseEvent& event){
	//int flags;
	if(event.LeftDClick()){
		showInfo(this);
		/*long item = HitTest(event.GetPosition(), flags);
		if(item>=0){
			AmeJob *j = wxGetApp().m_jl->get(item);
			MyTextDialog tx(wxGetApp().m_frame, wxT("File Info"), j->dump());
			tx.ShowModal();
		}*/
	}else if(event.RightDown()){
		wxMenu *pop = new wxMenu;
		pop->Append( EVT_PAUSE, wxT("Pause") );
		pop->AppendSeparator();
		pop->Append( EVT_SHOW_INFO, wxT("Show info") );
		pop->Append( EVT_WATCH_NOW, wxT("Watch now") );
		pop->Append( EVT_EXPLORE_FOLDER, wxT("Explore folder") );
		pop->AppendSeparator();
		pop->Append( EVT_REHASH, wxT("Rehash") );
		pop->Append( EVT_REID, wxT("Identify") );
		pop->Append( EVT_READD, wxT("Add to mylist") );
		pop->Append( EVT_REMOVE, wxT("Rem from mylist") );
		pop->Append( EVT_APPLY_RULES, wxT("Apply Rules") );
		pop->AppendSeparator();
		pop->Append( EVT_SET_FINISHED, wxT("Set finished") );
		pop->Append( EVT_RESTORE_NAME, wxT("Restore name") );
		pop->Append( EVT_SET_FOLDER, wxT("Set folder") );
		pop->Append( EVT_SET_PARENT_FOLDER, wxT("Set parent folder") );
		pop->Append( EVT_EDIT_FOLDER_PATH, wxT("Edit folder path") );
		pop->Append( EVT_EDIT_FILE_NAME, wxT("Edit file name") );
		pop->AppendSeparator();
		pop->Append( EVT_PARSE_WITH_AVINFO, wxT("Parse with avinfo") );
		pop->Append( EVT_SET_FID, wxT("Set fid (force)") );
		pop->Append( EVT_REMOVE_FROM_DB, wxT("Remove from db") );
		
		PopupMenu(pop);
		delete pop;

	}else event.Skip();
}

void AmeCtrlJobList::OnColumnClick(wxListEvent &event){  
	A.m_jl->sortColumn(event.GetColumn());
	RefreshItems(0,GetItemCount()-1);
} 
wxString AmeCtrlJobList::OnGetItemText(long item, long column) const{
	if(column==0) return wxString::Format(_T("%d"), item);
	if(item>=(long)wxGetApp().m_jl->count()) return _T("ERR");//TODO lock?
	AmeJob* j = wxGetApp().m_jl->get(item);
	AdbFile *f = j->getAdbFile();
	switch(column){
		//case 0: return wxString::Format(_T("%d"), j->getID());
		case 1: return j->getAbsolutePath();
		case 2: return j->getStatusText();
		case 3: if(f) return f->m_anime->ann; break;
		case 4: if(f) return f->m_episode->epn; break;
		case 5: if(f && f->m_group) return f->m_group->grp; break;
		case 6: if(f) return SPD(f->m_lid); break;
		case 7: if(f) return f->toMds(); break;
	}
	return wxT("N/A");
}
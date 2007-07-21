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
#include <wx/filename.h>
#include <wx/dynarray.h>
#include <wx/hashset.h>

#include "ame-id.h"
#include "ame-job.h"
#include "ame-db.h"
//#include "anidb-base.h"

WX_DEFINE_ARRAY(AmeJob*, myArray);
WX_DECLARE_HASH_SET( wxString, wxStringHash, wxStringEqual, myHashSet );

AmeJobList::AmeJobList(void){
	m_list = new myArray();
	m_hs = new myHashSet();
}
AmeJobList::~AmeJobList(void){
	for(size_t i=0; i<count(); i++)
		delete get(i);

	delete m_list;
	delete m_hs;
}
bool AmeJobList::addFile(wxString path){
	wxCriticalSectionLocker locker(m_cs);
	if(m_hs->find(path)==m_hs->end()){
		m_hs->insert(path);
		AmeJob *j = new AmeJob(path, jHASHWAIT|jH_NORMAL, m_list->Count());
		m_list->Add(j);
		int s = A.m_db->getJob(j, false);
		if(s>0) j->setStatus(s, false);
		//j->m_idx = m_list->Index(j, TRUE);
		return true;
	}
	return false;
}
bool AmeJobList::addJob(AmeJob *j){
	wxCriticalSectionLocker locker(m_cs);
	wxString path = j->getAbsolutePath();
	if(m_hs->find(path)==m_hs->end()){
		m_hs->insert(path);
		j->m_idx = m_list->Count();
		m_list->Add(j);
	}
	return false;
}
void AmeJobList::addPath(wxString path){
	wxCriticalSectionLocker locker(m_cs);
	if(m_hs->find(path)==m_hs->end()){
		m_hs->insert(path);
	}
}
size_t AmeJobList::count(void){
	wxCriticalSectionLocker locker(m_cs);
	return m_list->size();
}
AmeJob* AmeJobList::get(size_t i){
	wxCriticalSectionLocker locker(m_cs);
	return m_list->Item(i);
}
int mysort2(AmeJob *first, AmeJob *second){
	//POSTM(MSG_POP, first->getFileName());
	//return first->getAbsolutePath().CmpNoCase(second->getAbsolutePath());
	return rand()-RAND_MAX/2;
}
void AmeJobList::sortColumn(int c){
	m_list->Sort((CMPFUNC_wxArraymyArray) mysort2);
}
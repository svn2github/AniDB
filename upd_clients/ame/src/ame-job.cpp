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
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/hashmap.h>

#include "ame-job.h"

#include "ame-id.h"
#include "anidb-base.h"

AmeJob::AmeJob(wxString path, int status, size_t id, long size):
		/*m_file(path),*/m_file_name(path),m_err(wxT("N/A")),m_ed2(wxT("Not hashed")){
	m_status = status;
	if(wxFile::Exists(path)){
		wxFile f(path);
		m_size = f.Length();
		if(m_size<1) 
			setHealth0(jH_MISSING);
	}else{
		setHealth0(jH_MISSING);
		m_size = size;
	}
	int x = path.Find(FILE_SEPARATOR,TRUE);
	if(x>0){
		m_ori = path.SubString(x+1, path.Length());
	}
	m_idx = id;
	m_did = 0;
	m_fa = NULL;
	m_new = true;
}
AmeJob::~AmeJob(void){
	if(m_fa) delete m_fa;
}
void AmeJob::find(wxString path){
	AL();
	if(!check(jH_MISSING)) return;
	//JobMan.setJobFile(this, f);
	//m_file = path;
	m_file_name = path;
	A.m_jl->addPath(path);
	setHealth0(wxFile::Exists(path)?jH_PAUSED:jH_MISSING);
	m_did = -1;
}
void AmeJob::deleteAdbFile(){
	AL();
	if(m_fa){
		delete m_fa;
		m_fa=NULL;
	}
}
wxString statusStr(int i){
	switch(i){
		case jHASHWAIT:		return wxT("Wait/Hash");
		case jHASHING:		return wxT("Hashing");
		case jHASHED:		return wxT("Hashed");
		case jIDENTWAIT:	return wxT("Wait/ID");
		case jIDENTIFYING:	return wxT("Identifying");
		case jIDENTIFIED:	return wxT("Identified");
		case jADDWAIT:		return wxT("Wait/Add");
		case jADDING:		return wxT("Adding");
		case jADDED:		return wxT("Added");
		case jMOVEWAIT:		return wxT("Wait/Move");
		case jMOVING:		return wxT("Moving");
		case jMOVECHECK:	return wxT("Checking");
		case jMOVED:		return wxT("Moved");
		case jFINISHED:		return wxT("Finished");
		case jUNKNOWN:		return wxT("Unknown");
		case jFAILED:		return wxT("Failed");
//		case jINPUTWAIT:	return wxT("Waiting/Input ");
		case jH_PAUSED:		return wxT("P");
		case jH_DELETED:	return wxT("D");
		case jH_MISSING:	return wxT("M");
		case jPARSEWAIT:	return wxT("Wait/Parse");
		case jPARSING:		return wxT("Parsing");
		case jREMWAIT:		return wxT("Wait/Rem");
		case jREMING:		return wxT("Removing");
		default:			return wxString::Format(wxT("No Such Status: %d"), i);
	}
};
wxString AmeJob::getStatusText(void){
	if(check(jH_NORMAL))
		return statusStr(getStatus());
	return wxString::Format(_T("%s [%s]"), statusStr(getStatus()), statusStr(getHealth()));
}
int AmeJob::getStatus(void){
	AL();
	return m_status&jM_S;
}
int AmeJob::getHealth(void){
	AL();
	return m_status&jM_H;
}
inline int AmeJob::getRegVal(){
	AL();
	return (m_status&jM_R)|jF_DB;
}
inline bool AmeJob::check(int s){
	AL();
	return (m_status&s)==s;
}
inline bool AmeJob::checkOr(int s){
	AL();
	return (m_status&s)>0;
}
bool AmeJob::isLocked(int s){
	AL();
	int h = getHealth();
	if(h<jH_MISSING) return false;
	if(h==jH_MISSING){
		switch(s){
		case jFINISHED:
		case jREMWAIT:
			return false;
		}
	}
	return true;
}
wxString AmeJob::getFileName(void){
	AL();
	/*int x = m_file.Find(FILE_SEPARATOR,TRUE);
	if(x>0) return m_file.SubString(x+1, m_file.Length());
	return wxT("");*/
	return m_file_name.GetFullName();
}
wxString AmeJob::getFolderPath(void){
	AL();
	/*int x = m_file.Find(FILE_SEPARATOR,TRUE);
	if(x>0) return m_file.SubString(0, x-1);
	return wxT("NULL");*/
	return m_file_name.GetPath();
}
wxString AmeJob::getFileExt(void){
	AL();
	/*int x = m_file.Find('.',TRUE);
	if(x>0) return m_file.SubString(x, m_file.Length());
	return wxT("");*/
	wxString x = m_file_name.GetExt();
	if(x.Length()<1) return x;
	return wxT(".")+x;
}
wxString AmeJob::getAbsolutePath(void){
	//AL();
	return m_file_name.GetFullPath();
}
void AmeJob::setAbsolutePath(wxString s){
	AL();
	//m_file = s;
	m_file_name = s;
	A.m_jl->addPath(s);
}
wxString AmeJob::dump(void){
	AL();
	StringMap sm;
	makeMap(&sm);
	return sm.toString();
}
inline bool checkPart(wxString a, wxString b){
#ifdef WIN32
	return a.at(0)==b.at(0);
#else
	return false; //TODO PLATFORM
#endif
}
void AmeJob::setStatus(int status, bool test){
	AL();
	int health = getHealth();
	if(test){
		if(health>jH_PAUSED&&status<jREMWAIT) return;
		if(health==jH_PAUSED) setHealth(jH_NORMAL);
		health = getHealth();
	}
	if((status&jF_DB)==jF_DB){//only for main status
		//if(test&&health==H_NORMAL)
		//	A.jobs.updateQueues(this, getStatus(), status&jM_S);//TODO pause off fix
		//A.jobc.register(getRegVal(), (status|jH_NORMAL)&jM_R);
		m_status = status|jH_NORMAL;
	}else m_status = status|jH_NORMAL;
}
void AmeJob::setHealth(int health){
	int s = getStatus();
	//if(!check(jH_NORMAL)&&health==jH_NORMAL)
	//	A.jobs.updateQueues(this, 0, s);
	//else if(check(jH_NORMAL)&&health!=jH_NORMAL)
	//	A.jobs.updateQueues(this, s, -1);

	//A.jobc.register(getRegVal(), s|health);
	setHealth0(health);
}
void AmeJob::setHealth0(int health){
	m_status = (m_status&jM_S)|health;
}
void AmeJob::setError(wxString err){
	AL();
	m_err = err;
}
void AmeJob::updateHealth(int i){
	AL();
	if(i!=jH_DELETED && checkOr(jH_MISSING|jH_DELETED))
		return;
	int health = getHealth();
	switch(i){
		case jH_PAUSED:
			if(health==jH_NORMAL)
				health = jH_PAUSED;//turn pause on
			else if(health==jH_PAUSED)
				health = jH_NORMAL; //turn pause off
			break;
		case jH_DELETED:
			if(health==jH_DELETED){
				health = jH_NORMAL;
				//A.db.update(0, this, DB.I_J);
			}else{
				health = jH_DELETED; //delete
				//A.db.removeJob(this);
			}
			break;
		case jH_MISSING:
			health = jH_MISSING;
			break;
	}
	setHealth(health);
}
bool AmeJob::rename(wxString path){
	AL();
	
	wxFileName fn(path);
	//if(checkPart(m_file, m_fn)){
	if(AmeJobMan::makeDirs(fn.GetPath()) && wxRenameFile(m_file_name.GetFullPath(), path)){
		//wxString old = m_file;
		//removeDirs(old);
		//m_file = path;
		m_file_name = path;
		A.m_jl->addPath(path);
		//if(!wxFile::Exists(path))
		//	setHealth0(H_MISSING);
		m_did = -1;
		return true;
	}
//}else{
	//	setStatus(jMOVEWAIT, false);
	//}
	return false;
}

WX_DECLARE_STRING_HASH_MAP( wxString, StringHashMap );

StringMap::StringMap(){
	m_data = new StringHashMap();
}
StringMap::~StringMap(){
	delete m_data;
}
void StringMap::set(wxString key, wxString val){
	(*m_data)[key]=val;
}
wxString StringMap::get(wxString key){
	//wxString s = (*m_data)[key];
	StringHashMap::iterator it = m_data->find(key);
	if(it==m_data->end()) return key;
	return it->second;
}
wxString StringMap::toString(void){
	wxString data;
	StringHashMap::iterator it;

	for( it = m_data->begin(); it != m_data->end(); ++it )
    {
        wxString key = it->first, value = it->second;
		data.Append(key);
		data.Append(wxT(": "));
		data.Append(value);
		data.Append(wxT("\n"));
        // do something useful with key and value
    }
	return data;
}
wxString getVersion(int flags){
	switch(flags&0x3C){
			case 4: return wxT("v2");
			case 8: return wxT("v3");
			case 16: return wxT("v4");
			case 32: return wxT("v5");
			default: return wxT("");
		}
}
wxString getCensored(int stt){
	switch(stt&0xC0){
		case 64: return wxT("unc");
		case 128: return wxT("cen");
		default: return wxT("");
	}
}
wxString getInvalid(int stt){
	if((stt&2)==2)
		return wxT("invalid crc");
	return wxT("");
}
void AmeJob::makeMap(StringMap *sm){
	AL();
	sm->set(wxT("fil"), getFileName());
	sm->set(wxT("pat"), getFolderPath());
	sm->set(wxT("new"), getNewFilePath());
	sm->set(wxT("ori"), m_ori);
	sm->set(wxT("siz"), SPD(m_size));
	wxString stat = getStatusText();
	if(getStatus()==jFAILED&&m_err!=wxEmptyString) stat += wxT(". ")+m_err;
	sm->set(wxT("sta"), stat);

	if(m_fa!=NULL){
		sm->set(wxT("aid"), SPD(m_fa->m_aid));
		sm->set(wxT("eid"), SPD(m_fa->m_eid));
		sm->set(wxT("fid"), SPD(m_fa->m_fid));
		sm->set(wxT("gid"), SPD(m_fa->m_gid));
		sm->set(wxT("lid"), SPD(m_fa->m_lid));
		sm->set(wxT("ula"), m_fa->urlAnime());
		sm->set(wxT("ule"), m_fa->urlEpisode());
		sm->set(wxT("ulf"), m_fa->urlFile());
		sm->set(wxT("ulg"), m_fa->urlGroup());
		sm->set(wxT("ulx"), m_fa->urlMylistX());
		sm->set(wxT("ulm"), m_fa->urlMylist());
		sm->set(wxT("uly"), m_fa->urlYear());
		sm->set(wxT("ed2"), m_fa->m_ed2.Lower());
		sm->set(wxT("ED2"), m_fa->m_ed2.Upper());
		
		sm->set(wxT("inv"), getInvalid(m_fa->m_stt));
		sm->set(wxT("ver"), getVersion(m_fa->m_stt));
		sm->set(wxT("cen"), getCensored(m_fa->m_stt));
		
		sm->set(wxT("dub"), m_fa->m_dub);
		sm->set(wxT("sub"), m_fa->m_sub);
		sm->set(wxT("src"), m_fa->m_rip);
		sm->set(wxT("res"), m_fa->m_res);
		sm->set(wxT("vid"), m_fa->m_vid);
		sm->set(wxT("aud"), m_fa->m_aud);
		sm->set(wxT("qua"), m_fa->m_qua);

		if(m_fa->m_sha!=wxEmptyString){
			sm->set(wxT("sha"), m_fa->m_sha.Lower());
			sm->set(wxT("SHA"), m_fa->m_sha.Upper());
		}
		if(m_fa->m_md5!=wxEmptyString){
			sm->set(wxT("md5"), m_fa->m_md5.Lower());
			sm->set(wxT("MD5"), m_fa->m_md5.Upper());
		}
		if(m_fa->m_crc!=wxEmptyString){
			sm->set(wxT("CRC"), m_fa->m_crc.Upper());
			sm->set(wxT("crc"), m_fa->m_crc.Lower());
		}
		if(m_fa->m_anime!=NULL){
			sm->set(wxT("eps"), SPD(m_fa->m_anime->eps));
			sm->set(wxT("lep"),SPD(m_fa->m_anime->lep));
			sm->set(wxT("yea"),SPD(m_fa->m_anime->yea));
			sm->set(wxT("ann"), m_fa->m_anime->ann);
			sm->set(wxT("kan"), m_fa->m_anime->kan);
			sm->set(wxT("eng"), m_fa->m_anime->eng);
			sm->set(wxT("typ"),m_fa->m_anime->typ);
			sm->set(wxT("gen"), m_fa->m_anime->cat);
		}
		if(m_fa->m_episode!=NULL){
			sm->set(wxT("epn"), m_fa->m_episode->epn);
			sm->set(wxT("epk"), m_fa->m_episode->epk);
			sm->set(wxT("epr"), m_fa->m_episode->epr);
			sm->set(wxT("enr"), m_fa->m_episode->enr);
		}
		/*if(m_fa->m_anime!=NULL&&m_fa->m_episode!=NULL){
			sm->set(wxT("enr"), Parser.pad(m_fa->m_episode->num, m_fa->anime.getTotal()));
		}*/
		if(m_fa->m_group!=NULL&&m_fa->m_gid>0){
			sm->set(wxT("grp"), m_fa->m_group->grp);
			sm->set(wxT("grn"), m_fa->m_group->grn);
		}
		else{
			sm->set(wxT("grp"), wxT("unknown"));
			sm->set(wxT("grn"), wxT("unknown"));
		}
	}else if(m_ed2!=wxEmptyString){
		sm->set(wxT("ed2"), m_ed2.Lower());
		sm->set(wxT("ED2"), m_ed2.Upper());
	}
}

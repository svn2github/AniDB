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
#include <wx/filename.h>

#include "ame-id.h"
#include "ame-job.h"
#include "ame-db.h"
#include "anidb-base.h"
#include "ame-rules.h"

bool AmeJobMan::makeDirs(wxFileName f){
	wxFileName p = f.GetPath();
	if(f.DirExists())
		return true;
	if(!p.DirExists() && !makeDirs(p))
		return false;
	return f.Mkdir();
}
bool removeDirs(wxFileName f){
	if(f.DirExists() && f.Rmdir())
		return removeDirs(f.GetPath());
	return true;
}
bool AmeJobMan::removeFile(wxString path){
	if(wxRemoveFile(path)){
		wxFileName fn(path);
		return removeDirs(fn.GetPath());
	}
	POSTM(MSG_DEB, SP1("! Failed to remove '%s'. (wxRemoveFile)", path));
	return false;
}
bool AmeJobMan::add_to_mylist=false;

bool AmeJobMan::updatePath(AmeJob *j){
	return updatePath(j, wxGetApp().m_rules->apply(j));
}

void AmeJobMan::restoreName(AmeJob *job){
	setName(job, job->m_ori);
}
void AmeJobMan::setName(AmeJob *job, wxString &s){
	//synchronized(job){
		if(job->check(jS_DOING)) return;
		if(job->getHealth()<jH_DELETED && s.Length()>0){
			//File f = new File(job.m_fc.getParent()+File.separatorChar+s);
			//wxFileName f(s);
			//s = j->getParentFolder()+SEP+s.GetFullName();
			//if(
			wxString path = job->getFolderPath()+wxT(FILE_SEPARATOR)+s;
			if(job->check(jH_MISSING))
				job->find(path);
			else job->rename(path);
			A.m_db->update(dJ, 1, job);
			//	JobMan.setJobFile(job, f);
			//updateStatus(job, jFINISHED);
		}
	//}
}
void AmeJobMan::setPath(AmeJob *job, wxString &path, bool parent){
	//synchronized(job){
		if(job->check(jS_DOING)) return;
		//int status = jFAILED;
		
		wxFileName orig(job->getNewFilePath().Length()>0?job->getNewFilePath():job->getAbsolutePath());
		wxString name;
		if(parent){
			wxFileName pp = orig.GetPath();
			//pp = pp.GetPath();
			name += pp.GetFullName()+wxT(FILE_SEPARATOR);
		}
		name += orig.GetFullName();
		wxString file = path+wxT(FILE_SEPARATOR)+name;
		
		if(job->getHealth()==jH_MISSING){
			job->find(file);
			A.m_db->update(dJ, 1, job);
		} else if(updatePath(job, file)){
			if(job->getNewFilePath().Length()>0)
				job->setStatus(jMOVEWAIT, true);
			//	status = jMOVEWAIT;
			//else
			//	status = jFINISHED;
			A.m_db->update(dJ, 1, job);
		}
	//}
}
bool AmeJobMan::updatePath(AmeJob *job, wxString sf1){//only want to use renameTo if same partition
	job->m_fn = wxEmptyString;
	wxString sf0 = job->getAbsolutePath();
	if(sf1==wxEmptyString || sf0==sf1) return true;
//RULES ENABLED?
//	if(AmeRules.mImod==AmeRules.M_NOREN) return true;
	//POPT("SRC FILE HEALTH");
	
	
//SOURCE FILE HEALTHY?
	if(!wxFile::Exists(sf0)){
		job->setError(wxT("File does not exist."));
		//A.gui.println(sf0+" cannot be moved. File not found.");
		return false;
	}
	//POPT("DST FILE HEALTH");

	//POPT("updatePath3");
//DESTINATION FILE HEALTHY?
	if(wxFile::Exists(sf1)){
		wxFile f(sf1);
		if(job->m_size==f.Length()){ //could be the same
			job->setNewFilePath(sf1);
			//A.gui.println(sf0+" will be moved to "+sf1+" later.");
			return true;
		}
		job->setError(wxT("File cannot be moved. Destination file already exists!"));
		//A.gui.println(sf0+" cannot be moved to "+sf1+": Destination file already exists!");
		return false;
	}
	//POPT("SRC FOLDER HEALTH");

//DESTINATION FOLDER OK?
	wxFileName fn(sf1);
	wxFileName parent = fn.GetPath();
	if(!parent.DirExists() && !makeDirs(parent)){
		job->setError(wxT("Folder")+parent.GetFullPath()+wxT(" cannot be created!"));
		//A.gui.println("Folder"+parent+" cannot be created!");
		return false;
	}
	wxGetApp().m_jl->addPath(sf1);
	//job.mIdid = -1;
//TRY TO MOVE: WINDOWS
	//POPT("TRY MOVE");

	wxString p0 = sf0.Lower();
	wxString p1 = sf1.Lower();
	wxString sok = wxT("Cleanup after successful rename operation.");
	if(p0.at(1)==':'||p1.at(1)==':'){ //windows (not network)
		if(p0.at(0)==p1.at(0)){
			//POPT("NOW MOVE");
			if(job->rename(sf1)){
				//moveSub(job.m_fc, f);
				//A.deleteFile(job.m_fc.getParentFile(), sok);
				//AmeJobMan::setJobFile(job, f);
				//A.gui.println("Renamed "+sf0+" to "+sf1);
				POSTM(MSG_LOG, SP2("Renamed %s to %s", sf0, sf1));
				//AmeJobMan::removeFile(sf0);
				return true;
			}
			//A.gui.println(Hyper.error("Renaming failed!")+" ("+sf0+" to "+sf1+")");
			return false;
		}
		job->setNewFilePath(sf1);
		//A.gui.println(sf0+" will be moved to "+sf1+" later.");
		return true;
	}
	POPT("NIX");
//TRY TO MOVE: *NIX
	if(job->rename(sf1)){ //linux can't rename over partitions
		//A.deleteFile(job.m_fc.getParentFile(), sok);
		//AmeJobMan.setJobFile(job, f);
		//A.gui.println("Renamed"+sf0+" to "+sf1);
		return true;
	}
	job->setNewFilePath(sf0);
	//A.gui.println(sf0+" will be moved to "+sf1+" later.");
	return true;
//THE END
}
void AmeJobMan::updateStatus(AmeJob *job, int status, bool chck){
	//Sync on job?
	if(chck&&job->check(jS_DOING)) return;
	int idx = job->getID();
	if((status&jM_H)>0){
		job->updateHealth(status);
		POSTR(idx);
		return;
	}
	if(	job->isLocked(status) )
		return;
	/*if((status&0xf00)!=0){
		job->updateHealth(status);
		return;
	}
	if(	 job->getHealth()>jH_PAUSED && //we don't wanna mess with these jobs
		(job->getHealth()==jH_DELETED || status<jREMWAIT)) //unless
		return;*/
	int j = -1;
	switch(status){
		case jHASHED:
			if(job->m_ed2.Length()!=32) return;
			if(job->getAdbFile()==NULL)
				j = jIDENTWAIT;
			else j = jFINISHED;
			if(job->m_new && A.m_db->ok()){
				//if(!A.m_db->update(dJ, 1, job)){ //add job to db, if it exist...
					j = A.m_db->getJob(job, true); //then retrieve
					if(j>=0){ //if job exist in db... (should be true, always)
						//A.cache.gatherInfo(job,true); //retrieve info (like anime, group, etc.)
						//File old = job.nfile; //copy the original location (little abuse of nfile)
						job->m_fn = wxEmptyString; //overwrite path..
						//if(j==AmeJob.FINISHED&&old!=null&&old.exists()){ //presume old location is right
						//	j = AmeJob.MOVEWAIT; //move (or rather check and delete)
						//	job.nfile = old; //move function use nfile for this
						//}else{ //the original file was moved, or the job never finished
							if(updatePath(job)){ //apply rules
								if(job->getNewFilePath()!=wxEmptyString){ //check for move
									j = jMOVEWAIT; //we want to move to a new location, is it the same as the orig?
									//if(old!=null&&!job.nfile.equals(old)&&job.getFile().exists()) //if not then delete orig
									//	A.deleteFileAndFolder(old, "Dupe of "+job.getFile());
								}else{ //the file was renamed (same hd partition)
									j = jFINISHED;
									//if(old!=null&&!job.getFile().equals(old)&&job.getFile().exists()) //delete orig file if it exist and does not equal the new
									//	A.deleteFileAndFolder(old, "Dupe of "+job.getFile());
								}
							}else j = jFAILED;
						//}
						//old = null;
					}else{ //insert
						if(A.m_db->update(dJ, 1, job)){
							if(job->getAdbFile()==NULL)
								j = jIDENTWAIT;
							else j = jFINISHED;
						}else{
							j = jFAILED;
							job->setError(wxT("Database error. See log/debug."));
						}
					}
				//}
				job->m_new = false;
			}
			break;
		case jIDENTIFIED:{
				AdbFile *f = job->getAdbFile();
				if(f==NULL||f->m_fid==0||!updatePath(job))
					j = jFAILED;
				else{					
					if(add_to_mylist && f->m_lid==0)
						j = jADDWAIT;
					else if(job->m_fn.Length()>0)
						j = jMOVEWAIT;
					else j = jFINISHED;
					//Update database
					//TODO not every time for anime/group ?
					A.m_db->update(dF, f->m_fid, f);
					if(f->m_anime->src==2)
						A.m_db->update(dA, f->m_aid, f->m_anime);
					if(f->m_episode->src==2)
						A.m_db->update(dE, f->m_eid, f->m_episode);
					if(f->m_group && f->m_group->src==2)
						A.m_db->update(dG, f->m_gid, f->m_group);
				}
			}break;
		case jADDED:
			if(job->m_fn.Length()>0)
				j = jMOVEWAIT;
			else j = jFINISHED;
			break;
		case jMOVED:
			j = jFINISHED;
			break;
		case jFAILED:
			j = jFAILED;
			break;
		case jADDWAIT:
			if(job->m_fa==NULL){
				j = jFAILED;
				job->setError(wxT("Can't add unknown file to mylist."));
				break;
			}
		case jFINISHED:
			//if(job.afl==null||job.afl.fid==0) return; //is it ok to disable this?
			job->m_fn = wxT("");
			j = status;
			break;
		default:
			j = status;
	}
	job->setStatus(j, true);
	//if(!job.mBf && (j&0x0f)==0)
	//	A.db.update(0, job, DB.I_J);
	if(!job->m_new && job->check(jF_DB))
		A.m_db->update(dJ, 1, job);
	POSTR(idx);
}

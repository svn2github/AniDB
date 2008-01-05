// Copyright (C) 2005-2006 epoximator
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

/*
 * Created on 24.10.05
 *
 * @version 	01 (1.14)
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;

import epox.swing.Log;
import epox.util.Hyper;
import epox.util.U;

public class JobMan{
	public static Log log;
	public static void setPath(Job job, String path, boolean parent){
		synchronized(job){
			if(job.check(Job.S_DOING)) return;
			int status = Job.FAILED;
			File folder = new File(path);
			File orig = job.m_fn!=null?job.m_fn:job.m_fc;
			String name = "";
			if(parent) name += orig.getParentFile().getName()+File.separatorChar;
			name += orig.getName();
			File file = new File(folder+File.separator+name);
			if(job.getHealth()==Job.H_MISSING){
				job.find(file);
				A.db.update(0, job, DB.I_J);
			} else if(updatePath(job, file)){
				if(job.m_fn!=null)
					status = Job.MOVEWAIT;
				else status = Job.FINISHED;
				job.setStatus(status, true);
				A.db.update(0, job, DB.I_J);
			}
		}
	}
	public static void setName(Job job, String s){
		synchronized(job){
			if(job.check(Job.S_DOING)) return;
			if(job.getHealth()<Job.H_DELETED&&job.getStatus()==Job.FINISHED
					&&s!=null&&s.length()>0){
				File f = new File(job.m_fc.getParent()+File.separatorChar+s);
				if(job.m_fc.renameTo(f))
					JobMan.setJobFile(job, f);
				updateStatus(job, Job.FINISHED);
			}
		}
	}
	public static void restoreName(Job job){
		setName(job, job.mSo);
	}
	public static void c_watch(Job job){
		if(!job.getFile().exists()) return;
		Runtime rt = Runtime.getRuntime();
		try{
			rt.exec("rundll32 url.dll,FileProtocolHandler \""+job.getFile()+"\"");
		}catch(java.io.IOException f){f.printStackTrace();}
	}
	public static void c_expl(Job job){
		if(!job.getFile().exists()) return;
		Runtime rt = Runtime.getRuntime();
		try{
			rt.exec("explorer \""+job.getFile().getParent()+"\"");
		}catch(java.io.IOException f){f.printStackTrace();}
	}
	public static void c_avdump(Job job){
		if(!job.getFile().exists()) return;
		Runtime rt = Runtime.getRuntime();
		try{
			rt.exec("cmd /C start avdump -ps \""+job.getFile().getAbsolutePath()+"\"");
		}catch(java.io.IOException f){f.printStackTrace();}
	}
	public static void updateStatus(Job job, int status){
		updateStatus(job, status, false);
	}
	public static void updateStatus(Job job, int status, boolean chck){
		synchronized(job){
			if(chck&&job.check(Job.S_DOING)) return;
			if((status&Job.M_H)>0){
				job.updateHealth(status);
				return;
			}
			if(	job.isLocked(status) )//we don't wanna mess with all types
				return;
			int j = -1;
			switch(status){
			case Job.HASHED:
				if(job._ed2==null) return;
				if(job.m_fa==null)
					j = Job.IDENTWAIT;
				else j = Job.FINISHED;
				if(job.mBf){
					if(A.db._ok()&&!A.db.update(0, job, DB.I_J)){ //add job to db, if it exist...
						j = A.db.getJob(job, true); //then retrieve
						if(j>=0){ //if job exist in db... (should be true, always)
							A.cache.gatherInfo(job,true); //retrieve info (like anime, group, etc.)
							//File old = job.nfile; //copy the original location (little abuse of nfile)
							job.m_fn = null;
							//if(j==Job.FINISHED&&old!=null&&old.exists()){ //presume old location is right
							//	j = Job.MOVEWAIT; //move (or rather check and delete)
							//	job.nfile = old; //move function use nfile for this
							//}else{ //the original file was moved, or the job never finished
							if(updatePath(job)){ //apply rules
								if(job.m_fn!=null)
									j = Job.MOVEWAIT; //we want to move to a new location, is it the same as the orig?
									//if(old!=null&&!job.nfile.equals(old)&&job.getFile().exists()) //if not then delete orig
									//	A.deleteFileAndFolder(old, "Dupe of "+job.getFile());
								else
									j = Job.FINISHED;
									//if(old!=null&&!job.getFile().equals(old)&&job.getFile().exists()) //delete orig file if it exist and does not equal the new
									//	A.deleteFileAndFolder(old, "Dupe of "+job.getFile());
							}else j = Job.FAILED;
							//}
							//old = null;
						}else{
							j = Job.FAILED;
							job.setError("Database error. See log/debug.");
						}
					}
					job.mBf = false;
				}
				break;
			case Job.IDENTIFIED:
				if(job.m_fa==null||job.m_fa.fid==0||!updatePath(job))
					j = Job.FAILED;
				else if(A.autoadd&&job.mIlid==0)
					j = Job.ADDWAIT;
				else if(job.m_fn!=null)
					j = Job.MOVEWAIT;
				else j = Job.FINISHED;
				break;
			case Job.ADDED:
				if(job.m_fa==null||job.m_fa.fid==0) return;
				if(job.m_fn!=null)
					j = Job.MOVEWAIT;
				else j = Job.FINISHED;
				//A.db.addMylistEntry(this);
				break;
			case Job.MOVED:
				j = Job.FINISHED;
				break;
			case Job.FAILED:
				j = Job.FAILED;
				break;
			case Job.ADDWAIT:
				if(job.m_fa==null){
					j = Job.FAILED;
					job.setError("Can't add unknown file to mylist.");
					break;
				}
			case Job.FINISHED:
				//if(job.afl==null||job.afl.fid==0) return; //is it ok to disable this?
				job.m_fn = null;
				j = status;
				break;
			default:
				j = status;
			}
			job.setStatus(j, true);
			if(!job.mBf && job.check(Job.F_DB))
				A.db.update(0, job, DB.I_J);
		}
	}
	public static boolean updatePath(Job job){
		if(job.incompl()){
			job.setError("Extensive fileinfo not available.");
			log.add(job.m_fc+" cannot be renamed: Extensive fileinfo not available.");
			return false;
		}
		return updatePath(job, A.rules.apply(job));
	}
	public static int rcnt = 0;
	public static boolean updatePath(Job job, File f){//only want to use renameTo if same partition
		job.m_fn = null;
		if(f==null) return true;
//EQUAL
		boolean normal = true;
		if(job.m_fc.equals(f)){
			if(job.m_fc.getName().equals(f.getName()))//win case sens
				return true;
			normal = false;
		}
		String sf0 = Hyper.name(job.m_fc);
		String sf1 = Hyper.name(f);
//SOURCE FILE HEALTHY?
		if(!job.m_fc.exists()){
			job.setError("File does not exist.");
			log.add(sf0+" cannot be moved. File not found.");
			return false;
		}
//DESTINATION FILE HEALTHY?
		if(normal && f.exists()){
			if(job.m_fc.length()==f.length()){ //could be the same
				job.m_fn = f;
				log.add(sf0+" will be moved to "+sf1+" later.");
				return true;
			}
			job.setError("File cannot be moved. Destination file already exists!");
			log.add(sf0+" cannot be moved to "+sf1+": Destination file already exists!");
			return false;
		}
//DESTINATION FOLDER OK?
		File parent = f.getParentFile();
		if(!parent.exists() && !parent.mkdirs()){
			job.setError("Folder "+parent+" cannot be created!");
			log.add("Folder "+parent+" cannot be created!");
			return false;
		}
		A.jobs.addPath(f);
		job.mIdid = -1;
//TRY TO MOVE: WINDOWS
		String p0 = job.m_fc.getAbsolutePath().toLowerCase();
		String p1 = f.getAbsolutePath().toLowerCase();
		String sok = "Cleanup after successful rename operation.";
		if(p0.charAt(1)==':'||p1.charAt(1)==':'){ //windows (not network)
			if(p0.charAt(0)==p1.charAt(0)){
				if(job.m_fc.renameTo(f)){
					moveSub(job.m_fc, f);
					U.deleteFile(job.m_fc.getParentFile(), sok);
					JobMan.setJobFile(job, f);
					log.add("Renamed "+sf0+" to "+sf1);
					return true;
				}
				log.add(Hyper.error("Renaming failed!")+" ("+sf0+" to "+sf1+")");
				return false;
			}
			job.m_fn = f;
			log.add(sf0+" will be moved to "+sf1+" later.");
			return true;
		}
//TRY TO MOVE: *NIX
		if(job.m_fc.renameTo(f)){ //linux can't rename over partitions
			U.deleteFile(job.m_fc.getParentFile(), sok);
			JobMan.setJobFile(job, f);
			log.add("Renamed"+sf0+" to "+sf1);
			return true;
		}
		job.m_fn = f;
		log.add(sf0+" will be moved to "+sf1+" later.");
		return true;
//THE END
	}
	public static void setJobFile(Job j, File f){
		if(Cache.mImode==Cache.I_MAFF){
			A.cache.treeRemove(j);
			j.m_fc = f;
			A.cache.treeAdd(j);
		}else j.m_fc = f;
	}
	public static void showInfo(Job job){
		//new epox.webaom.ui.JFrameHtml((job.m_fc.getName(), job.convert(U.fileToString("d:\\java\\webaom\\src\\file.htm")));
		new epox.webaom.ui.JFrameHtml(job.m_fc.getName(), job.convert(A.fschema));
	}
	private static void moveSub(File a, File b){
		moveSub(a,b,"srt");
		moveSub(a,b,"ass");
		moveSub(a,b,"idx");
		moveSub(a,b,"sub");
		moveSub(a,b,"pdf");
		moveSub(a,b,"ssa");
	}
	private static void moveSub(File a, File b, String ext){
		File c = new File(changeExt(a, ext));
		if(c.exists())
			c.renameTo(new File(changeExt(b, ext)));
	}
	private static String changeExt(File f, String ext){
		String path = f.getAbsolutePath();
		return path.substring(0, 1+path.lastIndexOf('.'))+ext;
	}
}
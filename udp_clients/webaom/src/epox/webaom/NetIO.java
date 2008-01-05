//Copyright (C) 2005-2006 epoximator

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

/*
 * Created on 29.05.05
 *
 * @version 	06
 * @author 		epoximator
 */
package epox.webaom;

import epox.swing.Log;
import epox.util.Hyper;
import epox.webaom.Cache;
import epox.webaom.DB;
import epox.webaom.Job;
import epox.webaom.JobMan;
import epox.webaom.data.Anime;
import epox.webaom.data.Ep;
import epox.webaom.data.Group;
import epox.webaom.net.ACon;
import epox.webaom.net.AConE;
import epox.webaom.net.AConEx;
import epox.webaom.ui.JDialogLogin;

public class NetIO extends Service {
	private static final String S_TERM = "NetIO thread terminated.";

	public AConE conn = null;
	public AConE getConnection(){
		return conn;
	}

	private Job m_job;
	private Cache cache;

	public void setCache(Cache c){
		cache = c;
	}
	public boolean setEnabled(boolean enable){
		if(enable){
			if(!enabled && thread==null){
				if(A.up.ses==null && new JDialogLogin().getPass()==null)
					return false;
				thread = new Thread(this, "NetIO");
				thread.start();
				enabled = true;
			}
		}else if(enabled){
			enabled = false;
		}
		return enabled;
	}
	public void kill(){
		ACon.shutdown = true;
		setEnabled(false);
		if(thread!=null&&conn!=null) try {
			conn.disconnect();
		} catch (Exception e){
			e.printStackTrace();
		}
	}
	public void run(){
		log.add(Log.STN, "Checking connection...");
		boolean btimeout = false;
		AConE ac = A.getConnectionData();
		if(ac.connect()){
			if(ping(ac)){
				conn = ac;
				try{
					do_work();
					log.add(Log.LOG|Log.STN, S_TERM);
				}catch(AConEx e){ e.printStackTrace();
				String message = e.getMessage();
				btimeout = message!=null && message.indexOf("TIME OUT")>=0;
				log.add(Log.MSG|Log.STN, " "+((message==null)?"Null pointer exception.":message));
				if(!e.is(AConEx.ENCRYPTION)){
					A.stop();
				}
				cleanCurrentJob(e.getMessage());
				}catch(Exception e){
					e.printStackTrace();
				log.add(Log.LOG|Log.MSG, " "+Hyper.error(e.getMessage()));
				A.stop();
				cleanCurrentJob(e.getMessage());
				}
				conn.disconnect();
				conn = null;
			}else{
				ac.disconnect();
				ac = null;
				log.add(Log.STN, "Sleeping...");
				try{
					Thread.sleep(100);
				}catch(Exception e){/*don't care*/}
				log.add(Log.STN, S_TERM);
			}
			try{
				if(ac!=null && ac.isLoggedIn() && !btimeout && ac.logout())
					log.add("Logged out after extra check!");
			}catch(Exception e){ e.printStackTrace(); }
		}else{
			String s = ac.getLastError();
			if(s.endsWith("Cannot bind")) s ="The local port is already in use. Try another port.";
			log.add(Log.LOG|Log.MSG, Hyper.error(s));
		}
		thread = null;
		status.finished();
	}
	private void cleanCurrentJob(String err){
		if(m_job!=null){
			m_job.mSe = err;
			JobMan.updateStatus(m_job, Job.FAILED);
			m_job = null;
		}
	}
	private void do_work() throws AConEx, InterruptedException{
		log.add(Log.STN, "Authenticating...");
		if(conn.login()){
			status.started();
			do{
				m_job = A.jobs.getJobNio();
				if(m_job!=null){
					if(m_job.getStatus()==Job.REMWAIT)
						remove(m_job);
					else{
						if(m_job.getStatus()==Job.IDENTWAIT)
							identify(m_job);
						if(isEnabled() && m_job.getStatus()==Job.ADDWAIT)
							mylistAdd(m_job);
					}
				}else{
					log.add(Log.STN, "NetIO thread idle");
					Thread.sleep(500);
				}
			}while(isEnabled());
			log.add(Log.STN, "Disconnecting...");
			conn.logout();
		}
	}
	private void remove(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.REMING);
		log.add(Log.STN, "Removing from mylist: "+j.getFile());
		if(j.mIlid>0){
			if(conn.removeFromMylist(j.mIlid, j.getFile().getName())){
				j.mIlid = 0;
				log.add("Removed "+Hyper.name(j.getFile()));
				JobMan.updateStatus(j, Job.FINISHED);
				return;
			}
			log.add(Hyper.error("Could not remove: "+j.getFile()));
		}else log.add(Hyper.error("Not in mylist: "+j.getFile()));
		j.setError("Was not in mylist");
		JobMan.updateStatus(j, Job.FAILED);
	}
	private void identify(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.IDENTIFYING);
		log.add(Log.STN, "Retrieving file data for "+j.getFile().getName());
		if(j.m_fa==null){
			String[] s = null;
			if(j.mIfid>0)
				s = conn.retrieveFileData(j.mIfid, j.getFile().getName());
			else
				s = conn.retrieveFileData(j.mLs, j._ed2, j.getFile().getName());
			if(s!=null && cache.parseFile(s, j)!=null){
				j.mIlid = j.m_fa.lid;
				j.m_fa.setJob(j);
				log.add("Found "+Hyper.name(j.m_fa.def)+" "+
						Hyper.href(j.m_fa.urlAnime(), "a")+" "+
						Hyper.href(j.m_fa.urlEp(), "e")+" "+
						Hyper.href(j.m_fa.urlFile(), "f"));
				JobMan.updateStatus(j, Job.IDENTIFIED);
			}else
				JobMan.updateStatus(j, Job.UNKNOWN);
		}else{
			if(j.m_fa.group==null)
				j.m_fa.group = (Group)cache.get(j.m_fa.gid, DB.I_G);
			if(j.m_fa.ep==null)
				j.m_fa.ep = (Ep)cache.get(j.m_fa.eid, DB.I_E);
			if(j.m_fa.group==null)
				j.m_fa.anime = (Anime)cache.get(j.m_fa.aid, DB.I_A);
			JobMan.updateStatus(j, Job.IDENTIFIED);
		}
	}
	private void mylistAdd(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.ADDING);
		log.add(Log.STN, "Adding "+j.getFile()+" to your list...");
		int i;
		if((i=conn.addFileToMylist(j, A.getMylistData()))>0){
			j.mIlid = i;
			log.add("Added "+Hyper.name(j.getFile())+" to mylist");
		}
		JobMan.updateStatus(j, Job.ADDED);
	}
	public boolean ping(ACon ac){
		try{
			log.add("AniDB is reachable. Received reply in "+
					Hyper.number(""+ac.enCrypt())+" ms.");
			return true;
		}catch(java.net.SocketTimeoutException e){
			String str = "AniDB is not reachable.";
			log.add(Log.LOG|Log.MSG|Log.STN, Hyper.error(str));
		}catch(NumberFormatException e){
			log.add(Log.MSG, "Invalid number. "+e.getMessage());
		}catch(Exception e){
			e.printStackTrace();
			log.add(Log.LOG|Log.MSG, Hyper.error(e.getMessage()));
		}
		log.add("Check out the connection options or try again later.");
		return false;
	}
}
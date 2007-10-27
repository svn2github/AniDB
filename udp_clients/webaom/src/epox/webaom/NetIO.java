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
 * Created on 29.05.05
 *
 * @version 	06
 * @author 		epoximator
 */
package epox.webaom;

import epox.webaom.data.Anime;
import epox.webaom.data.Ep;
import epox.webaom.data.Group;
import epox.webaom.net.ACon;
import epox.webaom.net.AConE;
import epox.webaom.net.AConEx;

public class NetIO implements Runnable {
	private static final String S_TERM = "NetIO thread terminated.";

	private Job m_job;

	public void run(){
		A.gui.status1("Checking connection...");
		boolean btimeout = false;
		AConE ac = A.gui.getConnection();
		if(ac.connect()){
			if(ping(ac)){
				A.conn = ac;
				try{
					do_work();
					A.gui.println(S_TERM);
					A.gui.status1(S_TERM);
				}catch(AConEx e){ e.printStackTrace();
					String message = e.getMessage();
					btimeout = message!=null && message.indexOf("TIME OUT")>=0;
					A.gui.status1(message);
					A.gui.msg(" "+((message==null)?"Null pointer exception.":message));
					if(!e.is(AConEx.ENCRYPTION)){
						A.gui.kill();
						A.gui.fatal(true);
					}//else A.up.key = null;
					cleanCurrentJob(e.getMessage());
				}catch(Exception e){ e.printStackTrace();
					A.gui.println(" "+Hyper.error(e.getMessage()));
					A.gui.msg(" "+e.getMessage());
					A.gui.kill();
					A.gui.fatal(true);
					cleanCurrentJob(e.getMessage());
				}
				A.conn.disconnect();
				A.conn = null;
			}else{
				ac.disconnect();
				ac = null;
				//web.setEnabled_conn(true);
				A.gui.status1("Sleeping...");
				try{
					Thread.sleep(100);
				}catch(Exception e){/*don't care*/}
				A.gui.status1(S_TERM);
			}
			try{
				if(ac!=null && ac.isLoggedIn() && !btimeout && ac.logout())
					A.gui.println("Logged out after extra check!");
			}catch(Exception e){ e.printStackTrace(); }
		}else{
			String s = ac.getLastError();
			if(s.endsWith("Cannot bind")) s ="The local port is already in use. Try another port.";
			A.gui.println(Hyper.error(s));
			A.gui.msg(s);
		}
		//!A.nr_nio = -1;
		A.gui.setEnabled_nio(true);
		A.gui.nioEnable(false);
		A.gui.mWnio=null;
	}
	private void cleanCurrentJob(String err){
		if(m_job!=null){
			m_job.mSe = err;
			JobMan.updateStatus(m_job, Job.FAILED);
			m_job = null;
		}
		//!A.nr_nio = -1;
	}
	private void do_work() throws AConEx, InterruptedException{
		A.gui.status1("Authenticating...");
		if(A.conn.login()){
			A.gui.nioEnable(true);
			do{
				m_job = A.jobs.getJobNio();
				if(m_job!=null){
					//!A.nr_nio = m_job.mIid;
					if(m_job.getStatus()==Job.REMWAIT)
						remove(m_job);
					else{
						if(m_job.getStatus()==Job.IDENTWAIT)
							identify(m_job);
						if(A.gui.nioOK()&&m_job.getStatus()==Job.ADDWAIT)
							mylistAdd(m_job);
					}
					//A.gui.updateJobTable(m_job);
					//!A.nr_nio = -1;
				}else{
					A.gui.status1("Idle");
					Thread.sleep(500);
				}
			}while(A.gui.nioOK());
			A.gui.status1("Disconnecting...");
			A.conn.logout();
		}
	}
	private void remove(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.REMING);
		//A.gui.updateJobTable(j);
		A.gui.status1("Removing from mylist: "+j.getFile());
		if(j.mIlid>0){
			if(A.conn.removeFromMylist(j.mIlid, j.getFile().getName())){
				j.mIlid = 0;
				A.gui.println("Removed "+Hyper.name(j.getFile()));
				JobMan.updateStatus(j, Job.FINISHED);
				return;
			}
			A.gui.println(Hyper.error("Could not remove: "+j.getFile()));
		}else A.gui.println(Hyper.error("Not in mylist: "+j.getFile()));
		j.setError("Was not in mylist");
		JobMan.updateStatus(j, Job.FAILED);
	}
	private void identify(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.IDENTIFYING);
		//A.gui.updateJobTable(j);
		A.gui.status1("Retrieving file data for "+j.getFile().getName());
		if(j.m_fa==null){
			String[] s = null;
			if(j.mIfid>0)
				s = A.conn.retrieveFileData(j.mIfid, j.getFile().getName());
			else
				s = A.conn.retrieveFileData(j.mLs, j._ed2, j.getFile().getName());
			if(s!=null && A.cache.parseFile(s, j)!=null){
				j.mIlid = j.m_fa.lid;
				j.m_fa.setJob(j);
				A.gui.println("Found "+Hyper.name(j.m_fa.def)+" "+
							Hyper.href(j.m_fa.urlAnime(), "a")+" "+
							Hyper.href(j.m_fa.urlEp(), "e")+" "+
							Hyper.href(j.m_fa.urlFile(), "f"));
				JobMan.updateStatus(j, Job.IDENTIFIED);
			}else
				JobMan.updateStatus(j, Job.UNKNOWN);
		}else{
			if(j.m_fa.group==null)
				j.m_fa.group = (Group)A.cache.get(j.m_fa.gid, DB.I_G);
			if(j.m_fa.ep==null)
				j.m_fa.ep = (Ep)A.cache.get(j.m_fa.eid, DB.I_E);
			if(j.m_fa.group==null)
				j.m_fa.anime = (Anime)A.cache.get(j.m_fa.aid, DB.I_A);
			JobMan.updateStatus(j, Job.IDENTIFIED);
		}
	}
	private void mylistAdd(Job j) throws AConEx{
		JobMan.updateStatus(j, Job.ADDING);
		//A.gui.updateJobTable(j);
		A.gui.status1("Adding "+j.getFile()+" to your list...");
		int i;
		if((i=A.conn.addFileToMylist(j, A.gui.jpOmyl.getMylistData()))>0){
			j.mIlid = i;
			A.gui.println("Added "+Hyper.name(j.getFile())+" to mylist");
		}
		JobMan.updateStatus(j, Job.ADDED);
	}
	public boolean ping(ACon ac){
		try{
			A.gui.println("AniDB is reachable. Received reply in "+
						Hyper.number(""+ac.enCrypt())+" ms.");
			return true;
		}catch(java.net.SocketTimeoutException e){
			String str = "AniDB is not reachable.";
			A.gui.println(Hyper.error(str));
			A.gui.status1(str);
			A.gui.msg(str);
		}catch(NumberFormatException e){
			A.gui.msg("Invalid number. "+e.getMessage());
		}catch(Exception e){ e.printStackTrace();
			A.gui.println(Hyper.error(e.getMessage()));
			A.gui.msg(e.getMessage());
		}
		A.gui.println("Check out the connection options or try again later.");
		return false;
	}
}
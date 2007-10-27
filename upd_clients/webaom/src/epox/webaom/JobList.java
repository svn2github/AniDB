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
 * Created on 02.08.05
 *
 * @version 	03 (1.14,1.10,1.09)
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;
import java.util.ArrayList;
import java.util.HashSet;

import epox.util.LinkedHash;
import epox.webaom.ui.TableModelJobs;

public class JobList{
	private ArrayList/*!<Job>*/ m_al;
	private Job[] m_fl = null;
	private HashSet/*!<String>*/ m_hs;
	private LinkedHash m_lh[];

	//private int mIid = 0;

	public TableModelJobs jlm = null;

	public JobList(){
		m_al = new ArrayList/*!<Job>*/();
		m_hs = new HashSet/*!<File>*/();

		m_lh = new LinkedHash[3];
		for(int i=0; i<m_lh.length; i++)
			m_lh[i] = new LinkedHash();
	}
	/*public void dumpHashSet(){
		try {
			Iterator it = m_al.iterator();
			FileWriter fw;

			fw = new FileWriter("C:\\fdump.txt");

			while (it.hasNext()) {
				fw.write(it.next()+"\n");
			}
			fw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}*/
	public String toString(){
		return "HashSet: "+m_hs.size()+", ArrayList: "+m_al.size()+", Array: "+(m_fl==null?-1:m_fl.length);
	}
	public synchronized void clear(){
		m_fl=null;
		m_al.clear();
		m_hs.clear();
		for(int i=0; i<m_lh.length; i++)
			m_lh[i].clear();
		//mIid = 0;
	}
	public synchronized boolean has(File f){
		return m_hs.contains(f);
	}
	public synchronized boolean addPath(File f){
		return m_hs.add(f);
	}
	public synchronized void filter(int status, int state, boolean unk){
		if(status==0){
			m_fl = null;
			return;
		}
		long t0 = System.currentTimeMillis();
		ArrayList/*!<Job>*/ al = new ArrayList/*!<Job>*/(m_al.size());
		Job j;
		for(int i = 0; i < m_al.size(); i++){
			j = (Job)m_al.get(i);
			if(j.checkSep(status, state, unk))
				al.add(j);
				//j.mIid = i;
		}
		m_fl = (Job[])al.toArray(new Job[0]);
		System.out.println("! Filter: "+(System.currentTimeMillis()-t0));
	}
	private void add_0(Job j){
		m_al.add(j);
		//j.mIid = mIid++;
		jlm.insertJob(m_al.size()-1);
	}
	public synchronized Job add(File f){
		if(m_hs.add(f)){//TODO if update then check against existing files
			Job j = new Job(f, Job.HASHWAIT);
			int status = A.db.getJob(j, false);
			if(status>=0&&j.m_fa!=null){
				A.cache.gatherInfo(j, true);
				j.setStatus(status, false);
			}
			add_0(j);
			return j;
		}
		return null;
	}
	public synchronized boolean add(Job j){
		if(m_hs.add(j.m_fc)){
			add_0(j);
			return true;
		}
		return false;
	}
	public synchronized Job get(int i){
		try{
			if(m_fl!=null) return m_fl[i];
			return (Job)m_al.get(i);
		}catch(ArrayIndexOutOfBoundsException e){
			//e.printStackTrace();
			System.err.println("[ ArrayIndexOutOfBoundsException "+i);
		}
		catch(IndexOutOfBoundsException e){
			e.printStackTrace();
		}
		return null;
	}
	public synchronized Job rem(int i){
		Job j = (Job)m_al.remove(i);
		m_hs.remove(j.getFile());
		return j;
	}
	public synchronized Job[] array(){
		Job[] a = new Job[m_al.size()];
		m_al.toArray(a);
		return a;
	}
	public synchronized int size(){
		if(m_fl!=null) return m_fl.length;
		return m_al.size();
	}
	public Job getJobDio(){
		return (Job)m_lh[DIO].getFirst();
	}
	public Job getJobNio(){
		return (Job)m_lh[NIO].getFirst();
	}
	public boolean workForDio(){
		return m_lh[DIO].size()>0;
	}
	public boolean workForNio(){
		return m_lh[NIO].size()>0;
	}
	public void updateQueues(Job j, int os, int ns){
		synchronized(m_lh){
			updateHashSets(j, os, false); //remove from set
			updateHashSets(j, ns, true);  //add to set
		}
	}
	private void updateHashSets(Job j, int status, boolean add){
		if(status<0) return;
		int type = -1;
		if(A.bitcmp(status,Job.S_DO)||A.bitcmp(status,Job.S_DOING)){
			if(A.bitcmp(status,Job.D_DIO))
				type = DIO;
			else if(A.bitcmp(status,Job.D_NIO))
				type = NIO;
			else return;
		} else if(A.bitcmp(status,Job.FAILED)||A.bitcmp(status,Job.UNKNOWN))
			type = I_ERR;
		else return;
		if(add) m_lh[type].addLast(j);
		else m_lh[type].remove(j);
	}
	public static final int I_ERR = 0, DIO = 1, NIO = 2;
}
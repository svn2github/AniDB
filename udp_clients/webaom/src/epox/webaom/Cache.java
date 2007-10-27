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
 * Created on 30.07.05
 *
 * @version 	1.09
 * @author 		epoximator
 */

package epox.webaom;

import java.util.HashMap;

import epox.util.U;
import epox.webaom.data.AFile;
import epox.webaom.data.AG;
import epox.webaom.data.Anime;
import epox.webaom.data.Base;
import epox.webaom.data.Ep;
import epox.webaom.data.Group;
import epox.webaom.data.Path;

public class Cache{
	protected class MyMap extends HashMap/*!<Integer,Base>*/{
		//
	}
	private MyMap m_hm[];

	public Cache(){
		m_hm = new MyMap[3];
		for(int i=0; i<m_hm.length; i++)
			m_hm[i] = new MyMap();
	}
	public String toString(){
		return "Anime: "+m_hm[DB.I_A].size()+", Episode: "+m_hm[DB.I_E].size()+", Group: "+m_hm[DB.I_A].size();
	}
	public void clear(){
		for(int i=0; i<m_hm.length; i++)
			m_hm[i].clear();
	}
	public void add(Base b, int db, int typ){
		Integer id = new Integer(b.id);
		if(!m_hm[typ].containsKey(id)){
			m_hm[typ].put(id,b);
			if(db==1)A.db.update(b.id, b, typ);
		}
		if(db==2)A.db.update(b.id, b, typ);
	}
	public Base get(int id, int t){
		Base b = (Base)m_hm[t].get(new Integer(id));//!
		if(b==null){
			b = A.db.getGeneric(id, t);
			if(b!=null)
				m_hm[t].put(new Integer(id), b);
		}
		return b;
	}
	///////////////OTHER///////////////
	public synchronized String gatherInfo(Job j, boolean tree) { //for new files
		if(j==null||j.m_fa==null) return null; //nasty?
		AFile f = j.m_fa;
		try{
			//if(fresh) A.db.updateGeneric(f.fid, f, DB.I_F);
			f.anime = (Anime)get(f.aid, DB.I_A);
			if(f.gid!=0)
				f.group = (Group)get(f.gid, DB.I_G);
			else f.group = Group.none;
			f.ep = (Ep)get(f.eid, DB.I_E);
			//f.anime.regEp(f.ep);
			if(tree)
				treeAdd(j);
			return null;
		}catch(Exception e){
			U.err(f);
			e.printStackTrace();
			return e.getMessage();
		}
	}
	public AFile parseFile(String s[], Job j){
		if(s.length!=34){
			System.out.println("Unexpected response! len="+s.length);
			j.setError("Unexpected response from server.");
			return null;
		}
		AFile f = new AFile(s);
		int i = 20;
		//create/retrieve data objects
		f.anime = (Anime)m_hm[DB.I_A].get(new Integer(f.aid));
		if(f.anime==null)
			f.anime = new Anime(f.aid);
		else
			A.p.remove(f.anime);

		f.ep = (Ep)m_hm[DB.I_E].get(new Integer(f.eid));
		if(f.ep==null)
			f.ep = new Ep(f.eid);

		f.group = (Group)m_hm[DB.I_G].get(new Integer(f.gid));
		if(f.group==null)
			f.group = new Group(f.gid);

		//set group data
		f.group.name = s[i++];
		f.group.sname = s[i++];
		//set episode data
		f.ep.num = s[i++];
		f.ep.eng = s[i++];
		f.ep.rom = U.n(s[i++]);
		f.ep.kan = U.n(s[i++]);
		//set anime data
		f.anime.eps = Integer.parseInt(s[i++]);
		f.anime.lep = Integer.parseInt(s[i++]);

		try{
			f.anime.yea = Integer.parseInt(s[i++].substring(0,4));
		}catch(Exception e){
			f.anime.yea = 0;
		}
		f.anime.typ = s[i++];
		f.anime.rom = s[i++];
		f.anime.kan = U.n(s[i++]);
		f.anime.eng = U.n(s[i++]);
		f.anime.cat = s[i++];
		f.anime.init();
		//wrap up
		f.def = f.anime.rom+" - "+f.ep.num+" - "+f.ep.eng+" - ["+((f.gid>0)?f.group.sname:"RAW")+"]";
		f.pack();

		//update cache/db
		add(f.anime, 2, DB.I_A);
		add(f.ep, 2, DB.I_E);
		add(f.group, 2, DB.I_G);
		A.db.update(f.fid, f, DB.I_F);

		//update data tree
		j.m_fa = f;
		treeAdd(j);
		return f;
	}
	public String stats(){
		return m_hm[0].size()+","+m_hm[1].size()+","+m_hm[2].size();
	}
	public void rebuildTree(){
		long t = System.currentTimeMillis();
		A.p.clear();
		Job[] j = A.jobs.array();
		for(int i = 0; i < j.length; i++)
			treeAdd(j[i]);
		U.out("@ Rebuilt tree in " +(System.currentTimeMillis()-t)+" ms.");
	}
	public void treeAdd(Job j){
		boolean m = j.checkOr(Job.H_MISSING|Job.H_DELETED);
		if(	j.incompl()||
			(hideE&&!m)||
			(hideN&&m)||
			(j.hide(A.preg)) )
			return;
		AFile f = j.m_fa;
		Anime a = f.anime;
		if(A.p.has(a))
			A.p.remove(a);
		switch(mImode){
		case I_MAF:{
			a.add(f);
		}break;
		case I_MAEF:{
			if(a.has(f.ep)) a.remove(f.ep);
			if(f.ep.has(f)) f.ep.remove(f);
			f.ep.add(f);
			a.add(f.ep);
		}break;
		case I_MAGF:{
			if(f.gid<1) f.group = Group.none;
			Base b = a.get(f.group.getKey());
			if(b!=null) a.remove(b);
			else b = new AG(a, f.group);
			if(b.has(f)) b.remove(f);
			b.add(f);
			a.add(b);
		}break;
		case I_MAFF:{
			String p = j.getFile().getParent();
			Base b = a.get(p);
			if(b!=null) a.remove(b);
			else b = new Path(p);
			if(b.has(f)) b.remove(f);
			b.add(f);
			a.add(b);
		}break;
		}
		a.regEp(f.ep, true);
		a.updatePct();
		A.p.add(a);
	}
	public void treeRemove(Job j){
		if(	j.incompl() )
			return;
		AFile f = j.m_fa;
		Anime a = f.anime;
		if(A.p.has(a))
			A.p.remove(a);
		switch(mImode){
		case I_MAF:{
			a.remove(f);
		}break;
		case I_MAEF:{
			a.remove(f.ep);
			f.ep.remove(f);
			if(f.ep.size()>0)
			a.add(f.ep);
		}break;
		case I_MAGF:{
			if(f.gid<1) f.group = Group.none;
			Base b = a.get(f.group.getKey());
			if(b!=null){
				a.remove(b);
				b.remove(f);
				if(b.size()>0)
					a.add(b);
			}
		}break;
		case I_MAFF:{
			String p = j.getFile().getParent();
			Base b = a.get(p);
			if(b!=null){
				a.remove(b);
				b.remove(f);
				if(b.size()>0)
					a.add(b);
			}
		}break;
		}
		a.regEp(f.ep, f.ep.size()>0);
		a.updatePct();
		if(a.size()>0)
			A.p.add(a);
	}
	public static final int I_MAF=0, I_MAEF=1, I_MAGF=2, I_MAFF=3, I_MLEN=4;
	public static final String[] S_SM = {
		"Anime, File",
		"Anime, Episode, File",
		"Anime, Group, File",
		"Anime, Folder, File"
	};
	public static int mImode = I_MAEF;
	public static boolean hideE=false, hideN=false;
}
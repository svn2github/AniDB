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
 * Created on 29.09.05
 *
 * @version 	01
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.HashMap;

import epox.av.FileInfo;
import epox.util.U;
import epox.webaom.data.AFile;
import epox.webaom.data.Anime;
import epox.webaom.data.Base;
import epox.webaom.data.Ep;
import epox.webaom.data.Group;

public class DB{
	public static final int PLY = 1;
	public static final int I_A=0, I_E=1, I_G=2, I_F=3, I_J=4, I_L=5;
	private static final String sqjob = "select d.name,j.name,j.status,j.orig,j.ed2k,j.md5,j.sha1,j.tth,j.crc32,j.size,j.did,j.uid,j.lid,j.avxml,f.fid,f.aid,f.eid,f.gid,f.def_name,f.state,f.size,f.ed2k,f.md5,f.sha1,f.crc32,f.dublang,f.sublang,f.quality,f.ripsource,f.audio,f.video,f.resolution,f.ext,f.len,e.eid,e.number,e.english,e.romaji,e.kanji from dtb d,jtb j,ftb f,etb e where d.did=j.did and j.fid=f.fid and f.eid=e.eid";
	private Connection con = null;
	//!private HashMap<String,Integer> m_hmd = new HashMap<String,Integer>();
	private final HashMap m_hmd = new HashMap();
	private String mSc = null, mSu=null, mSp=null;

	private boolean mBinitd = false, mBallj = false, mBpgre=true, mBclean=false;

	//private PreparedStatement psau, pseu, psgu, psfu, psai, psei, psgi, psfi, psju, psji;
	private PreparedStatement psu[], psi[];
	private Statement stmt = null;
	private synchronized void _clean(){
		exec("delete from jtb", false);
		exec("delete from dtb", false);
		exec("delete from ftb where fid>0", false);
		exec("delete from atb", false);
		exec("delete from gtb", false);
		exec("delete from etb", false);
	}
	private void _exception(Exception e){
		e.printStackTrace();
	}
	private void _execStr(String s, boolean silent){
		String[] a = s.split("\\;");
		for(int i=0; i<a.length;i++)
			exec(a[i],silent);
	}
	public boolean _init(String dbstr){
		if(mBinitd) return false;
		try{
			if(dbstr.startsWith("!")){
				mBallj = true;
				dbstr = dbstr.substring(1);
			}else if(dbstr.startsWith("?")){
				mBclean = A.confirm("Warning", "Do you really want to clean the db?", "Yes", "No");
				dbstr = dbstr.substring(1);
			}else{
				mBallj = false;
				mBclean = false;
			}
			mSu="root";
			mSp=null;
			if(dbstr.indexOf("postgresql")>0)
				Class.forName("org.postgresql.Driver").newInstance();
			else{
				mBpgre = false;
				if(dbstr.indexOf("mysql")>0)
					Class.forName("com.mysql.jdbc.Driver").newInstance();
				else throw new Exception("Database type not supported.");
			}

			String str = dbstr.toLowerCase();
			int j = str.indexOf("&password=");
			if(j<0)
				j = dbstr.length();
			else mSp = dbstr.substring(j+10);
			int i = str.indexOf("?user=");
			if(i<0)
				i = j;
			else mSu = dbstr.substring(i+6,j);
			mSc = dbstr.substring(0,i);

			if(!_connect()) return false;
			if(!_updateDef()){
				_shutdown();
				return false;
			}
			if(mBclean) _clean();
			return true;
		}catch(Exception e){
			A.dialog("Database:", e.toString());
		}
		return false;
	}
	private boolean _connect(){
		mBinitd = false;
		if(con!=null) try{
			con.close();
		}catch(SQLException e){
			// don't care
		}
		try{
			if(mSp!=null) con = DriverManager.getConnection(mSc, mSu, mSp);
			else con = DriverManager.getConnection(mSc+"?user="+mSu);
			stmt = con.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE, ResultSet.CONCUR_UPDATABLE);

			psu = new PreparedStatement[I_L];
			psu[I_A] = con.prepareStatement("update atb set romaji=?,kanji=?,english=?,year=?,episodes=?,last_ep=?,type=?,genre=?,img=0 where aid=?");
			psu[I_E] = con.prepareStatement("update etb set english=?,kanji=?,romaji=?,number=? where eid=?");
			psu[I_F] = con.prepareStatement("update ftb set aid=?,eid=?,gid=?,def_name=?,state=?,size=?,len=?,ed2k=?,md5=?,sha1=?,crc32=?,dublang=?,sublang=?,quality=?,ripsource=?,audio=?,video=?,resolution=?,ext=? where fid=?");
			psu[I_G] = con.prepareStatement("update gtb set name=?,short=? where gid=?");
			psu[I_J] = con.prepareStatement("update jtb set name=?,did=?,status=?,md5=?,sha1=?,tth=?,crc32=?,fid=?,lid=?,avxml=? where size=? and ed2k=?");
			psi = new PreparedStatement[I_L];
			psi[I_A] = con.prepareStatement("insert into atb (romaji,kanji,english,year,episodes,last_ep,type,genre,img,aid) values (?,?,?,?,?,?,?,?,0,?)");
			psi[I_E] = con.prepareStatement("insert into etb (english,kanji,romaji,number,eid) values (?,?,?,?,?)");
			psi[I_F] = con.prepareStatement("insert into ftb (aid,eid,gid,def_name,state,size,len,ed2k,md5,sha1,crc32,dublang,sublang,quality,ripsource,audio,video,resolution,ext,fid) values (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			psi[I_G] = con.prepareStatement("insert into gtb (name,short,gid) values (?,?,?)");
			psi[I_J] = con.prepareStatement("insert into jtb (name,did,status,md5,sha1,tth,crc32,fid,lid,avxml,size,ed2k,orig,uid) values (?,?,?,?,?,?,?,?,?,?,?,?,?,1)");

			mBinitd = true;
			return true;
		}catch(SQLException e){
			return false;
		}
	}
	public boolean _ok(){
		return mBinitd;
	}
	public volatile boolean debug = true;
	private void _out(Object o){
		if(debug)
			System.out.println(o);
	}
	public void _shutdown(){
		if(!mBinitd) return;
		try{
			if(stmt!= null) stmt.close();
			if(con!=null) con.close();
		}catch(SQLException e){
			_exception(e);
		}
		mBinitd = false;
	}
	private boolean _updateDef(){
		boolean silent = true;
		try{
			ResultSet rs = query("select state from mylist where lid=0", silent);
			if(rs!=null && rs.next()){ // old system
				int v = rs.getInt(1);
				if(v<1) _execStr(A.getFileString("db01.sql"), silent);
				if(v<2) _execStr(A.getFileString("db02.sql"), silent);
			}else{ // new system or none db defined
				rs = query("select ver from vtb;", silent);
				if(rs!=null && rs.next()){
					int v = rs.getInt(1);
					if(v<4)
						if(!A.confirm("Warning", "The database definition has to be upgraded.\nThis will make it uncompatible with previous versions of WebAOM.\nDo you want to continue? (Backup now, if needed.)","Yes","No"))
							return false;
					if(v<1) _execStr(A.getFileString("db03.sql"), silent);
					if(v<2) _execStr(A.getFileString("db04.sql"), silent);
					if(v<3) _execStr(A.getFileString("db05.sql"), silent);
					if(v<4) _execStr(A.getFileString("db06.sql"), silent);
					if(v<5){
						if(mBpgre) _execStr(A.getFileString("db07a.sql"), silent);
						else _execStr(A.getFileString("db07b.sql"), silent);
					}
					if(v<6){
						if(mBpgre) _execStr(A.getFileString("db08a.sql"), silent);
						else _execStr(A.getFileString("db08b.sql"), silent);
					}
				}else{
					String s = A.getFileString("db00.sql");
					if(!mBpgre)
						s = U.replace(s,"serial","integer NOT NULL auto_increment");
					_execStr(s, silent);
				}
			}
		}catch(Exception e){
			e.printStackTrace();
			return false;
		}
		return true;
	}
	private static boolean comex(String s){
		return s.toLowerCase().indexOf("communication")>=0;
	}
	private boolean exec(String cmd, boolean silent){
		return exec(cmd, silent, PLY);
	}
	private ResultSet query(String cmd, boolean silent){
		return query(cmd, silent, PLY);
	}
	private boolean exec(String cmd, boolean silent, int ply){
		if(!mBinitd) return false;
		try{
			if(!silent)
				_out("} "+cmd);
			stmt.execute(cmd);
			return true;
		}catch(SQLException e){
			if(comex(e.getMessage())){
				_out("! CommunicationsException: "+e.getMessage());
				if(ply>0 && _connect())
					return exec(cmd, silent, ply-1);
				return false;
			}
			if(!silent)
				if(e.getErrorCode()!=1062){
					_out("! DB Error Code: "+e.getErrorCode());
					_exception(e);
				}else _out("{ DUPE!");
			return false;
		}
	}
	private ResultSet query(String cmd, boolean silent, int ply){
		if(!mBinitd) return null;
		try{
			if(!silent)
				_out("} "+cmd);
			return stmt.executeQuery(cmd);
		}catch(SQLException e){
			if(comex(e.getMessage())){
				_out("! CommunicationsException: "+e.getMessage());
				if(ply>0 && _connect())
					return query(cmd, silent, ply-1);
				return null;
			}
			if(!silent)
				if(e.getErrorCode()!=1062){
					_out("! DB Error Code: "+e.getErrorCode());
					_exception(e);
				}else _out("{ DUPE!");
			return null;
		}
	}
	private int fill(int i, PreparedStatement ps, int id, Object o, boolean u) throws SQLException{
		if(o instanceof Anime){
			Anime a = (Anime)o;
			ps.setString(i++,a.rom);
			ps.setString(i++,a.kan);
			ps.setString(i++,a.eng);
			ps.setInt(i++,a.yea);
			ps.setInt(i++,a.eps);
			ps.setInt(i++,a.lep);
			ps.setString(i++,a.typ);
			ps.setString(i++,a.cat);
			ps.setInt(i++, id);
		}else if(o instanceof Ep){
			Ep e = (Ep) o;
			ps.setString(i++,e.eng);
			ps.setString(i++,e.kan);
			ps.setString(i++,e.rom);
			ps.setString(i++,e.num);
			ps.setInt(i++, id);
		}else if(o instanceof AFile){
			AFile f = (AFile) o;
			ps.setInt(i++,f.aid);
			ps.setInt(i++,f.eid);
			ps.setInt(i++,f.gid);
			ps.setString(i++,f.def);
			ps.setInt(i++,f.stt);
			ps.setLong(i++,f.mLs);
			ps.setInt(i++,f.len);
			ps.setString(i++,f.ed2);
			ps.setString(i++,f.md5);
			ps.setString(i++,f.sha);
			ps.setString(i++,f.crc);
			ps.setString(i++,f.dub);
			ps.setString(i++,f.sub);
			ps.setString(i++,f.qua);
			ps.setString(i++,f.rip);
			ps.setString(i++,f.aud);
			ps.setString(i++,f.vid);
			ps.setString(i++,f.res);
			ps.setString(i++,f.ext);
			ps.setInt(i++,f.fid);
		}else if(o instanceof Group){
			Group g = (Group)o;
			ps.setString(i++,g.name);
			ps.setString(i++,g.sname);
			ps.setInt(i++, id);
		}else if(o instanceof Job){
			Job j = (Job)o;
			if(j.mIdid<1) j.mIdid = getDid(j.m_fc.getParent());
			ps.setString(i++,j.m_fc.getName());
			ps.setInt(i++,j.mIdid);
			ps.setInt(i++,j.getStatus());
			ps.setString(i++,j._md5);
			ps.setString(i++,j._sha);
			ps.setString(i++,j._tth);
			ps.setString(i++,j._crc);
			ps.setInt(i++,j.m_fa!=null?j.m_fa.fid:0);
			ps.setInt(i++,j.mIlid);
			ps.setString(i++,j.m_fi==null?null:j.m_fi.m_xml);
			if(j.m_fc.exists())
				ps.setLong(i++,j.m_fc.length());
			else ps.setLong(i++,j.mLs);
			ps.setString(i++,j._ed2);
			if(u) ps.setString(i, j.mSo);
		}
		return i;
	}
	private int getDid(String path){
		if(!mBinitd) return -1;
		if(path==null) path = "";
		try{
			path = U.replace(path,"\\","\\\\");
			path = U.replace(path,"'","\\'");
			Object obj = m_hmd.get(path);
			if(obj!=null)
				return ((Integer)obj).intValue();
			ResultSet rs = query("select did from dtb where name='"+path+"'", false);
			if(rs.first()){
				int did = rs.getInt(1);
				m_hmd.put(path, new Integer(did));
				return did;
			}
			_out("} insert into dtb (name) values ('"+path+"')");
			if(mBpgre){
				stmt.execute("insert into dtb (name) values ('"+path+"');SELECT currval('dtb_did_seq')");
				if(!stmt.getMoreResults()) return -1;
				rs = stmt.getResultSet(); //stmt.getGeneratedKeys();
				if(rs.first()){
					int did = rs.getInt(1);
					m_hmd.put(path, new Integer(did));
					return did;
				}
			}else{
				stmt.executeUpdate("insert into dtb (name) values ('"+path+"')");
				rs = stmt.getGeneratedKeys();
				if(rs.first()){
					int did = rs.getInt(1);
					m_hmd.put(path, new Integer(did));
					return did;
				}
			}
		}catch(SQLException e){
			_exception(e);
		}
		return -1;
	}
	public synchronized Base getGeneric(int id, int t){
		if(!mBinitd) return null;
		try{
			if(t==I_E){
				Ep ae = new Ep(id);
				ResultSet rs = query("select english,kanji,romaji,number from etb where eid="+id+";", false);
				if(rs.first()){
					ae.eng = rs.getString(1);
					ae.kan = rs.getString(2);
					ae.rom = rs.getString(3);
					ae.num = rs.getString(4).intern();
					_out("{ "+ae);
					return ae;
				}
			} else if(t==I_G){
				Group ag = new Group(id);
				ResultSet rs = query("select name,short from gtb where gid="+id+";", false);
				if(rs.first()){
					ag.name = rs.getString(1);
					ag.sname = rs.getString(2);
					_out("{ "+ag);
					return ag;
				}
			} else if(t==I_A){
				ResultSet rs = query("select episodes,last_ep,year,type,romaji,kanji,english,genre,img from atb where aid="+id+";", false);
				if(rs.first()){
					int i = 1;
					String s[] = new String[9];
					s[0] = ""+id;
					for(int j=1; j<s.length; j++)
						s[i] = rs.getString(i++);
					Anime a = new Anime(s);
					_out("{ "+a);
					return a;
				}
			}
			return null;
		}catch(SQLException e){
			_exception(e);
			return null;
		}
	}
	public synchronized int getJob(Job j, boolean ed){
		if(!mBinitd) return -1;
		try{
			ResultSet rs;
			if(ed) rs = query(sqjob+" and j.size="+j.m_fc.length()+" and j.ed2k="+s(j._ed2)+";", false);
			else{
				int did = getDid(j.m_fc.getParent());
				rs = query(sqjob+" and j.size="+j.m_fc.length()+" and j.name="+s(j.m_fc.getName())+" and j.did="+did+";", false);
			}
			if(rs.first()){
				int i = 1;
				j.m_fn = new File(rs.getString(i++)+File.separatorChar+rs.getString(i++));
				if(j.m_fc.equals(j.m_fn))
					j.m_fn = null;
				int status = rs.getInt(i++);
				//j.setStatus(rs.getInt(i++), false);
				mkJob(rs, i, j);
				j.mBf = false;
				_out("{ Job found: "+j+":"+status);
				return status;
			}
		}catch(SQLException e){
			_exception(e);
		}return -1;
	}
	public synchronized void getJobs(){
		if(!mBinitd) return;
		try{
			Job j;
			ResultSet rs;
			if(mBallj)
				//rs = query(sqjob+" order by RAND() LIMIT 100", false);
				//rs = query(sqjob+" AND f.aid=2165", false);
				rs = query(sqjob+" ORDER BY j.time", false);
			else
				rs = query(sqjob+" and j.status!="+Job.FINISHED+" ORDER BY j.time", false);

			while(rs.next()){
				int i = 1;
				File f = new File(rs.getString(i++)+File.separatorChar+rs.getString(i++));
				//if(!f.exists()||f.getParent().startsWith("I:")) continue;
				j = new Job(f, rs.getInt(i++));
				mkJob(rs, i, j);
				if(!A.jobs.add(j))
					U.err("DB: Dupe: "+j);
			}
		}catch(SQLException e){
			_exception(e);
		}
	}
	private void mkJob(ResultSet rs, int i, Job j) throws SQLException{
		j.mSo = rs.getString(i++);
		j._ed2 = rs.getString(i++);
		j._md5 = rs.getString(i++);
		j._sha = rs.getString(i++);
		j._tth = rs.getString(i++);
		j._crc = rs.getString(i++);
		j.mLs = rs.getLong(i++);
		j.mIdid = rs.getInt(i++);
		i++;//j.mIuid = rs.getInt(i++);
		j.mIlid = rs.getInt(i++);
		String xml = rs.getString(i++);
		if(xml!=null&&xml.length()>0) try{
			j.m_fi = new FileInfo(xml);
		}catch(Exception e){
			j.m_fi = null;
		}
		int fid = rs.getInt(i);
		if(fid>0){
			String s[] = new String[20];
			for(int x=0; x<20; x++)
				s[x] = rs.getString(i++);

			if (s[18]==null || s[18].length()<1)
				s[18] = j.getExtension();

			String def = s[4];
			s[4] = ""+j.mIlid;
			j.m_fa = new AFile(s);
			j.m_fa.pack();
			j.m_fa.setJob(j);
			j.m_fa.def = def;
			j.m_fa.pack();
			s = new String[5];
			for(int x=0; x<s.length;x++)
				s[x] = rs.getString(i++);
			A.cache.add(new Ep(s), 0, DB.I_E);
		}
	}
	public synchronized boolean removeJob(Job j){
		return exec("delete from jtb where ed2k="+s(j._ed2)+" and name="+s(j.m_fc.getName())+";", false);
	}
	private String s(String s){
		if(s==null) return "NULL";
		return "\'"+U.replace(s,"\'","\\\'")+"\'";
	}
	public synchronized boolean update(int id, Object o, int typ){
		return update2(id,o,typ,PLY);
	}
	private boolean update2(int id, Object o, int typ, int ply){
		try{
			return update1(id,o,typ);
		}catch(SQLException e){
			_out("! CommunicationsException: "+e.getMessage());
			if(ply>0 && _connect())
				return update2(id,o,typ,ply-1);
		}
		return false;
	}
	private boolean update1(int id, Object o, int typ) throws SQLException{
		if(!mBinitd) return false;
		try{
			fill(1, psu[typ], id, o, false);
			if(update(psu[typ])>0)
				return true;
		}catch(SQLException e){
			if(comex(e.getMessage()))
				throw e;
			_exception(e);
		}
		try{
			fill(1, psi[typ], id, o, true);
			if(update(psi[typ])>0)
				return true;
		}catch(SQLException e){
			if(comex(e.getMessage()))
				throw e;
			_exception(e);
		}
		return false;
	}
	private int update(PreparedStatement ps) throws SQLException{
		String str = ps.toString();
		if(!mBpgre){
			int i = str.indexOf(": ");
			if(i>0) str = str.substring(i+2);
			else{
				i = str.indexOf(" - ");
				if(i>0) str = str.substring(i+3);
			}
		}
		_out("} "+str);
		return ps.executeUpdate();
	}
}
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
 * Created on 10.des.2005 18:58:48
 * Filename: aConEx.java
 */
package epox.webaom.net;

import epox.swing.Log;
import epox.util.U;
import epox.webaom.Job;
import epox.webaom.data.Mylist;

public class AConE extends ACon {
	private static final String facode = "&fcode=123682590&acode=75435779";
	public AConE(Log l, AConS s){
		super(l,s);
	}
	public String[] retrieveFileData(long len, String ed2k, String name) throws AConEx{
		return retrieveFileData(send("FILE", "size="+len+"&ed2k="+ed2k+facode, true), "ed2k://|file|"+name+"|"+len+"|"+ed2k+"|");
	}
	public String[] retrieveFileData(int fid, String name) throws AConEx{
		return retrieveFileData(send("FILE", "fid="+fid+facode, true), name);
	}
	private String[] retrieveFileData(AConR r, String name){
		switch(r.code){
			case AConR.FILE: return U.split(r.data,'|');
			case AConR.NO_SUCH_FILE:
				error("No such file in AniDB: "+name);
				break;
			case AConR.INTERNAL_SERVER_ERROR:
				error("Internal server error on "+name+". (Illegal char in epname?)");
				break;
			default:
				error("Unexpected response ("+r.code+"): "+r.message);
		}
		return null;
	}
	public static String validate(String str){
		str = U.replace(str, "&","&amp;");
		str = U.replace(str, "\r","");
		str = U.replace(str, "\n","<br />");
		return str;
	}
	public int addFileToMylist(Job j, Mylist m) throws AConEx{
		StringBuffer sb = new StringBuffer(100);
		sb.append("fid=");		sb.append(j.m_fa.fid);
		sb.append("&state=");	sb.append(m.stt);
		sb.append("&viewed=");	sb.append(m.vie);
		sb.append("&source=");	sb.append(validate(m.sou));
		sb.append("&storage=");	sb.append(validate(m.sto));
		sb.append("&other=");	sb.append(validate(m.oth));
		sb.append("&edit=0");

		AConR r = send("MYLISTADD", sb.toString(), true);
		if(r==null) return 0;
		switch(r.code){
			case AConR.MYLIST_ENTRY_ADDED: return Integer.parseInt(r.data);
			case AConR.FILE_ALREADY_IN_MYLIST:
				error(j.m_fa.def+" is already in mylist.");
				return Integer.parseInt(r.data);
			case AConR.NO_SUCH_MYLIST_FILE:
				error(j.m_fa.def+" was not found in AniDB.");
			case AConR.MYLIST_ENTRY_EDITED:
			case AConR.NO_SUCH_MYLIST_ENTRY:
			default:
			error("Unexpected response ("+r.code+"): "+r.message);
		}
		return 0;
	}
	public boolean removeFromMylist(int lid, String name) throws AConEx{
		AConR r = send("MYLISTDEL", "lid="+lid, true);
		switch(r.code){
			case AConR.MYLIST_ENTRY_DELETED:
				return true;
			case AConR.NO_SUCH_MYLIST_ENTRY:
				return false;
			default:
				error(r.message+" ("+name+")");
		}
		return false;
	}
	/*public Anime getAnime(int aid) throws AConEx{
		AConR r = send("ANIME", "aid="+aid, true);
		if(r!=null && r.code==AConR.ANIME)
			return Parser.parseAnime(U.split(r.data,'|'));
		return null;
	}
	public Group getGroup(int gid) throws AConEx{
		AConR r = send("GROUP", "gid="+gid, true);
		if(r!=null && r.code==AConR.GROUP)
			return Parser.parseGroup(U.split(r.data,'|'));
		return null;
	}
	public Ep getEpisode(int eid) throws AConEx{
		AConR r = send("EPISODE", "eid="+eid, true);
		if(r!=null && r.code==AConR.EPISODE)
			return Parser.parseEpisode(U.split(r.data,'|'));
		return null;
	}*/
}

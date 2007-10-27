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
 * Created on 22.01.05
 *
 * @version 	03
 * @author 		epoximator
 */
package epox.webaom.data;

import epox.util.U;
import epox.webaom.A;
import epox.webaom.Job;

public class AFile extends Base{
	public int fid, aid, eid, gid, lid, stt, len;
	public String ed2,md5,sha,crc,dub,sub,qua,rip,res,vid,aud,def,ext;

	public Ep ep;
	public AG ag;
	public Group group;
	public Anime anime;
	private Job job = null;
	public AFile(int id){
		fid=id;
		ext=null;
	}
	public AFile(String s[]){
		int i = 0;
		fid = U.i(s[i++]);
		aid = U.i(s[i++]);
		eid = U.i(s[i++]);
		if(s[i].length()<1)
			gid = 0;
		else gid = U.i(s[i]);
		i++;
		lid = U.i(s[i++]);
		stt = U.i(s[i++]);
		if(s[i].length()<1)
			mLs=0;
		else mLs = Long.parseLong(s[i]);
		i++;
		ed2 = s[i++];
		md5 = U.n(s[i++]);
		sha = U.n(s[i++]);
		crc = U.n(s[i++]);

		dub = s[i++].intern();
		sub = s[i++].intern();
		qua = s[i++].intern();
		rip = s[i++].intern();
		aud = s[i++].intern();
		vid = s[i++].intern();
		res = s[i++].intern();
		ext = s[i++].intern();
		len = U.i(s[i++]);
	}
	public Job getJob() {
		return job;
	}
	public void setJob(Job j) {
		job = j;
		//mBs = job.getFile().exists();
		//if(mBs)
		//	mLs = job.getFile().length();
	}
	public String toString(){
		if(job!=null) return job.getFile().getName();
		return def;
	}
	/*public String toString(){
		return ed2+md5+sha+crc+dub+sub+qua+rip+res+vid+aud+def+ep;
	}*/
	public String serialize(){
		return ""+fid+S+aid+S+eid+S+gid+S+lid+S+stt+S+mLs
		+S+ed2+S+md5+S+sha+S+crc+S+dub+S+sub+S+qua+S+rip+S+aud+S+vid+S+res+S+len
		+S+(group!=null?group.serialize():"");
	}
	public static Base getInst(String[] s){
		return new AFile(s);
	}
	public Object getKey(){
		return new Integer(fid);
	}
	public void clear(){
		//
	}
	public void pack(){
		dub = dub.replace('/', '&'); // for jap/eng
		vid = vid.replace('/', ' '); // for 'H264/AVC'
		int i = vid.indexOf(" ");
		if(i>0) vid = vid.substring(0,i);
		i = aud.indexOf(" ("); // for Vorbis (Ogg Vorbis)
		if(i>0) aud = aud.substring(0,i);
		if(dub.startsWith("dual (")) // remove dual()
			dub = dub.substring(6, dub.lastIndexOf(')'));

		vid = vid.intern();
		aud = aud.intern();
		dub = dub.intern();
	}
	private String url0(String str, boolean non){
		if(non)
			return "http://"+A.S_WEB+"/perl-bin/animedb.pl?"+str+"&nonav=1";
		return "http://"+A.S_WEB+"/perl-bin/animedb.pl?"+str;
	}
	private String url1(String str){
		return url0("show="+str,true);
	}
	public String urlAnime(){
		return url1("anime&aid="+aid);
	}
	public String urlExport(){
		return url1("ed2kexport&h=1&aid="+aid);
	}
	public String urlEp(){
		return url1("ep&aid="+aid+"&eid="+eid);
	}
	public String urlFile(){
		return url1("file&aid="+aid+"&eid="+eid+"&fid="+fid);
	}
	public String urlGroup(){
		return url1("group&gid="+gid);
	}
	public String urlMylistE(int i){
		if(i<2) return urlMylist();
		return url1("mylist&do=add&aid="+aid+"&eid="+eid+"&fid="+fid+"&lid="+i);
	}
	public String urlMylist(){
		try{
			return url1("mylist&expand="+aid+"&char="+anime.rom.charAt(0)+"#a"+aid);
		}catch(Exception e){
			return url1("mylist");
		}
	}
	public String urlYear(){
		try{
			return url0("do.search=%20Start%20Search%20&show=search&noalias=1&search.anime.year="+anime.yea,false);
		}catch(Exception e){
			return url1("search");
		}
	}
	public static final int F_CRCOK		=1,
							F_CRCERR	=2,
							F_ISV2		=4,
							F_ISV3		=8,
							F_ISV4		=16,
							F_ISV5		=32,
							F_UNC		=64,
							F_CEN		=128;

	///
	public boolean inYear(String s){
		s.replaceAll(" ", "");
		int i = s.indexOf('-');
		if(i>0){ //Range
			int start = Integer.parseInt(s.substring(0, i));
			int end = Integer.parseInt(s.substring(i+1));
			if(start>end){
				int t = end;
				end = start;
				start = t;
			}
			return anime.yea>=start && anime.yea<=end;
		}
		return anime.yea == Integer.parseInt(s);
	}
	/* f_state
	 * 0 - crc ok
	 * 1 - corrupt
	 * 2 - v2
	 * 3
	 * 4
	 * 5 - v5
	 * 6 - uncensored
	 * 7 - censored
	 */
	public String getVersion(){
		switch(stt&0x3C){
			case F_ISV2: return "v2";
			case F_ISV3: return "v3";
			case F_ISV4: return "v4";
			case F_ISV5: return "v5";
			default: return "";
		}
	}
	public String getCensored(){
		switch(stt&0xC0){
			case F_CEN: return "unc";
			case F_UNC: return "cen";
			default: return "";
		}
	}
	public String getInvalid(){
		if((stt&F_CRCERR)==F_CRCERR)
			return "invalid crc";
		return "";
	}
	public String mds(){
		if(anime==null||ep==null) return "N/A";
		String x = "";

		if(crc==null||crc.length()<1) x+='c';
		if(	md5==null||md5.length()<1 ||
			sha==null||sha.length()<1) x+='h';
		if(len<1) x+='l';
		if(dub.indexOf("unknown")>=0) x+='d';
		if(sub.indexOf("unknown")>=0) x+='s';
		if(aud.indexOf("unknown")>=0) x+='a';
		if(vid.indexOf("unknown")>=0) x+='v';
		if(res.equals("0x0")||res.equals("unknown")) x+='x';

		return x;
	}
	public String mda(){
		if(anime==null||ep==null) return "N/A";
		String x = "";

		if(qua.indexOf("unknown")>=0) x+='q';
		if(rip.indexOf("unknown")>=0) x+='o';

		if(ep.eng==null||ep.eng.length()<1) x+='e';
		if(ep.kan==null||ep.kan.length()<1) x+='k';
		if(ep.rom==null||ep.rom.length()<1) x+='r';

		return x;
	}
	/*public int compareTo(Object o){
		if(o instanceof AFile){
			AFile f = (AFile)o;
			if(mBs==f.mBs){
				return job.getFile().compareTo(f.job.getFile());
			}
			if(mBs&&!f.mBs) return -1;
			return 1;
		}
		return super.compareTo(o);
	}*/
}
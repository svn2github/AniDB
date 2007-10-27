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
 * Created on 31.07.05
 *
 * @version 	05 (1.13,1.12,1.11,1.10,1.09)
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.StringTokenizer;

public class Options{
	public static final String S_SEP = "\1";
	private static final String S_VER = "001", S_SEP2 = "\2";
	private File m_file;
	public int[] mIa = new int[I_LEN];
	public String[] mSa = new String[S_LEN];
	public boolean[] mBa = new boolean[B_LEN];

	public Options(){
		String home = System.getProperty("user.home");
		m_file = new File(home+File.separator+".webaom");
	}
	public boolean onDisk(){
		return m_file.exists();
	}
	public String path(){
		return m_file.getAbsolutePath();
	}
	public boolean equals(Options o){
		for(int i=0; i<B_LEN; i++)
			if(mBa[i]!=o.mBa[i])
				return false;
		for(int i=0; i<I_LEN; i++)
			if(mIa[i]!=o.mIa[i])
				return false;
		for(int i=0; i<S_LEN; i++)
			if(!strcmp(mSa[i], o.mSa[i]))
				return false;
		return true;
	}
	private boolean strcmp(String a, String b){
		return a==null&&b==null||a!=null&&b!=null&&a.equals(b)||a.length()==0&&b==null;
	}
	public void save(){
		if(	!onDisk() &&
			!A.confirm("Warning", "Options will be stored here:\n"+path(), "Continue", "Cancel"))
				return;
		try{
			FileOutputStream fo = new FileOutputStream(m_file);
			fo.write(enc().getBytes("utf8"));
			fo.close();
			System.out.println("$ File written:"+m_file);
		}catch(IOException e){e.printStackTrace();}
	}
	public boolean load(){
		if(!onDisk()) return false;
		try{
			FileInputStream fw = new FileInputStream(m_file);
			int len = (int)m_file.length(), off=0, read;
			byte[] buffer = new byte[len];
			do{
				read = fw.read(buffer, off, len-off);
				off += read;
			}while(read>0);
			fw.close();
			System.out.println("$ File read:"+m_file);
			return dec(new String(buffer, "utf8"));
		}
		//catch(IOException e){e.printStackTrace();}
		catch(Exception e){e.printStackTrace();}
		return false;
	}
	public String enc(){
		return S_VER+S_SEP2+encB()+S_SEP2+encI()+S_SEP2+encS();
	}
	public boolean dec(String str){
		StringTokenizer st = new StringTokenizer(str, S_SEP2);
		if(st.countTokens()>0 && st.nextToken().equals(S_VER)){
			decB(st.nextToken());
			decI(st.nextToken());
			decS(st);
			return true;
		}
		System.out.println("! Options file is outdated. Could not load.");
		return false;
	}
	public boolean getB(int id){
		return mBa[id];
	}
	public void setB(int id, boolean b){
		mBa[id] = b;
	}
	public int getI(int id){
		return mIa[id];
	}
	public void setI(int id, int i){
		mIa[id] = i;
	}
	public String getS(int id){
		return mSa[id];
	}
	public void setS(int id, String s){
		mSa[id] = s;
	}
	private String encB(){
		String bool = "";
		for(int i=0; i<B_LEN; i++)
			bool += mBa[i] ? '1':'0';
		return bool;
	}
	private void decB(String bool){
		char[] c = bool.toCharArray();
		for(int i=0; i<B_LEN; i++)
			mBa[i] = (c[i]=='1');
	}
	private String encI(){
		String ints = "";
		for(int i=0; i<I_LEN; i++)
			ints += mIa[i]+Options.S_SEP;
		return ints;
	}
	private void decI(String str){
		StringTokenizer st = new StringTokenizer(str, Options.S_SEP);
		for(int i=0; i<I_LEN; i++)
			mIa[i] = Integer.parseInt(st.nextToken());
	}
	private String encS(){
		String s = "";
		for(int i=0; i<S_LEN; i++)
			s += ((mSa[i]==null||mSa[i].length()==0)?"null":mSa[i])+S_SEP2;
		return s;
	}
	private void decS(StringTokenizer st){
		String tmp;
		for(int i=0; i<S_LEN; i++){
			if(st.hasMoreTokens())
				tmp = st.nextToken();
			else tmp = null;
			mSa[i] = (tmp==null||tmp.equals("null"))?"":tmp;
		}
	}
	public static final int
	B_UNUSED0 =  0, //
	B_HASHCRC =  1,
	B_HASHMD5 =  2,
	B_HASHSHA =  3,
	B_HASHTTH =  4,
	B_ADDFILE =  5,
	B_WATCHED =  6,
	B_UNUSED1 =  7, //
	B_NATKEEP =  8,
	B_STORPSW =  9,
	B_ALOADDB = 10,
	B_AUTOLOG = 11,
	B_AUTOSAV = 12, //
	B_LEN     = 13,

	I_RPORT = 0,
	I_LPORT = 1,
	I_STATE = 2, //file state on mylist add
	I_TIMEO = 3, //timeout
	I_DELAY = 4, //datagram delay
	I_USMOD = 5, //rename mode
	I_LEN   = 6,

	S_USRNAME = 0,
	S_HOSTURL = 1,
	S_MYDBURL = 2,
	S_HASHDIR = 3,
	S_BROWSER = 4,
	S_EXTENSN = 5,
	S_SOURCEF = 6,
	S_STORAGE = 7,
	S_OTHERIN = 8,
	S_VRLSREN = 9,
	S_VRLSMOV = 10,
	S_REPLSYS = 11,
	S_HTMLCOL = 12,
	S_LOGFILE = 13,
	S_PATHREG = 14,
	S_FONTSTR = 15,
	S_LOGHEAD = 16,
	S_JOBCOLS = 17,
	S_LEN     = 18;

}
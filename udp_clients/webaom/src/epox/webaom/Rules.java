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
 * Created on 03.08.05
 *
 * @version 	05 (1.14,1.13,1.12,1.11,1.09)
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;
import java.util.StringTokenizer;
import java.util.Vector;
import java.util.regex.PatternSyntaxException;

import epox.swing.Log;
import epox.util.DSData;
import epox.util.Hyper;
import epox.util.U;
import epox.webaom.data.AMap;

public class Rules{
	public final static int
	M_NOREN = 0,
	M_RULES = 1,
	M_ANIDB = 2,
	M_FALLB = 3;
	public final static String TRUNC = "TRUNCATE<";

	public Vector/*!<DSData>*/ mVill;
	private String mSren, mSmov;
	private AMap mAmap=null;

	private Log log;
	public void setLog(Log l){
		log = l;
	}

	public Rules(){
		mVill = new Vector/*!<DSData>*/();
		mVill.add(new DSData("`", "'", true));
		mVill.add(new DSData("\"", "", true));
		mVill.add(new DSData("<", "", true));
		mVill.add(new DSData(">", "", true));
		mVill.add(new DSData("|", "", true));
		mVill.add(new DSData("/", "", true));
		mVill.add(new DSData(":", "", true));
		mVill.add(new DSData("\\", "", true));
		mVill.add(new DSData("?", "", true));
		mVill.add(new DSData("*", "", true));

		mSren="#Uncomment to enable default renaming\n#DO SET '%ann - %enr - %epn '\n#IF G(!unknown) DO ADD '[%grp]'\n#ELSE DO ADD '[RAW]'";
		mSmov="#Check the wiki before writing move rules";
	}
	public String getMov() {
		return mSmov;
	}
	public void setMov(String s) {
		mSmov = s;
	}
	public String getRen() {
		return mSren;
	}
	public void setRen(String s) {
		mSren = s;
	}
	private String replaceOldTags(String s){
		s = U.replace(s, "%year", "%yea");
		s = U.replace(s, "%type", "%typ");
		s = U.replace(s, "%qual", "%qua");
		s = U.replace(s, "%ed2k", "%ed2");
		return s;
	}
	public void optl(Options o){
		mSren = replaceOldTags(o.getS(Options.S_VRLSREN));
		mSmov = replaceOldTags(o.getS(Options.S_VRLSMOV));
		DSData.decode(mVill, o.getS(Options.S_REPLSYS));
	}
	public void opts(Options o){
		o.setS(Options.S_VRLSREN, mSren);
		o.setS(Options.S_VRLSMOV, mSmov);
		o.setS(Options.S_REPLSYS, DSData.encode(mVill));
	}
	public File apply(Job j){
		if(j==null||j.m_fa==null) return null;

		String path = j.m_fc.getParent();
		String name = j.m_fc.getName();

		if(j.m_fa.anime!=null){
			mAmap=j.genMap();
			String sren = get(j, mSren);
			String smov = get(j, mSmov);
			mAmap=null;

			if(smov!=null){
				String p = fin(j, smov);
				if(p.startsWith("."+File.separator))
					path += p.substring(1);
				else path = p;
			}
			if(sren!=null)
				name = fin(j, sren)+"."+j.getExtension();
		}
		else return null;

		String abs = path+File.separator+name;
		abs = U.replace(abs, ""+File.separator+File.separator, ""+File.separator);

		if(path.startsWith("\\\\"))
			abs = "\\"+abs;

		File f = new File(abs);

		System.out.println("% New file: "+f);
		return f;
	}
	private String get(Job j, String str){
		try{
			Vector v = build(str, j);
			if(v.size()<1) return null;
			String rule = "";
			while(v.size()>0) rule += v.remove(0);
			return rule;
		}catch(Exception e){e.printStackTrace();}
		return null;
	}
	private Vector/*!<Section>*/ build(String script, Job j) throws Exception{
		StringTokenizer st = new StringTokenizer(script, "\r\n");
		String tok, tup;
		Vector/*!<Section>*/ schema = new Vector/*!<Section>*/();
		int i; boolean prev=true;
		while(st.hasMoreTokens()){
			tok = st.nextToken();
			tup = tok.toUpperCase();
			if(tup.startsWith("#")) continue;
			if(tup.startsWith("DO ") && handle(schema, tok.substring(3))) break;
			else if(tup.startsWith("IF ")&&
					(prev=check(tok.substring(3, (i=tup.indexOf(" DO "))), j)) &&
					handle(schema, tok.substring(i+4))) break;
			else if(tup.startsWith("ELSE IF ")&&!prev &&
					(prev=check(tok.substring(8, (i=tup.indexOf(" DO "))), j)) &&
					handle(schema, tok.substring(i+4))) break;
			else if(tup.startsWith("ELSE DO ")&&!prev &&
					handle(schema, tok.substring(8))) break;
		}
		return schema;
	}
	private boolean handle(Vector/*!<Section>*/ sch, String op) throws Exception{
		String up = op.toUpperCase();
		if(up.startsWith("ADD ")){
			sch.add(new Section(op.substring(4)));
			return false;
		}
		if(up.startsWith("SET ")){
			sch.clear();
			sch.add(new Section(op.substring(4)));
			return false;
		}
		if(up.equals("FAIL")){
			sch.clear();
			return true;
		}
		if(up.equals("FINISH"))
			return true;
		if(up.startsWith("FINISH ")){
			sch.add(new Section(op.substring(7)));
			return true;
		}
		if(up.startsWith("RETURN ")){
			sch.clear();
			sch.add(new Section(op.substring(7)));
			return true;
		}
		if(up.startsWith("ASSUME ")){
			try{
				if(up.startsWith("ASSUME SPECIAL "))
					A.ASSP = U.i(up.substring(15).trim());
				else A.ASNO = U.i(up.substring(7).trim());
			}catch(NumberFormatException e){
				A.dialog("NumberFormatException", "Parsing '"+op+"' failed.");
			}
			return false;
		}
		if(up.startsWith(TRUNC)){
			int x = up.indexOf('>');
			int y = up.indexOf(',');
			if(x>0 && y>0 && y<x)
				sch.add(new Section(up.substring(0,x+1)));
			else log.add(Hyper.error("Invalid rule element: "+op));
			return false;
		}
		sch.clear();
		throw new Exception("Error after DO: Expected SET/ADD/FAIL/FINISH/TRUNCATE<int,str>: "+op);
	}
	private boolean check(String s, Job j) throws Exception{
		StringTokenizer st = new StringTokenizer(s, ";");
		if(st.countTokens()<1)return false;
		boolean b = true;
		while(st.hasMoreTokens() && b)
			try{
				b &= check1(st.nextToken().trim(), j);
			}catch (Exception e){
				e.printStackTrace();
				return false;
			}
			return b;
	}
	private int findEndPar(String s, int start){
		int level = 0; char c;
		for (int i = start; i < s.length(); i++){
			c = s.charAt(i);
			if(c=='(')
				level++;
			else if(c==')'){
				if(level==0)
					return i;
				level--;
			}
		}
		return -1;
	}
	private boolean check1(String s, Job j) throws Exception{
		char c = Character.toUpperCase(s.charAt(0));
		int x = s.indexOf('(')+1;
		int y = findEndPar(s, x);//s.indexOf(')', x);
		if(x<1||y<0) throw new Exception("Missing ( or ): "+s);

		StringTokenizer st = new StringTokenizer(s.substring(x, y), ",");
		while(st.hasMoreTokens())
			try{
				String test = st.nextToken().trim();
				boolean not = test.startsWith("!");
				if(not) test = test.substring(1);
				if(not^check0(c, test, j)) return true;
			}catch (Exception ex){ex.printStackTrace();}
			return false;
	}
	private boolean check0(char c, String s, Job j){
		switch(Character.toUpperCase(c)){
		case 'A':{ //Name
			try{
				return j.m_fa.aid==Integer.parseInt(s.trim());
			}catch(NumberFormatException e){
				return regtest(j.m_fa.anime.rom, s)
				|| regtest(j.m_fa.anime.kan, s)
				|| regtest(j.m_fa.anime.eng, s);
			}
		}
		case 'E':
			return regtest(j.m_fa.ep.num, s)||regtest(j.m_fa.ep.eng, s);
		case 'C': //Codec
			return j.m_fa.vid.equalsIgnoreCase(s) || cont(j.m_fa.aud,s);
		case 'Q': //Quality
			return j.m_fa.qua.equalsIgnoreCase(s);
		case 'R': //ripSource
			return j.m_fa.rip.equalsIgnoreCase(s);
		case 'T': //Type
			return j.m_fa.anime.typ.equalsIgnoreCase(s);
		case 'G':{ //Group
			if(j.m_fa.gid==0)
				return s.equalsIgnoreCase("unknown");
			try{
				return j.m_fa.gid==Integer.parseInt(s.trim());
			}catch(NumberFormatException e){/*part of plan*/}
			return	j.m_fa.group.name.equalsIgnoreCase(s)||
			j.m_fa.group.sname.equalsIgnoreCase(s);
		}
		case 'Y': //Year
			return j.m_fa.inYear(s);
		case 'D': //Dub lang
			return cont(j.m_fa.dub,s);
		case 'S': //Sub lang
			return cont(j.m_fa.sub,s);
		case 'X': //check number of eps
			return s.equals(""+j.m_fa.anime.eps);
		case 'P': //Path
			return regtest(j.m_fc.getAbsolutePath(), s);
		case 'N': //Genre
			return cont(j.m_fa.anime.cat,s);
		case 'I':{ //Is Defined
			/*if(s.equals("eng"))//probably most used
					return j.m_fa.anime.eng!=null;
				if(s.equals("ver"))//won't work with .containsKey since it is not null by default
					return j.m_fa.getVersion().length()>0;*/
			String t = (String)mAmap.get(s);
			return t!=null && t.length()>0;
		}
		case 'U':{
			String[] cmp = s.split(":", 2);
			if(cmp.length==2){
				return mAmap.containsKey(cmp[0])
				&& mAmap.containsKey(cmp[1])
				&& !mAmap.get(cmp[0]).equals(mAmap.get(cmp[1]));
			}
			System.out.println("ERROR: Invalid data in test: U("+s+")");
			return false;
		}

		default: return false;
		}
	}
	private static boolean regtest(String s, String t){
		if(s==null) return false;
		try{
			return s.matches(t);
		}catch(PatternSyntaxException e){
			A.dialog("Error", e.getMessage());
			return false;
		}
	}
	private static boolean cont(String s, String t){
		return s.toLowerCase().indexOf(t.toLowerCase())>=0;
	}
	private String fin(Job j, String s){
		s = s.replace(File.separatorChar, '\1').replace(':', '\2');
		s = truncate(j.convert(s));
		s = replace(s, mVill);
		s = s.replace('\1', File.separatorChar).replace('\2', ':');
		if(s.endsWith("_")){
			s = s.substring(0, s.length()-1);
			s = s.replace(' ', '_');
		}
		return s;
	}
	private static String replace(String s, Vector v){
		DSData ds;
		for(int i = 0; i<v.size(); i++){
			ds = (DSData)v.elementAt(i);
			if(ds.sel.booleanValue())
				s = U.replace(s, ds.src, ds.dst);
		}
		return s;
	}
	private static String truncate(String s){
		try{
			String t;
			int x = s.indexOf(TRUNC), y, z, i;
			while(x>0){
				y = s.indexOf('>', x);
				if(y<x) break;
				z = s.indexOf(',', x);
				if(z<x||z>y) break;
				i = U.i(s.substring(x+TRUNC.length(),z));
				t = s.substring(z+1,y);
				if(x>i+t.length())
					s = s.substring(0, i-t.length())+t+s.substring(y+1);
				else s = s.substring(0, x)+s.substring(y+1);
				x = s.indexOf(TRUNC);
			}
		}catch(NumberFormatException e){
			System.err.println(e);
		}
		return s;
	}
}
class Section{
	String str;
	float w = 1.0f;
	int max = 255;
	public Section(String s){
		int i = s.indexOf(" WEIGHT ");
		if(i>=0){
			str = s.substring(0,i);
			s = s.substring(i+8).trim();
			i = s.indexOf(':');
			if(i>=0){
				w = Float.parseFloat(s.substring(0,i));
				max = Integer.parseInt(s.substring(i+1));
			}
		} else str = s;
		i = str.indexOf('\'');
		int j = str.lastIndexOf('\'');
		if(i>=0&&j>i)
			str = str.substring(i+1,j);
		i = str.indexOf("//");
		if(i>0) str = str.substring(0,i);
		//System.out.println(str);
	}
	public String toString(){
		return str;//+" ["+w+":"+max+"]";
	}
}
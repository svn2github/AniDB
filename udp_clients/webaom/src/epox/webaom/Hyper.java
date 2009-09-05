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
 * @version 	02 (1.09,1.06)
 * @author 		epoximator
 */
package epox.webaom;

import java.util.StringTokenizer;

public class Hyper{

	public static String	S_WARN = "F00000",
							S_NAME = "006699",
							S_NUMB = "000080";

	private Hyper(){
		//static only
	}
	public static String href(String url, String name){
		return "<a href=\""+url+"\">"+name+"</a>";
	}
	public static String color(String col, String str){
		return "<font color=#"+col+">"+str+"</font>";
	}
	public static String error(String str){
		return color(S_WARN, str);
	}
	public static String name(String str){
		return color(S_NAME, str);
	}
	public static String name(Object o){
		if(o!=null)
			return color(S_NAME, o.toString());
		return "null";
	}
	public static String number(int i){
		return color(S_NUMB, ""+i);
	}
	public static String number(String str){
		return color(S_NUMB, str);
	}
	public static String enc(){
		return S_WARN+Options.S_SEP+S_NAME+Options.S_SEP+S_NUMB;
	}
	public static void dec(String str){
		if(str==null) return;
		StringTokenizer st = new StringTokenizer(str, Options.S_SEP);
		if(st.countTokens()!=3) return;
		String s = st.nextToken();
		if(s.length()==6) S_WARN = s;
		s = st.nextToken();
		if(s.length()==6) S_NAME = s;
		s = st.nextToken();
		if(s.length()==6) S_NUMB = s;
	}
}
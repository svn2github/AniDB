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
 * Copyright 2005 epoximator
 *
 * @version 	02
 * @author 		epoximator
 */
package epox.webaom.data;

import epox.util.Bits;
import epox.util.U;
import epox.webaom.ui.TableModelAlt;

public class Anime extends Base{
	private static int s0 = 1, s1 = 1;
	public static void setCol(int i){
		if(Math.abs(i)!=Math.abs(s0)) s1 = s0;
		s0 = i;
	}
	public int yea, eps, lep, pct;
	public String typ, rom, kan, eng, cat;
	public Bits pro = null;
	public Anime(int id){
		this.id = id;
	}
	public Anime(String[] s){
		int i = 0;
		id = U.i(s[i++]);
		eps = U.i(s[i++]);
		lep = U.i(s[i++]);
		yea = U.i(s[i++].substring(0,4));
		typ = s[i++];
		rom = s[i++];
		kan = U.n(s[i++]);
		eng = U.n(s[i++]);
		cat = s[i++];

		init();
	}
	public void init(){
		if(pro==null)
			pro = new Bits(eps>0?eps:lep);
	}
	public int getTotal(){
		return (eps<1?(lep<10?99:lep):eps);
	}
	public static int TPRI=0;
	public String toString(){
		switch(TPRI){
		case 1: return (kan==null?rom:kan)+" ("+eps+":"+lep+")";
		case 2: return (eng==null?rom:eng)+" ("+eps+":"+lep+")";
		default: return rom+" ("+eps+":"+lep+")";
		}
	}
	public String serialize(){
		return ""+id+S+eps+S+lep+S+yea+S+typ+S+rom+S+kan+S+eng+S+cat;
	}
	private void setorfill(int no, boolean b){
		if(!pro.set(no-1, b))
			if(no>(eps>0?eps:lep))
				if(!pro.fill(b))
					System.out.println("@ Completion "+(b?"over":"under")+"flow: "+this+" ["+typ+"] epno="+no);
	}
	public void regEp(Ep e, boolean b){
		if(Character.isDigit(e.num.charAt(0)))
			try{
				setorfill(Integer.parseInt(e.num), b);
			}catch(NumberFormatException x){
				String sa[] = e.num.split(",");
				for (int i = 0; i < sa.length; i++) {
					String sb[] = sa[0].split("-");
					switch(sb.length){
					case 2:
						for(int j=Integer.parseInt(sb[0]); j<=Integer.parseInt(sb[1]); j++)
							setorfill(j,b);
						break;
					case 1: setorfill(Integer.parseInt(sb[0]),b);
						break;
					default: System.out.println("@ Anime.regEp: Unexpected epno format!");
					}
				}
			}
		else if(e.num.charAt(0)=='O')
			try{
				if(e.eng.startsWith("Episodes ")){
					int i = e.eng.indexOf('-');

					if(i>0){
						int x = U.i(e.eng.substring(9,i));
						int j = e.eng.indexOf(' ', i);
						if(j<i) j = e.eng.length();
						int y = U.i(e.eng.substring(i+1,j));
						for(i=x-1; i<y; i++)
							pro.set(i, b);
					}
				}
			}catch(Exception x){
				//
			}
	}
	public void updatePct(){
		int max = eps;
		if(max==0) max = -lep;
		if(max==0) pct = 0;
		else pct = (pro.cnt()*100)/max;
	}
	public int getPct(){
		return pct;
	}
	/*
	 |XXX  | _ - normal
	 |  XXX| F - missing First part - could be burned
	 |XX XX| L - hole and has Last - should be able to complete
	 | XXX | E - Ends missing. fist part could be burned, probably not able to complete
	 | X X | # - there are # holes
	 |X X X| ^
	 */
	public char miss(){
		/*if(!pro.hole()) return ' ';
		return pro.last()?!pro.first()?'F':'L':'H';*/
		int x = pro.switchCount();
		//return (""+x).charAt(0);
		if(x<1) return ' ';
		if(x<2) return pro.last()?'f':' ';
		if(x<3) return pro.first()?'l':'e';
		return (char)(62+x);//(""+x).charAt(0);
	}
	public int compareTo(Object o){
		int i = comp(o, s0);
		if(i==0&&s0!=s1)
			return comp(o, s1);
		return i;
	}
	public int comp(Object obj, int i){
		if(obj instanceof Anime){
			Anime a = (Anime)obj;
			Anime b = this;
			if(i<0){
				b = a;
				a = this;
			}
			switch(Math.abs(i)-1){
				case TableModelAlt.NAME: return b.rom.compareTo(a.rom);
				case TableModelAlt.TYPE: return b.typ.compareTo(a.typ);
				case TableModelAlt.YEAR: return b.yea-a.yea;
				case TableModelAlt.NUMB: return b.size()-a.size();
				case TableModelAlt.SIZE: return (int)((b.mLs-a.mLs)/100000);
				//case AnimeModel.PRCT: return b.pro.pct()-a.pro.pct();
				case TableModelAlt.PRCT: return a.getPct()-b.getPct();
				case TableModelAlt.LAST: return a.miss()-b.miss();
					/*{
					if(b.last()==a.last())
						return 0;//a.getPct()-b.getPct();
					if(b.last()=='F') return 1;
					return -1;
				}*/
			}
		}
		return super.compareTo(obj);
	}
	public void clear(){
		super.clear();
		pro.reset();
	}
}
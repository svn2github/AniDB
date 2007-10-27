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
 * Created on 25.des.2005 16:37:58
 * Filename: aParent.java
 */
package epox.webaom.data;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;

import epox.util.U;

public class Base implements Comparable{
	protected static final char S = '|';
	private final HashMap/*!<Object,Base>*/ m_hm = new HashMap/*!<Object,Base>*/();
	private Object[] m_array = null;
	public int id;
	public long mLs;

	public Base get(int i){
		if(m_array==null) return null;
		return (Base)m_array[i];
	}
	public boolean has(Base b){
		return m_hm.containsKey(b.getKey());
	}
	public Base get(Object key){
		return (Base)m_hm.get(key);
	}
	public void add(Base b){
		Object k = b.getKey();
		if(!m_hm.containsKey(k)){
			m_hm.put(k, b);
			mLs += b.mLs;
		}//else U.err("Base: Tried to add ex obj: "+b+" ("+this+")");
	}
	public void remove(Base b){
		Object k = b.getKey();
		//if(n_hm[t].containsKey(k)){
		//	n_hm[t].remove(k);
		if(m_hm.containsKey(k)){
			m_hm.remove(k);
			mLs -= b.mLs;
			if(mLs<0){
				U.err("Base: Negative size: "+b+" ("+this+")");
				mLs = 0;
			}
		}else U.err("Base: Tried to remove non ex obj: "+b+" ("+this+")");
	}
	public void clear(){
		mLs = 0;
		Iterator it = m_hm.values().iterator();
		while(it.hasNext())
			((Base)it.next()).clear();

		m_hm.clear();
	}
	public void dump(String pre){
		System.out.println(pre+toString());
		Iterator it = m_hm.values().iterator();
		while(it.hasNext())
			((Base)it.next()).dump(pre+".");
	}

	public int size(){
		return m_hm.size();
	}
	public String toString(){
		return "Press F5 to update.";
	}
	public void mkArray(){
		m_array = m_hm.values().toArray();
		Arrays.sort(m_array);
	}
	public int compareTo(Object obj){
		return toString().compareTo(obj.toString());
	}
	public Object getKey(){
		return new Integer(id);
	}
	public String serialize(){
		return null;
	}
	public static Base getInst(String[] arg){
		if(arg==null)
			return null;
		return null;
	}
}
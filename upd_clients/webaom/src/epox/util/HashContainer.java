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
 * Created on 11.06.05
 *
 * @version 	01 (1.07)
 * @author 		epoximator
 */

package epox.util;

import jonelo.jacksum.algorithm.AbstractChecksum;

public class HashContainer{
	private String[] na, ha;
	private AbstractChecksum[] ca;
	private int size;

	public HashContainer(int s){
		size = s;
		na = new String[size];
		ha = new String[size];
		ca = new AbstractChecksum[size];
	}
	public void add(int index, String name, AbstractChecksum csum){
		na[index] = name;
		ha[index] = null;
		ca[index] = csum;
	}
	public void update(byte[] buffer, int offset, int length){
		for(int i=0; i<size; i++)
			ca[i].update(buffer, offset, length);
	}
	public void finalize(){
		for(int i=0; i<size; i++){
			ha[i] = ca[i].getHexValue();
			ca[i].reset();
		}
	}
	public String toString(){
		String res="";
		for(int i=1; i<size; i++)
			res += na[i]+": "+ha[i]+"\n";
		if(res.length()<1) return "";
		//return res;
		return res.substring(0, res.length()-1);
	}
	public String getHex(String name){
		for(int i=0; i<size; i++)
			if(na[i].equalsIgnoreCase(name))
				return ha[i];
		return null;
	}/*
	private void reset(){
		for(int i=0; i<size; i++){
			hasht[i].reset();
		}
	}*/
}
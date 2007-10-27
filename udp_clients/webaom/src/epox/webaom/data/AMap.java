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

package epox.webaom.data;

import java.util.HashMap;

public class AMap extends HashMap/*!<String,String>*/{
	public String put(String key, String value){
		if(value==null) return null;
		return (String)super.put(key, value);
	}
	public Object put(String key, int value){
		return put(key,""+value);
	}
	public Object put(String key, long value){
		return put(key,""+value);
	}
	public String[][] toArray(){
		Object[] keys = this.keySet().toArray();
		Object[] vals = this.values().toArray();
		int len = keys.length;
		String[][] res = new String[len][2];
		for(int i=0; i<len; i++){
			res[i][0] = (String)keys[i];
			res[i][1] = (String)vals[i];
		}
		return res;
	}
}
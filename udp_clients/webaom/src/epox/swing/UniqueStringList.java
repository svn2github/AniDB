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
 * @version 	1.01
 * @author 		epoximator
 */
package epox.swing;

import java.util.Vector;

import javax.swing.ListModel;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;

public class UniqueStringList implements ListModel
{
	private ListDataListener ldl;
	private Vector/*!<String>*/ table;
	private String sep;

	public UniqueStringList(String s){
		sep = s;
		table = new Vector/*!<String>*/();
	}
	public void add(String ext){
		if(!table.contains(ext))
		{
			int size = getSize();
			table.add(ext);
			ldl.intervalAdded(new ListDataEvent(this, ListDataEvent.INTERVAL_ADDED, size, size));
		}
	}
	/*public void addSilent(String ext){
		if(!table.contains(ext))
		{
			table.add(ext);
		}
	}*/
	public String removeElementAt(int i){
		String str = (String)table.remove(i);//!
		ldl.intervalRemoved(new ListDataEvent(this, ListDataEvent.INTERVAL_REMOVED, i, i));
		return str;
	}
	public void reset(){
		int i = getSize();
		table.removeAllElements();
		ldl.intervalRemoved(new ListDataEvent(this, ListDataEvent.INTERVAL_REMOVED, 0, i-1));
	}
	public boolean includes(String ext){
		return table.contains(ext);
	}
	public String toString(){
		String tmp = "";
		Object o[] = getStrings();
		for(int i=0; i<o.length; i++)
			tmp += (String)o[i]+sep;
		return tmp.trim();
	}
	public Object[] getStrings(){
		return table.toArray();
	}
	public int getSize(){
		return table.size();
	}
	public Object getElementAt(int row){
		return table.elementAt(row);
	}
	public String getStringAt(int row){
		return (String)table.elementAt(row);
	}
	public void addListDataListener(ListDataListener l){
		ldl = l;
	}
	public void removeListDataListener(ListDataListener l){
		ldl = null;
	}
}
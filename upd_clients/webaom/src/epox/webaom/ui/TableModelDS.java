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
 * @version 	1.09, 1.05
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.util.Vector;

import javax.swing.JTable;
import javax.swing.SwingConstants;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableColumnModel;

import epox.util.DSData;

public class TableModelDS extends AbstractTableModel{
	public static final int	SEL = 0,
							SRC = 1,
							DST = 2;

	private Vector/*!<DSData>*/ data;
	private String stitle0, stitle1;
	private DSData dummy;

	public TableModelDS(Vector/*<DSData>*/ d, String s0, String s1){
		setData(d);
		stitle0 = s0;
		stitle1 = s1;
		dummy = new DSData("", "", false);
	}
	public Vector/*!<DSData>*/ getData(){
		return data;
	}
	public void setData(Vector/*!<DSData>*/ d){
		data = d;
	}
	public int getColumnCount(){
		return 3;
	}
	public Class/*!<?>*/ getColumnClass(int col){
		switch (col)
		{
			case SEL: return Boolean.class;
			default: return String.class;
		}
	}
	public int getRowCount(){
		return data.size()+1;
	}
	public Object getValueAt(int row, int col){
		DSData rd;
		if(row==data.size())
			rd = dummy;
		else rd = (DSData)data.elementAt(row);//!
		switch (col)
		{
			case SEL:	return rd.sel;
			case SRC:	return rd.src;
			case DST:	return rd.dst;
		}
		return null;
	}
	public boolean isCellEditable(int row, int col){
		return true;
	}
	public void setValueAt(Object obj, int row, int col){
		DSData rd;
		if(row==data.size()){
			rd = new DSData("", "", false);
			data.add(rd);
			fireTableRowsInserted(row, row);
		}
		else rd = (DSData)data.elementAt(row);//!
		if(rd!=null)
			switch (col)
			{
				case SEL: if(rd.src.length()>0) rd.sel = (Boolean)obj; break;
				case SRC:
					rd.src = (String)obj;
					if(rd.src.length()==0)
						data.remove(rd);
				break;
				case DST: rd.dst = getString(obj); break;
			}
	}
	private String getString(Object o){
		String str = (String)o;
		DSData rd;
		for(int i=0; i<data.size(); i++){
			rd = (DSData)data.elementAt(i);//!
			if(rd.src.equals(str)) return "";
		}
		return str;
	}
	public String getColumnName(int columnIndex){
		switch (columnIndex)
		{
			case SEL:	return "Enabled";
			case SRC:	return stitle0;
			case DST:	return stitle1;
		}
			return "No such column!";
	}
	public static void formatTable(JTable table){
		TableColumnModel m = table.getColumnModel();
		m.getColumn(0).setMaxWidth(50);
		m.getColumn(1).setPreferredWidth(100);
		m.getColumn(2).setPreferredWidth(100);
		DefaultTableCellRenderer centerRend = new DefaultTableCellRenderer();
		centerRend.setHorizontalAlignment(SwingConstants.CENTER);
		m.getColumn(1).setCellRenderer(centerRend);
		m.getColumn(2).setCellRenderer(centerRend);
	}
}
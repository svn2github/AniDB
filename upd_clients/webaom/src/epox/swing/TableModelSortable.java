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

package epox.swing;

import javax.swing.table.AbstractTableModel;

public abstract class TableModelSortable extends AbstractTableModel{
	protected int[] m_idx;//, m_rev;
	private final TableSorter m_ts = new TableSorter(this);
	private int m_sort_col;

	public void reset(){
		m_sort_col = -1;
		m_ts.reset();
		m_idx = null;
	}
	protected int getRowIndex(int row){
		try{
			return m_idx[row];
		}catch(NullPointerException e){
			return row;
		}catch(ArrayIndexOutOfBoundsException e){
			return row;
		}
	}
	public void sortByColumn(int column){
		m_sort_col = column;
		sort(false);
	}
	public void sort(boolean refresh){
		if(m_sort_col<0) return;
		m_ts.sort(getIndexes(), m_sort_col, refresh);
		//for(int i=0; i<m_rev.length; i++)
		//	m_rev[m_idx[i]] = i;

		fireTableDataChanged();
	}
	private int[] getIndexes(){
		int n = getRowCount();
		if (m_idx != null && m_idx.length == n) return m_idx;

		m_idx = new int[n];
		//m_rev = new int[n];
		for (int i=0; i<n; i++)
			m_idx[i] = i;

		return m_idx;
	}
}
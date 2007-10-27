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

import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;

import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumnModel;

public class JTableSortable extends JTable{
	public HeaderListener m_hl;
	public JTableSortable(AbstractTableModel mod){
		super(mod);

		SortButtonRenderer r = new SortButtonRenderer();

		TableColumnModel m = this.getColumnModel();
		for (int i=0;i<m.getColumnCount();i++)
			m.getColumn(i).setHeaderRenderer(r);

		JTableHeader h = getTableHeader();
		m_hl = new HeaderListener(h, m, r);
		h.addMouseListener(m_hl);
	}
	private void calcRowHeight(Graphics g){
		Font f = getFont();
		FontMetrics fm = g.getFontMetrics(f);
		setRowHeight(fm.getHeight()+3);
	}
	private boolean needCalcRowHeight=true;
	public void paint(Graphics g){
		if (needCalcRowHeight) {
			calcRowHeight(g);
			needCalcRowHeight = false;
		}
		super.paint(g);
	}
	public void setFont(Font f){
		needCalcRowHeight=true;
		super.setFont(f);
	}
}
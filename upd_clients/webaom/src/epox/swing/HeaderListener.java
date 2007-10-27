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

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JCheckBoxMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

public class HeaderListener extends MouseAdapter{
	protected JTableHeader m_h;
	protected TableColumnModel m_c;
	protected SortButtonRenderer m_r;
	protected JPopupMenu m_pop = null;

	public HeaderListener(JTableHeader header, TableColumnModel columns, SortButtonRenderer renderer){
		m_h = header;
		m_c = columns;
		m_r = renderer;
		m_pop = null;
	}
	public void setMask(long mask){
		if(m_c!=null){
			m_pop = new JPopupMenu();
			for(int i=0; i<m_c.getColumnCount(); i++){
				TableColumn c = m_c.getColumn(i);
				JCheckBoxMenuItem mi = new JCheckBoxMenuItem(c.getHeaderValue().toString(), (1l<<i&mask)==1<<i);
				mi.addActionListener(new ColumnAction(m_c, c, mi));
				m_pop.add(mi);
			}
			for(int i=m_c.getColumnCount()-1; i>=0; i--)
				if((1l<<i&mask)!=1<<i)
					m_c.removeColumn(m_c.getColumn(i));
		}
	}
	private class ColumnAction implements ActionListener{
		private TableColumnModel m;
		private TableColumn c;
		private JCheckBoxMenuItem b;
		public ColumnAction(TableColumnModel m, TableColumn c, JCheckBoxMenuItem b){
			this.m = m; this.c=c; this.b = b;
		}
		public void actionPerformed(ActionEvent e){
			if(b.isSelected())
				m.addColumn(c);
			else if(m.getColumnCount()>1)
				m.removeColumn(c);
			else b.setSelected(true);
		}
	}
	public void mouseClicked(MouseEvent e){
		if(e.getButton()==MouseEvent.BUTTON1){
			int c = m_h.columnAtPoint(e.getPoint());
			int s = m_h.getTable().convertColumnIndexToModel(c);
			try {
				((TableModelSortable)m_h.getTable().getModel()).sortByColumn(s);
			} catch (Exception x) {
				//don't care
			}
		}else if(m_pop!=null)
			m_pop.show(m_h, e.getX(), e.getY());
	}
	public void mousePressed(MouseEvent e){
		if(e.getButton()==MouseEvent.BUTTON1){
			int col = m_h.columnAtPoint(e.getPoint());
			m_r.setPressedColumn(col);
			m_h.repaint();

			if(m_h.getTable().isEditing())
				m_h.getTable().getCellEditor().stopCellEditing();
		}
	}
	public void mouseReleased(MouseEvent e){
		if(e.getButton()==MouseEvent.BUTTON1){
			m_r.setPressedColumn(-1);
			m_h.repaint();
		}
	}
}
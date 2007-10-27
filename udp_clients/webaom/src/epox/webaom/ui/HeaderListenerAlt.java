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
 * Created on 26.des.2005 15:17:36
 * Filename: AnimeHeaderListener.java
 */
package epox.webaom.ui;

import java.awt.event.MouseEvent;

import javax.swing.table.TableColumnModel;


import epox.swing.HeaderListener;
import epox.swing.SortButtonRenderer;
import epox.webaom.data.Anime;

public class HeaderListenerAlt extends HeaderListener{
	private int cur = 0;
	protected JTreeTableR jttr;
	public HeaderListenerAlt(JTreeTableR tree){
		super(null, null, null);
		jttr = tree;
		m_r = new SortButtonRenderer();
		TableColumnModel model = jttr.getColumnModel();
		for (int i=0;i<model.getColumnCount();i++)
			model.getColumn(i).setHeaderRenderer(m_r);

		m_h = jttr.getTableHeader();
		m_h.addMouseListener(this);
	}
	public void mouseClicked(MouseEvent e){
		int col = m_h.columnAtPoint(e.getPoint());
		int sortCol = m_h.getTable().convertColumnIndexToModel(col)+1;
		if(cur==sortCol) sortCol *=-1;
		cur = sortCol;
		synchronized(jttr){
			Anime.setCol(sortCol);
			jttr.updateUI();
		}
	}
}

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

import java.awt.Component;
import java.awt.Insets;

import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

public class SortButtonRenderer extends JButton implements TableCellRenderer{
	int pc;

	public SortButtonRenderer(){
		pc = -1;
		setMargin(new Insets(0,0,0,0));
		setToolTipText("Right click to select columns");
	}
	public Component getTableCellRendererComponent(JTable t, Object o, boolean sel, boolean foc, int r, int c){
		setText((o == null) ? "" : o.toString());
		boolean isPressed = (c == pc);
		getModel().setPressed(isPressed);
		getModel().setArmed(isPressed);
		return this;
	}
	public void setPressedColumn(int c){
		pc = c;
	}
}
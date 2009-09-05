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
 * Created on 21.nov.2005 15:22:46
 * Filename: ChiiEmu.java
 */
package epox.swing;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

public class JPanelCommand extends JPanel implements ActionListener, Log{
	private JTextArea jta;
	private JComboBox jcb;
	private JScrollPane jsp;

	private CommandModel cm;

	public JPanelCommand(CommandModel cm, String str){
		this.cm = cm;

		jta = new JTextArea(str);
		jta.setMargin(new java.awt.Insets(2,2,2,2));
		//jta.setFont(new Font("monospaced", Font.PLAIN, 12));
		jcb = new JComboBox();
		jcb.setBackground(Color.white);

		jsp = new JScrollPane(jta);

		setLayout(new BorderLayout());
		add(jsp, BorderLayout.CENTER);
		add(jcb, BorderLayout.SOUTH);

		jcb.addActionListener(this);
		jcb.setEditable(true);
	}
	public void actionPerformed(ActionEvent e){
		if(e.getActionCommand().equals("comboBoxEdited"))
			handleCommand(jcb.getSelectedItem().toString().trim());
	}
	private void handleCommand(Object o){
		jcb.removeItem(o);
		if(o.toString().length()<1) return;
		jcb.insertItemAt(o,0);
		jcb.setSelectedItem("");
		println(o);
		cm.handleCommand(o.toString());
	}
	public void println(Object o){
		jta.append(o+"\r\n");
		jta.setCaretPosition(jta.getDocument().getLength());
	}
	public void status0(String str){
		println(str);
	}
	public void status1(String str){
		println(str);
	}
}

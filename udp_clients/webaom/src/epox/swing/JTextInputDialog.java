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
 * Created on 12.jan.2006 10:17:46
 * Filename: TextIputDialog.java
 */
package epox.swing;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class JTextInputDialog extends JDialog implements ActionListener {
	private JTextField jtf;
	private JButton ok, cl;
	private String val = null;
	private int num = -1;
	public JTextInputDialog(Frame f, String t, String v/*, boolean n*/){
		super(f, t, true);
		jtf = new JTextField(v);
		//if(n) num = 0;
		ok = new JButton("OK");
		cl = new JButton("Cancel");
		jtf.addActionListener(this);
		ok.addActionListener(this);
		cl.addActionListener(this);
		JPanel p = new JPanel();
		p.add(ok);
		p.add(cl);
		JPanel q = new JPanel();
		q.add(jtf);
		add(q, BorderLayout.CENTER);
		add(p , BorderLayout.SOUTH);
		pack();
		Dimension d = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
		Rectangle r = getBounds();
		setBounds(d.width/2-r.width/2, d.height/2-r.height/2, r.width, r.height);
	}
	public void actionPerformed(ActionEvent e) {
		if(e.getSource()==ok || e.getSource()==jtf){
			val = jtf.getText();
			if(num==0){
				try{
					num = Integer.parseInt(val.trim());
				}catch(NumberFormatException x){
					//don't care
					num = 0;
				}
				if(num>0) dispose();
				else setTitle("Only positive integers allowed.");
			}else dispose();
		}else dispose();
	}
	public String getStr(){
		setVisible(true);
		return num==0?null:val;
	}
	public int getInt(){
		num = 0;
		setVisible(true);
		return num;
	}
}

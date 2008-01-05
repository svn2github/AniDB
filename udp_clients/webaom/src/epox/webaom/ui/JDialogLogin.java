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
 * @version 	1.05, 1.03
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

import epox.util.U;
import epox.util.UserPass;
import epox.webaom.A;

public class JDialogLogin extends JDialog implements ActionListener{
	private boolean success = false;
	private JTextField usr, key;
	private JPasswordField psw;
	private JButton ok;
	private String usrt, pswt, keyt;
	public JDialogLogin(){
		super(A.frame, "Enter your AniDB username and password.", true);
		init();
	}
	private void init(){
		usr = new JTextField(A.up.usr, 20);
		psw = new JPasswordField(A.up.psw);
		key = new JTextField(A.up.key, 20);
		key.setToolTipText("Use blank if you don't care about encryption");
		ok  = new JButton("OK");
		ok.setToolTipText("<html>Login is required to access the <i>AniDB UDP Service</i> which enables<br>WebAOM to identify files and add them to your MyList.</html>");

		usr.addActionListener(this);
		psw.addActionListener(this);
		key.addActionListener(this);
		ok.addActionListener(this);

		JPanel c = new JPanel();
		c.setLayout(new GridBagLayout());
		c.setBorder(new EmptyBorder(2,0,2,0));

		GridBagConstraints cs = new GridBagConstraints();
		cs.insets = new Insets(2, 4, 2, 4);
		cs.fill = GridBagConstraints.HORIZONTAL;
		cs.anchor = GridBagConstraints.CENTER;
		cs.weighty = 1.0;

		cs.gridwidth = GridBagConstraints.RELATIVE;
		cs.weightx = 0.2;
		c.add(new JLabel("Username:"), cs);

		cs.gridwidth = GridBagConstraints.REMAINDER;
		cs.weightx = 0.8;
		c.add(usr, cs);

		cs.gridwidth = GridBagConstraints.RELATIVE;
		cs.weightx = 0.2;
		c.add(new JLabel("Password:"), cs);

		cs.gridwidth = GridBagConstraints.REMAINDER;
		cs.weightx = 0.8;
		c.add(psw, cs);

		cs.gridwidth = GridBagConstraints.RELATIVE;
		cs.weightx = 0.2;
		c.add(new JLabel("ApiPass (optional):"), cs);

		cs.gridwidth = GridBagConstraints.REMAINDER;
		cs.weightx = 0.8;
		c.add(key, cs);

		cs.weightx = 1.0;
		c.add(ok, cs);

		setContentPane(c);
		//setResizable(false);
		//setPreferredSize(new Dimension(300,200));
		pack();
		Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
		Rectangle r = this.getBounds();
		setBounds(d.width/2-r.width/2, d.height/2-r.height/2, r.width, r.height);

		if(usr.getText().length()>0)
			psw.requestFocus();
	}
	public void actionPerformed(ActionEvent e)
	{
		pswt = new String(psw.getPassword());
		usrt = usr.getText().toLowerCase();
		keyt = key.getText();
		if(usrt.length()<3)
			ok.setText("Username too short - OK");
		else if(usrt.length()>10)
			ok.setText("Username too long - OK");
		else if(!U.alfanum(usrt))
			ok.setText("Only letters and digits - OK");
		else if(A.up.ses==null && pswt.length()<4)
			ok.setText("Password too short - OK");
		//else if(pswt.indexOf('&')>=0||pswt.indexOf('=')>=0)
		//	ok.setText("Password cannot include '&' or '='.");
		else {
			A.up.usr = usrt;
			A.up.psw = pswt;
			A.up.key = keyt;
			success = true;
			dispose();
		}
	}
	public UserPass getPass(){
		setVisible(true);
		if(success)
			return new UserPass(usrt,pswt,keyt,A.up.ses);
		return null;
	}
}
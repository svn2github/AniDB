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
 * @version 	1.05, 1.01
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.JCheckBox;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import epox.webaom.Options;
import epox.webaom.data.Mylist;

public class JPanelOptMyl extends JPanel{
	private final String[] states = {"Unknown","On HDD","On CD","Deleted"};
	private JTextField tfSto, tfSou;
	private JComboBox coStt;
	private JTextArea taOth;
	private JCheckBox cbWat;

	public JPanelOptMyl() {

		tfSto = new JTextField();
		tfSou = new JTextField();
		taOth = new JTextArea();

		coStt = new JComboBox(states);
		coStt.setSelectedIndex(1);

		cbWat = new JCheckBox("Watched",false);

		GridBagConstraints cs = new GridBagConstraints();
		cs.insets = new Insets(4, 4, 4, 4);
		setLayout(new GridBagLayout());
		cs.fill = GridBagConstraints.HORIZONTAL;

		cs.weightx = 0.1;
		cs.gridwidth = 1;
		add(new JLabel("State"), cs);

		cs.weightx = 1.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(coStt, cs);

		cs.weightx = 0.1;
		cs.gridwidth = 1;
		add(new JLabel("Source"), cs);

		cs.weightx = 1.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(tfSou, cs);

		cs.weightx = 0.1;
		cs.gridwidth = 1;
		add(new JLabel("Storage"), cs);

		cs.weightx = 1.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(tfSto, cs);

		cs.weightx = 0.1;
		cs.gridwidth = 1;
		add(new JLabel("Other"), cs);

		cs.gridheight = 2;
		cs.weightx = 1.0;
		cs.weighty = 1.0;
		cs.fill = GridBagConstraints.BOTH;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(new JScrollPane(taOth), cs);

		cs.gridheight = 1;
		cs.weightx = 0.1;
		cs.weighty = 0.0;
		cs.fill = GridBagConstraints.HORIZONTAL;

		cs.gridx = 0;
		cs.gridy = 4;
		cs.gridheight = 1;
		cs.gridwidth = 1;
		add(cbWat, cs);
	}
	public Mylist getMylistData(){
		Mylist m = new Mylist();
		m.stt = coStt.getSelectedIndex();
		m.vie = cbWat.isSelected()?1:0;
		m.sto = tfSto.getText();
		m.sou = tfSou.getText();
		m.oth = taOth.getText();
		return m;
	}
	public void setEnabled(boolean en){
		cbWat.setEnabled(en);
		coStt.setEnabled(en);
		taOth.setEnabled(en);
		tfSou.setEnabled(en);
		tfSto.setEnabled(en);
		super.setEnabled(en);
	}
	public void opts(Options o){
		o.setB(Options.B_WATCHED, cbWat.isSelected());
		o.setI(Options.I_STATE, coStt.getSelectedIndex());
		o.setS(Options.S_STORAGE, tfSto.getText());
		o.setS(Options.S_SOURCEF, tfSou.getText());
		o.setS(Options.S_OTHERIN, taOth.getText());
	}
	public void optl(Options o){
		cbWat.setSelected(o.getB(Options.B_WATCHED));
		tfSto.setText(o.getS(Options.S_STORAGE));
		tfSou.setText(o.getS(Options.S_SOURCEF));
		taOth.setText(o.getS(Options.S_OTHERIN));
		coStt.setSelectedIndex(o.getI(Options.I_STATE));
	}
}

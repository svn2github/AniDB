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
 * @version 	1.09
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JSlider;
import javax.swing.JTextField;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;

import epox.webaom.Options;
import epox.webaom.net.ACon;

public class JPanelOptCon extends JPanel implements ChangeListener{
	private final int MIN_IPD = 3, MIN_PTO = 10;
	private JSlider jsTime, jsDelay;
	private JCheckBox jcbNat;
	public JButton jbPing;
	public JTextField tfHost, tfRPort, tfLPort;

	public JPanelOptCon(){
		super(new GridBagLayout());

		jsTime = new JSlider(MIN_PTO,60,20);
		jsTime.setMajorTickSpacing(10);
		jsTime.setPaintLabels(true);
		jsTime.addChangeListener(this);

		jsDelay = new JSlider(MIN_IPD,10,4);
		jsDelay.setMajorTickSpacing(1);
		jsDelay.setPaintLabels(true);
		jsDelay.setSnapToTicks(true);
		jsDelay.setToolTipText("Delay between each datagram sent to server.");

		jbPing = new JButton("Ping AniDB");

		tfHost = new JTextField(ACon.DEF_HOST);
		tfRPort = new JTextField(""+ACon.DEF_RPORT);
		tfLPort = new JTextField(""+ACon.DEF_LPORT);

		KeyAdapter on = new KeyAdapter(){
			public void keyTyped(KeyEvent e){
				if(!Character.isDigit(e.getKeyChar()))
					e.consume();
			}
		};
		tfRPort.addKeyListener(on);
		tfLPort.addKeyListener(on);

		jcbNat = new JCheckBox("Keep-Alive");

		GridBagConstraints cs = new GridBagConstraints();
		cs.insets = new Insets(2, 4, 2, 4);
		cs.anchor = GridBagConstraints.CENTER;
		cs.fill = GridBagConstraints.HORIZONTAL;
		cs.weighty = 0.1;

		add("AniDB Host", tfHost, cs);
		add("Remote Port", tfRPort, cs);

		cs.weightx = 0.0;
		cs.gridwidth = 1;
		add(new JLabel("Local Port"), cs);
		cs.weightx = 1.0;
		add(tfLPort, cs);
		cs.weightx = 0.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(jcbNat, cs);

		add("Delay (sec)", jsDelay, cs);
		add("Timeout (sec)", jsTime, cs);

		cs.weighty = 0.9;
		add(new JLabel(""), cs);

		cs.fill = GridBagConstraints.NONE;
		cs.weighty = 0.1;
		add(jbPing, cs);
	}
	private void add(String l, Component c, GridBagConstraints cs){
		cs.weightx = 0.0;
		cs.gridwidth = 1;
		add(new JLabel(l), cs);
		cs.weightx = 1.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(c, cs);
	}
	public void stateChanged(ChangeEvent ce){
		jsTime.setToolTipText(jsTime.getValue()+" sec");
	}
	public int getT(){
		return jsTime.getValue();
	}
	public int getD(){
		return jsDelay.getValue()*1000;
	}
	public boolean getN(){
		return jcbNat.isSelected();
	}
	public void setEnabled(boolean en){
		tfHost.setEnabled(en);
		tfLPort.setEnabled(en);
		tfRPort.setEnabled(en);
		jbPing.setEnabled(en);
		jsTime.setEnabled(en);
		jsDelay.setEnabled(en);
	}
	public void opts(Options o){
		o.setS(Options.S_HOSTURL, tfHost.getText());
		o.setI(Options.I_RPORT, Integer.parseInt(tfRPort.getText()));
		o.setI(Options.I_LPORT, Integer.parseInt(tfLPort.getText()));
		o.setI(Options.I_TIMEO, jsTime.getValue());
		o.setI(Options.I_DELAY, jsDelay.getValue());
		o.setB(Options.B_NATKEEP, jcbNat.isSelected());
	}
	public void optl(Options o){
		tfRPort.setText(""+o.getI(Options.I_RPORT));
		tfLPort.setText(""+o.getI(Options.I_LPORT));
		tfHost.setText(o.getS(Options.S_HOSTURL));

		int i = o.getI(Options.I_TIMEO);
		if(i<MIN_PTO) i = MIN_PTO;
		else if(i>60) i = 60;
		jsTime.setValue(i);

		i = o.getI(Options.I_DELAY);
		if(i<MIN_IPD) i = MIN_IPD;
		else if(i>10) i = 10;
		jsDelay.setValue(i);

		jcbNat.setSelected(o.getB(Options.B_NATKEEP));
	}
}
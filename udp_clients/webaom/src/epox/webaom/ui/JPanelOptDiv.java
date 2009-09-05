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
 * Created on 31.08.05
 *
 * @version 	1.09
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.io.File;
import java.util.StringTokenizer;

import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import epox.swing.JComboBoxLF;
import epox.util.HashContainer;
import epox.util.TTH;
import epox.webaom.A;
import epox.webaom.Options;

public class JPanelOptDiv extends JPanel{
	public final static int
		H_EDK = 0,
		H_CRC = 1,
		H_MD5 = 2,
		H_SHA = 3,
		H_TTH = 4;
	public JCheckBox[] cbHsh = new JCheckBox[5];
	private static String getHashName(int code){
		switch(code){
			case H_EDK: return "ed2k";
			case H_CRC: return "crc32";
			case H_MD5: return "md5";
			case H_SHA: return "sha1";
			case H_TTH: return "tth";
			default:	return "NOT HASH";
		}
	}
	public JTextField tfNewExt, tfHDirs, tfBrows, tfMyDBs, tfLogfl;
	//private JComboBox jcbModes;
	private JCheckBox jcbPpsw, jcbAdbc, jcbAlog, jcbAsav;

	public JPanelOptDiv(){
		super(new GridBagLayout());

		tfHDirs = new JTextField();
		tfBrows = new JTextField();
		tfMyDBs = new JTextField();
		tfLogfl = new JTextField();

		tfHDirs.setToolTipText("Check these directories for new files every now and then");
		tfBrows.setToolTipText("Absolute path to preferred browser");
		tfMyDBs.setToolTipText("JDBC url, press enter to connect");
		tfLogfl.setToolTipText("Absolute path to log file, press enter to enable");

		jcbAdbc = new JCheckBox("Auto db");
		jcbAdbc.setToolTipText("Load db on startup");
		jcbAlog = new JCheckBox("Auto log");
		jcbAlog.setToolTipText("Start logging to disk on startup");
		jcbAsav = new JCheckBox("Auto save");
		jcbAsav.setToolTipText("Save options on exit without asking");
		jcbPpsw = new JCheckBox("Store password");
		jcbPpsw.setToolTipText("Do you want to store your password on disk? (not cleartext)");

		GridBagConstraints cs = new GridBagConstraints();
		cs.insets = new Insets(2, 4, 2, 4);
		cs.anchor = GridBagConstraints.CENTER;
		cs.fill = GridBagConstraints.HORIZONTAL;
		cs.weighty = 0.1;

		add("Hash Dirs", tfHDirs, cs);
		add("Browser Path", tfBrows, cs);
		add("My Database", tfMyDBs, cs);
		add("Log File", tfLogfl, cs);
		add("LookAndFeel", new JComboBoxLF(A.component), cs);

		cs.gridwidth = 1;

		JPanel pHash = new JPanel(new GridBagLayout());
		for(int i = 0; i<cbHsh.length; i++){
			cbHsh[i] = new JCheckBox(getHashName(i), false);
			pHash.add(cbHsh[i], cs);
		}
		cbHsh[H_EDK].setSelected(true);
		cbHsh[H_EDK].setEnabled(false);

		JPanel pBool = new JPanel(new GridBagLayout());
		pBool.add(jcbAdbc, cs);
		pBool.add(jcbAlog, cs);
		pBool.add(jcbAsav, cs);
		pBool.add(jcbPpsw, cs);

		cs.gridwidth = GridBagConstraints.REMAINDER;

		add(pBool, cs);
		add(pHash, cs);
	}
	private void add(String l, Component c, GridBagConstraints cs){
		cs.weightx = 0.0;
		cs.gridwidth = 1;
		add(new JLabel(l), cs);
		cs.weightx = 1.0;
		cs.gridwidth = GridBagConstraints.REMAINDER;
		add(c, cs);
	}
	public boolean autoLoadDB(){
		return jcbAdbc.isSelected();
	}
	public boolean autoLog(){
		return jcbAlog.isSelected();
	}
	public boolean storePass(){
		return jcbPpsw.isSelected();
	}
	public File[] getDirs(){
		StringTokenizer st = new StringTokenizer(tfHDirs.getText(), ";");
		File dirs[] = new File[st.countTokens()];
		for(int i=0; i<dirs.length; i++)
			dirs[i] = new File(st.nextToken());
		return dirs;
	}
	public void setEnabled(boolean b){
		tfHDirs.setEnabled(b);
		for(int i=1; i<cbHsh.length; i++)
			cbHsh[i].setEnabled(b);
	}
	public void opts(Options o){
		o.setB(Options.B_HASHCRC, cbHsh[H_CRC].isSelected());
		o.setB(Options.B_HASHMD5, cbHsh[H_MD5].isSelected());
		o.setB(Options.B_HASHSHA, cbHsh[H_SHA].isSelected());
		o.setB(Options.B_HASHTTH, cbHsh[H_TTH].isSelected());
		o.setB(Options.B_STORPSW, jcbPpsw.isSelected());
		o.setB(Options.B_ALOADDB, jcbAdbc.isSelected());
		o.setB(Options.B_AUTOLOG, jcbAlog.isSelected());
		o.setB(Options.B_AUTOSAV, jcbAsav.isSelected());

//		o.setI(Options.I_USMOD, jcbModes.getSelectedIndex());

		o.setS(Options.S_HASHDIR, tfHDirs.getText());
		o.setS(Options.S_BROWSER, tfBrows.getText());
		o.setS(Options.S_MYDBURL, tfMyDBs.getText());
		o.setS(Options.S_LOGFILE, tfLogfl.getText());
	}
	public void optl(Options o){
		cbHsh[H_CRC].setSelected(o.getB(Options.B_HASHCRC));
		cbHsh[H_MD5].setSelected(o.getB(Options.B_HASHMD5));
		cbHsh[H_SHA].setSelected(o.getB(Options.B_HASHSHA));
		cbHsh[H_TTH].setSelected(o.getB(Options.B_HASHTTH));
		jcbPpsw.setSelected(o.getB(Options.B_STORPSW));
		jcbAdbc.setSelected(o.getB(Options.B_ALOADDB));
		jcbAlog.setSelected(o.getB(Options.B_AUTOLOG));
		jcbAsav.setSelected(o.getB(Options.B_AUTOSAV));

//		int i = o.getI(Options.I_USMOD);
//		jcbModes.setSelectedIndex((i<0||i>2)?0:i);

		tfHDirs.setText(o.getS(Options.S_HASHDIR));
		tfBrows.setText(o.getS(Options.S_BROWSER));
		tfMyDBs.setText(o.getS(Options.S_MYDBURL));
		tfLogfl.setText(o.getS(Options.S_LOGFILE));
	}
	public HashContainer getHashContainer(){
		try{
			int nr = 0;
			for(int i=0; i<cbHsh.length; i++)
				if(cbHsh[i].isSelected()) nr++;
			HashContainer hc = new HashContainer(nr);
			nr = 0;

			if(cbHsh[H_EDK].isSelected())
				hc.add(nr++, "ed2k", new jonelo.jacksum.algorithm.Edonkey());
			if(cbHsh[H_CRC].isSelected())
				hc.add(nr++, "crc32", new jonelo.jacksum.algorithm.Crc32());
			if(cbHsh[H_MD5].isSelected())
				hc.add(nr++, "md5", new com.twmacinta.util.MD5());
			if(cbHsh[H_SHA].isSelected())
				hc.add(nr++, "sha1", new jonelo.jacksum.algorithm.MD("SHA-1"));
			if(cbHsh[H_TTH].isSelected())
				hc.add(nr++, "tth", new TTH());
			return hc;
		}catch(java.security.NoSuchAlgorithmException e){e.printStackTrace();}
		return null;
	}
}

/*
 * Created on 09.mar.2006 15:55:57
 * Filename: JPanelAlt.java
 */
package epox.webaom.ui;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractAction;
import javax.swing.JComboBox;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.KeyStroke;

import epox.webaom.A;
import epox.webaom.Cache;

public class JPanelAlt extends JPanel {
	public JTreeTableR jttAlt;
	public JComboBox jcbAm, jcbAs, jcbAt, jcbEt;
	public JTextField jtfAp;

	public JPanelAlt(ActionListener al){
		TableModelAlt am = new TableModelAlt();
		jttAlt = new JTreeTableR(am);
		am.formatTable(jttAlt.getColumnModel());
		new HeaderListenerAlt(jttAlt);

		JScrollPane scAlt = new JScrollPane(jttAlt);
		scAlt.getViewport().setBackground(java.awt.Color.white);

		jcbAm = new JComboBox(Cache.S_SM);
		jcbAm.setSelectedIndex(Cache.mImode);
		jcbAm.setEditable(false);
		jcbAm.addActionListener(al);

		jcbAt = new JComboBox(new String[]{"Romaji", "Kanji", "English"});
		jcbAt.setEditable(false);
		jcbAt.addActionListener(al);

		jcbEt = new JComboBox(new String[]{"English", "Romaji", "Kanji"});
		jcbEt.setEditable(false);
		jcbEt.addActionListener(al);

		jcbAs = new JComboBox(new String[]{"Show all files", "Show only existing", "Show only non existing"});
		jcbAs.setEditable(false);
		jcbAs.addActionListener(al);

		jtfAp = new JTextField(20);
		jtfAp.setText(A.preg);
		jtfAp.setToolTipText("Path Regexp");
		jtfAp.addActionListener(al);

		JPanel pAltS = new JPanel();
		pAltS.add(jcbAt);
		pAltS.add(jcbEt);
		pAltS.add(jcbAm);
		pAltS.add(jcbAs);
		pAltS.add(jtfAp);

		setLayout(new BorderLayout());
		add(scAlt, BorderLayout.CENTER);
		add(pAltS, BorderLayout.SOUTH);

		JPopupMenuM pop = new JPopupMenuM(jttAlt, jttAlt);
		A.com1 = pop;
		jttAlt.addMouseListener(pop);

		jttAlt.getInputMap().put(KeyStroke.getKeyStroke("F5"),"refresh");
		jttAlt.getActionMap().put("refresh", new AbstractAction(){
			public void actionPerformed(ActionEvent e){
				updateAlt(false);
			}
		});
		jttAlt.addKeyListener(new KeyAdapterJob(jttAlt,jttAlt,this));
	}
	protected void updateAlt(boolean r){
		synchronized(A.p){
			if(r) A.cache.rebuildTree();
			jttAlt.updateUI();
		}
	}
}

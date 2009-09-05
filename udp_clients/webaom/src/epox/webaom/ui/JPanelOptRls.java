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

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.Arrays;
import java.util.StringTokenizer;
import java.util.Vector;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.border.EtchedBorder;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import epox.swing.JTableSortable;
import epox.util.DSData;
import epox.webaom.A;
import epox.webaom.RuleMenu;
import epox.webaom.Rules;
import epox.webaom.WebAOM;

public class JPanelOptRls extends JPanel implements Action, ActionListener, ItemListener{
	protected final JTextArea jta;
	private final JRadioButton jrbRen, jrbMov;
	private final JButton jbOK;

	private Rules rules;

	protected  JTable jtDef;
	protected  TableModelDS rmDef;

	public JPanelOptRls(Rules rules){
		super(new BorderLayout());
		//super(new GridLayout(2,1));
		this.rules = rules;
			//TOP
		jrbRen = new JRadioButton("Renaming (name)", true);
		jrbMov = new JRadioButton("Moving (path)", false);
		jrbRen.addItemListener(this);
		jrbMov.addItemListener(this);

		ButtonGroup group1 = new ButtonGroup();
		group1.add(jrbRen);
		group1.add(jrbMov);

		jbOK = new JButton("Apply!");
		jbOK.addActionListener(this);

		JPanel jpHead1 = new JPanel();
		jpHead1.add(jrbRen);
		jpHead1.add(jrbMov);
		jpHead1.add(jbOK);

		jta = new JTextArea(rules.getRen());
		jta.setMargin(new java.awt.Insets(2,4,2,4));

		final RuleMenu handler = new RuleMenu(jta);
		SAXParserFactory factory = SAXParserFactory.newInstance();
		try {
			SAXParser saxParser = factory.newSAXParser();
			saxParser.parse( WebAOM.class.getClassLoader().getResourceAsStream("rule-helper.xml"), handler );

			jta.addMouseListener(new MouseAdapter(){
				public void mouseClicked(MouseEvent e) {
					super.mouseClicked(e);
					if(e.getButton()==MouseEvent.BUTTON3)
						handler.getMenu().show(jta, e.getX(),e.getY());
				}
			});
			jta.setToolTipText("Right click for menu");
		}catch(Exception e){
			e.printStackTrace();
		}

		JPanel jpTop = new JPanel(new BorderLayout());
		//jpTop.setBorder(new TitledBorder("Scripts (See Wiki)"));
		jpTop.add(jpHead1, BorderLayout.NORTH);
		jpTop.add(new JScrollPane(jta));
		//BOTTOM
		rmDef = new TableModelDS(rules.mVill, "From", "To");
		jtDef = new JTableSortable(rmDef);
		//jtDef.setShowGrid(false);
		jtDef.setGridColor(Color.lightGray);
		//jtDef.setRowHeight(25);
		//jtDef.setFont(new Font("trebuchet ms",Font.PLAIN, 24));
		jtDef.getInputMap().put(KeyStroke.getKeyStroke("DELETE"),"remove");
		jtDef.getActionMap().put("remove", this);
		jtDef.getInputMap().put(KeyStroke.getKeyStroke("control UP"),"moveup");
		jtDef.getInputMap().put(KeyStroke.getKeyStroke("control DOWN"),"movedown");
		jtDef.getActionMap().put("moveup", new AbstractAction(){
			public void actionPerformed(ActionEvent e){
				moveElement(jtDef, rmDef.getData(), -1);
			}
		});
		jtDef.getActionMap().put("movedown", new AbstractAction(){
			public void actionPerformed(ActionEvent e){
				moveElement(jtDef, rmDef.getData(), 1);
			}
		});

		TableModelDS.formatTable(jtDef);

		JScrollPane sc = new JScrollPane(jtDef);
		sc.getViewport().setBackground(Color.white);
		JPanel jpBot = new JPanel(new BorderLayout());
		jpBot.add(sc, BorderLayout.CENTER);
//		jpBot.setBorder(new TitledBorder("Replace Table"));

			//MAIN
		setBorder(new EtchedBorder());
		JSplitPane jsp = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
		jsp.setDividerLocation(300);
		jsp.setOneTouchExpandable(true);
		jsp.add(jpTop);
		jsp.add(jpBot);

		add(jsp, BorderLayout.CENTER);
	}
	public void actionPerformed(ActionEvent e){
		Object obj = e.getSource();
		if(obj == jtDef)
			removeElements(rmDef.getData(), jtDef.getSelectedRows());
		else if(obj == jbOK)
			testAndApplyRules();
		jtDef.updateUI();
	}
	public void itemStateChanged(ItemEvent e){
		Object obj = e.getSource();
		if(e.getStateChange()==ItemEvent.DESELECTED){
			if(obj == jrbMov)
				rules.setMov(jta.getText());
			else// if(obj == jrbRen)
				rules.setRen(jta.getText());
		} else if(obj == jrbMov)
			jta.setText(rules.getMov());
		else //if(obj == jrbRen)
			jta.setText(rules.getRen());
	}
	public void testAndApplyRules(){
/*
		for(String line:handler.m_tf.getText().split("\\n"))
			System.out.println(line+" -> "+line.toLowerCase().matches("^((if|else|else ?if) ?([ageqrtydspnic] ?\\(.*\\))+ ?)?do ?(set|add|finish|return|fail)( .*)$"));//

 */
		String text = jta.getText(), line;
		StringTokenizer st = new StringTokenizer(text, "\r\n");
		int i = 0;
		while(st.hasMoreTokens()){
			i++;
			line = st.nextToken().toUpperCase();
			if(line.charAt(0)=='#') continue;
			if(line.indexOf("DO ")<0){
				A.dialog("Error in script @ line"+i, "All lines must include ' DO '.");
				return;
			}
		}
		if(jrbRen.isSelected())
			rules.setRen(text);
		else rules.setMov(text);
	}
	public void updateRules(){
		if(jrbRen.isSelected())
				jta.setText(rules.getRen());
		else jta.setText(rules.getMov());
	}
	private void removeElements(Vector/*!<DSData>*/ v, int rows[]){
		Arrays.sort(rows);
		for(int i=rows.length-1; i>=0; i--){
			if(rows[i]>=v.size())
				break;
			v.removeElementAt(rows[i]);
		}
		if(v.size()<=0) v.add(new DSData("", "", false));
	}
	protected void moveElement(JTable jt, Vector/*!<DSData>*/ v, int r){
		int i = jt.getSelectedRow();
		r += i;
		if(r>=v.size()||r<0) return;
		try{
			DSData o = (DSData)v.remove(i);
			v.insertElementAt(o, r);
			jt.setRowSelectionInterval(r, r);
			jt.updateUI();
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	public Object getValue(String key){return null;}
	public void putValue(String key, Object value){
		//don't care
	}
}
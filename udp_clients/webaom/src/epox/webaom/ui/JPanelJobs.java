/*
 * Created on 27.mai.2006 17:48:04
 * Filename: JPanelJobs.java
 */
package epox.webaom.ui;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.GridLayout;
import java.awt.dnd.DropTarget;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;
import java.util.StringTokenizer;

import javax.swing.JCheckBox;
import javax.swing.JPanel;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;

import epox.swing.JScrollTable;
import epox.util.U;
import epox.webaom.A;
import epox.webaom.Job;
import epox.webaom.Options;
import epox.webaom.data.AFile;

public class JPanelJobs extends JPanel implements ActionListener {
	private JTableJobs m_jtj;
	private JScrollTable m_jst;
	private TableModelJobs m_jlm;
	private JCheckBox jcbArr[];

	private int mIs = 0, mIf = 0; //status, file state
	private boolean mBu = false;

	public Container getTable(){
		return m_jst;
	}
	public void readdTable(){
		add(m_jst, BorderLayout.CENTER);
	}

	public JPanelJobs(JTableJobs j, TableModelJobs s){
		super(new BorderLayout());
		m_jtj = j;
		m_jlm = s;
		m_jst = new JScrollTable(j);

		JPanel south = new JPanel(new GridLayout(2,I_LEN/2));
		jcbArr = new JCheckBox[I_LEN];
		for (int i = 0; i < I_LEN; i++){
			jcbArr[i] = new JCheckBox(getStr(i), false);
			jcbArr[i].addActionListener(this);
			south.add(jcbArr[i]);
		}

		add(m_jst, BorderLayout.CENTER);
		add(south, BorderLayout.SOUTH);

		new DropTarget(this, m_jtj);
		new DropTarget(m_jst, m_jtj);
		m_jst.addKeyListener(m_jtj);
	}
	public void actionPerformed(ActionEvent e){
		mIs = 0;
		mIf = 0;
		if(jcbArr[I_NOR].isSelected()) mIs |= Job.H_NORMAL;
		if(jcbArr[I_PAU].isSelected()) mIs |= Job.H_PAUSED;
		if(jcbArr[I_DEL].isSelected()) mIs |= Job.H_DELETED;
		if(jcbArr[I_MIS].isSelected()) mIs |= Job.H_MISSING;
		if(jcbArr[I_WAI].isSelected()) mIs |= Job.S_DO;
		if(jcbArr[I_DOI].isSelected()) mIs |= Job.S_DOING;
		if(jcbArr[I_DON].isSelected()) mIs |= Job.S_DONE;
		if(jcbArr[I_FAI].isSelected()) mIs |= Job.S_FAILED;
		if(jcbArr[I_NET].isSelected()) mIs |= Job.D_NIO;
		if(jcbArr[I_DIS].isSelected()) mIs |= Job.D_DIO;
		if(jcbArr[I_CRC].isSelected()) mIf |= AFile.F_CRCERR;
		if(jcbArr[I_CEN].isSelected()) mIf |= AFile.F_UNC;
		mBu = jcbArr[I_UNK].isSelected();

		filter();
	}
	public void opts(Options o){
		String s = ""; TableColumn tc;
		Enumeration e = m_jtj.getColumnModel().getColumns();
		while(e.hasMoreElements()){
			tc = (TableColumn)e.nextElement();
			s += tc.getModelIndex()+","+tc.getPreferredWidth()+";";
		}
		o.setS(Options.S_JOBCOLS, s);
	}
	public void optl(Options o){
		String s = o.getS(Options.S_JOBCOLS);
		if(s==null || s.length()<1){
			//default hack
			s = "0,55;11,973;14,132";
		}
		TableColumnModel m = m_jtj.getColumnModel();
		long mask = 0;
		int a, b, i=0;
		StringTokenizer st = new StringTokenizer(s, ";");
		int[] x = new int[st.countTokens()];
		while(st.hasMoreTokens()){
			String sa[] = st.nextToken().split(",",2);
			a = U.i(sa[0]);
			b = U.i(sa[1]);
			mask |= 1<<i;
			x[i++] = a;
			m.getColumn(a).setPreferredWidth(b);
		}
		//System.out.println(Long.toBinaryString(mask));
		for(i=0; i<x.length; i++){
			m.moveColumn(x[i], i);
			for(int j=i+1; j<x.length; j++)
				if(x[j]<x[i]) x[j]++;
		}
		if(mask==0) mask = TableModelJobs.MASK;
		m_jtj.m_hl.setMask(mask);
	}
	private int m_ucnt=0;
	public void update(){
		m_ucnt++;
		if(m_ucnt%4==0 && jcbArr[I_UPD].isSelected())
			filter();
		else{
			int x = m_jst.getTopVisibleRow();
			int y = m_jst.getBottomVisibleRow();

			if(x>=0||y>x)
				m_jlm.fireTableRowsUpdated(x, y);
		}
	}
	private void filter(){
		A.jobs.filter(mIs, mIf, mBu);
		m_jlm.sort(true);
		//m_jlm.fireTableDataChanged();//<- unselects rows
		m_jtj.updateUI();
	}
	private static String getStr(int i){
		switch(i){
		case I_DEL: return "Deleted";
		case I_DIS: return "DiskIO";
		case I_DOI: return "Doing";
		case I_DON: return "Done";
		case I_FAI: return "Failed";
		case I_MIS: return "Missing";
		case I_NET: return "NetIO";
		case I_NOR: return "Normal";
		case I_PAU: return "Paused";
		case I_UPD: return "Auto Update";
		case I_WAI: return "Waiting";
		case I_UNK: return "Unknown";
		case I_CRC: return "Corrupt";
		case I_CEN: return "Censored";
		default : return "No such checkbox";
		}
	}
	private static final int
	I_NOR=0,
	I_PAU=1,
	I_WAI=2,
	I_DOI=3,
	I_DIS=4,
	I_CRC=5,
	I_UPD=6,
	I_MIS=7,
	I_DEL=8,
	I_DON=9,
	I_FAI=10,
	I_NET=11,
	I_CEN=12,
	I_UNK=13,
	I_LEN=14;
}

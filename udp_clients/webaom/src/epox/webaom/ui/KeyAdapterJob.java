/*
 * Created on 09.mar.2006 16:09:43
 * Filename: KeyAdapterJob.java
 */
package epox.webaom.ui;

import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

import javax.swing.JTable;

import epox.webaom.A;
import epox.webaom.Job;
import epox.webaom.JobMan;

public class KeyAdapterJob extends KeyAdapter {
	private JTable m_jt;
	private JTreeTableR m_tt;
	private RowModel m_rm;
	public KeyAdapterJob(JTable jt, RowModel rm){
		m_jt = jt;
		m_rm = rm;
		if(jt instanceof JTreeTableR)
			m_tt = (JTreeTableR)jt;
		else m_tt = null;
	}
	public void keyPressed(KeyEvent e) {
		try{
			int code = e.getKeyCode();
			switch(code){
				case 'R': A.gui.jpAlt.updateAlt(true); return;
				case 'D': A.p.dump("@ "); return; //A.jobs.dumpHashSet();
				case 'B': A.dumpStats(); return;
				case 'L': A.p.clear(); A.gui.jpAlt.updateAlt(false); return;
			}
			int i = m_jt.getSelectedRow();
			if(i<0) return;
			Job[] a = m_rm.getJobs(i);
			if(a==null||a.length<1) return;
			Job j = a[0];//A.jobs.get(m_rm.convertRow(i)[0]);
			boolean con = true;
			switch(code){
				case 'A': A.gui.hlGo(j.m_fa.urlAnime()); break;
				case 'M': A.gui.hlGo(j.m_fa.urlMylist()); break;
				case 'N': A.gui.hlGo(j.m_fa.urlMylistE(j.mIlid)); break;
				case 'E': A.gui.hlGo(j.m_fa.urlEp()); break;
				case 'G': A.gui.hlGo(j.m_fa.urlGroup()); break;
				case 'F': A.gui.hlGo(j.m_fa.urlFile()); break;
				case 'K': A.gui.hlGo(j.m_fa.urlExport()); break;
				case 'W': JobMan.c_watch(j); break;
				case 'X': JobMan.c_expl(j); break;
				case 'C': JobMan.c_avdump(j); A.gui.hlGo(j.m_fa.urlFile()); break;
				case 'P': JobMan.updateStatus(j, Job.H_PAUSED, true); break;
				case 'S': JobMan.updateStatus(j, Job.IDENTIFIED, true); break;
				case 'I': j.m_fa = null; JobMan.updateStatus(j, Job.HASHED, true); break;
				case ' ':
				case 10: JobMan.showInfo(j); break;
				case 39: if(m_tt!=null) m_tt.expandRow(); break;
				case 37: if(m_tt!=null) m_tt.collapseRow(); break;
				default: con = false;
			}
			if(con) e.consume();
		}catch (Exception x){
			x.printStackTrace();
		}
	}
}

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
 * @version 	1.15, 1.09, 1.07, 1.06, 1.05, 1.03, 1.00 (WebAOMJPanel)
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.BorderLayout;
import java.awt.DefaultKeyboardFocusManager;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.io.File;

import javax.swing.AbstractAction;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFileChooser;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.KeyStroke;
import javax.swing.SwingConstants;
import javax.swing.Timer;
import javax.swing.border.EmptyBorder;
import javax.swing.border.EtchedBorder;
import javax.swing.border.TitledBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import epox.swing.JPanelCommand;
import epox.swing.JPanelDebug;
import epox.swing.Log;
import epox.util.U;
import epox.webaom.A;
import epox.webaom.Cache;
import epox.webaom.ChiiEmu;
import epox.webaom.Hyper;
import epox.webaom.Job;
import epox.webaom.JobMan;
import epox.webaom.Options;
import epox.webaom.Parser;
import epox.webaom.data.Anime;
import epox.webaom.data.Ep;
import epox.webaom.net.ACon;
import epox.webaom.net.AConE;
import epox.webaom.net.AConS;
import epox.webaom.net.Pinger;

public class JPanelMain extends JPanel implements Log, ActionListener,HyperlinkListener,ChangeListener{
	protected JTableJobs jtJobs;
	protected JScrollBar jsbJobs;
	protected TableModelJobs jlmJobs;

	private JTextField jtfNewExt;
	private JEditorPaneLog jepM;
	private JTextArea jtaHash;
	private JButton[] jbButt;
	private JCheckBox jcbMa;
	private JTabbedPane jtp;

	public JProgressBar jpb0, jpb1;
	public JPanelOptRls jpOrls;
	public JPanelOptCon jpOnio;
	public JPanelOptDiv jpOdiv;
	public JPanelOptMyl jpOmyl;
	public JPanelJobs jpJob;
	public JPanelAlt jpAlt;

	protected boolean mBcrw; // cancel rec workr
	private boolean mBkill, mBdio, mBnio;
	private int mIupds = 0;
	private String mSmsg;
	private Runnable mRjsd;
	protected Timer mTdio, mTprg, mTunf, mTgui;
	public Thread mWdio, mWnio, mWdiv;

	public JPanelMain(){
		mBkill = mBdio = mBnio = false;
		init();

		mRjsd = new JobScrollDown();
		mTdio = new Timer(4000, this);
		mTprg = new Timer(1000, this);
		mTunf = new Timer(1000*60*30, this);
		mTgui = new Timer(500, this);

		mTprg.start();
		if(A.opt.load()) optl(A.opt);
		else{
			A.fha.addExt("mp4");
			A.fha.addExt("mkv");
			A.fha.addExt("ogm");
			A.fha.addExt("avi");
			jpJob.optl(A.opt); //default hack
		}
		try{
			Thread t[] = new Thread[Thread.activeCount()];
			Thread.enumerate(t);
			for(int i=0; i<t.length; i++)
				if(t[i].getName().equals("AWT-EventQueue-0"))
					t[i].setName("GUI");
		}catch(Exception e){/*don't care*/}
	}
	public void startup(){
		if(jpOdiv.autoLog())
			startLog();
		if(jpOdiv.autoLoadDB())
			startDB();
		else mTgui.start();
	}
	public void shutdown(){
		jepM.closeLogFile();
		if(mWnio!=null&&A.conn!=null&&A.conn.m_authed){
			ACon.shutdown = true;
			jbButt[B_CONN].setEnabled(false); // disable the button
			mBnio = false;
			try {
				mWnio.join();
			} catch (InterruptedException e){
				e.printStackTrace();
			}
		}
	}
	public void reset(){
		synchronized(A.p){
			if(mBdio) dioToggle();
			nioEnable(false);
			try{
				mWdio.join(1000);
			}catch(Exception x){
				//
			}
			A.db._shutdown();
			jpOdiv.tfMyDBs.setEnabled(true);
			A.p.clear();
			A.jobs.clear();
			A.cache.clear();
			A.jobc.reset();
			jlmJobs.reset();
			jtJobs.updateUI();
			jpAlt.jttAlt.updateUI();
			System.gc();
			System.runFinalization();
			System.gc();
		}
	}
	private void init(){
							/*	+---------------------------+
								|          BUTTONS          | NORTH/TOP
								+---------------------------+
								|RLS|OPT|JOB|ALT|LOG|INF|DEB|
								|                           | CENTER
								|       (TABBED PANE)       |
								|                           |
								+---------------------------+
								|       PROGRESSBARS!       | SOUTH
								+---------------------------+	*/

		////////////////////////////////BUTTONS/////////////////////////////////
		jbButt = new JButton[S_IBAL];
		JPanel pButtons = new JPanel(new GridLayout(1, jbButt.length, 2, 2));
		pButtons.setBorder(new EmptyBorder(2,2,2,2));
		for(int i = 0; i<jbButt.length; i++){
			jbButt[i] = new JButton(getName(i));
			jbButt[i].addActionListener(this);
			jbButt[i].setToolTipText(getToolTip(i));
			pButtons.add(jbButt[i]);
		}
		////////////////////////////////OPTIONS/////////////////////////////////
		//FILE OPTION PANEL
		jpOmyl = new JPanelOptMyl();

		//EXTENSIONs PANEL
		jtfNewExt = new JTextField();
		jtfNewExt.addActionListener(this);

		final JList jlExt = new JList(A.fha.m_ext);
		jlExt.getInputMap().put(KeyStroke.getKeyStroke("DELETE"),"pressed");
		jlExt.getActionMap().put("pressed", new AbstractAction(){
			public void actionPerformed(ActionEvent e){
				int i[] = jlExt.getSelectedIndices();
				java.util.Arrays.sort(i);
				for(int j=i.length-1; j>=0; j--)
					A.fha.removeExt(i[j]);
				jlExt.clearSelection();
			}
		});

		JPanel pExt = new JPanel(new BorderLayout());
		pExt.add(jtfNewExt, BorderLayout.NORTH);
		pExt.add(new JScrollPane(jlExt));
		pExt.setBorder(new TitledBorder("Extensions"));

		//OTHER PANEL
		jpOdiv = new JPanelOptDiv();
		jpOdiv.tfMyDBs.addActionListener(this);
		jpOdiv.tfLogfl.addActionListener(this);
		JPanel pOther = new JPanel(new BorderLayout());
		pOther.setBorder(new TitledBorder("Other"));
		pOther.add(jpOdiv, BorderLayout.CENTER);

		//CONNECTION OPTIONS PANEL
		jpOnio = new JPanelOptCon();
		jpOnio.jbPing.addActionListener(this);
		JPanel pCont = new JPanel(new BorderLayout());
		pCont.setBorder(new TitledBorder("Connection"));
		pCont.add(jpOnio, BorderLayout.CENTER);

		//FILE OPTIONS
		jcbMa = new JCheckBox("Add files to mylist automatically", true);
		jcbMa.addChangeListener(this);
		JPanel pFile = new JPanel(new BorderLayout());
		pFile.setBorder(new TitledBorder("Mylist"));
		pFile.add(jcbMa, BorderLayout.NORTH);
		pFile.add(jpOmyl, BorderLayout.CENTER);

		//THE PANEL
		JPanel pOptions = new JPanel(new GridLayout(2,1,0,0));
		pOptions.setBorder(new EtchedBorder());

		JPanel pOn = new JPanel(new GridLayout(1,2,0,0));
		JPanel pOs = new JPanel(new BorderLayout());

		pOptions.add(pOn);
		pOptions.add(pOs);

		pOn.add(pCont);
		pOn.add(pFile);
		pOs.add(pOther, BorderLayout.CENTER);
		pExt.setPreferredSize(new Dimension(100,100));
		pOs.add(pExt, BorderLayout.EAST);

		//////////////////////////////HTML LOG PANE/////////////////////////////
		jepM = new JEditorPaneLog();
		jepM.addHyperlinkListener(this);

		JScrollPane scM = new JScrollPane(jepM);
		jsbJobs = scM.getVerticalScrollBar();

		////////////////////////////////INFO PANE///////////////////////////////
		JTextArea jtaInfo = new JTextArea(A.getFileString("info.txt"));
		jtaInfo.setEditable(false);
		jtaInfo.setMargin(new Insets(2,2,2,2));

		////////////////////////////////HASH PANE///////////////////////////////
		jtaHash = new JTextArea();

		///////////////////////////////RULES PANE///////////////////////////////
		jpOrls = new JPanelOptRls(A.rules);

		////////////////////////////////JOBS PANE///////////////////////////////
		jlmJobs = new TableModelJobs(A.jobs);
		A.jobs.jlm = jlmJobs;
		jtJobs = new JTableJobs(jlmJobs);
		TableModelJobs.formatTable(jtJobs);
		jpJob = new JPanelJobs(jtJobs, jlmJobs);

		////////////////////////////////ALT VIEW////////////////////////////////
		jpAlt = new JPanelAlt(this);

		//////////////////////////////CHII EMULATOR/////////////////////////////
		ChiiEmu cm = new ChiiEmu(A.conn);
		JPanelCommand jcp = new JPanelCommand(cm, "Implemented commands: !uptime,!mystats,!anime,!group,!randomanime,!mylist,!state,!watched,!storage,!font\nTo test API directly start command with '?'.\nLogin/logout is done automatically, no need to set s=.\n");
		cm.setLog(jcp);

		//////////////////////////////TABBED PANE///////////////////////////////
		jtp = new JTabbedPane();
		jtp.addTab("Rules", jpOrls);
		jtp.addTab("Options", pOptions);
		jtp.addTab("Jobs", jpJob);
		jtp.addTab("Alt", jpAlt);
		jtp.addTab("Log", scM);
		jtp.addTab("Hash", new JScrollPane(jtaHash));
		jtp.addTab("Info", new JScrollPane(jtaInfo));
		jtp.addTab("Debug", new JPanelDebug(null));
		jtp.addTab("Chii Emu", jcp);
		jtp.setSelectedIndex(6);

		//////////////////////////////PROGRESSBARS//////////////////////////////
		jpb0 = new JProgressBar(SwingConstants.HORIZONTAL, 0, 1000);
		jpb0.setString("Welcome to WebAOM!");
		jpb0.setStringPainted(true);
		jpb1 = new JProgressBar(SwingConstants.HORIZONTAL, 0, 1000);
		jpb1.setString(A.S_VER);
		jpb1.setStringPainted(true);
		JPanel pProgress = new JPanel(new GridLayout(2,1));
		pProgress.add(jpb0);
		pProgress.add(jpb1);
		//////////////////////////////////MAIN//////////////////////////////////
		setLayout(new BorderLayout());
		add(pButtons, BorderLayout.NORTH);
		add(jtp, BorderLayout.CENTER);
		add(pProgress, BorderLayout.SOUTH);
		//////////////////////////////////END///////////////////////////////////

		KeyboardFocusManager kbfm = KeyboardFocusManager.getCurrentKeyboardFocusManager();
		kbfm.addKeyEventDispatcher(new DefaultKeyboardFocusManager(){
			public boolean dispatchKeyEvent(KeyEvent e) {
				if(e.getKeyCode()== KeyEvent.VK_F9 && (e.getID()==KeyEvent.KEY_PRESSED)){
					reset();
					return true;
				}
				return super.dispatchKeyEvent(e);
			}
		});
	}
	/*public void updateJobTable(Job j){
		//jlmJobs.updateRow(j);
	}*/
	public void setEnabled_dio(boolean b){
		jpOdiv.setEnabled(b);
	}
	public void setEnabled_nio(boolean b){
		if(!killed()){
			jpOnio.setEnabled(b);
			jcbMa.setEnabled(b);
			jpOmyl.setEnabled(b&&A.autoadd);
		}
	}
	public void nioEnable(boolean b){
		if(!killed()){
			if(b) jbButt[B_CONN].setText(S_NIOD);
			else{
				jbButt[B_CONN].setText(S_NIOE);
				mBnio = false;
			}
			jbButt[B_CONN].setEnabled(true);
		}
	}
	public void fatal(boolean b){
		if(b){
			if(mBdio) dioToggle();
			setEnabled_dio(false);
			setEnabled_nio(false);
			for(int i=1; i<jbButt.length; i++)
				jbButt[i].setEnabled(false);
			mTunf.start();
		}else{
			mTunf.stop();
			setEnabled_dio(true);
			setEnabled_nio(true);
			for(int i=1; i<jbButt.length; i++)
				jbButt[i].setEnabled(true);
		}
	}
	public String getHost(){
		return jpOnio.tfHost.getText();
	}
	public int getRPort(){
		return Integer.parseInt(jpOnio.tfRPort.getText());
	}
	public int getLPort(){
		return Integer.parseInt(jpOnio.tfLPort.getText());
	}
	/////////////////////////////IMPLEMENTATIONS////////////////////////////////
	public void hyperlinkUpdate(HyperlinkEvent e){
		if(e.getEventType()==HyperlinkEvent.EventType.ACTIVATED)
			hlGo(e.getDescription());
		else
		if(e.getEventType()==HyperlinkEvent.EventType.ENTERED){
			mSmsg = jpb0.getString();
			jpb0.setString(e.getDescription());
		}else
		if(e.getEventType()==HyperlinkEvent.EventType.EXITED)
			jpb0.setString(mSmsg);
	}
	public void actionPerformed(ActionEvent e){
		Object source = e.getSource();
		if(source==jbButt[B_SELF]) selectFiles();
		else if(source==jbButt[B_SELD]) selectDirs();
		else if(source==jbButt[B_HASH]) dioToggle();
		else if(source==jbButt[B_CONN]) nioToggle();
		else if(source==jbButt[B_EXPO]){
			if(mWdiv!=null) return;
			mWdiv = new ExImp(false);
			mWdiv.start();
		}
		else if(source==jbButt[B_IMPO]){
			if(mWdiv!=null) return;
			mWdiv = new ExImp(true);
			mWdiv.start();
		}
		else if(source==jbButt[B_SAVE]){
			opts(A.opt);
			A.opt.save();
		}
		else if(source==jbButt[B_WIKI]){
			hlGo("http://wiki.anidb.info/w/WebAOM");
		}
		else if(source==mTdio) dioStart();
		else if(source==mTprg)
			//System.err.println(A.frame.getFocusOwner());
			updateProgressbar();
		else if(source==mTunf){
			mBkill = false;
			fatal(false);
		}else if(source==mTgui){
			if(jtp.getSelectedComponent()==jpJob)
				jpJob.update();
		}
		else if(source==jpOnio.jbPing) new Pinger(this);
		else if(source==jpOdiv.tfMyDBs) startDB();
		else if(source==jtfNewExt){
			A.fha.addExt(jtfNewExt.getText());
			jtfNewExt.setText("");
		}else if(source==jpOdiv.tfLogfl)
			startLog();
		else if(source==jpAlt.jcbAm){
			Cache.mImode = jpAlt.jcbAm.getSelectedIndex();
			jpAlt.updateAlt(true);
		}else if(source==jpAlt.jcbAs){
			Cache.hideN = jpAlt.jcbAs.getSelectedIndex()==1;
			Cache.hideE = jpAlt.jcbAs.getSelectedIndex()==2;
			jpAlt.updateAlt(true);
		}else if(source==jpAlt.jtfAp){
			String s = jpAlt.jtfAp.getText();
			if(s.length()<1)
				A.preg = null;
			else A.preg = s;
			jpAlt.updateAlt(true);
		}else if(source==jpAlt.jcbAt){
			Anime.TPRI = jpAlt.jcbAt.getSelectedIndex();
			jpAlt.updateAlt(false);
		}else if(source==jpAlt.jcbEt){
			Ep.TPRI = jpAlt.jcbEt.getSelectedIndex();
			jpAlt.updateAlt(false);
		}
	}
	private void startLog(){
		 if(jepM.openLogFile(jpOdiv.tfLogfl.getText()))
			jpOdiv.tfLogfl.setEnabled(false);
	}
	private void startDB(){
		if(A.db._ok() || mWdiv!=null) return;
		mWdiv = new InitDB(jpOdiv.tfMyDBs);
		mWdiv.start();
	}
	public void stateChanged(ChangeEvent e){
		Object source = e.getSource();
		if(source==jcbMa){
			A.autoadd = jcbMa.isSelected();
			jpOmyl.setEnabled(A.autoadd);
		}
	}
	/////////////////////////////////DIV METH///////////////////////////////////
	public void dioToggle(){
		mBdio = !mBdio;
		if(mBdio){
			jbButt[B_HASH].setText(S_DIOD);
			dioStart();
			mTdio.start();
		}else{
			jbButt[B_HASH].setText(S_DIOE);
			mTdio.stop();
		}
	}
	private void dioStart(){
		if(mWdio==null){
			if(!A.jobs.workForDio()){
				Thread rw = new RecDir(jpOdiv.getDirs(), true);
				rw.start();
				Thread.yield();
				if(!A.jobs.workForDio()) return;
			}
			mWdio = new Thread(A.dio, "DiskIO");
			mWdio.start();
		}
	}
	/*public void dioStop(){
		mBdio = false;
	}*/
	private void nioToggle(){
		jbButt[B_CONN].setEnabled(false); // disable the button
		mBnio = !mBnio; // change the state
		if(mBnio){ // we want it to run
			setEnabled_nio(false); // freeze settings
			mBnio = nioEnableInt(); // try to start
			if(!mBnio){ // failed
				jbButt[B_CONN].setEnabled(true); // enable the button
				setEnabled_nio(true); // unfreeze settings
			}
		}
	}
	private boolean nioEnableInt(){
		if(!killed()&&mWnio==null){
			//if(A.up.usr==null||A.up.psw==null||A.up.key==null)
			if(new JDialogLogin().getPass()==null)
				return false;
			//A.conn = getConnection();
			mWnio = new Thread(A.nio, "NetIO");
			mWnio.start();
			return true;
		}
		return false;
	}
	public void kill(){
		mBkill = true;
	}
	public boolean dioOK(){
		return mBdio && !mBkill;
	}
	public boolean nioOK(){
		return mBnio && !mBkill;
	}
	public boolean killed(){
		return mBkill;
	}
	public void msg(String msg){
		A.dialog("Message", msg);
	}
	public void msg(String title, String msg){
		JOptionPane.showMessageDialog(A.component, msg, title, JOptionPane.WARNING_MESSAGE);
	}
	protected class JobScrollDown implements Runnable{
		public void run(){
			if(!jsbJobs.getValueIsAdjusting())
				jsbJobs.setValue(jsbJobs.getMaximum());
		}
	}
	public void println(Object o){
		jepM.println(o.toString());
		if(jepM.isVisible())
			javax.swing.SwingUtilities.invokeLater(mRjsd);
	}
	public void status0(String str){
		jpb0.setString(str);
		mSmsg = str;
	}
	public void status1(String str){
		jpb1.setString(str);
	}
	public void printHash(String msg){
		jtaHash.append(msg+"\r\n");
	}
	public void updateProgressbar(){
		jpb1.setValue(A.jobc.getProgress());
		if(A.frame!=null)
			A.frame.setTitle("WebAOM "+A.S_VER+" "+A.jobc.getStatus());
		if(((mIupds++)%10)==0)
			System.gc();
	}
	public void hlGo(String url){
		Runtime rt = Runtime.getRuntime();
		try{
			U.out(url);
			String path = jpOdiv.tfBrows.getText();
			if(path.length()>0)
				rt.exec(path+" "+url);
			else
				rt.exec("rundll32 url.dll,FileProtocolHandler \""+url+"\"");
		}catch(java.io.IOException f){f.printStackTrace();}
	}
	public AConE getConnection(){
		A.usetup = new AConS(getHost(), getRPort(), getLPort(), jpOnio.getT(), jpOnio.getD(), 3, jpOnio.getN());
		AConE a = new AConE(this, A.usetup);
		a.set(A.up.usr, A.up.psw, A.up.key);
		return a;
	}
	///////////////////////////////////OPTIONS//////////////////////////////////
	public void opts(Options o){
		o.setB(Options.B_ADDFILE, jcbMa.isSelected());
		o.setS(Options.S_HTMLCOL, Hyper.enc());
		o.setS(Options.S_USRNAME, A.up.get(jpOdiv.storePass()));

		jpOmyl.opts(o);
		A.fha.opts(o);
		jpOnio.opts(o);
		jpOdiv.opts(o);
		A.rules.opts(o);
		jpJob.opts(o);

		o.setS(Options.S_PATHREG, A.preg);
		o.setS(Options.S_FONTSTR, A.font);
		o.setS(Options.S_LOGHEAD, JEditorPaneLog.HEAD);
	}
	public void optl(Options o){
		try{
			jcbMa.setSelected(o.getB(Options.B_ADDFILE));
			Hyper.dec(o.getS(Options.S_HTMLCOL));
			A.up.set(o.getS(Options.S_USRNAME));
			jpOmyl.optl(o);
			A.fha.optl(o);
			jpOnio.optl(o);
			jpOdiv.optl(o);
			A.rules.optl(o);
			jpJob.optl(o);
			jpOrls.updateRules();

			String preg = o.getS(Options.S_PATHREG);
			if(preg.length()>0){
				A.preg = preg;
				jpAlt.jtfAp.setText(preg);
			}
			A.font = o.getS(Options.S_FONTSTR);
			jepM.setHeader(o.getS(Options.S_LOGHEAD));
		}catch(Exception e){
			e.printStackTrace();
			System.out.println("! Options file is outdated. Could not load.");
			println("Options file is outdated. Could not load.");
		}
	}
	/////////////////////////////////ADD FILES//////////////////////////////////
	private void selectFiles(){
		mBcrw = true;
		if(mWdiv != null) return;
		JFileChooser fc = new JFileChooser();
		fc.setFileFilter(A.fha.m_ff);
		fc.setMultiSelectionEnabled(true);
		if(A.dir!=null) fc.setCurrentDirectory(new File(A.dir));
		int option = fc.showDialog(A.component, "Select File(s)");
		if(option == JFileChooser.APPROVE_OPTION)
			select(fc.getSelectedFiles());
		else A.dir = fc.getCurrentDirectory().getAbsolutePath();
	}
	private void selectDirs(){
		mBcrw = true;
		if(mWdiv != null) return;
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		fc.setMultiSelectionEnabled(true);
		if(A.dir!=null) fc.setCurrentDirectory(new File(A.dir));
		int option = fc.showDialog(A.component, "Select Directory(ies) (recursive)");
		if(option == JFileChooser.APPROVE_OPTION)
			select(fc.getSelectedFiles());
		else A.dir = fc.getCurrentDirectory().getAbsolutePath();
	}
	public void select(File[] files){
		if(mWdiv != null){
			A.dialog("Message", "There is already a thread like this running.");
			return;
		}
		if(files.length<=0) return;
		if(files[0].getParent()!=null) A.dir = files[0].getParent();
		else A.dir = files[0].getAbsolutePath();
		mWdiv = new RecDir(files, false);
		mWdiv.start();
	}
	private class RecDir extends Thread{
		private File[] dirs;
		private int nd = 0;
		private boolean print;

		public RecDir(File[] d, boolean hdir){
			super("RecDir");
			dirs = d;
			mBcrw = false;
			print = !hdir && mWdio==null;
		}
		public void run(){
			long t0 = System.currentTimeMillis();
			setEnabled(false);
			int nr=0;
			for(int i=0; i<dirs.length; i++){
				String parent = dirs[i].getParent();
				if(parent!=null && parent.startsWith("\\\\"))
					msg("Windor network paths not supported: "+dirs[i]);
				else
					nr += addFileRecursive(dirs[i]);
			}
			if(print){
				String s = "";
				if(nr==1)
					s = "Added one file in "+(System.currentTimeMillis()-t0)+ " ms.";
				else if(nr>1)
					s = "Added "+nr+" files in "+(System.currentTimeMillis()-t0)+ " ms.";
				status0(s);
				if(s.length()>0)
					println(s);
			}
			setEnabled(true);
			Thread.yield();
			mWdiv = null;
		}
		private int addFileRecursive(File file){
			if(mBcrw) return 0;
			if(file.isDirectory()){
				if(print && (nd++)%100==0) status0("Checking: "+file);
				int nr=0;
				File[] files = file.listFiles(A.fha.m_ff);
				if(files==null) return 0;
				for(int i=0; i<files.length; i++)
					nr+=addFileRecursive(files[i]);
				return nr;
			}
			if(A.fha.addFile(file)) return 1;
			return 0;
		}
	}
	private class InitDB extends Thread{
		private JTextField jt;
		public InitDB(JTextField jt){
			super("InitDB");
			this.jt = jt;
		}
		public void run(){
			mTgui.stop();
			jt.setEnabled(false);
			if(A.db._init(jt.getText())){
				long t0 = System.currentTimeMillis();
				//A.mem3 = A.getUsed();
				A.db.getJobs();
				Object[] o = A.jobs.array();
				Job j;

				//A.mem4 = A.getUsed();

				A.db.debug = false;
				for(int i=0; i<o.length; i++){
					j = (Job)o[i];
					j.mBf = false;
					A.cache.gatherInfo(j, false);
					if(j.getStatus()==Job.MOVEWAIT)
						JobMan.updatePath(j);
					//updateJobTable(j);
				}
				int m = (int)(System.currentTimeMillis()-t0);
				println("Loaded db in "+Hyper.number(m)+" ms. "+Hyper.number(A.jobs.size())+" files found.");
				jpAlt.updateAlt(true);
			} else jt.setEnabled(true);
			A.db.debug = true;
			mWdiv = null;

			//A.mem5 = A.getUsed();
			//A.memstats();
			mTgui.start();
		}
	}
	private class ExImp extends Thread{
		private boolean imp = true;
		public ExImp(boolean b){
			super("Im/Ex");
			imp = b;
		}
		public void run(){
			try{
			if(imp){
				Parser.importDB();
				jpAlt.updateAlt(true);
			}
			else Parser.exportDB();
			}catch(Exception e){
				e.printStackTrace();
			}
			mWdiv = null;
		}
	}
	private final static int	B_WIKI=0,
								B_SELF=1,
								B_SELD=2,
								B_HASH=3,
								B_CONN=4,
								B_SAVE=5,
								B_EXPO=6,
								B_IMPO=7,
								S_IBAL=8;
	private static final String
	S_NIOE = "Login",
	S_NIOD = "Log out",
	S_DIOE = "Start",
	S_DIOD = "Stop";
	private static String getName(int code){
		switch(code){
		case B_SELF: return "Files...";
		case B_SELD: return "Folders...";
		case B_HASH: return S_DIOE;
		case B_CONN: return S_NIOE;
		case B_SAVE: return "Save opt";
		case B_WIKI: return "Help!";
		case B_EXPO: return "Export";
		case B_IMPO: return "Import";
		default:	 return "No text!";
		}
	}
	private static String getToolTip(int code){
		switch(code){
		case B_SELF: return "Add files you want to hash";
		case B_SELD: return "Add folders with files you want to hash";
		case B_HASH: return "Start/stop the disk operations thread. (Hashing and moving)";
		case B_CONN: return "Log on / log off the AniDB UDP Service";
		case B_SAVE: return "Save the options to disk";
		case B_WIKI: return "Check out the documentation @ AniDB WIKI";
		case B_EXPO: return "Export loaded data";
		case B_IMPO: return "Import exported data";
		default:	 return "No help!";
		}
	}
}
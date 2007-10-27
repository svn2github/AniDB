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
 * Created on 03.09.05
 *
 * @version 	1.09
 * @author 		epoximator
 */
package epox.webaom.ui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JFileChooser;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JTable;

import epox.av.AVInfo;
import epox.swing.JTextInputDialog;
import epox.webaom.A;
import epox.webaom.Job;
import epox.webaom.JobMan;

public class JPopupMenuM extends JPopupMenu implements MouseListener, ActionListener{
	protected final JTable jt;
	protected final RowModel jlm;
	protected MenuWorker worker = null;
	private String dir = null;
	private JMenuItem items[];

	public JPopupMenuM(final JTable jt, final RowModel jlm){
		this.jt = jt;
		this.jlm = jlm;

		items = new JMenuItem[iCommands];
		for(int i=0; i<iCommands; i++)
			if(separator(i))
				this.addSeparator();
			else{
				items[i] = new JMenuItem(commandText(i));
				items[i].addActionListener(this);
				items[i].setActionCommand(""+i);
				this.add(items[i]);
			}
	}
	public void stop(){
		try {
			worker.run = false;
		} catch (Exception e) {
			//don't care
		}
	}
	public void mouseEntered(MouseEvent e){/*don't care*/}
	public void mouseExited(MouseEvent e){/*don't care*/}
	public void mousePressed(MouseEvent e){
		if(jt instanceof JTableJobs)
			((JTableJobs)jt).upd = false;
	}
	public void mouseReleased(MouseEvent e){
		if(jt instanceof JTableJobs)
			((JTableJobs)jt).upd = true;
	}
	public void mouseClicked(MouseEvent e){
		if(worker==null&&(e.getModifiers()&InputEvent.BUTTON3_MASK)==InputEvent.BUTTON3_MASK){
			items[REMOVE_DB].setEnabled(A.db._ok());
			items[PARSE].setEnabled(AVInfo.ok());
			this.updateUI();
			show(jt, e.getX(), e.getY());
		}
	}
	public void actionPerformed(ActionEvent e){
		if(jt.getSelectedRowCount()>0)
		worker = new MenuWorker(Integer.parseInt(e.getActionCommand()));
	}
	private class MenuWorker extends Thread{
		int cmd;
		public boolean run = true;
		public MenuWorker(int c){
			super("pMenu");
			cmd = c;
			start();
		}
		public void run(){
			ie:
			if(single(cmd)) try{
				commandSingle(cmd, jlm.getJobs(jt.getSelectedRow())[0]);
			}catch(ArrayIndexOutOfBoundsException e){
				//
			}else{
				String arg = null;
				if(cmd==SET_FOLDER||cmd==SET_PAR_FLD){
					arg = getFolder();
					if(arg==null) break ie;
				}
				int rows[] = jt.getSelectedRows();

				jt.clearSelection();
				for(int i=0; i<rows.length&&run; i++)
					command(cmd, jlm.getJobs(rows[i]), arg);
			}
			worker = null;
		}
	}
	void command(int cmd, Job[] rows, String arg){
		if(rows.length<1) return;
		if(cmd==EDIT_PATH){
			arg = new JTextInputDialog(A.frame, "Edit path", rows[0].getFile().getParent()).getStr();
			if(arg==null||arg.length()<2) return;
		}
		for(int i=0; i<rows.length; i++)
			if(rows[i]!=null)
				command0(cmd, rows[i],arg);
	}
	void command0(int cmd, Job j, String arg){
		switch(cmd){
			case PAUSE:			JobMan.updateStatus(j, Job.H_PAUSED, true);
				break;
			case REHASH:		JobMan.updateStatus(j, Job.HASHWAIT, true);
				break;
			case REID:			j.m_fa = null; JobMan.updateStatus(j, Job.HASHED, true);
				break;
			case READD:			JobMan.updateStatus(j, Job.ADDWAIT, true);
				break;
			case REMOVE:		JobMan.updateStatus(j, Job.REMWAIT, true);
				break;
			case APPLY_RULES:	JobMan.updateStatus(j, Job.IDENTIFIED, true);
				break;
			case SET_FINISHED:	JobMan.updateStatus(j, Job.FINISHED, true);
				break;
			case SET_FOLDER:	if(arg!=null) JobMan.setPath(j, arg, false);
				break;
			case SET_PAR_FLD:	if(arg!=null) JobMan.setPath(j, arg, true);
				break;
			case REMOVE_DB:		JobMan.updateStatus(j, Job.H_DELETED, true);
				break;
			case RESTORE_NAME:	JobMan.restoreName(j);
				break;
			case EDIT_PATH:		if(arg!=null) JobMan.setPath(j, arg, false);
				break;
			case PARSE:			JobMan.updateStatus(j, Job.PARSEWAIT, true);
				break;
		}
	}
	void commandSingle(int cmd, Job j){
		switch(cmd){
			case SHOW_INFO:		JobMan.showInfo(j);
				break;
			case WATCH_NOW:		JobMan.c_watch(j);
				break;
			case EXPLORER:		JobMan.c_expl(j);
				break;
			case EDIT_NAME:
		JobMan.setName(j,new JTextInputDialog(A.frame, "Edit name", j.getFile().getName()).getStr());
				break;
			case SET_FID:		j.mIfid = new JTextInputDialog(A.frame, "Insert fid", "").getInt();
								if(j.mIfid>0){
									j.m_fa = null;
									JobMan.updateStatus(j, Job.HASHED);
									//jlm.updateRow(j);
								}
				break;
		}
	}
	String getFolder(){
		JFileChooser fc = new JFileChooser();
		fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
		fc.setMultiSelectionEnabled(false);
		if(dir!=null) fc.setCurrentDirectory(new java.io.File(dir));
		int option = fc.showDialog(A.component, "Select Directory");
		if(option == JFileChooser.APPROVE_OPTION){
			dir = fc.getSelectedFile().getAbsolutePath();
			return dir;
		}
		return null;
	}
	private static final int	PAUSE			= 0,
								SEPARATOR_0	 	= 1,
								SHOW_INFO		= 2,
								WATCH_NOW		= 3,
								EXPLORER		= 4,
								SEPARATOR_1	 	= 5,
								REHASH			= 6,
								REID			= 7,
								READD			= 8,
								REMOVE			= 9,
								APPLY_RULES		= 10,
								SEPARATOR_2		= 11,
								SET_FINISHED	= 12,
								RESTORE_NAME	= 13,
								SET_FOLDER		= 14,
								SET_PAR_FLD		= 15,
								EDIT_PATH		= 16,
								EDIT_NAME		= 17,
								SEPARATOR_3	 	= 18,
								PARSE			= 19,
								SET_FID			= 20,
								REMOVE_DB		= 21,
								iCommands		= 22;
	public static String commandText(int i){
		switch(i){
			case APPLY_RULES:	return "Apply Rules";
			case SHOW_INFO:		return "Show Info";
			case EXPLORER:		return "Explore Folder";
			case WATCH_NOW:		return "Watch Now";
			case SET_FINISHED:	return "Set Finished";
			case SET_FOLDER:	return "Set Folder";
			case SET_PAR_FLD:	return "Set Parent Folder";
			case REMOVE_DB:		return "Remove from DB";
			case RESTORE_NAME:	return "Restore Name";
			case PAUSE:			return "Pause";
			case REHASH:		return "Rehash";
			case REID:			return "Identify";
			case READD:			return "Add to mylist";
			case EDIT_PATH:		return "Edit Folder Path";
			case EDIT_NAME:		return "Edit File Name";
			case SET_FID:		return "Set fid (force)";
			case REMOVE:		return "Remove from mylist";
			case PARSE:			return "Parse with avinfo";
			default: return "fook";
		}
	}
	public static boolean separator(int i){
		switch(i){
			case SEPARATOR_0:
			case SEPARATOR_1:
			case SEPARATOR_2:
			case SEPARATOR_3:
				return true;
			default: return false;
		}
	}
	public static boolean single(int i){
		switch(i){
			case SHOW_INFO:
			case WATCH_NOW:
			case EXPLORER:
			case SET_FID:
			case EDIT_NAME:
				return true;
			default:
				return false;
		}
	}
}
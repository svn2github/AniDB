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
 * Created on 25.des.2005 20:18:12
 * Filename: JTreeTableR.java
 */
package epox.webaom.ui;

import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

import com.sun.swing.JTreeTable;
import com.sun.swing.TreeTableModel;

import epox.webaom.A;
import epox.webaom.Job;
import epox.webaom.JobMan;
import epox.webaom.data.AFile;
import epox.webaom.data.Base;

public class JTreeTableR extends JTreeTable implements RowModel,MouseListener {
	public JTreeTableR(TreeTableModel ttm){
		super(ttm);
		addMouseListener(this);
	}
	public void updateUI(){
		long t0 = System.currentTimeMillis();
		super.updateUI();
		t0 = System.currentTimeMillis()-t0;
		System.out.println("@ Alt.updateUI() in "+t0+" ms. ("+A.cache.stats()+")");
	}
	/*public void paint(Graphics g){
		long t0 = System.currentTimeMillis();
		super.paint(g);
		t0 = System.currentTimeMillis()-t0;
		System.out.println("@ Alt.paint() in "+t0+" ms. ("+A.cache.stats()+")");
	}*/
	/*public int[] convertRow(int row){
		Object o = tree.getPathForRow(row).getLastPathComponent();
		if(o instanceof AFile){
			AFile f = (AFile)o;
			if(f.getJob()!=null){
				return new int[]{f.getJob().mIid};
			}
		}else{
			ArrayList<Job> al = new ArrayList<Job>();
			rec(al, (Base)o);
			int[] a = new int[al.size()];
			for(int i=0;i<a.length;i++)
				a[i] = al.get(i).mIid;
			Arrays.sort(a);
			return a;
		}
		return null;
	}*/
	public Job[] getJobs(int row){
		Object o = tree.getPathForRow(row).getLastPathComponent();
		if(o instanceof AFile){
			AFile f = (AFile)o;
			if(f.getJob()!=null)
				return new Job[]{f.getJob()};//new int[]{f.getJob().mIid};
		}else{
			ArrayList/*!<Job>*/ al = new ArrayList/*!<Job>*/();
			rec(al, (Base)o);
			/*int[] a = new int[al.size()];
			for(int i=0;i<a.length;i++)
				a[i] = al.get(i).mIid;
			Arrays.sort(a);
			return a;*/
			Job[] j = (Job[])al.toArray(new Job[0]);
			return j;
		}
		return null;
	}
	private void rec(ArrayList/*!<Job>*/ al, Base p){
		if(p.size()<1){
			if(p instanceof AFile)
				al.add(((AFile)p).getJob());
			return;
		}
		p.mkArray();
		for(int i=0; i<p.size(); i++)
			rec(al,p.get(i));
	}
	/*public void updateRow(Job j){
		if(!Thread.holdsLock(A.p)){
			synchronized (A.p) {
				try{
					Thread.sleep(100);
				}catch(InterruptedException e){
					//
				}
				updateUI();
			}
		}
	}*/
	public void mouseClicked(MouseEvent e) {
		if (e.getClickCount() == 2){
			Object o = tree.getPathForRow(getSelectedRow()).getLastPathComponent();
			if(o instanceof AFile){
				AFile f = (AFile)o;
				if(f.getJob()!=null)
					if((e.getModifiers()&InputEvent.ALT_MASK)==InputEvent.ALT_MASK)
						JobMan.c_watch(f.getJob());
					else
						JobMan.showInfo(f.getJob());
			}
		}
	}
	public void mousePressed(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	public void mouseReleased(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	public void mouseEntered(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	public void mouseExited(MouseEvent e) {
		// TODO Auto-generated method stub
	}
	private void calcRowHeight(Graphics g){
		Font f = getFont();
		FontMetrics fm = g.getFontMetrics(f);
		setRowHeight(fm.getHeight()+3);
	}
	private boolean needCalcRowHeight=true;
	public void paint(Graphics g){
		if (needCalcRowHeight) {
			calcRowHeight(g);
			needCalcRowHeight = false;
		}
		super.paint(g);
	}
	public void setFont(Font f){
		needCalcRowHeight=true;
		super.setFont(f);
	}
}

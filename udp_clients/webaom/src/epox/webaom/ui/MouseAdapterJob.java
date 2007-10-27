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
 * Created on 02.09.05
 *
 * @version 	1.09
 * @author 		epoximator
 */

package epox.webaom.ui;

import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.JTable;

import epox.webaom.Job;
import epox.webaom.JobList;
import epox.webaom.JobMan;

public class MouseAdapterJob extends MouseAdapter
{
	private JTable jt;
	private JobList jl;
	private TableModelJobs jm;
	public MouseAdapterJob(JTable jt, TableModelJobs jm, JobList jl){
		this.jt = jt;
		this.jm = jm;
		this.jl = jl;
	}
	public void mouseClicked(MouseEvent e){
		if (e.getClickCount() == 2){
			int i = jm.convertRow(jt.getSelectedRow())[0];
			if(i>=0 && i<jl.size()){
				Job j = jl.get(i);
				if((e.getModifiers()&InputEvent.ALT_MASK)==InputEvent.ALT_MASK)
					JobMan.c_watch(j);
				else
					JobMan.showInfo(j);
			}
		}
	}
}
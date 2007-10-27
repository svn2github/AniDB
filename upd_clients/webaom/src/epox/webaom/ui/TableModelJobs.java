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

import javax.swing.JTable;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableColumnModel;

import epox.swing.TableModelSortable;
import epox.webaom.Job;
import epox.webaom.JobList;

public class TableModelJobs extends TableModelSortable implements RowModel{
	public static final int
	NUMB = 0,
	LIDN = 1,
	FIDN = 2,
	AIDN = 3,
	EIDN = 4,
	GIDN = 5,
	AYEA = 6,
	AEPS = 7,
	ALEP = 8,
	FSIZ = 9,
	FLEN = 10,
	FILE = 11,
	PATH = 12,
	NAME = 13,
	STAT = 14,
	AROM = 15,
	AKAN = 16,
	AENG = 17,
	ATYP = 18,
	ENUM = 19,
	EENG = 20,
	EKAN = 21,
	EROM = 22,
	GNAM = 23,
	GSHO = 24,
	FDUB = 25,
	FSUB = 26,
	FSRC = 27,
	FQUA = 28,
	FRES = 29,
	FVID = 30,
	FAUD = 31,
	FMDS = 32,
	FMDA = 33,
	ccnt = 34,
	JOB = -1;
	public static long MASK = 1<<NUMB|1<<FILE|1<<STAT;


	private static String STRA[];
	static{
		STRA = new String[ccnt];
		STRA[NUMB] = "#";
		STRA[LIDN] = "lid";
		STRA[FIDN] = "fid";
		STRA[AIDN] = "aid";
		STRA[EIDN] = "eid";
		STRA[GIDN] = "gid";
		STRA[FSIZ] = "size";
		STRA[FLEN] = "len";
		STRA[PATH] = "path";
		STRA[NAME] = "name";
		STRA[FILE] = "file";
		STRA[STAT] = "status";
		STRA[AKAN] = "kan";
		STRA[AROM] = "ann";
		STRA[AENG] = "eng";
		STRA[AYEA] = "yea";
		STRA[AEPS] = "eps";
		STRA[ALEP] = "lep";
		STRA[ATYP] = "typ";
		STRA[EKAN] = "epk";
		STRA[EROM] = "epr";
		STRA[EENG] = "epn";
		STRA[ENUM] = "enr";
		STRA[GNAM] = "grn";
		STRA[GSHO] = "grp";
		STRA[FDUB] = "dub";
		STRA[FSUB] = "sub";
		STRA[FSRC] = "src";
		STRA[FQUA] = "qua";
		STRA[FRES] = "res";
		STRA[FVID] = "vid";
		STRA[FAUD] = "aud";
		STRA[FMDS] = "mds";
		STRA[FMDA] = "mda";
	}
	private JobList jl;
	private int m_current_row;
	private Job m_current_job;

	public TableModelJobs(JobList j){
		jl = j;
		reset();
	}
	public void reset(){
		m_current_row = -1;
		m_current_job = null;
		super.reset();
	}
	public int getColumnCount(){
		return ccnt;
	}
	public Class/*!<?>*/ getColumnClass(int col){
		if(col==FSIZ)
			return Long.class;
		if(col<FILE)
			return Integer.class;
		return String.class;
	}
	public int getRowCount(){
		return jl.size();
	}
	public Object getValueAt(int row, int col){
		row = getRowIndex(row);
		if(col<1||row!=m_current_row){ //if col==0 then it is an update
			m_current_row = row;
			m_current_job = jl.get(row);
		}
		Job j = m_current_job;

		switch(col){
			case NUMB: return new Integer(row+1);
			case FSIZ: return new Long(j.mLs);
			case PATH: return j.getFile().getParent();
			case NAME: return j.getFile().getName();
			case FILE: return j.getFile().getAbsolutePath();
			case STAT: return j.getStatusText();
			case JOB: return j;
		}
		if(j.m_fa==null){
			//return col<FILE?0:"N/A";
			if(col<FILE) return new Integer(0);
			return "N/A";
		}
		String s = null;
		switch(col){
			case LIDN: return new Integer(j.mIlid);//j.m_fa.lid;
			case FIDN: return new Integer(j.m_fa.fid);
			case AIDN: return new Integer(j.m_fa.aid);
			case EIDN: return new Integer(j.m_fa.eid);
			case GIDN: return new Integer(j.m_fa.gid);
			case AYEA: return new Integer(j.m_fa.anime.yea);
			case AEPS: return new Integer(j.m_fa.anime.eps);
			case ALEP: return new Integer(j.m_fa.anime.lep);
			case FLEN: return new Integer(j.m_fa.len);
			case AKAN: s = j.m_fa.anime.kan; break;
			case AROM: s = j.m_fa.anime.rom; break;
			case AENG: s = j.m_fa.anime.eng; break;
			case ATYP: s = j.m_fa.anime.typ; break;
			case EKAN: s = j.m_fa.ep.kan; break;
			case EROM: s = j.m_fa.ep.rom; break;
			case EENG: s = j.m_fa.ep.eng; break;
			case ENUM: s = j.m_fa.ep.num; break;
			case GNAM: s = j.m_fa.group.name; break;
			case GSHO: s = j.m_fa.group.sname; break;
			case FDUB: s = j.m_fa.dub; break;
			case FSUB: s = j.m_fa.sub; break;
			case FSRC: s = j.m_fa.rip; break;
			case FQUA: s = j.m_fa.qua; break;
			case FRES: s = j.m_fa.res; break;
			case FVID: s = j.m_fa.vid; break;
			case FAUD: s = j.m_fa.aud; break;
			case FMDS: s = j.m_fa.mds(); break;
			case FMDA: s = j.m_fa.mda(); break;
		}
		return s==null?"":s;
	}
	public boolean isCellEditable(int row, int col){
		return false;
	}
	//public void setValueAt(Object obj, int row, int col){
	//}
	public String getColumnName(int columnIndex){
		return STRA[columnIndex];
	}
	public int getColumnIndex(String s){
		for(int i=0; i<STRA.length; i++)
			if(STRA[i].equals(s)) return i;
		return -1;
	}
	public static void formatTable(JTable table){
		table.setShowGrid(false);
		TableColumnModel m = table.getColumnModel();
		DefaultTableCellRenderer centerRend = new DefaultTableCellRenderer();
		centerRend.setHorizontalAlignment(SwingConstants.CENTER);
		for(int i=0; i<ccnt; i++)
			if(i!=FILE)
				m.getColumn(i).setCellRenderer(centerRend);
	}
	public void updateRow(int id){
		this.fireTableRowsUpdated(id, id);
	}
	/*public void updateRow(Job j){
		try{
			updateRow(m_rev[j.mIid]);
		}catch(Exception e){
			updateRow(j.mIid);
		}
	}*/
	public void insertJob(int x){
		//fireTableRowsInserted(j.mIid, j.mIid);
		fireTableRowsInserted(x, x);
	}
	public int[] convertRow(int row){
		return new int[]{getRowIndex(row)};

	}
	public Job[] getJobs(int row){
		return new Job[]{(Job)getValueAt(row, JOB)};
	}
	public void convertRows(int rows[]){
		for(int i=0; i<rows.length; i++)
			rows[i] = getRowIndex(rows[i]);
	}
}
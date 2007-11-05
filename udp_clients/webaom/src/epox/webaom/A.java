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
 * Created on 08.10.05
 *
 * @version 	01 (1.14)
 * @author 		epoximator
 */
package epox.webaom;

import java.awt.Component;
import java.awt.Font;
import java.io.File;
import java.io.InputStream;

import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import epox.util.U;
import epox.util.UserPass;
import epox.webaom.data.Base;
import epox.webaom.net.AConE;
import epox.webaom.net.AConS;
import epox.webaom.ui.JPanelMain;
/*
 * THE STATIX CLASS
 */
public class A{
	private A(){
		//static only
	}
	/*public static int mem0, mem1, mem2, mem3, mem4, mem5;
	public static void memstats(){
		System.out.println((mem1-mem0)/1048576f+"\t"+(mem2-mem1)/1048576f+"\t"+(mem3-mem2)/1048576f+"\t"+(mem4-mem3)/1048576f+"\t"+(mem5)/1048576f);
	}*/
	public static final String S_WEB="anidb.info", S_VER = "1.19l (071105)", S_N="\r\n";
	public static String fschema, dir = null, preg=null/*"^.*$"*/, font="";
	public static int ASNO = 99, ASSP = 99;

	public static java.awt.Component component = null;
	public static java.awt.Frame frame = null;

	public static DB db;
	public static NetIO nio;
	public static DiskIO dio;
	public static Options opt;
	public static Rules rules;
	public static Cache cache;
	public static AConE conn;
	public static JobCnt jobc;
	public static JobList jobs;
	public static AConS usetup;
	public static JPanelMain gui;
	public static FileHandler fha;

	public static Component com0, com1;

	public static Base p = new Base();

	public static boolean autoadd = false, opt_change = false;

	//public static volatile int nr_dio=-1, nr_nio = -1, nr_menu = -1;

	public static final UserPass up = new UserPass(null,null,null);

	public static void init(){
		//A.mem0 = A.getUsed();
		Thread.currentThread().setName("Main");
		jobs = new JobList();
		jobc = new JobCnt();
		rules = new Rules();
		cache = new Cache();
		db = new DB();
		fha = new FileHandler();
		opt = new Options();
		dio = new DiskIO();
		nio = new NetIO();
		//A.mem1 = A.getUsed();
		gui = new JPanelMain();
		fschema = U.fileToString(System.getProperty("user.home")+File.separator+".webaom.htm");
		if(fschema==null)
			fschema = A.getFileString("file.htm");

		if(font.length()>0) setFont(font);
		//A.mem2 = A.getUsed();
	}
	public static boolean shutdown(boolean opx){
		if(opx){
			Options o = new Options();
			if(o.onDisk()){
				gui.opts(o);
				if(!A.opt.equals(o))
					if(o.getB(Options.B_AUTOSAV))
						o.save();
					else
					switch(yes_no_cancel("The options has changed", "Do you want to save them?")){
					case 0: o.save(); break;
					case -1:
					case 2: return false;
					}
			}
		}
		//if(db!=null)
		//	db._shutdown();
		gui.reset();
		gui.shutdown();
		return true;
	}
	public static void setFont(String f){
		int i = f.lastIndexOf(','), size = 11;
		if(i>0) try {
			String s = f.substring(i+1);
			f = f.substring(0,i).trim();
			size = Integer.parseInt(s);
		} catch (NumberFormatException e) {
			//
		}
		Font fo = new Font(f, Font.PLAIN, size);
		WebAOM.setMyFont(fo,fo);
		SwingUtilities.updateComponentTreeUI(gui);
		SwingUtilities.updateComponentTreeUI(com0);
		SwingUtilities.updateComponentTreeUI(com1);
	}
	public static void dialog(String title, String msg){
		JOptionPane.showMessageDialog(component, msg, title, JOptionPane.PLAIN_MESSAGE);
	}
	public static void dialog2(String title, String msg){
		new epox.webaom.ui.JFrameHtml(title, msg);
	}
	public static boolean confirm(String title, String msg, String pos, String neg){
		Object[] o = { pos, neg };
		return JOptionPane.showOptionDialog(A.component, msg, title,
				JOptionPane.DEFAULT_OPTION, JOptionPane.WARNING_MESSAGE,
				null, o, o[0])==0;
	}
	public static int yes_no_cancel(String title, String msg){
		Object[] o = { "Yes", "No", "Cancel" };
		return JOptionPane.showOptionDialog(A.component, msg, title,
				JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.WARNING_MESSAGE,
				null, o, o[0]);
	}
	public static boolean bitcmp(int s, int m){
		return (s&m)==m;
	}
	public static String getFileString(String name){
		try{
			InputStream is = WebAOM.class.getClassLoader().getResourceAsStream(name);
			String str="";
			int buf_size = 1024;
			byte buffer[] = new byte[buf_size];
			int read;
			while((read=is.read(buffer, 0, buf_size))>0)
				str += new String(buffer, 0, read);
			return str;
		}catch (Exception e){ e.printStackTrace();
			return e.getMessage();
		}
	}
	public static void deleteFileAndFolder(File f, String s){
		deleteFile(f, s);
		deleteFile(f.getParentFile(), s);
	}
	public static void deleteFile(File f, String s){
		if(f.delete()) System.out.println("$ Deleted "+f+" ("+s+")");
	}
	public static void dumpStats(){
		System.out.println("@ JobList: "+A.jobs);
		System.out.println("@ Cache: "+A.cache);

		int sub0=0, sub1=0;
		Base b, c;
		for(int i = 0; i<A.p.size(); i++){
			b = A.p.get(i);
			if(b==null) continue;
			b.mkArray();
			sub0 += b.size();
			for(int j = 0; j<b.size(); j++){
				c = b.get(j);
				if(c!=null)
					sub1 += c.size();
			}
		}
		System.out.println("@ Tree: "+A.p.size()+", "+sub0+", "+sub1);
	}
	/*public static int getUsed(){
		MemoryMXBean mx = ManagementFactory.getMemoryMXBean();
		MemoryUsage muh = mx.getHeapMemoryUsage();
		MemoryUsage mus = mx.getNonHeapMemoryUsage();
		return (int)(muh.getUsed()+mus.getUsed());
	}*/
}
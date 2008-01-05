//Copyright (C) 2005-2006 epoximator

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

/*
 * Created on 22.01.05
 *
 * @version 	1.09, 1.07, 1.06, 1.05, 1.03, 1.00
 * @author 		epoximator
 */
package epox.webaom;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Toolkit;

import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.WindowConstants;

import epox.swing.MySwing;
import epox.webaom.net.ACon;
import epox.webaom.ui.JPanelSimple;

public class WebAOM extends JApplet{
	private static boolean global = false;
	private boolean inited = false;
	public void init(){
		/*try {
			javax.swing.UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
			javax.swing.SwingUtilities.updateComponentTreeUI(JPanelMain.this);
		} catch (Exception e) {
			e.printStackTrace();
		}*/
		if(global){
			JOptionPane.showMessageDialog(this,
					"You can only run one instance at once.",
					"Global report: You fail!", JOptionPane.PLAIN_MESSAGE);
			return;
		}
		global = true;
		inited = true;

		ACon.shutdown = false;

		MySwing.setFont(
				new Font("Tahoma", Font.PLAIN, 11),
				new Font("Times", Font.PLAIN, 11)
		);

		String u = null, s = null;
		try{
			u = getParameter("user");
			s = getParameter("sess");
		}catch(NullPointerException e){
			//
			//u = "epoximator";
			//s = "sTDdSBQvBPPNkJUd";
		}
		final String user = u, sess = s;

		final JPanelSimple simple = new JPanelSimple(user);
		final Container c = getContentPane();
		c.add(simple, BorderLayout.CENTER);

		new Thread(new Runnable(){
			public void run(){
				A.init(c, simple.getLog(), user, sess);
				simple.start();
			}
		}).start();
	}
	public void stop(){
		if(inited){
			A.stop();
		}
	}
	public void destroy(){
		if(inited){
			A.destroy();
			removeAll();
			super.destroy();
			inited = false;
			global = false;
		}
	}
	public static void main(String[] args){
		try{
			final JFrame f = new JFrame("WebAOM "+A.S_VER+" Loading...");
			f.setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

			Dimension d = Toolkit.getDefaultToolkit().getScreenSize();

			f.setSize(3*d.width/4,3*d.height/4);
			MySwing.centerComponent(f);

			final JApplet a = new WebAOM();
			a.init();

			f.setContentPane(a.getContentPane());
			f.setVisible(true);

			f.addWindowListener(new java.awt.event.WindowAdapter(){
				public void windowClosing(java.awt.event.WindowEvent e){
					if(A.saveOptions()){
						a.stop();
						a.destroy();
						f.dispose();
					}
				}
			});
		}catch(Exception e){
			e.printStackTrace();
			A.dialog("Exception", e.getMessage());
			System.exit(0);
		}
	}
}
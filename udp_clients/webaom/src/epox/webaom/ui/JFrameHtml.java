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
 * Created on 10.06.05
 *
 * @version 	1.07
 * @author 		epoximator
 */

package epox.webaom.ui;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JEditorPane;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.WindowConstants;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import epox.webaom.A;

public class JFrameHtml extends JFrame implements HyperlinkListener, KeyListener{
	public JFrameHtml(String title, String text){
		super(title);

		JEditorPane jep = new JEditorPane("text/html", text){
			protected void paintComponent(Graphics g) {
				Graphics2D g2 = (Graphics2D) g;
				g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
				RenderingHints.VALUE_ANTIALIAS_ON);

				super.paintComponent(g2);
			}
		};
		jep.setEditable(false);
		jep.addHyperlinkListener(this);

		getContentPane().add(new JScrollPane(jep));
		setDefaultCloseOperation(WindowConstants.DISPOSE_ON_CLOSE);
		pack();
		Dimension d = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
		Rectangle r = getBounds();
		int w = (int)(0.95*d.width);
		int h = (int)(0.95*d.height);
		if(r.width>w) r.width = w;
		if(r.height>h) r.height = h;
		setBounds(d.width/2-r.width/2, d.height/2-r.height/2, r.width, r.height);
		setVisible(true);
		jep.addKeyListener(this);
	}
	public void hyperlinkUpdate(HyperlinkEvent e){
		if(e.getEventType()==HyperlinkEvent.EventType.ACTIVATED)
			A.gui.hlGo(e.getDescription());
	}
	public void keyTyped(KeyEvent e) {
		if(e.getKeyChar()=='q') dispose();
	}
	public void keyPressed(KeyEvent e) {
		//
	}
	public void keyReleased(KeyEvent e) {
		//
	}
}
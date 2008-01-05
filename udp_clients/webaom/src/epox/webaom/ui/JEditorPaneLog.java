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
 * Created on 29.01.05
 *
 * @version 	1.06, 1.01
 * @author 		epoximator
 */

package epox.webaom.ui;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintStream;
import java.io.RandomAccessFile;
import java.io.Reader;
import java.io.StringReader;

import javax.swing.Action;
import javax.swing.JEditorPane;
import javax.swing.KeyStroke;
import javax.swing.text.Document;
import javax.swing.text.EditorKit;

import epox.swing.JTextInputDialog;
import epox.swing.Log;
import epox.util.Hyper;
import epox.util.U;
import epox.webaom.A;

public class JEditorPaneLog extends JEditorPane implements Log,Action,ActionListener{
	public static String HEAD = "<style type=\"text/css\">body{background-color:#FFFFFF;font-family:Verdana,Arial,Helvetica,sans-serif;font-size:11pt;}</style>";
	private PrintStream ps;

	public JEditorPaneLog(){
		super("text/html", HEAD);
		setEditable(false);
		getInputMap().put(KeyStroke.getKeyStroke("DELETE"),"remove");
		getActionMap().put("remove", this);
	}
	public synchronized void append(String t) {
        try {
        	Document doc = getDocument();
        	int len = doc.getLength();
		    if (t == null || t.equals(""))
		    	return;
		    Reader r = new StringReader(t);
		    EditorKit kit = getEditorKit();
		    kit.read(r, doc, len);
        } catch (Exception e) {
        	e.printStackTrace();
		}
    }
	public void add(int i, Object o){
		add(o);
	}
	public void add(Object o){
		String time = Hyper.number(U.time());
		StringBuffer line = new StringBuffer(256);
		line.append('[');
		line.append(time);
		line.append("] ");
		line.append(o);
		line.append("<br>\n");
		append(line.toString());

		try{
			ps.print(line.toString());
		}catch(NullPointerException e){
			//don't care
		}
	}
	public boolean openLogFile(String path){
		try{
			ps = new PrintStream(new AppendFileStream(path), true, "utf8");
			return true;
		}catch(IOException e){
			A.dialog("Log Error", e.getMessage());
			return false;
		}
	}
	public void closeLogFile(){
		try{
			ps.close();
		}catch(Exception e){
			//don't care
		}
	}
	private class AppendFileStream extends OutputStream {
		RandomAccessFile fd;
		public AppendFileStream(String file) throws IOException {
			fd = new RandomAccessFile(file,"rw");
			fd.seek(fd.length());
		}
		public void close() throws IOException {
			fd.close();
	    }
		public void write(byte[] b) throws IOException {
			fd.write(b);
		}
		public void write(byte[] b,int off,int len) throws IOException {
			fd.write(b,off,len);
		}
		public void write(int b) throws IOException {
			fd.write(b);
		}
	}
	public Object getValue(String key){return null;}
	public void putValue(String key, Object value){
		//don't care
	}
	public void actionPerformed(ActionEvent e){
		setHeader((new JTextInputDialog(A.frame, "Edit header", HEAD)).getStr());
	}
	public void setHeader(String h){
		if(h==null||h.length()<1) return;
		synchronized(this){
			HEAD = h;
			setText(HEAD);
			Runtime.getRuntime().gc();
		}
	}
}
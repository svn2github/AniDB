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
 * Created on 08.08.05
 *
 * @version 	01 (1.09) [from MainPanel]
 * @author 		epoximator
 */
package epox.webaom.net;

import epox.webaom.Hyper;
import epox.webaom.ui.JPanelMain;


public class Pinger extends Thread{
	private JPanelMain web;
	public Pinger(JPanelMain w){
		super("Pinger");
		web = w;
		web.fatal(true);
		start();
	}
	public void run(){
		ACon ani = null;
		try{
			ani = web.getConnection();
			if(ani.connect()){
				web.println("PING...");
				String str = "PONG (in "+(ani.ping()/1000f)+" seconds).";
				web.println(str);
				web.msg(str);
			}else web.msg(ani.getLastError()+".");
		}
		catch(java.net.SocketTimeoutException e){
			String str = "AniDB is not reachable";
			web.println(Hyper.error(str+"."));
			web.msg(str);
		}
		catch(NumberFormatException e){
			web.msg("Invalid number."+e.getMessage());
		}
		catch(Exception e){ e.printStackTrace();
			web.println(Hyper.error(e.getMessage()+"."));
			web.msg(e.getMessage()+".");
		}
		if(ani!=null) ani.disconnect();
		web.fatal(false);
	}
}
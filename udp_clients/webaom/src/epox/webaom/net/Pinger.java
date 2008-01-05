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

import epox.swing.Log;
import epox.swing.Status;
import epox.util.Hyper;

public class Pinger extends Thread{
	private ACon ani;
	private Log log;
	private Status status;
	public Pinger(ACon a, Log l, Status s){
		super("Pinger");
		ani = a;
		log = l;
		status = s;
		start();
	}
	public void run(){
		status.started();
		try{
			if(ani.connect()){
				log.add("PING...");
				String str = "PONG (in "+(ani.ping()/1000f)+" seconds).";
				log.add(Log.LOG|Log.MSG, str);
			}else log.add(Log.MSG, ani.getLastError()+".");
		}
		catch(java.net.SocketTimeoutException e){
			String str = "AniDB is not reachable";
			log.add(Log.LOG|Log.MSG, Hyper.error(str+"."));
		}
		catch(NumberFormatException e){
			log.add(Log.MSG, "Invalid number."+e.getMessage());
		}
		catch(Exception e){ e.printStackTrace();
			log.add(Log.LOG|Log.MSG, Hyper.error(e.getMessage()+"."));
		}
		if(ani!=null) ani.disconnect();
		status.finished();
	}
}
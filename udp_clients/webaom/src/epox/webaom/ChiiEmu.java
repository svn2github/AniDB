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
 * Created on 21.nov.2005 15:56:58
 * Filename: ChiiEmulator.java
 */
package epox.webaom;

import epox.swing.CommandModel;
import epox.swing.Log;
import epox.util.U;
import epox.webaom.net.ACon;
import epox.webaom.net.AConEx;
import epox.webaom.net.AConR;

public class ChiiEmu implements CommandModel{
	public Thread mTw;
	protected ACon m_ac;
	private Log m_log;

	public ChiiEmu(ACon a){
		m_log = null;
		m_ac = a;
	}
	public void handleCommand(String cmd){
		m_ac = A.conn;
		if(cmd.startsWith("!font")){
			if(cmd.length()>6){
				A.font = cmd.substring(6).trim();
				A.setFont(A.font);
			}else println("!font name,size");
		}else if(m_ac==null)
			println("No connection");
		else if(mTw==null){
			mTw = new EmuWorker(cmd);
			mTw.start();
		}else println("Please Wait!");
	}
	public void setLog(Log l){
		m_log = l;
	}
	public void println(Object o){
		if(m_log==null) return;
		if(o==null) m_log.println("err..");
		else m_log.println(o.toString());
	}
	private class EmuWorker extends Thread{
		private String mScmd;
		public EmuWorker(String c){
			super("ChiiEmu");
			mScmd = c;
		}
		public void run(){
			String res = "Query failed.";
			try {
				res = lookUp(mScmd);

			}catch(AConEx e) {
				res = e.getMessage();
			}catch(NullPointerException e){/*don't care*/}
			println(res);
			mTw = null;
		}
		public String lookUp(String s) throws AConEx{
			if(mScmd==null|mScmd.length()<3) return "No Command";
			if(mScmd.startsWith("!uptime")) return uptime(m_ac.send("UPTIME", null, true).data);
			if(mScmd.startsWith("!mystats")) return mystats(m_ac.send("MYLISTSTATS", null, true).data);
			//if(mScmd.startsWith("!stats")) return stats(m_ac.send("STATS", null, true).data);
			//if(mScmd.startsWith("!top")) return top(m_ac.send("TOP", null, true).data);
			if(mScmd.startsWith("!mylist")) return idOrName("MYLIST","aid=", "aname=", s.substring(7).trim());
			if(mScmd.startsWith("!anime")) return anime(idOrName("ANIME","aid=", "aname=", s.substring(6).trim()));
			if(mScmd.startsWith("!group")) return group(idOrName("GROUP","gid=", "gname=", s.substring(6).trim()));
			//if(cmd.startsWith("!episode")) return idOrName("EPISODE","eid=", null, s.substring(8).trim());
			if(mScmd.startsWith("!randomanime")) return randomanime(s.substring(12).trim());
			if(mScmd.startsWith("!state2")) return stosta(s.substring(7).trim(), I_STAT2);
			if(mScmd.startsWith("!state")) return state(s.substring(6).trim());
			if(mScmd.startsWith("!watched")) return watched(s.substring(8).trim());
			if(mScmd.startsWith("!storage")) return stosta(s.substring(8).trim(), I_STORA);
			if(mScmd.charAt(0)=='?') return m_ac.send(s.substring(1), true);
			return "Unknown command";
		}
		public String uptime(String str){
			long l = Long.parseLong(str)/60000;
			int d = (int)(l/1440);
			int h = (int)((l%1440)/60);
			int m = (int)(l%1440%60);
			return "UPTIME: "+d+"d "+h+"h "+m+"m";
		}
		public String idOrName(String c, String i, String n, String s) throws AConEx{
			try{
				return m_ac.send(c, i+Integer.parseInt(s), true).data;
			}catch(NumberFormatException e){/*part of plan*/}
			if(n==null) return "Not possible";
			return m_ac.send(c, n+s, true).data;
		}
		public String stosta(String p, int t) throws AConEx{
			String a[] = U.split(p, ',');
			if(a.length!=4) return getS(t);
			for(int i=0; i<a.length; i++){
				a[i] = a[i].trim();
				if(i<(a.length-1) && a[i].length()<1)
					return getS(t);
			}
			StringBuffer sb = new StringBuffer(50);
			sb.append("edit=1");
			int id;
			try{
				id = Integer.parseInt(a[0]);
				sb.append("&aid=");
				sb.append(id);
			}catch(NumberFormatException e){
				sb.append("&aname=");
				sb.append(a[0]);
			}
			try{
				id = Integer.parseInt(a[1]);
				sb.append("&gid=");
				sb.append(id);
			}catch(NumberFormatException e){
				if(!a[1].equalsIgnoreCase("all")){
					sb.append("&gname=");
					sb.append(a[1]);
				}
			}
			try{
				sb.append("&epno=");
				if(a[2].equalsIgnoreCase("all"))
					sb.append("0");
				else if(a[2].length()>5 && a[2].toLowerCase().startsWith("upto "))
					sb.append(-Integer.parseInt(a[2].substring(5)));
				else sb.append(Integer.parseInt(a[2]));
			}catch(NumberFormatException e){
				return getS(t);
			}
			String cmd;
			if(I_STORA==t){
				cmd = "STORAGE";
				sb.append("&storage=");
				sb.append(a[3]);
			}else{
				cmd = "STATE2";
				sb.append("&state=");
				if(a[3].equalsIgnoreCase("unknown"))
					sb.append(0);
				else if(a[3].equalsIgnoreCase("hdd"))
					sb.append(1);
				else if(a[3].equalsIgnoreCase("cd"))
					sb.append(2);
				else if(a[3].equalsIgnoreCase("deleted"))
					sb.append(3);
				else return cmd+": no such state";
			}

			AConR r = m_ac.send("MYLISTADD", sb.toString(),true);
			if(r.code!=AConR.MYLIST_ENTRY_EDITED||r.data==null||r.data.length()<1)
				return cmd+": no such entry";
			if(r.data.equals("1"))
				return cmd+": one entry updated";
			return cmd+": updated "+r.data+" entries";
		}
		public String randomanime(String param) throws AConEx{
			int type = 0;
			if(param.equals("watched"))
				type = 1;
			else if(param.equals("unwatched"))
				type = 2;
			else if(param.equals("all"))
				type = 3;
			AConR r = m_ac.send("RANDOMANIME", "type="+type, true);
			return "RANDOM "+anime(r.data);
		}
		public String anime(String str){
			String s[] = U.split(str, '|');
			if(s.length!=19) return str;
			StringBuffer sb = new StringBuffer(100);
			sb.append("ANIME: ");
			sb.append(s[12]);
			sb.append(" (");
			sb.append(s[0]);
			sb.append("), also known as ");
			sb.append(s[14]);
			sb.append(", ");
			sb.append(s[1]);
			sb.append(" eps, Year: ");
			sb.append(s[10]);
			sb.append(", Rating: ");
			sb.append(((float)Integer.parseInt(s[4])/100));
			sb.append(" (");
			sb.append(s[4]);
			sb.append(" votes), Reviews: ");
			sb.append(s[9]);
			sb.append(" (avg: ");
			sb.append(((float)Integer.parseInt(s[8])/100));
			sb.append(")");
			sb.append(", Cat: ");
			sb.append(s[18]);
			return sb.toString();
		}
		public String group(String str){
			String s[] = U.split(str, '|');
			if(s.length!=9) return str;
			StringBuffer sb = new StringBuffer(100);
			sb.append("GROUP: ");
			sb.append(s[5]);
			sb.append(" [");
			sb.append(s[6]);
			sb.append("] (");
			sb.append(s[0]);
			sb.append("), irc: ");
			sb.append(s[7]);
			sb.append(", rating: ");
			sb.append(((float)Integer.parseInt(s[1])/100));
			sb.append(" (");
			sb.append(s[2]);
			sb.append(" votes), db: ");
			sb.append(s[3]);
			sb.append(" animes/");
			sb.append(s[4]);
			sb.append(" files, url: ");
			sb.append(s[8]);
			sb.append(", http://anidb.net/g"+s[0]);
			return sb.toString();
		}
		public String top(String str){
			String s[] = U.split(str, '\n');
			String t[];
			if(s.length!=11) return str;

			StringBuffer sb = new StringBuffer(100);
			sb.append("TOP: ");
			for(int i=0; i<s.length; i++){
				sb.append(S_TOP[i]);
				t = U.split(s[i], '|');
				sb.append(t[0]);
				sb.append(" (");
				sb.append(t[1]);
				sb.append("), ");
			}
			sb.deleteCharAt(sb.length()-1);
			return sb.toString();
		}
		public String stats(String str){
			String s[] = U.split(str, '|');
			if(s.length!=7) return str;
			StringBuffer sb = new StringBuffer(100);
			sb.append("STATS: ");
			for(int i=0; i<s.length; i++){
				sb.append(s[i]);
				sb.append(S_STAT[i]);
			}
			return sb.toString();
		}
		public String mystats(String str){
			String s[] = U.split(str, '|');
			if(s.length!=16) return str;
			StringBuffer sb = new StringBuffer(100);
			sb.append("MYSTATS: ");
			sb.append(s[0]);
			sb.append(" animes, ");
			sb.append(s[1]);
			sb.append(" eps (");
			sb.append(s[13]);
			sb.append(" / ");
			sb.append(s[12]);
			sb.append("% watched) and ");
			sb.append(s[2]);
			sb.append(" files in mylist (");
			sb.append(U.sbyte(1048576l*U.i(s[3])));
			sb.append(", ");
			sb.append(s[11]);
			sb.append("% of AniDB, ");
			sb.append(s[10]);
			sb.append("% watched). ");
			sb.append(s[4]);
			sb.append(" animes, ");
			sb.append(s[5]);
			sb.append(" eps, ");
			sb.append(s[6]);
			sb.append(" files, ");
			sb.append(s[7]);
			sb.append(" groups, ");
			sb.append(s[14]);
			sb.append(" votes, ");
			sb.append(s[15]);
			sb.append(" reviews added to DB. Leech factor: ");
			sb.append(s[8]);
			sb.append("%, Lameness: ");
			sb.append(s[9]);
			sb.append("%.");
			return sb.toString();
		}
/**
 !state <anime> <ep#> <state>
 !state <anime> all <state>
 !state <anime> upto <ep#> <state>
 !state <anime> last <state>
 !state <fid> <state>
 !state <ed2k link> <state>
 !state last <state>

 !watched <anime> <epnumber>
 !watched <fid>
 !watched <ed2k link>
 !watched <anime> upto <epnumber>
 !watched <anime> all
 !watched <anime> none
**/
		public String watched(String s){
			try{
				String api = "edit=1&viewed=";
				int j = s.indexOf("ed2k://|file|");
				if(j>=0){
					String a[] = U.split(s.substring(13).trim(),'|');
					api += "&size="+a[1];
					api += "&ed2k="+a[2];
				}else{
					int epno = 0;
					j = s.indexOf(" none");
					if(j<0){
						j = s.indexOf(" all");
						api += "1";
						if(j<0){
							j = s.indexOf(" upto ");
							if(j>0)
								epno = -Integer.parseInt(s.substring(j+6).trim());
							else{
								j = s.lastIndexOf(' ');
								if(j<0) j=0;
								try{
									epno = Integer.parseInt(s.substring(j<0?0:j).trim());
								}catch(NumberFormatException e){
									return getS(I_WATCH);
								}
							}
						}
					}else api += "0";
					String anime = s.substring(0, j).trim();
					if(anime.length()<1){
						if(epno<1) return getS(I_WATCH);
						api += "&fid="+epno;
					}else{
						api += "&epno="+epno;
						try{
							api += "&aid="+Integer.parseInt(anime);
						}catch(NumberFormatException e){
							api += "&aname="+anime;
						}
					}
				}
				AConR r = m_ac.send("MYLISTADD", api, true);
				if(r.code==AConR.MYLIST_ENTRY_EDITED){
					if(r.data.length()>3)
						return "WATCHED: entry updated.";
					return "WATCHED: "+r.data+" entries updated.";
				}
				return "WATCHED: no such entry";
			}catch(Exception e){
				e.printStackTrace();
			}
			return getS(I_WATCH);
		}
		public String state(String s){
			try{
				int i = s.lastIndexOf(' ');
				int state = 0;
				String statestr = s.substring(i).trim();
				if(statestr.equalsIgnoreCase("unknown")) state=0;
				else if(statestr.equalsIgnoreCase("hdd")) state=1;
				else if(statestr.equalsIgnoreCase("cd")) state=2;
				else if(statestr.equalsIgnoreCase("deleted")) state=3;
				else return getS(I_STATE);
				String api = "edit=1&state="+state;
				int j = s.indexOf("ed2k://|file|");
				if(j>=0){
					String a[] = U.split(s.substring(13, i).trim(),'|');
					api += "&size="+a[1];
					api += "&ed2k="+a[2];
				}else{
					int epno = 0;
					j = s.indexOf(" all ");
					if(j<0){
						j = s.indexOf(" upto ");
						if(j>0)
							epno = -Integer.parseInt(s.substring(j+6, i).trim());
						else{
							j = s.lastIndexOf(' ', i-1);
							if(j<0) j=0;
							epno = Integer.parseInt(s.substring(j<0?0:j,i).trim());
						}
					}
					String anime = s.substring(0, j).trim();
					if(anime.length()<1){
						if(epno<1) return getS(I_STATE);
						api += "&fid="+epno;
					}else{
						api += "&epno="+epno;
						try{
							api += "&aid="+Integer.parseInt(anime);
						}catch(NumberFormatException e){
							api += "&aname="+anime;
						}
					}
				}
				AConR r = m_ac.send("MYLISTADD", api, true);
				if(r.code==AConR.MYLIST_ENTRY_EDITED){
					if(r.data.length()>3)
						return "STATE: entry updated.";
					return "STATE: "+r.data+" entries updated.";
				}
				return "STATE: no such entry";
			}catch(Exception e){
				e.printStackTrace();
			}
			return getS(I_STATE);
		}
	}
	protected static final int I_WATCH=0, I_STATE=1, I_STAT2=2, I_STORA=3;
	protected static String getS(int i){
		switch(i){
		case I_WATCH: return "WATCHED: usage: !watched <anime> <epnumber>, !state <fid>, !state <ed2k link>, epnumber may be 'all', 'upto <epno>' or 'none'.";
		case I_STATE: return "STATE: usage: !state <anime> <epnumber> <state>, !state <fid> <state>, !state <ed2k link> <state>, !state last <state>, epnumber may be 'all' or 'upto <epno>'. State is: unknown/hdd/cd/deleted.";
		case I_STAT2: return "!state2 anime/aid, group/gid/all, all/upto x/x, unknown/hdd/cd/deleted";
		case I_STORA: return "!storage anime/aid, group/gid/all, all/upto x/x, text";
		}
		return "NOO!";
	}
	protected static final String[] S_STAT = {
			" animes, ",
			" eps, ",
			" files, ",
			" groups, ",
			" users, ",
			" KB, ",
			" open change requests in DB" };
	protected static final String[] S_TOP = {
			"Longest MyList: ",
			"Largest MyList: ",
			"Most Lame Files: ",
			"Most Indep. User: ",
			"Big. Leech0r: ",
			"Most Anime Added: ",
			"Most Eps Added: ",
			"Most Files Added: ",
			"Most Groups Added: ",
			"Most Votes: ",
			"Most Reviews: " };
}

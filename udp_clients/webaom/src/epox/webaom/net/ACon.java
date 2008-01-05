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
 * Created on 22.01.05
 *
 * @version 	1.09, 1.07, 1.06, 1.05, 1.03, 1.00
 * @author 		epoximator
 */
package epox.webaom.net;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.security.MessageDigest;
import java.util.zip.DataFormatException;
import java.util.zip.Inflater;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import javax.swing.Timer;

import epox.swing.Log;
import epox.util.UserPass;
import epox.webaom.A;
import epox.webaom.ui.JDialogLogin;

public class ACon implements ActionListener{
	public static final String DEF_HOST="api.anidb.net";
	public static final int DEF_RPORT = 9000, DEF_LPORT = 45678;

	public static boolean shutdown = false;
	public boolean m_authed = false;

	private final String m_ver = "&protover=3&client=webaom&clientver=119&nat=1&comp=1&enc=utf8";
	private final int m_tag_len = 3;
	private final int m_buf_siz = 2048*2;
	private final int m_ka_time = 3*1000*60*10; //30 min
	private final int m_ka_nat = 3*1000*60; //3 min
	private int m_tcnt = 0;
	private boolean /*m_keep = false,*/ m_iscon = false;
	private UserPass m_up = null;

	private static int m_rem_login_att = 2;
	private long m_tused=0, m_tstamp=0;
	private int m_rem_auth_att = 3;
	private String m_err = "Not Initialized.";

	private SecretKeySpec m_key = null;
	private Cipher m_cip;

	private DatagramSocket m_ds;
	private InetAddress m_ia;
	private Timer m_tm;

	private AConS m_s;

	protected String m_session = null, m_tag = null;

	private String m_enc = "ascii";

	private Log m_log;
	public ACon(Log l, AConS s){
		m_log = l;
		m_s = s;
		generateTag();
		m_tm = new Timer(m_ka_time, this); //+1000
	}
	private void generateTag(){
		m_tcnt++;
		m_tag = ""+m_tcnt;
		while(m_tag.length()<m_tag_len)
			m_tag = "0"+m_tag;
		m_tag = 't'+m_tag;
	}
	public void actionPerformed(ActionEvent e){
		if(!m_s.nat) return;
		try {
			long now = System.currentTimeMillis();
			if((now-m_tstamp)>m_ka_time)
				ping();
		}catch(SocketTimeoutException x){
			debug("! No response from server.");
			m_tm.start();
		}catch(Exception x) {
			x.printStackTrace();
		}
	}
	///////////////////////////////////PASSW////////////////////////////////////
	public void set(String u, String p, String k, String s){
		m_up = new UserPass(u,p,k,s);
	}
	public void set(String u, String p, String k){
		set(u,p,k,null);
	}
	private void loginAtt(){
		m_rem_login_att--;
	}
	///////////////////////////////////DEBUG////////////////////////////////////
	protected void error(String str){
		debug(str);
		m_err = str;
		if(m_log==null)
			m_log.add(Log.LOG|Log.STN, "<font color=#FF0000>"+str+"</font>");
	}
	protected void debug(String str){
		System.out.println(str);
	}
	public String getLastError(){
		return m_err;
	}
	//////////////////////////////BASIC COMMANDS////////////////////////////////
	public boolean isLoggedIn(){
		return m_authed;
	}
	public int ping() throws Exception{
		send_layer0("PING", true);
		return (int)m_tused;
	}
	public int enCrypt() throws Exception{
		if(m_up.key==null||m_up.key.length()<1) return ping();
		AConR r = send_layer1("ENCRYPT", "user="+m_up.usr+"&type=1", true);
		if(r.code==AConR.ENCRYPTION_ENABLED) try {
			MessageDigest md = MessageDigest.getInstance("MD5");
			md.update(m_up.key.getBytes());
			md.update(r.data.getBytes());
			byte[] k = md.digest();
			m_key = new SecretKeySpec(k,"AES");
			m_cip = Cipher.getInstance("AES");
			return (int)m_tused;
		}catch(Exception e){
			e.printStackTrace();
			m_key = null;
			m_cip = null;
		}
		else if(r.code==AConR.API_PASSWORD_NOT_DEFINED)
			throw new AConEx(AConEx.ENCRYPTION, "AniPass not defined. Check your profile settings.");
		else if(r.code==AConR.NO_SUCH_USER)
			throw new AConEx(AConEx.ENCRYPTION, "No such user. Check username.");
		return -1;
	}
	private boolean showLoginDialog() throws AConEx{
		if(m_rem_login_att<=0)
			throw new AConEx(AConEx.CLIENT_USER, "Access Denied! No more login attempts allowed.");
		loginAtt();
		error("Logins left: "+m_rem_login_att);
		m_up = (new JDialogLogin()).getPass();
		return m_up!=null;
	}
	public boolean login() throws AConEx{
		if(m_up==null)
			if(!showLoginDialog()){
				error("User Abort");
				return false;
			}
		if(m_rem_auth_att<=0)
			throw new AConEx(AConEx.CLIENT_BUG, "Invalid session.");
		AConR r;
		if(m_up.ses!=null){
			r = send("AUTH", "user="+m_up.usr+"&sess="+m_up.ses+m_ver, true);
		}else{
			r = send("AUTH", "user="+m_up.usr+"&pass="+m_up.psw+m_ver, true);
		}

		m_rem_auth_att--;
		switch(r.code){
			//case Reply.ENCODING_NOT_SUPPORTED:
			//	throw new AudpcEx(AudpcEx.CLIENT_BUG);
			case AConR.LOGIN_ACCEPTED_NEW_VER:
				A.dialog("Note", AConEx.defaultMsg(AConEx.CLIENT_OUTDATED));
			case AConR.LOGIN_ACCEPTED:{
				m_session = r.data;
				if(r.data.length()>5){
					m_session = r.data.substring(0,5);
					if(m_s.nat) try{
						int p = Integer.parseInt(r.data.substring(1+r.data.lastIndexOf(':')));
						if(p!=m_s.lport){
							m_tm.setDelay(m_ka_nat);
							debug("! Nat detected.");
						}
					}catch(Exception e){
						e.printStackTrace();
					}
				}else m_session = r.data;
				m_authed = true;
				m_rem_auth_att = 2;
				return true;
			}
			case AConR.LOGIN_FAILED:
				A.up.ses = null;
				error("Login Failed");
				if(showLoginDialog())
					return login();
				return false;
			default:
				error(r.message);
		}
		return false;
	}
	public boolean logout() throws AConEx{
		if(shutdown){
			if(m_authed&&m_session!=null){ //last chance logout
				String s = "LOGOUT s="+m_session;
				m_authed = false;
				try {
					//m_ds.send(new DatagramPacket(s.getBytes(), s.length(), m_ia, m_rport));
					send_layer0(s, false);
				}catch(IOException e){
					//don't care
				}
			}
			return true;
		}
		AConR r = send("LOGOUT", null, true);
		if(r==null) return false;
		switch(r.code){
			case AConR.LOGGED_OUT:
			case AConR.NOT_LOGGED_IN:
			case AConR.INVALID_SESSION:
			case AConR.LOGIN_FIRST:
				m_authed = false;
				m_enc = "ascii";
				return true;
			default: error(r.message);
		}
		return false;
	}
	////////////////////////////////////CORE////////////////////////////////////
	public boolean connect(){
		try{
			m_ds = new DatagramSocket(m_s.lport);
			m_ds.setSoTimeout(m_s.tout);
			m_ia = InetAddress.getByName(m_s.host);
			m_ia.getHostAddress();
			m_iscon = true;
			return true;
		}catch(SocketException e){
			e.printStackTrace();
			error("SocketException: "+e.getMessage());
		}catch(UnknownHostException e){
			error("Unknown Host: "+m_s.host);
		}
		return false;
	}
	public void disconnect(){
		if(m_iscon){
			m_tm.stop();
			m_ds.disconnect();
			m_ds.close();
			m_ds = null;
			m_ia = null;
			m_iscon = false;
			m_authed = false;
		}
	}
	public synchronized AConR send(String op, String param, boolean wait) throws AConEx{
		if(op==null) throw new AConEx(AConEx.CLIENT_BUG);
		return send_layer2(op, param, wait);
	}
	public synchronized String send(String command, boolean wait) throws AConEx{
		if(command==null) throw new AConEx(AConEx.CLIENT_BUG);

		String arg[] = command.split(" ",2);
		AConR r = send_layer2(arg[0],arg.length>1?arg[1]:null, wait);
		return r.code+" "+r.message+(r.data!=null?"\n"+r.data:"");
	}
	private AConR send_layer2(String op, String param, boolean wait) throws AConEx{
		int timeouts = 0;
		AConR r;
		while(timeouts++<m_s.max_tout && !shutdown)
			try{
				r = send_layer1(op, param, wait);
				if(!op.equals("LOGOUT")&&(r.code==AConR.LOGIN_FIRST||r.code==AConR.INVALID_SESSION)){
					login();
					return send_layer1(op, param, wait);
				}
				return r;
			}catch(SocketTimeoutException e){
				generateTag();
				error("Operation Failed: TIME OUT. Try #"+timeouts);
				m_s.delay += 100;
				//m_tm.start();
			}catch(IOException e){ e.printStackTrace();
				error("Operation Failed: IOEXCEPT: "+e.getMessage());
			}
		throw new AConEx(AConEx.ANIDB_UNREACHABLE, getLastError());
	}
	private AConR send_layer1(String op, String param, boolean wait) throws IOException, SocketTimeoutException, AConEx{
		if(param!=null){
			if(m_session!=null) param += "&s="+m_session;
		}else if(m_session!=null) param = "s="+m_session;
		if(param!=null)
			param += "&tag="+m_tag;
		else param = "tag="+m_tag;
		return send_layer0(op+" "+param, wait);
	}
	private AConR send_layer0(String s, boolean wait) throws SocketTimeoutException, IOException, AConEx{
		m_tm.stop();
		if(wait)
			try{
				long td = System.currentTimeMillis()-m_tstamp;
				td = (td/100)*100;// round down to nearest 100
				if(td<m_s.delay){
					debug("- Sleep: "+(m_s.delay-td));
					Thread.sleep(m_s.delay-td);
				}
			}catch(InterruptedException e){
				throw new AConEx(AConEx.CLIENT_SYSTEM, "Java: "+e.getMessage());
			}
		String cen = s;
		int i = cen.indexOf("pass=");
		if(i>0){
			int j = cen.indexOf("&", i);
			if(j>0) cen = cen.substring(0, i+5)+"xxxxx"+cen.substring(j);
		}
		debug("> "+cen);
		byte[] out = s.getBytes(m_enc);
		int len = out.length;

		if(m_key!=null) try{
			m_cip.init(Cipher.ENCRYPT_MODE, m_key);
			out = m_cip.doFinal(out);
			len = out.length;
		}catch(Exception e) {
			e.printStackTrace();
		}

		DatagramPacket pckt_out = new DatagramPacket(out, len, m_ia, m_s.rport);
		m_tstamp = System.currentTimeMillis();

		m_ds.send(pckt_out);

		if((i=s.indexOf("&enc="))>0){
			i += 5;
			int y = s.indexOf('&',i);
			if(y<0) y = s.length();
			m_enc = s.substring(i, y);
		}

		if(!shutdown){//wait
			AConR r = receive();
			m_tm.start();
			return r;
		}
		return null;
	}
	private AConR receive() throws SocketTimeoutException, IOException, AConEx{
		if(shutdown) return null;
		byte[] buf = new byte[m_buf_siz];
		DatagramPacket pckt_in  = new DatagramPacket(buf, m_buf_siz);

		m_ds.receive(pckt_in);
		m_tused = System.currentTimeMillis()-m_tstamp;
		m_tstamp += m_tused/2; //test, share used time

		int len = pckt_in.getLength();

		if(m_key!=null) try{
			m_cip.init(Cipher.DECRYPT_MODE, m_key);
			buf = m_cip.doFinal(buf, 0, len);
			len = buf.length;
		}catch(Exception e) {
			debug("! Decryption failed: "+e.getMessage());
			m_key = null;
			m_cip = null;
			throw new AConEx(AConEx.ENCRYPTION);
		}
		if(buf.length>1 && buf[0]==0 && buf[1]==0) try{
			Inflater dec = new Inflater();
			dec.setInput(buf, 2, len-2);
			byte[] result = new byte[len*3];
			len = dec.inflate(result);
			dec.end();
			buf = result;
		}catch(DataFormatException e){
			e.printStackTrace();
		}
		try{
			byte[] raw = new byte[len];
			System.arraycopy(buf, 0, raw, 0, len);
			String rs = new String(raw, m_enc);
			rs = rs.substring(0, rs.length()-1);
			debug("< "+rs);
			return new AConR(m_tag, m_tag_len, rs);
		}catch(TagEx e){
			debug("! Wrong tag! Should be: "+m_tag);
			return receive();
		}
	}
}
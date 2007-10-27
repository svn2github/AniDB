/*
 * Created on 24.feb.2006 22:21:45
 * Filename: Setup.java
 */
package epox.webaom.net;

public class AConS{
	public String host;
	public int rport, lport, tout, max_tout, delay;
	public boolean nat;

	/**
	 * Constructor
	 * @param h		host string
	 * @param rp	remote port
	 * @param lp	local port
	 * @param to	time out in sec
	 * @param del	packet delay
	 * @param max	maximum timeouts
	 * @param n		enable nat check
	 */
	public AConS(String h, int rp, int lp, int to, int del, int max, boolean n){
		host = h;
		rport = rp;
		lport = lp;
		tout = to*1000;
		delay = del;
		max_tout = max;
		nat = n;
	}
}

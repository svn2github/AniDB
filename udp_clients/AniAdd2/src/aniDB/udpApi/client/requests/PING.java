/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.requests;

import aniDB.udpApi.client.accountManagement.UserAccount;

/**
 *
 * @author Arokh
 */
public class PING extends aniDB.udpApi.requests.PING<Cmd> {
	public UserAccount getAccount(){ return cmd.getAccount(); }
	public void setAccount(UserAccount account) { cmd.setAccount(account); }

	public static Cmd createRequest(UserAccount account, String tag, Boolean nat){
		PING ping = new PING();
		if(nat != null) ping.setNAT(nat);
		if(account != null) ping.setAccount(account);
		if(tag != null) ping.setTag(tag);

		return ping.makeFinal();
	}
}

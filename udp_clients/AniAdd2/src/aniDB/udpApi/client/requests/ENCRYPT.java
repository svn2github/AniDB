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
public class ENCRYPT extends aniDB.udpApi.requests.ENCRYPT<Cmd> {
	public ENCRYPT() { super(new Cmd()); }

	public UserAccount getAccount(){ return cmd.getAccount(); }
	public void setAccount(UserAccount account) { cmd.setAccount(account); }

}

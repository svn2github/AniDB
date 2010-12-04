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
public class AUTH extends aniDB.udpApi.requests.AUTH<Cmd> {
	public AUTH() { super(new Cmd()); }


	public UserAccount getAccount(){ return cmd.getAccount(); }
	public void setAccount(UserAccount account) { cmd.setAccount(account); cmd.add("user", account.getUserName().toLowerCase()); }
}

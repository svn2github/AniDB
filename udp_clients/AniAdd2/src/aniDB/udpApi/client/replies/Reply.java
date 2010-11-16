/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.replies;

import aniDB.udpApi.client.accountManagement.UserAccount;

/**
 *
 * @author Arokh
 */
public class Reply extends aniDB.udpApi.replies.Reply {
	private UserAccount account;

	public UserAccount getAccount(){
		return account;
	}
	public boolean setAccount(UserAccount account) {
		if(isFinal()) return false;
		this.account = account;
		return true;
	}
}

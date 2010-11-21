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
public class Cmd extends aniDB.udpApi.requests.Cmd {
	private UserAccount account;
	private boolean authRequired;

	public Cmd() { }

	public UserAccount getAccount(){
		return account;
	}
	public boolean setAccount(UserAccount account) {
		if(isFinal()) return false;
		this.account = account;
		return true;
	}

	public boolean isAuthRequired(){
		return authRequired;
	}
	public boolean setAuthRequired(boolean authRequired) {
		if(isFinal()) return false;
		this.authRequired = authRequired;
		return true;
	}

}

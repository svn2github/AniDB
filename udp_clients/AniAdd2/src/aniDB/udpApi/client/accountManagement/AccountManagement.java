/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.accountManagement;

import java.util.Iterator;

/**
 *
 * @author Arokh
 */
public class AccountManagement implements Iterable<UserAccount> {
	public void addAccountListener(IAccountListener l){
		
	}

	public boolean contains(UserAccount account) {
		return true;
	}

	public Iterable<String> getAccountNames() {
		throw new UnsupportedOperationException("Not yet implemented");
	}

	public UserAccount getAccountName(String accountName) {
		throw new UnsupportedOperationException("Not yet implemented");
	}

	public Iterator<UserAccount> iterator() {
		throw new UnsupportedOperationException("Not supported yet.");
	}
}

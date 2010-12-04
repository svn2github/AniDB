/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.accountManagement;

import aniDB.udpApi.shared.ByReference;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Arokh
 */
public class AccountManagement implements Iterable<UserAccount> {
	private final Object eventSyncRoot = new Object();
	private List<IAccountListener> listeners;
	private HashMap<String, UserAccount> accounts;

	public AccountManagement(ByReference<Internal> internalRef) {
		accounts = new HashMap<String, UserAccount>();
		listeners = new ArrayList<IAccountListener>();
		internalRef.setParam(new Internal());
	}


	public void add(UserAccount account) {
		if(account == null || accounts.containsKey(account.getUserName())) throw new IllegalArgumentException();
		accounts.put(account.getUserName(), account);
		accountEventFire(account, IAccountListener.AccountEvent.Added);
	}

	public void addAccountListener(IAccountListener comListener) {
		synchronized(eventSyncRoot) { listeners.add(comListener); }
	}

	public void removeAccountListener(IAccountListener comListener) {
		synchronized(eventSyncRoot) { listeners.remove(comListener); }
	}

	private void accountEventFire(UserAccount account, IAccountListener.AccountEvent accountEvent) {
		for(IAccountListener listener : listeners) listener.action(account, accountEvent);
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

	public Iterator<UserAccount> iterator() { return accounts.values().iterator(); }

	public class Internal {
		private Internal() { }

		public String getSession(UserAccount account) { return account.getSession(); }
		public void setSession(UserAccount account, String session) { account.setSession(session); }

		public String getUserPassword(UserAccount account) { return account.getUserPassword(); }
		public void setUserPassword(UserAccount account, String password) { account.setUserPassword(password); }

		public String getApiPassword(UserAccount account) { return account.getApiPassword(); }
		public void setApiPassword(UserAccount account, String password) { account.setApiPassword(password); }

		public boolean isShouldAuthenticate(UserAccount account) { return account.isShouldAuthenticate(); }
		public void setShouldAuthenticate(UserAccount account, boolean shouldAuthenticate) { account.setShouldAuthenticate(shouldAuthenticate); }

		public boolean isAuthenticated(UserAccount account) { return account.isAuthenticated(); }
		public void setAuthenticated(UserAccount account, boolean isAuthenticated) { account.setAuthenticated(isAuthenticated); }

	}
}

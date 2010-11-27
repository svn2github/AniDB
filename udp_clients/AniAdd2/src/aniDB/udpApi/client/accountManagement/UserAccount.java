/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.accountManagement;

import java.util.Arrays;
import java.util.EnumSet;

/**
 *
 * @author Arokh
 */
public class UserAccount {
	private boolean authenticated;
	private String session;
	private EnumSet<Option> options;


	public boolean isAuthenticated() {
		return authenticated;
	}

	public String getSession() {
		return session;
	}


	public boolean isOptionSet(Option option) {
		return options.contains(option);
	}
	public boolean areOptionsSet(Option ... options) {
		return this.options.containsAll(Arrays.asList(options));
	}
	public boolean areAnyOptionsSet(Option ... options) {
		boolean ret = false;
		for(Option option : options) ret |= isOptionSet(option);
		return ret;
	}

	public void setSession(String session) { //TODO: make default access
		this.session = session;
	}

	public void setAuthenticated(boolean authenticated) {
		this.authenticated = authenticated;
	}

	public enum Option {
		None,
		Push_Notify,
		Push_Message,
		Push_Buddy,
		Push_Ack,

		KeepAlive,

		Compression,
		Encryption,
	}

}

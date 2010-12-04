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
	private boolean shouldAuthenticate;
	private String userName;
	private String userPassword;
	private String apiPassword;
	private String session;
	private EnumSet<Option> options;

	public UserAccount(String userName, String userPassword, EnumSet<Option> options, boolean shouldAuthenticate, String apiPassword) {
		this.shouldAuthenticate = shouldAuthenticate;
		this.userName = userName;
		this.userPassword = userPassword;
		this.apiPassword = apiPassword;
		this.options = options.clone();
	}




	public boolean isAuthenticated() {
		return authenticated;
	}

	String getSession() {
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

	void setSession(String session) { //TODO: make default access
		this.session = session;
	}

	void setAuthenticated(boolean authenticated) {
		this.authenticated = authenticated;
	}

	public boolean isShouldAuthenticate() {
		return shouldAuthenticate;
	}

	void setShouldAuthenticate(boolean shouldAuthenticate) {
		this.shouldAuthenticate = shouldAuthenticate;
	}

	String getUserPassword() {
		return userPassword;
	}

	void setUserPassword(String userPassword) {
		this.userPassword = userPassword;
	}

	String getApiPassword() {
		return apiPassword;
	}

	void setApiPassword(String apiPassword) {
		this.apiPassword = apiPassword;
	}

	public String getUserName() {
		return userName;
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

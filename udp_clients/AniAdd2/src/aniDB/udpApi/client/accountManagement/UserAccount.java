/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.accountManagement;

import java.util.Collection;
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
		boolean ret = true;
		for(Option option : options) ret &= isOptionSet(option);
		return ret;
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

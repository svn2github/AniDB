/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.accountManagement;

import java.util.EventListener;

/**
 *
 * @author Arokh
 */
public interface IAccountListener extends EventListener {
	void action(UserAccount account, AccountEvent accountEvent);

	public static enum AccountEvent{  Added, Removed, Changed, Login, Logout }
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.registerManagement;

import aniDB.udpApi.client.replies.Reply;
import aniDB.udpApi.shared.IAction;
import aniDB.udpApi.shared.IdGenerator;

/**
 *
 * @author Arokh
 */
public class Registrant {
	private static int count = 0;

	private IdGenerator idGenerator;
	private IAction<Reply> callBack;

	public Registrant(IAction<Reply> callBack) {
		this.callBack = callBack;
		this.idGenerator = new IdGenerator();
		count++;
	}

	public IdGenerator getIdGenerator(){
		return idGenerator;
	}

	public void callBack(Reply reply) {
		this.callBack.invoke(reply);
	}

}

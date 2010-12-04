/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.udpApi;

import aniAdd2.communication.ComEvent;
import aniAdd2.exts.AA2Extension;
import aniDB.udpApi.client.ClientAPI;
import aniDB.udpApi.client.accountManagement.AccountManagement;
import aniDB.udpApi.client.registerManagement.RegisterManagement;
import aniDB.udpApi.client.requests.Cmd;

/**
 *
 * @author Arokh
 */
public class AA2AniDBUdpClient extends AA2Extension {
	private ClientAPI api;

	public AA2AniDBUdpClient() throws Exception {
		api = new ClientAPI();
	}



	public void queryCmd(Cmd cmd, Object handle) { api.queryCmd(cmd, handle); }
	public RegisterManagement getRegisterManagement() { return api.getRegisterManagement(); }
	public AccountManagement getAccountManagement() { return api.getAccountManagement(); }


	@Override
	protected void parentEventHandler(ComEvent comEvent) {
	}

	@Override
	protected void initializeInternal() {
		setState(State.Initialized);
	}

	public String getName() { return "AniDB UDP Client"; }

}

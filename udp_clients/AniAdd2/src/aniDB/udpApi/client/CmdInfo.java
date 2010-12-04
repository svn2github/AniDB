/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import aniDB.udpApi.client.requests.Cmd;
import aniDB.udpApi.shared.IdGenerator;
import java.util.Map.Entry;

/**
 *
 * @author Arokh
 */
public class CmdInfo {
	private Cmd cmd;
	private IdGenerator idGenerator;
	private Integer index;

	public CmdInfo(Cmd cmd, IdGenerator idGenerator, Integer index) {
		this.cmd = cmd;
		this.idGenerator = idGenerator;
		this.index = index;
	}

	

	public Cmd getCmd() {
		return cmd;
	}

	public IdGenerator getIdGenerator() {
		return idGenerator;
	}

	public Integer getIndex() {
		return index;
	}

	void setIndex(int index) {
		this.index = index;
	}

	String toString(String session) {
		if(index == null) throw new NullPointerException();

		String cmdStr;
		cmdStr = cmd.getAction() + " tag=" + Long.toString(idGenerator.generate(), 16) + ":" + (cmd.getTag() != null ? cmd.getTag() + ":" : "") + index;

		if(session != null && cmd.isAuthRequired()) cmdStr += "&s=" + session;

		for(Entry<String, String> entry : cmd) cmdStr += "&" + entry.getKey() + "=" + entry.getValue();
		return cmdStr;
	}
}

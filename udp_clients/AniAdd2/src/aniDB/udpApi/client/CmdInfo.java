/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import aniDB.udpApi.client.requests.Cmd;
import aniDB.udpApi.shared.IdGenerator;

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
		throw new UnsupportedOperationException("Not yet implemented");
	}
}

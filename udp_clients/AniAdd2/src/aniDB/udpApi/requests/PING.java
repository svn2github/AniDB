/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

import aniDB.udpApi.shared.CmdTools;

/**
 *
 * @author Arokh
 */
public abstract class PING<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public PING(CmdType cmd) {
		super("PING", cmd);
		dependencies = new Dependency[] {
			new Dependency(new String[0], new String[]{ "nat" })
		};
	}


	public Boolean isNAT() {
		return CmdTools.toBoolean(cmd, "nat");
	}

	public void setNAT(boolean nat) {
		cmd.add("nat", CmdTools.toString(nat));
	}
}

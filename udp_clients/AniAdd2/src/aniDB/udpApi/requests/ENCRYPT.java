/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

/**
 *
 * @author Arokh
 */
public class ENCRYPT<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public ENCRYPT(CmdType cmd) {
		super("ENCRYPT", cmd);
		dependencies = new Dependency[] {
			new Dependency(new String[] { "type", "user" }, new String[0])
		};
	}

	public String getType() { return cmd.get("type"); }
	public void setType(String type) { cmd.add("type", type); }
}

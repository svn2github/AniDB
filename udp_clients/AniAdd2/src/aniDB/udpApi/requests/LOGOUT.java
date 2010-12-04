/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

/**
 *
 * @author Arokh
 */
public class LOGOUT<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public LOGOUT(CmdType cmd) { super("LOGOUT", cmd); }

}

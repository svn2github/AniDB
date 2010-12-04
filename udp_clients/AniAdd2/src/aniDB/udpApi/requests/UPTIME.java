/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

/**
 *
 * @author Arokh
 */
public class UPTIME<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public UPTIME(CmdType cmd) { super("UPTIME", cmd); }

}

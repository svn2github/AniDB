/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

/**
 *
 * @author Arokh
 */
public class PUSH<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public PUSH(CmdType cmd) { super("PUSH", cmd); }

}

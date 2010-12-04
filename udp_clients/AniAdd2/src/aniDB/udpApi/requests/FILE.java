/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

/**
 *
 * @author Arokh
 */
public class FILE<CmdType extends Cmd> extends ExtCmd<CmdType> {
	public FILE(CmdType cmd) { super("FILE", cmd); }

}

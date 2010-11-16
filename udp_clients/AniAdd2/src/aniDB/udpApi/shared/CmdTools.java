/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

import aniDB.udpApi.requests.Cmd;

/**
 *
 * @author Arokh
 */
public class CmdTools {
	public static Boolean ToBoolean(Cmd cmd, String param) {
		if(cmd.get(param) == null || cmd.get(param).isEmpty()) return null;
		return (cmd.get(param).equals("1")) ? true : false;
	}
	public static Integer ToInt(Cmd cmd, String param) {
		if(cmd.get(param) == null || cmd.get(param).isEmpty()) return null;
		return Integer.getInteger(cmd.get(param));
	}
	public static Long ToLong(Cmd cmd, String param) {
		if(cmd.get(param) == null || cmd.get(param).isEmpty()) return null;
		return Long.getLong(cmd.get(param));
	}

	public static String ToString(boolean value){
		return value ? "1" : "0";
	}
}

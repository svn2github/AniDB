/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.requests;

import java.util.Map.Entry;

/**
 *
 * @author Arokh
 */
public abstract class ExtCmd<CmdType extends Cmd> {
	protected Dependency[] dependencies;
	protected CmdType cmd;

	public String getTag() { return cmd.getTag(); }
	public boolean setTag(String tag) {
		if(cmd.isFinal()) return false;
		cmd.setTag(tag);
		return true;
	}

	public boolean CheckValidity() {
		if(dependencies == null) return true;
		
		boolean isValid = true;
		boolean hasMatch;
		long reqParamSet;
		for(Dependency dependency : dependencies) {
			reqParamSet = 0;
			isValid = true;

			for(Entry<String, String> param : cmd) {
				hasMatch = false;
				for(int i = 0;i < dependency.required.length;i++) {
					if(dependency.required[i].equals(param.getKey()) && param.getValue() != null) {
						reqParamSet |= 1 << i;
						hasMatch = true;
						break;
					}
				}

				for(int i = 0;i < dependency.optional.length && !hasMatch;i++) {
					if(dependency.optional[i].equals(param.getKey()) && param.getValue() != null) {
						hasMatch = true;
						break;
					}
				}

				if(!hasMatch && !param.getKey().equals("tag") && !param.getKey().equals("s")) isValid = false;
			}
			for(int i = 0;i < dependency.required.length;i++) if((reqParamSet & (1 << i)) == 0) isValid = false;

			if(isValid) break;
		}
		return isValid;
	}

	public static class Dependency {
		public String[] required;
		public String[] optional;
	}
}

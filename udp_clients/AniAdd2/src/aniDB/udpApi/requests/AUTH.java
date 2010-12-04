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
public abstract class AUTH<CmdType extends Cmd> extends ExtCmd<CmdType> {

	public AUTH(CmdType cmd) {
		super("AUTH", cmd);
		dependencies = new Dependency[] {
			new Dependency(new String[]{"user", "pass", "protover", "client", "clientver"}, new String[]{"nat", "comp", "enc", "mtu"})
		};
	}

	public String getPassword() { return cmd.get("pass"); }
	public int getProtocolVersion() { return CmdTools.toInt(cmd, "protover"); }
	public String getClientName() { return cmd.get("client"); }
	public int getClientVersion() { return CmdTools.toInt(cmd, "clientver"); }
	public Boolean isNAT() { return CmdTools.toBoolean(cmd, "nat"); }
	public Boolean hasCompression() { return CmdTools.toBoolean(cmd, "comp"); }
	public Integer getMTU() { return CmdTools.toInt(cmd, "mtu"); }
	public String getEncoding() { return cmd.get("enc"); }

	public void setPassword(String password) { if(!cmd.isFinal()) cmd.add("pass", password); }
	public void setProtocolVersion(int version) { if(!cmd.isFinal()) cmd.add("protover", version + ""); }
	public void setClientName(String name) { if(!cmd.isFinal()) cmd.add("client", name.toLowerCase()); }
	public void setClientVersion(int version) { if(!cmd.isFinal()) cmd.add("clientver", version + ""); }
	public void setNAT(boolean nat) { if(!cmd.isFinal()) cmd.add("nat", CmdTools.toString(nat)); }
	public void setCompression(boolean compression) { if(!cmd.isFinal()) cmd.add("comp", CmdTools.toString(compression)); }
	public void setMTU(int MTU) { if(!cmd.isFinal()) cmd.add("mtu", MTU + ""); }
	public void setEncoding(String encoding) { if(!cmd.isFinal()) cmd.add("enc", encoding); }


	public static class EncodingTypes {
		public static final String ASCII = "";
		public static final String UTF8 = "UTF8";
	}

	
}

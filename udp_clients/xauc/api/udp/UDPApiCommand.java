package api.udp;

import java.util.Map;
import java.util.TreeMap;

import api.ApiCommand;

/**
 * Describes an UDP API Command
 * @author fahrenheit
 * @author Arokh
 */
public class UDPApiCommand implements ApiCommand {
	/** User tag, used to identify a command sent to the server */
	public String tag;
	/** Action to execute */
	public String action;
	/** If set to true, login is required for this command */
	public boolean loginReq;
	/** Query id */
	public int queryID = -1;
	/** Parameters associated with this command */
	public Map<String, String> params = new TreeMap<String, String>();

	/** Default UDPApiCommand constructor
	 * @param action Action to perform  
	 * @param tag Tag that identifies this command
	 * @param loginReq If set to true, this command requires login
	 */
	public UDPApiCommand(String action, String tag, boolean loginReq) {
		this.tag = tag;
		this.action = action;
		this.loginReq = loginReq;
	}
	/**
	 * Copy UDPApiCommand constructor
	 * @param cmd A UDP API Command from which to copy values
	 */
	public UDPApiCommand(UDPApiCommand cmd) {
		tag = cmd.tag;
		action = cmd.action;
		loginReq = cmd.loginReq;
		params.putAll(cmd.params);
	} 
}

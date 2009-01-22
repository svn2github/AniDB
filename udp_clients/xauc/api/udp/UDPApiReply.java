package api.udp;

import java.util.ArrayList;

import api.ApiReply;

/**
 * Describes an UDP API Reply
 * @author fahrenheit
 * @author Arokh
 */
public class UDPApiReply implements ApiReply {
	/** Tag used by the client to identify this command */
	public String tag;
	/** Command ID associated with this reply */
	public int cmdId;
	/** Response ID */
	public int responseID;
	/** Response text */
	public String response;
	/** Response fields */
	public ArrayList<String> dataField = new ArrayList<String>();
}

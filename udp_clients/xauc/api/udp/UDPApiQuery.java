package api.udp;

import java.util.Date;

import api.ApiQuery;

/**
 * Describes an UDP API Query
 * @author fahrenheit
 * @author Arokh
 */
public class UDPApiQuery implements ApiQuery {
	/** The UDP API command sent */
	public UDPApiCommand sentCmd;
	/** The UDP API reply */
	public UDPApiReply rcvdMsg;
	/** Send timestamp */
	public Date sentOn = new Date();
	/** Receive timestamp */
	public Date rcvdOn = new Date();
	/** Number of retries */
	public int retries;
	/** Indicates if query had success */
	public boolean success;
}

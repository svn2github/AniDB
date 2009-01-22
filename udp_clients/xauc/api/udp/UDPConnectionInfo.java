package api.udp;

import java.util.Date;
import java.net.InetAddress;

import api.ConnectionInfo;

/**
 * Describes UDP API Connection information 
 * @author fahrenheit
 * @author Arokh
 */
public class UDPConnectionInfo implements ConnectionInfo {
	/** AniDB API hostname, this value is editable by the user */
	public String hostname = "api.anidb.net";
	/** AniDB API port number, this value is editable by the user */
	public int port = 9000;
	/** The IP associated with the AniDB hostname */
	public InetAddress AniDBIP;

	/** API Protocol version this client supports */
	protected int protoVer = 3;
	/** Client name */
	protected String clientID = "xauc";
	/** Client version */
	protected int clientVer = 2;

	/** Session key */
	public String session;
	/** If should use a encoding set */
	public boolean encodingSet;
	/** If NAT handling should be used */
	public boolean NAT;
	/** If client is authenticated */ 
	public boolean isAuthed;
	/** Timestamp of connection */
	public Date connectedOn;
	/** Timestamp of last packet */
	public Date lastPacketOn;

	/** Indicates if the AniDB API is down */
	public boolean AniDBApiDown;
	/** Estimate of when the AniDB API will be up */
	public Date AniDBApiDownUntil;
	
	/** This sets if the client should get init messages */
	public boolean getInitMessages;
	/** This sets if the client should get push messages */
	public boolean getPushMessages;
	/** This sets if the client should use encryption */
	public boolean encryption;
	/** Ping interval time (in seconds) */
	public int pingInterval;
	
	public String getClientName() { return this.clientID; }
	public int getClientVersion() { return this.clientVer; }
	public int getProtocolVersion() { return this.protoVer; }
}

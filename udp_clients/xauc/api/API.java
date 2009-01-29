package api;

import java.lang.reflect.Method;

import utils.Log;

/**
 * This class represents a generic API.
 * <p>You need to instantiate specific APIs, like UDPApi, for usage.</p>
 * @author fahrenheit
 */
public interface API {
	/** Maximum number of connection tries (default: 3) */
	public static int MAX_CONNECTION_TRIES = 3;
	/** Packet send interval in ms (default: 2200) */
	public static int PACKET_SEND_INTERVAL = 2200; 
	
	/**
	 * Adds an handler to replies associated with a given tag
	 * @param tag Command tag for which this handler will be associated
	 * @param method Method name that will handle these types of messages
	 * @param methodContainer Class name of where the method that will handle these types of messages exists
	 */
	public void addHandler(String tag, Method method, Object methodContainer);
	/**
	 * Adds an handler to Error replies associated with a given numeric ID
	 * @param ID Numeric ID for which this handler will be associated
	 * @param method Method name that will handle these types of errors
	 * @param methodContainer Class name of where the method that will handle these types of errors exists
	 */
	public void addHandler(int ID, Method method, Object methodContainer);
	/**
	 * Remove an handler associated with a given message type
	 * @param tag Command tag associated with the handler to remove
	 */
	public void removeHandler(String tag);
	/**
	 * Remove an handler associated with a given error id
	 * @param ID Numeric ID associated with the handler to remove
	 */
	public void removeHandler(int ID);
	/** Connects to the server 
	 * @throws Throwable */
	public void connect();
	/** Disconnects from the server and cleans up */
	public void disconnect();
	/** Logs in to the AniDB API (and issues a connect request if the connection is down) */
	public void login();
	/** Logs out of the AniDB API (and cleans up) */
	public void logout();
	/** Gets the Log associated with this parser instance
	 * @return the log */
	public Log getLog();
	/** Sets the Log associated with this parser instance 
	 * @param log the Log to set */
	public void setLog(Log log);
	/** Checks if debug messages should be displayed or not
	 * @return true if debug messages should be displayed, false otherwise */
	public boolean isShowDebug();
	/** Defines if debug messages should be displayed or not 
	 * @param showDebug debug messages will be displayed if set to true, otherwise will be hidden */
	public void setShowDebug(boolean showDebug);
	/** Sends a command to the server */
	public void sendCommand(ApiCommand command);
}

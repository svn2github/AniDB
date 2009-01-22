package api;

/**
 * Defines Connection information with the AniDB api server.
 * @author fahrenheit
 */
public interface ConnectionInfo {
	/** Gets the client version */
	public int getClientVersion();
	/** Gets the Protocol version this client supports */
	public int getProtocolVersion();
	/** Gets the client name */
	public String getClientName();
}

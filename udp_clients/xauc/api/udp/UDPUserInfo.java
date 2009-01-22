package api.udp;

import api.UserInfo;

/**
 * Describes an UDP API User information 
 * @author fahrenheit
 * @author Arokh
 */
public class UDPUserInfo implements UserInfo {
	/** AniDB username */
	public String username;
	/** Session key */
	public String session;
	/** AniDB password (only used for applets tied with AniDB) */
	public String password;
	/** AniDB autopass */
	public String autopass;
	/** AniDB API password (as set in the user's profile, only used for encryption) */
	public String apiPassword;
}

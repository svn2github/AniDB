package api.udp;

import java.lang.reflect.Method;

import api.API;

/**
 * This is the main UDP API class.
 * <p>Clients should instantiate this class to handle UDP communication with AniDB.</p>
 * <p>Clients need to register their response handlers with this class before usage.<br>
 * This is done by accessing the addHandler() method and suppling the required fields.<br>
 * Please note that the handlers are for associated tags and not UDP API numeric replies (like: 200,401,etc).</p>
 * <p><b>Error handling:</b><br>This class will handle errors. Custom error handlers can be supplied by accessing the addErrorHandler() method.<br>
 * Please note that for error handlers the association is made by numeric UDP API replies and not by tags.</p> 
 * @author fahrenheit
 */
public class UDPApi implements API {

	@Override
	public void addErrorHandler(int returnId, Method function, Object methodContainer) {
		// TODO Auto-generated method stub
	}

	@Override
	public void addHandler(String tag, Method function, Object methodContainer) {
		// TODO Auto-generated method stub		
	}

	@Override
	public void removeErrorHandler(int returnId) {
		// TODO Auto-generated method stub		
	}

	@Override
	public void removeHandler(String tag) {
		// TODO Auto-generated method stub
	}

}

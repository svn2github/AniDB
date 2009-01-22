package api;

import java.lang.reflect.Method;

/**
 * This class represents a generic API.
 * <p>You need to instantiate specific APIs, like UDPApi, for usage.</p>
 * @author fahrenheit
 */
public interface API {
	/**
	 * Adds an handler to replies associated with a given tag
	 * @param tag Command tag for which this handler will be associated
	 * @param function Method name that will handle these types of messages
	 * @param methodContainer Class name of where the method that will handle these types of messages exists
	 */
	public void addHandler(String tag, Method function, Object methodContainer);
	/**
	 * Adds an handler to Error replies associated with a given numeric ID
	 * @param returnId Numeric ID for which this handler will be associated
	 * @param function Method name that will handle these types of errors
	 * @param methodContainer Class name of where the method that will handle these types of errors exists
	 */
	public void addErrorHandler(int returnId, Method function, Object methodContainer);
	/**
	 * Remove an handler associated with a given message type
	 * @param tag Command tag associated with the handler to remove
	 */
	public void removeHandler(String tag);
	/**
	 * Remove an handler associated with a given error id
	 * @param returnId Numeric ID associated with the handler to remove
	 */
	public void removeErrorHandler(int returnId);
}

package api.udp;

import java.lang.reflect.Method;

import api.RequestHandler;

/**
 * Describes an UDP API Request Flag.
 * <p>This class serves to associate an handler to a given Tag/Command ID.</p>
 * @author fahrenheit
 * @author Arokh
 */
public class UDPRequestHandler implements RequestHandler {
	/** Command id associated with this Request Handler */
	public int ID = 0;
	/** Command tag associated with this Request Handler */
	public String tag = null;
	/** Function associated with this Request Handler */
	public Method method = null;
	/** Method container associated with this Request Handler */
	public Object methodContainer = null;

	/**
	 * UDPRequestHandler constructor
	 * @param reference The Command ID/tag associated with this Request Handler
	 * @param isTag If set to true, this reference is a tag, otherwise it's a Command ID 
	 * @param method Name of the method that will take care of the handling of this tag
	 * @param methodContainer Class where the handling method is located
	 */
	protected UDPRequestHandler(Object reference, boolean isTag, Method method, Object methodContainer) {
		if (isTag) this.tag = (String)reference;
		else this.ID = (Integer)reference;
		this.method = method;
		this.methodContainer = methodContainer;
	}
	
	/**
	 * Tag based UDPRequestHandler constructor
	 * @param tag Command tag associated with this Request Handler
	 * @param method Name of the method that will take care of the handling of this tag
	 * @param methodContainer Class where the handling method is located
	 */
	public UDPRequestHandler(String tag, Method method, Object methodContainer) {
		this(tag,true,method,methodContainer);
	}
	
	/**
	 * Command ID based UDPRequestHandler constructor
	 * @param ID Command ID associated with this Request Handler
	 * @param method Name of the method that will take care of the handling of this command ID
	 * @param methodContainer Class where the handling method is located
	 */
	public UDPRequestHandler(int ID, Method method, Object methodContainer) {
		this(ID,false,method,methodContainer);
	}
}

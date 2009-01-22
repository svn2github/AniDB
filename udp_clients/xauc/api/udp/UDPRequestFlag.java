package api.udp;

import java.lang.reflect.Method;

/**
 * Describes an UDP API Request Flag.
 * <p>This class serves to associate an handler to a given tag.</p>
 * @author fahrenheit
 * @author Arokh
 */
public class UDPRequestFlag {
	/** Command tag associated with this ReqFlag */
	public String tag;
	/** Function associated with this ReqFlag */
	public Method func;
	/** Method container associated with this ReqFalg */
	public Object methodContainer;

	/**
	 * UDPReqFlag Default constructor
	 * @param tag New command tag associated with this ReqFlag
	 * @param func New function associated
	 * @param methodContainer New methodContainer associated
	 */
	public UDPRequestFlag(String tag, Method func, Object methodContainer) {
		this.tag = tag;
		this.func = func;
		this.methodContainer = methodContainer;
	}
}

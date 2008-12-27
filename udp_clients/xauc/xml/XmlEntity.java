package xml;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * A xml Entitiy
 * @author fahrenheit
 */
public class XmlEntity extends XmlElement {
	protected HashMap<String,String> enAttributes; 
	
	public XmlEntity() {
		this.name = "ENTITY";
		this.enAttributes = new HashMap<String,String>();
	}
	
	public XmlEntity(ArrayList<String> attributes) throws XmlObjectException, UnsupportedEncodingException {
		this();
		Iterator<String> it = attributes.iterator();
		while(it.hasNext()) {
			String attribute = it.next();
			this.enAttributes.put(attribute, attribute);
		}
	}
	
	/**
	 * Used to set Attributes of a Xml Entity
	 * @param attributes An ArrayList of Value strings
	 * @return The attributes that were sucessfuly added
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException 
	 */
	public synchronized ArrayList<String> setEnAttributes(ArrayList<String> attributes) throws XmlObjectException, UnsupportedEncodingException {
		ArrayList<String> added = new ArrayList<String>();
		Iterator<String> it = attributes.iterator();
		while(it.hasNext()) {
			String attribute = it.next();
			this.enAttributes.put(attribute,attribute);
			added.add(attribute);
		}
		return added;
	}
	
	/**
	 * Adds or updates (if key already exists) an attribute of the Xml Entity
	 * @param attribute A value which is the attribute
	 * @return The added attribute
	 * @throws XmlObjectException When an invalid attribute key is supplied
	 */
	public synchronized String addEnAttribute(String attribute) throws XmlObjectException {
		this.enAttributes.put(attribute,attribute);
		return attribute;
	}
	
	/**
	 * Removes an attribute from the Xml Entity
	 * @param key Name of the attribute to remove (key)
	 * @return The attribute 
	 * @throws XmlObjectException ?
	 */
	public synchronized String remEnAttribute(String key) throws XmlObjectException {
		String att = null;
		if (this.enAttributes.containsKey(key)) {
			String attribute = this.enAttributes.get(key);
			att = attribute;
			this.enAttributes.remove(key);
		}
		return att;
	}
	
	/**
	 * Outputs the Xml Entity as a String
	 * @return The XML string
	 */
	public synchronized String toString() {
		String result = "<!"+this.name;
		String attributes = "";
		if (this.enAttributes.size() > 0) {
			attributes += " ";
			Iterator<String> it = this.enAttributes.values().iterator();
			int i = 0;
			while(it.hasNext()) {
				String attribute = it.next();
				attributes += attribute;
				if (i < this.enAttributes.size()) attributes += " ";
				i++;
			}
		}
		result += ">";
		return result;
	}
}

package xml;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.regex.*;

/**
 * Generic XML Element
 * @author fahrenheit
 */
public abstract class XmlElement {
	protected String name;
	protected HashMap<String,String[]> attributes;
	
	protected boolean escapeValues = false; // false means use encloseStr
	protected Pattern nameTest = Pattern.compile("[^\\w\\.\\-\\:]+",Pattern.MULTILINE&Pattern.CASE_INSENSITIVE);
	protected Pattern encloseTest = Pattern.compile("[&\"\'><]+",Pattern.MULTILINE&Pattern.CASE_INSENSITIVE);

	/**
	 * Function that escapes text
	 * @param str String to escape
	 * @return Escaped String
	 * @throws UnsupportedEncodingException 
	 */
	protected String escapeStr(String str) throws UnsupportedEncodingException {
		str = new String(str.getBytes(),"UTF-8");
    	str = str.replace("&", "&amp");
    	str = str.replace("<", "&lt;");
    	str = str.replace(">", "&gt;");
    	str = str.replace("\"", "&quot;");
    	str = str.replace("'", "&apos;");
        return str;
    }
    
    /**
     * Function that encloses a string in a CDATA element
     * @param str String to process
     * @return CDATA enclosed string
     * @throws UnsupportedEncodingException 
     */
	protected String encloseStr(String str) throws UnsupportedEncodingException {
		if (str == null || str.length() < 1) return str;
		str = new String(str.getBytes(),"UTF-8");
		Matcher mat = encloseTest.matcher(str);
		if (mat.find()) return "<![CDATA["+str+"]]>";
    	return str;
    }
	
    /**
     * Function that encloses a string in a CDATA element
     * @param str String to process
     * @param force If set to true will force enclosing instead of finding for bad entities
     * @return CDATA enclosed string
     * @throws UnsupportedEncodingException 
     */
	protected String encloseStr(String str, boolean force) throws UnsupportedEncodingException {
		if (!force) return encloseStr(str);
		str = new String(str.getBytes(),"UTF-8");
		return "<![CDATA["+str+"]]>";
    }


	/**
	 * Default constructor of a Xml Element
	 * Use this if you wish to ensure syncronize
	 */
	public XmlElement() {
		this.name = null;
		this.attributes = new HashMap<String,String[]>();
	}
	
	/**
	 * Constructor which gives name
	 * @param name Name of the Element (only letters, digits, _, . and -)
	 * @throws XmlObjectException When invalid name is supplied
	 */
	public XmlElement(String name) throws XmlObjectException {
		this();
		Matcher test = nameTest.matcher(name); 
		if (test.find()) throw new XmlObjectException("Supplied name \""+name+"\" is Invalid.\nNames should only contain letters, numbers, _, . (dot) and - (hifen).");
		this.name = name;
	}

	/**
	 * Constructor which gives name and an ArrayList of Key,Pair values as attributes
	 * @param name Name of the Element (only letters, digits, _, . and -)
	 * @param attributes ArrayList of Element attributes
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlElement(String name, ArrayList<String[]> attributes) throws XmlObjectException, UnsupportedEncodingException {
		this(name);
		Iterator<String[]> it = attributes.iterator();
		// only add valid pairs and throw on invalid ones
		while(it.hasNext()) {
			String[] attribute = it.next();
			String key = attribute[0];
			String value = attribute[1];
			Matcher test = nameTest.matcher(key);
			if (test.find()) throw new XmlObjectException("Supplied key \""+key+"\" is Invalid.\nKeys should only contain letters, numbers, _, . (dot) and - (hifen).");
			String[] att = {key,escapeStr(value)};
			this.attributes.put(key, att);
		}
	}
	
	/**
	 * Sets the name of the Xml Element
	 * @param name Name of the Element (only letters, digits, _, . and -)
	 * @return The name
	 * @throws XmlObjectException When invalid name is supplied
	 */
	public synchronized String setName(String name) throws XmlObjectException {
		Matcher test = nameTest.matcher(name); 
		if (test.find()) throw new XmlObjectException("Supplied name \""+name+"\" is Invalid.\nNames should only contain letters, numbers, _, . (dot) and - (hifen).");
		this.name = name;
		return name;
	}
	
	/**
	 * Used to set Attributes of a Xml Element
	 * @param attributes An ArrayList of Key,Value strings
	 * @return The attributes that were sucessfuly added
	 * @throws XmlObjectException When an invalid attribute key is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public synchronized ArrayList<String[]> setAttributes(ArrayList<String[]> attributes) throws XmlObjectException, UnsupportedEncodingException {
		ArrayList<String[]> added = new ArrayList<String[]>();
		Iterator<String[]> it = attributes.iterator();
		// only add valid pairs and throw on invalid one
		while(it.hasNext()) {
			String[] attribute = it.next();
			String key = attribute[0];
			String value = attribute[1];
			Matcher test = nameTest.matcher(key);
			if (test.find()) throw new XmlObjectException("Supplied key \""+key+"\" is Invalid.\nKeys should only contain letters, numbers, _, . (dot) and - (hifen).");
			String[] att = {key,escapeStr(value)};
			this.attributes.put(key,att);
			added.add(att);
		}
		return added;
	}
	
	/**
	 * Adds or updates (if key already exists) an attribute of the Xml Element
	 * @param attribute A Key,Value pair which is the attribute
	 * @return The added attribute
	 * @throws XmlObjectException When an invalid attribute key is supplied
	 */
	public synchronized String[] addAttribute(String[] attribute) throws XmlObjectException {
		String key = attribute[0];
		Matcher test = nameTest.matcher(key);
		if (test.find()) throw new XmlObjectException("Supplied key \""+key+"\" is Invalid.\nKeys should only contain letters, numbers, _, . (dot) and - (hifen).");
		this.attributes.put(key,attribute);
		return attribute;
	}
	
	/**
	 * Adds or updates (if key already exists) an attribute of the Xml Element
	 * @param key The attribute key
	 * @param value The attribute value
	 * @return The added attribute
	 * @throws XmlObjectException When an invalid attribute key is supplied
	 */
	public synchronized String[] addAttribute(String key, String value) throws XmlObjectException {
		String[] attribute = {key,value};
		return addAttribute(attribute);
	}
	
	/**
	 * Removes an attribute from the Xml Element
	 * @param key Name of the attribute to remove (key)
	 * @return The Key/Value attribute pair 
	 * @throws XmlObjectException ?
	 */
	public synchronized String[] remAttribute(String key) throws XmlObjectException {
		String[] att = null;
		if (this.attributes.containsKey(key)) {
			String[] attribute = this.attributes.get(key);
			att = attribute;
			this.attributes.remove(key);
		}
		return att;
	}
	
	/**
	 * @return the escapeValue
	 */
	public synchronized boolean isEscapeValues() {
		return escapeValues;
	}

	/**
	 * Use this to control the behaviour of the usage of encloseStr or escapeStr
	 * @param escapeValues Set true to use escapeStr or false to use encloseStr
	 */
	public synchronized void setEscapeValues(boolean escapeValues) {
		this.escapeValues = escapeValues;
	}

	/**
	 * Use this to get the name of the Xml Element
	 * @return the name
	 */
	public synchronized String getName() {
		return name;
	}

	/**
	 * Use this to get a list of the attributes of the Xml Element
	 * @return the attributes
	 */
	public synchronized HashMap<String, String[]> getAttributes() {
		return attributes;
	}
}

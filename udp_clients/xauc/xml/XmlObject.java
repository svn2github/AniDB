package xml;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.regex.*;

/**
 * This class defines the XML document objects
 * @author fahrenheit
 */
public class XmlObject extends XmlElement {
	protected ArrayList<XmlObject> values;
	protected String value;

	/**
	 * Default constructor of a Xml Object
	 * Use this if you wish to ensure syncronize
	 */
	public XmlObject() {
		this.name = null;
		this.attributes = new HashMap<String,String[]>();
		this.values = new ArrayList<XmlObject>();
		this.value = null;
	}
	
	/**
	 * Constructor which gives name
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @throws XmlObjectException When invalid name is supplied
	 */
	public XmlObject(String name) throws XmlObjectException {
		this();
		Matcher test = nameTest.matcher(name); 
		if (test.find()) throw new XmlObjectException("Supplied name \""+name+"\" is Invalid.\nNames should only contain letters, numbers, _, . (dot) and - (hifen).");
		this.name = name;
	}
	
	/**
	 * Constructor which gives name and a text value
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @param value Value of the object (will be escaped)
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlObject(String name, String value) throws XmlObjectException, UnsupportedEncodingException {
		this(name);
		this.value = (escapeValues ? escapeStr(value) : encloseStr(value));
	}
	
	/**
	 * Constructor which gives name and a text value
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @param value Value of the object (will be escaped)
	 * @param encStr If true will encloseStr otherwise will use encodeStr
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlObject(String name, String value, boolean encStr) throws XmlObjectException, UnsupportedEncodingException {
		this(name);
		this.value = (encStr ? encloseStr(value, true) : escapeStr(value));
	}
	
	/**
	 * Constructor which gives name and an ArrayList of Key,Pair values as attributes
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @param attributes ArrayList of object attributes
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlObject(String name, ArrayList<String[]> attributes) throws XmlObjectException, UnsupportedEncodingException {
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
	 * Constructor for a Xml Object for which the user gives the name, attributes and a text value
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @param attributes ArrayList of object attributes
	 * @param value Value of the object (will be escaped)
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlObject(String name, ArrayList<String[]> attributes, String value) throws XmlObjectException, UnsupportedEncodingException {
		this(name, attributes);
		this.value = (escapeValues ? escapeStr(value) : encloseStr(value));
	}
	
	/**
	 * Constructor for a Xml Object for which the user gives the name, attributes and an ArrayList of Xml Objects as values
	 * @param name Name of the object (only letters, digits, _, . and -)
	 * @param attributes ArrayList of object attributes
	 * @param values ArrayList of Xml Objects
	 * @throws XmlObjectException When invalid name is supplied
	 * @throws UnsupportedEncodingException 
	 */
	public XmlObject(String name, ArrayList<String[]> attributes, ArrayList<XmlObject> values) throws XmlObjectException, UnsupportedEncodingException {
		this(name, attributes);
		this.values = values;
	}
	
	/**
	 * Used to set the value of a Xml Object
	 * @param value Value of the object (will be escaped or encolsed)
	 * @return The Value
	 * @throws XmlObjectException ?
	 * @throws UnsupportedEncodingException 
	 */
	public synchronized String setValue(String value) throws XmlObjectException, UnsupportedEncodingException {
		this.value = (escapeValues ? escapeStr(value) : encloseStr(value));
		return value;
	}
	
	/**
	 * Adds or Replaces the set of Xml Objects children
	 * @param values An ArrayList of Xml Objects
	 * @return The ArrayList of Xml Objects
	 * @throws XmlObjectException ?
	 */
	public synchronized ArrayList<XmlObject> setValues(ArrayList<XmlObject> values) throws XmlObjectException {
		this.values = values;
		this.value = null;
		return values;
	}
	
	/**
	 * Appends a new Xml Object to the list of Xml Objects
	 * @param obj The Xml Object to add
	 * @return The Xml Object
	 * @throws XmlObjectException ?
	 */
	public synchronized XmlObject addValue(XmlObject obj) throws XmlObjectException {
		this.values.add(obj);
		this.value = null;
		return obj;
	}
	
	/**
	 * Adds a string as the child of the the Xml Object (dropping existing childrens in the process)
	 * @param value The string to add as child of this Xml Object
	 * @return The given string
	 * @throws XmlObjectException ?
	 * @throws UnsupportedEncodingException 
	 */
	public synchronized String addValue(String value) throws XmlObjectException, UnsupportedEncodingException {
		this.values = new ArrayList<XmlObject>();
		this.value = (escapeValues ? escapeStr(value) : encloseStr(value));
		return value;
	}
	
	/**
	 * Adds a string as the child of the the Xml Object (dropping existing childrens in the process)
	 * @param value The string to add as child of this Xml Object
	 * @param encStr If set to true will use encloseStr otherwise will use escapeStr
	 * @return The given string
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException 
	 */
	public synchronized String addValue(String value, boolean encStr) throws XmlObjectException, UnsupportedEncodingException {
		this.values = new ArrayList<XmlObject>();
		this.value = (encStr ? encloseStr(value) : escapeStr(value));
		return value;
	}

	/**
	 * Use this to get a list of the Children (if the list is empty try using getValue)
	 * @return the values
	 */
	public synchronized ArrayList<XmlObject> getValues() {
		return values;
	}

	/**
	 * Use this to get the text of text only children (if null try using getValues)
	 * @return the value
	 */
	public synchronized String getValue() {
		return value;
	}

	/**
	 * Outputs the Xml Object as a String
	 * @return The XML string
	 */
	public synchronized String toString() {
		String result = "<"+this.name;
		String attributes = "";
		if (this.attributes.size() > 0) {
			attributes += " ";
			Iterator<String[]> it = this.attributes.values().iterator();
			int i = 0;
			while(it.hasNext()) {
				String[] attribute = it.next();
				attributes += attribute[0]+"=\""+attribute[1]+"\"";
				if (i < this.attributes.size()-1) attributes += " ";
				i++;
			}
		}
		if (attributes.length() > 0) result += attributes;
		String childs = "";
		if (this.value != null) childs = this.value;
		else if (this.values.size() > 0) {
			childs += '\n';
			Iterator<XmlObject> it = this.values.iterator();
			int i = 0;
			while(it.hasNext()) {
				XmlObject temp = it.next();
				childs += temp.toString();
				if (i < this.values.size()) childs += '\n';
				i++;
			}
		}
		if (childs.length() > 0) {
			result += ">";
			result += childs;
			result += "</"+this.name+">";
		} else {
			result += " />";
		}
		return result;
	}
}

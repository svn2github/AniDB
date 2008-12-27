package xml;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * This Class serves as a XML document generator
 * @author fahrenheit
 */
public class XmlDoc {
	protected String version;
	protected String encoding;
	protected ArrayList<XmlObject> children;
	
	/**
	 * Creates a basic XML Document
	 * @throws XmlObjectException ?
	 */
	public XmlDoc() throws XmlObjectException {
		this.version = "1.0";
		this.encoding = "utf-8";
		this.children = new ArrayList<XmlObject>();
	}
	
	/**
	 * Creates a XML document where all the elements are specified
	 * @param version XML version (default: 1.0)
	 * @param encoding Document encoding (default: utf-8) 
	 * @param children An ArrayList of Xml Objects to append as Children
	 * @throws XmlObjectException ?
	 */
	public XmlDoc(String version, String encoding, ArrayList<XmlObject> children) throws XmlObjectException {
		this.version = version;
		this.encoding = encoding;
		this.children = children;
	}
	
	/**
	 * Creates a XML document where only the version and encoding are specified
	 * @param version XML version (default: 1.0)
	 * @param encoding Document encoding (default: utf-8) 
	 * @throws XmlObjectException ?
	 */
	public XmlDoc(String version, String encoding) throws XmlObjectException  {
		this();
		this.version = version;
		this.encoding = encoding;
	}

	/**
	 * Creates a XML document where only the children are specified 
	 * @param children An ArrayList of Xml Objects to append as Children
	 * @throws XmlObjectException ?
	 */
	public XmlDoc(ArrayList<XmlObject> children) throws XmlObjectException  {
		this();
		this.children = children;
	}
	
	/**
	 * Sets document's XML version
	 * @param version The version to set (default: 1.0)
	 */
	public synchronized void setVersion(String version) {
		this.version = version;
	}
	
	/**
	 * Sets document's encoding
	 * @param encoding The encoding to set (default: utf-8)
	 */
	public synchronized void setEncoding(String encoding) {
		this.encoding = encoding;
	}
	
	/**
	 * Gets the document's XML version
	 * @return the version (default: 1.0)
	 */
	public synchronized String getVersion() {
		return this.version;
	}
	
	/**
	 * Gets the document's encoding
	 * @return the encoding (default: utf-8)
	 */
	public synchronized String getEncoding() {
		return this.encoding;
	}

	/**
	 * Gets the document's children
	 * @return the children
	 */
	public synchronized ArrayList<XmlObject> getChildren() {
		return children;
	}

	/**
	 * Use this to add/replace the document's children
	 * @param children the children to set
	 */
	public synchronized void setChildren(ArrayList<XmlObject> children) {
		this.children = children;
	}
	
	/**
	 * Adds a children to the documents list of children
	 * @param child The child to add
	 * @return The added child
	 * @throws XmlObjectException ?
	 */
	public synchronized XmlObject addChild(XmlObject child) throws XmlObjectException {
		this.children.add(child);
		return child;
	}
	
	/**
	 * Outputs the Xml document as a String
	 * @return The XML document string
	 */
	public synchronized String toString() {
		String result = "<?xml version=\""+this.version+"\" encoding=\""+this.encoding+"\" ?>\n";
		Iterator<XmlObject> it = this.children.iterator();
		int i = 0;
		while(it.hasNext()) {
			XmlObject temp = it.next();
			result += temp.toString();
			if (i < this.children.size()) result += '\n';
			i++;
		}
		return result;
	}
}

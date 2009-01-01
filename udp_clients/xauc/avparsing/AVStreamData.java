package avparsing;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import xml.XmlDoc;
import xml.XmlObject;
import xml.XmlObjectException;

/**
 * AV Stream Wrapper
 */
public class AVStreamData {
	public long size = 0; // in bytes
	public float duration = 0; // in seconds
	public String formatedDuration = "";
	public double bitrate = 0;
	public double timebase = 0;
	public String codecName = "";
	public String codecTag = "";
	public int type = -1;
	public String typeName = "other";
	public int streamId = 0;
	public AVStreamData() {}
	
	/**
	 * Formats a duration time in seconds to an hh:mm:ss representation
	 * @param time The time
	 * @return Formated time
	 */
	public String formatDurationSecs(double time) {
		String seconds = Integer.toString((int)(time % 60));
		String minutes = Integer.toString((int)((time % 3600) / 60));
		String hours = Integer.toString((int)(time / 3600));
		for (int i = 0; i < 2; i++) {
			if (seconds.length() < 2) seconds = "0" + seconds;
			if (minutes.length() < 2) minutes = "0" + minutes;
			if (hours.length() < 2) hours = "0" + hours;
		}
		return hours+":"+minutes+":"+seconds;
	}

	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [other]:");
		out.println("\t\tcodec: "+this.codecName);
		if (this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration));
		if (this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
	}
	
	/**
	 * Writes the Stream to System.out
	 */
	public synchronized void writeToConsole() {
		writeToFile(System.out);
	}
	
	/**
	 * Creates a xml object of the audio stream
	 * @return the xml object
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized XmlObject getXmlObject() throws XmlObjectException, UnsupportedEncodingException {
		XmlObject xml = new XmlObject("stream");
		xml.addValue(new XmlObject("type","other"));
		xml.addValue(new XmlObject("codec",this.codecName));
		if (this.size > 0) xml.addValue(new XmlObject("size",""+this.size));
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)this.duration));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		return xml;
	}
	
	/**
	 * Gets a valid XML Document complete with header for the current object
	 * @return XML document
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized String getXml() throws XmlObjectException, UnsupportedEncodingException {
		XmlDoc doc = new XmlDoc();
		doc.addChild(getXmlObject());
		return doc.toString();
	}
}

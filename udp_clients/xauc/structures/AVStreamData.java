package structures;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import xml.XmlDoc;
import xml.XmlObject;
import xml.XmlObjectException;

/**
 * AV Stream information holder.
 */
public class AVStreamData {
	/** Stream size in bytes */
	public long size = 0;
	/** Stream duration in seconds */
	public double duration = 0;
	/** Formated stream duration (hh:mm:ss) */
	public String formatedDuration = "";
	/** Stream bitrate in bps */
	public double bitrate = 0;
	/** Codec id (AVCodecLibrary.CODEC_ID_*) */
	public int codec_id;
	/** Codec name */
	public String codecName = "";
	/** Long codec name (more informative) */
	public String codecNameLong = "";
	/** Codec tag (for stuff like XVID in mpeg4 codecs) */
	public String codecTag = "";
	/** True if stream is VFR (only use this if fullParsed == true) */
	public boolean isVFR = false;
	/** True if stream is VBR (only use this if fullParsed == true) */
	public boolean isVBR = false;
	// Stream identification stuff
	/** Stream type */
	public int type = -1;
	/** Stream type name */
	public String typeName = "other";
	/** File based stream id */
	public int streamId = 0;
	/** Stream id in relation to other streams of the same type */
	public int relStreamId = 0;
	// FullParse auxiliar stuff
	/** If stream has been fully parsed this will be set to true */
	public boolean fullParsed = false;
	/** PresentationTimeStamp of the last frame */
	public long pts = 0;
	/** A PresentationTimeStamp array for VFR calculations */
	public ArrayList<Long> ptsArray = new ArrayList<Long>();
	/** The last packet bitrate for VBR calculations */
	public double lastPacketBitrate = 0;
	/** The last packet size for VBR calculations */
	public int lastPacketSize = 0;
	/** Stream timebase unit */
	public double timebase = 0;
	
	/** Default AVStreamData constructor */
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
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [other:"+this.relStreamId+"]:"+'\n');
		out.append("\t\tcodec: "+this.codecName+'\n');
		if (this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+'\n');
		if (this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [other:"+this.relStreamId+"]:");
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

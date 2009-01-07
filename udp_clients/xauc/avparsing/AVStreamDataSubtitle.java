package avparsing;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import xml.*;

/**
 * Subtitle stream data wrapper
 */
public class AVStreamDataSubtitle extends AVStreamData {
	public String language = "";
	public int subtitleType = 0;
	public boolean isForCommentary = false;
	public boolean isForForeignDub = false;
	public boolean isForHearingImpaired = false;
	public boolean isImageSubs = false;
	public boolean isStyledSubs = false;
	public boolean isUnstyledSubs = false;
	public AVStreamDataSubtitle() { super(); }
	
	/**
	 * Get the short subtitle type name
	 * @return String with subtitle type
	 */
	public synchronized String mapSubtitleTypeShort() {
		switch (this.subtitleType) {
			case 10: return "hard";
			case 20: return "soft";
			case 30: return "sup. soft";
			case 100: return "other";
			default: return "unknown";
		}
	}
	
	/**
	 * Get the long subtitle type name
	 * @return String with subtitle type
	 */
	public synchronized String mapSubtitleTypeLong() {
		switch (this.subtitleType) {
			case 10: return "hard sub";
			case 20: return "soft sub";
			case 30: return "supplementary soft sub (i.e. sign translations)";
			case 100: return "other";
			default: return "unknown";
		}
	}
	
	/**
	 * Gets the integer with the subtitle flags
	 * @return flags
	 */
	public synchronized int getSubtitleFlags() {
		int flags = 0;
		if (this.isForForeignDub) flags += 8;
		if (this.isForHearingImpaired) flags += 16;
		if (this.isForCommentary) flags += 32;
		if (this.isStyledSubs) flags += 64;
		if (this.isImageSubs) flags += 128;
		if (this.isUnstyledSubs) flags += 256;
		return flags;
	}
	
	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [subtitle]:"+'\n');
		out.append("\t\ttype: "+mapSubtitleTypeLong()+'\n');
		out.append("\t\tcodec: "+this.codecName+'\n');
		if (this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+'\n');
		if (this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		if (this.language != "") out.append("\t\tlanguage: "+this.language+'\n');
		if (this.isForForeignDub) out.append("\t\tfor foreign dub"+'\n');
		if (this.isForHearingImpaired) out.append("\t\tfor hearing impaired"+'\n');
		if (this.isForCommentary) out.append("\t\tfor commentary audio stream"+'\n');
		if (this.isStyledSubs) out.append("\t\tstyled subs (ASS/SSA)"+'\n');
		if (this.isImageSubs) out.append("\t\timage subs (VOBSUB)"+'\n');
		if (this.isUnstyledSubs) out.append("\t\tunstyled subs (SRT)"+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [subtitle]:");
		out.println("\t\ttype: "+mapSubtitleTypeLong());
		out.println("\t\tcodec: "+this.codecName);
		if (this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration));
		if (this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
		if (this.language != "") out.println("\t\tlanguage: "+this.language);
		if (this.isForForeignDub) out.println("\t\tfor foreign dub");
		if (this.isForHearingImpaired) out.println("\t\tfor hearing impaired");
		if (this.isForCommentary) out.println("\t\tfor commentary audio stream");
		if (this.isStyledSubs) out.println("\t\tstyled subs (ASS/SSA)");
		if (this.isImageSubs) out.println("\t\timage subs (VOBSUB)");
		if (this.isUnstyledSubs) out.println("\t\tunstyled subs (SRT)");
	}
	
	/**
	 * Creates a xml object of the subtitle stream
	 * @return the xml object
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized XmlObject getXmlObject() throws XmlObjectException, UnsupportedEncodingException {
		XmlObject xml = new XmlObject("stream");
		if (this.size > 0) xml.addValue(new XmlObject("size",""+this.size));
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)(this.duration*this.timebase)));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		xml.addValue(new XmlObject("lang",this.language));
		xml.addValue(new XmlObject("type",mapSubtitleTypeShort()));
		int flags = getSubtitleFlags();
		if (flags > 0) xml.addValue(new XmlObject("flags",""+flags));
		return xml;
	}
}

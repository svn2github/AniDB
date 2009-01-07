package avparsing;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import xml.XmlObject;
import xml.XmlObjectException;

/**
 * Audio stream data wrapper
 */
public class AVStreamDataAudio extends AVStreamData {
	public String language = "";
	public int sampleRate = 0; // in Hz
	public String sampleFormat = "";
	public int channels = 0;
	public String layout = "";
	public int audioType = 10;
	public boolean isVBR = false;
	public AVStreamDataAudio() { super(); }
	
	public synchronized String mapAudioType() {
		switch (this.audioType) {
			case 10: return "normal";
			case 20: return "commentary";
			case 30: return "fandub";
			case 40: return "alt. voiceover";
			case 100: return "other";
			default: return "unknown";
		}
	}
	
	/**
	 * Convert audio channels to anidb representation
	 * @return
	 */
	public synchronized String mapAudioChannels() {
		switch(this.channels) {
			case 0: return "unknown"; 
			case 1: return "1.0";
			case 2: return "2.0";
			case 3: return "2.1";
			case 4: return "4.0";
			case 6: return "5.1";
			case 7: return "6.1";
			case 8: return "7.1";
			default: return "other";
		}
	}

	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [audio]:"+'\n');
		out.append("\t\ttype: "+mapAudioType()+'\n');
		out.append("\t\tcodec: "+this.codecName+'\n');
		if (this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")"+'\n');
		if (this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		if (this.language != "") out.append("\t\tlanguage: "+this.language+'\n');
		out.append("\t\tchannels: "+this.layout+'\n');
		if (this.sampleRate > 0) out.append("\t\tsample rate: "+this.sampleRate+ " Hz"+'\n');
		if (this.sampleFormat != "") out.append("\t\tsample format: "+this.sampleFormat+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [audio]:");
		out.println("\t\ttype: "+mapAudioType());
		out.println("\t\tcodec: "+this.codecName);
		if (this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")");
		if (this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
		if (this.language != "") out.println("\t\tlanguage: "+this.language);
		out.println("\t\tchannels: "+this.layout);
		if (this.sampleRate > 0) out.println("\t\tsample rate: "+this.sampleRate+ " Hz");
		if (this.sampleFormat != "") out.println("\t\tsample format: "+this.sampleFormat);
	}
	
	/**
	 * Creates a xml object of the audio stream
	 * @return the xml object
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized XmlObject getXmlObject() throws XmlObjectException, UnsupportedEncodingException {
		XmlObject xml = new XmlObject("stream");
		if (this.size > 0) xml.addValue(new XmlObject("size",""+this.size));
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)this.duration));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		xml.addValue(new XmlObject("type",mapAudioType()));
		if (this.language != "") xml.addValue(new XmlObject("lang",this.language));
		xml.addValue(new XmlObject("codec",this.codecName));
		xml.addValue(new XmlObject("chan",mapAudioChannels()));
		if (this.sampleRate > 0) xml.addValue(new XmlObject("sr",""+this.sampleRate));
		if (this.sampleFormat != "") xml.addValue(new XmlObject("sf",this.sampleFormat));
		return xml;
	}
}

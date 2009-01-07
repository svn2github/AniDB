package avparsing;

import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import xml.XmlObject;
import xml.XmlObjectException;

/**
 * Video stream data wrapper
 */
public class AVStreamDataVideo extends AVStreamData {
	public String pictureFormat = "";
	public int width = 0;
	public int height = 0;
	public int cwidth = 0;
	public int cheight = 0;
	public String resolution = "";
	public String cresolution = "";
	public double fps = 0;
	public double ar = 0;
	public double car = 0;
	public boolean isVFR = false;
	public boolean isAnamorphic = false;
	public boolean isWrongAR = false;
	public boolean isCleanVideo = false;
	public AVStreamDataVideo() { super(); }

	/**
	 * Gets the integer with the video flags
	 * @return flags
	 */
	public synchronized int getVideoFlags() {
		int flags = 0;
		if (this.isAnamorphic) flags += 1;
		if (this.isWrongAR) flags += 2;
		if (this.isVFR) flags += 4;
		if (this.isCleanVideo) flags += 8;
		return flags;
	}
	
	/**
	 * Gets the stream AR
	 * @return Aspect Ratio
	 */
	public synchronized String getVideoAR() {
		String temp = String.format("%1.2f", this.ar);
		if (temp.equals("1,33")) return "4:3";
		else if (temp.equals("1,77")) return "16:9";
		else if (temp.equals("1,66")) return "1.66:1";
		else if (temp.equals("1,85")) return "1.85:1";
		else if (temp.equals("2,00")) return "2.00:1";
		else if (temp.equals("2,21")) return "2.21:1";
		else if (temp.equals("2,35")) return "2.35:1";
		else return "other";
	}
	
	/**
	 * Writes the Stream to a string.
	 * <p>Uses the same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("\tstream [video]:"+'\n');
		out.append("\t\tcodec: "+this.codecName+(this.codecTag != "" ? " ["+this.codecTag+"]" : "")+'\n');
		if (this.size > 0) out.append("\t\tsize: "+this.size+" bytes"+'\n');
		if (this.duration > 0) out.append("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")"+'\n');
		if (this.bitrate > 0) out.append("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps"+'\n');
		if (this.resolution != "") out.append("\t\tresolution: "+this.resolution+'\n');
		if (this.ar > 0) out.append("\t\taspect ratio: "+getVideoAR()+'\n');
		if (this.fps > 0) out.append(String.format("\t\tfps: %2.3f\n",this.fps));
		if (this.pictureFormat != "") out.append("\t\tpicture format: "+this.pictureFormat+'\n');
		if (this.isAnamorphic) out.append("\t\tanamorphic"+'\n');
		if (this.isWrongAR) out.append("\t\twrong aspect ratio"+'\n');
		if (this.isVFR) out.append("\t\tvariable frame rate"+'\n');
		if (this.isCleanVideo) out.append("\t\tclean video (no hardsubs...)"+'\n');
		return out.toString();
	}
	
	/**
	 * Writes the Stream to a file
	 * @param out PrintStream where to output the stream
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("\tstream [video]:");
		out.println("\t\tcodec: "+this.codecName+(this.codecTag != "" ? " ["+this.codecTag+"]" : ""));
		if (this.size > 0) out.println("\t\tsize: "+this.size+" bytes");
		if (this.duration > 0) out.println("\t\tduration: "+formatDurationSecs(this.duration)+" ("+this.duration+")");
		if (this.bitrate > 0) out.println("\t\tbitrate: "+(int)(this.bitrate/1000)+" kbps");
		if (this.resolution != "") out.println("\t\tresolution: "+this.resolution);
		if (this.ar > 0) out.println("\t\taspect ratio: "+getVideoAR());
		if (this.fps > 0) out.printf("\t\tfps: %2.3f\n",this.fps);
		if (this.pictureFormat != "") out.println("\t\tpicture format: "+this.pictureFormat);
		if (this.isAnamorphic) out.println("\t\tanamorphic");
		if (this.isWrongAR) out.println("\t\twrong aspect ratio");
		if (this.isVFR) out.println("\t\tvariable frame rate");
		if (this.isCleanVideo) out.println("\t\tclean video (no hardsubs...)");
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
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+(int)(this.duration*this.timebase)));
		if (this.bitrate > 0) xml.addValue(new XmlObject("br",""+(int)this.bitrate));
		xml.addValue(new XmlObject("res",this.resolution));
		xml.addValue(new XmlObject("codec",this.codecName));
		if (this.fps > 0) xml.addValue(new XmlObject("fps",String.format("%2.3f",this.fps)));
		if (this.ar > 0) xml.addValue(new XmlObject("ar",getVideoAR()));
		if (this.pictureFormat != "") xml.addValue(new XmlObject("pf",this.pictureFormat));
		int flags = getVideoFlags();
		if (flags > 0) xml.addValue(new XmlObject("flags",""+flags));
		return xml;
	}
}

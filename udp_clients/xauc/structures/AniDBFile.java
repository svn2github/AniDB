package structures;

import java.io.IOException;
import java.io.PrintStream;
import java.io.Serializable;
import java.io.File;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

import xml.*;

/**
 * AniDB File Implementation
 */
public class AniDBFile implements Serializable {
	private static final long serialVersionUID = -5117159935625276750L;
	public static final int DEFAULT = 0;
	public static final int ADDED = 1;
	public static final int HASHED = 2;
	public static final int PARSED = 4;
	public static final int ANIDBQUERIED = 8;
	public static final int IDENTIFIED = 16;
	public static final int ADDEDTOMYLIST = 32;
	public static final int UNKNOWN = 64;
	// Hashed states
	public static final int HASHED_ED2K = 0x10000;
	public static final int HASHED_CRC32 = 0x10001;
	public static final int HASHED_MD5 = 0x10002;
	public static final int HASHED_SHA1 = 0x10004;
	public static final int HASHED_TTH = 0x10008;
	// Parsed state
	public static final int PARSED_FULL = 0x20000;
	
	public String ed2k = "";
	public String crc32 = "";
	public String md5 = "";
	public String sha1 = "";
	public String tth = "";
	public String ed2klink = "";
	public String extension = "";
	public String filename = "";
	public String filetype = "";
	public int aid;
	public int eid;
	public int gid;
	public long lid;
	public int fid = -1;
	public long length = 0;
	public float duration = 0;
	public int numStreams = 0;
	public String formatedDuration = "";
	public String format = "";
	public ArrayList<AVStreamDataVideo> vidstreams = null;
	public ArrayList<AVStreamDataAudio> audstreams = null;
	public ArrayList<AVStreamDataSubtitle> substreams = null;
	public ArrayList<AVStreamData> othstreams = null;
	public AVStreamData[] streams = null;
	public int state = DEFAULT;
	
	public synchronized int getState() { return this.state; }
	
	public AniDBFile() {}
	public AniDBFile(File file) {
		this.filename = file.getName();
		this.length = file.length();
		this.extension = getExtension(file);
		this.vidstreams = new ArrayList<AVStreamDataVideo>(1);
		this.audstreams = new ArrayList<AVStreamDataAudio>(1);
		this.substreams = new ArrayList<AVStreamDataSubtitle>(1);
		this.othstreams = new ArrayList<AVStreamData>(1);
	}
	
	/**
	 * Gets the file Extension
	 * @param file
	 */
	protected synchronized String getExtension(File file) {
		int i = file.getName().lastIndexOf(".");
		if(i<0) this.extension = "";
		return file.getName().substring(i+1).toLowerCase();
	}
	
	/**
	 * Outputs the file data to a string.
	 * <p>Same format as writeToFile()</p>
	 * @see writeToFile
	 */
	public synchronized String writeToString() {
		StringBuffer out = new StringBuffer();
		out.append("file: "+filename+'\n');
		if (this.fid > 0) out.append("\tid: "+this.fid+'\n');
		if (this.filetype != "") out.append("\ttype: "+this.filetype+'\n');
		if (this.format != "") out.append("\tformat: "+this.format+'\n');
		out.append("\tsize: "+this.length+" bytes"+'\n');
		out.append("\textension: "+(this.extension != "" ? this.extension : "unk")+'\n');
		if (this.duration > 0) out.append("\tduration: "+this.formatedDuration+" ("+this.duration+")"+'\n');
		out.append("hashes:"+'\n');
		if (this.ed2k != "") {
			out.append("\ted2k: "+this.ed2k+'\n');
			out.append("\ted2k link: "+this.ed2klink+'\n');
		}
		if (this.crc32 != "") out.append("\tcrc32: "+this.crc32+'\n');
		if (this.md5 != "") out.append("\tmd5: "+this.md5+'\n');
		if (this.sha1 != "") out.append("\tsha1: "+this.sha1+'\n');
		if (this.tth != "") out.append("\ttth: "+this.tth+'\n');
		if (this.numStreams > 0) {
			out.append("streams:"+'\n');
			for (int i = 0; i < this.vidstreams.size(); i++)
				out.append(this.vidstreams.get(i).writeToString());
			for (int i = 0; i < this.audstreams.size(); i++)
				out.append(this.audstreams.get(i).writeToString());
			for (int i = 0; i < this.substreams.size(); i++)
				out.append(this.substreams.get(i).writeToString());
			for (int i = 0; i < this.othstreams.size(); i++)
				out.append(this.othstreams.get(i).writeToString());
		}
		return out.toString();
	}
	
	/**
	 * Outputs the file data to a file
	 */
	public synchronized void writeToFile(PrintStream out) {
		out.println("file: "+filename);
		if (this.fid > 0) out.println("\tid: "+this.fid);
		if (this.filetype != "") out.println("\ttype: "+this.filetype);
		if (this.format != "") out.println("\tformat: "+this.format);
		out.println("\tsize: "+this.length+" bytes");
		out.println("\textension: "+(this.extension != "" ? this.extension : "unk"));
		if (this.duration > 0) out.println("\tduration: "+this.formatedDuration+" ("+this.duration+")");
		out.println("hashes:");
		if (this.ed2k != "") {
			out.println("\ted2k: "+this.ed2k);
			out.println("\ted2k link: "+this.ed2klink);
		}
		if (this.crc32 != "") out.println("\tcrc32: "+this.crc32);
		if (this.md5 != "") out.println("\tmd5: "+this.md5);
		if (this.sha1 != "") out.println("\tsha1: "+this.sha1);
		if (this.tth != "") out.println("\ttth: "+this.tth);
		if (this.numStreams > 0) {
			out.println("streams:"+'\n');
			for (int i = 0; i < this.vidstreams.size(); i++)
				this.vidstreams.get(i).writeToFile(out);
			for (int i = 0; i < this.audstreams.size(); i++)
				this.audstreams.get(i).writeToFile(out);
			for (int i = 0; i < this.substreams.size(); i++)
				this.substreams.get(i).writeToFile(out);
			for (int i = 0; i < this.othstreams.size(); i++)
				this.othstreams.get(i).writeToFile(out);
		}
	}
	
	/**
	 * Outputs the file data to System.out
	 */
	public synchronized void writeToConsole() {
		writeToFile(System.out);
	}
	
	/**
	 * Creates a xml object of the file
	 * @return the xml object
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 */
	public synchronized XmlObject getXmlObject() throws XmlObjectException, UnsupportedEncodingException {
		XmlObject xml = new XmlObject("file");
		if (this.fid > 0) xml.addAttribute("id",""+this.fid);
		if (this.filetype != "") xml.addAttribute("type",this.filetype);
		if (this.format != "") xml.addValue(new XmlObject("format",this.format));
		xml.addValue(new XmlObject("size",""+this.length));
		xml.addValue(new XmlObject("ftype",(this.extension != "" ? this.extension : "unk")));
		if (this.duration > 0) xml.addValue(new XmlObject("len",""+Math.round(this.duration)));
		if (this.ed2k != "") xml.addValue(new XmlObject("ed2k",this.ed2k));
		if (this.crc32 != "") xml.addValue(new XmlObject("crc",this.crc32));
		if (this.md5 != "") xml.addValue(new XmlObject("md5",this.md5));
		if (this.sha1 != "") xml.addValue(new XmlObject("sha1",this.sha1));
		if (this.tth != "") xml.addValue(new XmlObject("tth",this.tth));
		if (this.numStreams > 0) {
			if (this.vidstreams.size() > 0) {
				XmlObject vid = new XmlObject("vid");
				for (int i = 0; i < this.vidstreams.size(); i++)
					vid.addValue(this.vidstreams.get(i).getXmlObject());
				vid.addAttribute("cnt", ""+this.vidstreams.size());
				xml.addValue(vid);
			}
			if (this.audstreams.size() > 0) {
				XmlObject aud = new XmlObject("aud");
				for (int i = 0; i < this.audstreams.size(); i++)
					aud.addValue(this.audstreams.get(i).getXmlObject());
				aud.addAttribute("cnt", ""+this.audstreams.size());
				xml.addValue(aud);
			}
			if (this.substreams.size() > 0) {
				XmlObject sub = new XmlObject("sub");
				for (int i = 0; i < this.substreams.size(); i++)
					sub.addValue(this.substreams.get(i).getXmlObject());
				sub.addAttribute("cnt", ""+this.substreams.size());
				xml.addValue(sub);
			}
			if (this.othstreams.size() > 0) {
				XmlObject oth = new XmlObject("oth");
				for (int i = 0; i < this.othstreams.size(); i++)
					oth.addValue(this.othstreams.get(i).getXmlObject());
				oth.addAttribute("cnt", ""+this.othstreams.size());
				xml.addValue(oth);
			}
		}
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
	
	/**
	 * Writes the objects XML document to a folder
	 * @throws XmlObjectException
	 * @throws UnsupportedEncodingException
	 * @throws IOException
	 */
	public synchronized void writeXml() throws XmlObjectException, UnsupportedEncodingException, IOException {
		String appDir = System.getProperty("user.home") + File.separator + ".xauc";
		String outputfile = appDir+File.separator+"file"+File.separator+(this.fid > 0 ? this.fid : this.ed2k)+".xml";
		File dir = new File("file");
		if (!dir.exists() || (dir.exists() && !dir.isDirectory())) dir.mkdir();
		PrintStream out = new PrintStream(outputfile);
		out.print(getXml());
	}
}

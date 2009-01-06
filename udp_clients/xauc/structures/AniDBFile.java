package structures;

import java.io.IOException;
import java.io.PrintStream;
import java.io.Serializable;
import java.io.File;
import java.io.UnsupportedEncodingException;

import hashing.Hasher;
import avparsing.*;
import xml.*;
import net.sf.ffmpeg_java.AVCodecLibrary;

/**
 * AniDB File Implementation
 */
public class AniDBFile implements Serializable {
	private static final long serialVersionUID = -5117159935625276750L;
	private static final int UNKNOWN = 0x00000000;
	private static final int HASHED = 0x00000001;
	private static final int PARSED = 0x00000002;
	private static final int IDENTIFIED = 0x00000004;
	private static final int NOTPRESENT = 0x00000008;
	private static final int LOCALONLY = 0x00000010;
	private static final int ADDEDANIDB = 0x00000020;
	
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
	public String formatedDuration = "";
	public String format = "";
	public AVStreamData[] streams = null;
	public int state = AniDBFile.UNKNOWN;
	
	public AniDBFile() {}
	public AniDBFile(File file) {
		this.filename = file.getName();
		this.length = file.length();
		this.extension = getExtension(file);
	}
	public AniDBFile(File file, Hasher filehasher) {
		this(file);
		this.crc32 = filehasher.getCrc32();
		this.ed2k = filehasher.getEd2k();
		this.ed2klink = filehasher.getEd2klink();
		this.sha1 = filehasher.getSha1();
		this.tth = filehasher.getTth();
		this.md5 = filehasher.getMd5();
		this.state = AniDBFile.HASHED;
	}
	public AniDBFile(File file, AVparser avparser) {
		this(file);
		this.duration = avparser.duration;
		this.formatedDuration = avparser.formatedDuration;
		this.format = avparser.format;
		this.streams = avparser.streams;
		this.state = AniDBFile.PARSED;
	}
	public AniDBFile(File file, Hasher filehasher, AVparser avparser) {
		this (file, filehasher);
		this.duration = avparser.duration;
		this.formatedDuration = avparser.formatedDuration;
		this.format = avparser.format;
		this.streams = avparser.streams;
		this.state = this.state|AniDBFile.PARSED;
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
		out.println("streams:");
		// vid streams
		for (int i = 0; i < this.streams.length; i++) {
			switch(this.streams[i].type) {
				case AVCodecLibrary.CODEC_TYPE_VIDEO:
					AVStreamDataVideo viddata = (AVStreamDataVideo)this.streams[i];
					viddata.writeToFile(out);
					break;
				case AVCodecLibrary.CODEC_TYPE_AUDIO:
					AVStreamDataAudio auddata = (AVStreamDataAudio)this.streams[i];
					auddata.writeToFile(out);
					break;
				case AVCodecLibrary.CODEC_TYPE_SUBTITLE:
					AVStreamDataSubtitle subdata = (AVStreamDataSubtitle)this.streams[i];
					subdata.writeToFile(out);
					break;
				default:
					this.streams[i].writeToFile(out);
			}
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
		// vid streams
		XmlObject vid = new XmlObject("vid");
		XmlObject aud = new XmlObject("aud");
		XmlObject sub = new XmlObject("sub");
		XmlObject other = new XmlObject("oth");
		int vidcnt = 0;
		int audcnt = 0;
		int subcnt = 0;
		int othercnt = 0;
		for (int i = 0; i < this.streams.length; i++) {
			switch(this.streams[i].type) {
				case AVCodecLibrary.CODEC_TYPE_VIDEO:
					AVStreamDataVideo viddata = (AVStreamDataVideo)this.streams[i];
					vid.addValue(viddata.getXmlObject());
					vidcnt++;
					break;
				case AVCodecLibrary.CODEC_TYPE_AUDIO:
					AVStreamDataAudio auddata = (AVStreamDataAudio)this.streams[i];
					aud.addValue(auddata.getXmlObject());
					audcnt++;
					break;
				case AVCodecLibrary.CODEC_TYPE_SUBTITLE:
					AVStreamDataSubtitle subdata = (AVStreamDataSubtitle)this.streams[i];
					sub.addValue(subdata.getXmlObject());
					subcnt++;
					break;
				default:
					other.addValue(this.streams[i].getXmlObject());
					othercnt++;
			}
		}
		if (vidcnt > 0) {
			vid.addAttribute("cnt", ""+vidcnt);
			xml.addValue(vid);
		}
		// aud streams
		if (audcnt > 0) {
			aud.addAttribute("cnt", ""+audcnt);
			xml.addValue(aud);
		}
		// sub streams
		if (subcnt > 0) {
			sub.addAttribute("cnt", ""+subcnt);
			xml.addValue(sub);
		}
		if (othercnt > 0) {
			other.addAttribute("cnt", ""+othercnt);
			xml.addValue(other);
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

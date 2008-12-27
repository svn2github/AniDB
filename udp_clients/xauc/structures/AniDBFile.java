package structures;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.PrintStream;
import java.io.Serializable;
import java.io.File;
import java.io.UnsupportedEncodingException;

import hashing.Hasher;
import xml.*;
/**
 * AniDB File Implementation
 */
public class AniDBFile implements Serializable {
	private static final long serialVersionUID = -5117159935625276750L;

	protected static enum AniDBFileState {
		NOT_HASHED,
		HASHED,
		IDENTIFIED,
		UNKNOWN
	}
	
	protected String ed2k = "";
	protected String crc32 = "";
	protected String md5 = "";
	protected String sha1 = "";
	protected String tth = "";
	protected String ed2klink = "";
	protected String extension = "";
	protected String filename = "";
	protected String filetype = "";
	protected int aid;
	protected int eid;
	protected int gid;
	protected long lid;
	protected int fid = -1;
	protected long length = 0;
	protected AniDBFileState state = AniDBFileState.NOT_HASHED;
	
	public AniDBFile() {}
	public AniDBFile(File file, Hasher filehasher) {
		this.filename = file.getName();
		this.length = file.length();
		this.crc32 = filehasher.getCrc32();
		this.ed2k = filehasher.getEd2k();
		this.ed2klink = filehasher.getEd2klink();
		this.sha1 = filehasher.getSha1();
		this.tth = filehasher.getTth();
		this.md5 = filehasher.getMd5();
		this.extension = filehasher.getExtension();
		this.state = AniDBFileState.HASHED;
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
		xml.addValue(new XmlObject("size",""+this.length));
		if (this.extension != "") xml.addValue(new XmlObject("ftype",this.extension));
		if (this.ed2k != "") xml.addValue(new XmlObject("ed2k",this.ed2k));
		if (this.crc32 != "") xml.addValue(new XmlObject("crc",this.crc32));
		if (this.md5 != "") xml.addValue(new XmlObject("md5",this.md5));
		if (this.sha1 != "") xml.addValue(new XmlObject("sha1",this.sha1));
		if (this.tth != "") xml.addValue(new XmlObject("tth",this.tth));
		return xml;
	}
	
	public synchronized String getXml() throws XmlObjectException, UnsupportedEncodingException {
		XmlDoc doc = new XmlDoc();
		doc.addChild(getXmlObject());
		return doc.toString();
	}
	
	public synchronized void writeXml() throws XmlObjectException, UnsupportedEncodingException, IOException {
		String outputfile = "file"+File.separator+(this.fid > 0 ? this.fid : this.ed2k)+".xml";
		File dir = new File("file");
		if (!dir.exists() || (dir.exists() && !dir.isDirectory())) dir.mkdir();
		PrintStream out = new PrintStream(outputfile);
		out.print(getXml());
	}
}

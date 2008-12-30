package hashing;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.NoSuchAlgorithmException;
import utils.Progress;
import utils.Log;

/**
 * General Hasher Class
 * @author fahrenheit
 */
public class Hasher {
	protected String crc32 = "";
	protected String ed2k = "";
	protected String ed2klink = "";
	protected String md5 = "";
	protected String sha1 = "";
	protected String tth = "";
	protected String extension = "";
	protected File file;
	protected boolean enableED2K = true;
	protected boolean enableCRC32 = false;
	protected boolean enableMD5 = false;
	protected boolean enableSHA1 = false;
	protected boolean enableTTH = false;
	protected int BUFSIZE = 1048576*3;
	protected Log log;
	protected Progress progress;
	protected boolean showDebug = false;
	
	public Hasher() {}
	public Hasher(File file) { 
		this.file = file;
		setExtension(this.file);
	}
	public Hasher(String file) { this(new File(file)); }
	public Hasher(File file, HasherOptions options) {
		this(file);
		this.enableED2K = options.isEnableED2K();
		this.enableCRC32 = options.isEnableCRC32();
		this.enableMD5 = options.isEnableMD5();
		this.enableSHA1 = options.isEnableSHA1();
		this.enableTTH = options.isEnableTTH();
	}
	public Hasher(String file, HasherOptions options) {
		this(new File(file), options);
	}
	
	/**
	 * Get's and sets the file Extension
	 * @param file
	 */
	protected synchronized void setExtension(File file) {
		int i = file.getName().lastIndexOf(".");
		if(i<0) this.extension = "";
		this.extension = file.getName().substring(i+1).toLowerCase();
	}
	
	/** @return Returns the file extension */
	public synchronized String getExtension() { return this.extension; }
	
	/**
	 * Method that runs over a file and gets its hashes
	 * @return 0 if operation is sucessful
	 * @throws NoSuchAlgorithmException
	 * @throws IOException
	 */
	public synchronized int startWork() throws NoSuchAlgorithmException, IOException {
		InputStream in = new FileInputStream(file);
		int last_read;
		long total_read = 0, len = file.length();
		float curprogress = 0;
		byte[] buffer = new byte[BUFSIZE];
		AbstractChecksum ed2k = new Edonkey();
		AbstractChecksum crc32 = new Crc32();
		AbstractChecksum md5 = new MD5();
		AbstractChecksum tth = new TTH();
		AbstractChecksum sha1 = new MD("sha1");
		if (showDebug) System.out.println("Started hashing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		long start = System.currentTimeMillis();
		while ((last_read = in.read(buffer)) != -1){
			if (enableED2K) ed2k.update(buffer, 0, last_read);
			if (enableCRC32) crc32.update(buffer, 0, last_read);
			if (enableMD5) md5.update(buffer, 0, last_read);
			if (enableTTH) tth.update(buffer, 0, last_read);
			if (enableSHA1) sha1.update(buffer, 0, last_read);
			total_read += last_read;
			curprogress = (float)total_read/len;
			if (progress != null) progress.setProgress(curprogress);
		}
		log.println("\nCompleted hashing of \""+file.getName()+"\" in "+(System.currentTimeMillis() - start)+"ms");
		if (enableED2K) {
			this.ed2k = ed2k.getHexValue();
			this.ed2klink = "ed2k://|file|"+file.getName()+"|"+file.length()+"|"+this.ed2k+"|";
			log.println("ed2k: "+this.ed2k);
			log.println("ed2k link: "+this.ed2klink);
		}
		if (enableCRC32) {
			this.crc32 = crc32.getHexValue();
			log.println("crc32: "+this.crc32);
		}
		if (enableMD5) {
			this.md5 = md5.getHexValue();
			log.println("md5: "+this.md5);
		}
		if (enableSHA1) {
			this.sha1 = sha1.getHexValue();
			log.println("sha1: "+this.sha1);
		}
		if (enableTTH) {
			this.tth = tth.getHexValue();
			log.println("tth: "+this.tth);
		}
		return 0;
	}
	/** @return the log */
	public synchronized Log getLog() { return log; }
	/** @param log the log to set */ 
	public synchronized void setLog(Log log) { this.log = log; }
	/** @return the progress */ 
	public synchronized Progress getProgress() { return progress; }
	/** @param progress the progress to set */
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	/** @return the file */
	public synchronized File getFile() { return file; }
	/** @param file the file to set */
	public synchronized void setFile(File file) { this.file = file; setExtension(this.file); }
	/** @return the BUFSIZE */
	public synchronized int getBUFSIZE() { return BUFSIZE; }
	/** @param bufsize the bUFSIZE to set */
	public synchronized void setBUFSIZE(int bufsize) { BUFSIZE = bufsize; }
	/** @return the showDebug */
	public synchronized boolean isShowDebug() { return showDebug; }
	/** @param showDebug the showDebug to set */
	public synchronized void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }
	/** @return the crc32 */
	public synchronized String getCrc32() { return crc32; }
	/** @return the ed2k */
	public synchronized String getEd2k() { return ed2k; }
	/** @return the ed2klink */
	public synchronized String getEd2klink() { return ed2klink; }
	/** @return the md5 */
	public synchronized String getMd5() { return md5; }
	/** @return the sha1 */
	public synchronized String getSha1() { return sha1; }
	/** @return the tth */
	public synchronized String getTth() { return tth; }
}

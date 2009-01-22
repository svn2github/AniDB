package hashing;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.NoSuchAlgorithmException;

import structures.AniDBFile;
import utils.Progress;
import utils.Log;
import utils.ThreadedWorker;
import utils.Utils;

/**
 * The Hasher Class does the work of coordinating file hashing.<br>
 * <p><b>Workflow:</b><br>
 * <ol><li>Instantiate class using Hasher hasher = new Hasher(File file); or any of the other constructors</li>
 * <li>Optionally use setters to set class parameters</li>
 * <li>If you wish to directly execute the hashing use work() method</li>
 * <li>If you wish to run this in a new thread, first enable the thread using the setEnabled(true) method and the use the run() method</li></ol>
 * <p><b>Error handling:</b><br>To know if there was some non-critical error, as in no native crash, just use the getErrorMessage() method, if it's a null string, there was no error.
 * @author fahrenheit
 */
public class Hasher extends ThreadedWorker {
	/** CRC32 hash if enabled */
	protected String crc32 = "";
	/** ED2K hash if enabled */
	protected String ed2k = "";
	/** ED2K link of the file is enabled */
	protected String ed2klink = "";
	/** MD5 hash if enabled */
	protected String md5 = "";
	/** SHA1 hash if enabled */
	protected String sha1 = "";
	/** TTH hash if enabled */
	protected String tth = "";
	/** File reference */
	protected File file;
	/** AniDBFile for this file */
	protected AniDBFile anidbFile = null;
	/** If set to true, the file will be processed to get it's ED2K hash */
	protected boolean enableED2K = true;
	/** If set to true, the file will be processed to get it's CRC32 hash */
	protected boolean enableCRC32 = false;
	/** If set to true, the file will be processed to get it's MD5 hash */
	protected boolean enableMD5 = false;
	/** If set to true, the file will be processed to get it's SHA1 hash */
	protected boolean enableSHA1 = false;
	/** If set to true, the file will be processed to get it's TTH hash */
	protected boolean enableTTH = false;
	/** Buffer size */
	protected int BUFSIZE = 1048576*3;
	/** Log where messages will be written to */
	protected Log log;
	/** The current hashing progress will be shown here */
	protected Progress progress;
	/** If set to true debug messages will be shown */
	protected boolean showDebug = false;
	/** If some error occours during processing its error message will be stored here */
	protected String errorMessage = "";
	
	/** Hasher Class constructor */
	protected Hasher() {}
	/**
	 * Hasher Class constructor
	 * @param file File to hash
	 */
	public Hasher(File file) { this.file = file; }
	/**
	 * Hasher Class constructor
	 * @param filename Filename of the file to hash
	 */
	public Hasher(String filename) { this(new File(filename)); }
	/**
	 * Hasher Class constructor
	 * @param file File to hash
	 * @param options Hashing Options
	 */
	public Hasher(File file, HasherOptions options) {
		this(file);
		this.enableED2K = options.isEnableED2K();
		this.enableCRC32 = options.isEnableCRC32();
		this.enableMD5 = options.isEnableMD5();
		this.enableSHA1 = options.isEnableSHA1();
		this.enableTTH = options.isEnableTTH();
		this.showDebug = options.isSeeDebug();
	}
	/**
	 * Hasher Class constructor
	 * @param filename Filename of the file to hash
	 * @param options Hashing Options
	 */
	public Hasher(String file, HasherOptions options) {
		this(new File(file), options);
	}
	
	/**
	 * Method that runs over a file and gets its hashes
	 */
	public synchronized void work() {
		InputStream in;
		try {
			in = new FileInputStream(file);
		} catch (FileNotFoundException e) {
			this.errorMessage = e.getLocalizedMessage() + " (hasher.run)";
			return;
		}
		int last_read;
		long total_read = 0, len = file.length();
		float curprogress = 0;
		byte[] buffer = new byte[BUFSIZE];
		AbstractChecksum ed2k = null;
		AbstractChecksum crc32 = null;
		AbstractChecksum md5 = null;
		AbstractChecksum tth = null;
		AbstractChecksum sha1 = null;
		try {
			ed2k = new Edonkey();
			crc32 = new Crc32();
			md5 = new MD5();
			tth = new TTH();
			sha1 = new MD("sha1");
		} catch (NoSuchAlgorithmException e) {
			this.errorMessage = e.getLocalizedMessage() + " (hasher.run)";
			return;
		}
		if (showDebug) System.out.println("Started hashing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		long start = System.currentTimeMillis();
		try {
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
		} catch (IOException e) {
			this.errorMessage = e.getLocalizedMessage() + " (hasher.run)";
			return;
		}
		long time = (System.currentTimeMillis() - start);
		log.println("completed hashing of \""+file.getName()+"\" in "+time+"ms @"+Utils.calculateProcessingRate(file.length(),time));
		if (this.anidbFile != null) this.anidbFile.state |= AniDBFile.HASHED;
		if (enableED2K) {
			this.ed2k = ed2k.getHexValue();
			this.ed2klink = "ed2k://|file|"+file.getName()+"|"+file.length()+"|"+this.ed2k+"|";
			if (this.anidbFile != null) {
				this.anidbFile.ed2k = this.ed2k;
				this.anidbFile.ed2klink = this.ed2klink;
				this.anidbFile.state |= AniDBFile.HASHED_ED2K;
			}
		}
		if (enableCRC32) {
			this.crc32 = crc32.getHexValue();
			if (this.anidbFile != null) {
				this.anidbFile.crc32 = this.crc32;
				this.anidbFile.state |= AniDBFile.HASHED_CRC32;
			}
		}
		if (enableMD5) {
			this.md5 = md5.getHexValue();
			if (this.anidbFile != null) {
				this.anidbFile.md5 = this.md5;
				this.anidbFile.state |= AniDBFile.HASHED_MD5;
			}
		}
		if (enableSHA1) {
			this.sha1 = sha1.getHexValue();
			if (this.anidbFile != null) {
				this.anidbFile.sha1 = this.sha1;
				this.anidbFile.state |= AniDBFile.HASHED_SHA1;
			}
		}
		if (enableTTH) {
			this.tth = tth.getHexValue();
			if (this.anidbFile != null) {
				this.anidbFile.tth = this.tth;
				this.anidbFile.state |= AniDBFile.HASHED_TTH;
			}
		}
		return;
	}
	
	/** Gets the Log associated with this parser instance
	 * @return the log */
	public synchronized Log getLog() { return log; }
	/** Sets the Log associated with this parser instance 
	 * @param log the Log to set */
	public synchronized void setLog(Log log) { this.log = log; }
	/** Gets the Progress instance associated with this parser instance
	 * @return the progress */
	public synchronized Progress getProgress() { return progress; }
	/** Sets the Progress instance associated with this parser instance 
	 * @param progress the Progress to set */
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	/** Gets the file associated with this parser instance
	 * @return the file */
	public synchronized File getFile() { return file; }
	/** Sets the file associated with this parser instance
	 * @param file the file to set */
	public synchronized void setFile(File file) { this.file = file; }
	/** Gets the AniDBFile associated with this parser instance
	 * @return the anidbFile */
	public synchronized AniDBFile getAnidbFile() { return anidbFile; }
	/** Sets the AniDBFile associated with this parser instance
	 * @param anidbFile the AniDBFile to set */
	public synchronized void setAnidbFile(AniDBFile anidbFile) { this.anidbFile = anidbFile; }
	/** Gets the current buffer size 
	 * @return the buffer size */
	public synchronized int getBUFSIZE() { return BUFSIZE; }
	/** Sets the buffer size for hashing 
	 * @param bufsize the buffer size to set */
	public synchronized void setBUFSIZE(int bufsize) { BUFSIZE = bufsize; }
	/** Checks if debug messages should be displayed or not
	 * @return true if debug messages should be displayed, false otherwise */
	public synchronized boolean isShowDebug() { return showDebug; }
	/** Defines if debug messages should be displayed or not 
	 * @param showDebug debug messages will be displayed if set to true, otherwise will be hidden */
	public synchronized void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }
	/** Gets the file's CRC32 hash (or empty string if it was not enabled) 
	 * @return the crc32 */
	public synchronized String getCrc32() { return crc32; }
	/** Gets the file's ED2K hash (or empty string if it was not enabled)
	 * @return the ed2k */
	public synchronized String getEd2k() { return ed2k; }
	/** Gets the file's ED2K link (or empty string if it was not enabled)
	 * @return the ed2klink */
	public synchronized String getEd2klink() { return ed2klink; }
	/** Gets the file's MD5 hash (or empty string if it was not enabled)
	 * @return the md5 */
	public synchronized String getMd5() { return md5; }
	/** Gets the file's SHA1 hash (or empty string if it was not enabled)
	 * @return the sha1 */
	public synchronized String getSha1() { return sha1; }
	/** Gets the file's TTH hash (or empty string if it was not enabled)
	 * @return the tth */
	public synchronized String getTth() { return tth; }
	/** Gets any error messages encountered during processing 
	 * @return the errorMessage */
	public synchronized String getErrorMessage() { return errorMessage; }
}

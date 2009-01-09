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

/**
 * General Hasher Class
 * @author fahrenheit
 */
public class Hasher extends ThreadedWorker {
	protected String crc32 = "";
	protected String ed2k = "";
	protected String ed2klink = "";
	protected String md5 = "";
	protected String sha1 = "";
	protected String tth = "";
	protected File file;
	protected AniDBFile anidbFile = null;
	protected boolean enableED2K = true;
	protected boolean enableCRC32 = false;
	protected boolean enableMD5 = false;
	protected boolean enableSHA1 = false;
	protected boolean enableTTH = false;
	protected int BUFSIZE = 1048576*3;
	protected Log log;
	protected Progress progress;
	protected boolean showDebug = false;
	protected String errorMessage = "";
	
	public Hasher() {}
	public Hasher(File file) { this.file = file; }
	public Hasher(String file) { this(new File(file)); }
	public Hasher(File file, HasherOptions options) {
		this(file);
		this.enableED2K = options.isEnableED2K();
		this.enableCRC32 = options.isEnableCRC32();
		this.enableMD5 = options.isEnableMD5();
		this.enableSHA1 = options.isEnableSHA1();
		this.enableTTH = options.isEnableTTH();
		this.showDebug = options.isSeeDebug();
	}
	public Hasher(String file, HasherOptions options) {
		this(new File(file), options);
	}

	/**
	 * Method that calculates the hashing rate in MB/s
	 * @param filesize File size (in bytes)
	 * @param time Hashing time (in ms)
	 * @return String with hashing rate
	 */
	protected String calculateHashRate(long filesize, long time) {
		double sizeInMBs = filesize / 1000000;
		double timeInSecs = time / 1000;
		double rate = sizeInMBs / timeInSecs;
		return rate+" MB/s";
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
		log.println("completed hashing of \""+file.getName()+"\" in "+time+"ms @"+calculateHashRate(file.length(),time));
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
	public synchronized void setFile(File file) { this.file = file; }
	/** @return the anidbFile */
	public synchronized AniDBFile getAnidbFile() { return anidbFile; }
	/** @param anidbFile the anidbFile to set */
	public synchronized void setAnidbFile(AniDBFile anidbFile) { this.anidbFile = anidbFile; }
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
	/** @return the errorMessage */
	public synchronized String getErrorMessage() { return errorMessage; }
}

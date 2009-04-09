package hashing;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.security.NoSuchAlgorithmException;

import options.HasherOptions;
import utils.Logger;
import utils.Progress;
import utils.Utils;

/**
 * The Hasher Class does the work of coordinating file hashing.<br>
 * <p><b>Workflow:</b><br>
 * <ol><li>Instantiate class using Hasher hasher = new Hasher(File file); or any of the other constructors</li>
 * <li>Optionally use setters to set class parameters</li>
 * <li>If you wish to directly execute the hashing use work() method</li>
 * <li>If you wish to run this in a new thread, first enable the thread using the setEnabled(true) method and then use the run() method</li></ol>
 * <p><b>Error handling:</b><br>To know if there was some non-critical error, as in no native crash, just use the getErrorMessage() method, if it's a null string, there was no error.
 * @author fahrenheit
 */
public class Hasher {
	// Hasher Locals
	/** Last Error Message */
	protected String errorMessage = "";
	/** Hasher options */
	protected HasherOptions options = null;
	/** File to hash */
	protected File file;
	/** Completion time */
	protected long cputime;
	/** Associated Logger object */
	protected Logger log = null;
	/** Associated Progress object */
	protected Progress progress = null;
	
	// Results
	/** ED2K in HEX notation */
	public String ed2k = "";
	/** CRC32 in HEX notation */
	public String crc32 = "";
	/** MD5 in HEX notation */
	public String md5 = "";
	/** SHA1 in HEX notation */
	public String sha1 = "";
	/** TTH in HEX notation */
	public String tth = "";

	/** Hasher Class constructor */
	protected Hasher() {}
	/**
	 * Hasher Class constructor
	 * @param file File to hash
	 */
	public Hasher(File file) { 
		this.file = file;
		this.options = new HasherOptions();
	}
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
		this.file = file;
		this.options = options;
	}
	/**
	 * Hasher Class constructor
	 * @param filename Filename of the file to hash
	 * @param options Hashing Options
	 */
	public Hasher(String filename, HasherOptions options) { this(new File(filename),options); }
	
	// GETTERS AND SETTERS
	/**
	 * Last Error message
	 * @return the errorMessage
	 */
	public synchronized String getErrorMessage() { return errorMessage; }
	/**
	 * Returns the time it took to process a file
	 * @return the cputime
	 */
	public synchronized long getCputime() { return cputime; }
	/**
	 * Sets the Logger object to use
	 * @param log The Logger Object to use
	 */
	public synchronized void setLog(Logger log) { this.log = log; }
	/**
	 * Sets the Progress object to use
	 * @param progress The Progress object to use
	 */
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	/**
	 * Sets the Hasher Options object to use
	 * @param options The Hasher Options object to use
	 */
	public synchronized void setHasherOptions(HasherOptions options) { this.options = options; }

	// MAIN WORK METHODS
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
		byte[] buffer = new byte[options.bufferSize];
		AbstractChecksum ed2k = null;
		AbstractChecksum crc32 = null;
		AbstractChecksum md5 = null;
		AbstractChecksum tth = null;
		AbstractChecksum sha1 = null;
		try {
			if (options.ed2k) ed2k = new Edonkey();
			if (options.crc32) crc32 = new Crc32();
			if (options.md5) md5 = new MD("md5");
			if (options.sha1) sha1 = new MD("sha1");
			if (options.tth) tth = new TTH();
		} catch (NoSuchAlgorithmException e) {
			this.errorMessage = e.getLocalizedMessage() + " (hasher.run)";
			return;
		}
		if (options.seeDebug) log.println("Started hashing of \""+file.getName()+"\" ["+file.length()+" bytes]",Logger.DATE|Logger.STDOUT);
		long start = System.currentTimeMillis();
		try {
			while ((last_read = in.read(buffer)) != -1){
				if (options.ed2k) ed2k.update(buffer, 0, last_read);
				if (options.crc32) crc32.update(buffer, 0, last_read);
				if (options.md5) md5.update(buffer, 0, last_read);
				if (options.tth) tth.update(buffer, 0, last_read);
				if (options.sha1) sha1.update(buffer, 0, last_read);
				total_read += last_read;
				curprogress = (float)total_read/len;
				if (progress != null) progress.setProgress(curprogress);
			}
		} catch (IOException e) {
			this.errorMessage = e.getLocalizedMessage() + " (hasher.run)";
			return;
		}
		cputime = (System.currentTimeMillis() - start);
		if (options.seeDebug) log.println("completed hashing of \""+file.getName()+"\" in "+cputime+"ms @"+Utils.calculateProcessingRate(file.length(),cputime),Logger.DATE|Logger.STDOUT);
		if (options.ed2k) { this.ed2k = ed2k.getFormattedValue(); }
		if (options.crc32) { this.crc32 = crc32.getFormattedValue(); }
		if (options.md5) { this.md5 = md5.getFormattedValue(); }
		if (options.sha1) { this.sha1 = sha1.getFormattedValue(); }
		if (options.tth) { this.tth = tth.getFormattedValue(); }
		return;
	}
}

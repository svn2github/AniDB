package avparsing;

import java.io.File;
import java.io.IOException;

import structures.AniDBFile;
import utils.Log;
import utils.Progress;
import utils.ThreadedWorker;
import utils.Utils;

/**
 * Null AVParser implementation, basicaly this is used if parsing support is not required
 * <p><b>Workflow:</b><br>
 * <ol><li>Instantiate class using AVParser parser = new AVParserNone(File file); or any of the other constructors</li>
 * <li>Optionally use setters to set class parameters</li>
 * <li>If you wish to directly execute the parsing use work() method</li>
 * <li>If you wish to run this in a new thread, first enable the thread using the setEnabled(true) method and the use the run() method</li></ol>
 * <p><b>Error handling:</b><br>To know if there was some non-critical error, as in no native crash, just use the getErrorMessage() method, if it's a null string, there was no error.
 * <p><b>Notes:</b><br>Does nothing but updating the progress bar to 100%.</p>
 * @author fahrenheit
 */
public class AVParserNone extends ThreadedWorker implements AVParser {
	// Protected class fields
	/** File reference */
	protected File file = null;
	/** AniDBFile for this file */
	protected AniDBFile anidbFile = null;
	/** Log where messages will be written to */
	protected Log log = null;
	/** The current parsing progress will be shown here */
	protected Progress progress = null;
	/** If set to true debug messages will be shown */
	protected boolean showDebug = false;
	/** If set to true a full file parsing will be done */
	protected boolean parseFile = false;
	/** If some error occours during processing its error message will be stored here */
	protected String errorMessage = "";
	/** VBR Calculation mode (will only be used if parseFile is enabled) */
	protected int vbr_calc_mode = VBR_BY_PACKET_BITRATE;
	// Public fields
	/** Parsed filename */
	public String filename = "";
	/** Formated duration of the file in hh:mm:ss format */
	public String formatedDuration = "";
	/** Duration of the file in seconds */
	public float duration = 0;
	/** Bitrate of the file in bps if known or 0 otherwise */
	public float bitrate = 0;
	/** Format of the file (usualy container format) */
	public String format = "";
	/** Number of streams of the file */
	public int numStreams = 0;

	public synchronized File getFile() { return file; }
	public synchronized void setFile(File file) { this.file = file; }
	public synchronized AniDBFile getAnidbFile() { return anidbFile; }
	public synchronized void setAnidbFile(AniDBFile anidbFile) { this.anidbFile = anidbFile; }
	public synchronized Log getLog() { return log; }
	public synchronized void setLog(Log log) { this.log = log; }
	public synchronized Progress getProgress() { return progress; }
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	public synchronized boolean isShowDebug() { return showDebug; }
	public synchronized void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }
	public synchronized boolean isParseFile() { return parseFile; }
	public synchronized void setParseFile(boolean parseFile) { this.parseFile = parseFile; }
	public synchronized String getErrorMessage() { return errorMessage;	}
	public synchronized int getVbr_calc_mode() { return vbr_calc_mode; }
	public synchronized void setVbr_calc_mode(int vbr_calc_mode) { this.vbr_calc_mode = vbr_calc_mode; }
	
	/**
	 * AVParserNone Class constructor
	 */
	protected AVParserNone() {
		this.log = new Log();
		this.progress = new Progress();
	}
	/**
	 * AVParserNone Class constructor 
	 * @param file File to parse
	 */
	public AVParserNone(File file) {
		this();
		try {
			this.filename = file.getCanonicalPath();
		} catch (IOException e) {
			String message = "error while getting canonical path for file \""+file.getAbsolutePath()+"\", reverting to absolute path (avparser)";
			System.err.println(message);
			log.println(message);
			this.filename = file.getAbsolutePath();
		}
		this.file = file;
	}
	/**
	 * AVParserNone Class constructor 
	 * @param filename Filename of the file to parse
	 */
	public AVParserNone(String filename) { this(new File(filename)); }
	/**
	 * AVParserNone Class constructor 
	 * @param file File to parse
	 * @param parsingOptions Parsing Options
	 */
	public AVParserNone(File file, AVParserOptions parsingOptions) {
		this(file);
		this.parseFile = parsingOptions.isFullParse();
		this.showDebug = parsingOptions.isSeeDebug();
		this.vbr_calc_mode = parsingOptions.getVbr_calc_mode();
	}
	/**
	 * AVParserNone Class constructor 
	 * @param filename Filename of the file to parse
	 * @param parsingOptions Parsing Options
	 */
	public AVParserNone(String filename, AVParserOptions parsingOptions) {
		this(new File(filename),parsingOptions);
	}

	/** Method that actualy doesn't parse a file */
	public void work() {
		// that doesn't prevent me from doing something :P
		
		if (showDebug) System.out.println("Started NOT parsing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		long start = System.currentTimeMillis();
		
		this.progress.setProgress(1); // finish it up
		
		long time = (System.currentTimeMillis() - start);
		log.println("Completed NOT parsing of \""+file.getName()+"\" in "+time+"ms @"+Utils.calculateProcessingRate(file.length(),time));

		
	}

}

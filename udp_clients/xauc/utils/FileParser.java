package utils;

import java.io.File;

import structures.AniDBFile;
import hashing.Hasher;
import hashing.HasherOptions;
import avparsing.AVParser;
import avparsing.AVParserOptions;

/**
 * A file parser
 */
public class FileParser extends ThreadedWorker {
	protected String threadName = "fileParser";
	protected File file = null;
	protected HasherOptions hasherOptions = null;
	protected AVParserOptions avparserOptions = null;
	protected Log log = null;
	protected Progress progress = null;
	protected AniDBFile anidbFile = null;
	protected String errorMessage = "";
	
	public FileParser() {
		this.log = new Log();
		this.progress = new Progress();
		this.avparserOptions = new AVParserOptions();
		this.hasherOptions = new HasherOptions();
	}
	public FileParser(File file) { this(); this.file = file; }
	public FileParser(String filename) { this(new File(filename)); }
	public FileParser(File file, HasherOptions hasherOptions) { 
		this(file);
		this.hasherOptions = hasherOptions;
	}
	public FileParser(File file, AVParserOptions avparserOptions) { 
		this(file);
		this.avparserOptions = avparserOptions;
	}
	public FileParser(File file, HasherOptions hasherOptions, AVParserOptions avparserOptions) {
		this(file, hasherOptions);
		this.avparserOptions = avparserOptions;
	}
	public FileParser(String filename, HasherOptions hasherOptions) {
		this (new File(filename), hasherOptions);
	}
	public FileParser(String filename, AVParserOptions avparserOptions) {
		this (new File(filename), avparserOptions);
	}
	public FileParser(String filename, HasherOptions hasherOptions, AVParserOptions avparserOptions) {
		this (new File(filename), hasherOptions, avparserOptions);
	}
	/** @return the file */
	public synchronized File getFile() { return file; }
	/** @param file the file to set */
	public synchronized void setFile(File file) { this.file = file; }
	/** @return the hasherOptions */
	public synchronized HasherOptions getHasherOptions() { return hasherOptions; }
	/** @param hasherOptions the hasherOptions to set */
	public synchronized void setHasherOptions(HasherOptions hasherOptions) { this.hasherOptions = hasherOptions; }
	/** @return the avparserOptions */ 
	public synchronized AVParserOptions getAvparserOptions() { return avparserOptions; }
	/** @param avparserOptions the avparserOptions to set */
	public synchronized void setAvparserOptions(AVParserOptions avparserOptions) { this.avparserOptions = avparserOptions; }
	/** @return the log */
	public synchronized Log getLog() { return log; }
	/** @param log the log to set */
	public synchronized void setLog(Log log) { this.log = log; }
	/** @return the progress */
	public synchronized Progress getProgress() { return progress; }
	/** @param progress the progress to set */
	public synchronized void setProgress(Progress progress) { this.progress = progress; }
	/** @return the anidbFile */
	public synchronized AniDBFile getAnidbFile() { return anidbFile; }
	/** @param anidbFile the anidbFile to set */
	public synchronized void setAnidbFile(AniDBFile anidbFile) { this.anidbFile = anidbFile; }
	/** @return the errorMessage */
	public synchronized String getErrorMessage() { return errorMessage;	}

	/**
	 * Executes the file parser.<br />
	 * If process is sucessful getAniDBFile() will <b>not</b> return null
	 */
	public synchronized void work() {
		if (file == null) {
			this.errorMessage = "no file supplied to file parser, skipping... (fileparser.run)";
			return;
		}
		
		this.anidbFile = new AniDBFile(file);
		Hasher filehasher = null;
		AVParser avparser = null;
		
		if (this.hasherOptions.isEnabled()) {
			filehasher = new Hasher(file,hasherOptions);
			filehasher.setLog(log);
			filehasher.setAnidbFile(this.anidbFile);
			progress.setAction("hashing");
			//progress.setShowProgress(false);
			filehasher.setProgress(progress);
			filehasher.work();
			if (!filehasher.getErrorMessage().equals("")) this.errorMessage = filehasher.getErrorMessage();
		}
		
		if (this.avparserOptions.isEnabled()) {
			avparser = new AVParser(file,avparserOptions);
			avparser.setLog(log);
			avparser.setAnidbFile(this.anidbFile);
			progress.setAction("parsing");
			//progress.setShowProgress(false);
			avparser.setProgress(progress);
			avparser.work();
			if (!avparser.getErrorMessage().equals("")) this.errorMessage = avparser.getErrorMessage();
		}
	}
}

package utils;

import java.io.File;

import clients.XaucOptions;
import clients.XaucShared;

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
	protected XaucShared sharedParent = null;
	protected HasherOptions hasherOptions = new HasherOptions();
	protected AVParserOptions avparserOptions = new AVParserOptions();
	protected XaucOptions clientOptions = new XaucOptions();
	protected Log log = null;
	protected Progress progress = null;
	protected AniDBFile anidbFile = null;
	protected String errorMessage = "";
	
	public FileParser() {
		this.log = new Log();
		this.progress = new Progress();
	}
	public FileParser(XaucShared sharedParent, File file) { 
		this(); 
		this.sharedParent = sharedParent;
		hasherOptions = sharedParent.getHashingOptions();
		avparserOptions = sharedParent.getParsingOptions();
		clientOptions = sharedParent.getClientOptions();
		this.file = file;
	}
	public FileParser(XaucShared sharedParent, String filename) { this(sharedParent,new File(filename)); }
	public FileParser(File file, HasherOptions hasherOptions, AVParserOptions avparserOptions, XaucOptions clientOptions) {
		this();
		this.file = file;
		this.hasherOptions = hasherOptions;
		this.avparserOptions = avparserOptions;
		this.clientOptions = clientOptions;
	}

	/** @return the file */
	public synchronized File getFile() { return file; }
	/** @param file the file to set */
	public synchronized void setFile(File file) { this.file = file; }
	/** @param sharedParent the sharedParent to set */
	public synchronized void setSharedParent(XaucShared sharedParent) { this.sharedParent = sharedParent; }
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
			filehasher.setProgress(progress);
			filehasher.work();
			if (!filehasher.getErrorMessage().equals("")) this.errorMessage = filehasher.getErrorMessage();
		}
		
		if (this.avparserOptions.isEnabled()) {
			avparser = new AVParser(file,avparserOptions);
			avparser.setLog(log);
			avparser.setAnidbFile(this.anidbFile);
			progress.setAction("parsing");
			avparser.setProgress(progress);
			avparser.work();
			if (!avparser.getErrorMessage().equals("")) this.errorMessage = avparser.getErrorMessage();
		}
	}
}

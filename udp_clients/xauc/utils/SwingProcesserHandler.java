package utils;

import java.io.File;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import javax.swing.JTable;

import clients.XaucConsole;

import structures.AniDBFile;
import structures.SwingFile;
import hashing.HasherOptions;
import avparsing.AVParserOptions;

public class SwingProcesserHandler implements Runnable {
	protected HashMap<File,SwingFile> files = null;
	protected HasherOptions hashingOptions = null;
	protected AVParserOptions parsingOptions = null;
	protected SwingProgress mainProgressBar = null;
	protected SwingProgress fileProgressBar = null;
	protected JTable filesTable = null;
	protected Log log = null;
	
	public SwingProcesserHandler() {
		this.files = new HashMap<File,SwingFile>();
		this.hashingOptions = new HasherOptions();
		this.parsingOptions = new AVParserOptions();
	}
	public SwingProcesserHandler(HashMap<File,SwingFile> files,HasherOptions hashingOptions, AVParserOptions parsingOptions) {
		this.files = files;
		this.hashingOptions = hashingOptions;
		this.parsingOptions = parsingOptions;
	}
	/** @return the files */
	public synchronized HashMap<File, SwingFile> getFiles() { return files; }
	/** @param files the files to set */
	public synchronized void setFiles(HashMap<File, SwingFile> files) { this.files = files; }
	/** @return the hashingOptions */
	public synchronized HasherOptions getHashingOptions() { return hashingOptions; }
	/** @param hashingOptions the hashingOptions to set */
	public synchronized void setHashingOptions(HasherOptions hashingOptions) { this.hashingOptions = hashingOptions; }
	/** @return the parsingOptions */
	public synchronized AVParserOptions getParsingOptions() { return parsingOptions; }
	/** @param parsingOptions the parsingOptions to set */
	public synchronized void setParsingOptions(AVParserOptions parsingOptions) { this.parsingOptions = parsingOptions; }
	/** @return the mainProgressBar */
	public synchronized SwingProgress getMainProgressBar() { return mainProgressBar; }
	/** @param mainProgressBar the mainProgressBar to set */
	public synchronized void setMainProgressBar(SwingProgress mainProgressBar) { this.mainProgressBar = mainProgressBar; }
	/** @return the fileProgressBar */
	public synchronized SwingProgress getFileProgressBar() { return fileProgressBar; }
	/** @param fileProgressBar the fileProgressBar to set */
	public synchronized void setFileProgressBar(SwingProgress fileProgressBar) { this.fileProgressBar = fileProgressBar; }
	/** @return the log */
	public synchronized Log getLog() { return log; }
	/** @param log the log to set */
	public synchronized void setLog(Log log) { this.log = log; }
	/** @return the filesTable */
	public synchronized JTable getFilesTable() { return filesTable; }
	/** @param filesTable the filesTable to set */
	public synchronized void setFilesTable(JTable filesTable) { this.filesTable = filesTable; }
	
	public synchronized void run() {
		Set<File> filesToCheck = this.files.keySet();
		Iterator<File> it = filesToCheck.iterator();
		if (this.mainProgressBar == null) this.mainProgressBar = new SwingProgress();
		this.mainProgressBar.setAction("processing");
		this.mainProgressBar.setShowProgress(true);
		if (this.fileProgressBar == null) this.fileProgressBar = new SwingProgress();
		FileParser parser;
		int processedFiles = 0;
		float curprogress = 0;
		this.mainProgressBar.setProgress(curprogress);
		while(it.hasNext()) {
			File file = it.next();
			SwingFile sf = this.files.get(file);
			if (sf.isHASHED()) { processedFiles++; continue; }
			if (this.filesTable != null) this.filesTable.setValueAt("processing", sf.getRowId(), 2);
			parser = new FileParser(file,this.hashingOptions,this.parsingOptions);
			parser.setLog(this.log);
			this.fileProgressBar.setProgress(0);
			parser.setProgress(this.fileProgressBar);
			parser.run();
			processedFiles++;
			curprogress = ((float)processedFiles / (float)filesToCheck.size());
			this.mainProgressBar.setProgress(curprogress);
			if (!parser.getErrorMessage().equals(""))
				this.log.println(parser.getErrorMessage());
			AniDBFile afile = parser.getAnidbFile();
			if (afile == null) {
				XaucConsole.printOptionError("some error occured while processing \"" + file.getName() + "\"");
				continue;
			}
			sf.setAniDBFile(afile);
			if (this.filesTable != null) this.filesTable.setValueAt(afile.ed2k, sf.getRowId(), 1);
			if (this.hashingOptions.isEnabled()) {
				sf.setHASHED();
				if (this.filesTable != null) this.filesTable.setValueAt("hashed", sf.getRowId(), 2);
			}
			if (this.parsingOptions.isEnabled()) {
				sf.setPARSED();
				if (this.filesTable != null) this.filesTable.setValueAt("parsed", sf.getRowId(), 2);
			}
		}
	}
}

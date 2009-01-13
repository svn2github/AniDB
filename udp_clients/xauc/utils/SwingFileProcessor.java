package utils;

import hashing.HasherOptions;

import java.io.File;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;

import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTree;
import javax.swing.text.Position;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;

import avparsing.AVParserOptions;
import clients.XaucConsole;
import clients.XaucOptions;
import clients.XaucShared;
import clients.XaucSwing;
import structures.AniDBFile;
import structures.SwingFile;

public class SwingFileProcessor extends ThreadedWorker {
	protected String threadName = "file processor";
	protected HashMap<File,SwingFile> files = null;
	protected XaucShared sharedParent = null;
	protected HasherOptions hashingOptions = new HasherOptions();
	protected AVParserOptions parsingOptions = new AVParserOptions();
	protected XaucOptions clientOptions = new XaucOptions();
	protected SwingProgress mainProgressBar = null;
	protected SwingProgress fileProgressBar = null;
	protected JTable filesTable = null;
	protected JTextArea hashesArea = null;
	protected Log log = null;
	protected XaucSwing parent = null;
	protected JTree filesTree = null;
	
	public SwingFileProcessor(){
		this.files = new HashMap<File,SwingFile>();
		this.mainProgressBar = new SwingProgress();
		this.fileProgressBar = new SwingProgress();
	}
	public SwingFileProcessor(XaucShared sharedParent, HashMap<File,SwingFile> files) {
		this();
		this.sharedParent = sharedParent;
		this.files = files;
		hashingOptions = sharedParent.getHashingOptions();
		parsingOptions = sharedParent.getParsingOptions();
		clientOptions = sharedParent.getClientOptions();
	}
	/** @return the files */
	public synchronized HashMap<File, SwingFile> getFiles() { return files; }
	/** @param files the files to set */
	public synchronized void setFiles(HashMap<File, SwingFile> files) { this.files = files; }
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
	/** @return the parent */
	public synchronized XaucSwing getParent() { return parent; }
	/** @param parent the parent to set */
	public synchronized void setParent(XaucSwing parent) { this.parent = parent; }
	/** @return the hashesArea */
	public synchronized JTextArea getHashesArea() { return hashesArea; }
	/** @param hashesArea the hashesArea to set */
	public synchronized void setHashesArea(JTextArea hashesArea) { this.hashesArea = hashesArea; }
	/** @return the threadName */
	public synchronized String getThreadName() { return threadName;	}
	/** @param threadName the threadName to set */
	public synchronized void setThreadName(String threadName) { this.threadName = threadName; }
	/** @return the filesTree */
	public synchronized JTree getFilesTree() { return filesTree; }
	/** @param filesTree the filesTree to set */
	public synchronized void setFilesTree(JTree filesTree) { this.filesTree = filesTree; }
	
	/**
	 * Parses files and sets states
	 */
	public synchronized void work() {
		if (this.files.isEmpty()) return;
		Set<File> filesToCheck = this.files.keySet();
		Iterator<File> it = filesToCheck.iterator();
		this.mainProgressBar.setAction("processing");
		//this.mainProgressBar.setShowProgress(true);
		FileParser parser;
		int processedFiles = 0;
		float curprogress = 0;
		this.mainProgressBar.setProgress(curprogress);
		while(it.hasNext()) {
			// Check for suspension, to prevent bad stuff only allowed before starting each file
			checkForSuspend();
			File file = it.next();
			SwingFile sf = this.files.get(file);
			if ((hashingOptions.isEnabled() && sf.isHASHED() && parsingOptions.isEnabled() && sf.isPARSED()) ||
					(hashingOptions.isEnabled() && !parsingOptions.isEnabled() && sf.isHASHED()) ||
					(!hashingOptions.isEnabled() && parsingOptions.isEnabled() && sf.isPARSED())) { 
				processedFiles++;
				curprogress = ((float)processedFiles / (float)filesToCheck.size());
				this.fileProgressBar.setProgress(1);
				this.mainProgressBar.setProgress(curprogress);
				continue;
			}
			if (this.filesTable != null) {
				this.filesTable.setValueAt("processing", sf.getRowId(), 2);
			}
			parser = new FileParser(this.sharedParent,file);
			parser.setLog(this.log);
			this.fileProgressBar.setProgress(0);
			parser.setProgress(this.fileProgressBar);
			parser.work();
			processedFiles++;
			curprogress = ((float)processedFiles / (float)filesToCheck.size());
			this.mainProgressBar.setProgress(curprogress);
			if (!parser.getErrorMessage().equals(""))
				this.log.println(parser.getErrorMessage());
			AniDBFile afile = parser.getAnidbFile();
			if (afile == null) {
				XaucConsole.printError("some error occured while processing \"" + file.getName() + "\"");
				XaucConsole.printError(parser.getErrorMessage());
				continue;
			}
			sf.setAniDBFile(afile);
			if (this.filesTable != null) this.filesTable.setValueAt(afile.ed2k, sf.getRowId(), 1);
			if (this.hashingOptions.isEnabled()) {
				sf.setHASHED();
				if (this.filesTable != null) this.filesTable.setValueAt("hashed", sf.getRowId(), 2);
				if (this.hashesArea != null) this.hashesArea.append(afile.ed2klink+'\n');
			}
			if (this.parsingOptions.isEnabled()) {
				sf.setPARSED();
				if (this.filesTable != null) this.filesTable.setValueAt("parsed", sf.getRowId(), 2);
			}
			if (this.filesTree != null) {
				DefaultTreeModel model = (DefaultTreeModel)this.filesTree.getModel();
				//DefaultMutableTreeNode treeFile = new DefaultMutableTreeNode(file.getName());
				TreePath path = this.filesTree.getNextMatch("unidentified", 0, Position.Bias.Forward);
				DefaultMutableTreeNode unidentified = (DefaultMutableTreeNode)path.getLastPathComponent();
				DefaultMutableTreeNode node = new DefaultMutableTreeNode(file.getName());
				node.setUserObject(sf);
				unidentified.add(node);
				model.reload();
			}
		}
		// warn parent window
		if (this.parent != null) this.parent.processingDone();
	}
}

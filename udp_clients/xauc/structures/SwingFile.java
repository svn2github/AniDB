package structures;

import java.io.File;

/**
 * Swing Interface File
 */
public class SwingFile {
	// states
	public static int DEFAULT = 0;
	public static int ADDED = 1;
	public static int HASHED = 2;
	public static int PARSED = 4;
	public static int ANIDBQUERIED = 8;
	public static int IDENTIFIED = 16;
	public static int ADDEDTOMYLIST = 32;
	public static int UNKNOWN = 64;

	protected File file = null;
	protected int rowId = -1;
	protected AniDBFile anidbfile = null;
	protected int state = DEFAULT;

	public SwingFile() {};
	public SwingFile(File file) { this.file = file; this.state = ADDED; }
	public SwingFile(File file, int rowId) { this(file); this.rowId = rowId; }
	public SwingFile(File file, int rowId, AniDBFile anidbfile) { this(file,rowId); this.anidbfile = anidbfile; }

	/** @return the file */
	public synchronized File getFile() { return file; }
	/** @return the rowId */
	public synchronized int getRowId() { return rowId; }
	/** @return the anidbfile */
	public synchronized AniDBFile getAniDBFile() { return anidbfile; }
	/** @param file File to set */
	public synchronized void setFile(File file) { this.file = file; }
	/** @param rowid rowId to set */
	public synchronized void setRowId(int rowId) { this.rowId = rowId; }
	/** @param anidbfile anidbfile to set */
	public synchronized void setAniDBFile(AniDBFile anidbfile) { this.anidbfile = anidbfile; }
	/** @return state */
	public synchronized int getState() { return this.state; }
	public synchronized int setADDED() { this.state |= ADDED; return this.state; }
	public synchronized int setHASHED() { this.state |= HASHED; return this.state; }
	public synchronized int setPARSED() { this.state |= PARSED; return this.state; }
	public synchronized int setANIDBQUERIED() { this.state |= ANIDBQUERIED; return this.state; }
	public synchronized int setIDENTIFIED() { this.state |= IDENTIFIED; return this.state; }
	public synchronized int setADDEDTOMYLIST() { this.state |= ADDEDTOMYLIST; return this.state; }
	public synchronized int setUNKNOWN() { this.state |= UNKNOWN; return this.state; }
	private synchronized boolean checkFlag(int state) { return ((this.state & state) == state); }
	public synchronized boolean isDEFAULT() { return checkFlag(DEFAULT); }
	public synchronized boolean isADDED() { return checkFlag(ADDED); }
	public synchronized boolean isHASHED() { return checkFlag(HASHED); }
	public synchronized boolean isPARSED() { return checkFlag(PARSED); }
	public synchronized boolean isANIDBQUERIED() { return checkFlag(ANIDBQUERIED); }
	public synchronized boolean isIDENTIFIED() { return checkFlag(IDENTIFIED); }
	public synchronized boolean isADDEDTOMYLIST() { return checkFlag(ADDEDTOMYLIST); }
	public synchronized boolean isUNKNOWN() { return checkFlag(UNKNOWN); }

}

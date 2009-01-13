package clients;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * XAUC client options
 */
public class XaucOptions  implements Serializable{
	private static final long serialVersionUID = -7238368047578065332L;
	/**
	 * List of output formats.
	 * <ul><li>NONE - no output</li>
	 * <li>TXT - output as tabbed text</li>
	 * <li>XML - output as XML</li></ul>
	 */
	public static enum OutputFormats { NONE, TXT, XML };
	// GENERAL settings (apply to all clients)
	/** See debug messages on clients */
	protected boolean seeDebug = false;
	/** Recurse into directories while searching for files */
	protected boolean recurseDir = false;
	/** Write parsed file information to disk format */
	protected OutputFormats writeFileOuputFormat = OutputFormats.NONE;
	// CLI settings
	/** Parsed file information output to console format */
	protected OutputFormats stdoutFileOuputFormat = OutputFormats.TXT;
	// GUI settings
	/** Parsed file information output to files tree format */
	protected OutputFormats treeFileOuputFormat = OutputFormats.TXT; 
	/** Serialization filename */
	protected String serFilename = "clientSettings.ser";
	
	/** Default XaucOptions constructor (use getters to set specific settings) */
	public XaucOptions() {}
	
	/**
	 * Returns the state of the debug messages verbosity
	 * @return seeDebug state
	 */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/**
	 * Sets the state of debug messages
	 * @param seeDebug the state to set (true/false)
	 */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/**
	 * Returns the current state of the recurse directories option
	 * @return recurseDir state
	 */
	public synchronized boolean isRecurseDir() { return recurseDir; }
	/**
	 * Sets the state of the recurse directories option
	 * @param recurseDir the state to set (true/false)
	 */
	public synchronized void setRecurseDir(boolean recurseDir) { this.recurseDir = recurseDir; }
	/**
	 * Gets the current output format for file information (as stored in the hard drive)
	 * @return the writeFileOuputFormat 
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized OutputFormats getWriteFileOuputFormat() { return writeFileOuputFormat; }
	/**
	 * Set the current output format for file information (as stored in the hard drive)
	 * @param writeFileOuputFormat the format to set
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized void setWriteFileOuputFormat(OutputFormats writeFileOuputFormat) { this.writeFileOuputFormat = writeFileOuputFormat; }
	/**
	 * Gets the current output format for file information (as displayed in standard output)
	 * @return the stdoutFileOuputFormat
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized OutputFormats getStdoutFileOuputFormat() { return stdoutFileOuputFormat; }
	/**
	 * Set the current output format for file information (as displayed in standard output)
	 * @param stdoutFileOuputFormat the format to set
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized void setStdoutFileOuputFormat(OutputFormats stdoutFileOuputFormat) { this.stdoutFileOuputFormat = stdoutFileOuputFormat; }
	/**
	 * Gets the current output format for file information (as displayed in the files tree)
	 * @return the treeFileOuputFormat
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized OutputFormats getTreeFileOuputFormat() { return treeFileOuputFormat; }
	/**
	 * Set the current output format for file information (as displayed in the files tree)
	 * @param treeFileOuputFormat the treeFileOuputFormat to set
	 * @see XaucOptions.OutputFormats
	 */
	public synchronized void setTreeFileOuputFormat(OutputFormats treeFileOuputFormat) { this.treeFileOuputFormat = treeFileOuputFormat; }
	/**
	 * Gets the serialization filename
	 * @return the filename
	 */
	public synchronized String getSerFilename() { return serFilename; }

	/**
	 * Serializes AVParserOptions to disk
	 * @param appDir Application directory where file will be saved to
	 * @return True if success, false otherwise
	 */
	public synchronized boolean saveSettings(String appDir) {
		FileOutputStream fOut=null;
		ObjectOutputStream oOut=null;
		String filename = appDir + File.separator + serFilename;
		boolean isOK = true;
		try{
			fOut= new FileOutputStream(filename);
			oOut = new ObjectOutputStream(fOut);
			oOut.writeObject(this);
			if (seeDebug) System.out.println("Saved parsing settings to: "+filename);
			isOK = true;
		} catch(IOException e){
			System.err.println("Error while saving parsing settings to: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally{
			try {
				oOut.flush();
				oOut.close();
				fOut.close();
			} catch (IOException e1) {
				System.err.println("Error while saving parsing settings to: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
	
	/**
	 * Loads serialized AVParserOptions from disk
	 * @param appDir Application directory where file will be loaded from
	 * @return True if success, false otherwise
	 */
	public synchronized boolean loadSettings(String appDir) {
		String filename = appDir + File.separator + serFilename;
		boolean isOK = true;
		FileInputStream fIn=null;
		ObjectInputStream oIn=null;

		try {
			fIn= new FileInputStream(filename);
			oIn = new ObjectInputStream(fIn);
			XaucOptions options = (XaucOptions) oIn.readObject();
			this.recurseDir = options.isRecurseDir();
			this.seeDebug = options.isSeeDebug();
			this.stdoutFileOuputFormat = options.getStdoutFileOuputFormat();
			this.treeFileOuputFormat = options.getTreeFileOuputFormat();
			this.writeFileOuputFormat = options.getWriteFileOuputFormat();
			if (seeDebug) System.out.println("Loaded parsing settings from: "+filename);
		} catch(IOException e) {
			System.err.println("Error while loading parsing settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} catch(ClassNotFoundException e) {
			System.err.println("Error while loading parsing settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally {
			try {
				oIn.close();
				fIn.close();
			} catch (IOException e1) {
				System.err.println("Error while loading parsing settings from: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
}

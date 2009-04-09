package options;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * Hasher Package Options
 * <p>Saves and loads hasher settings</p> 
 * @author fahrenheit
 * @version 1.0 - Initial Release
 * @since 08.09.2009 18:55
 */
public class HasherOptions implements Serializable {
	/** Serial Version */
	private static final long serialVersionUID = 646853252913469182L;
	// Hashing algorithms
	/** ED2K hashing support */
	public boolean ed2k = true;
	/** CRC32 hashing support */
	public boolean crc32 = true;
	/** MD5 hashing support */
	public boolean md5 = true;
	/** SHA1 hashing support */
	public boolean sha1 = true;
	/** TTH hashing support */
	public boolean tth = true;
	// Hasher settings
	/** Hashing enabled */
	public boolean enabled = true;
	/** Serialization filename */
	public String serFilename = "settings_hasher.ser";
	// if the buffer value is changed the result of the ed2k hash is wrong
	/** Buffer size (in bytes) */
	public int bufferSize = 1048576*3;
	/** Debug messages */
	public boolean seeDebug = true;
	
	/**
	 * Default HasherOptions constructor
	 */
	public HasherOptions() {}
	
	/**
	 * Serializes HasherOptions to disk
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
			if (seeDebug) System.out.println("Saved hashing settings to: "+filename);
			isOK = true;
		} catch(IOException e){
			System.err.println("Error while saving hashing settings to: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally{
			try {
				oOut.flush();
				oOut.close();
				fOut.close();
			} catch (IOException e1) {
				System.err.println("Error while saving hashing settings to: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
	
	/**
	 * Loads serialized HasherOptions from disk
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
			HasherOptions options = (HasherOptions) oIn.readObject();
			this.crc32 = options.crc32;
			this.ed2k = options.ed2k;
			this.md5 = options.md5;
			this.sha1 = options.sha1;
			this.tth = options.tth;
			this.bufferSize = options.bufferSize;
			this.seeDebug = options.seeDebug;
			this.enabled = options.enabled;
			if (seeDebug) System.out.println("Loaded hashing settings from: "+filename);
		} catch(IOException e) {
			System.err.println("Error while loading hashing settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} catch(ClassNotFoundException e) {
			System.err.println("Error while loading hashing settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally {
			try {
				oIn.close();
				fIn.close();
			} catch (IOException e1) {
				System.err.println("Error while loading hashing settings from: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
}

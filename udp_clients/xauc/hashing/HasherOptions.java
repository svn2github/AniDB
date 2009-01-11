package hashing;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/** Defines Hasher Options */
public class HasherOptions implements Serializable {
	private static final long serialVersionUID = 6688562230469350398L;
	/** If true, hasher will use the ed2k algorithm */
	protected boolean enableED2K = true;
	/** If true, hasher will use the crc32 algorithm */
	protected boolean enableCRC32 = false;
	/** If true, hasher will use the md5 algorithm */
	protected boolean enableMD5 = false;
	/** If true, hasher will use the sha1 algorithm */
	protected boolean enableSHA1 = false;
	/** If true, hasher will use the tth algorithm */
	protected boolean enableTTH = false;
	/** Enable to see debug messages on Hasher */
	protected boolean seeDebug = false;
	/** If true Hasher will process files */
	protected boolean enabled = true;
	
	/** Default HasherOptions constructor */
	public HasherOptions() {}
	/**
	 * HasherOptions constructor where algorithms uses are defined
	 * @param ed use ed2k
	 * @param crc use crc32
	 * @param md use md5
	 * @param sha use sha1
	 * @param tth use tth
	 */
	public HasherOptions(boolean ed, boolean crc, boolean md, boolean sha, boolean tth) {
		this.enableED2K = ed;
		this.enableCRC32 = crc;
		this.enableMD5 = md;
		this.enableSHA1 = sha;
		this.enableTTH = tth;
	}
	/** @return the enableED2K */
	public synchronized boolean isEnableED2K() { return enableED2K; }
	/** @param enableED2K the enableED2K to set */
	public synchronized void setEnableED2K(boolean enableED2K) { this.enableED2K = enableED2K; }
	/** @return the enableCRC32 */
	public synchronized boolean isEnableCRC32() { return enableCRC32; }
	/** @param enableCRC32 the enableCRC32 to set */ 
	public synchronized void setEnableCRC32(boolean enableCRC32) { this.enableCRC32 = enableCRC32; }
	/** @return the enableMD5 */
	public synchronized boolean isEnableMD5() { return enableMD5; }
	/** @param enableMD5 the enableMD5 to set */
	public synchronized void setEnableMD5(boolean enableMD5) { this.enableMD5 = enableMD5; }
	/** @return the enableSHA1 */
	public synchronized boolean isEnableSHA1() { return enableSHA1; }
	/** @param enableSHA1 the enableSHA1 to set */
	public synchronized void setEnableSHA1(boolean enableSHA1) { this.enableSHA1 = enableSHA1; }
	/** @return the enableTTH */
	public synchronized boolean isEnableTTH() { return enableTTH; }
	/** @param enableTTH the enableTTH to set */
	public synchronized void setEnableTTH(boolean enableTTH) { this.enableTTH = enableTTH; }
	/** @return the seeDebug */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/** @param seeDebug the seeDebug to set */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/** @return the enabled */
	public synchronized boolean isEnabled() { return enabled; }
	/** @param enabled the enabled to set */
	public synchronized void setEnabled(boolean enabled) { this.enabled = enabled; }
	
	/**
	 * Serializes HasherOptions to disk
	 * @param appDir Application directory where file will be saved to
	 * @return True if success, false otherwise
	 */
	public synchronized boolean saveSettings(String appDir) {
		FileOutputStream fOut=null;
		ObjectOutputStream oOut=null;
		String filename = appDir + File.separator + "hashingSettings.ser";
		boolean isOK = true;
		try{
			fOut= new FileOutputStream(filename);
			oOut = new ObjectOutputStream(fOut);
			oOut.writeObject(this);
			System.out.println("Saved hashing settings to: "+filename);
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
		String filename = appDir + File.separator + "hashingSettings.ser";
		boolean isOK = true;
		FileInputStream fIn=null;
		ObjectInputStream oIn=null;

		try {
			fIn= new FileInputStream(filename);
			oIn = new ObjectInputStream(fIn);
			HasherOptions options = (HasherOptions) oIn.readObject();
			this.enabled = options.enabled;
			this.seeDebug = options.seeDebug;
			this.enableCRC32 = options.enableCRC32;
			this.enableED2K = options.enableED2K;
			this.enableMD5 = options.enableMD5;
			this.enableSHA1 = options.enableSHA1;
			this.enableTTH = options.enableTTH;
			System.out.println("Loaded hashing settings from: "+filename);
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

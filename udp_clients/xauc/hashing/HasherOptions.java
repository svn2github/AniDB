package hashing;

import java.io.Serializable;

/** Defines Hasher Options */
public class HasherOptions implements Serializable {
	private static final long serialVersionUID = 6688562230469350398L;
	protected boolean enableED2K = true;
	protected boolean enableCRC32 = false;
	protected boolean enableMD5 = false;
	protected boolean enableSHA1 = false;
	protected boolean enableTTH = false;
	protected boolean seeDebug = false;
	
	public HasherOptions() {}
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
}

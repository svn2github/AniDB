package avparsing;

import java.io.Serializable;

/** Defines AV Parser options */
public class AVParserOptions implements Serializable {
	private static final long serialVersionUID = 1531456229135854609L;
	protected boolean fullParse = false;
	protected boolean seeDebug = false;
	protected boolean enabled = true;
	
	public AVParserOptions() {};
	public AVParserOptions(boolean fp) {
		this.fullParse = fp;
	}
	/** @return the fullParse */
	public synchronized boolean isFullParse() { return fullParse; }
	/** @param fullParse the fullParse to set */
	public synchronized void setFullParse(boolean fullParse) { this.fullParse = fullParse; }
	/** @return the seeDebug */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/** @param seeDebug the seeDebug to set */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/** @return the enabled */
	public synchronized boolean isEnabled() { return enabled; }
	/** @param enabled the enabled to set */
	public synchronized void setEnabled(boolean enabled) { this.enabled = enabled; }
}

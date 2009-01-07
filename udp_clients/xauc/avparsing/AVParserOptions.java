package avparsing;

import java.io.Serializable;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary;

/** Defines AV Parser options */
public class AVParserOptions implements Serializable {
	private static final long serialVersionUID = 1531456229135854609L;
	protected boolean fullParse = false;
	protected boolean seeDebug = false;
	/** User set */
	protected boolean enabled = true;
	/** Internaly set if init failed */
	protected boolean disabled = false;
	protected AVFormatLibrary AVFORMAT = null;
	protected AVCodecLibrary AVCODEC = null;
	
	public AVParserOptions() {};
	public AVParserOptions(boolean fp) {
		this.fullParse = fp;
		this.AVFORMAT = AVFormatLibrary.INSTANCE;
		this.AVCODEC = AVCodecLibrary.INSTANCE;
		if (this.AVCODEC == null || this.AVFORMAT == null) {
			System.err.println("AVCODEC and/or AVFORMAT initialization went south.");
			this.disabled = true;
		}
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
	public synchronized boolean isEnabled() { return (disabled ? false : enabled); }
	/** @param enabled the enabled to set */
	public synchronized void setEnabled(boolean enabled) { this.enabled = enabled; }
	/** @return the disabled */
	public synchronized boolean isDisabled() { return disabled; }
	/** @return the aVFORMAT */
	public synchronized AVFormatLibrary getAVFORMAT() { return AVFORMAT; }
	/** @return the aVCODEC */
	public synchronized AVCodecLibrary getAVCODEC() { return AVCODEC; }
}

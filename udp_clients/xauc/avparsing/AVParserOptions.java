package avparsing;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

import net.sf.ffmpeg_java.AVCodecLibrary;
import net.sf.ffmpeg_java.AVFormatLibrary;

/** Defines AV Parser options */
public class AVParserOptions implements Serializable {
	private static final long serialVersionUID = 1531456229135854609L;
	/** If set to true, AVParser will execute a FullParse on the file */
	protected boolean fullParse = false;
	/** Set AVParser's FullParse VBR calculation mode (default by packet bitrate) */
	protected int vbr_calc_mode = AVParser.VBR_BY_PACKET_BITRATE;
	/** Enable to see debug messages on AVParser */
	protected boolean seeDebug = false;
	/** User set for AVParser state */
	protected boolean enabled = true;
	/** Internaly set if init failed */
	protected transient boolean disabled = false;
	/** FFmpeg libavformat reference */
	protected transient AVFormatLibrary AVFORMAT = null;
	/** FFmpeg libavcodec reference */
	protected transient AVCodecLibrary AVCODEC = null;
	
	/**
	 * Default AVParserOptions constructor
	 */
	public AVParserOptions() {
		initFFmpeg();
	}
	/**
	 * AVParserOptions constructor where FullParse option is specified
	 * @param fp If true will execute a FullParse on the file
	 */
	public AVParserOptions(boolean fp) {
		this();
		this.fullParse = fp;
	}
	
	/**
	 * Initializes FFmpeg libraries
	 */
	public synchronized void initFFmpeg() {
		this.AVFORMAT = AVFormatLibrary.INSTANCE;
		this.AVCODEC = AVCodecLibrary.INSTANCE;
		if (this.AVCODEC == null || this.AVFORMAT == null) {
			System.err.println("AVCODEC and/or AVFORMAT initialization went sour, disabling avparsing.");
			this.disabled = true;
		}
		// register formats
		if (this.AVFORMAT != null) this.AVFORMAT.av_register_all();
		// not sure what the consequences of such a mismatch are, but it is worth logging a warning:
		if (seeDebug && this.AVCODEC != null && this.AVCODEC.avcodec_version() != AVCodecLibrary.LIBAVCODEC_VERSION_INT)
			System.err.println("ffmpeg-java and ffmpeg versions do not match: avcodec_version=" + AVCODEC.avcodec_version() + " LIBAVCODEC_VERSION_INT=" + AVCodecLibrary.LIBAVCODEC_VERSION_INT);
		// register codecs and all
		if (this.AVCODEC != null) this.AVCODEC.avcodec_init();
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
	/** @return the vbr_calc_mode */
	public synchronized int getVbr_calc_mode() { return vbr_calc_mode; }
	/** @param vbr_calc_mode the vbr_calc_mode to set */
	public synchronized void setVbr_calc_mode(int vbr_calc_mode) { this.vbr_calc_mode = vbr_calc_mode; }
	
	/**
	 * Serializes AVParserOptions to disk
	 * @param appDir Application directory where file will be saved to
	 * @return True if success, false otherwise
	 */
	public synchronized boolean saveSettings(String appDir) {
		FileOutputStream fOut=null;
		ObjectOutputStream oOut=null;
		String filename = appDir + File.separator + "parsingSettings.ser";
		boolean isOK = true;
		try{
			fOut= new FileOutputStream(filename);
			oOut = new ObjectOutputStream(fOut);
			oOut.writeObject(this);
			System.out.println("Saved parsing settings to: "+filename);
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
		String filename = appDir + File.separator + "parsingSettings.ser";
		boolean isOK = true;
		FileInputStream fIn=null;
		ObjectInputStream oIn=null;

		try {
			fIn= new FileInputStream(filename);
			oIn = new ObjectInputStream(fIn);
			AVParserOptions options = (AVParserOptions) oIn.readObject();
			this.fullParse = options.fullParse;
			this.enabled = options.enabled;
			this.seeDebug = options.seeDebug;
			this.vbr_calc_mode = options.vbr_calc_mode;
			System.out.println("Loaded parsing settings from: "+filename);
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

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
	protected boolean enabled = false;
	/** Parsing implementation to use */
	protected AVParser.ParserImplementations parserImp = AVParser.ParserImplementations.NONE;
	/** Internaly set if init failed */
	protected transient boolean disabled = false;
	/** FFmpeg libavformat reference */
	protected transient AVFormatLibrary AVFORMAT = null;
	/** FFmpeg libavcodec reference */
	protected transient AVCodecLibrary AVCODEC = null;
	/** Serialization filename */
	protected String serFilename = "parsingSettings.ser";
	
	/**
	 * Default AVParserOptions constructor
	 */
	public AVParserOptions() {
		if (this.enabled) initFFmpeg();
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
		// If no parser implmentation is selected no need to check for ffmpeg
		if (this.parserImp == AVParser.ParserImplementations.NONE) return;
		// First check that we can load the libraries or not
		try {
			System.loadLibrary(AVCodecLibrary.libname);
		} catch (UnsatisfiedLinkError e) {
			System.err.println("You do not have FFmpeg libavcodec in your library path, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;
		} catch (SecurityException e1) {
			System.err.println("The Security Manager prevents you from loading FFmpeg lib avcodec, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;
		} catch (NullPointerException e2) {
			System.err.println("The programmer of this application has made a stupid error that prevents you from loading any library, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;			
		}
		try {
			System.loadLibrary(AVFormatLibrary.libname);
		} catch (UnsatisfiedLinkError e) {
			System.err.println("You do not have FFmpeg libavformat in your library path, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;
		} catch (SecurityException e1) {
			System.err.println("The Security Manager prevents you from loading FFmpeg libavformat, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;
		} catch (NullPointerException e2) {
			System.err.println("The programmer of this application has made a stupid error that prevents you from loading any library, disabling file parsing.");
			this.enabled = false;
			this.disabled = true;
			return;			
		}
		// if i survided until here the rest should be safe
		if (this.parserImp == AVParser.ParserImplementations.FFMPEGJAVA) {
			this.AVFORMAT = AVFormatLibrary.SYNC_INSTANCE;
			this.AVCODEC = AVCodecLibrary.SYNC_INSTANCE;
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
	}
	
	/** Checks if the parsing instance should fully process the given file
	 * @return true if full process is enabled, false otherwise */
	public synchronized boolean isFullParse() { return fullParse; }
	/** Defines if the parsing instance should fully parse a file or not.
	 * <p><b>Warning:</b><br>Check if the current ffmpeg interface implementation is prone to crashing with this setting enabled before using</p> 
	 * @param fullParse if set to true parser will fully process file */
	public synchronized void setFullParse(boolean fullParse) { this.fullParse = fullParse; }
	/** Checks if debug messages should be displayed or not
	 * @return true if debug messages should be displayed, false otherwise */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/** Defines if debug messages should be displayed or not 
	 * @param seeDebug debug messages will be displayed if set to true, otherwise will be hidden */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/** Checks if the user has enabled parsing or not 
	 * @return true if enabled, false otherwise */
	public synchronized boolean isEnabled() { return (disabled ? false : enabled); }
	/** Defines if parsing should be enabled or not.
	 * <p><b>Note:</b><br>This will try to load FFmpeg libraries and if it fails will disable parsing</p> 
	 * @param enabled the enabled to set */
	public synchronized void setEnabled(boolean enabled) { 
		this.enabled = enabled;
		if (this.enabled) initFFmpeg();
	}
	/** Checks if parsing is disabled, this will only be true if it's impossible to load FFmpeg libraries 
	 * @return true if disabled, false otherwise */
	public synchronized boolean isDisabled() { return disabled; }
	/** Gets the libavformat java interface instance used by ffmpeg-java
	 * @return AVFORMAT instance */
	public synchronized AVFormatLibrary getAVFORMAT() { return AVFORMAT; }
	/** Gets the libavcodec java interface instance used by ffmpeg-java
	 * @return AVCODEC instance */
	public synchronized AVCodecLibrary getAVCODEC() { return AVCODEC; }
	/** Gets the current VBR calculation mode
	 * <p><b>Note:</b><br>VBR calculation is only done if full parse is enabled</p>
	 * @see AVParser.VBR_DELTA_SIZE
	 * @see AVParser.VBR_DELTA_BITRATE
	 * @return the vbr_calc_mode */
	public synchronized int getVbr_calc_mode() { return vbr_calc_mode; }
	/** Sets the current VBR calculation mode
	 * <p><b>Note:</b><br>VBR calculation is only done if full parse is enabled</p>
	 * @see AVParser.VBR_DELTA_SIZE
	 * @see AVParser.VBR_DELTA_BITRATE
	 * @param vbr_calc_mode the VBR calculation mode to use, should be one of the mentioned values */
	public synchronized void setVbr_calc_mode(int vbr_calc_mode) { this.vbr_calc_mode = vbr_calc_mode; }
	/**
	 * Gets the serialization filename
	 * @return the filename
	 */
	public synchronized String getSerFilename() { return serFilename; }
	/**
	 * Gets the current AVParser Implementation
	 * @return The Parser implementation
	 * @see AVParser.ParserImplementations
	 */
	public synchronized AVParser.ParserImplementations getParserImp() {
		return parserImp;
	}
	/** Sets the AVParser implementation to use
	 * @param parserImp the AVParser Implementation to use 
	 * @see AVParser.ParserImplementations
	 */
	public synchronized void setParserImp(AVParser.ParserImplementations parserImp) {
		this.parserImp = parserImp;
	}

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
			AVParserOptions options = (AVParserOptions) oIn.readObject();
			this.fullParse = options.fullParse;
			this.enabled = options.enabled;
			this.seeDebug = options.seeDebug;
			this.vbr_calc_mode = options.vbr_calc_mode;
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

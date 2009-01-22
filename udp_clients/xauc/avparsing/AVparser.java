package avparsing;

import java.io.File;

import structures.AniDBFile;
import utils.Log;
import utils.Progress;

/**
 * AVParser Interface.
 * <p>This interface holds the common interfaces for AVParser implementations.</p>
 * <p>Currently you can choose one of the following implementations:
 * <ul><li><b>AVParserFFmpegJava</b> - Original implementation, uses JNA to interface with FFmpeg, <b>not stable</b> for full parsing</li>
 * <li><b>AVParserXuggle</b> - Xuggle.org implementation, uses JNI to interface with FFmpeg, stable enough, some fields currently missing, beta</li>
 * <li><b>AVParserNone</b> - Fake implementation, uses no dependencies so should be good to offer when Parsing ability is required</li></ul>
 * </p>
 * @author fahrenheit
 */
public interface AVParser {
	/** Parser Implementations.
	 * <p>Contains an enumeration of valid Parser implementations</p> */
	public static enum ParserImplementations {
		/** AVParserNone implementation */
		NONE ("None", "Fake implementation, uses no dependencies so should be good to offer when Parsing ability is required"),
		/** AVParserXuggle implementation */
		XUGGLE ("Xuggle", "Xuggle.org implementation, uses JNI to interface with FFmpeg, stable enough, some fields currently missing, beta"),
		/** AVParserFFmpegJava implementation */
		FFMPEGJAVA ("FFmpeg-java", "Original implementation, uses JNA to interface with FFmpeg, *not stable* for full parsing");
		
		/** Implementation name */
		public String name;
		/** Implementation description */
		public String desc;
		
		/**
		 * ParserImplementations ENUM constructor
		 * @param name Parser implementation name
		 * @param desc Parser implementation description
		 */
		ParserImplementations(String name, String desc) { this.name = name; this.desc = desc; }
		/** Gets the implementation name */
		public String getName() { return name; }
		/** Gets the implementation name */
		public String getDesc() { return desc; }
		public String toString() { return name; }
	}
	
	/** Delta used to detect if a stream is VBR or not (used in VBR by packet bitrate mode) */
	public static final double VBR_DELTA_BITRATE = 0.03; // allow 2% in packetBitrate variations
	/** Delta used to detect if a stream is VBR or not (used in VBR by packet size mode) */
	public static final int VBR_DELTA_SIZE = 1; // allow 1 packetSizeUnits in packetSize variations
	/** Delta used to detect if a stream is VFR */
	public static final int VFR_DELTA = 1; // allow 1 dtsIntervalUnit in dtsInterval variations
	/** 
	 * VBR calculation is done by Packet Size
	 * <p>This mode is faster than the bitrate mode (in very relative terms) but could prove less acurate in certain corner cases</p>
	 * @see AVParser.VBR_DELTA_SIZE
	 * @see AVParser.VBR_BY_PACKET_BITRATE
	 */
	public static final int VBR_BY_PACKET_SIZE = 1; // calculate vbr by packet size
	/** 
	 * VBR calculation is done by Packet Bitrate
	 * <p>This mode is slower than the packet mode (in very relative terms) but should be very acurate even in certain corner cases</p>
	 * @see AVParser.VBR_DELTA_BITRATE
	 * @see AVParser.VBR_BY_PACKET_SIZE
	 */
	public static final int VBR_BY_PACKET_BITRATE = 2; // calculate vbr by packet bitrate

	/** Gets the file associated with this parser instance
	 * @return the file */
	public File getFile();
	/** Sets the file associated with this parser instance
	 * @param file the file to set */
	public void setFile(File file);
	/** Gets the AniDBFile associated with this parser instance
	 * @return the anidbFile */
	public AniDBFile getAnidbFile();
	/** Sets the AniDBFile associated with this parser instance
	 * @param anidbFile the AniDBFile to set */
	public void setAnidbFile(AniDBFile anidbFile);
	/** Gets the Log associated with this parser instance
	 * @return the log */
	public Log getLog();
	/** Sets the Log associated with this parser instance 
	 * @param log the Log to set */
	public void setLog(Log log);
	/** Gets the Progress instance associated with this parser instance
	 * @return the progress */
	public Progress getProgress();
	/** Sets the Progress instance associated with this parser instance 
	 * @param progress the Progress to set */
	public void setProgress(Progress progress);
	/** Checks if debug messages should be displayed or not
	 * @return true if debug messages should be displayed, false otherwise */
	public boolean isShowDebug();
	/** Defines if debug messages should be displayed or not 
	 * @param showDebug debug messages will be displayed if set to true, otherwise will be hidden */
	public void setShowDebug(boolean showDebug);
	/** Checks if the current instance should fully process the given file
	 * @return true if full process is enabled, false otherwise */
	public boolean isParseFile();
	/** Defines if the current file should be fully parsed or not.
	 * <p><b>Warning:</b><br>Check if the current ffmpeg interface implementation is prone to crashing with this setting enabled before using</p> 
	 * @param parseFile if set to true parser will fully process file */
	public void setParseFile(boolean parseFile);
	/** Gets any error messages encountered during processing 
	 * @return the errorMessage */
	public String getErrorMessage();
	/** Gets the current VBR calculation mode
	 * <p><b>Note:</b><br>VBR calculation is only done if full parse is enabled</p>
	 * @see AVParser.VBR_DELTA_SIZE
	 * @see AVParser.VBR_DELTA_BITRATE
	 * @return the vbr_calc_mode */
	public int getVbr_calc_mode();
	/** Sets the current VBR calculation mode
	 * <p><b>Note:</b><br>VBR calculation is only done if full parse is enabled</p>
	 * @see AVParser.VBR_DELTA_SIZE
	 * @see AVParser.VBR_DELTA_BITRATE
	 * @param vbr_calc_mode the VBR calculation mode to use, should be one of the mentioned values */
	public void setVbr_calc_mode(int vbr_calc_mode);
	
	/** Does the actual parsing work, can be called by the run() method to create a threaded worker */
	public void work();
}

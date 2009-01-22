package clients;

import hashing.HasherOptions;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import avparsing.AVParserOptions;

/**
 * Shared settings/functions between clients
 */
public class XaucShared {
	/** Major version of the internal source */
	protected static int internalVersionMajor = 0;
	/** Minor version of the internal source */
	protected static int internalVersionMinor = 6;
	/** Patch version of the internal source */
	protected static int internalVersionPatch = 5;
	
	/** Clients need to set this */
	protected static int clientVersionMajor = 0;
	/** Clients need to set this */
	protected static int clientVersionMinor = 0;
	/** Clients need to set this */
	protected static int clientVersionPatch = 0;
	
	/** Application directory */
	protected static String appDir = System.getProperty("user.home") + File.separator + ".xauc";
	/** Log file */
	protected static String logFile = appDir + File.separator + "log.txt";
	/** AVParser Options */
	protected static AVParserOptions parsingOptions = new AVParserOptions();
	/** Hasher Options */
	protected static HasherOptions hashingOptions = new HasherOptions();
	/** Client Options */
	protected static XaucOptions clientOptions = new XaucOptions();
	
	/**
	 * Gets the application directory
	 * @return the appDir
	 */
	public static synchronized String getAppDir() { return appDir; }
	/**
	 * Gets the default logFile location
	 * @return the logFile
	 */
	public static synchronized String getLogFile() { return logFile; }
	/**
	 * Gets current parsing options
	 * @return the parsingOptions
	 */
	public synchronized AVParserOptions getParsingOptions() { return parsingOptions; }
	/**
	 * Gets current hashing options
	 * @return the hashingOptions
	 */
	public synchronized HasherOptions getHashingOptions() { return hashingOptions; }
	/**
	 * Gets current client options
	 * @return the clientOptions
	 */
	public synchronized XaucOptions getClientOptions() { return clientOptions; }

	/**
	 * Prints an [ERROR] unrecognized option message to stderr
	 * @param message Message to display
	 */
	public static void printOptionError(String message) {
		System.err.println("[ERROR] unrecognized option: "+message);
	}
	
	/**
	 * Prints an [ERROR] message to stderr
	 * @param message Message to display
	 */
	public static void printError(String message) {
		System.err.println("[ERROR]: "+message);
	}
	
	/**
	 * Initiates the application.
	 * * Checks if the application directory exists and if some basic files/dirs are present
	 * * Does some sanity checks.
	 * * Loads saved settings
	 */
	public static void initApp() {
		// Check application directory
		File dir = new File(appDir);
		if (!dir.exists()) dir.mkdir();
		dir = new File(appDir+ File.separator + "file");
		if (!dir.exists()) dir.mkdir();
		dir = new File(appDir+ File.separator + "anime");
		if (!dir.exists()) dir.mkdir();
		dir = new File(appDir+ File.separator + "episode");
		if (!dir.exists()) dir.mkdir();
		dir = new File(appDir+ File.separator + "group");
		if (!dir.exists()) dir.mkdir();
		String extFile = appDir + File.separator + "extensions.txt";
		String logFile = appDir + File.separator + "log.txt";
		// Extension file
		File file = new File(extFile);
		BufferedWriter out;
		try {
			out = new BufferedWriter(new FileWriter(file));
			out.write("# Valid extensions, one per line"); out.newLine();
			out.write("# comments can only be the whole of the line"); out.newLine();
			out.write("avi"); out.newLine();
			out.write("mkv"); out.newLine();
			out.write("mp4"); out.newLine();
			out.write("ogm"); out.newLine();
			out.write("wmv"); out.newLine();
			out.write("mpg"); out.newLine();
			out.write("mpeg"); out.newLine();
			out.write("rm"); out.newLine();
			out.write("rmvb"); out.newLine();
			out.write("asf"); out.newLine();
			out.write("mov"); out.newLine();
			out.write("swf"); out.newLine();
			out.write("flv"); out.newLine();
/*			"avi", "mpg", "mpeg", "rm", "rmvb",
			"asf", "wmv", "mov", "ogm", "mp4", "mkv", "rar", "zip", "ace", "srt",
			"sub", "ssa", "smi", "idx", "ass", "txt", "swf", "flv"*/
			out.close();
		} catch (IOException e) {
			System.err.println(e.getLocalizedMessage());
		}
		file = new File(logFile);
		// default log file
		if (!file.exists()) {
			try {
				file.createNewFile();
			} catch (IOException e) {
				System.err.println(e.getLocalizedMessage());
			}
		}
		// Sanity Checks
		if (!(new File(appDir+File.separator+hashingOptions.getSerFilename()).exists())) hashingOptions.saveSettings(appDir);
		if (!(new File(appDir+File.separator+parsingOptions.getSerFilename()).exists())) parsingOptions.saveSettings(appDir);
		if (!(new File(appDir+File.separator+clientOptions.getSerFilename()).exists())) clientOptions.saveSettings(appDir);
		// Load settings
		hashingOptions.loadSettings(appDir);
		parsingOptions.loadSettings(appDir);
		clientOptions.loadSettings(appDir);
	}
	
	/**
	 * Gets the internal version of Xauc
	 * @return Internal version string, ie. 0.5.2
	 */
	public static String getInternalVersion() {
		return XaucShared.internalVersionMajor + "." + XaucShared.internalVersionMinor + "." + XaucShared.internalVersionPatch;
	}
	
	/**
	 * Gets the client version
	 * @return Version string, ie. 0.5.2
	 */
	public static String getClientVersion() {
		return XaucShared.clientVersionMajor + "." + XaucShared.clientVersionMinor + "." + XaucShared.clientVersionPatch;
	}
}

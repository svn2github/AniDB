package clients;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

import avparsing.AVParserOptions;
import hashing.HasherOptions;
import utils.*;
import structures.AniDBFile;

/** Xauc Cli (Console) frontend */
public class XaucConsole {
	protected static int versionMajor = 0;
	protected static int versionMinor = 3;
	
	/**
	 * Displays help
	 */
	public static void printHelp() {
		System.out.println("XAUC - X AniDB UDP Client (v"+versionMajor+"."+versionMinor+")");
		System.out.println();
		System.out.println("Usage: [OPTIONS] file|dir");
		System.out.println("Options:");
		System.out.println("\t--help displays this help");
		System.out.println("\t--r Recurse directory (if a directory is supplied)");
		System.out.println("\t--d See debug messages");
		System.out.println("\t--h:(on|off) Enable/disable file hashing (default: on)");
		System.out.println("\t--h:ed2k:(on|off) Hash file with the ed2k algorithm (default: on)");
		System.out.println("\t--h:crc32:(on|off) Hash file with the crc32 algorithm (default: off)");
		System.out.println("\t--h:md5:(on|off) Hash file with the md5 algorithm (default: off)");
		System.out.println("\t--h:sha1:(on|off) Hash file with the sha1 algorithm (default: off)");
		System.out.println("\t--h:tth:(on|off) Hash file with the sha1 algorithm (default: off)");
		System.out.println("\t--a:(on|off) Enable/disable file av parsing (default: on)");
		System.out.println("\t--a:fp:(on|off) Enable/disable full file av parsing (slower if enabled but more information is supplied, default: off)");
		System.out.println("\t--f:xml:(on|off) Enable/disable writing of parsed files xml to disk (see below for directory, default: on)");
		System.out.println("\t--f:console:(on|off) Enable/disable writing of parsed files data to console (default: off)");
		System.out.println();
		System.out.println("Application directory: "+System.getProperty("user.home") + File.separator + ".xauc");
		return;
	}
	
	/**
	 * Prints an [ERROR] unrecognized option message to err
	 * @param message Message to display
	 */
	public static void printOptionError(String message) {
		System.err.println("[ERROR] unrecognized option: "+message);
	}
	
	/**
	 * Prints an [ERROR] message to err
	 * @param message Message to display
	 */
	public static void printError(String message) {
		System.err.println("[ERROR]: "+message);
	}
	
	/**
	 * Checks if the application directory exists and if some basic files/dirs are present
	 */
	public static void checkAppDir() {
		String appDir = System.getProperty("user.home") + File.separator + ".xauc";
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
		// Extesion file
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
	}
	
	public static void main(String[] args) {
		boolean recurseDir = false;
		boolean seeDebug = false;
		boolean writeXMLtoDisk = true;
		boolean writeFILEtoConsole = false;
		HasherOptions hasherOptions = new HasherOptions();
		AVParserOptions avparserOptions = new AVParserOptions();
		File file = null;
		ArrayList<File> files = new ArrayList<File>();
		String appDir = System.getProperty("user.home") + File.separator + ".xauc";
		String logFile = appDir + File.separator + "log.txt";
				
		if (args.length < 1) { printHelp(); return; }

		checkAppDir();

		for (int i = 0; i < args.length; i++) {
			if (args[i].charAt(0) == '-' && args[i].charAt(1) == '-') { //options
				int firstIndexOfColon = args[i].indexOf(':');
				int secondIndexOfColon = args[i].indexOf(':', firstIndexOfColon+1);
				String type = args[i].substring(2, (firstIndexOfColon >= 0 ? firstIndexOfColon : args[i].length()) );
				String option = "";
				String value = "";
				if (firstIndexOfColon >= 0 && secondIndexOfColon >= 0) 
					option = args[i].substring(firstIndexOfColon+1, secondIndexOfColon);
				else if (firstIndexOfColon >= 0 && secondIndexOfColon < 0)
					value = args[i].substring(firstIndexOfColon+1);
				if (secondIndexOfColon >= 0) value = args[i].substring(secondIndexOfColon+1);
				type = type.toLowerCase();
				option = option.toLowerCase();
				value = value.toLowerCase();
				//System.out.println("option: "+type+":"+option+":"+value);
				if (type.equals("r")) recurseDir = true;
				else if (type.equals("help") || type.equals("?")) { printHelp(); return; }
				else if (type.equals("d")) seeDebug = true;
				else if (type.equals("h")) { // parse hashing options
					if (option.equals("")) {
						if (value.equals("on")) hasherOptions.setEnabled(true);
						else if (value.equals("off")) hasherOptions.setEnabled(false);
						else printOptionError(args[i]);
					} else if (option.equals("ed2k")) {
						if (value.equals("on")) hasherOptions.setEnableED2K(true);
						else if (value.equals("off")) hasherOptions.setEnableED2K(false);
						else printOptionError(args[i]);
					} else if (option.equals("crc32")) {
						if (value.equals("on")) hasherOptions.setEnableCRC32(true);
						else if (value.equals("off")) hasherOptions.setEnableCRC32(false);
						else printOptionError(args[i]);
					} else if (option.equals("md5")) {
						if (value.equals("on")) hasherOptions.setEnableMD5(true);
						else if (value.equals("off")) hasherOptions.setEnableMD5(false);
						else printOptionError(args[i]);
					} else if (option.equals("sha1")) {
						if (value.equals("on")) hasherOptions.setEnableSHA1(true);
						else if (value.equals("off")) hasherOptions.setEnableSHA1(false);
						else printOptionError(args[i]);
					} else if (option.equals("tth")) {
						if (value.equals("on")) hasherOptions.setEnableTTH(true);
						else if (value.equals("off")) hasherOptions.setEnableTTH(false);
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else if (type.equals("a")) { // parse avparsing options
					if (option.equals("")) {
						if (value.equals("on")) avparserOptions.setEnabled(true);
						else if (value.equals("off")) avparserOptions.setEnabled(false);
						else printOptionError(args[i]);
					} else if (option.equals("fp")) {
						if (value.equals("on")) avparserOptions.setFullParse(true);
						else if (value.equals("off")) avparserOptions.setFullParse(false);
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else if (type.equals("f")) {
					if (option.equals("xml")) {
						if (value.equals("on")) writeXMLtoDisk = true;
						else if (value.equals("off")) writeXMLtoDisk = false;
						else printOptionError(args[i]);
					} else if (option.equals("console")) {
						if (value.equals("on")) writeFILEtoConsole = true;
						else if (value.equals("off")) writeFILEtoConsole = false;
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else printOptionError(args[i]);
			} else { // we have a file or a dir
				file = new File(args[i]);
				if (file.isFile()) files.add(file);
				else if (file.isDirectory()) {
					DirectoryParser dirparser = new DirectoryParser(recurseDir);
					files.addAll(dirparser.scanDirectory(file));
				} else 
					printError("\""+args[i]+"\" is not a file, directory or option, skipping...");
			}
		}
		
		System.out.println("Found "+files.size()+" file(s)");
		if (files.size() > 0) {
			hasherOptions.setSeeDebug(seeDebug);
			avparserOptions.setSeeDebug(seeDebug);
			Log log = new Log(logFile,false);
			Progress process = new Progress();
			process.setAction("processing");
			process.setShowProgress(true);
			Progress progress = new Progress();
			Iterator<File> it = files.iterator();
			FileParser parser;
			int processedFiles = 0;
			float curprogress = 0;
			while(it.hasNext()) {
				file = it.next();
				parser = new FileParser(file,hasherOptions,avparserOptions);
				parser.setLog(log);
				parser.setProgress(progress);
				parser.work();
				processedFiles++;
				curprogress = ((float)processedFiles / (float)files.size());
				process.setProgress(curprogress);
				if (!parser.getErrorMessage().equals(""))
					log.println(parser.getErrorMessage());
				AniDBFile afile = parser.getAnidbFile();
				if (afile == null) {
					printError("some error occured while processing \""+file.getName()+"\"");
					continue;
				}
				if (writeXMLtoDisk) {
					try {
						afile.writeXml();
					} catch (Exception e) {
						printError("some error occured while writing \""+file.getName()+"\" xml file.");
						log.println(e.getLocalizedMessage());
					}
				}
				if (writeFILEtoConsole) {
					afile.writeToConsole();
				}
			}
		}
	}
}

package clients;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;

import avparsing.AVParserFFmpegJava;
import utils.*;
import structures.AniDBFile;

/** Xauc Cli (Console) frontend */
public class XaucConsole extends XaucShared{
	/**
	 * Displays help
	 */
	public static void printHelp() {
		System.out.println("XAUC - X AniDB UDP Client (v"+getClientVersion()+"/v"+getInternalVersion()+")");
		System.out.println();
		System.out.println("Usage: [OPTIONS] file|dir");
		System.out.println("Options:");
		System.out.println("\t--help displays this help");
		System.out.println("\t--d See debug messages");
		System.out.println("\t--h:(on|off) Enable/disable file hashing (default: on)");
		System.out.println("\t--h:save:(on|off) Enable/disable saving of hashing options (default: off)");
		System.out.println("\t--h:ed2k:(on|off) Hash file with the ed2k algorithm (default: on)");
		System.out.println("\t--h:crc32:(on|off) Hash file with the crc32 algorithm (default: off)");
		System.out.println("\t--h:md5:(on|off) Hash file with the md5 algorithm (default: off)");
		System.out.println("\t--h:sha1:(on|off) Hash file with the sha1 algorithm (default: off)");
		System.out.println("\t--h:tth:(on|off) Hash file with the sha1 algorithm (default: off)");
		System.out.println("\t--a:(on|off) Enable/disable file av parsing (default: off)");
		System.out.println("\t--a:save:(on|off) Enable/disable saving of av parsing options (default: off)");
		System.out.println("\t--a:fp:(on|off) Enable/disable full file av parsing (slower if enabled but more information is supplied, default: off)");
		System.out.println("\t--a:vbrmode:(size|bitrate) Set vbr calculation mode in full file av parsing (size should be faster but less accurate, default: bitrate)");
		System.out.println("\t--f:write:(none|xml|text) Enable/disable writing of parsed files xml to disk (see below for directory, default: none)");
		System.out.println("\t--f:console:(none|xml|text) Enable/disable writing of parsed files data to console (default: text)");
		System.out.println("\t--f:recurse:(on|off) Enable/disable recursing in to directories (if a directory is supplied, default: off)");
		System.out.println();
		System.out.println("Application directory: "+System.getProperty("user.home") + File.separator + ".xauc");
		return;
	}

	public static void main(String[] args) {
		XaucShared.clientVersionMajor = 0;
		XaucShared.clientVersionMinor = 3;
		XaucShared.clientVersionPatch = 1;
		boolean seeDebug = false;
		boolean saveParsingSettings = false;
		boolean saveHashingSettings = false;
		boolean saveClientSettings = false;
		File file = null;
		ArrayList<File> files = new ArrayList<File>();
				
		if (args.length < 1) { printHelp(); return; }

		initApp();

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
				if (type.equals("help") || type.equals("?")) { printHelp(); return; }
				else if (type.equals("d")) seeDebug = true;
				else if (type.equals("h")) { // parse hashing options
					if (option.equals("")) {
						if (value.equals("on")) hashingOptions.setEnabled(true);
						else if (value.equals("off")) hashingOptions.setEnabled(false);
						else printOptionError(args[i]);
					} else if (option.equals("ed2k")) {
						if (value.equals("on")) hashingOptions.setEnableED2K(true);
						else if (value.equals("off")) hashingOptions.setEnableED2K(false);
						else printOptionError(args[i]);
					} else if (option.equals("crc32")) {
						if (value.equals("on")) hashingOptions.setEnableCRC32(true);
						else if (value.equals("off")) hashingOptions.setEnableCRC32(false);
						else printOptionError(args[i]);
					} else if (option.equals("md5")) {
						if (value.equals("on")) hashingOptions.setEnableMD5(true);
						else if (value.equals("off")) hashingOptions.setEnableMD5(false);
						else printOptionError(args[i]);
					} else if (option.equals("sha1")) {
						if (value.equals("on")) hashingOptions.setEnableSHA1(true);
						else if (value.equals("off")) hashingOptions.setEnableSHA1(false);
						else printOptionError(args[i]);
					} else if (option.equals("tth")) {
						if (value.equals("on")) hashingOptions.setEnableTTH(true);
						else if (value.equals("off")) hashingOptions.setEnableTTH(false);
						else printOptionError(args[i]);
					} else if (option.equals("save")) {
						if (value.equals("on")) saveHashingSettings = true;
						else if (value.equals("off")) saveHashingSettings = false;
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else if (type.equals("a")) { // parse avparsing options
					if (option.equals("")) {
						if (value.equals("on")) parsingOptions.setEnabled(true);
						else if (value.equals("off")) parsingOptions.setEnabled(false);
						else printOptionError(args[i]);
					} else if (option.equals("fp")) {
						if (value.equals("on")) parsingOptions.setFullParse(true);
						else if (value.equals("off")) parsingOptions.setFullParse(false);
						else printOptionError(args[i]);
					} else if (option.equals("vbrmode")) {
						if (value.equals("bitrate")) parsingOptions.setVbr_calc_mode(AVParserFFmpegJava.VBR_BY_PACKET_BITRATE);
						else if (value.equals("size")) parsingOptions.setVbr_calc_mode(AVParserFFmpegJava.VBR_BY_PACKET_SIZE);
						else printOptionError(args[i]);
					} else if (option.equals("save")) {
						if (value.equals("on")) saveParsingSettings = true;
						else if (value.equals("off")) saveParsingSettings = false;
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else if (type.equals("f")) {
					if (option.equals("xml")) {
						if (value.equals("none")) clientOptions.setWriteFileOuputFormat(XaucOptions.OutputFormats.NONE);
						else if (value.equals("xml")) clientOptions.setWriteFileOuputFormat(XaucOptions.OutputFormats.XML);
						else if (value.equals("text")) clientOptions.setWriteFileOuputFormat(XaucOptions.OutputFormats.TXT);
						else printOptionError(args[i]);
					} else if (option.equals("console")) {
						if (value.equals("none")) clientOptions.setStdoutFileOuputFormat(XaucOptions.OutputFormats.NONE);
						else if (value.equals("xml")) clientOptions.setStdoutFileOuputFormat(XaucOptions.OutputFormats.XML);
						else if (value.equals("text")) clientOptions.setStdoutFileOuputFormat(XaucOptions.OutputFormats.TXT);
						else printOptionError(args[i]);
					} else if (option.equals("recurse")) {
						if (value.equals("on")) clientOptions.setRecurseDir(true);
						else if (value.equals("off")) clientOptions.setRecurseDir(false);
						else printOptionError(args[i]);
					} else if (option.equals("save")) {
						if (value.equals("on")) saveClientSettings = true;
						else if (value.equals("off")) saveClientSettings = false;
						else printOptionError(args[i]);
					} else printOptionError(args[i]);
				} else printOptionError(args[i]);
			} else { // we have a file or a dir
				file = new File(args[i]);
				if (file.isFile()) files.add(file);
				else if (file.isDirectory()) {
					DirectoryParser dirparser = new DirectoryParser(clientOptions.isRecurseDir());
					files.addAll(dirparser.scanDirectory(file));
				} else 
					printError("\""+args[i]+"\" is not a file, directory or option, skipping...");
			}
		}
		
		hashingOptions.setSeeDebug(seeDebug);
		parsingOptions.setSeeDebug(seeDebug);
		clientOptions.setSeeDebug(seeDebug);
		parsingOptions.setParserImp(avparsing.AVParser.ParserImplementations.XUGGLE);
		if (saveHashingSettings) hashingOptions.saveSettings(appDir);
		if (saveParsingSettings) parsingOptions.saveSettings(appDir);
		if (saveClientSettings) clientOptions.saveSettings(appDir);
		
		System.out.println("Found "+files.size()+" file(s)");
		if (files.size() > 0) {
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
				parser = new FileParser(file,hashingOptions,parsingOptions,clientOptions);
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
				if (clientOptions.getWriteFileOuputFormat() == XaucOptions.OutputFormats.XML) {
					try {
						afile.writeXml();
					} catch (Exception e) {
						printError("some error occured while writing \""+file.getName()+"\" xml file.");
						log.println(e.getLocalizedMessage());
					}
				} else if (clientOptions.getWriteFileOuputFormat() == XaucOptions.OutputFormats.TXT) {
					try {
						afile.writeText();
					} catch (Exception e) {
						printError("some error occured while writing \""+file.getName()+"\" txt file.");
						log.println(e.getLocalizedMessage());
					}
				}
				if (clientOptions.getStdoutFileOuputFormat() == XaucOptions.OutputFormats.XML) {
					afile.writeXMLtoConsole();
				} else if (clientOptions.getStdoutFileOuputFormat() == XaucOptions.OutputFormats.TXT) {
					afile.writeTXTtoConsole();
				}
			}
		}
	}
}

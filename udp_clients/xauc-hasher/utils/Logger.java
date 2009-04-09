package utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Logger {
	/** Logger Options */
	public int LogOptions = DEFAULT;
	/** If this text area is defined it will be used as the default output channel, otherwise STDOUT will be the default */
	protected static javax.swing.JTextArea outputTextArea = null;
	/** Log file */
	public PrintStream logFile = null;
	/** New line character */
	protected static String newline = System.getProperty("line.separator");
	// WRITE Options
	/** Write to the default output channel */
	public static final int DEFAULT = 1;
	/** Write to the standart output */
	public static final int STDOUT = 2;
	/** Write to the standart error */
	public static final int STDERR = 4;
	/** Write to the log file */
	public static final int LOG = 8;
	/** Pre-append the current datetime to messages */
	public static final int DATE = 16;
	/** Append a new line character */
	public static final int NEWLINE = 32;
	
	/** Creates a default Logger object */
	public Logger() {}
	/**
	 * Creates a Logger object
	 * @param file File handle of the log file
	 */
	public Logger(File file) {
		try {
			this.logFile = new PrintStream(file);
		} catch (FileNotFoundException e) {
			System.err.println(e.getLocalizedMessage());
			//e.printStackTrace();
		}
	}
	/**
	 * Creates a Logger object
	 * @param filename File name of the log file
	 */
	public Logger(String filename) {
		this(new File(filename));
	}
	/**
	 * Creates a Logger object
	 * @param file File handle of the log file
	 * @param logToConsole Tells if we should write to console
	 */
	public Logger(File file, boolean logToConsole) {
		this(file);
		LogOptions = LogOptions|STDOUT;
	}
	/**
	 * Creates a Logger object
	 * @param filename File name of the log file
	 * @param logToConsole Tells if we should write to console
	 */
	public Logger(String filename, boolean logToConsole) {
		this(filename);
		LogOptions = LogOptions|STDOUT;
	}
	
	/**
	 * Method that writes a message to the output medium and the log file (without a trailing \n)
	 * @param message Message to write
	 * @param options Write Options (see above, default is 0)
	 */
	public synchronized void print(String message, int options) {
		if ((options & NEWLINE) == NEWLINE)
			message += newline;
		if ((options & DATE) == DATE)
			message = "["+(new SimpleDateFormat("dd.MM.yyyy kk:mm:ss").format(new Date()))+"] "+message;
		if ((options & STDOUT) == STDOUT)
			System.out.print(message);
		if ((options & STDERR) == STDERR)
			System.err.print(message);
		if ((options & LOG) == LOG)
			logFile.print(message);
		if ((options & DEFAULT) == DEFAULT) { // should allways be true
			if (outputTextArea != null)
				outputTextArea.append(message);
			else
				System.out.print(message);
		}
	}
	
	/**
	 * Method that writes a message to the output medium and the log file (without a trailing \n)
	 * @param message Message to write
	 * @param options Write Options (see above, default is 0)
	 */
	public synchronized void print(String message) {
		print(message, LogOptions);
	}
	
	/**
	 * Method that writes a message to the output medium and the log file (with a trailing \n)
	 * @param message Message to write
	 * @param options Write Options (see above, default is 0)
	 */
	public synchronized void println(String message, int options) {
		print(message, options|NEWLINE);
	}
	
	/**
	 * Method that writes a message to the output medium and the log file (with a trailing \n)
	 * @param message Message to write
	 */
	public synchronized void println(String message) {
		print(message, LogOptions|NEWLINE);
	}
}

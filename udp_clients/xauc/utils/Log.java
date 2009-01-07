package utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Logger class
 */
public class Log {
	protected PrintStream out;
	protected boolean logToConsole = false;
	
	public Log() { this.out = System.out; }
	public Log(File file) {
		try {
			this.out = new PrintStream(file);
		} catch (FileNotFoundException e) {
			System.err.println(e.getLocalizedMessage());
			//e.printStackTrace();
		}
	}
	public Log(String filename) {
		this(new File(filename));
	}
	public Log(File file, boolean logToConsole) {
		this(file);
		this.logToConsole = logToConsole;
	}
	public Log(String filename, boolean logToConsole) {
		this(filename);
		this.logToConsole = logToConsole;
	}
	
	/**
	 * Appends the current date time to the log (without a trailing new line)
	 */
	public void printDateTime() {
		String message = "["+(new SimpleDateFormat("dd.MM.yyyy kk:mm:ss").format(new Date()))+"] ";
		print(message);
	}
	
	/**
	 * Appends a message in the text log (without a trailing new line)
	 * @param message Message to append
	 */
	public void print(String message) {
		if (logToConsole) System.out.print(message);
		out.print(message);
	}

	/**
	 * Appends a message in the log (with a trailing new line)
	 * @param message Message to append
	 */
	public void println(String message) {
		message = "["+(new SimpleDateFormat("dd.MM.yyyy kk:mm:ss").format(new Date()))+"] "+message;
		if (logToConsole) System.out.println(message);
		out.println(message);
	}
}

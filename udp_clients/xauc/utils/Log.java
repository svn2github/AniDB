package utils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintStream;

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
	 * Prints a message in the log (without a new line) at end
	 * @param message Message to print
	 */
	public void print(String message) {
		if (logToConsole) System.out.print(message);
		out.print(message);
	}

	/**
	 * Prints a message in the log (with a new line) at end
	 * @param message Message to print
	 */
	public void println(String message) {
		if (logToConsole) System.out.println(message);
		out.println(message);
	}
}

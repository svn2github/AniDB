package utils;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.JTextArea;

/**
 * Logger class but with extra support for Swing
 */
public class SwingLog extends Log {
	protected JTextArea logArea = null;
	
	public SwingLog() { super(); }
	public SwingLog(File file) { super(file); }
	public SwingLog(String filename) { super(filename); }
	public SwingLog(File file, boolean logToConsole) { super(file,logToConsole); }
	public SwingLog(String filename, boolean logToConsole) { super(filename, logToConsole); }
	
	/** @return the logArea */
	public synchronized JTextArea getLogArea() { return logArea; }
	/** @param logArea the logArea to set */
	public synchronized void setLogArea(JTextArea logArea) { this.logArea = logArea; }

	/**
	 * Appends a message in the text log and at the LogArea (if any) (without a trailing new line)
	 * @param message Message to append
	 */
	public void print(String message) {
		if (logToConsole) System.out.print(message);
		out.print(message);
		if (logArea != null) logArea.append(message);
	}

	/**
	 * Appends a message in the log and at the LogArea (if any) (with a trailing new line)
	 * @param message Message to append
	 */
	public void println(String message) {
		message = "["+(new SimpleDateFormat("dd.MM.yyyy kk:mm:ss").format(new Date()))+"] "+message;
		if (logToConsole) System.out.println(message);
		out.println(message);
		if (logArea != null) logArea.append(message+"\n");
	}
}

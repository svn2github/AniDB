package utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;

/**
 * A recursive directory parser
 */
public class DirectoryParser {
	protected ArrayList<String> extensions;
	protected String extensionFilter = "";
	protected boolean recurseDir = false;
	
	public DirectoryParser() {
		this.extensions = new ArrayList<String>();
		loadExtensions();
	}
	public DirectoryParser(boolean recurseDir) {
		this();
		this.recurseDir = recurseDir;
	}
		
	public ArrayList<String> getExtensions() { return this.extensions; }
	public String getExtensionFilter() { return this.extensionFilter; }
	
	/**
	 * Loads the extensions for filtering files
	 */
	public void loadExtensions() {
		String extFile = System.getProperty("user.home") + File.separator + ".xauc" + File.separator + "extensions.txt";
		File file = new File(extFile);
		BufferedReader in;
		try {
			in = new BufferedReader(new FileReader(file));
			String line;
			while ((line = in.readLine()) != null) {
				if (line.charAt(0) == '#') continue; // comments
				this.extensions.add(line);
			}
			for (int i = 0; i < extensions.size(); i++) {
				this.extensionFilter += extensions.get(i);
				if (i < extensions.size() - 1) this.extensionFilter += '|';
			}
			in.close();
		} catch (FileNotFoundException e) {
			this.extensionFilter = "avi|mkv|ogm|mp4|wmv";
		} catch (IOException e) {
			System.err.println(e.getLocalizedMessage());
			e.printStackTrace();
		}
	}
	
	/**
	 * Method that recursively scans a directory for files with matching extensions
	 * @param directory The directory to scan
	 * @return An ArrayList with all the found files
	 */
	public synchronized ArrayList<File> scanDirectory(String directory) {
		ArrayList<File> retfiles = new ArrayList<File>();
		File curdir = new File(directory);
		FilenameFilter filefilter = new filesFilter(extensionFilter);
		FilenameFilter dirfilter = new directoryFilter();
		ArrayList<File> directories = new ArrayList<File>();
		directories.add(curdir);
		while (directories.size() > 0) {
			File dir = directories.remove(0);
			File[] files = dir.listFiles(filefilter);
			for (int i = 0; i < files.length; i++) retfiles.add(files[i]);
			if (this.recurseDir) {
				File[] dirs = dir.listFiles(dirfilter);
				for (int i = 0; i < dirs.length; i++) directories.add(dirs[i]);
			}
		}
		return retfiles;
	}
	
	/**
	 * Method that recursively scans a directory for files with matching extensions
	 * @param directory The directory to scan
	 * @return An ArrayList with all the found files
	 */	
	public ArrayList<File> scanDirectory(File dir) {
		return scanDirectory(dir.getAbsolutePath());
	}
	
	/**
	 * File filters, based upon loaded extensions
	 */
	protected class filesFilter implements FilenameFilter {
		protected String extensionFilter = "";
		public filesFilter() {};
		public filesFilter(String extensionFilter) {
			this.extensionFilter = extensionFilter;
		}
		public boolean accept(File dir, String filename) {
			int i = filename.lastIndexOf(".");
			if(i<0) return false;
			String extension = filename.substring(i+1).toLowerCase();
			if (this.extensionFilter.indexOf(extension) < 0) return false;
			return true;
		}
	}
	
	/**
	 * Directory filters
	 */
	protected class directoryFilter implements FilenameFilter {
		public boolean accept(File dir, String filename) {
			File file = new File(dir.getAbsoluteFile()+File.separator+filename);
			return file.isDirectory();
		}
	}
}

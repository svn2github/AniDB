package clients;

import java.io.File;

import options.HasherOptions;
import options.SwingOptionsEditor;

import utils.Logger;
import utils.Progress;

import hashing.Hasher;

public class Tester {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		if (args.length < 1) {
			System.err.println("Not enough arguments:\nappname filename");
			System.exit(1);
		}
		String filename = args[0];
		File file = new File(filename);
		Logger log = new Logger();
		Progress progress = new Progress(0,"hashing");
		
		/** Application directory */
		String appDir = System.getProperty("user.home") + File.separator + ".xauc";
		
		HasherOptions hasherOptions = new HasherOptions();
		if (!(new File(appDir+File.separator+hasherOptions.serFilename).exists())) 
			hasherOptions.saveSettings(appDir);
		else
			hasherOptions.loadSettings(appDir);
		
		final SwingOptionsEditor editor = new SwingOptionsEditor();
		editor.setAppDir(appDir);
		
		java.awt.EventQueue.invokeLater(new Runnable() {
            public void run() {
            	editor.setVisible(true);
            }
        });
		
		int num_tests = 1;
		for (int i = 0; i < num_tests; i++) {
			Hasher hasher = new Hasher(file);
			hasher.setLog(log);
			hasher.setProgress(progress);
			hasher.work();
			System.out.println("ed2k: "+hasher.ed2k);
			System.out.println("crc32: "+hasher.crc32);
			System.out.println("md5: "+hasher.md5);
			System.out.println("sha1: "+hasher.sha1);
			System.out.println("tth: "+hasher.tth);
		}
/*	
		System.out.println("completed hashing of \""+file.getName()+"\" in "+cputime+"ms @"+Utils.calculateProcessingRate(file.length(),cputime));
		System.out.println("ed2k: "+hasher.ed2k);
		System.out.println("crc32: "+hasher.crc32);
		System.out.println("md5: "+hasher.md5);
		System.out.println("sha1: "+hasher.sha1);
		System.out.println("tth: "+hasher.tth);
*/
	}

}

import hashing.Hasher;
import avparsing.AVparser;
import utils.*;
import xml.XmlObjectException;
import structures.*;

import java.io.File;
import java.io.IOException;
import java.security.NoSuchAlgorithmException;

public class Xauc {

	/**
	 * @param args
	 * @throws IOException 
	 * @throws NoSuchAlgorithmException 
	 * @throws XmlObjectException 
	 */
	public static void main(String[] args) throws NoSuchAlgorithmException, IOException, XmlObjectException {
		if (args.length < 1) { System.out.println("No file"); return; }

		File file = new File(args[0]);
		Log log = new Log("log.txt");
		Progress progress = new Progress();
/*
		Hasher filehasher = new Hasher(file);
		filehasher.setLog(log);
		progress.setAction("hashing");
		progress.setShowProgress(true);
		filehasher.setProgress(progress);
		System.out.println("Started hashing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		filehasher.startWork();
		
		AniDBFile afile = new AniDBFile(file,filehasher);
		afile.writeXml();
*/
		AVparser avparser = new AVparser(file);
		avparser.setLog(log);
		progress.setAction("parsing");
		progress.setShowProgress(false);
		avparser.setProgress(progress);
		avparser.startWork();
/*
		long start = System.currentTimeMillis();
		DirectoryParser parser = new DirectoryParser();
		ArrayList<File> files = parser.scanDirectory("D:/Anime");
		System.out.println("Found "+files.size()+" files in "+(System.currentTimeMillis() - start)+"ms");
*/
		return;
	}

}

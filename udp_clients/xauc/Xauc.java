import hashing.Hasher;
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
		Hasher filehasher = new Hasher(file);
		filehasher.setLog(new Log("log.txt")); //"log.txt"
		Progress progress = new Progress();
		progress.setAction("hashing");
		progress.setShowProgress(true);
		filehasher.setProgress(progress);
		System.out.println("Started hashing of \""+file.getName()+"\" ["+file.length()+" bytes]");
		filehasher.startWork();
		
		AniDBFile afile = new AniDBFile(file,filehasher);
		afile.writeXml();
/*
		long start = System.currentTimeMillis();
		DirectoryParser parser = new DirectoryParser();
		ArrayList<File> files = parser.scanDirectory("D:/Anime");
		System.out.println("Found "+files.size()+" files in "+(System.currentTimeMillis() - start)+"ms");
*/
		return;
	}

}

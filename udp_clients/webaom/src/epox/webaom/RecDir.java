/*
 * Created on 1. des.. 2007 13.30.11
 * Filename: RecDir.java
 */
package epox.webaom;

import java.io.File;

import epox.swing.Log;

public class RecDir extends Thread{
	private static boolean mBcrw; // cancel rec workr
	public static void kill(){
		mBcrw = true;
	}
	private File[] dirs;
	private int nd = 0;
	private boolean print;
	private final Log log;

	public RecDir(File[] d, boolean hdir, Log l){
		super("RecDir");
		dirs = d;
		log = l;
		mBcrw = false;
		print = !hdir && !A.dio.isEnabled();
	}
	public void run(){
		long t0 = System.currentTimeMillis();
		//setEnabled(false);
		int nr=0;
		for(int i=0; i<dirs.length; i++){
			String parent = dirs[i].getParent();
			if(parent!=null && parent.startsWith("\\\\"))
				log.add("Windor network paths not supported: "+dirs[i]);
			else
				nr += addFileRecursive(dirs[i]);
		}
		if(print){
			String s = "";
			if(nr==1)
				s = "Added one file in "+(System.currentTimeMillis()-t0)+ " ms.";
			else if(nr>1)
				s = "Added "+nr+" files in "+(System.currentTimeMillis()-t0)+ " ms.";
			log.add(Log.STD, s);
			if(s.length()>0)
				log.add(s);
		}
		//setEnabled(true);
		Thread.yield();
		A.mWdiv = null;
	}
	private int addFileRecursive(File file){
		if(mBcrw) return 0;
		if(file.isDirectory()){
			if(print && (nd++)%100==0) log.add(Log.STD, "Checking: "+file);
			int nr=0;
			File[] files = file.listFiles(A.fha.m_ff);
			if(files==null) return 0;
			for(int i=0; i<files.length; i++)
				nr+=addFileRecursive(files[i]);
			return nr;
		}
		if(A.fha.addFile(file)) return 1;
		return 0;
	}
}

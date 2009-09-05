// Copyright (C) 2005-2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

/*
 * Created on 22.01.05
 *
 * @version 	05
 * @author 		epoximator
 */
package epox.webaom;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.text.DecimalFormat;

import jonelo.jacksum.algorithm.Edonkey;
import epox.av.AVInfo;
import epox.util.HashContainer;

public class DiskIO implements Runnable {
	private static final String S_OUTS = "There is not enough space on the disk";
	private static final DecimalFormat _DF = new DecimalFormat("0.00");
	private static final int I_BSIZ = 1048576*3;
	private static final byte[] m_buf = new byte[I_BSIZ];
	private static String mSfail, mSabrt, mSsucc;

	private HashContainer m_hc;

	public DiskIO(){
		String s0 = "Cleanup after ";
		String s1 = " moving operation.";
		mSfail = s0+"failed"+s1;
		mSabrt = s0+"aborted"+s1;
		mSsucc = s0+"successful"+s1;
	}
	public void run(){
		A.gui.setEnabled_dio(false);
		Job j = null;
		try{
			m_hc = A.gui.jpOdiv.getHashContainer();
			A.gui.status0("DiskIO thread started.");
			while(A.gui.dioOK() && (j=A.jobs.getJobDio())!=null){
				//!A.nr_dio = j.mIid;
				switch(j.getStatus()){
				case Job.HASHWAIT: fileHash(j); break;
				case Job.MOVEWAIT: fileMove(j); break;
				case Job.PARSEWAIT: fileParse(j); break;
				default: A.dialog("INF LOOP", "Illegal status: "+j.getStatusText()); A.gui.kill();
				}
				//!A.nr_dio = -1;
				A.gui.jpb0.setValue(0);
			}
			A.gui.status0("DiskIO thread terminated.");
		}catch(IOException e){ e.printStackTrace();
			if(j!=null){
				if(j.m_fn!=null)
					A.deleteFileAndFolder(j.m_fn, mSfail);
				JobMan.updateStatus(j, Job.FAILED);
				j.setError(e.getMessage());
				//A.gui.updateJobTable(j);
			}

			String msg = e.getMessage();
			A.gui.println(Hyper.error(msg));
			A.gui.status0(msg);
			if(A.gui.dioOK())
				A.gui.dioToggle();
		}
		//!A.nr_dio = -1;
		A.gui.mWdio = null;
		A.gui.setEnabled_dio(true);
	}
	private void fileParse(Job j) throws IOException{
		if(!AVInfo.ok()){
			JobMan.updateStatus(j, Job.FAILED);
			return;
		}
		File file = j.getFile();
		JobMan.updateStatus(j, Job.PARSING);
		//A.gui.updateJobTable(j);
		A.gui.status0("Parsing "+file.getName());//+" (#"+(j.mIid+1)+")");
		A.gui.jpb0.setValue(0);

		AVInfo av = new AVInfo(file);
		long t0 = System.currentTimeMillis();
		float p = av.parse();
		while(A.gui.dioOK() && p>=0){
			A.gui.jpb0.setValue((int)(10*p));
			p = av.parse();
		}
		if(p<0)
			j.m_fi = av.build();
		av.close();
		long t1 = System.currentTimeMillis();
		A.gui.jpb0.setValue(1000);
		A.gui.println("Parsed "+Hyper.name(file)+" @ "+stats(file.length(), (t1-t0)/1000f));
		JobMan.updateStatus(j, Job.FINISHED);
		//A.gui.updateJobTable(j);
	}
	private void fileHash(Job j) throws IOException{
		File file = j.getFile();
		j.mLs = file.length();
		if(j.mLs<1){
			JobMan.updateStatus(j, Job.FAILED);
			j.setError("File size less than 1.");
			//A.gui.updateJobTable(j);
			return;
		}
		int last_read;
		long tot_read = 0, len = file.length();
		float prog = 0;

		JobMan.updateStatus(j, Job.HASHING);
		//A.gui.updateJobTable(j);
		A.gui.status0("Hashing "+file.getName());//+" (#"+(j.mIid+1)+")");
		A.gui.jpb0.setValue(0);

		long t0 = System.currentTimeMillis();
		InputStream in = new FileInputStream(file);
		while (A.gui.dioOK() && (last_read = in.read(m_buf)) != -1){
			m_hc.update(m_buf, 0, last_read);
			tot_read +=last_read;
			prog = (float)tot_read/len;
			A.gui.jpb0.setValue((int)(1000*prog));
		}
		in.close();
		m_hc.finalize();
		long t1 = System.currentTimeMillis();
		A.gui.jpb0.setValue(0);

		if(prog==1){
			j._ed2 = m_hc.getHex("ed2k");
			j._md5  = m_hc.getHex("md5");
			j._sha = m_hc.getHex("sha1");
			j._tth  = m_hc.getHex("tth");
			j._crc= m_hc.getHex("crc32");

			String link = "ed2k://|file|"+file.getName()+"|"+file.length()+"|"+j._ed2+"|";

			A.gui.printHash(link);
			A.gui.printHash(m_hc.toString());

			A.gui.println("Hashed "+Hyper.name(file)+" @ "+stats(file.length(), (t1-t0)/1000f));
			JobMan.updateStatus(j, Job.HASHED);
		} else JobMan.updateStatus(j, Job.HASHWAIT);
		//A.gui.updateJobTable(j);
	}
	private void fileMove(Job j) throws IOException{
		if(j.m_fc.equals(j.m_fn)){
			j.m_fn = null;
			JobMan.updateStatus(j, Job.MOVED);
			//A.gui.updateJobTable(j);
		}
		if(!j.m_fc.exists()){
			JobMan.updateStatus(j, Job.FAILED);
			j.setError("File does not exist.");
			//A.gui.updateJobTable(j);
			A.gui.println(Hyper.error("File "+j.m_fc+" does not exist!"));
			return;
		}
		if(!j.m_fc.canRead()){
			System.out.println("! Cannot read file: "+j.m_fc);
			JobMan.updateStatus(j, Job.FAILED);
			j.setError("File can not be read.");
			return;
		}
		File parent = j.m_fn.getParentFile();
		if(!parent.exists() && !parent.mkdirs()){
			String s = "Folder "+parent+" cannot be created!";
			j.setError(s);
			A.gui.println(Hyper.error(s));
			return;
		}
		JobMan.updateStatus(j, Job.MOVING);
		//A.gui.updateJobTable(j);
		A.gui.status0("Moving "+j.m_fc.getName());

		long t0 = System.currentTimeMillis();
		boolean copy = !j.m_fn.exists();
		if(copy && !fileCopy(j.m_fc, j.m_fn)){
			A.deleteFileAndFolder(j.m_fn, mSabrt);
			JobMan.updateStatus(j, Job.MOVEWAIT);
			//A.gui.updateJobTable(j);
			return;
		}

		JobMan.updateStatus(j, Job.MOVECHECK);
		//A.gui.updateJobTable(j);
		A.gui.status0("Checking "+j.m_fc.getName());
		String sum = fileCheck(j.m_fn);
		if(sum==null){//canceled
			if(copy)
				A.deleteFileAndFolder(j.m_fn, mSabrt);
			if(!A.gui.dioOK()) JobMan.updateStatus(j, Job.MOVEWAIT);
		}else if(j._ed2.equalsIgnoreCase(sum)){
			A.gui.println("Moved "+Hyper.name(j.m_fc)+" to "+Hyper.name(j.m_fn)+" @ "+
						stats(j.m_fc.length(), (System.currentTimeMillis()-t0)/1000f));
			A.deleteFileAndFolder(j.m_fc, mSsucc);
			JobMan.setJobFile(j, j.m_fn);
			j.m_fn = null;
			j.mIdid = -1;
			JobMan.updateStatus(j, Job.MOVED);
		}else{
			JobMan.updateStatus(j, Job.FAILED);
			if(copy){
				j.setError("CRC check failed on copy. HW problem?");
				A.gui.println(Hyper.error(j.m_fc+" was not moved! CRC check failed. HW problem?"));
			}else{
				j.setError("CRC check failed on copy. Destination file does already exist, but with wrong CRC. Handle this manually.");
				A.gui.println(Hyper.error(j.m_fc+" was not moved! Destination file '"+j.m_fn+"' does already exist, but with wrong CRC. Handle this manually."));
			}
		}
		//A.gui.updateJobTable(j);
	}
	private String stats(long len, float time){
		return	Hyper.number(_DF.format(len/time/1048576))+" MB/s ("+
				Hyper.number(len+"")+ " bytes in "+
				Hyper.number(_DF.format(time))+" seconds)";
	}
	private boolean fileCopy(File a, File b) throws IOException{
		int num_read;
		long tot_read = 0, len = a.length();
		float prog = 0;

		InputStream fi = new FileInputStream(a);
		OutputStream fo = new FileOutputStream(b);

		A.gui.jpb0.setValue(0);
		try{
			while (A.gui.dioOK() && (num_read = fi.read(m_buf)) != -1){
				fo.write(m_buf, 0, num_read);
				tot_read += num_read;
				prog = (float)tot_read/len;
				A.gui.jpb0.setValue((int)(1000*prog));
			}
			fo.close();
			fi.close();
		}catch(IOException e){
			if(e.getMessage().equals(S_OUTS))
				A.dialog("IOException", S_OUTS+":\n"+b);
			else throw e;
		}
		return prog==1;
	}
	private String fileCheck(File f) throws IOException{
		Edonkey ed;
		try{
			ed = new Edonkey();
		}catch(java.security.NoSuchAlgorithmException e){
			e.printStackTrace();
			return null;
		}
		int num_read;
		long tot_read = 0, len = f.length();
		float prog = 0;
		InputStream in = new FileInputStream(f);
		A.gui.jpb0.setValue(0);
		while (A.gui.dioOK() && (num_read = in.read(m_buf)) != -1){
			ed.update(m_buf, 0, num_read);
			tot_read += num_read;
			prog = (float)tot_read/len;
			A.gui.jpb0.setValue((int)(1000*prog));
		}
		in.close();
		if(prog<1) return null;
		//return crc.getHexValue();
		return ed.getHexValue();
	}
}
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
import epox.swing.Log;
import epox.util.HashContainer;
import epox.util.Hyper;
import epox.util.U;
import epox.webaom.Job;
import epox.webaom.JobMan;

public class DiskIO extends Service {
	private static final String S_OUTS = "There is not enough space on the disk";
	private static final DecimalFormat _DF = new DecimalFormat("0.00");
	private static final int I_BSIZ = 1048576*3;
	private static final byte[] m_buf = new byte[I_BSIZ];
	private static String mSfail, mSabrt, mSsucc;

	private HashContainer m_hc;
	public void setHashCotainer(HashContainer hc){
		m_hc = hc;
	}

	public boolean setEnabled(boolean enable){
		if(enable){
			if(thread==null){
				thread = new Thread(this, "DiskIO");
				thread.start();
				enabled = true;
			}
		}else if(enabled){
			enabled = false;
		}
		return enabled;
	}
	public void kill(){
		setEnabled(false);
		if(thread!=null) try{
			thread.join(1000);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	public DiskIO(){
		String s0 = "Cleanup after ";
		String s1 = " moving operation.";
		mSfail = s0+"failed"+s1;
		mSabrt = s0+"aborted"+s1;
		mSsucc = s0+"successful"+s1;
	}
	public void run(){
		status.started();
		Job j = null;
		try{
			log.add(Log.STD|Log.LOG, "DiskIO thread started.");
			while(isEnabled()){
				j = A.jobs.getJobDio();
				if(j==null){
					log.add(Log.STD, "DiskIO thread idle");
					Thread rw = new RecDir(A.getDirs(), true, log);
					rw.start();
					try{
						rw.join();
						if(!A.jobs.workForDio())
							for(int i=0; i<40 && isEnabled(); i++)
								Thread.sleep(100);
					}catch(InterruptedException e){
						e.printStackTrace();
					}
				}else{
					switch(j.getStatus()){
					case Job.HASHWAIT: fileHash(j); break;
					case Job.MOVEWAIT: fileMove(j); break;
					case Job.PARSEWAIT: fileParse(j); break;
					default: log.add(Log.MSG, "Illegal status: "+j.getStatusText());
						A.stop();
					}
					progress.set(0);
				}
			}
			log.add(Log.STD|Log.LOG, "DiskIO thread terminated.");
		}catch(IOException e){
			e.printStackTrace();
			if(j!=null){
				if(j.m_fn!=null)
					U.deleteFileAndFolder(j.m_fn, mSfail);
				JobMan.updateStatus(j, Job.FAILED);
				j.setError(e.getMessage());
			}
			log.add(Log.LOG|Log.STD, Hyper.error(e.getMessage()));
		}
		thread = null;
		enabled = false;
		status.finished();
	}
	private void fileParse(Job j) throws IOException{
		if(!AVInfo.ok()){
			JobMan.updateStatus(j, Job.FAILED);
			return;
		}
		File file = j.getFile();
		JobMan.updateStatus(j, Job.PARSING);
		log.add(Log.STD, "Parsing "+file.getName());//+" (#"+(j.mIid+1)+")");
		progress.set(0);

		AVInfo av = new AVInfo(file);
		long t0 = System.currentTimeMillis();
		float p = av.parse();
		while(isEnabled() && p>=0){
			progress.set(p/100);
			p = av.parse();
		}
		if(p<0)
			j.m_fi = av.build();
		av.close();
		long t1 = System.currentTimeMillis();
		progress.set(1);
		log.add("Parsed "+Hyper.name(file)+" @ "+stats(file.length(), (t1-t0)/1000f));
		JobMan.updateStatus(j, Job.FINISHED);
	}
	private void fileHash(Job j) throws IOException{
		File file = j.getFile();
		j.mLs = file.length();
		if(j.mLs<1){
			JobMan.updateStatus(j, Job.FAILED);
			j.setError("File size less than 1.");
			return;
		}
		int last_read;
		long tot_read = 0, len = file.length();
		float prog = 0;

		JobMan.updateStatus(j, Job.HASHING);
		log.add(Log.STD, "Hashing "+file.getName());//+" (#"+(j.mIid+1)+")");
		progress.set(0);

		long t0 = System.currentTimeMillis();
		InputStream in = new FileInputStream(file);
		while (isEnabled() && (last_read = in.read(m_buf)) != -1){
			m_hc.update(m_buf, 0, last_read);
			tot_read +=last_read;
			prog = (float)tot_read/len;
			progress.set(prog);
		}
		in.close();
		m_hc.finalize();
		long t1 = System.currentTimeMillis();
		progress.set(0);

		if(prog==1){
			j._ed2 = m_hc.getHex("ed2k");
			j._md5  = m_hc.getHex("md5");
			j._sha = m_hc.getHex("sha1");
			j._tth  = m_hc.getHex("tth");
			j._crc= m_hc.getHex("crc32");

			String link = "ed2k://|file|"+file.getName()+"|"+file.length()+"|"+j._ed2+"|";

			log.add(Log.SPE, link);
			log.add(Log.SPE, m_hc.toString());

			log.add("Hashed "+Hyper.name(file)+" @ "+stats(file.length(), (t1-t0)/1000f));
			JobMan.updateStatus(j, Job.HASHED);
		} else JobMan.updateStatus(j, Job.HASHWAIT);
		//A.gui.updateJobTable(j);
	}
	private void fileMove(Job j) throws IOException{
		if(j.m_fc.equals(j.m_fn)){
			j.m_fn = null;
			JobMan.updateStatus(j, Job.MOVED);
		}
		if(!j.m_fc.exists()){
			JobMan.updateStatus(j, Job.FAILED);
			j.setError("File does not exist.");
			log.add(Hyper.error("File "+j.m_fc+" does not exist!"));
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
			log.add(Hyper.error(s));
			return;
		}
		JobMan.updateStatus(j, Job.MOVING);
		log.add(Log.STD, "Moving "+j.m_fc.getName());

		long t0 = System.currentTimeMillis();
		boolean copy = !j.m_fn.exists();
		if(copy && !fileCopy(j.m_fc, j.m_fn)){
			U.deleteFileAndFolder(j.m_fn, mSabrt);
			JobMan.updateStatus(j, Job.MOVEWAIT);
			return;
		}

		JobMan.updateStatus(j, Job.MOVECHECK);
		log.add(Log.STD, "Checking "+j.m_fc.getName());
		String sum = fileCheck(j.m_fn);
		if(sum==null){//canceled
			if(copy)
				U.deleteFileAndFolder(j.m_fn, mSabrt);
			if(!isEnabled()) JobMan.updateStatus(j, Job.MOVEWAIT);
		}else if(j._ed2.equalsIgnoreCase(sum)){
			log.add("Moved "+Hyper.name(j.m_fc)+" to "+Hyper.name(j.m_fn)+" @ "+
						stats(j.m_fc.length(), (System.currentTimeMillis()-t0)/1000f));
			U.deleteFileAndFolder(j.m_fc, mSsucc);
			JobMan.setJobFile(j, j.m_fn);
			j.m_fn = null;
			j.mIdid = -1;
			JobMan.updateStatus(j, Job.MOVED);
		}else{
			JobMan.updateStatus(j, Job.FAILED);
			if(copy){
				j.setError("CRC check failed on copy. HW problem?");
				log.add(Hyper.error(j.m_fc+" was not moved! CRC check failed. HW problem?"));
			}else{
				j.setError("CRC check failed on copy. Destination file does already exist, but with wrong CRC. Handle this manually.");
				log.add(Hyper.error(j.m_fc+" was not moved! Destination file '"+j.m_fn+"' does already exist, but with wrong CRC. Handle this manually."));
			}
		}
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

		progress.set(0);
		try{
			while (isEnabled() && (num_read = fi.read(m_buf)) != -1){
				fo.write(m_buf, 0, num_read);
				tot_read += num_read;
				prog = (float)tot_read/len;
				progress.set(prog);
			}
			fo.close();
			fi.close();
		}catch(IOException e){
			if(e.getMessage().equals(S_OUTS))
				log.add(Log.MSG, S_OUTS+":\n"+b);
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
		progress.set(0);
		while (isEnabled() && (num_read = in.read(m_buf)) != -1){
			ed.update(m_buf, 0, num_read);
			tot_read += num_read;
			prog = (float)tot_read/len;
			progress.set(prog);
		}
		in.close();
		if(prog<1) return null;
		//return crc.getHexValue();
		return ed.getHexValue();
	}
}
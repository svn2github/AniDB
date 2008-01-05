/*
 * Created on 1. des.. 2007 15.02.04
 * Filename: Service.java
 */
package epox.webaom;

import epox.swing.Log;
import epox.swing.Progress;
import epox.swing.Status;

public abstract class Service implements Runnable{
	protected Thread thread = null;
	protected boolean enabled = false;
	protected Log log;
	protected Status status;
	protected Progress progress;

	public void setLog(Log l){
		log = l;
	}
	public void setStatus(Status s){
		status = s;
	}
	public void setProgress(Progress p){
		progress = p;
	}
	public boolean isEnabled(){
		return enabled;
	}
	public abstract boolean setEnabled(boolean b);
}

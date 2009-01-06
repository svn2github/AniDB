package utils;

/**
 * This class implements a threaded work.<br />
 * Extend this class, do not use it is just the framework
 */
public abstract class ThreadedWorker implements Runnable {
	protected volatile boolean enabled = false;
	public volatile boolean suspended = false;
	public volatile Thread myThread = null;
	protected String threadName = "worker";
	protected boolean seeDebug = false;
	protected long suspendInterval = 1000;

	/** @return the enabled */
	public synchronized boolean isEnabled() { return enabled; }
	/** @param enabled the enabled to set */
	public synchronized boolean setEnabled(boolean enabled) {
		if (seeDebug) System.out.println("setting enabled state of "+this.threadName+" thread to: "+enabled);
		if(enabled){
			if(myThread==null){
				myThread = new Thread(this, threadName);
				this.enabled = true;
				myThread.start();
			}
		} else if(enabled){
			this.enabled = false;
		}
		return this.enabled;
	}
	/** @return the suspended */
	public synchronized boolean isSuspended() { return suspended; }
	/** @param suspended the suspended to set */
	public synchronized void setSuspended(boolean suspended) {
		if (seeDebug) System.out.println("setting suspended state of "+this.threadName+" thread to: "+enabled);
		this.suspended = suspended; 
	}
	/** @return the myThread */
	public synchronized Thread getMyThread() { return myThread; } 
	/** @return the threadName */
	public synchronized String getThreadName() { return threadName;	}
	/** @param threadName the threadName to set */
	public synchronized void setThreadName(String threadName) { this.threadName = threadName; }
	/** @return the seeDebug */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/** @param seeDebug the seeDebug to set */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/** @return the suspendInterval */
	public synchronized long getSuspendInterval() {	return suspendInterval; }
	/** @param suspendInterval the suspendInterval to set */
	public synchronized void setSuspendInterval(long suspendInterval) { this.suspendInterval = suspendInterval; }
	
	/** 
	 * Class work goes here.<br />
	 * Should do a <i>checkForSuspend()</i> periodicaly.
	 * @see checkForSuspend
	 */
	public abstract void work();

	public synchronized void run() {
		if (myThread == null) {
			if (seeDebug) System.err.println("trying to start execution without starting thread.");
			return; // stopped before started.
		}
		while(isEnabled()) { // execute code
			if (seeDebug) System.out.println("starting execution of "+this.threadName+" thread");
			work(); // do the work
			this.enabled = false;
		}
		this.myThread = null;
	}

	/** Used to periodicaly check for thread suspension */
	public synchronized void checkForSuspend() {
		Thread.yield(); // let another thread have some time perhaps to stop this one.
		if (!isSuspended()) return;
		while (isSuspended()) {
			try {
				wait(suspendInterval);
				Thread.yield();
			} catch (InterruptedException e) {
				Thread.currentThread().interrupt();
			}
		}
	}
	
	/**
	 * Pauses execution of the thread.
	 * <p><b>note:</b> Thread has to be awaken externaly by using Thread.notify()</p>
	 */
	public synchronized void pauseThread() {
		if (myThread == null) return; // no thread to work
		myThread.interrupt();
	}

	/** Stops a thread */
	public synchronized void stopThread() {
		Thread tmpThread = myThread;
		myThread = null;
		if (tmpThread != null) tmpThread.interrupt();
	}
	
	/**
	 * Checks if the running thread is interrupted.
	 * @return Thread interrupted state
	 */
	public synchronized boolean isInterrupted() {
		if (myThread == null) return true;
		return this.myThread.isInterrupted();
	}
}

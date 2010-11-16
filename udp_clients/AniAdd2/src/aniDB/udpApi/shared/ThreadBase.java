/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public abstract class ThreadBase<T> {
	private Thread t;
	protected T api;
	protected boolean terminate;

	public ThreadBase(T api) {
		t = createThread();
		this.api = api;
	}

	public void terminate(){
		try {
			terminate = true;
			if(t.getState() != Thread.State.NEW) t.join();
		} catch(InterruptedException ex) {
		}
	}

	public void Start() {
		terminate = false;

		switch(t.getState()){
			case NEW:
				t.start();
				break;
			case TERMINATED:
				t = createThread();
				t.start();
				break;
		}
	}
	public boolean isAlive(){
		return t == null? false : t.isAlive();
	}

	private Thread createThread(){
		return new Thread(new Runnable() {
			public void run() {
				DoWork();
			}
		}, this.toString());
	}

	protected abstract void DoWork();
}

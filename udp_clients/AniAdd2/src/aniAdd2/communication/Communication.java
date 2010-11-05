/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.communication;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Arokh
 */
public abstract class Communication implements ICommunication {
	private final Object eventSyncRoot = new Object();
	private List<IComListener> listeners;

	public Communication() {
		listeners = new ArrayList<IComListener>();

	}

	public void addComListener(IComListener comListener) {
		synchronized(eventSyncRoot) {
			listeners.add(comListener);
		}
	}

	public void removeComListener(IComListener comListener) {
		synchronized(eventSyncRoot) {
			listeners.remove(comListener);
		}
	}

	protected void comFire(ComEvent comEvent) {
		for(IComListener listener : listeners) listener.eventHandler(comEvent);
	}

	protected abstract void parentEventHandler(ComEvent comEvent);
}

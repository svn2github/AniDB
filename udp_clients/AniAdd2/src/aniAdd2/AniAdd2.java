/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2;

import aniAdd2.communication.ComEvent;
import aniAdd2.communication.IComListener;
import aniAdd2.exts.IAA2Extension;
import aniAdd2.exts.gui.AA2GUI;
import aniAdd2.exts.processing.AA2Processing;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author Arokh
 */
public class AniAdd2 implements IAniAdd2 {
	private final Object eventSyncRoot = new Object();
	private List<IComListener> listeners;
	private ExtensionListener extensionListener;
	private List<IAA2Extension> extensions;

	public AniAdd2() {
		listeners = new ArrayList<IComListener>();

		extensionListener = new ExtensionListener();
		extensions = new ArrayList<IAA2Extension>();

		IAA2Extension ext = new AA2GUI();
		ext.addComListener(extensionListener);
		extensions.add(ext);

		ext = new AA2Processing();
		ext.addComListener(extensionListener);
		extensions.add(ext);

	}

	public void initialize() {
		for(IAA2Extension ext : extensions) {
			System.out.println("Initializing: " + ext.getName());
			ext.initialize(this);
		}

		boolean allModsInitialized = false;
		do {
			allModsInitialized = true;
			for(IAA2Extension ext : extensions) allModsInitialized &= ext.getState() == IAA2Extension.State.Initialized;
			if(!allModsInitialized) ThreadUtil.Sleep(100);
		} while(!allModsInitialized);

		comFire(new ComEvent(this, ComEvent.ComType.Information, IAA2Extension.State.Initialized));
	}

	public IAA2Extension getExtension(Class extensionClass) {
		for(IAA2Extension ext : extensions) if(ext.getClass() == extensionClass) return ext;
		return null;
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

	public Iterator<IAA2Extension> iterator() {
		return extensions.iterator();
	}

	private void comFire(ComEvent comEvent) {
		System.out.println("AniAdd Event: " + comEvent.toString());
		for(IComListener listener : listeners) listener.eventHandler(comEvent);

	}

	private class ExtensionListener implements IComListener {
		public void eventHandler(ComEvent comEvent) {
			//TODO
		}
	}
}

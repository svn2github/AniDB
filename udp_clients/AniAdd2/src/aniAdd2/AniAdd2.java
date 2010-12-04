/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2;

import aniAdd2.communication.ComEvent;
import aniAdd2.communication.IComListener;
import aniAdd2.exts.AA2Extension;
import aniAdd2.exts.IAA2Extension;
import aniAdd2.exts.StateComEvent;
import aniAdd2.exts.gui.AA2GUI;
import aniAdd2.exts.processing.AA2Processing;
import aniAdd2.exts.storage.AA2Storage;
import aniAdd2.exts.udpApi.AA2AniDBUdpClient;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map.Entry;

/**
 *
 * @author Arokh
 */
public class AniAdd2 {
	private final Object eventSyncRoot = new Object();
	private HashMap<IAA2Extension, IComListener> listeners;
	private ExtensionListener extensionListener;
	private List<IAA2Extension> extensions;

	public AniAdd2() throws Exception {
		listeners = new HashMap<IAA2Extension, IComListener>();
		extensionListener = new ExtensionListener();
		extensions = new ArrayList<IAA2Extension>();

		IAA2Extension ext = new AA2GUI();
		ext.addComListener(extensionListener);
		extensions.add(ext);

		ext = new AA2Processing();
		ext.addComListener(extensionListener);
		extensions.add(ext);

		ext = new AA2Storage();
		ext.addComListener(extensionListener);
		extensions.add(ext);

		ext = new AA2AniDBUdpClient();
		ext.addComListener(extensionListener);
		extensions.add(ext);
	}

	public void initialize() {
		for(IAA2Extension ext : extensions) {
			System.out.println("Initializing: " + ext.getName());
			ext.initialize(new AniAdd2Sub(ext));
		}

		boolean allModsInitialized = false;
		do {
			allModsInitialized = true;
			for(IAA2Extension ext : extensions) allModsInitialized &= ext.getState() == IAA2Extension.State.Initialized;
			if(!allModsInitialized) ThreadUtil.Sleep(100);
		} while(!allModsInitialized);

		comFire(new StateComEvent(this, IAA2Extension.State.Initialized));
	}

	public IAA2Extension getExtension(Class extensionClass) {
		for(IAA2Extension ext : extensions) if(ext.getClass() == extensionClass) return ext;
		return null;
	}

	private void addComListener(IAA2Extension owner,IComListener comListener) {
		synchronized(eventSyncRoot) { listeners.put(owner, comListener); }
	}

	private void removeComListener(IAA2Extension owner, IComListener comListener) {
		synchronized(eventSyncRoot) { if(listeners.get(owner) == comListener) listeners.remove(owner); }
	}

	private Iterator<IAA2Extension> iterator() { return extensions.iterator(); }

	private void comFire(ComEvent comEvent) {
		System.out.println("AniAdd Event: " + comEvent.toString());
		for(IComListener listener : listeners.values()) listener.eventHandler(comEvent);
	}
	private void sendMessage(IAA2Extension sender, IAA2Extension receiver, Object arg) {
		if(listeners.containsKey(receiver)) {
			listeners.get(receiver).eventHandler(new MessageComEvent(this, sender, arg));
		}
	}

	private class ExtensionListener implements IComListener {
		public void eventHandler(ComEvent comEvent) {
			//TODO
		}
	}

	public class AniAdd2Sub implements IAniAdd2 {
		private IAA2Extension extension;

		private AniAdd2Sub(IAA2Extension extension) { this.extension = extension; }

		public IAA2Extension getExtension(Class extensionClass) { return AniAdd2.this.getExtension(extensionClass); }
		public void addComListener(IComListener comListener) { AniAdd2.this.addComListener(extension, comListener); }
		public void removeComListener(IComListener comListener) { AniAdd2.this.removeComListener(extension, comListener); }
		public void sendMessage(IAA2Extension receiver, Object arg) { AniAdd2.this.sendMessage(extension, receiver, arg); }
		public Iterator<IAA2Extension> iterator() { return AniAdd2.this.iterator(); }
	}
}

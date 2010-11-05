/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.gui;

import aniAdd2.ThreadUtil;
import aniAdd2.communication.ComEvent;
import aniAdd2.communication.IComListener;
import aniAdd2.exts.AA2Extension;
import aniAdd2.exts.gui.exts.IAA2GUIExtension;
import aniAdd2.exts.gui.exts.processing.AA2GUIProcessing;
import aniAdd2.exts.gui.notify.Notify;
import java.awt.Component;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Arokh
 */
public class AA2GUI extends AA2Extension {
	private ExtensionListener extensionListener;
	private List<IAA2GUIExtension> extensions;

	private MainUserInterface gui;

	public AA2GUI() {
		extensionListener = new ExtensionListener();
		extensions = new ArrayList<IAA2GUIExtension>();

		IAA2GUIExtension ext = new AA2GUIProcessing();
		ext.addComListener(extensionListener);
		extensions.add(ext);
	}



	public Notify addNotify(String message, Notify.NotifyType type){
		return gui.getNotifyManager().add(message, type);
	}

	public Component getComponent(){
		return gui;
	}

	@Override
	public String getName() {
		return "AniAdd2 GUI";
	}

	@Override
	protected void initializeInternal() {
		gui = new MainUserInterface();

		for(IAA2GUIExtension ext : extensions) {
			System.out.println("Initializing: GUIExt(" + ext.getName() + ")");
			ext.initialize(aniAdd2);
		}

		boolean allModsInitialized = false;
		do {
			allModsInitialized = true;
			for(IAA2GUIExtension ext : extensions) allModsInitialized &= ext.getState() == IAA2GUIExtension.State.Initialized;
			if(!allModsInitialized) ThreadUtil.Sleep(100);
		} while(!allModsInitialized);

		Component component;
		for(IAA2GUIExtension ext : extensions) {
			component = ext.getComponent(IAA2GUIExtension.Location.MainTabControl);
			if(component != null) gui.addTab(IAA2GUIExtension.Location.MainTabControl, ext.getName(), component);

			component = ext.getComponent(IAA2GUIExtension.Location.Options);
			if(component != null) gui.addTab(IAA2GUIExtension.Location.Options, ext.getName(), component);

			component = ext.getComponent(IAA2GUIExtension.Location.Help);
			if(component != null) gui.addTab(IAA2GUIExtension.Location.Help, ext.getName(), component);
		}
		
		setState(State.Initialized);
	}

	@Override
	protected void parentEventHandler(ComEvent comEvent) {
	}

	private class ExtensionListener implements IComListener {
		public void eventHandler(ComEvent comEvent) {
			//TODO
		}
	}
}

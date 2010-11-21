/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.exts.processing;

import aniAdd2.communication.ComEvent;
import aniAdd2.exts.gui.exts.AA2GUIExtension;
import java.awt.Component;

/**
 *
 * @author Arokh
 */
public class AA2GUIProcessing extends AA2GUIExtension {
	private ProcessingGUITab tab;
	private ProcessingGUIOptions options;
	private ProcessingGUIHelp help;

	@Override
	protected void initializeInternal() {
		tab = new ProcessingGUITab(aniAdd2);
		options = new ProcessingGUIOptions();
		help = new ProcessingGUIHelp();

		//TODO
		setState(State.Initialized);
	}

	@Override
	protected void parentEventHandler(ComEvent comEvent) {
		//TODO
	}

	public String getName() {
		return "File Processing";
	}

	public Component getComponent(Location location) {
		switch(location) {
			case MainTabControl:
				return tab;
			case Options:
				return options;
			case Help:
				return help;
			default:
				return null;
		}
	}

}

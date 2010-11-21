/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.storage;

import aniAdd2.communication.ComEvent;
import aniAdd2.exts.AA2Extension;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author Arokh
 */
public class AA2Storage extends AA2Extension {
	private Map<String, Object> storage;

	public AA2Storage() {
		storage = new HashMap<String, Object>();
	}

	
	
	@Override
	protected void initializeInternal() {
		setState(State.Initialized);
	}

	@Override
	protected void parentEventHandler(ComEvent comEvent) {
	}

	public String getName() { return "Storage"; }

	public Settings getSettings(String extName) {
		return new Settings(extName, storage);
	}

}

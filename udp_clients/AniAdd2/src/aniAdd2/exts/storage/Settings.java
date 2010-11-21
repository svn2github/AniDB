/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.storage;

import java.util.Map;

/**
 *
 * @author Arokh
 */
public final class Settings {
	private String extName;
	private Map<String, Object> storage;

	public Settings(String extName, Map<String, Object> storage) {
		this.extName = extName;
		this.storage = storage;
	}

	public <T> T get(String key) {
		return (T)storage.get(extName + "/" + key);
	}
	public <T> T get(String key, T defValue) {
		return storage.containsValue(extName + "/" + key) ? (T)storage.get(extName + "/" + key) : defValue;
	}

	public boolean set(String key, Object value) {
		if(key.contains("/")) return false;
		storage.put(extName + "/" + key, value);
		return true;
	}
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.requests;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

/**
 *
 * @author Arokh
 */
public abstract class Cmd implements Iterable<Entry<String, String>> {
	private boolean isFinal;

	protected String tag;
	protected String action;
	protected Map<String, String> args;

	public Cmd() {
		args = new HashMap<String, String>();
	}

	public Iterator<Entry<String, String>> iterator() {
		return args.entrySet().iterator();
	}

	public String getTag() { return tag; }
	public boolean setTag(String tag) {
		if(isFinal) return false;
		this.tag = tag;
		return true;
	}
	
	public String getAction() { return action; }
	public boolean setAction(String action) {
		if(isFinal) return false;
		this.action = action;
		return true;
	}

	public boolean add(String key, String value){
		if(isFinal) return false;
		args.put(key, value);
		return true;
	}
	public String get(String key) {
		return args.get(key);
	}

	public void setFinal() { isFinal = true; }
	public boolean isFinal() { return isFinal; }
}

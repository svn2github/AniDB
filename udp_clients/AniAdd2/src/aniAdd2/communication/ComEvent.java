/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.communication;

import aniAdd2.ClassUtil;
import aniAdd2.exts.IAA2Extension;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.EventObject;
import java.util.List;

/**
 *
 * @author Arokh
 */
public class ComEvent extends EventObject {
	private Date createdOn;
	private ComType type;
	private List<Object> params;

	public ComEvent(Object source, ComType type, ArrayList<Object> params) {
		this(source, type, params.toArray());
	}

	public ComEvent(Object source, ComType type, Object... params) {
		this(source, type);

		this.params = Arrays.asList(params);
	}

	public ComEvent(Object source, ComType type) {
		this(source);
		this.type = type;
	}

	private ComEvent(Object source) {
		super(source);
		this.createdOn = new Date();
	}

	public ComType getType() {
		return type;
	}

	public Object getParam(int i) {
		return params.get(i);
	}

	public int getParamCount() {
		return params.size();
	}

	@Override
	public String toString() {
		String str;
		str = (new SimpleDateFormat("HH:mm:ss")).format(createdOn) + " " + type + ": " + ClassUtil.getClassName(getSource());
		for(int i = 0; i < getParamCount(); i++) str += " " + getParam(i);

		return str;
	}

	public enum ComType {
		Debug, Information, Manipulation, Warning, Error, Fatal
	}
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.replies;

import aniDB.udpApi.shared.ReplyCodes;

/**
 *
 * @author Arokh
 */
public abstract class Reply {
	private boolean isFinal;
	private ReplyCodes code;
	private String message;
	private String tag;
	private String[] data;


	protected Reply() { };


	public void setFinal() { isFinal = true; }
	public boolean isFinal() { return isFinal; }

	public ReplyCodes getCode() {
		return code;
	}

	public boolean setCode(ReplyCodes code) {
		if(isFinal) return false;
		this.code = code;
		return true;
	}

	public String getMessage() {
		return message;
	}

	public boolean setMessage(String message) {
		if(isFinal) return false;
		this.message = message;
		return true;
	}

	public String getTag() {
		return tag;
	}

	public boolean setTag(String tag) {
		if(isFinal) return false;
		this.tag = tag;
		return true;
	}

	public String[] getData() {
		return (String[])data.clone();
	}

	public boolean setData(String[] data) {
		if(isFinal) return false;
		this.data = (String[])data.clone();
		return true;
	}

	@Override
	public String toString() {
		String replyStr = (tag != null ? tag + " " : "") + code.getCode() + " " + message;
		if(data != null) for(String entry : data) replyStr += "\n" + entry;
		return replyStr + "\n";
	}
}
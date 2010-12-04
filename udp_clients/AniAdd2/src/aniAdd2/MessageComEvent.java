/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2;

import aniAdd2.communication.ComEvent;
import aniAdd2.exts.IAA2Extension;

/**
 *
 * @author Arokh
 */
public class MessageComEvent extends ComEvent {
	private IAA2Extension sender;
	private Object arg;

	public MessageComEvent(Object source, IAA2Extension sender, Object arg) {
		super(source);
		this.arg = arg;
		this.sender = sender;
	}

}

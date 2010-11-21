/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts;

import aniAdd2.communication.ComEvent;

public class StateComEvent extends ComEvent {
	private IAA2Extension.State state;

	public StateComEvent(Object source, IAA2Extension.State state) {
		super(source);
		this.state = state;
	}

	public IAA2Extension.State getState() {
		return state;
	}
}

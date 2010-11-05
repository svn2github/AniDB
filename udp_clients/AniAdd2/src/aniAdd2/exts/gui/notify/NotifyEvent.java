/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.notify;

import java.util.EventObject;

/**
 *
 * @author Arokh
 */
public class NotifyEvent extends EventObject {
	public NotifyEvent(Notify notify) {
		super(notify);
	}
	
	public Notify getNotify(){
		return (Notify)getSource();
	}
}

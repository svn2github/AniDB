/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.communication;

import java.util.Date;
import java.util.EventObject;

/**
 *
 * @author Arokh
 */
public abstract class ComEvent extends EventObject {
	private Date createdOn;

	public ComEvent(Object source) {
		super(source);
		this.createdOn = new Date();
	}
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts;

import aniAdd2.IAniAdd2;
import aniAdd2.communication.ICommunication;

/**
 *
 * @author Arokh
 */
public interface IAA2Extension extends ICommunication {
	String getName();

	void initialize(IAniAdd2 aniAdd2);

	State getState();

	public static enum State { New, Initializing, Initialized, Terminating, Terminated }
}

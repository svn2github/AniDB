/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.communication;

import java.util.EventListener;

/**
 *
 * @author Arokh
 */
public interface IComListener extends EventListener {
	void eventHandler(ComEvent comEvent);
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.communication;

/**
 *
 * @author Arokh
 */
public interface ICommunication {
	void addComListener(IComListener comListener);
	void removeComListener(IComListener comListener);
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public interface IAction<T> {
	void invoke(T param);
}
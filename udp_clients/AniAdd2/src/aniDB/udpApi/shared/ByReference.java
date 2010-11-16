/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public class ByReference<T> {
	private T param;


	public void setParam(T param){
		this.param = param;
	}
	public T getParam(){
		return param;
	}

}

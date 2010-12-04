/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2;

/**
 *
 * @author Arokh
 */
public class ThreadUtil {
	public static void Sleep(int ms){
		try {
			Thread.sleep(ms);
		} catch(InterruptedException ex) {
			System.err.println("Sleep was interrupted");
		}
	}
}

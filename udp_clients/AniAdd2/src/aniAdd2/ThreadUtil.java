/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2;

import java.util.logging.Level;
import java.util.logging.Logger;

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

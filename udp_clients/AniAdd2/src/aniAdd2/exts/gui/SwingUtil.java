/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.gui;

import javax.swing.SwingUtilities;

/**
 *
 * @author Arokh
 */
public class SwingUtil {
	public static void invokeAndWait(Runnable r, String errorMsg) {
			if(SwingUtilities.isEventDispatchThread()) {
				r.run();
			} else {
				try {
					SwingUtilities.invokeAndWait(r);
				} catch(Exception e) {
					System.err.println(errorMsg);
				}
			}
	}
}

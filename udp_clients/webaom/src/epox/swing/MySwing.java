/*
 * Created on 29. juni. 2007 23.40.57
 * Filename: MyUtils.java
 */
package epox.swing;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Rectangle;

public class MySwing {
	public static void centerComponent(Component c){
		Dimension d = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
		Rectangle r = c.getBounds();
		c.setBounds(d.width/2-r.width/2, d.height/2-r.height/2, r.width, r.height);
	}
}

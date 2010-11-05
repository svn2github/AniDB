/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.exts;

import aniAdd2.exts.IAA2Extension;
import java.awt.Component;

/**
 *
 * @author Arokh
 */
public interface IAA2GUIExtension extends IAA2Extension {
	Component getComponent(Location location);

	public static enum Location {
		MainTabControl, Help, Options
	}
}

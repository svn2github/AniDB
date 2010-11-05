/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2;

/**
 *
 * @author Arokh
 */
public class ClassUtil {
	public static String getClassName(Object c) {
		String fullClassName = c.getClass().getName();
		int firstChar;
		firstChar = fullClassName.lastIndexOf('.') + 1;
		if(firstChar > 0) {
			fullClassName = fullClassName.substring(firstChar);
		}
		return fullClassName;
	}
}

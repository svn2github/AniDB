/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

import java.util.Date;

/**
 *
 * @author Arokh
 */
public class DateUtil {
	public static TimeSpan substract(Date d1, Date d2) {
		return new TimeSpan(d1.getTime() - d2.getTime());
	}
}

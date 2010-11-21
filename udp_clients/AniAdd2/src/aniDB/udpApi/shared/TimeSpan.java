/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public class TimeSpan implements Comparable<TimeSpan> {
	private long milliseconds;

	public TimeSpan(long milliseconds) {
		this.milliseconds = milliseconds;
	}

	public int compareTo(TimeSpan o) {
		return o.milliseconds > milliseconds ? -1 : o.milliseconds != milliseconds ? 0 : 1;
	}

	public int getTotalSeconds() {
		throw new UnsupportedOperationException("Not yet implemented");
	}

}

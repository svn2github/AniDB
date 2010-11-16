/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 *
 * @author Arokh
 */
public class IdGenerator {
	private static final Random rnd = new Random();
	private static final List<Long> usedIds = new ArrayList<Long>();

	public static long getUniqueNumber() {
		int index;
		Long id;
		synchronized(usedIds) {
			id = rnd.nextLong();
			while((index = binarySearch(usedIds, id)) >= 0) id = (id + 1) % Long.MAX_VALUE;
			usedIds.add(~index, id);
		}
		return id;
	}

	private static int binarySearch(List<Long> sorted, Long key) {
		int first = 0;
		int upto  = sorted.size();

		while (first < upto) {
			int mid = (first + upto) / 2;  // Compute mid point.
			if (key.compareTo(sorted.get(mid)) < 0) {
				upto = mid;       // repeat search in bottom half.
			} else if (key.compareTo(sorted.get(mid)) > 0) {
				first = mid + 1;  // Repeat search in top half.
			} else {
				return mid;       // Found it. return position
			}
		}
		return -(first + 1);      // Failed to find key
	}


	private List<Long> ids;

	public IdGenerator() {
		ids = new ArrayList<Long>();
	}
	public boolean isValid(long id, boolean freeId) { return freeId ? ids.remove(id) : ids.contains(id); }
	public long generate(){
		long id = getUniqueNumber();
		ids.add(~binarySearch(ids, id), id);
		return id;
	}
}
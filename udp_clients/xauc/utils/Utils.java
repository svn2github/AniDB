package utils;

import java.util.BitSet;

/**
 * Utility class
 */
public class Utils {
	/**
	 * Method that calculates the parsing rate in MB/s
	 * @param filesize File size (in bytes)
	 * @param time Hashing time (in ms)
	 * @return String with parsing rate
	 */
	public static String calculateProcessingRate(long filesize, long time) {
		double sizeInMBs = filesize / 1000000;
		double timeInSecs = time / 1000;
		double rate = sizeInMBs / timeInSecs;
		if (Double.isInfinite(rate) || Double.isNaN(rate)) return "n/a MB/s";
		return rate+" MB/s";
	}

	/**
	 * Formats a duration time in seconds to an hh:mm:ss representation
	 * @param time The time
	 * @return Formated time
	 */
	public static String formatDurationSecs(double time) {
		String seconds = Integer.toString((int)(time % 60));
		String minutes = Integer.toString((int)((time % 3600) / 60));
		String hours = Integer.toString((int)(time / 3600));
		for (int i = 0; i < 2; i++) {
			if (seconds.length() < 2) seconds = "0" + seconds;
			if (minutes.length() < 2) minutes = "0" + minutes;
			if (hours.length() < 2) hours = "0" + hours;
		}
		return hours+":"+minutes+":"+seconds;
	}

	/**
	 * Formats a duration time in nanoseconds to an hh:mm:ss representation
	 * @param timeMillis The time
	 * @return Formated time
	 */
	public static String formatDuration(long timeMillis) {
		long time = timeMillis / 1000000;
		return formatDurationSecs(time);
	}	

	/**
	 * Used to check if a bitmask contains a particular value
	 * @param bitmask The bitmask to check
	 * @param value The value to check
	 * @return true if bitmask contains value, false otherwise
	 */
	public static boolean checkBitmask(int bitmask, int value) {
		return ((bitmask & value) == value);
	}

	/**
	 * Converts a Byte Array to a String (also creates a new string in the process)
	 * @param array The Byte Array to convert
	 * @return Converted String
	 */
	public static String convertByteArray(byte[] array) {
		String string = "";
		for (int i = 0; i < array.length; i++) if ((int) array[i] != 0) string += (char)array[i];
		return string;
	}

	/**
	 * Checks if a possible number in a string is indeded a number 
	 * @param s String where possible number lies
	 * @return true if possible number is a number, false otherwise
	 */
	public static final boolean isNumber(final String s) {
		try {
			Integer.parseInt(s);
			return true;
		} catch (Exception exception) {
			return false;
		}
	}

	/**
	 * Checks if a number is contained in a number array 
	 * @param Numbers The number array
	 * @param Number The number to check for containment
	 * @return true if number is contained, false otherwise
	 */
	public static boolean ContainsNumber(int[] Numbers, int Number) {
		for (int Num : Numbers)
			if (Num == Number) return true;
		return false;
	}

	/**
	 * Sum the bits in a given bit set and return a long with the result
	 * @param BA The Bitset
	 * @return The sum
	 */
	public static long SumBits(BitSet BA) {
		long Sum = 0;
		for (int I = BA.nextSetBit(0); I >= 0; I = BA.nextSetBit(I+1)) {
			Sum = Sum + (long)Math.pow((double)2,(double)I);
		}
		return Sum;
	}

	/**
	 * Converts a given bit set to Hexadecimal
	 * @param BA Bitset
	 * @return The hexadecimal representation of the bitset
	 */
	public static String ToHex(BitSet BA) {
		String Hex = "";
		int HexPart;

		for (int I = 0; I < BA.size(); I=I+4) {
			HexPart = 0;
			for (int J = 0; J < 4; J++) {
				HexPart += BA.get(I+J)?0:1 * (int)Math.pow(2, J);
			}
			Hex += Integer.toHexString(HexPart).toUpperCase();
		}
		return Hex;
	}

	/**
	 * Returns a bitset from a given Integer
	 * @param b The Integer
	 * @return A bitset
	 */
	public static BitSet GetBits(int b) {
		BitSet Bits = new BitSet(8);

		int I = 7;
		while (b > 0) {
			if (b != (b = (byte)(b % Math.pow(2, I)))) {
				Bits.set(I);
			}
			I--;
		}
		return Bits;
	}
}

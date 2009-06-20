/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package aniAdd.misc;

import java.io.File;
import java.util.ArrayList;
import java.util.BitSet;

public class Misc {

    public static final boolean isNumber(final String s) {
        try {
            Integer.parseInt(s);
            return true;
        } catch (Exception exception) {
            return false;
        }
    }

    public static boolean containsNumber(int[] Numbers, int Number) {
        for (int Num : Numbers) {
            if (Num == Number) {
                return true;
            }
        }

        return false;
    }

    // Get All Files from Folder & SubFolders
    public static ArrayList<File> getFiles(File dir, String[] filter) {
        ArrayList<File> Files = new ArrayList<File>();
        if (dir.isDirectory()) {
            if (dir.listFiles() != null) {
                for (File child : dir.listFiles()) {
                    Files.addAll(getFiles(child, filter));
                }
            }
        } else if (isVideoFile(dir, filter)) {
            Files.add(dir);
        }
        return Files;
    }

    public static boolean isVideoFile(File file, String[] filter) {
        for (String s : filter) {
            if (file.getName().toLowerCase().endsWith("." + s)) {
                return true;
            }
        }
        return false;
    }

    public static long sumBits(BitSet BA) {
        long Sum = 0;
        for (int I = BA.nextSetBit(0); I >= 0; I = BA.nextSetBit(I + 1)) {
            Sum = Sum + (long) Math.pow((double) 2, (double) I);
        }
        return Sum;
    }

    public static String toMask(BitSet ba, int length) {
        String hex = "";
        byte hexPart;

        for (int I = 0; I < length; I += 4) {
            hexPart = 0;
            for (int J = 0; J < 4; J++) {
                hexPart += ba.get(I + J) ? (1 << J) : 0;
            }
            hex = Integer.toHexString(hexPart).toUpperCase() + hex;
        }
        return hex.substring(6, 8) + hex.substring(4, 6) + hex.substring(2, 4) + hex.substring(0, 2);
    }

    public static BitSet getBits(int b) {
        BitSet Bits = new BitSet(8);

        int I = 7;
        while (b > 0) {
            if (b != (b = (byte) (b % Math.pow(2, I)))) {
                Bits.set(I);
            }
            I--;
        }

        return Bits;
    }

    public static String stringPadding(String str, int size, char padChar) {
        StringBuffer padded = new StringBuffer(str);
        while (padded.length() < size) {
            padded.insert(0, padChar);
        }
        return padded.toString();
    }

    public static String longToTime(long time) {
        long s, m, h;
        time = time % (24 * 60 * 60 * 1000);
        s = time / 1000;
        h = s / 3600;
        s -= h * 3600;
        m = s / 60;
        s -= m * 60;
        return stringPadding(h + "", 2, '0') + ":" + stringPadding(m + "", 2, '0') + ":" + stringPadding(s + "", 2, '0');
    }

    public static int binarySearch(ArrayList<Integer> a, int x) {
        int low = 0;
        int high = a.size() - 1;
        int mid=0;

        while (low <= high) {
            mid = (low + high) / 2;

            if (a.get(mid) < x) {
                low = mid + 1;
            } else if (a.get(mid) > x) {
                high = mid - 1;
            } else {
                return mid;
            }
        }
        return mid ^ -1;
    }
}


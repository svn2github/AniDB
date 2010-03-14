/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package aniAdd.misc;

import java.io.File;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Date;

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
        int hexPart;

        for (int i = 0; i < length; i += 8) {
            hexPart = 0;
            for (int j = 0; j < 8; j++) hexPart += ba.get(i + j) ? (1 << j) : 0;
            hex += stringPadding(Integer.toHexString(hexPart).toUpperCase(), 2, '0');
        }
        return hex;
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
        time = (time % (24 * 60 * 60 * 1000))/1000;

        s = time % 60;
        time = (time-s)/60;

        m = time % 60;
        time = (time-m)/60;

        h = time;

        return stringPadding(h + "", 2, '0') + ":" + stringPadding(m + "", 2, '0') + ":" + stringPadding(s + "", 2, '0');
    }

    public static String DateToString(Date date){
        DateFormat dateFormat = new SimpleDateFormat("HH:mm:ss");
        return dateFormat.format(date);
    }

    public static String DateToString(Date date, String format){
        DateFormat dateFormat = new SimpleDateFormat(format);
        return dateFormat.format(date);

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


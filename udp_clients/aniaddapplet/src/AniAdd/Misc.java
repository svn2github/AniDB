/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package AniAdd;

import Processing.EpProcessing;
import java.io.File;
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
    
    public static boolean ContainsNumber(int[] Numbers, int Number) {
        for (int Num : Numbers) {
            if (Num == Number) return true;
        }
        
        return false;
    }
    
    // Get All Files from Folder & SubFolders
    public static java.util.ArrayList<java.io.File> GetFiles(java.io.File dir) {
        java.util.ArrayList<java.io.File> Files = new java.util.ArrayList<java.io.File>();
        if (dir.isDirectory()) {
            if(dir.listFiles()!=null)
                for(File child:dir.listFiles())
                    Files.addAll(GetFiles(child));
        } else if(EpProcessing.isVideoFile(dir)){
            Files.add(dir);
        }
        return Files;        
    }
    
    public static long SumBits(BitSet BA) {
        long Sum = 0;
        for (int I = BA.nextSetBit(0); I >= 0; I = BA.nextSetBit(I+1)) {
            Sum = Sum + (long)Math.pow((double)2,(double)I);
        }
        return Sum;
    }
    
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

/**
    public static String PageContact(String PageURL, Map<String, String> Params) {
        String PostData = "";
        byte[] PostBinData;
   
        try {
            for (Map.Entry<String, String> Param : Params.entrySet()) {
                PostData += "&" + Param.getKey() + "=" + java.net.URLEncoder.encode(Param.getValue(), "UTF-8");
            }            
            PostBinData = PostData.getBytes("UTF-8");
            
            URLConnection URLCon;
            URLCon = new URL(PageURL).openConnection();
            URLCon.setDoOutput(true);
            URLCon.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
            
            URLCon.getOutputStream().write(PostBinData);
            URLCon.getOutputStream().flush();
            
                
            InputStreamReader input = new InputStreamReader(URLCon.getInputStream(), "UTF-8");
            
            int I;
            char[] str = new char[64];
            String PageContent = "";
            while (( I=input.read(str) ) != -1) PageContent += new String(Arrays.copyOf(str, I));
            
            input.close();
            
            return PageContent;
            
        } catch (Exception exception) {
            exception.printStackTrace();
            return "";
        }
    }
*/
}


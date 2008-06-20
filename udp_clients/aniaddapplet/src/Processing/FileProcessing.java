/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package Processing;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.BitSet;

public class FileProcessing {
    AniAdd.AniAdd CC;
    Thread InfoSource;
    
    public FileProcessing(AniAdd.AniAdd CallingClass) {
        CC = CallingClass;
    }
    
    public void ProcessFile(File ProcFile,String Fash) {
        if (Fash != null) {
            /*
            GetFromWeb Web = new GetFromWeb();
            Web.Fash = Fash;
            Web.ProcFile = ProcFile;
            
            InfoSource = new Thread(Web, "FromWeb");
            InfoSource.start();
            */
        } else {
            GetFromParser Parser = new GetFromParser();
            Parser.ProcFile = ProcFile;
            
            InfoSource = new Thread(Parser,"FromParser");
            InfoSource.start();
        }
    }

/**
    class GetFromWeb implements Runnable {
        public File ProcFile;
        public String Fash;
        
        String URL = "http://arokh.kilu.de/Programs/AniAdd/FileInfo.php";
        Map<String, String> URLParams = new TreeMap<String, String>();
        
        public void run() {
            URLParams.put("Action", "Get");
            URLParams.put("UID", CC.API.UserInfo.UserName);
            URLParams.put("Fash", Fash);
            URLParams.put("Client", CC.API.ConInfo.ClientID);
            URLParams.put("ClientVer", String.valueOf(CC.API.ConInfo.ClientVer));
            URLParams.put("Size", Long.toHexString(ProcFile.length()));
            
            String URLContent[] = AniAdd.Misc.PageContact(URL, URLParams).split("\\|");
            
            if (URLContent[0].equals("Info") && !URLContent[1].equals("Not Found")) {
                CC.EpProc.ContinueProcessing(ProcFile, URLContent[1], "Web");
            } else {
                ProcessFile(ProcFile, null);
            }
            
            System.out.print("Web Response: ");
            for (String Str : URLContent) System.out.print(Str + " ");
            System.out.println();
        }
    }
*/
    class GetFromParser implements Runnable {
        public File ProcFile;
        Ed2kHasher.Edonkey Ed2k;

        public void run() {
            try {
                Ed2k = new Ed2kHasher.Edonkey();
                
                int num_read, Iteration=0;
                byte[] m_buf = new byte[1048576*3];
                InputStream in = new FileInputStream(ProcFile);
                while ((num_read = in.read(m_buf)) != -1){
                    while (CC.EpProc.IsPaused) {try {Thread.sleep(500);} catch (Exception exception) {}}
                    //System.out.println(Iteration + " * " + m_buf.length + " / " + ProcFile.length() + " * 100 = " + (int)(((double)Iteration * (double)m_buf.length) / (double)ProcFile.length() * 100));
                    CC.EpProc.processedMB += num_read/1024/1024;
                    CC.EpProc.UpdateProgress((int)(((double)Iteration++ * (double)m_buf.length) / (double)ProcFile.length() * 100));
                    Ed2k.update(m_buf, 0, num_read);
                }
                in.close();
            } catch (Exception exception) {exception.printStackTrace();}
            
            String Ed2kHexVal = Ed2k.getHexValue();
            CC.EpProc.ContinueProcessing(ProcFile, Ed2kHexVal, "Parser");
      }
    }

    public static BitSet GenFastFileID(File HashFile) {
        java.io.FileInputStream FileIO;
        byte[] Bytes = new byte[7];
        BitSet Bits;
        BitSet ID = new BitSet(64);
        int SubID;
                
        try {
            FileIO = new java.io.FileInputStream(HashFile);
            
            for (int I=0; I < 64; I++) {
                FileIO.read(Bytes);
                
                SubID = 0;
                for (byte b : Bytes){
                    Bits = AniAdd.Misc.GetBits(b & 0xFF);
                    SubID += Bits.cardinality();
                }
                
                if (SubID > 64 / 2) {
                    ID.set(I);
                }
                
                FileIO.skip((long)Math.round((HashFile.length() / 64 - 8)));
                //Damn .NET for its wierd CLng() rounding (Incompatible to AniTracker on rare cases)
            }
            
            FileIO.close();
        } catch (Exception exception) {exception.printStackTrace();}
        
        return ID;
    }
}

/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package Processing;

import AniAdd.*;
import java.io.File;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.BitSet;
import java.util.Collection;

public class EpProcessing {
    AniAdd CC;
    public java.util.ArrayList<cFileInfo> Files = new java.util.ArrayList<cFileInfo>();
    boolean IsProcessing = false;
    boolean IsPaused = false;
    
    public cUser UserSettings = new cUser();
    
    //FIXME: This is a bad idea on its own! (Better to hash a few too many
    //files than too few)
    private static String[] supportedFiles={"avi", "mpg", "mpeg", "rm", "rmvb",
	"asf", "wmv", "mov", "ogm", "mp4", "mkv", "rar", "zip", "ace", "srt",
	"sub", "ssa", "smi", "idx", "ass", "txt", "swf", "flv"};
    //Check if this file is a video file by looking at extension
    public static boolean isVideoFile(File file){
        for(String s:EpProcessing.supportedFiles)
            if(file.getName().toLowerCase().endsWith(s))
                return true;
        return false;
    }
    
    public enum eRenameStyle {standard, dbstandard, profile, custom}
    public String GetRenamedFileName(cFileInfo FileInfo, eRenameStyle Style) {
        String SName, EpName, GroupName, EpNo;
        String FN = "";
        String AbsPath = FileInfo.FilePath.getPath();
        AbsPath = AbsPath.substring(0, AbsPath.lastIndexOf(File.separatorChar));
	String DB_FN = (String)FileInfo.Data.get("DB_FileName");
	String Ext = DB_FN.substring(DB_FN.lastIndexOf("."));
        
        switch (Style) {
            case standard:
                SName = (String)FileInfo.Data.get("DB_SN_Romaji");
                EpName = (String)FileInfo.Data.get("DB_EpN_English");
                GroupName ="[" + (String)FileInfo.Data.get("DB_Group_Short") + "]";
                EpNo = (String)FileInfo.Data.get("DB_EpNo");
                
                FN = GroupName + " " + SName + " " + EpNo + " - " + EpName + Ext;
                break;
                
            case dbstandard:
                FN = DB_FN;
                break;
                
            case profile:
                
            case custom:
                SName=""; EpName="";
                for (int I=0;I<3;I++) {
                    if (SName.equals("")) SName = (String)FileInfo.Data.get("DB_SN_" + UserSettings.SeriesLang[I].toString());
                    if (EpName.equals("")) EpName = (String)FileInfo.Data.get("DB_EpN_" + UserSettings.EpisodeLang[I].toString());
                }
                
                GroupName ="[" + (String)FileInfo.Data.get("DB_Group_Short") + "]";
                EpNo = (String)FileInfo.Data.get("DB_EpNo");
                
                FN = GroupName + " " + SName + " " + EpNo + " - " + EpName + Ext;
                break;
        }
        return AbsPath + File.separatorChar + FN.replaceAll("[\\\\:\"/*|<>?]", "");
    }
    
    public EpProcessing(AniAdd CallingClass){
        CC = CallingClass;
        
        Method Func;
        try {
            Func = this.getClass().getMethod("AniDBInfoRspnd", new Class[] {int.class});
            CC.API.RegisterEvent(Func, this, "file");
            
            Func = this.getClass().getMethod("AniDBMyListRspnd", new Class[] {int.class});
            CC.API.RegisterEvent(Func, this, "mladd", "mldel");

            Func = this.getClass().getMethod("AniDBVoteRspnd", new Class[] {int.class});
            CC.API.RegisterEvent(Func, this, "vote");
            
        } catch (Exception exception) {exception.printStackTrace();}
    }
    
    public void AddFiles(Collection<File> NewFiles){
        for (File cf:NewFiles)
            AddFile(cf);
    }
    public void AddFile(File cf){
        if (GetFileIndex(cf, "FilePath")!=-1) return;
        
        cFileInfo FileInfo = new cFileInfo(cf);
        FileInfo.FileState=cFileInfo.eFileState.unprocessed;
        FileInfo.MLState=cFileInfo.eMyListState.queued;
        FileInfo.Data.put("Viewed", true);
        FileInfo.Data.put("MLFileState", cFileInfo.eMLFileState.onhdd);
        
        Files.add(FileInfo);
        CC.GUI.addFileToList(Files.size()-1);
        UpdateProgress(-1);
    }
    public void DelFile(int FIndex) {
        Files.remove(FIndex);
    }
    
    public enum eProcess {Start, Pause, Resume, Stop}
    public void Processing(eProcess Proc) {
        switch (Proc) {
            case Start:
                processStartOn = System.currentTimeMillis();
                pausedOn = 0; pausedTime = 0;
                processedMB = 0; totalMB = 0;
                
                for (cFileInfo File : Files) if (File.FileState==cFileInfo.eFileState.unprocessed) totalMB += File.FilePath.length()/1024/1024;
                
                ProcessEps();
                IsProcessing = true;
                IsPaused = false;
                break;
            case Pause:
                IsPaused = true;
                pausedOn = System.currentTimeMillis();
                break;
            case Resume:
                IsPaused = false;
                pausedTime += System.currentTimeMillis() - pausedOn;
                pausedOn=0;
                break;
            case Stop:
                //Not yet supported
                processStartOn = 0; pausedOn = 0; pausedTime = 0;
                processedMB = 0;
                
                IsProcessing = false;
                IsPaused = false;
                break;
        }
    }
    
    void ProcessEps(){
        int I = 0;
        for (cFileInfo ProcFile : Files) {
            if (ProcFile.FileState == cFileInfo.eFileState.unprocessed) {
                ProcFile.FileState = cFileInfo.eFileState.processing;
                UpdateFileRow(I);
                
                //BitSet Fash = FileProcessing.GenFastFileID(ProcFile.FilePath);
                ProcFile.Data.put("Fash", null); //ProcFile.Data.put("Fash", Misc.ToHex(Fash)); //Fash=null: no communication outsinde AniDB allowed
                
                FileProcessing FProc = new FileProcessing(CC);
                FProc.ProcessFile(ProcFile.FilePath, (String)ProcFile.Data.get("Fash"));
                
                System.out.println("Processing:" + ProcFile.FilePath.getName());
                return;
            }
            I++;
        }
        IsProcessing = false;
        CC.GUI.ProcessingDone();
    }
    
    public void ContinueProcessing(File FilePath, String Ed2kHash, String Ed2kFrom) {
        System.out.println("ContinueProcessing: " + FilePath.getName() + " Ed2kFrom: " + Ed2kFrom);

        int FileIndex = GetFileIndex(FilePath, "FilePath");
        cFileInfo ProcFile = Files.get(FileIndex);
        
        ProcFile.Data.put("Ed2kFrom", Ed2kFrom);
        
        ProcFile.Data.put("Ed2k", Ed2kHash);
        ProcFile.FileState = cFileInfo.eFileState.processed;

        ProcessEps(); //Hash next file
        
        UpdateProgress(0);
        UpdateFileRow(FileIndex);
        
        RequestDBFileInfo(Ed2kHash, Long.toString(ProcFile.FilePath.length()));
        RequestDBMyList(ProcFile);
    }
    
    void RequestDBFileInfo(String Ed2k, String Size) {
        UDPApi.cApiCmd Cmd = new UDPApi.cApiCmd("FILE", "file", true);
        
        BitSet BinCode = new BitSet(32);
        BinCode.set(1); //aid
        BinCode.set(2); //eid
        BinCode.set(3); //gid
        BinCode.set(4); //'lid
        BinCode.set(8); //'state
        //BinCode.set(10); //'ed2k (identifier)
        BinCode.set(30); //'anidb filename scheme
        Cmd.Params.put("fcode",Long.toString(Misc.SumBits(BinCode)));

        BinCode = new BitSet(32);
        BinCode.set(0); //'group name
        BinCode.set(1); //'group short name
        BinCode.set(8); //'epno
        BinCode.set(9); //'ep name
        BinCode.set(10); //'ep romaji
        BinCode.set(11); //'ep kanji
        BinCode.set(19); //'type
        BinCode.set(20); //'romaji name
        BinCode.set(21); //'kanji name
        BinCode.set(22); //'english name
        BinCode.set(23); //'other name
        BinCode.set(24); //'short name
        BinCode.set(25); //'synonym
        Cmd.Params.put("acode", Long.toString(Misc.SumBits(BinCode)));
        Cmd.Params.put("size", Size);
        Cmd.Params.put("ed2k", Ed2k);
        
        CC.API.QueryCmd(Cmd);
    }
    
    void RequestDBMyList(cFileInfo ProcFile) {
        UDPApi.cApiCmd Cmd = new UDPApi.cApiCmd("MYLISTADD", "mladd", true);
        Cmd.Params.put("size", Long.toString(ProcFile.FilePath.length()));
        Cmd.Params.put("ed2k", (String)ProcFile.Data.get("Ed2k"));
        Cmd.Params.put("viewed", CC.GUI.isWatched()?"1":"0");
        Cmd.Params.put("state", Integer.toString(CC.GUI.getFileState()));
        
        CC.API.QueryCmd(Cmd);
    }
    
    void RequestDBVote(cFileInfo ProcFile) {
        UDPApi.cApiCmd Cmd = new UDPApi.cApiCmd("VOTE", "vote", true);
        Cmd.Params.put("id", (String)ProcFile.Data.get("AId"));
        Cmd.Params.put("type", true?"1":"2"); //decision missing (Perm/Temp Vote)
        Cmd.Params.put("value", (String)ProcFile.Data.get("Vote"));        
        Cmd.Params.put("epno", (String)ProcFile.Data.get("EpNo"));    
        
        CC.API.QueryCmd(Cmd);
    }
    
    public void AniDBInfoRspnd(int QueryIndex) {
        UDPApi.cQuery Query = CC.API.Queries.get(QueryIndex);
        int RespID = Query.RcvdMsg.ResponseID;
        
        int FileIndex = GetFileIndex(Query.SendCmd.Params.get("ed2k"), "Ed2k");
        if (FileIndex < 0) return; //File not found (Todo: throw error)
        
        cFileInfo ProcFile = Files.get(FileIndex);
        ProcFile.Data.put("FuncDBFileInfo", true);
        System.out.println("AniDBInfoRspnd: " + ProcFile.FilePath.getName());
        
        if (RespID == 322) {
            return; //Multiple files with FId (Todo: Throw error)
        } else if(RespID == 320) {
            ProcFile.MLState = cFileInfo.eMyListState.unknown;
            UpdateFileRow(FileIndex);
        } else {
            ArrayList<String> DF = Query.RcvdMsg.DataField;
            ProcFile.Data.put("DB_FId", DF.get(0));
            ProcFile.Data.put("DB_AId", DF.get(1));
            ProcFile.Data.put("DB_EId", DF.get(2));
            ProcFile.Data.put("DB_GId", DF.get(3));
            ProcFile.Data.put("DB_LId", DF.get(4));
            ProcFile.Data.put("DB_State", DF.get(5));
            ProcFile.Data.put("DB_FileName", DF.get(6));
            ProcFile.Data.put("DB_Group_Long", DF.get(7));
            ProcFile.Data.put("DB_Group_Short", DF.get(8));
            ProcFile.Data.put("DB_EpNo", DF.get(9));
            ProcFile.Data.put("DB_EpN_English", DF.get(10));
            ProcFile.Data.put("DB_EpN_Romaji", DF.get(11));
            ProcFile.Data.put("DB_EpN_Kanji", DF.get(12));
            ProcFile.Data.put("DB_Type", DF.get(13));
            ProcFile.Data.put("DB_SN_Romaji", DF.get(14));
            ProcFile.Data.put("DB_SN_Kanji", DF.get(15));
            ProcFile.Data.put("DB_SN_English", DF.get(16));
            ProcFile.Data.put("DB_SN_Other", DF.get(17));
            ProcFile.Data.put("DB_SN_Short", DF.get(18));
            ProcFile.Data.put("DB_Synonym", DF.get(19));

            ProcFile.FileState = cFileInfo.eFileState.gotfileinfo;
            ProcFile.Data.put("Renamed", GetRenamedFileName(ProcFile, UserSettings.Type));
            UpdateFileRow(FileIndex);

            //UploadFileInfo(ProcFile); //no outside communication allowed
        }
          
        if (!ProcFile.Data.containsKey("FinalProc") && ProcFile.Data.containsKey("FuncMyList")) {
            FinalProcessing(FileIndex); //Call FinalProcessing after AniDBMyListRspnd & AniDBInfoRspnd only once
        } else {
        }
    }
    
    public void AniDBMyListRspnd(int QueryIndex) {
        UDPApi.cQuery Query = CC.API.Queries.get(QueryIndex);
        int RespID = Query.RcvdMsg.ResponseID;
        
        int FileIndex = GetFileIndex(Query.SendCmd.Params.get("ed2k"), "Ed2k");
        if (FileIndex < 0) return; //File not found (Todo: throw error)
        
        cFileInfo ProcFile = Files.get(FileIndex);
        ProcFile.Data.put("FuncMyList", true);
        System.out.println("AniDBMyListRspnd: " + ProcFile.FilePath.getName());
        
        if (RespID == 210) {
            //File Added
            ProcFile.MLState = cFileInfo.eMyListState.added;
        } else if (RespID == 311) {
            //File Edited
        } else if (RespID == 310) {
            //File Already Added
            ProcFile.MLState = cFileInfo.eMyListState.inmylist;
        } else if (RespID == 320 && RespID == 330 && RespID == 350) {
            ProcFile.MLState = cFileInfo.eMyListState.unknown;
        }
        UpdateFileRow(FileIndex);
        
        if (!ProcFile.Data.containsKey("FinalProc") && ProcFile.Data.containsKey("FuncDBFileInfo")) {
            FinalProcessing(FileIndex); //Call FinalProcessing after AniDBMyListRspnd & AniDBInfoRspnd only once
        } else {
        }
    }
    
    public void AniDBVoteRspnd(int QueryIndex) {
        UDPApi.cQuery Query = CC.API.Queries.get(QueryIndex);
        int RespID = Query.RcvdMsg.ResponseID;
        
        int FileIndex = GetFileIndex(Query.SendCmd.Params.get("ed2k"), "Ed2k");
        if (FileIndex < 0) return; //File not found (Todo: throw error)
        
        cFileInfo ProcFile = Files.get(FileIndex);
        System.out.println("AniDBVoteRspnd: " + ProcFile.FilePath.getName());

        if (RespID == 260 && RespID == 262) {
            ProcFile.Data.put("Voted", true); //Voted
        } else if (RespID == 263) {
            ProcFile.Data.put("Voted", false);//Revoked
        } else if (RespID == 363) {
            //PermVote Not Allowed
        }
    }
    
    void FinalProcessing(int FileIndex) {
        Files.get(FileIndex).Data.put("FinalProc", true);
        
        if (Files.get(FileIndex).Data.get("Vote") != null) {
            RequestDBVote(Files.get(FileIndex));
        }
        
        if (CC.GUI.shouldRename() && Files.get(FileIndex).FileState == cFileInfo.eFileState.gotfileinfo) { //Check if user want to rename files
            File RenamedFile = new File((String)Files.get(FileIndex).Data.get("Renamed"));
            if (Files.get(FileIndex).FilePath.renameTo(RenamedFile)) {
                System.out.println("File renamed");
                Files.get(FileIndex).Data.put("IsRenamed", "Yes");
                Files.get(FileIndex).FilePath = RenamedFile;
            } else {
                System.out.println("Error while renaming: " + RenamedFile.getPath());
                Files.get(FileIndex).Data.put("IsRenamed", "Error");
            }
        }
        
        CC.GUI.editFile(FileIndex);
        System.out.println("I was here");
        
        boolean Done=true;
        for (cFileInfo FileInfo : Files) if(!FileInfo.Data.containsKey("FinalProc")) Done = false;
        if(Done) {
            UI.UIProcDone ReportDialog = new UI.UIProcDone(CC);
            ReportDialog.setSize(400, 200);
            ReportDialog.setVisible(true);
        }
    }
    
/**
    void UploadFileInfo(cFileInfo ProcFile) {
        if (ProcFile.Data.get("Ed2kFrom").equals("Parser")) {
            Map<String, String> Params = new TreeMap<String, String>();
            
            Params.put("Action", "Add");
            Params.put("UID", CC.API.UserInfo.UserName.toLowerCase());
            Params.put("Client", CC.API.ConInfo.ClientID);
            Params.put("ClientVer",Integer.toString(CC.API.ConInfo.ClientVer));
            Params.put("Fash", (String)ProcFile.Data.get("Fash"));
            Params.put("Ed2k", (String)ProcFile.Data.get("Ed2k"));
            Params.put("Size", Long.toHexString(ProcFile.FilePath.length()));
            Params.put("AId", (String)ProcFile.Data.get("DB_AId"));
            Params.put("FId", (String)ProcFile.Data.get("DB_FId"));
            Params.put("GId", (String)ProcFile.Data.get("DB_GId"));
            Params.put("Type", (String)ProcFile.Data.get("DB_Type"));
            Params.put("EpNo", (String)ProcFile.Data.get("DB_EpNo"));
            Params.put("Group", (String)ProcFile.Data.get("DB_Group_Long") + "*" + (String)ProcFile.Data.get("DB_Group_Short"));
            Params.put("SeriesName", (String)ProcFile.Data.get("DB_SN_English") + "||" + (String)ProcFile.Data.get("DB_SN_Romaji") + "||" + (String)ProcFile.Data.get("DB_SN_Kanji"));
            Params.put("SeriesNameOther", (String)ProcFile.Data.get("DB_SN_Other") + "||" + (String)ProcFile.Data.get("DB_SN_Short"));
            Params.put("EpName", (String)ProcFile.Data.get("DB_EpN_English") + "||" + (String)ProcFile.Data.get("DB_EpN_Romaji") + "||" + (String)ProcFile.Data.get("DB_EpN_Kanji"));

            Misc.PageContact("http://arokh.kilu.de/Programs/AniAdd/FileInfo.php", Params);
        }        
    }
*/    
    
    void UpdateFileRow(final int FileIndex) {
        Runnable Update = new Runnable() {
            public void run() {
                CC.GUI.editFile(FileIndex);
            }
        };
        javax.swing.SwingUtilities.invokeLater(Update);
    }
    
    long processStartOn, pausedOn, pausedTime;
    long totalMB;
    public long processedMB;
    void UpdateProgress(final int FProg) {
        int I=0;
        
        for (cFileInfo F: Files) if (F.FileState == cFileInfo.eFileState.processed || F.FileState == cFileInfo.eFileState.gotfileinfo) I++;
        final int TProg = (100 * I + (FProg==-1?0:FProg)) / Files.size();

        Runnable Update = new Runnable() {
            public void run() {
                CC.GUI.updateProgressBars(TProg, FProg);
                if (FProg != -1) {
                    CC.GUI.updateETA(totalMB, processedMB, (System.currentTimeMillis() - processStartOn - pausedTime));
                } else {
                    CC.GUI.updateETA(totalMB, processedMB, 0);
                }
            }
        };
        javax.swing.SwingUtilities.invokeLater(Update);
    }
    
    int GetFileIndex(Object Search, String IndexOf) { //temporary, will be replaced with a faster version
        if (IndexOf.equals("FilePath")) {
            for (int I=0; I < Files.size(); I++) {
                if (Files.get(I).FilePath.equals((File)Search)) {
                    return I;
                }
            }
        } else if (IndexOf.equals("Ed2k")) {
            for (int I=0; I < Files.size(); I++) {
                if (Files.get(I).Data.get("Ed2k").equals(Search)) {
                    return I;
                }
            }
        }
        return -1;
    }
}

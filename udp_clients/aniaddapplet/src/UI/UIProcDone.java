package UI;

//@author Arokh

import Processing.cFileInfo;
import javax.swing.JLabel;

public class UIProcDone extends javax.swing.JDialog {
    AniAdd.AniAdd CC;

    public UIProcDone(AniAdd.AniAdd CallingClass) {
        CC = CallingClass;
        String Report = GetReport();
        
        JLabel lbl_Report= new JLabel();
        lbl_Report.setText(Report);
        lbl_Report.setSize(500,600);
        add(lbl_Report);
    }
    
    String GetReport() {
        String Report="<html><body>";
        String ReportBlock;
        boolean bReportBlock=false;
        int Count = 0;
        
        for(cFileInfo FileInfo : CC.EpProc.Files) {
            if(FileInfo.MLState == cFileInfo.eMyListState.inmylist) Count++;
        }
        
        if(Count != 0) {
            Report += Count + " files added to MyList"; Count=0;
            Report += " and marked as " + (CC.GUI.isWatched()?"Watched":"Unwatched");
            Report +="<br>";
        }
        
        for(cFileInfo FileInfo : CC.EpProc.Files) {
            if(FileInfo.Data.containsKey("IsRenamed") && FileInfo.Data.get("IsRenamed").equals("Yes")) Count++;
        }
        if(Count != 0) {
            Report += Count + " files renamed"; Count=0;
        } else {
            Report += "No files have been renamed";
        }
        Report += "<br>";
        
        ReportBlock = "<br>Files which coudn't be found in AniDB (Unknown Files):<br>";
        for(cFileInfo FileInfo : CC.EpProc.Files) {
            if(FileInfo.MLState == cFileInfo.eMyListState.unknown) {
                bReportBlock = true;
                ReportBlock += FileInfo.FilePath.getName() + "<br>";
            }
        }
        if(bReportBlock) {
            Report+= ReportBlock;
            Report += "Most causes: The file may have yet to be added to AniDB or your file is corrupt\n";
            bReportBlock = false;
        }
        
        
        ReportBlock = "<br>Files which failed to be renamed:<br>";
        for(cFileInfo FileInfo : CC.EpProc.Files) {
            if(FileInfo.Data.containsKey("IsRenamed") && FileInfo.Data.get("IsRenamed").equals("Error")) {
                bReportBlock = true;
                ReportBlock += FileInfo.FilePath.getName() + " Target Name: " + (String)(FileInfo.Data.get("Renamed")) + "<br>";
            }
        }
        if(bReportBlock) {
            Report+= ReportBlock;
            Report += "Most causes: The target filename contains illegal characters<br>";
            bReportBlock = false;
        }
        
        return Report + "</body></html>";
    }
    

}

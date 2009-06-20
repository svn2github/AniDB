package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.IAniAdd;
import aniAdd.Module;
import aniAdd.misc.Misc;
import aniAdd.misc.Mod_Memory;
import java.awt.Color;
import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableModel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;
import processing.FileInfo;
import processing.FileParser;
import processing.Mod_EpProcessing;
import udpApi.Mod_UdpApi;
import udpApi.Query;

public class Mod_GUI extends GUIComponents implements Module {
    //private Thread tLblupdater;
    private LabelUpdater labelUpdater = new LabelUpdater();
    
    private HashMap<Integer, Integer> fileId2LogItemId = new HashMap<Integer, Integer>();
    private Mod_EpProcessing epProc;
    private Mod_Memory mem;
    private IAniAdd aniAdd;
    private Mod_UdpApi api;

    
    private void AddCmdEvent(final ComEvent comEvent) {
        if(comEvent.Type() != ComEvent.eType.Information ||
          !((comEvent.ParamCount() > 0) && (comEvent.Params(0).equals("Cmd"))) ||
          (api.ModState() == eModState.Terminating)) return;
        
        if(!SwingUtilities.isEventDispatchThread()){
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        AddCmdEvent(comEvent);
                    }
                });
            } catch (Exception ex) {}
            return;
        }

        boolean scroll = false;
        DefaultTreeModel trModel = (DefaultTreeModel)trvw_Cmd.getModel();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode)trModel.getRoot();
        TreeNode lastNode = new TreeNode(root);


        if(!root.isLeaf() && !lastNode.isLeaf()) {
            lastNode = (TreeNode)lastNode.getChildAt(lastNode.getChildCount());
            if(trvw_Cmd.isExpanded(new TreePath(trModel.getPathToRoot(lastNode)))) lastNode = (TreeNode)root.getChildAt(lastNode.getChildCount());
            if(trvw_Cmd.isVisible(new TreePath(lastNode))) scroll = true;
        }

        Integer queryIndex = (Integer)comEvent.Params(1);
        if(queryIndex < 0) return;

        Query query = api.Queries().get(queryIndex);
        String nodeText = Misc.longToTime(query.getSendOn()) + " ID " + queryIndex + " " + query.getCmd().Action();
        nodeText += (query.getRetries() > 0 ? " Retries: " + query.getRetries() : "") + " Arrived: " + (query.getSuccess()!=null ? (query.getSuccess() ? "Yes" : "Failed") : "Pending");

        TreeNode node = null;
        if((Boolean)comEvent.Params(2)) {
            node = new TreeNode(nodeText, queryIndex.toString());

            nodeText = Misc.longToTime(query.getSendOn()) + " Send: " + query.getCmd().Action() + " ";
            for (Map.Entry<String,String> entrySet : query.getCmd().Params().entrySet()) {
                nodeText += entrySet.getKey() + "=" + (entrySet.getKey().equals("pass")?"***":entrySet.getValue())+" ";
            }
            node.add(new TreeNode(nodeText, ""));

            trModel.insertNodeInto(node, root , trModel.getChildCount(root));
            lastNode = node;

        } else {
            for(int i = root.getChildCount() - 1; i >= 0; i--) {
                if(((TreeNode)root.getChildAt(i)).Name().equals(queryIndex.toString())) { node = (TreeNode)root.getChildAt(i); break; }
            }
            if(node == null) return;
            node.setUserObject(nodeText);

            node.BackColor(query.getRetries() >= 3 && query.getSuccess()!=null && !query.getSuccess() ? Color.red : null);
            if(query.getRetries() == 0) {
                nodeText = Misc.longToTime(query.getSendOn()) + " Rcvd: " + query.getReply().ReplyId() + " " + query.getReply().ReplyMsg();
                node.add(new TreeNode(nodeText, null));

                nodeText = "";
                for(int i = 0; i < query.getReply().DataField().size(); i++) {
                    nodeText += "D" + (i + 1) + ": " + query.getReply().DataField().get(i) + "  ";
                    if((i % 6 == 0) && (i != 0)) {
                        node.add(new TreeNode(nodeText, null));
                        nodeText = "";
                    }
                }
                if(!nodeText.equals("")) node.add(new TreeNode(nodeText, null));
            }
        }
        if(scroll) trvw_Cmd.makeVisible(new TreePath(lastNode));
        trModel.reload();
    }
    private void AddFileEvent(final ComEvent comEvent){
        if(!SwingUtilities.isEventDispatchThread()){
            try {
                SwingUtilities.invokeAndWait(new Runnable() {
                    public void run() {
                        AddFileEvent(comEvent);
                    }
                });
            } catch (Exception ex) {
                ex.printStackTrace();
            }
            return;
        }

        FileInfo file = epProc.id2FileInfo((Integer)comEvent.Params(2));
        int fileIndex = epProc.Id2Index(file.Id());
        ((DefaultTableModel)tbl_Files.getModel()).fireTableRowsUpdated(fileIndex, fileIndex);

        TreeNode node;
        int logItemId; String title = "";
        DefaultTreeModel model = (DefaultTreeModel)trvw_Event.getModel();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode)model.getRoot();
        if((Mod_EpProcessing.eComSubType)comEvent.Params(1) == Mod_EpProcessing.eComSubType.Processing) {
            node = new TreeNode();
            node.Name("File " + file.FileObj().getName() + ": [*]");
            node.setUserObject(node.Name().replace("[*]", "Processing started"));
            
            fileId2LogItemId.put(file.Id(), root.getChildCount());
            model.insertNodeInto(node, root, root.getChildCount());
            return;
        }

        logItemId = fileId2LogItemId.get(file.Id());
        node = (TreeNode)root.getChildAt(logItemId);
        switch((Mod_EpProcessing.eComSubType)comEvent.Params(1)) {
            case NoWriteAccess:
                title = "Fileaccess denied";
                node.add(new TreeNode(title, null));
                break;
            case GotFromHistory:
                title = "Got fileinformation from history";
                node.add(new TreeNode(title, null));
                break;
            case ParsingDone:
                processedBytes = epProc.processedBytes();
                title = "AVDump processing done";
                FileParser fp = (FileParser)comEvent.Params(3);
                node.add(new TreeNode("Parsing done (" + (int)(file.FileObj().length() / 1024 / 1024) + "MB in " + (fp.ParseDuration()/100/10d) + "s with "+fp.MBPerSecond()+"mb/s)"));
                break;
            case ParsingError:
                processedBytes = epProc.processedBytes();
                title = "File Parsing error";
                node.add(new TreeNode("Parsing Error"));
                break;
            case GetDBInfo:
                if((Boolean)comEvent.Params(3) || (Boolean)comEvent.Params(4)) {
                    title = "Sending packets to AniDB";
                    if((Boolean)comEvent.Params(3) & (Boolean)comEvent.Params(4)) {
                        node.add(new TreeNode("Sending packets to AniDB (FILE and MYLIST)"));
                    } else if((Boolean)comEvent.Params(3)) {
                        node.add(new TreeNode("Sending packets to AniDB (FILE)"));
                    } else {
                        node.add(new TreeNode("Sending packets to AniDB (MYLIST)"));
                    }
                } else {
                    title = "Finalizing";
                    node.add(new TreeNode("No comunication to AniDB needed, all needed info available. Finalizing."));
                }
                break;
            case FileCmd_NotFound:
                title = "File not found";
                node.add(new TreeNode("(FileCmd) File not found"));
                break;
            case FileCmd_GotInfo:
                title = "Got fileinformation";
                node.add(new TreeNode("Got fileinformation from AniDB"));
                break;
            case FileCmd_Error:
                title = "(FileCmd) Error";
                node.add(new TreeNode(title));
                break;
            case MLCmd_FileAdded:
                title = "File added to MyList";
                node.add(new TreeNode(title));
                break;
            case MLCmd_AlreadyAdded:
                title = "File already added";
                node.add(new TreeNode(title));
                break;
            case MLCmd_FileRemoved:
                title = "File removed from MyList";
                node.add(new TreeNode(title));
                break;
            case MLCmd_NotFound:
                title = "File not found";
                node.add(new TreeNode(title));
                break;
            case MLCmd_Error:
                title = "(MLCmd) Error";
                node.add(new TreeNode(title));
                break;
            /*case VoteCmd_EpVoted:
                title = "Episode vote set";
                node.add(new TreeNode(title + " to " + file.Vote.Value));
                break;*/
            case VoteCmd_EpVoteRevoked:
                title = "Episode vote revoked";
                node.add(new TreeNode(title));
                break;
            case VoteCmd_Error:
                title = "(VoteCmd) Error";
                node.add(new TreeNode(title));
                break;
            case RenamingFailed:
                title = "Renaming failed";
                node.add(new TreeNode("Couldn't rename file."));
                break;
            case FileRenamed:
                title = "File renamed";
                node.add(new TreeNode("File renamed to " + ((File)comEvent.Params(4)).getName()));
                break;
            case RenamingNotNeeded:
                title = "No renaming needed";
                node.add(new TreeNode(title));
                break;
            /*case RelFilesRenamed:
                title = "Related files renamed";
                node.add(new TreeNode("Related files renamed (" + (string)msg[3] + ")"));
                break;
            case RelFilesRenamingFailed:
                title = "Couldn't rename related files";
                node.add(new TreeNode(title, null));
                break;*/
            case Done:
                title = "Processing done" + (!file.ActionsError().isEmpty() ? " (with errors)" : "");
                node.add(new TreeNode(title));
                fileId2LogItemId.remove(file.Id());
                break;
            default:
                break;
        }

        node.setUserObject(node.Name().replace("[*]", title));
        model.reload();
    }
    private void EpProcStatusEvent(final ComEvent comEvent){
        if(comEvent.Params(1) == Mod_EpProcessing.eProcess.Start){
            if(processingStartOn==0) processingStartOn = System.currentTimeMillis();
            labelUpdater.Start();
           
        } else if(comEvent.Params(1) == Mod_EpProcessing.eProcess.Pause){
            processingPausedOn = System.currentTimeMillis();
            
        } else if(comEvent.Params(1) == Mod_EpProcessing.eProcess.Resume){
            pausedTime += System.currentTimeMillis() - processingPausedOn;
            processingPausedOn = 0;
            
        } else if(comEvent.Params(1) == Mod_EpProcessing.eComSubType.Done){
            SwingUtilities.invokeLater(new Runnable() {
                public void run() {
                    ProcessingDone();
                    //UpdateProgressBars();
                    //UpdateStatusLabels();
                }
            });
        }
    }
    
    protected void UpdateProgressBars() {
        UpdateProgressBars(FileProgress(), TotalProgress());
    }
    protected void UpdateStatusLabels() {
        UpdateStatusLabels(byteCount, ProcByteCount(), ElapsedTime(), ETA());
    }

    long processingStartOn;
    long processingPausedOn;
    long pausedTime;
    long byteCount;
    long processedBytes;


    public double FileProgress(){
        return (double)epProc.processedBytesCurrentFile() / (double)epProc.totalBytesCurrentFile();
    }
    public double TotalProgress(){
        double prgValFile = (double)ProcByteCount() / (double)byteCount;

        int pendingFileCmds =  (epProc.FileCount() - epProc.processedFileCount())*((Boolean)mem.get("AddToMyList")?2:1);
        int procCmd = api.totalCmdCount() - api.waitingCmdCount();
        double partialCmd = 1-api.currendCmdDelay()/(double)api.cmdSendDelay();
        double prgValCmd = (procCmd+partialCmd) / (double)(api.totalCmdCount()+pendingFileCmds);
        return Math.min(prgValFile, prgValCmd);
    }
    public long ProcByteCount(){
        return processedBytes + epProc.processedBytesCurrentFile();
    }
    public long ElapsedTime(){
        return processingStartOn!=0?System.currentTimeMillis() - processingStartOn - pausedTime:0;
    }
    public long ETA(){
        long procByteCount = ProcByteCount();
        double etaFile = (double)(byteCount-procByteCount)/((double)procByteCount/(double)ElapsedTime());

        int pendingFileCmds =  (epProc.FileCount() - epProc.processedFileCount())*((Boolean)mem.get("AddToMyList")?2:1);
        double etaCmd = (api.waitingCmdCount() + pendingFileCmds)*api.cmdSendDelay()-api.cmdSendDelay()+api.currendCmdDelay();

        return (long)Math.max(etaFile, etaCmd);
    }

    class LabelUpdater implements Runnable{
        Thread t;

        public void Start(){
            t = new Thread(this);
            t.start();
        }

        public void Terminate(){
            try {
                if( t!= null) t.join();
            } catch (InterruptedException ex) { ex.printStackTrace();}
        }

        public void run() {
            while((epProc.isProcessing() || api.waitingCmdCount()!=0) && modState != eModState.Terminating){
                do {
                    try {Thread.sleep(100);} catch (InterruptedException ex) {}
                } while (epProc.isPaused() && modState != eModState.Terminating);

                if( modState != eModState.Terminating){
                    try { SwingUtilities.invokeAndWait(new Runnable() {
                        public void run() {
                            UpdateProgressBars();
                            UpdateStatusLabels();
                        }
                    });} catch (Exception ex) {}
                }
            }

            processingStartOn = 0;
            pausedTime = 0;
            processingPausedOn = 0;
        }
    }

    public void DisplayEvent(String msg, ComEvent.eType msgType){
        super.DisplayEvent(msg, msgType==ComEvent.eType.Warning?Color.yellow:(msgType==ComEvent.eType.Error?Color.red:Color.red));
    }

    protected void SaveToMem(String key, Object value) {mem.put(key, value);}
    protected Object LoadFromMem(String key, Object defVal) {return mem.get(key, defVal);}

    protected void AddFiles(ArrayList<File> files) {epProc.addFiles(files);}
    protected void ToggleMLCmd(boolean doAction) {
        SetOptions(doAction, new ISetOption() {
            public void setOption(Object type, FileInfo file) {
                if((Boolean)type){
                    file.ActionsTodo().add(FileInfo.eAction.MyListCmd);
                } else {
                    file.ActionsTodo().remove(FileInfo.eAction.MyListCmd);
                }
            }
        });
    }
    protected void ToggleFileRename(boolean doAction) {
        SetOptions(doAction, new ISetOption() {
            public void setOption(Object type, FileInfo file) {
                if((Boolean)type){
                    file.ActionsTodo().add(FileInfo.eAction.Rename);
                } else {
                    file.ActionsTodo().remove(FileInfo.eAction.Rename);
                }
            }
        });
    }
    protected void ToggleStorageType(int type) {
        SetOptions(type, new ISetOption() {
            public void setOption(Object type, FileInfo file) {
                file.MLStorage(FileInfo.eMLStorageState.values()[(Integer)type]);
            }
        });
    }
    protected void ToggleWatched(boolean doAction) {
        SetOptions(doAction, new ISetOption() {
            public void setOption(Object type, FileInfo file) {
                if((Boolean)type){
                    file.ActionsTodo().add(FileInfo.eAction.Watched);
                } else {
                    file.ActionsTodo().remove(FileInfo.eAction.Watched);
                }
            }
        });
    }
    protected void ToggleProcessing(String type) {
        if(type.equals("Start")){
            epProc.processing(Mod_EpProcessing.eProcess.Start);

        } else if(type.equals("Pause")){
            epProc.processing(Mod_EpProcessing.eProcess.Pause);

        } else if(type.equals("Resume")){
            epProc.processing(Mod_EpProcessing.eProcess.Resume);
        }
    }
    private void SetOptions(Object type, ISetOption optionSetter){
        int size = epProc.FileCount();
        for (int i = 0; i < size; i++) {
            FileInfo fileInfo = epProc.index2FileInfo(i);
            if(fileInfo.Served()) continue;

            optionSetter.setOption(type, fileInfo);
        }
    }
    private interface ISetOption{void setOption(Object type, FileInfo file);}


    // <editor-fold defaultstate="collapsed" desc="IModule">
    protected String modName = "MainGUI";
    protected eModState modState = eModState.New;

    public eModState ModState() { return modState; }
    public String ModuleName() {return modName;}
    public void Initialize(IAniAdd aniAdd) {
        modState = eModState.Initializing;

        this.aniAdd = aniAdd;
        aniAdd.AddComListener(new AniAddEventHandler());
        mem = (Mod_Memory)aniAdd.GetModule("Memory");
        epProc = (Mod_EpProcessing)aniAdd.GetModule("EpProcessing");
        api = (Mod_UdpApi)aniAdd.GetModule("UdpApi");

        api.AddComListener(new ComListener() {
            public void EventHandler(ComEvent comEvent) {
                AddCmdEvent(comEvent);
            }
        });
        epProc.AddComListener(new ComListener() {
            public void EventHandler(ComEvent comEvent) {
                if(comEvent.Params(0) == Mod_EpProcessing.eComType.FileEvent) {
                    AddFileEvent(comEvent);
                } else if(comEvent.Params(0) == Mod_EpProcessing.eComType.FileCountChanged) {
                    byteCount = epProc.totalBytes();
                } else if(comEvent.Params(0) == Mod_EpProcessing.eComType.Status) {
                    EpProcStatusEvent(comEvent);
                }
            }
        });

        ComListener comListener;
        for (Module module : aniAdd.GetModules()) { 
            comListener = new ComListener() {
                public void EventHandler(ComEvent comEvent) {
                    if(comEvent.Type()==ComEvent.eType.Error ||
                       comEvent.Type()==ComEvent.eType.Warning ||
                       comEvent.Type()==ComEvent.eType.Fatal){
                        DisplayEvent((String)comEvent.Params(0), comEvent.Type());
                    }
                }
            };
            module.AddComListener(comListener);
        }
        
        FileTable_TM tm = new gui.FileTable_TM(epProc);
        tbl_Files.setDefaultRenderer(Object.class, new FileTable_Renderer());
        tbl_Files.setModel(tm);
        tbl_Files.setRowHeight(19);

        tbl_Files.getColumnModel().getColumn(1).setMaxWidth(72);

        modState = eModState.Initialized;
   }
    public void Terminate() {
        modState = eModState.Terminating;

        labelUpdater.Terminate();

        modState = eModState.Terminated;
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Com System">
	private ArrayList<ComListener> listeners = new ArrayList<ComListener>();
    protected void ComFire(ComEvent comEvent){
        for (ComListener listener : listeners) {
            listener.EventHandler(comEvent);
        }
    }
	public void AddComListener(ComListener comListener){ listeners.add(comListener); }
	public void RemoveComListener(ComListener comListener){ listeners.remove(comListener); }
    class AniAddEventHandler implements ComListener{
        public void EventHandler(ComEvent comEvent) {

        }
    }
    // </editor-fold>
}

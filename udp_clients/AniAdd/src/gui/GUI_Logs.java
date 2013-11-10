package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.Communication.ComListener;
import aniAdd.IAniAdd;
import aniAdd.Modules.IModule;
import aniAdd.Modules.IModule.eModState;
import aniAdd.misc.Misc;
import aniAdd.misc.Mod_Memory;
import java.awt.Color;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.StringSelection;
import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;
import javax.swing.JTree;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreePath;
import processing.FileInfo;
import processing.FileParser;
import processing.Mod_EpProcessing;
import processing.Mod_EpProcessing.eComType;
import udpApi.Mod_UdpApi;
import udpApi.Query;

public class GUI_Logs extends javax.swing.JPanel implements GUI.ITab {

    private HashMap<Integer, Integer> fileId2LogItemId = new HashMap<Integer, Integer>();
    private ArrayList<ComEvent> comEvents = new ArrayList<ComEvent>();
    private IAniAdd aniAdd;
    private Mod_EpProcessing epProc;
    private Mod_UdpApi api;

    public GUI_Logs() {
        initComponents();
    }

    private void CopyTree(JTree tree) {
        Clipboard cb = Toolkit.getDefaultToolkit().getSystemClipboard();
        String cmdTree = CopyTree((DefaultMutableTreeNode) tree.getModel().getRoot(), 0);

        cb.setContents(new StringSelection(cmdTree), null);
    }

    private String CopyTree(DefaultMutableTreeNode node, int depth) {
        String treeStr = "";
        do {
            for (int i = 0; i < depth; i++) {
                treeStr += "  ";
            }
            treeStr += (String) node.getUserObject() + "\n";
            if (node.getChildCount() != 0) {
                treeStr += CopyTree((DefaultMutableTreeNode) node.getChildAt(0), depth + 1);
            }
        } while ((node = node.getNextSibling()) != null);
        return treeStr;
    }

    private void CopyEvents() {
        Clipboard cb = Toolkit.getDefaultToolkit().getSystemClipboard();
        String eventStr = (new Date()).toString() + "\n";

        Mod_Memory mem = (Mod_Memory) aniAdd.GetModule("Memory");
        eventStr += mem.toString();


        for (ComEvent comEvent : comEvents) {
            eventStr += comEvent.toString() + "\n";
        }

        cb.setContents(new StringSelection(eventStr), null);
    }

    public String TabName() {
        return "Log";
    }

    public int PreferredTabLocation() {
        return 1;
    }

    private void AddCmdEvent(final ComEvent comEvent) {
        if (comEvent.Type() != ComEvent.eType.Information ||
                !((comEvent.ParamCount() > 0) && (comEvent.Params(0).equals("Cmd") || comEvent.Params(0).equals("Reply"))) ||
                (api.ModState() == eModState.Terminating)) {
            return;
        }

        if ((Integer) comEvent.Params(1) < 0) {
            return;
        }

        if (!SwingUtilities.isEventDispatchThread()) {
            try {
                SwingUtilities.invokeAndWait(new Runnable() {

                    public void run() {
                        AddCmdEvent(comEvent);
                    }
                });
            } catch (Exception ex) {
            }
            return;
        }

        boolean scroll = false;
        DefaultTreeModel trModel = (DefaultTreeModel) trvw_Cmd.getModel();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) trModel.getRoot();
        TreeNode lastNode = new TreeNode(root);


        if (!root.isLeaf() && !lastNode.isLeaf()) {
            lastNode = (TreeNode) lastNode.getChildAt(lastNode.getChildCount());
            if (trvw_Cmd.isExpanded(new TreePath(trModel.getPathToRoot(lastNode)))) {
                lastNode = (TreeNode) root.getChildAt(lastNode.getChildCount());
            }
            if (trvw_Cmd.isVisible(new TreePath(lastNode))) {
                scroll = true;
            }
        }

        Integer queryIndex = (Integer) comEvent.Params(1);

        Query query = api.Queries().get(queryIndex);
        String nodeText = Misc.DateToString(query.getSendOn()) + " ID " + queryIndex + " " + query.getCmd().Action();
        nodeText += (query.getRetries() > 0 ? " Retries: " + query.getRetries() : "") + " Arrived: " + (query.getSuccess() != null ? (query.getSuccess() ? "Yes" : "Failed") : "Pending");

        TreeNode node = null;
        if ((Boolean) comEvent.Params(2)) {
            node = new TreeNode(nodeText, queryIndex.toString());

            nodeText = Misc.DateToString(query.getSendOn()) + " Send: " + query.getCmd().Action() + " ";
            for (Map.Entry<String, String> entrySet : query.getCmd().Params().entrySet()) {
                nodeText += entrySet.getKey() + "=" + ((entrySet.getKey().equals("pass") || entrySet.getKey().equals("sess") || entrySet.getKey().equals("autopass")) ? "***" : entrySet.getValue()) + " ";
            }
            node.add(new TreeNode(nodeText, ""));

            trModel.insertNodeInto(node, root, trModel.getChildCount(root));
            lastNode = node;

        } else {
            for (int i = root.getChildCount() - 1; i >= 0; i--) {
                if (((TreeNode) root.getChildAt(i)).Name().equals(queryIndex.toString())) {
                    node = (TreeNode) root.getChildAt(i);
                    break;
                }
            }
            if (node == null) {
                return;
            }
            node.setUserObject(nodeText);

            node.BackColor(query.getRetries() >= 3 && query.getSuccess() != null && !query.getSuccess() ? Color.red : null);
            if (query.getRetries() == 0) {
                nodeText = Misc.DateToString(query.getSendOn()) + " Rcvd: " + query.getReply().ReplyId() + " " + query.getReply().ReplyMsg();
                node.add(new TreeNode(nodeText, null));

                nodeText = "";
                for (int i = 0; i < query.getReply().DataField().size(); i++) {
                    nodeText += "D" + (i + 1) + ": " + query.getReply().DataField().get(i) + "  ";
                    if ((i % 6 == 0) && (i != 0)) {
                        node.add(new TreeNode(nodeText, null));
                        nodeText = "";
                    }
                }
                if (!nodeText.equals("")) {
                    node.add(new TreeNode(nodeText, null));
                }
            }
        }
        if (scroll) {
            trvw_Cmd.makeVisible(new TreePath(lastNode));
        }
        trModel.reload();
    }

    private void AddFileEvent(final ComEvent comEvent) {
        if (!SwingUtilities.isEventDispatchThread()) {
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

        FileInfo file = epProc.id2FileInfo((Integer) comEvent.Params(2));
        int fileIndex = epProc.Id2Index(file.Id());


        TreeNode node;
        Integer logItemId;
        String title = "";
        DefaultTreeModel model = (DefaultTreeModel) trvw_Event.getModel();
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
        if ((Mod_EpProcessing.eComSubType) comEvent.Params(1) == Mod_EpProcessing.eComSubType.Processing) {
            node = new TreeNode();
            node.Name("File " + file.FileObj().getName() + ": [*]");
            node.setUserObject(node.Name().replace("[*]", "Processing started"));

            fileId2LogItemId.put(file.Id(), root.getChildCount());
            model.insertNodeInto(node, root, root.getChildCount());
            return;
        }

        logItemId = fileId2LogItemId.get(file.Id());
        if (logItemId == null || ((DefaultMutableTreeNode) root).getChildCount() <= logItemId) {
            return;
        }
        node = (TreeNode) root.getChildAt(logItemId);
        switch ((Mod_EpProcessing.eComSubType) comEvent.Params(1)) {
            case NoWriteAccess:
                title = "Fileaccess denied";
                node.add(new TreeNode(title, null));
                break;
            case GotFromHistory:
                title = "Got fileinformation from history";
                node.add(new TreeNode(title, null));
                break;
            case ParsingDone:
                //processedBytes = epProc.processedBytes();
                title = "AVDump processing done";
                FileParser fp = (FileParser) comEvent.Params(3);
                node.add(new TreeNode("Parsing done (" + (int) (file.FileObj().length() / 1024 / 1024) + "MB in " + (fp.ParseDuration() / 100 / 10d) + "s with " + fp.MBPerSecond() + "mb/s)"));
                break;
            case ParsingError:
                //processedBytes = epProc.processedBytes();
                title = "File Parsing error";
                node.add(new TreeNode("Parsing Error"));
                break;
            case GetDBInfo:
                if ((Boolean) comEvent.Params(3) || (Boolean) comEvent.Params(4)) {
                    title = "Sending packets to AniDB";
                    if ((Boolean) comEvent.Params(3) & (Boolean) comEvent.Params(4)) {
                        node.add(new TreeNode("Sending packets to AniDB (FILE and MYLIST)"));
                    } else if ((Boolean) comEvent.Params(3)) {
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
                node.add(new TreeNode("Couldn't rename file. " + (comEvent.ParamCount() == 5 ? (String) comEvent.Params(4) : "")));
                break;
            case FileRenamed:
                title = "File renamed";
                node.add(new TreeNode("File renamed to " + ((File) comEvent.Params(3)).getAbsolutePath() + ((Boolean) comEvent.Params(4) ? " (Truncated)" : "")));
                break;
            case RenamingNotNeeded:
                title = "No renaming needed";
                node.add(new TreeNode(title));
                break;
            case DeletedEmptyFolder:
                title = "Empty folder deleted";
                node.add(new TreeNode("Deleted empty folder: " + ((File) comEvent.Params(3)).getAbsolutePath()));
                break;
            case DeletetingEmptyFolderFailed:
                title = "Could not delete empty folder";
                node.add(new TreeNode("Could not delete empty folder: " + ((File) comEvent.Params(3)).getAbsolutePath() + ((Boolean) comEvent.Params(4) ? ". " + (String) comEvent.Params(4) : "")));
                break;
            case RelFilesRenamed:
                title = "Related files renamed";
                node.add(new TreeNode("Related files renamed (" + (String) comEvent.Params(3) + ")"));
                break;
            case RelFilesRenamingFailed:
                title = "Couldn't rename some or all of the related files";
                node.add(new TreeNode(title + ": " + (String) comEvent.Params(3)));
                break;
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

    public void Initialize(IAniAdd aniAdd, IGUI gui) {
        this.aniAdd = aniAdd;
        api = (Mod_UdpApi) aniAdd.GetModule("UdpApi");
        epProc = (Mod_EpProcessing) aniAdd.GetModule("EpProcessing");

        InitEventHandler();
    }

    private void InitEventHandler() {
        btn_CopyCmdTree.addActionListener(new java.awt.event.ActionListener() {

            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CopyTree(trvw_Cmd);
            }
        });
        btn_CopyLogTree.addActionListener(new java.awt.event.ActionListener() {

            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CopyTree(trvw_Event);
            }
        });
        btn_CopyDebugMsgs.addActionListener(new java.awt.event.ActionListener() {

            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CopyEvents();
            }
        });

        api.addComListener(new ComListener() {

            public void EventHandler(ComEvent comEvent) {
                if (comEvent.Type() == ComEvent.eType.Information) {
                    AddCmdEvent(comEvent);
                }
            }
        });

        epProc.addComListener(new ComListener() {

            public void EventHandler(ComEvent comEvent) {
                if (comEvent.Type() == ComEvent.eType.Information && comEvent.Params(0) instanceof eComType && comEvent.Params(0) == eComType.FileEvent) {
                    AddFileEvent(comEvent);
                }
            }
        });

        ComListener comListener;
        for (IModule module : aniAdd.GetModules()) {
            comListener = new ComListener() {

                public void EventHandler(ComEvent comEvent) {
                    comEvents.add(comEvent);
                }
            };
            module.addComListener(comListener);
        }

    }

    public void Terminate() {
    }

    public void GUIEventHandler(ComEvent comEvent) {
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        spnl_Logs = new javax.swing.JSplitPane();
        scrpn_trvw_Cmd = new javax.swing.JScrollPane();
        trvw_Cmd = new javax.swing.JTree();
        scrpn_trvw_Event = new javax.swing.JScrollPane();
        trvw_Event = new javax.swing.JTree();
        pnl_Logs_Ctrls = new javax.swing.JPanel();
        btn_CopyCmdTree = new javax.swing.JButton();
        btn_CopyLogTree = new javax.swing.JButton();
        btn_CopyDebugMsgs = new javax.swing.JButton();

        spnl_Logs.setDividerLocation(200);
        spnl_Logs.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        spnl_Logs.setResizeWeight(0.5);

        javax.swing.tree.DefaultMutableTreeNode treeNode1 = new javax.swing.tree.DefaultMutableTreeNode("Cmds");
        trvw_Cmd.setModel(new javax.swing.tree.DefaultTreeModel(treeNode1));
        scrpn_trvw_Cmd.setViewportView(trvw_Cmd);

        spnl_Logs.setTopComponent(scrpn_trvw_Cmd);

        treeNode1 = new javax.swing.tree.DefaultMutableTreeNode("Event Log");
        trvw_Event.setModel(new javax.swing.tree.DefaultTreeModel(treeNode1));
        scrpn_trvw_Event.setViewportView(trvw_Event);

        spnl_Logs.setRightComponent(scrpn_trvw_Event);

        pnl_Logs_Ctrls.setOpaque(false);

        btn_CopyCmdTree.setText("Copy Cmd Log Tree");

        btn_CopyLogTree.setText("Copy Event Log Tree");

        btn_CopyDebugMsgs.setText("Copy Debug Messages");

        javax.swing.GroupLayout pnl_Logs_CtrlsLayout = new javax.swing.GroupLayout(pnl_Logs_Ctrls);
        pnl_Logs_Ctrls.setLayout(pnl_Logs_CtrlsLayout);
        pnl_Logs_CtrlsLayout.setHorizontalGroup(
            pnl_Logs_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_Logs_CtrlsLayout.createSequentialGroup()
                .addComponent(btn_CopyCmdTree)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(btn_CopyLogTree)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(btn_CopyDebugMsgs)
                .addContainerGap(94, Short.MAX_VALUE))
        );
        pnl_Logs_CtrlsLayout.setVerticalGroup(
            pnl_Logs_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_Logs_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                .addComponent(btn_CopyCmdTree)
                .addComponent(btn_CopyLogTree)
                .addComponent(btn_CopyDebugMsgs))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(pnl_Logs_Ctrls, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(spnl_Logs, javax.swing.GroupLayout.DEFAULT_SIZE, 507, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(spnl_Logs, javax.swing.GroupLayout.DEFAULT_SIZE, 251, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(pnl_Logs_Ctrls, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
    }// </editor-fold>//GEN-END:initComponents
    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JButton btn_CopyCmdTree;
    protected javax.swing.JButton btn_CopyDebugMsgs;
    protected javax.swing.JButton btn_CopyLogTree;
    protected javax.swing.JPanel pnl_Logs_Ctrls;
    protected javax.swing.JScrollPane scrpn_trvw_Cmd;
    protected javax.swing.JScrollPane scrpn_trvw_Event;
    protected javax.swing.JSplitPane spnl_Logs;
    protected javax.swing.JTree trvw_Cmd;
    protected javax.swing.JTree trvw_Event;
    // End of variables declaration//GEN-END:variables

    public void GainedFocus() {
    }

    public void LostFocus() {
    }
}

package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.Communication.ComListener;
import aniAdd.IAniAdd;
import aniAdd.Modules.IModule.eModState;
import aniAdd.misc.Misc;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.KeyEvent;
import java.io.File;
import java.util.ArrayList;
import java.util.List;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.SwingUtilities;
import javax.swing.TransferHandler;
import javax.swing.filechooser.FileFilter;
import javax.swing.table.DefaultTableModel;
import processing.FileInfo;
import processing.Mod_EpProcessing;
import udpApi.Mod_UdpApi;

public class GUI_FileAdd extends javax.swing.JPanel implements GUI.ITab {
    final static boolean DEFWATCHED = true;
    final static boolean DEFADDTOML = true;
    final static boolean DEFRENAMEMOVE = false;
    final static int DEFSTORAGETYPE = 1;

    LabelUpdater labelUpdater = new LabelUpdater();

    private IGUI gui;
    private IAniAdd aniAdd;
    private Mod_EpProcessing epProc;
    private Mod_UdpApi api;

    private long processingStartOn;
    private long processingPausedOn;
    private long pausedTime;
    private long byteCount;
    //private long processedBytes;

    public GUI_FileAdd() {
        initComponents();
    }

    public void Initialize(IAniAdd aniAdd, final IGUI gui) {
        this.gui = gui;
        this.aniAdd = aniAdd;
        epProc = (Mod_EpProcessing)aniAdd.GetModule("EpProcessing");
        api = (Mod_UdpApi)aniAdd.GetModule("UdpApi");

        api.addComListener(new ComListener() {
            public void EventHandler(ComEvent comEvent) {
                if (comEvent.Type() == ComEvent.eType.Error || comEvent.Type() == ComEvent.eType.Fatal) {
                    LockDown();
                }
            }
        });
        epProc.addComListener(new ComListener() {
            public void EventHandler(ComEvent comEvent) {
                if (comEvent.Type() == ComEvent.eType.Information) {
                    if (comEvent.Params(0) == Mod_EpProcessing.eComType.FileEvent) {
                        int fileIndex = epProc.Id2Index((Integer) comEvent.Params(2));
                        ((DefaultTableModel) tbl_Files.getModel()).fireTableRowsUpdated(fileIndex, fileIndex);
                    } else if (comEvent.Params(0) == Mod_EpProcessing.eComType.FileCountChanged) {
                        byteCount = epProc.totalBytes();
                        //processedBytes = epProc.processedBytes();
                        ((DefaultTableModel) tbl_Files.getModel()).fireTableDataChanged();
                    } else if (comEvent.Params(0) == Mod_EpProcessing.eComType.Status) {
                        EpProcStatusEvent(comEvent);
                    }
                } else if (comEvent.Type() == ComEvent.eType.Error || comEvent.Type() == ComEvent.eType.Fatal) {
                    LockDown();
                }

            }
        });

        FileTable_TM tm = new gui.FileTable_TM(epProc);
        tbl_Files.setDefaultRenderer(Object.class, new FileTable_Renderer());
        tbl_Files.setModel(tm);
        tbl_Files.setRowHeight(19);
        tbl_Files.getColumnModel().getColumn(1).setMaxWidth(72);

        chck_MarkWatched.setSelected((Boolean) gui.FromMem("SetWatched", DEFWATCHED));
        chck_AddToML.setSelected((Boolean) gui.FromMem("AddToMyList", DEFADDTOML));
        chck_MarkWatched.setEnabled(chck_AddToML.isSelected());
        chck_RenameMoveFiles.setSelected((Boolean) gui.FromMem("RenameFiles", DEFRENAMEMOVE));
        chck_SetWatchedState.setSelected((Boolean) gui.FromMem("SetWatchedState", false));
        cmb_Storage.setSelectedIndex((Integer) gui.FromMem("SetStorageType", DEFSTORAGETYPE));
        txt_Other.setText((String) gui.FromMem("OtherText", ""));
        txt_Source.setText((String) gui.FromMem("SourceText", ""));
        txt_Storage.setText((String) gui.FromMem("StorageText", ""));

        UpdateSetWatchedState();
        UpdateFileInfoPane();
        UpdateEditBoxes();
        UpdateRenameMove();

        gui.SetDragnDropHandler(new FSTransfer());
    }

    private void LockDown() {
        if (true) {
            return;
        }
        btn_Start.setEnabled(false);
        btn_AddFiles.setEnabled(false);
        btn_AddFolders.setEnabled(false);
        gui.SetDragnDropHandler(null);
    }
    public void Terminate() {
        labelUpdater.Terminate();
    }
    public void GUIEventHandler(ComEvent comEvent) {
        if (comEvent.ParamCount() > 1 && "OptionChange".equals((String) comEvent.Params(0))) {
            if ("ShowFileInfoPane".equals((String) comEvent.Params(1))) {
                UpdateFileInfoPane();
            } else if ("ShowSrcStrOtEditBoxes".equals((String) comEvent.Params(1))) {
                UpdateEditBoxes();
            } else if ("EnableFileMoving".equals((String) comEvent.Params(1))) {
                UpdateRenameMove();
            } else if ("ShowSetWatchedStateBox".equals((String) comEvent.Params(1))) {
                UpdateSetWatchedState();

                Boolean watched; //refractor
                if((Boolean)gui.FromMem("ShowSetWatchedStateBox", false)){
                    watched = chck_SetWatchedState.isSelected()? chck_MarkWatched.isSelected() : null;
                } else {
                    watched = chck_MarkWatched.isSelected() ? true : null;
                }
                ToggleWatched(watched);
            }

        }
    }
    public String TabName() {
        return "FileAdd";
    }

    public int PreferredTabLocation() {
        return 0;
    }

    public void GainedFocus() {}
    public void LostFocus() {}

    class FSTransfer extends TransferHandler {

        static final long serialVersionUID = 0;

        public boolean importData(JComponent comp, Transferable t) {
            final List<File> files = GUI.transferableToFileList(t);
            final ArrayList<File> allFiles = new ArrayList<File>();
            if (files != null) {
                new Runnable() {

                    public void run() {
                        for (File sf : files) {
                            allFiles.addAll(Misc.getFiles(sf, Mod_EpProcessing.supportedFiles));
                        }
                    }
                }.run();
                AddFiles(allFiles);
                UpdateStatusLabels();
                UpdateProgressBars();
                tbl_Files.updateUI();
                return true;
            } else {
                return false;
            }
        }

        public boolean canImport(JComponent comp, DataFlavor[] transferFlavors) {
            return true;
        }
    }

    private void UpdateProgressBars() {
        UpdateProgressBars(FileProgress(), TotalProgress());
    }
    private void UpdateStatusLabels() {
        UpdateStatusLabels(byteCount, ProcByteCount(), ElapsedTime(), ETA());
    }
    private void UpdateProgressBars(double fileProg, double totalProg) {
        prg_File.setValue((int) (fileProg * 10000));
        prg_Total.setValue((int) (totalProg * 10000));
    }
    private void UpdateStatusLabels(long totalBytes, long totalProcessedBytes, long elapsed, long eta) {
        lbl_MBProcessed.setText((totalProcessedBytes / 1024 / 1024) + " of " + (totalBytes / 1024 / 1024) + "MB");
        lbl_TimeElapsed.setText("Time elapsed: " + Misc.longToTime(elapsed));
        lbl_TimeRemaining.setText("Time remaining: " + Misc.longToTime(eta));
    }
    private void EpProcStatusEvent(final ComEvent comEvent) {
        if (comEvent.Params(1) == Mod_EpProcessing.eProcess.Start) {
            if (processingStartOn == 0) {
                processingStartOn = System.currentTimeMillis();
            }
            labelUpdater.Start();

        } else if (comEvent.Params(1) == Mod_EpProcessing.eProcess.Pause) {
            processingPausedOn = System.currentTimeMillis();

        } else if (comEvent.Params(1) == Mod_EpProcessing.eProcess.Resume) {
            pausedTime += System.currentTimeMillis() - processingPausedOn;
            processingPausedOn = 0;

        } else if (comEvent.Params(1) == Mod_EpProcessing.eComSubType.Done) {
            SwingUtilities.invokeLater(new Runnable() {

                public void run() {
                    FileProcessingDone();
                    //UpdateProgressBars();
                    //UpdateStatusLabels();
                }
            });
        }
    }
    private void FileProcessingDone() {
        btn_Start.setText("Start");
    }
    private void EpProcessingDone() {
        btn_Clear.setVisible(true);
    }
    private double FileProgress() {
        return (double) epProc.processedBytesCurrentFile() / (double) epProc.totalBytesCurrentFile();
    }
    private double TotalProgress() {
        double prgValFile = (double) ProcByteCount() / (double) byteCount;
        int pendingFileCmds = (epProc.FileCount() - epProc.processedFileCount()) * ((Boolean) gui.FromMem("AddToMyList", DEFADDTOML) ? 2 : 1);
        int procCmd = api.totalCmdCount() - api.waitingCmdCount();
        double partialCmd = 1 - api.currendCmdDelay() / (double) api.cmdSendDelay();
        double prgValCmd = (procCmd + partialCmd) / (double) (api.totalCmdCount() + pendingFileCmds);
        return Math.min(prgValFile, prgValCmd);
    }
    private long ProcByteCount() {
        return epProc.processedBytes() + epProc.processedBytesCurrentFile();
    }
    private long ElapsedTime() {
        return processingStartOn != 0 ? System.currentTimeMillis() - processingStartOn - pausedTime : 0;
    }
    private long ETA() {
        long procByteCount = ProcByteCount();
        double etaFile = (double) (byteCount - procByteCount) / ((double) procByteCount / (double) ElapsedTime());

        int pendingFileCmds = (epProc.FileCount() - epProc.processedFileCount()) * ((Boolean) gui.FromMem("AddToMyList", DEFADDTOML) ? 2 : 1);
        double etaCmd = (api.waitingCmdCount() + pendingFileCmds) * api.cmdSendDelay() - api.cmdSendDelay() + api.currendCmdDelay();

        return (long) Math.max(etaFile, etaCmd);
    }

    private void RemoveFiles() {
        epProc.delFiles(tbl_Files.getSelectedRows());
    }
    private void ClearFiles() {
        epProc.ClearFiles();
    }
    private void AddPaths(boolean File) {
        JFileChooser FC = new javax.swing.JFileChooser();
        if (File) {
            FC.setFileSelectionMode(JFileChooser.FILES_ONLY);
        } else {
            FC.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        }

        String lastDirectory = (String) gui.FromMem("LastDirectory", null);
        if (lastDirectory != null) {
            FC.setCurrentDirectory(new File(lastDirectory));
        }

        FC.setMultiSelectionEnabled(true);
        FC.addChoosableFileFilter(new FileFilter() {

            public boolean accept(File file) {
                return file.isDirectory() || Misc.isVideoFile(file, Mod_EpProcessing.supportedFiles);
            }

            public String getDescription() {
                return "Video Files and Directories";
            }
        });

        if (FC.showOpenDialog(this) == javax.swing.JFileChooser.APPROVE_OPTION) {
            ArrayList<File> files = new ArrayList<File>();

            for (File sf : FC.getSelectedFiles()) {
                lastDirectory = sf.getParentFile().getAbsolutePath();
                files.addAll(Misc.getFiles(sf, Mod_EpProcessing.supportedFiles));
            }
            AddFiles(files);
            gui.ToMem("LastDirectory", lastDirectory);
        }

        UpdateStatusLabels();
        UpdateProgressBars();
        tbl_Files.updateUI();
    }
    private void AddFiles(ArrayList<File> files) {
        epProc.addFiles(files);
    }

    
    private void UpdateSetWatchedState() {
        boolean showSetWatchedStateBox = (Boolean) gui.FromMem("ShowSetWatchedStateBox", false);

        chck_SetWatchedState.setVisible(showSetWatchedStateBox);
        chck_SetWatchedState.setEnabled(chck_AddToML.isSelected());
        chck_MarkWatched.setText(showSetWatchedStateBox?"Watched":"Mark Watched");

        UpdateMarkWatched();
    }
    private void UpdateMarkWatched() {
        boolean showSetWatchedState = (Boolean) gui.FromMem("ShowSetWatchedStateBox", false);
        chck_MarkWatched.setEnabled((chck_AddToML.isSelected() && !showSetWatchedState) || (chck_AddToML.isSelected() && showSetWatchedState && chck_SetWatchedState.isSelected()));
    }
    
    private void UpdateFileInfoPane() {
        if ((Boolean) gui.FromMem("ShowFileInfoPane", false)) {
            spnl_FileAdd.setDividerLocation(spnl_FileAdd.getDividerLocation() - 100);
            pnl_FileAdd_FileInfo.setVisible(true);
            spnl_FileAdd.setDividerSize(3);
        } else {
            spnl_FileAdd.setDividerSize(0);
            pnl_FileAdd_FileInfo.setVisible(false);
            spnl_FileAdd.setDividerLocation(-1 * (int) pnl_FileAdd_Ctrls.getPreferredSize().getHeight());
        }
    }
    private void UpdateEditBoxes() {
        if ((Boolean) gui.FromMem("ShowSrcStrOtEditBoxes", false)) {
            pnl_EditBoxes.setVisible(true);
        } else {
            pnl_EditBoxes.setVisible(false);
        }
    }
    private void UpdateRenameMove() {
        boolean doRename = (Boolean) gui.FromMem("EnableFileRenaming", true);
        boolean doMove = (Boolean) gui.FromMem("EnableFileMove", false);

        if (!(doMove || doRename)) {
            chck_RenameMoveFiles.setVisible(false);
            chck_RenameMoveFiles.setSelected(false);
            ToggleFileRename(false);
        } else {
            chck_RenameMoveFiles.setVisible(true);
            chck_RenameMoveFiles.setText((doRename ? "Rename" : "") + (doRename && doMove ? "/" : "") + (doMove ? "Move" : "") + " File");
        }
    }

    private void ToggleMLCmd(boolean doAction) {
        SetOptions(doAction, new ISetOption() {

            public void setOption(Object type, FileInfo file) {
                if ((Boolean) type) {
                    file.ActionsTodo().add(FileInfo.eAction.MyListCmd);
                } else {
                    file.ActionsTodo().remove(FileInfo.eAction.MyListCmd);
                }
            }
        });
    }
    private void ToggleFileRename(boolean doAction) {
        SetOptions(doAction, new ISetOption() {

            public void setOption(Object type, FileInfo file) {
                if ((Boolean) type) {
                    file.ActionsTodo().add(FileInfo.eAction.Rename);
                } else {
                    file.ActionsTodo().remove(FileInfo.eAction.Rename);
                }
            }
        });
    }
    private void ToggleStorageType(int type) {
        SetOptions(type, new ISetOption() {

            public void setOption(Object type, FileInfo file) {
                file.MLStorage(FileInfo.eMLStorageState.values()[(Integer) type]);
            }
        });
    }
    private void ToggleWatched(Boolean doAction) {
        SetOptions(doAction, new ISetOption() {
            public void setOption(Object type, FileInfo file) {
                file.Watched((Boolean)type);
            }
        });
    }
    private void ToggleEditBox(final int box, String text) {
        SetOptions(text, new ISetOption() {
            int boxId = box;
            public void setOption(Object type, FileInfo file) {
                if (boxId == 0) {
                    file.Data().put("EditSource", (String) type);
                } else if (boxId == 1) {
                    file.Data().put("EditStorage", (String) type);
                } else if (boxId == 2) {
                    file.Data().put("EditOther", (String) type);
                }
            }
        });
    }
    private void SetOptions(Object type, ISetOption optionSetter) { //TODO: Refractor to remove first parameter
        int size = epProc.FileCount();
        for (int i = 0; i < size; i++) {
            FileInfo fileInfo = epProc.index2FileInfo(i);
            if (fileInfo.Served()) continue;
            optionSetter.setOption(type, fileInfo);
        }
    }
    private interface ISetOption { void setOption(Object type, FileInfo file); }
    
    private void ToggleProcessing(String type) {
        if (type.equals("Start")) {
            epProc.processing(Mod_EpProcessing.eProcess.Start);
        } else if (type.equals("Pause")) {
            epProc.processing(Mod_EpProcessing.eProcess.Pause);
        } else if (type.equals("Resume")) {
            epProc.processing(Mod_EpProcessing.eProcess.Resume);
        }
    }

    class LabelUpdater implements Runnable {

        Thread t;

        public void Start() {
            if (t == null || !t.isAlive()) {
                gui.LogEvent(ComEvent.eType.Debug, "Starting progress updater");
                t = new Thread(this);
                t.start();
            }
        }

        public void Terminate() {
            try {
                gui.LogEvent(ComEvent.eType.Debug, "Waiting for progress updater thread to terminate");
                if (t != null) {
                    t.join();
                }
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }

        public void run() {
            Runnable updater = new Runnable() {
                public void run() {
                    UpdateProgressBars();
                    UpdateStatusLabels();
                }
            };
            
            gui.LogEvent(ComEvent.eType.Debug, "Starting progress updater");
            while ((epProc.isProcessing() || api.waitingCmdCount() != 0) && gui.ModState() != eModState.Terminating) {
                do {
                    try { Thread.sleep(50); } catch (InterruptedException ex) { }
                } while (epProc.isPaused() && gui.ModState() != eModState.Terminating);

                if (gui.ModState() != eModState.Terminating) {
                    try { SwingUtilities.invokeAndWait(updater); } catch (Exception ex) { }
                }
            }

            SwingUtilities.invokeLater(new Runnable() {

                public void run() {
                    EpProcessingDone();
                }
            });
            processingStartOn = 0;
            pausedTime = 0;
            processingPausedOn = 0;

            gui.LogEvent(ComEvent.eType.Debug, "Progress updater thread terminated");
        }
    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        spnl_FileAdd = new javax.swing.JSplitPane();
        pnl_FileAdd_Lstvw = new javax.swing.JPanel();
        scr_tbl_Files = new javax.swing.JScrollPane();
        tbl_Files = new gui.components.JExtTable();
        pnl_FileAdd_Cont = new javax.swing.JPanel();
        pnl_FileAdd_Status = new javax.swing.JPanel();
        prg_File = new javax.swing.JProgressBar();
        prg_Total = new javax.swing.JProgressBar();
        lbl_MBProcessed = new javax.swing.JLabel();
        lbl_TimeElapsed = new javax.swing.JLabel();
        lbl_TimeRemaining = new javax.swing.JLabel();
        pnl_FileAdd_FileInfo = new javax.swing.JPanel();
        pnl_FileAdd_Ctrls = new javax.swing.JPanel();
        cmb_Storage = new javax.swing.JComboBox();
        chck_RenameMoveFiles = new javax.swing.JCheckBox();
        chck_SetWatchedState = new javax.swing.JCheckBox();
        chck_MarkWatched = new javax.swing.JCheckBox();
        chck_AddToML = new javax.swing.JCheckBox();
        pnl_EditBoxes = new javax.swing.JPanel();
        scrl_txt_Source = new javax.swing.JScrollPane();
        txt_Source = new gui.components.JHintTextArea();
        scrl_txt_Storage = new javax.swing.JScrollPane();
        txt_Storage = new gui.components.JHintTextArea();
        scrl_txt_Other = new javax.swing.JScrollPane();
        txt_Other = new gui.components.JHintTextArea();
        pnl_StartClear = new javax.swing.JPanel();
        btn_Clear = new javax.swing.JButton();
        btn_Start = new javax.swing.JButton();
        pnl_AddMedia = new javax.swing.JPanel();
        btn_AddFiles = new javax.swing.JButton();
        btn_AddFolders = new javax.swing.JButton();

        spnl_FileAdd.setDividerLocation(340);
        spnl_FileAdd.setDividerSize(2);
        spnl_FileAdd.setOrientation(javax.swing.JSplitPane.VERTICAL_SPLIT);
        spnl_FileAdd.setResizeWeight(1.0);
        spnl_FileAdd.setOpaque(false);

        pnl_FileAdd_Lstvw.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        pnl_FileAdd_Lstvw.setOpaque(false);

        tbl_Files.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {

            }
        ));
        tbl_Files.setFocusable(false);
        tbl_Files.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                tbl_FilesKeyPressed(evt);
            }
        });
        scr_tbl_Files.setViewportView(tbl_Files);

        javax.swing.GroupLayout pnl_FileAdd_LstvwLayout = new javax.swing.GroupLayout(pnl_FileAdd_Lstvw);
        pnl_FileAdd_Lstvw.setLayout(pnl_FileAdd_LstvwLayout);
        pnl_FileAdd_LstvwLayout.setHorizontalGroup(
            pnl_FileAdd_LstvwLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(scr_tbl_Files, javax.swing.GroupLayout.DEFAULT_SIZE, 728, Short.MAX_VALUE)
        );
        pnl_FileAdd_LstvwLayout.setVerticalGroup(
            pnl_FileAdd_LstvwLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(scr_tbl_Files, javax.swing.GroupLayout.DEFAULT_SIZE, 118, Short.MAX_VALUE)
        );

        spnl_FileAdd.setLeftComponent(pnl_FileAdd_Lstvw);

        pnl_FileAdd_Cont.setOpaque(false);

        pnl_FileAdd_Status.setOpaque(false);

        prg_File.setMaximum(10000);

        prg_Total.setMaximum(10000);

        lbl_MBProcessed.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        lbl_MBProcessed.setText("0 of 0 MB processed");

        lbl_TimeElapsed.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        lbl_TimeElapsed.setText("Time elapsed: 0:00:00");

        lbl_TimeRemaining.setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
        lbl_TimeRemaining.setText("Time remaining: 0:00:00");

        javax.swing.GroupLayout pnl_FileAdd_StatusLayout = new javax.swing.GroupLayout(pnl_FileAdd_Status);
        pnl_FileAdd_Status.setLayout(pnl_FileAdd_StatusLayout);
        pnl_FileAdd_StatusLayout.setHorizontalGroup(
            pnl_FileAdd_StatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(prg_File, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(prg_Total, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, pnl_FileAdd_StatusLayout.createSequentialGroup()
                .addComponent(lbl_MBProcessed, javax.swing.GroupLayout.PREFERRED_SIZE, 274, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lbl_TimeElapsed, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lbl_TimeRemaining, javax.swing.GroupLayout.PREFERRED_SIZE, 304, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pnl_FileAdd_StatusLayout.setVerticalGroup(
            pnl_FileAdd_StatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileAdd_StatusLayout.createSequentialGroup()
                .addComponent(prg_File, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, 0)
                .addComponent(prg_Total, javax.swing.GroupLayout.PREFERRED_SIZE, 14, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, 0)
                .addGroup(pnl_FileAdd_StatusLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_MBProcessed, javax.swing.GroupLayout.DEFAULT_SIZE, 15, Short.MAX_VALUE)
                    .addComponent(lbl_TimeElapsed, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(lbl_TimeRemaining)))
        );

        pnl_FileAdd_FileInfo.setBorder(javax.swing.BorderFactory.createTitledBorder("File Information"));
        pnl_FileAdd_FileInfo.setOpaque(false);

        javax.swing.GroupLayout pnl_FileAdd_FileInfoLayout = new javax.swing.GroupLayout(pnl_FileAdd_FileInfo);
        pnl_FileAdd_FileInfo.setLayout(pnl_FileAdd_FileInfoLayout);
        pnl_FileAdd_FileInfoLayout.setHorizontalGroup(
            pnl_FileAdd_FileInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        pnl_FileAdd_FileInfoLayout.setVerticalGroup(
            pnl_FileAdd_FileInfoLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );

        pnl_FileAdd_Ctrls.setBorder(javax.swing.BorderFactory.createEtchedBorder());
        pnl_FileAdd_Ctrls.setOpaque(false);

        cmb_Storage.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Unknown", "Internal (HDD)", "External (CD/DVD)", "Deleted" }));
        cmb_Storage.setSelectedIndex(1);
        cmb_Storage.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmb_StorageItemStateChanged(evt);
            }
        });

        chck_RenameMoveFiles.setText("Move/Rename Files");
        chck_RenameMoveFiles.setOpaque(false);
        chck_RenameMoveFiles.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_RenameMoveFilesActionPerformed(evt);
            }
        });

        chck_SetWatchedState.setIconTextGap(0);
        chck_SetWatchedState.setMargin(new java.awt.Insets(2, 2, 2, 0));
        chck_SetWatchedState.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_SetWatchedStateActionPerformed(evt);
            }
        });

        chck_MarkWatched.setText("Mark Watched");
        chck_MarkWatched.setOpaque(false);
        chck_MarkWatched.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_MarkWatchedActionPerformed(evt);
            }
        });

        chck_AddToML.setSelected(true);
        chck_AddToML.setText("Add File to MyList");
        chck_AddToML.setOpaque(false);
        chck_AddToML.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_AddToMLActionPerformed(evt);
            }
        });

        pnl_EditBoxes.setOpaque(false);

        txt_Source.setColumns(8);
        txt_Source.setLineWrap(true);
        txt_Source.setRows(3);
        txt_Source.setFont(new java.awt.Font("Tahoma", 0, 10)); // NOI18N
        txt_Source.setTextHint("Source");
        txt_Source.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_SourceKeyReleased(evt);
            }
        });
        scrl_txt_Source.setViewportView(txt_Source);

        txt_Storage.setColumns(8);
        txt_Storage.setLineWrap(true);
        txt_Storage.setRows(3);
        txt_Storage.setFont(new java.awt.Font("Tahoma", 0, 10)); // NOI18N
        txt_Storage.setTextHint("Storage");
        txt_Storage.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_StorageKeyReleased(evt);
            }
        });
        scrl_txt_Storage.setViewportView(txt_Storage);

        txt_Other.setColumns(8);
        txt_Other.setLineWrap(true);
        txt_Other.setRows(3);
        txt_Other.setFont(new java.awt.Font("Tahoma", 0, 10)); // NOI18N
        txt_Other.setTextHint("Other");
        txt_Other.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_OtherKeyReleased(evt);
            }
        });
        scrl_txt_Other.setViewportView(txt_Other);

        javax.swing.GroupLayout pnl_EditBoxesLayout = new javax.swing.GroupLayout(pnl_EditBoxes);
        pnl_EditBoxes.setLayout(pnl_EditBoxesLayout);
        pnl_EditBoxesLayout.setHorizontalGroup(
            pnl_EditBoxesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_EditBoxesLayout.createSequentialGroup()
                .addComponent(scrl_txt_Source, javax.swing.GroupLayout.DEFAULT_SIZE, 92, Short.MAX_VALUE)
                .addGap(0, 0, 0)
                .addComponent(scrl_txt_Storage, javax.swing.GroupLayout.DEFAULT_SIZE, 91, Short.MAX_VALUE)
                .addGap(0, 0, 0)
                .addComponent(scrl_txt_Other, javax.swing.GroupLayout.DEFAULT_SIZE, 92, Short.MAX_VALUE))
        );
        pnl_EditBoxesLayout.setVerticalGroup(
            pnl_EditBoxesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(scrl_txt_Source, javax.swing.GroupLayout.Alignment.TRAILING)
            .addComponent(scrl_txt_Other, javax.swing.GroupLayout.Alignment.TRAILING)
            .addComponent(scrl_txt_Storage, javax.swing.GroupLayout.Alignment.TRAILING)
        );

        pnl_StartClear.setOpaque(false);

        btn_Clear.setText("Clear");
        btn_Clear.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_ClearActionPerformed(evt);
            }
        });

        btn_Start.setText("Start");
        btn_Start.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_StartActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pnl_StartClearLayout = new javax.swing.GroupLayout(pnl_StartClear);
        pnl_StartClear.setLayout(pnl_StartClearLayout);
        pnl_StartClearLayout.setHorizontalGroup(
            pnl_StartClearLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(btn_Clear, 0, 77, Short.MAX_VALUE)
            .addComponent(btn_Start, javax.swing.GroupLayout.DEFAULT_SIZE, 77, Short.MAX_VALUE)
        );
        pnl_StartClearLayout.setVerticalGroup(
            pnl_StartClearLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, pnl_StartClearLayout.createSequentialGroup()
                .addComponent(btn_Start, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(0, 0, 0)
                .addComponent(btn_Clear))
        );

        pnl_AddMedia.setOpaque(false);

        btn_AddFiles.setText("Add Files");
        btn_AddFiles.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_AddFilesActionPerformed(evt);
            }
        });

        btn_AddFolders.setText("Add Folders");
        btn_AddFolders.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_AddFoldersActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pnl_AddMediaLayout = new javax.swing.GroupLayout(pnl_AddMedia);
        pnl_AddMedia.setLayout(pnl_AddMediaLayout);
        pnl_AddMediaLayout.setHorizontalGroup(
            pnl_AddMediaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(btn_AddFiles, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(btn_AddFolders, javax.swing.GroupLayout.DEFAULT_SIZE, 109, Short.MAX_VALUE)
        );
        pnl_AddMediaLayout.setVerticalGroup(
            pnl_AddMediaLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_AddMediaLayout.createSequentialGroup()
                .addComponent(btn_AddFolders)
                .addGap(0, 0, 0)
                .addComponent(btn_AddFiles)
                .addContainerGap())
        );

        javax.swing.GroupLayout pnl_FileAdd_CtrlsLayout = new javax.swing.GroupLayout(pnl_FileAdd_Ctrls);
        pnl_FileAdd_Ctrls.setLayout(pnl_FileAdd_CtrlsLayout);
        pnl_FileAdd_CtrlsLayout.setHorizontalGroup(
            pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileAdd_CtrlsLayout.createSequentialGroup()
                .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(cmb_Storage, javax.swing.GroupLayout.PREFERRED_SIZE, 134, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addGroup(pnl_FileAdd_CtrlsLayout.createSequentialGroup()
                        .addGap(6, 6, 6)
                        .addComponent(chck_AddToML, javax.swing.GroupLayout.PREFERRED_SIZE, 128, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pnl_FileAdd_CtrlsLayout.createSequentialGroup()
                        .addComponent(chck_SetWatchedState)
                        .addGap(0, 0, 0)
                        .addComponent(chck_MarkWatched, javax.swing.GroupLayout.PREFERRED_SIZE, 98, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(chck_RenameMoveFiles))
                .addGap(6, 6, 6)
                .addComponent(pnl_EditBoxes, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(pnl_AddMedia, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(0, 0, 0)
                .addComponent(pnl_StartClear, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
        );
        pnl_FileAdd_CtrlsLayout.setVerticalGroup(
            pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileAdd_CtrlsLayout.createSequentialGroup()
                .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                        .addComponent(pnl_AddMedia, javax.swing.GroupLayout.Alignment.LEADING, 0, 0, Short.MAX_VALUE)
                        .addComponent(pnl_StartClear, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                        .addGroup(pnl_FileAdd_CtrlsLayout.createSequentialGroup()
                            .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(cmb_Storage, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(chck_RenameMoveFiles))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addGroup(pnl_FileAdd_CtrlsLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(chck_MarkWatched, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(chck_SetWatchedState, javax.swing.GroupLayout.PREFERRED_SIZE, 23, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(chck_AddToML)))
                        .addComponent(pnl_EditBoxes, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                .addContainerGap())
        );

        javax.swing.GroupLayout pnl_FileAdd_ContLayout = new javax.swing.GroupLayout(pnl_FileAdd_Cont);
        pnl_FileAdd_Cont.setLayout(pnl_FileAdd_ContLayout);
        pnl_FileAdd_ContLayout.setHorizontalGroup(
            pnl_FileAdd_ContLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(pnl_FileAdd_FileInfo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(pnl_FileAdd_Status, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addComponent(pnl_FileAdd_Ctrls, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        pnl_FileAdd_ContLayout.setVerticalGroup(
            pnl_FileAdd_ContLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, pnl_FileAdd_ContLayout.createSequentialGroup()
                .addComponent(pnl_FileAdd_FileInfo, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(0, 0, 0)
                .addComponent(pnl_FileAdd_Status, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(0, 0, 0)
                .addComponent(pnl_FileAdd_Ctrls, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE))
        );

        spnl_FileAdd.setRightComponent(pnl_FileAdd_Cont);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 734, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addComponent(spnl_FileAdd, javax.swing.GroupLayout.DEFAULT_SIZE, 734, Short.MAX_VALUE)
                    .addGap(0, 0, 0)))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 242, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(spnl_FileAdd, javax.swing.GroupLayout.DEFAULT_SIZE, 242, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void cmb_StorageItemStateChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmb_StorageItemStateChanged
        gui.ToMem("SetStorageType", cmb_Storage.getSelectedIndex());
        ToggleStorageType(cmb_Storage.getSelectedIndex());
}//GEN-LAST:event_cmb_StorageItemStateChanged
    private void chck_RenameMoveFilesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_RenameMoveFilesActionPerformed
        gui.ToMem("RenameFiles", chck_RenameMoveFiles.isSelected());
        ToggleFileRename(chck_RenameMoveFiles.isSelected());
        tbl_Files.updateUI();
}//GEN-LAST:event_chck_RenameMoveFilesActionPerformed
    private void chck_MarkWatchedActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_MarkWatchedActionPerformed
        gui.ToMem("SetWatched", chck_MarkWatched.isSelected());

        //System.out.println(gui.FromMem("ShowSetWatchedStateBox", false) + "? " + chck_MarkWatched.isSelected() + "?true:null) : (" + chck_SetWatchedState.isSelected()+"?"+chck_MarkWatched.isSelected()+":null)");
        Boolean watched;
        if((Boolean)gui.FromMem("ShowSetWatchedStateBox", false)){
            watched = chck_SetWatchedState.isSelected()? chck_MarkWatched.isSelected() : null;
        } else {
            watched = chck_MarkWatched.isSelected() ? true : null;
        }

        ToggleWatched(watched);
}//GEN-LAST:event_chck_MarkWatchedActionPerformed
    private void chck_AddToMLActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_AddToMLActionPerformed
        gui.ToMem("AddToMyList", chck_AddToML.isSelected());
        UpdateSetWatchedState();
        ToggleMLCmd(chck_AddToML.isSelected());
        tbl_Files.updateUI();
}//GEN-LAST:event_chck_AddToMLActionPerformed
    private void btn_ClearActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_ClearActionPerformed
        ClearFiles();
        UpdateProgressBars();
        UpdateStatusLabels();
}//GEN-LAST:event_btn_ClearActionPerformed
    private void btn_StartActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_StartActionPerformed
        ToggleProcessing(btn_Start.getText());
        if (btn_Start.getText().equals("Start")) {
            btn_Start.setText("Pause");
            btn_Clear.setVisible(false);
        } else if (btn_Start.getText().equals("Pause")) {
            btn_Start.setText("Resume");
        } else if (btn_Start.getText().equals("Resume")) {
            btn_Start.setText("Pause");
        }
}//GEN-LAST:event_btn_StartActionPerformed
    private void btn_AddFilesActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_AddFilesActionPerformed
        AddPaths(true);
}//GEN-LAST:event_btn_AddFilesActionPerformed
    private void btn_AddFoldersActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_AddFoldersActionPerformed
        AddPaths(false);
}//GEN-LAST:event_btn_AddFoldersActionPerformed

    private void txt_SourceKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_SourceKeyReleased
        gui.ToMem("SourceText", txt_Source.getText());
        ToggleEditBox(0, txt_Source.getText());
    }//GEN-LAST:event_txt_SourceKeyReleased

    private void txt_StorageKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_StorageKeyReleased
        gui.ToMem("StorageText", txt_Storage.getText());
        ToggleEditBox(1, txt_Storage.getText());
    }//GEN-LAST:event_txt_StorageKeyReleased

    private void txt_OtherKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_OtherKeyReleased
        gui.ToMem("OtherText", txt_Other.getText());
        ToggleEditBox(2, txt_Other.getText());
    }//GEN-LAST:event_txt_OtherKeyReleased

    private void tbl_FilesKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_tbl_FilesKeyPressed
        if (btn_Start.getText().equals("Start") || true) {
            if (evt.getKeyCode() == KeyEvent.VK_DELETE) {
                RemoveFiles();
            }
        }
    }//GEN-LAST:event_tbl_FilesKeyPressed

    private void chck_SetWatchedStateActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_SetWatchedStateActionPerformed
        gui.ToMem("SetWatchedState", chck_SetWatchedState.isSelected());
        UpdateMarkWatched();
        
        Boolean watched;
        if((Boolean)gui.FromMem("ShowSetWatchedStateBox", false)){
            watched = chck_SetWatchedState.isSelected()? chck_MarkWatched.isSelected() : null;
        } else {
            watched = chck_MarkWatched.isSelected() ? true : null;
        }
        
        ToggleWatched(watched);
    }//GEN-LAST:event_chck_SetWatchedStateActionPerformed
    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JButton btn_AddFiles;
    protected javax.swing.JButton btn_AddFolders;
    private javax.swing.JButton btn_Clear;
    protected javax.swing.JButton btn_Start;
    protected javax.swing.JCheckBox chck_AddToML;
    protected javax.swing.JCheckBox chck_MarkWatched;
    protected javax.swing.JCheckBox chck_RenameMoveFiles;
    private javax.swing.JCheckBox chck_SetWatchedState;
    protected javax.swing.JComboBox cmb_Storage;
    protected javax.swing.JLabel lbl_MBProcessed;
    protected javax.swing.JLabel lbl_TimeElapsed;
    protected javax.swing.JLabel lbl_TimeRemaining;
    private javax.swing.JPanel pnl_AddMedia;
    protected javax.swing.JPanel pnl_EditBoxes;
    protected javax.swing.JPanel pnl_FileAdd_Cont;
    protected javax.swing.JPanel pnl_FileAdd_Ctrls;
    protected javax.swing.JPanel pnl_FileAdd_FileInfo;
    protected javax.swing.JPanel pnl_FileAdd_Lstvw;
    protected javax.swing.JPanel pnl_FileAdd_Status;
    private javax.swing.JPanel pnl_StartClear;
    protected javax.swing.JProgressBar prg_File;
    protected javax.swing.JProgressBar prg_Total;
    protected javax.swing.JScrollPane scr_tbl_Files;
    protected javax.swing.JScrollPane scrl_txt_Other;
    protected javax.swing.JScrollPane scrl_txt_Source;
    protected javax.swing.JScrollPane scrl_txt_Storage;
    protected javax.swing.JSplitPane spnl_FileAdd;
    protected gui.components.JExtTable tbl_Files;
    protected gui.components.JHintTextArea txt_Other;
    protected gui.components.JHintTextArea txt_Source;
    protected gui.components.JHintTextArea txt_Storage;
    // End of variables declaration//GEN-END:variables
}

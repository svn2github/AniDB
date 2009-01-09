/*
 * XaucSwing.java
 * Created on 2/Jan/2009, 20:19:31
 */
package clients;

import avparsing.AVParserOptions;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

import javax.swing.ImageIcon;
import javax.swing.JComponent;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.TransferHandler;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.table.DefaultTableModel;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeSelectionModel;

import structures.SwingFile;
import hashing.HasherOptions;
import ui.XaucFilesTreeSelectionListener;
import utils.DirectoryParser;
import utils.SwingLog;
import utils.SwingFileProcessor;
import utils.SwingProgress;

/** Xauc GUI (Swing) frontend */
public class XaucSwing extends JFrame implements WindowListener {
	private static final long serialVersionUID = 6222926566397039140L;
	
	protected static String appName = "xAUC";
	protected static int versionMajor = 0;
	protected static int versionMinor = 6;
	
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton buttonAddFile;
    private javax.swing.JButton buttonAddFolder;
    private javax.swing.JButton buttonHashesClear;
    private javax.swing.JButton buttonHashesCopy;
    private javax.swing.JButton buttonLogClear;
    private javax.swing.JButton buttonLogCopy;
    private javax.swing.JButton buttonRemoveAll;
    private javax.swing.JButton buttonStartHash;
    private javax.swing.JPanel filesFrame;
    private javax.swing.JScrollPane filesPane;
    private javax.swing.JTable filesTable;
    private javax.swing.JTextArea hashesArea;
    private javax.swing.JPanel hashesPane;
    private javax.swing.JScrollPane hashesScrollPane;
    private javax.swing.JPanel infoFilesPane;
    private javax.swing.JEditorPane infoPaneEditor;
    private javax.swing.JScrollPane infoScrollPane;
    private javax.swing.JTextArea logArea;
    private javax.swing.JPanel logPane;
    private javax.swing.JScrollPane logScrollPane;
    private javax.swing.JMenuBar menuBar;
    private javax.swing.JMenu menuEdit;
    private javax.swing.JMenuItem menuEditPreferences;
    private javax.swing.JMenu menuFile;
    private javax.swing.JMenuItem menuFileAddFile;
    private javax.swing.JMenuItem menuFileAddFolder;
    private javax.swing.JMenuItem menuFileExit;
    private javax.swing.JSeparator menuFileSeparator;
    private javax.swing.JMenu menuHelp;
    private javax.swing.JMenuItem menuHelpAbout;
    private javax.swing.JProgressBar progressFiles;
    private javax.swing.JProgressBar progressOverall;
    private javax.swing.JLabel statusIcon;
    private javax.swing.JTextField statusText;
    private javax.swing.JTabbedPane tabsPane;
    private javax.swing.JTree treeFiles;
    private javax.swing.JPanel treeFilesPane;
    private javax.swing.JScrollPane treeFilesScrollPane;
    private javax.swing.JPanel treePane;
    private javax.swing.JPanel workspacePane;
    // End of variables declaration//GEN-END:variables
	protected boolean disableFileAdd = false;
	protected HashMap<File,SwingFile> files = new HashMap<File,SwingFile>();
	protected HashMap<Integer,File> filesToRows = new HashMap<Integer,File>();
	protected HasherOptions hashingOptions = new HasherOptions();
	protected AVParserOptions parsingOptions = new AVParserOptions();
	protected boolean recurseDir = false;
	protected boolean seeDebug = false;
	protected boolean hashingEnabled = true;
	protected boolean parsingEnabled = false; // not enabled for the moment
	protected boolean writeXMLtoDisk = false;
	protected boolean writeFILEtoConsole = false;
	protected String appDir = System.getProperty("user.home") + File.separator + ".xauc";
	protected String logFile = appDir + File.separator + "log.txt";
	protected SwingLog log = new SwingLog(logFile);
	protected DirectoryParser dirparser = new DirectoryParser(true);
	protected File lastDirectory = null;
	protected Thread pthread = null; // file processor thread
	protected SwingFileProcessor fileProcessor = null;

	/** @return the hashingOptions */
	public synchronized HasherOptions getHashingOptions() { return hashingOptions; }
	/** @param hashingOptions the hashingOptions to set */
	public synchronized void setHashingOptions(HasherOptions hashingOptions) { this.hashingOptions = hashingOptions; }
	/** @return the parsingOptions */
	public synchronized AVParserOptions getParsingOptions() { return parsingOptions; }
	/** @param parsingOptions the parsingOptions to set */
	public synchronized void setParsingOptions(AVParserOptions parsingOptions) { this.parsingOptions = parsingOptions; }
	/** @return the seeDebug */
	public synchronized boolean isSeeDebug() { return seeDebug; }
	/** @param seeDebug the seeDebug to set */
	public synchronized void setSeeDebug(boolean seeDebug) { this.seeDebug = seeDebug; }
	/** @return the writeXMLtoDisk */
	public synchronized boolean isWriteXMLtoDisk() { return writeXMLtoDisk; }
	/** @param writeXMLtoDisk the writeXMLtoDisk to set */
	public synchronized void setWriteXMLtoDisk(boolean writeXMLtoDisk) { this.writeXMLtoDisk = writeXMLtoDisk; }
	/** @return the writeFILEtoConsole */
	public synchronized boolean isWriteFILEtoConsole() { return writeFILEtoConsole; }
	/** @param writeFILEtoConsole the writeFILEtoConsole to set */
	public synchronized void setWriteFILEtoConsole(boolean writeFILEtoConsole) { this.writeFILEtoConsole = writeFILEtoConsole; }

	/** Creates new form XaucSwing */
	public XaucSwing() {
		XaucConsole.checkAppDir();
		initComponents();
		setFilesTableLayout();
		createFilesTree();
		this.addWindowListener(this);
		attachDragAndDrop(this.filesTable,new FSTransfer());
		this.log.setLogArea(this.logArea);
		this.log.println("Initializing "+XaucSwing.appName+" (v"+XaucSwing.versionMajor+"."+XaucSwing.versionMinor+")");
	}

	/** This method is called from within the constructor to
	 * initialize the form.
	 * WARNING: Do NOT modify this code. The content of this method is
	 * always regenerated by the Form Editor.
	 */
	@SuppressWarnings({ "unchecked", "serial" })
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        statusIcon = new javax.swing.JLabel();
        statusText = new javax.swing.JTextField();
        tabsPane = new javax.swing.JTabbedPane();
        workspacePane = new javax.swing.JPanel();
        buttonAddFile = new javax.swing.JButton();
        buttonAddFolder = new javax.swing.JButton();
        buttonRemoveAll = new javax.swing.JButton();
        buttonStartHash = new javax.swing.JButton();
        filesFrame = new javax.swing.JPanel();
        filesPane = new javax.swing.JScrollPane();
        filesTable = new javax.swing.JTable();
        progressFiles = new javax.swing.JProgressBar();
        progressOverall = new javax.swing.JProgressBar();
        treePane = new javax.swing.JPanel();
        treeFilesPane = new javax.swing.JPanel();
        treeFilesScrollPane = new javax.swing.JScrollPane();
        treeFiles = new javax.swing.JTree();
        infoFilesPane = new javax.swing.JPanel();
        infoScrollPane = new javax.swing.JScrollPane();
        infoPaneEditor = new javax.swing.JEditorPane();
        hashesPane = new javax.swing.JPanel();
        hashesScrollPane = new javax.swing.JScrollPane();
        hashesArea = new javax.swing.JTextArea();
        buttonHashesClear = new javax.swing.JButton();
        buttonHashesCopy = new javax.swing.JButton();
        logPane = new javax.swing.JPanel();
        logScrollPane = new javax.swing.JScrollPane();
        logArea = new javax.swing.JTextArea();
        buttonLogClear = new javax.swing.JButton();
        buttonLogCopy = new javax.swing.JButton();
        menuBar = new javax.swing.JMenuBar();
        menuFile = new javax.swing.JMenu();
        menuFileAddFile = new javax.swing.JMenuItem();
        menuFileAddFolder = new javax.swing.JMenuItem();
        menuFileSeparator = new javax.swing.JSeparator();
        menuFileExit = new javax.swing.JMenuItem();
        menuEdit = new javax.swing.JMenu();
        menuEditPreferences = new javax.swing.JMenuItem();
        menuHelp = new javax.swing.JMenu();
        menuHelpAbout = new javax.swing.JMenuItem();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("xAUC - x AniDB UDP Client");
        setCursor(new java.awt.Cursor(java.awt.Cursor.DEFAULT_CURSOR));
        setMinimumSize(new java.awt.Dimension(740, 500));
        setName("mainFrame"); // NOI18N

        statusIcon.setFont(statusIcon.getFont().deriveFont(statusIcon.getFont().getSize()-1f));
        statusIcon.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/disconnect.png"))); // NOI18N
        statusIcon.setToolTipText("not connected to AniDB");

        statusText.setEditable(false);
        statusText.setFont(statusText.getFont().deriveFont(statusText.getFont().getSize()-1f));
        statusText.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                statusTextActionPerformed(evt);
            }
        });

        buttonAddFile.setFont(buttonAddFile.getFont().deriveFont(buttonAddFile.getFont().getSize()-1f));
        buttonAddFile.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_add.png"))); // NOI18N
        buttonAddFile.setText("Add files");
        buttonAddFile.setBorder(null);
        buttonAddFile.setContentAreaFilled(false);
        buttonAddFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonAddFileActionPerformed(evt);
            }
        });

        buttonAddFolder.setFont(buttonAddFolder.getFont().deriveFont(buttonAddFolder.getFont().getSize()-1f));
        buttonAddFolder.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/folder_add.png"))); // NOI18N
        buttonAddFolder.setText("Add directory");
        buttonAddFolder.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        buttonAddFolder.setContentAreaFilled(false);
        buttonAddFolder.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonAddFolderActionPerformed(evt);
            }
        });

        buttonRemoveAll.setFont(buttonRemoveAll.getFont().deriveFont(buttonRemoveAll.getFont().getSize()-1f));
        buttonRemoveAll.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_delete.png"))); // NOI18N
        buttonRemoveAll.setText("Remove all");
        buttonRemoveAll.setBorder(null);
        buttonRemoveAll.setContentAreaFilled(false);
        buttonRemoveAll.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonRemoveAllActionPerformed(evt);
            }
        });

        buttonStartHash.setFont(buttonStartHash.getFont().deriveFont(buttonStartHash.getFont().getSize()-1f));
        buttonStartHash.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_go.png"))); // NOI18N
        buttonStartHash.setText("Hash files");
        buttonStartHash.setBorder(javax.swing.BorderFactory.createEmptyBorder(1, 1, 1, 1));
        buttonStartHash.setContentAreaFilled(false);
        buttonStartHash.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonStartHashActionPerformed(evt);
            }
        });

        filesFrame.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Files and hashes", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 0, 10))); // NOI18N
        filesFrame.setAutoscrolls(true);
        filesFrame.setFont(filesFrame.getFont().deriveFont(filesFrame.getFont().getSize()-1f));

        filesTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Filename", "Hash", "State"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class, java.lang.String.class
            };
            boolean[] canEdit = new boolean [] {
                true, true, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        filesTable.setFillsViewportHeight(true);
        filesPane.setViewportView(filesTable);
        filesTable.getColumnModel().getColumn(1).setMinWidth(200);
        filesTable.getColumnModel().getColumn(1).setPreferredWidth(200);
        filesTable.getColumnModel().getColumn(1).setMaxWidth(200);
        filesTable.getColumnModel().getColumn(2).setMinWidth(100);
        filesTable.getColumnModel().getColumn(2).setPreferredWidth(100);
        filesTable.getColumnModel().getColumn(2).setMaxWidth(100);

        javax.swing.GroupLayout filesFrameLayout = new javax.swing.GroupLayout(filesFrame);
        filesFrame.setLayout(filesFrameLayout);
        filesFrameLayout.setHorizontalGroup(
            filesFrameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(filesPane, javax.swing.GroupLayout.DEFAULT_SIZE, 719, Short.MAX_VALUE)
        );
        filesFrameLayout.setVerticalGroup(
            filesFrameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(filesPane, javax.swing.GroupLayout.DEFAULT_SIZE, 316, Short.MAX_VALUE)
        );

        progressFiles.setFont(progressFiles.getFont().deriveFont(progressFiles.getFont().getSize()-1f));
        progressFiles.setStringPainted(true);

        progressOverall.setFont(progressOverall.getFont().deriveFont(progressOverall.getFont().getSize()-1f));
        progressOverall.setStringPainted(true);

        javax.swing.GroupLayout workspacePaneLayout = new javax.swing.GroupLayout(workspacePane);
        workspacePane.setLayout(workspacePaneLayout);
        workspacePaneLayout.setHorizontalGroup(
            workspacePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(workspacePaneLayout.createSequentialGroup()
                .addComponent(buttonAddFile, javax.swing.GroupLayout.PREFERRED_SIZE, 93, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(buttonAddFolder, javax.swing.GroupLayout.PREFERRED_SIZE, 109, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(buttonRemoveAll, javax.swing.GroupLayout.PREFERRED_SIZE, 98, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 326, Short.MAX_VALUE)
                .addComponent(buttonStartHash, javax.swing.GroupLayout.PREFERRED_SIZE, 97, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addGroup(workspacePaneLayout.createSequentialGroup()
                .addGap(5, 5, 5)
                .addComponent(progressOverall, javax.swing.GroupLayout.DEFAULT_SIZE, 720, Short.MAX_VALUE)
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, workspacePaneLayout.createSequentialGroup()
                .addGap(5, 5, 5)
                .addComponent(progressFiles, javax.swing.GroupLayout.DEFAULT_SIZE, 720, Short.MAX_VALUE)
                .addContainerGap())
            .addGroup(workspacePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(filesFrame, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        workspacePaneLayout.setVerticalGroup(
            workspacePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(workspacePaneLayout.createSequentialGroup()
                .addGroup(workspacePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(buttonAddFile, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(buttonAddFolder, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(buttonRemoveAll, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(buttonStartHash, javax.swing.GroupLayout.PREFERRED_SIZE, 28, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 354, Short.MAX_VALUE)
                .addComponent(progressFiles, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(progressOverall, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(5, 5, 5))
            .addGroup(workspacePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(workspacePaneLayout.createSequentialGroup()
                    .addGap(28, 28, 28)
                    .addComponent(filesFrame, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addContainerGap(52, Short.MAX_VALUE)))
        );

        tabsPane.addTab("Workspace", new javax.swing.ImageIcon(getClass().getResource("/icons/table.png")), workspacePane); // NOI18N

        treeFilesPane.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Tree", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 0, 10))); // NOI18N

        treeFiles.setFont(treeFiles.getFont().deriveFont(treeFiles.getFont().getSize()-1f));
        treeFiles.setShowsRootHandles(true);
        treeFilesScrollPane.setViewportView(treeFiles);

        javax.swing.GroupLayout treeFilesPaneLayout = new javax.swing.GroupLayout(treeFilesPane);
        treeFilesPane.setLayout(treeFilesPaneLayout);
        treeFilesPaneLayout.setHorizontalGroup(
            treeFilesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(treeFilesScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 254, Short.MAX_VALUE)
        );
        treeFilesPaneLayout.setVerticalGroup(
            treeFilesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(treeFilesScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 385, Short.MAX_VALUE)
        );

        infoFilesPane.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Information", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Tahoma", 0, 10))); // NOI18N

        infoPaneEditor.setEditable(false);
        infoPaneEditor.setFont(infoPaneEditor.getFont().deriveFont(infoPaneEditor.getFont().getSize()-1f));
        infoScrollPane.setViewportView(infoPaneEditor);

        javax.swing.GroupLayout infoFilesPaneLayout = new javax.swing.GroupLayout(infoFilesPane);
        infoFilesPane.setLayout(infoFilesPaneLayout);
        infoFilesPaneLayout.setHorizontalGroup(
            infoFilesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(infoScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 423, Short.MAX_VALUE)
        );
        infoFilesPaneLayout.setVerticalGroup(
            infoFilesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(infoScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 385, Short.MAX_VALUE)
        );

        javax.swing.GroupLayout treePaneLayout = new javax.swing.GroupLayout(treePane);
        treePane.setLayout(treePaneLayout);
        treePaneLayout.setHorizontalGroup(
            treePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(treePaneLayout.createSequentialGroup()
                .addContainerGap()
                .addComponent(treeFilesPane, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(infoFilesPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );
        treePaneLayout.setVerticalGroup(
            treePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, treePaneLayout.createSequentialGroup()
                .addGroup(treePaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(infoFilesPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(treeFilesPane, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap())
        );

        tabsPane.addTab("Files", new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_stack.png")), treePane); // NOI18N

        hashesArea.setColumns(20);
        hashesArea.setRows(5);
        hashesScrollPane.setViewportView(hashesArea);

        buttonHashesClear.setFont(buttonHashesClear.getFont().deriveFont(buttonHashesClear.getFont().getSize()-1f));
        buttonHashesClear.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_delete.png"))); // NOI18N
        buttonHashesClear.setText("Clear");
        buttonHashesClear.setContentAreaFilled(false);
        buttonHashesClear.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonHashesClearActionPerformed(evt);
            }
        });

        buttonHashesCopy.setFont(buttonHashesCopy.getFont().deriveFont(buttonHashesCopy.getFont().getSize()-1f));
        buttonHashesCopy.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_copy.png"))); // NOI18N
        buttonHashesCopy.setText("Copy");
        buttonHashesCopy.setContentAreaFilled(false);
        buttonHashesCopy.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonHashesCopyActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout hashesPaneLayout = new javax.swing.GroupLayout(hashesPane);
        hashesPane.setLayout(hashesPaneLayout);
        hashesPaneLayout.setHorizontalGroup(
            hashesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(hashesScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 735, Short.MAX_VALUE)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, hashesPaneLayout.createSequentialGroup()
                .addContainerGap(567, Short.MAX_VALUE)
                .addComponent(buttonHashesCopy)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(buttonHashesClear)
                .addContainerGap())
        );
        hashesPaneLayout.setVerticalGroup(
            hashesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, hashesPaneLayout.createSequentialGroup()
                .addComponent(hashesScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 385, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(hashesPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(buttonHashesClear)
                    .addComponent(buttonHashesCopy))
                .addGap(9, 9, 9))
        );

        tabsPane.addTab("Hashes", new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_link.png")), hashesPane); // NOI18N

        logArea.setColumns(20);
        logArea.setEditable(false);
        logArea.setRows(5);
        logScrollPane.setViewportView(logArea);

        buttonLogClear.setFont(buttonLogClear.getFont().deriveFont(buttonLogClear.getFont().getSize()-1f));
        buttonLogClear.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_delete.png"))); // NOI18N
        buttonLogClear.setText("Clear");
        buttonLogClear.setContentAreaFilled(false);
        buttonLogClear.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonLogClearActionPerformed(evt);
            }
        });

        buttonLogCopy.setFont(buttonLogCopy.getFont().deriveFont(buttonLogCopy.getFont().getSize()-1f));
        buttonLogCopy.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_copy.png"))); // NOI18N
        buttonLogCopy.setText("Copy");
        buttonLogCopy.setContentAreaFilled(false);
        buttonLogCopy.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                buttonLogCopyActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout logPaneLayout = new javax.swing.GroupLayout(logPane);
        logPane.setLayout(logPaneLayout);
        logPaneLayout.setHorizontalGroup(
            logPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(logScrollPane, javax.swing.GroupLayout.DEFAULT_SIZE, 735, Short.MAX_VALUE)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, logPaneLayout.createSequentialGroup()
                .addContainerGap(567, Short.MAX_VALUE)
                .addComponent(buttonLogCopy)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(buttonLogClear)
                .addContainerGap())
        );
        logPaneLayout.setVerticalGroup(
            logPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(logPaneLayout.createSequentialGroup()
                .addComponent(logScrollPane, javax.swing.GroupLayout.PREFERRED_SIZE, 385, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(logPaneLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(buttonLogClear)
                    .addComponent(buttonLogCopy))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        tabsPane.addTab("Log", new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_text.png")), logPane); // NOI18N

        menuBar.setName("menuBar"); // NOI18N

        menuFile.setText("File");
        menuFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileActionPerformed(evt);
            }
        });

        menuFileAddFile.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_F, java.awt.event.InputEvent.CTRL_MASK));
        menuFileAddFile.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/page_white_add.png"))); // NOI18N
        menuFileAddFile.setText("Add files");
        menuFileAddFile.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileAddFileActionPerformed(evt);
            }
        });
        menuFile.add(menuFileAddFile);

        menuFileAddFolder.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_D, java.awt.event.InputEvent.CTRL_MASK));
        menuFileAddFolder.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/folder_add.png"))); // NOI18N
        menuFileAddFolder.setText("Add directory");
        menuFileAddFolder.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileAddFolderActionPerformed(evt);
            }
        });
        menuFile.add(menuFileAddFolder);
        menuFile.add(menuFileSeparator);

        menuFileExit.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_X, java.awt.event.InputEvent.CTRL_MASK));
        menuFileExit.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/cross.png"))); // NOI18N
        menuFileExit.setText("Exit");
        menuFileExit.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                menuFileExitActionPerformed(evt);
            }
        });
        menuFile.add(menuFileExit);

        menuBar.add(menuFile);

        menuEdit.setText("Edit");

        menuEditPreferences.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_P, java.awt.event.InputEvent.CTRL_MASK));
        menuEditPreferences.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/cog.png"))); // NOI18N
        menuEditPreferences.setText("Preferences");
        menuEdit.add(menuEditPreferences);

        menuBar.add(menuEdit);

        menuHelp.setText("Help");

        menuHelpAbout.setAccelerator(javax.swing.KeyStroke.getKeyStroke(java.awt.event.KeyEvent.VK_A, java.awt.event.InputEvent.CTRL_MASK));
        menuHelpAbout.setIcon(new javax.swing.ImageIcon(getClass().getResource("/icons/information.png"))); // NOI18N
        menuHelpAbout.setText("About");
        menuHelp.add(menuHelpAbout);

        menuBar.add(menuHelp);

        setJMenuBar(menuBar);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(statusIcon)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(statusText, javax.swing.GroupLayout.DEFAULT_SIZE, 720, Short.MAX_VALUE))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addComponent(tabsPane, javax.swing.GroupLayout.DEFAULT_SIZE, 740, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(460, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(statusText, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(statusIcon)))
            .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addComponent(tabsPane, javax.swing.GroupLayout.PREFERRED_SIZE, 455, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addContainerGap(24, Short.MAX_VALUE)))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void menuFileActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileActionPerformed
		// TODO add your handling code here:
    }//GEN-LAST:event_menuFileActionPerformed

    private void menuFileExitActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileExitActionPerformed
		XaucSwing.this.processWindowEvent(new WindowEvent(XaucSwing.this, WindowEvent.WINDOW_CLOSED));
    }//GEN-LAST:event_menuFileExitActionPerformed

	private void menuFileAddFileActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileAddFileActionPerformed
		addFileAction();
	}//GEN-LAST:event_menuFileAddFileActionPerformed

	private void menuFileAddFolderActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_menuFileAddFolderActionPerformed
		addFolderAction();
	}//GEN-LAST:event_menuFileAddFolderActionPerformed

	private void buttonAddFolderActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonAddFolderActionPerformed
		addFolderAction();
}//GEN-LAST:event_buttonAddFolderActionPerformed

	private void buttonAddFileActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonAddFileActionPerformed
		addFileAction();
}//GEN-LAST:event_buttonAddFileActionPerformed

	private void buttonRemoveAllActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonRemoveAllActionPerformed
		removeFilesAction();
}//GEN-LAST:event_buttonRemoveAllActionPerformed

	private void statusTextActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_statusTextActionPerformed
		// TODO add your handling code here:
}//GEN-LAST:event_statusTextActionPerformed

	private void buttonStartHashActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonStartHashActionPerformed
		startProcessing();
}//GEN-LAST:event_buttonStartHashActionPerformed

	private void buttonLogClearActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonLogClearActionPerformed
		this.logArea.setText("");
	}//GEN-LAST:event_buttonLogClearActionPerformed

	private void buttonLogCopyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonLogCopyActionPerformed
		this.logArea.copy();
	}//GEN-LAST:event_buttonLogCopyActionPerformed

	private void buttonHashesCopyActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonHashesCopyActionPerformed
		this.hashesArea.copy();
}//GEN-LAST:event_buttonHashesCopyActionPerformed

	private void buttonHashesClearActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_buttonHashesClearActionPerformed
		this.hashesArea.setText("");
	}//GEN-LAST:event_buttonHashesClearActionPerformed

	/**
	 * @param args the command line arguments
	 */
	public static void main(String args[]) {
		try {
			// Set platform Java L&F
			UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
		} catch (UnsupportedLookAndFeelException e) {
			//return;
		} catch (Exception e) {
			//return;
		}
		java.awt.EventQueue.invokeLater(new Runnable() {

			public void run() {
				new XaucSwing().setVisible(true);
			}
		});
	}

	/**
	 * This method sets the layout of the files table
	 */
	public void setFilesTableLayout() {
		this.filesTable.addKeyListener(new java.awt.event.KeyAdapter() {
			public void keyPressed(KeyEvent evt) {
				if (evt.getKeyCode() == KeyEvent.VK_DELETE) deleteFileAction();
			}
		});
		this.filesTable.setFillsViewportHeight(true);
		this.filesTable.getColumnModel().getColumn(2).setCellRenderer(new ui.ProcessingCellRenderer());
	}

	/**
	 * Creates the default file tree
	 */
	public void createFilesTree() {
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("xauc");
		root.add(new DefaultMutableTreeNode("unidentified"));
		root.add(new DefaultMutableTreeNode("identified"));
		DefaultTreeModel model = ((DefaultTreeModel)this.treeFiles.getModel());
		model.setRoot(root);
		this.treeFiles.getSelectionModel().setSelectionMode(TreeSelectionModel.SINGLE_TREE_SELECTION);
		ImageIcon leafIcon = new ImageIcon(XaucSwing.class.getResource("/icons/page_white.png"));
		DefaultTreeCellRenderer renderer = new DefaultTreeCellRenderer();
		if (leafIcon != null) { renderer.setLeafIcon(leafIcon); }
		this.treeFiles.setCellRenderer(renderer);
		this.treeFiles.addTreeSelectionListener(new XaucFilesTreeSelectionListener(this.treeFiles,this.infoPaneEditor,this.log));
	}
	
	/**
	 * This method attaches Drag&Drop events to a given JComponent (not top level)<br />
	 * @param c The Component that will have the D&D even attached
	 * @param handler The transfer handler
	 */
	public void attachDragAndDrop(JComponent c, TransferHandler handler) {
		if(c instanceof JList)
			((JList)c).setDragEnabled(false);
		c.setTransferHandler(handler);
	}

	/**
	 * Class that actualy checks the transfers and sees if they are files or not
	 * @param t The transferable object
	 * @return A List of files
	 */
	private static List<File> transferableToFileList(Transferable t){
		try			{
			List<File> files=new LinkedList<File>();
			if(t.isDataFlavorSupported(DataFlavor.stringFlavor)){
				String data = (String)t.getTransferData(DataFlavor.stringFlavor);
				BufferedReader buf=new BufferedReader(new StringReader(data));
				String line;
				while((line=buf.readLine())!=null){
					if(line.startsWith("file:/")){
						line=line.substring("file:/".length());
						files.add(new File(line));
					} else
						System.out.println("Not sure how to read: "+line);
				}
				return files;
			} else if (t.isDataFlavorSupported(DataFlavor.javaFileListFlavor)){
				List<?> data = (List<?>)t.getTransferData(DataFlavor.javaFileListFlavor);
				Iterator<?> i = data.iterator();
				while (i.hasNext())
					files.add((File)i.next());
				return files;
			}
			return null;
		} catch (UnsupportedFlavorException e){
			e.printStackTrace();
		} catch (IOException e){
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * Transfer handler that checks if objects dragged are files and appends them to the filesTable
	 */
	public class FSTransfer extends TransferHandler {
		static final long serialVersionUID=0;
		public boolean importData(JComponent comp, Transferable t){
			final List<File> files=transferableToFileList(t);
			if(files!=null){
				new Runnable() {
					public void run(){
						for(File sf:files)
							addFileToFilesTable(sf);
					}}.run();
					return true;
			} else
				return false;
		}
		public boolean canImport(JComponent comp, DataFlavor[] transferFlavors) {
			return true;
		}
	}
	
	/**
	 * Method that deletes selected rows from the file list
	 */
	public void deleteFileAction(){
		int rows[] = this.filesTable.getSelectedRows();
		DefaultTableModel model = (DefaultTableModel) filesTable.getModel();
        
        for (int i = 0; i < rows.length; i++) {
        	int row = rows[i];
        	File file = this.filesToRows.get(row);
        	this.files.remove(file);
        	this.filesToRows.remove(row);
            model.removeRow(row);
            model.fireTableRowsDeleted(row, row);
        }
    }
	
	/**
	 * Shows a fileChooser dialog, paramenter controls what type it is
	 * @param fileMode true if dialog will be used to add a file, false for directories
	 */
	public void showFileChooser(boolean fileMode) {
		if (this.disableFileAdd) return;
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setAcceptAllFileFilterUsed(false);
		fileChooser.setCurrentDirectory(lastDirectory);
		fileChooser.setMultiSelectionEnabled(true);
		if (fileMode) {
			fileChooser.setDialogTitle("Add file(s)");
			fileChooser.setApproveButtonToolTipText("Add selected file");
			fileChooser.setFileSelectionMode(javax.swing.JFileChooser.FILES_AND_DIRECTORIES);
			fileChooser.setAcceptAllFileFilterUsed(true);
			fileChooser.addChoosableFileFilter(new xaucFilesFilter(dirparser.getExtensionFilter()));
		} else {
			fileChooser.setDialogTitle("Add folder(s)");
			fileChooser.setApproveButtonToolTipText("Add selected folder(s)");
			fileChooser.setFileSelectionMode(javax.swing.JFileChooser.DIRECTORIES_ONLY);
			fileChooser.setAcceptAllFileFilterUsed(false);
		}

		if (fileChooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION) {
			for(File sf:fileChooser.getSelectedFiles()){
				if (sf.isDirectory()) {
					ArrayList<File> files = dirparser.scanDirectory(sf);
					Iterator<File> it = files.iterator();
					while (it.hasNext())
						addFileToFilesTable(it.next());
				} else if (sf.isFile()) {
					addFileToFilesTable(sf);
				} else continue;
				lastDirectory = sf.getParentFile();
			}
		}		
	}	
	
	/**
	 * File filter class
	 */
	public class xaucFilesFilter extends javax.swing.filechooser.FileFilter {
		protected String extensionFilter = "";
		public xaucFilesFilter() {};
		public xaucFilesFilter(String extensionFilter) {
			this.extensionFilter = extensionFilter;
		}

        public boolean accept(File file) {
			if (file.isDirectory()) return true;
			String filename = file.getName();
			int i = filename.lastIndexOf(".");
			if(i<0) return false;
			String extension = filename.substring(i+1).toLowerCase();
			if (this.extensionFilter.indexOf(extension) < 0) return false;
			return true;
        }
        public String getDescription() {
            return "video files ("+this.extensionFilter+")";
        }
    }
	
	/**
	 * Action executed when deleting all files
	 */
	public void removeFilesAction() {
		if (this.disableFileAdd) return;
		DefaultTableModel model = (DefaultTableModel) filesTable.getModel();
		while (model.getRowCount() > 0) model.removeRow(0);
		this.files = new HashMap<File,SwingFile>();
		this.filesToRows = new HashMap<Integer,File>();
	}
	
	/**
	 * Action exectuted for adding files
	 */
	public void addFileAction() { showFileChooser(true); }

	/**
	 * Action exectuted for adding files from folder
	 */
	public void addFolderAction() { showFileChooser(false);	}

	public synchronized void startProcessing() {
		if (this.disableFileAdd) {
			synchronized(this) {
				if (!fileProcessor.suspended) {
					this.buttonStartHash.setText("Resume hashing");
					this.buttonStartHash.setToolTipText(null);
					fileProcessor.suspended = true;
				} else {
					this.buttonStartHash.setText("Pause hashing");
					this.buttonStartHash.setToolTipText("Pause will only occur after the current file has been processed");
					fileProcessor.suspended = false;
				}
			}
			return;
		}
		if (this.files.isEmpty()) return;
		this.statusText.setText("file processing: started");
		this.disableFileAdd = true;
		this.buttonStartHash.setText("Pause hashing");
		this.buttonStartHash.setToolTipText("Pause will only occur after the current file has been processed");
		SwingProgress mainProgressBar = new SwingProgress();
		mainProgressBar.setProgressBar(this.progressOverall);
		SwingProgress fileProgressBar = new SwingProgress();
		fileProgressBar.setProgressBar(this.progressFiles);
		//this.parsingOptions.setFullParse(true);
		fileProcessor = new SwingFileProcessor(this.files,this.hashingOptions,this.parsingOptions);
		fileProcessor.setMainProgressBar(mainProgressBar);
		fileProcessor.setFileProgressBar(fileProgressBar);
		fileProcessor.setParent(this);
		fileProcessor.setFilesTable(this.filesTable);
		fileProcessor.setLog(this.log);
		fileProcessor.setHashesArea(this.hashesArea);
		fileProcessor.setFilesTree(this.treeFiles);
		fileProcessor.setEnabled(true);
	}
	
	/**
	 * File Processing has completed (signaled from the file processor).<br />
	 * Reset files.
	 */
	public void processingDone() {
		this.disableFileAdd = false;
		this.buttonStartHash.setText("Hash files");
		this.buttonStartHash.setToolTipText(null);
		this.statusText.setText("file processing: complete");
	}
	
	/**
	 * Adds a file to the FilesTable
	 * @param file File to add
	 */
	public void addFileToFilesTable(File file) {
		if (this.files.containsKey(file)) return; // file already exists
		DefaultTableModel model = (DefaultTableModel) filesTable.getModel();
		model.addRow(new Object[]{file.getName(),"","queued"});
		int row = model.getRowCount()-1;
		SwingFile sf = new SwingFile(file,row);
		sf.setADDED();
		this.files.put(file, sf);
		this.filesToRows.put(row, file);
	}
		
	@Override
	public void windowOpened(WindowEvent e) {
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public void windowClosing(WindowEvent e) {
		log.println("Quiting "+XaucSwing.appName+", have a nice day.");
	//throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public void windowClosed(WindowEvent e) {
		// Do some clean up
		System.out.println("Closed window");
		// Now exit
		System.exit(0);
	}

	@Override
	public void windowIconified(WindowEvent e) {
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public void windowDeiconified(WindowEvent e) {
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public void windowActivated(WindowEvent e) {
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public void windowDeactivated(WindowEvent e) {
		//throw new UnsupportedOperationException("Not supported yet.");
	}
}

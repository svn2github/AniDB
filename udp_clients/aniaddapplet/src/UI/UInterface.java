/**
 * @author Arokh
 * @author Mahogny
 * @email DvdKhl@googlemail.com
 */
package UI;

import AniAdd.AniAdd;
import AniAdd.Misc;
import Processing.EpProcessing;
import Processing.cFileInfo;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Rectangle;
import java.awt.event.KeyEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.Iterator;
import java.util.LinkedList;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.util.List;

//Applet user interface
public class UInterface extends javax.swing.JPanel {
    
    public final AniAdd aniadd;   
    private final UI.ListView lstvw_Files=new UI.ListView();
    private final JProgressBar[] prgbr_Progress = new JProgressBar[] {new JProgressBar(), new JProgressBar()};
    public final JCheckBox checkFilesWatched = new javax.swing.JCheckBox("Mark Watched");
    public final JCheckBox checkRenameFiles = new javax.swing.JCheckBox("Rename Files");
    private final JComboBox comboState = new javax.swing.JComboBox();
    private final JButton bStart = new JButton("Start");
    private final JButton bRemove = new JButton("Remove");
    private final JLabel lblProcessedMB = new JLabel("0 of 0 MB processed", JLabel.CENTER);
    private final JLabel lblTimeElapsed = new JLabel("Time elapsed: 0:00:00", JLabel.CENTER);
    private final JLabel lblTimeRemaining = new JLabel("Time remaining: 0:00:00", JLabel.CENTER);
    
    private UIRenameStyle RenameDLG;
    
    public File lastDirectory=null; //Last directory from which a file was opened

    public UInterface(AniAdd aniadd) {
        this.aniadd=aniadd;
        
        lstvw_Files.SetColumns(new String[] {"Filename","Progress", "Is Renamed"});
        lstvw_Files.getColumnModel().getColumn(1).setPreferredWidth(80);
        lstvw_Files.getColumnModel().getColumn(1).setWidth(80);
        lstvw_Files.getColumnModel().getColumn(1).setMaxWidth(80);
        lstvw_Files.getColumnModel().getColumn(2).setPreferredWidth(100);
        lstvw_Files.getColumnModel().getColumn(2).setWidth(100);
        lstvw_Files.getColumnModel().getColumn(2).setMaxWidth(100);

        comboState.setModel(new javax.swing.DefaultComboBoxModel(new String[] {"Unknown", "On HDD", "OnCD", "Deleted" })); //currently relying on item order
        comboState.setSelectedIndex(1);
        
        
        JScrollPane scrollPane = new javax.swing.JScrollPane();
        scrollPane.setViewportView(lstvw_Files);
        
        JButton bFile=new JButton("Add File");
        JButton bDir=new JButton("Add Dir");
        JButton bRenamePattern=new JButton("Rename Config");
                      
        JPanel lowerPanel = new javax.swing.JPanel(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.HORIZONTAL;
          
        c.gridx=0; c.gridy=0;
        c.gridwidth = GridBagConstraints.REMAINDER;
        lowerPanel.add(prgbr_Progress[0],c); c.gridy++;
        lowerPanel.add(prgbr_Progress[1],c); c.gridy++;
        
        JPanel innerPanel = new javax.swing.JPanel(new GridBagLayout());
        GridBagConstraints cInner = new GridBagConstraints();
        cInner.gridx=0; cInner.gridy=0;
        cInner.fill = GridBagConstraints.HORIZONTAL;
        cInner.weightx = 1;
        innerPanel.add(lblProcessedMB, cInner); cInner.gridx++;
        innerPanel.add(lblTimeElapsed, cInner); cInner.gridx++;
        innerPanel.add(lblTimeRemaining, cInner); cInner.gridx++;
        lowerPanel.add(innerPanel,c); c.gridy++;
        
        c.gridwidth=1;
        c.weightx=1; lowerPanel.add(withLabel("Set state",comboState),c);
        c.weightx=0; c.gridx++; lowerPanel.add(checkFilesWatched,c);
        c.gridx++; lowerPanel.add(checkRenameFiles,c);
        c.gridx++; lowerPanel.add(bRenamePattern,c);
        c.gridx++; lowerPanel.add(bFile,c);
        c.gridx++; lowerPanel.add(bDir,c);
        c.gridx++; lowerPanel.add(bRemove,c);
        c.gridx++; lowerPanel.add(bStart,c);
        c.gridx++;
        
        setLayout(new BorderLayout());
        add(scrollPane,BorderLayout.CENTER);
        add(lowerPanel,BorderLayout.SOUTH);
        
        
        
        bFile.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                actionAddFile();
            }
        });
        bDir.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                actionAddDir();
            }
        });
        final AniAdd a=this.aniadd;
        bStart.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                if (bStart.getText().equals("Start")) {                    
                    bStart.setText("Pause");
                    a.EpProc.Processing(EpProcessing.eProcess.Start);
                } else if (bStart.getText().equals("Pause")) {
                    bStart.setText("Resume");
                    a.EpProc.Processing(EpProcessing.eProcess.Pause);
                } else if (bStart.getText().equals("Resume")) {
                    bStart.setText("Pause");
                    a.EpProc.Processing(EpProcessing.eProcess.Resume);
                }
            }
        });
        bRenamePattern.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                if (RenameDLG == null) RenameDLG = new UIRenameStyle(a);
                
                if (!RenameDLG.isVisible()) {
                    RenameDLG.setSize(214, 180);
                    RenameDLG.setVisible(true);
                } else {
                    RenameDLG.setVisible(false);
                }
            }
        });
        checkRenameFiles.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                for(int I=0; I < lstvw_Files.getRowCount(); I++) {editFile(I);}
            }
        });
        lstvw_Files.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                if (evt.getKeyCode() == KeyEvent.VK_DELETE)
                    deleteAction();
            }
        });
        bRemove.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                deleteAction();
            }
        });
        attachDragAndDrop(this);
    }

    public void deleteAction(){
        int Rows[] = lstvw_Files.getSelectedRows();
        
        for(int I=Rows.length-1;I>=0; I--) {
            aniadd.EpProc.DelFile(Rows[I]);
            lstvw_Files.Items.Remove(Rows[I]);            
        }
    }

    //Handle drag and drop of files to JList
    public void attachDragAndDrop(JComponent c) {
        if(c instanceof JList)
            ((JList)c).setDragEnabled(false);
        c.setTransferHandler(new FSTransfer());
    }
    @SuppressWarnings("unchecked") public static List<File> transferableToFileList(Transferable t){
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
                List data = (List)t.getTransferData(DataFlavor.javaFileListFlavor);
                Iterator i = data.iterator();
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
    private class FSTransfer extends TransferHandler {
        static final long serialVersionUID=0;
        public boolean importData(JComponent comp, Transferable t){
            final List<File> files=transferableToFileList(t);
            if(files!=null){
                new Runnable() {
                    public void run(){
                         for(File sf:files)
                            aniadd.EpProc.AddFiles(Misc.GetFiles(sf));
                    }}.run();
                    return true;
            } else
                return false;
        }
        public boolean canImport(JComponent comp, DataFlavor[] transferFlavors) {
            return true;
        }
    }
    

    //Add a label to the left of a swing component
    public static JPanel withLabel(String s, JComponent c){
        JPanel p=new JPanel(new BorderLayout());
        p.add(new JLabel(s),BorderLayout.WEST);
        p.add(c,BorderLayout.CENTER);
        return p;
    }
  
    
    
    //Bring up dialog to add files
    public void actionAddFile() {
        addFileDir(true);
     }
    //Bring up dialog to add files
    public void actionAddDir() {
        addFileDir(false);
     }
    //Bring up dialog to add files
    private void addFileDir(boolean File) {
        /**
        FileDialog FC=new FileDialog(new Frame());
        FC.setFilenameFilter(new FilenameFilter() {
            public boolean accept(File file, String string) {
                return file.isDirectory() || EpProcessing.isVideoFile(file);
            }
        });
        FC.setVisible(true);
        System.out.println(""+FC.getDirectory()+" "+FC.getFile());
        if(FC.getDirectory()!=null) {
            File sf=FC.getFile()==null ? new File(FC.getDirectory()) : new File(FC.getDirectory(),FC.getFile());
            System.out.println(" "+sf.getPath());
            aniadd.EpProc.AddFiles(Misc.GetFiles(sf));
        }
         **/
        
        javax.swing.JFileChooser FC = new javax.swing.JFileChooser();
        if (File) {
            FC.setFileSelectionMode(javax.swing.JFileChooser.FILES_ONLY);
        } else {
            FC.setFileSelectionMode(javax.swing.JFileChooser.DIRECTORIES_ONLY);
        }
        if(lastDirectory!=null)
            FC.setCurrentDirectory(lastDirectory);
        
        FC.setMultiSelectionEnabled(true);
        FC.addChoosableFileFilter(new FileFilter() {
            public boolean accept(File file) {return file.isDirectory() || EpProcessing.isVideoFile(file);}
            public String getDescription() {return "Video Files and Directories";}
        });
        FC.setMultiSelectionEnabled(true);
        
        if (FC.showOpenDialog(aniadd) == javax.swing.JFileChooser.APPROVE_OPTION) {
           for(File sf:FC.getSelectedFiles()){
               lastDirectory=sf.getParentFile();
               aniadd.EpProc.AddFiles(Misc.GetFiles(sf));
           }
        }
     }
    

    //Add File to ListView
    public void addFileToList(int FileIndex) {
        lstvw_Files.Items.Add(getRowStyle(aniadd.EpProc.Files.get(FileIndex)));
    }
    
    public void editFile(int FileIndex) {
        lstvw_Files.Items.Edit(getRowStyle(aniadd.EpProc.Files.get(FileIndex)), FileIndex);
        
        if (aniadd.EpProc.Files.get(FileIndex).FileState == cFileInfo.eFileState.processing) {
            Rectangle rect = lstvw_Files.getCellRect(FileIndex, 0, true);
            lstvw_Files.scrollRectToVisible(rect);
        }
    }
    public void updateProgressBars(int TProg, int FProg){
        prgbr_Progress[0].setValue(TProg);
        if (FProg != -1) prgbr_Progress[1].setValue(FProg);
    }
    
    public void updateETA(long totalMB, long processedMB, long elapsedtime) {
              
        if (elapsedtime/1000 != 0 && processedMB != 0) {
            long secs, h,m;
            long secLeft=(long)(((float)totalMB-(float)processedMB)/((float)processedMB/(float)elapsedtime));
            
            secs=secLeft/1000;h=secs/3600; secs-=h*3600;m=secs/60; secs-=m*60;
            lblTimeRemaining.setText("Time remaining: " + ((h<=9)?("0"+h):h) + ":" + ((m<=9)?("0"+m):m) + ":" + ((secs<=9)?("0"+secs):secs));
            
            secs=elapsedtime/1000; h=secs/3600; secs-=h*3600; m=secs/60; secs-=m*60;
            lblTimeElapsed.setText("Time elapsed: " + ((h<=9)?("0"+h):h) + ":" + ((m<=9)?("0"+m):m) + ":" + ((secs<=9)?("0"+secs):secs));
        }
        lblProcessedMB.setText(processedMB + " of " + totalMB +  " MB processed");
    }
    public int getFileState() {return comboState.getSelectedIndex();};
    public boolean isWatched() {return checkFilesWatched.isSelected();};
    public boolean shouldRename() {return checkRenameFiles.isSelected();};
    
    public void ProcessingDone() { bStart.setText("Start");}
    
    public cListViewItemRow getRowStyle(Processing.cFileInfo FileInfo) {
        cListViewItemRow Item = new cListViewItemRow();
        cListViewItemCell Cell = new cListViewItemCell();
        
        if (FileInfo.FileState == cFileInfo.eFileState.unprocessed) {
            Cell.BGColor = null;
        } else if (FileInfo.FileState == cFileInfo.eFileState.processing) {
            Cell.BGColor = Color.yellow;
        } else if (FileInfo.FileState == cFileInfo.eFileState.processed) {
            Cell.BGColor = Color.CYAN;
        } else if (FileInfo.FileState == cFileInfo.eFileState.gotfileinfo) {
            Cell.BGColor = Color.green;
        } else if (FileInfo.FileState == cFileInfo.eFileState.error) {
            Cell.BGColor = Color.red;
        }
        Cell.Value = FileInfo.FilePath.getName();
        Item.Columns.add(Cell);
        
        Cell = new cListViewItemCell();
        if (FileInfo.MLState == cFileInfo.eMyListState.queued) {
            if (FileInfo.FileState == cFileInfo.eFileState.unprocessed) {
                Cell.BGColor = null;
                Cell.Value = "Queued";
            } else {
                Cell.BGColor = Color.yellow;
                Cell.Value = "Identifying";
            }
        } else if (FileInfo.MLState == cFileInfo.eMyListState.inmylist) {
            Cell.BGColor = Color.green;
            Cell.Value = "In MyList";
        } else if (FileInfo.MLState == cFileInfo.eMyListState.unknown) {
            Cell.BGColor = Color.red;
            Cell.Value = "Unknown File";
        }
        Item.Columns.add(Cell);
        
        Cell = new cListViewItemCell();
        if (FileInfo.Data.containsKey("IsRenamed")) {
            if (FileInfo.Data.get("IsRenamed").equals("Yes")) {
                Cell.Value = "Yes";
                Cell.BGColor = Color.green;
            } else if (FileInfo.Data.get("IsRenamed").equals("Error")) {
                Cell.Value = "Error";
                Cell.BGColor = Color.red;
            }
        } else {
            if (shouldRename()) {
                Cell.BGColor = null;
                Cell.Value = "Missing Fileinfo";
            } else {
                Cell.BGColor = null;
                Cell.Value = "No Filerenaming";
            }
        }
        Item.Columns.add(Cell);
        
        return Item;
    }
}

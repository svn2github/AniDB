package gui;

import aniAdd.Communication.ComEvent;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;

public class GUI_Options_Misc extends javax.swing.JPanel {

    private IGUI gui;

    public GUI_Options_Misc() {
        initComponents();
    }

    public GUI_Options_Misc(IGUI gui) {
        this();
        this.gui = gui;

        chck_ShowFileInfoPane.setSelected((Boolean) gui.FromMem("ShowFileInfoPane", false));
        chck_ShowEditboxes.setSelected((Boolean) gui.FromMem("ShowSrcStrOtEditBoxes", false));
        chck_DeleteEmptyFolder.setSelected((Boolean) gui.FromMem("DeleteEmptyFolder", false));
        chck_RecursivelyDeleteEmptyFolders.setSelected((Boolean) gui.FromMem("RecursivelyDeleteEmptyFolders", false));
        chck_RenameRelatedFiles.setSelected((Boolean) gui.FromMem("RenameRelatedFiles", false));
        chck_OverwriteMLEntries.setSelected((Boolean) gui.FromMem("OverwriteMLEntries", true));
        chck_ShowUnwatchedOption.setSelected((Boolean) gui.FromMem("ShowSetWatchedStateBox", false));
        chck_ShowFileInfoPane.setVisible(false);

        initEventHandler();
    }
    private void initEventHandler(){
        chck_ShowFileInfoPane.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("ShowFileInfoPane", chck_ShowFileInfoPane.isSelected());
                gui.GUIEvent(new ComEvent(this, ComEvent.eType.Information, "OptionChange", "ShowFileInfoPane", chck_ShowFileInfoPane.isSelected()));
            }
        });
        chck_ShowEditboxes.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("ShowSrcStrOtEditBoxes", chck_ShowEditboxes.isSelected());
                gui.GUIEvent(new ComEvent(this, ComEvent.eType.Information, "OptionChange", "ShowSrcStrOtEditBoxes", chck_ShowEditboxes.isSelected()));
            }
        });
        chck_DeleteEmptyFolder.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("DeleteEmptyFolder", chck_DeleteEmptyFolder.isSelected());
            }
        });
        chck_RecursivelyDeleteEmptyFolders.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("RecursivelyDeleteEmptyFolders", chck_RecursivelyDeleteEmptyFolders.isSelected());
            }
        });
        chck_RenameRelatedFiles.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("RenameRelatedFiles", chck_RenameRelatedFiles.isSelected());
            }
        });
        chck_OverwriteMLEntries.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("OverwriteMLEntries", chck_OverwriteMLEntries.isSelected());
            }
        });
        chck_ShowUnwatchedOption.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("ShowSetWatchedStateBox", chck_ShowUnwatchedOption.isSelected());
                gui.GUIEvent(new ComEvent(this, ComEvent.eType.Information, "OptionChange", "ShowSetWatchedStateBox", chck_ShowFileInfoPane.isSelected()));
            }
        });
        
        
        

    }

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        chck_ShowFileInfoPane = new javax.swing.JCheckBox();
        chck_ShowEditboxes = new javax.swing.JCheckBox();
        chck_DeleteEmptyFolder = new javax.swing.JCheckBox();
        chck_RenameRelatedFiles = new javax.swing.JCheckBox();
        chck_OverwriteMLEntries = new javax.swing.JCheckBox();
        chck_ShowUnwatchedOption = new javax.swing.JCheckBox();
        chck_RecursivelyDeleteEmptyFolders = new javax.swing.JCheckBox();

        chck_ShowFileInfoPane.setLabel("Show Fileinfo Pane");
        chck_ShowFileInfoPane.setOpaque(false);

        chck_ShowEditboxes.setLabel("Show Storage/Source/Other Editboxes");
        chck_ShowEditboxes.setOpaque(false);

        chck_DeleteEmptyFolder.setText("Delete empty folder (after last file moved out)");
        chck_DeleteEmptyFolder.setOpaque(false);

        chck_RenameRelatedFiles.setText("Rename related files (e.g. external subtitles)");
        chck_RenameRelatedFiles.setOpaque(false);

        chck_OverwriteMLEntries.setText("Overwrite existing MyList Entries");
        chck_OverwriteMLEntries.setOpaque(false);

        chck_ShowUnwatchedOption.setText("Show SetWatchedState CheckBox");

        chck_RecursivelyDeleteEmptyFolders.setText("Recursively delete parent folders until non-empty folder is found");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(chck_ShowEditboxes)
                    .addComponent(chck_ShowFileInfoPane)
                    .addComponent(chck_DeleteEmptyFolder)
                    .addComponent(chck_RenameRelatedFiles)
                    .addComponent(chck_OverwriteMLEntries)
                    .addComponent(chck_ShowUnwatchedOption)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(chck_RecursivelyDeleteEmptyFolders)))
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_ShowFileInfoPane)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chck_ShowEditboxes)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chck_DeleteEmptyFolder)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chck_RecursivelyDeleteEmptyFolders)
                .addGap(3, 3, 3)
                .addComponent(chck_RenameRelatedFiles)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chck_OverwriteMLEntries)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(chck_ShowUnwatchedOption)
                .addContainerGap(49, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JCheckBox chck_DeleteEmptyFolder;
    protected javax.swing.JCheckBox chck_OverwriteMLEntries;
    protected javax.swing.JCheckBox chck_RecursivelyDeleteEmptyFolders;
    protected javax.swing.JCheckBox chck_RenameRelatedFiles;
    protected javax.swing.JCheckBox chck_ShowEditboxes;
    protected javax.swing.JCheckBox chck_ShowFileInfoPane;
    protected javax.swing.JCheckBox chck_ShowUnwatchedOption;
    // End of variables declaration//GEN-END:variables
}

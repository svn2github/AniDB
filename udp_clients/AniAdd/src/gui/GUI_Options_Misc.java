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
        chck_RenameRelatedFiles.setSelected((Boolean) gui.FromMem("RenameRelatedFiles", false));
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
        chck_RenameRelatedFiles.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("RenameRelatedFiles", chck_RenameRelatedFiles.isSelected());
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

        chck_ShowFileInfoPane.setLabel("Show Fileinfo Pane");
        chck_ShowFileInfoPane.setOpaque(false);

        chck_ShowEditboxes.setLabel("Show Storage/Source/Other Editboxes");
        chck_ShowEditboxes.setOpaque(false);

        chck_DeleteEmptyFolder.setText("Delete empty folders (after last file moved out)");
        chck_DeleteEmptyFolder.setOpaque(false);

        chck_RenameRelatedFiles.setText("Rename related files (e.g. external subtitles)");
        chck_RenameRelatedFiles.setOpaque(false);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(chck_ShowEditboxes)
                    .addComponent(chck_ShowFileInfoPane)
                    .addComponent(chck_DeleteEmptyFolder)
                    .addComponent(chck_RenameRelatedFiles))
                .addGap(12, 12, 12))
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
                .addComponent(chck_RenameRelatedFiles))
        );
    }// </editor-fold>//GEN-END:initComponents

    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JCheckBox chck_DeleteEmptyFolder;
    protected javax.swing.JCheckBox chck_RenameRelatedFiles;
    protected javax.swing.JCheckBox chck_ShowEditboxes;
    protected javax.swing.JCheckBox chck_ShowFileInfoPane;
    // End of variables declaration//GEN-END:variables
}

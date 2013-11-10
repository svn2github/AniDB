
package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.IAniAdd;
import javax.swing.JOptionPane;

public class GUI_Options extends javax.swing.JPanel implements GUI.ITab {
    private IGUI gui;
    private IAniAdd aniAdd;

    public String TabName() {return "Options";}
    public int PreferredTabLocation() { return 2; }
    public void Initialize(IAniAdd aniAdd, IGUI gui) {
        this.gui = gui;
        this.aniAdd = aniAdd;

        initComponents();

        gUI_Options_AddFoldersOnStartup1.setVisible(false);
        SetMode((Boolean)gui.FromMem("AdvMode", false));
    }
    public void Terminate() {}
    public void GUIEventHandler(ComEvent comEvent) {}

    public void SetMode(boolean advanced){
        btn_Mode.setText(advanced?"Go Simple":"Go Advanced");
        gUI_Options_RenameMove1.ptn_MoveToFolder.setVisible(advanced);
        gUI_Options_RenameMove1.ptn_UseAniDBFN.setVisible(advanced);
        gUI_Options_RenameMove1.ptn_UseTaggingSystemFile.setVisible(advanced);
        gUI_Options_RenameMove1.ptn_UseTaggingSystemFolder.setVisible(advanced);
        gUI_Options_RenameMove1.lbl_Renaming.setVisible(advanced);
        gUI_Options_RenameMove1.btn_EditTagsystem.setVisible(advanced);
        gUI_Options_RenameMove1.chck_EnableFileRenaming.setVisible(advanced);
        gUI_Options_Misc1.setVisible(advanced);


        if(!advanced){
            gUI_Options_Misc1.chck_ShowFileInfoPane.setSelected(false);
            gUI_Options_Misc1.chck_ShowEditboxes.setSelected(false);
            gUI_Options_Misc1.chck_DeleteEmptyFolder.setSelected(false);
            gUI_Options_Misc1.chck_OverwriteMLEntries.setSelected(true);
            gUI_Options_Misc1.chck_RenameRelatedFiles.setSelected(false);
            gUI_Options_Misc1.chck_RecursivelyDeleteEmptyFolders.setSelected(false);
            gUI_Options_Misc1.chck_ShowUnwatchedOption.setSelected(false);

            gUI_Options_RenameMove1.ptn_MoveToFolder.setSelected(true);
            gUI_Options_RenameMove1.ptn_UseAniDBFN.setSelected(true);
            gUI_Options_RenameMove1.chck_EnableFileRenaming.setSelected(true);
            gUI_Options_RenameMove1.chck_EnableFileMoving.setSelected(false);
        }
        gUI_Options_RenameMove1.ToggleFileMoving();
    }
    
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        gUI_Options_AddFoldersOnStartup1 = new GUI_Options_AddFoldersOnStartup(gui);
        gUI_Options_RenameMove1 = new GUI_Options_RenameMove(gui);
        gUI_Options_Misc1 = new GUI_Options_Misc(gui);
        btn_Mode = new javax.swing.JButton();

        btn_Mode.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_ModeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap(665, Short.MAX_VALUE)
                .addComponent(btn_Mode, javax.swing.GroupLayout.PREFERRED_SIZE, 146, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(gUI_Options_RenameMove1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 384, Short.MAX_VALUE)
                    .addComponent(gUI_Options_AddFoldersOnStartup1, javax.swing.GroupLayout.DEFAULT_SIZE, 384, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gUI_Options_Misc1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(69, 69, 69))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(gUI_Options_AddFoldersOnStartup1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gUI_Options_RenameMove1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(32, 32, 32))
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(gUI_Options_Misc1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 280, Short.MAX_VALUE)
                .addComponent(btn_Mode)
                .addContainerGap())
        );
    }// </editor-fold>//GEN-END:initComponents

    private void btn_ModeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_ModeActionPerformed
        boolean isAdv = !(Boolean)gui.FromMem("AdvMode", false);
        if(!isAdv || isAdv && JOptionPane.showConfirmDialog(this, "Are you sure you want to enable advanced options?", "Enable advanced settings", JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION){
            SetMode(isAdv);
            gui.ToMem("AdvMode", isAdv);
        }
    }//GEN-LAST:event_btn_ModeActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btn_Mode;
    private gui.GUI_Options_AddFoldersOnStartup gUI_Options_AddFoldersOnStartup1;
    private gui.GUI_Options_Misc gUI_Options_Misc1;
    private gui.GUI_Options_RenameMove gUI_Options_RenameMove1;
    // End of variables declaration//GEN-END:variables

    public void GainedFocus() {}
    public void LostFocus() {}
}

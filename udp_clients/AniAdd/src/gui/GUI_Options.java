
package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.IAniAdd;

public class GUI_Options extends javax.swing.JPanel implements GUI.ITab {
    private IGUI gui;
    private IAniAdd aniAdd;

    public String TabName() {return "Options";}
    public int PreferredTabLocation() { return 2; }
    public void Initialize(IAniAdd aniAdd, IGUI gui) {
        this.gui = gui;
        this.aniAdd = aniAdd;

        initComponents();
    }
    public void Terminate() {}
    public void GUIEventHandler(ComEvent comEvent) {}

    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        gUI_Options_AddFoldersOnStartup1 = new GUI_Options_AddFoldersOnStartup(gui);
        gUI_Options_RenameMove1 = new GUI_Options_RenameMove(gui);
        gUI_Options_Misc1 = new GUI_Options_Misc(gui);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(gUI_Options_AddFoldersOnStartup1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 283, Short.MAX_VALUE)
                    .addComponent(gUI_Options_RenameMove1, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gUI_Options_Misc1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(gUI_Options_AddFoldersOnStartup1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(gUI_Options_RenameMove1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
            .addComponent(gUI_Options_Misc1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
        );
    }// </editor-fold>//GEN-END:initComponents


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private gui.GUI_Options_AddFoldersOnStartup gUI_Options_AddFoldersOnStartup1;
    private gui.GUI_Options_Misc gUI_Options_Misc1;
    private gui.GUI_Options_RenameMove gUI_Options_RenameMove1;
    // End of variables declaration//GEN-END:variables

    public void GainedFocus() {}
    public void LostFocus() {}

}

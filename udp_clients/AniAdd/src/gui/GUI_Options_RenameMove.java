package gui;

import aniAdd.Communication.ComEvent;
import java.awt.Color;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.io.File;
import javax.swing.JFileChooser;

public class GUI_Options_RenameMove extends javax.swing.JPanel {
    private IGUI gui;

    public GUI_Options_RenameMove() {
        initComponents();
    }
    public GUI_Options_RenameMove(IGUI gui) {
        this();
        this.gui = gui;

        chck_EnableFileMoving.setSelected((Boolean)gui.FromMem("EnableFileMove", false));
        chck_EnableFileRenaming.setSelected((Boolean)gui.FromMem("EnableFileRenaming", true));

        ptn_UseAniDBFN.setSelected((Boolean)gui.FromMem("RenameTypeAniDBFileName", true));
        ptn_UseTaggingSystemFile.setSelected(!(Boolean)gui.FromMem("RenameTypeAniDBFileName", true));

        ptn_MoveToFolder.setSelected((Boolean)gui.FromMem("MoveTypeUseFolder", true));
        ptn_UseTaggingSystemFolder.setSelected(!(Boolean)gui.FromMem("MoveTypeUseFolder", true));
        txt_MoveToFolder.setText((String)gui.FromMem("MoveToFolder", ""));
        chck_AppendAnimeTitle.setSelected((Boolean)gui.FromMem("AppendAnimeTitle", true));
        cmb_AnimeTitleType.setSelectedIndex((Integer)gui.FromMem("AppendAnimeTitleType", 1));

        ptn_UseAniDBFN.setSelected((Boolean)gui.FromMem("RenameTypeAniDBFileName", true));
        ptn_UseTaggingSystemFile.setSelected(!(Boolean)gui.FromMem("RenameTypeAniDBFileName", true));

        initEventHandler();
        ToggleFileMoving();
    }

    public void ToggleFileMoving(){
        boolean movingEnabled = chck_EnableFileMoving.isSelected();
        boolean renamingEnabled = chck_EnableFileRenaming.isSelected();
        boolean folderUseTagSystem = !ptn_MoveToFolder.isSelected();


        ptn_UseTaggingSystemFile.setEnabled(renamingEnabled);
        ptn_UseAniDBFN.setEnabled(renamingEnabled);

        ptn_MoveToFolder.setEnabled(movingEnabled);
        ptn_UseTaggingSystemFolder.setEnabled(movingEnabled);
        txt_MoveToFolder.setEnabled(movingEnabled && !folderUseTagSystem);
        chck_AppendAnimeTitle.setEnabled(movingEnabled && !folderUseTagSystem);
        btn_ChooseDestFolder.setEnabled(movingEnabled && !folderUseTagSystem);
        cmb_AnimeTitleType.setEnabled(movingEnabled && !folderUseTagSystem && chck_AppendAnimeTitle.isSelected());
        
        lbl_DestFolder.setEnabled(movingEnabled && !folderUseTagSystem);
        btn_EditTagsystem.setEnabled((folderUseTagSystem && movingEnabled) || (!ptn_UseAniDBFN.isSelected() && renamingEnabled));
        
        gui.GUIEvent(new ComEvent(this, ComEvent.eType.Information, "OptionChange", "EnableFileMoving"));
    }

    private void initEventHandler(){
        chck_EnableFileMoving.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("EnableFileMove", chck_EnableFileMoving.isSelected());
                ToggleFileMoving();
            }
        });
        chck_EnableFileRenaming.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("EnableFileRenaming", chck_EnableFileRenaming.isSelected());
                ToggleFileMoving();
            }
        });
        chck_AppendAnimeTitle.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("AppendAnimeTitle", chck_AppendAnimeTitle.isSelected());
                ToggleFileMoving();
            }
        });
        
        ptn_MoveToFolder.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("MoveTypeUseFolder", ptn_MoveToFolder.isSelected());
                ToggleFileMoving();
            }
        });
        ptn_UseAniDBFN.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                gui.ToMem("RenameTypeAniDBFileName", ptn_UseAniDBFN.isSelected());
                ToggleFileMoving();
            }
        });

        txt_MoveToFolder.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                File f = new File(txt_MoveToFolder.getText());
                txt_MoveToFolder.setBackground(f.isAbsolute()?Color.GREEN:Color.RED);
                gui.ToMem("MoveToFolder", txt_MoveToFolder.getText());
            }
        });


        cmb_AnimeTitleType.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                gui.ToMem("AppendAnimeTitleType", cmb_AnimeTitleType.getSelectedIndex());
            }
        });


        btn_EditTagsystem.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                gui.SelectTab(gui.AddTab(new GUI_TagSystem()));
            }
        });
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        buttonGroup1 = new javax.swing.ButtonGroup();
        buttonGroup2 = new javax.swing.ButtonGroup();
        pnl_FileRenamingMoving = new javax.swing.JPanel();
        chck_EnableFileMoving = new javax.swing.JCheckBox();
        pnl_FileMove = new javax.swing.JPanel();
        ptn_MoveToFolder = new javax.swing.JRadioButton();
        ptn_UseTaggingSystemFolder = new javax.swing.JRadioButton();
        txt_MoveToFolder = new javax.swing.JTextField();
        chck_AppendAnimeTitle = new javax.swing.JCheckBox();
        cmb_AnimeTitleType = new javax.swing.JComboBox();
        lbl_DestFolder = new javax.swing.JLabel();
        btn_ChooseDestFolder = new javax.swing.JButton();
        pnl_FileRename = new javax.swing.JPanel();
        lbl_Renaming = new javax.swing.JLabel();
        ptn_UseAniDBFN = new javax.swing.JRadioButton();
        ptn_UseTaggingSystemFile = new javax.swing.JRadioButton();
        btn_EditTagsystem = new javax.swing.JButton();
        chck_EnableFileRenaming = new javax.swing.JCheckBox();

        pnl_FileRenamingMoving.setBorder(javax.swing.BorderFactory.createTitledBorder("File Renaming/Moving"));
        pnl_FileRenamingMoving.setOpaque(false);

        chck_EnableFileMoving.setText("Enable Filemoving");
        chck_EnableFileMoving.setOpaque(false);

        pnl_FileMove.setOpaque(false);

        buttonGroup2.add(ptn_MoveToFolder);
        ptn_MoveToFolder.setText("Move to Folder");
        ptn_MoveToFolder.setEnabled(false);
        ptn_MoveToFolder.setOpaque(false);

        buttonGroup2.add(ptn_UseTaggingSystemFolder);
        ptn_UseTaggingSystemFolder.setText("Use Tagging System");
        ptn_UseTaggingSystemFolder.setEnabled(false);
        ptn_UseTaggingSystemFolder.setOpaque(false);

        txt_MoveToFolder.setEnabled(false);

        chck_AppendAnimeTitle.setText("Append Anime Title");
        chck_AppendAnimeTitle.setEnabled(false);
        chck_AppendAnimeTitle.setOpaque(false);

        cmb_AnimeTitleType.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "English", "Romaji", "Kanji" }));
        cmb_AnimeTitleType.setSelectedIndex(1);
        cmb_AnimeTitleType.setEnabled(false);

        lbl_DestFolder.setText("Destination:");
        lbl_DestFolder.setEnabled(false);

        btn_ChooseDestFolder.setText("...");
        btn_ChooseDestFolder.setEnabled(false);
        btn_ChooseDestFolder.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                btn_ChooseDestFolderActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout pnl_FileMoveLayout = new javax.swing.GroupLayout(pnl_FileMove);
        pnl_FileMove.setLayout(pnl_FileMoveLayout);
        pnl_FileMoveLayout.setHorizontalGroup(
            pnl_FileMoveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileMoveLayout.createSequentialGroup()
                .addGroup(pnl_FileMoveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(ptn_UseTaggingSystemFolder)
                    .addGroup(pnl_FileMoveLayout.createSequentialGroup()
                        .addComponent(ptn_MoveToFolder)
                        .addGap(149, 149, 149))
                    .addGroup(pnl_FileMoveLayout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(chck_AppendAnimeTitle)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(cmb_AnimeTitleType, 0, 106, Short.MAX_VALUE))
                    .addGroup(pnl_FileMoveLayout.createSequentialGroup()
                        .addGap(21, 21, 21)
                        .addComponent(lbl_DestFolder, javax.swing.GroupLayout.PREFERRED_SIZE, 58, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(txt_MoveToFolder, javax.swing.GroupLayout.DEFAULT_SIZE, 157, Short.MAX_VALUE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(btn_ChooseDestFolder, javax.swing.GroupLayout.PREFERRED_SIZE, 24, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        pnl_FileMoveLayout.setVerticalGroup(
            pnl_FileMoveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileMoveLayout.createSequentialGroup()
                .addComponent(ptn_MoveToFolder)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_FileMoveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_MoveToFolder, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(btn_ChooseDestFolder)
                    .addComponent(lbl_DestFolder))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_FileMoveLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(chck_AppendAnimeTitle)
                    .addComponent(cmb_AnimeTitleType, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(ptn_UseTaggingSystemFolder)
                .addContainerGap(2, Short.MAX_VALUE))
        );

        pnl_FileRename.setOpaque(false);

        lbl_Renaming.setText("Renaming:");

        buttonGroup1.add(ptn_UseAniDBFN);
        ptn_UseAniDBFN.setText("Use AniDB Filename");
        ptn_UseAniDBFN.setOpaque(false);

        buttonGroup1.add(ptn_UseTaggingSystemFile);
        ptn_UseTaggingSystemFile.setText("Use Tagging System");
        ptn_UseTaggingSystemFile.setOpaque(false);

        javax.swing.GroupLayout pnl_FileRenameLayout = new javax.swing.GroupLayout(pnl_FileRename);
        pnl_FileRename.setLayout(pnl_FileRenameLayout);
        pnl_FileRenameLayout.setHorizontalGroup(
            pnl_FileRenameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileRenameLayout.createSequentialGroup()
                .addGroup(pnl_FileRenameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pnl_FileRenameLayout.createSequentialGroup()
                        .addGap(10, 10, 10)
                        .addComponent(ptn_UseAniDBFN)
                        .addGap(18, 18, 18)
                        .addComponent(ptn_UseTaggingSystemFile))
                    .addComponent(lbl_Renaming))
                .addContainerGap(16, Short.MAX_VALUE))
        );
        pnl_FileRenameLayout.setVerticalGroup(
            pnl_FileRenameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileRenameLayout.createSequentialGroup()
                .addComponent(lbl_Renaming)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_FileRenameLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(ptn_UseAniDBFN)
                    .addComponent(ptn_UseTaggingSystemFile)))
        );

        btn_EditTagsystem.setText("Edit Tagging System");

        chck_EnableFileRenaming.setText("Enable Filerenaming");
        chck_EnableFileRenaming.setOpaque(false);

        javax.swing.GroupLayout pnl_FileRenamingMovingLayout = new javax.swing.GroupLayout(pnl_FileRenamingMoving);
        pnl_FileRenamingMoving.setLayout(pnl_FileRenamingMovingLayout);
        pnl_FileRenamingMovingLayout.setHorizontalGroup(
            pnl_FileRenamingMovingLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(chck_EnableFileMoving)
            .addComponent(pnl_FileRename, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(pnl_FileRenamingMovingLayout.createSequentialGroup()
                .addComponent(chck_EnableFileRenaming)
                .addContainerGap())
            .addComponent(pnl_FileMove, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(pnl_FileRenamingMovingLayout.createSequentialGroup()
                .addComponent(btn_EditTagsystem, javax.swing.GroupLayout.PREFERRED_SIZE, 231, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );
        pnl_FileRenamingMovingLayout.setVerticalGroup(
            pnl_FileRenamingMovingLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_FileRenamingMovingLayout.createSequentialGroup()
                .addComponent(chck_EnableFileMoving)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(pnl_FileMove, javax.swing.GroupLayout.PREFERRED_SIZE, 101, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addComponent(chck_EnableFileRenaming)
                .addGap(0, 0, 0)
                .addComponent(pnl_FileRename, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addGap(3, 3, 3)
                .addComponent(btn_EditTagsystem, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addContainerGap())
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(pnl_FileRenamingMoving, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(pnl_FileRenamingMoving, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGap(0, 0, 0))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void btn_ChooseDestFolderActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_btn_ChooseDestFolderActionPerformed
        JFileChooser FC = new javax.swing.JFileChooser();
        FC.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
        FC.setMultiSelectionEnabled(false);

        if (FC.showOpenDialog(this) == javax.swing.JFileChooser.APPROVE_OPTION) {
            File f = FC.getSelectedFile();
            txt_MoveToFolder.setText(f.getPath());
            txt_MoveToFolder.setBackground(f.isAbsolute()?Color.GREEN:Color.RED);
            gui.ToMem("MoveToFolder", txt_MoveToFolder.getText());
        }
    }//GEN-LAST:event_btn_ChooseDestFolderActionPerformed


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton btn_ChooseDestFolder;
    protected javax.swing.JButton btn_EditTagsystem;
    private javax.swing.ButtonGroup buttonGroup1;
    private javax.swing.ButtonGroup buttonGroup2;
    protected javax.swing.JCheckBox chck_AppendAnimeTitle;
    protected javax.swing.JCheckBox chck_EnableFileMoving;
    protected javax.swing.JCheckBox chck_EnableFileRenaming;
    protected javax.swing.JComboBox cmb_AnimeTitleType;
    private javax.swing.JLabel lbl_DestFolder;
    protected javax.swing.JLabel lbl_Renaming;
    protected javax.swing.JPanel pnl_FileMove;
    protected javax.swing.JPanel pnl_FileRename;
    protected javax.swing.JPanel pnl_FileRenamingMoving;
    protected javax.swing.JRadioButton ptn_MoveToFolder;
    protected javax.swing.JRadioButton ptn_UseAniDBFN;
    protected javax.swing.JRadioButton ptn_UseTaggingSystemFile;
    protected javax.swing.JRadioButton ptn_UseTaggingSystemFolder;
    protected javax.swing.JTextField txt_MoveToFolder;
    // End of variables declaration//GEN-END:variables

}

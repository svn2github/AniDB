package UI;

import Processing.EpProcessing.eRenameStyle;
import Processing.cUser.eLanguage;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.MouseAdapter;
import javax.swing.ButtonGroup;
import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JRadioButton;

/**
 * @author Arokh
 */
public class UIRenameStyle extends javax.swing.JDialog {
    AniAdd.AniAdd CC;
    final JList lst_SN;
    final JList lst_Ep;
    JRadioButton ptn_RenStanDB = new JRadioButton("AniDB rename style", false);
    JRadioButton ptn_RenCust = new JRadioButton("Custom File Rename", true);

    
    public UIRenameStyle(AniAdd.AniAdd CallingClass) {
        CC = CallingClass;
        setTitle("File Rename Pattern");
                
                
        DefaultListModel DefModel = new DefaultListModel();
        for (int I=0; I < CC.EpProc.UserSettings.SeriesLang.length; I++) {
           DefModel.addElement(CC.EpProc.UserSettings.SeriesLang[I].toString());
        }
        lst_SN = new JList(DefModel);
                
        DefModel = new DefaultListModel();
        for (int I=0; I < CC.EpProc.UserSettings.EpisodeLang.length; I++) {
           DefModel.addElement(CC.EpProc.UserSettings.EpisodeLang[I].toString());
        }
        lst_Ep = new JList(DefModel);
        
        lst_SN.setToolTipText("Prioritylist: Top language is first choice (only custom)");
        lst_Ep.setToolTipText("Prioritylist: Top language is first choice (only custom)");
                
        ImageIcon icoDown = createImageIcon("/UI/Down16.gif");
        ImageIcon icoUp = createImageIcon("/UI/Up16.gif");

        JButton btn_SNUp = new JButton(icoUp); btn_SNUp.setName("SNU");
        JButton btn_SNDown = new JButton(icoDown); btn_SNDown.setName("SND");
        JButton btn_EpUp = new JButton(icoUp); btn_EpUp.setName("EpU");
        JButton btn_EpDown = new JButton(icoDown); btn_EpDown.setName("EpD");
        JButton btn_OK = new JButton("OK");
        
        
        ButtonGroup Grp = new ButtonGroup();
        Grp.add(ptn_RenStanDB);
        Grp.add(ptn_RenCust);
        
        switch (CC.EpProc.UserSettings.Type) {
            case custom:
                ptn_RenCust.setSelected(true);
            break;
            
            case dbstandard:
                ptn_RenStanDB.setSelected(true);
            break;
        }
        
        JPanel LayoutGrid = new javax.swing.JPanel(new GridBagLayout());
        GridBagConstraints c = new GridBagConstraints();
        c.fill = GridBagConstraints.BOTH;
        c.insets = new Insets(0, 4, 0, 4);
        
        c.gridx=0; c.gridy=0;
        c.gridwidth = GridBagConstraints.REMAINDER;
        LayoutGrid.add(ptn_RenStanDB, c);
        
        c.gridy++; LayoutGrid.add(ptn_RenCust, c);
        
        c.gridwidth = 1;
        c.gridy++; LayoutGrid.add(new JLabel("Series Name"), c);
        c.gridx++; LayoutGrid.add(new JLabel("Ep Name"), c);
        
        c.gridx--;c.gridy++;
        JPanel SubGrid = new javax.swing.JPanel(new GridBagLayout());
        GridBagConstraints d = new GridBagConstraints();
        d.gridx=0; d.gridy=0; d.gridheight=GridBagConstraints.REMAINDER;
        SubGrid.add(lst_SN, d);
        d.gridx++; d.gridheight=1;
        SubGrid.add(btn_SNUp, d);
        d.gridy++;
        SubGrid.add(btn_SNDown, d);
        LayoutGrid.add(SubGrid, c);
        
        c.gridx++;
        SubGrid = new javax.swing.JPanel(new GridBagLayout());
        d = new GridBagConstraints();
        d.gridx=0; d.gridy=0; d.gridheight=GridBagConstraints.REMAINDER;
        SubGrid.add(lst_Ep, d);
        d.gridx++; d.gridheight=1;
        SubGrid.add(btn_EpUp, d);
        d.gridy++;
        SubGrid.add(btn_EpDown, d);
        LayoutGrid.add(SubGrid, c);
        
        c.gridy++;c.gridx=0;
        c.gridwidth = GridBagConstraints.REMAINDER;
        LayoutGrid.add(btn_OK, c);

        java.awt.event.MouseAdapter BtnClick = new java.awt.event.MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                JList List;
                JButton Button = (JButton)evt.getComponent();
                
                if (Button.getName().startsWith("SN")) {
                    List = lst_SN; } else {List = lst_Ep;}
                
                int Index = List.getSelectedIndex();
                DefaultListModel DefModel = (DefaultListModel)List.getModel();
               
                if (Button.getName().endsWith("U")) {
                    if (Index==0 || Index== -1) return; 
                    DefModel.add(Index-1, (String)List.getSelectedValue());
                    DefModel.removeElementAt(Index+1);
                    List.setSelectedIndex(Index-1);
                } else {
                    if (Index==DefModel.getSize()-1 || Index == -1) return; 
                    DefModel.add(Index+2, (String)List.getSelectedValue());
                    DefModel.removeElementAt(Index);
                    List.setSelectedIndex(Index+1);
               }
            }
        };
        btn_SNUp.addMouseListener(BtnClick);
        btn_SNDown.addMouseListener(BtnClick);
        btn_EpUp.addMouseListener(BtnClick);
        btn_EpDown.addMouseListener(BtnClick);
        
        btn_OK.addMouseListener(new MouseAdapter() {
            public void mouseClicked(java.awt.event.MouseEvent evt) {
                SetSettings();
            }
        });

        add(LayoutGrid);
    }
    
    void SetSettings() {
        for (int I=0; I < lst_SN.getModel().getSize(); I++) {
           CC.EpProc.UserSettings.SeriesLang[I] = eLanguage.valueOf((String)lst_SN.getModel().getElementAt(I));
        }
        for (int I=0; I < lst_Ep.getModel().getSize(); I++) {
           CC.EpProc.UserSettings.EpisodeLang[I] = eLanguage.valueOf((String)lst_Ep.getModel().getElementAt(I));
        }
        
        if (ptn_RenCust.isSelected()) {
             CC.EpProc.UserSettings.Type = eRenameStyle.custom;
        } else if (ptn_RenStanDB.isSelected()) {
             CC.EpProc.UserSettings.Type = eRenameStyle.dbstandard;
        }
        
        dispose();
    }
    
    /* Returns an ImageIcon, or null if the path was invalid. */
    protected static ImageIcon createImageIcon(String path) {
        java.net.URL imgURL = UIRenameStyle.class.getResource(path);
        if (imgURL != null) {
            return new ImageIcon(imgURL);
        } else {
            System.err.println("Couldn't find file: " + path);
            return null;
        }
    }

}

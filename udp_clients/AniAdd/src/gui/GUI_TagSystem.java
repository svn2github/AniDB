package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.IAniAdd;
import java.util.Map.Entry;
import java.util.TreeMap;
import javax.swing.table.DefaultTableModel;
import javax.swing.text.BadLocationException;
import processing.TagSystem;

public class GUI_TagSystem extends javax.swing.JPanel implements GUI.ITab {
    private IGUI gui;

    public GUI_TagSystem() {
        initComponents();
    }
    protected void InitDefaults(){
        txt_CodeBox.setText((String)gui.FromMem("TagSystemCode", txt_CodeBox.getText()));
    }

    private void UpdateCursorLocationLabel(){
        try {
            int row = txt_CodeBox.getLineOfOffset(txt_CodeBox.getCaretPosition());
            int column = txt_CodeBox.getCaretPosition() - txt_CodeBox.getLineStartOffset(row) ;
            lbl_CursorLocation.setText("Row: " + (row+1)  + "  Column: " + (column+1) + "  Char: " + txt_CodeBox.getCaretPosition());
            txt_CodeBox.getCaretPosition();
        } catch (BadLocationException ex) {
        }
    }
    
    protected void TagSystemCodeChange(){
        TreeMap<String, String> tags = new TreeMap<String, String>();
        tags.put("ATr", txt_AT_Romaji.getText());
        tags.put("ATe", txt_AT_English.getText());
        tags.put("ATk", txt_AT_Kanji.getText());
        tags.put("ATs", txt_AT_Synomymn.getText());
        tags.put("ATo", txt_AT_Other.getText());
        tags.put("AYearBegin", txt_YearBegin.getText());
        tags.put("AYearEnd", txt_YearEnd.getText());
        tags.put("ACatList", txt_ACatList.getText());

        tags.put("ETr", txt_ET_Romaji.getText());
        tags.put("ETe", txt_ET_English.getText());
        tags.put("ETk", txt_ET_Kanji.getText());
        tags.put("EAirDate", "");

        tags.put("GTs", txt_GT_Short.getText());
        tags.put("GTl", txt_GT_Long.getText());


        tags.put("FCrc", txt_Crc.getText());
        tags.put("FALng", txt_Dubs.getText());
        tags.put("FACodec", txt_AudioCodec.getText());
        tags.put("FSLng", txt_Subs.getText());
        tags.put("FVCodec", txt_VideoCodec.getText());
        tags.put("FVideoRes", txt_Resolution.getText());
        
        tags.put("FColorDepth", "");
        tags.put("FDuration", "1440");
         
        tags.put("AniDBFN", "Suzumiya_Haruhi_no_Yuuutsu_(2009)_-_01_-_The_Melancholy_of_Suzumiya_Haruhi_Part_1_-_[a.f.k.](32f2f4ea).avi");
        tags.put("CurrentFN", txt_CurrentFN.getText());
        
        
        tags.put("EpNo", updown_EpNo.getValue().toString());
        tags.put("EpHiNo", updown_EpHiNo.getValue().toString());
        tags.put("EpCount", updown_EpCount.getValue().toString());
        
        tags.put("FId", "1");
        tags.put("AId", "2");
        tags.put("EId", "3");
        tags.put("GId", "4");
        tags.put("LId", "5");        
             
        tags.put("OtherEps", "5'7");
        
        tags.put("Quality", cmb_Quality.getSelectedItem().toString());
        tags.put("Source", cmb_Source.getSelectedItem().toString());
        tags.put("Type", cmb_Type.getSelectedItem().toString());

        tags.put("Watched", chck_IsWatched.isSelected()?"1":"");

        tags.put("Depr", chck_IsDeprecated.isSelected()?"1":"");
        tags.put("CrcOK", "1");
        tags.put("CrcErr", "0");
        tags.put("Depr", chck_IsDeprecated.isSelected()?"1":"");
        
        tags.put("Cen", chck_IsCensored.isSelected()?"1":"");
        tags.put("UnCen", !chck_IsCensored.isSelected()?"1":"");
        
        tags.put("Ver", updown_Version.getValue().toString());
        
        DefaultTableModel model = (DefaultTableModel)tbl_Vars.getModel();
        model.setRowCount(0);
        for (Entry<String, String> tag : tags.entrySet()) {
            model.addRow(new Object[]{ tag.getKey(), tag.getValue() });
        }

        try {
            TagSystem.Evaluate(txt_CodeBox.getText(), tags);

            lbl_FileNameStr.setText(tags.get("FileName"));
            lbl_DirNameStr.setText(tags.get("PathName"));
            lbl_ErrorMsg.setText(" ");
        } catch (Exception ex) {
            lbl_FileNameStr.setText(" ");
            lbl_DirNameStr.setText(" ");
            lbl_ErrorMsg.setText(ex.getMessage());
        }

        if(gui != null) gui.ToMem("TagSystemCode", txt_CodeBox.getText());
    }

    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        scrl_txt_CodeBox = new javax.swing.JScrollPane();
        txt_CodeBox = new javax.swing.JTextArea();
        lbl_FileName = new javax.swing.JLabel();
        lbl_DirName = new javax.swing.JLabel();
        lbl_DirNameStr = new javax.swing.JLabel();
        lbl_FileNameStr = new javax.swing.JLabel();
        lbl_ErrorMsg = new javax.swing.JLabel();
        lbl_CursorLocation = new javax.swing.JLabel();
        jTabbedPane1 = new javax.swing.JTabbedPane();
        jPanel1 = new javax.swing.JPanel();
        pnl_AnimeTitles = new javax.swing.JPanel();
        lbl_AT_English = new javax.swing.JLabel();
        txt_AT_English = new javax.swing.JTextField();
        lbl_AT_Romaji = new javax.swing.JLabel();
        txt_AT_Romaji = new javax.swing.JTextField();
        lbl_AT_Kanji = new javax.swing.JLabel();
        txt_AT_Kanji = new javax.swing.JTextField();
        lbl_AT_Other = new javax.swing.JLabel();
        txt_AT_Other = new javax.swing.JTextField();
        lbl_AT_Synomymn = new javax.swing.JLabel();
        txt_AT_Synomymn = new javax.swing.JTextField();
        pnl_GroupNames = new javax.swing.JPanel();
        lbl_GT_Short = new javax.swing.JLabel();
        txt_GT_Short = new javax.swing.JTextField();
        lbl_GT_Long = new javax.swing.JLabel();
        txt_GT_Long = new javax.swing.JTextField();
        pnl_EpTitles = new javax.swing.JPanel();
        lbl_ET_English = new javax.swing.JLabel();
        txt_ET_English = new javax.swing.JTextField();
        lbl_ET_Romaji = new javax.swing.JLabel();
        txt_ET_Romaji = new javax.swing.JTextField();
        lbl_ET_Kanji = new javax.swing.JLabel();
        txt_ET_Kanji = new javax.swing.JTextField();
        jPanel2 = new javax.swing.JPanel();
        jPanel3 = new javax.swing.JPanel();
        chck_IsCensored = new javax.swing.JCheckBox();
        chck_IsDeprecated = new javax.swing.JCheckBox();
        updown_Version = new javax.swing.JSpinner();
        chck_IsWatched = new javax.swing.JCheckBox();
        lbl_Quality = new javax.swing.JLabel();
        lbl_Source = new javax.swing.JLabel();
        cmb_Quality = new javax.swing.JComboBox();
        cmb_Source = new javax.swing.JComboBox();
        lbl_Version = new javax.swing.JLabel();
        lbl_Dubs = new javax.swing.JLabel();
        txt_Dubs = new javax.swing.JTextField();
        lbl_Subs = new javax.swing.JLabel();
        txt_Subs = new javax.swing.JTextField();
        lbl_Crc = new javax.swing.JLabel();
        txt_Crc = new javax.swing.JTextField();
        txt_Resolution = new javax.swing.JTextField();
        lbl_Resolution = new javax.swing.JLabel();
        lbl_AudioCodec = new javax.swing.JLabel();
        txt_AudioCodec = new javax.swing.JTextField();
        txt_VideoCodec = new javax.swing.JTextField();
        lbl_VideoCodec = new javax.swing.JLabel();
        lbl_CurrentFN = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        txt_CurrentFN = new javax.swing.JTextArea();
        jPanel4 = new javax.swing.JPanel();
        lbl_EpCount = new javax.swing.JLabel();
        updown_EpCount = new javax.swing.JSpinner();
        lbl_EpHiNo = new javax.swing.JLabel();
        lbl_Type = new javax.swing.JLabel();
        cmb_Type = new javax.swing.JComboBox();
        updown_EpHiNo = new javax.swing.JSpinner();
        lbl_YearBegin = new javax.swing.JLabel();
        txt_YearBegin = new javax.swing.JTextField();
        lbl_YearEnd = new javax.swing.JLabel();
        txt_YearEnd = new javax.swing.JTextField();
        txt_ACatList = new javax.swing.JTextField();
        lbl_ACatList = new javax.swing.JLabel();
        jPanel5 = new javax.swing.JPanel();
        lbl_EpNo = new javax.swing.JLabel();
        updown_EpNo = new javax.swing.JSpinner();
        jPanel6 = new javax.swing.JPanel();
        jScrollPane2 = new javax.swing.JScrollPane();
        tbl_Vars = new javax.swing.JTable();

        setPreferredSize(new java.awt.Dimension(780, 560));

        txt_CodeBox.setColumns(20);
        txt_CodeBox.setFont(new java.awt.Font("Consolas", 0, 11)); // NOI18N
        txt_CodeBox.setRows(5);
        txt_CodeBox.setText("AT:=[%ATr%,%ATe%,%ATk%]\nET:=[%ETe%,%ETr%,%ETk%]\nGT:=\"[\" [%GTs%,%GTl%] \"]\"\n\nEpNoPad:=$pad(%EpNo%,$max($len(%EpHiNo%),$len(%EpCount%)),\"0\")\n\nSrc:=\"[\"%Source%\"]\"\nDepr:={%Depr%?\"[Depr]\":\"\"}\nCen:={%Cen%?\"[Cen]\":\"\"}\nVer:={%Ver%=\"1\"?\"\":\"v\"%Ver%} \n\nFileName:=%AT%\" \"%EpNoPad% %Ver% \" - \"%ET%\" \"%GT% %Depr% %Cen% %Src%\nPathName:=\"E:\\Anime\\!Processed\\\" $repl(%AT%,'[\\\\\\\":/*|<>?]',\"\")");
        txt_CodeBox.addMouseListener(new java.awt.event.MouseAdapter() {
            public void mousePressed(java.awt.event.MouseEvent evt) {
                txt_CodeBoxMousePressed(evt);
            }
        });
        txt_CodeBox.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyPressed(java.awt.event.KeyEvent evt) {
                txt_CodeBoxKeyPressed(evt);
            }
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_CodeBoxKeyReleased(evt);
            }
        });
        scrl_txt_CodeBox.setViewportView(txt_CodeBox);

        lbl_FileName.setText("Filename:");

        lbl_DirName.setText("Directoryname:");

        lbl_DirNameStr.setText(" ");

        lbl_FileNameStr.setText(" ");

        lbl_ErrorMsg.setText(" ");
        lbl_ErrorMsg.setOpaque(true);

        lbl_CursorLocation.setText("Row: 0 Column: 0");

        jPanel1.setOpaque(false);

        pnl_AnimeTitles.setBorder(javax.swing.BorderFactory.createTitledBorder("Anime Titles:"));
        pnl_AnimeTitles.setOpaque(false);

        lbl_AT_English.setText("English");

        txt_AT_English.setText("The Melancholy of Haruhi Suzumiya (2009)");
        txt_AT_English.setToolTipText("ATe");
        txt_AT_English.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_AT_Romaji.setText("Romaji");

        txt_AT_Romaji.setText("Suzumiya Haruhi no Yuuutsu (2009)");
        txt_AT_Romaji.setToolTipText("ATr");
        txt_AT_Romaji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_AT_Kanji.setText("Kanji");

        txt_AT_Kanji.setText("涼宮ハルヒの憂鬱 (2009)");
        txt_AT_Kanji.setToolTipText("ATk");
        txt_AT_Kanji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_AT_Other.setText("Other");

        txt_AT_Other.setText("haruhi2");
        txt_AT_Other.setToolTipText("ATo");
        txt_AT_Other.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_AT_Synomymn.setText("Synonymn");

        txt_AT_Synomymn.setToolTipText("ATs");
        txt_AT_Synomymn.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        javax.swing.GroupLayout pnl_AnimeTitlesLayout = new javax.swing.GroupLayout(pnl_AnimeTitles);
        pnl_AnimeTitles.setLayout(pnl_AnimeTitlesLayout);
        pnl_AnimeTitlesLayout.setHorizontalGroup(
            pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_AnimeTitlesLayout.createSequentialGroup()
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_AT_English)
                    .addComponent(lbl_AT_Romaji)
                    .addComponent(lbl_AT_Kanji)
                    .addComponent(lbl_AT_Other)
                    .addComponent(lbl_AT_Synomymn))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(txt_AT_Synomymn)
                    .addComponent(txt_AT_Other)
                    .addComponent(txt_AT_Kanji)
                    .addComponent(txt_AT_Romaji)
                    .addComponent(txt_AT_English)))
        );
        pnl_AnimeTitlesLayout.setVerticalGroup(
            pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_AnimeTitlesLayout.createSequentialGroup()
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_AT_English, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_AT_English))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_AT_Romaji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_AT_Romaji))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_AT_Kanji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_AT_Kanji))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_AT_Other, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_AT_Other))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_AT_Synomymn, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_AT_Synomymn))
                .addGap(39, 39, 39))
        );

        pnl_GroupNames.setBorder(javax.swing.BorderFactory.createTitledBorder("Group Names:"));
        pnl_GroupNames.setOpaque(false);

        lbl_GT_Short.setText("Short");

        txt_GT_Short.setText("a.f.k.");
        txt_GT_Short.setToolTipText("GTs");
        txt_GT_Short.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_GT_Long.setText("Long");

        txt_GT_Long.setText("a.f.k. (Long)");
        txt_GT_Long.setToolTipText("GTl");
        txt_GT_Long.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        javax.swing.GroupLayout pnl_GroupNamesLayout = new javax.swing.GroupLayout(pnl_GroupNames);
        pnl_GroupNames.setLayout(pnl_GroupNamesLayout);
        pnl_GroupNamesLayout.setHorizontalGroup(
            pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_GroupNamesLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_GT_Short)
                    .addComponent(lbl_GT_Long))
                .addGap(21, 21, 21)
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txt_GT_Long, javax.swing.GroupLayout.DEFAULT_SIZE, 140, Short.MAX_VALUE)
                    .addComponent(txt_GT_Short, javax.swing.GroupLayout.DEFAULT_SIZE, 140, Short.MAX_VALUE)))
        );
        pnl_GroupNamesLayout.setVerticalGroup(
            pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_GroupNamesLayout.createSequentialGroup()
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_GT_Short, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_GT_Short))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_GT_Long, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_GT_Long)))
        );

        pnl_EpTitles.setBorder(javax.swing.BorderFactory.createTitledBorder("Episode Titles:"));
        pnl_EpTitles.setOpaque(false);

        lbl_ET_English.setText("English");

        txt_ET_English.setText("Bamboo Leaf Rhapsody");
        txt_ET_English.setToolTipText("ETe");
        txt_ET_English.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_ET_Romaji.setText("Romaji");

        txt_ET_Romaji.setText("Sasa no Ha Rhapsody");
        txt_ET_Romaji.setToolTipText("ETr");
        txt_ET_Romaji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_ET_Kanji.setText("Kanji");

        txt_ET_Kanji.setText("笹の葉ラプソディ");
        txt_ET_Kanji.setToolTipText("ETk");
        txt_ET_Kanji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        javax.swing.GroupLayout pnl_EpTitlesLayout = new javax.swing.GroupLayout(pnl_EpTitles);
        pnl_EpTitles.setLayout(pnl_EpTitlesLayout);
        pnl_EpTitlesLayout.setHorizontalGroup(
            pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_EpTitlesLayout.createSequentialGroup()
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_ET_English)
                    .addComponent(lbl_ET_Romaji)
                    .addComponent(lbl_ET_Kanji))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(txt_ET_Kanji)
                    .addComponent(txt_ET_Romaji)
                    .addComponent(txt_ET_English, javax.swing.GroupLayout.DEFAULT_SIZE, 160, Short.MAX_VALUE)))
        );
        pnl_EpTitlesLayout.setVerticalGroup(
            pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_EpTitlesLayout.createSequentialGroup()
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_ET_English)
                    .addComponent(txt_ET_English, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_ET_Romaji)
                    .addComponent(txt_ET_Romaji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_ET_Kanji)
                    .addComponent(txt_ET_Kanji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
        );

        javax.swing.GroupLayout jPanel1Layout = new javax.swing.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel1Layout.createSequentialGroup()
                .addComponent(pnl_AnimeTitles, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(pnl_GroupNames, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_EpTitles, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addContainerGap(284, Short.MAX_VALUE))
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel1Layout.createSequentialGroup()
                .addGroup(jPanel1Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel1Layout.createSequentialGroup()
                        .addComponent(pnl_EpTitles, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(pnl_GroupNames, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addComponent(pnl_AnimeTitles, javax.swing.GroupLayout.PREFERRED_SIZE, 155, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(23, Short.MAX_VALUE))
        );

        jTabbedPane1.addTab("Titles", jPanel1);

        jPanel2.setOpaque(false);

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("File Info:"));
        jPanel3.setOpaque(false);

        chck_IsCensored.setText("Is Censored");
        chck_IsCensored.setToolTipText("Cen");
        chck_IsCensored.setOpaque(false);
        chck_IsCensored.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CheckedChanged(evt);
            }
        });

        chck_IsDeprecated.setText("Is Deprecated");
        chck_IsDeprecated.setToolTipText("Depr");
        chck_IsDeprecated.setOpaque(false);
        chck_IsDeprecated.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CheckedChanged(evt);
            }
        });

        updown_Version.setModel(new javax.swing.SpinnerNumberModel(Integer.valueOf(1), Integer.valueOf(1), null, Integer.valueOf(1)));
        updown_Version.setToolTipText("FVer");
        updown_Version.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                UpDownChanged(evt);
            }
        });

        chck_IsWatched.setText("Is Watched");
        chck_IsWatched.setToolTipText("Watched");
        chck_IsWatched.setOpaque(false);
        chck_IsWatched.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                CheckedChanged(evt);
            }
        });

        lbl_Quality.setText("Quality:");

        lbl_Source.setText("Source:");

        cmb_Quality.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Very Good", "Good", "Eye Cancer" }));
        cmb_Quality.setToolTipText("Quality");
        cmb_Quality.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                ComboChanged(evt);
            }
        });

        cmb_Source.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Web", "DTV", "HDTV", "DVD", "B-R" }));
        cmb_Source.setSelectedIndex(2);
        cmb_Source.setToolTipText("Source");
        cmb_Source.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                ComboChanged(evt);
            }
        });

        lbl_Version.setText("File Ver:");

        lbl_Dubs.setText("Dubs:");

        txt_Dubs.setText("english'japanese'german");
        txt_Dubs.setToolTipText("FALng");
        txt_Dubs.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_Subs.setText("Subs:");

        txt_Subs.setText("english'german");
        txt_Subs.setToolTipText("FSLng");
        txt_Subs.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_Crc.setText("CRC:");

        txt_Crc.setText("4a8cbc62");
        txt_Crc.setToolTipText("FCrc");
        txt_Crc.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        txt_Resolution.setText("1920x1080");
        txt_Resolution.setToolTipText("FVideoRes");
        txt_Resolution.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_Resolution.setText("Resolution:");

        lbl_AudioCodec.setText("Audio Codec:");

        txt_AudioCodec.setText("AC3");
        txt_AudioCodec.setToolTipText("FACodec");
        txt_AudioCodec.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        txt_VideoCodec.setText("H264/AVC");
        txt_VideoCodec.setToolTipText("FVCodec");
        txt_VideoCodec.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_VideoCodec.setText("Video Codec:");

        lbl_CurrentFN.setText("Current Filename:");

        txt_CurrentFN.setColumns(20);
        txt_CurrentFN.setFont(new java.awt.Font("Tahoma", 0, 11)); // NOI18N
        txt_CurrentFN.setLineWrap(true);
        txt_CurrentFN.setRows(5);
        txt_CurrentFN.setText("[Chihiro]​_Suzumiya​_Haruhi​_no​_Yuutsu​_(2009)​_-​_01​_[848x480​_H.​264​_AAC][7595C366].​mkv");
        txt_CurrentFN.setToolTipText("CurrentFN");
        txt_CurrentFN.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });
        jScrollPane1.setViewportView(txt_CurrentFN);

        javax.swing.GroupLayout jPanel3Layout = new javax.swing.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGap(9, 9, 9)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(chck_IsWatched)
                                    .addComponent(chck_IsCensored)
                                    .addComponent(chck_IsDeprecated))
                                .addGap(9, 9, 9)
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(lbl_Source)
                                    .addComponent(lbl_Quality)
                                    .addComponent(lbl_Version))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(updown_Version, javax.swing.GroupLayout.PREFERRED_SIZE, 50, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(cmb_Source, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                    .addComponent(cmb_Quality, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(lbl_Subs)
                                    .addComponent(lbl_Dubs))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(txt_Subs)
                                    .addComponent(txt_Dubs)))))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(lbl_Crc)
                        .addGap(6, 6, 6)
                        .addComponent(txt_Crc, javax.swing.GroupLayout.PREFERRED_SIZE, 61, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addGap(18, 18, 18)
                        .addComponent(lbl_Resolution)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(txt_Resolution, javax.swing.GroupLayout.PREFERRED_SIZE, 61, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 186, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())
                    .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                        .addGroup(jPanel3Layout.createSequentialGroup()
                            .addComponent(lbl_CurrentFN, javax.swing.GroupLayout.PREFERRED_SIZE, 93, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addContainerGap())
                        .addGroup(jPanel3Layout.createSequentialGroup()
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                .addComponent(lbl_AudioCodec, javax.swing.GroupLayout.PREFERRED_SIZE, 70, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(lbl_VideoCodec, javax.swing.GroupLayout.PREFERRED_SIZE, 69, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                .addComponent(txt_AudioCodec)
                                .addComponent(txt_VideoCodec))
                            .addGap(11, 11, 11)))))
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel3Layout.createSequentialGroup()
                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addComponent(chck_IsCensored)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(chck_IsDeprecated)
                                    .addComponent(cmb_Source, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(lbl_Source)))
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(cmb_Quality, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(lbl_Quality)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(chck_IsWatched)
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(updown_Version, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(lbl_Version)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(lbl_Dubs)
                            .addComponent(txt_Dubs, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(lbl_Subs)
                            .addComponent(txt_Subs, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(lbl_Crc)
                            .addComponent(txt_Crc, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_Resolution)
                            .addComponent(txt_Resolution, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                    .addGroup(jPanel3Layout.createSequentialGroup()
                        .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel3Layout.createSequentialGroup()
                                .addGap(24, 24, 24)
                                .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(txt_VideoCodec, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(lbl_VideoCodec)))
                            .addGroup(jPanel3Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                .addComponent(txt_AudioCodec, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addComponent(lbl_AudioCodec)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(lbl_CurrentFN)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.DEFAULT_SIZE, 84, Short.MAX_VALUE)))
                .addContainerGap())
        );

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("Anime Info:"));
        jPanel4.setOpaque(false);

        lbl_EpCount.setText("Episode Count:");

        updown_EpCount.setModel(new javax.swing.SpinnerNumberModel(Integer.valueOf(1), Integer.valueOf(1), null, Integer.valueOf(1)));
        updown_EpCount.setToolTipText("EpCount");
        updown_EpCount.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                UpDownChanged(evt);
            }
        });

        lbl_EpHiNo.setText("Highest Ep Number:");

        lbl_Type.setText("Type:");

        cmb_Type.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "TV Series", "Movie", "OVA", "Web" }));
        cmb_Type.setToolTipText("Type");
        cmb_Type.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                ComboChanged(evt);
            }
        });

        updown_EpHiNo.setModel(new javax.swing.SpinnerNumberModel(Integer.valueOf(1), Integer.valueOf(1), null, Integer.valueOf(1)));
        updown_EpHiNo.setToolTipText("EpHiNo");
        updown_EpHiNo.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                UpDownChanged(evt);
            }
        });

        lbl_YearBegin.setText("Year Begin:");

        txt_YearBegin.setText("2001");
        txt_YearBegin.setToolTipText("AYearBegin");
        txt_YearBegin.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_YearEnd.setText("Year End:");

        txt_YearEnd.setText("2002");
        txt_YearEnd.setToolTipText("AYearEnd");
        txt_YearEnd.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        txt_ACatList.setText("Clubs'Comedy'School Life'Seinen");
        txt_ACatList.setToolTipText("ACatList");
        txt_ACatList.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                TextChanged(evt);
            }
        });

        lbl_ACatList.setText("Categories");

        javax.swing.GroupLayout jPanel4Layout = new javax.swing.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(lbl_YearBegin)
                    .addComponent(lbl_YearEnd)
                    .addComponent(lbl_Type)
                    .addComponent(lbl_ACatList))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addComponent(cmb_Type, javax.swing.GroupLayout.PREFERRED_SIZE, 84, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addContainerGap())
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel4Layout.createSequentialGroup()
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(txt_ACatList, javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(jPanel4Layout.createSequentialGroup()
                                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                                    .addComponent(txt_YearBegin, javax.swing.GroupLayout.PREFERRED_SIZE, 69, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(txt_YearEnd))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel4Layout.createSequentialGroup()
                                        .addGap(22, 22, 22)
                                        .addComponent(lbl_EpCount))
                                    .addComponent(lbl_EpHiNo))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(updown_EpHiNo, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(updown_EpCount, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.PREFERRED_SIZE, 45, javax.swing.GroupLayout.PREFERRED_SIZE))))
                        .addGap(69, 69, 69))))
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel4Layout.createSequentialGroup()
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addComponent(lbl_YearBegin)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel4Layout.createSequentialGroup()
                                .addComponent(txt_YearBegin, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                                    .addComponent(txt_YearEnd, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                    .addComponent(lbl_YearEnd))))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(cmb_Type, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_Type)))
                    .addGroup(jPanel4Layout.createSequentialGroup()
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(updown_EpCount, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_EpCount))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(lbl_EpHiNo)
                            .addComponent(updown_EpHiNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel4Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_ACatList, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_ACatList)))
        );

        jPanel5.setBorder(javax.swing.BorderFactory.createTitledBorder("Episode Info:"));
        jPanel5.setOpaque(false);

        lbl_EpNo.setText("Episode Number:");

        updown_EpNo.setModel(new javax.swing.SpinnerNumberModel(Integer.valueOf(1), Integer.valueOf(1), null, Integer.valueOf(1)));
        updown_EpNo.setToolTipText("EpNo");
        updown_EpNo.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                UpDownChanged(evt);
            }
        });

        javax.swing.GroupLayout jPanel5Layout = new javax.swing.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(lbl_EpNo)
                .addGap(4, 4, 4)
                .addComponent(updown_EpNo, javax.swing.GroupLayout.PREFERRED_SIZE, 43, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addContainerGap(165, Short.MAX_VALUE))
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel5Layout.createSequentialGroup()
                .addGroup(jPanel5Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(updown_EpNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_EpNo))
                .addContainerGap(15, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout jPanel2Layout = new javax.swing.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, jPanel2Layout.createSequentialGroup()
                .addComponent(jPanel3, javax.swing.GroupLayout.PREFERRED_SIZE, 444, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jPanel4, javax.swing.GroupLayout.DEFAULT_SIZE, 315, Short.MAX_VALUE))
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(jPanel2Layout.createSequentialGroup()
                .addGap(0, 0, 0)
                .addGroup(jPanel2Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jPanel3, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(jPanel2Layout.createSequentialGroup()
                        .addComponent(jPanel4, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jPanel5, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))))
        );

        jTabbedPane1.addTab("Misc", jPanel2);

        tbl_Vars.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Name", "Value"
            }
        ) {
            Class[] types = new Class [] {
                java.lang.String.class, java.lang.String.class
            };
            boolean[] canEdit = new boolean [] {
                false, false
            };

            public Class getColumnClass(int columnIndex) {
                return types [columnIndex];
            }

            public boolean isCellEditable(int rowIndex, int columnIndex) {
                return canEdit [columnIndex];
            }
        });
        jScrollPane2.setViewportView(tbl_Vars);

        javax.swing.GroupLayout jPanel6Layout = new javax.swing.GroupLayout(jPanel6);
        jPanel6.setLayout(jPanel6Layout);
        jPanel6Layout.setHorizontalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane2, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, 775, Short.MAX_VALUE)
        );
        jPanel6Layout.setVerticalGroup(
            jPanel6Layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addComponent(jScrollPane2, javax.swing.GroupLayout.DEFAULT_SIZE, 193, Short.MAX_VALUE)
        );

        jTabbedPane1.addTab("Variables", jPanel6);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(lbl_CursorLocation)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(lbl_ErrorMsg, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_DirName)
                    .addComponent(lbl_FileName))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_DirNameStr, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(lbl_FileNameStr, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
            .addComponent(scrl_txt_CodeBox)
            .addComponent(jTabbedPane1)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addComponent(jTabbedPane1, javax.swing.GroupLayout.PREFERRED_SIZE, 221, javax.swing.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_FileName)
                    .addComponent(lbl_FileNameStr))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_DirName)
                    .addComponent(lbl_DirNameStr))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(scrl_txt_CodeBox, javax.swing.GroupLayout.DEFAULT_SIZE, 273, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_ErrorMsg)
                    .addComponent(lbl_CursorLocation)))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void txt_CodeBoxMousePressed(java.awt.event.MouseEvent evt) {//GEN-FIRST:event_txt_CodeBoxMousePressed
        UpdateCursorLocationLabel();
    }//GEN-LAST:event_txt_CodeBoxMousePressed

    private void txt_CodeBoxKeyPressed(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_CodeBoxKeyPressed
        UpdateCursorLocationLabel();
    }//GEN-LAST:event_txt_CodeBoxKeyPressed

    private void txt_CodeBoxKeyReleased(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_CodeBoxKeyReleased
        UpdateCursorLocationLabel();
        if(!evt.isActionKey()){
            TagSystemCodeChange();
        }
        
    }//GEN-LAST:event_txt_CodeBoxKeyReleased

    private void CheckedChanged(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_CheckedChanged
        TagSystemCodeChange();
    }//GEN-LAST:event_CheckedChanged

    private void UpDownChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_UpDownChanged
        TagSystemCodeChange();
    }//GEN-LAST:event_UpDownChanged

    private void ComboChanged(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_ComboChanged
        TagSystemCodeChange();
    }//GEN-LAST:event_ComboChanged

    private void TextChanged(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_TextChanged
        TagSystemCodeChange();
    }//GEN-LAST:event_TextChanged


    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JCheckBox chck_IsCensored;
    protected javax.swing.JCheckBox chck_IsDeprecated;
    protected javax.swing.JCheckBox chck_IsWatched;
    protected javax.swing.JComboBox cmb_Quality;
    protected javax.swing.JComboBox cmb_Source;
    protected javax.swing.JComboBox cmb_Type;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JPanel jPanel6;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JTabbedPane jTabbedPane1;
    private javax.swing.JLabel lbl_ACatList;
    protected javax.swing.JLabel lbl_AT_English;
    protected javax.swing.JLabel lbl_AT_Kanji;
    protected javax.swing.JLabel lbl_AT_Other;
    protected javax.swing.JLabel lbl_AT_Romaji;
    protected javax.swing.JLabel lbl_AT_Synomymn;
    private javax.swing.JLabel lbl_AudioCodec;
    private javax.swing.JLabel lbl_Crc;
    private javax.swing.JLabel lbl_CurrentFN;
    private javax.swing.JLabel lbl_CursorLocation;
    protected javax.swing.JLabel lbl_DirName;
    protected javax.swing.JLabel lbl_DirNameStr;
    private javax.swing.JLabel lbl_Dubs;
    protected javax.swing.JLabel lbl_ET_English;
    protected javax.swing.JLabel lbl_ET_Kanji;
    protected javax.swing.JLabel lbl_ET_Romaji;
    protected javax.swing.JLabel lbl_EpCount;
    protected javax.swing.JLabel lbl_EpHiNo;
    protected javax.swing.JLabel lbl_EpNo;
    private javax.swing.JLabel lbl_ErrorMsg;
    protected javax.swing.JLabel lbl_FileName;
    protected javax.swing.JLabel lbl_FileNameStr;
    protected javax.swing.JLabel lbl_GT_Long;
    protected javax.swing.JLabel lbl_GT_Short;
    protected javax.swing.JLabel lbl_Quality;
    private javax.swing.JLabel lbl_Resolution;
    protected javax.swing.JLabel lbl_Source;
    private javax.swing.JLabel lbl_Subs;
    protected javax.swing.JLabel lbl_Type;
    protected javax.swing.JLabel lbl_Version;
    private javax.swing.JLabel lbl_VideoCodec;
    private javax.swing.JLabel lbl_YearBegin;
    private javax.swing.JLabel lbl_YearEnd;
    protected javax.swing.JPanel pnl_AnimeTitles;
    protected javax.swing.JPanel pnl_EpTitles;
    protected javax.swing.JPanel pnl_GroupNames;
    protected javax.swing.JScrollPane scrl_txt_CodeBox;
    private javax.swing.JTable tbl_Vars;
    private javax.swing.JTextField txt_ACatList;
    protected javax.swing.JTextField txt_AT_English;
    protected javax.swing.JTextField txt_AT_Kanji;
    protected javax.swing.JTextField txt_AT_Other;
    protected javax.swing.JTextField txt_AT_Romaji;
    protected javax.swing.JTextField txt_AT_Synomymn;
    private javax.swing.JTextField txt_AudioCodec;
    protected javax.swing.JTextArea txt_CodeBox;
    private javax.swing.JTextField txt_Crc;
    private javax.swing.JTextArea txt_CurrentFN;
    private javax.swing.JTextField txt_Dubs;
    protected javax.swing.JTextField txt_ET_English;
    protected javax.swing.JTextField txt_ET_Kanji;
    protected javax.swing.JTextField txt_ET_Romaji;
    protected javax.swing.JTextField txt_GT_Long;
    protected javax.swing.JTextField txt_GT_Short;
    private javax.swing.JTextField txt_Resolution;
    private javax.swing.JTextField txt_Subs;
    private javax.swing.JTextField txt_VideoCodec;
    private javax.swing.JTextField txt_YearBegin;
    private javax.swing.JTextField txt_YearEnd;
    protected javax.swing.JSpinner updown_EpCount;
    protected javax.swing.JSpinner updown_EpHiNo;
    protected javax.swing.JSpinner updown_EpNo;
    protected javax.swing.JSpinner updown_Version;
    // End of variables declaration//GEN-END:variables

    public String TabName() { return "TagSystem"; }
    public int PreferredTabLocation() { return 4; }

    public void Initialize(IAniAdd aniAdd, IGUI gui) {
        this.gui = gui;

        InitDefaults();
        TagSystemCodeChange();
    }
    public void Terminate() {}
    public void GUIEventHandler(ComEvent comEvent) {}

    public void GainedFocus() {}
    public void LostFocus() {
        gui.RemoveTab("TagSystem");
    }

}

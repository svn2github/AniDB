package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.IAniAdd;
import java.util.TreeMap;
import processing.TagSystem;

public class GUI_TagSystem extends javax.swing.JPanel implements GUI.ITab {
    private IGUI gui;

    public GUI_TagSystem() {
        initComponents();
    }
    protected void InitDefaults(){
        txt_CodeBox.setText((String)gui.FromMem("TagSystemCode", txt_CodeBox.getText()));
    }

    protected void TagSystemCodeChange(){
        TagSystem ts = new TagSystem();
        TreeMap<String, String> tags = new TreeMap<String, String>();
        tags.put("ATr", txt_AT_Romaji.getText());
        tags.put("ATe", txt_AT_English.getText());
        tags.put("ATk", txt_AT_Kanji.getText());
        tags.put("ATs", txt_AT_Synomymn.getText());
        tags.put("ATo", txt_AT_Other.getText());

        tags.put("ETr", txt_ET_Romaji.getText());
        tags.put("ETe", txt_ET_English.getText());
        tags.put("ETk", txt_ET_Kanji.getText());

        tags.put("GTs", txt_GT_Short.getText());
        tags.put("GTl", txt_GT_Long.getText());

        tags.put("EpNo", updown_EpNo.getValue().toString());
        tags.put("EpHiNo", updown_EpHiNo.getValue().toString());
        tags.put("EpCount", updown_EpCount.getValue().toString());

        tags.put("Source", cmb_Source.getSelectedItem().toString());
        tags.put("Type", cmb_Type.getSelectedItem().toString());

        tags.put("Depr", chck_IsDeprecated.isSelected()?"1":"");
        tags.put("Cen", chck_IsCensored.isSelected()?"1":"");

        try {
            ts.parseAndTransform(txt_CodeBox.getText(), tags);
            lbl_FileNameStr.setText(tags.get("FileName"));
            lbl_DirNameStr.setText(tags.get("PathName"));
        } catch (Exception ex) {
            lbl_FileNameStr.setText("Error");
            lbl_DirNameStr.setText("Error");
        }


        gui.ToMem("TagSystemCode", txt_CodeBox.getText());
    }

    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        pnl_AnimeTitles = new javax.swing.JPanel();
        lbl_AnimeTitles = new javax.swing.JLabel();
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
        lbl_GroupNames = new javax.swing.JLabel();
        lbl_GT_Short = new javax.swing.JLabel();
        txt_GT_Short = new javax.swing.JTextField();
        lbl_GT_Long = new javax.swing.JLabel();
        txt_GT_Long = new javax.swing.JTextField();
        pnl_EpTitles = new javax.swing.JPanel();
        lbl_EpTitles = new javax.swing.JLabel();
        lbl_ET_English = new javax.swing.JLabel();
        txt_ET_English = new javax.swing.JTextField();
        lbl_ET_Romaji = new javax.swing.JLabel();
        txt_ET_Romaji = new javax.swing.JTextField();
        lbl_ET_Kanji = new javax.swing.JLabel();
        txt_ET_Kanji = new javax.swing.JTextField();
        scrl_txt_CodeBox = new javax.swing.JScrollPane();
        txt_CodeBox = new javax.swing.JTextArea();
        lbl_FileName = new javax.swing.JLabel();
        lbl_DirName = new javax.swing.JLabel();
        lbl_DirNameStr = new javax.swing.JLabel();
        lbl_FileNameStr = new javax.swing.JLabel();
        pnl_Misc = new javax.swing.JPanel();
        lbl_Misc = new javax.swing.JLabel();
        chck_IsCensored = new javax.swing.JCheckBox();
        chck_IsDeprecated = new javax.swing.JCheckBox();
        lbl_EpNo = new javax.swing.JLabel();
        updown_EpNo = new javax.swing.JSpinner();
        lbl_EpHiNo = new javax.swing.JLabel();
        updown_EpHiNo = new javax.swing.JSpinner();
        lbl_EpCount = new javax.swing.JLabel();
        updown_EpCount = new javax.swing.JSpinner();
        lbl_Type = new javax.swing.JLabel();
        cmb_Type = new javax.swing.JComboBox();
        lbl_Source = new javax.swing.JLabel();
        cmb_Source = new javax.swing.JComboBox();

        pnl_AnimeTitles.setOpaque(false);

        lbl_AnimeTitles.setText("Anime Titles:");

        lbl_AT_English.setText("English");

        txt_AT_English.setText("The Melancholy of Haruhi Suzumiya (2009)");
        txt_AT_English.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_AT_EnglishTxtChange(evt);
            }
        });

        lbl_AT_Romaji.setText("Romaji");

        txt_AT_Romaji.setText("Suzumiya Haruhi no Yuuutsu (2009)");
        txt_AT_Romaji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_AT_RomajiTxtChange(evt);
            }
        });

        lbl_AT_Kanji.setText("Kanji");

        txt_AT_Kanji.setText("涼宮ハルヒの憂鬱 (2009)");
        txt_AT_Kanji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_AT_KanjiTxtChange(evt);
            }
        });

        lbl_AT_Other.setText("Other");

        txt_AT_Other.setText("haruhi2");
        txt_AT_Other.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_AT_OtherTxtChange(evt);
            }
        });

        lbl_AT_Synomymn.setText("Synonymn");

        txt_AT_Synomymn.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_AT_SynomymnTxtChange(evt);
            }
        });

        javax.swing.GroupLayout pnl_AnimeTitlesLayout = new javax.swing.GroupLayout(pnl_AnimeTitles);
        pnl_AnimeTitles.setLayout(pnl_AnimeTitlesLayout);
        pnl_AnimeTitlesLayout.setHorizontalGroup(
            pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_AnimeTitlesLayout.createSequentialGroup()
                .addComponent(lbl_AnimeTitles)
                .addContainerGap(17, Short.MAX_VALUE))
            .addGroup(pnl_AnimeTitlesLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_AT_English)
                    .addComponent(lbl_AT_Romaji)
                    .addComponent(lbl_AT_Kanji)
                    .addComponent(lbl_AT_Other)
                    .addComponent(lbl_AT_Synomymn))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txt_AT_Synomymn, javax.swing.GroupLayout.DEFAULT_SIZE, 14, Short.MAX_VALUE)
                    .addComponent(txt_AT_Other, javax.swing.GroupLayout.DEFAULT_SIZE, 14, Short.MAX_VALUE)
                    .addComponent(txt_AT_Kanji, javax.swing.GroupLayout.DEFAULT_SIZE, 14, Short.MAX_VALUE)
                    .addComponent(txt_AT_Romaji, javax.swing.GroupLayout.DEFAULT_SIZE, 14, Short.MAX_VALUE)
                    .addComponent(txt_AT_English, javax.swing.GroupLayout.DEFAULT_SIZE, 14, Short.MAX_VALUE)))
        );
        pnl_AnimeTitlesLayout.setVerticalGroup(
            pnl_AnimeTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_AnimeTitlesLayout.createSequentialGroup()
                .addComponent(lbl_AnimeTitles)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
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
                    .addComponent(lbl_AT_Synomymn)))
        );

        pnl_GroupNames.setOpaque(false);

        lbl_GroupNames.setText("Group Names:");

        lbl_GT_Short.setText("Short");

        txt_GT_Short.setText("a.f.k.");
        txt_GT_Short.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_GT_ShortTxtChange(evt);
            }
        });

        lbl_GT_Long.setText("Long");

        txt_GT_Long.setText("a.f.k. (Long)");
        txt_GT_Long.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_GT_LongTxtChange(evt);
            }
        });

        javax.swing.GroupLayout pnl_GroupNamesLayout = new javax.swing.GroupLayout(pnl_GroupNames);
        pnl_GroupNames.setLayout(pnl_GroupNamesLayout);
        pnl_GroupNamesLayout.setHorizontalGroup(
            pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_GroupNamesLayout.createSequentialGroup()
                .addComponent(lbl_GroupNames)
                .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
            .addGroup(pnl_GroupNamesLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_GT_Short)
                    .addComponent(lbl_GT_Long))
                .addGap(21, 21, 21)
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txt_GT_Long, javax.swing.GroupLayout.DEFAULT_SIZE, 21, Short.MAX_VALUE)
                    .addComponent(txt_GT_Short, javax.swing.GroupLayout.DEFAULT_SIZE, 21, Short.MAX_VALUE)))
        );
        pnl_GroupNamesLayout.setVerticalGroup(
            pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_GroupNamesLayout.createSequentialGroup()
                .addComponent(lbl_GroupNames)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_GT_Short, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_GT_Short))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_GroupNamesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_GT_Long, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_GT_Long)))
        );

        pnl_EpTitles.setOpaque(false);

        lbl_EpTitles.setText("Episode Titles:");

        lbl_ET_English.setText("English");

        txt_ET_English.setText("Bamboo Leaf Rhapsody");
        txt_ET_English.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_ET_EnglishTxtChange(evt);
            }
        });

        lbl_ET_Romaji.setText("Romaji");

        txt_ET_Romaji.setText("Sasa no Ha Rhapsody");
        txt_ET_Romaji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_ET_RomajiTxtChange(evt);
            }
        });

        lbl_ET_Kanji.setText("Kanji");

        txt_ET_Kanji.setText("笹の葉ラプソディ");
        txt_ET_Kanji.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_ET_KanjiTxtChange(evt);
            }
        });

        javax.swing.GroupLayout pnl_EpTitlesLayout = new javax.swing.GroupLayout(pnl_EpTitles);
        pnl_EpTitles.setLayout(pnl_EpTitlesLayout);
        pnl_EpTitlesLayout.setHorizontalGroup(
            pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_EpTitlesLayout.createSequentialGroup()
                .addComponent(lbl_EpTitles)
                .addContainerGap(337, Short.MAX_VALUE))
            .addGroup(pnl_EpTitlesLayout.createSequentialGroup()
                .addContainerGap()
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_ET_English)
                    .addComponent(lbl_ET_Romaji)
                    .addComponent(lbl_ET_Kanji))
                .addGap(21, 21, 21)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(txt_ET_Kanji, javax.swing.GroupLayout.DEFAULT_SIZE, 342, Short.MAX_VALUE)
                    .addComponent(txt_ET_Romaji, javax.swing.GroupLayout.DEFAULT_SIZE, 342, Short.MAX_VALUE)
                    .addComponent(txt_ET_English, javax.swing.GroupLayout.DEFAULT_SIZE, 342, Short.MAX_VALUE)))
        );
        pnl_EpTitlesLayout.setVerticalGroup(
            pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_EpTitlesLayout.createSequentialGroup()
                .addComponent(lbl_EpTitles)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_ET_English, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_ET_English))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_ET_Romaji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_ET_Romaji))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(pnl_EpTitlesLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(txt_ET_Kanji, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(lbl_ET_Kanji)))
        );

        txt_CodeBox.setColumns(20);
        txt_CodeBox.setFont(new java.awt.Font("Consolas", 0, 10));
        txt_CodeBox.setRows(5);
        txt_CodeBox.setText("AT:=[%ATr%,%ATe%,%ATk%]\nET:=[%ETe%,%ETr%,%ETk%]\nGT:=[%GTs%,%GTl%]\nEpNoPad:=$pad(%EpNo%,$max($len(%EpHiNo%),$len(%EpCount%)),\"0\")\nSrcStyled:=\"[\"%Source%\"]\"\nisMovieType:={%Type%=\"Movie\"?\"True\":\"False\"}\nisDepr:={%Depr%?\"[Depr]\":\"\"}\nisCen:={%Cen%?\"[Cen]\":\"\"}\n\nFileName:=%AT%\" \"%EpNoPad%\" - \"%ET%\" \"%GT% %isDepr% %isCen% %SrcStyled%\nPathName:=\"E:\\Anime\\!Processed\\\"%AT%");
        txt_CodeBox.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyReleased(java.awt.event.KeyEvent evt) {
                txt_CodeBoxTxtChange(evt);
            }
        });
        scrl_txt_CodeBox.setViewportView(txt_CodeBox);

        lbl_FileName.setText("Filename:");

        lbl_DirName.setText("Directoryname:");

        lbl_DirNameStr.setText(" ");

        lbl_FileNameStr.setText(" ");

        pnl_Misc.setOpaque(false);

        lbl_Misc.setText("Misc Options:");

        chck_IsCensored.setText("Is Censored");
        chck_IsCensored.setOpaque(false);
        chck_IsCensored.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_IsCensoredCheckedChange(evt);
            }
        });

        chck_IsDeprecated.setText("Is Deprecated");
        chck_IsDeprecated.setOpaque(false);
        chck_IsDeprecated.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                chck_IsDeprecatedCheckedChange(evt);
            }
        });

        lbl_EpNo.setText("Episode Number:");

        updown_EpNo.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                updown_EpNoSpinnerChange(evt);
            }
        });

        lbl_EpHiNo.setText("Highest Ep Number:");

        updown_EpHiNo.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                updown_EpHiNoSpinnerChange(evt);
            }
        });

        lbl_EpCount.setText("Episode Count:");

        updown_EpCount.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                updown_EpCountSpinnerChange(evt);
            }
        });

        lbl_Type.setText("Type:");

        cmb_Type.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "TV Series", "Movie", "OVA" }));
        cmb_Type.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmb_TypeDropDownChange(evt);
            }
        });

        lbl_Source.setText("Source:");

        cmb_Source.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Web", "DTV", "HDTV", "DVD", "B-R" }));
        cmb_Source.setSelectedIndex(2);
        cmb_Source.addItemListener(new java.awt.event.ItemListener() {
            public void itemStateChanged(java.awt.event.ItemEvent evt) {
                cmb_SourceDropDownChange(evt);
            }
        });

        javax.swing.GroupLayout pnl_MiscLayout = new javax.swing.GroupLayout(pnl_Misc);
        pnl_Misc.setLayout(pnl_MiscLayout);
        pnl_MiscLayout.setHorizontalGroup(
            pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_MiscLayout.createSequentialGroup()
                .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_Misc)
                    .addGroup(pnl_MiscLayout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(chck_IsCensored)
                            .addComponent(chck_IsDeprecated))
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                            .addGroup(pnl_MiscLayout.createSequentialGroup()
                                .addGap(8, 8, 8)
                                .addComponent(lbl_Type)
                                .addGap(18, 18, 18)
                                .addComponent(cmb_Type, javax.swing.GroupLayout.PREFERRED_SIZE, 81, javax.swing.GroupLayout.PREFERRED_SIZE))
                            .addGroup(pnl_MiscLayout.createSequentialGroup()
                                .addGap(6, 6, 6)
                                .addComponent(lbl_Source)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(cmb_Source, 0, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, pnl_MiscLayout.createSequentialGroup()
                                .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(lbl_EpHiNo)
                                    .addComponent(lbl_EpNo))
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED))
                            .addGroup(pnl_MiscLayout.createSequentialGroup()
                                .addComponent(lbl_EpCount)
                                .addGap(26, 26, 26)))
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addComponent(updown_EpCount, javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(updown_EpHiNo)
                            .addComponent(updown_EpNo, javax.swing.GroupLayout.DEFAULT_SIZE, 53, Short.MAX_VALUE))))
                .addGap(10, 10, 10))
        );
        pnl_MiscLayout.setVerticalGroup(
            pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(pnl_MiscLayout.createSequentialGroup()
                .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(pnl_MiscLayout.createSequentialGroup()
                        .addComponent(lbl_Misc)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(chck_IsCensored)
                            .addComponent(cmb_Type, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_Type))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(chck_IsDeprecated)
                            .addComponent(cmb_Source, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_Source)))
                    .addGroup(pnl_MiscLayout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(updown_EpNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_EpNo))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(updown_EpHiNo, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_EpHiNo))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(pnl_MiscLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                            .addComponent(updown_EpCount, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                            .addComponent(lbl_EpCount))))
                .addContainerGap(23, Short.MAX_VALUE))
        );

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(pnl_GroupNames, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_AnimeTitles, javax.swing.GroupLayout.Alignment.TRAILING, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                    .addComponent(pnl_EpTitles, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_Misc, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_DirName)
                    .addComponent(lbl_FileName))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(lbl_DirNameStr, javax.swing.GroupLayout.DEFAULT_SIZE, 412, Short.MAX_VALUE)
                    .addComponent(lbl_FileNameStr, javax.swing.GroupLayout.DEFAULT_SIZE, 412, Short.MAX_VALUE))
                .addGap(2, 2, 2))
            .addComponent(scrl_txt_CodeBox, javax.swing.GroupLayout.DEFAULT_SIZE, 494, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(pnl_AnimeTitles, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(pnl_GroupNames, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(pnl_EpTitles, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(pnl_Misc, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_FileName)
                    .addComponent(lbl_FileNameStr))
                .addGap(2, 2, 2)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(lbl_DirName)
                    .addComponent(lbl_DirNameStr))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(scrl_txt_CodeBox, javax.swing.GroupLayout.DEFAULT_SIZE, 278, Short.MAX_VALUE))
        );
    }// </editor-fold>//GEN-END:initComponents

    private void txt_AT_EnglishTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_AT_EnglishTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_AT_EnglishTxtChange

    private void txt_AT_RomajiTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_AT_RomajiTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_AT_RomajiTxtChange

    private void txt_AT_KanjiTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_AT_KanjiTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_AT_KanjiTxtChange

    private void txt_AT_OtherTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_AT_OtherTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_AT_OtherTxtChange

    private void txt_AT_SynomymnTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_AT_SynomymnTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_AT_SynomymnTxtChange

    private void txt_GT_ShortTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_GT_ShortTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_GT_ShortTxtChange

    private void txt_GT_LongTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_GT_LongTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_GT_LongTxtChange

    private void txt_ET_EnglishTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_ET_EnglishTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_ET_EnglishTxtChange

    private void txt_ET_RomajiTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_ET_RomajiTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_ET_RomajiTxtChange

    private void txt_ET_KanjiTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_ET_KanjiTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_ET_KanjiTxtChange

    private void txt_CodeBoxTxtChange(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_txt_CodeBoxTxtChange
        TagSystemCodeChange();
}//GEN-LAST:event_txt_CodeBoxTxtChange

    private void chck_IsCensoredCheckedChange(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_IsCensoredCheckedChange
        TagSystemCodeChange();
}//GEN-LAST:event_chck_IsCensoredCheckedChange

    private void chck_IsDeprecatedCheckedChange(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_chck_IsDeprecatedCheckedChange
        TagSystemCodeChange();
}//GEN-LAST:event_chck_IsDeprecatedCheckedChange

    private void updown_EpNoSpinnerChange(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_updown_EpNoSpinnerChange
        TagSystemCodeChange();
}//GEN-LAST:event_updown_EpNoSpinnerChange

    private void updown_EpHiNoSpinnerChange(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_updown_EpHiNoSpinnerChange
        TagSystemCodeChange();
}//GEN-LAST:event_updown_EpHiNoSpinnerChange

    private void updown_EpCountSpinnerChange(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_updown_EpCountSpinnerChange
        TagSystemCodeChange();
}//GEN-LAST:event_updown_EpCountSpinnerChange

    private void cmb_TypeDropDownChange(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmb_TypeDropDownChange
        TagSystemCodeChange();
}//GEN-LAST:event_cmb_TypeDropDownChange

    private void cmb_SourceDropDownChange(java.awt.event.ItemEvent evt) {//GEN-FIRST:event_cmb_SourceDropDownChange
        TagSystemCodeChange();
}//GEN-LAST:event_cmb_SourceDropDownChange


    // Variables declaration - do not modify//GEN-BEGIN:variables
    protected javax.swing.JCheckBox chck_IsCensored;
    protected javax.swing.JCheckBox chck_IsDeprecated;
    protected javax.swing.JComboBox cmb_Source;
    protected javax.swing.JComboBox cmb_Type;
    protected javax.swing.JLabel lbl_AT_English;
    protected javax.swing.JLabel lbl_AT_Kanji;
    protected javax.swing.JLabel lbl_AT_Other;
    protected javax.swing.JLabel lbl_AT_Romaji;
    protected javax.swing.JLabel lbl_AT_Synomymn;
    protected javax.swing.JLabel lbl_AnimeTitles;
    protected javax.swing.JLabel lbl_DirName;
    protected javax.swing.JLabel lbl_DirNameStr;
    protected javax.swing.JLabel lbl_ET_English;
    protected javax.swing.JLabel lbl_ET_Kanji;
    protected javax.swing.JLabel lbl_ET_Romaji;
    protected javax.swing.JLabel lbl_EpCount;
    protected javax.swing.JLabel lbl_EpHiNo;
    protected javax.swing.JLabel lbl_EpNo;
    protected javax.swing.JLabel lbl_EpTitles;
    protected javax.swing.JLabel lbl_FileName;
    protected javax.swing.JLabel lbl_FileNameStr;
    protected javax.swing.JLabel lbl_GT_Long;
    protected javax.swing.JLabel lbl_GT_Short;
    protected javax.swing.JLabel lbl_GroupNames;
    protected javax.swing.JLabel lbl_Misc;
    protected javax.swing.JLabel lbl_Source;
    protected javax.swing.JLabel lbl_Type;
    protected javax.swing.JPanel pnl_AnimeTitles;
    protected javax.swing.JPanel pnl_EpTitles;
    protected javax.swing.JPanel pnl_GroupNames;
    protected javax.swing.JPanel pnl_Misc;
    protected javax.swing.JScrollPane scrl_txt_CodeBox;
    protected javax.swing.JTextField txt_AT_English;
    protected javax.swing.JTextField txt_AT_Kanji;
    protected javax.swing.JTextField txt_AT_Other;
    protected javax.swing.JTextField txt_AT_Romaji;
    protected javax.swing.JTextField txt_AT_Synomymn;
    protected javax.swing.JTextArea txt_CodeBox;
    protected javax.swing.JTextField txt_ET_English;
    protected javax.swing.JTextField txt_ET_Kanji;
    protected javax.swing.JTextField txt_ET_Romaji;
    protected javax.swing.JTextField txt_GT_Long;
    protected javax.swing.JTextField txt_GT_Short;
    protected javax.swing.JSpinner updown_EpCount;
    protected javax.swing.JSpinner updown_EpHiNo;
    protected javax.swing.JSpinner updown_EpNo;
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

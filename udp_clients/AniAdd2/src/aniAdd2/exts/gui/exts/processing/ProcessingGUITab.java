/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.exts.processing;

import aniAdd2.IAniAdd2;
import aniAdd2.exts.gui.components.GroupBox;
import aniAdd2.exts.gui.components.TriStateCheckBox;
import aniAdd2.exts.gui.exts.processing.icons.IconsCell;
import aniAdd2.exts.processing.AA2Processing;
import aniAdd2.exts.processing.FileInfo;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.ParseException;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JSeparator;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Arokh
 */
public class ProcessingGUITab extends JPanel {
	private final IAniAdd2 aniAdd2;

	public ProcessingGUITab(IAniAdd2 aniAdd2) {
		this.aniAdd2 = aniAdd2;

		initComponents();
	}

	private void initComponents(){
		setOpaque(false);
		
		JPanel pnl_Status = createStatusPanel();
		JPanel pnl_Buttons = createButtonsPanel();
		JPanel pnl_Control = createControlPanel();
		JTable tbl_Files = createFilesTable();

		JScrollPane scr_tbl_Files = new JScrollPane();
		scr_tbl_Files.setViewportView(tbl_Files);

		GroupLayout layout = new GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup()
                    .addComponent(scr_tbl_Files, 0, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_Status, 0, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                .addGroup(layout.createParallelGroup()
                    .addComponent(pnl_Buttons, 150, 150, 150)
                    .addComponent(pnl_Control, 150, 150, 150)))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup()
                    .addComponent(scr_tbl_Files, 0, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_Control, 0, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                .addGroup(layout.createParallelGroup()
                    .addComponent(pnl_Buttons, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(pnl_Status, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE)))
        );
	}


	private JTable createFilesTable() {
		JTable table = new JTable();
		table.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
			private IconsCell cellComponent = new IconsCell();

			@Override
			public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
				FileInfo fileInfo = (FileInfo)value;

				if(column == 0) {
					return super.getTableCellRendererComponent(table, fileInfo.getPath(), isSelected, hasFocus, row, column);
				} else {
					cellComponent.setFile(fileInfo);
					return cellComponent;
				}
			}
		});
		final AA2Processing processingExt = (AA2Processing)aniAdd2.getExtension(AA2Processing.class);
		table.setModel(new DefaultTableModel() {
			AA2Processing processing = processingExt;

			@Override
			public int getRowCount() {
				return processing != null ? processing.fileCount() : 0;
			}

			@Override
			public int getColumnCount() {
				return 2;
			}

			@Override
			public String getColumnName(int columnIndex) {
				return columnIndex == 0 ? "Filename" : "Progress";
			}

			@Override
			public Class<?> getColumnClass(int columnIndex) {
				return columnIndex == 0 ? String[].class : Image[].class;
			}

			@Override
			public Object getValueAt(int rowIndex, int columnIndex) {
				return processing.index2FileInfo(rowIndex);
			}
		});
		table.setRowHeight(19);
		table.getColumnModel().getColumn(1).setMaxWidth(72);

		return table;
	}
	private JPanel createButtonsPanel(){
		JButton btn_Start = new JButton("Start");
		btn_Start.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_stop = new JButton("Stop");
		btn_stop.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_pause = new JButton("Pause");
		btn_pause.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_Clear = new JButton("Clear");
		btn_Clear.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_resume = new JButton("Resume");
		btn_resume.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_AddFiles = new JButton("Add Files");
		btn_AddFiles.setMargin(new Insets(0, 0, 0, 0));
		JButton btn_AddFolders = new JButton("Add Folders");
		btn_AddFolders.setMargin(new Insets(0, 0, 0, 0));

		JPanel pnl_Buttons = new JPanel(new GridLayout(0, 2));
		pnl_Buttons.setOpaque(false);
		pnl_Buttons.add(btn_Start);
		pnl_Buttons.add(btn_Clear);
		pnl_Buttons.add(btn_AddFiles);
		pnl_Buttons.add(btn_AddFolders);

		return pnl_Buttons;
	}
	private JPanel createStatusPanel(){
		JProgressBar prg_File = new JProgressBar();
		JProgressBar prg_Total = new JProgressBar();

		JLabel lbl_BytesProcessed = new JLabel("0 of 0 MB processed");
		JLabel lbl_TimeElapsed = new JLabel("Time elapsed: 0:00:00");
		lbl_TimeElapsed.setHorizontalAlignment(SwingConstants.CENTER);
		JLabel lbl_TimeRemaining = new JLabel("Time remaining: 0:00:00");
		lbl_TimeRemaining.setHorizontalAlignment(SwingConstants.RIGHT);

		JPanel pnl_Stats = new JPanel(new GridLayout(1, 0));
		pnl_Stats.setOpaque(false);
		pnl_Stats.add(lbl_BytesProcessed);
		pnl_Stats.add(lbl_TimeElapsed);
		pnl_Stats.add(lbl_TimeRemaining);

		JPanel pnl_Status = new JPanel(new GridLayout(0, 1));
		pnl_Status.setOpaque(false);
		pnl_Status.add(prg_File);
		pnl_Status.add(prg_Total);
		pnl_Status.add(pnl_Stats);
	
		return pnl_Status;
	}
	private JPanel createControlPanel(){
		JPanel pnl_Control = new JPanel();
		pnl_Control.setOpaque(false);

		JPanel pnl_ControlSettings = createControlSettingsPanel();
		JPanel pnl_ControlVote = createControlVotePanel();

        GroupLayout layout = new GroupLayout(pnl_Control);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addComponent(pnl_ControlSettings, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addComponent(pnl_ControlVote, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(pnl_ControlSettings)
				.addGap(8)
                .addComponent(pnl_ControlVote))
        );
		pnl_Control.setLayout(layout);

		return pnl_Control;
	}	
	private JPanel createControlSettingsPanel(){
		GroupBox grp_ControlSettings = GroupBox.createExpander(new JLabel("Settings"));
		grp_ControlSettings.setOpaque(false);

 		JPanel pnl_ControlSettings = grp_ControlSettings.getContentPane(); //new JPanel();

        final JComboBox cmb_FileState = new JComboBox(new String[] { "Unknown", "Internal", "External", "Deleted" });
        final TriStateCheckBox chck_AddFileToMyList = new TriStateCheckBox("Add File to MyList", TriStateCheckBox.SELECTED);
        final TriStateCheckBox chck_SetWatchedState = new TriStateCheckBox("Watched State", TriStateCheckBox.SELECTED);
        final JRadioButton ptn_SetWatched = new JRadioButton("Set", true);
        final JRadioButton ptn_SetUnwatched = new JRadioButton("Unset", false);
        final TriStateCheckBox chck_RenameFile = new TriStateCheckBox("Rename", TriStateCheckBox.SELECTED);
        final TriStateCheckBox chck_MoveFile = new TriStateCheckBox("Move", TriStateCheckBox.NOT_SELECTED);

		cmb_FileState.setSelectedIndex(1);
		ButtonGroup btngrp_Watched = new ButtonGroup();
		btngrp_Watched.add(ptn_SetWatched);
		btngrp_Watched.add(ptn_SetUnwatched);

		cmb_FileState.setOpaque(false);
		chck_AddFileToMyList.setOpaque(false);
		chck_SetWatchedState.setOpaque(false);
		ptn_SetWatched.setOpaque(false);
		ptn_SetUnwatched.setOpaque(false);
		chck_RenameFile.setOpaque(false);
		chck_MoveFile.setOpaque(false);
   
        chck_SetWatchedState.setMargin(new java.awt.Insets(2, 2, 0, 2));
        ptn_SetWatched.setMargin(new java.awt.Insets(0, 2, 2, 2));
        ptn_SetUnwatched.setMargin(new java.awt.Insets(0, 2, 2, 2));

		JSeparator spr_MLMove = new JSeparator(SwingConstants.HORIZONTAL);

		ChangeListener enableBehavior = new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				boolean mlOptions = chck_AddFileToMyList.getState() == TriStateCheckBox.SELECTED;
				boolean mlOptionsSetWatched = chck_SetWatchedState.getState() == TriStateCheckBox.SELECTED;

				cmb_FileState.setEnabled(mlOptions);
				chck_SetWatchedState.setEnabled(mlOptions);
				ptn_SetWatched.setEnabled(mlOptions && mlOptionsSetWatched);
				ptn_SetUnwatched.setEnabled(mlOptions && mlOptionsSetWatched);

				//chck_RenameFile.setEnabled();
				//chck_MoveFile.setEnabled();
			}
		};
		chck_AddFileToMyList.addChangeListener(enableBehavior);
		chck_SetWatchedState.addChangeListener(enableBehavior);
		enableBehavior.stateChanged(null);


        GroupLayout layout = new GroupLayout(pnl_ControlSettings);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addComponent(chck_AddFileToMyList, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addComponent(chck_SetWatchedState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)//)
            .addGroup(layout.createSequentialGroup()
                .addGap(20)
                .addComponent(ptn_SetWatched)
                .addComponent(ptn_SetUnwatched, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
			.addComponent(spr_MLMove)
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_RenameFile)
                .addComponent(chck_MoveFile, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_AddFileToMyList)
                .addComponent(chck_SetWatchedState)
                .addGroup(layout.createParallelGroup()
                    .addComponent(ptn_SetWatched)
                    .addComponent(ptn_SetUnwatched))
                .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE)
				.addGap(4)
				.addComponent(spr_MLMove)
                .addGroup(layout.createParallelGroup()
                    .addComponent(chck_RenameFile)
                    .addComponent(chck_MoveFile)))
        );

        pnl_ControlSettings.setLayout(layout);


		return grp_ControlSettings;
	}
	private JPanel createControlVotePanel(){
		final TriStateCheckBox chck_SetVote = new TriStateCheckBox("Episode Voting");
 		final GroupBox grp_ControlVote = GroupBox.createExpander(chck_SetVote);
        final JRadioButton ptn_RevokeVote = new JRadioButton("Revoke", false);
        final JRadioButton ptn_SetVote = new JRadioButton("Set Vote", true);
        final JFormattedTextField ftxt_Vote = new JFormattedTextField(new JFormattedTextField.AbstractFormatter() {
			@Override
			public Object stringToValue(String text) throws ParseException {
				try {
					text = text.replaceAll("\\.", "");
					int vote = Integer.parseInt(text);

					if(vote >= 1 && vote <= 10) vote= vote*100;

					if(/*(vote < 1 || vote > 10) &&*/ (vote < 100 || vote > 1000)) throw new ParseException("Invalid Vote", 0);

					return vote;
				} catch(Exception e) {
					throw new ParseException("Parsing error", 0);
				}
			}
			@Override
			public String valueToString(Object value) throws ParseException {
				if(value instanceof Integer){
					String str = String.format("%04d", value);
					return str.substring(0, 2) + "." + str.substring(2, 4);
				} else {
					throw new ParseException("Not an integer", 0);
				}
			}
		});
		final JPanel pnl_ControlVotePane = grp_ControlVote.getContentPane();
		
		chck_SetVote.setOpaque(false);
		chck_SetVote.setBorder(BorderFactory.createEmptyBorder());
		ptn_RevokeVote.setOpaque(false);
 		ptn_SetVote.setOpaque(false);
		grp_ControlVote.setOpaque(false);
		ftxt_Vote.setHorizontalAlignment(JTextField.RIGHT);
		ftxt_Vote.setText("05.50");

		ButtonGroup btngrp_Vote = new ButtonGroup();
		btngrp_Vote.add(ptn_RevokeVote);
		btngrp_Vote.add(ptn_SetVote);

		ChangeListener enableBehavior = new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				boolean voteEnabled = chck_SetVote.getState() == TriStateCheckBox.SELECTED;
				boolean setVote = ptn_SetVote.isSelected();

				grp_ControlVote.setEnabled(voteEnabled);
				ptn_RevokeVote.setEnabled(voteEnabled);
				ptn_SetVote.setEnabled(voteEnabled);
				ftxt_Vote.setEnabled(voteEnabled && setVote);
				pnl_ControlVotePane.setEnabled(voteEnabled);
			}
		};
		chck_SetVote.addChangeListener(enableBehavior);
		ptn_RevokeVote.addChangeListener(enableBehavior);
		ptn_SetVote.addChangeListener(enableBehavior);
		enableBehavior.stateChanged(null);

        GroupLayout layout = new GroupLayout(pnl_ControlVotePane);
        pnl_ControlVotePane.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(ptn_SetVote)
                .addComponent(ftxt_Vote))
            .addComponent(ptn_RevokeVote, GroupLayout.DEFAULT_SIZE, 122, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                    .addComponent(ptn_SetVote)
                    .addComponent(ftxt_Vote, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE))
                .addComponent(ptn_RevokeVote))
        );

		return grp_ControlVote;
	}
}
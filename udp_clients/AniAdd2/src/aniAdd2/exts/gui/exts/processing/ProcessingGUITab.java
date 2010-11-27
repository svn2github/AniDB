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
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
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
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author Arokh
 */
public class ProcessingGUITab extends JPanel {
	private final IAniAdd2 aniAdd2;
	private final AA2Processing processingExt;


	JComboBox cmb_FileState;
	TriStateCheckBox chck_UpdateMyList;
	TriStateCheckBox chck_SetWatchedState;
	JRadioButton ptn_SetWatched;
	JRadioButton ptn_SetUnwatched;
    JRadioButton ptn_MLAdd;
    JRadioButton ptn_MLRemove;
	JRadioButton ptn_WatchedUnchanged;
	JRadioButton ptn_MLUnchanged;
	JRadioButton ptn_VoteUnchanged;
	TriStateCheckBox chck_RenameMoveFile;

	TriStateCheckBox chck_SetVote;
	GroupBox grp_ControlVote;
	JRadioButton ptn_RevokeVote;
	JRadioButton ptn_SetVote;
	JFormattedTextField ftxt_Vote;
	JPanel pnl_ControlVotePane;

	JTable tbl_Files;

	OptionsUpdater optionsUpdater = new OptionsUpdater();


	public ProcessingGUITab(IAniAdd2 aniAdd2) {
		this.aniAdd2 = aniAdd2;
		processingExt = (AA2Processing)aniAdd2.getExtension(AA2Processing.class);
		initComponents();
	}

	private void initComponents(){
		setOpaque(false);
		
		JTable tbl_Files = createFilesTable();
		JPanel pnl_Status = createStatusPanel();
		JPanel pnl_Buttons = createButtonsPanel();
		JPanel pnl_Control = createControlPanel();

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
		final JTable tbl_Files = this.tbl_Files = new JTable();

		tbl_Files.setDefaultRenderer(Object.class, new DefaultTableCellRenderer() {
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
		tbl_Files.setModel(new DefaultTableModel() {

			@Override
			public int getRowCount() {
				return processingExt != null ? processingExt.fileCount() : 0;
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
				return processingExt.index2FileInfo(rowIndex);
			}
		});

		tbl_Files.getSelectionModel().addListSelectionListener(optionsUpdater);

		tbl_Files.setRowHeight(19);
		tbl_Files.getColumnModel().getColumn(1).setMaxWidth(87);
		tbl_Files.getColumnModel().getColumn(1).setMinWidth(87);

		return tbl_Files;
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

        final JComboBox cmb_FileState = this.cmb_FileState = new JComboBox(FileInfo.MLStorageState.values());//new String[] { "Unknown", "Internal", "External", "Deleted" });
        final TriStateCheckBox chck_UpdateMyList = this.chck_UpdateMyList = new TriStateCheckBox("Update MyList", TriStateCheckBox.SELECTED);
        final TriStateCheckBox chck_SetWatchedState = this.chck_SetWatchedState = new TriStateCheckBox("Watched State", TriStateCheckBox.SELECTED);
        final JRadioButton ptn_SetWatched = this.ptn_SetWatched = new JRadioButton("Set", true);
        final JRadioButton ptn_SetUnwatched = this.ptn_SetUnwatched = new JRadioButton("Unset", false);
		final JRadioButton ptn_WatchedUnchanged = this.ptn_WatchedUnchanged = new JRadioButton();
        final JRadioButton ptn_MLAdd = this.ptn_MLAdd = new JRadioButton("Add", true);
        final JRadioButton ptn_MLRemove = this.ptn_MLRemove = new JRadioButton("Remove", false);
		final JRadioButton ptn_MLUnchanged = this.ptn_MLUnchanged = new JRadioButton();
        final TriStateCheckBox chck_RenameMoveFile = this.chck_RenameMoveFile = new TriStateCheckBox("Rename/Move", TriStateCheckBox.SELECTED);

		cmb_FileState.setSelectedIndex(1);
		ButtonGroup btngrp_Watched = new ButtonGroup();
		btngrp_Watched.add(ptn_SetWatched);
		btngrp_Watched.add(ptn_SetUnwatched);
		btngrp_Watched.add(ptn_WatchedUnchanged);

		ButtonGroup btngrp_MLState = new ButtonGroup();
		btngrp_MLState.add(ptn_MLAdd);
		btngrp_MLState.add(ptn_MLRemove);
		btngrp_MLState.add(ptn_MLUnchanged);

		cmb_FileState.setOpaque(false);
		chck_UpdateMyList.setOpaque(false);
		chck_SetWatchedState.setOpaque(false);
		ptn_SetWatched.setOpaque(false);
		ptn_SetUnwatched.setOpaque(false);
		ptn_MLAdd.setOpaque(false);
		ptn_MLRemove.setOpaque(false);
		chck_RenameMoveFile.setOpaque(false);
  
        chck_UpdateMyList.setMargin(new java.awt.Insets(2, 2, 0, 2));
        ptn_MLAdd.setMargin(new java.awt.Insets(0, 2, 2, 2));
        ptn_MLRemove.setMargin(new java.awt.Insets(0, 2, 2, 2));

        chck_SetWatchedState.setMargin(new java.awt.Insets(2, 2, 0, 2));
        ptn_SetWatched.setMargin(new java.awt.Insets(0, 2, 2, 2));
        ptn_SetUnwatched.setMargin(new java.awt.Insets(0, 2, 2, 2));

		JSeparator spr_MLMove = new JSeparator(SwingConstants.HORIZONTAL);

		ChangeListener enableBehavior = new ChangeListener() {
			public void stateChanged(ChangeEvent e) {
				boolean mlOptions = chck_UpdateMyList.getState() == TriStateCheckBox.SELECTED;
				boolean mlOptionsAdd = ptn_MLAdd.isSelected();
				boolean mlOptionsSetWatched = chck_SetWatchedState.getState() == TriStateCheckBox.SELECTED;
				boolean mlOptionsSetUnwatched = ptn_SetUnwatched.isSelected();

				cmb_FileState.setEnabled(mlOptions && mlOptionsAdd);
				chck_SetWatchedState.setEnabled(mlOptions && mlOptionsAdd);
				ptn_SetWatched.setEnabled(mlOptions && mlOptionsAdd && mlOptionsSetWatched);
				ptn_SetUnwatched.setEnabled(mlOptions && mlOptionsAdd && mlOptionsSetWatched);
				ptn_MLAdd.setEnabled(mlOptions);
				ptn_MLRemove.setEnabled(mlOptions);
			}
		};
		chck_UpdateMyList.addChangeListener(enableBehavior);
		chck_SetWatchedState.addChangeListener(enableBehavior);
		ptn_SetUnwatched.addChangeListener(enableBehavior);
		ptn_MLAdd.addChangeListener(enableBehavior);
		enableBehavior.stateChanged(null);


		cmb_FileState.addItemListener(optionsUpdater);
		chck_UpdateMyList.addItemListener(optionsUpdater);
		chck_SetWatchedState.addItemListener(optionsUpdater);
		ptn_SetWatched.addActionListener(optionsUpdater);
		ptn_SetUnwatched.addActionListener(optionsUpdater);
		ptn_WatchedUnchanged.addActionListener(optionsUpdater);
		ptn_MLAdd.addActionListener(optionsUpdater);
		ptn_MLRemove.addActionListener(optionsUpdater);
		ptn_MLUnchanged.addActionListener(optionsUpdater);
		chck_RenameMoveFile.addItemListener(optionsUpdater);



        GroupLayout layout = new GroupLayout(pnl_ControlSettings);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addComponent(chck_UpdateMyList, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addGroup(layout.createSequentialGroup()
                .addGap(20)
                .addComponent(ptn_MLAdd)
                .addComponent(ptn_MLRemove, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addComponent(chck_SetWatchedState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)//)
            .addGroup(layout.createSequentialGroup()
                .addGap(20)
                .addComponent(ptn_SetWatched)
                .addComponent(ptn_SetUnwatched, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
			.addComponent(spr_MLMove)
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_RenameMoveFile))
                //.addComponent(chck_MoveFile, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_UpdateMyList)
                .addGroup(layout.createParallelGroup()
                    .addComponent(ptn_MLAdd)
                    .addComponent(ptn_MLRemove))
                .addComponent(chck_SetWatchedState)
                .addGroup(layout.createParallelGroup()
                    .addComponent(ptn_SetWatched)
                    .addComponent(ptn_SetUnwatched))
                .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE)
				.addGap(4)
				.addComponent(spr_MLMove)
                .addGroup(layout.createParallelGroup()
                    .addComponent(chck_RenameMoveFile)))
                    //.addComponent(chck_MoveFile)
        );

        pnl_ControlSettings.setLayout(layout);


		return grp_ControlSettings;
	}
	private JPanel createControlVotePanel(){
		final TriStateCheckBox chck_SetVote = this.chck_SetVote = new TriStateCheckBox("Episode Voting");
 		final GroupBox grp_ControlVote = this.grp_ControlVote = GroupBox.createExpander(chck_SetVote);
        final JRadioButton ptn_RevokeVote = this.ptn_RevokeVote = new JRadioButton("Revoke", false);
        final JRadioButton ptn_SetVote = this.ptn_SetVote = new JRadioButton("Set Vote", true);
        final JRadioButton ptn_VoteUnchanged = this.ptn_VoteUnchanged = new JRadioButton();
        final JFormattedTextField ftxt_Vote = this.ftxt_Vote = new JFormattedTextField(new JFormattedTextField.AbstractFormatter() {
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
		final JPanel pnl_ControlVotePane = this.pnl_ControlVotePane = grp_ControlVote.getContentPane();
		
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
		btngrp_Vote.add(ptn_VoteUnchanged);

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

		
		chck_SetVote.addItemListener(optionsUpdater);
		ptn_RevokeVote.addActionListener(optionsUpdater);
		ptn_SetVote.addActionListener(optionsUpdater);
		ptn_VoteUnchanged.addActionListener(optionsUpdater);
		ftxt_Vote.addActionListener(optionsUpdater);

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

	
	private class OptionsUpdater implements ListSelectionListener, ChangeListener, ItemListener, ActionListener {
		private final Object syncRoot = new Object();
		private boolean isBusy;

		private static final int UPDATE_ML = 0;
		private static final int SET_WATCHED_STATE = 1;
		private static final int SET_WATCHED_STATE_WATCHED = 2;
		private static final int UPDATE_ML_STATE = 3;
		private static final int FILE_LOCATION = 10;
		private static final int RENAMEMOVE_FILE = 4;
		private static final int SET_EP_VOTE_STATE = 6;
		private static final int SET_EP_VOTE_STATE_VOTE = 7;
		private static final int EP_VOTE = 9;
		private static final int SET_TEXT_OTHER = 11;
		private static final int SET_TEXT_STORAGE = 12;
		private static final int SET_TEXT_SOURCE = 13;


		public void valueChanged(ListSelectionEvent e) {
			if(e.getValueIsAdjusting()) return;



			FileInfo fi, fiFirst = null;

			int[] selectedRows = tbl_Files.getSelectedRows();
			
			boolean updateUI = false;
			for(int index : selectedRows) {
				fiFirst =  processingExt.index2FileInfo(index);
				if(!fiFirst.isLocked()) { updateUI = true; break; }
			}
			if(!updateUI || fiFirst == null) return;
			
			synchronized(syncRoot) {
				if(isBusy) return;
				isBusy = true;
			}

			boolean[] diff = new boolean[14];		
			for(int index : selectedRows) {
				fi = processingExt.index2FileInfo(index);

				if(fi.isLocked()) continue;

				if(!diff[UPDATE_ML] && fi.getMlCmd().equals(fiFirst.getMlCmd())) {
					FileInfo.MLOptions mlO1 = fiFirst.getMlOptions(), mlO2 = fi.getMlOptions();
					if(!diff[UPDATE_ML_STATE] && (mlO1.getState() == FileInfo.MLState.Set && mlO2.getState() == FileInfo.MLState.Set)) {
						
						if(!diff[SET_WATCHED_STATE] && (mlO1.getWatchedState().equals(mlO2.getWatchedState()) ||
						                               (mlO1.getWatchedState() == FileInfo.MLState.Unset && mlO2.getWatchedState() == FileInfo.MLState.Set) ||
													   (mlO1.getWatchedState() == FileInfo.MLState.Set && mlO2.getWatchedState() == FileInfo.MLState.Unset))) {
							if(!diff[SET_WATCHED_STATE_WATCHED] && !mlO1.getWatchedState().equals(mlO2.getWatchedState())) diff[SET_WATCHED_STATE_WATCHED] = true ;
						} else {
							diff[SET_WATCHED_STATE] = true;
						}

						if(!diff[FILE_LOCATION] && !mlO1.getStorageState().equals(mlO2.getStorageState())) diff[FILE_LOCATION] = true;

						if(!diff[SET_TEXT_STORAGE] && !mlO1.getStorage().equals( mlO2.getStorage())) diff[SET_TEXT_STORAGE] = true;
						if(!diff[SET_TEXT_OTHER] && !mlO1.getOther().equals(mlO2.getOther())) diff[SET_TEXT_OTHER] = true;
						if(!diff[SET_TEXT_SOURCE] && !mlO1.getSource().equals(mlO2.getSource())) diff[SET_TEXT_SOURCE] = true;
					} else if(!diff[UPDATE_ML_STATE] && (mlO1.getState() == FileInfo.MLState.Unset && mlO2.getState() == FileInfo.MLState.Unset)) {
					} else if(!diff[UPDATE_ML_STATE] && (mlO1.getState() == FileInfo.MLState.None && mlO2.getState() == FileInfo.MLState.None)) {
					} else {
						diff[UPDATE_ML_STATE] = true;						
					}
				} else {
					diff[UPDATE_ML] = true;
				}

				if(!diff[RENAMEMOVE_FILE] && !fiFirst.getMove().equals(fi.getMove())) diff[RENAMEMOVE_FILE] = true;

				if(!diff[SET_EP_VOTE_STATE] && fi.getVoteCmd().equals(fiFirst.getVoteCmd())) {
					//TODO
					if(!diff[SET_EP_VOTE_STATE_VOTE] && !(fiFirst.getVote() == 0 ^ fi.getVote() == 0)) {
						if(!diff[EP_VOTE] && fiFirst.getVote() != fi.getVote()) diff[EP_VOTE] = true;
					} else {
						diff[SET_EP_VOTE_STATE_VOTE] = true;
					}
				} else {
					diff[SET_EP_VOTE_STATE] = true;
				}
			}

			if(!diff[UPDATE_ML] && fiFirst.getMlCmd().equals(FileInfo.Action.Todo)) {
				FileInfo.MLOptions mlO = fiFirst.getMlOptions();
				chck_UpdateMyList.setState(TriStateCheckBox.SELECTED);

				if(!diff[UPDATE_ML_STATE]) {

					if(mlO.getState().equals(FileInfo.MLState.Set)) {
						ptn_MLAdd.setSelected(true);

						if(!diff[FILE_LOCATION]) {
							cmb_FileState.setSelectedItem(mlO.getStorageState());
						} else {
							cmb_FileState.setEditable(true);
							cmb_FileState.setSelectedItem("[MIXED]");
							cmb_FileState.setEditable(false);
						}

					} else {
						ptn_MLRemove.setSelected(true);
					}


					
					if(!diff[SET_WATCHED_STATE]) {
						chck_SetWatchedState.setState(!mlO.getWatchedState().equals(FileInfo.MLState.None) ?  TriStateCheckBox.SELECTED : TriStateCheckBox.NOT_SELECTED);

						if(!diff[SET_WATCHED_STATE_WATCHED]) {
							if(!mlO.getWatchedState().equals(FileInfo.MLState.None)) {
								(mlO.getWatchedState().equals(FileInfo.MLState.Set) ? ptn_SetWatched : ptn_SetUnwatched).setSelected(true);
							}
						} else {
							ptn_WatchedUnchanged.setSelected(true);
						}

						//TODO
					} else {
						chck_SetWatchedState.setState(TriStateCheckBox.DONT_CARE);
					}
				} else {
					ptn_MLUnchanged.setSelected(true);
				}

			} else {
				chck_UpdateMyList.setState(diff[UPDATE_ML] ? TriStateCheckBox.DONT_CARE : TriStateCheckBox.NOT_SELECTED);
			}

			if(!diff[SET_EP_VOTE_STATE]) {
				chck_SetVote.setState(fiFirst.getVoteCmd().equals(FileInfo.Action.Todo) ? TriStateCheckBox.SELECTED : TriStateCheckBox.NOT_SELECTED);

				if(!diff[SET_EP_VOTE_STATE_VOTE]) {
					(fiFirst.getVote() != 0 ? ptn_SetVote : ptn_RevokeVote).setSelected(true);

					if(!diff[EP_VOTE] && fiFirst.getVote() != 0) {
						ftxt_Vote.setValue(fiFirst.getVote());
					} else {
						ftxt_Vote.setValue(-1);
					}

				} else {
					ptn_VoteUnchanged.setSelected(true);
				}

			} else {
				chck_SetVote.setState(TriStateCheckBox.DONT_CARE);
			}

			if(!diff[RENAMEMOVE_FILE]){
				chck_RenameMoveFile.setState(fiFirst.getMove() == FileInfo.Action.Todo ? TriStateCheckBox.SELECTED : TriStateCheckBox.NOT_SELECTED);
			} else {
				chck_RenameMoveFile.setState(TriStateCheckBox.DONT_CARE);
			}

			synchronized(syncRoot) { isBusy = false; }
		}

		private void optionsChanged(Object source) {
			synchronized(syncRoot) {
				if(isBusy) return;
				isBusy = true;
			}

			FileInfo fi;
			for(int index : tbl_Files.getSelectedRows()) {
				fi =  processingExt.index2FileInfo(index);

				if(fi.isLocked()) continue;

				if(source == cmb_FileState) {
					fi.getMlOptions().setStorageState((FileInfo.MLStorageState)cmb_FileState.getSelectedItem());
				} else if(source == chck_UpdateMyList) {
					fi.setMlCmd(chck_UpdateMyList.getState().equals(TriStateCheckBox.SELECTED) ? FileInfo.Action.Todo : FileInfo.Action.None);
				} else if(source == ptn_MLAdd || source == ptn_MLRemove) {
					fi.getMlOptions().setState(ptn_MLAdd.isSelected() ? FileInfo.MLState.Set : FileInfo.MLState.Unset);

				} else if(source == chck_SetWatchedState || source == ptn_SetWatched || source == ptn_SetUnwatched) {
					fi.getMlOptions().setWatchedState(chck_SetWatchedState.getState().equals(TriStateCheckBox.SELECTED) ? (ptn_SetUnwatched.isSelected() ? FileInfo.MLState.Unset : FileInfo.MLState.Set) : FileInfo.MLState.None);

				} else if(source == chck_RenameMoveFile) {
					fi.setMove(chck_RenameMoveFile.getState().equals(TriStateCheckBox.SELECTED) ? FileInfo.Action.Todo : FileInfo.Action.None);


				} else if(source == chck_SetVote) {
					fi.setVoteCmd(chck_SetVote.getState().equals(TriStateCheckBox.SELECTED) ? FileInfo.Action.Todo : FileInfo.Action.None);

				} else if(source == ptn_RevokeVote) {
					fi.setVote(0);
				} else if(source == ptn_SetVote || source == ftxt_Vote) {
					fi.setVote((Integer)ftxt_Vote.getValue());
				}
			}

			((DefaultTableModel)tbl_Files.getModel()).fireTableRowsUpdated(tbl_Files.getSelectedRows()[0], tbl_Files.getSelectedRows()[tbl_Files.getSelectedRowCount()-1]);

			synchronized(syncRoot) { isBusy = false; }

		}

		public void stateChanged(ChangeEvent e) {
			optionsChanged(e.getSource());
		}

		public void itemStateChanged(ItemEvent e) {
			optionsChanged(e.getSource());
		}


		public void actionPerformed(ActionEvent e) {
			optionsChanged(e.getSource());
			
		}
	}
}
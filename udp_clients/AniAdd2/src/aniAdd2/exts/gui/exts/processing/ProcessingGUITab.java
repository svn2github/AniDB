/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.exts.processing;

import aniAdd2.IAniAdd2;
import aniAdd2.exts.gui.components.TriStateCheckBox;
import aniAdd2.exts.gui.exts.processing.icons.IconsCell;
import aniAdd2.exts.processing.AA2Processing;
import aniAdd2.exts.processing.FileInfo;
import java.awt.Component;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Insets;
import javax.swing.BorderFactory;
import javax.swing.GroupLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JRadioButton;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.SwingConstants;
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

		setOpaque(false);
		initComponents();
	}

	private void initComponents(){
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
                    .addComponent(scr_tbl_Files, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_Status, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
                .addGroup(layout.createParallelGroup()
                    .addComponent(pnl_Buttons, 150, 150, 150)
                    .addComponent(pnl_Control, 150, 150, 150)))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup()
                    .addComponent(scr_tbl_Files, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
                    .addComponent(pnl_Control, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
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

        GroupLayout layout = new GroupLayout(pnl_Control);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addComponent(pnl_ControlSettings, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(pnl_ControlSettings))
        );
		pnl_Control.setLayout(layout);

		//pnl_Control.add(createControlSettingsPanel());

		return pnl_Control;
	}	
	private JPanel createControlSettingsPanel(){
 		JPanel pnl_ControlSettings = new JPanel(new GridLayout(0, 1));
		pnl_ControlSettings.setOpaque(false);

        JComboBox cmb_FileState = new JComboBox(new String[] { "Unkown", "Internal", "External", "Deleted" });
        TriStateCheckBox chck_AddFileToMyList = new TriStateCheckBox("Add File to MyList");
        TriStateCheckBox chck_SetWatchedState = new TriStateCheckBox("Watched State");
        JRadioButton ptn_MarkWatched = new JRadioButton("Set");
        JRadioButton ptn_MarkUnwatched = new JRadioButton("Unset");
        TriStateCheckBox chck_RenameFile = new TriStateCheckBox("Rename");
        TriStateCheckBox chck_MoveFile = new TriStateCheckBox("Move");

		cmb_FileState.setOpaque(false);
		chck_AddFileToMyList.setOpaque(false);
		chck_SetWatchedState.setOpaque(false);
		ptn_MarkWatched.setOpaque(false);
		ptn_MarkUnwatched.setOpaque(false);
		chck_RenameFile.setOpaque(false);
		chck_MoveFile.setOpaque(false);
   
        chck_SetWatchedState.setMargin(new java.awt.Insets(2, 2, 0, 2));
        ptn_MarkWatched.setMargin(new java.awt.Insets(0, 2, 2, 2));
        ptn_MarkUnwatched.setMargin(new java.awt.Insets(0, 2, 2, 2));

        GroupLayout layout = new GroupLayout(pnl_ControlSettings);
        layout.setHorizontalGroup(
            layout.createParallelGroup()
            .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addComponent(chck_AddFileToMyList, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE)
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_SetWatchedState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addGap(20)
                .addComponent(ptn_MarkWatched)
                .addComponent(ptn_MarkUnwatched, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
            .addGroup(layout.createSequentialGroup()
                .addComponent(chck_RenameFile)
                .addComponent(chck_MoveFile, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup()
            .addGroup(layout.createSequentialGroup()
                .addComponent(cmb_FileState, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE, GroupLayout.PREFERRED_SIZE)
                .addComponent(chck_AddFileToMyList)
                .addComponent(chck_SetWatchedState)
                .addGroup(layout.createParallelGroup()
                    .addComponent(ptn_MarkWatched)
                    .addComponent(ptn_MarkUnwatched))
				.addGap(4)
                .addGroup(layout.createParallelGroup()
                    .addComponent(chck_RenameFile)
                    .addComponent(chck_MoveFile)))
        );

        pnl_ControlSettings.setLayout(layout);
		pnl_ControlSettings.setOpaque(false);
        pnl_ControlSettings.setBorder(BorderFactory.createTitledBorder("Settings"));

		return pnl_ControlSettings;
	}
}
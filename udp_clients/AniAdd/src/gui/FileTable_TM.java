/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package gui;

import java.awt.Image;
import javax.swing.table.DefaultTableModel;
import processing.Mod_EpProcessing;

/**
 *
 * @author Arokh
 */
public class FileTable_TM extends DefaultTableModel {
	Mod_EpProcessing epProc;

	public FileTable_TM(Mod_EpProcessing epProc) {
		super();

		this.epProc = epProc;
		/*epProc.AddComListener(new ComListener() {
		public void EventHandler(ComEvent comEvent) {
		if(comEvent.Type() == ComEvent.eType.Information && (Mod_EpProcessing.eComType)comEvent.Params(0) == Mod_EpProcessing.eComType.FileEvent){
		int index = (Integer)comEvent.Params(2);
		fireTableRowsUpdated(index, index);
		}
		}
		});*/
	}

	public int getRowCount() {
		return epProc != null ? epProc.FileCount() : 0;
	}

	public int getColumnCount() {
		return 2;
	}

	public String getColumnName(int columnIndex) {
		return columnIndex == 0 ? "Filename" : "Progress";
	}

	public Class<?> getColumnClass(int columnIndex) {
		return columnIndex == 0 ? String[].class : Image[].class;
	}

	public Object getValueAt(int rowIndex, int columnIndex) {
		return epProc.index2FileInfo(rowIndex);
	}
}

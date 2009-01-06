package ui;

import javax.swing.table.DefaultTableCellRenderer;

import java.awt.Color;

public class ProcessingCellRenderer extends DefaultTableCellRenderer {
	private static final long serialVersionUID = 5994733505686032501L;

	public ProcessingCellRenderer() {
		super();
		setHorizontalAlignment(javax.swing.SwingConstants.CENTER);
	}
	
	public void setValue(Object value) {
		if ((value != null) && (value instanceof String)) {
			String processingValue = (String) value;
			if (processingValue.equals("processing")) super.setForeground(Color.cyan);
			else if (processingValue.equals("hashed")) super.setForeground(Color.magenta);
			else if (processingValue.equals("parsed")) super.setForeground(Color.blue);
			else super.setForeground(Color.black);
		} 
		super.setValue(value);
	} 
}

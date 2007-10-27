/*
 * Created on 30. juni. 2007 00.05.11
 * Filename: JComboBoxLF.java
 */
package epox.swing;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.DefaultComboBoxModel;
import javax.swing.JComboBox;
import javax.swing.SwingUtilities;
import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;

public class JComboBoxLF extends JComboBox{
	protected final static LookAndFeelInfo lf[] = UIManager.getInstalledLookAndFeels();
	public JComboBoxLF(final Component c){
		super(new DefaultComboBoxModel(){
			public Object getElementAt(int index) {
				return lf[index].getName();
			}
			public int getSize() {
				return lf.length;
			}
		});
		addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e) {
				try {
					UIManager.setLookAndFeel(lf[getSelectedIndex()].getClassName());
				} catch (Exception x) {
					x.printStackTrace();
				}
				SwingUtilities.updateComponentTreeUI(c);
			}
		});
		String s = UIManager.getLookAndFeel().getClass().getCanonicalName();
		int i;
		for(i=0; i<lf.length; i++)
			if(s.equals(lf[i].getClassName()))
				break;
		setSelectedIndex(i);
		setToolTipText("Select wanted look and feel here.");
	}
}

/*
 * Created on 29. juni. 2007 23.40.57
 * Filename: MyUtils.java
 */
package epox.swing;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Rectangle;

import javax.swing.UIManager;
import javax.swing.plaf.FontUIResource;

public class MySwing {
	public static void centerComponent(Component c){
		Dimension d = java.awt.Toolkit.getDefaultToolkit().getScreenSize();
		Rectangle r = c.getBounds();
		c.setBounds(d.width/2-r.width/2, d.height/2-r.height/2, r.width, r.height);
	}
	public static void setFont(Font f0, Font f1){
		FontUIResource f = new FontUIResource(f0);
		FontUIResource f2 = new FontUIResource(f1);
		UIManager.put("Button.font", f);
		UIManager.put("CheckBox.font", f);
		UIManager.put("CheckBoxMenuItem.acceleratorFont", f);
		UIManager.put("CheckBoxMenuItem.font", f);
		//UIManager.put("ColorChooser.font", f);
		UIManager.put("ComboBox.font", f);
		UIManager.put("DesktopIcon.font", f);
		UIManager.put("EditorPane.font", f);
		UIManager.put("FormattedTextField.font", f);
		//UIManager.put("InternalFrame.titleFont", f);
		UIManager.put("Label.font", f);
		UIManager.put("List.font", f);
		//UIManager.put("Menu.acceleratorFont", f);
		UIManager.put("Menu.font", f);
		//UIManager.put("MenuBar.font", f);
		UIManager.put("MenuItem.acceleratorFont", f);
		UIManager.put("MenuItem.font", f);
		UIManager.put("OptionPane.font", f);
		UIManager.put("Panel.font", f);
		UIManager.put("PasswordField.font", f);
		UIManager.put("PopupMenu.font", f);
		UIManager.put("ProgressBar.font", f);
		UIManager.put("RadioButton.font", f);
		UIManager.put("RadioButtonMenuItem.acceleratorFont", f);
		UIManager.put("RadioButtonMenuItem.font", f);
		UIManager.put("ScrollPane.font", f);
		UIManager.put("Slider.font", f);
		//UIManager.put("Spinner.font", f);
		UIManager.put("TabbedPane.font", f);
		UIManager.put("Table.font", f2);
		UIManager.put("TableHeader.font", f);
		UIManager.put("TextArea.font", f);
		UIManager.put("TextField.font", f);
		UIManager.put("TextPane.font", f);
		UIManager.put("TitledBorder.font", f);
		UIManager.put("ToggleButton.font", f);
		//UIManager.put("ToolBar.font", f);
		UIManager.put("ToolTip.font", f);
		UIManager.put("Tree.font", f);
		UIManager.put("Viewport.font", f);
	}
}

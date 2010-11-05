/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui;

import aniAdd2.exts.gui.exts.IAA2GUIExtension;
import java.awt.BorderLayout;
import java.awt.Component;
import javax.swing.*;

/**
 *
 * @author Arokh
 */
public class MainUserInterface extends JPanel {
	private JPanel notifiesPanel;
	private JTabbedPane tabControl;
	private JTabbedPane optionTab;
	private JTabbedPane helpTab;
	private NotifyManager notifyManager;

	public MainUserInterface() {
		initComponents();

		notifyManager = new NotifyManager(notifiesPanel);
	}

	private void initComponents() {
		setLayout(new BorderLayout());
		setOpaque(false);

		notifiesPanel = new JPanel();
		BoxLayout boxLayout = new BoxLayout(notifiesPanel, BoxLayout.Y_AXIS);
		notifiesPanel.setLayout(boxLayout);
		add(notifiesPanel, BorderLayout.NORTH);

		JPanel optionsTabGeneral = new JPanel();
		optionsTabGeneral.setOpaque(false);

		optionTab = new JTabbedPane(JTabbedPane.LEFT);
		optionTab.addTab("General", optionsTabGeneral);


		JPanel helpTabGeneral = new JPanel();
		helpTabGeneral.setOpaque(false);

		helpTab = new JTabbedPane(JTabbedPane.LEFT);
		helpTab.addTab("General", helpTabGeneral);

		tabControl = new JTabbedPane();
		tabControl.setOpaque(false);
		tabControl.addTab("Options", optionTab);
		tabControl.addTab("Help", helpTab);

		add(tabControl, BorderLayout.CENTER);
	}

	public void addTab(IAA2GUIExtension.Location location, String title, Component component){ //TODO remove IAA2GUIExtension.Location dependency?
		JTabbedPane tabCtrl = location == IAA2GUIExtension.Location.MainTabControl ? tabControl : ( location == IAA2GUIExtension.Location.Options ? optionTab : helpTab);

		tabCtrl.insertTab(title, null, component, null, 0);
		tabCtrl.setSelectedIndex(0);
	}

	public NotifyManager getNotifyManager() {
		return notifyManager;
	}
}

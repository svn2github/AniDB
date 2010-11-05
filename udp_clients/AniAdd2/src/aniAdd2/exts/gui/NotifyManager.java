/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui;

import aniAdd2.exts.gui.notify.Notify;
import aniAdd2.exts.gui.notify.Notify.NotifyType;
import aniAdd2.exts.gui.notify.NotifyEvent;
import aniAdd2.exts.gui.notify.NotifyEventListener;
import com.sun.net.httpserver.Authenticator.Success;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.Error;
import java.util.ArrayList;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author Arokh
 */
public class NotifyManager {
	private static int lastId = 0;
	private JPanel notifiesPanel;
	private ArrayList<NotifyPair> notifies;

	public NotifyManager(JPanel notifiesPanel) {
		this.notifiesPanel = notifiesPanel;
		notifies = new ArrayList<NotifyPair>();
	}

	private void closeNotify(Notify notify) {
		NotifyPair notifyPair = null;
		for(int i = 0; i < notifies.size(); i++) if(notify == notifies.get(i).notify) notifyPair = notifies.get(i);
		if(notifyPair == null) {
			System.err.println("Cannot close Notify: Notify not found");
			return;
		}

		final NotifyPanel panel = notifyPair.panel;
		SwingUtil.invokeAndWait(new Runnable() {
			public void run() {
				notifiesPanel.remove(panel);
				notifiesPanel.revalidate(); //TODO?
			}
		}, "Couldn't remove Notify from gui.");

		notifies.remove(notifyPair);
	}

	public Notify add(String message, Notify.NotifyType type) {
		Notify notify = new Notify(message, type, lastId++);
		notify.addNotifyEventListener(new NotifyEventListener() {
			@Override
			public void closed(NotifyEvent e) {
				closeNotify(e.getNotify());
			}
		});

		NotifyPanel panel = new NotifyPanel(notify);
		notifies.add(new NotifyPair(notify, panel));

		notifiesPanel.add(panel);
		notifiesPanel.revalidate(); //TODO?

		return notify;
	}

	private static class NotifyPair {
		public Notify notify;
		public NotifyPanel panel;

		public NotifyPair(Notify notify, NotifyPanel panel) {
			this.notify = notify;
			this.panel = panel;
		}
	}

	private static class NotifyPanel extends JPanel {
		private JLabel messageLabel;

		public NotifyPanel(final Notify notify) {
			notify.addNotifyEventListener(new NotifyEventListener() {
				@Override
				public void messageChanged(final NotifyEvent e) {
					SwingUtil.invokeAndWait(new Runnable() {
						public void run() {
							messageLabel.setText(e.getNotify().getMessage());
						}
					}, "Couldn't change Notifymessage");
				}

				@Override
				public void typeChanged(final NotifyEvent e) {
					SwingUtil.invokeAndWait(new Runnable() {
						public void run() {
							setBackgroundColor(e.getNotify().getType());
						}
					}, "Couldn't change color for NotifyType change");
				}
			});


			setLayout(new BorderLayout()); //TODO?

			setBackgroundColor(notify.getType());

			messageLabel = new JLabel(notify.getMessage());
			add(messageLabel, BorderLayout.CENTER);

			JButton closeButton = new JButton("X");
			closeButton.setMargin(new Insets(0, 0, 0, 0));
			closeButton.addActionListener(new ActionListener() {
				public void actionPerformed(ActionEvent e) {
					notify.closeNotify();
				}
			});
			add(closeButton, BorderLayout.EAST);
		}

		private void setBackgroundColor(NotifyType type) {
			switch(type) {
				case Error:
					setBackground(new Color(255, 128, 128));
					break;
				case Information:
					setBackground(new Color(64, 128, 255));
					break;
				case Warning:
					setBackground(new Color(255, 255, 128));
					break;
				case Success:
					setBackground(new Color(128, 255, 128));
					break;
			}
		}
	}
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2;

import aniAdd2.exts.gui.AA2GUI;
import aniAdd2.exts.gui.notify.Notify;
import aniAdd2.exts.gui.notify.Notify.NotifyType;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.WindowConstants;

/**
 *
 * @author Arokh
 */
public class Main {
	private static AniAdd2 aniAdd2;

	/**
	 * @param args the command line arguments
	 */
	public static void main(String[] args) {
		aniAdd2 = new AniAdd2();
		aniAdd2.initialize();

		webStart();
	}

	private static void webStart() {

		JFrame frame = new JFrame("AniAdd2");
		frame.setVisible(true);
		frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

		AA2GUI gui = (AA2GUI)aniAdd2.getExtension(AA2GUI.class);

		if(gui == null) {
			frame.getContentPane().add(new JLabel("Couldn't find GUI extension", null, SwingConstants.CENTER));
			frame.setSize(400, 200);
			return;
		}

		frame.getContentPane().add(gui.getComponent());
		frame.setSize(1024, 768);

		notifyTest(gui);
	}

	private static void notifyTest(AA2GUI gui) {
		Notify notifySuccess = gui.addNotify("Success", NotifyType.Success);
		gui.addNotify("Information", NotifyType.Information);
		gui.addNotify("Warning", NotifyType.Warning);
		Notify notifyError = gui.addNotify("Error", NotifyType.Error);

		int i = 5;
		while(i != 0) {
			notifySuccess.setMessage("Success. Closing in " + i + " seconds");
			notifyError.setMessage("Error. Solving error in " + i-- + " seconds");
			ThreadUtil.Sleep(1000);
		}
		notifySuccess.closeNotify();
		notifyError.setMessage("Success");
		notifyError.setType(NotifyType.Success);
	}
}

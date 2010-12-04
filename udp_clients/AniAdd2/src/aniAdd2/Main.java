/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2;

import aniAdd2.exts.gui.AA2GUI;
import aniAdd2.exts.gui.notify.Notify;
import aniAdd2.exts.gui.notify.Notify.NotifyType;
import aniAdd2.exts.udpApi.AA2AniDBUdpClient;
import aniDB.udpApi.client.accountManagement.UserAccount;
import aniDB.udpApi.client.accountManagement.UserAccount.Option;
import java.awt.Dimension;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.EnumSet;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
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
	public static void main(String[] args) throws Exception {
		try {
			UIManager.setLookAndFeel(UIManager.getCrossPlatformLookAndFeelClassName());
			UIManager.put("swing.boldMetal", Boolean.FALSE);
		} catch (Exception e) {
		   // handle exception
		}
		
		aniAdd2 = new AniAdd2();
		aniAdd2.initialize();


		webStart();
	}

	private static void webStart() throws Exception {

		JFrame frame = new JFrame("AniAdd2");
		frame.setVisible(true);
		frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

		AA2GUI gui = (AA2GUI)aniAdd2.getExtension(AA2GUI.class);

		if(gui == null) {
			frame.getContentPane().add(new JLabel("Couldn't find GUI extension", null, SwingConstants.CENTER));
			frame.setSize(new Dimension(400, 200));
			return;
		}

		frame.getContentPane().add(gui.getComponent());
		frame.setSize(new Dimension(800, 600));


		File file = new File(Main.class.getResource("AccInfo").toURI());
		if(file.exists() && false) {
			AA2AniDBUdpClient client = (AA2AniDBUdpClient)aniAdd2.getExtension(AA2AniDBUdpClient.class);
			BufferedReader br = new BufferedReader(new FileReader(file));
			client.getAccountManagement().add(new UserAccount(br.readLine(), br.readLine(), EnumSet.of(Option.KeepAlive, Option.Compression), true, null));
		}
		
		//notifyTest(gui);
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

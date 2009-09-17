package aniAdd.startup;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
import aniAdd.Modules.IModule;
import aniAdd.*;
import aniAdd.Communication.ComEvent;
import gui.GUI;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.UIManager;
import udpApi.Mod_UdpApi;

/**
 *
 * @author Arokh
 */
public class Main {

    static String username, session, password, autopass;
    static JFrame frm = new JFrame();
    static AniAdd aniAdd;

    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception ex) {
        }

        aniAdd = new AniAdd();

        frm.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        frm.addWindowListener(new WindowAdapter() {

            @Override
            public void windowClosing(WindowEvent e) {
                super.windowClosing(e);
                aniAdd.Stop();
            }
        });

        aniAdd.AddComListener(new Communication.ComListener() {

            public void EventHandler(ComEvent comEvent) {
                if (comEvent.Type() == ComEvent.eType.Information) {
                    if ((IModule.eModState) comEvent.Params(0) == IModule.eModState.Initialized) {
                        Initialize();
                    }
                }
            }
        });

        aniAdd.Start();
    }

    private static void Initialize() {
        GUI gui = (GUI) aniAdd.GetModule("MainGUI");
        Mod_UdpApi api = (Mod_UdpApi) aniAdd.GetModule("UdpApi");

        username = JOptionPane.showInputDialog(frm, "User", "");//"dvdkhl");
        password = JOptionPane.showInputDialog(frm, "Password", "");//"Perpetuity");
        api.setPassword(password);
        api.setSession(session);
        api.setUsername(username);

        if (api.authenticate()) {
        } else {
        }

        frm.setDefaultLookAndFeelDecorated(true);

        frm.add(gui);
        frm.pack();
        frm.setVisible(true);
    }
}

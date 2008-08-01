/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package AniAdd;

import Processing.EpProcessing;
import Processing.cUser.eLanguage;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.File;
import java.security.AllPermission;
import java.util.prefs.Preferences;
import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import java.net.CookieHandler;
import java.net.URI;
import java.util.*;

public class AniAdd extends JApplet {
    public UI.UInterface GUI;
    public UDPApi.Api API;
    public EpProcessing EpProc;

    public static boolean runsLocally=false;

    public void displayError(String msg){
        JLabel l=new JLabel(msg);
        add(l);
        //This function can only be called once. it assumes the real GUI is never added
    }


    @Override
    public void init() {
        try {
            if(!runsLocally){
                AllPermission perm = new AllPermission();
                java.security.AccessController.checkPermission(perm);
            }
        } catch (Exception exception) {
            displayError("Failed to init: "+exception.getMessage());
            System.out.println("Failed to init: "+exception.getMessage());
            exception.printStackTrace();
            return;
        }

        API = new UDPApi.Api(this);
        GUI = new UI.UInterface(this);
        EpProc = new EpProcessing(this);
        System.out.println("Initialized");
    }


    @Override
    public void start() {
       // File TestFile = new File("E:\\Media\\Anime\\!UnSeen\\Bamboo Blade[Huzzah-Doremi] 12 - Azuma's Circumstances And Mei's Circumstances.Mkv");
      //  EpProc.AddFiles(new File[] {TestFile});
        //EpProc.StartProcessing();

        if (API == null) {
            System.out.println("no API");
            return;
        }

        java.lang.System.out.println("Authenticate");
        API.UserInfo.ApiPassword="";
	API.UserInfo.UserName = getParameter("user");
	API.UserInfo.Session = getParameter("sess");
	API.UserInfo.Autopass = getParameter("autopass");
	if (API.UserInfo.UserName == null || API.UserInfo.Session == null) {
	    try {
		Hashtable<String,String> cookies = new Hashtable<String,String>();

		Map<String,List<String>> headers = CookieHandler.getDefault().get(new URI("http://anidb.net/"), new HashMap<String,List<String>>());
		List<String> cookie_lists = headers.get("Cookie");

		// This isn't a proper parser
		for (String cookie_list : cookie_lists)
		    for (String cookie : cookie_list.split(";")) {
			String attr = cookie.substring(0, cookie.indexOf('=')).trim();
			String value = cookie.substring(cookie.indexOf('=') + 1).trim();
			cookies.put(attr, value);
		    }

		API.UserInfo.UserName = cookies.get("adbsessuser");
		API.UserInfo.Session = cookies.get("adbsess");
		if (API.UserInfo.UserName == null)
			API.UserInfo.UserName = cookies.get("adbautouser");
		API.UserInfo.Autopass = cookies.get("autopass");
		if (API.UserInfo.UserName == null || (API.UserInfo.Session == null && API.UserInfo.Autopass == null))
		    throw new NullPointerException();
	    } catch(Exception e) {
		API.UserInfo.UserName = JOptionPane.showInputDialog(this, "User");
		API.UserInfo.Password = JOptionPane.showInputDialog(this, "Password");
	    }
	}
        if (API.UserInfo.UserName != null)
            try {
                API.Connect();
            } catch (Exception exception) {
                displayError("Connect failed: "+exception.getMessage());
                return;
            }

        add(GUI);
        loadConfig();
        System.out.println("Started");
    }

    @Override
    public void stop(){
       saveConfig();
       System.out.println("Stopped");
    }

    @Override
    public void destroy(){
        API.Finalize();
        System.out.println("Destroyed");
    }

    public static void main(String[] arg){
        runsLocally=true;
        JFrame f=new JFrame("AniAdd");
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        final JApplet a = new AniAdd();
        f.add(a);
        a.init();
        f.setSize(800,300);
        a.start();
        f.setVisible(true);
        f.addWindowListener(new WindowListener() {
            public void windowActivated(WindowEvent windowEvent) {
            }
            public void windowClosed(WindowEvent windowEvent) {

            }
            public void windowClosing(WindowEvent windowEvent) {
                a.stop();
            }
            public void windowDeactivated(WindowEvent windowEvent) {
            }
            public void windowDeiconified(WindowEvent windowEvent) {
            }
            public void windowIconified(WindowEvent windowEvent) {
            }
            public void windowOpened(WindowEvent windowEvent) {
            }
        });
    }


    public void loadConfig(){
        if(GUI!=null){
            Preferences prefs = Preferences.userNodeForPackage(AniAdd.class);

            try {
                String filesWatched=prefs.get("filesWatched", null);
                GUI.checkFilesWatched.setSelected(Boolean.parseBoolean(filesWatched));

                String renameFiles=prefs.get("renameFiles", null);
                GUI.checkRenameFiles.setSelected(Boolean.parseBoolean(renameFiles));

                String renameType = prefs.get("renameType", null);
                EpProc.UserSettings.Type = EpProc.UserSettings.Type.valueOf(renameType);

                String lastDirectory=prefs.get("lastDirectory",null);
                if(lastDirectory!=null) //In case dialog was never opened. This can happen
                    GUI.lastDirectory=new File(lastDirectory);

                String Episode, Series;
                for (int I=0; I<3; I++ ){
                    Episode = prefs.get("episodeLang"+I, renameFiles);
                    Series = prefs.get("seriesLang"+I, renameFiles);
                    EpProc.UserSettings.EpisodeLang[I] = eLanguage.valueOf(Episode);
                    EpProc.UserSettings.SeriesLang[I] = eLanguage.valueOf(Series);
                }
            } catch (Exception exception) {
                System.out.print("Loading settings failed");
                exception.printStackTrace();
            }

        }
    }

    public void saveConfig(){
        System.out.println("Saving config");
        Preferences prefs = Preferences.userNodeForPackage(AniAdd.class);
        prefs.put("filesWatched", ""+GUI.checkFilesWatched.isSelected());
        prefs.put("renameFiles", ""+GUI.checkRenameFiles.isSelected());
        prefs.put("renameType", EpProc.UserSettings.Type.toString());
        if(GUI.lastDirectory!=null)
            prefs.put("lastDirectory", GUI.lastDirectory.toString());

        for (int I=0; I<3; I++ ){
            prefs.put("seriesLang"+I, EpProc.UserSettings.SeriesLang[I].toString());
            prefs.put("episodeLang"+I, EpProc.UserSettings.EpisodeLang[I].toString());
        }
    }



}

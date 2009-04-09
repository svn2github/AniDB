package options;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * Remote Package Options
 * <p>Saves and loads remote settings</p> 
 * @author fahrenheit
 * @version 1.0 - Initial Release
 * @since 08.09.2009 18:55
 */
public class RemoteOptions implements Serializable {
	/** Serial Version */
	private static final long serialVersionUID = 646853252913469182L;
	/** Remote Protocol: Local (loopback) */
	public static final int LOCAL = 0;
	/** Remote Protocol: UDP (will use UDP API) */
	public static final int UDP = 1;
	/** Remote Protocol: TCP (will use TCP API) */
	public static final int TCP = 2;
	// Settings
	/** API Remote Host */
	public String apihost = "api.anidb.net";
	/** API Remote Port */
	public int apiport = 9000;
	/** Local Port */
	public int localport = 0;
	/** NAT Setting */
	public boolean nat = false;
	/** Encryption Setting */
	public boolean encryption = false;
	/** User Name */
	public String username = "";
	/** User Password */
	public String password = "";
	/** API Password */
	public String apipass = "";
	/** Protocol to use */
	public int protocol = UDP;
	/** Hashing enabled */
	public boolean enabled = true;
	/** Serialization filename */
	public String serFilename = "settings_remote.ser";
	/** Buffer size (in bytes) */
	public int bufferSize = 1048576*3;
	/** Debug messages */
	public boolean seeDebug = true;
	
	/**
	 * Default HasherOptions constructor
	 */
	public RemoteOptions() {}
	
	/**
	 * Serializes HasherOptions to disk
	 * @param appDir Application directory where file will be saved to
	 * @return True if success, false otherwise
	 */
	public synchronized boolean saveSettings(String appDir) {
		FileOutputStream fOut=null;
		ObjectOutputStream oOut=null;
		String filename = appDir + File.separator + serFilename;
		boolean isOK = true;
		try{
			fOut= new FileOutputStream(filename);
			oOut = new ObjectOutputStream(fOut);
			oOut.writeObject(this);
			if (seeDebug) System.out.println("Saved remote settings to: "+filename);
			isOK = true;
		} catch(IOException e){
			System.err.println("Error while saving remote settings to: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally{
			try {
				oOut.flush();
				oOut.close();
				fOut.close();
			} catch (IOException e1) {
				System.err.println("Error while saving remote settings to: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
	
	/**
	 * Loads serialized HasherOptions from disk
	 * @param appDir Application directory where file will be loaded from
	 * @return True if success, false otherwise
	 */
	public synchronized boolean loadSettings(String appDir) {
		String filename = appDir + File.separator + serFilename;
		boolean isOK = true;
		FileInputStream fIn=null;
		ObjectInputStream oIn=null;

		try {
			fIn= new FileInputStream(filename);
			oIn = new ObjectInputStream(fIn);
			RemoteOptions options = (RemoteOptions) oIn.readObject();
			this.apihost = options.apihost;
			this.apiport = options.apiport;
			this.localport = options.localport;
			this.protocol = options.protocol;
			this.nat = options.nat;
			this.username = options.username;
			this.password = options.password;
			this.apipass = options.apipass;
			this.encryption = options.encryption;
			this.bufferSize = options.bufferSize;
			this.seeDebug = options.seeDebug;
			this.enabled = options.enabled;
			if (seeDebug) System.out.println("Loaded remote settings from: "+filename);
		} catch(IOException e) {
			System.err.println("Error while loading remote settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} catch(ClassNotFoundException e) {
			System.err.println("Error while loading remote settings from: "+filename);
			System.err.println(e.getLocalizedMessage());
			isOK = false;
		} finally {
			try {
				oIn.close();
				fIn.close();
			} catch (IOException e1) {
				System.err.println("Error while loading remote settings from: "+filename);
				System.err.println(e1.getLocalizedMessage());
				isOK = false;
			}
		}
		return isOK;
	}
}

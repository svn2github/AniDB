package api.udp;

import java.lang.reflect.Method;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Map;
import java.util.TreeMap;

import clients.XaucShared;

import api.API;
import api.ApiCommand;
import utils.Log;

/**
 * This is the main UDP API class.
 * <p>Clients should instantiate this class to handle UDP communication with AniDB.</p>
 * <p>Clients need to register their response handlers with this class before usage.<br>
 * This is done by accessing the addHandler() method and suppling the required fields.<br>
 * Please note that the handlers are for associated tags and not UDP API numeric replies (like: 200,401,etc).</p>
 * <p><b>Error handling:</b><br>This class will handle errors. Custom error handlers can be supplied by accessing the addErrorHandler() method.<br>
 * Please note that for error handlers the association is made by numeric UDP API replies and not by tags.</p> 
 * @author fahrenheit
 */
public class UDPApi implements API {
	/** User Information */
	protected UDPUserInfo userInfo = new UDPUserInfo();
	/** Connection Information */
	protected UDPConnectionInfo connectionInfo = new UDPConnectionInfo();
	/** Log file */
	protected Log log = new Log(); 
	/** Indicates if the client is connected to the API */
	protected boolean isConnected = false;
	/** Indicates if the client is logged on */
	protected boolean isLogged = false;
	/** Indicates if debug messages should be shown */
	protected boolean showDebug = false;
	/** Indicates that the API is finishing stuff up */
	protected boolean isFinalizing = false;
	/** Sender thread */
	Thread Sender = new Thread(new UDPApiSender(this.connectionInfo), "Sender");
	/** System call thread */
//	Thread SysCall = new Thread(new IdleTasks(), "SysCall");
	/** Receiver thread */
//	Thread Receiver = new Thread(new ReceivedData(),"Receiver");
	
	/** Tag based Request Handlers */
	protected Map<String, UDPRequestHandler> tagRequestHandlers = new TreeMap<String, UDPRequestHandler>();
	/** Command ID Request Handlers */
	protected Map<Integer, UDPRequestHandler> errorRequestHandlers = new TreeMap<Integer, UDPRequestHandler>();
	
	// Request Handler methods
	@Override
	public void addHandler(int ID, Method method, Object methodContainer) {
		errorRequestHandlers.put(ID, new UDPRequestHandler(ID,method,methodContainer));
	}
	@Override
	public void addHandler(String tag, Method method, Object methodContainer) {
		tagRequestHandlers.put(tag, new UDPRequestHandler(tag,method,methodContainer));		
	}
	@Override
	public void removeHandler(int ID) { errorRequestHandlers.remove(ID); }
	@Override
	public void removeHandler(String tag) { tagRequestHandlers.remove(tag); }
	
	// Auxiliar methods 
	
	@Override
	public void setLog(Log log) { this.log = log; }
	@Override
	public Log getLog() { return this.log; }
	@Override
	public boolean isShowDebug() { return this.showDebug; }
	@Override
	public void setShowDebug(boolean showDebug) { this.showDebug = showDebug; }

	// Connection methods
	
	@Override
	public void connect() {
		log.printDateTime();
		log.print("[UDPApi] Establishing connection to API server...");
		try {
			connectionInfo.AniDBIP = java.net.InetAddress.getByName(connectionInfo.hostname);
		} catch (UnknownHostException e) {
			String message = "[UDPApi] Could not determine the IP associated with \""+connectionInfo.hostname+"\", terminating connection attempt.";
			log.println("FAIL\n"+message);
			XaucShared.printError(message);
			isConnected = false;
			try {
				super.finalize();
			} catch (Throwable e1) {
				e1.printStackTrace();
			}
			return;
		}
		try {
			connectionInfo.socket = new java.net.DatagramSocket(connectionInfo.localPort);
		} catch (SocketException e) {
			String message = "[UDPApi] Could not create a socket to \""+connectionInfo.hostname+"\", terminating connection attempt.\n"+e.getLocalizedMessage();
			log.println("FAIL\n"+message);
			XaucShared.printError(message);
			isConnected = false;
			try {
				super.finalize();
			} catch (Throwable e1) {
				e1.printStackTrace();
			}
			return;
		}
		log.println("OK");   

		connectionInfo.pingInterval = 1;
		connectionInfo.getInitMessages = true;
		connectionInfo.getPushMessages = false;

		log.printDateTime();
/*
		log.print("[UDPApi] Starting IdleTasks Thread");
		if (SysCall.getState() == Thread.State.NEW) {
			SysCall.start();
		} else if (SysCall.getState() == Thread.State.TERMINATED) {
			SysCall = new Thread(new IdleTasks(), "IdleTasks");
			SysCall.start();            
		}
		log.println("OK");

		log.printDateTime();
		log.print("[UDPApi] Starting Receiver Thread");
		if (Receiver.getState() == Thread.State.NEW) {
			Receiver.start();
		} else if (Receiver.getState() == Thread.State.TERMINATED) {
			Receiver = new Thread(new ReceivedData(), "Receiver");
			Receiver.start();            
		}
		log.println("OK");
*/
		isConnected = true;
	}
	@Override
	public void disconnect() {
		log.printDateTime();
		log.print("[UDPApi] Finnalizing connection to the API server...");
		isFinalizing = true;
		sendCommand(new UDPApiCommand("TERMINATE","terminate", false));
		connectionInfo.socket.close();

		try {
			long WaitForJoin = System.currentTimeMillis();
			Sender.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
//			SysCall.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
//			Receiver.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
		} catch (Exception e) {
			log.println("FAIL\n"+e.getLocalizedMessage());
			try {
				super.finalize();
			} catch (Throwable e1) {
				e1.printStackTrace();
			}
			return;
		}
		log.println("OK");
	}
	@Override
	public void login() {
		for (int i = 0; i < API.MAX_CONNECTION_TRIES; i++ ) {
			if (!isConnected) connect();
			if (isConnected) break;
			else if( i == API.MAX_CONNECTION_TRIES-1) { // failed all connection attempts
				log.println("[UDPApi] Maximum limit of connection attempts reached, quiting.");
				return;
			}
		}	
		UDPApiCommand cmd = new UDPApiCommand("AUTH","auth", false);
		cmd.params.put("user", userInfo.username);
		if (userInfo.session != null)
			cmd.params.put("sess", connectionInfo.session);
		if (userInfo.password != null)
			cmd.params.put("pass", userInfo.password);
		cmd.params.put("protover",Integer.toString(connectionInfo.getProtocolVersion()));
		cmd.params.put("client", connectionInfo.getClientName());
		cmd.params.put("clientver", Integer.toString(connectionInfo.getClientVersion()));
		cmd.params.put("nat", "1");
		cmd.params.put("enc", "UTF8");

		log.println("[UDPApi] Sent auth request to AniDB");
		sendCommand(cmd);
	}
	@Override
	public void logout() {
		if (isConnected) {
			log.println("[UDPApi] Logout sent to AniDB");
			sendCommand(new UDPApiCommand("LOGOUT","logout", false));
		} else if (this.showDebug) 
			System.out.println("tried to logout with being connected and/or authed");
		connectionInfo.encodingSet = false;
		connectionInfo.isAuthed = false;
	}
	@Override
	public void sendCommand(ApiCommand command) {
		connectionInfo.queuedCommands.add((UDPApiCommand) command);

		if (Sender.getState() == Thread.State.NEW) {
			Sender.start();
		} else if (Sender.getState() == Thread.State.TERMINATED) {
			Sender = new Thread(new UDPApiSender(this.connectionInfo), "Sender");
			Sender.start();            
		}
	}
}

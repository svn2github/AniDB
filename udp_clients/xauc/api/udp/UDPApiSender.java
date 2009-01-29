package api.udp;

import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.nio.charset.Charset;
import java.util.Date;
import java.util.Map;
import java.util.concurrent.LinkedBlockingQueue;

public class UDPApiSender implements Runnable {
	/** The queued commands list */
	protected LinkedBlockingQueue<UDPApiCommand> queuedCommands = null;
	/** The last delayed packet timestamp */
	protected Date lastDelayPacket = null;
	/** Current timestamp */
	protected Date now;
	/** How many queued replies do we have */
	protected volatile int pendingReplies;
	/** Connection Information */
	protected UDPConnectionInfo connectionInfo = new UDPConnectionInfo();
	
	/**
	 * Creates a new Sender Thread
	 * @param connectionInfo A reference to connection information
	 */
	public UDPApiSender(UDPConnectionInfo connectionInfo) {
		this.connectionInfo = connectionInfo;
		this.queuedCommands = this.connectionInfo.queuedCommands;
		this.pendingReplies = this.connectionInfo.pendingReplies;
	}

	/**
	 * Transforms a command in its byte representation to send over a socket
	 * @param cmd Command to transform
	 * @return Transformed command
	 */
	public byte[] TransformCmd(UDPApiCommand cmd) {
		String CmdString;

		UDPApiQuery Query;
		if(cmd.queryID == -1) {
			cmd.queryID = this.connectionInfo.queuedQueries.size();
			Query = new UDPApiQuery();
			Query.sentCmd = cmd;
			this.connectionInfo.queuedQueries.add(Query);
		} else
			Query = this.connectionInfo.queuedQueries.get(cmd.queryID);

		Query.sentOn = new Date(System.currentTimeMillis());

		CmdString = cmd.action + " tag=" + cmd.tag + "-" + cmd.queryID;

		if (this.connectionInfo.session != null) 
			CmdString += "&s=" + this.connectionInfo.session;

		for (Map.Entry<String, String> Param : cmd.params.entrySet())
			CmdString += "&" + Param.getKey() + "=" + Param.getValue();

		System.out.print("Sending packet: " + CmdString);

		if (this.connectionInfo.encodingSet)
			return CmdString.getBytes(Charset.forName("ASCII"));
		else
			return CmdString.getBytes(Charset.forName("UTF8"));
	}
	
	@Override
	public void run() {
		try {
			/** Queued command to process */
			UDPApiCommand cmd = queuedCommands.take();
			/** The data packet */
			DatagramPacket packet;
			// This thread will execute while there are commands to run
			while (!cmd.action.equals("TERMINATE")) {
				now = new Date(System.currentTimeMillis());
				long remainingGoTime = (now.getTime() - connectionInfo.lastPacketOn.getTime());
				if (pendingReplies < 3)
				
/*				
				if(pendingReplies < 3 || !connectionInfo.isAuthed) {
					if((!cmd.loginReq || connectionInfo.isAuthed) && 
							(connectionInfo.expressPackets.contains(cmd.action) ||
									lastDelayPacket == null || (now.getTime() - lastDelayPacket.getTime()) > 2200)) {
						byte[] CmdBin = TransformCmd(cmd);
						packet = new DatagramPacket(CmdBin, CmdBin.length, connectionInfo.AniDBIP, connectionInfo.port);

						try {
							connectionInfo.socket.send(packet);
						} catch (Exception exception) {
							exception.printStackTrace();
						}
						connectionInfo.lastPacketOn = new Date(now.getTime());
						System.out.println("... sent");

						if(!connectionInfo.expressPackets.contains(cmd.action)) lastDelayPacket = new Date(now.getTime());

					} else {
						for (int I = 0; I < CmdToSend.size(); I++) {
							if (!CmdToSend.get(I).loginReq) {
								CmdToSend.add(0, CmdToSend.get(I));
								CmdToSend.remove(I + 1);
								break;
							}
						}
					}
				}
*/
				// Sleeps while next command isn't in queue
				cmd = queuedCommands.take();
			}
		}
		catch (Exception e) {
			System.out.println(Thread.currentThread().getName() + " " + e.getMessage());
		}
	}

}

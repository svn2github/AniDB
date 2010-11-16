/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import aniDB.udpApi.client.accountManagement.AccountManagement;
import aniDB.udpApi.client.accountManagement.IAccountListener;
import aniDB.udpApi.client.accountManagement.IAccountListener.AccountEvent;
import aniDB.udpApi.client.accountManagement.UserAccount;
import aniDB.udpApi.client.registerManagement.RegisterManagement;
import aniDB.udpApi.client.replies.Reply;
import aniDB.udpApi.shared.ByReference;
import aniDB.udpApi.shared.IAction;
import aniDB.udpApi.shared.ThreadBase;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.zip.Inflater;
import java.util.zip.InflaterInputStream;

/**
 *
 * @author Arokh
 */
public class ClientAPI {
	public static final int MAX_RETRY_COUNT = 3; //Drop Cmd after * retry
	public static final int MAX_REPLY_PENDING_COUNT = 3; //Delay sinding next cmd until pending replies is lower than *
	public static final int CLIENT_VERSION = 8;
	public static final int PROTOCOL_VERSION = 3;
	public static final int ANIDB_API_PORT = 9000;
	public static final String ANIDB_API_HOST = "127.0.0.1";//"api.anidb.info";
	public static final String CLIENT_TAG = "anitracker";
	public static final String NO_DELAY = "";//"FILE,ANIME,MYLISTADD"; //Cmds which do not need to respect PACKET_DELAY
	public static final int PACKET_DELAY = 2200; // TimeSpan.FromSeconds(2.2 /*0.0*/); //Delay between each packet send
	public static final int PING_INTERVAL = 5 * 60 * 1000; // TimeSpan.FromMinutes(5); //Pinginterval, to keep connection alive (NAT)
	public static final int REPLY_TIMEOUT = 20000; // TimeSpan.FromSeconds(20 /*2*/); //Time until reply is considered lost


	private DatagramSocket com;
	private List<CmdInfo> pendingRequests;
	private List<ReplyInfo> serverReplies;
	private List<Query> queries;

	private Receive receive;
	private Send send;
	private Idle idle;

	private IAction<ReplyInfo> throwEvent;
	private Object hQuery;


	private long lastPacketOn;
	private long lastDelayPacketOn;

	private int pendingReplyCount;

	private boolean behindNAT;
	private boolean isEncodingSet;

	private boolean apiUnavailable;
	private long apiDownUntil;


	private RegisterManagement registerManagement;
	private AccountManagement accountManagement;

	public ClientAPI() {
		ByReference<IAction<ReplyInfo>> throwEventRef = new ByReference<IAction<ReplyInfo>>();
		registerManagement = new RegisterManagement(throwEventRef);
		throwEvent = throwEventRef.getParam();

		accountManagement = new AccountManagement();
		accountManagement.addAccountListener(new IAccountListener() {
			public void action(UserAccount account, AccountEvent accountEvent) {
				accountListener(account, accountEvent);
			}
		});

		pendingRequests = new ArrayList<CmdInfo>();
		serverReplies = new ArrayList<ReplyInfo>();
		queries = new ArrayList<Query>();

		receive = new Receive(this);
		send = new Send(this);
		idle = new Idle(this);

		try {
			hQuery = registerManagement.register(new IAction<Reply>() { public void invoke(Reply param) { internalReplyHandling(param); } });
		} catch(Exception ex) {}

	}

	private void accountListener(UserAccount account, AccountEvent accountEvent){
		switch(accountEvent) {
			case Added:
				authenticate(account);
		}
	}

	private void authenticate(UserAccount account) {
		throw new UnsupportedOperationException("Not yet implemented");
	}

	private void processReply(String replyStr){

	}


	private void internalReplyHandling(Reply reply) {

	}
	private boolean internalReplyHook(ReplyInfo replyInfo) {
		return true;
	}

	private class Receive extends ThreadBase<ClientAPI> {
		public Receive(ClientAPI api) { super(api); }

        private byte[] inflatePacket(byte[] buffer) throws IOException{
			ByteArrayInputStream stream = new ByteArrayInputStream(buffer);
            stream.skip(4);
            InflaterInputStream iis = new InflaterInputStream(stream, new Inflater(true));
            ByteArrayOutputStream baos = new ByteArrayOutputStream(2 * 1400);

            int readBytes;
            byte[] b = new byte[1024];
            while((readBytes = iis.read(b))!= -1) baos.write(b, 0, readBytes);

            return baos.toByteArray();
        }

		@Override
		protected void DoWork() {
			byte[] b = new byte[1400];
			DatagramPacket packet = new DatagramPacket(b, 1400);
			String replystr = "";
			
			Thread t;

			while(!terminate) {
				try {
					api.com.receive(packet);

					replystr = new String(b[0] == 0 && b[1] == 0 ? inflatePacket(b) : b, "UTF8");

					t = new Thread(new Reply(replystr));
					t.start();
				} catch(Exception ex) {

				}
			}

		}

		private class Reply implements Runnable {
            String replyStr;

            public Reply(String replyStr) {
                this.replyStr = replyStr;
            }

            public void run() {
                api.processReply(replyStr);
            }
        }
	}
	private class Send extends ThreadBase<ClientAPI> {
		public Send(ClientAPI api) { super(api); }



		@Override
		protected void DoWork() {
			throw new UnsupportedOperationException("Not supported yet.");
		}

	}
	private class Idle extends ThreadBase<ClientAPI> {
		public Idle(ClientAPI api) { super(api); }



		@Override
		protected void DoWork() {
			throw new UnsupportedOperationException("Not supported yet.");
		}

	}
}

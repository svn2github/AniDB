package api.udp;

import utils.Utils;

import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Map;
import java.lang.reflect.Method;
import java.util.Date;
import java.util.TreeMap;

/**
 * UDP API implementation
 * @author fahrenheit
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */

public class UDPApiOld {
	public ArrayList<UDPApiQuery> Queries = new ArrayList<UDPApiQuery>();
	public ArrayList<UDPApiReply> ServerReply = new ArrayList<UDPApiReply>();
	int ReplysPending;
	ArrayList<String> NoDelay = new ArrayList<String>();

	java.net.DatagramSocket Com;

	Thread Sender = new Thread(new SendData(), "Sender");
	Thread SysCall = new Thread(new IdleTasks(), "SysCall");
	Thread Reciever = new Thread(new RecievedData(),"Reciever");
	Map<String, UDPRequestFlag> RegFuncs = new TreeMap<String, UDPRequestFlag>();

	boolean IsFinalizing;

	public UDPUserInfo userInfo = new UDPUserInfo();
	public UDPConnectionInfo conInfo = new UDPConnectionInfo();

	public ArrayList<UDPApiCommand> CmdToSend = new ArrayList<UDPApiCommand>();

	public UDPApiOld(Object CallingClass){

		try {
			Method ErrorHandlerFunc = this.getClass().getMethod("InternalReplyErrorHandling", new Class[] {int.class});
			RegisterEvent(ErrorHandlerFunc, this, "501", "502", "505", "506", "555", "598", "600", "601", "60");

			Method DefaultHandlerFunc = this.getClass().getMethod("InternalReplyHandling", new Class[] {int.class});
			RegisterEvent(DefaultHandlerFunc, this, "auth", "logout", "ping", "uptime");
		} catch (Exception exception) {java.lang.System.out.println("Event Reg: " + exception);}

	}

	public void Logout(boolean SendCmd) {
		if (SendCmd) {
			UDPApiCommand Cmd = new UDPApiCommand("LOGOUT","logout", false);
			QueryCmd(Cmd);
		}

		conInfo.encodingSet = false;
		conInfo.isAuthed = false;
	}

	public void Connect() throws Exception  {
		conInfo.AniDBIP = java.net.InetAddress.getByName(conInfo.hostname);
		Com = new java.net.DatagramSocket(12000);
		java.lang.System.out.println("Con Established");   

		conInfo.pingInterval = 1;
		conInfo.getInitMessages = true;
		conInfo.getPushMessages = false;

		java.lang.System.out.print("Starting IdleTasks Thread");
		if (SysCall.getState() == java.lang.Thread.State.NEW) {
			SysCall.start();
		} else if (SysCall.getState() == java.lang.Thread.State.TERMINATED) {
			SysCall = new Thread(new IdleTasks(), "IdleTasks");
			SysCall.start();            
		}
		java.lang.System.out.println("... started");

		java.lang.System.out.print("Starting Reciever Thread");
		if (Reciever.getState() == java.lang.Thread.State.NEW) {
			Reciever.start();
		} else if (Reciever.getState() == java.lang.Thread.State.TERMINATED) {
			Reciever = new Thread(new RecievedData(), "Reciever");
			Reciever.start();            
		}
		java.lang.System.out.println("... started");

		Authenticate();
	}

	void Authenticate(){ 
		UDPApiCommand cmd = new UDPApiCommand("AUTH","auth", false);
		cmd.params.put("user", userInfo.username);
		if (userInfo.session != null)
			cmd.params.put("sess", conInfo.session);
		if (userInfo.password != null)
			cmd.params.put("pass", userInfo.password);
		cmd.params.put("protover",Integer.toString(conInfo.getProtocolVersion()));
		cmd.params.put("client", conInfo.getClientName());
		cmd.params.put("clientver", Integer.toString(conInfo.getClientVersion()));
		cmd.params.put("nat", "1");
		cmd.params.put("enc", "UTF8");

		java.lang.System.out.println("Query Auth Command");
		QueryCmd(cmd);
	}

	public void QueryCmd(UDPApiCommand Cmd) {
		CmdToSend.add(Cmd);

		if (Sender.getState() == java.lang.Thread.State.NEW) {
			Sender.start();
		} else if (Sender.getState() == java.lang.Thread.State.TERMINATED) {
			Sender = new Thread(new SendData(), "Sender");
			Sender.start();            
		}
	}

	public void Finalize() {
		IsFinalizing = true;
		Com.close();

		try {
			long WaitForJoin = System.currentTimeMillis();
			Sender.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
			SysCall.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
			Reciever.join(4000);
			System.out.println(-1 * WaitForJoin + (WaitForJoin = System.currentTimeMillis()));
		} catch (Exception exception) {exception.printStackTrace();}

	}

	byte[] TransformCmd(UDPApiCommand cmd) {
		String CmdString;

		UDPApiQuery Query;
		if(cmd.queryID == -1) {
			cmd.queryID = Queries.size();
			Query = new UDPApiQuery();
			Query.sentCmd = cmd;
			Queries.add(Query);
		} else {
			Query = Queries.get(cmd.queryID);
		}
		Query.sentOn = new java.util.Date(System.currentTimeMillis());


		CmdString = cmd.action + " tag=" + cmd.tag + "-" + cmd.queryID;

		if (conInfo.session != null) CmdString += "&s=" + conInfo.session;

		for (Map.Entry<String, String> Param : cmd.params.entrySet()) {
			CmdString += "&" + Param.getKey() + "=" + Param.getValue();
		}

		java.lang.System.out.print("Sending packet: " + CmdString);

		if (conInfo.encodingSet) {
			return CmdString.getBytes(Charset.forName("ASCII"));
		} else {
			return CmdString.getBytes(Charset.forName("UTF8"));            
		}
	}

	public void ParseMessage(String Msg) {
		UDPApiReply ApiReply = new UDPApiReply();
		int Pos;

		if (!Utils.isNumber(Msg.substring(0, 3))) {
			Pos = Msg.indexOf("-");
			ApiReply.tag = Msg.substring(0, Pos);
			Msg = Msg.substring(Pos+1);

			Pos = Msg.indexOf(" ");
			ApiReply.cmdId = Integer.parseInt(Msg.substring(0, Pos));
			Msg = Msg.substring(Pos+1);

		} else {
			ApiReply.tag = "";
			ApiReply.cmdId = ServerReply.size();
		}

		Pos = Msg.indexOf(" ");
		ApiReply.responseID = Integer.parseInt(Msg.substring(0, Pos));
		Msg = Msg.substring(Pos+1);

		Pos = Msg.indexOf("\n");
		ApiReply.response = Msg.substring(0, Pos);
		Msg = Msg.substring(Pos+1);

		if (Msg.endsWith("\n")) {
			Msg = Msg.substring(0, Msg.length() - 1);
		}

		if (Msg.indexOf("|") != -1) {
			String[] DataFields;
			if (Msg.indexOf("\n") != Msg.lastIndexOf("\n") || ApiReply.response.equals("NOTIFYLIST")) {
				DataFields = Msg.split("\n");
			} else {
				//wierd splitting function: if last field empty, it is omitted. Adding a space & & delete it again after splitting
				DataFields = (Msg + " ").split("\\|");
				int I = DataFields.length-1;
				DataFields[I] = DataFields[I].substring(0, DataFields[I].length()-1);
			}

			for (String Str : DataFields) ApiReply.dataField.add(Str);

		} else if (!Msg.equals("")) {
			ApiReply.dataField.add(Msg);
		}

		if (!ApiReply.tag.equals("")) {
			Queries.get(ApiReply.cmdId).rcvdMsg = ApiReply;
			Queries.get(ApiReply.cmdId).rcvdOn.setTime(System.currentTimeMillis());
			Queries.get(ApiReply.cmdId).success = true;
		} else {
			ApiReply.tag = "[Server]";
			ServerReply.add(ApiReply);
		}

		DeliverReply(ApiReply);
	}

	void DeliverReply(UDPApiReply ApiReply) {
		UDPRequestFlag ReqFlag = RegFuncs.get(ApiReply.tag);
		if(ReqFlag == null) ReqFlag = RegFuncs.get(ApiReply.responseID);
		if(ReqFlag != null) {
			try {ReqFlag.func.invoke(ReqFlag.methodContainer, ApiReply.cmdId);} catch (Exception e) {}
		}
		if(ReqFlag == null) System.out.println("Msg didn't have any registered events: ID " + ApiReply.responseID);
	}

	public void RegisterEvent(Method Func, Object MethodContainer, String... Tags) {        
		for(String Tag : Tags) RegFuncs.put(Tag, new UDPRequestFlag(Tag, Func, MethodContainer));    
	}

	public void InternalReplyHandling(int ReplyIndex) {
		int Pos; String Resp;
		UDPApiReply ApiReply = Queries.get(ReplyIndex).rcvdMsg;

		if (ApiReply.tag.equals("auth")) {
			if (Utils.ContainsNumber(new int[] {200, 201}, ApiReply.responseID)) {
				if (conInfo.isAuthed) Logout(false);

				Resp = ApiReply.response;
				Pos = Resp.indexOf(" ");
				conInfo.session = Resp.substring(0, Pos);
				Resp = Resp.substring(Pos+1);

				conInfo.connectedOn = new Date(System.currentTimeMillis());
				conInfo.isAuthed = true;
				conInfo.AniDBApiDown = false;

				java.net.InetAddress IP;
				java.net.InetAddress RetIP;
				try {
					IP = java.net.InetAddress.getLocalHost();

					Pos = Resp.indexOf(":");
					RetIP = java.net.InetAddress.getByName(Resp.substring(0, Pos));   

					Resp = Resp.substring(Pos + 1);
					Pos = Resp.indexOf(" ");
					if (!IP.equals(RetIP) || !Resp.substring(0, Pos).equals("12000"))
						conInfo.NAT = true;
				} catch (Exception exception) {exception.printStackTrace();} 

				if (ApiReply.responseID == 201) {
					//New Version
				}

			} else if (Utils.ContainsNumber(new int[] {501, 503, 504}, ApiReply.responseID)) {
				//Failed Connecting
			}
		} else if (ApiReply.tag.equals("logout")) {
			if (ApiReply.responseID == 203) {
				//logged out
			} else if (ApiReply.responseID == 403) {
				//not logged in
			}
		} else if (ApiReply.tag.equals("ping")) {
		}

	}

	public void InternalReplyErrorHandling(int ReplyIndex) {
		UDPApiQuery Query = Queries.get(ReplyIndex);

		if (Utils.ContainsNumber(new int[] {501, 502, 505, 601, 602}, Query.rcvdMsg.responseID)) {
			Logout(false);
		}

		if (Utils.ContainsNumber(new int[] {501, 506}, Query.rcvdMsg.responseID)) {
			Authenticate();
			QueryCmd(Query.sentCmd);
		}
	}

	class RecievedData implements Runnable{
		public void run() {
			java.net.DatagramPacket Packet;
			String Reply;

			while (true) {
				Packet = new java.net.DatagramPacket(new byte[1399], 1399);
				try {
					Com.receive(Packet);

					Reply = new String(Packet.getData(), "UTF8");
					Reply = Reply.substring(0, Reply.lastIndexOf("\n") + 1);

					java.lang.System.out.print("ServerReply: " + Reply);
					ParseMessage(Reply);
				} catch (Exception exception) {
					if (IsFinalizing) break;
					conInfo.AniDBApiDown = true;
					exception.printStackTrace();  
					try {Thread.sleep(2000);} catch (Exception exception2) {}
				}

			}
		}
	}

	class SendData implements Runnable {
		Date LastDelayPacket = null;
		Date Now;

		public void run() {
			java.net.DatagramPacket Packet;

			while (CmdToSend.size() > 0) {
				if (IsFinalizing) break;

				Now = new java.util.Date(System.currentTimeMillis());

				if(ReplysPending < 3 || !conInfo.isAuthed) {
					if((!CmdToSend.get(0).loginReq || conInfo.isAuthed) && 
							(NoDelay.contains(CmdToSend.get(0).action) ||
									LastDelayPacket == null || (Now.getTime() - LastDelayPacket.getTime()) > 2200)) {
						byte[] CmdBin = TransformCmd(CmdToSend.get(0));
						Packet = new java.net.DatagramPacket(CmdBin, CmdBin.length, conInfo.AniDBIP, 9000);

						try {Com.send(Packet);} catch (Exception exception) {java.lang.System.out.println(exception);}
						conInfo.lastPacketOn = new Date(Now.getTime());
						java.lang.System.out.println("... sent");

						if(!NoDelay.contains(CmdToSend.get(0).action)) LastDelayPacket = new Date(Now.getTime());
						CmdToSend.remove(0);

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
			}

			try {Thread.sleep(200);} catch (Exception exception) {}
		}
	}

	class IdleTasks implements Runnable{
		public void run() {
			int PendingCount;
			java.util.Date LastNATActivity = new java.util.Date();

			try {Thread.sleep(1000);} catch (Exception exception) {} //avoid immediate ping

			while (true) {
				java.util.Date Now = new java.util.Date(System.currentTimeMillis());

				if (!conInfo.AniDBApiDown){
					if (conInfo.NAT && conInfo.lastPacketOn != null &&
							((Now.getTime() - conInfo.lastPacketOn.getTime()) / 60000) > conInfo.pingInterval &&
							((Now.getTime() - LastNATActivity.getTime()) / 60000 > 1)) {
						UDPApiCommand Cmd = new UDPApiCommand("PING", "ping", false);
						LastNATActivity.setTime(Now.getTime());
						QueryCmd(Cmd);
					}

					PendingCount = 0;
					for (int I = 0; I < Queries.size(); I++) {
						UDPApiQuery Query = Queries.get(I);

						if (!Query.success && Query.sentOn != null) {
							if(((Now.getTime() - Query.sentOn.getTime()) / 1000) > 20) {
								if(Query.retries < 3) {
									Query.retries += 1;
									Query.sentOn = null;
									System.out.print("TimeOut");

									QueryCmd(Query.sentCmd);
								} else {
									Query.success = false;
								}
							} else if(Query.retries <= 3) {
								PendingCount += 1;
							}
						}
					}
					ReplysPending = PendingCount;
				}

				try {Thread.sleep(1000);} catch (Exception exception) {}
				if (IsFinalizing) break;
			}
		}
	}
}

/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.Map;
import AniAdd.*;
import java.lang.reflect.Method;
import java.util.Date;

public class Api {
    AniAdd CC;

    public ArrayList<cQuery> Queries = new ArrayList<cQuery>();
    public ArrayList<cApiReply> ServerReply = new ArrayList<cApiReply>();

    java.net.DatagramSocket Com;
    
    Thread Sender = new Thread(new SendData(), "Sender");
    Thread SysCall = new Thread(new IdleTasks(), "SysCall");
    Thread Reciever = new Thread(new RecievedData(),"Reciever");
    ArrayList<cReqFlag> RegFuncs = new ArrayList<cReqFlag>();
    
    boolean IsFinalizing;
    
    public cUser UserInfo = new cUser();
    public cSettings Settings = new cSettings();
    public cConnectionInfo ConInfo = new cConnectionInfo();
    
    public ArrayList<cApiCmd> CmdToSend = new ArrayList<cApiCmd>();
    
    public Api(AniAdd CallingClass){
        CC = CallingClass;

        Method Func = null;
                
        try {
            Func = this.getClass().getMethod("InternalReplyErrorHandling", new Class[] {int.class});
            int[] Codes = {501, 502, 505, 506, 555, 598, 600, 601, 602};
            RegisterEvent(null, Codes, Func, this.getClass());
            
            Func = this.getClass().getMethod("InternalReplyHandling", new Class[] {int.class});
            RegisterEvent(new String[] {"auth"}, null, Func, this);
            RegisterEvent(new String[] {"logout"}, null, Func, this);
            RegisterEvent(new String[] {"ping"}, null, Func, this);
        } catch (Exception exception) {java.lang.System.out.println("Event Reg: " + exception);}
        

    }
    
    public void Logout(boolean SendCmd) {
        if (SendCmd) {
            cApiCmd Cmd = new cApiCmd("LOGOUT","logout", false);
            QueryCmd(Cmd);
        }
        
        ConInfo.EncodingSet = false;
        ConInfo.Connected = false;
    }
    
    public void Connect() throws Exception  {
        ConInfo.AniDBIP = java.net.InetAddress.getByName(ConInfo.Host);
        Com = new java.net.DatagramSocket(12000);
        java.lang.System.out.println("Con Established");   
        
        Settings.PingInterval = 1;
        Settings.GetInitMessages = true;
        Settings.GetPushMessages = false;
        
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
        
        cApiCmd Cmd = new cApiCmd("AUTH","auth", false);
        Cmd.Params.put("user", UserInfo.UserName);
	if (UserInfo.Session != null) {
	    Cmd.Params.put("sess", UserInfo.Session);
	}
	if (UserInfo.Password != null) {
	    Cmd.Params.put("pass", UserInfo.Password);
	}
	if (UserInfo.Autopass != null) {
	    Cmd.Params.put("autopass", UserInfo.Autopass);
	}
        Cmd.Params.put("protover",Integer.toString(ConInfo.ProtoVer));
        Cmd.Params.put("client", ConInfo.ClientID);
        Cmd.Params.put("clientver", Integer.toString(ConInfo.ClientVer));
        Cmd.Params.put("nat", "1");
        Cmd.Params.put("enc", "UTF8");
        
        java.lang.System.out.println("Query Auth Command");
        QueryCmd(Cmd);
    }

    public void QueryCmd(cApiCmd Cmd) {
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
    
    byte[] TransformCmd(cApiCmd Cmd) {
        String CmdString;
        
        CmdString = Cmd.Action + " tag=" + Cmd.Tag + "-" + Queries.size();
        
        if (ConInfo.Session != null) CmdString += "&s=" + ConInfo.Session;
        
        for (Map.Entry<String, String> Param : Cmd.Params.entrySet()) {
            CmdString += "&" + Param.getKey() + "=" + Param.getValue();
        }
        
        java.lang.System.out.print("Sending packet: " + CmdString);

        if (ConInfo.EncodingSet) {
            return CmdString.getBytes(Charset.forName("ASCII"));
        } else {
            return CmdString.getBytes(Charset.forName("UTF8"));            
        }
    }
    
    public void ParseMessage(String Msg) {
        cApiReply ApiReply = new cApiReply();
        int Pos;
        
        if (!Misc.isNumber(Msg.substring(0, 3))) {
            Pos = Msg.indexOf("-");
            ApiReply.Tag = Msg.substring(0, Pos);
            Msg = Msg.substring(Pos+1);
            
            Pos = Msg.indexOf(" ");
            ApiReply.CmdId = Integer.parseInt(Msg.substring(0, Pos));
            Msg = Msg.substring(Pos+1);
            
        } else {
            ApiReply.Tag = "";
            ApiReply.CmdId = ServerReply.size();
        }
        
        Pos = Msg.indexOf(" ");
        ApiReply.ResponseID = Integer.parseInt(Msg.substring(0, Pos));
        Msg = Msg.substring(Pos+1);
        
        Pos = Msg.indexOf("\n");
        ApiReply.Response = Msg.substring(0, Pos);
        Msg = Msg.substring(Pos+1);
        
        if (Msg.endsWith("\n")) {
            Msg = Msg.substring(0, Msg.length() - 1);
        }
        
        if (Msg.indexOf("|") != -1) {
            String[] DataFields;
            if (Msg.indexOf("\n") != Msg.lastIndexOf("\n") || ApiReply.Response.equals("NOTIFYLIST")) {
                DataFields = Msg.split("\n");
            } else {
                 //wierd splitting function: if last field empty, it is omitted. Adding a space & & delete it again after splitting
                DataFields = (Msg + " ").split("\\|");
                int I = DataFields.length-1;
                DataFields[I] = DataFields[I].substring(0, DataFields[I].length()-1);
            }
            
            for (String Str : DataFields) ApiReply.DataField.add(Str);
            
        } else if (!Msg.equals("")) {
            ApiReply.DataField.add(Msg);
        }
        
        if (!ApiReply.Tag.equals("")) {
            Queries.get(ApiReply.CmdId).RcvdMsg = ApiReply;
            Queries.get(ApiReply.CmdId).RcvdOn.setTime(System.currentTimeMillis());
            Queries.get(ApiReply.CmdId).Success = true;
        } else {
            ApiReply.Tag = "[Server]";
            ServerReply.add(ApiReply);
        }
        
        DeliverReply(ApiReply);
    }
    
    void DeliverReply(cApiReply ApiReply) {
        cReqFlag Flag = new cReqFlag(new String[] {ApiReply.Tag},new int[] {ApiReply.ResponseID}, null, null);
        
        for (cReqFlag RF : RegFuncs) {
            if (cReqFlag.Compare(RF, Flag)) {
                System.out.println("Invoking: " + RF.Func.getName());
                try {
                    if (ApiReply.Tag.equals("[Server]")) {
                        RF.Func.invoke(RF.MethodContainer, ApiReply.CmdId ^ -1);
                    } else {
                        RF.Func.invoke(RF.MethodContainer, ApiReply.CmdId);
                    }
                } catch (Exception exception) {exception.printStackTrace();}
                return;
            }
        }
    }
    
    public void RegisterEvent(String[]Tag, int[] IDs, Method Func, Object MethodContainer) {
        cReqFlag Flag = new cReqFlag(Tag, IDs, null, null);
        if (Func != null) {
            for (cReqFlag RF : RegFuncs) {
                if (cReqFlag.Compare(RF, Flag)) {
                    RF.Func = Func;
                    return;
                }
            }
            RegFuncs.add(new cReqFlag(Tag, IDs, Func, MethodContainer));
        } else {
            for (cReqFlag RF : RegFuncs) {
                if (cReqFlag.Compare(RF, Flag)) {
                    RegFuncs.remove(RF);
                    return;
                }
            }
        }
    }
    
    public void InternalReplyHandling(int ReplyIndex) {
        int Pos; String Resp;
        cApiReply ApiReply = Queries.get(ReplyIndex).RcvdMsg;
        
        if (ApiReply.Tag.equals("auth")) {
            if (Misc.ContainsNumber(new int[] {200, 201}, ApiReply.ResponseID)) {
                if (ConInfo.Connected) Logout(false);
                
                Resp = ApiReply.Response;
                Pos = Resp.indexOf(" ");
                ConInfo.Session = Resp.substring(0, Pos);
                Resp = Resp.substring(Pos+1);
                
                ConInfo.ConnectedOn = new Date(System.currentTimeMillis());
                ConInfo.Connected = true;
                ConInfo.AniDBApiDown = false;
                
                java.net.InetAddress IP;
                java.net.InetAddress RetIP;
                try {
                    IP = java.net.InetAddress.getLocalHost();
                    
                    Pos = Resp.indexOf(":");
                    RetIP = java.net.InetAddress.getByName(Resp.substring(0, Pos));   
                    
                        ConInfo.NAT = true;
		    Resp = Resp.substring(Pos+1);
                    Pos = Resp.indexOf(" ");
                    if (!IP.equals(RetIP) || !Resp.substring(0, Pos).equals("12000"))
                        ConInfo.NAT = true;
                } catch (Exception exception) {exception.printStackTrace();} 

                if (ApiReply.ResponseID == 201) {
                    //New Version
                }
                
            } else if (Misc.ContainsNumber(new int[] {501, 503, 504}, ApiReply.ResponseID)) {
                //Failed Connecting
            }
        } else if (ApiReply.Tag.equals("logout")) {
            if (ApiReply.ResponseID == 203) {
                //logged out
            } else if (ApiReply.ResponseID == 403) {
                //not logged in
            }
        } else if (ApiReply.Tag.equals("ping")) {
        }

    }
    
    public void InternalReplyErrorHandling(int ReplyIndex) {
        cQuery Query = Queries.get(ReplyIndex);
        
        if (Misc.ContainsNumber(new int[] {501, 502, 505, 601, 602}, Query.RcvdMsg.ResponseID)) {
            Logout(false);
        }
        
        if (Misc.ContainsNumber(new int[] {501, 506}, Query.RcvdMsg.ResponseID)) {
            Authenticate();
            QueryCmd(Query.SendCmd);
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
                    ConInfo.AniDBApiDown = true;
                    exception.printStackTrace();  
                    try {Thread.sleep(2000);} catch (Exception exception2) {}
                }
                
            }
        }
    }
    
    class SendData implements Runnable {
        public void run() {
            int Continuity = 0;
            java.net.DatagramPacket Packet;
            
            //java.lang.System.out.println("Beginning");

            while (CmdToSend.size() > 0) {
                if (IsFinalizing) break;
                //java.lang.System.out.print("Middle ");
                
                if (!CmdToSend.get(0).LoginReq || ConInfo.Connected) {
                    //java.lang.System.out.println("0");

                    ConInfo.LastPackedOn = new java.util.Date(System.currentTimeMillis());

                    byte[] CmdBin = TransformCmd(CmdToSend.get(0));
                    Packet = new java.net.DatagramPacket(CmdBin, CmdBin.length, ConInfo.AniDBIP, 9000);

                    try {
                        Com.send(Packet);
                    } catch (Exception exception) {
                        java.lang.System.out.println(exception);
                    }
                    java.lang.System.out.println("... sent");


                    cQuery Query = new cQuery();
                    Query.SendCmd = CmdToSend.get(0);
                    Query.SendOn = new java.util.Date(System.currentTimeMillis());
                    Queries.add(Query);

                    Continuity += 1;

                    CmdToSend.remove(0);
                } else {
                    //Cmd needs login but client is not logged in
                    //Move cmds without login req. to top else wait a sec before trying again if all of them req. login
                    //java.lang.System.out.println("1");

                    for (int I = 0; I < CmdToSend.size(); I++) {
                        if (!CmdToSend.get(I).LoginReq) {
                            CmdToSend.add(0, CmdToSend.get(I));
                            CmdToSend.remove(I + 1);
                        }
                    }
                    try {Thread.sleep(1000);} catch (Exception exception) {}
                    continue;
                }

                if (!ConInfo.Connected || Continuity <= 20) {
                    try {Thread.sleep(100);} catch (Exception exception) {}
                } else {
                    try {Thread.sleep(30000);} catch (Exception exception) {}
                    Continuity = 0;
                }
                
            }
            
            //java.lang.System.out.println("Ending");
        }
    }
    
    class IdleTasks implements Runnable{
        public void run() {
            java.util.Date LastNATActivity = new java.util.Date();
            java.util.Date LastInitMsgRet = new java.util.Date();

            try {Thread.sleep(1000);} catch (Exception exception) {} //avoid immediate ping

            while (true) {
                java.util.Date Now = new java.util.Date(System.currentTimeMillis());

                /*if (ConInfo.DownUntil != null && ConInfo.AniDBApiDown && ConInfo.DownUntil.getTime() - Now.getTime() < 0) {
                    Authenticate();
                }*/

                if (!ConInfo.AniDBApiDown){
                    if (ConInfo.NAT &&
                     ((Now.getTime() - ConInfo.LastPackedOn.getTime()) / 60000) > Settings.PingInterval &&
                     ((Now.getTime() - LastNATActivity.getTime()) / 60000 > 1)) {
                        cApiCmd Cmd = new cApiCmd("PING", "ping", false);
                        LastNATActivity.setTime(Now.getTime());
                        QueryCmd(Cmd);
                    }

                    if (ConInfo.Connected) {
                        if (Settings.GetInitMessages && ((Now.getTime() - LastInitMsgRet.getTime()) / 60000 > 20)) {
                            cApiCmd Cmd = new cApiCmd("NOTIFY", "notify", true);
                            Cmd.Params.put("buddy", "1");
                            LastNATActivity.setTime(Now.getTime());
                            QueryCmd(Cmd);
                            Settings.GetInitMessages = false;
                        }
                    }

                    for (int I = 0; I < Queries.size(); I++) {
                        cQuery Query = Queries.get(I);

                        if (!Query.Success && Query.SendOn != null && ((Now.getTime() - Query.SendOn.getTime()) / 1000) > 10) {
                            if (Query.Retries < 2) {
                                Query.SendOn = null; //needs change
                                Query.Retries += 1;
                                QueryCmd(Query.SendCmd);
                                System.out.println(Query.Retries);
                            } else {
                                //Faild to send the packet 2 times.. stopped trying
                            }
                        }
                    }
                }

                try {Thread.sleep(5000);} catch (Exception exception) {}
                if (IsFinalizing) break;
            }
        }
    }
}

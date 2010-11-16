/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client.registerManagement;

import aniDB.udpApi.client.ReplyInfo;
import aniDB.udpApi.client.replies.Reply;
import aniDB.udpApi.shared.ByReference;
import aniDB.udpApi.shared.IAction;
import aniDB.udpApi.shared.IdGenerator;
import aniDB.udpApi.shared.ReplyCodes;
import java.util.HashMap;
import java.util.Map;

/**
 *
 * @author Arokh
 */
public class RegisterManagement {
	private Map<IdGenerator, Registrant> register;
	private Map<ReplyCodes, IAction<Reply>> serverRegister;

	public RegisterManagement(ByReference<IAction<ReplyInfo>> throwEventRef) {
		register = new HashMap<IdGenerator, Registrant>();
		serverRegister = new HashMap<ReplyCodes, IAction<Reply>>();

		throwEventRef.setParam(new IAction<ReplyInfo>() { public void invoke(ReplyInfo param) { throwEvent(param); } });
	}

	public Object register(IAction<Reply> callBack) throws Exception {
		if(callBack == null) throw new Exception("callBack may not be null");

		Registrant registrant = new Registrant(callBack);
		register.put(registrant.getIdGenerator(), registrant);

		return registrant.getIdGenerator();
	}

	public void unregister(Object handle) throws Exception{
		if(handle == null || !(handle instanceof IdGenerator)) throw new Exception("Invalid handle");
		if(!register.containsKey((IdGenerator)handle)) throw new Exception("Handle not registered");

		register.remove((IdGenerator)handle);
	}

	//public void registerServer(IAction<Reply> callBack, ReplyCodes ... replyCodes) {
	//	for(ReplyCodes replyCode in replyCodes) serverRegister.get(replyCode). += callBack;
	//}
	//public void unregisterServer(Action<Reply> callBack, params eReplyCode[] eventCodes) {
	//	foreach(var eventCode in eventCodes) serverRegister[eventCode] -= callBack;
	//}

	private void throwEvent(ReplyInfo replyInfo) {
		if(replyInfo.getId() == null) {
			IAction<Reply> callBack;
			if((callBack = serverRegister.get(replyInfo.getReply().getCode())) == null) return;
			callBack.invoke(replyInfo.getReply());
		} else {
			for(Registrant registrant : register.values()) {
				if(registrant.getIdGenerator().isValid(replyInfo.getId(), true)) registrant.callBack(replyInfo.getReply());
			}
		}
	}
}

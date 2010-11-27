/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import aniDB.udpApi.client.replies.Reply;
import aniDB.udpApi.shared.ReplyCodes;

/**
 *
 * @author Arokh
 */
public class ReplyInfo {
	private Reply reply;
	private Long id;
	private int index;

	public ReplyInfo(Reply reply, Long id, int index) {
		this.reply = reply;
		this.id = id;
		this.index = index;
	}

	


	public Reply getReply() {
		return reply;
	}

	public Long getId() {
		return id;
	}

	public int getIndex() {
		return index;
	}

	public static ReplyInfo parseReplyString(String replyStr) throws Exception{
		if(replyStr == null || replyStr.isEmpty()) throw new Exception();

		Reply reply = new Reply();
		Integer index;
		Long id;

		if(replyStr.substring(0, replyStr.indexOf(' ')).contains(":")) {
			id = Long.parseLong(replyStr.substring(0, replyStr.indexOf(':')), 16); replyStr = replyStr.substring(replyStr.indexOf(':') + 1);

			if(replyStr.contains(":") && replyStr.indexOf(':') < replyStr.indexOf(' ')) {
				reply.setTag(replyStr.substring(0, replyStr.indexOf(':'))); replyStr = replyStr.substring(replyStr.indexOf(':') + 1);
			}

			index = Integer.parseInt(replyStr.substring(0, replyStr.indexOf(' '))); replyStr = replyStr.substring(replyStr.indexOf(' ') + 1);
		} else {
			id = null;
			index = null;
		}

		reply.setCode(ReplyCodes.getEnum(Integer.parseInt(replyStr.substring(0, replyStr.indexOf(' '))))); replyStr = replyStr.substring(replyStr.indexOf(' ') + 1);
		reply.setMessage(replyStr.substring(0, replyStr.indexOf('\n'))); replyStr = replyStr.substring(replyStr.indexOf("\n") + 1);

		if(replyStr.endsWith("\n")) replyStr = replyStr.substring(0, replyStr.length() - 1);

		if(replyStr.contains("|")) {
			if(replyStr.indexOf("\n") != replyStr.lastIndexOf("\n") || reply.getMessage().equals("NOTIFYLIST")) {
				reply.setData(replyStr.split("\n"));
			} else {
				String[] dataFields = (replyStr+" ").split("\\|");
				dataFields[dataFields.length] = dataFields[dataFields.length].substring(0, dataFields[dataFields.length].length() - 1);
				reply.setData(dataFields);
			}

		} else if(!replyStr.isEmpty()) {
			reply.setData(new String[] { replyStr });
		} else {
			reply.setData(new String[0]);
		}

		reply.setFinal();
		return new ReplyInfo(reply, id, index);
	}

}

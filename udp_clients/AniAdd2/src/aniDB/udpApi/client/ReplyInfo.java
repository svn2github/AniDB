/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import aniDB.udpApi.client.replies.Reply;
import aniDB.udpApi.shared.ByReference;

/**
 *
 * @author Arokh
 */
public class ReplyInfo {
	private Reply reply;
	private Long id;
	private int index;

	public Reply getReply() {
		return reply;
	}

	public Long getId() {
		return id;
	}

	public int getIndex() {
		return index;
	}

	public static Reply ParseReplyString(String replyStr, ByReference<Long> idRef, ByReference<Integer> index) throws Exception{
		if(replyStr == null || replyStr.isEmpty()) throw new Exception();

		Reply reply = new Reply();


		return reply;
	}

}

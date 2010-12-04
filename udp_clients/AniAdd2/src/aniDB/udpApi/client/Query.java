/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.client;

import java.util.Date;

/**
 *
 * @author Arokh
 */
public class Query {
	private Date requestOn;
	private Date replyOn;
	private CmdInfo cmdInfo;
	private ReplyInfo replyInfo;
	private int retryCount;
	private State state;

	void setRequestOn(Date requestOn) {
		this.requestOn = requestOn;
	}

	void setReplyOn(Date replyOn) {
		this.replyOn = replyOn;
	}

	public Date getRequestOn() {
		return requestOn;
	}

	public Date getReplyOn() {
		return replyOn;
	}

	public CmdInfo getCmdInfo() {
		return cmdInfo;
	}

	public ReplyInfo getReplyInfo() {
		return replyInfo;
	}

	void setCmdInfo(CmdInfo cmdInfo) {
		this.cmdInfo = cmdInfo;
	}

	void setReplyInfo(ReplyInfo replyInfo) {
		this.replyInfo = replyInfo;
	}

	void raiseRetryCount() { retryCount++; }

	void setState(State state) { this.state = state;  }

	State getState() { return state; }

	int getRetryCount() { return retryCount; }

	public static enum State { Pending, Success, Failed }
}

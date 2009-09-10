/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package udpApi;

import java.util.Date;

public class Query {
    private Cmd cmd;
    private Reply reply;
    private Date sendOnMills;
    private Date replyOnMills;
    private int retries;
    private Boolean success;

    public Cmd getCmd() {return cmd;}
	public void setCmd(Cmd cmd) {this.cmd = cmd;}
	public Reply getReply() {return reply;}
	public void setReply(Reply reply) {this.reply = reply;}
	public Date getSendOn() {return sendOnMills;}
	public void setSendOn(Date sendOn) {this.sendOnMills = sendOn;}
	public Date getReplyOn() {return replyOnMills;}
	public void setReplyOn(Date replyOn) {this.replyOnMills = replyOn;}
	public int getRetries() {return retries;}
	public void setRetries(int retries) {this.retries = retries;}
	public Boolean getSuccess() {return success;}
	public void setSuccess(Boolean success) {this.success = success;}
}

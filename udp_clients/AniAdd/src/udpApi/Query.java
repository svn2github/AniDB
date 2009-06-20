/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package udpApi;

public class Query {
    private Cmd cmd;
	private Reply reply;
    private Long sendOnMills;
    private Long replyOnMills;
    private int retries;
    private Boolean success;   
    
    public Cmd getCmd() {return cmd;}
	public void setCmd(Cmd cmd) {this.cmd = cmd;}
	public Reply getReply() {return reply;}
	public void setReply(Reply reply) {this.reply = reply;}
	public Long getSendOn() {return sendOnMills;}
	public void setSendOn(Long sendOn) {this.sendOnMills = sendOn;}
	public Long getReplyOn() {return replyOnMills;}
	public void setReplyOn(Long replyOn) {this.replyOnMills = replyOn;}
	public int getRetries() {return retries;}
	public void setRetries(int retries) {this.retries = retries;}
	public Boolean getSuccess() {return success;}
	public void setSuccess(Boolean success) {this.success = success;}
}

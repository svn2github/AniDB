/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package udpApi;

import java.util.*;

public class Reply {

    private String tag;
    private String identifier;
    private Integer queryId;
    private Integer replyId;
    private String replyMsg;
    private ArrayList<String> dataField;

    public Reply() {
        dataField = new ArrayList<String>();
    }

    public String Identifier() {
        return identifier;
    }

    public String Tag() {
        return tag;
    }

    public Integer QueryId() {
        return queryId;
    }

    public Integer ReplyId() {
        return replyId;
    }

    public String ReplyMsg() {
        return replyMsg;
    }

    public ArrayList<String> DataField() {
        return dataField;
    }

    public void Identifier(String identifier) {
        this.identifier = identifier;
    }

    public void Tag(String tag) {
        this.tag = tag;
    }

    public void QueryId(Integer queryId) {
        this.queryId = queryId;
    }

    public void ReplyId(Integer replyId) {
        this.replyId = replyId;
    }

    public void ReplyMsg(String reply) {
        this.replyMsg = reply;
    }
}

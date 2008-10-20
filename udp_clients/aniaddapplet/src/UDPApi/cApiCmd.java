/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

public class cApiCmd {
    public String Tag;
    public String Action;
    public boolean LoginReq;
    public int QueryID = -1;
    public java.util.Map<String, String> Params = new java.util.TreeMap<String, String>();

    public cApiCmd(String SetAction, String SetTag, boolean SetLoginReq) {
        Tag = SetTag;
        Action = SetAction;
        LoginReq = SetLoginReq;
    }
    public cApiCmd(cApiCmd APICmd) {
        Tag = APICmd.Tag;
        Action = APICmd.Action;
        LoginReq = APICmd.LoginReq;

        Params.putAll(APICmd.Params);
    } 
}

/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

import java.util.*;

public class cQuery {
    public cApiCmd SendCmd;
    public cApiReply RcvdMsg;
    public Date SendOn = new Date();
    public Date RcvdOn = new Date();
    public int Retries;
    public boolean Success;               
}

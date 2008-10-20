/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

import java.util.*;

public class cConnectionInfo {
    public String Host = "api.anidb.info";
    public java.net.InetAddress AniDBIP;

    public int ProtoVer = 3;
    public String ClientID = "aniadd";
    public int ClientVer = 2;

    public String Session;
    public boolean EncodingSet;
    public boolean NAT;
    public boolean IsAuthed;
    public Date ConnectedOn;
    public Date LastPackedOn;

    public boolean AniDBApiDown;
    public Date DownUntil;
}

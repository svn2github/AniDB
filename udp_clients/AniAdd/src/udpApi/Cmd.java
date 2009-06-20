/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package udpApi;

import java.util.Map;
import java.util.TreeMap;
import java.util.Map.Entry;

public class Cmd {
	private String identifier;
	private String tag;
	
	private String action;
	private boolean loginReq;
	private Integer queryId;
	private Map<String, String> params = new TreeMap<String, String>();

	public String Identifier() {return identifier;}
	public String Tag() {return tag;}
	public String Action() {return action;}
	public boolean LoginReq() {return loginReq;}
	public Integer QueryId() {return queryId;}
	public Map<String, String> Params() {return params;}

	public void Identifier(String identifier) {this.identifier = identifier;}
	public void Tag(String tag) {this.tag = tag;}
	public void Action(String action) {this.action = action;}
	public void LoginReq(boolean loginReq) {this.loginReq = loginReq;}
	public void QueryId(Integer queryId) {this.queryId = queryId;}

	public Cmd(String action, String identifier, String tag, boolean loginReq) {
		this.identifier = identifier;
		this.tag = tag;
		this.action = action;
		this.loginReq = loginReq;
		this.queryId = null;
	}

	public Cmd(Cmd cmd, boolean isNewCmd) {
		tag = cmd.tag;
		action = cmd.action;
		queryId = isNewCmd?null:cmd.queryId;
		loginReq = cmd.loginReq;
		identifier = cmd.identifier;

		params.putAll(cmd.params);
	}

	public void setArgs(String...args){
		for (int i = 0; i < args.length; i=i+2) {
			params.put(args[i], args[i+1]);
		}
	}
	public String toString(String session) {
        String cmdStr;
        cmdStr = action + " tag=" + identifier + (tag==null?"":":"+tag) + "-" + queryId;

        if(session != null) cmdStr += "&s=" + session;

        for (Entry<String, String> arg : params.entrySet()) {
			cmdStr += "&" + arg.getKey() + "=" + arg.getValue();
		}

        return cmdStr;
	}
}

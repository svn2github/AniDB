/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public enum DBSCode {
	IsV1CrcUnChecked(0),
	CrcOK(1),
	CrcErr(2),
	IsV2(4),
	IsV3(8),
	IsV4(16),
	IsV5(32),
	Uncensored(64),
	Censored(128);

	int code;
	DBSCode(int code){
		this.code = code;
	}
	public int getCode(){
		return code;
	}
}

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public enum DBFCode {
	//Byte 1
	//public static final B17=1<<7,
	B16_AId(1 << 6),
	B15_EId(1 << 5),
	B14_GId(1 << 4),
	B13_LId(1 << 3),
	B12_LstOtherEps(1 << 2),
	B11_IsDeprecated(1 << 1),
	B10_State(1),
	//Byte 2
	B27_Size(1 << 15),
	B26_Ed2k(1 << 14),
	B25_Md5(1 << 13),
	B24_Sha1(1 << 12),
	B23_Crc32(1 << 11),
	//B22=1<<10),
	//B21=1<<9),
	//B20=1<<8),
	//Byte 3
	B37_Quality(1 << 23),
	B36_Source(1 << 22),
	B35_AudioCodec(1 << 21),
	B34_AudioBitRate(1 << 20),
	B33_VideoCodec(1 << 19),
	B32_VideoBitRate(1 << 18),
	B31_VideoRes(1 << 17),
	B30_FileExt(1 << 16),
	//Byte 4
	B47_DubLanguage(1 << 31),
	B46_SubLanguage(1 << 30),
	B45_LengthInSec(1 << 29),
	B44_Description(1 << 28),
	B43_ReleaseDate(1 << 27),
	//B42=1<<26),
	//B41=1<<25),
	B40_AniDBFileName(1 << 24);


	int code;
	DBFCode(int code){
		this.code = code;
	}
	public int getCode(){
		return code;
	}

}

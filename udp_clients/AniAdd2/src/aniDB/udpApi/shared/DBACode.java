/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniDB.udpApi.shared;

/**
 *
 * @author Arokh
 */
public enum DBACode {
	//Byte 1
	B17_TotalEps(1 << 7),
	B16_HighestEpNo(1 << 6),
	B15_Year(1 << 5),
	B14_Type(1 << 4),
	B13_RelatedAIdType(1 << 3),
	B12_RelatedAIdLst(1 << 2),
	B11_CategoryLst(1 << 1),
	//B10=1<<0),
	//Byte 2
	B27_SeriesRomajiName(1 << 15),
	B26_SeriesKanjiName(1 << 14),
	B25_SeriesEngName(1 << 13),
	B24_SeriesOtherName(1 << 12),
	B23_SeriesShortName(1 << 11),
	B22_SeriesSynonym(1 << 10),
	B21_ProducerNames(1 << 9),
	B20_ProducerIds(1 << 8),
	//Byte 3
	B37_EpNo(1 << 23),
	B36_EpEngName(1 << 22),
	B35_EpRomajiName(1 << 21),
	B34_EpKanjiName(1 << 20),
	//B33=1<<19),
	//B32=1<<18),
	//B31=1<<17),
	//B30=1<<16),
	//Byte 4
	B47_GroupName(1 << 31),
	B46_GroupNameShort(1 << 30),
	//B45=1<<29),
	//B44=1<<28),
	//B43=1<<27),
	//B42=1<<26),
	//B41=1<<25),
	B40_DateAIdRecordUpdated(1 << 24);


	int code;
	DBACode(int code){
		this.code = code;
	}
	public int getCode(){
		return code;
	}
}

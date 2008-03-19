#pragma once
#include "tag.h"

class CID3V2Tag :
	public CTag
{
public:
	static CID3V2Tag* FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD dwBegin, DWORD dwEnd);

	CID3V2Tag(CMPAStream* pMPAStream, bool bAppended, DWORD dwOffset);
	~CID3V2Tag(void);

	WORD m_wVersion;
	bool m_bUnsynchronization, m_bExtHeader, m_bExperimental, m_bFooter;
private:
	static DWORD GetSynchsafeInteger(DWORD dwValue);

};

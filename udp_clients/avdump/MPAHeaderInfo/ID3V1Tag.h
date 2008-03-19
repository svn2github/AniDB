#pragma once
#include "tag.h"

class CID3V1Tag :
	public CTag
{
public:
	static CID3V1Tag* FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD dwBegin, DWORD dwEnd);

	CID3V1Tag(CMPAStream* pMPAStream, DWORD dwOffset);
	~CID3V1Tag(void);
};

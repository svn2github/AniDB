#pragma once
#include "tag.h"

class CLyrics3Tag :
	public CTag
{
public:
	static CLyrics3Tag* FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD dwBegin, DWORD dwEnd);

	CLyrics3Tag(CMPAStream* pMPAStream, DWORD dwOffset, bool bVersion2);
	virtual ~CLyrics3Tag(void);
};

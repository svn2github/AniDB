#pragma once
#include "tag.h"

class CAPETag :
	public CTag
{
public:
	static CAPETag* CAPETag::FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD dwBegin, DWORD dwEnd);

	CAPETag(CMPAStream* pMPAStream, bool bAppended, DWORD dwOffset);
	virtual ~CAPETag(void);

private:
	DWORD m_dwNumItems;
	bool m_bFooter, m_bHeader;

};

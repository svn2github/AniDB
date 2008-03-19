#pragma once
#include "mpaframe.h"
#include "vbrheader.h"


class CXINGHeader :
	public CVBRHeader
{
public:
	static CXINGHeader* FindHeader(const CMPAFrame* pMPAFrame);

	CXINGHeader(const CMPAFrame* pMPAFrame, DWORD dwOffset);
	virtual ~CXINGHeader(void);

	virtual DWORD SeekPosition(float& fPercent) const;
};

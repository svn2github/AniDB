#pragma once

#include <vector>

#include "mpastream.h"
#include "tag.h"

class CTags
{
public:
	CTags(CMPAStream* pMPAStream, DWORD& dwBegin, DWORD& dwEnd);
	~CTags(void);

	CTag* GetNextTag(int& nIndex) const;

//	void Dump(CString& strInfo) const;

private:
	template<typename Tag> bool FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD& dwBegin, DWORD& dwEnd);
	std::vector <CTag*> m_Tags;
};


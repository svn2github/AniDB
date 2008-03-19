#include "StdAfx.h"
#include ".\tags.h"
#include "mpaexception.h"

#include "id3v1tag.h"
#include "id3v2tag.h"
#include "lyrics3tag.h"
#include "apetag.h"

CTags::CTags(CMPAStream* pMPAStream, DWORD& dwBegin, DWORD& dwEnd)
{
	// all appended tags
	/////////////////////////////

	// look for ID3V1 Tag (always last tag)
	FindTag<CID3V1Tag>(pMPAStream, true, dwBegin, dwEnd);

	// look for prepended ID3V2 Tag (always before ID3V1 or last tag)
	FindTag<CID3V2Tag>(pMPAStream, true, dwBegin, dwEnd);

	// look for Lyrics3 Tag
	bool bLyricsTag = FindTag<CLyrics3Tag>(pMPAStream, true, dwBegin, dwEnd);
	
	// look for appended APE Tag
	FindTag<CAPETag>(pMPAStream, true, dwBegin, dwEnd);
	
    if (!bLyricsTag)
	{
		// look for Lyrics3 Tag (again, because order can differ)
		FindTag<CLyrics3Tag>(pMPAStream, true, dwBegin, dwEnd);
	}


	// all prepended tags
	/////////////////////////////

	// look for prepended ID3V2 Tag (always first tag)
	FindTag<CID3V2Tag>(pMPAStream, false, dwBegin, dwEnd);
		
	// look for appended APE Tag
	FindTag<CAPETag>(pMPAStream, false, dwBegin, dwEnd);
}

CTags::~CTags(void)
{
	for (int n=0; n<m_Tags.size(); n++)
	{
		delete m_Tags[n];
	}
}

template<typename Tag>
bool CTags::FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD& dwBegin, DWORD& dwEnd)
{
	try
	{
		Tag* pTag = Tag::FindTag(pMPAStream, bAppended, dwBegin, dwEnd);
		if (pTag)
		{
			if (bAppended)
				dwEnd = pTag->GetOffset();
			else
				dwBegin = pTag->GetEnd();
			m_Tags.push_back(pTag);
			return true;
		}
	}
	catch(CMPAException& Exc)
	{
		Exc.SetFileName(pMPAStream->GetFilename());
		Exc.ShowError();
	}
	return false;
}


CTag* CTags::GetNextTag(int& nIndex) const
{
	CTag* pTag;
	if (nIndex < m_Tags.size())
		pTag = m_Tags[nIndex];
	else
		pTag = NULL;

	nIndex++;
	return pTag;
}
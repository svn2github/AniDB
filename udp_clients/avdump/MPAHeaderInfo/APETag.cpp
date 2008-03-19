#include "StdAfx.h"
#include ".\apetag.h"

CAPETag* CAPETag::FindTag(CMPAStream* pMPAStream, bool bAppended, DWORD dwBegin, DWORD dwEnd)
{
	DWORD dwOffset;

	if (!bAppended)
	{
		// stands at the beginning of file (complete header is 32 bytes)
		dwOffset = dwBegin;
	}
	else
	{
		// stands at the end of the file (complete footer is 32 bytes)
		dwOffset = dwEnd - 32;		
	}
	BYTE* pBuffer = pMPAStream->ReadBytes(8, dwOffset, false);

	if (memcmp("APETAGEX", pBuffer, 8) == 0)
		return new CAPETag(pMPAStream, bAppended, dwOffset);
	return NULL;
}

CAPETag::CAPETag(CMPAStream* pMPAStream, bool bAppended, DWORD dwOffset) :
	CTag(pMPAStream, _T("APE"), bAppended, dwOffset)
{
	dwOffset += 8;
	DWORD dwVersion = pMPAStream->ReadLEValue(4, dwOffset);
	
	// first byte is version number
	m_fVersion = dwVersion/1000.0f;
	
	// get size
	m_dwSize = pMPAStream->ReadLEValue(4, dwOffset);

	m_dwNumItems = pMPAStream->ReadLEValue(4, dwOffset);

	// only valid for version 2
	DWORD dwFlags = pMPAStream->ReadLEValue(4, dwOffset);
	if (m_fVersion > 1.0f)
	{
		m_bHeader = dwFlags >> 31 & 0x1;
		m_bFooter = dwFlags >> 30 & 0x1;
	}
	else
	{
		m_bHeader = false;
		m_bFooter = true;
	}

	if (m_bHeader)
		m_dwSize += 32;	// add header

	if (bAppended)
		m_dwOffset -= (m_dwSize - 32);
}

CAPETag::~CAPETag(void)
{
}

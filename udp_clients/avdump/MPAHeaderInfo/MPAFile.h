#pragma once
#include "mpaexception.h"
#include "mpastream.h"
#include "mpaframe.h"
#include "tags.h"

class CMPAFile
{
public:
	CMPAFile(LPCTSTR szFile, DWORD dwBegin = 0, DWORD dwEnd = 0, HANDLE hFile = INVALID_HANDLE_VALUE);
	~CMPAFile(void);

	DWORD GetFileSize() const { return (m_dwEnd - m_dwBegin); };
	DWORD GetLengthSec() const { return (GetFileSize() / m_dwBytesPerSec); };
	
	enum GetType
	{
		First,
		Last,
		Next,
		Prev,
		Resync
	};

	CMPAFrame* GetFrame(GetType Type, CMPAFrame* pMPAFrame = NULL, bool bDeleteOldFrame = true, DWORD dwOffset = 0);
	
private:
	CMPAFrame* FindFrame(DWORD& dwOffset, bool bSubsequentFrame = true, bool bExactOffset = false, bool bReverse = false, CMPAHeader* pCompHeader = NULL);
	bool SkipEmptyFrames();

	
// order is important!!!!
private:
	static const DWORD m_dwMaxRange;
	static const DWORD m_dwTolerance;
	DWORD m_dwBytesPerSec;
	
public:
	DWORD m_dwBegin;	// beginning of first MPEG Audio frame
	DWORD m_dwEnd;		// end of last MPEG Audio frame (estimated)
	CMPAStream m_MPAStream;
	CTags m_Tags;					// contain list of present tags

	CMPAFrame* m_pMPAFrame;			// always first frame
	CVBRHeader* m_pVBRHeader;		// XING or VBRI or NULL
};

#pragma once
#include "mpastream.h"
#include "mpaheader.h"
#include "vbrheader.h"
#include "vbriheader.h"
#include "xingheader.h"

class CMPAFrame
{
public:
	CMPAFrame(CMPAStream* pMPAStream, DWORD dwOffset);
	~CMPAFrame(void);

	CVBRHeader* FindVBRHeader() const;
	
	DWORD GetSubsequentHeaderOffset() const { return m_dwOffset + m_dwFrameSize; };
	bool CheckCRC() const;
	
public:
	CMPAHeader* m_pMPAHeader;
	CMPAStream* m_pMPAStream;

	DWORD m_dwOffset;	// offset in bytes where frame begins
	DWORD m_dwFrameSize;// calculated frame size
	
private:
	static const DWORD m_dwProtectedBitsLayer2[5][2];
	static WORD CalcCRC16(BYTE* pBuffer, DWORD dwSize);
};

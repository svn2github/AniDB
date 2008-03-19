#include "stdafx.h"

#include <math.h>	// for ceil
#include "mpaframe.h"
#include "mpaexception.h"


// number of bits that are used for CRC check in MPEG 1 Layer 2
// (this table is invalid for Joint Stereo/Intensity Stereo)
// 1. index = allocation table index, 2. index = mono
const DWORD CMPAFrame::m_dwProtectedBitsLayer2[5][2] =
{
	{284,142},	// table a
	{308,154},	// table b
	{42,84},	// table c
	{62,114},	// table d
	{270,135}	// table for MPEG 2/2.5
};

CMPAFrame::CMPAFrame(CMPAStream* pMPAStream, DWORD dwOffset) :
	m_pMPAStream(pMPAStream),
	m_dwOffset(dwOffset)
{
	// decode header of frame at position dwOffset
	try
	{
		BYTE* pBuffer = m_pMPAStream->ReadBytes(MPA_HEADER_SIZE, dwOffset, false);
		m_pMPAHeader = new CMPAHeader(pBuffer);

		m_dwFrameSize = m_pMPAHeader->CalcFrameSize();
	}
	catch(CMPAException& Exc)
	{
		Exc.SetFileName(m_pMPAStream->GetFilename());
		// rethrow current exception
		throw;
	}
}

CVBRHeader* CMPAFrame::FindVBRHeader() const
{
	// read out VBR header (if available), must be freed with delete
	return CVBRHeader::FindHeader(this);
}


CMPAFrame::~CMPAFrame(void)
{
	delete m_pMPAHeader;
}


// returns true if CRC is correct otherwise false
// do only call this function, if header contains a CRC checksum
bool CMPAFrame::CheckCRC() const
{
	if (!m_pMPAHeader->m_bCRC)
		return false;

	DWORD dwProtectedBits;
	
	
	switch(m_pMPAHeader->m_Layer)
	{
		case CMPAHeader::Layer1:
			dwProtectedBits = 4* (m_pMPAHeader->GetNumChannels() * m_pMPAHeader->m_wBound + (32 - m_pMPAHeader->m_wBound));
			break;
		case CMPAHeader::Layer2:
			// no check for Layer II files (not easy to compute number protected bits, need to get allocation bits first)
			return true;
			break;
		// for Layer III the protected bits are the side information
		case CMPAHeader::Layer3:
			dwProtectedBits = m_pMPAHeader->GetSideInfoSize()*8;
		default:
			return true;
	}

	// CRC is also calculated from the last 2 bytes of the header
	dwProtectedBits += MPA_HEADER_SIZE*8 +16;	// +2 for CRC itself

	DWORD dwByteSize = (DWORD)ceil(dwProtectedBits/8.0); 
	
	// the first 2 bytes and the CRC itself are automatically skipped
	DWORD dwOffset = m_dwOffset;
	WORD wCRC16 = CalcCRC16(m_pMPAStream->ReadBytes(dwByteSize, dwOffset, false), dwProtectedBits);

	// read out crc from frame (it follows frame header)
	dwOffset += + MPA_HEADER_SIZE;
	if (wCRC16 == m_pMPAStream->ReadBEValue(2, dwOffset, false))
		return true;

	return false;
}

// CRC16 check
WORD CMPAFrame::CalcCRC16(BYTE* pBuffer, DWORD dwBitSize)
{
	DWORD n;
	WORD tmpchar, crcmask, tmpi;
	crcmask = tmpchar = 0;
	WORD crc = 0xffff;			// start with inverted value of 0

	// start with byte 2 of header
	for (n = 16;  n < dwBitSize;  n++)
	{
		if (n < 32 || n >= 48) // skip the 2 bytes of the crc itself
		{
			if (n%8 == 0)
			{
				crcmask = 1 << 8;
				tmpchar = pBuffer[n/8];
			}
			crcmask >>= 1;
			tmpi = crc & 0x8000;
			crc <<= 1;
			
			if (!tmpi ^ !(tmpchar & crcmask))
				crc ^= 0x8005;
		}
	}
	crc &= 0xffff;	// invert the result
	return crc;
}
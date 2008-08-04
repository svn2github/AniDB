//#include "stdafx.h"

#include "mpaexception.h"
#include ".\mpafile.h"


/// CMPAFile
//////////////////////////////////////////
// tolerance range, look at expected offset +/- m_dwTolerance/2 for beginning of a frame
const DWORD CMPAFile::m_dwTolerance = 6;	// +/-3 bytes

// max. range where to look for frame sync
const DWORD CMPAFile::m_dwMaxRange = 1024*1024;//16384; //145685 //176775 //1048576


CMPAFile::CMPAFile(LPCTSTR szFile, DWORD dwBegin, DWORD dwEnd, HANDLE hFile) :
	m_dwBegin(dwBegin), m_dwEnd(dwEnd), m_MPAStream(szFile, m_dwEnd, hFile), m_Tags(&m_MPAStream, m_dwBegin, m_dwEnd), m_pMPAFrame(NULL) 
{
	// find first valid MPEG frame
	m_pMPAFrame = FindFrame(m_dwBegin);

	// check for VBR header
	m_pVBRHeader = m_pMPAFrame->FindVBRHeader();
	
	if (m_pVBRHeader)
	{
		m_dwBytesPerSec = m_pMPAFrame->m_pMPAHeader->GetBytesPerSecond(m_pVBRHeader->m_dwFrames, m_pVBRHeader->m_dwBytes);
	}
	else
	{
		m_dwBytesPerSec = m_pMPAFrame->m_pMPAHeader->GetBytesPerSecond();
		// always skip empty (32kBit) frames
		SkipEmptyFrames();	
	}
}

// destructor
CMPAFile::~CMPAFile(void)
{
	if (m_pVBRHeader)
		delete m_pVBRHeader;

	if (m_pMPAFrame)
		delete m_pMPAFrame;
}

// look for a frame header close to position dwExpectedOffset
// if bExactOffset = true then look for frame at offset +/- tolerance, otherwise start looking at offset and go through file
// if bFindSubsequentFrame = true then look also for the subsequent frame
// bReverse gives the direction in which to look, when dwExpectedOffset is not the start of a frame
// if pCompHeader set, the returned header must resemble this header
// returns found header or otherwise throws an exception
CMPAFrame* CMPAFile::FindFrame(DWORD& dwOffset, bool bFindSubsequentFrame, bool bExactOffset, bool bReverse, CMPAHeader* pCompHeader)
{
	CMPAFrame* pMPAFrame;
	DWORD dwStep = 1;

	// is new offset within valid range?
	while(dwOffset >= m_dwBegin && dwOffset < (m_dwEnd - MPA_HEADER_SIZE))
	{
		pMPAFrame = NULL;
		try
		{
			BYTE* pBuffer = m_MPAStream.ReadBytes(2, dwOffset, false, bReverse);
			
			// sync bytes found?
			if ((pBuffer[0] == 0xFF) && ((pBuffer[1] & 0xE0) == 0xE0)) // first 11 bits should be 1
			{
				/*wchar_t buffer[1024];
				wsprintf(buffer, L"Frame found at %d\n", dwStep, m_dwMaxRange);
				MessageBox (NULL, buffer, _T("MPAFile Error"), MB_OK);*/
				//printf("Frame found at %d\n", dwOffset);

				// frame at position dwOffset
				pMPAFrame = new CMPAFrame(&m_MPAStream, dwOffset);

				if (pCompHeader)
				{
					// are the headers compatible?
					if (!(*pMPAFrame->m_pMPAHeader == *pCompHeader))
						throw CMPAException(CMPAException::IncompatibleHeader, m_MPAStream.GetFilename());
				}

				// do extended check?
				if (bFindSubsequentFrame)
				{	
					// calc offset for next frame header
					DWORD dwNewOffset = pMPAFrame->GetSubsequentHeaderOffset();
					if (dwNewOffset < (m_dwEnd - MPA_HEADER_SIZE))
					{
						CMPAFrame* pSubsequentFrame = FindFrame(dwNewOffset, false, true, false, pMPAFrame->m_pMPAHeader);
						delete pSubsequentFrame;
					}
					return pMPAFrame;			
					
				}
				else
					return pMPAFrame;
			}

		}
		catch(CMPAException& Exc)
		{	
			if (pMPAFrame)
				delete pMPAFrame;		
			// altough sync bytes are found, there occured an error
			switch(Exc.GetErrorID())
			{
				
				case CMPAException::HeaderCorrupt:
				case CMPAException::IncompatibleHeader:
				case CMPAException::NoFrameInTolerance:
				case CMPAException::NoFrame:
					break;
				/*default:
					throw;	// just rethrow in case of any other error*/

			}
			
		}

		if (!bExactOffset)
		{	
			// just go forward/backward to find sync
			dwOffset += (bReverse?-1:+1);
			
			// check only within dwMaxRange
			if (dwStep > m_dwMaxRange){
				// out of tolerance range
				throw CMPAException(CMPAException::NoFrameInTolerance, m_MPAStream.GetFilename());	
			}
			dwStep++;
		}
		else
		{
			if (dwStep > m_dwTolerance)
			{
				// out of tolerance range
				throw CMPAException(CMPAException::NoFrameInTolerance, m_MPAStream.GetFilename());
			}
			
			// look around dwExpectedOffset with increasing steps (+1,-2,+3,-4,...)
			if (dwStep%2 == 1)
				dwOffset += dwStep;
			else
				dwOffset -= dwStep;
			dwStep++;			
		}
	}	

	// out of tolerance range
	throw CMPAException(CMPAException::NoFrame, m_MPAStream.GetFilename());
}

#define MAX_EMPTY_FRAMES 25

// skips first 32kbit/s or lower bitrate frames to estimate bitrate (returns true if bitrate is variable)
bool CMPAFile::SkipEmptyFrames()
{
	if (m_pMPAFrame->m_pMPAHeader->m_dwBitrate > 32000)
		return false;

	CMPAFrame* pMPAFrame = GetFrame(First);
	DWORD dwFrames = 0;

	try
	{
		while (pMPAFrame && pMPAFrame->m_pMPAHeader->m_dwBitrate <= 48000)
		{
			pMPAFrame = GetFrame(Next, pMPAFrame);
			if (dwFrames++ > MAX_EMPTY_FRAMES)	
				break;
		};
	}
	catch(CMPAException& /*Exc*/) // just catch the exception and return false
	{
		return false;
	}
	m_dwBytesPerSec = pMPAFrame->m_pMPAHeader->GetBytesPerSecond();
	delete pMPAFrame;
	return true;
}

#define MIN_FRAME_SIZE 24	// MPEG2, LayerIII, 8kbps, 24kHz => Framesize = 24 Bytes
CMPAFrame* CMPAFile::GetFrame(CMPAFile::GetType Type, CMPAFrame* pMPAFrame, bool bDeleteOldFrame, DWORD dwOffset)
{
	CMPAFrame* pNewMPAFrame;
	CMPAHeader* pCompHeader = NULL;
	bool bSubsequentFrame = false;
	bool bReverse = false;
	bool bExactOffset = true;
	
	switch(Type)
	{
		case First:
			dwOffset = m_dwBegin;
			bSubsequentFrame = true;
			bExactOffset = false;
			break;
		case Last:
			dwOffset = m_dwEnd - MIN_FRAME_SIZE;
			bReverse = true;
			bExactOffset = false;
			pCompHeader = m_pMPAFrame->m_pMPAHeader;
			break;
		case Next:
			if (!pMPAFrame)
				return NULL;
			pCompHeader = m_pMPAFrame->m_pMPAHeader;
			dwOffset = pMPAFrame->GetSubsequentHeaderOffset();
			break;
		case Prev:
			if (!pMPAFrame)
				return NULL;
			dwOffset = pMPAFrame->m_dwOffset-MIN_FRAME_SIZE;
			bReverse = true;
			bExactOffset = false;
			pCompHeader = m_pMPAFrame->m_pMPAHeader;
			break;
		case Resync:
			bSubsequentFrame = true;
			bExactOffset = false;
			//pCompHeader = m_pMPAFrame->m_pMPAHeader;
			break;
		default:
			return NULL;
	}

	try
	{
		pNewMPAFrame = FindFrame(dwOffset, bSubsequentFrame, bExactOffset, bReverse, pCompHeader);
		
		if (pMPAFrame && bDeleteOldFrame)
			delete pMPAFrame;
	}
	catch(...)
	{
		// try a complete resync from position dwOffset
		if (Type == Next)
		{
			return GetFrame(Resync, pMPAFrame, bDeleteOldFrame, dwOffset);
		}
		
		return NULL;
	}


	return pNewMPAFrame;
}


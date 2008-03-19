#include "stdafx.h"

#include "mpastream.h"
#include "mpaexception.h"

#include <windows.h>	// for CreateFile, CloseHandle, ...

// 1KB is inital buffersize
const DWORD CMPAStream::m_dwInitBufferSize = 1024;	

// constructor
// either open file szFilename or use file with hande hFile
CMPAStream::CMPAStream(LPCTSTR szFilename, DWORD& dwEnd, HANDLE hFile) : 
	m_hFile(hFile), m_bMustReleaseFile(false), m_pBuffer(NULL), m_dwBufferSize(m_dwInitBufferSize), m_dwOffset(0)
{
	// open file, if not already done
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		// open with CreateFile (no limitation of 128byte filename length, like in mmioOpen)
		m_hFile = ::CreateFile(szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, 
										    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (m_hFile == INVALID_HANDLE_VALUE)
		{
			// throw error
			throw CMPAException(CMPAException::ErrOpenFile, szFilename, _T("CreateFile"), true);
		}
		m_bMustReleaseFile = true;
	}

	// save filename
	m_szFile = _tcsdup(szFilename);

	// fill buffer for first time
	m_pBuffer = new BYTE[m_dwBufferSize];
	FillBuffer(m_dwOffset, m_dwBufferSize, false);

	// get end of file
	if (dwEnd == 0)
		dwEnd = GetSize();
}

CMPAStream::~CMPAStream(void)
{
	if (m_pBuffer)
		delete[] m_pBuffer;
	
	// close file
	if (m_bMustReleaseFile)
		::CloseHandle(m_hFile);	

	if (m_szFile)
		free((void*)m_szFile);
}

// set file position
void CMPAStream::SetPosition(LONGLONG lOffset, DWORD dwMoveMethod) const
{
	LARGE_INTEGER liOff;

	liOff.QuadPart = lOffset;
	liOff.LowPart = ::SetFilePointer(m_hFile, liOff.LowPart, &liOff.HighPart, dwMoveMethod); 
	if (liOff.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
	{ 
		// != NO_ERROR
		// throw error
		throw CMPAException(CMPAException::ErrSetPosition, m_szFile, _T("SetFilePointer"), true);
	}
}

// read from file, return number of bytes read
DWORD CMPAStream::Read(LPVOID pData, DWORD dwOffset, DWORD dwSize) const
{
	DWORD dwBytesRead = 0;
	
	// set position first
	SetPosition(dwOffset, FILE_BEGIN);

	if (!::ReadFile(m_hFile, pData, dwSize, &dwBytesRead, NULL))
		throw CMPAException(CMPAException::ErrReadFile, m_szFile, _T("ReadFile"), true);
	
	return dwBytesRead;
}

DWORD CMPAStream::GetSize() const
{
	DWORD dwSize = ::GetFileSize(m_hFile, NULL);
	if (dwSize == INVALID_FILE_SIZE)
		throw CMPAException(CMPAException::ErrReadFile, m_szFile, _T("GetFileSize"), true);
	return dwSize;
}

BYTE* CMPAStream::ReadBytes(DWORD dwSize, DWORD& dwOffset, bool bMoveOffset, bool bReverse) const
{
	// enough bytes in buffer, otherwise read from file
	if (m_dwOffset > dwOffset || ( ((int)((m_dwOffset + m_dwBufferSize) - dwOffset)) < (int)dwSize))
		FillBuffer(dwOffset, dwSize, bReverse);

	BYTE* pBuffer = m_pBuffer + (dwOffset-m_dwOffset);
	if (bMoveOffset)
		dwOffset += dwSize;
	
	return pBuffer;
}

DWORD CMPAStream::ReadLEValue(DWORD dwNumBytes, DWORD& dwOffset, bool bMoveOffset) const
{
	_ASSERTE(dwNumBytes > 0);
	_ASSERTE(dwNumBytes <= 4);	// max 4 byte

	BYTE* pBuffer = ReadBytes(dwNumBytes, dwOffset, bMoveOffset);

	DWORD dwResult = 0;

	// little endian extract (least significant byte first) (will work on little and big-endian computers)
	DWORD dwNumByteShifts = 0;

	for (DWORD n=0; n < dwNumBytes; n++)
	{
		dwResult |= pBuffer[n] << 8 * dwNumByteShifts++;                                                          
	}
	
	return dwResult;
}

// convert from big endian to native format (Intel=little endian) and return as DWORD (32bit)
DWORD CMPAStream::ReadBEValue(DWORD dwNumBytes, DWORD& dwOffset,  bool bMoveOffset) const
{	
	_ASSERTE(dwNumBytes > 0);
	_ASSERTE(dwNumBytes <= 4);	// max 4 byte

	BYTE* pBuffer = ReadBytes(dwNumBytes, dwOffset, bMoveOffset);

	DWORD dwResult = 0;

	// big endian extract (most significant byte first) (will work on little and big-endian computers)
	DWORD dwNumByteShifts = dwNumBytes - 1;

	for (DWORD n=0; n < dwNumBytes; n++)
	{
		dwResult |= pBuffer[n] << 8*dwNumByteShifts--; // the bit shift will do the correct byte order for you                                                           
	}
	
	return dwResult;
}

// throws exception if not possible
void CMPAStream::FillBuffer(DWORD dwOffset, DWORD dwSize, bool bReverse) const
{
	// calc new buffer size
	if (dwSize <= m_dwInitBufferSize)
		m_dwBufferSize = m_dwInitBufferSize;
	else
	{
        m_dwBufferSize = dwSize;
		
		// release old buffer 
		delete[] m_pBuffer;

		// reserve new buffer
		m_pBuffer = new BYTE[m_dwBufferSize];
	}	

	if (bReverse)
	{
		if (dwOffset + dwSize < m_dwBufferSize)
			dwOffset = 0;
		else
			dwOffset = dwOffset + dwSize - m_dwBufferSize;
	}

	// read <m_dwBufferSize> bytes from offset <dwOffset>
	m_dwBufferSize = Read(m_pBuffer, dwOffset, m_dwBufferSize);

	// set new offset
	m_dwOffset = dwOffset;
}
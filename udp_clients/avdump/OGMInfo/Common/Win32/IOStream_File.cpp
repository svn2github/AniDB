/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Specialisation of I/O stream base class for file accesses (Win32)
 *
 * Copyright (C) 2003 Julien Coloos.  All rights reserved.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE.QPL included in the packaging of this file.
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 *
 * Licensees holding an other license may use this file in accordance with 
 * the Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See http://www.opensource.org/licenses/qtpl.php for QPL licensing information.
 * See http://www.opensource.org/licenses/gpl-license.html for GPL licensing information.
 *
 * Contact cyrius@corecodec.org if any conditions of this licensing
 * are not clear to you.
 *
 ********************************************************************/

#include "../common.h"

#if defined(WIN32)

#include "../IOStream_File.h"
#include "../IOStream_AccessLocker.h"
#include "common_Win32.h"
#include <windows.h>


using namespace nsFile;

IOFile::IOFile(ToolSystem& toolSystem)
: IOStream(toolSystem, NULL)
, m_hFile(INVALID_HANDLE_VALUE)
{
}

IOFile::IOFile(ToolSystem& toolSystem, const char *szFile, uint32 iFlags, bool bUseAccessLock)
: IOStream(toolSystem, NULL, iFlags)
, m_hFile(INVALID_HANDLE_VALUE)
{
	Open(szFile, iFlags, bUseAccessLock);
}

IOFile::IOFile(ToolSystem& toolSystem, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock)
: IOStream(toolSystem, NULL, iFlags)
, m_hFile(INVALID_HANDLE_VALUE)
{
	Open(wszFile, iFlags, bUseAccessLock);
}

bool IOFile::_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock) {
	Close();

	std::string fileName;
	if(wszFile && !g_bUseUnicodeWinAPI) {
		fileName = TextWToA(wszFile, -1);
		szFile = fileName.c_str();
	}

	m_iFlags = iFlags;

	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	std::wstring strMode(L"");
	if(iFlags & kRead)
		strMode = L"READ";
	if(iFlags & kWrite) {
		if(strMode.length())
			strMode += L'&';
		strMode += L"WRITE";
	}
	
	if(iFlags & kRead)		dwDesiredAccess |= GENERIC_READ;
	if(iFlags & kWrite)		dwDesiredAccess |= GENERIC_WRITE;

	if(!(iFlags & kDenyRead))	dwShareMode |= FILE_SHARE_READ;
	if(!(iFlags & kDenyWrite))	dwShareMode |= FILE_SHARE_WRITE;

	switch(iFlags & kCreationMask) {
	case kCreateNew:
		dwCreationDisposition = CREATE_NEW;
		break;
	case kCreateAlways:
		dwCreationDisposition = CREATE_ALWAYS;
		break;
	case kOpenExisting:
		dwCreationDisposition = OPEN_EXISTING;
		break;
	case kOpenAlways:
		dwCreationDisposition = OPEN_ALWAYS;
		break;
	case kTruncateExisting:
		dwCreationDisposition = TRUNCATE_EXISTING;
		break;
	default:
		assert(false);
		break;
	}

	if(iFlags & kSequential)		dwFlagsAndAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
	if(iFlags & kRandomAccess)		dwFlagsAndAttributes |= FILE_FLAG_RANDOM_ACCESS;
	if(iFlags & kWriteThrough)		dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
	if(iFlags & kUnbuffered)		dwFlagsAndAttributes |= FILE_FLAG_NO_BUFFERING;

	if(szFile) {
		m_Filename = TextAToW(szFile, -1);
		char szPath[MAX_PATH];
		LPSTR lpFilePart = NULL;
		DWORD dwSize;
		m_Name = L"[";
		if((dwSize=GetFullPathNameA(szFile, MAX_PATH, szPath, &lpFilePart)) && (dwSize<MAX_PATH) && lpFilePart)
			m_Name += TextAToW(lpFilePart, -1);
		else
			m_Name += m_Filename;
		m_Name += L"]";
		DEBUG_MESSAGE(m_ToolSystem, kTrace_Action, MSG_INFO, kMsg, L"Opening [%ls] (%ls mode)\n", m_Filename.c_str(), strMode.c_str());
		m_hFile = CreateFileA(szFile, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
		if(m_hFile == INVALID_HANDLE_VALUE)
			PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while opening %ls : %%ls\n"
				, getName());
		else if(bUseAccessLock) {
			m_pLocker = IOStream_AccessLocker::getLocker(m_ToolSystem, m_hFile);
			DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, MSG_INFO, kMsg, L"%ls's locker pointer : %08X\n", getName(), m_pLocker);
		}
	} else if(wszFile) {
		m_Filename = wszFile;
		wchar_t szPath[MAX_PATH];
		LPWSTR lpFilePart = NULL;
		DWORD dwSize;
		m_Name = L"[";
		if((dwSize=GetFullPathNameW(wszFile, MAX_PATH, szPath, &lpFilePart)) && (dwSize<MAX_PATH) && lpFilePart)
			m_Name += lpFilePart;
		else
			m_Name += m_Filename;
		m_Name += L"]";
		DEBUG_MESSAGE(m_ToolSystem, kTrace_Action, MSG_INFO, kMsg, L"Opening [%ls] (%ls mode)\n", m_Filename.c_str(), strMode.c_str());
		m_hFile = CreateFileW(wszFile, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
		if(m_hFile == INVALID_HANDLE_VALUE)
			PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while opening %ls : %%ls\n"
				, getName());
		else if(bUseAccessLock) {
			m_pLocker = IOStream_AccessLocker::getLocker(m_ToolSystem, m_hFile);
			DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, MSG_INFO, kMsg, L"%ls's locker pointer : %08X\n", getName(), m_pLocker);
		}
	} else
		DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, MSG_INFO, kMsg, L"No file name given\n");

	return IsOpened();
}

bool IOFile::IsOpened(void) {
	return (m_hFile != INVALID_HANDLE_VALUE);
}

void IOFile::SetEOF(void) {
	if(!SetEndOfFile(m_hFile))
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while setting end of file for %ls : %%ls\n"
			, getName());
}

void IOFile::Close(void) {
	DEBUG_MESSAGE(m_ToolSystem, kTrace_Action, MSG_INFO, kMsg, L"Closing [%ls]\n", m_Filename.c_str());
	if(IsOpened()) {
		if(!CloseHandle(m_hFile))
			PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while closing %ls : %%ls\n"
				, getName());
		m_hFile = INVALID_HANDLE_VALUE;
	} else
		DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, MSG_INFO, kMsg, L"[%ls] is already closed.\n", m_Filename.c_str());
}

sint64 IOFile::GetPosition(void) {
	long iHigh = 0;
	DWORD iLow = SetFilePointer(m_hFile, 0, &iHigh, FILE_CURRENT);
	if ( (iLow==INVALID_SET_FILE_POINTER) && (GetLastError()!=NO_ERROR) ) {
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while getting position in %ls : %%ls\n"
			, getName());
		return -1;
	}
	return ((uint64(iHigh)<<32) | iLow);
}

sint64 IOFile::GetSize(void) {
	DWORD iLow, iHigh;

	iLow = GetFileSize(m_hFile, &iHigh);

	if(iLow == INVALID_FILE_SIZE && (GetLastError()!=NO_ERROR) ) {
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while getting size of %ls : %%ls\n"
			, getName());
		return -1;
	}

	return ((uint64(iHigh)<<32) | iLow);
}

sint32 IOFile::_Read(void *pBuffer, sint32 iBytes) {
	DWORD dwRead = 0;
	if(!ReadFile(m_hFile, pBuffer, iBytes, &dwRead, NULL)) {
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while reading %ld bytes from %ls : %%ls\n"
			, iBytes, getName());
		return -1;
	}
	return dwRead;
}

sint32 IOFile::_Write(void *pBuffer, sint32 iBytes) {
	DWORD dwWritten = 0;
	if(!WriteFile(m_hFile, pBuffer, iBytes, &dwWritten, NULL) && !m_bOutputSystem)
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while reading %ld bytes from %ls : %%ls\n"
			, iBytes, getName());
	return dwWritten;
}

sint64 IOFile::Seek(sint64 iOffset, int mode) {
	//DEBUG_MESSAGE(m_ToolSystem, kTrace_Action, MSG_INFO, kMsg, "Seeking in [%ls] (offset: %lld, from %ls).\n"
	//	, m_Filename.c_str(), iOffset
	//	, (mode==SEEK_SET ? "beginning" : mode==SEEK_END ? "end" : "current position"));
	DWORD dwMethod;
	switch(mode) {
	case SEEK_SET:
		dwMethod=FILE_BEGIN;
		break;
	case SEEK_CUR:
		dwMethod=FILE_CURRENT;
		break;
	case SEEK_END:
		dwMethod=FILE_END;
		break;
	default:
		assert(false);
		break;
	}

	LONG iHigh = LONG(iOffset>>32);
	DWORD iLow = SetFilePointer(m_hFile, LONG(iOffset & 0xFFFFFFFF), &iHigh, dwMethod);
	if( (iLow==INVALID_SET_FILE_POINTER) && (GetLastError()!=NO_ERROR) ) {
		PrintWin32Message(m_ToolSystem, kMsgError, GetLastError(), L"Error while seeking in %ls (offset: %lld, from %ls) : %%ls\n"
			, getName(), iOffset
			, (mode==SEEK_SET ? L"beginning" : mode==SEEK_END ? L"end" : L"current position"));
		return -1;
	}
	return ((uint64(iHigh)<<32) | iLow);
}

//////////////////////////////////////////////////////////////////////

void IOFastWriteFile::CreateBuffer(sint32 iNbBlocks, sint32 iBlockSize) {
	DestroyBuffer();

	// iBlockSize *must* be a multiple of the disk sector size.
	//
	// There isn't a reliable way to determine the disk sector size,
	// because GetDiskFreeSpace() lies for disks >4Gb (FAT32). So
	// we're just going to assume a 4K sector size.

	m_iSectorSize = 4096;
	
	iBlockSize = (iBlockSize + m_iSectorSize - 1) & (-m_iSectorSize);

	m_iBlockSize = iBlockSize;

	//assert(m_iBlockSize);

	m_iBufferSize = iNbBlocks * m_iBlockSize;

	// Allocate the buffer. VirtualAlloc() guarantees that the buffer will
	// be aligned to the nearest page boundary, which is 4K or 4Mb on Intel
	// CPUs.

	m_pBuffer = VirtualAlloc(NULL, m_iBufferSize, MEM_COMMIT, PAGE_READWRITE);

	assert(m_pBuffer);
}

void IOFastWriteFile::DestroyBuffer(void) {
	if(m_pBuffer) {
		VirtualFree(m_pBuffer, 0, MEM_RELEASE);
		m_pBuffer = NULL;
		m_iBufferSize = 0;
	}
}

//////////////////////////////////////////////////////////////////////

void IOFastReadFile::CreateBuffer(sint32 iNbBlocks, sint32 iBlockSize) {
	DestroyBuffer();

	// iBlockSize *must* be a multiple of the disk sector size.
	//
	// There isn't a reliable way to determine the disk sector size,
	// because GetDiskFreeSpace() lies for disks >4Gb (FAT32). So
	// we're just going to assume a 4K sector size.

	m_iSectorSize = 4096;
	m_iNbBlocks = iNbBlocks;

	iBlockSize = (iBlockSize + m_iSectorSize - 1) & (-m_iSectorSize);

	m_iBlockSize = iBlockSize;

	//assert(m_iBlockSize);

	m_iBufferSize = iNbBlocks * m_iBlockSize;

	// Allocate the buffer. VirtualAlloc() guarantees that the buffer will
	// be aligned to the nearest page boundary, which is 4K or 4Mb on Intel
	// CPUs.

	m_pBuffer = VirtualAlloc(NULL, m_iBufferSize, MEM_COMMIT, PAGE_READWRITE);

	assert(m_pBuffer);

	m_BlockInfo = new IOFastReadFile_Info[m_iNbBlocks];
	Flush();

	assert(m_BlockInfo);
}

void IOFastReadFile::DestroyBuffer(void) {
	if(m_pBuffer) {
		VirtualFree(m_pBuffer, 0, MEM_RELEASE);
		m_pBuffer = NULL;
		m_iBufferSize = 0;
	}
	if(m_BlockInfo) {
		delete[] m_BlockInfo;
		m_BlockInfo = NULL;
	}
}

#endif /* Win32 */

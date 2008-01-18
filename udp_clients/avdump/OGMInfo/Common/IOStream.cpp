/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * I/O stream base class
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

#include "common_endian.h"
#include "IOStream.h"
#include "IOStream_AccessLocker.h"
#include <vector>


IOStream::IOStream(ToolSystem& toolSystem, IOStream_AccessLocker *pLocker, uint32 iFlags, bool bOutputSystem)
: m_ToolSystem(toolSystem)
, m_iFlags(iFlags)
, m_bOutputSystem(bOutputSystem)
, m_pLocker(pLocker)
{
	DEBUG_MESSAGE(m_ToolSystem, kTrace_Tors, MSG_INFO, kMsg, L"ctor\n");
}

IOStream::~IOStream(void) {
	DEBUG_MESSAGE(m_ToolSystem, kTrace_Tors, MSG_INFO, kMsg, L"dtor\n");

	Close();
}

void IOStream::setOutputSystem(bool bOutputSystem) {
	m_bOutputSystem = bOutputSystem;
}

const wchar_t *IOStream::getName(void) {
	return m_Name.c_str();
}

uint32 IOStream::getFlags(void) {
	return m_iFlags;
}

IOStream_AccessLocker *IOStream::getAccessLocker(void) {
	return m_pLocker;
}

bool IOStream::IsOpened(void) {
	return false;
}

IOStream *IOStream::Clone(void) {
	return Clone(getFlags(), !!getAccessLocker());
}

void IOStream::Close(void) {
}

bool IOStream::SynchOn(const uint8 *pSynchValue, const uint8 *pSynchMask, sint16 iSynchLength, sint32 iMaxSearch, sint32& iSkippedBytes) {
	sint64 iStartPosition = GetPosition();
	sint32 iBytesRead;
	std::vector<uint8> buffer;
	// Reserve enough space for backward seeking (without calling Seek)
	// First part of the buffer contains the previously read bytes
	// Second part of the buffer contains the newly read bytes
	buffer.resize(2 * iSynchLength);
	uint8 *pBuffer = &buffer[iSynchLength];
	sint16 iSynchBytes = 0;	// number of bytes in synch
	sint16 iBytesRemaining;	// number of bytes remaining
	uint8 *pBufferRead;		// current position in the buffer
	iSkippedBytes = 0;
	do {
		// If we reached the end of the stream (no more data to read), exit
		if((iBytesRead=Read(pBuffer, iSynchLength)) <= 0) {
			Seek(iStartPosition, SEEK_SET);
			return false;
		}
		// Reset the buffer vars for keeping on searching
		pBufferRead = pBuffer;
		iBytesRemaining = iBytesRead;
		do {
			while(((*pBufferRead) & pSynchMask[iSynchBytes]) == pSynchValue[iSynchBytes]) {
				pBufferRead++;
				iBytesRemaining--;
				iSynchBytes++;
				// If we found the synch, exit :D
				if(iSynchBytes >= iSynchLength) {
					// Better seek back to the beginning of the synch :P
					if(Seek(-iBytesRemaining-iSynchLength, SEEK_CUR) != -1)
						return true;
					return false;
				}
				if(!iBytesRemaining)
					break;
			}
			if(iBytesRemaining) {
				// Not (anymore) in synch
				if(iSynchBytes) {
					// We had some bytes in synch up to now
					// Seek backward in our buffer to start again the byte
					// after the previous 'false' synch
					pBufferRead -= iSynchBytes - 1;
					iBytesRemaining += iSynchBytes - 1;
				} else {
					// There was no bytes in synch up to now
					// Just go forward in the buffer
					pBufferRead++;
					iBytesRemaining--;
				}
				// Reset the synch we (may have) got
				iSynchBytes = 0;
				// In any case we skipped only one byte
				iSkippedBytes++;
				// If we exceeded the search extent, exit
				if((iMaxSearch>0) && (iSkippedBytes>=iMaxSearch)) {
					Seek(iStartPosition, SEEK_SET);
					return false;
				}
			} else {
				// No more data in the buffer, synch is OK up to now
				// Store the useful synch bytes in the first part of the buffer
				// (the first synch byte is useless here)
				if(iSynchBytes > 1) {
					sint16 iDelta = iSynchBytes - 1;
					memcpy(pBuffer - iDelta, pBufferRead - iDelta, iDelta);
				}
				break;
			}
		} while(true);
	} while(true);
}

sint32 IOStream::Read(void *pBuffer, sint32 iBytes) {
	if(m_pLocker) {
		sint32 iResult;

		m_pLocker->Lock();

		try {
			iResult = _Read(pBuffer, iBytes);
		} catch(...) {

			m_pLocker->Unlock();

			throw;
		}

		m_pLocker->Unlock();

		return iResult;
	} else
		return _Read(pBuffer, iBytes);
}

uint8 IOStream::Read_uint8(bool *pSuccess) {
	uint8 iValue;

	if(Read(&iValue, 1) != 1) {
		if(pSuccess)
			*pSuccess = false;

		return 0;
	}

	if(pSuccess)
		*pSuccess = true;

	return iValue;
}

#define DEFINE_FUNCTION_IOSTREAM_READ_ENDIAN(bits)							\
uint##bits IOStream::Read_uint##bits(eEndianness endian, bool *pSuccess) {	\
	uint##bits iValue;														\
																			\
	if(Read(&iValue, bits/8) != bits/8) {									\
		if(pSuccess)														\
			*pSuccess = false;												\
																			\
		return 0;															\
	}																		\
																			\
	switch(endian) {														\
	case kBigEndian:														\
		FIX_INT##bits##_FROM_BE(iValue);									\
		break;																\
	case kLittleEndian:														\
		FIX_INT##bits##_FROM_LE(iValue);									\
		break;																\
	}																		\
																			\
	if(pSuccess)															\
		*pSuccess = true;													\
																			\
	return iValue;															\
}

DEFINE_FUNCTION_IOSTREAM_READ_ENDIAN(16)
DEFINE_FUNCTION_IOSTREAM_READ_ENDIAN(32)
DEFINE_FUNCTION_IOSTREAM_READ_ENDIAN(64)

sint32 IOStream::Write(void *pBuffer, sint32 iBytes) {
	if(m_pLocker) {
		sint32 iResult;

		m_pLocker->Lock();

		try {
			iResult = _Write(pBuffer, iBytes);
		} catch(...) {

			m_pLocker->Unlock();

			throw;
		}

		m_pLocker->Unlock();

		return iResult;
	} else
		return _Write(pBuffer, iBytes);
}

bool IOStream::Write_uint8(uint8 iValue) {
	return (Write(&iValue, 1) == 1);
}

#define DEFINE_FUNCTION_IOSTREAM_WRITE_ENDIAN(bits)							\
bool IOStream::Write_uint##bits(uint##bits iValue, eEndianness endian) {	\
	switch(endian) {														\
	case kBigEndian:														\
		FIX_INT##bits##_TO_BE(iValue);										\
		break;																\
	case kLittleEndian:														\
		FIX_INT##bits##_TO_LE(iValue);										\
		break;																\
	}																		\
																			\
	return (Write(&iValue, bits/8) == bits/8);								\
}

DEFINE_FUNCTION_IOSTREAM_WRITE_ENDIAN(16)
DEFINE_FUNCTION_IOSTREAM_WRITE_ENDIAN(32)
DEFINE_FUNCTION_IOSTREAM_WRITE_ENDIAN(64)

//////////////////////////////////////////////////////////////////////

IONull::IONull(ToolSystem& toolSystem)
: IOStream(toolSystem, NULL, false)
, m_iPosition(0)
, m_iCurrentSize(0)
{
	m_Name = L"[IONull]";
}

IOStream *IONull::Clone(uint32 iFlags, bool bUseAccessLock) {
	IONull *pIO = new IONull(m_ToolSystem);

	pIO->Seek(GetSize(), SEEK_SET);
	pIO->SetEOF();
	pIO->Seek(GetPosition(), SEEK_SET);

	return pIO;
}

bool IONull::ChangeFlags(uint32 iFlags) {
	m_iFlags = iFlags;
	return true;
}

sint32 IONull::_Read(void *pBuffer, sint32 iBytes) {
	return 0;
}

sint32 IONull::_Write(void *pBuffer, sint32 iBytes) {
	if(iBytes > 0)
		m_iPosition += iBytes;
	if(m_iPosition > m_iCurrentSize)
		m_iCurrentSize = m_iPosition;
	return iBytes;
}

bool IONull::IsOpened(void) {
	return true;
}

void IONull::SetEOF(void) {
	m_iCurrentSize = m_iPosition;
}

sint64 IONull::GetPosition(void) {
	return m_iPosition;
}

sint64 IONull::GetSize(void) {
	return m_iCurrentSize;
}

sint64 IONull::Seek(sint64 iOffset, int mode) {
	switch(mode) {
	case SEEK_SET:
		m_iPosition = iOffset;
		break;
	case SEEK_CUR:
		m_iPosition += iOffset;
		break;
	case SEEK_END:
		m_iPosition = m_iCurrentSize + iOffset;
		break;
	}
	if(m_iPosition < 0)
		m_iPosition = 0;

	return m_iPosition;
}

//////////////////////////////////////////////////////////////////////

TOOLS_DLL_API void IOStreamPrintW(void *pPrivate, eMsgType type, const wchar_t *szString) {
	((IOStream *)pPrivate)->Write((void *)szString, wcslen(szString) * sizeof(wchar_t));
}

TOOLS_DLL_API void IOStreamPrintA(void *pPrivate, eMsgType type, const wchar_t *szString) {
	std::string str = TextWToA(szString, -1);
	((IOStream *)pPrivate)->Write((void *)str.c_str(), str.length());
}

TOOLS_DLL_API void WriteBOM(IOStream& output) {
	assert(output.IsOpened());

	if(output.IsOpened()) {
		assert(!output.GetPosition());

		if(sizeof(wchar_t) > 1) {
			wchar_t iBOM = 0x0000FEFF;
			output.Write(&iBOM, sizeof(wchar_t));
		}
	}
}

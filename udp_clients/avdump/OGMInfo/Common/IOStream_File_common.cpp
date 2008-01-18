/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Specialisation of I/O stream base class for file accesses
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

#include "IOStream_File.h"
#include "common.h"
#include "IOStream_AccessLocker.h"
#include "thread.h"


using namespace nsFile;

IOFile::~IOFile(void) {
	Close();
}

const wchar_t *IOFile::getFilename(void) {
	return m_Filename.c_str();
}

IOStream *IOFile::Clone(uint32 iFlags, bool bUseAccessLock) {
	IOStream *pIO = new IOFile(m_ToolSystem, m_Filename.c_str(), (iFlags ? iFlags : m_iFlags), bUseAccessLock);
	if(!pIO)
		return NULL;

	if(!pIO->IsOpened()) {
		delete pIO;
		return NULL;
	}

	sint64 iPosition = GetPosition();
	if(iPosition > 0)
		pIO->Seek(iPosition, SEEK_SET);

	return pIO;
}

bool IOFile::Open(const char *szFile, uint32 iFlags, bool bUseAccessLock) {
	return _Open(szFile, NULL, iFlags, bUseAccessLock);
}

bool IOFile::Open(const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock) {
	return _Open(NULL, wszFile, iFlags, bUseAccessLock);
}

bool IOFile::ChangeFlags(uint32 iFlags) {
	if(!IsOpened())
		return false;

	if(m_iFlags == iFlags)
		return true;

	std::wstring filename = m_Filename;

	return Open(filename.c_str(), iFlags, !!m_pLocker);
}

//////////////////////////////////////////////////////////////////////

/*
 * 'Fast' I/O file classes.
 * Inspired by those of VirtualDub (C) 2003 Avery Lee.
 *
 * Principle:
 *  x 'Write' class
 *    The class pre-allocate a set (user defined) of buffers of a specific
 *    (user defined) size. The class open the specified file in write mode
 *    2 times: one in a 'standard' way, another by disabling any caching
 *    done by the OS.
 *    The unbuffered access needs (at least under Win32) its operation to
 *    work on multiple of the sector size. Thus, if necessary, the size of
 *    the buffers used is extended to fit this constraint
 *    When the user write data through this class, those data are temporarily
 *    stored in the allocated buffers, and flushed to the disk when necessary.
 *    Generally the flushing is done when all the buffers are full, or can
 *    be done by a thread running in the background (if the user asked for it).
 *    When operations on non multiple sizes of the sector size are needed, the
 *    other 'normal' file access is used, and the necessary things are done to
 *    keep the unbuffered access method consistant.
 *
 *  x 'Read' class
 *    The class pre-allocate a set (user defined) of buffers of a specific
 *    (user defined) size.
 *    Here the user can choose the way (buffered/unbuffered, ...) the file
 *    is to be opened. Thus, if necessary, the size of the buffers used is
 *    extended to fit this constraint (sector size).
 *    When the user read data through this class, the class first see if
 *    those data are already in one of the buffers. If this is the case
 *    the data are returned immediatly.
 *    If the data aren't buffered yet, the class will do the necessary to
 *    buffer them. To do so it will first choose the most appropriate
 *    buffer (i.e. the less accessed one recently) to use.
 */

IOFastWriteFile::IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread)
: IOFile(toolSystem)
, m_BufferedIO(toolSystem)
#if defined(WIN32)
, m_pUnbufferedIO(&m_BufferedIO)
#endif
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
, m_bError(false)
, m_bUseThread(bUseThread)
, m_bEndThread(true)
{
	CreateBuffer(iNbBlocks, iBlockSize);
}

IOFastWriteFile::IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread, const char *szFile, bool bUseAccessLock)
: IOFile(toolSystem)
, m_BufferedIO(toolSystem)
#if defined(WIN32)
, m_pUnbufferedIO(&m_BufferedIO)
#endif
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
, m_bError(false)
, m_bUseThread(bUseThread)
, m_bEndThread(true)
{
	// Create the buffer (and define the block size) before opening !!
	// When opening the thread is launched. If we have not enough time
	// to allocate the buffer _and_ define the block size to a value other than
	// 0 the thread loop may trigger a divide by zero or try to read data from
	// the buffer (which hasn't been allocated yet !!)
	CreateBuffer(iNbBlocks, iBlockSize);
	Open(szFile, kRead | kWrite | kDenyNone | kOpenAlways | kSequential, bUseAccessLock);
}

IOFastWriteFile::IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread, const wchar_t *wszFile, bool bUseAccessLock)
: IOFile(toolSystem)
, m_BufferedIO(toolSystem)
#if defined(WIN32)
, m_pUnbufferedIO(&m_BufferedIO)
#endif
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
, m_bError(false)
, m_bUseThread(bUseThread)
, m_bEndThread(true)
{
	CreateBuffer(iNbBlocks, iBlockSize);
	Open(wszFile, kRead | kWrite | kDenyNone | kOpenAlways | kSequential, bUseAccessLock);
}

IOFastWriteFile::~IOFastWriteFile(void) {
	Close();
	DestroyBuffer();
}

bool IOFastWriteFile::_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock) {
	Close();

	if(!m_BufferedIO._Open(szFile, wszFile, kRead | kWrite | kDenyNone | kOpenAlways | kSequential, bUseAccessLock))
		return false;

#if defined(WIN32)
	std::string fileName;
	if(wszFile && !g_bUseUnicodeWinAPI) {
		fileName = TextWToA(wszFile, -1);
		szFile = fileName.c_str();
	}

	m_pUnbufferedIO = NULL;

	if(szFile)
		m_pUnbufferedIO = new IOFile(m_ToolSystem, szFile, kRead | kWrite | kDenyNone | kOpenAlways | kWriteThrough | kUnbuffered, true);
	else if(wszFile)
		m_pUnbufferedIO = new IOFile(m_ToolSystem, wszFile, kRead | kWrite | kDenyNone | kOpenAlways | kWriteThrough | kUnbuffered, true);

	if(m_pUnbufferedIO && !m_pUnbufferedIO->IsOpened()) {
		delete m_pUnbufferedIO;
		// Try buffered
		if(szFile)
			m_pUnbufferedIO = new IOFile(m_ToolSystem, szFile, kRead | kWrite | kDenyNone | kOpenAlways | kWriteThrough, true);
		else if(wszFile)
			m_pUnbufferedIO = new IOFile(m_ToolSystem, wszFile, kRead | kWrite | kDenyNone | kOpenAlways | kWriteThrough, true);
	}

	if(m_pUnbufferedIO && !m_pUnbufferedIO->IsOpened()) {
		delete m_pUnbufferedIO;
		m_pUnbufferedIO = NULL;
	}

	if(!m_pUnbufferedIO)
		m_pUnbufferedIO = &m_BufferedIO;
#endif

	m_Filename = m_BufferedIO.getFilename();
	m_Name = m_BufferedIO.getName();

	m_bError = false;

	m_DataReceivedSignal.unsignal();
	m_DataProcessedSignal.unsignal();
	m_EndThreadSignal.unsignal();
	m_bEndThread = false;
#if TOOLS_USETHREAD
	if(m_bUseThread) {
		if(Thread_Reset()) {
			Thread_Run();
		} else {
			m_ToolSystem.PrintMessage(kMsgWarning, L"Error while reseting thread state of %ls.\n"
				, getName());
			m_bUseThread = false;
		}
	}
#else
	m_bUseThread = false;
#endif /* TOOLS_USETHREAD */

	return true;
}

IOStream *IOFastWriteFile::Clone(uint32 iFlags, bool bUseAccessLock) {
	return NULL;
}

bool IOFastWriteFile::IsOpened(void) {
#if defined(WIN32)
	return (m_pUnbufferedIO && m_pUnbufferedIO->IsOpened() && m_pBuffer);
#else
	return (m_BufferedIO.IsOpened() && m_pBuffer);
#endif
}

void IOFastWriteFile::Close(void) {
	if(IsOpened()) {
		Flush(false);
		m_BufferedIO.SetEOF();
	}
	if(m_bUseThread && !m_bEndThread) {
		m_bEndThread = true;
		m_DataReceivedSignal.signal();
		m_EndThreadSignal.wait();
	}
	m_BufferedIO.Close();
#if defined(WIN32)
	if(m_pUnbufferedIO && (m_pUnbufferedIO != &m_BufferedIO)) {
		m_pUnbufferedIO->Close();
		delete m_pUnbufferedIO;
		m_pUnbufferedIO = NULL;
	}
#endif
}

void IOFastWriteFile::SetEOF(void) {
	Flush(true);
	m_BufferedIO.SetEOF();
}

bool IOFastWriteFile::ChangeFlags(uint32 iFlags) {
	return false;
}

sint64 IOFastWriteFile::GetPosition(void) {
	sint64 iPosition;

	++m_BufferLock;

#if defined(WIN32)
	iPosition = m_pUnbufferedIO->GetPosition();
#else
	iPosition = m_BufferedIO.GetPosition();
#endif

	if(iPosition >= 0)
		iPosition += m_iBufferLevel;

	--m_BufferLock;
	
	return iPosition;
}

sint64 IOFastWriteFile::GetSize(void) {
	++m_BufferLock;

#if defined(WIN32)
	sint64 iSize = m_pUnbufferedIO->GetSize();
#else
	sint64 iSize = m_BufferedIO.GetSize();
#endif
	if(iSize < GetPosition())
		iSize = GetPosition();

	--m_BufferLock;

	return iSize;
}

sint32 IOFastWriteFile::Read(void *pBuffer, sint32 iBytes) {
	return 0;
}

sint32 IOFastWriteFile::Write(void *pBuffer, sint32 iBytes) {
	if(m_bError)
		return 0;

	sint32 iRemainingBytes = iBytes;
	sint32 iFree;
	while(iRemainingBytes > 0) {
		++m_BufferLock;

		while((iFree=m_iBufferSize-m_iBufferLevel) <= 0) {

			--m_BufferLock;

			if(m_bUseThread)
				m_DataProcessedSignal.wait();
			else {
				while(ProcessPendingData());
			}

			++m_BufferLock;

		}

		if(m_iBufferWrite + iFree > m_iBufferSize)
			iFree = m_iBufferSize - m_iBufferWrite;

		assert(iFree);

		sint32 iToProcess = iRemainingBytes;
		if(iToProcess > iFree)
			iToProcess = iFree;

		memcpy((char *)m_pBuffer + m_iBufferWrite, pBuffer, iToProcess);

		iRemainingBytes -= iToProcess;
		pBuffer = (char *)pBuffer + iToProcess;
		m_iBufferWrite += iToProcess;
		if(m_iBufferWrite >= m_iBufferSize)
			m_iBufferWrite -= m_iBufferSize;

		m_iBufferLevel += iToProcess;
		if(m_iBufferLevel >= m_iBlockSize)
			m_DataReceivedSignal.signal();

		--m_BufferLock;
	}

	return iBytes;
}

sint64 IOFastWriteFile::Seek(sint64 iOffset, int mode) {
#if defined(WIN32)
	sint64 iCurrentPosition = GetPosition();
	if(iCurrentPosition == -1)
		return -1;
	
	Flush(false);

	assert(!m_iBufferLevel && (m_iBufferRead==m_iBufferWrite));

	sint64 iNewPosition;
	switch(mode) {
	case SEEK_SET:
		iNewPosition = iOffset;
		break;
	case SEEK_CUR:
		iNewPosition = iCurrentPosition + iOffset;
		break;
	case SEEK_END:
		iNewPosition = GetSize() + iOffset;
		break;
	default:
		assert(false);
		break;
	}

	sint32 iSectorOffset = iNewPosition % m_iSectorSize;
	sint64 iSectorBase = iNewPosition - iSectorOffset;

	if(m_BufferedIO.Seek(iSectorBase, SEEK_SET) != iSectorBase)
		return -1;
	if(m_pUnbufferedIO->Seek(iSectorBase, SEEK_SET) != iSectorBase)
		return -1;

	++m_BufferLock;

	if(iSectorOffset) {
		memset((char *)m_pBuffer + m_iBufferWrite, 0, iSectorOffset);
		m_BufferedIO.Read((char *)m_pBuffer + m_iBufferWrite, iSectorOffset);
		m_iBufferLevel += iSectorOffset;
		m_iBufferWrite += iSectorOffset;
	}

	--m_BufferLock;

	return iNewPosition;
#else
	Flush(false);
	return m_BufferedIO.Seek(iOffset, mode);
#endif
}

void IOFastWriteFile::Flush(bool bPreBuffer) {

	++m_BufferLock;

	while(m_iBufferLevel >= m_iBlockSize) {

		--m_BufferLock;

		if(m_bUseThread)
			m_DataProcessedSignal.wait();
		else {
			while(ProcessPendingData());
		}

		++m_BufferLock;
	}

#if defined(WIN32)
	sint64 iPosition = m_pUnbufferedIO->GetPosition();
	if((iPosition>=0) && (m_BufferedIO.Seek(iPosition, SEEK_SET)==iPosition)) {
		if(m_iBufferLevel) {
			m_bError = (m_BufferedIO.Write((char *)m_pBuffer + m_iBufferRead, m_iBufferLevel) != m_iBufferLevel);

			sint64 iNewPosition = iPosition + m_iBufferLevel;
			sint32 iSectorOffset = iNewPosition % m_iSectorSize;
			sint64 iSectorBase = iNewPosition - iSectorOffset;

			if(bPreBuffer && iSectorOffset) {
				m_pUnbufferedIO->Seek(iSectorBase, SEEK_SET);
				m_iBufferRead += m_iBufferLevel - (m_iBufferLevel % m_iSectorSize);
				m_iBufferLevel -= (m_iBufferLevel % iSectorOffset);

				assert(m_iBufferRead == m_iBufferWrite - iSectorOffset);
				assert(m_iBufferLevel == iSectorOffset);

				if(m_iBufferRead) {
					memcpy(m_pBuffer, (char *)m_pBuffer + m_iBufferRead, m_iBufferLevel);
					m_iBufferRead = 0;
					m_iBufferWrite = m_iBufferLevel;
				}
			} else {
				m_iBufferWrite = 0;
				m_iBufferRead = 0;
				m_iBufferLevel = 0;
			}
		}
	} else {
		assert(false);
	}
#else
	if(m_iBufferLevel) {
		m_bError = (m_BufferedIO.Write((char *)m_pBuffer + m_iBufferRead, m_iBufferLevel) != m_iBufferLevel);
		m_iBufferWrite = 0;
		m_iBufferRead = 0;
		m_iBufferLevel = 0;
	}
#endif

	--m_BufferLock;
}

void IOFastWriteFile::ProcessWrite(sint32 iOffset, sint32 iBytes) {
	assert(!(iBytes % m_iBlockSize));

#if defined(WIN32)
	if(!m_bError)
		m_bError = (m_pUnbufferedIO->Write((char *)m_pBuffer + iOffset, iBytes) != iBytes);
#else
	if(!m_bError)
		m_bError = (m_BufferedIO.Write((char *)m_pBuffer + iOffset, iBytes) != iBytes);
#endif
}

bool IOFastWriteFile::ProcessPendingData(void) {

	++m_BufferLock;

	if(m_iBufferLevel<m_iBlockSize) {
		--m_BufferLock;

		return false;
	}

	sint32 iBytesToWrite = m_iBufferLevel - m_iBufferLevel % m_iBlockSize;
	if(iBytesToWrite > m_iBlockSize)
		iBytesToWrite = m_iBlockSize;

	if(m_iBufferRead + iBytesToWrite > m_iBufferSize) {
		sint32 iFirstPart = m_iBufferSize - m_iBufferRead;

		ProcessWrite(m_iBufferRead, iFirstPart);
		ProcessWrite(0, iBytesToWrite - iFirstPart);

		m_iBufferRead = iBytesToWrite - iFirstPart;
	} else {
		ProcessWrite(m_iBufferRead, iBytesToWrite);

		m_iBufferRead += iBytesToWrite;
		if(m_iBufferRead >= m_iBufferSize)
			m_iBufferRead -= m_iBufferSize;
	}

	m_iBufferLevel -= iBytesToWrite;

	m_DataProcessedSignal.signal();

	--m_BufferLock;

	return true;
}

#if TOOLS_USETHREAD
void *IOFastWriteFile::_Thread_Run(void) {
	DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, kMsg, L"%ls running ...\n", getName());
	while(!m_bEndThread) {
		if(!ProcessPendingData())
			m_DataReceivedSignal.wait();
	}
	DEBUG_MESSAGE(m_ToolSystem, kTrace_Checkup, kMsg, L"%ls finished.\n", getName());
	return (void *)0;
}

void IOFastWriteFile::Thread_OnExit(void) {
	m_EndThreadSignal.signal();
}
#endif /* TOOLS_USETHREAD */

//////////////////////////////////////////////////////////////////////

IOFastReadFile::IOFastReadFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize)
: IOFile(toolSystem)
, m_iFilePosition(0)
, m_BlockInfo(NULL)
, m_iLastAccessedBlockInfo(0)
, m_iRequest(0)
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iNbBlocks(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
{
	CreateBuffer(iNbBlocks, iBlockSize);
}

IOFastReadFile::IOFastReadFile(ToolSystem& toolSystem, const char *szFile, uint32 iFlags, bool bUseAccessLock, sint32 iNbBlocks, sint32 iBlockSize)
: IOFile(toolSystem, szFile, iFlags, bUseAccessLock)
, m_iFilePosition(0)
, m_BlockInfo(NULL)
, m_iLastAccessedBlockInfo(0)
, m_iRequest(0)
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iNbBlocks(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
{
	CreateBuffer(iNbBlocks, iBlockSize);
}

IOFastReadFile::IOFastReadFile(ToolSystem& toolSystem, const wchar_t *szFile, uint32 iFlags, bool bUseAccessLock, sint32 iNbBlocks, sint32 iBlockSize)
: IOFile(toolSystem, szFile, iFlags, bUseAccessLock)
, m_iFilePosition(0)
, m_BlockInfo(NULL)
, m_iLastAccessedBlockInfo(0)
, m_iRequest(0)
, m_pBuffer(NULL)
, m_iSectorSize(0)
, m_iNbBlocks(0)
, m_iBlockSize(0)
, m_iBufferSize(0)
, m_iBufferLevel(0)
, m_iBufferRead(0)
, m_iBufferWrite(0)
{
	CreateBuffer(iNbBlocks, iBlockSize);
}

IOFastReadFile::~IOFastReadFile(void) {
	DestroyBuffer();
}

void IOFastReadFile::Flush(void) {
	int iIndex;
	for(iIndex=0 ; iIndex<m_iNbBlocks ; iIndex++) {
		m_BlockInfo[iIndex].m_iBlockNb = -1;
		m_BlockInfo[iIndex].m_iLastRequest = 0;
	}

	m_iRequest = 0;
	m_iLastAccessedBlockInfo = 0;
}

IOStream *IOFastReadFile::Clone(uint32 iFlags, bool bUseAccessLock) {
	IOStream *pIO = new IOFastReadFile(m_ToolSystem, m_Filename.c_str(), (iFlags ? iFlags : m_iFlags), bUseAccessLock, m_iNbBlocks, m_iBlockSize);
	if(!pIO)
		return NULL;

	if(!pIO->IsOpened()) {
		delete pIO;
		return NULL;
	}

	sint64 iPosition = GetPosition();
	if(iPosition > 0)
		pIO->Seek(iPosition, SEEK_SET);

	return pIO;
}

bool IOFastReadFile::_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock) {
	m_iFilePosition = 0;
	Flush();
	return IOFile::_Open(szFile, wszFile, iFlags, bUseAccessLock);
}

bool IOFastReadFile::IsOpened(void) {
	return (m_pBuffer && m_BlockInfo && IOFile::IsOpened());
}

void IOFastReadFile::Close(void) {
	Flush();
	IOFile::Close();
}

void IOFastReadFile::SetEOF(void) {
}

bool IOFastReadFile::ChangeFlags(uint32 iFlags) {
	if(iFlags & kWrite)
		return false;

	return IOFile::ChangeFlags(iFlags);
}

sint64 IOFastReadFile::GetPosition(void) {
	return m_iFilePosition;
}

sint32 IOFastReadFile::Read(sint64 iOffset, void *pBuffer, sint32 iBytes) {
	sint32 iActualBytes = 0;
	//sint32 iBlockInfo;
	sint32 iToRead;

	sint64 iBlockNb = iOffset / m_iBlockSize;
	sint32 iBlockOffset = iOffset % m_iBlockSize;

	uint8 *pDest = (uint8 *)pBuffer;

	while(iBytes) {
		m_iLastAccessedBlockInfo = PickBlock(iBlockNb);

		iToRead = m_BlockInfo[m_iLastAccessedBlockInfo].m_iBytes - iBlockOffset;
		if(iToRead > iBytes)
			iToRead = iBytes;
		else if(iToRead < 0)
			break;

		memcpy(pDest, (uint8 *)m_pBuffer + m_iLastAccessedBlockInfo * m_iBlockSize + iBlockOffset, iToRead);
		iActualBytes += iToRead;

		if(iToRead < m_iBlockSize - iBlockOffset) {
			// No more to read (either we don't want more, or the file don't have more)
			break;
		}

		iBytes -= iToRead;
		pDest += iToRead;
		iBlockOffset = 0;
		iBlockNb++;
	}

	m_iFilePosition = iOffset + iActualBytes;

	return iActualBytes;
}

sint32 IOFastReadFile::Read(void *pBuffer, sint32 iBytes) {
	return Read(m_iFilePosition, pBuffer, iBytes);
}

sint32 IOFastReadFile::Write(void *pBuffer, sint32 iBytes) {
	return 0;
}

sint64 IOFastReadFile::Seek(sint64 iOffset, int mode) {
	sint64 iNewPosition = -1;
	switch(mode) {
	case SEEK_SET:
		iNewPosition = iOffset;
		break;
	case SEEK_CUR:
		iNewPosition = m_iFilePosition + iOffset;
		break;
	case SEEK_END:
		iNewPosition = GetSize() + iOffset;
		break;
	default:
		assert(false);
		break;
	}

	m_iFilePosition = iNewPosition;

	return iNewPosition;
}

sint32 IOFastReadFile::PickBlock(sint64 iBlockNb) {
	int iIndex;

	m_iRequest++;

	/* First test the last accessed block */
	if(m_BlockInfo[m_iLastAccessedBlockInfo].m_iBlockNb == iBlockNb)
		return m_iLastAccessedBlockInfo;

	/* Is it already cached ? */
	for(iIndex=0 ; iIndex<m_iNbBlocks ; iIndex++) {
		if(m_BlockInfo[iIndex].m_iBlockNb == iBlockNb) {
			m_BlockInfo[iIndex].m_iLastRequest = m_iRequest;
			return iIndex;
		}
	}

	/* Pick a victim :] */
	iIndex = PickVictim();

	m_BlockInfo[iIndex].m_iBlockNb = iBlockNb;
	m_BlockInfo[iIndex].m_iLastRequest = m_iRequest;
	/* Warning: GetPosition calls SetFilePointer which fails if the actual
	 * file position isn't a multiple of the sector size for unbuffered files 
	 */
	/*if(IOFile::GetPosition() != iBlockNb * m_iBlockSize)*/
	IOFile::Seek(iBlockNb * m_iBlockSize, SEEK_SET);
	sint32 iRead = _Read((uint8 *)m_pBuffer + iIndex * m_iBlockSize, m_iBlockSize);
	if(iRead < 0)
		iRead = 0;
	m_BlockInfo[iIndex].m_iBytes = iRead;

	return iIndex;
}

sint32 IOFastReadFile::PickVictim(void) {
	int iIndex;
	int iVictim = 0;

	/* Is there a free block ? */
	for(iIndex=0 ; iIndex<m_iNbBlocks ; iIndex++) {
		if(m_BlockInfo[iIndex].m_iBlockNb == -1)
			return iIndex;
	}

	/* Choose the oldest requested block */
	for(iIndex=1 ; iIndex<m_iNbBlocks ; iIndex++) {
		if(m_BlockInfo[iIndex].m_iLastRequest < m_BlockInfo[iVictim].m_iLastRequest)
			iVictim = iIndex;
	}

	return iVictim;
}

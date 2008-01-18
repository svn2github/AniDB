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

#ifndef __TOOLS_IOSTREAM_FILE_H__
#define __TOOLS_IOSTREAM_FILE_H__

#include "IOStream.h"
#include "common.h"
#include "thread.h"
#if defined(__POSIX__)

#include "posix/common_POSIX.h"
#define DEFAULT_USELOCK			POSIX_DEFAULT_USELOCK

#elif defined(WIN32)

#include "Win32/common_win32.h"
#define DEFAULT_USELOCK			WIN32_DEFAULT_USELOCK

#endif


class TOOLS_DLL_API IOFile : public IOStream {
friend class IOFastWriteFile;
protected:
#if defined(__POSIX__)
	FILE			*m_fFile;
#elif defined(WIN32)
	HANDLE			m_hFile;
#endif
	std::wstring	m_Filename;

	virtual bool	_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	sint32	_Read(void *pBuffer, sint32 iBytes);
	sint32	_Write(void *pBuffer, sint32 iBytes);

public:
	IOFile(ToolSystem& toolSystem);
	IOFile(ToolSystem& toolSystem, const char *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	IOFile(ToolSystem& toolSystem, const wchar_t *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	virtual ~IOFile(void);

	const wchar_t	*getFilename(void);
	IOStream	*Clone(uint32 iFlags, bool bUseAccessLock);
	bool	Open(const char *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	bool	Open(const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	bool	IsOpened(void);
	void	Close(void);
	void	SetEOF(void);
	bool	ChangeFlags(uint32 iFlags);
	sint64	GetPosition(void);
	sint64	GetSize(void);
	sint64	Seek(sint64 iOffset, int mode=SEEK_CUR);
};

/*
 * 'Fast' reading and writing classes.
 * Inspired by those in VirtualDub (c) 2003  Avery Lee
 */
class TOOLS_DLL_API IOFastWriteFile : public IOFile
#if TOOLS_USETHREAD
, public Thread
#endif /* TOOLS_USETHREAD */
{
protected:
	IOFile	m_BufferedIO;
#if defined(WIN32)
	IOFile	*m_pUnbufferedIO;
#endif
	CriticalSection	m_BufferLock;
	void		*m_pBuffer;
	sint32		m_iSectorSize;
	sint32		m_iBlockSize;
	sint32		m_iBufferSize;
	volatile sint32	m_iBufferLevel;
	sint32		m_iBufferRead;
	sint32		m_iBufferWrite;
	Event		m_DataReceivedSignal;
	Event		m_DataProcessedSignal;
	volatile bool	m_bError;

	bool				m_bUseThread;
	volatile bool		m_bEndThread;
	EventPersistent		m_EndThreadSignal;
#if TOOLS_USETHREAD
	void				*_Thread_Run(void);
	void				Thread_OnExit(void);
#endif /* TOOLS_USETHREAD */
	bool				ProcessPendingData(void);
	void				ProcessWrite(sint32 iOffset, sint32 iBytes);

	void	CreateBuffer(sint32 iNbBlocks, sint32 iBlockSize);
	void	DestroyBuffer(void);
	bool	_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	void	Flush(bool bPreBuffer);

public:
	IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread);
	IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread, const char *szFile, bool bUseAccessLock=DEFAULT_USELOCK);
	IOFastWriteFile(ToolSystem& toolSystem, sint32 iNbBlocks, sint32 iBlockSize, bool bUseThread, const wchar_t *wszFile, bool bUseAccessLock=DEFAULT_USELOCK);
	virtual ~IOFastWriteFile(void);

	IOStream	*Clone(uint32 iFlags, bool bUseAccessLock);
	bool	IsOpened(void);
	void	Close(void);
	void	SetEOF(void);
	bool	ChangeFlags(uint32 iFlags);
	sint64	GetPosition(void);
	sint64	GetSize(void);
	sint32	Read(void *pBuffer, sint32 iBytes);
	sint32	Write(void *pBuffer, sint32 iBytes);
	sint64	Seek(sint64 iOffset, int mode=SEEK_CUR);
};


typedef struct {
	sint64	m_iBlockNb;
	sint64	m_iLastRequest;
	sint32	m_iBytes;
} IOFastReadFile_Info;

class TOOLS_DLL_API IOFastReadFile : public IOFile {
protected:
	sint64		m_iFilePosition;
	IOFastReadFile_Info	*m_BlockInfo;
	sint32		m_iLastAccessedBlockInfo;
	sint64		m_iRequest;
	void		*m_pBuffer;
	sint32		m_iSectorSize;
	sint32		m_iNbBlocks;
	sint32		m_iBlockSize;
	sint32		m_iBufferSize;
	volatile sint32	m_iBufferLevel;
	sint32		m_iBufferRead;
	sint32		m_iBufferWrite;

	bool	_Open(const char *szFile, const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	//sint32	_Read(void *pBuffer, sint32 iBytes);
	//sint32	_Write(void *pBuffer, sint32 iBytes);

	void	CreateBuffer(sint32 iNbBlocks, sint32 iBlockSize);
	void	DestroyBuffer(void);
	void	Flush(void);

	sint32	PickBlock(sint64 iBlockNb);
	sint32	PickVictim(void);	/* I like this function name :D */

public:
	IOFastReadFile(ToolSystem& toolSystem, sint32 iNbBlocks=8, sint32 iBlockSize=128*1024);
	IOFastReadFile(ToolSystem& toolSystem, const char *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK, sint32 iNbBlocks=8, sint32 iBlockSize=128*1024);
	IOFastReadFile(ToolSystem& toolSystem, const wchar_t *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK, sint32 iNbBlocks=8, sint32 iBlockSize=128*1024);
	virtual ~IOFastReadFile(void);

	//const wchar_t	*getFilename(void);
	IOStream	*Clone(uint32 iFlags, bool bUseAccessLock);
	//bool	Open(const char *szFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	//bool	Open(const wchar_t *wszFile, uint32 iFlags, bool bUseAccessLock=DEFAULT_USELOCK);
	bool	IsOpened(void);
	void	Close(void);
	void	SetEOF(void);
	bool	ChangeFlags(uint32 iFlags);
	sint64	GetPosition(void);
	//sint64	GetSize(void);
	sint32	Read(sint64 iOffset, void *pBuffer, sint32 iBytes);
	sint32	Read(void *pBuffer, sint32 iBytes);
	sint32	Write(void *pBuffer, sint32 iBytes);
	sint64	Seek(sint64 iOffset, int mode=SEEK_CUR);
};

#endif // __TOOLS_IOSTREAM_FILE_H__

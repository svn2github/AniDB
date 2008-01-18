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

#ifndef __TOOLS_IOSTREAM_H__
#define __TOOLS_IOSTREAM_H__

#include "common.h"
#include "common_endian.h"
#include <stdio.h>
#include <string>

/*
 * Some common values.
 * Taken from VirtualDub (C) 2003 Avery Lee
 */
namespace nsFile {
	enum eFlags {
		kRead			= 0x00000001,
		kWrite			= 0x00000002,
		kReadWrite		= kRead | kWrite,
		kAccessMask		= 0x0000000F,

		kDenyNone		= 0x00000000,
		kDenyRead		= 0x00000010,
		kDenyWrite		= 0x00000020,
		kDenyAll		= kDenyRead | kDenyWrite,
		kDenyMask		= 0x000000F0,

		kCreateNew			= 0x00000100,
		kCreateAlways		= 0x00000200,
		kOpenExisting		= 0x00000300,
		kOpenAlways			= 0x00000400,
		kTruncateExisting	= 0x00000500,		// not particularly useful, really
		kCreationMask		= 0x0000FF00,

		kSequential			= 0x00010000,
		kRandomAccess		= 0x00020000,
		kUnbuffered			= 0x00040000,		// much faster on Win32 thanks to the crappy cache, but possibly bad in Unix?
		kWriteThrough		= 0x00080000,
		kFlagsMask			= 0x00FF0000,

		kAllFileFlags		= 0xFFFFFFFF
	};
};

class IOStream_AccessLocker;

class TOOLS_DLL_API IOStream {
protected:
	ToolSystem&		m_ToolSystem;
	std::wstring	m_Name;
	uint32			m_iFlags;
	bool			m_bOutputSystem;
	IOStream_AccessLocker	*m_pLocker;

	virtual sint32	_Read(void *pBuffer, sint32 iBytes) = 0;
	virtual sint32	_Write(void *pBuffer, sint32 iBytes) = 0;

public:
	IOStream(ToolSystem& toolSystem, IOStream_AccessLocker *pLocker, uint32 iFlags=0, bool bOutputSystem=false);
	virtual ~IOStream(void);

	virtual const wchar_t	*getName(void);
	void			setOutputSystem(bool bOutputSystem);
	uint32			getFlags(void);
	IOStream_AccessLocker	*getAccessLocker(void);

	virtual IOStream	*Clone(void);
	virtual IOStream	*Clone(uint32 iFlags, bool bUseAccessLock) = 0;
	virtual bool	IsOpened(void);
	virtual void	Close(void);
	virtual void	SetEOF(void) = 0;
	virtual bool	ChangeFlags(uint32 iFlags) = 0;
	virtual sint64	GetPosition(void) = 0;
	virtual sint64	GetSize(void) = 0;
	virtual sint32	Read(void *pBuffer, sint32 iBytes);
			uint8	Read_uint8(bool *pSuccess=NULL);
			uint16	Read_uint16(eEndianness endian=SYSTEM_ENDIAN, bool *pSuccess=NULL);
			uint32	Read_uint32(eEndianness endian=SYSTEM_ENDIAN, bool *pSuccess=NULL);
			uint64	Read_uint64(eEndianness endian=SYSTEM_ENDIAN, bool *pSuccess=NULL);
	virtual sint32	Write(void *pBuffer, sint32 iBytes);
			bool	Write_uint8(uint8 iValue);
			bool	Write_uint16(uint16 iValue, eEndianness endian=SYSTEM_ENDIAN);
			bool	Write_uint32(uint32 iValue, eEndianness endian=SYSTEM_ENDIAN);
			bool	Write_uint64(uint64 iValue, eEndianness endian=SYSTEM_ENDIAN);
	virtual sint64	Seek(sint64 iOffset, int mode=SEEK_CUR) = 0;
	virtual bool	SynchOn(const uint8 *pSynchValue, const uint8 *pSynchMask, sint16 iSynchLength, sint32 iMaxSearch, sint32& iSkippedBytes);
};


class TOOLS_DLL_API IONull : public IOStream {
protected:
	sint64	m_iPosition;
	sint64	m_iCurrentSize;

	sint32	_Read(void *pBuffer, sint32 iBytes);
	sint32	_Write(void *pBuffer, sint32 iBytes);

public:
	IONull(ToolSystem& toolSystem);

	IOStream	*Clone(uint32 iFlags, bool bUseAccessLock);
	bool	ChangeFlags(uint32 iFlags);
	bool	IsOpened(void);
	void	SetEOF(void);
	sint64	GetPosition(void);
	sint64	GetSize(void);
	sint64	Seek(sint64 iOffset, int mode=SEEK_CUR);
};

TOOLS_DLL_API void IOStreamPrintW(void *pPrivate, eMsgType type, const wchar_t *szString);
TOOLS_DLL_API void IOStreamPrintA(void *pPrivate, eMsgType type, const wchar_t *szString);

TOOLS_DLL_API void	WriteBOM(IOStream& output);


#endif // __TOOLS_IOSTREAM_H__

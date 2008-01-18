/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Lockers for I/O streams
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

#include "IOStream_AccessLocker.h"
#include "common.h"
#include <list>
#if defined(WIN32)
#include "Win32/common_Win32.h"
#endif


TOOLS_DLL_API IOStream_AccessLocker_NoLock	g_AccessLocker_NoLock;

CriticalSection g_Universal_CriticalSection;

IOStream_AccessLocker g_UniversalLocker(g_Universal_CriticalSection);

//////////////////////////////////////////////////////////////////////

#if defined(WIN32)

class DiskAccessLocker_Win32 {
public:
	DWORD					m_dwVolumeSerialNumber;
	CriticalSection			*m_pLock;
	IOStream_AccessLocker	*m_pLocker;

	DiskAccessLocker_Win32(void);
	virtual ~DiskAccessLocker_Win32(void);
};

DiskAccessLocker_Win32::DiskAccessLocker_Win32(void) {
	m_dwVolumeSerialNumber = 0;
	m_pLock = new CriticalSection();
	if(m_pLock != NULL)
		m_pLocker = new IOStream_AccessLocker(*m_pLock);
	else
		m_pLocker = &g_UniversalLocker;
}

DiskAccessLocker_Win32::~DiskAccessLocker_Win32(void) {
	if(m_pLocker)
		delete m_pLocker;
	if(m_pLock)
		delete m_pLock;
}

class DiskAccessLockerManager_Win32 {
protected:
	std::list<DiskAccessLocker_Win32 *>	m_AccessLockers;

public:
	virtual ~DiskAccessLockerManager_Win32(void);

	IOStream_AccessLocker	*getLocker(ToolSystem& toolSystem, HANDLE hFile);
} g_DiskAccessLockerManager_Win32;

DiskAccessLockerManager_Win32::~DiskAccessLockerManager_Win32(void) {
	LIST_CLEAR(DiskAccessLocker_Win32, m_AccessLockers);
}

IOStream_AccessLocker *DiskAccessLockerManager_Win32::getLocker(ToolSystem& toolSystem, HANDLE hFile) {
	if(hFile != INVALID_HANDLE_VALUE) {
		BY_HANDLE_FILE_INFORMATION fileInf;
		if(GetFileInformationByHandle(hFile, &fileInf)) {
			std::list<DiskAccessLocker_Win32 *>::iterator lockerIt;
			for(lockerIt=m_AccessLockers.begin() ; lockerIt!=m_AccessLockers.end() ; lockerIt++) {
				DiskAccessLocker_Win32 *pLocker = *lockerIt;
				if(pLocker->m_dwVolumeSerialNumber == fileInf.dwVolumeSerialNumber)
					return pLocker->m_pLocker;
			}
			DiskAccessLocker_Win32 *pNewLocker = new DiskAccessLocker_Win32();
			if(pNewLocker) {
				pNewLocker->m_dwVolumeSerialNumber = fileInf.dwVolumeSerialNumber;
				m_AccessLockers.push_back(pNewLocker);
				return pNewLocker->m_pLocker;
			} else
				return &g_UniversalLocker;
		} else {
			PrintWin32Message(toolSystem, kMsgError, GetLastError(), L"Error while getting information on a file handle : %%ls\n");
			return &g_UniversalLocker;
		}
	} else
		return NULL;
}

IOStream_AccessLocker *IOStream_AccessLocker::getLocker(ToolSystem& toolSystem, HANDLE hFile) {
	return g_DiskAccessLockerManager_Win32.getLocker(toolSystem, hFile);
}

#else

#endif

//////////////////////////////////////////////////////////////////////

IOStream_AccessLocker::IOStream_AccessLocker(CriticalSection& lock)
: m_Lock(lock)
{
}

void IOStream_AccessLocker::Lock(void) {
	++m_Lock;
}

void IOStream_AccessLocker::Unlock(void) {
	--m_Lock;
}

//////////////////////////////////////////////////////////////////////

IOStream_AccessLocker_NoLock::IOStream_AccessLocker_NoLock(void)
: IOStream_AccessLocker(g_Universal_CriticalSection)
{
}

void IOStream_AccessLocker_NoLock::Lock(void) {
}

void IOStream_AccessLocker_NoLock::Unlock(void) {
}

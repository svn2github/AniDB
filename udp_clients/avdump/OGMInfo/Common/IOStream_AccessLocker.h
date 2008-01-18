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

#ifndef __TOOLS_IOSTREAM_ACCESSLOCKER_H__
#define __TOOLS_IOSTREAM_ACCESSLOCKER_H__

#include "common.h"
#include "thread.h"


class TOOLS_DLL_API IOStream_AccessLocker {
protected:
	CriticalSection&	m_Lock;

public:
	IOStream_AccessLocker(CriticalSection& lock);

#if defined(WIN32)
	static IOStream_AccessLocker	*getLocker(ToolSystem& toolSystem, HANDLE hFile);
#endif

	virtual void	Lock(void);
	virtual void	Unlock(void);
};


class TOOLS_DLL_API IOStream_AccessLocker_NoLock : public IOStream_AccessLocker {
public:
	IOStream_AccessLocker_NoLock(void);

	void	Lock(void);
	void	Unlock(void);
};

extern TOOLS_DLL_API IOStream_AccessLocker_NoLock	g_AccessLocker_NoLock;

#endif // __TOOLS_IOSTREAM_ACCESSLOCKER_H__

/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Common Win32 tools
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

#ifndef __TOOLS_COMMON_WIN32_H__
#define __TOOLS_COMMON_WIN32_H__

#include "../common.h"

#if defined(WIN32)

#include <string>
#include <windows.h>


#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER	((DWORD)-1)
#endif

#define WIN32_DEFAULT_USELOCK	true

static bool g_bUseUnicodeWinAPI = ((LONG)GetVersion() >= 0);

TOOLS_DLL_API std::wstring	GetWindowsError(DWORD dwError);
TOOLS_DLL_API void		PrintWin32Message(ToolSystem& toolSystem, eMsgType eType, DWORD dwError, const wchar_t *szFormat, ...);
TOOLS_DLL_API void		PrintWin32Message(ToolSystem& toolSystem, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, DWORD dwError, const wchar_t *szFormat, ...);

#endif // Win32

#endif // __TOOLS_COMMON_WIN32_H__

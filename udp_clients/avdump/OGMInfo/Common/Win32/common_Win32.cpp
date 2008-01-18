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

#include "../common.h"

#if defined(WIN32)

#include "common_Win32.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>


TOOLS_DLL_API std::wstring GetWindowsError(DWORD dwError) {
	LPVOID lpMessage = NULL;
	if(g_bUseUnicodeWinAPI)
		FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPWSTR)&lpMessage, 0, NULL);
	else
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, (LPSTR)&lpMessage, 0, NULL);
	std::wstring szMessage;
	if(lpMessage) {
		if(g_bUseUnicodeWinAPI)
			szMessage = (LPWSTR)lpMessage;
		else
			szMessage = TextAToW((LPSTR)lpMessage, -1);
		LocalFree(lpMessage);
	}
	return szMessage;
}

TOOLS_DLL_API void PrintWin32Message(ToolSystem& toolSystem, eMsgType eType, DWORD dwError, const wchar_t *szFormat, ...) {
	std::wstring szMsg;
	std::wstring szWin32Error = GetWindowsError(dwError);
	va_list argList;
	va_start(argList, szFormat);
	toolSystem.vFormatString(szMsg, szFormat, argList);
	va_end(argList);
	toolSystem.PrintMessage(eType, szMsg.c_str(), szWin32Error.c_str());
}

TOOLS_DLL_API void PrintWin32Message(ToolSystem& toolSystem, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, DWORD dwError, const wchar_t *szFormat, ...) {
	std::wstring szMsg;
	std::wstring szWin32Error = GetWindowsError(dwError);
	va_list argList;
	va_start(argList, szFormat);
	toolSystem.vFormatString(szMsg, szFormat, argList);
	va_end(argList);
	toolSystem.PrintMessage(szFile, szFunction, iLine, eType, szMsg.c_str(), szWin32Error.c_str());
}

//////////////////////////////////////////////////////////////////////

inline int TextWToALength(const wchar_t *pWstr, int iLength) {
	assert(iLength);

	int iRequired = WideCharToMultiByte(CP_ACP, 0, pWstr, iLength, NULL, 0, NULL, NULL);

	if((iLength<0) && (iRequired>=0))
		iRequired--;

	return iRequired;
}

TOOLS_DLL_API std::string TextWToA(const wchar_t *pWstr, int iLength) {
	std::string str;

	if(pWstr) {
		int iRequired = TextWToALength(pWstr, iLength);

		if(iRequired) {
			str.resize(iRequired);

			WideCharToMultiByte(CP_ACP, 0, pWstr, iLength, (char *)str.c_str(), str.length() + 1, NULL, NULL);
		}
	}

	return str;
}

inline int TextAToWLength(const char *pStr, int iLength) {
	assert(iLength);

	int iRequired = MultiByteToWideChar(CP_ACP, 0, pStr, iLength, NULL, 0);

	if((iLength<0) && (iRequired>=0))
		iRequired--;

	return iRequired;
}

TOOLS_DLL_API std::wstring TextAToW(const char *pStr, int iLength) {
	std::wstring wstr;

	if(pStr) {
		int iRequired = TextAToWLength(pStr, iLength);

		if(iRequired) {
			wstr.resize(iRequired);

			MultiByteToWideChar(CP_ACP, 0, pStr, iLength, (wchar_t *)wstr.c_str(), wstr.length() + 1);
		}
	}

	return wstr;
}

//////////////////////////////////////////////////////////////////////

TOOLS_DLL_API std::wstring GetFilename(const wchar_t *wszFile) {
	wchar_t wszFilename[_MAX_FNAME];
	wchar_t wszExtension[_MAX_EXT];

	if(!g_bUseUnicodeWinAPI)
		return TextAToW(GetFilename(TextWToA(wszFile).c_str()).c_str());

	*wszFilename = 0;
	*wszExtension = 0;
	_wsplitpath(wszFile, NULL, NULL, wszFilename, wszExtension);
	std::wstring wszName(wszFilename);
	wszName += wszExtension;

	return wszName;
}

TOOLS_DLL_API std::string GetFilename(const char *szFile) {
	char szFilename[_MAX_FNAME];
	char szExtension[_MAX_EXT];
	*szFilename = 0;
	*szExtension = 0;
	_splitpath(szFile, NULL, NULL, szFilename, szExtension);
	std::string szName(szFilename);
	szName += szExtension;

	return szName;
}

TOOLS_DLL_API std::wstring GetPath(const wchar_t *wszFile) {
	wchar_t wszDrive[_MAX_DRIVE];
	wchar_t wszDirectory[_MAX_DIR];

	if(!g_bUseUnicodeWinAPI)
		return TextAToW(GetPath(TextWToA(wszFile).c_str()).c_str());

	*wszDrive = 0;
	*wszDirectory = 0;
	_wsplitpath(wszFile, wszDrive, wszDirectory, NULL, NULL);
	std::wstring wszPath(wszDrive);
	wszPath += wszDirectory;

	return wszPath;
}

TOOLS_DLL_API std::string GetPath(const char *szFile) {
	char szDrive[_MAX_DRIVE];
	char szDirectory[_MAX_DIR];
	*szDrive = 0;
	*szDirectory = 0;
	_splitpath(szFile, szDrive, szDirectory, NULL, NULL);
	std::string szPath(szDrive);
	szPath += szDirectory;

	return szPath;
}

TOOLS_DLL_API std::wstring MakePath(const wchar_t *wszFolder, const wchar_t *wszFilename) {
	if(!g_bUseUnicodeWinAPI)
		return TextAToW(MakePath(TextWToA(wszFolder).c_str(), TextWToA(wszFilename).c_str()).c_str());

	std::vector<wchar_t> fullPath(_MAX_PATH);
	int iEstimated = wcslen(wszFolder) + 1 + wcslen(wszFilename);
	if(fullPath.size() < iEstimated) {
		fullPath.resize(iEstimated);
	}

	_wmakepath(&fullPath.front(), NULL, wszFolder, wszFilename, NULL);

	std::wstring wszPath(&fullPath.front());
	return wszPath;
}

TOOLS_DLL_API std::string  MakePath(const char *szFolder, const char *szFilename) {
	std::vector<char> fullPath(_MAX_PATH);
	int iEstimated = strlen(szFolder) + 1 + strlen(szFilename);
	if(fullPath.size() < iEstimated) {
		fullPath.resize(iEstimated);
	}

	_makepath(&fullPath.front(), NULL, szFolder, szFilename, NULL);

	std::string szPath(&fullPath.front());
	return szPath;
}

TOOLS_DLL_API bool FileExist(const wchar_t *wszFile) {
	struct _stat stats;
	if(!g_bUseUnicodeWinAPI)
		return FileExist(TextWToA(wszFile).c_str());

	memset(&stats, 0, sizeof stats);

	return ((_wstat(wszFile, &stats)!=-1) || (errno!=ENOENT));
}

TOOLS_DLL_API bool FileExist(const char *szFile) {
	struct _stat stats;

	memset(&stats, 0, sizeof stats);

	return ((_stat(szFile, &stats)!=-1) || (errno!=ENOENT));
}

#endif /* WIN32 */


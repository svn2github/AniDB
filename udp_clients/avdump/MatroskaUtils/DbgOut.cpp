/**
	*  Part of MusicBar, a plugin to control foobar2000/TCMP from the titlebar
	*
	*  Copyright (C) Jory Stone - 2003
	*
	*
	*  MusicBar is free software; you can redistribute it and/or modify
	*  it under the terms of the GNU General Public License as published by
	*  the Free Software Foundation; either version 2, or (at your option)
	*  any later version.
	*
	*  MusicBar is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	*  GNU General Public License for more details.
	*
	*  You should have received a copy of the GNU General Public License
	*  along with this program; see the file COPYING.  If not, write to
	*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
	*
*/

#include "DbgOut.h"

#ifdef DEBUG_OUT_PUT
#include <tchar.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <Mmsystem.h>

static DWORD g_dwTimeOffset = timeGetTime();

void WINAPI DbgStringOut(const TCHAR *pFormat,...)
{
  TCHAR szInfo[2000];

  // Format the variable length parameter list
  va_list va;
  va_start(va, pFormat);

  lstrcpy(szInfo, TEXT("MatroskaUtils"));
  wsprintf(szInfo + lstrlen(szInfo),
            TEXT("(tid %x) %8d : "),
            GetCurrentThreadId(), timeGetTime() - g_dwTimeOffset);

  _vstprintf(szInfo + lstrlen(szInfo), pFormat, va);
  lstrcat(szInfo, TEXT("\r\n"));
  OutputDebugString(szInfo);
  printf("OOOOUT: %s\r\n", szInfo);

  va_end(va);
};

#ifdef _UNICODE
void WINAPI DbgStringOut(const CHAR *pFormat,...)
{
	TCHAR szInfo[2000];

    // Format the variable length parameter list
    va_list va;
    va_start(va, pFormat);

    lstrcpy(szInfo, TEXT("MatroskaUtils"));
    wsprintf(szInfo + lstrlen(szInfo),
             TEXT("(tid %x) %8d : "),
             GetCurrentThreadId(), timeGetTime() - g_dwTimeOffset);

    CHAR szInfoA[2000];
    WideCharToMultiByte(CP_ACP, 0, szInfo, -1, szInfoA, sizeof(szInfoA)/sizeof(CHAR), 0, 0);

    wvsprintfA(szInfoA + lstrlenA(szInfoA), pFormat, va);
    lstrcatA(szInfoA, "\r\n");

    WCHAR wszOutString[2000];
    MultiByteToWideChar(CP_ACP, 0, szInfoA, -1, wszOutString, sizeof(wszOutString)/sizeof(WCHAR));
    OutputDebugString(wszOutString);
	wprintf(wszOutString);

    va_end(va);
}
#endif
#endif


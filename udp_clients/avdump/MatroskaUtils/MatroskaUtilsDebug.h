/*
 *  Part of The TCMP Matroska CDL, and Matroska Shell Extension
 *
 *  MatroskaUtilsDebug.h
 *
 *  Copyright (C) Jory Stone - June 2003
 *
 *  The TCMP Matroska CDL is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  The TCMP Matroska CDL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with The TCMP Matroska CDL; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
*/

/*!
    \file MatroskaUtilsDebug.h
		\version \$Id: MatroskaUtilsDebug.h,v 1.23 2004/02/20 06:12:32 jcsston Exp $
    \brief This is where the debugging defines go
		\author Jory Stone     <jcsston @ toughguy.net>

*/

#ifndef _MATROSKA_UTILS_DEBUG_H_
#define _MATROSKA_UTILS_DEBUG_H_

#ifdef DEBUG_MEM
#ifdef _DEBUG
   #define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
   #define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif
#endif

#ifdef USING_GETTTEXT
#define USE_SAFE_GETTEXT_DLL
#include <libintl.h>
#include "safe_gettext_dll.h"
// Simple macro for faster typing
#define _(String) gettext(String)
#ifdef _UNICODE
#define _W(String) gettextW(String)
#else
#define _W(String) gettext(String)
#endif
#else
#define _(String) String
#ifdef _UNICODE
#define _W2(x) L ## x
#define _W(x) _W2(x)
#else
#define _W(String) (String)
#endif
#endif

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <exception>
//#include "JString.h"

#define MAX_MATROSKA_PATH MAX_PATH*2

#ifdef DEBUG
// The crash handling code can get in the way of debuggers
#define CRASH_PROTECT_START
#define CRASH_PROTECT_END
#endif

#ifdef MATROSKA_INFO
// The info dll is stripped down for size
#define CRASH_PROTECT_START
#define CRASH_PROTECT_END
#endif

#define CRASH_PROTECT_BREAKPOINT

#ifndef CRASH_PROTECT_BREAKPOINT
#define CRASH_PROTECT_BREAKPOINT \
	{ \
		std::ostringstream buf; \
		buf << ("Breakpoint,\nFile: " __FILE__ "\nFunction: " __FUNCTION__ "\nLine:"); \
		buf << __LINE__; \
		MessageBoxA(NULL, buf.str().c_str(), "Breakpoint", MB_TASKMODAL); \
	}
#endif

#ifndef CRASH_PROTECT_START
#define CRASH_PROTECT_START try {
#endif
#ifndef CRASH_PROTECT_END
#define CRASH_PROTECT_END \
	} catch (std::exception &ex) { \
		std::ostringstream buf; \
		buf << "Exception: "; \
		buf << ex.what(); \
		buf << ("\nFile: " __FILE__ "\nFunction: " __FUNCTION__ "\nLine:"); \
		buf << __LINE__; \
		buf << "\nTry to generate bug report?"; \
		int msgBoxRet = MessageBoxA(NULL, buf.str().c_str(), "Error", MB_YESNO|MB_TASKMODAL); \
		if (msgBoxRet == IDYES) { \
				throw; \
		} \
	} catch (...) { \
		std::ostringstream buf; \
		buf << ("Unhandled Exception,\nFile: " __FILE__ "\nFunction: " __FUNCTION__ "\nLine:"); \
		buf << __LINE__; \
		buf << "\nTry to generate bug report?"; \
		int msgBoxRet = MessageBoxA(NULL, buf.str().c_str(), "Error", MB_YESNO|MB_TASKMODAL); \
		if (msgBoxRet == IDYES) { \
				throw; \
		} \
	};
#define CRASH_PROTECT_END_EXTRA(extra_info) \
	} catch (std::exception &ex) { \
		std::ostringstream buf; \
		buf << "Exception: "; \
		buf << ex.what(); \
		buf << ("\nFile: " __FILE__ "\nFunction: " __FUNCTION__ "\nLine:"); \
		buf << __LINE__; \
		buf << ("\nExtra: "); \
		buf << extra_info; \
		buf << "\nTry to generate bug report?"; \
		int msgBoxRet = MessageBoxA(NULL, buf.str().c_str(), "Error", MB_YESNO|MB_TASKMODAL); \
		if (msgBoxRet == IDYES) { \
				throw; \
		} \
	} catch (...) { \
		std::ostringstream buf; \
		buf << ("Unhandled Exception,\nFile: " __FILE__ "\nFunction: " __FUNCTION__ "\nLine:"); \
		buf << __LINE__; \
		buf << ("\nExtra: "); \
		buf << extra_info; \
		buf << "\nTry to generate bug report?"; \
		int msgBoxRet = MessageBoxA(NULL, buf.str().c_str(), "Error", MB_YESNO|MB_TASKMODAL); \
		if (msgBoxRet == IDYES) { \
				throw; \
		} \
	};
#endif

#include "DbgOut.h"

#ifndef wcsicmp
#define wcsicmp _wcsicmp
#endif

#ifndef snprintf
#define snprintf _snprintf
#endif

#ifndef snwprintf
#define snwprintf _snwprintf 
#endif

#ifdef _atoi64
#define atoi64(x) _atoi64(x)
#else
#ifdef strtoll 
#define atoi64(x) strtoll(x)
#else
#ifdef atoll
#define atoi64(x) strtoll(x)
#else
#define atoi64(x) atol(x)
#endif
#endif
#endif

#ifndef stricmp
#ifdef _stricmp
#define stricmp _stricmp
#else
#ifdef strcmpi
// Zen reported a strange problem with the Borland C++ Builder compiler
// having the strcmpi and yet not having a strcmpi function
#if !defined (__BORLANDC__)
#define stricmp strcmpi
#endif //__BORLANDC__
#else
#ifdef strcasecmp
#define stricmp strcasecmp
#else
#define stricmp strcmp
#endif
#endif
#endif
#endif

#endif // _MATROSKA_UTILS_DEBUG_H_


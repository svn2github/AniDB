/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Most common tools
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

#ifndef __TOOLS_COMMON_H__
#define __TOOLS_COMMON_H__

#if defined(_DEBUG) || defined(DEBUG)
#	define TOOLS_DEBUG
#endif

#if defined(_MSC_VER)
/* enable memory leaking detection system for MSVC in debug builds */
#	define _CRTDBG_MAP_ALLOC
#	include <stdlib.h>
#	include <crtdbg.h>
#	define ENABLE_MEMORYLEAKS_DETECTION		\
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );	\
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#else
#	define ENABLE_MEMORYLEAKS_DETECTION
#endif

#include "common_dll.h"

#include <stdarg.h>
#include <assert.h>
#include <algorithm>
#include <string>

#if defined(_WIN32) && !defined(WIN32)
#	define WIN32
#endif

#if defined(WIN32)
#	define strcasecmp	stricmp
#	define wcscasecmp	wcsicmp
#	define strncasecmp	strnicmp
#	define wcsncasecmp	wcsnicmp
#	define snprintf		_snprintf
#	define vswprintf(a,b,c,d)	_vsnwprintf(a,b,c,d)
#	define vsprintf(a,b,c,d)	_vsnprintf(a,b,c,d)
#	define STRINGFORMAT_NEEDFIXES	1
#	define UINT32_MAX	_UI32_MAX
#   define INT64_MAX    _I64_MAX
#	define STRING_FORMAT_U64	"I64u"
#	define STRING_FORMAT_I64	"I64d"
#	define WSTRING_FORMAT_U64	L"I64u"
#	define WSTRING_FORMAT_I64	L"I64d"
#elif #system(posix)
#	define __POSIX__
#	define UINT32_MAX	ULONG_MAX
#	define STRING_FORMAT_U64	"llu"
#	define STRING_FORMAT_I64	"lld"
#	define WSTRING_FORMAT_U64	L"llu"
#	define WSTRING_FORMAT_I64	L"lld"
#endif

#ifndef INT64_MAX
#   define INT64_MAX    _LL(0x7FFFFFFFFFFFFFFF)
#endif /* INT64_MAX */


#ifdef __GNUC__
#	define __ALIGN(s,x)	s __attribute__((__aligned__(x)))
#else
#	define __ALIGN(s,x)	__declspec(align(x)) s
#endif


#if defined(__MINGW32__)
namespace std {
	TOOLS_EXPIMP_TEMPLATE template class TOOLS_DLL_API basic_string<wchar_t>;
	typedef basic_string<wchar_t> wstring;
}
#endif /* __MINGW32__ */


#if defined(__POSIX__)

inline wchar_t *wcstok(wchar_t *pToken, const wchar_t *pSep) {
	wchar_t *pPtr = NULL;
	return wcstok(pToken, pSep, &pPtr);
}

#endif /* __POSIX__ */


#if defined(_MSC_VER)
/* MSVC can be annoying sometimes ...
 *
 * Warning 4200: nonstandard extension used : zero-sized array in struct/union
 * Warning 4231: nonstandard extension used : 'extern' before template explicit instantiation
 * Warning 4251: class '%' needs to have dll-interface to be used by clients of class '%'
 * Warning 4355: 'this' : used in base member initializer list
 * Warning 4786: '%' : identifier was truncated to '255' characters in the debug information
 */
#	pragma warning ( disable : 4200 4231 4251 4355 4786 )
#endif /* _MSC_VER */

#include <vector>
#include <list>


#ifndef _LL
	#ifdef _MSC_VER
		#define _LL(i)	i ## i64
	#else
		#define _LL(i)	i ## ll
	#endif
#endif

// Fix taken from VirtualDub (c) Avery Lee
///////////////////////////////////////////////////////////////////////////
//
//	STL fixes
//
///////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#if _MSC_VER < 1300
	namespace std {
		template<typename T>
		inline const T& min(const T& x, const T& y) {
			return _cpp_min(x, y);
		}

		template<class T>
		inline const T& max(const T& x, const T& y) {
			return _cpp_max(x, y);
		}
	};
#endif
#endif

#ifdef _MSC_VER
typedef signed __int8		sint8;
typedef signed __int16		sint16;
typedef signed __int32		sint32;
typedef signed __int64		sint64;
typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;
#else
typedef signed char			sint8;
typedef signed short int	sint16;
typedef signed long			sint32;
typedef signed long long	sint64;
typedef unsigned char		uint8;
typedef unsigned short int	uint16;
typedef unsigned long		uint32;
typedef unsigned long long	uint64;
#endif


#define RoundToLong(f)	(sint32)((f)+0.5)
#define RoundToULong(f)	(uint32)((f)+0.5)


typedef enum {
	kMsg,
	kMsgWarning,
	kMsgError
} eMsgType;

typedef enum {
	kTrace_Tors		= 0x00000001,
	kTrace_Init		= 0x00000002,
	kTrace_Action	= 0x00000004,
	kTrace_Checkup	= 0x00000008,
	kTrace_Memory	= 0x00000010,
	kTrace_All		= 0xFFFFFFFF
} eTraceMode;

#if defined(TOOLS_DEBUG)
#	define DEFAULT_TRACE_MASK	(kTrace_All)
#else
#	define DEFAULT_TRACE_MASK	(0)
#endif

class TOOLS_DLL_API ToolSystem {
protected:
	void (*m_Print)(void *pPrivate, eMsgType eType, const wchar_t *szString);
	void	*m_pPrintPrivate;
	uint32	m_iTraceMask;

public:
	ToolSystem(void (*_Print)(void *pPrivate, eMsgType eType, const wchar_t *szString)=NULL, void *pPrintPrivate=NULL, uint32 iTraceMask=DEFAULT_TRACE_MASK);
	virtual ~ToolSystem(void);

	void	setPrintFunction(void (*)(void *pPrivate, eMsgType eType, const wchar_t *szString), void *pPrivate);
	uint32	getTraceMask(void);
	void		setTraceMask(uint32 iTraceMask);

#if STRINGFORMAT_NEEDFIXES
	static std::wstring	FixFormatString(const wchar_t *szFormat);
#endif
	static void	vFormatString(std::wstring& msg, const wchar_t *szFormat, va_list argList);
	static std::wstring	FormatString(const wchar_t *szFormat, ...);
	static void	FormatString(std::wstring& msg, const wchar_t *szFormat, ...);
	void	vPrintMessage(eMsgType eType, const wchar_t *szFormat, va_list argList);
	void	PrintMessage(eMsgType eType, const wchar_t *szFormat, ...);
	void	vPrintMessage(const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, va_list argList);
	void	PrintMessage(const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...);
};


//TOOLS_DLL_API void	PrintMessage(ToolSystem& toolSystem, eMsgType eType, const wchar_t *szFormat, ...);
TOOLS_DLL_API void	PrintMessage(ToolSystem& toolSystem, uint32 iMode, eMsgType eType, const wchar_t *szFormat, ...);
//TOOLS_DLL_API void	PrintMessage(ToolSystem& toolSystem, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...);
TOOLS_DLL_API void	PrintMessage(ToolSystem& toolSystem, uint32 iMode, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...);

#define DEBUG_MESSAGE	PrintMessage


// Default printing function : printf
TOOLS_DLL_API void DefaultPrint(void *pPrivate, eMsgType type, const wchar_t *szString);

TOOLS_DLL_API std::string	TextWToU8(const wchar_t *pWstr, int iLength=-1);
TOOLS_DLL_API std::string	TextWToU8(std::wstring& wstr);
TOOLS_DLL_API std::wstring	TextU8ToW(const char *pStr, bool& bValid, int iLength=-1);
TOOLS_DLL_API std::wstring	TextU8ToW(std::string& str, bool& bValid);
TOOLS_DLL_API std::string	TextWToA(const wchar_t *pWstr, int iLength=-1);
TOOLS_DLL_API std::string	TextWToA(std::wstring& wstr);
TOOLS_DLL_API std::wstring	TextAToW(const char *pStr, int iLength=-1);
TOOLS_DLL_API std::wstring	TextAToW(std::string& str);

TOOLS_DLL_API bool			IsBlank(std::wstring& str);

TOOLS_DLL_API std::wstring	GetFilename(const wchar_t *wszFile);
TOOLS_DLL_API std::string	GetFilename(const char *szFile);
TOOLS_DLL_API std::wstring	GetPath(const wchar_t *wszFile);
TOOLS_DLL_API std::string	GetPath(const char *szFile);
TOOLS_DLL_API std::wstring	MakePath(const wchar_t *wszFolder, const wchar_t *wsfFilename);
TOOLS_DLL_API std::string	MakePath(const char *szFolder, const char *szFilename);

TOOLS_DLL_API bool			FileExist(const wchar_t *wszFile);
TOOLS_DLL_API bool			FileExist(const char *szFile);


#define CONTAINER_DUPLICATE_DEFINITION(container)									\
template<class _T>																	\
void Duplicate##container(std::container<_T *>& dst, std::container<_T *>& src) {	\
	typename std::container<_T *>::iterator It;										\
	for(It=src.begin() ; It!=src.end() ; It++) {									\
		_T *pNewNode = new _T(*(*It));												\
		dst.push_back(pNewNode);													\
	}																				\
}

#define CONTAINER_CLEAR_DEFINITION(container)			\
template<class _T>										\
void Clear##container(std::container<_T *>& cont) {		\
	typename std::container<_T *>::iterator It;			\
	for(It=cont.begin() ; It!=cont.end() ; It++) {		\
		if(*It)											\
			delete *It;									\
	}													\
	cont.clear();										\
}

CONTAINER_DUPLICATE_DEFINITION(list)
CONTAINER_DUPLICATE_DEFINITION(vector)
CONTAINER_CLEAR_DEFINITION(list)
CONTAINER_CLEAR_DEFINITION(vector)


#define CONTAINER_DUPLICATE_0(container, _T, dst, src)	\
	Duplicate##container<_T>(dst, src)

#define CONTAINER_MERGE(dst, src)					\
	dst.insert(dst.end(), src.begin(), src.end())

#define CONTAINER_CLEAR_0(container, _T, cont)	\
	Clear##container<_T>(cont)

#define LIST_DUPLICATE(_T, dst, src)			\
	CONTAINER_DUPLICATE_0(list, _T, dst, src)
#define LIST_CLEAR(_T, cont)			\
	CONTAINER_CLEAR_0(list, _T, cont)
#define VECTOR_DUPLICATE(_T, dst, src)			\
	CONTAINER_DUPLICATE_0(vector, _T, dst, src)
#define VECTOR_CLEAR(_T, cont)			\
	CONTAINER_CLEAR_0(vector, _T, cont)


// Instantiate some template classes
// This does not create an object. It only forces the generation of all of the members
// of the class. It exports them from the DLL and imports them into the .exe file.
TOOLS_EXPIMP_TEMPLATE template class TOOLS_DLL_API std::vector<uint8>;


#define __FILENAME__	GetFilename(__FILE__).c_str()

#if defined(__FUNCTION__)
#	define MSG_INFO	__FILENAME__, __FUNCTION__, __LINE__
#else
#	define MSG_INFO	__FILENAME__, NULL, __LINE__
#endif

//////////////////////////////////////////////////////////////////////
// Taken from VirtualDub (C) Avery Lee

// TODO macros
//
// These produce a diagnostic during compilation that indicate a TODO for
// later:
//
//		#pragma message(__TODO__ "Fix this.")
//		#pragma_TODO("Fix this.")

#define __TODO1__(x)	#x
#define __TODO0__(x) __TODO1__(x)
#define __TODO__ __FILE__ "(" __TODO0__(__LINE__) ") : TODO: "

#ifdef _MSC_VER
#define pragma_TODO(x)		message(__TODO__ x)
#else
#define pragma_TODO(x)
#endif


#endif // __TOOLS_COMMON_H__

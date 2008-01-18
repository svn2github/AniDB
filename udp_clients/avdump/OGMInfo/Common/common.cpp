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

#include "common.h"
/* We need to call it somewhere */
#include "common_endian.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>


/* Default printf function: printf
 *  - Win32 prefers ANSI strings in DOS command boxes
 *  - Linux (POSIX too ?) doesn't like to mix wprintf and printf
 */

#if defined(WIN32) | defined(__POSIX__)
TOOLS_DLL_API void DefaultPrint(void *pPrivate, eMsgType type, const wchar_t *szString) {
	printf("%s", TextWToA(szString, -1).c_str());
}
#else
TOOLS_DLL_API void DefaultPrint(void *pPrivate, eMsgType type, const wchar_t *szString) {
	wprintf(L"%ls", szString);
}
#endif

//////////////////////////////////////////////////////////////////////

ToolSystem::ToolSystem(void (*_Print)(void *pPrivate, eMsgType eType, const wchar_t *szString), void *pPrintPrivate, uint32 iTraceMask)
: m_Print(_Print ? _Print : DefaultPrint)
, m_pPrintPrivate(pPrintPrivate)
, m_iTraceMask(iTraceMask)
{
}

ToolSystem::~ToolSystem(void) {
}

void ToolSystem::setPrintFunction(void (*_Print)(void *pPrivate, eMsgType eType, const wchar_t *szString), void *pPrivate) {
	m_Print = (_Print ? _Print : DefaultPrint);
	m_pPrintPrivate = pPrivate;
}

uint32 ToolSystem::getTraceMask(void) {
	return m_iTraceMask;
}

void ToolSystem::setTraceMask(uint32 iTraceMask) {
	m_iTraceMask = iTraceMask;
}

#if STRINGFORMAT_NEEDFIXES
	/* We need to fix some format specifiers for Win32 */
std::wstring ToolSystem::FixFormatString(const wchar_t *szFormat) {
	std::wstring fixedFormat;
	/* Reserve space beforehand */
	int iRemainings = wcslen(szFormat);
	fixedFormat.reserve(iRemainings + 1);
	wchar_t wChar;
	bool bFormat, bFlag, bWidth, bWidthDigit, bComma, bPrecision, bPrecisionDigit;
	bFormat = false;
	while(iRemainings && ((wChar=(*szFormat))!=0)) {
		iRemainings--;
		szFormat++;
		if(wChar == L'%') {
			if(bFormat)
				bFormat = false;
			else {
				bFormat = true;
				bFlag = false;
				bWidth = false;
				bWidthDigit = false;
				bComma = false;
				bPrecision = false;
				bPrecisionDigit = false;
			}
		}
		if(bFormat && (iRemainings>0)) {
			bool bSkip = false;
			/* First, test if there is a flag */
			if(!bFlag && !bWidth && !bWidthDigit && !bComma && !bPrecision && !bPrecisionDigit) {
				switch(*szFormat) {
				case L'-':
				case L'+':
				case L' ':
				case L'#':
				case L'0':
					bFlag = true;
					bSkip = true;
					break;
				}
			}
			/* Then test if we are specifying a width and/or precision */
			if(!bSkip && !bWidth && !bPrecision) {
				if(iswdigit(*szFormat)) {
					if(bComma)
						bPrecisionDigit = true;
					else
						bWidthDigit = true;
					bSkip = true;
				} else if((*szFormat) == L'*') {
					/* Don't allow '*' and digits mixing */
					if((bComma && !bPrecisionDigit) || (!bComma && !bWidthDigit)) {
						if(bComma)
							bPrecision = true;
						else
							bWidth = true;
						bSkip = true;
					}
				} else if(!bComma && ((*szFormat) == L'.')) {
					bComma = true;
					bSkip = true;
				}
			}
			/* Otherwise check if we need to fix the format */
			if(!bSkip) {
				bFormat = false;
				if( (iRemainings>=3)
					&& (szFormat[0]==L'l')
					&& (szFormat[1]==L'l')
					&& ((szFormat[2]==L'u')||(szFormat[2]==L'd')||(szFormat[2]==L'i')||(szFormat[2]=='o')||(szFormat[2]=='x')||(szFormat[2]=='X'))
				) {
					fixedFormat += wChar;
					fixedFormat += L"I64";
					wChar = szFormat[2];
					iRemainings -= 3;
					szFormat += 3;
				} else if((iRemainings>=1)
					&& (szFormat[0]==L's')
				) {
					fixedFormat += wChar;
					wChar = L'S';
					iRemainings--;
					szFormat++;
				}
			}
		}
		fixedFormat += wChar;
	}
	return fixedFormat;
}
#endif

void ToolSystem::vFormatString(std::wstring& msg, const wchar_t *szFormat, va_list argList) {
	sint32 iBufferLength = 0x0080;
	sint32 iMsgSize;
	msg.resize(iBufferLength);
#if STRINGFORMAT_NEEDFIXES
	std::wstring fixedFormat = FixFormatString(szFormat);
	szFormat = fixedFormat.c_str();
#endif
	while((iMsgSize=vswprintf((wchar_t *)msg.c_str(), iBufferLength, szFormat, argList)) < 0) {
		iBufferLength <<= 1;
		msg.resize(iBufferLength);
	}
	msg.resize(iMsgSize);
}

std::wstring ToolSystem::FormatString(const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	std::wstring msg;
	vFormatString(msg, szFormat, argList);
	va_end(argList);
	return msg;
}

void ToolSystem::FormatString(std::wstring& msg, const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	vFormatString(msg, szFormat, argList);
	va_end(argList);
}

void ToolSystem::vPrintMessage(eMsgType eType, const wchar_t *szFormat, va_list argList) {
	std::wstring szNewMsg;
	if((eType==kMsgWarning) || (eType==kMsgError))
		szNewMsg += L"[!] ";
	std::wstring szMsg;
	vFormatString(szMsg, szFormat, argList);
	szNewMsg += szMsg;
	//m_Print(m_pPrintPrivate, eType, szNewMsg.c_str());
}

void ToolSystem::PrintMessage(eMsgType eType, const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	vPrintMessage(eType, szFormat, argList);
	va_end(argList);
}

void ToolSystem::vPrintMessage(const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, va_list argList) {
	std::wstring szInfo(L"[");
	if(szFile)
		szInfo += TextAToW(szFile);
	if(szFunction) {
		if(szFile)
			szInfo += L", ";
		szInfo += TextAToW(szFunction);
	}
	if(iLine > 0) {
		if(szFile || szFunction)
			szInfo += L", ";
		szInfo += FormatString(L"%ld", iLine);
	}
	szInfo += L"]";
	std::wstring szMsg;
	vFormatString(szMsg, szFormat, argList);
	PrintMessage(eType, L"%ls %ls", szInfo.c_str(), szMsg.c_str());
}

void ToolSystem::PrintMessage(const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	vPrintMessage(szFile, szFunction, iLine, eType, szFormat, argList);
	va_end(argList);
}

//////////////////////////////////////////////////////////////////////

/*TOOLS_DLL_API void PrintMessage(ToolSystem& toolSystem, eMsgType eType, const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	toolSystem.vPrintMessage(eType, szFormat, argList);
	va_end(argList);
}*/

TOOLS_DLL_API void PrintMessage(ToolSystem& toolSystem, uint32 iMode, eMsgType eType, const wchar_t *szFormat, ...) {
	if(toolSystem.getTraceMask() & iMode) {
		va_list argList;
		va_start(argList, szFormat);
		toolSystem.vPrintMessage(eType, szFormat, argList);
		va_end(argList);
	}
}

/*TOOLS_DLL_API void PrintMessage(ToolSystem& toolSystem, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...) {
	va_list argList;
	va_start(argList, szFormat);
	toolSystem.vPrintMessage(szFile, szFunction, iLine, eType, szFormat, argList);
	va_end(argList);
}*/

TOOLS_DLL_API void PrintMessage(ToolSystem& toolSystem, uint32 iMode, const char *szFile, const char *szFunction, sint32 iLine, eMsgType eType, const wchar_t *szFormat, ...) {
	if(toolSystem.getTraceMask() & iMode) {
		va_list argList;
		va_start(argList, szFormat);
		toolSystem.vPrintMessage(szFile, szFunction, iLine, eType, szFormat, argList);
		va_end(argList);
	}
}

//////////////////////////////////////////////////////////////////////

/*
 * Some Unicode specs
 * ==================
 * http://www.unicode.org
 * http://czyborra.com/utf/
 * http://uk.geocities.com/BabelStone1357/Unicode/surrogates.html
 * http://www.i18nguy.com/surrogates.html
 *
 * UCS-4 (native 32-bit Unicode format) allow any code point value in the range U+00000000..U+7FFFFFFF.
 *
 * UTF-32 (UCS-4 subset) allow the ranges U+0000..U+D7FF, U+E000..U+FFFF and U+10000..U+10FFFF.
 * Note that the range U+D800..U+DFFF is not valid UTF-32 codes.
 * It is used by UTF-16 to encode values in the range above U+FFFF (see surrogates).
 * UTF-32 will therefore never have any codes assigned to those values.
 * In addition, any value such that (code & 0xFFFE) == 0xFFFE is invalid.
 * This includes 0xFFFE, 0xFFFF, 0x1FFFE, 0x1FFFF, 0x2FFFE, 0x2FFFF, .. up to 0x10FFFE and 0x10FFFF.
 * These codes are never assigned to valid UTF-32 codes.
 *
 * At first there was only UTF-16/UCS-2 (16-bit format). Due to a lack of space Unicode had to be extended
 * to UTF-32 and UCS-4. Currently UTF-32 should be enough. In order to reference UTF-32 characters with
 * UTF-16, the range U+D800..U+DFFF was used for the surrogate mechanism. This mechanism allow to
 * reference a code point in the range U+10000..U+10FFFF.
 * An UTF-32 char (wc) in this range can be stored using two UTF-16 chars (a surrogate pair) :
 *  x 0xD7C0 + (wc >> 10)	(High surrogate)
 *  x 0xDC00 | wc & 0x3FF	(Low surrogate)
 *
 * In UTF-16/32 you have the question of endianess.
 * UTF-16/32LE is UTF-16/32 as it appear natively on a little endian machine.
 * UTF-16/32BE is UTF-16/32 as it appear natively on a big endian machine.
 * UTF-16/32 is either UTF-16/32LE or UTF-16/32BE.
 * To determine which, the UTF-16/32 has as the first code of the stream a so-called "byte order mark" or BOM.
 * The BOM is the code U+FEFF. The UNICODE value for this code is "zero with no break space".
 * I.e. a space that prohibit linebreak and it has zero width.
 * As the first character of a stream this appear useless but it is used as a byte order mark.
 * The idea is that if you read this code with the wrong endianess you will read the code as either one of the following :
 *  x UTF-16 :
 *    0xFEFF - endianess is correct, just continue to read
 *    0xFFFE - wrong endianess, you need to swap the bytes so 12 becomes 21.
 *  x UTF-32 :
 *    0x0000FEFF - endianess is correct, just continue to read.
 *    0xFFFE0000 - wrong endianess, you need to swap the bytes so 1234 becomes 4321.
 *    0x0000FFFE - wrong endianess, you need to swap the bytes so 1234 becomes 2143.
 *    0xFEFF0000 - wrong endianess, you need to swap the bytes so 1234 becomes 3412.
 *
 * UTF-8 is a 8-bit format used to store Unicode (UCS-4) text while staying compatible with
 * programs working only on ASCII strings.
 *
 * (RFC 2279)
 * Unicode (UCS-4) Code point           <=>  UTF-8 encoding
 * 00000000 00000000 00000000 0xxxxxxx  <=>  0xxxxxxx
 * 00000000 00000000 00000xxx xxyyyyyy  <=>  110xxxxx 10yyyyyy
 * 00000000 00000000 xxxxyyyy yyzzzzzz  <=>  1110xxxx 10yyyyyy 10zzzzzz
 * 00000000 000xxxyy yyyyzzzz zzuuuuuu  <=>  11110xxx 10yyyyyy 10zzzzzz 10uuuuuu
 * 000000xx yyyyyyzz zzzzuuuu uuvvvvvv  <=>  111110xx 10yyyyyy 10zzzzzz 10uuuuuu 10vvvvvv
 * 0xyyyyyy zzzzzzuu uuuuvvvv vvwwwwww  <=>  1111110x 10yyyyyy 10zzzzzz 10uuuuuu 10vvvvvv 10wwwwww
 *
 * Note that in UTF-8 it is invalid to encode a specific value with a longer sequence of bytes
 * if it could be specified by a shorter sequence. So, even though 0xC0 0xb1 would map to 0x31
 * it is invalid since the 1 byte sequence 0x31 also maps to 0x31.
 * Also note that (since Unicode 3.2?) it is illegal to encode UTF-16 surrogates in UTF-8.
 * The surrogate pair must be combined to form a valid UCS-4/UTF-32 character before converting
 * Quoting Unicode specs (Chapter 3.9, D36) :
 *  x Before the Unicode Standard, Version 3.1, the problematic “non-shortest form”
 *    byte sequences in UTF-8 were those where BMP characters could be represented in
 *    more than one way. These sequences are ill-formed, because they are not allowed by
 *    Table 3-6.
 *  x Because surrogate code points are not Unicode scalar values, any UTF-8 byte sequence that
 *    would otherwise map to code points D800..DFFF is ill-formed.
 * Note : the BMP (Basic Multilingual Plane) represents the range U+0000..U+FFFF, also called
 * plane 0 (plane 1 is the range U+10000..U+1FFFF, ...; there are 256 planes)
 */

TOOLS_DLL_API std::string TextWToU8(const wchar_t *pWstr, int iLength) {
	std::string utf8;

	if(pWstr) {
		if(iLength < 0) {
			iLength = 0;
			const wchar_t *pTemp = pWstr;
			while(*pTemp++)
				iLength++;
		}
	
		utf8.resize(6 * iLength);
		const wchar_t *pSrc = pWstr;
		uint32 wc;
		char *pDst = (char *)utf8.c_str();

		while(iLength--) {
			wc = *pSrc++;
			// We must combine surrogates
			if((wc>=0xD800) && (wc<0xDC00)) {
				// This may be a high surrogate
				if(iLength && ((*pSrc)>=0xDC00) && ((*pSrc)<0xE000)) {
					// There is our low surrogate
					wc = 0x10000 + ((wc & 0x3FF)<<10) + ((*pSrc++) & 0x3FF);
					iLength--;
				} else
					wc = L'?';
			}
			if(wc < 0x0080)
				*pDst++ = wc;
			else {
				if(wc < 0x0800) {
					*pDst++ = 0xC0 | (wc>>6);
				} else {
					if(wc < 0x10000) {
						*pDst++ = 0xE0 | (wc>>12);
					} else {
						if(wc < 0x200000) {
							*pDst++ = 0xF0 | (wc>>18);
						} else {
							if(wc < 0x4000000) {
								*pDst++ = 0xF8 | (wc>>24);
							} else /*if(wc < 0x80000000) */{
								*pDst++ = 0xFC | (wc>>30);
								*pDst++ = 0x80 | ((wc>>24) & 0x3F);
							}
							*pDst++ = 0x80 | ((wc>>18) & 0x3F);
						}
						*pDst++ = 0x80 | ((wc>>12) & 0x3F);
					}
					*pDst++ = 0x80 | ((wc>>6) & 0x3F);
				}
				*pDst++ = 0x80 | (wc & 0x3F);
			}
		}

		utf8.resize(pDst - utf8.c_str());
	}

	return utf8;
}

TOOLS_DLL_API std::string TextWToU8(std::wstring& wstr) {
	return TextWToU8(wstr.c_str(), wstr.length());
}

TOOLS_DLL_API std::wstring TextU8ToW(const char *pStr, bool& bValid, int iLength) {
	std::wstring wstr;

	if(pStr) {
		if(iLength < 0) {
			iLength = 0;
			const char *pTemp = pStr;
			while(*pTemp++)
				iLength++;
		}
	
		wstr.resize(iLength);
		const unsigned char *pSrc = (const unsigned char *)pStr;
		unsigned char c;
		uint8 iRequiredChars;
		uint32 wc;
		wchar_t *pDst = (wchar_t *)wstr.c_str();
		bValid = true;
		bool bFirst = true;

		while(iLength--) {
			c = *pSrc++;
			wc = c;
			// Translate to UCS-4 first and then split UTF-32 to UTF-16 surrogates if necessary
			if(c >= 0x80) {
				// 0xC0, 0xC1 and 0xFC aren't valid UTF-8 char since it would
				// make a too-long encoded string ... but we never know
				if((c < 0xC0) || (c > 0xFD) ) {
					bValid = false;
					wc = '?';
				} else {
					iRequiredChars = 0;
					while(c >= 0xC0) {
						c <<= 1;
						iRequiredChars++;
					}
					wc = (c & 0x3F) >> iRequiredChars;
					do {
						if(!iLength-- || (((c=*pSrc++)&0xC0) != 0x80)) {
							bValid = false;
							break;
						}

						wc = (wc << 6) + (c & 0x3F);
					} while(--iRequiredChars);
				}
			}
			if(sizeof(wchar_t) > 2) {
				// keep UCS-4 / UTF-32
				// Combine surrogates if any
				if((wc>=0xDC00) && (wc<0xE000)) {
					// maybe a low surrogate
					uint32 iPrevious;
					if(!bFirst) {
						iPrevious = *(pDst-1);
						if((iPrevious>=0xD800) && (iPrevious<0xDC00)) {
							// There is our high surrogate
							wc = 0x10000 + ((iPrevious & 0x3FF)<<10) + (wc & 0x3FF);
							pDst--;
						} else {
							bValid = false;
							wc = L'?';
						}
					} else {
						bValid = false;
						wc = L'?';
					}
				}
			} else {
				// (try to) convert to UTF-16 (with surrogates)
				if(wc >= 0x10000) {
					assert(wc < 0x110000);

					wc -= 0x10000;
					*pDst++ = 0xD800 | ((wc>>10) & 0x3FF);
					wc = 0xDC00 + (wc & 0x3FF);
				}
			}
			*pDst++ = wc;
			bFirst = false;
		}

		wstr.resize(pDst - wstr.c_str());
	} else
		bValid = true;

	return wstr;
}

TOOLS_DLL_API std::wstring TextU8ToW(std::string& str, bool& bValid) {
	return TextU8ToW(str.c_str(), bValid, str.length());
}

TOOLS_DLL_API std::string TextWToA(std::wstring& wstr) {
	return TextWToA(wstr.c_str(), -1);
}

TOOLS_DLL_API std::wstring TextAToW(std::string& str) {
	return TextAToW(str.c_str(), -1);
}

TOOLS_DLL_API bool IsBlank(std::wstring& str) {
	const wchar_t *pChar = str.c_str();
	wchar_t iCh;
	while((iCh = (*pChar)) != 0) {
		if((iCh!=L' ') && (iCh!=L'\t') && (iCh!=L'\r') && (iCh!=L'\n'))
			return false;
		pChar++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////

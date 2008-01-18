/*
*  The JString class, A Unicode string class
*
*  JString.h
*
*  Copyright (C) Jory Stone - July 2003
*
*
*  The JString class is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2, or (at your option)
*  any later version.
*
*  The JString class is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with The JString class; see the file COPYING.  If not, write to
*  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

/*!
	\file JString.h
	\version \$Id: JString.h,v 1.8 2004/01/12 03:53:14 jcsston Exp $
	\brief A Unicode string class
	\author Jory Stone     <jcsston @ toughguy.net>
*/

#ifndef _JSTRING_H_
#define _JSTRING_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <vector>
#include <ctype.h>

#include <sstream>
#include <string>

#ifdef USE_VC_TYPES
typedef unsigned char binary;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef __int64 int64;
typedef __int32 int32;
typedef __int16 int16;
#else
#include "ebml/EbmlTypes.h"
using namespace LIBEBML_NAMESPACE;
#endif

class JString
{
public:
	JString();
	JString(unsigned long length);
	JString(const wchar_t *value);
	JString(const char *value);	
	JString(const JString &value);
	~JString();
	/// Append string to end of current string
	JString & append(const wchar_t *value);
	JString & append(const char *value);
	JString & append(const wchar_t value);
	JString & append(const char value);
	/// Function to clear all the data
	void clear();

	/// Get the count that a char appears in the string
	size_t Count(wchar_t value);
	
	long Find(const wchar_t *value, bool bBackwards = false);
	long Find(wchar_t value, bool bBackwards = false);

  /// Get all characters before the first occurance of ch
  /// (returns the whole string if ch not found)
  JString BeforeFirst(wchar_t ch);
  /// Get all characters before the last occurence of ch
  /// (returns empty string if ch not found)
  JString BeforeLast(wchar_t ch);
  /// Get all characters after the first occurence of ch
  /// (returns empty string if ch not found)
  JString AfterFirst(wchar_t ch);
  /// Get all characters after the last occurence of ch
	/// (returns the whole string if ch not found)
  JString AfterLast(wchar_t ch);


	JString Left(unsigned long count);
	JString Right(unsigned long count);
	JString Mid(unsigned long start, unsigned long count);
	/// printf style Formating function
	static JString Format(const wchar_t *value, ...);
	static JString Format(const char *value, ...);

	//Convert to other datatypes
	long toLong();
	double toDouble();

	/// Line break types
	enum LineBreakType {
		DosLineBreaks,
		UnixLineBreaks,
		MacLineBreaks
	};
	/// Find linebreak type
	int GetLineBreakType();
	/// Convert to linebreak type
	JString & Convert2Dos();
	/// Copy the string to a char *
	void CopyTo(wchar_t **destStr);
	void CopyTo(char **destStr);
	/// Standard c_str() function
	const wchar_t* c_str() const {return m_Data;}
	/// Return ASCII const char *
	const char* mb_str() {_UpdateASCII(); return m_DataASCII;}
	/// returns wchar_t or char depending on if UNICODE is defined
#ifdef UNICODE
	const wchar_t* t_str() {return c_str();}
#else
	const char* t_str() {return mb_str();}	
#endif
	/// Return length of string
	unsigned long length() {return m_Length;}
	unsigned long Length() {return m_Length;}

	//Operators
	JString & operator=(const JString &value) {_Set(value.c_str()); return *this;};
	JString & operator=(const wchar_t *value) {_Set(value); return *this;};
	JString & operator=(const char *value) {_Set(value); return *this;};
	JString & operator=(wchar_t value) {_Set(value); return *this;};
	JString & operator=(char value) {_Set(value); return *this;};
	JString & operator=(const binary *value) {_Set(value); return *this;};

	JString & operator+=(const JString &value) {append(value.c_str()); return *this;};
	JString & operator+=(const wchar_t *value) {append(value); return *this;};
	JString & operator+=(const char *value) {append(value); return *this;};
	JString & operator+=(wchar_t value) {append(value); return *this;};
	JString & operator+=(char value) {append(value); return *this;};	

	JString & operator<<(const wchar_t *value) {append(value); return *this;};
	JString & operator<<(wchar_t value) {append(value); return *this;};
	JString & operator<<(const char *value) {append(value); return *this;};
	JString & operator<<(char value) {append(value); return *this;};

	JString operator+(const wchar_t *value) {append(value); return *this;};
	JString operator+(const char *value) {append(value); return *this;};
	JString operator+(wchar_t value) {append(value); return *this;};
	JString operator+(char value) {append(value); return *this;};

	wchar_t operator[](size_t index) const {assert(index < m_Length); return m_Data[index];}
	char operator[](size_t index) {assert(index < m_Length); _UpdateASCII(); return m_DataASCII[index];}

	operator const wchar_t*() const {return c_str();}		

protected:
	/// Function to clear all the data, (for consuctor only)
	inline void _Initialize();
	/// Checks if the ASCII copy of the data needs to be updated
	void _UpdateASCII();	
	//void _Set(const JString &value);
	inline void _Set(const wchar_t *value);
	inline void _Set(const char *value);
	inline void _Set(const wchar_t value);
	inline void _Set(const char value);
	void _Set(const binary *value);

	/// Get the length of a NULL-terminated string, being simple 1-line loops these can benfit from inlining
	//inline static size_t strlen(const wchar_t *str);
	//inline static size_t strlen(const char *str);
	/// Removes (trims) leading whitespace characters from a string
	static wchar_t *wslefttrim(wchar_t *source);
	/// Removes (trims) trailing whitespace characters from a string
	static wchar_t *wsrighttrim(wchar_t *source);

	/// The string data itself
	wchar_t *m_Data;
	/// Flag true if ASCII data needs to be updated
	bool m_bUpdateASCII;
	/// ASCII version of data
	char *m_DataASCII;
	/// Length of the string, cached to faster access
	unsigned long m_Length;
	
	/// Line break type
	enum LineBreakType m_lineBreak;

	/// String Data class
	/*class JStringData {
	public:
		JStringData() {
			m_Data = NULL;
			m_DataASCII = NULL;
			m_Length = 0;
			m_RefCount = 0;
		};
		~JStringData() {
			delete m_Data;
			delete m_DataASCII;
		};
		unsigned short AddRef() { return ++m_RefCount; }
		unsigned short Release() { 
			if (--m_RefCount)
				return m_RefCount;

			delete this;
			return 0;
		};
		
		/// The string data itself
		wchar_t *m_Data;
		/// Flag true if ASCII data needs to be updated
		bool m_bUpdateASCII;
		/// ASCII version of data
		char *m_DataASCII;
		/// Length of the string, cached to faster access
		unsigned long m_Length;		
	protected:
		unsigned short m_RefCount;
	};
	JStringData *m_JData;*/
	/// UTF-8 to Unicode conversion class
	class UTF8toUCS4 {
	public:
		enum { // return codes from put.
			need_more = 0,
			complete_code,
			invalid_code,
		};
		UTF8toUCS4() : m_code(0), m_shift(0), m_mval(0) {}

		/// reset the statemachine.
		void reset() { m_code = m_shift = m_mval = 0; }

		/// Read a byte and convert it, if we have a complete code we place it in val and return complete_code.
		/// otherwise, if any error we return invalid_code and do not modify state.
		/// otherwise if no error we update state and return need_more.
		int Convert(binary uc, uint64 & val);

	protected:
		/// the code we are currently building.
		int m_code;
		/// how much to shift in the next bits.
		int m_shift;
		/// minimum value.
		int m_mval; 
	};
};

class SmartStringFormat : public std::wostringstream
{
public:
	SmartStringFormat();
	SmartStringFormat(const SmartStringFormat& copy);
	~SmartStringFormat();

	SmartStringFormat & operator << (const char *inStr);
	SmartStringFormat & operator << (double value);
	SmartStringFormat & operator << (float value);
	SmartStringFormat & operator << (int16 value);
	SmartStringFormat & operator << (int32 value);
	SmartStringFormat & operator << (int64 value);
	SmartStringFormat & operator << (uint16 value);
	SmartStringFormat & operator << (uint32 value);
	SmartStringFormat & operator << (uint64 value);

protected:
	wchar_t *m_TextBuffer;
	unsigned long m_TextBufferSize;
};

#endif //_JSTRING_H_

/*
*  The JString class, A Unicode string class
*
*  JString.cpp
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
	\file JString.cpp
	\version \$Id: JString.cpp,v 1.11 2004/03/06 09:10:04 jcsston Exp $
	\brief A Unicode string class
	\author Jory Stone     <jcsston @ toughguy.net>
*/

#include "JString.h"

#ifndef snwprintf
#define snwprintf _snwprintf
#endif

JString::JString()
{
	_Initialize();
	clear();
};

JString::JString(unsigned long length)
{
	m_Length = length;
	m_Data = new wchar_t[length+1];
	m_Data[0] = 0;
};

JString::JString(const wchar_t *value)
{
	_Initialize();
	if (value == NULL)
		value = L"";
	_Set(value);
};

JString::JString(const char *value)
{
	_Initialize();
	if (value == NULL)
		value = "";
	_Set(value);
};	

JString::JString(const JString &value)
{
	_Initialize();
	_Set(value.c_str());	
};

JString::~JString()
{
	delete m_Data;	
	delete m_DataASCII;
};

JString & JString::append(const wchar_t *value)
{
	wchar_t *oldData = m_Data;
	//m_OldData.AddItem((void *)oldData);
	m_Data = new wchar_t[m_Length+wcslen(value)+1];
	wcscpy(m_Data, oldData);	
	delete oldData;

	wcscat(m_Data, value);
	m_Length = wcslen(m_Data);

	m_bUpdateASCII = true;

	return *this;
};

JString & JString::append(const char *value)
{
	wchar_t *oldData = m_Data;
	//m_OldData.AddItem((void *)oldData);
	m_Data = new wchar_t[m_Length+strlen(value)+1];
	wcscpy(m_Data, oldData);	
	delete oldData;

	wchar_t *convertBuffer = new wchar_t[strlen(value)+1];
	mbstowcs(convertBuffer, value, strlen(value)+1);	

	wcscat(m_Data, convertBuffer);	
	m_Length = wcslen(m_Data);
	delete convertBuffer;

	m_bUpdateASCII = true;

	return *this;
};

JString & JString::append(const wchar_t value)
{
	wchar_t *oldData = m_Data;
	//m_OldData.AddItem((void *)oldData);

	m_Data = new wchar_t[m_Length+1+1];
	wcscpy(m_Data, oldData);	
	delete oldData;
	m_Data[m_Length+1] = value;
	m_Data[m_Length+2] = 0;

	m_Length = wcslen(m_Data);

	m_bUpdateASCII = true;

	return *this;
};

JString & JString::append(const char value)
{
	wchar_t *oldData = m_Data;
	//m_OldData.AddItem((void *)oldData);

	m_Data = new wchar_t[m_Length+1+1];
	wcscpy(m_Data, oldData);	
	delete oldData;
	m_Data[m_Length] = value;
	m_Data[m_Length+1] = 0;

	m_Length = wcslen(m_Data);

	m_bUpdateASCII = true;

	return *this;
};

void JString::clear()
{
	delete m_Data;
	delete m_DataASCII;

	m_Length = 0;
	m_Data = new wchar_t[1];
	m_Data[0] = 0;
};

JString JString::Left(unsigned long count)
{
	if (count > m_Length)
		count = m_Length;

	wchar_t *newData = new wchar_t[count+1];

	for (unsigned long c = 0; c < count; c++)
		newData[c] = m_Data[c];
	newData[count] = 0;

	JString newString(newData);
	delete newData;
	return newString;
};

JString JString::Right(unsigned long count)
{
	if (count > m_Length)
		count = m_Length;

	wchar_t *newData = new wchar_t[count+1];

	unsigned long r = 0;
	for (unsigned long c = count; c <= count; c--)
	{
		newData[r++] = m_Data[m_Length-c];	
	}

	JString newString(newData);
	delete newData;
	return newString;
};

JString JString::Mid(unsigned long start, unsigned long count)
{
	if (start+count > m_Length)
		count = m_Length-start;

	wchar_t *newData = new wchar_t[count+1];

	for (unsigned long c = start; c < count; c++)
		newData[c] = m_Data[c];
	newData[count] = 0;

	JString newString(newData);
	delete newData;
	return newString;	
};

size_t JString::Count(wchar_t value)
{
	size_t c = 0;

	for (size_t i = 0; i <= m_Length; i++)
		if (m_Data[i] == value)
			c++;
	
	return c;
};

long JString::Find(wchar_t value, bool bBackwards)
{
	bool bFound = false;
	size_t i;

	if (bBackwards) {
		for (i = m_Length; i >= 0; i--)
			if (m_Data[i] == value)
			{
				bFound = true;
				break;	
			}
	}else {
		for (i = 0; i <= m_Length; i++)
			if (m_Data[i] == value)
			{
				bFound = true;
				break;	
			}
	}

	if (bFound)
		return i;

	return -1;
};

long JString::Find(const wchar_t *value, bool bBackwards)
{
	bool bFound = false;
	size_t i;
	size_t vLen = wcslen(value);
	if (bBackwards) {
		for (i = m_Length-vLen; i >= 0; i--)
		{
			bFound = true;
			for (size_t c = 0; c < vLen; c++)
				bFound = bFound && (m_Data[i+c] == value[c]);
				
			if(bFound)
				break;	
			else
				bFound = false;
	}
	}else {
		for (i = 0; i <= m_Length-vLen; i++)
		{
			bFound = !_wcsnicmp(m_Data+i, value, vLen);
			
			//for (size_t c = 0; c < vLen; c++)
			//	bFound = bFound && (m_Data[i+c] == value[c]);
				
			if(bFound)
				break;	
			else
				bFound = false;
		}
	}

	if (bFound)
		return i;

	return -1;
};

JString JString::BeforeFirst(wchar_t ch)
{
	long i = Find(ch);
	if (i != -1)
	{
		return Left(i);
	}
	return JString(L"");
};

JString JString::BeforeLast(wchar_t ch)
{
	long i = Find(ch, true);
	if (i != 0)
	{
		return Left(i);
	}	
	return JString(L"");
};

JString JString::AfterFirst(wchar_t ch)
{
	long i = Find(ch);
	if (i > 0)
	{
		return Right(m_Length-i-1);
	}	
	return JString(L"");
};

JString JString::AfterLast(wchar_t ch)
{
	long i = Find(ch, true);
	if (i > 0)
	{
		return Right(m_Length-i-1);
	}	
	return JString(L"");
};

JString JString::Format(const wchar_t *value, ...)
{
	va_list argptr;

	//Get a pointer to the list of arg
	va_start(argptr, value);	

	size_t size = 1024;
	wchar_t *data = new wchar_t[size+1];

	_vsnwprintf(data, size, value, argptr);
	va_end(argptr);

	JString dataString(data);
	delete data;
	return dataString;
};

JString JString::Format(const char *value, ...)
{
	va_list argptr;

	//Get a pointer to the list of arg
	va_start(argptr, value);	

	size_t size = 1024;
	char *data = new char[size];

	_vsnprintf(data, size, value, argptr);
	va_end(argptr);

	JString dataString(data);
	delete data;
	return dataString;
};

long JString::toLong()
{		
	return atol(this->mb_str());
};

double JString::toDouble() 
{	
	return atof(this->mb_str());
};

int JString::GetLineBreakType()
{
	//First search for DOS line breaks
	long lineBreakPos = this->Find(L"\r\n");
	if (lineBreakPos != -1) {
		m_lineBreak = DosLineBreaks;
	} else {
		//Search for Unix style line breaks
		lineBreakPos = this->Find(L'\n');
		if (lineBreakPos != -1) {
			m_lineBreak = UnixLineBreaks;
		} else {
			//Search for Mac? style line breaks
			lineBreakPos = this->Find(L'\r');
			if (lineBreakPos != -1) {
				m_lineBreak = MacLineBreaks;
			}
		}
	}
		
	return m_lineBreak;
};

JString &JString::Convert2Dos()
{
	GetLineBreakType();
	
	return *this;
};

void JString::CopyTo(wchar_t **destStr)
{
	*destStr = new wchar_t[m_Length+1];
	memcpy(*destStr, c_str(), m_Length);
	*destStr[m_Length] = L'\0';
};

void JString::CopyTo(char **destStr)
{
	*destStr = new char[m_Length+1];
	memcpy(*destStr, mb_str(), m_Length);
	*destStr[m_Length] = '\0';
};

wchar_t *JString::wslefttrim(wchar_t *source)
{
	wchar_t *cp = source;

	if (cp && *cp)
	{
		// find first non-whitespace character
		while (iswspace(*cp))
			cp++;

		if (cp != source)
			memcpy(source, cp, (wcslen(cp)+1)*sizeof(wchar_t));
	}

	return source;
}

wchar_t *JString::wsrighttrim(wchar_t *source)
{
	wchar_t *cp = source;

	if (cp && *cp)
	{
		bool bNonSpaceSeen = false;

		// check if string is blank
		while (*cp)
		{
			if (!iswspace(*cp))
				bNonSpaceSeen = true;
			cp++;
		}

		if (bNonSpaceSeen)
		{
			cp--;

			// find last non-whitespace character
			while ((cp >= source) && (iswspace(*cp)))
				*cp-- = L'\0';
		}
		else
		{
			// string contains only whitespace characters
			*source = L'\0';
		}
	}

	return source;
}

void JString::_Initialize()
{	
	//m_JData = NULL;
	m_Data = NULL;
	m_DataASCII = NULL;
	m_Length = 0;	
	m_bUpdateASCII = false;
};

/*void JString::_Set(const JString &value)
{
	this->m_JData = value.m_JData;
	this->m_JData->AddRef();
};*/

void JString::_Set(const wchar_t *value)
{
	m_Length = wcslen(value);
	
	//m_OldData.AddItem((void *)m_Data);
	delete m_Data;
	m_Data = new wchar_t[m_Length+2];
	wcsncpy(m_Data, value, m_Length+1);

	m_bUpdateASCII = true;	
};	

void JString::_Set(const char *value)
{
	m_Length = strlen(value);

	//m_OldData.AddItem((void *)m_Data);
	delete m_Data;
	m_Data = new wchar_t[m_Length+2];
	mbstowcs(m_Data, value, m_Length+1);	

	m_bUpdateASCII = true;
};	

void JString::_Set(const wchar_t value)
{
	//m_OldData.AddItem((void *)oldData);
	delete m_Data;

	m_Length = 1;
	m_Data = new wchar_t[2];
	m_Data[0] = value;
	m_Data[1] = 0;

	m_bUpdateASCII = true;
};	

void JString::_Set(const char value)
{
	//m_OldData.AddItem((void *)oldData);
	delete m_Data;

	m_Length = 1;
	m_Data = new wchar_t[2];
	m_Data[0] = value;
	m_Data[1] = 0;

	m_bUpdateASCII = true;	
};	

void JString::_Set(const binary *value)
{
	//When we don't know the type of the string, UTF-8, Unicode, ASCII
	if (value[0] == 0xff && value[1] == 0xfe) { //Unicode
		_Set((wchar_t *)value);

	} else if (value[0] == 0xef && value[1] == 0xbb && value[2] == 0xbf) { //UTF-8
		UTF8toUCS4 utfConversion;
		std::vector<uint64> unicodeString;
		unsigned long b = 0;
		int state = UTF8toUCS4::need_more;
		while (state != UTF8toUCS4::invalid_code)
		{
			uint64 unicodeChar = 0;
			while (state == UTF8toUCS4::need_more)
			{			
				state = utfConversion.Convert(value[b], unicodeChar);
				b++;			
			}
			if (state == UTF8toUCS4::complete_code)
			{
				unicodeString.push_back(unicodeChar);
				state = UTF8toUCS4::need_more;
			}
		}
		delete m_Data;
		m_Data = new wchar_t[unicodeString.size()+1];
		for (b = 0; b < unicodeString.size()-1; b++)
			m_Data[b] = (wchar_t)unicodeString.at(b+1);		

	} else { //Assuming ASCII
		_Set((char *)value);
	}
};

void JString::_UpdateASCII()
{
	if (m_bUpdateASCII)
	{
		//m_OldData.AddItem((void *)m_DataASCII);
		delete m_DataASCII;

		m_DataASCII = new char[m_Length+1];
		wcstombs(m_DataASCII, m_Data, m_Length+1);

		m_bUpdateASCII = false;
	}
};

int JString::UTF8toUCS4::Convert(binary uc, uint64 &val)
{	
	// if m_shift == 0 we expect either a single byte or a first byte, otherwise we require a follow byte.
	if (m_shift == 0) {
		int code;
		int mval;
		int shift;

		if (uc < 0x80) { // complete code.
			val = uc;
			return complete_code;
		} else if (uc < 0xc0) { // follow byte here, error.
			return invalid_code;
		} else if (uc < 0xe0) { // 0xc0..0xdf, two byte code.
			code = (uc & 0x1f) << 6;
			shift = 6;
			mval = 0x80;
		} else if (uc < 0xf0) { // 0xe0..0xef, three byte code.
			code = (uc & 0x0f) << 12;
			shift = 12;
			mval = 0x800;
		} else if (uc < 0xf8) { // 0xf0..0xf7, four byte code (last unicode byte).
			code = (uc & 0x07) << 18;
			shift = 18;
			mval = 0x10000;
		} else if (uc < 0xfc) { // 0xf8..0xfb, five byte code.
			code = (uc & 0x03) << 24;
			shift = 24;
			mval = 0x200000;
		} else if (uc < 0xfe) { // 0xfc..0xfd, six byte code.
			code = (uc & 0x01) << 30;
			shift = 30;
			mval = 0x4000000;
		} else { // invalid codes.
			return invalid_code;
		}
		if (code < mval)
			return invalid_code;
		m_code = code;
		m_shift = shift;
		m_mval = mval;
		return need_more;
	} else if (uc < 0x80 || uc >= 0xc0) { // single or first byte here???
		return invalid_code;
	} else {
		int shift = m_shift - 6;
		int code = m_code | ((uc & 0x3f) << shift);

		if (code < m_mval) // invalid code.
			return invalid_code;

		m_shift = shift;
		if (shift == 0) { // complete code.
			val = code;
			m_code = m_mval = 0;
			return complete_code;
		}
		m_code = code;
		return need_more;
	}
};

SmartStringFormat::SmartStringFormat() {
	m_TextBuffer = new wchar_t[256];
	m_TextBufferSize = 255;
}

SmartStringFormat::SmartStringFormat(const SmartStringFormat& copy) {
	SmartStringFormat();
	write(copy.str().c_str(), copy.str().length());
};

SmartStringFormat::~SmartStringFormat() {
	if (m_TextBuffer != NULL)
		delete [] m_TextBuffer;
}; 

SmartStringFormat & SmartStringFormat::operator << (const char *inStr) {
	unsigned long inStrLength = strlen(inStr);
	if (inStrLength > m_TextBufferSize || m_TextBuffer == NULL) {
		if (m_TextBuffer != NULL)
			delete [] m_TextBuffer;

		m_TextBufferSize = inStrLength+2;
		m_TextBuffer = new wchar_t[m_TextBufferSize+2];
	
	}	
	mbstowcs(m_TextBuffer, inStr, inStrLength+1);
	
	write(m_TextBuffer, wcslen(m_TextBuffer));

	return *this;
};

SmartStringFormat & SmartStringFormat::operator << (double value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%f", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (float value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%f", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (int16 value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%hi", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (int32 value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%li", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (int64 value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%I64i", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (uint16 value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%hu", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (uint32 value)
{
	int strLength = _snwprintf(m_TextBuffer, m_TextBufferSize, L"%lu", value);
	write(m_TextBuffer, strLength);

	return *this;	
};

SmartStringFormat & SmartStringFormat::operator << (uint64 value)
{
	int strLength = snwprintf(m_TextBuffer, m_TextBufferSize, L"%I64u", value);
	write(m_TextBuffer, strLength);

	return *this;	
};


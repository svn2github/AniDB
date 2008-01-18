/*
 * Copyright (C) 2001-2005 Jacek Sieka, arnetheduck on gmail point com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#if !defined(EXCEPTION_H)
#define EXCEPTION_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Exception  
{
public:
	Exception() { };
	Exception(const string& aError) throw() : error(aError) { dcdrun(if(error.size()>0)) dcdebug("Thrown: %s\n", error.c_str()); };
	virtual ~Exception() throw() { };
	virtual const string& getError() const throw() { return error; };
protected:
	string error;
};

#ifdef _DEBUG

#define STANDARD_EXCEPTION(name) class name : public Exception { \
public:\
	name() throw() : Exception(#name) { } \
	name(const string& aError) throw() : Exception(#name ": " + aError) { } \
	virtual ~name() throw() { } \
}

#else // _DEBUG

#define STANDARD_EXCEPTION(name) class name : public Exception { \
public:\
	name() throw() : Exception() { } \
	name(const string& aError) throw() : Exception(aError) { } \
	virtual ~name() throw() { } \
}
#endif

#endif // !defined(EXCEPTION_H)

/**
 * @file
 * $Id: Exception.h,v 1.18 2005/04/24 08:13:36 arnetheduck Exp $
 */

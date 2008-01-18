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

#if !defined(HASH_VALUE_H)
#define HASH_VALUE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FastAlloc.h"

template<class Hasher>
struct HashValue : FastAlloc<HashValue<Hasher> >{
	static const size_t SIZE = Hasher::HASH_SIZE;

	typedef HashValue* Ptr;
	struct PtrHash {
		size_t operator()(const Ptr rhs) const { return *(size_t*)rhs; };
		bool operator()(const Ptr lhs, const Ptr rhs) const { return (*lhs) == (*rhs); };
	};
	struct PtrLess {
		int operator()(const Ptr lhs, const Ptr rhs) { return (*lhs) < (*rhs); };
	};

	struct Hash {
		size_t operator()(const HashValue& rhs) const { return *(size_t*)&rhs; };
		bool operator()(const HashValue& lhs, const HashValue& rhs) const { return lhs == rhs; };
	};
	struct Less {
		int operator()(const HashValue& lhs, const HashValue& rhs) { return lhs < rhs; };
	};

	HashValue() { };
	explicit HashValue(u_int8_t* aData) { memcpy(data, aData, SIZE); }
	explicit HashValue(const string& base32) { Encoder::fromBase32(base32.c_str(), data, SIZE); };
	HashValue(const HashValue& rhs) { memcpy(data, rhs.data, SIZE); }
	HashValue& operator=(const HashValue& rhs) { memcpy(data, rhs.data, SIZE); return *this; }
	bool operator!=(const HashValue& rhs) const { return !(*this == rhs); }
	bool operator==(const HashValue& rhs) const { return memcmp(data, rhs.data, SIZE) == 0; }
	bool operator<(const HashValue& rhs) const { return memcmp(data, rhs.data, SIZE) < 0; }

	string toBase32() const { return Encoder::toBase32(data, SIZE); };
	string& toBase32(string& tmp) const { return Encoder::toBase32(data, SIZE, tmp); };

	u_int8_t data[SIZE];
};

#endif // !defined(HASH_VALUE_H)

/**
 * @file
 * $Id: HashValue.h,v 1.11 2005/04/24 08:13:37 arnetheduck Exp $
 */

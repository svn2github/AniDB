/*********************************************************************
 * Common set of tools (classes & functions)
 *
 * Rational Arithmetic
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

#ifndef __TOOLS_FRACTION_H__
#define __TOOLS_FRACTION_H__

#include "common.h"

/*
 * The use of rational numbers help a lot where rounding errors
 * can be a problem.
 *
 * In particular I find the use of such a tool as a great help
 * inside VirtualDub ((c) Avery Lee).
 * This class provides the same features than in VirtualDub so
 * that I don't get lost when using this feature ;)
 * (anyway there aren't that many ways to make them available)
 *
 * Nb: this class works with 64-bits numbers, while in VirtualDub
 * it's only 32-bits. 32-bits are enough in most cases, but I
 * think there are some (rare) cases where it could limit the
 * precision of the results ...
 */
class TOOLS_DLL_API Fraction {
friend Fraction operator*(sint64 iValue, const Fraction fraction);

protected:
	sint64	m_iHi;
	sint64	m_iLo;

public:
	Fraction(void);
	explicit	Fraction(sint64 iValue);
				Fraction(sint64 iHi, sint64 iLo);
	explicit	Fraction(double fValue);
	Fraction(const Fraction& fraction);

	bool	operator<(Fraction fraction) const;
	bool	operator<=(Fraction fraction) const;
	bool	operator>(Fraction fraction) const;
	bool	operator>=(Fraction fraction) const;
	bool	operator==(Fraction fraction) const;
	bool	operator!=(Fraction fraction) const;

	Fraction	operator+(Fraction b) const;
	Fraction	operator-(Fraction b) const;
	Fraction	operator*(Fraction b) const;
	Fraction	operator/(Fraction b) const;

	Fraction	operator*(sint64 iValue) const;
	Fraction	operator/(sint64 iValue) const;

	sint64	scale64t(sint64 iValue) const;
	sint64	scale64r(sint64 iValue) const;
	sint64	scale64u(sint64 iValue) const;
	sint64	scale64it(sint64 iValue) const;
	sint64	scale64ir(sint64 iValue) const;
	sint64	scale64iu(sint64 iValue) const;

	sint64	round64t(void) const;
	sint64	round64r(void) const;
	sint64	round64u(void) const;

	operator	sint64(void) const;
	operator	uint64(void) const;
	operator	double(void) const;

	sint64	getHi(void) { return m_iHi; };
	sint64	getLo(void) { return m_iLo; };

	static Fraction			reduce(sint64 iHi, sint64 iLo);
	static inline Fraction	reduce64(sint64 iHi, sint64 iLo) { return reduce(iHi, iLo); }
	inline void				reduce(void) { *this = reduce(m_iHi, m_iLo); };
};

inline Fraction	operator*(sint64 iValue, const Fraction fraction) { return fraction*iValue; }

#endif // __TOOLS_FRACTION_H__

//////////////////////////////////////////////////////////////////////

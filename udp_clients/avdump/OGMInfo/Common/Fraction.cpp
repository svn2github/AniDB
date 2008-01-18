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

#include "Fraction.h"
#include "math.h"


Fraction Fraction::reduce(sint64 iHi, sint64 iLo) {
	if(iLo == 0)
		return Fraction(0,0);

	if(iHi == 0)
		return Fraction(0,1);

	if(iLo == iHi)
		return Fraction(1,1);

	while(((iHi | iLo) & 1) == 0) {
		iHi >>= 1;
		iLo >>= 1;
	}

	while(((iLo%3)==0) && ((iHi%3)==0)) {
		iLo /= 3;
		iHi /= 3;
	}

	sint64 A, B, C;
	if(iLo > iHi) {
		A = iLo;
		B = iHi;
	} else {
		A = iHi;
		B = iLo;
	}

	do {
		C = A % B;
		A = B;
		B = C;
	} while(B > 0);

	iLo /= A;
	iHi /= A;

	return Fraction(iHi, iLo);
}

Fraction::Fraction(void)
: m_iHi(0)
, m_iLo(1)
{
}

Fraction::Fraction(sint64 iValue)
: m_iHi(iValue)
, m_iLo(1)
{
}

Fraction::Fraction(sint64 iHi, sint64 iLo)
: m_iHi(iHi)
, m_iLo(iLo)
{
}

Fraction::Fraction(double fValue) {
	int xp;
	double mant = frexp(fValue, &xp);

	// fValue = mant * 2 ^ xp;
	// 0.5 <= mant < 1.0

	if(mant == 0.5) {
		xp--;
		if(xp > 62)
			m_iHi = _LL(0x7FFFFFFFFFFFFFFF);
		else
			m_iHi = sint64(1) << xp;
		m_iLo = 1;
	} else if(xp > 63) {
		m_iHi = _LL(0x7FFFFFFFFFFFFFFF);
		m_iLo = 1;
	} else if (xp < -63) {
		m_iHi = 0;
		m_iLo = 1;
	} else if (xp >= 0) {
		*this = reduce(sint64(0.5 + ldexp(mant, 62)), _LL(1)<<(62-xp));
	} else {
		*this = reduce(sint64(0.5 + ldexp(mant, xp+62)), _LL(1)<<62);
	}
}

Fraction::Fraction(const Fraction& fraction) {
	m_iHi = fraction.m_iHi;
	m_iLo = fraction.m_iLo;
}

//////////////////////////////////////////////////////////////////////

bool Fraction::operator==(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo == m_iLo * fraction.m_iHi);
}

bool Fraction::operator!=(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo != m_iLo * fraction.m_iHi);
}

bool Fraction::operator<(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo < m_iLo * fraction.m_iHi);
}

bool Fraction::operator<=(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo <= m_iLo * fraction.m_iHi);
}

bool Fraction::operator>(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo > m_iLo * fraction.m_iHi);
}

bool Fraction::operator>=(Fraction fraction) const {
	return (m_iHi * fraction.m_iLo >= m_iLo * fraction.m_iHi);
}

//////////////////////////////////////////////////////////////////////

Fraction Fraction::operator+(Fraction fraction) const {
	return reduce(m_iHi * fraction.m_iLo + m_iLo * fraction.m_iHi, m_iLo * fraction.m_iLo);
}

Fraction Fraction::operator-(Fraction fraction) const {
	return reduce(m_iHi * fraction.m_iLo - m_iLo * fraction.m_iHi, m_iLo * fraction.m_iLo);
}

Fraction Fraction::operator*(Fraction fraction) const {
	return reduce(m_iHi * fraction.m_iHi, m_iLo * fraction.m_iLo);
}

Fraction Fraction::operator/(Fraction fraction) const {
	return reduce(m_iHi * fraction.m_iLo, m_iLo * fraction.m_iHi);
}

Fraction Fraction::operator*(sint64 iValue) const {
	return reduce(m_iHi * iValue, m_iLo);
}

Fraction Fraction::operator/(sint64 iValue) const {
	return reduce(m_iHi, m_iLo * iValue);
}

//////////////////////////////////////////////////////////////////////

sint64 Fraction::scale64t(sint64 iValue) const {
	assert(m_iLo);

	return (m_iHi * iValue) / m_iLo;
}

sint64 Fraction::scale64r(sint64 iValue) const {
	assert(m_iLo);

	return (m_iHi * iValue + (m_iLo >> 1)) / m_iLo;
}

sint64 Fraction::scale64u(sint64 iValue) const {
	assert(m_iLo);

	return (m_iHi * iValue + m_iLo - 1) / m_iLo;
}

sint64 Fraction::scale64it(sint64 iValue) const {
	assert(m_iHi);

	return (m_iLo * iValue) / m_iHi;
}

sint64 Fraction::scale64ir(sint64 iValue) const {
	assert(m_iHi);

	return (m_iLo * iValue + (m_iHi >> 1)) / m_iHi;
}

sint64 Fraction::scale64iu(sint64 iValue) const {
	assert(m_iHi);

	return (m_iLo * iValue + m_iHi - 1) / m_iHi;
}

sint64 Fraction::round64t(void) const {
	assert(m_iLo);

	return m_iHi / m_iLo;
}

sint64 Fraction::round64r(void) const {
	assert(m_iLo);

	return (m_iHi + (m_iLo >> 1)) / m_iLo;
}

sint64 Fraction::round64u(void) const {
	assert(m_iLo);

	return (m_iHi + m_iLo - 1) / m_iLo;
}

//////////////////////////////////////////////////////////////////////

Fraction::operator sint64(void) const {
	assert(m_iLo);

	return (m_iHi + (m_iLo >> 1)) / m_iLo;
}

Fraction::operator uint64(void) const {
	assert(m_iLo);

	return uint64((m_iHi + (m_iLo >> 1)) / m_iLo);
}

Fraction::operator double(void) const {
	assert(m_iLo);

	return (double(m_iHi) / double(m_iLo));
}

//////////////////////////////////////////////////////////////////////

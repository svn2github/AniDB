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

#ifndef _DBG_OUT_H_
#define _DBG_OUT_H_

#include <windows.h>

#ifdef _DEBUG
#define DEBUG_OUT_PUT
#endif

#ifdef DEBUG_OUT_PUT
void WINAPI DbgStringOut(const TCHAR *pFormat,...);
#ifdef _UNICODE
void WINAPI DbgStringOut(const CHAR *pFormat,...);
#endif
#define NOTE(x) DbgStringOut(x)
#define NOTE1(x, y1) DbgStringOut(x, y1)
#define NOTE2(x, y1, y2) DbgStringOut(x, y1, y2)
#define NOTE3(x, y1, y2, y3) DbgStringOut(x, y1, y2, y3)
#define NOTE4(x, y1, y2, y3, y4) DbgStringOut(x, y1, y2, y3, y4)
#define NOTE5(x, y1, y2, y3, y4, y5) DbgStringOut(x, y1, y2, y3, y4, y5)
#define NOTE6(x, y1, y2, y3, y4, y5, y6) DbgStringOut(x, y1, y2, y3, y4, y5, y6)
#define NOTE10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10) DbgStringOut(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#define ODS(x) DbgStringOut(x)
#define ODS1(x, y1) DbgStringOut(x, y1)
#define ODS2(x, y1, y2) DbgStringOut(x, y1, y2)
#define ODS3(x, y1, y2, y3) DbgStringOut(x, y1, y2, y3)
#define ODS4(x, y1, y2, y3, y4) DbgStringOut(x, y1, y2, y3, y4)
#define ODS5(x, y1, y2, y3, y4, y5) DbgStringOut(x, y1, y2, y3, y4, y5)
#define ODS6(x, y1, y2, y3, y4, y5, y6) DbgStringOut(x, y1, y2, y3, y4, y5, y6)
#define ODS10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10) DbgStringOut(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#define TRACE(x) DbgStringOut(x)
#define TRACE1(x, y1) DbgStringOut(x, y1)
#define TRACE2(x, y1, y2) DbgStringOut(x, y1, y2)
#define TRACE3(x, y1, y2, y3) DbgStringOut(x, y1, y2, y3)
#define TRACE4(x, y1, y2, y3, y4) DbgStringOut(x, y1, y2, y3, y4)
#define TRACE5(x, y1, y2, y3, y4, y5) DbgStringOut(x, y1, y2, y3, y4, y5)
#define TRACE6(x, y1, y2, y3, y4, y5, y6) DbgStringOut(x, y1, y2, y3, y4, y5, y6)
#define TRACE10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10) DbgStringOut(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#else
#define NOTE(x)
#define NOTE1(x, y1)
#define NOTE2(x, y1, y2)
#define NOTE3(x, y1, y2, y3)
#define NOTE4(x, y1, y2, y3)
#define NOTE5(x, y1, y2, y3, y4, y5)
#define NOTE6(x, y1, y2, y3, y4, y5, y6)
#define NOTE10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#define ODS(x)
#define ODS1(x, y1)
#define ODS2(x, y1, y2)
#define ODS3(x, y1, y2, y3)
#define ODS4(x, y1, y2, y3, y4)
#define ODS5(x, y1, y2, y3, y4, y5)
#define ODS6(x, y1, y2, y3, y4, y5, y6)
#define ODS10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#define TRACE(x)
#define TRACE1(x, y1)
#define TRACE2(x, y1, y2)
#define TRACE3(x, y1, y2, y3)
#define TRACE4(x, y1, y2, y3, y4)
#define TRACE5(x, y1, y2, y3, y4, y5)
#define TRACE6(x, y1, y2, y3, y4, y5, y6)
#define TRACE10(x, y1, y2, y3, y4, y5, y6, y7, y8, y9, y10)
#endif


#endif // _DBG_OUT_H_

/*
 *  Part of The TCMP Matroska CDL, and Matroska Shell Extension
 *
 *  MatroskaUtilsReader.h
 *
 *  Copyright (C) Jory Stone - October 2003
 *
 *  Parts of the XCD parsing code based on code from alexnoe's AVIMux-GUI
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
    \file MatroskaUtilsReader.h
		\version \$Id: MatroskaUtilsReader.h,v 1.6 2004/02/18 06:52:18 jcsston Exp $
    \brief File IOCallback class Reader, supports XCD
		\author Jory Stone     <jcsston @ toughguy.net>

*/
#ifndef _MATROSKAUTILS_READER_H_
#define _MATROSKAUTILS_READER_H_

//#include "JString.h"
#include "ebml/EbmlCrc32.h"
#include "ebml/IOCallback.h"
#include "ebml/StdIOCallback.h"
#include "ebml/MemIOCallback.h"
#ifdef WIN32
#include "WinIOCallback.h"
#endif // WIN32

using namespace LIBEBML_NAMESPACE;

//namespace MatroskaUtilsNamespace {

#define CHECK_XCD_HEADER(x) \
  (x[0] == 0x00 && x[1] == 0xFF && x[2] == 0xFF && x[3] == 0xFF && \
  x[4] == 0xFF && x[5] == 0xFF && x[6] == 0xFF && x[7] == 0xFF && \
  x[8] == 0xFF && x[9] == 0xFF && x[10] == 0xFF && x[11] == 0x00)

#define XCD_SECTOR_SIZE 2352
#define XCD_SECTOR_DATA_SIZE 2324

#define MakeFourCC(chStr)                              \
                ((DWORD)(BYTE)(chStr[0]) | ((DWORD)(BYTE)(chStr[1]) << 8) |   \
                ((DWORD)(BYTE)(chStr[2]) << 16) | ((DWORD)(BYTE)(chStr[3]) << 24 ))

struct CHUNKHEADER
{
	DWORD	dwFourCC;
	DWORD dwLength;
};

struct LISTHEADER
{
	DWORD	dwListID;
	DWORD dwLength;
	DWORD dwFourCC;
};

struct XCDSECTOR {
  byte sync[12];
  byte header[4];
  byte subheader[8];
  byte data[2324];
  byte edc[4];
};

class MatroskaUilsFileReader : public IOCallback
{
public:
	MatroskaUilsFileReader(const char *Path, const open_mode Mode);
	MatroskaUilsFileReader(const wchar_t *Path, const open_mode Mode);
	~MatroskaUilsFileReader();

	virtual uint32 read(void *Buffer, size_t Size);
	virtual size_t write(const void *Buffer, size_t Size);
	virtual void setFilePointer(int64 Offset, seek_mode Mode=seek_beginning);
	virtual uint64 getFilePointer();
	virtual void close();
  
  virtual bool IsOk() { return m_fileHandle->IsOk(); };
  virtual bool IsXCD() { return m_IsXCD; };
  
	WinIOCallback *GetWinIOCallback() { return m_fileHandle; };
protected:
  inline void Setup();
  inline bool ReadNextXCDSector();
  
	WinIOCallback *m_fileHandle;
	// XCD stuff
	bool m_IsXCD;
	bool b_bCheckCRC32;
	// The translated file size/pos if the source is a XCD
	uint64 m_VirtualFileSize;
	uint64 m_VirtualFilePos;
  uint32 m_SectorCount;
	uint32 m_CurrentSectorNo;
	uint16 m_CurrentSectorDataPos;
	XCDSECTOR m_CurrentSector;
};

//}; //End MatroskaUtilsNamespace

#endif // _MATROSKAUTILS_READER_H_

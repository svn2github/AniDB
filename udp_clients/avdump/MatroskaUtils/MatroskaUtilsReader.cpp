/*
 *  Part of The TCMP Matroska CDL, and Matroska Shell Extension
 *
 *  MatroskaUtilsReader.cpp
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
    \file MatroskaUtilsReader.cpp
		\version \$Id: MatroskaUtilsReader.cpp,v 1.5 2004/03/06 09:10:05 jcsston Exp $
    \brief File IOCallback class Reader, supports XCD
		\author Jory Stone     <jcsston @ toughguy.net>

*/

#include "MatroskaUtilsReader.h"

//using namespace MatroskaUtilsNamespace;

MatroskaUilsFileReader::MatroskaUilsFileReader(const char *Path, const open_mode Mode){
	m_fileHandle = new WinIOCallback(Path, Mode);
	Setup();
};
MatroskaUilsFileReader::MatroskaUilsFileReader(const wchar_t *Path, const open_mode Mode){
	m_fileHandle = new WinIOCallback(Path, Mode);
	Setup();
};
MatroskaUilsFileReader::~MatroskaUilsFileReader() {
		delete m_fileHandle; 
};
void MatroskaUilsFileReader::Setup() {
  m_IsXCD = false;
	b_bCheckCRC32 = false;
  m_VirtualFileSize = 0;
  m_VirtualFilePos = 0;
  m_SectorCount = 0;
  m_CurrentSectorNo = 0;
  m_CurrentSectorDataPos = 0;
  memset(&m_CurrentSector, 0, sizeof(XCDSECTOR));
  
  if (m_fileHandle->IsOk()) {
    LISTHEADER riffHeader;
    if (m_fileHandle->read(&riffHeader, sizeof(LISTHEADER)) == sizeof(LISTHEADER)) {
      if (riffHeader.dwListID == MakeFourCC("RIFF")
       && riffHeader.dwFourCC == MakeFourCC("CDXA"))
      {
        // XCD format ?
        m_fileHandle->setFilePointer(24, seek_current);
        CHUNKHEADER mainChunkHeader;
        m_fileHandle->read(&mainChunkHeader, sizeof(CHUNKHEADER));
        if (mainChunkHeader.dwFourCC == MakeFourCC("data")) {
          m_SectorCount = mainChunkHeader.dwLength / XCD_SECTOR_SIZE;
          // Calc the true total file size, without the XCD sector overhead
          m_VirtualFileSize = m_SectorCount * XCD_SECTOR_DATA_SIZE;
          if (ReadNextXCDSector()) {
            // Yep, a valid XCD sector
            m_IsXCD = true;
          }
      	}
      }
    }
		// Move back to the beginning
		m_fileHandle->setFilePointer(0);
  }
};  
bool MatroskaUilsFileReader::ReadNextXCDSector() {
	if (m_CurrentSectorNo >= m_SectorCount)
		return false;

  if (m_fileHandle->read(&m_CurrentSector, sizeof(XCDSECTOR)) == 0)
		return false;

  if (CHECK_XCD_HEADER(m_CurrentSector.sync)) {
    m_CurrentSectorNo++;
    m_CurrentSectorDataPos = 0;
		if (b_bCheckCRC32) {
			// Check CRC32
			EbmlCrc32 crc_creator;
			crc_creator.FillCRC32((binary *)&m_CurrentSector, 16+8+2324-1);
			if (*((uint32 *)m_CurrentSector.edc) != crc_creator.GetCrc32()) {
				// Bad CRC32 !
				//ODS("Bad CRC32 in XCD sector!!!");
			}
		}
    return true;
  }

  return false;
};
uint32 MatroskaUilsFileReader::read(void *Buffer, size_t Size) {
  if (!m_IsXCD)
	 return m_fileHandle->read(Buffer, Size);
	else {
		// m_CurrentSectorDataPos is supposed to be the current position of the data inside the XCD sector
		assert(m_CurrentSectorDataPos < XCD_SECTOR_DATA_SIZE);    
    if (m_CurrentSectorDataPos + Size < XCD_SECTOR_DATA_SIZE) {
      memcpy(Buffer, m_CurrentSector.data+m_CurrentSectorDataPos, Size);
			m_CurrentSectorDataPos += Size;
			m_VirtualFilePos += Size;
			return Size;
    } else {
			char *theBuffer = (char *)Buffer;
			uint32 TotalSize = 0;
			uint16 BlockSize = XCD_SECTOR_DATA_SIZE-m_CurrentSectorDataPos;
      // We have to read more than one XCD sector
      // Copy the rest of this sector
      memcpy(theBuffer, m_CurrentSector.data+m_CurrentSectorDataPos, BlockSize);
			// Move the file pos pointer
			m_VirtualFilePos += BlockSize;
      // This is the size of the data copied
      Size -= BlockSize;
			TotalSize += BlockSize;
      // Move the data buffer pointer up
      theBuffer += BlockSize;			
      if (ReadNextXCDSector()) {
				TotalSize += this->read(theBuffer, Size);
			} else {
				return TotalSize;
			}
			return TotalSize;
    }
 }
};
size_t MatroskaUilsFileReader::write(const void *Buffer, size_t Size) {
  if (!m_IsXCD)
	 return m_fileHandle->write(Buffer, Size);
 else
 return 0;
};
void MatroskaUilsFileReader::setFilePointer(int64 Offset, seek_mode Mode) {
  if (!m_IsXCD)
	 m_fileHandle->setFilePointer(Offset, Mode);
	else {
    // Translate the virtual file postion to the true XCD file position
    switch (Mode)
    {
			case seek_beginning:
      {
				if (m_CurrentSectorNo-1 == Offset / XCD_SECTOR_DATA_SIZE) {
					// Same sector, just move the data pos
					if (Offset > 0)						
						m_CurrentSectorDataPos = (uint16)(Offset - ((m_CurrentSectorNo-1) * XCD_SECTOR_DATA_SIZE));
					else
						m_CurrentSectorDataPos = 0;
					m_VirtualFilePos = Offset;
				} else {
					m_CurrentSectorNo = (uint32)(Offset / XCD_SECTOR_DATA_SIZE);
					m_fileHandle->setFilePointer(44 + (m_CurrentSectorNo * XCD_SECTOR_SIZE), seek_beginning);
					if (ReadNextXCDSector()) {
						if (Offset > 0)						
							m_CurrentSectorDataPos = (uint16)(Offset - ((m_CurrentSectorNo-1) * XCD_SECTOR_DATA_SIZE));
						else
							m_CurrentSectorDataPos = 0;
						m_VirtualFilePos = Offset;
					}
				}
        break;
      }
			case seek_current:
      {
        if (m_CurrentSectorDataPos + Offset > 0 && (m_CurrentSectorDataPos + Offset < XCD_SECTOR_DATA_SIZE)) {
          m_CurrentSectorDataPos += (uint16)Offset;
          m_VirtualFilePos += Offset;
        } else {
					Offset += m_VirtualFilePos;
					this->setFilePointer(Offset, seek_beginning);
          /*m_CurrentSectorNo = m_CurrentSectorNo + (Offset / XCD_SECTOR_DATA_SIZE);
          m_fileHandle->setFilePointer(44 + (m_CurrentSectorNo * XCD_SECTOR_SIZE), seek_beginning);
          if (ReadNextXCDSector()) {
            m_CurrentSectorDataPos = Offset - (m_CurrentSectorNo * XCD_SECTOR_DATA_SIZE);
            m_VirtualFilePos += Offset;
          }*/
        }
        break;
      }
			case seek_end:
      {
				Offset += m_VirtualFileSize;
				this->setFilePointer(Offset, seek_beginning);
        break;
      }
    }
 }
};	
uint64 MatroskaUilsFileReader::getFilePointer() {
  if (!m_IsXCD)
	 return m_fileHandle->getFilePointer();
	else {
    return m_VirtualFilePos;
 }
};	
void MatroskaUilsFileReader::close() {
	m_fileHandle->close();
};



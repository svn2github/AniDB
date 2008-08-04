// Copyright (C) 2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifdef ENABLE_HASH

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include <windows.h>
#include <tchar.h>

#include "avhash.h"

#include "MPAHeaderInfo/MPAFile.h"
#include "MPAHeaderInfo/MPAException.h"

const int BUFFER_SIZE = 1024*1024;

void InvalidChange(LPCTSTR szWhat, DWORD dwFrame, LPCTSTR szOldValue, LPCTSTR szNewValue)
{
	wprintf(_T("%s changed in frame %d from %s to %s\r\n"), szWhat, dwFrame, szOldValue, szNewValue);
}

int check_mp3(const wchar_t *path, std::vector<DWORD> &voffset, std::vector<DWORD> &vsize){
	CMPAFile* m_pMPAFile = new CMPAFile(path, 0);

	int nTag = 0;
	CTag* pTag;

	while ((pTag = m_pMPAFile->m_Tags.GetNextTag(nTag)) != NULL){
		wprintf(_T(" %s V%.2f: %d + %d\n"), pTag->GetName(), pTag->GetVersion(), pTag->GetOffset(), pTag->GetSize());
		if(voffset.size()<1){
			voffset.push_back(pTag->GetOffset());
			vsize.push_back(pTag->GetSize());
		}else{
			for(DWORD i=0; i<voffset.size();i++){
				if(voffset[i]>pTag->GetOffset()){
					voffset.insert(voffset.begin()+i,pTag->GetOffset());
					vsize.insert(vsize.begin()+i,pTag->GetSize());
					break;
				}
			}
		}
	}
	if (m_pMPAFile->m_pVBRHeader)
	{			
		wstring strHelp, strHeader;
		CXINGHeader* pXINGHeader;
		CVBRIHeader* pVBRHeader;

		// we need runtime type information for the dynamic_cast -> compile this with /GR switch
		// is it a XING header?
		if ((pXINGHeader = dynamic_cast<CXINGHeader*>(m_pMPAFile->m_pVBRHeader)) != NULL)
		{
			if (m_pMPAFile->m_pVBRHeader->m_dwQuality != -1)
				wprintf(_T(" XING: Quality: %d\n"), m_pMPAFile->m_pVBRHeader->m_dwQuality );
			else
				wprintf(_T(" XING: Quality: not set\n") );
		}
		// is it a VBRI header?
		else if ((pVBRHeader = dynamic_cast<CVBRIHeader*>(m_pMPAFile->m_pVBRHeader)) != NULL)
		{
			wprintf( _T(" VBRI: Version %d\nDelay %.2f\nQuality %d\n"),
				pVBRHeader->m_dwVersion, pVBRHeader->m_fDelay, pVBRHeader->m_dwQuality );	
		}
		
		wprintf(_T(" %s\nOffset: Byte %d\nNumber Of Frames: %d\nFile Size: %d Byte (%d KB)\n"), 
					strHeader,
					m_pMPAFile->m_pVBRHeader->m_dwOffset,
					m_pMPAFile->m_pVBRHeader->m_dwFrames,
					m_pMPAFile->m_pVBRHeader->m_dwBytes,
					m_pMPAFile->m_pVBRHeader->m_dwBytes/1024 );
	}

	CMPAFrame* pCurMPAFrame;
	CMPAFrame* pPrevMPAFrame;

	pCurMPAFrame =  m_pMPAFile->GetFrame(CMPAFile::First);
	pPrevMPAFrame = m_pMPAFile->GetFrame(CMPAFile::First);
	if (!pCurMPAFrame || !pPrevMPAFrame)
	{
		wprintf(_T(" Couldn't find first frame. This is probably no MPEG audio file!"));
		return 0;
	}
	printf(" First frame starts at %d\n", m_pMPAFile->m_dwBegin);
	if(m_pMPAFile->m_dwBegin>0){
		size_t len = voffset.size();
		if(len<1){
			voffset.push_back(0);
			vsize.push_back(m_pMPAFile->m_dwBegin);
			//wprintf(_T(" Junk1: %d + %d\n"), 0, m_pMPAFile->m_dwBegin);
		}else{
			for(size_t i=0; i<len;i++){
				if(voffset[i]>=0){
					voffset.insert(voffset.begin()+i,0);
					vsize.insert(vsize.begin()+i,m_pMPAFile->m_dwBegin);
					//wprintf(_T(" Junk2: %d + %d\n"), 0, m_pMPAFile->m_dwBegin);
					break;
				}
			}
		}
	}
	CMPAFrame* pLastMPAFrame =  m_pMPAFile->GetFrame(CMPAFile::Last);
	if(pLastMPAFrame)
		printf(" Last frame ends at %d\n", pLastMPAFrame->m_dwOffset+pLastMPAFrame->m_dwFrameSize);
	else
		printf(" Last frame not found!\n");
	
	DWORD dwPadded = 0, dwUnpadded = 0;
	DWORD dwFrameNo = 0;
	DWORD dwAverBitrate = pCurMPAFrame->m_pMPAHeader->m_dwBitrate;
	bool bVBR = false;
	wstring strHelp;

	do
	{
		dwFrameNo++;

		// check CRC
		if (pCurMPAFrame->m_pMPAHeader->m_bCRC)
		{
			if (!pCurMPAFrame->CheckCRC())
			{
				wprintf(_T(" Wrong CRC checksum in frame %d\r\n"), dwFrameNo);
			};
		}
		// is it VBR?
		if (pCurMPAFrame->m_pMPAHeader->m_dwBitrate != dwAverBitrate)
		{
			// aver. bitrate += (new bitrate - aver. bitrate / #frames)
			// convert to int (some values can be negative)
			dwAverBitrate += (int)((int)pCurMPAFrame->m_pMPAHeader->m_dwBitrate - (int)dwAverBitrate) / (int)dwFrameNo;
			bVBR = true;
		}

		// even in VBR files these values must not change
		if (pCurMPAFrame->m_pMPAHeader->m_dwSamplesPerSec != pPrevMPAFrame->m_pMPAHeader->m_dwSamplesPerSec)
		{
			wprintf(_T(" Sampling rate changed in frame %d from %d Hz to %d Hz\r\n"), dwFrameNo, pPrevMPAFrame->m_pMPAHeader->m_dwSamplesPerSec, pCurMPAFrame->m_pMPAHeader->m_dwSamplesPerSec);
		}
		if (pCurMPAFrame->m_pMPAHeader->m_Layer != pPrevMPAFrame->m_pMPAHeader->m_Layer)
		{
			InvalidChange( _T(" Layer"), dwFrameNo, CMPAHeader::m_szLayers[pPrevMPAFrame->m_pMPAHeader->m_Layer], CMPAHeader::m_szLayers[pCurMPAFrame->m_pMPAHeader->m_Layer] );
		}
		if (pCurMPAFrame->m_pMPAHeader->m_Version != pPrevMPAFrame->m_pMPAHeader->m_Version)
		{
			InvalidChange( _T(" Version"), dwFrameNo, CMPAHeader::m_szMPEGVersions[pPrevMPAFrame->m_pMPAHeader->m_Version], CMPAHeader::m_szMPEGVersions[pCurMPAFrame->m_pMPAHeader->m_Version] );
		}
		if (dwFrameNo > 1 && pCurMPAFrame->m_dwOffset != pPrevMPAFrame->GetSubsequentHeaderOffset())
		{
			wprintf( _T(" Frame %d header expected at byte %d, but found at byte %d.\r\n")
							  _T(" Frame %d (bytes %d-%d) was %d bytes long (expected %d bytes).\r\n"),
							  dwFrameNo, 
							  pPrevMPAFrame->GetSubsequentHeaderOffset(),
							  pCurMPAFrame->m_dwOffset,	
							  dwFrameNo-1,
							  pPrevMPAFrame->m_dwOffset,
							  pCurMPAFrame->m_dwOffset,
							  pCurMPAFrame->m_dwOffset - pPrevMPAFrame->m_dwOffset,
							  pPrevMPAFrame->m_dwFrameSize );
		}
		
		if (pCurMPAFrame->m_pMPAHeader->m_dwPaddingSize > 0)
			dwPadded++;
		else
			dwUnpadded++;

		delete pPrevMPAFrame;
		pPrevMPAFrame = pCurMPAFrame;

	}while ((pCurMPAFrame = m_pMPAFile->GetFrame(CMPAFile::Next, pCurMPAFrame, false)) != NULL);
	
	if (pPrevMPAFrame->GetSubsequentHeaderOffset() > m_pMPAFile->m_dwEnd)
	{
		wprintf(_T(" Last frame truncated.\r\nFrame %d (bytes %d-%d) was %d bytes long (expected %d bytes)\r\n\r\n"), 
			dwFrameNo, pPrevMPAFrame->m_dwOffset, m_pMPAFile->m_dwEnd, m_pMPAFile->m_dwEnd - pPrevMPAFrame->m_dwOffset, pPrevMPAFrame->m_dwFrameSize);
	}

	wprintf(_T(" Total number of frames: %d, unpadded: %d, padded: %d\r\n"), dwFrameNo, dwUnpadded, dwPadded);
	
	DWORD dwLengthSec = pPrevMPAFrame->m_pMPAHeader->GetLengthSecond(dwFrameNo);
	if (bVBR)
		wprintf( _T(" File is VBR. Average bitrate is %d kbps."), dwAverBitrate/1000);
	else
		wprintf(_T(" File is CBR. Bitrate of each frame is %d kbps."), dwAverBitrate/1000);
	
	wprintf(_T(" Length: %.2d:%.2d\n"), dwLengthSec / 60, dwLengthSec % 60);
	
	delete pPrevMPAFrame;
	delete m_pMPAFile;

	return dwLengthSec*100;
}

int hash_mp3(wstring path, FILE* file_handle, unsigned __int64 file_size, char* sum, int &duration){
	std::vector<DWORD> voffset;
	std::vector<DWORD> vsize;
	try{
		duration = check_mp3(path.c_str(), voffset, vsize);
	}catch(CMPAException& Exc){
		//Exc.ShowError();
		printf("Failed to parse file! (tag)\n");
		return 0;
	}

	int id = hash_create(HASH_SHA1, 0);
	char* databuffer = new char[BUFFER_SIZE];
	int read = 0, end, skipped=0;
	int rt, off;
	size_t i;
	fseek(file_handle, 0, SEEK_SET);
	while(read<file_size){
		end = read+BUFFER_SIZE;
		for(i=0; i<voffset.size();i++){
			off = voffset[i];
			if(off>=read && off<end){
				rt = off-read;
				break;
			}
		}
		if(i>=voffset.size()){
			if(end>file_size)
				rt = BUFFER_SIZE-(end-file_size);
			else rt = BUFFER_SIZE;
		}
		if(rt){
			fread((char*)databuffer, 1, rt, file_handle);
			printf("  READ: %d-%d\n", read, (read+rt));
			hash_update(id, databuffer, rt);
			read += rt;
		}
		if(i<voffset.size()){
			fseek(file_handle, vsize[i], SEEK_CUR);
			printf("  SEEK: %d-%d\n", read, read+vsize[i]);
			read+=vsize[i];
			skipped += vsize[i];
			vsize.erase(vsize.begin()+i);
			voffset.erase(voffset.begin()+i);
		}
	}
	delete databuffer;

	i = hash_digest(id, HASH_SHA1, sum, 64);
	hash_destroy(id);
	sum[i] = '\0';
	_strlwr_s(sum, 64);
	printf(" SHA1  : %s\n", sum);
	printf(" size  : %d\n", (int)file_size);
	printf(" length: %d\n", read-skipped);
	printf(" skipd : %d\n", skipped);

	return read-skipped;
}

#endif

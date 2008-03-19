#include "stdafx.h"

#include <vector>
#include "MPAFile.h"
//#include "MPEGAudioInfo.h"


void InvalidChange(LPCTSTR szWhat, DWORD dwFrame, LPCTSTR szOldValue, LPCTSTR szNewValue)
{
	wprintf(_T("%s changed in frame %d from %s to %s\r\n"), szWhat, dwFrame, szOldValue, szNewValue);
}

int wmain(int argc, wchar_t* argv[]){
		// go through every frame of the file and look if the next frame is at the computed offset and valid

	if(argc<2)
		return 0;
	CMPAFile* m_pMPAFile = new CMPAFile(argv[1], 0);
	//CMPAFile* m_pMPAFile = new CMPAFile(L"N:\\_t\\aud\\defective\\01 REVIVE.mp3", 0);

	int nTag = 0;
	CTag* pTag;
	std::vector<DWORD> voffset;
	std::vector<DWORD> vsize;

	while ((pTag = m_pMPAFile->m_Tags.GetNextTag(nTag)) != NULL)
	{
		wprintf(_T("%s V%.2f: %d + %d\n"), pTag->GetName(), pTag->GetVersion(), pTag->GetOffset(), pTag->GetSize());
		voffset.push_back(pTag->GetOffset());
		vsize.push_back(pTag->GetSize());
	}
	for(int i=0; i<voffset.size();i++)
	{
		printf("SKIP %d bytes at %d\n", vsize[i], voffset[i]);
	}
	if (m_pMPAFile->m_pVBRHeader)
	{			
		CString strHelp, strHeader;
		CXINGHeader* pXINGHeader;
		CVBRIHeader* pVBRHeader;

		// we need runtime type information for the dynamic_cast -> compile this with /GR switch
		// is it a XING header?
		if ((pXINGHeader = dynamic_cast<CXINGHeader*>(m_pMPAFile->m_pVBRHeader)) != NULL)
		{
			if (m_pMPAFile->m_pVBRHeader->m_dwQuality != -1)
				wprintf(_T("XING: Quality: %d\n"), m_pMPAFile->m_pVBRHeader->m_dwQuality );
			else
				wprintf(_T("XING: Quality: not set\n") );
		}
		// is it a VBRI header?
		else if ((pVBRHeader = dynamic_cast<CVBRIHeader*>(m_pMPAFile->m_pVBRHeader)) != NULL)
		{
			wprintf( _T("VBRI: Version %d\nDelay %.2f\nQuality %d\n"),
				pVBRHeader->m_dwVersion, pVBRHeader->m_fDelay, pVBRHeader->m_dwQuality );	
		}
		
		wprintf(_T("%s\nOffset: Byte %d\nNumber Of Frames: %d\nFile Size: %d Byte (%d KB)\n"), 
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
		wprintf(_T("Couldn't find first frame. This is probably no MPEG audio file!"));
		return 1;
	}
	printf("First frame: %d\n", m_pMPAFile->m_dwBegin);
	
	DWORD dwPadded = 0, dwUnpadded = 0;
	DWORD dwFrameNo = 0;
	DWORD dwAverBitrate = pCurMPAFrame->m_pMPAHeader->m_dwBitrate;
	bool bVBR = false;
	CString strHelp;

	do
	{
		dwFrameNo++;

		// check CRC
		if (pCurMPAFrame->m_pMPAHeader->m_bCRC)
		{
			if (!pCurMPAFrame->CheckCRC())
			{
				wprintf(_T("Wrong CRC checksum in frame %d\r\n"), dwFrameNo);
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
			CString strNewSamplingRate, strOldSamplingRate;
			strOldSamplingRate.Format(_T("%d Hz"), pPrevMPAFrame->m_pMPAHeader->m_dwSamplesPerSec);
			// new sampling rate
			strNewSamplingRate.Format(_T("%d Hz"), pCurMPAFrame->m_pMPAHeader->m_dwSamplesPerSec);
			InvalidChange( _T("Sampling rate"), dwFrameNo, strOldSamplingRate, strNewSamplingRate);
		}
		if (pCurMPAFrame->m_pMPAHeader->m_Layer != pPrevMPAFrame->m_pMPAHeader->m_Layer)
		{
			InvalidChange( _T("Layer"), dwFrameNo, CMPAHeader::m_szLayers[pPrevMPAFrame->m_pMPAHeader->m_Layer], CMPAHeader::m_szLayers[pCurMPAFrame->m_pMPAHeader->m_Layer] );
		}
		if (pCurMPAFrame->m_pMPAHeader->m_Version != pPrevMPAFrame->m_pMPAHeader->m_Version)
		{
			InvalidChange( _T("Version"), dwFrameNo, CMPAHeader::m_szMPEGVersions[pPrevMPAFrame->m_pMPAHeader->m_Version], CMPAHeader::m_szMPEGVersions[pCurMPAFrame->m_pMPAHeader->m_Version] );
		}
		if (dwFrameNo > 1 && pCurMPAFrame->m_dwOffset != pPrevMPAFrame->GetSubsequentHeaderOffset())
		{
			wprintf( _T("Frame %d header expected at byte %d, but found at byte %d.\r\n")
							  _T("Frame %d (bytes %d-%d) was %d bytes long (expected %d bytes).\r\n\r\n"),
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
		wprintf(_T("Last frame truncated.\r\nFrame %d (bytes %d-%d) was %d bytes long (expected %d bytes)\r\n\r\n"), 
			dwFrameNo, pPrevMPAFrame->m_dwOffset, m_pMPAFile->m_dwEnd, m_pMPAFile->m_dwEnd - pPrevMPAFrame->m_dwOffset, pPrevMPAFrame->m_dwFrameSize);
	}

	wprintf(_T("\r\nSummary:\r\n===============\r\n"));
	wprintf(_T("Total number of frames: %d, unpadded: %d, padded: %d\r\n"), dwFrameNo, dwUnpadded, dwPadded);
	
	DWORD dwLengthSec = pPrevMPAFrame->m_pMPAHeader->GetLengthSecond(dwFrameNo);
	if (bVBR)
		wprintf( _T("File is VBR. Average bitrate is %d kbps.\r\n"), dwAverBitrate/1000);
	else
		wprintf(_T("File is CBR. Bitrate of each frame is %d kbps.\r\n"), dwAverBitrate/1000);
	
	wprintf(_T("Length: %.2d:%.2d\r\n"), dwLengthSec / 60, dwLengthSec % 60);
	
	delete pPrevMPAFrame;
	delete m_pMPAFile;
	//system("pause");

	return 0;
}
/*int main(int argc, char **argv){
	
	try{
		main2(argc, argv);
	}catch(CMPAException& Exc){
		Exc.ShowError();
	}
	return 0;
	
}*/
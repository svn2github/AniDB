// MPEGAudioInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MPEGAudioInfo.h"
#include "MPEGAudioInfoDlg.h"
#include "AboutDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMPEGAudioInfoDlg dialog
CMPEGAudioInfoDlg::CMPEGAudioInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMPEGAudioInfoDlg::IDD, pParent)
	, m_strFile(_T(""))
	, m_strInfo1(_T(""))
	, m_strInfo2(_T(""))
	, m_strVBRInfo(_T(""))
	, m_strFrameNo(_T(""))
	, m_strOutput(_T(""))
	, m_pMPAFile(NULL)
	, m_pMPAFrame(NULL)
	, m_strTagInfo(_T(""))
	, m_strFileInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMPEGAudioInfoDlg::~CMPEGAudioInfoDlg()
{
	if (m_pMPAFrame)
		delete m_pMPAFrame;
	if (m_pMPAFile)
		delete m_pMPAFile;
}

void CMPEGAudioInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILE, m_strFile);
	DDX_Text(pDX, IDC_INFO1, m_strInfo1);
	DDX_Text(pDX, IDC_INFO2, m_strInfo2);
	DDX_Text(pDX, IDC_VBRINFO, m_strVBRInfo);
	DDX_Text(pDX, IDC_FRAMENO, m_strFrameNo);
	DDX_Text(pDX, IDC_OUTPUT, m_strOutput);
	DDX_Control(pDX, IDC_PREVFRAME, m_CtrlPrevFrame);
	DDX_Control(pDX, IDC_NEXTFRAME, m_CtrlNextFrame);
	DDX_Control(pDX, IDC_LASTFRAME, m_CtrlLastFrame);
	DDX_Control(pDX, IDC_FIRSTFRAME, m_CtrlFirstFrame);
	DDX_Control(pDX, IDC_CHECKFILE, m_CtrlCheckFile);
	DDX_Text(pDX, IDC_TAGINFO, m_strTagInfo);
	DDX_Text(pDX, IDC_FILEINFO, m_strFileInfo);
}

BEGIN_MESSAGE_MAP(CMPEGAudioInfoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPENFILE, OnBnClickedOpenFile)
	ON_BN_CLICKED(IDC_NEXTFRAME, OnBnClickedNextframe)
	ON_BN_CLICKED(IDC_PREVFRAME, OnBnClickedPrevframe)
	ON_BN_CLICKED(IDC_FIRSTFRAME, OnBnClickedFirstframe)
	ON_BN_CLICKED(IDC_LASTFRAME, OnBnClickedLastframe)
	ON_BN_CLICKED(IDC_CHECKFILE, OnBnClickedCheckfile)
	ON_BN_CLICKED(IDC_ABOUT, OnBnClickedAbout)
END_MESSAGE_MAP()


// CMPEGAudioInfoDlg message handlers

BOOL CMPEGAudioInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}*/

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMPEGAudioInfoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMPEGAudioInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMPEGAudioInfoDlg::OnBnClickedOpenFile()
{
	TCHAR szFilter[] = _T("MPEG Audio Files (*.mp*)|*.mp*|All Files (*.*)|*.*||");
	CFileDialog FileDlg(true,  NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	if (FileDlg.DoModal() == IDOK)
	{
		m_strFile.Format(_T("File: %s"), FileDlg.GetPathName());
		UpdateData(false);
		LoadMPEGFile(FileDlg.GetPathName());
	}
}

// display frame specific information
void CMPEGAudioInfoDlg::GetFrameInfo(CMPAFrame* pMPAFrame, DWORD dwFrameNo)
{
	m_strInfo1.Format(	_T("%s %s\n%d kbps, %s\nSampling Rate: %d Hz\n\nCopyright: %s, CRC: %s, Private: %s,\n")
						_T("Original: %s, Emphasis: %s"),
						CMPAHeader::m_szMPEGVersions[pMPAFrame->m_pMPAHeader->m_Version], 
						CMPAHeader::m_szLayers[pMPAFrame->m_pMPAHeader->m_Layer],
						pMPAFrame->m_pMPAHeader->GetBytesPerSecond()/125, 
						CMPAHeader::m_szChannelModes[pMPAFrame->m_pMPAHeader->m_ChannelMode],
						pMPAFrame->m_pMPAHeader->m_dwSamplesPerSec,
						IsVarTrue(pMPAFrame->m_pMPAHeader->m_bCopyright),
						IsVarTrue(pMPAFrame->m_pMPAHeader->m_bCRC),
						IsVarTrue(pMPAFrame->m_pMPAHeader->m_bPrivate),
						IsVarTrue(pMPAFrame->m_pMPAHeader->m_bOriginal),
						CMPAHeader::m_szEmphasis[pMPAFrame->m_pMPAHeader->m_Emphasis] );

	m_strInfo2.Format(	_T("Beginning of header: Byte %d\nComputed Frame Size: %d Byte\n")
						_T("Padding Size: %d Byte\nSamples Per Frame: %d"),
						
						pMPAFrame->m_dwOffset,
						pMPAFrame->m_dwFrameSize,
						pMPAFrame->m_pMPAHeader->m_dwPaddingSize,
						pMPAFrame->m_pMPAHeader->m_dwSamplesPerFrame);
			
	if (dwFrameNo == 0)
		m_strFrameNo = _T("Frame ?");
	else
		m_strFrameNo.Format(_T("Frame %d"), dwFrameNo);	

	UpdateData(false);
}

void CMPEGAudioInfoDlg::LoadMPEGFile(LPCTSTR szFile)
{
	m_strOutput.Empty();

	try
	{
		// remove previous frame
		if (m_pMPAFrame)
		{
			delete m_pMPAFrame;
			m_pMPAFrame = NULL;
			m_dwFrameNo = 0;
		}
		
		// remove previous file
		if (m_pMPAFile)
		{
			delete m_pMPAFile;
			m_pMPAFile = NULL;
		}

		m_pMPAFile = new CMPAFile(szFile, 0);

		m_CtrlPrevFrame.EnableWindow(true);
		m_CtrlNextFrame.EnableWindow(true);
		m_CtrlFirstFrame.EnableWindow(true);
		m_CtrlLastFrame.EnableWindow(true);
		m_CtrlCheckFile.EnableWindow(true);

		m_strTagInfo.Empty();
		int nTag = 0;
		CTag* pTag;
		CString strTag;
		while ((pTag = m_pMPAFile->m_Tags.GetNextTag(nTag)) != NULL)
		{
			strTag.Format(_T("%s V%.2f:\tPosition: Byte %d\n\t\tSize: %d Byte\n"), pTag->GetName(), pTag->GetVersion(), pTag->GetOffset(), pTag->GetSize());
			m_strTagInfo += strTag;
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
					strHelp.Format(_T("Quality: %d\n"), m_pMPAFile->m_pVBRHeader->m_dwQuality );
				else
					strHelp.Format(_T("Quality: not set\n") );
				strHeader = _T("XING");
			}
			// is it a VBRI header?
			else if ((pVBRHeader = dynamic_cast<CVBRIHeader*>(m_pMPAFile->m_pVBRHeader)) != NULL)
			{
				strHelp.Format( _T("Version %d\nDelay %.2f\nQuality %d\n"),
					pVBRHeader->m_dwVersion, pVBRHeader->m_fDelay, pVBRHeader->m_dwQuality );	
				strHeader = _T("VBRI");
			}
			
			m_strVBRInfo.Format(_T("%s\nOffset: Byte %d\nNumber Of Frames: %d\nFile Size: %d Byte (%d KB)\n"), 
						strHeader,
						m_pMPAFile->m_pVBRHeader->m_dwOffset,
						m_pMPAFile->m_pVBRHeader->m_dwFrames,
						m_pMPAFile->m_pVBRHeader->m_dwBytes,
						m_pMPAFile->m_pVBRHeader->m_dwBytes/1024 );
			m_strVBRInfo += strHelp;
		}
		else
			m_strVBRInfo.Format(_T("None"));

		DWORD dwLengthSec = m_pMPAFile->GetLengthSec();
		DWORD dwFileSize = m_pMPAFile->GetFileSize();
		m_strFileInfo.Format(_T("File Size: %d Byte (%d KB)\nEstimated Length: %.2d:%.2d"), dwFileSize,
							 dwFileSize / 1024, dwLengthSec / 60, dwLengthSec % 60);

		// current frame is first frame	(need new instance)
		OnBnClickedFirstframe();
	}
	catch(CMPAException& Exc)
	{
		Exc.ShowError();
		m_CtrlPrevFrame.EnableWindow(false);
		m_CtrlNextFrame.EnableWindow(false);
		m_CtrlFirstFrame.EnableWindow(false);
		m_CtrlLastFrame.EnableWindow(false);
		m_CtrlCheckFile.EnableWindow(false);
	}
	UpdateData(false);
}

CString CMPEGAudioInfoDlg::IsVarTrue(bool bVar)
{
	return bVar?_T("Yes"):_T("No");
}

void CMPEGAudioInfoDlg::OnBnClickedNextframe()
{
	CMPAFrame* pMPAFrame;
	if ((pMPAFrame = m_pMPAFile->GetFrame(CMPAFile::Next, m_pMPAFrame)) != NULL)
	{
		m_pMPAFrame = pMPAFrame;
		if (m_dwFrameNo > 0)
			m_dwFrameNo++;
		GetFrameInfo(m_pMPAFrame, m_dwFrameNo);	
	}
}

void CMPEGAudioInfoDlg::OnBnClickedPrevframe()
{
	CMPAFrame* pMPAFrame;
	if ((pMPAFrame = m_pMPAFile->GetFrame(CMPAFile::Prev, m_pMPAFrame)) != NULL)
	{
		m_pMPAFrame = pMPAFrame;
		if (m_dwFrameNo > 1)
			m_dwFrameNo--;
		GetFrameInfo(m_pMPAFrame, m_dwFrameNo);
	}
}

void CMPEGAudioInfoDlg::OnBnClickedFirstframe()
{
	if ((m_pMPAFrame = m_pMPAFile->GetFrame(CMPAFile::First, m_pMPAFrame)) != NULL)
	{
		m_dwFrameNo = 1;
		GetFrameInfo(m_pMPAFrame, m_dwFrameNo);
	}
}

void CMPEGAudioInfoDlg::OnBnClickedLastframe()
{
	if ((m_pMPAFrame = m_pMPAFile->GetFrame(CMPAFile::Last, m_pMPAFrame)) != NULL)
	{
		m_dwFrameNo = 0;
		GetFrameInfo(m_pMPAFrame, m_dwFrameNo);
	}
}

void CMPEGAudioInfoDlg::OnBnClickedCheckfile()
{
	m_strOutput = _T("");

	// go through every frame of the file and look if the next frame is at the computed offset and valid
	CMPAFrame* pCurMPAFrame;
	CMPAFrame* pPrevMPAFrame;

	pCurMPAFrame = m_pMPAFile->GetFrame(CMPAFile::First);
	pPrevMPAFrame = m_pMPAFile->GetFrame(CMPAFile::First);
	if (!pCurMPAFrame || !pPrevMPAFrame)
	{
		m_strOutput = _T("Couldn't find first frame. This is probably no MPEG audio file!");
		return;
	}
	
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
				strHelp.Format(_T("Wrong CRC checksum in frame %d\r\n"), dwFrameNo);	
				m_strOutput += strHelp;
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
			strHelp.Format( _T("Frame %d header expected at byte %d, but found at byte %d.\r\n")
							  _T("Frame %d (bytes %d-%d) was %d bytes long (expected %d bytes).\r\n\r\n"),
							  dwFrameNo, 
							  pPrevMPAFrame->GetSubsequentHeaderOffset(),
							  pCurMPAFrame->m_dwOffset,	
							  dwFrameNo-1,
							  pPrevMPAFrame->m_dwOffset,
							  pCurMPAFrame->m_dwOffset,
							  pCurMPAFrame->m_dwOffset - pPrevMPAFrame->m_dwOffset,
							  pPrevMPAFrame->m_dwFrameSize );
			m_strOutput += strHelp;
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
		strHelp.Format(_T("Last frame truncated.\r\nFrame %d (bytes %d-%d) was %d bytes long (expected %d bytes)\r\n\r\n"), 
			dwFrameNo, pPrevMPAFrame->m_dwOffset, m_pMPAFile->m_dwEnd, m_pMPAFile->m_dwEnd - pPrevMPAFrame->m_dwOffset, pPrevMPAFrame->m_dwFrameSize);
		m_strOutput += strHelp;
	}

	m_strOutput += (_T("\r\nSummary:\r\n===============\r\n"));
	strHelp.Format(_T("Total number of frames: %d, unpadded: %d, padded: %d\r\n"), dwFrameNo, dwUnpadded, dwPadded);
	m_strOutput += strHelp;

	DWORD dwLengthSec = pPrevMPAFrame->m_pMPAHeader->GetLengthSecond(dwFrameNo);
	if (bVBR)
		strHelp.Format( _T("File is VBR. Average bitrate is %d kbps.\r\n"), dwAverBitrate/1000);
	else
		strHelp.Format(_T("File is CBR. Bitrate of each frame is %d kbps.\r\n"), dwAverBitrate/1000);
	m_strOutput += strHelp;

	strHelp.Format(_T("Length: %.2d:%.2d\r\n"), dwLengthSec / 60, dwLengthSec % 60);
	m_strOutput += strHelp;
	delete pPrevMPAFrame;
	UpdateData(false);
}

void CMPEGAudioInfoDlg::InvalidChange(LPCTSTR szWhat, DWORD dwFrame, LPCTSTR szOldValue, LPCTSTR szNewValue)
{
	CString strHelp;
	strHelp.Format(_T("%s changed in frame %d from %s to %s\r\n"), szWhat, dwFrame, szOldValue, szNewValue);
	m_strOutput += strHelp;
}

void CMPEGAudioInfoDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

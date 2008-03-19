#include "stdafx.h"
#include ".\mpaexception.h"

/// CMPAException: exception class
//////////////////////////////////////////////

CMPAException::CMPAException(ErrorIDs ErrorID, LPCTSTR szFile, LPCTSTR szFunction, bool bGetLastError ) :
m_ErrorID( ErrorID ), m_bGetLastError( bGetLastError )
{
	m_szFile = _tcsdup(szFile);
	m_szFunction = _tcsdup(szFunction);
}

// copy constructor (necessary for exception throwing without pointers)
CMPAException::CMPAException(const CMPAException& Source)
{
	m_ErrorID = Source.m_ErrorID;
	m_bGetLastError = Source.m_bGetLastError;
	m_szFile = _tcsdup(Source.m_szFile);
	m_szFunction = _tcsdup(Source.m_szFunction);
}

// destructor
CMPAException::~CMPAException()
{
	if (m_szFile)
		free( m_szFile );
	if (m_szFunction)
		free( m_szFunction );
}

// should be in resource file for multi language applications
LPCTSTR CMPAException::m_szErrors[CMPAException::NumIDs] = 
{
	_T("Can't open the file."), 
	_T("Can't set file position."),
	_T("Can't read from file."),
	_T("Reached end of buffer."),
	_T("No VBR Header found."),
	_T("Incomplete VBR Header."),
	_T("No subsequent frame found within tolerance range."),
	_T("No frame found."),
	_T("HeaderCorrupt"),
	_T("Free Bitrate currently not supported"),
	_T("Incompatible Header"),
	_T("Corrupt Lyrics3 Tag")
};

#define MAX_ERR_LENGTH 256
void CMPAException::ShowError() const
{
	TCHAR szErrorMsg[MAX_ERR_LENGTH] = {0};
	TCHAR szHelp[MAX_ERR_LENGTH];

	// this is not buffer-overflow-proof!
	if (m_szFunction)
	{
		_stprintf(szHelp, _T("%s: "), m_szFunction);
		_tcscat(szErrorMsg, szHelp );
	}
	if (m_szFile)
	{
		_stprintf(szHelp, _T("'%s'\n"), m_szFile);
		_tcscat(szErrorMsg, szHelp);
	}
	_tcscat(szErrorMsg, m_szErrors[m_ErrorID]);

	if (m_bGetLastError)
	{
		// get error message of last system error id
		LPVOID pMsgBuf;
		if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							GetLastError(),
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR) &pMsgBuf,
							0,
							NULL))
		{
			_tcscat(szErrorMsg, _T("\n"));
			_tcscat(szErrorMsg, (LPCTSTR)pMsgBuf);
			LocalFree(pMsgBuf);
		}
	}
	// show error message
	::MessageBox (NULL, szErrorMsg, _T("MPAFile Error"), MB_OK);
}
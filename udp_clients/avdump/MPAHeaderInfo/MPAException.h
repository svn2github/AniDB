#pragma once

#include <windows.h>
#include <tchar.h>

// exception class
class CMPAException
{
public:
	
	enum ErrorIDs
	{
		ErrOpenFile,
		ErrSetPosition,
		ErrReadFile,
		EndOfBuffer,
		NoVBRHeader,
		IncompleteVBRHeader,
		NoFrameInTolerance,
		NoFrame,
		HeaderCorrupt,
		FreeBitrate,
		IncompatibleHeader,
		CorruptLyricsTag,
		NumIDs			// this specifies the total number of possible IDs
	};

	CMPAException(ErrorIDs ErrorID, LPCTSTR szFile = NULL, LPCTSTR szFunction = NULL, bool bGetLastError=false );
	// copy constructor (necessary because of LPSTR members)
	CMPAException(const CMPAException& Source);
	~CMPAException(void);

	ErrorIDs GetErrorID() const { return m_ErrorID; };
	void SetFileName(LPCTSTR szFile) { m_szFile = _tcsdup(szFile); };

	void ShowError() const;

private:
	ErrorIDs m_ErrorID;
	bool m_bGetLastError;
	LPTSTR m_szFunction;
	LPTSTR m_szFile;

	static LPCTSTR m_szErrors[CMPAException::NumIDs];
};

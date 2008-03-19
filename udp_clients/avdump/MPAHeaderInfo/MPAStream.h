#pragma once

class CMPAStream
{
public:
	CMPAStream(LPCTSTR szFilename, DWORD& dwEnd, HANDLE hFile = INVALID_HANDLE_VALUE);
	~CMPAStream(void);

	DWORD GetSize() const;
	BYTE* ReadBytes(DWORD dwSize, DWORD& dwOffset, bool bMoveOffset = true, bool bReverse = false) const;
	DWORD ReadBEValue(DWORD dwNumBytes, DWORD& dwOffset, bool bMoveOffset = true) const;
	DWORD ReadLEValue(DWORD dwNumBytes, DWORD& dwOffset, bool bMoveOffset = true) const;
	LPCTSTR GetFilename() const { return m_szFile; };

private:
	static const DWORD m_dwInitBufferSize;

	HANDLE m_hFile;
	LPTSTR m_szFile;
	bool m_bMustReleaseFile;
	
	// concerning read-buffer
	mutable BYTE* m_pBuffer;
	mutable DWORD m_dwOffset;	// offset (beginning if m_pBuffer)
	mutable DWORD m_dwBufferSize;

	// methods for file access
	void SetPosition(LONGLONG lOffset, DWORD dwMoveMethod) const;
	DWORD Read(LPVOID pData, DWORD dwOffset, DWORD dwSize) const;
	void FillBuffer(DWORD dwOffset, DWORD dwSize, bool bReverse) const;
};

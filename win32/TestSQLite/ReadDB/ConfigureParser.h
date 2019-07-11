#pragma once

#define CONF_BUF_MAX 1024*2
class CConfigureParser
{
public:
	CConfigureParser();
	virtual ~CConfigureParser();

	void SetConfPath(LPCTSTR pszPath);
	int Read(LPCTSTR pszSectionName, LPCTSTR pszValueName);
	void Read(LPCTSTR pszSectionName, LPCTSTR pszValueName, LPTSTR pszBuffer, DWORD nBufferSize);
	void SetValue(LPCTSTR pszSectionName, LPCTSTR pszValueName, LPCTSTR pszValue);
	BOOL IsDefault(int nValue);
	LPCTSTR Read(LPCTSTR pszSectionName, LPCTSTR pszValueName);

private:
	TCHAR m_szPATH[1024];
	int m_nDefalutValue;
	TCHAR* m_pBuffer;
};


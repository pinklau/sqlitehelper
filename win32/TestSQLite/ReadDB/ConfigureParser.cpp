#include "ConfigureParser.h"



CConfigureParser::CConfigureParser()
	:m_nDefalutValue(0xffffff1)
	,m_pBuffer(NULL)
{
	memset(m_szPATH, 0, sizeof(m_szPATH));
}


CConfigureParser::~CConfigureParser()
{
	if (m_pBuffer)
		delete[]m_pBuffer;
}

void CConfigureParser::SetConfPath(LPCTSTR pszPath)
{
	memset(m_szPATH, 0, sizeof(m_szPATH));
	memcpy(m_szPATH, pszPath, lstrlen(pszPath)*sizeof(TCHAR) < sizeof(m_szPATH) ? lstrlen(pszPath)*sizeof(TCHAR) : sizeof(m_szPATH) -sizeof(TCHAR));
}

int CConfigureParser::Read(LPCTSTR pszSectionName, LPCTSTR pszValueName)
{
	if (pszSectionName && pszValueName)
		return GetPrivateProfileInt(pszSectionName, pszValueName, m_nDefalutValue, m_szPATH);
	return m_nDefalutValue;
}

void CConfigureParser::Read(LPCTSTR pszSectionName, LPCTSTR pszValueName, LPTSTR pszBuffer, DWORD nBufferSize)
{
	if (pszSectionName && pszValueName && pszBuffer)
		GetPrivateProfileString(pszSectionName, pszValueName, TEXT(""), pszBuffer, nBufferSize, m_szPATH);
}

void CConfigureParser::SetValue(LPCTSTR pszSectionName, LPCTSTR pszValueName, LPCTSTR pszValue)
{
	if (pszSectionName && pszValueName && pszValue)
		WritePrivateProfileString(pszSectionName, pszValueName, pszValue, m_szPATH);
}

BOOL CConfigureParser::IsDefault(int nValue)
{
	return nValue == m_nDefalutValue;
}

LPCTSTR CConfigureParser::Read(LPCTSTR pszSectionName, LPCTSTR pszValueName)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new TCHAR[CONF_BUF_MAX];
	}
	memset(m_pBuffer, 0, sizeof(TCHAR)*(CONF_BUF_MAX));
	Read(pszSectionName, pszValueName, m_pBuffer, CONF_BUF_MAX);
	return m_pBuffer;
}
#include "AdoHelper.h"



CAdoHelper::CAdoHelper()
	:m_pConn(NULL)
{
	CoInitialize(NULL);
	HRESULT hRet = m_pConn.CreateInstance(__uuidof(Connection));
	if (hRet == S_OK)
	{
		OutputDebugString(TEXT("Create connection object successful\n"));
	}
}


CAdoHelper::~CAdoHelper()
{
	CoUninitialize();
}

BOOL CAdoHelper::Open(LPCTSTR pszUserName, LPCTSTR pszPwd, LPCTSTR pszServer)
{
	BOOL bRet = FALSE;
	
	return bRet;
}

BOOL CAdoHelper::Close()
{
	if (IsConnect())
		m_pConn->Close();
	return TRUE;
}





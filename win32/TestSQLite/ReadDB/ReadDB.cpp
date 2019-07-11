// ReadDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ReadDB.h"

extern TCHAR g_szConfPath[MAX_PATH];

// This is the constructor of a class that has been exported.
// see ReadDB.h for the class definition
CReadDB::CReadDB()
	:m_nIndex(-1)
{
    return;
}

CReadDB::~CReadDB()
{

}

int CReadDB::ValueToInt(const int index)
{
	int val = 0;

	return val;
}

short CReadDB::ValueToShort(const int index)
{
	short val = 0;

	return val;
}

long CReadDB::ValueToLong(const int index)
{
	long val = 0;;

	return val;
}

__int64 CReadDB::ValueToInt64(const int index)
{
	__int64 val = 0;;

	return val;
}

float CReadDB::ValueToFloat(const int index)
{
	float val = 0;;

	return val;
}

double CReadDB::ValueToDouble(const int index)
{
	double val = 0;;

	return val;
}

BOOL CReadDB::ValueToBool(const int index)
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CReadDB::ValueToString(const int index, LPCTSTR pszBuf, UINT size)
{
	BOOL bRet = FALSE;

	return bRet;
}

int CReadDB::IndexOf(LPCTSTR pszFieldName)
{

	return -1;
}

BOOL CReadDB::Exce(LPCTSTR pszSQL, int type, CReadDB* pRecordset)
{
	BOOL bRet = FALSE;
	
	return bRet;
}

BOOL CReadDB::First()
{
	BOOL bRet = FALSE;
	m_nIndex = 0;
	if (m_res.size())
		bRet = TRUE;
	return bRet;
}

BOOL CReadDB::Next()
{
	BOOL bRet = FALSE;
	m_nIndex++;
	if (m_nIndex < m_res.size())
		bRet = TRUE;
	return bRet;
}

BOOL CReadDB::ValueToBlob(const int index, BYTE* buf, UINT size)
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CReadDB::Init()
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CReadDB::Release()
{
	BOOL bRet = FALSE;

	return bRet;
}

void CReadDB::SetConf(LPCTSTR pszPath)
{
	int len = lstrlen(pszPath)*sizeof(TCHAR);
	int size = sizeof(TCHAR)*(MAX_PATH);
	memcpy(g_szConfPath, pszPath, len < size ? len : size - 1);
}
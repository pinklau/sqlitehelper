// ReadDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ReadDB.h"


// This is an example of an exported variable
READDB_API int nReadDB=0;


// This is an example of an exported function.
READDB_API int fnReadDB(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ReadDB.h for the class definition
CReadDB::CReadDB()
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

int CReadDB::IndexOf(LPCTSTR pszFieldName)
{

	return -1;
}

BOOL CReadDB::Exce(LPCTSTR pszSQL)
{
	BOOL bRet = FALSE;
	
	return bRet;
}

BOOL CReadDB::First()
{
	BOOL bRet = FALSE;

	return bRet;
}

BOOL CReadDB::Next()
{
	BOOL bRet = FALSE;

	return bRet;
}
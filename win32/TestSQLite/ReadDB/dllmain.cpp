// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ReadDB.h"

READDB_API LPCTSTR g_pszConfPath = new TCHAR[MAX_PATH];

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if (g_pszConfPath) delete[] g_pszConfPath;
		break;
	}

	return TRUE;
}

READDB_CAPI READDB_API BOOL Request(const char* szXML, CReadDB* pRecords)
{
	BOOL bRet = FALSE;

	return bRet;
}

READDB_CAPI READDB_API BOOL Init()
{
	BOOL bRet = FALSE;

	return bRet;
}

READDB_CAPI READDB_API BOOL Release()
{
	BOOL bRet = FALSE;

	return bRet;
}
#ifndef __CSQLITE_STRING__H__
#define __CSQLITE_STRING__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include "SQLiteTypes.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <WinNls.h>
#else

#endif


inline
std::wstring convert2Wchar(const char* datas)
{
    std::wstring str;
    int len = strlen(datas) + 1;
    wchar_t* pBuf = (wchar_t*)malloc(len*sizeof(wchar_t));
	wmemset(pBuf, 0, len);
#if defined(_WIN32) || defined(_WIN64)	
	MultiByteToWideChar(CP_ACP, 0, datas, len - 1, pBuf, len);
#else
    int size = mbstowcs(pBuf, datas, len);
#endif

	str = pBuf;
    free(pBuf);
    return str;
}

inline 
std::string convert2Char(const wchar_t* datas)
{
    std::string str;
	int len = wcslen(datas)*2 + 1;
	char* pBuf = (char*)malloc(len);
	memset(pBuf, 0, len);
#if defined(_WIN32) || defined(_WIN64)	
	WideCharToMultiByte(CP_ACP, 0, datas, wcslen(datas), pBuf, len, NULL, NULL);
#else    
	setlocale(LC_CTYPE, "zh_CN.utf8");
    wcstombs(pBuf, datas, len);
#endif   
	str = pBuf;
	free(pBuf);
    return str;
}

inline string convertString(SLiteString strString)
{
	string str;
#if defined(UNICODE) || defined(_UNICODE)
	str = convert2Char(strString.data());
#else
	str = strString;
#endif
	return str;
}

inline
string getTableName(string sql)
{
	string szTableName = "";
	int pos = sql.rfind("from");
	if (pos != string::npos)
	{
		char * sz = NULL;
		int size = 0;
		pos += 5;
		int space = sql.find(' ', pos);
		if (space != string::npos) size = space - pos + 1;
		else size = sql.length() - pos + 1;

		if (size < 1) return szTableName;

		sz = new char[size];
		memset(sz, 0, size);
#if defined(_WIN32) || defined(_WIN64)
		sql._Copy_s(sz, size, size - 1, pos);
#else
		sql.copy(sz, size, pos);
#endif
		szTableName = sz;
		delete[]sz;
	}
	return szTableName;
}

#endif
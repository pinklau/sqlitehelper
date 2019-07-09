#ifndef __CSQLITE_STRING__H__
#define __CSQLITE_STRING__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <WinNls.h>
#else

#endif


template<class T>
class CSQLiteString : public T
{
public:
    CSQLiteString(){

    }

    ~CSQLiteString(){

    }

	int toInt()
	{
		int val = 0;

		return val;
	}

	double toDouble()
	{
		double val = 0;
		return val;
	}

	float toFloat()
	{
		float val = 0;
		return val;
	}


};

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


#endif
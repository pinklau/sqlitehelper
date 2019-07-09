#ifndef __CSQLITE_STRING__H__
#define __CSQLITE_STRING__H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>


template<class T>
class CSQLiteString : public T
{
public:
    CSQLiteString(){

    }

    ~CSQLiteString(){

    }



};

inline
std::wstring convert2Wchar(const char* datas)
{
    std::wstring str;
    int len = strlen(datas) + 1;
    wchar_t* pBuf = (wchar_t*)malloc(len*sizeof(wchar_t));
    int size = mbstowcs(pBuf, datas, len);
    str = pBuf;
    free(pBuf);
    return str;
}

inline 
std::string convert2Char(const wchar_t* datas)
{
    std::string str;
    int len = wcslen(datas) + 1;
    char* pBuf = malloc(nlen);
    wcrtomb(pBuf, datas, len);
    str = pBuf;
    free(pBuf);
    return str;
}


#endif
#ifndef __SQLITE_TYPES__H__
#define __SQLITE_TYPES__H__

#include <iostream>
#include <vector>
#include <map>
#include <deque>
using namespace std;


#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring SLiteString;
#else
typedef std::string SLiteString;
#endif

#endif
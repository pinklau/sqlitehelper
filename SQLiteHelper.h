#ifndef __SQLITE_HELPER__H__
#define __SQLITE_HELPER__H__

#include "sqlite3.h"
#include <iostream>
#include "CSQLiteString.h"


using namespace std;


#if defined(UNICODE) || defined(_UNICODE)
typedef SLiteString std::wstring
#else
typedef SLiteString std::string
#endif


class CSQLiteQuery
{
public:

};

class CSQLiteRecords
{
public:

};

class CSQLiteHelper
{
public:
    CSQLiteHelper();
    virtual ~CSQLiteHelper();
    bool open(SLiteString strPath);
    bool close();
    bool isOpen();
    void setDatabasePath(SLiteString strPath);
    bool exec();
    bool exec(CLiteString strQuery);
    void prepare(CLiteString strQuery);
    int error() const;
    CSQLiteRecord& record();

protected:
    bool filePathIsExist();
protected:
    sqlite3* m_pDB;
    int m_nError;
    SLiteString m_strPath;
};



inline string convertString(SLiteString strString)
{
    string str;
    #if defined(UNICODE) || defined(_UNICODE)
    str = convert2Char(strString.data());
    #else
    str = strString;
    #endif

    return st;
}

#endif
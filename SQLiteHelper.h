#ifndef __SQLITE_HELPER__H__
#define __SQLITE_HELPER__H__

#include "sqlite3.h"
#include <iostream>
#include "CSQLiteString.h"
#include <vector>
#include <map>
#include <deque>
using namespace std;


#if defined(UNICODE) || defined(_UNICODE)
typedef std::wstring SLiteString;
#else
typedef std::string SLiteString;
#endif


class CSQLiteQuery
{
public:

};

class CSQLiteRow
{
public:

};

class CSQLiteRecords
{
public:
static int results(void*para, int nCount, char** pValue, char** pName);
public:
	CSQLiteRecords();
	~CSQLiteRecords();
public:
	void clear();
	SLiteString value(int row, int col);
	SLiteString value(int row, SLiteString recordName);
	CSQLiteRow& row(int row) const;
	int count();
public:
	void append(int row, int col, int fieldType, const unsigned char* data);
	void appendFieldName(int index, const char* szFieldName);
	void setValue(int row, int col, float val);
	void setValue(int row, int col, double val);
	void setValue(int row, int col, int val);
	void setValue(int row, int col, const char* text);

#if defined(_WIN32) || defined(_WIN64)
	void setValue(int row, int col, SYSTEMTIME &st);
#else
	void setValue(int row, int col, struct tm& st);
#endif

private:
	deque<map<int, SLiteString>*> m_res;
	map<SLiteString,int> m_fieldNames;
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
    bool exec(SLiteString strQuery);
    void prepare(SLiteString strQuery);
    int error() const;

	bool tableExist(SLiteString strTableName);

protected:
    bool filePathIsExist();
	bool getFields(string szTableName);
protected:
    sqlite3* m_pDB;
    int m_nError;
    SLiteString m_strPath;
    CSQLiteRecords m_records;
};



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
		if (space != string::npos)
			size = space - pos + 1;
		else
			size = sql.length() - pos + 1 ;

		if (size < 1)
			return szTableName;

		sz = new char[size];
		memset(sz, 0, size);
#if defined(_WIN32) || defined(_WIN64)
		sql._Copy_s(sz, size, size-1, pos);
#else
		sql.copy(sz, size, pos);
#endif
		szTableName = sz;
		delete[]sz;
	}
	return szTableName;
}


#endif
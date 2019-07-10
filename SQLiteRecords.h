#ifndef __SQLITE_RECORDS__H__
#define __SQLITE_RECORDS__H__

#include "SQLiteTypes.h"
#include <iostream>
#include <vector>
#include <map>
#include <deque>
using namespace std;


class CSQLiteRecords
{
public:
static int results(void*para, int nCount, char** pValue, char** pName);
public:
	CSQLiteRecords();
	~CSQLiteRecords();
public:
	void clear();
	int count() const;
	SLiteString tableName() const;
	void setTableName(const SLiteString strTableName);
	int columnCount();
public:
	bool first();
	bool next();
	SLiteString valueToString(const int index);
	SLiteString valueToString(const SLiteString strFieldName);
	void valueToDateTime(const int index, struct tm& st);
	void valueToBlob(const int index, unsigned char* buf, unsigned int nBufSize);
	int valueToInt(const int index);
	float valueToFloat(const int index);
	double valueToDouble(const int index);
	short valueToShort(const int index);
	long long valueToInt64(const int index);
	long valueToLong(const int index);
	SLiteString indexOfColumn(const int index);

public:
	void append(int row, int col, int fieldType, const unsigned char* data);
	void appendFieldName(int index, const char* szFieldName);

private:
	deque<map<int, string>*> m_res;
	map<SLiteString,int> m_fieldNames;
	SLiteString m_strTableName;
	int m_nIndex;
};


#endif
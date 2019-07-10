#ifndef __SQLITE_HELPER__H__
#define __SQLITE_HELPER__H__

#include "sqlite3.h"
#include <iostream>
#include "SQLiteRecords.h"
#include "SQLiteTypes.h"
#include <vector>
#include <map>
#include <deque>
using namespace std;

class CSQLiteHelper
{
public:
    CSQLiteHelper();
    virtual ~CSQLiteHelper();
    void setDatabasePath(SLiteString strPath);

    bool open(SLiteString strPath);

    bool close();

    bool isOpen();

    bool exec();

    bool exec(SLiteString strQuery);
	
    void prepare(SLiteString strQuery);

	// current operation error code
    int error() const;

	// check table exist status in database
	bool tableExist(SLiteString strTableName);

	// close/open synchronous 
	bool offSynchronous(bool bOff);

	CSQLiteRecords& record();

public:
	// begin transaction
	bool begin();

	// commit transaction
	bool commit();

	// rollback transaction
	bool rollback();

protected:
	// check path file is exist
    bool filePathIsExist();

	// get fields name in query type is select
	bool getFields(string szTableName);

private:
	bool exec(const char* sql);

protected:
    sqlite3* m_pDB;
    int m_nError;
    SLiteString m_strPath;
    CSQLiteRecords m_records;
	SLiteString m_strQuery;
};



#endif
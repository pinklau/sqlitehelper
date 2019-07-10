#include "SQLiteHelper.h"
#include <algorithm> 
#include "CSQLiteString.h"

#if defined(_WIN32) || defined(_WIN64)
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#include <unistd.h>   
#include <fcntl.h>   
#endif

CSQLiteHelper::CSQLiteHelper()
:m_pDB(NULL)
,m_nError(0)
#if defined(UNICODE) || defined(_UNICODE)
,m_strPath(L"")
,m_strQuery(L"")
#else
, m_strPath("")
, m_strQuery("")
#endif
{

}

CSQLiteHelper::~CSQLiteHelper()
{

}

bool CSQLiteHelper::open(SLiteString strPath)
{
	m_strPath = strPath;
    if (filePathIsExist())
    {
        m_nError = sqlite3_open(convertString(strPath).data(), &m_pDB);
    }
    else
    {
        m_nError = sqlite3_open_v2(convertString(strPath).data(), &m_pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    }
    
    if (m_nError = SQLITE_OK)
    {
        return true;
    }

    return false;
}

bool CSQLiteHelper::close()
{
    if (m_pDB)
    {
        m_nError = sqlite3_close(m_pDB);
        if (m_nError == SQLITE_OK)
        {
            return true;
        }
    }

    return false;
}

bool CSQLiteHelper::isOpen()
{
    return ((m_pDB != NULL) && (m_nError == SQLITE_OK));
}

bool CSQLiteHelper::filePathIsExist()
{
#if defined(_WIN32) || defined(_WIN64)
	if (PathFileExists(m_strPath.data()))
	{
		return true;
	}
#else
	if (access(convertString(m_strPath).data(), F_OK) != -1)
	{
		return true;
	}
#endif
	return false;
}

void CSQLiteHelper::setDatabasePath(SLiteString strPath)
{
	m_strPath = strPath;
}

bool CSQLiteHelper::exec(SLiteString strQuery)
{
	if (!isOpen())
		return false;

	char* errMsg = NULL;
	int err = -1;
	transform(strQuery.begin(), strQuery.end(), strQuery.begin(), ::tolower);

	string sql = convertString(strQuery).data();

	if (sql.find("select") != string::npos)
	{
		sqlite3_stmt* st = NULL;
		const char* szTail = NULL;
		sqlite3_prepare_v3(m_pDB, sql.data(), -1, SQLITE_PREPARE_NORMALIZE, &st, &szTail);
		int index = 0;
		while (sqlite3_step(st) == SQLITE_ROW)
		{
			int nCount = sqlite3_column_count(st);
			
			for (int i = 0; i < nCount; i++)
			{
				int sqlType = sqlite3_column_type(st, i);
				m_records.append(index, i, sqlType, sqlite3_column_text(st, i));
			}
			index++;
		}
		sqlite3_finalize(st);
		string szTable = getTableName(sql);
		getFields(szTable);
#if defined(UNICODE) || defined(_UNICODE)
		m_records.setTableName(convert2Wchar(szTable.data()));
#else
		m_records.setTableName(szTable);
#endif
	}
	else
	{
		err= sqlite3_exec(m_pDB, sql.data(), &CSQLiteRecords::results, &m_records, &errMsg);
	}
	
	
	errMsg != NULL ? printf("%s\n", errMsg) : printf("no error\n");
	if (err == SQLITE_OK)
	{
		return true;
	}
	return false;
}

bool CSQLiteHelper::exec()
{	
	return exec(m_strQuery);
}

void CSQLiteHelper::prepare(SLiteString strQuery)
{
	m_strQuery = strQuery;
}

bool CSQLiteHelper::tableExist(SLiteString strTableName)
{
	string str = "SELECT count(*)" + convertString(strTableName) + " FROM sqlite_master WHERE type = 'table' AND name = '";
	str += convertString(strTableName) + "'";
	char *errMsg;
	int err = sqlite3_exec(m_pDB, str.data(), &CSQLiteRecords::results, &m_records, &errMsg);
	errMsg != NULL ? printf("%s\n", errMsg) : printf("no error\n");
	if (err == SQLITE_OK)
	{
		if (m_records.count())
		{
			return true;
		}

	}
	return false;
}

bool CSQLiteHelper::getFields(string szTableName)
{
	string sql = "select * from " + szTableName + " limit 0,1";
	sqlite3_stmt *st = NULL;
	char **szResult = NULL, *errMsg = NULL;
	int row, col;
	int err = sqlite3_get_table(m_pDB, sql.data(), &szResult, &row, &col, &errMsg);
	bool bRet = false;
	if (err == SQLITE_OK)
	{
		bRet = true;
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				m_records.appendFieldName(col*i + j, *(szResult + col*i + j));
			}
		}
	}

	if (errMsg)
	{
		sqlite3_free(errMsg);
	}

	if (szResult)
		sqlite3_free_table(szResult);
	return bRet;
}

bool CSQLiteHelper::begin()
{
	if (isOpen())
	{
		return exec("begin transaction");
	}
	return false;
}

bool CSQLiteHelper::commit()
{
	if (isOpen())
	{
		return exec("commit transaction");
	}
	return false;
}

bool CSQLiteHelper::rollback()
{
	if (isOpen())
	{
		return exec("rollback transaction");
	}
	return false;
}

bool CSQLiteHelper::exec(const char* sql)
{
	char* errMsg = NULL;
	int err = sqlite3_exec(m_pDB, sql, 0, 0, &errMsg);
	if (err == SQLITE_OK)
		return true;
	return false;
}

bool CSQLiteHelper::offSynchronous(bool bOff)
{
	char* sql;
	if (bOff)
	{
		sql = "PRAGMA synchronous = OFF";
	}
	else
	{
		sql = "PRAGMA synchronous = FULL";
	}
	return exec(sql);
}

CSQLiteRecords& CSQLiteHelper::record()
{
	return m_records;
}
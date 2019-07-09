#include "SQLiteHelper.h"
#include <algorithm> 


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
		getFields(getTableName(sql));
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

	return false;
}

void CSQLiteHelper::prepare(SLiteString strQuery)
{

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


CSQLiteRecords::CSQLiteRecords()
{

}

CSQLiteRecords::~CSQLiteRecords()
{

}

int CSQLiteRecords::results(void*para, int nCount, char** pValue, char** pName)
{

	for (int i = 0; i < nCount; i++)
	{
		printf("field name=%s, value=%s\n", pName[i], pValue[i]);
	}
	return 0;
}

int CSQLiteRecords::count()
{
	return 0;
}

void CSQLiteRecords::clear()
{
	int nCount = m_res.size();
	for (int i = 0; i < nCount; i++)
	{
		if (m_res[i])
		{
			delete m_res[i];
		}
	}
	m_res.clear();
}

void CSQLiteRecords::append(int row, int col, int fieldType, const unsigned char* data)
{
	switch (fieldType)
	{
	case SQLITE_INTEGER:
		printf("%d ", atoi((const char*)data));
		break;

	case SQLITE_FLOAT:
		printf("%f ", atof((const char*)data));
		break;

	case SQLITE_TEXT:
		printf("%s ", data);
		
		break;

	case SQLITE_BLOB:
		break;

	case SQLITE_NULL:
		break;

	default:
		break;
	}
}

void CSQLiteRecords::appendFieldName(int index, const char* szFieldName)
{
#if defined(UNICODE) || defined(_UNICODE)
	m_fieldNames[convert2Wchar(szFieldName)] = index;
#else
	m_fieldNames[szFieldName] = index;
#endif
}
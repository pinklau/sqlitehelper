#include "SQLiteHelper.h"

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
    if (filePathIsExist())
    {
        m_nError = sqlite3_open(convertString(strPath).data(), &m_pDB);
    }
    else
    {
        m_nError = sqlite3_open_v2(convertString(strPath), &m_pDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
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
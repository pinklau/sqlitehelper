#include "SQLiteRecords.h"
#include <algorithm> 
#include "CSQLiteString.h"
#include "sqlite3.h"

#if defined(_WIN32) || defined(_WIN64)
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#else
#include <unistd.h>   
#include <fcntl.h>   
#endif

CSQLiteRecords::CSQLiteRecords()
	:m_nIndex(0)
#if defined(UNICODE) || defined(_UNICODE)
	,m_strTableName(L"")
#else
	m_strTableName("")
#endif
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

int CSQLiteRecords::count() const
{
	return m_res.size();
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
#if defined(UNICODE) || defined(_UNICODE)
	m_strTableName = L"";
#else
	m_strTableName = "";
#endif
}

void CSQLiteRecords::append(int row, int col, int fieldType, const unsigned char* data)
{
	string szData;
	switch (fieldType)
	{
	case SQLITE_INTEGER:
	case SQLITE_FLOAT:
	case SQLITE_TEXT:
	case SQLITE_BLOB:
		szData = (const char*)data;		
		break;

	case SQLITE_NULL:
		szData = "";
		break;

	default:
		break;
	}

	if (row >= m_res.size())
	{
		map<int, string>* pRow = new map<int, string>;
		pRow->insert(pair<int, string>(col, szData));
		m_res.push_back(pRow);
	}
	else
	{
		m_res[row]->insert(pair<int, string>(col, szData));
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

void CSQLiteRecords::setTableName(const SLiteString strTableName)
{
	m_strTableName = strTableName;
}

bool CSQLiteRecords::first()
{
	m_nIndex = 0;
	if (m_res.size()) return true;
	return false;
}

bool CSQLiteRecords::next()
{
	m_nIndex++;
	if (m_nIndex < m_res.size())
		return true;
	return false;
}

SLiteString CSQLiteRecords::valueToString(const int index)
{
	SLiteString strVal;
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return strVal;
#if defined(UNICODE) || defined(_UNICODE)
	strVal = convert2Wchar((*m_res[index])[index].data());
#else
	strVal = (*m_res[index])[index];
#endif
	return strVal;
}

SLiteString CSQLiteRecords::valueToString(const SLiteString strFieldName)
{
	SLiteString strVal;
	map<SLiteString,int>::iterator it = m_fieldNames.find(strFieldName);
	if (it != m_fieldNames.end())
	{
#if defined(UNICODE) || defined(_UNICODE)
		strVal = convert2Wchar((*m_res[m_nIndex])[it->second].data());
#else
		strVal = (*m_res[m_nIndex])[it->second];
#endif
	}
	return strVal;
}

void CSQLiteRecords::valueToDateTime(const int index, struct tm& st)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return;
	char szTime[64] = { 0 };
#if defined(_WIN32) || defined(_WIN64)
	sscanf_s((*m_res[index])[index].data(), "%04d-%02d-%02d %02d:%02d:%02d",
		&st.tm_year, &st.tm_mon, &st.tm_mday, &st.tm_hour, &st.tm_min, &st.tm_sec);
#else
	sscanf((*m_res[index])[index].data(), "%04d-%02d-%02d %02d:%02d:%02d",
		&st.tm_year, &st.tm_mon, &st.tm_mday, &st.tm_hour, &st.tm_min, &st.tm_sec);
#endif
}

void CSQLiteRecords::valueToBlob(const int index, unsigned char* buf, unsigned int nBufSize)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return;
	int len = (*m_res[index])[index].length();
	memcpy(buf, (*m_res[index])[index].data(), len > nBufSize ? nBufSize : len);
}

int CSQLiteRecords::valueToInt(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return atoi((*m_res[index])[index].data());
}

float CSQLiteRecords::valueToFloat(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return (float)atof((*m_res[index])[index].data());
}

double CSQLiteRecords::valueToDouble(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return atof((*m_res[index])[index].data());
}

short CSQLiteRecords::valueToShort(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return (short)atoi((*m_res[index])[index].data());
}

long long CSQLiteRecords::valueToInt64(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return atoll((*m_res[index])[index].data());
}

long CSQLiteRecords::valueToLong(const int index)
{
	if (index < 0 || index >= m_res[m_nIndex]->size())
		return 0;
	return atol((*m_res[index])[index].data());
}

int CSQLiteRecords::columnCount()
{
	return m_fieldNames.size();
}

SLiteString CSQLiteRecords::indexOfColumn(const int index)
{
	SLiteString szFieldName;
	if (index >= 0 && index < m_fieldNames.size())
	{
		for (map<SLiteString, int>::iterator it = m_fieldNames.begin(); it != m_fieldNames.end(); it++)
		{
			if (it->second == index)
				szFieldName = it->first;
		}
	}

	return szFieldName;
}
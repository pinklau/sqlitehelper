#ifndef __READDB__H__
#define __READDB__H__

#include <deque>
#include <string>
#include <map>
using namespace std;

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the READDB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// READDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef READDB_EXPORTS
#define READDB_API __declspec(dllexport)
#else
#define READDB_API __declspec(dllimport)
#endif


#define READDB_EXEC_XML 0 /* executable state is xml */
#define READDB_EXEC_SQL 1 /* executable state is query string*/

// This class is exported from the ReadDB.dll
class READDB_API CReadDB {
public:
	CReadDB(void);
	~CReadDB();

	/*
	* @brief: execute the query string and return executable result
	* @param pszSQL: query string or query xml format data
	* @param type: parameters pszSQL data type READDB_EXEC_XML, READDB_EXEC_SQL
	* @param pRecordset: query return record set
	* @return: if execute is successful then return true else false
	*/

	static BOOL Exce(LPCTSTR pszSQL, int type, CReadDB* pRecorset = NULL);

	/*
	* @brief: initialize of database connecting and configures
	* @return: if true successful initialize then failure
	*/

	static BOOL Init();

	/*
	* @brief: destroy on application close or module detach
	*/

	static BOOL Release();
	
	/*
	* @brief: set database initialize configure parameters,the file type is *.ini
	*     
	*     the file format example:
	*     [DBConf]
	*     DatabaseType=0
	*     Server=127.0.0.1
	*     DatabaseName=Read
	*     UserName=sa
	*     Password=123456
	*     
	*/

	static void SetConf(LPCTSTR pszPath);

	BOOL First();
	BOOL Next();

	int ValueToInt(const int index);

	short ValueToShort(const int index);

	long ValueToLong(const int index);

	__int64 ValueToInt64(const int index);

	float ValueToFloat(const int index);

	double ValueToDouble(const int index);

	BOOL ValueToBlob(const int index, BYTE* buf, UINT size);

	BOOL ValueToBool(const int index);

	BOOL ValueToString(const int index, LPCTSTR pszBuf, UINT size);
	
	int IndexOf(LPCTSTR pszFieldName);

private:

#if defined(UNICODE) || defined(_UNICODE)
	deque< map<int, wstring>*> m_res;
	map<wstring, int> m_fieldsName;
#else
	deque<map<int,string>*> m_res;
	map<string, int> m_fieldsName;
#endif

	int m_nIndex;
};


#endif






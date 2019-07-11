#pragma once

//#import "msado15.dll" rename("EOF", "EndOfFile")
#ifdef _WIN64
#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename("EOF", "EndOfFile")
#elif defined(_WIN32)
#import "C:\Program Files\Common Files (x86)\System\ado\msado15.dll" rename("EOF", "EndOfFile")
#endif

using namespace ADODB;

class CAdoHelper
{
public:
	CAdoHelper();
	virtual ~CAdoHelper();
	BOOL Open(LPCTSTR pszUserName, LPCTSTR pszPwd, LPCTSTR pszServer);
	BOOL Close();
	BOOL IsConnect();
	_ConnectionPtr GetConnection();
	BOOL Exec(LPCTSTR pszSQL);
private:
	_ConnectionPtr m_pConn;
	TCHAR m_szConn[MAX_PATH];
	BOOL m_bCreate;
};

class CAdoHelperRecordset
{
public:
	CAdoHelperRecordset();
	virtual ~CAdoHelperRecordset();

private:
	_RecordsetPtr m_recordset;
};

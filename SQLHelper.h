#ifndef SQL_HELPER__H__
#define SQL_HELPER__H__

#include <stdlib.h>
#include <Windows.h>


#include <string>
#include <map>
#include <vector>

using namespace std;

typedef enum {
	SH_SELECT = 1, /** Select*/
	SH_INSERT,     /** Insert into*/
	SH_UPDATE,     /** Update*/ 
	SH_DELETE      /** Delete*/
}SHEVENT;

typedef enum {
	SHA_GREAT_THAN,    /** > */
	SHA_LESS_THAN,     /** < */
	SHA_EQUAL,         /** = */
	SHA_NO_EQUAL,      /** != */
	SHA_LESS_THAN_EQUAL, /** <= */
	SHA_GREAT_THAN_EQUAL,/** >= */
	SHA_IN,            /** in */
	SHA_AND,           /** And */
	SHA_OR,            /** or */     
	SHA_NOT_NULL,      /** not null*/
	SHA_LIKE,          /** like */
	SHA_NULL           /** IS NULL*/
}SHArithmetic;

extern const char* xmlNodeRoot           ;
extern const char* xmlNodeTableName      ;
extern const char* xmlNodeCommandType    ;
extern const char* xmlNodeFields         ;
extern const char* xmlNodeField          ;
extern const char* xmlNodeConditions     ;
extern const char* xmlNodeConditionLogic ;
extern const char* xmlNodeRecords        ;
extern const char* xmlNodeRecord         ;
extern const char* xmlNodeOrderby        ;
extern const char* xmlNodeGroupby        ;

extern const char* xmlFieldName          ;
extern const char* xmlFieldValue         ;
extern const char* xmlFieldType          ;
extern const char* xmlFieldArithmetic    ;
extern const char* xmlFieldPredicate     ;
extern const char* FieldValueTick        ;  


typedef CMap<CString,LPCTSTR,CString,LPCTSTR> SQL_Field;
typedef CMap<int,int,CString,LPCTSTR> SQL_Row;
typedef CMap<CString, LPCTSTR, int, int> SQL_COL;
typedef CArray<SQL_Field*> SQL_Recordset;
typedef CStringArray SQL_ORDER_GROUP;
typedef CArray<SQL_Row*> SQLSet;

typedef CMap<CString, LPCTSTR, CStringArray*, CStringArray*> SQL_In;

struct tagSQLConditions
{
	SQL_Field* m_equal;
	SQL_Field* m_notEqual;
	SQL_In* m_in;
	SQL_Field* m_greatThan;
	SQL_Field* m_lessThan;
	SQL_ORDER_GROUP* m_notNull;
	SQL_Field* m_like;
	SQL_ORDER_GROUP* m_userString;
	SQL_ORDER_GROUP* m_orderby;
	SQL_Field* m_orderbySort;
	SQL_ORDER_GROUP* m_groupby;
	SQL_Field* m_greatThanEqual;
	SQL_Field* m_lessThanEqual;
	SQL_ORDER_GROUP* m_null;
};

extern void CopySQLField(SQL_Field* pSrc, SQL_Field* pDest);
extern void CopySQLIn(SQL_In* pSrc, SQL_In* pDest);
extern void CopySQLOrderGroup(SQL_ORDER_GROUP* pSrc, SQL_ORDER_GROUP* pDest);

class CSQLHelperFields
{
public:
	CSQLHelperFields();
	virtual ~CSQLHelperFields();

	void Add(LPCTSTR pszFields, LPCTSTR pszValue = TEXT(""));
	void Add(LPCTSTR pszFields, int nValue);
	void Add(LPCTSTR pszFields, double dValue);
	
	// Update,insert
	LPCTSTR GetFieldsAndValues();
	// select
	LPCTSTR GetFields();

	//TiXmlElement* Format2XML();

	void Clean();

	int HaveCount() const;

protected:
	CMap<CString,LPCTSTR,CString,LPCTSTR> m_fields;

};

class CSQLHelperArithmetic
{
public:
	CSQLHelperArithmetic();
	CSQLHelperArithmetic(const CSQLHelperArithmetic& csa);
	~CSQLHelperArithmetic();

	CSQLHelperArithmetic& operator=(const CSQLHelperArithmetic& csa);
	// 将csa中的堆转移到当前对象
	void Move(CSQLHelperArithmetic& csa);
	// bAndOr值为TRUE时表示And，值为FALSE时表示Or
	//void Add(BOOL bAndOr, SHArithmetic arithmetic, sqlString szField, sqlString szValue);
	// 特殊情况使用（非常用语句）如生成复合语句自定义语句等
	void AddUserString(LPCTSTR szValue);
	// bOrderGroup值为TRUE时order by，值为FALSE时group by
	void Add(BOOL bOrderGroup, LPCTSTR szFieldName);
	// bDesc值为TRUE DESC，值为FALSE时为 ASC
	void Add(BOOL bOrderGroup, LPCTSTR szFieldName, BOOL bDesc);
	// 清除设置值
	void Clean();
	// 释放所有资源
	void ReleaseAll();

	void Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, LPCTSTR szValue = TEXT(""), BOOL bMoreValues = FALSE);
	void Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, int nValue, BOOL bMoreValues = FALSE);
	void Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, float fValue, BOOL bMoreValues = FALSE);
	void Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, double fValue, BOOL bMoreValues = FALSE);
	void Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, long fValue, BOOL bMoreValues = FALSE);
protected:
	void put_SQLFields(SQL_Field*& pFields, LPCTSTR szField, LPCTSTR szValue, BOOL bMoreValue = FALSE);
	void put_In(SQL_In*& pIn, LPCTSTR szField, LPCTSTR szValue);
	void put_OrderGroupby(SQL_ORDER_GROUP*& pVec, LPCTSTR szFieldName);
	void put_NotNull(SQL_ORDER_GROUP*& pVec, LPCTSTR szField);
	void put_Like(SQL_Field*& pFields, LPCTSTR szField, LPCTSTR szValue);
	void put_UserString(SQL_ORDER_GROUP*& pFields, LPCTSTR szValue);
	tagSQLConditions* FindField(LPCTSTR pszField);


protected:

#define INDEX_OR          0
#define INDEX_AND         1
#define INDEX_ORDER_GROUP 2

	CMap<int, int, tagSQLConditions*, tagSQLConditions*> m_mapConditions; // and/or
};



class CSQLHelpRecordset
{
public:
	CSQLHelpRecordset();
	~CSQLHelpRecordset();
	void Move(CSQLHelpRecordset& records);

	void SetTableName(LPCTSTR pszTableName);
	CString GetTableName() const;
	void Clean();
	int GetCount() const;
	void Append(int row, int col, LPCTSTR pszValue);
	void Append(int row, LPCTSTR pszFieldName, LPCTSTR pszValue);

public:
	CSQLHelpRecordset& operator=(const CSQLHelpRecordset& res);
	CSQLHelpRecordset& operator+=(const CSQLHelpRecordset & res);
	CSQLHelpRecordset& operator+(const CSQLHelpRecordset& res);
	CSQLHelpRecordset& operator-(const CSQLHelpRecordset & res);
public:
	BOOL First();
	BOOL Next();
	int ValueToInt(const int index);
	short ValueToShort(const int index);
	long ValueToLong(const int index);
	__int64 ValueToInt64(const int index);
	float ValueToFloat(const int index);
	double ValueToDouble(const int index);
	BOOL ValueToBlob(const int index);

protected:
	SQLSet m_recordset;
	CString m_strTableName;
	SQL_COL m_fieldName;

};

class CSQLHelper
{
public:
	CSQLHelper();
	CSQLHelper(const char* szVersion, const char* szEncode, const char* szStandalone);
	virtual ~CSQLHelper();

	string EventID2String(SHEVENT eventID);
	SHEVENT Str2CmdType(string& szAriType);
	// 设置生成XML格式数据的版本，编码，外部定义的DTD等信息
	void SetXMLDeclare(const char* szVersion, const char* szEncode, const char* szStandalone);
	void SetCtrlId(int nDlgId, int nCtrlId);
	void SetTableName(LPCTSTR pszTabelName);
	void SetEventType(SHEVENT eventType);
	void SetPredicate(LPCTSTR pszPredicate);
	string CreateXml();
	string CreateXml(CSQLHelperFields* pFields, CSQLHelperArithmetic* pArithmetic);

	// 将准备生成SQL语句的数据生成XML格式
	string CreateXml(SHEVENT eventID, 
		LPCTSTR szTableName, 
		CSQLHelperFields* pFields = NULL,
		LPCTSTR szPredicate = TEXT(""),
		CSQLHelperArithmetic* pArithmetic = NULL
		);

	// 将XML数据解释成SQL查询字符串
	LPCTSTR ToString(const char* pszXMLData);
	LPCTSTR ToString(const char* pszXMLData, CString& strTable, int& strDlgID, int& strCtrlID);

public:


protected:


private:
	char m_szVersion[24];
	char m_szEncode[24];
	char m_szStandalone[24];
	CString m_strData;
	int m_nDlgId;
	int m_nCtrlId;
	CString m_strTableName;
	CString m_strPredicate;
	SHEVENT m_eventId;
};

#endif
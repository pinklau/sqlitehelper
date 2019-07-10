
#include "SQLHelper.h"
#include <atlconv.h>
//////////////////////////////////////////////////////////////////////////
// CSQLHelper类说明：
// 格式化数据生成XML格式数据传输
//<?xml version="1,0" encoding="GB2312" standalone="yes" ?>
//<DB>
//	<TableName Name="">
//		<CommandType Type="select" Predicate = ""/>
//		<Fields>
//			<Field Name="" Value="">
//           ...
//      <Fields>
//		<Conditions>         如果节点为空 <Conditions/>
//			<ConditionLogic Type="and">
//				<Filed Name="" Value="" Arithmetic=">"/>
//              <Field Name="" Value="" Arithmetic="in"/>
//			</ConditionLogic>
//		</Conditions> 
//		<Groupby>             如果节点为空 <Groupby/>
//			<Field Name="">
//           ...
//      </Groupby>
//		<Orderby>            如果节点为空 <Orderby/>
//			<Field Name="">
//			...
//      </Orderby>
//	</TableName>
//  <UserString>
//		<Field Value="">
//		...
//  </UserString>
//</DB>
//
// 解释接收的XML数据格式
//<?xml version="1,0" encoding="GB2312" standalone="yes" ?>
//<DB>
//	<TableName Name="">
//		<Records Count="" />
//			<Record>
//				<Field Name="" Value="" />
//			</Record>
//		    <Record>
//          ...
//			</Record>
//		</Records>
//	</TableName>
//</DB>

const char* xmlNodeRoot           = ("DB");
const char* xmlNodeTableName      = ("TableName");
const char* xmlNodeCommandType    = ("CommandType");
const char* xmlNodeFields         = ("Fields");
const char* xmlNodeField          = ("Field");
const char* xmlNodeConditions     = ("Conditions");
const char* xmlNodeConditionLogic = ("ConditionLogic");
const char* xmlNodeRecords        = ("Records");
const char* xmlNodeRecord         = ("Record");
const char* xmlNodeOrderby        = ("Orderby");
const char* xmlNodeGroupby        = ("Groupby");
const char* xmlNodeDlg            = "Dlg";

const char* xmlFieldName          = ("Name");
const char* xmlFieldValue         = ("Value");
const char* xmlFieldType          = ("Type");
const char* xmlFieldArithmetic    = ("Arithmetic");
const char* xmlFieldPredicate     = ("Predicate");
const char* FieldValueTick        = (";");
const char* xmlFieldID            = "ID";
const char* xmlFieldCtrlId        = "nControlID";


void CopySQLField(SQL_Field* pSrc, SQL_Field* pDest)
{
	if (!pSrc || !pDest)
		return;

	POSITION pos = pSrc->GetStartPosition();
	while (pos)
	{
		CString strKey, strValue;
		pSrc->GetNextAssoc(pos, strKey, strValue);
		pDest->SetAt(strKey, strValue);
	}
}

void CopySQLIn(SQL_In* pSrc, SQL_In* pDest)
{
	if (!pSrc || !pDest)
		return;

	POSITION pos = pSrc->GetStartPosition();
	while (pos)
	{
		CString strKey;
		CStringArray* pArrs, *pArrDest = NULL;
		pSrc->GetNextAssoc(pos, strKey, pArrs);

		if (pArrs)
		{
			pArrDest = new CStringArray;
			int nCount = pArrs->GetCount();
			for (int i = 0; i < nCount; i++)
			{
				pArrDest->Add(pArrs->GetAt(i));
			}
		}
		pDest->SetAt(strKey, pArrDest);
	}
}

void CopySQLOrderGroup(SQL_ORDER_GROUP* pSrc, SQL_ORDER_GROUP* pDest)
{
	if (!pSrc || !pDest)
		return;

	int nCount = pSrc->GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pDest->Add(pSrc->GetAt(i));
	}
}

//////////////////////////////////////////////////////////////////////////
//CSQLHelperFields
CSQLHelperFields::CSQLHelperFields()
{

}

CSQLHelperFields::~CSQLHelperFields()
{
	Clean();
}


void CSQLHelperFields::Add(LPCTSTR pszFields, LPCTSTR pszValue /* = TEXT("") */)
{
	m_fields.SetAt(pszFields, pszValue);
}

void CSQLHelperFields::Add(LPCTSTR pszFields, int nValue)
{
	CString str;
	str.Format(TEXT("%d"), nValue);
	m_fields.SetAt(pszFields, str);
}

void CSQLHelperFields::Add(LPCTSTR pszFields, double dValue)
{
	CString str;
	str.Format(TEXT("%f"), dValue);
	m_fields.SetAt(pszFields, str);
}

LPCTSTR CSQLHelperFields::GetFieldsAndValues()
{
	static CString strTemp;
	strTemp.Format(TEXT("%s"), TEXT(""));

	POSITION pos = m_fields.GetStartPosition();
	while(pos)
	{
		CString strKey, strValue;
		m_fields.GetNextAssoc(pos, strKey, strValue);
		if (pos)
		{
			strTemp.AppendFormat(TEXT("%s=%s,"), strKey, strValue);
		}
		else
		{
			strTemp.AppendFormat(TEXT("%s=%s"), strKey, strValue);
		}
	}

	return strTemp;
}



LPCTSTR CSQLHelperFields::GetFields()
{
	static CString strTemp;
	strTemp.Format(TEXT("%s"), TEXT(""));

	POSITION pos = m_fields.GetStartPosition();
	while (pos)
	{
		CString strKey, strValue;
		m_fields.GetNextAssoc(pos, strKey, strValue);
		if (pos)
		{
			strTemp.AppendFormat(TEXT("%s,"), strKey);
		}
		else
		{
			strTemp.AppendFormat(TEXT("%s"), strKey);
		}
	}
	return strTemp;
}

void CSQLHelperFields::Clean()
{
	m_fields.RemoveAll();
}


int CSQLHelperFields::HaveCount() const
{
	return m_fields.GetCount();
}

//////////////////////////////////////////////////////////////////////////
//CSQLHelperArithmetic
CSQLHelperArithmetic::CSQLHelperArithmetic()
{

}

CSQLHelperArithmetic::CSQLHelperArithmetic(const CSQLHelperArithmetic& csa)
{
	operator=(csa);
}

CSQLHelperArithmetic::~CSQLHelperArithmetic()
{
	ReleaseAll();
}

CSQLHelperArithmetic& CSQLHelperArithmetic::operator=(const CSQLHelperArithmetic& csa)
{
	POSITION pos = csa.m_mapConditions.GetStartPosition();
	while (pos)
	{
		tagSQLConditions* pCond;
		int nKey;
		csa.m_mapConditions.GetNextAssoc(pos, nKey, pCond);
		if (pCond)
		{
			m_mapConditions[nKey] = new tagSQLConditions;
			if (pCond->m_equal){CopySQLField(pCond->m_equal, m_mapConditions[nKey]->m_equal);}
			if (pCond->m_notEqual){CopySQLField(pCond->m_notEqual, m_mapConditions[nKey]->m_notEqual);}
			if (pCond->m_in){CopySQLIn(pCond->m_in, m_mapConditions[nKey]->m_in);}		
			if (pCond->m_greatThan){CopySQLField(pCond->m_greatThan, m_mapConditions[nKey]->m_greatThan);}
			if (pCond->m_lessThan){CopySQLField(pCond->m_lessThan, m_mapConditions[nKey]->m_lessThan);}	
			if (pCond->m_notNull){CopySQLOrderGroup(pCond->m_notNull, m_mapConditions[nKey]->m_notNull);}
			if (pCond->m_like){CopySQLField(pCond->m_like, m_mapConditions[nKey]->m_like);}

			if (pCond->m_orderby){CopySQLOrderGroup(pCond->m_orderby, m_mapConditions[nKey]->m_orderby);}		
			if (pCond->m_orderbySort){CopySQLField(pCond->m_orderbySort, m_mapConditions[nKey]->m_orderbySort);}		
			if (pCond->m_groupby){CopySQLOrderGroup(pCond->m_groupby, m_mapConditions[nKey]->m_groupby);}		
			if (pCond->m_greatThanEqual){CopySQLField(pCond->m_greatThanEqual, m_mapConditions[nKey]->m_greatThanEqual);}
			if (pCond->m_lessThanEqual){CopySQLField(pCond->m_lessThan, m_mapConditions[nKey]->m_lessThan);}
			if (pCond->m_null){CopySQLOrderGroup(pCond->m_null, m_mapConditions[nKey]->m_null);}
		}		
	}
	return *this;
}

void CSQLHelperArithmetic::Move(CSQLHelperArithmetic& csa)
{
	// 将csa中的堆转移到当前对象
	POSITION pos = csa.m_mapConditions.GetStartPosition();
	while (pos)
	{
		int nKey; 
		tagSQLConditions* pCond;
		csa.m_mapConditions.GetNextAssoc(pos, nKey, pCond);
		m_mapConditions.SetAt(nKey, pCond);
	}
	csa.m_mapConditions.RemoveAll();
}

void CSQLHelperArithmetic::Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, LPCTSTR szValue, BOOL bMoreValue)
{
	// bAndOr值为TRUE时表示And，值为FALSE时表示Or
	tagSQLConditions* pCond = NULL;
	m_mapConditions.Lookup(bAndOr, pCond);
	if (!pCond)
	{
		pCond = new tagSQLConditions;
		memset(pCond, 0, sizeof(tagSQLConditions));
		m_mapConditions.SetAt(bAndOr, pCond);
	}

	switch (arithmetic)
	{
	case SHA_EQUAL:
		put_SQLFields((pCond->m_equal), szField, szValue, bMoreValue);
		break;

	case SHA_NO_EQUAL:
		put_SQLFields((pCond->m_notEqual), szField, szValue, bMoreValue);
		break;

	case SHA_GREAT_THAN:
		put_SQLFields((pCond->m_greatThan), szField, szValue, bMoreValue);
		break;

	case SHA_LESS_THAN:
		put_SQLFields((pCond->m_lessThan), szField, szValue, bMoreValue);
		break;

	case SHA_GREAT_THAN_EQUAL:
		put_SQLFields((pCond->m_greatThanEqual), szField, szValue, bMoreValue);
		break;

	case SHA_LESS_THAN_EQUAL:
		put_SQLFields((pCond->m_lessThanEqual), szField, szValue, bMoreValue);
		break;

	case SHA_IN:
		put_In((pCond->m_in), szField, szValue);
		break;

	case SHA_LIKE:
		put_Like(pCond->m_like, szField, szValue);
		break;

	case SHA_NOT_NULL:
		put_NotNull(pCond->m_notNull, szField);
		break;

	case SHA_NULL:
		put_NotNull(pCond->m_null, szField);
		break;
	default:
		break;
	}
}


void CSQLHelperArithmetic::Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, int nValue, BOOL bMoreValues)
{
	CString strTemp;
	strTemp.Format(TEXT("%d"), nValue);
	Add(bAndOr, arithmetic, szField, strTemp, bMoreValues);
}

void CSQLHelperArithmetic::Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, float fValue, BOOL bMoreValues)
{
	CString strTemp;
	strTemp.Format(TEXT("%f"), fValue);
	Add(bAndOr, arithmetic, szField, strTemp, bMoreValues);
}

void CSQLHelperArithmetic::Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, double fValue, BOOL bMoreValues )
{
	CString strTemp;
	strTemp.Format(TEXT("%g"), fValue);
	Add(bAndOr, arithmetic, szField, strTemp, bMoreValues);
}

void CSQLHelperArithmetic::Add(BOOL bAndOr, SHArithmetic arithmetic, LPCTSTR szField, long nValue, BOOL bMoreValues)
{
	CString strTemp;
	strTemp.Format(TEXT("%d"), nValue);
	Add(bAndOr, arithmetic, szField, strTemp, bMoreValues);
}

void CSQLHelperArithmetic::AddUserString(LPCTSTR szValue)
{

}

void CSQLHelperArithmetic::Add(BOOL bOrderGroup, LPCTSTR szFieldName)
{
	tagSQLConditions* pCond = NULL; 
	m_mapConditions.Lookup(INDEX_ORDER_GROUP, pCond);
	if (pCond)
	{
		if (bOrderGroup)
		{
			put_OrderGroupby((pCond->m_orderby), szFieldName);
		}
		else
		{
			put_OrderGroupby((pCond->m_groupby), szFieldName);
		}
	}
	else
	{
		pCond = new tagSQLConditions;
		memset(pCond, 0, sizeof(tagSQLConditions));

		if (bOrderGroup) 
			put_OrderGroupby(pCond->m_orderby, szFieldName);
		else 
			put_OrderGroupby(pCond->m_groupby, szFieldName);
		m_mapConditions.SetAt(INDEX_ORDER_GROUP, pCond);
	}
}

void CSQLHelperArithmetic::Add(BOOL bOrderGroup, LPCTSTR szFieldName, BOOL bDesc /* = FALSE */)
{
	tagSQLConditions* pCond = NULL;
	m_mapConditions.Lookup(INDEX_ORDER_GROUP, pCond);
	if (pCond)
	{
		if (bOrderGroup)
		{
			put_OrderGroupby((pCond->m_orderby), szFieldName);
			if (!pCond->m_orderbySort)
				pCond->m_orderbySort = new SQL_Field;
			if (bDesc)
			{					
				pCond->m_orderbySort->SetAt(szFieldName, TEXT("DESC"));
			}
			else
			{
				pCond->m_orderbySort->SetAt(szFieldName, TEXT("ASC"));
			}
		}
		else
		{
			put_OrderGroupby((pCond->m_groupby), szFieldName);
			
		}
	}
	else
	{
		pCond = new tagSQLConditions;
		memset(pCond, 0, sizeof(tagSQLConditions));

		if (bOrderGroup) 
		{	
			put_OrderGroupby(pCond->m_orderby, szFieldName);
			if (!pCond->m_orderbySort) 
				pCond->m_orderbySort = new SQL_Field;
			if (bDesc)
			{					
				pCond->m_orderbySort->SetAt(szFieldName, TEXT("DESC"));
			}
			else
			{
				pCond->m_orderbySort->SetAt(szFieldName, TEXT("ASC"));
			}
		}
		else 
		{	
			put_OrderGroupby(pCond->m_groupby, szFieldName);
			
		}
		m_mapConditions.SetAt(INDEX_ORDER_GROUP, pCond);
	}
}

void CSQLHelperArithmetic::Clean()
{
	POSITION pos = m_mapConditions.GetStartPosition();
	while (pos)
	{
		int nKey;
		tagSQLConditions* pCond;
		m_mapConditions.GetNextAssoc(pos, nKey, pCond);
		if (pCond)
		{
			if (pCond->m_equal)
			{
				pCond->m_equal->RemoveAll();
			}
			if (pCond->m_notEqual)
				pCond->m_notEqual->RemoveAll();
			if (pCond->m_greatThan)
				pCond->m_greatThan->RemoveAll();
			if (pCond->m_lessThan)
				pCond->m_lessThan->RemoveAll();
			if (pCond->m_in)
			{
				POSITION pos = pCond->m_in->GetStartPosition();
				while (pos)
				{
					CString strKey;
					CStringArray* pArr = NULL;
					pCond->m_in->GetNextAssoc(pos, strKey, pArr);
					if (pArr)
						delete pArr;
				}
				pCond->m_in->RemoveAll();	
			}
			if (pCond->m_orderby)
				pCond->m_orderby->RemoveAll();
			if (pCond->m_orderbySort)
				pCond->m_orderbySort->RemoveAll();
			if (pCond->m_groupby)
				pCond->m_groupby->RemoveAll();
			if (pCond->m_notNull)
				pCond->m_notNull->RemoveAll();
			if (pCond->m_like)
				pCond->m_like->RemoveAll();
			if (pCond->m_userString)
				pCond->m_userString->RemoveAll();
			if (pCond->m_greatThanEqual)
				pCond->m_greatThanEqual->RemoveAll();
			if (pCond->m_lessThanEqual)
				pCond->m_lessThanEqual->RemoveAll();
			if (pCond->m_null)
				pCond->m_null->RemoveAll();
		}
	}
}

void CSQLHelperArithmetic::ReleaseAll()
{
	POSITION pos = m_mapConditions.GetStartPosition();
	while (pos)
	{
		int nKey;
		tagSQLConditions* pCond;
		m_mapConditions.GetNextAssoc(pos, nKey, pCond);
		if (pCond)
		{
			if (pCond->m_equal)
			{	pCond->m_equal->RemoveAll(); delete pCond->m_equal;}
			if (pCond->m_notEqual)
			{	pCond->m_notEqual->RemoveAll(); delete pCond->m_notEqual;}
			if (pCond->m_in)
			{	
				POSITION pos = pCond->m_in->GetStartPosition();
				while (pos)
				{
					CString strKey;
					CStringArray* pArr = NULL;
					pCond->m_in->GetNextAssoc(pos, strKey, pArr);
					if (pArr)
						delete pArr;
				}
				pCond->m_in->RemoveAll(); 				
				delete pCond->m_in;
			}
			if (pCond->m_greatThan) 
			{	pCond->m_greatThan->RemoveAll(); delete pCond->m_greatThan;}
			if (pCond->m_lessThan)
			{	pCond->m_lessThan->RemoveAll(); delete pCond->m_lessThan;}
			if (pCond->m_notNull)
			{pCond->m_notNull->RemoveAll(); delete pCond->m_notNull;}
			if (pCond->m_like)
			{ pCond->m_like->RemoveAll(); delete pCond->m_like;}
			if (pCond->m_userString)
			{ pCond->m_userString->RemoveAll(); delete pCond->m_userString;}
			if (pCond->m_orderby)
			{	pCond->m_orderby->RemoveAll(); delete pCond->m_orderby;}
			if (pCond->m_orderbySort)
			{	pCond->m_orderbySort->RemoveAll(); delete pCond->m_orderbySort; }
			if (pCond->m_groupby)
			{	pCond->m_groupby->RemoveAll(); delete pCond->m_groupby;	}
			if (pCond->m_greatThanEqual)
			{	pCond->m_greatThanEqual->RemoveAll(); delete pCond->m_greatThanEqual; }
			if (pCond->m_lessThanEqual)
			{	pCond->m_lessThanEqual->RemoveAll(); delete pCond->m_lessThanEqual;}
			if (pCond->m_null)
			{
				pCond->m_null->RemoveAll(); delete pCond->m_null;
			}
			delete pCond;
		}

	}
	m_mapConditions.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////

CSQLHelper::CSQLHelper()
:m_strData(_T(""))
,m_nCtrlId(0)
,m_nDlgId(0)
,m_strPredicate(_T(""))
,m_strTableName(_T(""))
,m_eventId(SH_SELECT)
{
	SetXMLDeclare("1.0", "GB2312", "yes");
}

CSQLHelper::CSQLHelper(const char* szVersion, const char* szEncode, const char* szStandalone)
{
	if (strlen(szVersion) && strlen(szEncode) && strlen(szStandalone))
		SetXMLDeclare(szVersion, szEncode, szStandalone);
	else
		SetXMLDeclare("1.0", "GB2312", "yes");
}

CSQLHelper::~CSQLHelper()
{

}

void CSQLHelper::SetXMLDeclare(const char* szVersion, const char* szEncode, const char* szStandalone)
{
	memset(m_szVersion, 0, sizeof(m_szVersion));
	memset(m_szEncode, 0, sizeof(m_szEncode));
	memset(m_szStandalone, 0, sizeof(m_szStandalone));
	memcpy(m_szVersion, szVersion, sizeof(m_szVersion));
	memcpy(m_szEncode, szEncode, sizeof(m_szEncode));
	memcpy(m_szStandalone, szStandalone, sizeof(szStandalone));
}

void CSQLHelper::SetCtrlId(int nDlgId, int nCtrlId)
{
	m_nCtrlId = nCtrlId;
	m_nDlgId = nDlgId;
}

void CSQLHelper::SetTableName(LPCTSTR pszTableName)
{
	m_strTableName.Format(TEXT("%s"), pszTableName);
}

void  CSQLHelper::SetPredicate(LPCTSTR pszPredicate)
{
	m_strPredicate.Format(TEXT("%s"), pszPredicate);
}

void CSQLHelper::SetEventType(SHEVENT eventType)
{
	m_eventId = eventType;
}

string CSQLHelper::CreateXml(SHEVENT eventID, 
	LPCTSTR szTableName, 
	CSQLHelperFields* pFields /* = NULL */, 
	LPCTSTR szPredicate /* = "" */, 
	CSQLHelperArithmetic* pArithmetic /* = NULL */ )
{
	string szResult = "";

	return szResult;
}

LPCTSTR CSQLHelper::ToString(const char* pszXMLData)
{
	return m_strData;
}

LPCTSTR CSQLHelper::ToString(const char* pszXMLData, CString& strTable, int& nDlgID, int& nCtrlID)
{

	return m_strData;
}



string CSQLHelper::CreateXml(CSQLHelperFields* pFields, CSQLHelperArithmetic* pArithmetic)
{
	return CreateXml(m_eventId, m_strTableName, pFields, m_strPredicate, pArithmetic);
}

string CSQLHelper::EventID2String(SHEVENT eventID)
{
	string szEvent;
	switch (eventID)
	{
	case SH_DELETE:
		szEvent = ("DELETE");
		break;

	case SH_SELECT:
		szEvent = ("SELECT");
		break;

	case SH_UPDATE:
		szEvent = ("UPDATE");
		break;

	case SH_INSERT:
		szEvent = ("INSERT INTO");
		break;
	}
	return szEvent;
}


SHEVENT CSQLHelper::Str2CmdType(string& szAriType)
{
	SHEVENT eventType;

	if (!szAriType.compare(("SELECT")))
		eventType = SH_SELECT;
	else if (!szAriType.compare(("UPDATE")))
		eventType = SH_UPDATE;
	else if (!szAriType.compare(("DELETE")))
		eventType = SH_DELETE;
	else if (!szAriType.compare(("INSERT INTO")))
		eventType = SH_INSERT;

	return eventType;
}




//////////////////////////////////////////////////////////////////////////
// CSQLHelpRecordset
CSQLHelpRecordset::CSQLHelpRecordset()
{

}

CSQLHelpRecordset::~CSQLHelpRecordset()
{
	Clean();
}

void CSQLHelpRecordset::Move(CSQLHelpRecordset& records)
{

}


void CSQLHelpRecordset::SetTableName(LPCTSTR pszTableName)
{
	ASSERT(pszTableName != NULL);
	m_strTableName.Format(TEXT("%s"), pszTableName);
}

CString CSQLHelpRecordset::GetTableName() const
{
	return m_strTableName;
}

void CSQLHelpRecordset::Clean()
{

}

int CSQLHelpRecordset::GetCount() const
{
	return m_recordset.GetCount();
}



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

#ifdef __cplusplus
#define READDB_CAPI extern "C"
#else
#define READDB_CAPI
#endif

class CRecordsets;

// This class is exported from the ReadDB.dll
class READDB_API CReadDB {
public:
	CReadDB(void);
	~CReadDB();
	BOOL Exce(LPCTSTR pszSQL);

	BOOL First();
	BOOL Next();

	int ValueToInt(const int index);
	short ValueToShort(const int index);
	long ValueToLong(const int index);
	__int64 ValueToInt64(const int index);
	float ValueToFloat(const int index);
	double ValueToDouble(const int index);
	
	int IndexOf(LPCTSTR pszFieldName);

};

extern READDB_API int nReadDB;
extern READDB_API LPCTSTR g_pszConfPath;

READDB_API READDB_API int fnReadDB(void);
READDB_CAPI READDB_API BOOL Request(const char* szXML, CReadDB* pRecords);
READDB_CAPI READDB_API BOOL Init();
READDB_CAPI READDB_API BOOL Release();

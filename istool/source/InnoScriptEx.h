// InnoScriptEx.h: interface for the CInnoScriptEx class.

#pragma once

typedef struct {
	WORD	Build;
	BYTE	Minor, Major;
} TSetupVersionDataVersion;

typedef struct {
	TSetupVersionDataVersion	WinVersion, NTVersion;
	WORD						NTServicePack;
} TSetupVersionData;

class CInnoScriptEx : public CInnoScript {
public:
	CInnoScriptEx();
	virtual ~CInnoScriptEx();
	void GetList(SECTION sec,CScriptList& list);
	void GetCompleteList(CScriptList& list);
	void AddLine(CLine*);
	void DeleteLine(CLine*);

	UINT CountLines(SECTION sec);
	void AddComment(SECTION sec,LPCTSTR pszComment);

	bool GetPropertyBool(LPCTSTR pszName,SECTION sec=SEC_SETUP);
	LONG GetPropertyNumber(LPCTSTR pszName,SECTION sec=SEC_SETUP);
	LPCTSTR GetPropertyString(LPCTSTR pszName,SECTION sec=SEC_SETUP);
	void SetPropertyBool(LPCTSTR pszName,bool bValue,SECTION sec=SEC_SETUP);
	void SetPropertyNumber(LPCTSTR pszName,LONG nValue,SECTION sec=SEC_SETUP);
	void SetPropertyString(LPCTSTR pszName,LPCTSTR pszValue,SECTION sec=SEC_SETUP);

	static int GetDeleteType(LPCTSTR pszType);
	static LPCTSTR GetDeleteType(int nType);
	static int GetDirExistsWarning(LPCTSTR pszValue);
	static LPCTSTR GetDirExistsWarning(int nMode);
	static int GetPrivilegesRequired(LPCTSTR pszValue);
	static LPCTSTR GetPrivilegesRequired(int nMode);
	static int GetRestartComputer(LPCTSTR pszValue);
	static LPCTSTR GetRestartComputer(int nMode);
	static int GetBackColorDirection(LPCTSTR pszValue);
	static LPCTSTR GetBackColorDirection(int nMode);
	static int GetUninstallLogMode(LPCTSTR pszValue);
	static LPCTSTR GetUninstallLogMode(int nMode);
	static int GetRegistryRoot(LPCTSTR pszValue);
	static LPCTSTR GetRegistryRoot(int nRoot);
	static int GetRegistryValueType(LPCTSTR pszValue);
	static LPCTSTR GetRegistryValueType(int nRoot);
	static int GetLanguageDetectionMethod(LPCTSTR pszValue);
	static LPCTSTR GetLanguageDetectionMethod(int nMode);
	static int GetInternalCompressLevel(LPCTSTR pszValue);
	static LPCTSTR GetInternalCompressLevel(int nMode);
	int GetShowLanguageDialog(LPCTSTR pszValue);
	LPCTSTR GetShowLanguageDialog(int nMode);

	static void GetDestDir(CLine* pLine,CString& ref);
	static void GetDestName(CLine* pLine,CString& ref);
	static void GetDestTitle(CLine* pLine,CString& ref);
	static bool GetBoolean(LPCTSTR pszValue);
	
	static void SetString(CLine* pLine,bool bForce,LPCTSTR pszName,LPCTSTR pszValue);
	static void SetLong(CLine* pLine,bool bForce,LPCTSTR pszName,LONG nDefault,LONG nValue);
	static void SetFlag(CLine* pLine,LPCTSTR pszName,LPCTSTR pszFlag,int nFlag);

	LPCTSTR GetDefault(LPCTSTR pszName,SECTION sec=SEC_SETUP);

	typedef struct {
		LPCTSTR	m_pszString;
		int		m_nValue;
	} CConverter;

	// File system additions
	static bool GetFolderName(CString& ref,CScriptLine* p);
	static LPCTSTR GetConstantName(LPCTSTR pszConstant);
	static LPCTSTR GetRootName(LPCTSTR pszRoot);
	static LPCTSTR GetDisplayName(CScriptLine* p);
	static bool IsFolderConstant(LPCTSTR pszConstant);

	typedef struct {
		LPCTSTR	m_pszConstant;
		LPCTSTR	m_pszName;
		bool	m_bFolder;
	} CConstantInfo;

	static const CConstantInfo	m_constants[];
	static bool StrToVersionNumbers(const CString& S,TSetupVersionData& VerData);

	CString GetMessage(const CString& strName,CString strHomeDir);

	// BEGIN FROM DOC
	// END FROM DOC

protected:
	static void Split(const CString& Str,TSetupVersionDataVersion& Ver,WORD& ServicePack);
	static int Convert(const CConverter* pData,LPCTSTR pszValue);
	static LPCTSTR Convert(const CConverter* pData,int nValue);
};

/**
** @file
** @brief Miscellaneous classes not placed elsewhere
*/

#pragma once

/// My namespace
namespace Henden {

/** 
** @brief This class will initialize COM in your application.
** 
** Initializes COM for your application.
** @sa CUseOle
*/
class CUseCom {
public:
	/// Constructor
	CUseCom() { 
		::CoInitialize(NULL); 
	}

	/// Destructor
	~CUseCom() { 
		::CoUninitialize(); 
	}
};

/**
** @brief This class will initialize OLE in your application.
**
** Initializes OLE for your application. Normally you will
** use CUseCom, but for some projects use this class instead.
** @sa CUseCom
*/
class CUseOle {
public:
	/// Constructor
	CUseOle() { 
		::OleInitialize(NULL); 
	}

	/// Destructor
	~CUseOle() { 
		::OleUninitialize(); 
	}
};

/**
** @brief This class encapsulates the SHGetFolderPath function.
**
** Allows the use of SHGetFolderPath on all systems.
*/
class CShellFolder {
private:
	CShellFolder() {}
public:
	static HRESULT GetFolderPath(HWND hwndOwner,int nFolder,HANDLE hToken,DWORD dwFlags,LPTSTR pszPath) {
		HRESULT (WINAPI* _SHGetFolderPath)(HWND,int,HANDLE,DWORD,LPTSTR);
		HMODULE hModule;

		hModule = LoadLibrary(_T("shell32.dll"));
		if(!hModule) return E_FAIL;
		_SHGetFolderPath = (HRESULT (WINAPI*)(HWND,int,HANDLE,DWORD,LPTSTR))GetProcAddress(hModule,"SHGetFolderPathA");
		if(_SHGetFolderPath) {
			HRESULT hr = _SHGetFolderPath(hwndOwner,nFolder,hToken,dwFlags,pszPath);
			FreeLibrary(hModule);
			return hr;
		}
		FreeLibrary(hModule);

		hModule = LoadLibrary(_T("shfolder.dll"));
		if(!hModule) return E_FAIL;
		_SHGetFolderPath = (HRESULT (WINAPI*)(HWND,int,HANDLE,DWORD,LPTSTR))GetProcAddress(hModule,"SHGetFolderPathA");
		if(_SHGetFolderPath) {
			HRESULT hr = _SHGetFolderPath(hwndOwner,nFolder,hToken,dwFlags,pszPath);
			FreeLibrary(hModule);
			return hr;
		}
		FreeLibrary(hModule);
		return E_FAIL;
	}
};

/**
** @brief A class for storing application information in ini-files.
**
** This class is used to read values from, and write values to ini-files.
** @sa CRecentDocumentList
*/
class CAppFile {
public:
	/// Constructor
	CAppFile(UINT uID) {
		CString str;
		str.LoadString(uID);
#if 1
		CShellFolder::GetFolderPath(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,m_strAppFile.GetBuffer(MAX_PATH));
#else
		::SHGetFolderPath(NULL,CSIDL_APPDATA,NULL,SHGFP_TYPE_CURRENT,m_strAppFile.GetBuffer(MAX_PATH));
#endif
		m_strAppFile.ReleaseBuffer();
		Henden::CUtils::EndWith(m_strAppFile,_T('\\'));
		m_strAppFile += str;
		::CreateDirectory(m_strAppFile,NULL);
		Henden::CUtils::EndWith(m_strAppFile,_T('\\'));
		m_strAppFile += str;
		m_strAppFile += _T(".ini");
		//AtlMessageBox(NULL,(LPCTSTR)m_strAppFile);
	}

	/// Returns the name of the ini-file.
	LPCTSTR GetAppFile() const {
		return m_strAppFile;
	}

	/// Returns the name of the ini-file.
	operator LPCTSTR() const {
		return GetAppFile();
	}

	/// Saves a DWORD value to the ini-file.
	bool SetDWORDValue(LPCTSTR pszSection,LPCTSTR pszValueName,DWORD dwValue) {
		CString tmp;
		tmp.Format(_T("%d"),dwValue);
		return ::WritePrivateProfileString(pszSection,pszValueName,tmp,GetAppFile())!=FALSE;
	}

	/// Saves a STRING value to the ini-file.
	bool SetStringValue(LPCTSTR pszSection,LPCTSTR pszValueName,LPCTSTR pszValue) {
		return ::WritePrivateProfileString(pszSection,pszValueName,pszValue,GetAppFile())!=FALSE;
	}

	/// Deletes a value from the ini-file.
	bool DeleteValue(LPCTSTR pszSection,LPCTSTR pszValueName) {
		return ::WritePrivateProfileString(pszSection,pszValueName,NULL,GetAppFile())!=FALSE;
	}

	/// Queries a DWORD value from the ini-file.
	bool QueryDWORDValue(LPCTSTR pszSection,LPCTSTR pszValueName,DWORD& dwRet) {
		const DWORD dwMagic = 0xFFCCFFCC;
		dwRet = ::GetPrivateProfileInt(pszSection,pszValueName,dwMagic,GetAppFile());
		return dwRet!=dwMagic;
	}

	/// Queries a STRING value from the ini-file.
	bool QueryStringValue(LPCTSTR pszSection,LPCTSTR pszValueName,CString& strRet) {
		const LPCTSTR pszMagic = _T("dummy_wtl");
		DWORD dwRet, dwSize = MAX_PATH;
		do {
			dwSize <<= 1;
			dwRet = ::GetPrivateProfileString(pszSection,pszValueName,pszMagic,strRet.GetBuffer(dwSize),dwSize,GetAppFile());
		} while(dwRet==dwSize-1);
		strRet.ReleaseBuffer(dwRet);
		return strRet.Compare(pszMagic)!=0;
	}

	/// Saves a BINARY value to the ini-file.
	bool SetBinaryValue(LPCTSTR pszSection,LPCTSTR pszValueName,LPVOID lpStruct,UINT uSize) {
		return ::WritePrivateProfileStruct(pszSection,pszValueName,lpStruct,uSize,GetAppFile())!=0;
	}

	/// Queries a BINARY value from the ini-file.
	bool QueryBinaryValue(LPCTSTR pszSection,LPCTSTR pszValueName,LPVOID lpStruct,UINT uSize) {
		return ::GetPrivateProfileStruct(pszSection,pszValueName,lpStruct,uSize,GetAppFile())!=0;
	}

private:
	CString	m_strAppFile;	// Holds the name of the ini file.
};

#ifdef _WTL_MRUEMPTY_TEXT

/**
** @brief Overrides some functions from the WTL class CRecentDocumentListBase
**
** Overrides the WTL class CRecentDocumentListBase to use ini file instead of registry
*/
template <class T, int t_cchItemLen = MAX_PATH, int t_nFirstID = ID_FILE_MRU_FIRST, int t_nLastID = ID_FILE_MRU_LAST>
class CRecentDocumentListBase : public WTL::CRecentDocumentListBase<T,t_cchItemLen,t_nFirstID,t_nLastID> {
private:
	CString	m_strAppFile;
public:
	/// Sets the application ini file
	void SetAppFile(LPCTSTR pszAppFile) {
		m_strAppFile = pszAppFile;
	}

	/// Returns the application ini file
	LPCTSTR GetAppFile() const {
		return m_strAppFile;
	}

	/**
	** @brief Reads the recent file list from the ini file - not the registry
	** @param lpstrRegKey This parameter is ignored
	** @sa WriteToRegistry()
	*/
	BOOL ReadFromRegistry(LPCTSTR lpstrRegKey) {
		T* pT = static_cast<T*>(this);

		DWORD dwRet = 0;
		LRESULT lRet = QueryDWORDValue(_T("DocumentCount"), dwRet);
		if(lRet != ERROR_SUCCESS)
			return FALSE;
		SetMaxEntries(dwRet);

		m_arrDocs.RemoveAll();

		TCHAR szRetString[t_cchItemLen] = { 0 };
		_DocEntry de;

		for(int nItem = m_nMaxEntries; nItem > 0; nItem--)
		{
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
			ULONG ulCount = t_cchItemLen;
			lRet = QueryStringValue(szBuff, szRetString, &ulCount);
			if(lRet == ERROR_SUCCESS && (lstrcpy(de.szDocName, szRetString) != NULL))
				m_arrDocs.Add(de);
		}

		return pT->UpdateMenu();
	}

	/**
	** @brief Writes the recent file list to the ini file - not the registry
	** @param lpstrRegKey This parameter is ignored
	** @sa ReadFromRegistry()
	*/
	BOOL WriteToRegistry(LPCTSTR lpstrRegKey) {
		LRESULT lRet = SetDWORDValue(_T("DocumentCount"), m_nMaxEntries);
		ATLASSERT(lRet == ERROR_SUCCESS);

		// set new values
		int nItem;
		for(nItem = m_arrDocs.GetSize(); nItem > 0; nItem--) {
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
			TCHAR szDocName[MAX_PATH] = { 0 };
			GetFromList(ID_FILE_MRU_FIRST + nItem - 1, szDocName, MAX_PATH);
			lRet = SetStringValue(szBuff, szDocName);
			ATLASSERT(lRet == ERROR_SUCCESS);
		}

		// delete unused keys
		for(nItem = m_arrDocs.GetSize() + 1; nItem < m_nMaxEntries_Max; nItem++)
		{
			const int cchBuff = 11;
			TCHAR szBuff[cchBuff] = { 0 };
			wsprintf(szBuff, _T("Document%i"), nItem);
			DeleteValue(szBuff);
		}

		return TRUE;
	}
private:
	LRESULT SetDWORDValue(LPCTSTR pszValueName,DWORD dwValue) {
		T* pT = static_cast<T*>(this);
		const LPCTSTR pszFileName = m_strAppFile;

		CString tmp;
		tmp.Format(_T("%d"),dwValue);
		::WritePrivateProfileString(pT->GetRegKeyName(),pszValueName,tmp,pszFileName);

		return ERROR_SUCCESS;
	}

	LRESULT SetStringValue(LPCTSTR pszValueName,LPCTSTR pszValue) {
		T* pT = static_cast<T*>(this);
		const LPCTSTR pszFileName = m_strAppFile;

		::WritePrivateProfileString(pT->GetRegKeyName(),pszValueName,pszValue,pszFileName);

		return ERROR_SUCCESS;
	}

	LRESULT DeleteValue(LPCTSTR pszValueName) {
		T* pT = static_cast<T*>(this);
		const LPCTSTR pszFileName = m_strAppFile;

		::WritePrivateProfileString(pT->GetRegKeyName(),pszValueName,NULL,pszFileName);

		return ERROR_SUCCESS;
	}

	LRESULT QueryDWORDValue(LPCTSTR pszValueName,DWORD& dwRet) {
		T* pT = static_cast<T*>(this);
		const LPCTSTR pszFileName = m_strAppFile;

		DWORD dwOld = dwRet;
		dwRet = ::GetPrivateProfileInt(pT->GetRegKeyName(),pszValueName,dwRet,pszFileName);
		return dwRet==dwOld ? ERROR_FILE_NOT_FOUND : ERROR_SUCCESS;
	}

	LRESULT QueryStringValue(LPCTSTR pszValueName,LPTSTR pszValue,ULONG* pnChars) {
		T* pT = static_cast<T*>(this);
		const LPCTSTR pszFileName = m_strAppFile;

		*pnChars = ::GetPrivateProfileString(pT->GetRegKeyName(),pszValueName,_T(""),pszValue,*pnChars,pszFileName);
		return *pszValue ? ERROR_SUCCESS : ERROR_FILE_NOT_FOUND;
	}
};

/** 
** @brief A class for storing a recent file list in a ini-file.
**
** Derived from the WTL class of the same name. This class saves to a ini file instead of registry.
** @sa CAppFile
*/
class CRecentDocumentList : public CRecentDocumentListBase<CRecentDocumentList> {
public:
// nothing here
};

#endif // _WTL_MRUEMPTY_TEXT

} // namespace Henden

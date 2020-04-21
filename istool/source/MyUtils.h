#ifndef _MYUTILS_H
#define _MYUTILS_H

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <io.h>
#include <tchar.h>
#include <winsvc.h>

#define PACKVERSION(major,minor) MAKELONG(minor,major)

class CMyUtils {
public:
	// Returns True if an administrator is logged onto the system. Always returns
	// True on Windows 95/98.
	static bool IsAdminLoggedOn() {
		if(GetVersion() & 0x80000000)
			return true;
		else {
			// Try an admin privileged API
			SC_HANDLE hSC = OpenSCManager(NULL, NULL, GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE);
			if(hSC) {
				CloseServiceHandle(hSC);
				return true;
			}
			return false;
		}
	}

	static bool GetSysError(CString& strError,DWORD dwError,LPCTSTR pszModule=NULL) {
		HMODULE hModule = NULL;
		DWORD dwFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

		if(pszModule) {
			hModule = ::GetModuleHandle(pszModule);
			dwFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}

		if(!dwError) dwError = GetLastError();
		LPVOID lpMsgBuf = NULL;
		if(!::FormatMessage(	
			dwFlags,
			hModule,	 
			dwError,	
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language    
			(LPTSTR)&lpMsgBuf,    
			0,    
			NULL
		)) {
			strError.Format(_T("Error code: %d"),dwError);
			return false;
		} else {
			strError = (LPCTSTR)lpMsgBuf;
			LocalFree(lpMsgBuf); 
			return true;
		}
	}

#if defined(_WININET_)
	static void GetInetResponse(CString& strResponse) {
		DWORD	dwError;
		DWORD	dwBufferLength = 0;

		InternetGetLastResponseInfo(&dwError,strResponse.GetBuffer(0),&dwBufferLength);
		InternetGetLastResponseInfo(&dwError,strResponse.GetBuffer(dwBufferLength),&dwBufferLength);
	}
#endif

	// Returns true if the specified path is relative
	static bool IsRelativePath(LPCTSTR pszPath) {
		int nLen = lstrlen(pszPath);

		if(nLen>=2 && pszPath[1]==_T(':'))
			return false;

		if(nLen>2 && pszPath[0]==_T('\\') && pszPath[1]==_T('\\'))
			return false;

		// This one isn't absolute anyway
		//if(nLen && pszPath[0]=='\\')
		//	return false;

		return true;
	}

	// Returns true if specified file name is a file or directory
	static bool FileExists(LPCTSTR lpszFileName) {
		WIN32_FIND_DATA wfd;
		
		HANDLE hFind = FindFirstFile(lpszFileName,&wfd);
		if(hFind==INVALID_HANDLE_VALUE) return false;
		FindClose(hFind);
		return true;
	}
	
	// Returns true if specified file name is a directory
	static bool IsDirectory(LPCTSTR lpszFileName) {
		DWORD dw = GetFileAttributes(lpszFileName);
		return dw!=INVALID_FILE_ATTRIBUTES && dw & FILE_ATTRIBUTE_DIRECTORY;
	}
	
	// Returns true if specified file name is a file
	static bool IsFile(LPCTSTR lpszFileName) {
		return !IsDirectory(lpszFileName) && FileExists(lpszFileName);
	}

	// Returns true if specified folder was retrieved
	// See SHGetSpecialFolderLocation for folder constants
	static bool GetShellFolderPath(const int nFolder,LPTSTR pszValue) {
		IMalloc* Malloc;
		ITEMIDLIST* pidl;

		*pszValue = _T('\0');
		
		if(FAILED(SHGetMalloc(&Malloc)))
			Malloc = NULL;

		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL, nFolder, &pidl))) {
		    if(!SHGetPathFromIDList(pidl, pszValue))
		    	*pszValue = _T('\0');
		    if(Malloc) Malloc->Free(pidl);
		}
		return *pszValue!=0;
	}

#ifdef __ATLSTR_H__
	// Returns true if specified folder was retrieved
	// See SHGetSpecialFolderLocation for folder constants
	static bool GetShellFolderPath(const int nFolder,CString& strValue) {
		GetShellFolderPath(nFolder,strValue.GetBuffer(_MAX_PATH));
		strValue.ReleaseBuffer();
		return !strValue.IsEmpty();
	}
#endif

	// Displays system messages. Find res with GetLastError()
	static void ShowSysMsg(LONG res) {
		LPVOID lpMsgBuf;
		FormatMessage(	
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |	 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,	 
			res,	
			MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT), // Default language    
			(LPTSTR)&lpMsgBuf,    
			0,    
			NULL
		);
#ifdef _MFC_VER
		AfxMessageBox((char*)lpMsgBuf);
#else
		MessageBox(NULL,(LPTSTR)lpMsgBuf,_T("Information"),MB_OK|MB_ICONINFORMATION);
#endif
		LocalFree(lpMsgBuf); 
	}

	/** Append a character to the end of the string if the string does not
	already end with that character. This is mostly used when building a
	string that contains a file path that should end with a trailing
	VFILE_PATH_SEP_CHAR character. If the string already ends with the
	character specified by nChar, this function will be a NOOP. This
	function will only succeed on a string that already has length. It will
	not set the first character of an otherwise empty string to nChar.*/
	static void EndWith(LPTSTR pszStr,TCHAR nChar) {
		int nLength = (int)_tcslen(pszStr);

		if(nLength) {
			if(*(pszStr + (nLength-1)) != nChar) {
				pszStr[nLength++] = nChar;
				pszStr[nLength] = 0;
			}
		}
	}

//#ifdef _MFC_VER
#ifdef __CSTRINGT_H__
	static void EndWith(CString& ref,TCHAR nChar) {
		int nLength = (int)_tcslen(ref);
		if(!nLength || ref[nLength-1]!=nChar)
			ref += _T('\\');
	}
#endif

#ifdef _MFC_VER
	/*
	** MessageBox with formatting
	*/
	static int MessageBox(UINT nType,LPCTSTR pszText,...) {
		char szBuf[512];
		va_list args;
		va_start(args,pszText);
		wvsprintf(szBuf,pszText,args);
		return AfxMessageBox(szBuf,nType==0 ? MB_OK : nType);
	}
#endif

#if 1
	static bool GetRegString(CRegKey& reg,LPCTSTR pszValueName,CString& out,LPCTSTR pszDefault=NULL) {
		ULONG nChars = 1024;
		if(reg.QueryStringValue(pszValueName,out.GetBuffer(nChars),&nChars)==ERROR_SUCCESS) {
			out.ReleaseBufferSetLength(nChars-1);
			return true;
		} else {
			if(pszDefault)
				out = pszDefault;
			else
				out.Empty();
			return false;
		}
	}
	static bool GetRegLong(CRegKey& reg,LPCTSTR pszValueName,long& out,long nDefault=0) {
		DWORD dw;
		if(reg.QueryDWORDValue(pszValueName,dw)==ERROR_SUCCESS) {
			out = dw;
			return true;
		} else {
			out = nDefault;
			return false;
		}
	}
#endif

	static DWORD GetDllVersion(LPCTSTR lpszDllName) {
		HINSTANCE hinstDll;
		DWORD dwVersion = 0;

		hinstDll = LoadLibrary(lpszDllName);
		
		if(hinstDll) {
			DLLGETVERSIONPROC pDllGetVersion;

			pDllGetVersion = (DLLGETVERSIONPROC) GetProcAddress(hinstDll, "DllGetVersion");
			/*
			** Because some DLLs may not implement this function, you
			** must test for it explicitly. Depending on the particular 
			** DLL, the lack of a DllGetVersion function may
			** be a useful indicator of the version.
			*/
			if(pDllGetVersion) {
				DLLVERSIONINFO dvi;
				HRESULT hr;

				ZeroMemory(&dvi, sizeof(dvi));
				dvi.cbSize = sizeof(dvi);

				hr = (*pDllGetVersion)(&dvi);

				if(SUCCEEDED(hr)) {
					dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
				}
			}
			FreeLibrary(hinstDll);
		}
		return dwVersion;
	}


	/*----------------------------------------------------------------------
	Copyright (c) 1998,1999 Gipsysoft. All Rights Reserved.
	File:	CreateDirectoryPath.cpp
	Owner:	russf@gipsysoft.com
	Purpose:	Create a dfirectory path, that is given "c:\blah\de\blah" it
						will create the entire path even if none of the parent directories
						exist.
						Also, it copes with UNC path names in the form of "\\Russ\C\blah\de\blah"
	----------------------------------------------------------------------*/
	static bool CreateDirectoryPath(LPCTSTR pcszDirectory) {
		//	Must be passed a valid string!
		//ASSERT_VALID_STR( pcszDirectory );

		static TCHAR cSlash = _T('\\');

		bool bRetVal = false;

		const int nLength = (int)_tcslen( pcszDirectory ) + 1;
		LPTSTR pszDirectoryPath = (LPTSTR)malloc( nLength * sizeof( TCHAR ) );
		if( pszDirectoryPath )
		{

			LPCTSTR pcszNextDirectory = pcszDirectory;

			//
			//	Determine if the path is a UNC path. We do this by looking at the first two bytes
			//	and checkin they are both backslashes
			if( nLength > 2 && *pcszNextDirectory == cSlash && *(pcszNextDirectory+1) == cSlash )
			{
				//	We need to skip passed this bit and copy it into out local path.
				//	"\\Russ\C\"
				pcszNextDirectory += 2;
				while( *pcszNextDirectory && *pcszNextDirectory != cSlash )	pcszNextDirectory++;
				pcszNextDirectory++;
				while( *pcszNextDirectory && *pcszNextDirectory != cSlash )	pcszNextDirectory++;
				_tcsncpy( pszDirectoryPath, pcszDirectory, pcszNextDirectory - pcszDirectory );
				pszDirectoryPath[ pcszNextDirectory - pcszDirectory ] = '\000';
			}

			//
			//	Set the return value to true because the nly thing that can fail now is the
			//	CreateDirectory. If that fails then we change the return value back to fals.
			bRetVal = true;

			//
			//	Now, loop over the path, creating directories as we go. If we fail at any point then get out of the loop
			do
			{
				if( *pcszNextDirectory )
					pcszNextDirectory++;

				while( *pcszNextDirectory && *pcszNextDirectory != cSlash && *pcszNextDirectory!='/')
					pcszNextDirectory++;

				_tcsncpy( pszDirectoryPath, pcszDirectory, pcszNextDirectory - pcszDirectory );
				pszDirectoryPath[ pcszNextDirectory - pcszDirectory ] = '\000';

				if( _taccess( pszDirectoryPath, 0 ) )
				{
					if( !CreateDirectory( pszDirectoryPath, NULL ) )
					{
						bRetVal = false;
						break;
					}
				}
			}
			while( *pcszNextDirectory );

			free( pszDirectoryPath );
			pszDirectoryPath = NULL;
		}
		return bRetVal;
	}

	static bool IsWin5() {
		return LOBYTE(::GetVersion())>=5;
	}
};

/*
** CIniFile
*/
class CIniFile {
public:
	CIniFile(LPCTSTR pszFile) : m_strFile(pszFile) {}

	void GetString(LPCTSTR pszSection,LPCTSTR pszKey,CString& ref) {
		DWORD dwResult, dwSize = 128;
		do {
			dwSize *= 2;
			dwResult = GetPrivateProfileString(pszSection,pszKey,NULL,ref.GetBuffer(dwSize),dwSize,m_strFile);
		} while(dwResult==dwSize-1);
		ref.ReleaseBuffer(dwResult);
	}

	LONG GetLong(LPCTSTR pszSection,LPCTSTR pszKey,LONG nDefault) {
		CString str;
		GetString(pszSection,pszKey,str);
		if(str.IsEmpty())
			return nDefault;
		else
			return _ttol(str);
	}

protected:
	CString	m_strFile;
};

/*
** Very simple thread function... see below for example
*/
class CMyThread {
public:
    CMyThread() : _isDying(0) {
        _handle = CreateThread(
            0, // Security attributes
            0, // Stack size
            ThreadEntry,
            this,
            CREATE_SUSPENDED,
            &_tid);
	}

    virtual ~CMyThread() {
		CloseHandle(_handle); 
	}
    
	void Kill(DWORD dwMilliSeconds=INFINITE) {
		_isDying++;
		FlushThread();
		// Let's make sure it's gone
        WaitForSingleObject(_handle, dwMilliSeconds);
	}
    
	void Resume () { ResumeThread (_handle); }

protected:
    int		_isDying;

    virtual void Run () = 0;
    virtual void FlushThread () = 0;

private:
    HANDLE	_handle;
    DWORD	_tid;     // thread id

    static DWORD WINAPI ThreadEntry(void* pArg) {
		reinterpret_cast<CMyThread*>(pArg)->Run();
		return 0;
	}
};

/*
class CTestThread : public CMyThread {
public:
	CTest() {
		Resume();
	}
	virtual void Run() {
		for(UINT i=0;i<100;i++) {
			printf(".");
			fflush(stdout);
			Sleep(100);
			if(_isDying) break;
		}
		printf("!\n");
	}
    virtual void FlushThread() {};
};
*/

class CStringSplitter {
public:
	CStringSplitter() : m_strings(NULL) {
	}
	
	CStringSplitter(LPCTSTR pszString,LPCTSTR pszSplit) : m_strings(NULL) {
		Split(pszString,pszSplit);
	}

	void Split(LPCTSTR pszString,LPCTSTR pszSplit) {
		Free();

		// 1. Scan string and find string count
		UINT nCount = 1;
		LPCTSTR ptr = pszString;
		while(*ptr) {
			if(_tcschr(pszSplit,*ptr)) nCount++;
			ptr++;
		}

		// 2. Allocate array of pointers
		m_strings = new TCHAR*[nCount+1];
		nCount = 0;
		ptr = pszString;
		while(*ptr) {
			UINT nLength = 0;
			while(ptr[nLength] && !_tcschr(pszSplit,ptr[nLength])) nLength++;
			m_strings[nCount] = new TCHAR[nLength+1];
			_tcsncpy(m_strings[nCount],ptr,nLength);
			m_strings[nCount][nLength] = 0;
			nCount++;
			
			if(!ptr[nLength]) break;
			ptr += nLength + 1;
			//while(strchr(pszSplit,*ptr)) ptr++;
		}
		m_strings[nCount] = NULL;
	}

	UINT GetLength() {
		if(!m_strings) return 0;

		TCHAR** ptr = m_strings;
		UINT nCount = 0;
		while(ptr[nCount]) nCount++;
		return nCount;
	}

	LPCTSTR GetAt(UINT n) {
		if(!m_strings) return NULL;
		return m_strings[n];
	}

	LPCTSTR operator[](UINT n) {
		return GetAt(n);
	}

protected:
	void Free() {
		if(m_strings) {
			TCHAR** ptr = m_strings;
			while(*ptr) {
				delete []*ptr;
				ptr++;
			}
			delete []m_strings;
			m_strings = NULL;
		}
	}

	TCHAR**	m_strings;
};


#endif

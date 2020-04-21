#pragma once

class CWebUpdate {
public:
	CWebUpdate(HWND hWnd,LPCTSTR pszAddress) : m_hWnd(hWnd) {
		_tcscpy(m_szAddress,pszAddress);
	}
	
	virtual ~CWebUpdate() {
	}
	
	bool Check(CString& refData) {
		refData.Empty();
		HINTERNET hInternet = InternetOpen(_T("WebUpdate"),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		if(!hInternet) {
			CString strError;
			CMyUtils::GetSysError(strError,GetLastError(),_T("wininet.dll"));
			MsgBox(strError,MB_OK|MB_ICONSTOP);
			return false;
		}

		HINTERNET hFile = InternetOpenUrl(hInternet,
			m_szAddress,
			NULL,
			0,
			INTERNET_FLAG_RELOAD,
			0
		);
		if(!hFile) {
			CString strError;
			CMyUtils::GetSysError(strError,GetLastError(),_T("wininet.dll"));
			MsgBox(strError,MB_OK|MB_ICONSTOP);
			InternetCloseHandle(hInternet);
			return false;
		}

		char buffer[4097];
		CString strData;
		DWORD dwBytes = 0;
		BOOL bOk;
		while(bOk = InternetReadFile(hFile,buffer,sizeof buffer-1,&dwBytes)) {
			if(!dwBytes) break;
			buffer[dwBytes] = _T('\0');
			strData += buffer;
		}
		if(!bOk) {
			CString strError;
			CMyUtils::GetSysError(strError,GetLastError(),_T("wininet.dll"));
			MsgBox(strError,MB_OK|MB_ICONSTOP);
			InternetCloseHandle(hFile);
			InternetCloseHandle(hInternet);
			return false;
		}

		InternetCloseHandle(hFile);
		InternetCloseHandle(hInternet);

		refData = strData;
		return true;
	}

protected:
	int MsgBox(LPCTSTR pszText,UINT uType=MB_OK|MB_ICONINFORMATION) {
		return ::MessageBox(m_hWnd,pszText,_T("Update Check"),uType);
	}

	HWND	m_hWnd;
	TCHAR	m_szAddress[MAX_PATH];
};

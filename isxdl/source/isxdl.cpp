// Copyright © 2002-2003 Bjørnar Henden
// isxdl.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "wininet.h"
#include "DownloadDlg.h"

#define DLLAPI  __stdcall

CWinInet	inet;

// To minimize crt use (and save size)
ULONGLONG myatol(LPCTSTR psz) {
	ULONGLONG n = 0;
	while(psz && *psz && *psz>='0' && *psz<='9') {
		n *= 10;
		n += *psz - '0';
		psz++;
	}
	return n;
}

//#define toupper(x) ((x)>='a' && (x)<='z' ? (x) + 'A' - 'a' : (x))
#define toupper(x) (x)

int mystricmp(LPCTSTR psz1,LPCTSTR psz2) {
	for(int i=0;;i++) {
		int c1 = toupper(psz1[i]);
		int c2 = toupper(psz2[i]);
		int res = c1 - c2;
		if(res!=0) return res;
		if(!c1 || !c2) break;
	}
	return 0;
}

BOOL APIENTRY DllMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID lpReserved) {
	if(ul_reason_for_call==DLL_PROCESS_DETACH)
		CDownloadDlg::CleanupStatic();

    return TRUE;
}

extern "C" long DLLAPI DownloadFiles(HWND hWndParent) {
	if(!CDownloadDlg::GetFileCount()) return 1;	// No files to download, everything ok
	
	CDownloadDlg dlg(hWndParent);
	if(!hWndParent) hWndParent = ::GetActiveWindow();
	dlg.DoModal(hWndParent);
	return dlg.m_bAllOk ? 1 : 0;
}

extern "C" void DLLAPI AddFile(LPCTSTR pszURL,LPCTSTR pszFile) {
	CDownloadDlg::AddFile(pszURL,pszFile);
}

extern "C" void DLLAPI AddFileSize(LPCTSTR pszURL,LPCTSTR pszFile,DWORD dwSize) {
	CDownloadDlg::AddFile(pszURL,pszFile,dwSize);
}

extern "C" long DLLAPI Download(HWND hWndParent,LPCTSTR pszURL,LPCTSTR pszFile) {
	CDownloadDlg::AddFile(pszURL,pszFile);
	return DownloadFiles(hWndParent);
}

extern "C" void DLLAPI ClearFiles() {
	CDownloadDlg::ClearFiles();
}

// 
extern "C" long DLLAPI IsConnected() {
	return CDownloadDlg::IsConnected();
}

extern "C" long DLLAPI SetOption(LPCTSTR pszOption,LPCTSTR pszValue) {
	if(!lstrcmpi(pszOption,_T("title"))) {
		CDownloadDlg::m_strOptionTitle = pszValue;
	} else if(!lstrcmpi(pszOption,_T("debug"))) {
		CDownloadDlg::m_bOptionDebug = !lstrcmpi(pszValue,_T("true"));
	} else if(!lstrcmpi(pszOption,_T("simple"))) {
		if(pszValue && *pszValue) {
			CDownloadDlg::m_strOptionSimple = pszValue;
			CDownloadDlg::m_nDialogID = IDD_DOWNLOAD_SIMPLE;
		} else {
			CDownloadDlg::m_strOptionSimple.Empty();
			CDownloadDlg::m_nDialogID = IDD_DOWNLOAD;
		}
	} else if(!lstrcmpi(pszOption,_T("label"))) {
		CDownloadDlg::m_strOptionLabel = pszValue;
	} else if(!lstrcmpi(pszOption,_T("description"))) {
		CDownloadDlg::m_strOptionDescription = pszValue;
	} else if(!lstrcmpi(pszOption,_T("language"))) {
		CDownloadDlg::m_strOptionLanguage = pszValue;
#ifdef USE_RESUME
	} else if(!lstrcmpi(pszOption,_T("resume"))) {
		CDownloadDlg::m_bOptionResume = !lstrcmpi(pszValue,_T("true"));
#endif
	} else if(!lstrcmpi(pszOption,_T("smallwizardimage"))) {
		CDownloadDlg::m_strSmallWizardImage = pszValue;
	} else {
		return false;
	}
	return true;
}

extern "C" LPCTSTR DLLAPI GetFileName(LPCTSTR pszURL) {
	for(int n=0;n<CDownloadDlg::m_files.GetSize();n++) {
		CFileInfo* p = CDownloadDlg::m_files[n];
		
		if(!lstrcmpi(pszURL,p->m_szURL)) {
			LPCTSTR pszPath = p->m_szName[0] ? p->m_szName : p->m_szURL;
			LPCTSTR pszFileName = pszPath;
			
			while(*pszPath) {
				if(*pszPath==_T('/') || *pszPath==_T('\\')) pszFileName = pszPath+1;
				pszPath++;
			}
			
			return pszFileName;
		}
	}
	return NULL;
}

#ifdef _DEBUG
CAppModule _Module;
#endif

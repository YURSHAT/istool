// WTL
#pragma once

#include "CompInt.h"
#include "MyDoc.h"
#include <time.h>

class CTempDir {
public:
	void Create() {
		CString tmp;
		GetTempPath(MAX_PATH,tmp.GetBuffer(MAX_PATH));
		tmp.ReleaseBuffer();

		do {
			DWORD dw = GetTickCount();
			m_strDir.Format("%sist7z%04x.tmp",tmp,dw & 0xFFFF);
		} while(!CreateDirectory(m_strDir,NULL));
	}
	~CTempDir() {
		if(!m_strDir.IsEmpty())
			DeleteDir(m_strDir);
	}

	void GetFile(LPCTSTR pszName,CString& ref) {
		ref.Format("%s\\%s",m_strDir,pszName);
	}

	const CString GetSlashDir() {
		return m_strDir + '\\';
	}

	LPCTSTR GetDir() const {
		return m_strDir;
	}
protected:
	void DeleteDir(LPCTSTR pszDir) {
		CString mask;
		mask.Format("%s\\*.*",pszDir);
		WIN32_FIND_DATA wfd;
		HANDLE hFind = FindFirstFile(mask,&wfd);
		if(hFind!=INVALID_HANDLE_VALUE) {
			do {
				if(!((wfd.cFileName[0]=='.' && wfd.cFileName[1]==0) || (wfd.cFileName[0]=='.' && wfd.cFileName[1]=='.' && wfd.cFileName[2]==0))) {
					CString tmp;
					tmp.Format("%s\\%s",pszDir,wfd.cFileName);
					//AtlMessageBox(NULL,(LPCTSTR)tmp);

					if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						DeleteDir(tmp);
					else
						DeleteFile(tmp);
				}
			} while(FindNextFile(hFind,&wfd));
			FindClose(hFind);
		}
		RemoveDirectory(pszDir);
	}

	CString		m_strDir;
};

/////////////////////////////////////////////////////////////////////////////
// CDlgCompile dialog

class CDlgCompile : public CDialogImpl<CDlgCompile>, public CMyDialogBase<CDlgCompile> {
public:
	enum { IDD = IDD_COMPILE };

	BEGIN_MSG_MAP(CDlgCompile)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_GOTOERROR, OnGotoError)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgCompile>)
	END_MSG_MAP()

	CDlgCompile(CMyDoc* pDoc,const bool bForceRun);
	virtual ~CDlgCompile();

protected:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnGotoError(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	CListBox	m_wndList;
	long		m_nErrorLine;
	CString		m_strIncludeFile;
	FILE*		m_logFile;
	CString		m_strTranslation;
	CProgressBarCtrl	m_wndProgress;

	HINSTANCE					m_hCompiler, m_hCompilerDLS;
	ISDllCompileScriptProcA		m_fCompileScriptA;
	ISDllCompileScriptProcW		m_fCompileScriptW;
	ISDllCompileScriptISPPProcA	m_fCompileScriptISPPA;
	ISDllCompileScriptISPPProcW	m_fCompileScriptISPPW;
	ISDllGetVersionProc			m_fGetVersion;
	CMyDoc*						m_pDoc;
	long						m_nCurrentLine;
	CString						m_strCurrentLine;
	CInnoScript::SECTION		m_sec;
	Henden::CEvent				m_eDone, m_eAbort;
	CString						m_strOutputExeFilename;
	const bool					m_bForceRun;
	CInnoScriptEx				m_script;

public:
	bool LoadCompiler();
	long GetErrorLine();

protected:
	static DWORD WINAPI ThreadEntry(LPVOID lpParameter);
	UINT PreProcess();
	UINT DoCompile();

	static LONG __stdcall CompilerCallbackA(LONG Code,TCompilerCallbackDataA* Data, DWORD AppData);
	UINT CompilerCallbackA(LONG Code,TCompilerCallbackDataA* Data);
	static LONG __stdcall CompilerCallbackW(LONG Code,TCompilerCallbackDataW* Data, DWORD AppData);
	UINT CompilerCallbackW(LONG Code,TCompilerCallbackDataW* Data);

	bool AddDownloadSection();
	void ParseDir(LPCTSTR pszFilter,CAtlTemporaryFile& file,const CString strDestDir,const CString& strRoot);
	void AppendLogFile(LPCTSTR pszFileName);
	void SetFinished();
	int AddListString(LPCTSTR pszString);
	bool RunCompileSteps(CInnoScript::SECTION sec);

	//------- Charset conversion routines ------------------------------------------------------------
	static inline int WINAPI UnicodeLengthOfCustom(long cp,PCSTR pUTF8, int cbUTF8) {
		return MultiByteToWideChar(cp, 0, pUTF8, cbUTF8, NULL, 0);
	}
	static inline int CustomToUnicode(long cp,LPCSTR lpSrcStr,int cchSrc,LPWSTR lpDestStr,int cchDest) {
		return MultiByteToWideChar(cp, 0, lpSrcStr, cchSrc, lpDestStr, cchDest);
	}
	static inline int WINAPI CustomLengthOfUnicode(long cp,LPWSTR lpSrcStr, int cchSrc) {
		return WideCharToMultiByte(cp,0,lpSrcStr,cchSrc,NULL,0,0,NULL);
	}
	static inline int UnicodeToCustom(long cp,LPWSTR lpSrcStr,int cchSrc,BYTE* lpDestStr,int cbDest) {
		return WideCharToMultiByte(cp,0,lpSrcStr,cchSrc,(LPSTR)lpDestStr,cbDest,0,NULL);
	}
	static inline long GetDefaultCharSet() {
		TCHAR szBuf[8];
		long nSize = GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDEFAULTANSICODEPAGE,szBuf,8);
		szBuf[nSize] = 0;
		return _ttol(szBuf);
	}

	bool ConvertCharSet(long from,long to,CString& ref) {
		if(from==to) return true;

		bool ok = false;
		long n = UnicodeLengthOfCustom(from,ref,ref.GetLength());
		WCHAR* pw = new WCHAR[n+1];
		n = CustomToUnicode(from,ref,ref.GetLength(),pw,n+1);
		if(!n) {
			CMyUtils::ShowSysMsg(GetLastError());
			delete []pw;
			return false;
		}
		pw[n] = 0;

		n = CustomLengthOfUnicode(to,pw,n);
		BYTE* pb = new BYTE[n*4];
		n = UnicodeToCustom(to,pw,n,pb,n*4);
		if(!n) {
			CMyUtils::ShowSysMsg(GetLastError());
		} else {
			pb[n] = 0;
			ref = pb;
			ok = true;
		}
		delete []pw;
		delete []pb;
		
		return ok;
	}
};

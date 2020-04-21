// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgUpdates dialog

class CDlgUpdates : 
	public CDialogImpl<CDlgUpdates>, 
	public CWinDataExchange<CDlgUpdates>,
	public CMyDialogBase<CDlgUpdates>
{
public:
	enum { IDD = IDD_UPDATES };

	CDlgUpdates() {
		CString strLibrary;
		if(::GetModuleFileName(_Module.GetModuleInstance(), strLibrary.GetBuffer(_MAX_PATH), _MAX_PATH)) {
			int nPos = strLibrary.ReverseFind('\\');
			if(nPos<0) nPos = strLibrary.ReverseFind('/');
			if(nPos>=0) {
				strLibrary.ReleaseBuffer(nPos+1);
			}
		}
		strLibrary += "isxdl.dll";

		m_hLib = LoadLibrary(strLibrary);
		if(m_hLib) {
			isxdl_Download = (long(__stdcall*)(HWND,LPCTSTR,LPCTSTR))GetProcAddress(m_hLib,"isxdl_Download");
			isxdl_ClearFiles = (void(__stdcall*)())GetProcAddress(m_hLib,"isxdl_ClearFiles");
			isxdl_SetOption = (void(__stdcall*)(LPCTSTR,LPCTSTR))GetProcAddress(m_hLib,"isxdl_SetOption");
		}
	}

	virtual ~CDlgUpdates() {
		if(m_hLib) CloseHandle(m_hLib);
	}

	BEGIN_MSG_MAP(CDlgUpdates)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_UPDATES_WEB_PAGE, OnUpdatesWebPage)
		COMMAND_ID_HANDLER(IDC_DOWNLOAD, OnDownload)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgUpdates>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgUpdates)
		DDX_TEXT(IDC_UPDATES_INFO, m_strInfo)
	END_DDX_MAP()

	CString	m_strInfo, m_strAddress;
	CString m_strFileUpdate;
	HMODULE m_hLib;
	long(__stdcall*isxdl_Download)(HWND,LPCTSTR,LPCTSTR);
	void(__stdcall*isxdl_ClearFiles)();
	void(__stdcall*isxdl_SetOption)(LPCTSTR,LPCTSTR);

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Updates");
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);

		GetDlgItem(IDC_DOWNLOAD).EnableWindow(m_hLib && isxdl_Download && isxdl_ClearFiles && isxdl_SetOption);
		return TRUE;
	}

	LRESULT OnUpdatesWebPage(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CWaitCursor wait;
		ShellExecute(AfxGetMainWnd(),"open",m_strAddress,NULL,NULL,SW_SHOWDEFAULT);
		return 0;
	}

	LRESULT OnDownload(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		ATLASSERT(m_hLib);
		ATLASSERT(isxdl_Download);
		ATLASSERT(isxdl_ClearFiles);

		CString str;
		str.LoadString(WTL_IDS_APP_TITLE);
		isxdl_SetOption("title",str);
		isxdl_SetOption("label",str);
		str.LoadString(IDS_DOWNLOADING_UPDATE);
		isxdl_SetOption("description",str);

		GetTempPath(MAX_PATH,m_strFileUpdate.GetBuffer(MAX_PATH));
		m_strFileUpdate.ReleaseBuffer();
		CMyUtils::EndWith(m_strFileUpdate,'\\');
		m_strFileUpdate += "istoolupdate.exe";

		isxdl_ClearFiles();
		if(isxdl_Download(m_hWnd,"http://www.istool.org/getistool.aspx",m_strFileUpdate)) {
			EndDialog(IDOK);
		} else {
			m_strFileUpdate.Empty();
			AtlMessageBox(m_hWnd,_L("The update was not downloaded."),IDR_MAINFRAME,MB_OK|MB_ICONWARNING);
		}

		return 0;
	}
};

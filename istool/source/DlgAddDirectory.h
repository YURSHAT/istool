// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDirectory dialog

class CDlgAddDirectory : 
	public CDialogImpl<CDlgAddDirectory>, 
	public CWinDataExchange<CDlgAddDirectory>,
	public CMyDialogBase<CDlgAddDirectory>
{
public:
	enum { IDD = IDD_ADD_DIRECTORY };

	BEGIN_MSG_MAP(CDlgAddDirectory)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER(IDC_CHECK_ADD_DIRECTORIES, OnCheckAddDirectories)
		COMMAND_ID_HANDLER(IDC_CHECK_ADD_FILES, OnCheckAddFiles)
		COMMAND_ID_HANDLER(IDC_EXTERNAL, OnExternal)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgAddDirectory>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgAddDirectory)
		DDX_CHECK(IDC_CHECK_ADD_DIRECTORIES,m_bAddDirectories)
		DDX_CHECK(IDC_CHECK_INCLUDE_SUB_DIRECTORIES,m_bIncludeSubDirectories)
		DDX_CHECK(IDC_CHECK_ADD_FILES,m_bAddFiles)
		DDX_TEXT(IDC_ADD_DIRECTORY_WILDCARD, m_strWildCard)
//		DDX_CONTROL(IDC_ADD_DIRECTORY_WILDCARD,m_wndWildCard)
		DDX_CHECK(IDC_EXTERNAL,m_bExternal)
		DDX_TEXT(IDC_ROOT,m_strRoot)
	END_DDX_MAP()

	// CEdit		m_wndWildCard;
	BOOL			m_bAddDirectories;
	BOOL			m_bIncludeSubDirectories;
	BOOL			m_bAddFiles;
	BOOL			m_bExternal;
	CString			m_strWildCard;
	CString			m_strRoot;
	const CString	m_strPath;

	CDlgAddDirectory(LPCTSTR pszPath) : m_strPath(pszPath) {
		m_bAddDirectories = FALSE;
		m_bIncludeSubDirectories = TRUE;
		m_bAddFiles = TRUE;
		m_bExternal = FALSE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		DoDataExchange(DDX_SAVE);
		if(wID==IDOK) {
			CMyUtils::EndWith(m_strRoot,'\\');
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		_L(m_hWnd,"AddDirectory");
		CenterWindow(GetParent());

//		m_wndWildCard.SubclassWindow(GetDlgItem(IDC_ADD_DIRECTORY_WILDCARD));
		DoDataExchange(DDX_LOAD);

		return TRUE;
	}

	LRESULT OnCheckAddDirectories(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		if(!m_bAddDirectories && !m_bAddFiles) {
			m_bAddFiles = TRUE;
			DoDataExchange(DDX_LOAD);
		}
		return TRUE;
	}

	LRESULT OnCheckAddFiles(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		if(!m_bAddFiles && !m_bAddDirectories) {
			m_bAddDirectories = TRUE;
			DoDataExchange(DDX_LOAD);
		}
		//m_wndWildCard.EnableWindow(m_bAddFiles);
		::EnableWindow(GetDlgItem(IDC_ADD_DIRECTORY_WILDCARD),m_bAddFiles);
		return TRUE;
	}

	LRESULT OnExternal(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE,IDC_EXTERNAL);
		GetDlgItem(IDC_ROOT).EnableWindow(m_bExternal);
		if(m_bExternal) {
			DoDataExchange(DDX_SAVE,IDC_ROOT);
			if(m_strRoot.IsEmpty()) {
				m_strRoot = m_strPath;
				DoDataExchange(DDX_LOAD,IDC_ROOT);
			}
		}
		return 0;
	}
};

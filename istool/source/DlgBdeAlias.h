// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgBdeAlias dialog

class CDlgBdeAlias : 
	public CDialogImpl<CDlgBdeAlias>, 
	public CWinDataExchange<CDlgBdeAlias>,
	public CMyDialogBase<CDlgBdeAlias>
{
public:
	enum { IDD = IDD_BDE_ALIAS };

	BEGIN_MSG_MAP(CDlgBdeAlias)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgBdeAlias>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgBdeAlias)
		DDX_CHECK(IDC_BDE_OVERWRITE,m_bOverwrite)
		DDX_TEXT(IDC_BDE_ALIAS_NAME, m_strAliasName)
		DDX_TEXT(IDC_BDE_DATA_DIRECTORY, m_strDataDirectory)
		DDX_TEXT(IDC_BDE_DRIVER_NAME, m_strDriverName)
	END_DDX_MAP()

	CString	m_strAliasName;
	CString	m_strDataDirectory;
	CString	m_strDriverName;
	BOOL			m_bOverwrite;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"BDEAlias");
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}
};

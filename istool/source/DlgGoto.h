// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgGoto dialog

class CDlgGoto : 
	public CDialogImpl<CDlgGoto>, 
	public CWinDataExchange<CDlgGoto>,
	public CMyDialogBase<CDlgGoto>
{
public:
	enum { IDD = IDD_GOTO };

	BEGIN_MSG_MAP(CDlgGoto)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgGoto>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgGoto)
		DDX_TEXT(IDC_LINENUMBER, m_strLineNumber)
	END_DDX_MAP()

	CString	m_strLineNumber;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			DoDataExchange(DDX_SAVE);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Goto");
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}
};

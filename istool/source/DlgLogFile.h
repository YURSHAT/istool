// WTL
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgLogFile dialog

class CDlgLogFile : 
	public CDialogImpl<CDlgLogFile>, 
	public CWinDataExchange<CDlgLogFile>,
	public CMyDialogBase<CDlgLogFile>
{
public:
	enum { IDD = IDD_LOGFILE };

	BEGIN_MSG_MAP(CDlgLogFile)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgLogFile>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgLogFile)
		DDX_TEXT(IDC_LOGFILE, m_strLogFile)
		DDX_CHECK(IDC_LOGFILEAPPEND, m_bLogFileAppend)
	END_DDX_MAP()

	CString		m_strLogFile;
	BOOL		m_bLogFileAppend;
	Henden::CButtonFile	m_wndLogFile;

	CDlgLogFile(CInnoScriptEx& script) : m_script(script), m_wndLogFile(false,"Log Files (*.log)|*.log|All Files (*.*)|*.*|") {
		m_strLogFile = m_script.GetPropertyString("LogFile",CInnoScript::PRJ_ISTOOL);
		m_bLogFileAppend = m_script.GetPropertyBool("LogFileAppend",CInnoScript::PRJ_ISTOOL);
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		if(wID==IDOK) {
			m_script.SetPropertyString("LogFile",m_strLogFile,CInnoScript::PRJ_ISTOOL);
			m_script.SetPropertyBool("LogFileAppend",m_bLogFileAppend ? true : false,CInnoScript::PRJ_ISTOOL);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		m_wndLogFile.SubclassWindow(GetDlgItem(IDC_LOGFILEBROWSE));

		_L(m_hWnd,"LogFile");
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	CInnoScriptEx&	m_script;
};

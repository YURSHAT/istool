// WTL
#pragma once

#include "ButtonFolder.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSysPath dialog

class CDlgSysPath : 
	public CDialogImpl<CDlgSysPath>, 
	public CWinDataExchange<CDlgSysPath>,
	public CMyDialogBase<CDlgSysPath>
{
public:
	enum { IDD = IDD_SYSPATH };

	BEGIN_MSG_MAP(CDlgSysPath)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgSysPath>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgBdeAlias)
	//DDX_Control(IDC_SYSPATH_B, m_wndSysPath);
	DDX_TEXT(IDC_SYSPATH, m_strSysPath);
	DDX_TEXT(IDC_SYSFILE, m_strSysFile);
	DDX_TEXT(IDC_SYSDESTDIR, m_strSysDestDir);
	END_DDX_MAP()

	CButtonFolder	m_wndSysPath;
	CString	m_strSysPath;
	CString	m_strSysFile;
	CString	m_strSysDestDir;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		CTransDialog trans(m_hWnd);
		trans.Translate();
		CenterWindow(GetParent());
		DoDataExchange(DDX_LOAD);
		m_wndSysPath.SubclassWindow(GetDlgItem(IDC_SYSPATH_B));
		return TRUE;
	}

	CDlgSysPath() : m_wndSysPath("Select Source Folder") {}
};

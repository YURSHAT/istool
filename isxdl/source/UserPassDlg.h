// Copyright © 2002 Bjørnar Henden
// UserPassDlg.h : Asks for user name and password for ftp server login.

#pragma once

// ---------------------------------------------------------
#include "resource.h"

// ---------------------------------------------------------
class CUserPassDlg : public CDialogImpl<CUserPassDlg> {
public:
	enum { IDD = IDD_USERPASS };

	BEGIN_MSG_MAP(CUserPassDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	CUserPassDlg(CUserPass* p) : m_pUserPass(p) {}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		CenterWindow(GetParent());

		CString tmp;
		if(CDownload::GetString(IDD_USERPASS,tmp)) SetWindowText(tmp);
		if(CDownload::GetString(168,tmp)) GetDlgItem(IDOK).SetWindowText(tmp);
		if(CDownload::GetString(167,tmp)) GetDlgItem(IDCANCEL).SetWindowText(tmp);
		if(CDownload::GetString(IDC_USERNAME0,tmp)) GetDlgItem(IDC_USERNAME0).SetWindowText(tmp);
		if(CDownload::GetString(IDC_PASSWORD0,tmp)) GetDlgItem(IDC_PASSWORD0).SetWindowText(tmp);
		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			GetDlgItemText(IDC_USERNAME,m_pUserPass->m_strUserName);
			GetDlgItemText(IDC_PASSWORD,m_pUserPass->m_strPassword);
		}
		EndDialog(wID);
		return 0;
	}

	CUserPass*	m_pUserPass;
};


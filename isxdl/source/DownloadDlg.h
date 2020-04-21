// Copyright © 2002 Bjørnar Henden
// DownloadDlg.h : Main download class.

#pragma once

#define WIZ

// ---------------------------------------------------------
#include "resource.h"
#include "Download.h"
#include "UserPassDlg.h"

// ---------------------------------------------------------
class CDownloadDlg : public CDownload, public CDialogImpl<CDownloadDlg> {
public:
	CDownloadDlg(HWND hWndParent);
	virtual ~CDownloadDlg();
	static void CleanupStatic();
	static UINT					m_nDialogID;
	const UINT IDD;

protected:
	CString						m_strTitle;
	HWND						m_hWndParent;
	bool						m_bBusy, m_bThreadDone;
	IPicture*					m_pSmallWizardImage;

	CProgressBarCtrl			m_wndProgressFile, m_wndProgressAll;

	virtual bool IsSimple();

	BEGIN_MSG_MAP(CDownloadDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(UWM_THREADDONE, OnThreadDone)
		MESSAGE_HANDLER(UWM_POSFILE, OnPosFile)
		MESSAGE_HANDLER(UWM_POSALL, OnPosAll)
		MESSAGE_HANDLER(UWM_ERRORDLG, OnErrorDlg)
		MESSAGE_HANDLER(UWM_MSGBOX, OnMsgBox)
		MESSAGE_HANDLER(UWM_TEXT, OnText)
		MESSAGE_HANDLER(UWM_USERPASS, OnUserPass)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
#ifdef WIZ
		if(!IsSimple()) {
			MESSAGE_HANDLER(WM_PAINT, OnPaint)
			MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		}
#endif
	END_MSG_MAP()

#ifdef WIZ
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
#endif
	LRESULT OnThreadDone(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnErrorDlg(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMsgBox(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnText(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnPosFile(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnPosAll(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnUserPass(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
};


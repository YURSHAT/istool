/*==============================================================================

  $Id: MenuFile.cpp,v 1.1 2002/03/21 17:55:38 net Exp $
  
  Implements the File menu in the CMainFrame class

==============================================================================*/

#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

#include "PrefsGeneral.h"
#include "PrefsEditor.h"

LRESULT CMainFrame::OnFilePreferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CString			strTitle = _L("DialogTitles|Preferences","Preferences");
	CString			strPage1 = _L("DialogTitles|General","General");
	CString			strPage3 = _L("DialogTitles|Editor","Editor");

	CMyPropertySheet	sheet((LPCTSTR)strTitle);

	CPrefsGeneral	page1(&m_document,strPage1);
	CPrefsEditor	page3(&m_document,strPage3);

	sheet.AddPage(page1);
	sheet.AddPage(page3);
	sheet.DoModal(m_hWnd);
	return 0;
}

LRESULT CMainFrame::OnFilePrintSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CPrintDialog().DoModal(m_hWnd);
	return 0;
}

LRESULT CMainFrame::OnAppExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	SendMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.OnNewDocument(m_hWnd,false);
	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CString newName;

	if(!m_document.DoPrompt(m_hWnd,newName,true,WTL_IDS_OPENFILE))
		return 0;

	CWaitCursor wait;
	if(m_document.OnOpenDocument(m_hWnd,newName))
		AfxGetMainWnd().AddToRecentFileList(newName);
	return 0;
}

LRESULT CMainFrame::OnFileMerge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CString newName;

	if(!m_document.DoPrompt(m_hWnd,newName,true,IDS_MERGEFILE))
		return 0;

	CWaitCursor wait;
	m_document.OnMergeDocument(m_hWnd,newName);
	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.DoSave(m_hWnd,m_document.GetPathName());
	return 0;
}

LRESULT CMainFrame::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.DoSave(m_hWnd,NULL);
	return 0;
}

LRESULT CMainFrame::OnFileSaveCopyAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	m_document.DoSave(NULL,false);
	return 0;
}

LRESULT CMainFrame::OnFileRecent(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	// get file name from the MRU list
	TCHAR szFile[MAX_PATH];
	if(m_mru.GetFromList(wID, szFile, MAX_PATH)) {
		// open file
		CWaitCursor wait;
		if(m_document.OnOpenDocument(m_hWnd,szFile)) {
			m_mru.MoveToTop(wID);
		} else {
			m_mru.RemoveFromList(wID);
		}
		m_mru.WriteToRegistry(NULL);
	} else {
		::MessageBeep(MB_ICONERROR);
	}
	return 0;
}

LRESULT CMainFrame::OnFilePageSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CPageSetupDialog dlg;
	dlg.m_psd.hDevMode = m_devmode.CopyToHDEVMODE();
	dlg.m_psd.hDevNames = m_printer.CopyToHDEVNAMES();
	dlg.m_psd.rtMargin = m_rcMargin;

	if(dlg.DoModal(m_hWnd) == IDOK) {
		m_devmode.CopyFromHDEVMODE(dlg.m_psd.hDevMode);
		m_printer.ClosePrinter();
		m_printer.OpenPrinter(dlg.m_psd.hDevNames, m_devmode.m_pDevMode);
		m_rcMargin = dlg.m_psd.rtMargin;
	}

	GlobalFree(dlg.m_psd.hDevMode);
	GlobalFree(dlg.m_psd.hDevNames);
	
	return 0;
}

LRESULT CMainFrame::OnFilePrintPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	ATLASSERT(FALSE);
	return 0;
}

LRESULT CMainFrame::OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_PRINT,0,0);
	return 0;
}

LRESULT CMainFrame::OnAccCallTip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_CALLTIP,0,0);
	return 0;
}

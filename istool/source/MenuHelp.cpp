/*==============================================================================

  $Id: MenuHelp.cpp,v 1.1 2002/03/21 17:55:38 net Exp $
  
==============================================================================*/

#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"
#include "StringToken.h"
#include "DlgUpdates.h"
#include "AboutDlg.h"

LRESULT CMainFrame::OnHelpInnoSetupHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CString strHelp(CMyApp::m_prefs.m_strInnoFolder);
	CMyUtils::EndWith(strHelp,'\\');
	strHelp += "ISetup.chm";

	//::WinHelp(AfxGetMainWnd(),strHelp,HELP_FINDER,0);
	::ShellExecute(AfxGetMainWnd(),_T("open"),strHelp,NULL,NULL,SW_SHOWDEFAULT);
	return 0;
}

LRESULT CMainFrame::OnHelpISPP(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CString strHelp(CMyApp::m_prefs.m_strInnoFolder);
	CMyUtils::EndWith(strHelp,'\\');
	strHelp += "ISPP.chm";

	//::WinHelp(AfxGetMainWnd(),strHelp,HELP_FINDER,0);
	::ShellExecute(AfxGetMainWnd(),_T("open"),strHelp,NULL,NULL,SW_SHOWDEFAULT);
	return 0;
}
LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CAboutDlg().DoModal(AfxGetMainWnd());
	return 0;
}

LRESULT CMainFrame::OnHelp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	if(wID==ID_HELP_FINDER || wID==ID_HELP) 
		AfxGetApp()->OpenHtmlHelp(HH_DISPLAY_TOC,0);
	else if(wID==ID_HELP_INDEX)
		AfxGetApp()->OpenHtmlHelp(HH_DISPLAY_INDEX,0);
	else
		AfxGetApp()->OpenHtmlHelp(HH_DISPLAY_SEARCH,0);

	return 0;
}

LRESULT CMainFrame::OnHelpUpdates(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CWaitCursor wait;
	CWebUpdate	update(m_hWnd,"http://www.istool.org/getversion.aspx?id=1");
	CString		strInfo;

	if(update.Check(strInfo)) {
		CStringToken	token(strInfo,"\n");
		CStringToken	ver(token.GetNext(),".");
		CString			strAddress = token.GetNext();

		LPCTSTR psz;
		UINT nCount = 4;
		DWORD dwNewVersion = 0;
		while(nCount-- && (psz = ver.GetNext())) {
			dwNewVersion |= atol(psz) << (nCount*8);
		}

#ifdef VER_BUILD
		const DWORD		dwOldVersion = (VER_MAJOR << 24) | (VER_MINOR << 16) | (VER_REVISION << 8) | VER_BUILD;
#else
		const DWORD		dwOldVersion = (VER_MAJOR << 24) | (VER_MINOR << 16) | (VER_REVISION << 8);
#endif

		if(dwNewVersion>dwOldVersion) {
			CDlgUpdates	dlg;
			dlg.m_strInfo += token.GetRest();
			dlg.m_strAddress = strAddress;
			if(dlg.DoModal(m_hWnd)==IDOK && !dlg.m_strFileUpdate.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("UpdateClose","ISTool will close to start the update."),IDR_MAINFRAME);
				if(!m_document.SaveModified(m_hWnd))
					return 0;
				
				PostMessage(WM_QUIT);
				ShellExecute(NULL,"open",dlg.m_strFileUpdate,NULL,NULL,SW_SHOWDEFAULT);
			}
			return 0;
		}

		AtlMessageBox(m_hWnd,_L("NoNewVersions","There are no new versions available."),IDR_MAINFRAME);
	}
	return 0;
}

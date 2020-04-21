/*==============================================================================

  $Id: MenuView.cpp,v 1.1 2002/03/21 17:55:39 net Exp $

==============================================================================*/

#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

LRESULT CMainFrame::OnViewSection(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	SetSection(wID);
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CMyApp::m_prefs.m_bToolBar ^= 1;
	::SendMessage(m_hWndToolBar, RB_SHOWBAND, IsNewWindows() ? 1 : 0, CMyApp::m_prefs.m_bToolBar); // toolbar is band #1
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CMyApp::m_prefs.m_bStatusBar ^= 1;
	::ShowWindow(m_hWndStatusBar, CMyApp::m_prefs.m_bStatusBar ? SW_SHOWNOACTIVATE : SW_HIDE);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewSectionPanel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	CMyApp::m_prefs.m_bSectionPanel ^= 1;
	m_wndSplitter.SetSinglePaneMode(CMyApp::m_prefs.m_bSectionPanel ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
	return 0;
}

LRESULT CMainFrame::OnViewAsList(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	switch(wID) {
	case ID_VIEW_FILESASLIST:
		CMyApp::m_prefs.m_bFilesList ^= 1;
		SetSection(ID_VIEW_FILES);
		break;
	case ID_VIEW_ICONSASLIST:
		CMyApp::m_prefs.m_bIconsList ^= 1;
		SetSection(ID_VIEW_ICONS);
		break;
	case ID_VIEW_REGISTRYASLIST:
		CMyApp::m_prefs.m_bRegistryList ^= 1;
		SetSection(ID_VIEW_REGISTRY);
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_PROPERTIES,0,0);
	return 0;
}

LRESULT CMainFrame::OnViewCustomize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_CUSTOMIZE,0,0);
	return 0;
}


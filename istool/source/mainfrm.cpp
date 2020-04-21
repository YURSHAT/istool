#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"
#include "SectionView.h"

CMainFrame* CMainFrame::m_pMainWnd;
CMyDoc* CMainFrame::m_pDoc;

LRESULT SendUpdateUI(HWND hWndFocus,UINT uID,LRESULT lDefault) {
	if(hWndFocus) {
		return ::SendMessage(hWndFocus,UWM_UPDATEUI,uID,uID);
	} else
		return lDefault;
}


void CMainFrame::AddToRecentFileList(LPCTSTR pszPath) {
	m_mru.AddToList(pszPath);
	m_mru.WriteToRegistry(NULL);
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	m_pMainWnd = this;
	m_pDoc = &m_document;

	UINT uResID = IDR_MAINFRAME;
	DWORD dwMajor = 0;
	DWORD dwMinor = 0;
	HRESULT hRet = AtlGetCommCtrlVersion(&dwMajor, &dwMinor);
	if(SUCCEEDED(hRet) && dwMajor>=6) uResID = IDB_XPTOOLBAR_SMALL1;

	_L(GetMenu(),"MainMenu");

	HWND hWndCmdBar = NULL;
	if(IsNewWindows()) {
		// create command bar window
		hWndCmdBar = m_wndCmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_wndCmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_wndCmdBar.LoadImages(uResID);
		// remove old menu
		SetMenu(NULL);
	}

	// ToolBar Setup
	m_wndToolBar = CreateSimpleToolBarCtrl(m_hWnd, uResID, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE|TBSTYLE_LIST);
	if(uResID==IDB_XPTOOLBAR_SMALL1) {
		HIMAGELIST hImageHot = ImageList_LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_XPTOOLBAR_SMALL2), 16, 1, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
		m_wndToolBar.SetHotImageList(hImageHot);
	}
	m_wndToolBar.SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	TBBUTTONINFO bi = { sizeof TBBUTTONINFO };

	bi.dwMask = TBIF_TEXT|TBIF_STYLE;
	bi.pszText = "Sections";
	bi.fsStyle = BTNS_BUTTON|BTNS_AUTOSIZE|BTNS_WHOLEDROPDOWN;
	m_wndToolBar.SetButtonInfo(ID_VIEW_SECTIONDROPDOWN,&bi);

	bi.dwMask = TBIF_TEXT|TBIF_STYLE;
	bi.pszText = "Options";
	bi.fsStyle = BTNS_BUTTON|BTNS_AUTOSIZE|BTNS_DROPDOWN;
	m_wndToolBar.SetButtonInfo(ID_PROJECT_OPTIONS,&bi);
	// --

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	if(IsNewWindows())
		AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(m_wndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();
	m_sbar.SubclassWindow(m_hWndStatusBar);
	int arrParts[] = {
		ID_DEFAULT_PANE,
		ID_ROW_PANE,
		ID_COL_PANE
	};
	m_sbar.SetPanes(arrParts, sizeof(arrParts) / sizeof(int), false);
	m_sbar.SetPaneText(ID_ROW_PANE, "Row: ");
	m_sbar.SetPaneText(ID_COL_PANE, "Col: ");

	CreateClient();	// Create splitters and panes

	UIAddToolBar(m_wndToolBar);
	if(!CMyApp::m_prefs.m_bToolBar)
		::SendMessage(m_hWndToolBar, RB_SHOWBAND, IsNewWindows() ? 1 : 0, FALSE);

	if(!CMyApp::m_prefs.m_bSectionPanel)
		m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);


#ifdef _DEBUG
	m_wndToolBarPP = CreateSimpleToolBarCtrl(m_hWnd, IDR_PRINTPREVIEW, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE, ATL_IDW_TOOLBAR + 1);
	AddSimpleReBarBand(m_wndToolBarPP, NULL, TRUE);
	::SendMessage(m_hWndToolBar, RB_SHOWBAND, IsNewWindows() ? 2 : 1, FALSE);	// print preview toolbar is band #2
	UIAddToolBar(m_wndToolBarPP);
#endif

	if(true) {
		Henden::CAppFile appFile(IDR_MAINFRAME);
		m_mru.SetAppFile(appFile);
	}

	if(IsNewWindows()) {
		CMenuHandle menu = m_wndCmdBar.GetMenu();
		CMenuHandle menuFile = menu.GetSubMenu(0);
		m_mru.SetMenuHandle(menuFile);
	} else {
		CMenuHandle menu = GetMenu();
		CMenuHandle menuFile = menu.GetSubMenu(0);
		m_mru.SetMenuHandle(menuFile);
	}
	m_mru.ReadFromRegistry(NULL);
	m_mru.SetMaxEntries(8);


	// File
    UIEnable(ID_FILE_PRINT,FALSE);
	// Edit
	UIEnable(ID_EDIT_UNDO,FALSE);
	UIEnable(ID_EDIT_REDO,FALSE);
	UIEnable(ID_EDIT_CUT,FALSE);
	UIEnable(ID_EDIT_COPY,FALSE);
	UIEnable(ID_EDIT_PASTE,FALSE);
	UIEnable(ID_EDIT_DELETEITEM,FALSE);
	UIEnable(ID_EDIT_SELECT_ALL,FALSE);
	UIEnable(ID_EDIT_MOVEDOWN,FALSE);
	UIEnable(ID_EDIT_MOVEUP,FALSE);
	UIEnable(ID_EDIT_FIND,FALSE);
	UIEnable(ID_EDIT_REPEAT,FALSE);
	UIEnable(ID_EDIT_FIND_PREVIOUS,FALSE);
	UIEnable(ID_EDIT_REPLACE,FALSE);
	UIEnable(ID_EDIT_COMMENTSELECTION,FALSE);
	UIEnable(ID_EDIT_UNCOMMENTSELECTION,FALSE);
	// View
	UIEnable(ID_VIEW_PROPERTIES,FALSE);
	// Popups
	UIEnable(ID_POPUP_CREATEICON,FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	m_document.OnNewDocument(m_hWnd,true);
	SetSection(CMyApp::m_prefs.m_uStartupSection);

	if(__argc>1) {
		for(int n=1;n<__argc;n++) {
			if(!_stricmp(__argv[n],"-langedit")) {
				//CTransDialog::SetLanguageMode();
			} else if(!_stricmp(__argv[n],"-compile")) {
				m_document.m_bCompileAndExit = true;
			} else {
				if(m_document.OnOpenDocument(m_hWnd,__argv[n]))
					AddToRecentFileList(__argv[n]);
				break;
			}
		}
	} else if(CMyApp::m_prefs.m_bOpenLastProject) {
		TCHAR szFile[MAX_PATH];
		CWaitCursor wait;
		if(m_mru.GetFromList(ID_FILE_MRU_FIRST, szFile, MAX_PATH))
			m_document.OnOpenDocument(m_hWnd,szFile);
	}

	PostMessage(WM_USER+101);

	return 0;
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
	LRESULT res = CFrameWindowImpl<CMainFrame>::OnSize(uMsg,wParam,lParam,bHandled);
	if(m_wndSplitter.IsWindow() && (wParam==SIZE_MAXIMIZED || wParam==SIZE_RESTORED)) {
		if(CMyApp::m_prefs.m_nSplitterPos<20)
			CMyApp::m_prefs.m_nSplitterPos = 100;

		if(m_wndSplitter.GetSplitterPos()<20)
			m_wndSplitter.SetSplitterPos(CMyApp::m_prefs.m_nSplitterPos);
	}
	return res;
}

LRESULT CMainFrame::OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(CMyApp::m_prefs.m_nSplitterPos<20)
		CMyApp::m_prefs.m_nSplitterPos = 20;
	m_wndSplitter.SetSplitterPos(CMyApp::m_prefs.m_nSplitterPos);
	
	if(m_document.m_bCompileAndExit) {
		SendMessage(WM_COMMAND,MAKEWPARAM(ID_PROJECT_COMPILE_SCRIPT,0),NULL);
		SendMessage(WM_CLOSE);
	}
	return 0;
}

void CMainFrame::CreateClient() {
	CRect rcVert;
	GetClientRect(&rcVert);

	m_hWndClient = m_wndSplitter.Create(m_hWnd,rcDefault,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_wndSplitter.SetSplitterExtendedStyle(0,SPLIT_PROPORTIONAL);

	m_wndSectionPane.Create(m_wndSplitter);
	m_wndSectionPane.SetTitle("Sections");

	m_wndSectionMain.Create(m_wndSplitter);
	m_wndSectionMain.SetTitle("Main");
	m_wndSectionMain.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON);

	m_wndSplitter.SetSplitterPane(0,m_wndSectionPane);	
	m_wndSplitter.SetSplitterPane(1,m_wndSectionMain);	

	// Test
	m_wndSectionView.Create(m_wndSectionPane);
	m_wndSectionPane.SetClient(m_wndSectionView);

	m_wndSectionView.InsertHeader(_L("SectionStrings|Sections","Sections"));
	m_wndSectionView.InsertItem(_L("SectionStrings|Script","Script"),ID_VIEW_SCRIPT,0);
	m_wndSectionView.InsertItem(_L("SectionStrings|FilesDirs","Files and Dirs"),ID_VIEW_FILES,1);
	m_wndSectionView.InsertItem(_L("SectionStrings|Icons","Icons"),ID_VIEW_ICONS,2);
	m_wndSectionView.InsertItem(_L("SectionStrings|INI","INI"),ID_VIEW_INI,3);
	m_wndSectionView.InsertItem(_L("SectionStrings|Registry","Registry"),ID_VIEW_REGISTRY,4);
	m_wndSectionView.InsertItem(_L("SectionStrings|InstallDelete","Install Delete"),ID_VIEW_INSTALLDELETE,5);
	m_wndSectionView.InsertItem(_L("SectionStrings|InstallRun","Install Run"),ID_VIEW_RUN,6);
	m_wndSectionView.InsertItem(_L("SectionStrings|UninstallDelete","Uninstall Delete"),ID_VIEW_UNINSTALLDELETE,7);
	m_wndSectionView.InsertItem(_L("SectionStrings|UninstallRun","Uninstall Run"),ID_VIEW_UNINSTALLRUN,8);
	m_wndSectionView.InsertItem(_L("SectionStrings|Messages","Messages"),ID_VIEW_MESSAGES,9);
	m_wndSectionView.InsertItem(_L("SectionStrings|CustomMessages","Custom Messages"),ID_VIEW_CUSTOMMESSAGES,9);
	m_wndSectionView.InsertItem(_L("SectionStrings|Types","Types"),ID_VIEW_TYPES,10);
	m_wndSectionView.InsertItem(_L("SectionStrings|Components","Components"),ID_VIEW_COMPONENTS,11);
	m_wndSectionView.InsertItem(_L("SectionStrings|Tasks","Tasks"),ID_VIEW_TASKS,12);
	m_wndSectionView.InsertItem(_L("SectionStrings|Code","Code"),ID_VIEW_CODE,0);
	m_wndSectionView.InsertItem(_L("SectionStrings|Languages","Languages"),ID_VIEW_LANGUAGES,0);

	m_wndSectionView.InsertHeader(_L("SectionStrings|ISTool","ISTool Sections"));
	m_wndSectionView.InsertItem(_L("SectionStrings|Pre","Pre Compilation Steps"),ID_VIEW_PRECOMPILATIONSTEPS,13);
	m_wndSectionView.InsertItem(_L("SectionStrings|Post","Post Compilation Steps"),ID_VIEW_POSTCOMPILATIONSTEPS,13);
	m_wndSectionView.InsertItem(_L("SectionStrings|Download","Download"),ID_VIEW_DOWNLOAD,1);
}

LRESULT CMainFrame::OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/) {
	if(hWndCtl==m_wndSectionPane) {
		m_wndSplitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
		CMyApp::m_prefs.m_bSectionPanel = false;
	}
	return 0;
}

void CMainFrame::SetSection(UINT nID) {
	if(m_wndView.m_hWnd) {
		if(m_wndView.SendMessage(WM_HIDEVIEW)!=0) return;	// Anything other than 0 aborts changeview
		m_wndSectionMain.SetClient(NULL);
		switch(m_uViewIndex) {
		case  0: m_wndScript.DestroyView(); break;
		case  1: m_wndFilesL.DestroyView(); break;
		case  2: m_wndFilesT.DestroyView(); break;
		case  3: m_wndIconsL.DestroyView(); break;
		case  4: m_wndIconsT.DestroyView(); break;
		case  5: m_wndIni.DestroyView(); break;
		case  6: m_wndRegistryL.DestroyView(); break;
		case  7: m_wndRegistryT.DestroyView(); break;
		case  8: m_wndDeleteInstall.DestroyView(); break;
		case  9: m_wndDeleteUninstall.DestroyView(); break;
		case 10: m_wndRunInstall.DestroyView(); break;
		case 11: m_wndRunUninstall.DestroyView(); break;
		case 12: m_wndMessages.DestroyView(); break;
		case 13: m_wndCustomMessages.DestroyView(); break;
		case 14: m_wndTypes.DestroyView(); break;
		case 15: m_wndComponents.DestroyView(); break;
		case 16: m_wndTasks.DestroyView(); break;
		case 17: m_wndCode.DestroyView(); break;
		case 18: m_wndCompileStepPre.DestroyView(); break;
		case 19: m_wndCompileStepPost.DestroyView(); break;
		case 20: m_wndDownload.DestroyView(); break;
		case 21: m_wndLanguages.DestroyView(); break;
		default: ATLASSERT(FALSE); break;
		}

		m_wndView.Detach();
		UISetCheck(m_uCurrentView,FALSE);
		m_uViewIndex = -1;
	}

	DWORD dwFlagsL = WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER;
	DWORD dwFlagsT = WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	DWORD dwFlags2 = WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
	DWORD dwExFlags = WS_EX_CLIENTEDGE;

	CViewItem<CViewFilesL,0>	wndFilesL;
	CViewItem<CViewIconsT,1>	wndIconsT;
	CViewItem<CViewScript,2>	wndScript;

	wndFilesL.CreateView(m_wndSectionMain);

	switch(nID) {
	case ID_VIEW_SCRIPT:
		m_wndView.Attach(m_wndScript.CreateView(m_wndSectionMain));
		m_uViewIndex = 0;
		break;
	case ID_VIEW_FILES:
		if(CMyApp::m_prefs.m_bFilesList) {
			m_wndView.Attach(m_wndFilesL.CreateView(m_wndSectionMain));
			m_uViewIndex = 1;
		} else {
			m_wndView.Attach(m_wndFilesT.CreateView(m_wndSectionMain));
			m_uViewIndex = 2;
		}
		break;
	case ID_VIEW_ICONS:
		if(CMyApp::m_prefs.m_bIconsList) {
			m_wndView.Attach(m_wndIconsL.CreateView(m_wndSectionMain));
			m_uViewIndex = 3;
		} else {
			m_wndView.Attach(m_wndIconsT.CreateView(m_wndSectionMain));
			m_uViewIndex = 4;
		}
		break;
	case ID_VIEW_INI:
		m_wndView.Attach(m_wndIni.CreateView(m_wndSectionMain));
		m_uViewIndex = 5;
		break;
	case ID_VIEW_REGISTRY:
		if(CMyApp::m_prefs.m_bRegistryList) {
			m_wndView.Attach(m_wndRegistryL.CreateView(m_wndSectionMain));
			m_uViewIndex = 6;
		} else {
			m_wndView.Attach(m_wndRegistryT.CreateView(m_wndSectionMain));
			m_uViewIndex = 7;
		}
		break;
	case ID_VIEW_INSTALLDELETE:
		m_wndView.Attach(m_wndDeleteInstall.CreateView(m_wndSectionMain));
		m_uViewIndex = 8;
		break;
	case ID_VIEW_UNINSTALLDELETE:
		m_wndView.Attach(m_wndDeleteUninstall.CreateView(m_wndSectionMain));
		m_uViewIndex = 9;
		break;
	case ID_VIEW_RUN:
		m_wndView.Attach(m_wndRunInstall.CreateView(m_wndSectionMain));
		m_uViewIndex = 10;
		break;
	case ID_VIEW_UNINSTALLRUN:
		m_wndView.Attach(m_wndRunUninstall.CreateView(m_wndSectionMain));
		m_uViewIndex = 11;
		break;
	case ID_VIEW_MESSAGES:
		m_wndView.Attach(m_wndMessages.CreateView(m_wndSectionMain));
		m_uViewIndex = 12;
		break;
	case ID_VIEW_CUSTOMMESSAGES:
		m_wndView.Attach(m_wndCustomMessages.CreateView(m_wndSectionMain));
		m_uViewIndex = 13;
		break;
	case ID_VIEW_TYPES:
		m_wndView.Attach(m_wndTypes.CreateView(m_wndSectionMain));
		m_uViewIndex = 14;
		break;
	case ID_VIEW_COMPONENTS:
		m_wndView.Attach(m_wndComponents.CreateView(m_wndSectionMain));
		m_uViewIndex = 15;
		break;
	case ID_VIEW_TASKS:
		m_wndView.Attach(m_wndTasks.CreateView(m_wndSectionMain));
		m_uViewIndex = 16;
		break;
	case ID_VIEW_CODE:
		m_wndView.Attach(m_wndCode.CreateView(m_wndSectionMain));
		m_uViewIndex = 17;
		break;
	case ID_VIEW_PRECOMPILATIONSTEPS:
		m_wndView.Attach(m_wndCompileStepPre.CreateView(m_wndSectionMain));
		m_uViewIndex = 18;
		break;
	case ID_VIEW_POSTCOMPILATIONSTEPS:
		m_wndView.Attach(m_wndCompileStepPost.CreateView(m_wndSectionMain));
		m_uViewIndex = 19;
		break;
	case ID_VIEW_DOWNLOAD:
		m_wndView.Attach(m_wndDownload.CreateView(m_wndSectionMain));
		m_uViewIndex = 20;
		break;
	case ID_VIEW_LANGUAGES:
		m_wndView.Attach(m_wndLanguages.CreateView(m_wndSectionMain));
		m_uViewIndex = 21;
		break;
	default:
		return;
	}

	m_uCurrentView = nID;
	UISetCheck(m_uCurrentView,TRUE);
//	m_pView->SetFocus();
	CString strTitle;
	strTitle.LoadString(m_uCurrentView);
	int nPos = strTitle.Find('\n');
	if(nPos>=0) strTitle = strTitle.Mid(nPos+1);
	m_wndSectionMain.SetClient(m_wndView);
	m_wndSectionMain.SetTitle(strTitle);
	m_wndSectionView.SelectSection(m_uCurrentView);
	m_wndView.SendMessage(WM_SHOWVIEW);
}

LRESULT CMainFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if(!m_document.SaveModified(m_hWnd))
		return 0;

	// Don't save settings for compile-runs
	if(!m_document.m_bCompileAndExit) {
		// Save window position
		CMyApp* app = AfxGetApp();
		WINDOWPLACEMENT wp = { sizeof WINDOWPLACEMENT };
		if(GetWindowPlacement(&wp)) {
			app->WriteProfileInt(_T("Settings"), _T("FrameStatus"), wp.showCmd);
			app->WriteProfileInt(_T("Settings"), _T("FrameTop"),    wp.rcNormalPosition.top);
			app->WriteProfileInt(_T("Settings"), _T("FrameLeft"),   wp.rcNormalPosition.left);
			app->WriteProfileInt(_T("Settings"), _T("FrameBottom"), wp.rcNormalPosition.bottom);
			app->WriteProfileInt(_T("Settings"), _T("FrameRight"),  wp.rcNormalPosition.right);
		}

		// Don't save splitter position if window is minimized
		if(wp.showCmd!=SW_SHOWMINIMIZED)
			// Save splitter position
			CMyApp::m_prefs.m_nSplitterPos = m_wndSplitter.GetSplitterPos();
	}

	// Cleanup
	SetSection(-1);

	m_document.OnCloseDocument();

	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::OnToolBarDropdown(int idCtrl, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	NMTOOLBAR* pnmtb = (NMTOOLBAR*)pnmh;

	CMenu menu;
	CMenuHandle pPopup;

	// Switch on button command id's.
	switch(pnmtb->iItem) {
	case ID_VIEW_SECTIONDROPDOWN:
		{
			menu.LoadMenu(IDR_POPUP);
			pPopup = menu.GetSubMenu(6);
			_L(pPopup,"Popup");
		}
		break;
	case ID_PROJECT_OPTIONS:
		{
			menu.LoadMenu(IDR_MAINFRAME);
			pPopup = menu.GetSubMenu(3);
			pPopup = pPopup.GetSubMenu(0);
			_L(pPopup,"Popup");
		}
		break;
	default:
		return 0;
	}

	CRect rc;
	m_wndToolBar.GetRect(pnmtb->iItem, rc);
	m_wndToolBar.ClientToScreen(rc);

	TrackPopupMenu(pPopup,TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom);
	return 0;
}

void CMainFrame::TrackPopupMenu(CMenuHandle hPopup,UINT nFlags,int x,int y) {
	if(IsNewWindows())
		m_wndCmdBar.TrackPopupMenu(hPopup,nFlags,x,y);
	else
		hPopup.TrackPopupMenu(nFlags,x,y,m_hWnd);
}

void CMainFrame::UIUpdateAll() {
	CScriptList	list;
	HWND hWndFocus = GetFocusView();

	// File
	UIEnable(ID_FILE_PRINT_PREVIEW,FALSE);
	UIEnable(ID_FILE_PRINT,			SendUpdateUI(hWndFocus,ID_FILE_PRINT,FALSE));
	
	// Edit
	UIEnable(ID_EDIT_NEWITEM,			SendUpdateUI(hWndFocus,ID_EDIT_NEWITEM,FALSE));
	UIEnable(ID_EDIT_MOVEDOWN,			SendUpdateUI(hWndFocus,ID_EDIT_MOVEDOWN,FALSE));
	UIEnable(ID_EDIT_MOVEUP,			SendUpdateUI(hWndFocus,ID_EDIT_MOVEUP,FALSE));
	UIEnable(ID_EDIT_UNDO,				SendUpdateUI(hWndFocus,ID_EDIT_UNDO,FALSE));
	UIEnable(ID_EDIT_REDO,				SendUpdateUI(hWndFocus,ID_EDIT_REDO,FALSE));
	UIEnable(ID_EDIT_CUT,				SendUpdateUI(hWndFocus,ID_EDIT_CUT,FALSE));
	UIEnable(ID_EDIT_COPY,				SendUpdateUI(hWndFocus,ID_EDIT_COPY,FALSE));
	UIEnable(ID_EDIT_PASTE,				SendUpdateUI(hWndFocus,ID_EDIT_PASTE,FALSE));
	UIEnable(ID_EDIT_DELETEITEM,		SendUpdateUI(hWndFocus,ID_EDIT_DELETEITEM,FALSE));
	UIEnable(ID_EDIT_SELECT_ALL,		SendUpdateUI(hWndFocus,ID_EDIT_SELECT_ALL,FALSE));
	UIEnable(ID_EDIT_MOVEDOWN,			SendUpdateUI(hWndFocus,ID_EDIT_MOVEDOWN,FALSE));
	UIEnable(ID_EDIT_MOVEUP,			SendUpdateUI(hWndFocus,ID_EDIT_MOVEUP,FALSE));
	UIEnable(ID_EDIT_FIND,				SendUpdateUI(hWndFocus,ID_EDIT_FIND,FALSE));
	UIEnable(ID_EDIT_REPEAT,			SendUpdateUI(hWndFocus,ID_EDIT_REPEAT,FALSE));
	UIEnable(ID_EDIT_FIND_PREVIOUS, FALSE);
	UIEnable(ID_EDIT_REPLACE,			SendUpdateUI(hWndFocus,ID_EDIT_REPLACE,FALSE));
	UIEnable(ID_EDIT_GOTO,				SendUpdateUI(hWndFocus,ID_EDIT_GOTO,FALSE));
	UIEnable(ID_EDIT_COMMENTSELECTION,	SendUpdateUI(hWndFocus,ID_EDIT_COMMENTSELECTION,FALSE));
	UIEnable(ID_EDIT_UNCOMMENTSELECTION,SendUpdateUI(hWndFocus,ID_EDIT_UNCOMMENTSELECTION,FALSE));
	// View
	UIEnable(ID_VIEW_PROPERTIES,FALSE);
	// Popups
	UIEnable(ID_POPUP_CREATEICON,		SendUpdateUI(hWndFocus,ID_POPUP_CREATEICON,FALSE));


	// View
	UISetCheck(ID_VIEW_STATUS_BAR, CMyApp::m_prefs.m_bStatusBar);
	UISetCheck(ID_VIEW_TOOLBAR, CMyApp::m_prefs.m_bToolBar);
	UISetCheck(ID_VIEW_SECTIONPANEL, CMyApp::m_prefs.m_bSectionPanel);
	UISetCheck(ID_VIEW_FILESASLIST,CMyApp::m_prefs.m_bFilesList);
	UISetCheck(ID_VIEW_ICONSASLIST,CMyApp::m_prefs.m_bIconsList);
	UISetCheck(ID_VIEW_REGISTRYASLIST,CMyApp::m_prefs.m_bRegistryList);
	
	UIEnable(ID_VIEW_CUSTOMIZE,		SendUpdateUI(hWndFocus,ID_VIEW_CUSTOMIZE,FALSE));
	UIEnable(ID_VIEW_PROPERTIES,	SendUpdateUI(hWndFocus,ID_VIEW_PROPERTIES,FALSE));

	// Project
	UIEnable(ID_PROJECT_UNINSTALL,IsInstalled());
	
	UISetCheck(ID_PROJECT_USE_ABSOLUTE_PATHS,m_document.GetUseAbsolutePaths());
	CString strSourceDir;
	UIEnable(ID_PROJECT_USE_ABSOLUTE_PATHS,m_document.GetSourceDir(strSourceDir));

	UIEnable(ID_PROJECT_CREATE_UNINSTALL_ICON,m_document.GetScript().GetPropertyBool("Uninstallable"));

	list.RemoveAll();
	m_document.GetScript().GetList(CInnoScript::SEC_MESSAGES,list);
	UIEnable(ID_PROJECT_EXPORT_MESSAGES,list.GetSize()>0);

	list.RemoveAll();
	m_document.GetScript().GetList(CInnoScript::SEC_REGISTRY,list);
	UIEnable(ID_PROJECT_EXPORT_REGISTRY,list.GetSize()>0);

	// Help
    UIEnable(ID_HELP_INNO_SETUP_HELP,!CMyApp::m_prefs.m_strInnoFolder.IsEmpty());
    UIEnable(ID_HELP_ISPP,!CMyApp::m_prefs.m_strInnoFolder.IsEmpty());

	StatusBarInfo sbi;
	if(hWndFocus) ::SendMessage(hWndFocus,UWM_STATUSBAR,0,(LPARAM)&sbi);
	
	TCHAR szText[32];
	m_sbar.GetPaneText(ID_ROW_PANE,szText);
	if(sbi.m_strRow.CompareNoCase(szText))
		m_sbar.SetPaneText(ID_ROW_PANE,sbi.m_strRow);
	m_sbar.GetPaneText(ID_COL_PANE,szText);
	if(sbi.m_strCol.CompareNoCase(szText))
		m_sbar.SetPaneText(ID_COL_PANE,sbi.m_strCol);

	UIUpdateToolBar();
}

void CMainFrame::OnUpdate(LONG lHint,void* pParam) {
	if(!m_wndView.m_hWnd) return;

	if(!lHint)
		m_wndView.SendMessage(WM_SHOWVIEW);
	else if(lHint==CUpdate::HINT_APPLYCHANGES)
		m_wndView.SendMessage(WM_HIDEVIEW);
}

bool CMainFrame::IsInstalled() {
	CInnoScriptEx& script = m_document.GetScript();

	if(!script.GetPropertyBool("Uninstallable"))
		return false;

	LPCTSTR pszKey = script.GetPropertyString("AppID");
	if(!pszKey || !*pszKey) 
		return false;

	const LPCTSTR pszPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\%s_is1";
	CString strKey;
	strKey.Format(pszPath,pszKey);

	CRegKey reg;
	if(reg.Open(HKEY_CURRENT_USER,strKey,KEY_READ)!=ERROR_SUCCESS)
		if(reg.Open(HKEY_LOCAL_MACHINE,strKey,KEY_READ)!=ERROR_SUCCESS)
			return false;

	return true;
}

LRESULT CMainFrame::OnPopupCreateIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_CREATEICON,0,0);
	return 0;
}


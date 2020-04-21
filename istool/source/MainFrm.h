// MainFrm.h

#pragma once

#include "mydoc.h"
#include "ViewTypes.h"
#include "ViewFilesT.h"
#include "ViewFilesL.h"
#include "ViewIconsT.h"
#include "ViewIconsL.h"
#include "ViewIni.h"
#include "ViewRegistryL.h"
#include "ViewRegistryT.h"
#include "ViewDeleteInstall.h"
#include "ViewDeleteUninstall.h"
#include "ViewRunInstall.h"
#include "ViewRunUninstall.h"
#include "ViewMessages.h"
#include "ViewCustomMessages.h"
#include "ViewComponents.h"
#include "ViewTasks.h"
#include "ViewCompileStepPre.h"
#include "ViewCompileStepPost.h"
#include "ViewScript.h"
#include "ViewLanguages.h"
#include "ViewDownload.h"
#include "SectionView.h"

template<class T,UINT t_uType,long t_flags = 0>
class CViewItem {
public:
	CViewItem<T,t_uType,t_flags>() {
		m_pView = NULL;
	}
	HWND CreateView(HWND hParent) {
		ATLASSERT(m_pView==NULL);
		m_pView = new T;
		DWORD dwFlags = WS_VISIBLE | WS_CHILD;
		switch(t_uType) {
		case 0:
			dwFlags |= LVS_REPORT | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS | LVS_NOSORTHEADER;
			break;
		case 1:
			dwFlags |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
			break;
		case 2:
			dwFlags |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | t_flags;
			break;
		}
		return m_pView->Create(hParent,CWindow::rcDefault,NULL,dwFlags,WS_EX_CLIENTEDGE);
	}
	void DestroyView() {
		ATLASSERT(m_pView!=NULL);
		m_pView->DestroyWindow();
		m_pView = NULL;
	}
	T* GetView() {
		return m_pView;
	}
	BOOL PreTranslateMessage(LPMSG pMsg) {
		if(!m_pView || m_pView->m_hWnd!=::GetFocus()) return FALSE;
		else return m_pView->PreTranslateMessage(pMsg);
	}
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) {
		if(!m_pView) return FALSE;
		ATLASSERT(::IsWindow(m_pView->m_hWnd));
		return m_pView->ProcessWindowMessage(hWnd,uMsg,wParam,lParam,lResult,dwMsgMapID);
	}
protected:
	T*		m_pView;
};



class CMainFrame;

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, 
	public CIdleHandler,
	public CUpdate,
	public CTranslateFrame<CMainFrame>
{
public:
	friend CMainFrame& AfxGetMainWnd();
	friend HWND AfxGetMainHWnd();
	friend CMyDoc* AfxGetDocument();
	friend int AfxMessageBox(LPCTSTR,UINT,UINT);

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	CMainFrame() {
		m_uCurrentView = 0;
		m_uViewIndex = -1;
		m_printer.OpenDefaultPrinter();
		m_devmode.CopyFromPrinter(m_printer);
		m_pFindReplaceDlg = NULL;
		
		TCHAR localeInfo[3];
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, localeInfo, 3);
		if(localeInfo[0] == '0') {	// Metric system. '1' is US System
			m_rcMargin.left = 1500;
			m_rcMargin.top = 1500;
			m_rcMargin.right = 1500;
			m_rcMargin.bottom = 1500;
		} else {
			m_rcMargin.left = 600;
			m_rcMargin.top = 600;
			m_rcMargin.right = 600;
			m_rcMargin.bottom = 600;
		}
	}

	void AddToRecentFileList(LPCTSTR pszPath);

	// Print Information
	CPrinterT<true>		m_printer;
	CDevModeT<true>		m_devmode;
	CPrintPreviewWindow m_prev;
	CRect				m_rcMargin;
	CToolBarCtrl		m_wndToolBarPP;

	CToolBarCtrl			m_wndToolBar;
	CCommandBarCtrl			m_wndCmdBar;
	Henden::CRecentDocumentList	m_mru;
	CMultiPaneStatusBarCtrl m_sbar;
	CSplitterWindow			m_wndSplitter;
	CPaneContainer			m_wndSectionPane, m_wndSectionMain;
	CSectionTree			m_wndSectionView;
	int						m_nItems[CInnoScript::SEC_NONE];
	CMyDoc					m_document;

	UINT								m_uViewIndex;
	CViewItem<CViewScript,2>			m_wndScript;
	CViewItem<CViewFilesL,0>			m_wndFilesL;
	CViewItem<CViewFilesT,1>			m_wndFilesT;
	CViewItem<CViewIconsT,1>			m_wndIconsT;
	CViewItem<CViewIconsL,0>			m_wndIconsL;
	CViewItem<CViewIni,0>				m_wndIni;
	CViewItem<CViewRegistryL,0>			m_wndRegistryL;
	CViewItem<CViewRegistryT,1>			m_wndRegistryT;
	CViewItem<CViewDeleteInstall,0>		m_wndDeleteInstall;
	CViewItem<CViewDeleteUninstall,0>	m_wndDeleteUninstall;
	CViewItem<CViewRunInstall,0>		m_wndRunInstall;
	CViewItem<CViewRunUninstall,0>		m_wndRunUninstall;
	CViewItem<CViewMessages,0>			m_wndMessages;
	CViewItem<CViewCustomMessages,0>	m_wndCustomMessages;
	CViewItem<CViewTypes,0>				m_wndTypes;
	CViewItem<CViewComponents,0>		m_wndComponents;
	CViewItem<CViewTasks,0>				m_wndTasks;
	CViewItem<CViewCompileStepPre,0>	m_wndCompileStepPre;
	CViewItem<CViewCompileStepPost,0>	m_wndCompileStepPost;
	CViewItem<CViewScript,2,LVS_REPORT>	m_wndCode;
	CViewItem<CViewDownload,0>			m_wndDownload;
	CViewItem<CViewLanguages,0>			m_wndLanguages;

	HWND GetFocusView() {
		return m_wndView.m_hWnd;
	}

	CWindow				m_wndView;
	UINT				m_uCurrentView;	// ID of current view

	void SetSection(UINT nID);
	bool IsInstalled();

	virtual BOOL PreTranslateMessage(MSG* pMsg) {
		if(m_pFindReplaceDlg && m_pFindReplaceDlg->IsDialogMessage(pMsg))
			return TRUE;

		return CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg);
	}

	virtual BOOL OnIdle() {
		UIUpdateAll();
		return FALSE;
	}

	void UIUpdateAll();
	void TrackPopupMenu(CMenuHandle hPopup,UINT nFlags,int x,int y);

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		// File
		UPDATE_ELEMENT(ID_FILE_PRINT, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_PRINT_SETUP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_FILE_PRINT_PREVIEW, UPDUI_MENUPOPUP)
		// Edit
        UPDATE_ELEMENT(ID_EDIT_UNDO, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_REDO, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_CUT, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_COPY, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_PASTE, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_DELETEITEM, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_NEWITEM, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_SELECT_ALL, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_MOVEDOWN, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_MOVEUP, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
        UPDATE_ELEMENT(ID_EDIT_FIND, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_REPEAT, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_FIND_PREVIOUS, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_REPLACE, UPDUI_MENUPOPUP)
        UPDATE_ELEMENT(ID_EDIT_GOTO, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_COMMENTSELECTION, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_EDIT_UNCOMMENTSELECTION, UPDUI_MENUPOPUP)
		// View
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_SECTIONPANEL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_FILESASLIST, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ICONSASLIST, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_REGISTRYASLIST, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PROPERTIES, UPDUI_MENUPOPUP | UPDUI_TOOLBAR)
		UPDATE_ELEMENT(ID_VIEW_CUSTOMIZE, UPDUI_MENUPOPUP)
		// View->Sections
		UPDATE_ELEMENT(ID_VIEW_FILES, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_ICONS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_INI, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_REGISTRY, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_INSTALLDELETE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_RUN, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_UNINSTALLDELETE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_UNINSTALLRUN, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_MESSAGES, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TYPES, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_COMPONENTS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TASKS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_CODE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_LANGUAGES, UPDUI_MENUPOPUP)
		// View->ISTool Sections
		UPDATE_ELEMENT(ID_VIEW_SCRIPT, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_PRECOMPILATIONSTEPS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_POSTCOMPILATIONSTEPS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_DOWNLOAD, UPDUI_MENUPOPUP)
		// Project
		UPDATE_ELEMENT(ID_PROJECT_USE_ABSOLUTE_PATHS, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PROJECT_CREATE_UNINSTALL_ICON, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PROJECT_UNINSTALL, UPDUI_MENUPOPUP)
		// Project->Import From File
		UPDATE_ELEMENT(ID_PROJECT_IMPORT_REGISTRY, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PROJECT_IMPORT_INI, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PROJECT_IMPORT_MESSAGES, UPDUI_MENUPOPUP)
		// Project->Export To File
		UPDATE_ELEMENT(ID_PROJECT_EXPORT_REGISTRY, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_PROJECT_EXPORT_MESSAGES, UPDUI_MENUPOPUP)
		// Help
		UPDATE_ELEMENT(ID_HELP_INNO_SETUP_HELP, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_HELP_ISPP, UPDUI_MENUPOPUP)
		// Popups
		UPDATE_ELEMENT(ID_POPUP_CREATEICON, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_USER+101, OnPostCreate)
		MESSAGE_HANDLER(WM_COPYDATA, OnDebuggerCopyData)
		MESSAGE_HANDLER(WM_Debugger_Hello, OnDebuggerHello)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE,OnPaneClose)
		// MenuFile.cpp
		COMMAND_ID_HANDLER(ID_FILE_PREFERENCES,OnFilePreferences)
		COMMAND_ID_HANDLER(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
		COMMAND_ID_HANDLER(ID_APP_EXIT,OnAppExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW,OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_OPEN,OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_MERGE,OnFileMerge)
		COMMAND_ID_HANDLER(ID_FILE_SAVE,OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS,OnFileSaveAs)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_COPY_AS,OnFileSaveCopyAs)
		COMMAND_ID_HANDLER(ID_FILE_PAGE_SETUP, OnFilePageSetup)
		COMMAND_ID_HANDLER(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
		COMMAND_ID_HANDLER(ID_FILE_PRINT,OnFilePrint)
		COMMAND_ID_HANDLER(ID_ACC_CALLTIP,OnAccCallTip)
		//COMMAND_ID_HANDLER(ID_FILE_PRINT,m_wndScript.OnFilePrint)
		COMMAND_RANGE_HANDLER(ID_FILE_MRU_FIRST, ID_FILE_MRU_LAST, OnFileRecent)
		// MenuEdit.h
		COMMAND_ID_HANDLER(ID_EDIT_CUT,OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_COPY,OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE,OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_NEWITEM, OnEditNewItem)
		COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnEditSelectAll)
		COMMAND_ID_HANDLER(ID_EDIT_DELETEITEM, OnEditDelete)
		COMMAND_ID_HANDLER(ID_EDIT_MOVEUP, OnEditMoveUp)
		COMMAND_ID_HANDLER(ID_EDIT_MOVEDOWN, OnEditMoveDown)
		COMMAND_ID_HANDLER(ID_EDIT_FIND, OnEditFind)
		COMMAND_ID_HANDLER(ID_EDIT_REPEAT,OnEditRepeat)
		COMMAND_ID_HANDLER(ID_EDIT_REPLACE, OnEditReplace)
		COMMAND_ID_HANDLER(ID_EDIT_GOTO, OnEditGoto)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_REDO, OnEditRedo)
		COMMAND_ID_HANDLER(ID_EDIT_COMMENTSELECTION, OnEditCommentSelection)
		COMMAND_ID_HANDLER(ID_EDIT_UNCOMMENTSELECTION, OnEditCommentSelection)
		// MenuView.cpp
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_SECTIONPANEL, OnViewSectionPanel)
		COMMAND_ID_HANDLER(ID_VIEW_TYPES,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_FILES,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_ICONS,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_INI,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_REGISTRY,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_INSTALLDELETE,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_UNINSTALLDELETE,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_RUN,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_UNINSTALLRUN,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_MESSAGES,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_CUSTOMMESSAGES,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_COMPONENTS,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_TASKS,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_PRECOMPILATIONSTEPS,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_POSTCOMPILATIONSTEPS,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_SCRIPT,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_CODE,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_LANGUAGES,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_DOWNLOAD,OnViewSection)
		COMMAND_ID_HANDLER(ID_VIEW_FILESASLIST,OnViewAsList)
		COMMAND_ID_HANDLER(ID_VIEW_ICONSASLIST,OnViewAsList)
		COMMAND_ID_HANDLER(ID_VIEW_REGISTRYASLIST,OnViewAsList)
		COMMAND_ID_HANDLER(ID_VIEW_PROPERTIES, OnViewProperties)
		COMMAND_ID_HANDLER(ID_VIEW_CUSTOMIZE, OnViewCustomize)
		// MenuProject.cpp
		COMMAND_RANGE_HANDLER(ID_PROJECT_OPTIONS,ID_PROJECT_OPTIONS_UNINSTALL,OnProjectOptions)
		COMMAND_ID_HANDLER(ID_PROJECT_INSTALL_FONTS,OnProjectInstallFonts)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_DIRECTORY, OnProjectCreateDirectory)
		COMMAND_ID_HANDLER(ID_PROJECT_INSERT_FILES, OnProjectInsertFiles)
		COMMAND_ID_HANDLER(ID_PROJECT_USE_ABSOLUTE_PATHS, OnProjectUseAbsolutePaths)
		COMMAND_ID_HANDLER(ID_PROJECT_LANG_OPTIONS, OnProjectLangOptions)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_ASSOCIATION, OnProjectCreateAssociation)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_ODBC_PROFILE, OnProjectCreateOdbcProfile)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_BDE_ALIAS, OnProjectCreateBdeAlias)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_INTERNET_SHORTCUT, OnProjectCreateInternetShortcut)
		COMMAND_ID_HANDLER(ID_PROJECT_CREATE_UNINSTALL_ICON, OnProjectCreateUninstallIcon)
		COMMAND_ID_HANDLER(ID_PROJECT_VERIFY_FILES, OnProjectVerifyFiles)
		COMMAND_ID_HANDLER(ID_PROJECT_RUN_SETUP, OnProjectRunSetup)
		COMMAND_ID_HANDLER(ID_PROJECT_COMPILE_AND_RUN_SETUP, OnProjectCompileAndRunSetup)
		COMMAND_ID_HANDLER(ID_PROJECT_OPEN_COMPILER, OnProjectOpenCompiler)
		COMMAND_ID_HANDLER(ID_PROJECT_COMPILE_SCRIPT, OnProjectCompileScript)
		COMMAND_ID_HANDLER(ID_PROJECT_EXPORT_REGISTRY, OnProjectExportRegistry)
		COMMAND_ID_HANDLER(ID_PROJECT_EXPORT_MESSAGES, OnProjectExportMessages)
		COMMAND_ID_HANDLER(ID_PROJECT_IMPORT_REGISTRY, OnProjectImportRegistry)
		COMMAND_ID_HANDLER(ID_PROJECT_IMPORT_INI, OnProjectImportIni)
		COMMAND_ID_HANDLER(ID_PROJECT_IMPORT_MESSAGES, OnProjectImportMessages)
		COMMAND_ID_HANDLER(ID_PROJECT_LOGFILE, OnProjectLogFile)
		COMMAND_ID_HANDLER(ID_PROJECT_UNINSTALL, OnProjectUninstall)
		// MenuHelp
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_HELP, OnHelp)
		COMMAND_ID_HANDLER(ID_HELP_FINDER, OnHelp)
		COMMAND_ID_HANDLER(ID_HELP_INDEX, OnHelp)
		COMMAND_ID_HANDLER(ID_HELP_SEARCH, OnHelp)
		COMMAND_ID_HANDLER(ID_HELP_INNO_SETUP_HELP, OnHelpInnoSetupHelp)
		COMMAND_ID_HANDLER(ID_HELP_ISPP, OnHelpISPP)
		COMMAND_ID_HANDLER(ID_HELP_UPDATES, OnHelpUpdates)
		//
		COMMAND_ID_HANDLER(ID_POPUP_CREATEICON, OnPopupCreateIcon)
		NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolBarDropdown)
		//
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CTranslateFrame<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPostCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnPaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMenuSelect(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	// Debugger
	LRESULT OnDebuggerCopyData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDebuggerHello(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	// MenuFile.cpp
	LRESULT OnFilePreferences(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePrintSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileMerge(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSaveCopyAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePageSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePrintPreview(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileRecent(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAccCallTip(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	// MenuEdit.h
	LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditNewItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditMoveUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditMoveDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditFind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditReplace(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditRepeat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditGoto(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditRedo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditCommentSelection(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	// MenuView.cpp
	LRESULT OnViewSection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewSectionPanel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewAsList(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewProperties(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewCustomize(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	// MenuProject.cpp
	LRESULT OnProjectOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectInstallFonts(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateDirectory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectInsertFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectUseAbsolutePaths(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectLangOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateAssociation(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateOdbcProfile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateBdeAlias(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateInternetShortcut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCreateUninstallIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectVerifyFiles(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCompileAndRunSetup(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectOpenCompiler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectCompileScript(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectExportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectExportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectImportRegistry(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectImportIni(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectImportMessages(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectLogFile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnProjectUninstall(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHelpInnoSetupHelp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHelpISPP(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnHelpUpdates(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//
	LRESULT OnToolBarDropdown(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnPopupCreateIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
protected:
	void CreateClient();

	static CMainFrame*	m_pMainWnd;
	static CMyDoc*		m_pDoc;

public:
	void OnUpdate(LONG lHint,void* pParam);
	CFindReplaceDialog*	m_pFindReplaceDlg;
};

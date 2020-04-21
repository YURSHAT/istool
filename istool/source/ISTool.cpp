// ISTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ISTool.h"

#include "MainFrm.h"
#include "MyDoc.h"
#include "Registry.h"

CMyApp theApp;

CImageList	CMyApp::m_imageList;
int			CMyApp::m_nExitCode;

CUpdate* CUpdate::m_pFirst;

CMyApp* AfxGetApp() {
	return &theApp;
}

CAppModule _Module;

void Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT) {
	if(!theApp.InitInstance()) return;

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	CRect rc = CRect(0,0,800,560);
	bool bCenterIt = false;
#if 1
	if(true) {
		// Load window settings
		CMyApp* app = AfxGetApp();
		int s, t, b, r, l;

		// only restore if there is a previously saved position
		if ( -1 != (s = app->GetProfileInt(_T("Settings"),_T("FrameStatus"),   -1)) &&
			-1 != (t = app->GetProfileInt(_T("Settings"),_T("FrameTop"),      -1)) &&
			-1 != (l = app->GetProfileInt(_T("Settings"),_T("FrameLeft"),     -1)) &&
			-1 != (b = app->GetProfileInt(_T("Settings"),_T("FrameBottom"),   -1)) &&
			-1 != (r = app->GetProfileInt(_T("Settings"),_T("FrameRight"),    -1))
		) {
			// restore the window's status
			nCmdShow = s;

			// restore the window's width and height
			int cx = r - l;
			int cy = b - t;

			// the following correction is needed when the taskbar is
			// at the left or top and it is not "auto-hidden"
			RECT workArea;
			SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
			l += workArea.left;
			t += workArea.top;

			// make sure the window is not completely out of sight
			int max_x = GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICON);
			int max_y = GetSystemMetrics(SM_CYSCREEN) - GetSystemMetrics(SM_CYICON);
			rc.left = min(l, max_x);
			rc.top = min(t, max_y);
			rc.right = rc.left + cx;
			rc.bottom = rc.top + cy;
		} else
			bCenterIt = true;
	}
#endif

	if(wndMain.CreateEx(NULL,rc) == NULL)
	{
		ATLTRACE(_L("Main window creation failed!\n"));
		return;
	}

	if(bCenterIt) wndMain.CenterWindow();
	wndMain.ShowWindow(nCmdShow);

	theLoop.Run();

	_Module.RemoveMessageLoop();
	theApp.ExitInstance();
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
//	HRESULT hRes = ::CoInitialize(NULL);
	HRESULT hRes = ::OleInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	//::CoUninitialize();
	::OleUninitialize();

	return CMyApp::m_nExitCode;
}

/////////////////////////////////////////////////////////////////////////////
// CMyApp construction

const LPCTSTR CMyApp::m_pszKeyIS = "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Inno Setup 5_is1";

CMyApp::CMyApp() : m_mutex(FALSE,"ISTool") {}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMyApp object

CMyPrefs CMyApp::m_prefs("Prefs");

CMyPrefs::CMyPrefs(LPCTSTR pszSubKey) : m_strSubKey(pszSubKey) {}

bool CMyPrefs::SavePrefs() {
	CMyApp& app = *AfxGetApp();

	app.WriteProfileString(m_strSubKey,"FontName",m_strFontName);
	app.WriteProfileInt(m_strSubKey,"FontHeight",m_nFontHeight);
	app.WriteProfileInt(m_strSubKey,"TabStopValue",m_nTabStopValue);
	app.WriteProfileInt(m_strSubKey,"ShowVerticalTabLines",m_bShowVerticalTabLines ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"AutoIndent",m_bAutoIndent ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"ShowLineNumbers",m_bShowLineNumbers ? 1 : 0);

	app.WriteProfileInt(m_strSubKey,"ReplaceCopy",m_bReplaceCopy ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"AutoComponentSelect",m_bAutoComponentSelect ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"TestCompiledSetup",m_bTestCompiledSetup ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"FilesAsList",m_bFilesList ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"IconsAsList",m_bIconsList ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"RegistryAsList",m_bRegistryList ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"IgnoreDefaults",m_bIgnoreDefaults ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"OverwriteMessages",m_bOverwriteMessages ? 1 : 0);
	app.WriteProfileString(m_strSubKey,"InnoFolder",m_strInnoFolder);
	app.WriteProfileString(m_strSubKey,"ScriptFolder",m_strScriptFolder);
	app.WriteProfileInt(m_strSubKey,"ToolBar",m_bToolBar ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"StatusBar",m_bStatusBar ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"SectionPanel",m_bSectionPanel ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"OpenLastProject",m_bOpenLastProject ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"ShowNewWizard",m_bShowNewWizard ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"StartupSection",m_uStartupSection);
	app.WriteProfileString(m_strSubKey,"LanguageFile",m_strLanguageFile);
	app.WriteProfileInt(m_strSubKey,"SplitterPos",m_nSplitterPos);
	app.WriteProfileInt(m_strSubKey,"PreProcess",m_bPreProcess ? 1 : 0);
	app.WriteProfileInt(m_strSubKey,"NoOutputExeFilename",m_bNoOutputExeFilename ? 1 : 0);

	app.WriteProfileInt(m_strSubKey,"LanguageDirCount",m_languageDirs.GetCount());
	for(UINT i=0;i<m_languageDirs.GetCount();i++) {
		CString tmp;
		tmp.Format("LanguageDir%02X",i);
		app.WriteProfileString(m_strSubKey,tmp,m_languageDirs[i]);
	}

	CScintillaPrefs::SavePrefs(app);
	return true;
}

bool CMyPrefs::LoadPrefs() {
	CMyApp& app = *AfxGetApp();

	m_strFontName			= app.GetProfileString(m_strSubKey,"FontName","Courier New");
	m_nFontHeight			= app.GetProfileInt(m_strSubKey,"FontHeight",10);
	m_nTabStopValue			= app.GetProfileInt(m_strSubKey,"TabStopValue",4);
	m_bShowVerticalTabLines	= app.GetProfileInt(m_strSubKey,"ShowVerticalTabLines",1)!=0;
	m_bAutoIndent			= app.GetProfileInt(m_strSubKey,"AutoIndent",1)!=0;
	m_bShowLineNumbers		= app.GetProfileInt(m_strSubKey,"ShowLineNumbers",1)!=0;

	m_bReplaceCopy			= app.GetProfileInt(m_strSubKey,"ReplaceCopy",1) ? true : false;
	m_bAutoComponentSelect	= app.GetProfileInt(m_strSubKey,"AutoComponentSelect",0) ? true : false;
	m_bTestCompiledSetup	= app.GetProfileInt(m_strSubKey,"TestCompiledSetup",1) ? true : false;
	m_bFilesList			= app.GetProfileInt(m_strSubKey,"FilesAsList",1) ? true : false;
	m_bIconsList			= app.GetProfileInt(m_strSubKey,"IconsAsList",1) ? true : false;
	m_bRegistryList			= app.GetProfileInt(m_strSubKey,"RegistryAsList",1) ? true : false;
	m_bIgnoreDefaults		= app.GetProfileInt(m_strSubKey,"IgnoreDefaults",0) ? true : false;
	m_bOverwriteMessages	= app.GetProfileInt(m_strSubKey,"OverwriteMessages",0) ? true : false;
	m_strInnoFolder			= app.GetProfileString(m_strSubKey,"InnoFolder");
	m_strScriptFolder		= app.GetProfileString(m_strSubKey,"ScriptFolder");
	m_bToolBar				= app.GetProfileInt(m_strSubKey,"ToolBar",1) ? true : false;
	m_bStatusBar			= app.GetProfileInt(m_strSubKey,"StatusBar",1) ? true : false;
	m_bSectionPanel			= app.GetProfileInt(m_strSubKey,"SectionPanel",1) ? true : false;
	m_bOpenLastProject		= app.GetProfileInt(m_strSubKey,"OpenLastProject",0) ? true : false;
	m_bShowNewWizard		= app.GetProfileInt(m_strSubKey,"ShowNewWizard",1) ? true : false;
	m_uStartupSection		= app.GetProfileInt(m_strSubKey,"StartupSection",ID_VIEW_SCRIPT);
	m_strLanguageFile		= app.GetProfileString(m_strSubKey,"LanguageFile");
	m_nSplitterPos			= app.GetProfileInt(m_strSubKey,"SplitterPos",164);
	m_bPreProcess			= app.GetProfileInt(m_strSubKey,"PreProcess",0) ? true : false;
	m_bNoOutputExeFilename	= app.GetProfileInt(m_strSubKey,"NoOutputExeFilename",1) ? true : false;

	UINT iCount 			= app.GetProfileInt(m_strSubKey,"LanguageDirCount",0);
	while(iCount--) {
		CString tmp;
		tmp.Format("LanguageDir%02X",iCount);
		CString str = app.GetProfileString(m_strSubKey,tmp,"");
		if(!str.IsEmpty())
			m_languageDirs.InsertAt(0,str);
	}

	if(m_strInnoFolder.IsEmpty()) {
		CRegKey	rk;

		LONG lRet = rk.Create(HKEY_LOCAL_MACHINE, CMyApp::m_pszKeyIS);
		if(lRet!=ERROR_SUCCESS) rk.Create(HKEY_CURRENT_USER, CMyApp::m_pszKeyIS);

		if(lRet==ERROR_SUCCESS) {
			ULONG nChars = MAX_PATH;
			rk.QueryStringValue("Inno Setup: App Path",m_strInnoFolder.GetBuffer(nChars),&nChars);
		}
	}

	if(::GetModuleFileName(_Module.GetModuleInstance(), m_strAppDir.GetBuffer(_MAX_PATH), _MAX_PATH)) {
		int nPos = m_strAppDir.ReverseFind('\\');
		if(nPos>=0) m_strAppDir.ReleaseBuffer(nPos);
	}

	if(m_languageDirs.GetCount()==0 && !m_strInnoFolder.IsEmpty()) {
		m_languageDirs.Add(m_strInnoFolder);
		m_languageDirs.Add(m_strInnoFolder + _T("\\Languages"));
	}

	CScintillaPrefs::LoadPrefs(app);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CMyApp initialization

BOOL CMyApp::InitInstance() {
	m_prefs.LoadPrefs();

	// Locate help files
#ifdef NDEBUG
	SetHtmlHelpFile("ISTool.chm");
	m_strCallTipsFile = "calltips.txt";
	CString strHelp;
	if(::GetModuleFileName(_Module.GetModuleInstance(), strHelp.GetBuffer(_MAX_PATH), _MAX_PATH)) {
		int nPos = strHelp.ReverseFind('\\');
		if(nPos<0) nPos = strHelp.ReverseFind('/');
		if(nPos>=0) {
			strHelp.ReleaseBuffer(nPos+1);
			SetHtmlHelpFile(strHelp + "ISTool.chm");
			m_strCallTipsFile = strHelp + "calltips.txt";
			m_strProgramPath = strHelp;
			//CTransDialog::SetIndexFile(strHelp + "ISTool.idx");
		}
	}
	CTranslate::AddFile(CMyApp::m_prefs.m_strLanguageFile);
#else
	SetHtmlHelpFile("U:\\ISTool\\help\\html\\ISTool.chm");
	m_strCallTipsFile = "U:\\istool\\calltips.txt";
	//CTransDialog::SetIndexFile("U:\\ISTool\\distribution\\ISTool.idx");
	//CTransDialog::SetLanguageFile("F:\\Utvk\\ISTool\\distribution\\German.lng");
	CTranslate::AddFile(CMyApp::m_prefs.m_strLanguageFile);
#endif

	m_imageList.Create(16,16,ILC_MASK|ILC_COLOR16,8,1);
	CBitmap bm;
	bm.LoadBitmap(IDB_IMAGELIST);
	m_imageList.Add(bm,RGB(255,0,255));
	bm.Detach();

	// Make sure our registry key is there
	CRegistryEx reg;
	if(!reg.VerifyKey(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool"))
		reg.CreateKey(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool");
	if(!reg.VerifyKey(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool\\command"))
		reg.CreateKey(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool\\command");

	reg.Open(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool");
	if(!reg.VerifyValue("")) reg.Write("",_L("Open with &ISTool"));
	reg.Close();

	reg.Open(HKEY_CLASSES_ROOT,"InnoSetupScriptFile\\shell\\OpenWithISTool\\command");
	if(!reg.VerifyValue("")) {
		CString str;
		str.Format("\"%s\" \"%%1\"",__argv[0]);
		if(reg.Write("",str))
			SHChangeNotify(SHCNE_ASSOCCHANGED, 0, NULL, NULL);
	}
	reg.Close();

	if(CMyUtils::IsAdminLoggedOn()) {
		/*
		** Check if Inno Setup 2 is installed, and the user
		** if he wants to open Inno Setup's web page.
		*/
		CRegistryEx	reg;
		if(
			!reg.Open(HKEY_LOCAL_MACHINE,m_pszKeyIS) &&
			!reg.Open(HKEY_CURRENT_USER,m_pszKeyIS)) 
		{
			CString txt = _L("NeedIS5","You don't seem to have Inno Setup 5 installed. This is\nrequired to compile the scripts you create with ISTool.\n\nDo you want to go to http://www.innosetup.com/ and download it now?");
			if(AtlMessageBox(AfxGetMainHWnd(),(LPCTSTR)txt,IDR_MAINFRAME,MB_YESNO|MB_ICONQUESTION)==IDYES) {
				CWaitCursor wait;
				ShellExecute(AfxGetMainHWnd(),"open","http://www.innosetup.com/",NULL,NULL,SW_SHOWDEFAULT);
			}
		}
	}

	if(FILE* fp = fopen(m_strCallTipsFile,"rb")) {
		CString strLine;
		long nSection = -1;
		while(fgets(strLine.GetBuffer(1000),1000,fp)) {
			strLine.ReleaseBuffer();
			strLine.Trim();
			if(strLine.IsEmpty() || strLine[0]==';') continue;

			if(!strLine.CompareNoCase("[functions]"))
				nSection = 1;
			else if(!strLine.CompareNoCase("[constants]"))
				nSection = 2;
			else if(!strLine.CompareNoCase("[calltips]"))
				nSection = 3;
			else if(nSection>0) {
				CString strName, strDescription;

				long pos = strLine.Find('=');
				if(pos>0) {
					strName = strLine.Left(pos).Trim();
					strDescription = strLine.Mid(pos+1).Trim();
				} else
					strName = strLine;

				CallTipInfo* p = new CallTipInfo;
				p->m_strName = strName;
				p->m_strDescription = strDescription;
				switch(nSection) {
					case 1:
						m_functions.Add(p);
						break;
					case 2:
						m_constants.Add(p);
						break;
					case 3:
						m_calltips.Add(p);
						break;
				}
			}
		}
		fclose(fp);
	}

	return TRUE;
}

CSimpleArray<CallTipInfo*>	CMyApp::m_functions;
CSimpleArray<CallTipInfo*>	CMyApp::m_constants;
CSimpleArray<CallTipInfo*>	CMyApp::m_calltips;

void CMyApp::ExitInstance() {
	for(long i=0;i<m_functions.GetSize();i++) delete m_functions[i];
	for(long i=0;i<m_constants.GetSize();i++) delete m_constants[i];
	for(long i=0;i<m_calltips.GetSize();i++) delete m_calltips[i];
	m_prefs.SavePrefs();
}

void CMyApp::OpenHtmlHelp(UINT nCmd,DWORD dwData) {
	static const struct {
		UINT	m_nID;
		LPCTSTR	m_pszTopic;
	} m_topics[] = {
		IDD_CUSTOMIZE,			"/customizevisiblecolumns.html",
		IDD_PREFS_GENERAL,		"/generalpreferences.html",
		IDD_PREFS_EDITOR,		"/editorpreferenes.html",
		IDD_COMMON_LANGUAGES,	"/common_languages.html"
	};

	if(nCmd==HH_DISPLAY_TOPIC) {
		for(int n=0;n<sizeof(m_topics)/sizeof(m_topics[0]);n++) {
			if(m_topics[n].m_nID == (dwData & 0x7FFF)) {
				CString strTmp;
				strTmp.Format("%s::%s",GetHtmlHelpFile(),m_topics[n].m_pszTopic);
				::HtmlHelp(AfxGetMainWnd(), strTmp, HH_DISPLAY_TOPIC, NULL);
				return;
			}
		}
		// No context found, open contents
		::HtmlHelp(AfxGetMainWnd(), GetHtmlHelpFile(), HH_DISPLAY_TOC, NULL);
		return;
	} else if(nCmd==HH_DISPLAY_SEARCH) {
		HH_FTS_QUERY q = { sizeof HH_FTS_QUERY };
		::HtmlHelp(AfxGetMainWnd(), AfxGetApp()->GetHtmlHelpFile(), nCmd, (DWORD)&q);
	} else {
		::HtmlHelp(AfxGetMainWnd(), GetHtmlHelpFile(), nCmd, NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMyApp message handlers

int myFind(const CString& ref,TCHAR ch,int startPos/*=0*/) {
	int pos = startPos;
	int len = ref.GetLength();

	int nInConstant = 0;
	
	while(pos<len) {
		if(ref[pos]=='{') {
			nInConstant++;
		} else if(ref[pos]=='}') {
			nInConstant--;
		} else if(!nInConstant && ref[pos]==ch) {
			return pos;
		}
		pos++;
	}
	return -1;
}

int myReverseFind(const CString& ref,TCHAR ch) {
	int pos = myFind(ref,ch);
	if(pos<0) return pos;
	int result;
	do {
		result = pos;
		pos = myFind(ref,ch,pos+1);
	} while(pos>=0);
	return result;
}

HTREEITEM CMyApp::FindParentItem(CTreeViewCtrl& ctrl,LPCTSTR lpszFolder,bool bSystem/*=false*/) {
	CString str(lpszFolder);
	CString sub;

	HTREEITEM hRoot = TVI_ROOT;

	do {
		int pos = myFind(str,'\\');
		if(pos>=0) {
			sub = str.Left(pos);
			str = str.Mid(pos+1);
		} else {
			sub = str;
			str.Empty();
		}
		if(sub.IsEmpty()) break;

		HTREEITEM hChild = ctrl.GetChildItem(hRoot);
		while(hChild) {
			CString txt;
			ctrl.GetItemText(hChild,txt);
			if(!txt.CompareNoCase(sub)) break;
			//if(txt==sub) break;
			hChild = ctrl.GetNextSiblingItem(hChild);
		}

		if(!hChild) {
			HTREEITEM hSave = hRoot;
			if(bSystem)
				hRoot = ctrl.InsertItem(sub,3,3,hRoot,TVI_SORT);
			else {
				if(!CInnoScriptEx::IsFolderConstant(sub))
					hRoot = ctrl.InsertItem(sub,8,9,hRoot,TVI_SORT);
				else
					hRoot = ctrl.InsertItem(sub,12,13,hRoot,TVI_SORT);
			}
#if 1 // Don't expand everything
			HTREEITEM hParent = ctrl.GetParentItem(hRoot);
			if(hParent) {
				CScriptLine* pParent = (CScriptLine*)ctrl.GetItemData(hParent);
				if(!pParent || pParent->m_dwUserFlags & 1)
					ctrl.Expand(hParent,TVE_EXPAND);
			}
#else
			ctrl.Expand(hSave,TVE_EXPAND);
#endif
		} else
			hRoot = hChild;
	} while(!sub.IsEmpty());
	return hRoot;
}

void CMyApp::MyExpand(CTreeViewCtrl& ctrl,HTREEITEM hItem) {
#if 1
	if(!hItem) return;

	if(hItem==TVI_ROOT) {
		ctrl.Expand(hItem,TVE_EXPAND);
	} else {
		CScriptLine* pLine = (CScriptLine*)ctrl.GetItemData(hItem);
		if(!pLine || pLine->m_dwUserFlags & 1)
			ctrl.Expand(hItem,TVE_EXPAND);
	}
#else
	ctrl.Expand(hItem,TVE_EXPAND);
#endif
}

DWORD CMyApp::MyExec(LPCTSTR pszFilename,LPCTSTR pszParams,LPCTSTR pszWorkingDir/*=NULL*/,
				   bool bWaitUntilTerminated/*=true*/,
				   bool bRunMinimized/*=false*/,
				   bool bWaitForIdle/*=false*/) 
{
	CString				strCmdLine;
	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;
	DWORD				dwResult = -1;

#if NDEBUG
	strCmdLine.Format("\"%s\" %s",pszFilename,pszParams ? pszParams : "");
#else
	strCmdLine.Format("\"%s\" %s /DEBUGWND=%d",pszFilename,pszParams ? pszParams : "",AfxGetMainHWnd());
#endif

	memset(&si,0,sizeof si);
	si.cb = sizeof si;
	if(bRunMinimized) {
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWMINIMIZED;
	}

	if(!CreateProcess(NULL,strCmdLine.GetBuffer(MAX_PATH),NULL,NULL,FALSE,0,NULL,pszWorkingDir,&si,&pi)) {
		return dwResult;
	}

	CloseHandle(pi.hThread);
	if(bWaitForIdle) WaitForInputIdle(pi.hProcess,INFINITE);
	if(bWaitUntilTerminated) {
#if 1
		WaitForSingleObject(pi.hProcess,INFINITE);
#else
		do {
			Sleep(0);
			//if Assigned(ProcessMessagesProc) then
			//	ProcessMessagesProc;
		} while(MsgWaitForMultipleObjects(1, &pi.hProcess, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0+1);
#endif
		GetExitCodeProcess(pi.hProcess,&dwResult);
	}


    CloseHandle(pi.hProcess);
	return dwResult;
}

bool CMyApp::IsBooleanExp(LPCTSTR pszArg) {
	CString str;
	str.Format(" %s ",pszArg);
	str.MakeLower();
	if(str.FindOneOf("()")>=0)
		return true;

	if(str.Find(" or ")>=0)
		return true;

	if(str.Find(" and ")>=0)
		return true;

	return false;
}


CMainFrame& AfxGetMainWnd() {
	ATLASSERT(CMainFrame::m_pMainWnd);
	return *CMainFrame::m_pMainWnd;
}

HWND AfxGetMainHWnd() {
	if(CMainFrame::m_pMainWnd)
		return CMainFrame::m_pMainWnd->m_hWnd;
	else
		return ::GetActiveWindow();
}

CMyDoc* AfxGetDocument() {
	return CMainFrame::m_pDoc;
}

void AfxGetFileTitle(LPCTSTR pszPathName, LPSTR pszBuffer, UINT nLength) {
	CString str(pszPathName);
	int nPos1 = str.ReverseFind('\\');
	int nPos2 = str.ReverseFind('/');
	if(nPos2>nPos1) str = str.Mid(nPos2+1);
	else str = str.Mid(nPos1+1);
	nPos1 = str.ReverseFind('.');
	if(nPos1>=0)
		str.ReleaseBuffer(nPos1);
	_tcscpy(pszBuffer,str);
}


// PrefsGeneral.h

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPrefsGeneral dialog

#include "Registry.h"
#include "kFontCombo.h"

static const LPCTSTR pszAssPath = "InnoSetupScriptFile\\shell\\open\\command";
static const LPCTSTR pszISToolPath = "InnoSetupScriptFile\\shell\\OpenWithISTool\\command";
static const LPCTSTR pszISetupPath = "InnoSetupScriptFile\\shell\\OpenWithInnoSetup\\command";

class CPrefsGeneral : 
	public CPropertyPageImpl<CPrefsGeneral>,
	public CMyPropertyPageBase<CPrefsGeneral>,
	public CWinDataExchange<CPrefsGeneral>,
	public Henden::CToolTipDialog<CPrefsGeneral>
{
public:
	enum { IDD = IDD_PREFS_GENERAL };

	BEGIN_MSG_MAP(CPrefsGeneral)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPrefsGeneral>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPrefsGeneral>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPrefsGeneral>)
		COMMAND_HANDLER(IDC_ASSOCIATE_ISTOOL, BN_CLICKED, OnAssociateISTool)
		COMMAND_HANDLER(IDC_PREFS1_COMPILERDIR, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_IGNOREDEFAULTS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_OPENLAST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_USENEWSCRIPTWIZARD, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_REPLACECOPY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_AUTOCOMPONENTSELECT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GENERAL_TEST_COMPILED_SETUP, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_PREFS1_LANGUAGEDLL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_STARTUPSECTION, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_PREPROCESS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_NOOUTPUTEXEFILENAME, BN_CLICKED, OnModified)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"PrefsGeneral");

		struct {
			LPCTSTR		pszName;
			UINT		nID;
		} sections[] = {
			"Script",			ID_VIEW_SCRIPT,
			"Files and Dirs",	ID_VIEW_FILES,
			NULL
		};

		CComboBox wnd(GetDlgItem(IDC_STARTUPSECTION));
		UINT nSec = 0;
		while(sections[nSec].pszName) {
			int iItem = wnd.AddString(_L(sections[nSec].pszName));
			wnd.SetItemData(iItem,sections[nSec].nID);
			if(CMyApp::m_prefs.m_uStartupSection==sections[nSec].nID)
				//wnd.SetCurSel(iItem);
				m_nStartupSection = iItem;
			nSec++;
		}
		wnd.Detach();

		GetDlgItem(IDC_ASSOCIATE_ISTOOL).EnableWindow(!IsAssociated());

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	BEGIN_DDX_MAP(CPrefsGeneral)
		DDX_TEXT(IDC_PREFS1_COMPILERDIR, m_strCompilerDir)
		DDX_CHECK(IDC_GENERAL_IGNOREDEFAULTS, m_bIgnoreDefaults)
		DDX_CHECK(IDC_GENERAL_OPENLAST, m_bOpenLast)
		DDX_CHECK(IDC_GENERAL_USENEWSCRIPTWIZARD, m_bUseNewScriptWizard)
		DDX_CHECK(IDC_GENERAL_REPLACECOPY, m_bReplaceCopy)
		DDX_CHECK(IDC_GENERAL_AUTOCOMPONENTSELECT, m_bAutoComponentSelect)
		DDX_CHECK(IDC_GENERAL_TEST_COMPILED_SETUP, m_bTestCompiledSetup)
		DDX_CHECK(IDC_GENERAL_OVERWRITE_MESSAGES, m_bOverwriteMessages)
		DDX_TEXT(IDC_PREFS1_LANGUAGEDLL, m_strLanguageFile)
		DDX_CONTROL(IDC_PREFS1_COMPILERDIR_B, m_btnCompilerDir)
		DDX_CONTROL(IDC_PREFS1_LANGUAGEDLL_BTN, m_btnLanguageDLL)
		DDX_CBINDEX(IDC_STARTUPSECTION, m_nStartupSection)
		DDX_CHECK(IDC_PREPROCESS, m_bPreProcess)
		DDX_CHECK(IDC_NOOUTPUTEXEFILENAME, m_bNoOutputExeFilename)
	END_DDX_MAP()
	
	Henden::CButtonFolder	m_btnCompilerDir;
	Henden::CButtonFile		m_btnLanguageDLL;

	CString	m_strCompilerDir;
	BOOL			m_bIgnoreDefaults;
	BOOL			m_bOpenLast;
	BOOL			m_bUseNewScriptWizard;
	BOOL			m_bReplaceCopy;
	BOOL			m_bAutoComponentSelect;
	BOOL			m_bTestCompiledSetup;
	BOOL			m_bOverwriteMessages;
	CString	m_strLanguageFile;
	int				m_nStartupSection;
	BOOL			m_bPreProcess;
	BOOL			m_bNoOutputExeFilename;

	CPrefsGeneral(CMyDoc* pDoc,LPCTSTR pszTitle) : 
		m_pDoc(pDoc),
		m_btnCompilerDir("Select compiler directory"),
		m_btnLanguageDLL(true,"Language Files (*.lng)|*.lng|All Files (*.*)|*.*|"), 
		CPropertyPageImpl<CPrefsGeneral>(pszTitle)
	{
		//{{AFX_DATA_INIT(CPrefsGeneral)
		m_bIgnoreDefaults = FALSE;
		m_bUseNewScriptWizard = CMyApp::m_prefs.m_bShowNewWizard;
		m_bOverwriteMessages = FALSE;
		//}}AFX_DATA_INIT

		m_strCompilerDir = CMyApp::m_prefs.m_strInnoFolder;
		m_bIgnoreDefaults = CMyApp::m_prefs.m_bIgnoreDefaults;

		m_bOpenLast = CMyApp::m_prefs.m_bOpenLastProject;

		m_bReplaceCopy = CMyApp::m_prefs.m_bReplaceCopy;
		m_bAutoComponentSelect = CMyApp::m_prefs.m_bAutoComponentSelect;
		m_bTestCompiledSetup = CMyApp::m_prefs.m_bTestCompiledSetup;

		m_bOverwriteMessages = CMyApp::m_prefs.m_bOverwriteMessages;
		m_nStartupSection = -1;
		m_strLanguageFile = CMyApp::m_prefs.m_strLanguageFile;
		m_bPreProcess = CMyApp::m_prefs.m_bPreProcess;
		m_bNoOutputExeFilename	= CMyApp::m_prefs.m_bNoOutputExeFilename;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CMyApp::m_prefs.m_strInnoFolder = m_strCompilerDir;
		CMyApp::m_prefs.m_bIgnoreDefaults = m_bIgnoreDefaults ? true : false;

		CMyApp::m_prefs.m_bOpenLastProject = m_bOpenLast ? true : false;
		CMyApp::m_prefs.m_bShowNewWizard = m_bUseNewScriptWizard ? true : false;

		CMyApp::m_prefs.m_bReplaceCopy = m_bReplaceCopy ? true : false;
		CMyApp::m_prefs.m_bAutoComponentSelect = m_bAutoComponentSelect ? true : false;
		CMyApp::m_prefs.m_bTestCompiledSetup = m_bTestCompiledSetup ? true : false;
		CMyApp::m_prefs.m_bOverwriteMessages = m_bOverwriteMessages ? true : false;

		if(m_nStartupSection>=0) {
			CComboBox wnd(GetDlgItem(IDC_STARTUPSECTION));
			CMyApp::m_prefs.m_uStartupSection = wnd.GetItemData(m_nStartupSection);
			wnd.Detach();
		}

		CMyApp::m_prefs.m_strLanguageFile = m_strLanguageFile;
		CMyApp::m_prefs.m_bPreProcess = m_bPreProcess ? true : false;
		CMyApp::m_prefs.m_bNoOutputExeFilename = m_bNoOutputExeFilename ? true : false;
		CTranslate::AddFile(m_strLanguageFile);

		return PSNRET_NOERROR;
	}

	bool IsAssociated() {
		// Check if associate check box should be checked or not
		CRegistryEx reg;
		CString strPath;
		if(!reg.Open(HKEY_CLASSES_ROOT,pszAssPath)) return false;
		if(!reg.Read("",strPath)) return false;
		reg.Close();

		if(!strPath.IsEmpty()) {
			LPCTSTR pszPath = strPath;
			while(*pszPath) {
				if(!_tcsnicmp(pszPath,"ISTool",6)) return true;
				pszPath++;
			}
		}
		return false;
	}

	LRESULT OnAssociateISTool(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		CString err;
		CRegistryEx reg;
		CString strData;
		LPCTSTR pszPath;
		bool bError = false;

		CWaitCursor wait;

		DoDataExchange(DDX_SAVE);
		//if(m_bAssociateISTool)
			pszPath = pszISToolPath;
		//else
		//	pszPath = pszISetupPath;

		if(!bError && !reg.Open(HKEY_CLASSES_ROOT,pszPath)) {
			CMyUtils::GetSysError(err,reg.m_Info.lMessage);
			AtlMessageBox(m_hWnd,(LPCTSTR)err,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			bError = true;
		}
		if(!bError && !reg.Read("",strData)) {
			CMyUtils::GetSysError(err,reg.m_Info.lMessage);
			AtlMessageBox(m_hWnd,(LPCTSTR)err,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
			bError = true;
		}
		reg.Close();

		if(!bError && !strData.IsEmpty()) {
			if(!bError && !reg.CreateKey(HKEY_CLASSES_ROOT,pszAssPath)) {
				CMyUtils::GetSysError(err,reg.m_Info.lMessage);
				AtlMessageBox(m_hWnd,(LPCTSTR)err,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				bError = true;
			}
			if(!bError && !reg.Write("",strData)) {
				CMyUtils::GetSysError(err,reg.m_Info.lMessage);
				AtlMessageBox(m_hWnd,(LPCTSTR)err,IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				bError = true;
			}
			reg.Close();

			if(!bError) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, NULL, NULL);
		} else if(!bError) {
			AtlMessageBox(m_hWnd,_L("AssocFailed","Failed to associate ISTool with .iss files"),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		}

		GetDlgItem(IDC_ASSOCIATE_ISTOOL).EnableWindow(!IsAssociated());

		return 0;
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_PREFS1_COMPILERDIR, _L("Help|General|CompilerDir","The directory where Inno Setup is installed."))
		TOOLTIP_HANDLER(IDC_GENERAL_IGNOREDEFAULTS, _L("Help|General|IgnoreDefaults","Setup directives that equals the default value aren't written to the script."))
		TOOLTIP_HANDLER(IDC_GENERAL_OPENLAST, _L("Help|General|OpenLast","Opens last opened script when ISTool starts."))
		TOOLTIP_HANDLER(IDC_GENERAL_USENEWSCRIPTWIZARD, _L("Help|General|ScriptWiz","Should ISTool use the Inno Setup wizard when creating new scripts."))
		TOOLTIP_HANDLER(IDC_GENERAL_REPLACECOPY, _L("Help|General|ReplaceCopy","When you type \"(c)\" it will be replaced with ©"))
		TOOLTIP_HANDLER(IDC_GENERAL_AUTOCOMPONENTSELECT, _L("Help|General|AutoCompSelect","Automatically sets component for a file based on directory or something!?!"))
		TOOLTIP_HANDLER(IDC_GENERAL_TEST_COMPILED_SETUP, _L("Help|General|TestCompiledSetup","After compilation ISTool asks if you want to run the setup."))
		TOOLTIP_HANDLER(IDC_GENERAL_OVERWRITE_MESSAGES, _L("Help|General|OverwriteMessages","Deletes existing messages when importing a message file."))
		TOOLTIP_HANDLER(IDC_PREFS1_LANGUAGEDLL, _L("Help|General|LanguageFile","The file used to translate ISTool."))
		TOOLTIP_HANDLER(IDC_PREFS1_COMPILERDIR_B, _L("Help|General|CompilerDirButton","Click this button to select the directory where Inno Setup is installed."))
		TOOLTIP_HANDLER(IDC_PREFS1_LANGUAGEDLL_BTN, _L("Help|General|LanguageFileButton","Click this button to select a language file."))
		TOOLTIP_HANDLER(IDC_STARTUPSECTION, _L("Help|General|StartupSection","Tells what section is active when ISTool opens."))
		TOOLTIP_HANDLER(IDC_PREPROCESS, _L("Help|General|PreProcess","If this is checked ISTool will pre-process the script before sending it to Inno Setup for compilation."))
		TOOLTIP_HANDLER(IDC_NOOUTPUTEXEFILENAME, _L("Help|General|NoOutputExeFileName","If this is checked, the name of the compiled setup will not be written to the script."))
		TOOLTIP_HANDLER(IDC_ASSOCIATE_ISTOOL, _L("Help|General|Associate","Click this button to associate ISTool with Inno Setup scripts."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*	m_pDoc;
};

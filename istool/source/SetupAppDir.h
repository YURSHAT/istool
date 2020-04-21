// Setup3AppDir.h

#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupAppDir dialog

class CSetupAppDir : 
	public CPropertyPageImpl<CSetupAppDir>,
	public CMyPropertyPageBase<CSetupAppDir>,
	public CWinDataExchange<CSetupAppDir>,
	public Henden::CToolTipDialog<CSetupAppDir>
{
public:
	enum { IDD = IDD_SETUP_APPDIR };

	BEGIN_MSG_MAP(CSetupAppDir)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupAppDir>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupAppDir>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupAppDir>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CREATEAPPDIR, BN_CLICKED, OnCreateAppDir)
		COMMAND_HANDLER(IDC_DISABLEDIRPAGE, BN_CLICKED, OnDisableDirPage)
		COMMAND_HANDLER(IDC_ENABLEDIRDOESNTEXISTWARNING, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTDIRNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ALLOWROOTDIRECTORY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_USEPREVIOUSAPPDIR, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DIREXISTSWARNING, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_ALLOWUNCPATH, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPENDDEFAULTDIRNAME, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupAppDir)
		DDX_CHECK(IDC_CREATEAPPDIR, m_bCreateAppDir)
		DDX_TEXT(IDC_DEFAULTDIRNAME, m_strDefaultDirName)
		DDX_CHECK(IDC_DISABLEDIRPAGE, m_bDisableDirPage)
		DDX_CHECK(IDC_ENABLEDIRDOESNTEXISTWARNING, m_bEnableDirDoesntExistWarning)
		DDX_CHECK(IDC_ALLOWROOTDIRECTORY, m_bAllowRootDirectory)
		DDX_CHECK(IDC_USEPREVIOUSAPPDIR, m_bUsePreviousAppDir)
		DDX_CBINDEX(IDC_DIREXISTSWARNING, m_nDirExistsWarning)
		DDX_CHECK(IDC_ALLOWUNCPATH, m_bAllowUNCPath)
		DDX_CHECK(IDC_APPENDDEFAULTDIRNAME, m_bAppendDefaultDirName)
	END_DDX_MAP()

	CComboBox	m_wndDirExistsWarning;
	CEdit2		m_wndDefaultDirName;
	BOOL		m_bCreateAppDir;
	CString		m_strDefaultDirName;
	BOOL		m_bDisableDirPage;
	BOOL		m_bEnableDirDoesntExistWarning;
	BOOL		m_bAllowRootDirectory;
	BOOL		m_bUsePreviousAppDir;
	int			m_nDirExistsWarning;
	BOOL		m_bAllowUNCPath;
	BOOL		m_bAppendDefaultDirName;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"AppDir");
		m_wndDirExistsWarning.Attach(GetDlgItem(IDC_DIREXISTSWARNING));
		m_wndDefaultDirName.SubclassWindow(GetDlgItem(IDC_DEFAULTDIRNAME));

		m_wndDirExistsWarning.AddString(_L("AppDir|Auto","Auto"));
		m_wndDirExistsWarning.AddString(_L("AppDir|Yes","Yes"));
		m_wndDirExistsWarning.AddString(_L("AppDir|No","No"));

		EnableControls();
		
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		script.SetPropertyBool("CreateAppDir",m_bCreateAppDir ? true : false);
		script.SetPropertyBool("DisableDirPage",m_bDisableDirPage ? true : false);
		script.SetPropertyString("DefaultDirName",m_strDefaultDirName);
		script.SetPropertyBool("EnableDirDoesntExistWarning",m_bEnableDirDoesntExistWarning ? true : false);
		script.SetPropertyBool("AllowRootDirectory",m_bAllowRootDirectory ? true : false);
		script.SetPropertyBool("UsePreviousAppDir",m_bUsePreviousAppDir ? true : false);
		script.SetPropertyString("DirExistsWarning",script.GetDirExistsWarning(m_nDirExistsWarning));
		script.SetPropertyBool("AllowUNCPath",m_bAllowUNCPath ? true : false);
		script.SetPropertyBool("AppendDefaultDirName",m_bAppendDefaultDirName ? true : false);
		
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnCreateAppDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_CREATEAPPDIR);
		EnableControls();
		return 0;
	}

	LRESULT OnDisableDirPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_DISABLEDIRPAGE);
		EnableControls();
		return 0;
	}

	void EnableControls() {
		m_wndDirExistsWarning.EnableWindow(m_bCreateAppDir && !m_bDisableDirPage);
		m_wndDefaultDirName.EnableWindow(m_bCreateAppDir);
		GetDlgItem(IDC_DISABLEDIRPAGE).EnableWindow(m_bCreateAppDir);
		GetDlgItem(IDC_ALLOWROOTDIRECTORY).EnableWindow(m_bCreateAppDir && !m_bDisableDirPage);
		GetDlgItem(IDC_USEPREVIOUSAPPDIR).EnableWindow(m_bCreateAppDir);
		GetDlgItem(IDC_ENABLEDIRDOESNTEXISTWARNING).EnableWindow(m_bCreateAppDir && !m_bDisableDirPage);
		GetDlgItem(IDC_ALLOWUNCPATH).EnableWindow(m_bCreateAppDir && !m_bDisableDirPage);
		GetDlgItem(IDC_STATIC1).EnableWindow(m_bCreateAppDir);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bCreateAppDir && !m_bDisableDirPage);
		GetDlgItem(IDC_APPENDDEFAULTDIRNAME).EnableWindow(m_bCreateAppDir);
	}

	CSetupAppDir(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew), 
		m_wndDefaultDirName(pDoc),
		CPropertyPageImpl<CSetupAppDir>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bCreateAppDir					= script.GetPropertyBool("CreateAppDir");
		m_bDisableDirPage				= script.GetPropertyBool("DisableDirPage");
		m_strDefaultDirName				= script.GetPropertyString("DefaultDirName");
		m_bEnableDirDoesntExistWarning	= script.GetPropertyBool("EnableDirDoesntExistWarning");
		m_bAllowRootDirectory			= script.GetPropertyBool("AllowRootDirectory");
		m_bUsePreviousAppDir			= script.GetPropertyBool("UsePreviousAppDir");
		m_nDirExistsWarning				= script.GetDirExistsWarning(script.GetPropertyString("DirExistsWarning"));
		m_bAllowUNCPath					= script.GetPropertyBool("AllowUNCPath");
		m_bAppendDefaultDirName			= script.GetPropertyBool("AppendDefaultDirName");
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_CREATEAPPDIR, _L("Help|AppDir|CreateAppDir","If this is set to no, no directory for the application will be created, the Select Destination Location wizard page will not be displayed, and the {app} directory constant is equivalent to the {win} directory constant. If the uninstall feature is enabled when CreateAppDir is no, the uninstall data files are created in the system's Windows directory."))
		TOOLTIP_HANDLER(IDC_DEFAULTDIRNAME, _L("Help|AppDir|DefaultDirName","This value of this required directive is used for the default directory name, which is used in the Select Destination Location page of the wizard. Normally it is prefixed by a directory constant."))
		TOOLTIP_HANDLER(IDC_DISABLEDIRPAGE, _L("Help|AppDir|DisableDirPage","If this is set to yes, Setup will not show the Select Destination Location wizard page. In this case, it will always use the default directory name."))
		TOOLTIP_HANDLER(IDC_ENABLEDIRDOESNTEXISTWARNING, _L("Help|AppDir|EnableDirDoesntExistWarnnig","When set to yes, Setup will display a message box if the directory the user selects doesn't exist. Usually you will also set DirExistsWarning=no when this is yes."))
		TOOLTIP_HANDLER(IDC_ALLOWROOTDIRECTORY, _L("Help|AppDir|AllowRootDirectory","When set to no, the default, the user will not be allowed to enter a root directory (such as \"C:\\\") on the Select Destination Location page of the wizard."))
		TOOLTIP_HANDLER(IDC_USEPREVIOUSAPPDIR, _L("Help|AppDir|UserPreviousAppDir","When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the directory of the previous installation as the default directory presented to the user in the wizard."))
		TOOLTIP_HANDLER(IDC_DIREXISTSWARNING, _L("Help|AppDir|DirExistsWarning","When set to auto, the default setting, Setup will show a \"The directory ... already exists. Would you like to install to that directory anyway?\" message if the user selects a directory that already exists on the Select Destination Location wizard page, except when another version of the same application is already installed and the selected directory is the same as the previous one (only if UsePreviousAppDir is yes, the default setting).\r\n\r\nWhen set to yes, Setup will always display the \"Directory Exists\" message when the user selects an existing directory.\r\n\r\nWhen set to no, Setup will never display the \"Directory Exists\" message."))
		TOOLTIP_HANDLER(IDC_ALLOWUNCPATH, _L("Help|AppDir|AllowUNCPath","If set to no, the user will not be allowed to enter a UNC path (such as \"\\\\server\\share\") on the Select Destination Location page of the wizard."))
		TOOLTIP_HANDLER(IDC_APPENDDEFAULTDIRNAME, _L("Help|AppDir|AppendDefaultDirName","By default, when a folder in the dialog displayed by the Browse... button on the Select Destination Location wizard page is clicked, Setup automatically appends the last component of DefaultDirName onto the new path. For example, if DefaultDirName is {pf}\\My Program and \"Z:\\\" is clicked, the new path will become \"Z:\\My Program\".\r\n\r\nSetting this directive to no disables the aforementioned behavior. In addition, it causes a Make New Folder button to appear on the dialog."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

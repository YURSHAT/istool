// SetupUninstall.h

#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"
#include "IconComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupUninstall dialog

class CSetupUninstall : 
	public CPropertyPageImpl<CSetupUninstall>,
	public CMyPropertyPageBase<CSetupUninstall>,
	public CWinDataExchange<CSetupUninstall>,
	public Henden::CToolTipDialog<CSetupUninstall>
{
public:
	enum { IDD = IDD_SETUP_UNINSTALL };

	BEGIN_MSG_MAP(CSetupUninstall)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupUninstall>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupUninstall>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupUninstall>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_UNINSTALLABLE, BN_CLICKED, OnUninstallable)
		COMMAND_HANDLER(IDC_CREATEUNINSTALLREGKEY, BN_CLICKED, OnCreateUninstallRegkey)
		COMMAND_HANDLER(IDC_UNINSTALLDISPLAYICON, CBN_EDITCHANGE, OnChangeUninstalldisplayicon)
		COMMAND_HANDLER(IDC_UNINSTALLDISPLAYICON, CBN_KILLFOCUS, OnKillFocusUninstallDisplayIcon)
		COMMAND_HANDLER(IDC_UNINSTALLDISPLAYICON, CBN_DROPDOWN, OnDropDownUninstallDisplayIcon)
		COMMAND_HANDLER(IDC_UNINSTALLLOGMODE, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPID, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPPUBLISHER, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPPUBLISHERURL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPSUPPORTURL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPUPDATESURL, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPSUPPORTPHONE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APPVERSION, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSTALLFILESDIR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSTALLFILESDIR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSTALLDISPLAYICONINDEX, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_UNINSTALLDISPLAYNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_UPDATEUNINSTALLLOGAPPNAME, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_CHECK_UNINSTALLRESTARTCOMPUTER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_GUIDGEN, BN_CLICKED, OnGuidGen)
		COMMAND_HANDLER(IDC_APPMODIFYPATH, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPCOMMENTS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPCONTACT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPREADMEFILE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SIGNEDUNINSTALLER, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SIGNEDUNINSTALLERDIR, EN_CHANGE, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupUninstall)
		DDX_CHECK(IDC_UNINSTALLABLE, m_bUninstallable)
		DDX_CHECK(IDC_CREATEUNINSTALLREGKEY, m_bCreateUninstallRegKey)
		DDX_CBINDEX(IDC_UNINSTALLLOGMODE, m_nUninstallLogMode)
		DDX_TEXT(IDC_APPPUBLISHER, m_strAppPublisher)
		DDX_TEXT(IDC_APPPUBLISHERURL, m_strAppPublisherURL)
		DDX_TEXT(IDC_APPSUPPORTURL, m_strAppSupportURL)
		DDX_TEXT(IDC_APPUPDATESURL, m_strAppUpdatesURL)
		DDX_TEXT(IDC_APPVERSION, m_strAppVersion)
		DDX_TEXT_LEN(IDC_APPID, m_strAppID, 127)
//		DDX_CBSTRING(IDC_UNINSTALLFILESDIR, m_strUninstallFilesDir)
//		DDX_CBSTRING(IDC_UNINSTALLDISPLAYICON, m_strUninstallDisplayIcon)
		DDX_TEXT(IDC_UNINSTALLFILESDIR, m_strUninstallFilesDir)
		DDX_TEXT(IDC_UNINSTALLDISPLAYICON, m_strUninstallDisplayIcon)
		DDX_CBINDEX(IDC_UNINSTALLDISPLAYICONINDEX, m_nUninstallDisplayIconIndex)
		DDX_TEXT(IDC_UNINSTALLDISPLAYNAME, m_strUninstallDisplayName)
		DDX_CHECK(IDC_UPDATEUNINSTALLLOGAPPNAME, m_bUpdateUninstallLogAppName)
		DDX_CHECK(IDC_CHECK_UNINSTALLRESTARTCOMPUTER, m_bUninstallRestartComputer)
		DDX_TEXT(IDC_APPMODIFYPATH,m_strAppModifyPath)
		DDX_TEXT(IDC_APPCOMMENTS,m_strAppComments)
		DDX_TEXT(IDC_APPCONTACT,m_strAppContact)
		DDX_TEXT(IDC_APPREADMEFILE,m_strAppReadmeFile)
		DDX_TEXT(IDC_APPSUPPORTPHONE,m_strAppSupportPhone)
		DDX_CHECK(IDC_SIGNEDUNINSTALLER, m_bSignedUninstaller)
		DDX_TEXT(IDC_SIGNEDUNINSTALLERDIR, m_strSignedUninstallerDir)
		DDX_CONTROL(IDC_SIGNEDUNINSTALLERDIR2, m_btnSignedUninstallerDir)
	END_DDX_MAP()

	CLargeIconComboBox	m_wndUninstallDisplayIconIndex;
	CComboBox	m_wndUninstallDisplayIcon;
	CComboBox2	m_wndUninstallFilesDir;
	CEdit	m_wndAppID;
	CButton	m_wndCreateUninstallRegKey;
	CComboBox	m_wndUninstallLogMode;
	BOOL	m_bUninstallable;
	BOOL	m_bCreateUninstallRegKey;
	int		m_nUninstallLogMode;
	CString	m_strAppPublisher;
	CString	m_strAppPublisherURL;
	CString	m_strAppSupportURL;
	CString	m_strAppUpdatesURL;
	CString	m_strAppVersion;
	CString	m_strAppID;
	CString	m_strUninstallFilesDir;
	CString	m_strUninstallDisplayIcon;
	int		m_nUninstallDisplayIconIndex;
	CString	m_strUninstallDisplayName;
	BOOL	m_bUpdateUninstallLogAppName, m_bSignedUninstaller;
	CString	m_strAppModifyPath, m_strAppComments, m_strAppContact, m_strAppReadmeFile, m_strAppSupportPhone;
	CString	m_strSignedUninstallerDir;
	Henden::CButtonFolder	m_btnSignedUninstallerDir;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Uninstall");
		m_wndUninstallDisplayIconIndex.SubclassWindow(GetDlgItem(IDC_UNINSTALLDISPLAYICONINDEX));
		m_wndUninstallDisplayIcon.Attach(GetDlgItem(IDC_UNINSTALLDISPLAYICON));
		m_wndUninstallFilesDir.SubclassWindow(GetDlgItem(IDC_UNINSTALLFILESDIR));
		m_wndAppID.Attach(GetDlgItem(IDC_APPID));
		m_wndCreateUninstallRegKey.Attach(GetDlgItem(IDC_CREATEUNINSTALLREGKEY));
		m_wndUninstallLogMode.Attach(GetDlgItem(IDC_UNINSTALLLOGMODE));

		m_wndUninstallLogMode.AddString(_L("Uninstall|LogMode|Append","append"));
		m_wndUninstallLogMode.AddString(_L("Uninstall|LogMode|New","new"));
		m_wndUninstallLogMode.AddString(_L("Uninstall|LogMode|Overwrite","overwrite"));

		EnableControls();

		m_pDoc->AddDirConstants(m_wndUninstallFilesDir);
		m_pDoc->AddDirs(m_wndUninstallFilesDir);

		SetIcons();

		theApp.m_shell.SHAutoComplete(GetDlgItem(IDC_APPPUBLISHERURL),SHACF_URLALL);
		theApp.m_shell.SHAutoComplete(GetDlgItem(IDC_APPSUPPORTURL),SHACF_URLALL);
		theApp.m_shell.SHAutoComplete(GetDlgItem(IDC_APPUPDATESURL),SHACF_URLALL);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		CString strTmp;
		script.SetPropertyBool("Uninstallable",m_bUninstallable ? true : false);
		script.SetPropertyBool("CreateUninstallRegKey",m_bCreateUninstallRegKey ? true : false);
		script.SetPropertyString("UninstallLogMode",script.GetUninstallLogMode(m_nUninstallLogMode));
		script.SetPropertyString("AppPublisher",m_strAppPublisher);
		script.SetPropertyString("AppPublisherURL",m_strAppPublisherURL);
		script.SetPropertyString("AppSupportURL",m_strAppSupportURL);
		script.SetPropertyString("AppUpdatesURL",m_strAppUpdatesURL);
		script.SetPropertyString("AppVersion",m_strAppVersion);
		script.SetPropertyString("AppID",m_strAppID);
		script.SetPropertyString("UninstallFilesDir",m_strUninstallFilesDir);
		script.SetPropertyBool("UpdateUninstallLogAppName",m_bUpdateUninstallLogAppName ? true : false);
		script.SetPropertyBool("UninstallRestartComputer",m_bUninstallRestartComputer ? true : false);
		script.SetPropertyString("AppModifyPath",m_strAppModifyPath);
		script.SetPropertyString("AppComments",m_strAppComments);
		script.SetPropertyString("AppContact",m_strAppContact);
		script.SetPropertyString("AppReadmeFile",m_strAppReadmeFile);
		script.SetPropertyString("AppSupportPhone",m_strAppSupportPhone);
		script.SetPropertyBool("SignedUninstaller",m_bSignedUninstaller ? true : false);
		script.SetPropertyString("SignedUninstallerDir",m_strSignedUninstallerDir);

		if(m_nUninstallDisplayIconIndex>0) {
			strTmp.Format("%s,%d",
				m_strUninstallDisplayIcon,
				m_nUninstallDisplayIconIndex);
			script.SetPropertyString("UninstallDisplayIcon",strTmp);
		} else
			script.SetPropertyString("UninstallDisplayIcon",m_strUninstallDisplayIcon);

		script.SetPropertyString("UninstallDisplayName",m_strUninstallDisplayName);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnGuidGen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE,IDC_APPID);
		if(!m_strAppID.IsEmpty()) {
			if(AtlMessageBox(m_hWnd,_L("Uninstall|AppIDExists","An application id already exists.\n\nDo you want to overwrite it?"),IDR_MAINFRAME,MB_YESNO|MB_ICONWARNING)!=IDYES)
				return 0;
		}

		//SetModified();
		GUID guid;
		UuidCreate(&guid);

		m_strAppID.Format(
			"{{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			guid.Data1, 
			guid.Data2, 
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]
		);
		DoDataExchange(DDX_LOAD,IDC_APPID);

		return 0;
	}

	LRESULT OnUninstallable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		EnableControls();
		return 0;
	}

	LRESULT OnCreateUninstallRegkey(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		EnableControls();
		return 0;
	}

	void EnableControls() {
		m_wndUninstallLogMode.EnableWindow(m_bUninstallable);
		m_wndCreateUninstallRegKey.EnableWindow(m_bUninstallable);
//		m_wndAppID.EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		m_wndUninstallDisplayIconIndex.EnableWindow(m_bUninstallable && !m_strUninstallDisplayIcon.IsEmpty() && m_bCreateUninstallRegKey);

//		GetDlgItem(IDC_STATIC1).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bUninstallable);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bUninstallable);
		GetDlgItem(IDC_STATIC4).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
//		GetDlgItem(IDC_STATIC5).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATIC6).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATIC7).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATIC8).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATIC9).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATICa).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_STATICb).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
//		GetDlgItem(IDC_GUIDGEN).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_UNINSTALLFILESDIR).EnableWindow(m_bUninstallable);
		GetDlgItem(IDC_UPDATEUNINSTALLLOGAPPNAME).EnableWindow(m_bUninstallable);
		GetDlgItem(IDC_CHECK_UNINSTALLRESTARTCOMPUTER).EnableWindow(m_bUninstallable);

//		GetDlgItem(IDC_APPPUBLISHER).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_APPPUBLISHERURL).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_APPSUPPORTURL).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_APPUPDATESURL).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_APPVERSION).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_UNINSTALLDISPLAYICON).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_UNINSTALLDISPLAYNAME).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
		GetDlgItem(IDC_APPSUPPORTPHONE).EnableWindow(m_bUninstallable && m_bCreateUninstallRegKey);
	}

	void SetIcons() {
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_wndUninstallDisplayIconIndex.ResetContent();

		EnableControls();

		// Go through files and find this file
		CScriptList files;
		script.GetList(CInnoScript::SEC_FILES,files);
		for(int nPos=0;nPos<files.GetSize();nPos++) {
			CScriptLine* pFile = files[nPos];
			CString strName;
			script.GetDestName(pFile,strName);

			if(!m_strUninstallDisplayIcon.CompareNoCase(strName)) {
				UpdateIconList(pFile->GetParameter("Source"));
				m_wndUninstallDisplayIconIndex.EnableWindow(m_bUninstallable);
				return;
			}
		}
		m_wndUninstallDisplayIconIndex.EnableWindow(FALSE);
	}

	void UpdateIconList(LPCTSTR pszFilename) {
		m_wndUninstallDisplayIconIndex.ResetContent();

		CString strFileName;
		m_pDoc->GetScriptFileName(strFileName,pszFilename);
		pszFilename = strFileName;

		if(CMyUtils::IsFile(pszFilename)) {
			int nNum = (int)ExtractIcon(_Module.GetResourceInstance(), pszFilename, (UINT) -1);
			for(int i=0; i<nNum; i++) {
				HICON hIcon = ExtractIcon(_Module.GetResourceInstance(), pszFilename, i);
				m_wndUninstallDisplayIconIndex.AddIcon(hIcon,i);
			}
			m_wndUninstallDisplayIconIndex.SetCurSel(m_nUninstallDisplayIconIndex);
		}
	}

	LRESULT OnChangeUninstalldisplayicon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	LRESULT OnKillFocusUninstallDisplayIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		SetIcons();
		return 0;
	}

	LRESULT OnDropDownUninstallDisplayIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(m_wndUninstallDisplayIcon.GetCount()<=0) {
			CWaitCursor wait;
			m_pDoc->AddFiles(m_wndUninstallDisplayIcon);
		}
		return 0;
	}

	CSetupUninstall(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew), 
		m_wndUninstallFilesDir(pDoc), 
		m_bUninstallRestartComputer(FALSE),
		CPropertyPageImpl<CSetupUninstall>(pszTitle),
		m_btnSignedUninstallerDir(_T("Select Signed Uninstaller Directory"))
	{
		//{{AFX_DATA_INIT(CSetupUninstall)
		m_strUninstallDisplayIcon = _T("");
		m_nUninstallDisplayIconIndex = -1;
		m_strUninstallDisplayName = _T("");
		m_bUpdateUninstallLogAppName = FALSE;
		//}}AFX_DATA_INIT
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bUninstallable				= script.GetPropertyBool("Uninstallable");
		m_bCreateUninstallRegKey		= script.GetPropertyBool("CreateUninstallRegKey");
		m_nUninstallLogMode				= script.GetUninstallLogMode(script.GetPropertyString("UninstallLogMode"));
		m_strAppPublisher				= script.GetPropertyString("AppPublisher");
		m_strAppPublisherURL			= script.GetPropertyString("AppPublisherURL");
		m_strAppSupportURL				= script.GetPropertyString("AppSupportURL");
		m_strAppUpdatesURL				= script.GetPropertyString("AppUpdatesURL");
		m_strAppVersion					= script.GetPropertyString("AppVersion");
		m_strAppID						= script.GetPropertyString("AppID");
		m_strUninstallFilesDir			= script.GetPropertyString("UninstallFilesDir");
		m_bUpdateUninstallLogAppName	= script.GetPropertyBool("UpdateUninstallLogAppName");
		m_bUninstallRestartComputer		= script.GetPropertyBool("UninstallRestartComputer");
		m_strAppModifyPath				= script.GetPropertyString("AppModifyPath");
		m_strAppComments				= script.GetPropertyString("AppComments");
		m_strAppContact					= script.GetPropertyString("AppContact");
		m_strAppReadmeFile				= script.GetPropertyString("AppReadmeFile");
		m_strAppSupportPhone			= script.GetPropertyString("AppSupportPhone");
		m_bSignedUninstaller			= script.GetPropertyBool("SignedUninstaller");
		m_strSignedUninstallerDir		= script.GetPropertyString("SignedUninstallerDir");

		CString str(script.GetPropertyString("UninstallDisplayIcon"));
		int nPos = str.ReverseFind(',');
		if(nPos>0) {
			m_nUninstallDisplayIconIndex = atol(str.Mid(nPos+1));
			m_strUninstallDisplayIcon = str.Left(nPos);
		} else
			m_strUninstallDisplayIcon = str;

		m_strUninstallDisplayName		= script.GetPropertyString("UninstallDisplayName");
	}

	CMyDoc*		m_pDoc;
	const bool	m_bNew;
	BOOL		m_bUninstallRestartComputer;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_UNINSTALLABLE, _L("Help|Uninstall|Uninstallable","This determines if Inno Setup's automatic uninstaller is to be included in the installation. If this is yes the uninstaller is included. If this is no, no uninstallation support is included, requiring the end-user to manually remove the files pertaining to your application."))
		TOOLTIP_HANDLER(IDC_CREATEUNINSTALLREGKEY, _L("Help|Uninstall|CreateUninstallRegKey","If this is set to no, Setup won't create an entry in the Add/Remove Programs Control Panel applet."))
		TOOLTIP_HANDLER(IDC_UNINSTALLLOGMODE, _L("Help|Uninstall|UninstallLogMode","append, the default setting, instructs Setup to append to an existing uninstall log when possible.\r\n\r\nnew, which corresponds to the behavior in pre-1.3 versions of Inno Setup, instructs Setup to always create a new uninstall log.\r\n\r\noverwrite instructs Setup to overwrite any existing uninstall logs from the same application instead of appending to them (this is not recommended). The same rules for appending to existing logs apply to overwriting existing logs."))
		TOOLTIP_HANDLER(IDC_APPPUBLISHER, _L("Help|Uninstall|AppPublisher","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPPUBLISHERURL, _L("Help|Uninstall|AppPublisherURL","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPSUPPORTURL, _L("Help|Uninstall|AppSupportURL","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPUPDATESURL, _L("Help|Uninstall|AppUpdatesURL","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPVERSION, _L("Help|Uninstall|AppVersion","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPID, _L("Help|Uninstall|AppID","The value of AppId is stored inside uninstall log files (unins???.dat), and is checked by subsequent installations to determine whether it may append to a particular existing uninstall log.\r\n\r\nAppId also determines the actual name of the Uninstall registry key, to which Inno Setup tacks on \"_is1\" at the end."))
		TOOLTIP_HANDLER(IDC_UNINSTALLFILESDIR, _L("Help|Uninstall|UninstallFilesDir","Specifies the directory where the \"unins*.*\" files for the uninstaller are stored."))
		TOOLTIP_HANDLER(IDC_UNINSTALLDISPLAYICON, _L("Help|Uninstall|UninstallDisplayIcon","This lets you specify a particular icon file (either an executable or an .ico file) to display for the Uninstall entry in the Add/Remove Programs Control Panel applet on Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_UNINSTALLDISPLAYICONINDEX, _L("Help|Uninstall|UninstallDisplayIconIndex","The index of the icon."))
		TOOLTIP_HANDLER(IDC_UNINSTALLDISPLAYNAME, _L("Help|Uninstall|UninstallDisplayName","This lets you specify a custom name for the program's entry in the Add/Remove Programs Control Panel applet. The value may include constants."))
		TOOLTIP_HANDLER(IDC_UPDATEUNINSTALLLOGAPPNAME, _L("Help|Uninstall|UpdateUninstallLogAppName","If yes, when appending to an existing uninstall log, Setup will replace the AppName field in the log with the current installation's AppName."))
		TOOLTIP_HANDLER(IDC_CHECK_UNINSTALLRESTARTCOMPUTER, _L("Help|Uninstall|UninstallRestartComputer","When set to yes, the uninstaller will always prompt the user to restart the system at the end of a successful uninstallation, regardless of whether it is necessary."))
		TOOLTIP_HANDLER(IDC_GUIDGEN,_L("Help|Uninstall|GuidGen","Click this to generate a unique identifier as app id."))
		TOOLTIP_HANDLER(IDC_APPMODIFYPATH, _L("Help|Uninstall|AppModifyPath","When this directive is set, a separate \"Modify\" button in the Add/Remove Programs Control Panel applet in Windows 2000/XP will be displayed. Setting it is optional, and will have no effect on earlier Windows versions. The value may include constants."))
		TOOLTIP_HANDLER(IDC_APPCOMMENTS, _L("Help|Uninstall|AppComments","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPCONTACT, _L("Help|Uninstall|AppContact","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPREADMEFILE, _L("Help|Uninstall|AppReadmeFile","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_APPSUPPORTPHONE, _L("Help|Uninstall|AppSupportPhone","Used for display purposes on the \"Support\" dialog of the Add/Remove Programs Control Panel applet in Windows 2000/XP."))
		TOOLTIP_HANDLER(IDC_SIGNEDUNINSTALLER, _L("Help|Uninstall|SignedUninstaller","Specifies whether the uninstaller program (unins???.exe) should be deployed with a digital signature attached. When the uninstaller has a valid digital signature, Windows Vista users will not see an \"unidentified program\" warning when launching it from outside of Control Panel."))
		TOOLTIP_HANDLER(IDC_SIGNEDUNINSTALLERDIR, _L("Help|Uninstall|SignedUninstallerDir","Specifies the directory in which signed uninstaller files should be stored. By default, such files are stored in the output directory."))
		TOOLTIP_HANDLER(IDC_SIGNEDUNINSTALLERDIR2, _L("Help|Uninstall|SignedUninstallerDir","Specifies the directory in which signed uninstaller files should be stored. By default, such files are stored in the output directory."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOPRODUCTNAME, _L("Help|Uninstall|ProductName","Specifies the product name value for the Setup version info."))
		TOOLTIP_HANDLER(IDC_VERSIONINFOPRODUCTVERSION, _L("Help|Uninstall|ProductVersion","Specifies the product version value for the Setup version info."))
	END_TOOLTIP_MAP()
};

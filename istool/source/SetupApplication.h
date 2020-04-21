// SetupApplication.h

#pragma once

#include "ButtonVer.h"
#include "MyDoc.h"
#include "StringToken.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupApplication dialog

class CSetupApplication : 
	public CPropertyPageImpl<CSetupApplication>,
	public CMyPropertyPageBase<CSetupApplication>,
	public CWinDataExchange<CSetupApplication>,
	public Henden::CToolTipDialog<CSetupApplication>
{
public:
	enum { IDD = IDD_SETUP_APPLICATION };

	BEGIN_MSG_MAP(CSetupApplication)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupApplication>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupApplication>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupApplication>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_APPNAME, EN_CHANGE, OnModified)
		//COMMAND_HANDLER(IDC_APPNAME, EN_KILLFOCUS, OnKillFocusAppName)
		COMMAND_HANDLER(IDC_APPCOPYRIGHT, EN_CHANGE, OnChangeAppCopyright)
		COMMAND_HANDLER(IDC_APPVERNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_PASSWORD, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APP_MINIMUM_VERSION_9X, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APP_MINIMUM_VERSION_NT, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APP_ONLY_BELOW_VERSION_9X, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_APP_ONLY_BELOW_VERSION_NT, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_INFOAFTERFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_INFOBEFOREFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_LICENSEFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_PRIVILEGESREQUIRED, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_CHANGESASSOCIATIONS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ALLOWROOTDIRECTORY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPMUTEX, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_EXTRADISKSPACEREQUIRED, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_ALWAYSRESTART, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_RESTARTIFNEEDEDBYRUN, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ALLOWCANCELDURINGINSTALL, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_TIMESTAMPSINUTC, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_TIMESTAMPROUNDING, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TOUCHDATE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_TOUCHTIME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_CHANGESENVIRONMENT, BN_CLICKED, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupApplication)
		DDX_CONTROL(IDC_BTN_LICENSEFILE, m_btnLicenseFile)
		DDX_CONTROL(IDC_BTN_INFOBEFOREFILE, m_btnInfoBeforeFile)
		DDX_CONTROL(IDC_BTN_INFOAFTERFILE, m_btnInfoAfterFile)
		DDX_TEXT(IDC_APPCOPYRIGHT, m_strAppCopyright)
		DDX_TEXT_LEN(IDC_APPNAME, m_strAppName, 127)
		DDX_TEXT(IDC_APPVERNAME, m_strAppVerName)
		DDX_TEXT(IDC_INFOAFTERFILE, m_strInfoAfterFile)
		DDX_TEXT(IDC_INFOBEFOREFILE, m_strInfoBeforeFile)
		DDX_TEXT(IDC_LICENSEFILE, m_strLicenseFile)
		DDX_CHECK(IDC_CHANGESASSOCIATIONS, m_bChangesAssociations)
		DDX_TEXT(IDC_PASSWORD, m_strPassword)
		DDX_TEXT(IDC_APPMUTEX, m_strAppMutex)
		DDX_TEXT(IDC_APP_MINIMUM_VERSION_9X, m_strMinimumVersion9X)
		DDX_TEXT(IDC_APP_MINIMUM_VERSION_NT, m_strMinimumVersionNT)
		DDX_TEXT(IDC_APP_ONLY_BELOW_VERSION_9X, m_strOnlyBelowVersion9X)
		DDX_TEXT(IDC_APP_ONLY_BELOW_VERSION_NT, m_strOnlyBelowVersionNT)
		DDX_CBINDEX(IDC_PRIVILEGESREQUIRED, m_nPrivilegesRequired)
		DDX_INT(IDC_EXTRADISKSPACEREQUIRED, m_nExtraDiskSpaceRequired)
		DDX_CHECK(IDC_ALWAYSRESTART,m_bAlwaysRestart)
		DDX_CHECK(IDC_RESTARTIFNEEDEDBYRUN,m_bRestartIfNeededByRun)
		DDX_CHECK(IDC_ALLOWCANCELDURINGINSTALL,m_bAllowCancelDuringInstall)
		DDX_CHECK(IDC_TIMESTAMPSINUTC,m_bTimeStampsInUTC)
		DDX_INT(IDC_TIMESTAMPROUNDING,m_nTimeStampRounding)
		DDX_TEXT(IDC_TOUCHDATE,m_strTouchDate)
		DDX_TEXT(IDC_TOUCHTIME,m_strTouchTime)
		DDX_CHECK(IDC_CHANGESENVIRONMENT,m_bChangesEnvironment)
	END_DDX_MAP()

	CEdit		m_wndAppCopyright;
	CButtonVer	m_wndBelowVersionNT;
	CButtonVer	m_wndBelowVersionWin;
	CButtonVer	m_wndMinVersionNT;
	CButtonVer	m_wndMinVersionWindows;
	Henden::CButtonFile	m_btnLicenseFile;
	Henden::CButtonFile	m_btnInfoBeforeFile;
	Henden::CButtonFile	m_btnInfoAfterFile;
	CString		m_strAppCopyright;
	CString		m_strAppName;
	CString		m_strAppVerName;
	CString		m_strInfoAfterFile;
	CString		m_strInfoBeforeFile;
	CString		m_strLicenseFile;
	BOOL		m_bChangesAssociations;
	CString		m_strPassword;
	CString		m_strAppMutex;
	CString		m_strMinimumVersion9X;
	CString		m_strMinimumVersionNT;
	CString		m_strOnlyBelowVersion9X;
	CString		m_strOnlyBelowVersionNT;
	int			m_nPrivilegesRequired;
	CString		m_strPrivilegesRequired;
	UINT		m_nExtraDiskSpaceRequired;
	BOOL		m_bAlwaysRestart;
	BOOL		m_bRestartIfNeededByRun;
	BOOL		m_bAllowCancelDuringInstall;
	BOOL		m_bTimeStampsInUTC;
	UINT		m_nTimeStampRounding;
	CString		m_strTouchDate, m_strTouchTime;
	BOOL		m_bChangesEnvironment;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Application");
		m_wndAppCopyright.Attach(GetDlgItem(IDC_APPCOPYRIGHT));
		m_wndBelowVersionNT.SubclassWindow(GetDlgItem(IDC_BTN_BELOWVERSIONNT));
		m_wndBelowVersionWin.SubclassWindow(GetDlgItem(IDC_BTN_BELOWVERSIONWIN));
		m_wndMinVersionNT.SubclassWindow(GetDlgItem(IDC_BTN_MINVERSIONNT));
		m_wndMinVersionWindows.SubclassWindow(GetDlgItem(IDC_BTN_MINVERSIONWIN));

		CComboBox wnd(GetDlgItem(IDC_PRIVILEGESREQUIRED));
		wnd.AddString("none");
		wnd.AddString("poweruser");
		wnd.AddString("admin");

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		CString strTmp;

		if(!m_strMinimumVersion9X.IsEmpty() || !m_strMinimumVersionNT.IsEmpty()) {
			strTmp.Format("%s,%s",m_strMinimumVersion9X,m_strMinimumVersionNT);
			script.SetPropertyString("MinVersion",strTmp);
		} else
			script.SetPropertyString("MinVersion","");

		if(!m_strOnlyBelowVersion9X.IsEmpty() || !m_strOnlyBelowVersionNT.IsEmpty()) {
			strTmp.Format("%s,%s",m_strOnlyBelowVersion9X,m_strOnlyBelowVersionNT);
			script.SetPropertyString("OnlyBelowVersion",strTmp);
		} else
			script.SetPropertyString("OnlyBelowVersion","");

		script.SetPropertyString("AppCopyright",m_strAppCopyright);
		script.SetPropertyString("AppName",m_strAppName);
		script.SetPropertyString("AppVerName",m_strAppVerName);
		script.SetPropertyString("InfoAfterFile",m_strInfoAfterFile);
		script.SetPropertyString("InfoBeforeFile",m_strInfoBeforeFile);
		script.SetPropertyString("LicenseFile",m_strLicenseFile);
		script.SetPropertyBool("ChangesAssociations",m_bChangesAssociations ? true : false);
		script.SetPropertyString("Password",m_strPassword);
		script.SetPropertyString("AppMutex",m_strAppMutex);
		script.SetPropertyNumber("ExtraDiskSpaceRequired",m_nExtraDiskSpaceRequired);
		// Inno Setup 3.0.3
		script.SetPropertyBool("AlwaysRestart",m_bAlwaysRestart ? true : false);
		script.SetPropertyBool("RestartIfNeededByRun",m_bRestartIfNeededByRun ? true : false);
		// Inno Setup 3.0.4
		script.SetPropertyString("PrivilegesRequired",script.GetPrivilegesRequired(m_nPrivilegesRequired));
		// Inno Setup 4.0.9
		script.SetPropertyBool("AllowCancelDuringInstall",m_bAllowCancelDuringInstall ? true : false);
		// Inno Setup 4.0.10
		script.SetPropertyBool("TimeStampsInUTC",m_bTimeStampsInUTC ? true : false);
		// Inno Setup 4.1.0
		script.SetPropertyNumber("TimeStampRounding",m_nTimeStampRounding);
		// Inno Setup 4.2.0
		script.SetPropertyString("TouchDate",m_strTouchDate);
		script.SetPropertyString("TouchTime",m_strTouchTime);
		script.SetPropertyBool("ChangesEnvironment",m_bChangesEnvironment ? true : false);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	LRESULT OnKillFocusAppName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		bool bChanged = false;

		DoDataExchange(DDX_SAVE);
		if(m_strAppVerName.IsEmpty()) {
			m_strAppVerName = m_strAppName;
			bChanged = true;
		}

		if(bChanged) DoDataExchange(DDX_LOAD);
		return 0;
	}

	LRESULT OnChangeAppCopyright(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		if(!CMyApp::m_prefs.m_bReplaceCopy) return 0;

		CString str;
		m_wndAppCopyright.GetWindowText(str.GetBuffer(256),256);
		int nPos = str.Find("(c)");
		if(nPos<0) nPos = str.Find("(C)");
		if(nPos<0) return 0;

		m_wndAppCopyright.SetSel(nPos,nPos+3,TRUE);
		m_wndAppCopyright.ReplaceSel("©");
		return 0;
	}

	CSetupApplication(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) 
	: m_pDoc(pDoc), m_bNew(bNew),
		m_btnLicenseFile(true,"License Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||"),
		m_btnInfoBeforeFile(true,"Info Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||"),
		m_btnInfoAfterFile(true,"Info Files (.txt;.rtf)|*.txt;*.rtf|All Files (*.*)|*.*||"),
		m_wndMinVersionWindows(VER_WIN),
		m_wndMinVersionNT(VER_NT),
		m_wndBelowVersionWin(VER_WIN),
		m_wndBelowVersionNT(VER_NT),
		CPropertyPageImpl<CSetupApplication>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_strAppCopyright			= script.GetPropertyString("AppCopyright");
		m_strAppName				= script.GetPropertyString("AppName");
		m_strAppVerName				= script.GetPropertyString("AppVerName");
		m_strInfoAfterFile			= script.GetPropertyString("InfoAfterFile");
		m_strInfoBeforeFile			= script.GetPropertyString("InfoBeforeFile");
		m_strLicenseFile			= script.GetPropertyString("LicenseFile");
		m_bChangesAssociations		= script.GetPropertyBool("ChangesAssociations");
		m_strPassword				= script.GetPropertyString("Password");
		m_strAppMutex				= script.GetPropertyString("AppMutex");
		m_nPrivilegesRequired		= script.GetPrivilegesRequired(script.GetPropertyString("PrivilegesRequired"));
		m_nExtraDiskSpaceRequired	= script.GetPropertyNumber("ExtraDiskSpaceRequired");
		// Inno Setup 3.0.3
		m_bAlwaysRestart				= script.GetPropertyBool("AlwaysRestart");
		m_bRestartIfNeededByRun			= script.GetPropertyBool("RestartIfNeededByRun");
		// Inno Setup 4.0.9
		m_bAllowCancelDuringInstall		= script.GetPropertyBool("AllowCancelDuringInstall");
		// Inno Setup 4.0.10
		m_bTimeStampsInUTC				= script.GetPropertyBool("TimeStampsInUTC");
		// Inno Setup 4.1.0
		m_nTimeStampRounding			= script.GetPropertyNumber("TimeStampRounding");
		// Inno Setup 4.2.0
		m_strTouchDate					= script.GetPropertyString("TouchDate");
		m_strTouchTime					= script.GetPropertyString("TouchTime");
		m_bChangesEnvironment			= script.GetPropertyBool("ChangesEnvironment");
		
		{ // Version stuff
			CString strMinVersion(script.GetPropertyString("MinVersion"));
			if(strMinVersion.IsEmpty()) {
				m_strMinimumVersion9X.Empty();
				m_strMinimumVersionNT.Empty();
			} else {
				CStringToken tok(strMinVersion,",");
				m_strMinimumVersion9X = tok.GetNext();
				m_strMinimumVersionNT = tok.GetNext();
			}

			CString strOnlyBelowVersion(script.GetPropertyString("OnlyBelowVersion"));
			if(strOnlyBelowVersion.IsEmpty()) {
				m_strOnlyBelowVersion9X.Empty();
				m_strOnlyBelowVersionNT.Empty();
			} else {
				CStringToken tok(strOnlyBelowVersion,",");
				m_strOnlyBelowVersion9X = tok.GetNext();
				m_strOnlyBelowVersionNT = tok.GetNext();
			}
		}
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_BTN_LICENSEFILE, _L("Help|Application|LicenseFileButton","Click to select license file."))
		TOOLTIP_HANDLER(IDC_BTN_INFOBEFOREFILE, _L("Help|Application|InfoBeforeFileButton","Click to select info before file."))
		TOOLTIP_HANDLER(IDC_BTN_INFOAFTERFILE, _L("Help|Application|InfoAfterFileButton","Click to select info after file."))
		TOOLTIP_HANDLER(IDC_APPCOPYRIGHT, _L("Help|Application|AppCopyright","This is optional, and is only used to display a copyright message in the bottom-right corner of Setup's background window."))
		TOOLTIP_HANDLER(IDC_APPNAME, _L("Help|Application|AppName","This required directive specifies the title of the application you are installing. Do not include the version number, as the AppVerName directive is for that purpose."))
		TOOLTIP_HANDLER(IDC_APPVERNAME, _L("Help|Application|AppVerName","The value of this required directive should be the same (or similar to) the value of AppName, but it should also include the program's version number. The value may include constants."))
		TOOLTIP_HANDLER(IDC_INFOAFTERFILE, _L("Help|Application|InfoAfterFile","Specifies the name of an optional \"readme\" file, in .txt or .rtf (rich text) format, which is displayed after a successful install."))
		TOOLTIP_HANDLER(IDC_INFOBEFOREFILE, _L("Help|Application|InfoBeforeFile","Specifies the name of an optional \"readme\" file, in .txt or .rtf (rich text) format, which is displayed before the user selects the destination directory for the program."))
		TOOLTIP_HANDLER(IDC_LICENSEFILE, _L("Help|Application|LicenseFile","Specifies the name of an optional license agreement file, in .txt or .rtf (rich text) format, which is displayed before the user selects the destination directory for the program."))
		TOOLTIP_HANDLER(IDC_CHANGESASSOCIATIONS, _L("Help|Application|ChangesAssociations","When set to yes, Setup will tell Explorer to refresh its file associations information at the end of the installation, and Uninstall will do the same at the end of uninstallation."))
		TOOLTIP_HANDLER(IDC_PASSWORD, _L("Help|Application|Password","Specifies a password you want to prompt the user for at the beginning of the installation."))
		TOOLTIP_HANDLER(IDC_APPMUTEX, _L("Help|Application|AppMutex","This directive is used to prevent the user from installing new versions of an application while the application is still running, and to prevent the user from uninstalling a running application."))
		TOOLTIP_HANDLER(IDC_APP_MINIMUM_VERSION_9X, _L("Help|Application|MinVerWin","This directive lets you specify a minimum version of Windows that your software runs on."))
		TOOLTIP_HANDLER(IDC_APP_MINIMUM_VERSION_NT, _L("Help|Application|MinVerNT","This directive lets you specify a minimum version of Windows NT that your software runs on."))
		TOOLTIP_HANDLER(IDC_APP_ONLY_BELOW_VERSION_9X, _L("Help|Application|BelowVerWin","This directive lets you specify a minimum version of Windows that your software will not run on."))
		TOOLTIP_HANDLER(IDC_APP_ONLY_BELOW_VERSION_NT, _L("Help|Application|BelowVerNT","This directive lets you specify a minimum version of Windows NT that your software will not run on."))
		TOOLTIP_HANDLER(IDC_PRIVILEGESREQUIRED, _L("Help|Application|PrivilegesRequired","This directive specifies the minimum user privileges required to run the installation."))
		TOOLTIP_HANDLER(IDC_EXTRADISKSPACEREQUIRED, _L("Help|Application|ExtraDiskSpaceRequired","Normally, the disk space requirement displayed on the wizard is calculated by adding up the size of all the files in the [Files] section. If you want to increase the disk space display for whatever reason, set ExtraDiskSpaceRequired to the amount of bytes you wish to add to this figure."))
		TOOLTIP_HANDLER(IDC_ALWAYSRESTART,_L("Help|Application|AlwaysRestart","When set to yes, Setup will always prompt the user to restart the system at the end of a successful installation, regardless of whether this is necessary."))
		TOOLTIP_HANDLER(IDC_RESTARTIFNEEDEDBYRUN,_L("Help|Application|RestartIfNeededByRun","When set to yes, and a program executed in the [Run] section queues files to be replaced on the next reboot, Setup will detect this and prompt the user to restart the computer at the end of installation."))
		TOOLTIP_HANDLER(IDC_ALLOWCANCELDURINGINSTALL,_L("Help|Application|AllowCAncelDuringInstall","Setting this to no prevents the user from cancelling during the actual installation process, by disabling the Cancel button and ignoring clicks on the close button."))
		TOOLTIP_HANDLER(IDC_TIMESTAMPSINUTC,_L("Help|Application|TimeStampsInUTC","If TimeStampsInUTC is set to yes, time stamps will be saved and restored in UTC -- the native time format of Win32 and NTFS."))
		TOOLTIP_HANDLER(IDC_TIMESTAMPROUNDING,_L("Help|Application|TimeStampRounding","By default, time stamps on files referenced by [Files] section entries are rounded down to the nearest 2-second boundary. FAT partitions have only a 2-second time stamp resolution, so this ensures that time stamps are set the same way on both FAT and NTFS partitions.\r\n\r\nThe rounding can be altered or disabled by setting the TimeStampRounding directive. Setting it to 0 will disable the rounding. Setting it to a number between 1 and 60 will cause time stamps to be rounded down to the nearest TimeStampRounding-second boundary."))
		TOOLTIP_HANDLER(IDC_TOUCHDATE,_L("Help|Application|TouchDate","The date used in the time/date stamp of files referenced by [Files] section entries that include the touch flag."))
		TOOLTIP_HANDLER(IDC_TOUCHTIME,_L("Help|Application|TouchTime","The time used in the time/date stamp of files referenced by [Files] section entries that include the touch flag."))
		TOOLTIP_HANDLER(IDC_BTN_BELOWVERSIONNT,_L("Help|Application|BelowVerNTButton","Click to select version."))
		TOOLTIP_HANDLER(IDC_BTN_BELOWVERSIONWIN,_L("Help|Application|BelowVerWinButton","Click to select version."))
		TOOLTIP_HANDLER(IDC_BTN_MINVERSIONNT,_L("Help|Application|MinVerNTButton","Click to select version."))
		TOOLTIP_HANDLER(IDC_BTN_MINVERSIONWIN,_L("Help|Application|MinVerWinButton","Click to select version."))
		TOOLTIP_HANDLER(IDC_CHANGESENVIRONMENT,_L("Help|Application|ChangesEnvironment","When set to yes, at the end of the installation Setup will notify other running applications (notably Windows Explorer) that they should reload their environment variables from the registry."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

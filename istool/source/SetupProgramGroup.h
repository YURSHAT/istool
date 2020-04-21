// Setup3.h

#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupProgramGroup dialog

class CSetupProgramGroup : 
	public CPropertyPageImpl<CSetupProgramGroup>,
	public CMyPropertyPageBase<CSetupProgramGroup>,
	public CWinDataExchange<CSetupProgramGroup>,
	public Henden::CToolTipDialog<CSetupProgramGroup>
{
public:
	enum { IDD = IDD_SETUP_PROGRAMGROUP };

	BEGIN_MSG_MAP(CSetupProgramGroup)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupProgramGroup>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupProgramGroup>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupProgramGroup>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_DISABLEPROGRAMGROUPPAGE, BN_CLICKED, OnDisableProgramGroupPage)
		COMMAND_HANDLER(IDC_ALLOWNOICONS, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ALWAYSUSEPERSONALGROUP, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTGROUPNAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_USEPREVIOUSGROUP, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_APPENDDEFAULTGROUPNAME, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupProgramGroup)
		DDX_CHECK(IDC_ALLOWNOICONS, m_bAllowNoIcons)
		DDX_CHECK(IDC_DISABLEPROGRAMGROUPPAGE, m_bDisableProgramGroupPage)
		DDX_TEXT(IDC_DEFAULTGROUPNAME, m_strDefaultGroupName)
		DDX_CHECK(IDC_ALWAYSUSEPERSONALGROUP, m_bAlwaysUsePersonalGroup)
		DDX_CHECK(IDC_USEPREVIOUSGROUP, m_bUsePreviousGroup)
		DDX_CHECK(IDC_APPENDDEFAULTGROUPNAME, m_bAppendDefaultGroupName)
	END_DDX_MAP()

	BOOL	m_bAllowNoIcons;
	BOOL	m_bDisableProgramGroupPage;
	CString	m_strDefaultGroupName;
	BOOL	m_bAlwaysUsePersonalGroup;
	BOOL	m_bUsePreviousGroup;
	BOOL	m_bAppendDefaultGroupName;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"ProgramGroup");

		EnableControls();
		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		script.SetPropertyBool("AllowNoIcons",m_bAllowNoIcons ? true : false);
		script.SetPropertyBool("DisableProgramGroupPage",m_bDisableProgramGroupPage ? true : false);
		script.SetPropertyString("DefaultGroupName",m_strDefaultGroupName);
		script.SetPropertyBool("AlwaysUsePersonalGroup",m_bAlwaysUsePersonalGroup ? true : false);
		script.SetPropertyBool("UsePreviousGroup",m_bUsePreviousGroup ? true : false);
		script.SetPropertyBool("AppendDefaultGroupName",m_bAppendDefaultGroupName ? true : false);
		
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnCreateAppDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	LRESULT OnDisableProgramGroupPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		EnableControls();
		return 0;
	}

	LRESULT OnUninstallable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	void EnableControls() {
		GetDlgItem(IDC_ALLOWNOICONS).EnableWindow(!m_bDisableProgramGroupPage);
		GetDlgItem(IDC_DEFAULTGROUPNAME).EnableWindow(!m_bDisableProgramGroupPage);
		GetDlgItem(IDC_ALWAYSUSEPERSONALGROUP).EnableWindow(!m_bDisableProgramGroupPage);
		GetDlgItem(IDC_STATIC1).EnableWindow(!m_bDisableProgramGroupPage);
		GetDlgItem(IDC_APPENDDEFAULTGROUPNAME).EnableWindow(!m_bDisableProgramGroupPage);
	}

	CSetupProgramGroup(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew), 
		CPropertyPageImpl<CSetupProgramGroup>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bAllowNoIcons					= script.GetPropertyBool("AllowNoIcons");
		m_bDisableProgramGroupPage		= script.GetPropertyBool("DisableProgramGroupPage");
		m_strDefaultGroupName			= script.GetPropertyString("DefaultGroupName");
		m_bAlwaysUsePersonalGroup		= script.GetPropertyBool("AlwaysUsePersonalGroup");
		m_bUsePreviousGroup				= script.GetPropertyBool("UsePreviousGroup");
		m_bAppendDefaultGroupName		= script.GetPropertyBool("AppendDefaultGroupName");
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_ALLOWNOICONS, _L("Help|ProgramGroup|AllowNoIcons","This is used to determine whether Setup should display a Don't create any icons check box, which allows the user to skip creation of program icons. If it is no the check box will not be displayed; if it is yes it will be displayed."))
		TOOLTIP_HANDLER(IDC_DISABLEPROGRAMGROUPPAGE, _L("Help|ProgramGroup|DisableProgramGroupPage","If this is set to yes, Setup will not show the Select Start Menu Folder wizard page. In this case, it uses the folder name specified by the DefaultGroupName [Setup] section directive, or \"(Default)\" if none is specified."))
		TOOLTIP_HANDLER(IDC_DEFAULTGROUPNAME, _L("Help|ProgramGroup|DefaultGroupName","The value of this directive is used for the default Start Menu folder name on the Select Start Menu Folder page of the wizard. If this directive is blank or isn't specified, it will use \"(Default)\" for the name."))
		TOOLTIP_HANDLER(IDC_ALWAYSUSEPERSONALGROUP, _L("Help|ProgramGroup|AlwaysUsePersonalGroup","Normally on Windows NT platforms, Inno Setup's {group} constant points to the All Users start menu if the user has administrative privileges. If this directive is set to yes, it always uses current user's profile."))
		TOOLTIP_HANDLER(IDC_USEPREVIOUSGROUP, _L("Help|ProgramGroup|UsePreviousGroup","When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the Start Menu folder name of the previous installation as the default Start Menu folder name presented to the user in the wizard."))
		TOOLTIP_HANDLER(IDC_APPENDDEFAULTGROUPNAME, _L("Help|ProgramGroup|AppendDefaultGroupName","By default, when a folder in the dialog displayed by the Browse... button on the Select Start Menu Folder wizard page is clicked, Setup automatically appends the last component of DefaultGroupName onto the new path. For example, if DefaultGroupName is My Program and \"Accessories\" is clicked, the new path will become \"Accessories\\My Program\".\r\n\r\nSetting this directive to no disables the aforementioned behavior. In addition, it causes a Make New Folder button to appear on the dialog."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

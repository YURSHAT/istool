// SetupPages.h

#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupPages dialog

class CSetupPages : 
	public CPropertyPageImpl<CSetupPages>,
	public CMyPropertyPageBase<CSetupPages>,
	public CWinDataExchange<CSetupPages>,
	public Henden::CToolTipDialog<CSetupPages>
{
public:
	enum { IDD = IDD_SETUP_PAGES };

	BEGIN_MSG_MAP(CSetupPages)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupPages>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupPages>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupPages>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_DISABLESTARTUPPROMPT, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DISABLEFINISHEDPAGE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ALWAYSSHOWDIRONREADYPAGE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_ALWAYSSHOWGROUPONREADYPAGE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SHOWTASKSTREELINES, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SHOWLANGUAGEDIALOG, CBN_SELCHANGE, OnShowLanguageDialog)
		COMMAND_HANDLER(IDC_EXT_DISABLE_READY_PAGE, BN_CLICKED, OnDisableReadyPage)
		COMMAND_HANDLER(IDC_EXT_DISABLEREADYMEMO, BN_CLICKED, OnDisableReadyMemo)
		COMMAND_HANDLER(IDC_LANGUAGEDETECTIONMETHOD, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_TERMINALSERVICESAWARE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_SHOWUNDISPLAYABLELANGUAGES, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupPages)
		DDX_CHECK(IDC_DISABLESTARTUPPROMPT, m_bDisableStartupPrompt)
		DDX_CHECK(IDC_DISABLEFINISHEDPAGE, m_bDisableFinishedPage)
		DDX_CHECK(IDC_EXT_DISABLEREADYMEMO, m_bDisableReadyMemo)
		DDX_CHECK(IDC_EXT_ALWAYSSHOWCOMPONENTSLIST, m_bAlwaysShowComponentsList)
		DDX_CHECK(IDC_EXT_SHOWCOMPONENTSIZES, m_bShowComponentSizes)
		DDX_CHECK(IDC_EXT_FLATCOMPONENTSLIST, m_bFlatComponentsList)
		DDX_CHECK(IDC_EXT_DISABLE_READY_PAGE, m_bDisableReadyPage)
		DDX_CHECK(IDC_EXT_USEPREVIOUSSETUPTYPE, m_bUsePreviousSetupType)
		DDX_CHECK(IDC_EXT_USE_PREVIOUS_TASKS, m_bUsePreviousTasks)
		DDX_CHECK(IDC_ALWAYSSHOWDIRONREADYPAGE, m_bAlwaysShowDirOnReadyPage)
		DDX_CHECK(IDC_ALWAYSSHOWGROUPONREADYPAGE, m_bAlwaysShowGroupOnReadyPage)
		DDX_CHECK(IDC_SHOWTASKSTREELINES, m_bShowTasksTreeLines)
		DDX_CBINDEX(IDC_SHOWLANGUAGEDIALOG, m_nShowLanguageDialog)
		DDX_CBINDEX(IDC_LANGUAGEDETECTIONMETHOD, m_nLanguageDetectionMethod)
		DDX_CHECK(IDC_TERMINALSERVICESAWARE, m_bTerminalServicesAware)
		DDX_CHECK(IDC_SHOWUNDISPLAYABLELANGUAGES, m_bShowUndisplayableLanguages)
	END_DDX_MAP()

	CComboBox	m_wndRestartComputer;
	BOOL	m_bDisableStartupPrompt;
	BOOL	m_bDisableFinishedPage;
	BOOL	m_bDisableReadyMemo;
	BOOL	m_bAlwaysShowComponentsList;
	BOOL	m_bShowComponentSizes;
	BOOL	m_bFlatComponentsList;
	BOOL	m_bDisableReadyPage;
	BOOL	m_bUsePreviousSetupType;
	BOOL	m_bUsePreviousTasks;
	BOOL	m_bAlwaysShowDirOnReadyPage;
	BOOL	m_bAlwaysShowGroupOnReadyPage;
	BOOL	m_bShowTasksTreeLines;
	int		m_nShowLanguageDialog;
	BOOL	m_bDetectLanguageUsingLocale;
	int		m_nLanguageDetectionMethod;
	BOOL	m_bTerminalServicesAware, m_bShowUndisplayableLanguages;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Pages");
		m_wndRestartComputer.Attach(GetDlgItem(IDC_RESTARTCOMPUTER));

		EnableControls();

		CComboBox box = GetDlgItem(IDC_LANGUAGEDETECTIONMETHOD);
		box.AddString("uilanguage");
		box.AddString("locale");
		box.AddString("none");

		box = GetDlgItem(IDC_SHOWLANGUAGEDIALOG);
		box.AddString("auto");
		box.AddString("yes");
		box.AddString("no");


		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		script.SetPropertyBool("DisableStartupPrompt",m_bDisableStartupPrompt ? true : false);
		script.SetPropertyBool("DisableFinishedPage",m_bDisableFinishedPage ? true : false);
		// Inno Setup 2.0.0
		script.SetPropertyBool("DisableReadyMemo",m_bDisableReadyMemo ? true : false);
		script.SetPropertyBool("AlwaysShowComponentsList",m_bAlwaysShowComponentsList ? true : false);
		script.SetPropertyBool("ShowComponentSizes",m_bShowComponentSizes ? true : false);
		script.SetPropertyBool("FlatComponentsList",m_bFlatComponentsList ? true : false);
		script.SetPropertyBool("DisableReadyPage",m_bDisableReadyPage ? true : false);
		script.SetPropertyBool("UsePreviousSetupType",m_bUsePreviousSetupType ? true : false);
		script.SetPropertyBool("UsePreviousTasks",m_bUsePreviousTasks ? true : false);
		// Inno Setup 2.0.7
		script.SetPropertyBool("AlwaysShowDirOnReadyPage",m_bAlwaysShowDirOnReadyPage ? true : false);
		script.SetPropertyBool("AlwaysShowGroupOnReadyPage",m_bAlwaysShowGroupOnReadyPage ? true : false);
		// Inno Setup 4.0.0
		script.SetPropertyBool("ShowTasksTreeLines",m_bShowTasksTreeLines ? true : false);
		script.SetPropertyString("ShowLanguageDialog",script.GetShowLanguageDialog(m_nShowLanguageDialog));
		// Inno Setup 4.0.1
		script.SetPropertyBool("DetectLanguageUsingLocale",m_bDetectLanguageUsingLocale ? true : false);
		// Inno Setup 4.0.10
		script.SetPropertyString("LanguageDetectionMethod",script.GetLanguageDetectionMethod(m_nLanguageDetectionMethod));
		// Inno Setup 5.1.7
		script.SetPropertyBool("TerminalServicesAware",m_bTerminalServicesAware ? true : false);
		script.SetPropertyBool("ShowUndisplayableLanguages",m_bShowUndisplayableLanguages ? true : false);
		
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnDisableReadyPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_EXT_DISABLE_READY_PAGE);
		EnableControls();
		return 0;
	}

	LRESULT OnDisableReadyMemo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_EXT_DISABLEREADYMEMO);
		EnableControls();
		return 0;
	}

	LRESULT OnShowLanguageDialog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE,IDC_SHOWLANGUAGEDIALOG);
		EnableControls();
		return 0;
	}

	void EnableControls() {
		GetDlgItem(IDC_EXT_DISABLEREADYMEMO).EnableWindow(!m_bDisableReadyPage);
		GetDlgItem(IDC_ALWAYSSHOWDIRONREADYPAGE).EnableWindow(!m_bDisableReadyPage && !m_bDisableReadyMemo);
		GetDlgItem(IDC_ALWAYSSHOWGROUPONREADYPAGE).EnableWindow(!m_bDisableReadyPage && !m_bDisableReadyMemo);
	}

	CSetupPages(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew),
		CPropertyPageImpl<CSetupPages>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bDisableStartupPrompt			= script.GetPropertyBool("DisableStartupPrompt");
		m_bDisableFinishedPage			= script.GetPropertyBool("DisableFinishedPage");
		// Inno Setup 2.0.0
		m_bDisableReadyMemo				= script.GetPropertyBool("DisableReadyMemo");
		m_bAlwaysShowComponentsList		= script.GetPropertyBool("AlwaysShowComponentsList");
		m_bShowComponentSizes			= script.GetPropertyBool("ShowComponentSizes");
		m_bFlatComponentsList			= script.GetPropertyBool("FlatComponentsList");
		m_bDisableReadyPage				= script.GetPropertyBool("DisableReadyPage");
		m_bUsePreviousSetupType			= script.GetPropertyBool("UsePreviousSetupType");
		m_bUsePreviousTasks				= script.GetPropertyBool("UsePreviousTasks");
		// Inno Setup 2.0.7
		m_bAlwaysShowDirOnReadyPage		= script.GetPropertyBool("AlwaysShowDirOnReadyPage");
		m_bAlwaysShowGroupOnReadyPage	= script.GetPropertyBool("AlwaysShowGroupOnReadyPage");
		// Inno Setup 4.0.0
		m_bShowTasksTreeLines			= script.GetPropertyBool("ShowTasksTreeLines");
		m_nShowLanguageDialog			= script.GetShowLanguageDialog(script.GetPropertyString("ShowLanguageDialog"));
		// Inno Setup 4.0.1
		m_bDetectLanguageUsingLocale	= script.GetPropertyBool("DetectLanguageUsingLocale");
		// Inno Setup 4.0.10
		m_nLanguageDetectionMethod		= script.GetLanguageDetectionMethod(script.GetPropertyString("LanguageDetectionMethod"));
		// Inno Setup 5.1.7
		m_bTerminalServicesAware		= script.GetPropertyBool("TerminalServicesAware");
		m_bShowUndisplayableLanguages	= script.GetPropertyBool("ShowUndisplayableLanguages");
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_DISABLESTARTUPPROMPT, _L("Help|Pages|DisableStartupPrompt","When this is set to yes, Setup will not show the This will install... Do you wish to continue? prompt."))
		TOOLTIP_HANDLER(IDC_DISABLEFINISHEDPAGE, _L("Help|Pages|DisableFinishedPage","If this is set to yes, Setup will not show the Setup Completed wizard page, and instead will immediately close the Setup program once the installation process finishes."))
		TOOLTIP_HANDLER(IDC_EXT_DISABLEREADYMEMO, _L("Help|Pages|DisableReadyMemo","If this is set to yes, Setup will not show a list of settings on the Ready to Install wizard page. Otherwise the list is shown and contains information like the chosen setup type and the chosen components."))
		TOOLTIP_HANDLER(IDC_EXT_ALWAYSSHOWCOMPONENTSLIST, _L("Help|Pages|AlwaysShowComponentsList","If this directive is set to yes, Setup will always show the components list for customizable setups. If this is set to no Setup will only show the components list if the user selected a custom type from the type list."))
		TOOLTIP_HANDLER(IDC_EXT_SHOWCOMPONENTSIZES, _L("Help|Pages|ShowComponentSizes","When this directive is set to yes, Setup will show the size of a component in the components list."))
		TOOLTIP_HANDLER(IDC_EXT_FLATCOMPONENTSLIST, _L("Help|Pages|FlatComponentsList","When this directive is set to yes, Setup will use 'flat' checkboxes for the components list. Otherwise Setup will use '3D' checkboxes."))
		TOOLTIP_HANDLER(IDC_EXT_DISABLE_READY_PAGE, _L("Help|Pages|DisableReadyPage","If this is set to yes, Setup will not show the Ready to Install wizard page."))
		TOOLTIP_HANDLER(IDC_EXT_USEPREVIOUSSETUPTYPE, _L("Help|Pages|UsePreviousSetupType","When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the setup type and component settings of the previous installation as the default settings presented to the user in the wizard."))
		TOOLTIP_HANDLER(IDC_EXT_USE_PREVIOUS_TASKS, _L("Help|Pages|UsePreviousTasks","When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the task settings of the previous installation as the default settings presented to the user in the wizard."))
		TOOLTIP_HANDLER(IDC_ALWAYSSHOWDIRONREADYPAGE, _L("Help|Pages|AlwaysShowDirOnReadyPage","If this directive is set to yes, Setup will always show the selected directory in the list of settings on the Ready to Install wizard page. If this is set to no, Setup will not show the selected directory if DisableDirPage is yes."))
		TOOLTIP_HANDLER(IDC_ALWAYSSHOWGROUPONREADYPAGE, _L("Help|Pages|AlwaysShowGroupOnReadyPage","If this directive is set to yes, Setup will always show the selected Start Menu folder name in the list of settings on the Ready to Install wizard page. If this is set to no, Setup will not show the selected Start Menu folder name if DisableProgramGroupPage is yes."))
		TOOLTIP_HANDLER(IDC_SHOWTASKSTREELINES, _L("Help|Pages|ShowTasksTreeLines","When this directive is set to yes, Setup will show 'tree lines' between parent and sub tasks."))
		TOOLTIP_HANDLER(IDC_SHOWLANGUAGEDIALOG, _L("Help|Pages|ShowLanguageDialog","When set to yes and there are multiple [Languages] section entries, a Select Language dialog will be displayed to give the user an opportunity to override the language Setup chose by default. See the [Languages] section documentation for more information.\r\n\r\nWhen set to no, the dialog will never be displayed.\r\n\r\nWhen set to auto, the dialog will only be displayed if Setup does not find a language identifier match."))
		TOOLTIP_HANDLER(IDC_LANGUAGEDETECTIONMETHOD, _L("Help|Pages|LanguageDetectionMethod","Specifies how setup will detect the users language."))
		TOOLTIP_HANDLER(IDC_TERMINALSERVICESAWARE, _L("Help|Pages|TerminalServicesAware","Specifies whether the compiler should set the \"Terminal Services aware\" flag in the headers of the Setup and Uninstall programs."))
		TOOLTIP_HANDLER(IDC_SHOWUNDISPLAYABLELANGUAGES, _L("Help|Pages|ShowUndisplayableLanguages","By default, languages that cannot be displayed on a user's system due to a code page mismatch are not listed in the Select Language dialog. For example, Russian text can only be displayed properly if the active code page is 1251; if the user isn't running code page 1251 they will not see Russian as an option.\r\n\r\nIf this directive is set to yes, all languages will be listed in the Select Language dialog. To avoid user confusion, it is not recommended that you enable this in production installs."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

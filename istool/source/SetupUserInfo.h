// Setup3.h

#pragma once

#include "MyDoc.h"
#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupUserInfo dialog

class CSetupUserInfo : 
	public CPropertyPageImpl<CSetupUserInfo>,
	public CMyPropertyPageBase<CSetupUserInfo>,
	public CWinDataExchange<CSetupUserInfo>,
	public Henden::CToolTipDialog<CSetupUserInfo>
{
public:
	enum { IDD = IDD_SETUP_USERINFO };

	BEGIN_MSG_MAP(CSetupUserInfo)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupUserInfo>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupUserInfo>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupUserInfo>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_USERINFOPAGE, BN_CLICKED, OnUserInfoPage)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFONAME, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFOORG, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_USEPREVIOUSUSERINFO, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_DEFAULTUSERINFOSERIAL, BN_CLICKED, OnModified)
	//	REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupUserInfo)
		DDX_CHECK(IDC_USEPREVIOUSUSERINFO, m_bUsePreviousUserInfo)
		DDX_CHECK(IDC_USERINFOPAGE, m_bUserInfoPage)
		DDX_TEXT(IDC_DEFAULTUSERINFONAME, m_strDefaultUserInfoName)
		DDX_TEXT(IDC_DEFAULTUSERINFOORG, m_strDefaultUserInfoOrg)
		DDX_TEXT(IDC_DEFAULTUSERINFOSERIAL,m_strDefaultUserInfoSerial)
	END_DDX_MAP()

	BOOL	m_bUsePreviousUserInfo;
	BOOL	m_bUserInfoPage;
	CString	m_strDefaultUserInfoName;
	CString	m_strDefaultUserInfoOrg;
	CString	m_strDefaultUserInfoSerial;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"UserInfo");

		GetDlgItem(IDC_STATIC1).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFONAME).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOORG).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_USEPREVIOUSUSERINFO).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOSERIAL).EnableWindow(m_bUserInfoPage);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		// Inno Setup 3.0.0
		script.SetPropertyBool("UsePreviousUserInfo",m_bUsePreviousUserInfo ? true : false);
		script.SetPropertyBool("UserInfoPage",m_bUserInfoPage ? true : false);
		script.SetPropertyString("DefaultUserInfoName",m_strDefaultUserInfoName);
		script.SetPropertyString("DefaultUserInfoOrg",m_strDefaultUserInfoOrg);
		// My Inno Setup Extensions 3.0.6.1
		script.SetPropertyString("DefaultUserInfoSerial",m_strDefaultUserInfoSerial);
		
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

		return 0;
	}

	LRESULT OnUninstallable(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);
		return 0;
	}

	LRESULT OnUserInfoPage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		DoDataExchange(DDX_SAVE);

		GetDlgItem(IDC_STATIC1).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC2).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFONAME).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOORG).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_USEPREVIOUSUSERINFO).EnableWindow(m_bUserInfoPage);
		GetDlgItem(IDC_DEFAULTUSERINFOSERIAL).EnableWindow(m_bUserInfoPage);
		return 0;
	}

	CSetupUserInfo(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew),
		CPropertyPageImpl<CSetupUserInfo>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		// Inno Setup 3.0.0
		m_bUsePreviousUserInfo			= script.GetPropertyBool("UsePreviousUserInfo");
		m_bUserInfoPage					= script.GetPropertyBool("UserInfoPage");
		m_strDefaultUserInfoName		= script.GetPropertyString("DefaultUserInfoName");
		m_strDefaultUserInfoOrg			= script.GetPropertyString("DefaultUserInfoOrg");
		// My Inno Setup Extensions 3.0.6.1
		m_strDefaultUserInfoSerial		= script.GetPropertyString("DefaultUserInfoSerial");
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_USEPREVIOUSUSERINFO, _L("Help|UserInfo|UsePreviousUserInfo","When this directive is yes, the default, at startup Setup will look in the registry to see if the same application is already installed, and if so, it will use the name, organization and serial number entered previously as the default settings presented to the user on the User Information wizard page."))
		TOOLTIP_HANDLER(IDC_USERINFOPAGE, _L("Help|UserInfo|UserInfoPage","If this is set to yes, Setup will show a User Information wizard page which asks for the user's name, organization and possibly a serial number."))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFONAME, _L("Help|UserInfo|DefaultUserInfoName","Specifies the default name shown on the User Information wizard page. This can include constants."))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFOORG, _L("Help|UserInfo|DefaultUserInfoOrg","Specifies the default organization shown on the User Information wizard page. This can include constants."))
		TOOLTIP_HANDLER(IDC_DEFAULTUSERINFOSERIAL,_L("Help|UserInfo|DefaultUserInfoSerial","Specifies the default serial number shown on the User Information wizard page. This can include constants."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

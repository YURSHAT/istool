// SetupAppearance.h

#pragma once

class CMyDoc;

#include "MyDoc.h"
#include "ColorPickerCB.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupAppearance dialog

class CSetupAppearance : 
	public CPropertyPageImpl<CSetupAppearance>,
	public CMyPropertyPageBase<CSetupAppearance>,
	public CWinDataExchange<CSetupAppearance>,
	public Henden::CToolTipDialog<CSetupAppearance>
{
public:
	enum { IDD = IDD_SETUP_APPEARANCE };

	BEGIN_MSG_MAP(CSetupAppearance)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CSetupAppearance>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CSetupAppearance>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CSetupAppearance>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
		COMMAND_HANDLER(IDC_BACKCOLOR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_BACKCOLOR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_BACKSOLID, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_WIZARDIMAGEFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_WIZARDIMAGEBACKCOLOR, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_WIZARDIMAGEBACKCOLOR, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_WINDOWVISIBLE, BN_CLICKED, OnWindowVisible)
		COMMAND_HANDLER(IDC_WINDOWSHOWCAPTION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_WINDOWSTARTMAXIMIZED, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_WINDOWRESIZABLE, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_BACKCOLOR2, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_BACKCOLORDIRECTION, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SETUPICONFILE, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_WIZARDIMAGESTRETCH, BN_CLICKED, OnModified)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	BEGIN_DDX_MAP(CSetupAppearance)
		DDX_CONTROL(IDC_BTN_WIZARD_IMAGE_FILE, m_btnWizardImageFile)
		DDX_CONTROL(IDC_EXT_WIZARDSMALLIMAGEFILE2, m_wndWizardSmallImageFile)
		DDX_CHECK(IDC_BACKSOLID, m_bBackSolid)
		DDX_CHECK(IDC_WINDOWRESIZABLE, m_bWindowResizable)
		DDX_CHECK(IDC_WINDOWSHOWCAPTION, m_bWindowShowCaption)
		DDX_CHECK(IDC_WINDOWSTARTMAXIMIZED, m_bWindowStartMaximized)
		DDX_CHECK(IDC_WINDOWVISIBLE, m_bWindowVisible)
		DDX_TEXT(IDC_WIZARDIMAGEFILE, m_strWizardImageFile)
		DDX_CBINDEX(IDC_BACKCOLORDIRECTION, m_nBackColorDirection)
		DDX_TEXT(IDC_EXT_WIZARDSMALLIMAGEFILE, m_strWizardSmallImageFile)
		DDX_TEXT(IDC_SETUPICONFILE, m_strSetupIconFile)
		DDX_CHECK(IDC_WIZARDIMAGESTRETCH, m_bWizardImageStretch)
	END_DDX_MAP()

	Henden::CButtonFile		m_btnWizardImageFile;
	Henden::CButtonFile		m_wndWizardSmallImageFile;
	Henden::CButtonFile		m_wndSetupIconFile;
	CColorPickerCB	m_wndBackColor2;
	CColorPickerCB	m_wndWizardImageBackColor;
	CColorPickerCB	m_wndBackColor;
	BOOL			m_bBackSolid;
	BOOL			m_bWindowResizable;
	BOOL			m_bWindowShowCaption;
	BOOL			m_bWindowStartMaximized;
	BOOL			m_bWindowVisible;
	CString			m_strWizardImageFile;
	int				m_nBackColorDirection;
	CString			m_strWizardSmallImageFile;
	CString			m_strSetupIconFile;
	BOOL			m_bWizardImageStretch;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"Appearance");
		m_wndBackColor2.SubclassWindow(GetDlgItem(IDC_BACKCOLOR2));
		m_wndWizardImageBackColor.SubclassWindow(GetDlgItem(IDC_WIZARDIMAGEBACKCOLOR));
		m_wndBackColor.SubclassWindow(GetDlgItem(IDC_BACKCOLOR));
		m_wndSetupIconFile.SubclassWindow(GetDlgItem(IDC_SETUPICONFILE_B));

		CInnoScriptEx& script = m_pDoc->GetScript();
		m_wndBackColor.SetInnoColor(script.GetPropertyString("BackColor"));
		m_wndBackColor2.SetInnoColor(script.GetPropertyString("BackColor2"));
		m_wndWizardImageBackColor.SetInnoColor(script.GetPropertyString("WizardImageBackColor"));

		CComboBox box(GetDlgItem(IDC_BACKCOLORDIRECTION));
		box.AddString(_L("Appearance|BackColorDirection|TopBottom","Top to bottom"));
		box.AddString(_L("Appearance|BackColorDirection|LeftRight","Left to right"));

		EnableControls();

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		CInnoScriptEx& script = m_pDoc->GetScript();

		CString strTmp;

		script.SetPropertyBool("BackSolid",m_bBackSolid ? true : false);
		script.SetPropertyBool("WindowShowCaption",m_bWindowShowCaption ? true : false);
		script.SetPropertyBool("WindowStartMaximized",m_bWindowStartMaximized ? true : false);
		script.SetPropertyBool("WindowResizable",m_bWindowResizable ? true : false);
		script.SetPropertyBool("WindowVisible",m_bWindowVisible ? true : false);
		script.SetPropertyString("WizardImageFile",m_strWizardImageFile);
		script.SetPropertyString("BackColorDirection",script.GetBackColorDirection(m_nBackColorDirection));

		m_wndBackColor.GetInnoColor(strTmp);
		script.SetPropertyString("BackColor",strTmp);
		
		m_wndBackColor2.GetInnoColor(strTmp);
		script.SetPropertyString("BackColor2",strTmp);
		
		m_wndWizardImageBackColor.GetInnoColor(strTmp);
		script.SetPropertyString("WizardImageBackColor",strTmp);

		// Inno Setup 2.0.0
		script.SetPropertyString("WizardSmallImageFile",m_strWizardSmallImageFile);
		// Inno Setup 4.1.0
		script.SetPropertyString("SetupIconFile",m_strSetupIconFile);
		// Inno Script 4.1.3
		script.SetPropertyBool("WizardImageStretch",m_bWizardImageStretch ? true : false);
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnWindowVisible(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		DoDataExchange(DDX_SAVE,IDC_WINDOWVISIBLE);
		EnableControls();
		return 0;
	}

	void EnableControls() {
		GetDlgItem(IDC_WINDOWSHOWCAPTION).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_WINDOWSTARTMAXIMIZED).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_WINDOWRESIZABLE).EnableWindow(m_bWindowVisible);

		GetDlgItem(IDC_STATIC2).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_STATIC3).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_STATIC4).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_BACKCOLOR).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_BACKCOLOR2).EnableWindow(m_bWindowVisible);
		GetDlgItem(IDC_BACKCOLORDIRECTION).EnableWindow(m_bWindowVisible);
	}

	CSetupAppearance(CMyDoc* pDoc,const bool bNew,LPCTSTR pszTitle) : 
		m_pDoc(pDoc), 
		m_bNew(bNew),
		m_wndWizardSmallImageFile(true,"Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
		m_btnWizardImageFile(true,"Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"),
		m_wndSetupIconFile(true,"Icon Files (*.ico)|*.ico|All Files (*.*)|*.*||"),
		CPropertyPageImpl<CSetupAppearance>(pszTitle)
	{
		CInnoScriptEx& script = m_pDoc->GetScript();

		m_bBackSolid				= script.GetPropertyBool("BackSolid");
		m_bWindowShowCaption		= script.GetPropertyBool("WindowShowCaption");
		m_bWindowStartMaximized		= script.GetPropertyBool("WindowStartMaximized");
		m_bWindowResizable			= script.GetPropertyBool("WindowResizable");
		m_bWindowVisible			= script.GetPropertyBool("WindowVisible");
		m_strWizardImageFile		= script.GetPropertyString("WizardImageFile");
		m_nBackColorDirection		= script.GetBackColorDirection(script.GetPropertyString("BackColorDirection"));
		// Inno Setup 2.0.0
		m_strWizardSmallImageFile	= script.GetPropertyString("WizardSmallImageFile");
		// Inno Setup 4.1.0
		m_strSetupIconFile			= script.GetPropertyString("SetupIconFile");
		// Inno Setup 4.1.3
		m_bWizardImageStretch		= script.GetPropertyBool("WizardImageStretch");
	}

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_BTN_WIZARD_IMAGE_FILE, _L("Help|Appearance|WizardImageButton","Click this button to select wizard image file."))
		TOOLTIP_HANDLER(IDC_EXT_WIZARDSMALLIMAGEFILE2, _L("Help|Appearance|WizardSmallImageButton","Click this button to select the small wizard image file."))
		TOOLTIP_HANDLER(IDC_BACKSOLID, _L("Help|Appearance|BackSolid","This specifies whether to use a solid or gradient background on the setup window. If this is yes, the background is a solid color (the color specified by BackColor; BackColor2 is ignored)."))
		TOOLTIP_HANDLER(IDC_WINDOWRESIZABLE, _L("Help|Appearance|WindowResizable","If set to no, the user won't be able to resize the Setup program's background window when it's not maximized."))
		TOOLTIP_HANDLER(IDC_WINDOWSHOWCAPTION, _L("Help|Appearance|WindowShowCaption","If set to no, Setup will be truly \"full screen\" -- it won't have a caption bar or border, and it will be on top of the taskbar."))
		TOOLTIP_HANDLER(IDC_WINDOWSTARTMAXIMIZED, _L("Help|Appearance|WindowStartMaximized","If set to yes, the Setup program's background window will initially be displayed in a maximized state, where it won't cover over the taskbar."))
		TOOLTIP_HANDLER(IDC_WINDOWVISIBLE, _L("Help|Appearance|WindowVisible","If set to yes, there will be a gradient background window displayed behind the wizard."))
		TOOLTIP_HANDLER(IDC_WIZARDIMAGEFILE, _L("Help|Appearance|WizardImageFile","Specifies the name of the bitmap file to display on the left side of the wizard in the Setup program."))
		TOOLTIP_HANDLER(IDC_BACKCOLORDIRECTION, _L("Help|Appearance|BackColorDirection","This determines the direction of the gradient background on the setup window."))
		TOOLTIP_HANDLER(IDC_EXT_WIZARDSMALLIMAGEFILE, _L("Help|Appearance|WizardSmallImageFile","Specifies the name of the bitmap file to display in the upper right corner of the wizard window."))
		TOOLTIP_HANDLER(IDC_SETUPICONFILE, _L("Help|Appearance|SetupIconFile","Specifies a new icon for the Setup program."))
		TOOLTIP_HANDLER(IDC_WIZARDIMAGESTRETCH, _L("Help|Appearance|WizardImageStretch","If set to yes, the default, the wizard images will be stretched or shrunk if the wizard is larger or smaller than normal, e.g. if the user is running in Large Fonts.\r\n\r\nIf set to no, the wizard images will be centered in their respective areas if the wizard is larger than normal, and clipped if the wizard is smaller than normal."))
		TOOLTIP_HANDLER(IDC_BACKCOLOR,_L("Help|Appearance|BackColor","The BackColor directive specifies the color to use at the top of the setup window's gradient background."))
		TOOLTIP_HANDLER(IDC_BACKCOLOR2,_L("Help|Appearance|BackColor2","BackColor2 specifies the color to use at the bottom."))
		TOOLTIP_HANDLER(IDC_WIZARDIMAGEBACKCOLOR,_L("Help|Appearance|WizardImageBackColor","This directive specifies the background color used to fill any unused space around the wizard bitmap."))
		TOOLTIP_HANDLER(IDC_SETUPICONFILE_B,_L("Help|Appearance|SetupIconFileButton","Click to select an icon for the setup application."))
	END_TOOLTIP_MAP()

protected:
	CMyDoc*		m_pDoc;
	const bool	m_bNew;
};

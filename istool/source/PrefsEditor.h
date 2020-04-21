// PrefsEditor.h

#pragma once

#include "Scintilla/ScintillaView.h"
#include "ColorButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPrefsEditor dialog

class CPrefsEditor : 
	public CPropertyPageImpl<CPrefsEditor>,
	public CMyPropertyPageBase<CPrefsEditor>,
	public CWinDataExchange<CPrefsEditor>,
	public CDialogResize<CPrefsEditor>
{
public:
	enum { IDD = IDD_PREFS_EDITOR };

	BEGIN_DLGRESIZE_MAP(CPrefsEditor)
		DLGRESIZE_CONTROL(IDC_FONTLIST, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FONTSIZE, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_FONTSIZESPIN, DLSZ_MOVE_X)
		DLGRESIZE_CONTROL(IDC_STYLE, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SCHEME, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_BOLD, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_ITALIC, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_UNDERLINE, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STATIC2, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CPrefsEditor)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPrefsEditor>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPrefsEditor>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_SCHEME,CBN_SELCHANGE,OnSchemeSelChange)
		COMMAND_HANDLER(IDC_STYLES,LBN_SELCHANGE,OnStyleSelChange)
		NOTIFY_HANDLER(IDC_FORE,CPN_SELCHANGE,OnSaveStyle)
		NOTIFY_HANDLER(IDC_BACK,CPN_SELCHANGE,OnSaveStyle)
		COMMAND_HANDLER(IDC_BOLD,BN_CLICKED,OnSaveStyle)
		COMMAND_HANDLER(IDC_ITALIC,BN_CLICKED,OnSaveStyle)
		COMMAND_HANDLER(IDC_UNDERLINE,BN_CLICKED,OnSaveStyle)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPrefsEditor>)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		CWaitCursor wait;
		DlgResize_Init(false,false,0);

		m_wndFont.SubclassWindow(GetDlgItem(IDC_FONTLIST));
		m_wndFont.Init();
		m_wndFontSizeSpin.Attach(GetDlgItem(IDC_FONTSIZESPIN));
		m_wndFontSizeSpin.SetRange(6,32);
		m_wndFont.SelectString(0,CMyApp::m_prefs.m_strFontName);

		m_wndFore.SubclassWindow(GetDlgItem(IDC_FORE));
		m_wndBack.SubclassWindow(GetDlgItem(IDC_BACK));

		m_wndScheme.Attach(GetDlgItem(IDC_SCHEME));
		for(int i=0;i<2;i++) {
			int iItem = m_wndScheme.AddString(CScintillaPrefs::m_schemes[i].pszName);
			m_wndScheme.SetItemData(iItem,CScintillaPrefs::m_schemes[i].uSchemeID);
		}

		m_wndStyles.Attach(GetDlgItem(IDC_STYLES));

		_L(m_hWnd,"PrefsEditor");

		DoDataExchange(DDX_LOAD);
		EnableControls();
		return TRUE;
	}

	BEGIN_DDX_MAP(CPrefsEditor)
		DDX_INT(IDC_FONTSIZE, m_nFontSize)
		DDX_INT(IDC_TABSTOPVALUE, m_nTabStopValue)
		DDX_CHECK(IDC_SHOWVERTICALTABLINES, m_bShowVerticalTabLines)
		DDX_CHECK(IDC_AUTOINDENT, m_bAutoIndent)
		DDX_CHECK(IDC_SHOWLINENUMBERS, m_bShowLineNumbers)
	END_DDX_MAP()
	
	kFontCombo		m_wndFont;
	CUpDownCtrl		m_wndFontSizeSpin;
	long			m_nFontSize, m_nTabStopValue;
	bool			m_bShowVerticalTabLines;
	bool			m_bAutoIndent;
	bool			m_bShowLineNumbers;
	CComboBox		m_wndScheme;
	CListBox		m_wndStyles;
	CColorButton	m_wndFore, m_wndBack;

	CPrefsEditor(CMyDoc* pDoc,LPCTSTR pszTitle) : 
		m_pDoc(pDoc),
		CPropertyPageImpl<CPrefsEditor>(pszTitle)
	{
		m_nFontSize				= CMyApp::m_prefs.m_nFontHeight;
		m_nTabStopValue			= CMyApp::m_prefs.m_nTabStopValue;
		m_bShowVerticalTabLines	= CMyApp::m_prefs.m_bShowVerticalTabLines;
		m_bAutoIndent			= CMyApp::m_prefs.m_bAutoIndent;
		m_bShowLineNumbers		= CMyApp::m_prefs.m_bShowLineNumbers;

		memcpy(m_styles,CScintillaPrefs::m_styles,sizeof m_styles);
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetModified();
		return 0;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		m_wndFont.GetWindowText(CMyApp::m_prefs.m_strFontName);
		CMyApp::m_prefs.m_nFontHeight			= m_nFontSize;
		CMyApp::m_prefs.m_nTabStopValue			= m_nTabStopValue;
		CMyApp::m_prefs.m_bShowVerticalTabLines	= m_bShowVerticalTabLines;
		CMyApp::m_prefs.m_bAutoIndent			= m_bAutoIndent;
		CMyApp::m_prefs.m_bShowLineNumbers		= m_bShowLineNumbers;
		memcpy(CScintillaPrefs::m_styles,m_styles,sizeof m_styles);
		return PSNRET_NOERROR;
	}

	LRESULT OnSchemeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		m_wndStyles.ResetContent();
		UINT uSchemeID = m_wndScheme.GetItemData(m_wndScheme.GetCurSel());
		for(int i=0;i<sizeof(m_styles)/sizeof(m_styles[0]);i++) {
			if(m_styles[i].uSchemeID==uSchemeID) {
				int iItem = m_wndStyles.AddString(m_styles[i].pszName);
				m_wndStyles.SetItemData(iItem,m_styles[i].uStyle);
			}
		}
		EnableControls();
		return 0;
	}

	LRESULT OnStyleSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		EnableControls();
		int uStyle = FindCurrentStyle();
		if(uStyle<0) return 0;

		CButton(GetDlgItem(IDC_BOLD)).SetCheck(m_styles[uStyle].bBold);
		CButton(GetDlgItem(IDC_ITALIC)).SetCheck(m_styles[uStyle].bItalic);
		CButton(GetDlgItem(IDC_UNDERLINE)).SetCheck(m_styles[uStyle].bUnderline);
		m_wndFore.SetColor(m_styles[uStyle].cFore);
		m_wndBack.SetColor(m_styles[uStyle].cBack);
		return 0;
	}

	LRESULT OnSaveStyle(int,NMHDR* pNMHDR,BOOL&) {
		//NMCOLORBUTTON* pcb = (NMCOLORBUTTON*)pNMHDR;
		SaveStyle();
		return 0;
	}

	LRESULT OnSaveStyle(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SaveStyle();
		return 0;
	}
protected:
	CMyDoc*	m_pDoc;

	void EnableControls() {
		bool bStyles = m_wndScheme.GetCurSel()>=0;
		bool bControls = bStyles && m_wndStyles.GetCurSel()>=0;
		GetDlgItem(IDC_STYLES).EnableWindow(bStyles);
		GetDlgItem(IDC_STATIC1).EnableWindow(bControls);
		GetDlgItem(IDC_FORE).EnableWindow(bControls);
		GetDlgItem(IDC_STATIC2).EnableWindow(bControls);
		GetDlgItem(IDC_BACK).EnableWindow(bControls);
		GetDlgItem(IDC_BOLD).EnableWindow(bControls);
		GetDlgItem(IDC_ITALIC).EnableWindow(bControls);
		GetDlgItem(IDC_UNDERLINE).EnableWindow(bControls);
	}

	int FindCurrentStyle() {
		if(m_wndScheme.GetCurSel()<0)
			return -1;
		if(m_wndStyles.GetCurSel()<0)
			return -1;

		UINT uSchemeID = m_wndScheme.GetItemData(m_wndScheme.GetCurSel());
		UINT uStyle = m_wndStyles.GetItemData(m_wndStyles.GetCurSel());

		for(int i=0;i<sizeof m_styles/sizeof m_styles[0];i++) {
			if(m_styles[i].uSchemeID==uSchemeID && m_styles[i].uStyle==uStyle)
				return i;
		}
		return -1;
	}

	void SaveStyle() {
		int uStyle = FindCurrentStyle();
		if(uStyle<0) return;

		tagSTYLE& style = m_styles[uStyle];
		style.cFore = m_wndFore.GetColor();
		style.cBack = m_wndBack.GetColor();
		style.bBold = CButton(GetDlgItem(IDC_BOLD)).GetCheck()!=0;
		style.bItalic = CButton(GetDlgItem(IDC_ITALIC)).GetCheck()!=0;
		style.bUnderline = CButton(GetDlgItem(IDC_UNDERLINE)).GetCheck()!=0;
	}

	tagSTYLE m_styles[CScintillaPrefs::NUMSTYLES];
};

// WTL - remember OnwerDraw FIXED in dialog font combo boxes
#pragma once

#include "MyDoc.h"
#include "FontCombo.h"

static const struct {
	UINT	m_nLanguageID;
	LPCTSTR	m_pszLanguageName;
} m_langInfo[] = {
	0x0436, "Afrikaans",
	0x041c, "Albanian",
	0x0401, "Arabic (Saudi Arabia)",
	0x0801, "Arabic (Iraq)",
	0x0c01, "Arabic (Egypt)",
	0x1001, "Arabic (Libya)",
	0x1401, "Arabic (Algeria)",
	0x1801, "Arabic (Morocco)",
	0x1c01, "Arabic (Tunisia)",
	0x2001, "Arabic (Oman)",
	0x2401, "Arabic (Yemen)",
	0x2801, "Arabic (Syria)",
	0x2c01, "Arabic (Jordan)",
	0x3001, "Arabic (Lebanon)",
	0x3401, "Arabic (Kuwait)",
	0x3801, "Arabic (U.A.E.)",
	0x3c01, "Arabic (Bahrain)",
	0x4001, "Arabic (Qatar)",
	0x042b, "Armenian",
	0x044d, "Assamese",
	0x042c, "Azeri (Latin)",
	0x082c, "Azeri (Cyrillic)",
	0x042d, "Basque",
	0x0423, "Belarussian",
	0x0445, "Bengali",
	0x0402, "Bulgarian",
	0x0455, "Burmese",
	0x0403, "Catalan",
	0x0404, "Chinese (Taiwan)",
	0x0804, "Chinese (PRC)",
	0x0c04, "Chinese (Hong Kong SAR, PRC)",
	0x1004, "Chinese (Singapore)",
	0x1404, "Chinese (Macau SAR)",
	0x041a, "Croatian",
	0x0405, "Czech",
	0x0406, "Danish",
	0x0413, "Dutch (Netherlands)",
	0x0813, "Dutch (Belgium)",
	0x0409, "English (United States)",
	0x0809, "English (United Kingdom)",
	0x0c09, "English (Australian)",
	0x1009, "English (Canadian)",
	0x1409, "English (New Zealand)",
	0x1809, "English (Ireland)",
	0x1c09, "English (South Africa)",
	0x2009, "English (Jamaica)",
	0x2409, "English (Caribbean)",
	0x2809, "English (Belize)",
	0x2c09, "English (Trinidad)",
	0x3009, "English (Zimbabwe)",
	0x3409, "English (Philippines)",
	0x0425, "Estonian",
	0x0438, "Faeroese",
	0x0429, "Farsi",
	0x040b, "Finnish",
	0x040c, "French (Standard)",
	0x080c, "French (Belgian)",
	0x0c0c, "French (Canadian)",
	0x100c, "French (Switzerland)",
	0x140c, "French (Luxembourg)",
	0x180c, "French (Monaco)",
	0x0437, "Georgian",
	0x0407, "German (Standard)",
	0x0807, "German (Switzerland)",
	0x0c07, "German (Austria)",
	0x1007, "German (Luxembourg)",
	0x1407, "German (Liechtenstein)",
	0x0408, "Greek",
	0x0447, "Gujarati",
	0x040d, "Hebrew",
	0x0439, "Hindi",
	0x040e, "Hungarian",
	0x040f, "Icelandic",
	0x0421, "Indonesian",
	0x0410, "Italian (Standard)",
	0x0810, "Italian (Switzerland)",
	0x0411, "Japanese",
	0x044b, "Kannada",
	0x0860, "Kashmiri (India)",
	0x043f, "Kazakh",
	0x0457, "Konkani",
	0x0412, "Korean",
	0x0812, "Korean (Johab)",
	0x0426, "Latvian",
	0x0427, "Lithuanian",
	0x0827, "Lithuanian (Classic)",
	0x042f, "Macedonian",
	0x043e, "Malay (Malaysian)",
	0x083e, "Malay (Brunei Darussalam)",
	0x044c, "Malayalam",
	0x0458, "Manipuri",
	0x044e, "Marathi",
	0x0861, "Nepali (India)",
	0x0414, "Norwegian (Bokmal)",
	0x0814, "Norwegian (Nynorsk)",
	0x0448, "Oriya",
	0x0415, "Polish",
	0x0416, "Portuguese (Brazil)",
	0x0816, "Portuguese (Standard)",
	0x0446, "Punjabi",
	0x0418, "Romanian",
	0x0419, "Russian",
	0x044f, "Sanskrit",
	0x0c1a, "Serbian (Cyrillic)",
	0x081a, "Serbian (Latin)",
	0x0459, "Sindhi",
	0x041b, "Slovak",
	0x0424, "Slovenian",
	0x040a, "Spanish (Traditional Sort)",
	0x080a, "Spanish (Mexican)",
	0x0c0a, "Spanish (Modern Sort)",
	0x100a, "Spanish (Guatemala)",
	0x140a, "Spanish (Costa Rica)",
	0x180a, "Spanish (Panama)",
	0x1c0a, "Spanish (Dominican Republic)",
	0x200a, "Spanish (Venezuela)",
	0x240a, "Spanish (Colombia)",
	0x280a, "Spanish (Peru)",
	0x2c0a, "Spanish (Argentina)",
	0x300a, "Spanish (Ecuador)",
	0x340a, "Spanish (Chile)",
	0x380a, "Spanish (Uruguay)",
	0x3c0a, "Spanish (Paraguay)",
	0x400a, "Spanish (Bolivia)",
	0x440a, "Spanish (El Salvador)",
	0x480a, "Spanish (Honduras)",
	0x4c0a, "Spanish (Nicaragua)",
	0x500a, "Spanish (Puerto Rico)",
	0x0430, "Sutu",
	0x0441, "Swahili (Kenya)",
	0x041d, "Swedish",
	0x081d, "Swedish (Finland)",
	0x0449, "Tamil",
	0x0444, "Tatar (Tatarstan)",
	0x044a, "Telugu",
	0x041e, "Thai",
	0x041f, "Turkish",
	0x0422, "Ukrainian",
	0x0420, "Urdu (Pakistan)",
	0x0820, "Urdu (India)",
	0x0443, "Uzbek (Latin)",
	0x0843, "Uzbek (Cyrillic)",
	0x042a, "Vietnamese",
	0x0000,	NULL
};

/////////////////////////////////////////////////////////////////////////////
// CDlgLangOptions dialog

class CDlgLangOptions : 
	public CDialogImpl<CDlgLangOptions>, 
	public CWinDataExchange<CDlgLangOptions>,
	public CMyDialogBase<CDlgLangOptions>
{
public:
	enum { IDD = IDD_LANGOPTIONS };

	BEGIN_MSG_MAP(CDlgLangOptions)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_LANGOPTIONS_LANGUAGENAME,CBN_SELCHANGE,OnSelChangeLanguageName)
		COMMAND_HANDLER(IDC_LANGOPTIONS_LANGUAGEID,CBN_SELCHANGE,OnSelChangeLanguageID)
		CHAIN_MSG_MAP(CMyDialogBase<CDlgLangOptions>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CDlgLangOptions)
		DDX_CBINDEX(IDC_LANGOPTIONS_LANGUAGENAME, m_nLanguageName)
		DDX_CBINDEX(IDC_LANGOPTIONS_LANGUAGEID, m_nLanguageID)
		DDX_CBINDEX(IDC_LANGOPTIONS_TITLEFONTNAME, m_nTitleFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_WELCOMEFONTNAME, m_nWelcomeFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_COPYRIGHTFONTNAME, m_nCopyrightFontName)
		DDX_CBINDEX(IDC_LANGOPTIONS_DIALOGFONTNAME, m_nDialogFontName)
		DDX_INT(IDC_LANGOPTIONS_TITLEFONTSIZE, m_nTitleFontSize)
		DDX_INT(IDC_LANGOPTIONS_DIALOGFONTSIZE, m_nDialogFontSize)
		DDX_INT(IDC_LANGOPTIONS_WELCOMEFONTSIZE, m_nWelcomeFontSize)
		DDX_INT(IDC_LANGOPTIONS_COPYRIGHTFONTSIZE, m_nCopyrightFontSize)
		DDX_INT(IDC_LANGOPTIONS_DIALOGFONTSTANDARDHEIGHT, m_nDialogFontStandardHeight)
		DDX_CHECK(IDC_RIGHTTOLEFT, m_bRightToLeft)
	END_DDX_MAP()

	CUpDownCtrl	m_wndSpin5;
	CUpDownCtrl	m_wndSpin4;
	CUpDownCtrl	m_wndSpin3;
	CUpDownCtrl	m_wndSpin2;
	CUpDownCtrl	m_wndSpin1;
	CFontCombo	m_wndCopyrightFontName;
	CFontCombo	m_wndWelcomeFontName;
	CFontCombo	m_wndTitleFontName;
	CFontCombo	m_wndDialogFontName;
	CComboBox	m_wndLanguageID;
	CComboBox	m_wndLanguageName;
	int		m_nLanguageName;
	int		m_nLanguageID;
	int		m_nTitleFontName;
	int		m_nWelcomeFontName;
	int		m_nCopyrightFontName;
	int		m_nDialogFontName;
	UINT	m_nTitleFontSize;
	UINT	m_nDialogFontSize;
	UINT	m_nWelcomeFontSize;
	UINT	m_nCopyrightFontSize;
	UINT	m_nDialogFontStandardHeight;
	BOOL	m_bRightToLeft;

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(wID==IDOK) {
			DoDataExchange(DDX_SAVE);
			CInnoScriptEx& script = m_pDoc->GetScript();
			CString		strLanguageName = m_langInfo[m_nLanguageName].m_pszLanguageName;
			CString		strLanguageID; strLanguageID.Format("$%04X",m_langInfo[m_nLanguageName].m_nLanguageID);

			int n = strLanguageName.Find('(');
			if(n>=0) {
				strLanguageName.GetBufferSetLength(n);
				strLanguageName.TrimRight();
			}

			CString str;
			
			script.SetPropertyString("LanguageName",strLanguageName,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyString("LanguageID",strLanguageID,CInnoScript::SEC_LANGOPTIONS);
			m_wndDialogFontName.GetWindowText(str.GetBuffer(255),256); str.ReleaseBuffer();
			script.SetPropertyString("DialogFontName",str,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber("DialogFontSize",m_nDialogFontSize,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber("DialogFontStandardHeight",m_nDialogFontStandardHeight,CInnoScript::SEC_LANGOPTIONS);
			m_wndTitleFontName.GetWindowText(str.GetBuffer(255),256); str.ReleaseBuffer();
			script.SetPropertyString("TitleFontName",str,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber("TitleFontSize",m_nTitleFontSize,CInnoScript::SEC_LANGOPTIONS);
			m_wndWelcomeFontName.GetWindowText(str.GetBuffer(255),256); str.ReleaseBuffer();
			script.SetPropertyString("WelcomeFontName",str,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber("WelcomeFontSize",m_nWelcomeFontSize,CInnoScript::SEC_LANGOPTIONS);
			m_wndCopyrightFontName.GetWindowText(str.GetBuffer(255),256); str.ReleaseBuffer();
			script.SetPropertyString("CopyrightFontName",str,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyNumber("CopyrightFontSize",m_nCopyrightFontSize,CInnoScript::SEC_LANGOPTIONS);
			script.SetPropertyBool("RightToLeft",m_bRightToLeft ? true : false,CInnoScript::SEC_LANGOPTIONS);
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		_L(m_hWnd,"LangOptions");
		CenterWindow(GetParent());

		m_wndSpin1.Attach(GetDlgItem(IDC_SPIN1));
		m_wndSpin2.Attach(GetDlgItem(IDC_SPIN2));
		m_wndSpin3.Attach(GetDlgItem(IDC_SPIN3));
		m_wndSpin4.Attach(GetDlgItem(IDC_SPIN4));
		m_wndSpin5.Attach(GetDlgItem(IDC_SPIN5));
		m_wndCopyrightFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_COPYRIGHTFONTNAME));
		m_wndWelcomeFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_WELCOMEFONTNAME));
		m_wndTitleFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_TITLEFONTNAME));
		m_wndDialogFontName.SubclassWindow(GetDlgItem(IDC_LANGOPTIONS_DIALOGFONTNAME));
		m_wndLanguageID.Attach(GetDlgItem(IDC_LANGOPTIONS_LANGUAGEID));
		m_wndLanguageName.Attach(GetDlgItem(IDC_LANGOPTIONS_LANGUAGENAME));

		m_nLanguageName = -1;
		m_nLanguageID = -1;
		m_nTitleFontName = -1;
		m_nWelcomeFontName = -1;
		m_nCopyrightFontName = -1;
		m_nDialogFontName = -1;

		CInnoScriptEx& script = m_pDoc->GetScript();
		m_nDialogFontSize = script.GetPropertyNumber("DialogFontSize",CInnoScript::SEC_LANGOPTIONS);
		m_nTitleFontSize = script.GetPropertyNumber("TitleFontSize",CInnoScript::SEC_LANGOPTIONS);
		m_nWelcomeFontSize = script.GetPropertyNumber("WelcomeFontSize",CInnoScript::SEC_LANGOPTIONS);
		m_nCopyrightFontSize = script.GetPropertyNumber("CopyrightFontSize",CInnoScript::SEC_LANGOPTIONS);
		m_nDialogFontStandardHeight = script.GetPropertyNumber("DialogFontStandardHeight",CInnoScript::SEC_LANGOPTIONS);

		m_wndDialogFontName.Initialize();
		m_wndTitleFontName.Initialize();
		m_wndWelcomeFontName.Initialize();
		m_wndCopyrightFontName.Initialize();
		m_wndSpin1.SetRange32(6,128);
		m_wndSpin2.SetRange32(6,128);
		m_wndSpin3.SetRange32(6,128);
		m_wndSpin4.SetRange32(6,128);
		m_wndSpin5.SetRange32(6,128);

		CString strLanguageID = script.GetPropertyString("LanguageID",CInnoScript::SEC_LANGOPTIONS);
		UINT nLanguage = 0;
		while(m_langInfo[nLanguage].m_nLanguageID && m_langInfo[nLanguage].m_pszLanguageName) {
			m_wndLanguageName.AddString(m_langInfo[nLanguage].m_pszLanguageName);
			CString strID;
			strID.Format("$%04X",m_langInfo[nLanguage].m_nLanguageID);
			int nIndex = m_wndLanguageID.AddString(strID);
			if(!_stricmp(strLanguageID,strID)) {
				m_nLanguageName = nIndex;
				m_nLanguageID = nIndex;
			}
			nLanguage++;
		}

		m_nDialogFontName = m_wndDialogFontName.FindStringExact(0,script.GetPropertyString("DialogFontName",CInnoScript::SEC_LANGOPTIONS));
		m_wndDialogFontName.SetCurSel(m_nDialogFontName);
		m_nTitleFontName = m_wndTitleFontName.FindStringExact(0,script.GetPropertyString("TitleFontName",CInnoScript::SEC_LANGOPTIONS));
		m_wndTitleFontName.SetCurSel(m_nTitleFontName);
		m_nWelcomeFontName = m_wndWelcomeFontName.FindStringExact(0,script.GetPropertyString("WelcomeFontName",CInnoScript::SEC_LANGOPTIONS));
		m_wndWelcomeFontName.SetCurSel(m_nWelcomeFontName);
		m_nCopyrightFontName = m_wndCopyrightFontName.FindStringExact(0,script.GetPropertyString("CopyrightFontName",CInnoScript::SEC_LANGOPTIONS));
		m_wndCopyrightFontName.SetCurSel(m_nCopyrightFontName);

		m_nTitleFontSize = script.GetPropertyNumber("TitleFontSize",CInnoScript::SEC_LANGOPTIONS);
		m_bRightToLeft = script.GetPropertyBool("RightToLeft",CInnoScript::SEC_LANGOPTIONS);

		DoDataExchange(DDX_LOAD);
		return TRUE;
	}

	LRESULT OnSelChangeLanguageName(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		m_nLanguageID = m_nLanguageName;
		DoDataExchange(DDX_LOAD);
		return 0;
	}

	LRESULT OnSelChangeLanguageID(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		DoDataExchange(DDX_SAVE);
		m_nLanguageName = m_nLanguageID;
		DoDataExchange(DDX_LOAD);
		return 0;
	}

	CDlgLangOptions(CMyDoc* pDoc) : m_pDoc(pDoc) {}
protected:
	CMyDoc*	m_pDoc;
};

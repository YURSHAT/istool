// WTL
#pragma once

#include "MyDoc.h"
#include "ComboBox2.h"

/////////////////////////////////////////////////////////////////////////////
// CPageIni dialog

class CPageIni : 
	public CPropertyPageImpl<CPageIni>,
	public CMyPropertyPageBase<CPageIni>,
	public CWinDataExchange<CPageIni>,
	public CDialogResize<CPageIni>,
	public Henden::CToolTipDialog<CPageIni>
{
public:
	enum { IDD = IDD_INI };

	BEGIN_DLGRESIZE_MAP(CPageIni)
		DLGRESIZE_CONTROL(IDC_INI_FILENAME, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_SECTION, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_KEY, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_STRING, DLSZ_SIZE_X)
		DLGRESIZE_CONTROL(IDC_FLAGS, DLSZ_SIZE_X)
	END_DLGRESIZE_MAP()

	BEGIN_MSG_MAP(CPageIni)
		CHAIN_MSG_MAP(Henden::CToolTipDialog<CPageIni>)
		CHAIN_MSG_MAP(CMyPropertyPageBase<CPageIni>)
		CHAIN_MSG_MAP(CPropertyPageImpl<CPageIni>)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_HANDLER(IDC_CREATEKEYIFDOESNTEXIST, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETEENTRY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETESECTION, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_UNINSDELETESECTIONIFEMPTY, BN_CLICKED, OnModified)
		COMMAND_HANDLER(IDC_KEY, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_STRING, EN_CHANGE, OnModified)
		COMMAND_HANDLER(IDC_INI_FILENAME, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_INI_FILENAME, CBN_SELCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SECTION, CBN_EDITCHANGE, OnModified)
		COMMAND_HANDLER(IDC_SECTION, CBN_SELCHANGE, OnModified)
	//	REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CDialogResize<CPageIni>)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CPageIni)
		DDX_TEXT(IDC_INI_FILENAME, m_strFilename)
		DDX_TEXT(IDC_SECTION, m_strSection)
		DDX_TEXT(IDC_KEY, m_strKey)
		DDX_TEXT(IDC_STRING, m_strString)
		DDX_CHECK(IDC_CREATEKEYIFDOESNTEXIST, m_nCreateKeyIfDoesntExist)
		DDX_CHECK(IDC_UNINSDELETEENTRY, m_nUninsDeleteEntry)
		DDX_CHECK(IDC_UNINSDELETESECTION, m_nUninsDeleteSection)
		DDX_CHECK(IDC_UNINSDELETESECTIONIFEMPTY, m_nUninsDeleteSectionIfEmpty)
	END_DDX_MAP()

	CEdit2	m_wndString;
	CEdit2	m_wndKey;
	CComboBox2	m_wndSection;
	CComboBox2	m_wndFilename;
	CString	m_strFilename;
	CString	m_strSection;
	CString	m_strKey;
	CString	m_strString;
	int		m_nCreateKeyIfDoesntExist;
	int		m_nUninsDeleteEntry;
	int		m_nUninsDeleteSection;
	int		m_nUninsDeleteSectionIfEmpty;

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		DlgResize_Init(false,false,0);
		_L(m_hWnd,"INI");
		m_wndString.SubclassWindow(GetDlgItem(IDC_STRING));
		m_wndKey.SubclassWindow(GetDlgItem(IDC_KEY));
		m_wndSection.SubclassWindow(GetDlgItem(IDC_SECTION));
		m_wndFilename.SubclassWindow(GetDlgItem(IDC_INI_FILENAME));

		for(int nPos=0;nPos<m_listIni.GetSize();nPos++) {
			CScriptLine* pLine = m_listIni[nPos];

			if(m_strFilename.CompareNoCase(SAFESTR(pLine->GetParameter("Filename"))))
				m_strFilename.Empty();
			if(m_strSection.CompareNoCase(SAFESTR(pLine->GetParameter("Section"))))
				m_strSection.Empty();
			if(m_strKey.CompareNoCase(SAFESTR(pLine->GetParameter("Key"))))
				m_strKey.Empty();
			if(m_strString.CompareNoCase(SAFESTR(pLine->GetParameter("String"))))
				m_strString.Empty();

			//Flags
			if(m_nCreateKeyIfDoesntExist != (pLine->GetParameterFlag("Flags","createkeyifdoesntexist") ? TRUE : FALSE)) {
				GetDlgItem(IDC_CREATEKEYIFDOESNTEXIST).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nCreateKeyIfDoesntExist = 2;
			}

			if(m_nUninsDeleteEntry != (pLine->GetParameterFlag("Flags","uninsdeleteentry") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETEENTRY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteEntry = 2;
			}

			if(m_nUninsDeleteSection != (pLine->GetParameterFlag("Flags","uninsdeletesection") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETESECTION).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteSection = 2;
			}

			if(m_nUninsDeleteSectionIfEmpty != (pLine->GetParameterFlag("Flags","uninsdeletesectionifempty") ? TRUE : FALSE)) {
				GetDlgItem(IDC_UNINSDELETESECTIONIFEMPTY).ModifyStyle(BS_CHECKBOX|BS_3STATE|BS_AUTO3STATE,BS_AUTO3STATE);
				m_nUninsDeleteSectionIfEmpty = 2;
			}
		}
		DoDataExchange(DDX_LOAD);

		m_pDoc->AddIniFilenames(m_wndFilename);
		m_pDoc->AddIniSections(m_wndSection);
		return TRUE;
	}

	LRESULT OnApply() {
		DoDataExchange(DDX_SAVE);
		const bool bForce = m_listIni.GetSize()==1;

		if(bForce) {
			if(m_strFilename.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Filename is required."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
			if(m_strSection.IsEmpty()) {
				AtlMessageBox(m_hWnd,_L("Error|INI|NeedSection","Missing required parameter section."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
				return PSNRET_INVALID;
			}
		}

		for(int nPos=0;nPos<m_listIni.GetSize();nPos++) {
			CScriptLine* pLine = m_listIni[nPos];

			CInnoScriptEx::SetString(pLine,bForce,"Filename",m_strFilename);

			CInnoScriptEx::SetString(pLine,bForce,"Section",m_strSection);
			CInnoScriptEx::SetString(pLine,bForce,"Key",m_strKey);
			CInnoScriptEx::SetString(pLine,bForce,"String",m_strString);

			// Flags
			CInnoScriptEx::SetFlag(pLine,"Flags","createkeyifdoesntexist",m_nCreateKeyIfDoesntExist);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeleteentry",m_nUninsDeleteEntry);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeletesection",m_nUninsDeleteSection);
			CInnoScriptEx::SetFlag(pLine,"Flags","uninsdeletesectionifempty",m_nUninsDeleteSectionIfEmpty);
		}
		return PSNRET_NOERROR;
	}

	LRESULT OnModified(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		if(!m_bNew) SetModified();
		return 0;
	}

	CString m_strTitle;
	CPageIni::CPageIni(CScriptList& listIni,CMyDoc* pDoc,const bool bNew) : 
		m_listIni(listIni), m_pDoc(pDoc), m_bNew(bNew),
		m_wndFilename(pDoc), m_wndSection(pDoc), m_wndKey(pDoc), m_wndString(pDoc)
	{
		m_strTitle = _L("DialogTitles|INI","INI");
		SetTitle((LPCTSTR)m_strTitle);

		m_nCreateKeyIfDoesntExist = FALSE;
		m_nUninsDeleteEntry = FALSE;
		m_nUninsDeleteSection = FALSE;
		m_nUninsDeleteSectionIfEmpty = FALSE;
		for(int nPos=0;nPos<m_listIni.GetSize();nPos++) {
			CScriptLine* pLine = m_listIni[nPos];

			m_strFilename = pLine->GetParameter("Filename");
			m_strSection = pLine->GetParameter("Section");
			m_strKey = pLine->GetParameter("Key");
			m_strString = pLine->GetParameter("String");
			//Flags
			if(pLine->GetParameterFlag("Flags","createkeyifdoesntexist")) m_nCreateKeyIfDoesntExist = 1;
			if(pLine->GetParameterFlag("Flags","uninsdeleteentry")) m_nUninsDeleteEntry = 1;
			if(pLine->GetParameterFlag("Flags","uninsdeletesection")) m_nUninsDeleteSection = 1;
			if(pLine->GetParameterFlag("Flags","uninsdeletesectionifempty")) m_nUninsDeleteSectionIfEmpty = 1;
			break;
		}
	}

	CScriptList&	m_listIni;
	CMyDoc*		m_pDoc;
	const bool	m_bNew;

	BEGIN_TOOLTIP_MAP()
		TOOLTIP_HANDLER(IDC_INI_FILENAME,_L("Help|INI|FileName","The name of the .INI file you want Setup to modify, which can include constants. If this parameter is blank, it writes to WIN.INI in the system's Windows directory."))
		TOOLTIP_HANDLER(IDC_SECTION,_L("Help|INI|Section","The name of the section to create the entry in, which can include constants."))
		TOOLTIP_HANDLER(IDC_KEY,_L("Help|INI|Key","The name of the key to set, which can include constants. If this parameter is not specified or is blank, no key is created."))
		TOOLTIP_HANDLER(IDC_STRING,_L("Help|INI|String","The value to assign to the key, which can use constants. If this parameter is not specified, no key is created."))
		TOOLTIP_HANDLER(IDC_CREATEKEYIFDOESNTEXIST,_L("Help|INI|CreateKeyIfDoesntExist","Assign to the key only if the key name doesn't already exist."))
		TOOLTIP_HANDLER(IDC_UNINSDELETEENTRY,_L("Help|INI|UninsDeleteEntry","Delete the entry when the program is uninstalled. This can be combined with the uninsdeletesectionifempty flag."))
		TOOLTIP_HANDLER(IDC_UNINSDELETESECTION,_L("Help|INI|UninsDeleteSection","When the program is uninstalled, delete the entire section in which the entry is located. It obviously wouldn't be a good idea to use this on a section that is used by Windows itself (like some of the sections in WIN.INI). You should only use this on sections private to your application."))
		TOOLTIP_HANDLER(IDC_UNINSDELETESECTIONIFEMPTY,_L("Help|INI|UninsDeleteSectionIfEmpty","Same as uninsdeletesection, but deletes the section only if there are no keys left in it. This can be combined with the uninsdeleteentry flag."))
	END_TOOLTIP_MAP()
};

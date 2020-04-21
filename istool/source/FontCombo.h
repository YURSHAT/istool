#pragma once

#define TRUETYPE_FONT		0x0001
#define PRINTER_FONT		0x0002
#define DEVICE_FONT			0x0004

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         // 
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       // 
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      // 
//////////////////////////////////////////////////////////////////////////

class CFontObj {
protected:	
	DWORD	m_nFlags;	// Font flags
	CString	m_strName;
public:
	CFontObj(LPCTSTR pszName,DWORD nFlags) : m_strName(pszName) {	
		m_nFlags = nFlags;
	}

	CFontObj(CFontObj* pFontObj) {	
		m_nFlags = pFontObj->GetFlags();
	}

	int GetFlags() const { return m_nFlags; }
	LPCTSTR GetName() const { return m_strName; }
};

/////////////////////////////////////////////////////////////////////////////
// CFontCombo window

class CFontCombo : public CWindowImpl<CFontCombo,CComboBox> {
public:
	void Initialize() {
		// Set default font name
		CString strDefault = "";
		
		CFontObj* pFontObj;
		CString strKey,strComp;
		EnumerateFonts();

		for(int nPos=0;nPos<m_mapFonts.GetSize();nPos++) {
			//m_mapFonts.GetNextAssoc(pos,strKey,pFontObj);
			pFontObj = m_mapFonts[nPos];
			strKey = pFontObj->GetName();

			int nMax = GetCount();
			BOOL bInsert = FALSE;
			for (int nIdx=0;nIdx < nMax;nIdx++)
			{			
				GetLBText(nIdx,strComp);
				
				if (strComp.Collate(strKey) == 1)
				{
					bInsert = TRUE;
					InsertString(nIdx,strKey);
					break;
				}
			}		

			if (!bInsert)
				AddString(strKey);
		}

	}

protected:
	BOOL CFontCombo::EnumerateFonts() {
		HDC hDC;
		
		// Get screen fonts
		hDC = ::GetWindowDC(NULL);
		
		LOGFONT lf;
		
		ZeroMemory(&lf,sizeof(lf));
		lf.lfCharSet = ANSI_CHARSET;

		if (!EnumFontFamiliesEx(
				hDC,	// handle to device context
				&lf,	// pointer to logical font information
				(FONTENUMPROC)EnumFamScreenCallBackEx,	// pointer to callback function
				(LPARAM) this,	// application-supplied data
				(DWORD) 0))
			return FALSE;

		::ReleaseDC(NULL,hDC);	

		AddFont("MS Shell Dlg", 0);

		return TRUE; // All's ok
	}

	void AddFont(CString strName, DWORD dwFlags) {
		m_mapFonts.Add(new CFontObj(strName,dwFlags));
	}
	
	void SetCurrentFont() {
		CString strSelFont;

		// Change font in edit box is a known font
		// otherwise reject and restore orginal
		int nSel = GetCurSel();

		if (nSel == CB_ERR) {
			//GetWindowText(strSelFont);
			GetWindowText(strSelFont.GetBuffer(1024),1025);
			strSelFont.ReleaseBuffer();

			nSel = FindStringExact(-1,strSelFont);
			
			if (nSel == CB_ERR)
			{
				SetWindowText(m_strFontSave);
			}
		}
	}
public:
	static BOOL CALLBACK EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, 
		NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)

	{
		// don't put in non-printer raster fonts
		if (FontType & RASTER_FONTTYPE)
			return 1;
		
		DWORD dwData;
		
		dwData = (FontType & TRUETYPE_FONTTYPE) ? TRUETYPE_FONT : 0;
		((CFontCombo*)pThis)->AddFont(pelf->elfLogFont.lfFaceName, dwData);
		
		return 1; // Call me back
	}


protected:
	CString			m_strFontSave;
	CString			m_strDefault;

	CSimpleArray<CFontObj*>	m_mapFonts;
 
	BEGIN_MSG_MAP(CFontCombo)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_KILLFOCUS, OnKillfocus)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_SETFOCUS, OnSetfocus)
		REFLECTED_COMMAND_CODE_HANDLER(CBN_CLOSEUP, OnCloseUp)
	END_MSG_MAP()

	LRESULT OnKillfocus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetCurrentFont();
		return 0;
	}

	LRESULT OnSetfocus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		// Save off original font
		//GetWindowText(m_strFontSave);	
		GetWindowText(m_strFontSave.GetBuffer(1024),1025);
		m_strFontSave.ReleaseBuffer();
		return 0;
	}

	LRESULT OnCloseUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		int nSel;	

		CString strFont;
		// Set Face Name
		SetCurrentFont();
		nSel = GetCurSel();
		if (nSel != CB_ERR)
			GetLBText(nSel,strFont);
		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		Initialize();
		bHandled = FALSE;
		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		while(m_mapFonts.GetSize()) {
			delete m_mapFonts[0];
			m_mapFonts.RemoveAt(0);
		}

		bHandled = FALSE;
		return 0;
	}
};

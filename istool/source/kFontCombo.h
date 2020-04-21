/*
 Copyright (c) 2000 
 Author: Konstantin Boukreev 
 E-mail: konstantin@mail.primorye.ru 

 Created: 30.06.2000 14:14:21
 Version: 1.0.0

 Font selection combo

 Portions Copyright:
	Girish Bharadwaj (Girish_Bharadwaj@smtpnotes.pictel.com) - http://codeguru.earthweb.com/combobox/font_selection_combo.shtml
	Norm Almond of Paramax Technology Limited (nalmond@hotmail.com) - http://codeguru.earthweb.com/combobox/fontcombo.shtml
*/

#ifndef _kFontCombo_0690d7cc_49b3_4d9a_9390_978bc2839650
#define _kFontCombo_0690d7cc_49b3_4d9a_9390_978bc2839650

#if _MSC_VER > 1000 
#pragma once
#endif // _MSC_VER > 1000

//#include "kFlatCombo.h"

class kFontCombo : public CWindowImpl<kFontCombo,CComboBox>
{	
	friend class CPrefsEditor;
public:	
	kFontCombo() { m_cyItem = ITEM_DEFAULT_HEIGHT; }

	BEGIN_MSG_MAP(kFontCombo)
//		MESSAGE_HANDLER(WM_CREATE, OnCreate);
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy);
		MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem);
		MESSAGE_HANDLER(OCM_MEASUREITEM, OnMeasureItem);
		//CHAIN_MSG_MAP(CComboBox)		
	END_MSG_MAP()

	HWND CreateEx(HWND hWnd, RECT& rc, int cyItem = -1, int ID = 0)
	{
		m_cyItem = cyItem;
		return Create(hWnd, rc, 0, WS_CHILD|WS_VSCROLL|WS_TABSTOP|WS_VISIBLE|CBS_DROPDOWN|CBS_OWNERDRAWFIXED|CBS_HASSTRINGS | CBS_SORT,
				0, ID, 0);		
	}

	const TCHAR* GetSelFontName()
	{
		static TCHAR szFaceName[LF_FACESIZE];		
		GetLBText(GetCurSel(), szFaceName);
		return szFaceName;
	}

	int SetSelFont (CHARFORMAT& cf)
	{
		return SelectString(-1, cf.szFaceName);
	}

	int SetSelFont (CHARFORMAT2& cf)
	{
		return SelectString(-1, cf.szFaceName);
	}

protected:
	CImageList m_img;	
	int m_cyItem;
	
	enum 
	{
		ITEM_DEFAULT_HEIGHT = 15,
			GLYPH_WIDTH = 15
	};

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}

	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		DRAWITEMSTRUCT *lpDIS = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);

		ATLASSERT(lpDIS->CtlType == ODT_COMBOBOX); 
	
		CDCHandle dc = lpDIS->hDC;		
		RECT rc = lpDIS->rcItem;
	
		if (lpDIS->itemState & ODS_FOCUS)
			dc.DrawFocusRect(&rc);
			
		int nIndexDC = dc.SaveDC();

		CBrush br;
		
		if (lpDIS->itemState & ODS_SELECTED)
		{
			br.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
			dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		}
		else
		{
			br.CreateSolidBrush(dc.GetBkColor());
		}

		dc.SetBkMode(TRANSPARENT);
		dc.FillRect(&rc, br);
		
		DWORD dwData = GetItemData(lpDIS->itemID);

		int nLen = GetLBTextLen(lpDIS->itemID);
		TCHAR* psFont = (TCHAR *)_alloca(sizeof TCHAR * (nLen + 1));
		GetLBText(lpDIS->itemID, psFont);
		
#ifdef FTODO
		if (dwData & TRUETYPE_FONTTYPE)
			m_img.Draw(dc, 0, rc.left, rc.top,ILD_TRANSPARENT);
#endif
	
		rc.left += GLYPH_WIDTH + 2;
		dc.TextOut(rc.left, rc.top, psFont);

		dc.RestoreDC(nIndexDC);
		return 0;
	}

	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		MEASUREITEMSTRUCT* pmis = reinterpret_cast<MEASUREITEMSTRUCT*>(lParam);
		ATLASSERT(pmis->CtlType == ODT_COMBOBOX);

		if (m_cyItem == -1)
		{
			// calculate height
			CClientDC dc(m_hWnd);
			HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
			dc.SelectFont( hFont ); //GetFont()
			TEXTMETRIC tm;
			dc.GetTextMetrics(&tm); 

			m_cyItem = tm.tmHeight + tm.tmInternalLeading;
		}

		pmis->itemHeight = m_cyItem;
		return TRUE;			
	}
	
	void Init ()
	{		
		HFONT hFont = ((HFONT)GetStockObject( DEFAULT_GUI_FONT ));
		SetFont(hFont);

		//m_img.CreateFromImage(IDB_TRUETYPE_FONTTYPE, GLYPH_WIDTH, 1, RGB(255,255,255), IMAGE_BITMAP);
		CClientDC dc(m_hWnd);		

		EnumFonts (dc, 0,(FONTENUMPROC) EnumFontProc,(LPARAM)this); //Enumerate font
	}


	static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC /*lptm*/, DWORD dwType, LPARAM lpData)	
	{	
		kFontCombo *pThis = reinterpret_cast<kFontCombo*>(lpData);		
		int index = pThis->AddString(lplf->lfFaceName);
		pThis->SetItemData (index, dwType); 
		return TRUE;
	}

};

#endif //_kFontCombo_0690d7cc_49b3_4d9a_9390_978bc2839650


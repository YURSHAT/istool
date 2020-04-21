#pragma once

/*
Module : ICONCOMBOBOX.H
Purpose: Defines the interface for 2 MFC classes
         to implement an icon selection combo box
Created: PJN / 2-08-1998
History: None

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/

class CSmallIconComboBox : public CWindowImpl<CSmallIconComboBox,CComboBox> {
public:
	int  AddIcon(UINT nIconID);
	int  InsertIcon(int nIndex, UINT nIconID);
	int  DeleteIcon(int nIndex);
	UINT GetCurSelIcon() const;
	int  SetCurSelIcon(UINT nIconID);
	int  AddString(LPCTSTR lpszString);
	int  InsertString(int nIndex, LPCTSTR lpszString);
	int  DeleteString(int nIndex);
	BOOL SubclassWindow(HWND hWnd);

	BEGIN_MSG_MAP(CSmallIconComboBox)
		MESSAGE_HANDLER(WM_MEASUREITEM,OnMeasureItem)
		MESSAGE_HANDLER(WM_DRAWITEM,OnDrawItem)
	    MESSAGE_HANDLER(OCM__BASE + WM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};


class CLargeIconComboBox : public CSmallIconComboBox {
public:
//Methods
	int  AddIcon(UINT nIconID);
	int  AddIcon(HICON hIcon,UINT nIconID/*=-1*/);
	int  InsertIcon(int nIndex, UINT nIconID);
	BOOL SubclassWindow(HWND hWnd);

	BEGIN_MSG_MAP(CLargeIconComboBox)
		MESSAGE_HANDLER(WM_MEASUREITEM,OnMeasureItem)
		MESSAGE_HANDLER(WM_DRAWITEM,OnDrawItem)
	    MESSAGE_HANDLER(OCM__BASE + WM_DRAWITEM, OnDrawItem)
	END_MSG_MAP()

	LRESULT OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

//DDX Support
// void DDX_CBIcon(CDataExchange* pDX, int nIDC, int& nIconID);

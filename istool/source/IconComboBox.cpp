/*
Module : ICONCOMBOBOX.CPP
Purpose: Defines the implementation for 2 MFC classes
         to implement an icon selection combo box
Created: PJN / 23-08-1998
History: PJN / 12-09-1998 1) Removed some files from the workspace which were not being used.
                          2) Updated workspace to use folders.
                          3) Added AddString, InsertString & DeleteString functions. These 
                          functions ASSERT to ensure that the icon combo box operates correctly
                          4) Added DeleteIcon function. This should be called instead of DeleteString
                          5) Modfied internal implementation of InsertIcon & AddIcon for both classes

Copyright (c) 1998 by PJ Naughter.  
All rights reserved.

*/


/////////////////////////////////  Includes  //////////////////////////////////
#include "stdafx.h"
#include "IconComboBox.h"

////////////////////////////// Locals /////////////////////////////////////////

struct _IconComboData
{
  HICON m_hIcon;
  UINT  m_nIconID;
};


////////////////////////////// Implementation /////////////////////////////////

BOOL CSmallIconComboBox::SubclassWindow(HWND hWnd) {
	if(!CWindowImpl<CSmallIconComboBox,CComboBox>::SubclassWindow(hWnd))
		return FALSE;

	//combo box must manage the strings
	ATLASSERT(GetWindowLong(GWL_STYLE) & CBS_HASSTRINGS);

	//combo box must be owner draw variable
	ATLASSERT(GetWindowLong(GWL_STYLE) & CBS_OWNERDRAWVARIABLE);

	//Set the Height of the combo box to just contain one small icon
	::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, 18L);
	return TRUE;
}

int CSmallIconComboBox::AddIcon(UINT nIconID)
{
	ATLASSERT(FALSE);
	//Load up the icon
	HICON hIcon = (HICON) LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 16, 16, 0);
	ATLASSERT(hIcon != NULL);
	if (hIcon == NULL)
		return CB_ERR;

  //Add it to the combo box
  int nOffset = CComboBox::AddString(_T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CSmallIconComboBox::InsertIcon(int nIndex, UINT nIconID)
{
  //Load up the icon
	HICON hIcon = (HICON) LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 16, 16, 0);
	ATLASSERT(hIcon != NULL);
	if (hIcon == NULL)
		return CB_ERR;

  //Add it to the combo box at the specified offset
  int nOffset = CComboBox::InsertString(nIndex, _T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CSmallIconComboBox::DeleteIcon(int nIndex)
{
  //just let the parent do its thing
  return CComboBox::DeleteString(nIndex);
}

UINT CSmallIconComboBox::GetCurSelIcon() const
{
  int nSel = GetCurSel();
	if (nSel == CB_ERR)
	  return 0;

	_IconComboData* pData = (_IconComboData*) GetItemData(nSel);
  return pData->m_nIconID;
}

int CSmallIconComboBox::SetCurSelIcon(UINT nIconID)
{
	//iterate through all the CB items and set the one which 
	//has the corresponding item data
	for (int i=0; i<GetCount(); i++)
	{
		_IconComboData* pData = (_IconComboData*) GetItemData(i);
		if (pData->m_nIconID == nIconID)
		  return SetCurSel(i);
	}
	ATLTRACE(_T("Warning: Could not find matching icon in the icon combo box\n"));
	return CB_ERR;
}

LRESULT CSmallIconComboBox::OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPMEASUREITEMSTRUCT lpMeasureItemStruct = (LPMEASUREITEMSTRUCT)lParam;
	lpMeasureItemStruct->itemHeight = 18;
	return TRUE;
}

LRESULT CSmallIconComboBox::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

  //Create A CDC from the SDK struct passed in
	CDC dc(lpDIS->hDC);

  //If the item is selected, draw the selection background
	if ((lpDIS->itemState & ODS_SELECTED) &&                         
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - draw selection rectangle
		COLORREF crHilite = GetSysColor(COLOR_HIGHLIGHT);
		CBrush br;
		br.CreateSolidBrush(crHilite);
		dc.FillRect(&lpDIS->rcItem, br);
	}

  //If the item is not selected, draw a white background
	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove selection rectangle
		CBrush br;
		br.CreateSolidBrush(RGB(255, 255, 255));
		dc.FillRect(&lpDIS->rcItem, br);
	}

  //Draw the icon
	_IconComboData* pData = (_IconComboData*) lpDIS->itemData;
	if (pData && lpDIS->itemData != 0xFFFFFFFF)
    DrawIconEx(dc, lpDIS->rcItem.left+1, lpDIS->rcItem.top+1, pData->m_hIcon, 16, 16, 0, NULL, DI_NORMAL);
	dc.Detach();
	return TRUE;
}

int CSmallIconComboBox::AddString(LPCTSTR /*lpszString*/)
{
	ATLASSERT(FALSE); //Use AddIcon not AddString
	return CB_ERR;
}

int CSmallIconComboBox::InsertString(int /*nIndex*/, LPCTSTR /*lpszString*/)
{
	ATLASSERT(FALSE); //Use InsertIcon not InsertString
	return CB_ERR;
}

int CSmallIconComboBox::DeleteString(int /*nIndex*/)
{
	ATLASSERT(FALSE); //Use DeleteIcon not DeleteString
	return CB_ERR;
}

BOOL CLargeIconComboBox::SubclassWindow(HWND hWnd) {
	if(!CSmallIconComboBox::SubclassWindow(hWnd))
		return FALSE;

	//Set the Height of the combo box to just contain one large icon
	::SendMessage(m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, 34L);
	return TRUE;
}

int CLargeIconComboBox::AddIcon(UINT nIconID)
{
  //Load up the icon
	HICON hIcon = (HICON) LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 32, 32, 0);
  return AddIcon(hIcon,nIconID);
}

int CLargeIconComboBox::AddIcon(HICON hIcon,UINT nIconID) {
  ATLASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box
  int nOffset = CComboBox::AddString(_T("34"));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}

int CLargeIconComboBox::InsertIcon(int nIndex, UINT nIconID)
{
  //Load up the icon
  HICON hIcon = (HICON) LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(nIconID), IMAGE_ICON, 32, 32, 0);
  ATLASSERT(hIcon != NULL);
  if (hIcon == NULL)
    return CB_ERR;

  //Add it to the combo box at the specified offset
  int nOffset = CComboBox::InsertString(nIndex, _T(""));

	//Hive away the info in the item data field
	_IconComboData* pData = new _IconComboData;
	pData->m_hIcon = hIcon;
	pData->m_nIconID = nIconID;
  SetItemData(nOffset, (DWORD) pData);

  //Return the offset at which it was added
	return nOffset;
}


LRESULT CLargeIconComboBox::OnMeasureItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPMEASUREITEMSTRUCT lpMeasureItemStruct = (LPMEASUREITEMSTRUCT)lParam;
	lpMeasureItemStruct->itemHeight = 34;
	return TRUE;
}

LRESULT CLargeIconComboBox::OnDrawItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;

  //Create A CDC from the SDK struct passed in
	CDC dc(lpDIS->hDC);

  //If the item is selected, draw the selection background
	if ((lpDIS->itemState & ODS_SELECTED) &&                         
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - draw selection rectangle
		COLORREF crHilite = GetSysColor(COLOR_HIGHLIGHT);
		CBrush br;
		br.CreateSolidBrush(crHilite);
		dc.FillRect(&lpDIS->rcItem, br);
	}

  //If the item is not selected, draw a white background
	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove selection rectangle
		CBrush br;
		br.CreateSolidBrush(RGB(255, 255, 255));
		dc.FillRect(&lpDIS->rcItem, br);
	}

  //Draw the icon
	_IconComboData* pData = (_IconComboData*) lpDIS->itemData;
	if (pData && lpDIS->itemData != 0xFFFFFFFF)
    DrawIconEx(dc, lpDIS->rcItem.left+1, lpDIS->rcItem.top+1, pData->m_hIcon, 32, 32, 0, NULL, DI_NORMAL);
	dc.Detach();
	return TRUE;
}

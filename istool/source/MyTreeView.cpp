// MyTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "istool.h"
#include "MyTreeView.h"
#include "StringToken.h"
#include "MyDoc.h"
#include "MainFrm.h"
#include "Sheets.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView

CMyTreeView::CMyTreeView(CInnoScript::SECTION sec) : m_dwFlags(0), CMyView<CMyTreeView>(sec) {}

LRESULT CMyTreeView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	DefWindowProc();
	SetImageList(CMyApp::m_imageList,TVSIL_NORMAL);

	if(m_dwFlags & VFL_DRAGACCEPTFILES)
		DragAcceptFiles(TRUE);

//	bHandled = FALSE;
	return 0;
}

void CMyTreeView::OnFinalMessage(HWND /*hWnd*/) {
	delete this;
}

LRESULT CMyTreeView::OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
	CPoint point(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	if (point.x == -1 && point.y == -1){
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);

		point = rect.TopLeft();
		point.Offset(5, 5);
	}

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_POPUP));

	CMenuHandle pPopup = menu.GetSubMenu(m_nSubMenu);
	ATLASSERT(pPopup != NULL);
	_L(pPopup,"Popup");

	AfxGetMainWnd().OnIdle();
	AfxGetMainWnd().TrackPopupMenu(pPopup, TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y);

	return 0;
}

LRESULT CMyTreeView::OnRButtonDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/) {
	CPoint point(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
	UINT uFlags = wParam;
	HTREEITEM htItem = HitTest(point, &uFlags);
	if(htItem && (uFlags & TVHT_ONITEM)) {
		//GetTreeCtrl().GetSelectedItem();
		Select(htItem, TVGN_CARET);
	}
	return 0;
}

LRESULT CMyTreeView::OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/) {
	CScriptLine* pBase = GetSelectedEntry();
	if(pBase) {
		SendMessage(UWM_PROPERTIES);
		return 1;
	}
	
	return 0;
}

LRESULT CMyTreeView::OnItemExpanded(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/) {
	LPNMTREEVIEW p = (LPNMTREEVIEW)pnmh;

	CScriptLine* pLine = (CScriptLine*)GetItemData(p->itemNew.hItem);
	if(!pLine) return 0;

	if(p->action==TVE_EXPAND) {
		pLine->m_dwUserFlags |= 1;
	} else if(p->action==TVE_COLLAPSE) {
		pLine->m_dwUserFlags &= ~1;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView message handlers



CScriptLine* CMyTreeView::GetSelectedEntry() {
	HTREEITEM hItem = GetSelectedItem();
	if(!hItem) return NULL;
	return reinterpret_cast<CScriptLine*>(GetItemData(hItem));
}

LRESULT CMyTreeView::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
	UINT nChar = wParam;
	if(nChar==VK_RETURN)
		SendMessage(UWM_PROPERTIES);
	else if(nChar==VK_DELETE)
		SendMessage(UWM_DELETE);
	else
		bHandled = FALSE;

	return 0;
}

void CMyTreeView::Populate() {}

LRESULT CMyTreeView::OnShowView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	Populate();
	return 0;
}

void CMyTreeView::UpdateView() {
	Populate();
}

bool CMyTreeView::ApplyView() {
	HTREEITEM hItem = GetSelectedItem();
	long nLine = -1;
	if(hItem) {
		CScriptLine* pLine = (CScriptLine*)GetItemData(hItem);
		nLine = GetDocument()->GetScript().GetLineNumber(pLine);
	}
	if(nLine<0) nLine = GetDocument()->GetScript().GetLineNumber(GetSection());
	GetDocument()->m_nScriptLine = nLine;
	return true;
}

void CMyTreeView::OnUpdate(LONG lHint,void* pParam) {
	CMyView<CMyTreeView>::OnUpdate(lHint,pParam);
	switch(lHint) {
	case CUpdate::HINT_INITVIEW:
		UpdateView();
		break;
	}
}

LRESULT CMyTreeView::OnUpdateUI(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	switch(wParam) {
		case ID_VIEW_PROPERTIES:
		case ID_EDIT_DELETEITEM:
		case ID_EDIT_CUT:
		case ID_EDIT_COPY:
			return GetSelectedEntry()!=NULL;
		case ID_EDIT_PASTE:
			return IsClipboardFormatAvailable(CF_TEXT);
		case ID_EDIT_NEWITEM:
			return 1;
	}
	return 0;
}

LRESULT CMyTreeView::OnCut(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CString strClip;
#if 1
	CScriptLine* pBase = GetSelectedEntry();
	if(!pBase) return 0;
	{
#else
	for(POSITION pos=GetListCtrl().GetFirstSelectedItemPosition();pos;) {
		int nItem = GetListCtrl().GetNextSelectedItem(pos);
		CScriptLine* pBase = reinterpret_cast<CScriptLine*>(GetListCtrl().GetItemData(nItem));
#endif

		CString str;
		pBase->Write(str.GetBuffer(5000),5000);
		str.ReleaseBuffer();
		strClip += str;
		strClip += "\r\n";
	}

	if(OpenClipboard()) {
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,strClip.GetLength()+1);
		LPVOID lp = GlobalLock(hGlobal);
		_tcscpy(reinterpret_cast<LPSTR>(lp),strClip);
		GlobalUnlock(lp);
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hGlobal);
		CloseClipboard();

		SendMessage(UWM_DELETE);
	} else {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

LRESULT CMyTreeView::OnCopy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CString strClip;
#if 1
	CScriptLine* pBase = GetSelectedEntry();
	if(!pBase) return 0;
	{
#else
	for(POSITION pos=GetListCtrl().GetFirstSelectedItemPosition();pos;) {
		int nItem = GetListCtrl().GetNextSelectedItem(pos);
		CScriptLine* pBase = reinterpret_cast<CScriptLine*>(GetListCtrl().GetItemData(nItem));
#endif

		CString str;
		pBase->Write(str.GetBuffer(5000),5000);
		str.ReleaseBuffer();
		strClip += str;
		strClip += "\r\n";
	}

	if(OpenClipboard()) {
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE,strClip.GetLength()+1);
		LPVOID lp = GlobalLock(hGlobal);
		_tcscpy(reinterpret_cast<LPSTR>(lp),strClip);
		GlobalUnlock(lp);
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hGlobal);
		CloseClipboard();
	} else {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

LRESULT CMyTreeView::OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(!OpenClipboard()) {
		AtlMessageBox(m_hWnd,_L("Failed to open clipboard."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
		return 0;
	}

	HGLOBAL hGlobal = GetClipboardData(CF_TEXT);
	LPVOID lp = GlobalLock(hGlobal);
	CString str(reinterpret_cast<LPCTSTR>(lp));
	GlobalUnlock(lp);
	CloseClipboard();

	CStringToken token(str,"\n");
	bool bModified = false;
	do {
		LPCTSTR lpsz = token.GetNext();
		if(!lpsz) break;
		CString str(lpsz);
		if(str.IsEmpty()) continue;

		CScriptLine* pLine = NULL;
		try {
			if(m_sec==CInnoScript::SEC_FILES) {
				// TODO: differ between dirs and files
				pLine = new CInnoScript::CLineParam(m_sec,str);
			} else if(m_sec==CInnoScript::SEC_MESSAGES) {
				pLine = new CInnoScript::CLineSetup(m_sec,str);
			} else {
				pLine = new CInnoScript::CLineParam(m_sec,str);
			}
			GetDocument()->GetScript().AddLine(pLine);
			bModified = true;
			continue;
		} catch(...) {
			if(pLine) delete pLine;
		}
	} while(true);

	if(bModified) {
		Populate();
		GetDocument()->SetModifiedFlag();
	} else {
		AtlMessageBox(m_hWnd,_L("Incorrect clipboard format."),IDR_MAINFRAME,MB_OK|MB_ICONERROR);
	}

	return 0;
}

LRESULT CMyTreeView::OnNewItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CScriptLine* pLine = new CScriptLine(m_sec);
	CScriptList	list;

	list.Add(pLine);

	OnPreNewItem(pLine);
	if(!CSheets::ShowSheet(m_hWnd,list,true)) {
		delete pLine;
		return 0;
	}

	CWaitCursor wait;
	GetDocument()->GetScript().AddLine(pLine);
	InsertItem(pLine);
	GetDocument()->SetModifiedFlag();
	return 0;
}

LRESULT CMyTreeView::OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	HTREEITEM hItem = GetSelectedItem();
	if(!hItem) return 0;
	CScriptLine* pRegistry = (CScriptLine*)GetItemData(hItem);
	if(!pRegistry) return 0;

	GetDocument()->GetScript().DeleteLine(pRegistry);
	DeleteItem(hItem);
	GetDocument()->SetModifiedFlag();
	Populate();
	return 0;
}

LRESULT CMyTreeView::OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	CScriptList	list;

	HTREEITEM hItem = GetSelectedItem();
	if(!hItem) return 0;
	CScriptLine* pLine = (CScriptLine*)GetItemData(hItem);
	if(!pLine) return 0;
	list.Add(pLine);

	if(!CSheets::ShowSheet(m_hWnd,list,false))
		return 0;

//	if(strOldPath==strNewPath)
//		SetItemData(hItem,pLine);
//	else
		Populate();

	GetDocument()->SetModifiedFlag();

	return 0;
}

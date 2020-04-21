#pragma once

#include "MyView.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTreeView view

class CMyTreeView : public CWindowImpl<CMyTreeView, CTreeViewCtrl>, public CMyView<CMyTreeView> {
public:
	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())

	CMyTreeView(CInnoScript::SECTION sec);
	virtual void UpdateView();
	virtual bool ApplyView();

	BEGIN_MSG_MAP(CMyTreeView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CONTEXTMENU,OnContextMenu)
		MESSAGE_HANDLER(WM_RBUTTONDOWN,OnRButtonDown)
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		MESSAGE_HANDLER(WM_SHOWVIEW,OnShowView)
		MESSAGE_HANDLER(WM_CUT,OnCut)
		MESSAGE_HANDLER(WM_COPY,OnCopy)
		MESSAGE_HANDLER(WM_PASTE,OnPaste)
		MESSAGE_HANDLER(UWM_NEWITEM,OnNewItem)
		MESSAGE_HANDLER(UWM_DELETE,OnDelete)
		MESSAGE_HANDLER(UWM_PROPERTIES,OnProperties)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_DBLCLK,OnDoubleClick)
		REFLECTED_NOTIFY_CODE_HANDLER(TVN_ITEMEXPANDED, OnItemExpanded)
		CHAIN_MSG_MAP(CMyView<CMyTreeView>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnShowView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCut(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaste(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnNewItem(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnItemExpanded(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateUI(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	virtual void OnFinalMessage(HWND /*hWnd*/);

	CTreeViewCtrl& GetTreeCtrl() { return *this; }
	CScriptLine* GetSelectedEntry();
	virtual void Populate();

protected:
	DWORD	m_dwFlags;	
	enum {
		VFL_DRAGACCEPTFILES	= 0x01,	// The section supports drag and drop
		VFL_SETUP			= 0x02	// For setup and messages section
	};
	
protected:
	virtual void OnUpdate(LONG lHint,void* pParam);
	virtual void OnPreNewItem(CScriptLine* pLine) {}
	virtual HTREEITEM InsertItem(CScriptLine* pLine) { return NULL; }
};

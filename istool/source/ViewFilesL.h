// WTL complete

#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewFilesL view

class CViewFilesL : public CMyListView {
public:
	CViewFilesL();

protected:
	virtual void Populate();
	virtual UINT InsertItem(CInnoScript::CLine* pLine);
	virtual void SetItemTexts(UINT nItem);
	void RefreshText();

	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }

	LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCreateIcon(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateUI(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CViewFilesL)
		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
		MESSAGE_HANDLER(UWM_NEWITEM,OnNewItem)
		MESSAGE_HANDLER(UWM_CREATEICON,OnCreateIcon)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		CHAIN_MSG_MAP(CMyListView)
	END_MSG_MAP()
};

#pragma once

#include "MyTreeView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryT view

class CViewRegistryT : public CMyTreeView {
public:
	CViewRegistryT();           // protected constructor used by dynamic creation

	LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CViewRegistryT)
		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
		CHAIN_MSG_MAP(CMyTreeView)
	END_MSG_MAP()

protected:
	void GetItemDirectory(HTREEITEM,CString&);
	virtual void Populate();
	void SetItemData(HTREEITEM hItem,CScriptLine* pLine);

	virtual HTREEITEM InsertItem(CScriptLine* pLine);
};

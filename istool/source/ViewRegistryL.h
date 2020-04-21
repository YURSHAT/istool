#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewRegistryL view

class CViewRegistryL : public CMyListView {
public:
	CViewRegistryL();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo() { return m_colInfo; }

	LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CViewRegistryL)
		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
		CHAIN_MSG_MAP(CMyListView)
	END_MSG_MAP()
};

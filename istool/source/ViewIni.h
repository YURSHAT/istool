#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewIni view

class CViewIni : public CMyListView {
public:
	CViewIni();           // protected constructor used by dynamic creation

protected:
	static const CListInfo m_colInfo[];
	virtual const CListInfo* GetListInfo();

	LRESULT OnDropFiles(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CViewIni)
		MESSAGE_HANDLER(WM_DROPFILES,OnDropFiles)
		CHAIN_MSG_MAP(CMyListView)
	END_MSG_MAP()
};

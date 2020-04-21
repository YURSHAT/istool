#pragma once

#include "MyListView.h"

/////////////////////////////////////////////////////////////////////////////
// CViewMessages view

class CViewMessages : public CMyListView {
public:
	CViewMessages();
	CViewMessages(CInnoScript::SECTION sec);

protected:
	enum {
		DF_NAME = 0,
		DF_MESSAGE,
	};

	virtual void SetItemTexts(UINT nItem);

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnUpdateUI(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	BEGIN_MSG_MAP(CViewMessages)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		CHAIN_MSG_MAP(CMyListView)
	END_MSG_MAP()
};

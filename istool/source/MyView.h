// MyView.h: interface for the CMyView class.

#pragma once

class CMyDoc;

#include "Update.h"

template <class T>
class CMyView {
public:
	CMyView(CInnoScript::SECTION sec) : m_sec(sec), m_nSubMenu(0) {}

	CMyDoc* GetDocument() {
		return AfxGetDocument();
	}

	BEGIN_MSG_MAP(CMyView)
		MESSAGE_HANDLER(WM_SHOWVIEW,OnShowView)
		MESSAGE_HANDLER(WM_HIDEVIEW,OnHideView)
	END_MSG_MAP()

	CInnoScript::SECTION GetSection() {
		return m_sec;
	}

	virtual void UpdateView() = 0;
	virtual bool ApplyView() = 0;

protected:
	CInnoScript::SECTION	m_sec;
	UINT					m_nSubMenu;	// Popup Submenu to display 

	LRESULT OnShowView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		UpdateView();
		return 0;
	}

	LRESULT OnHideView(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		if(!ApplyView())
			return 1;
		return 0;
	}

	virtual void OnUpdate(LONG lHint,void* pParam) {
		if(!lHint)
			UpdateView();
		else if(lHint==CUpdate::HINT_APPLYCHANGES)
			ApplyView();
	}

	void SetSubMenu(UINT nSubMenu) { m_nSubMenu = nSubMenu; }
};

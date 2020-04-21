#pragma once

#include "Edit2.h"

/////////////////////////////////////////////////////////////////////////////
// CComboBox2 window

class CComboBox2 : public CWindowImpl<CComboBox2,CComboBox> {
public:
	CComboBox2(CMyDoc* pDoc) : m_edit(pDoc) {}

	BEGIN_MSG_MAP(CComboBox2)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
		m_edit.SubclassWindow(GetDlgItem(1001));
		bHandled = FALSE;
		return 0;
	}

	BOOL SubclassWindow(HWND hWnd) {
		if(!CWindowImpl<CComboBox2,CComboBox>::SubclassWindow(hWnd))
			return FALSE;

		m_edit.SubclassWindow(GetDlgItem(1001));
		return TRUE;
	}

protected:
	CEdit2	m_edit;
};

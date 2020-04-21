// ConstantListBox.h

#pragma once

class CConstantListBox : public CWindowImpl<CConstantListBox,CListBox> {
public:
	DECLARE_WND_SUPERCLASS(NULL, CListBox::GetWndClassName())

	BEGIN_MSG_MAP(CConstantListBox)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_KILLFOCUS,OnKillFocus)
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
	END_MSG_MAP()

	void HideMe() {
		ShowWindow(SW_HIDE);
		GetParent().SetFocus();
	}

	LRESULT OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
		HideMe();
		return 0;
	}

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled) {
		if(wParam==VK_ESCAPE) {
			HideMe();
		} else if(wParam==VK_RETURN) {
			int sel = GetCurSel();
			if(sel!=LB_ERR) {
				static CString str;
				GetText(sel,str);
				//AtlMessageBox(m_hWnd,(LPCTSTR)str,"DEBUG");
				GetParent().SendMessage(WM_USER+99,(WPARAM)(LPCTSTR)str);
			}
			HideMe();
		} else
			bHandled = FALSE;
		return 0;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		LRESULT res = DefWindowProc();

		SetFont(AtlGetDefaultGuiFont());
		UINT nPos = 0;
		while(CInnoScriptEx::m_constants[nPos].m_pszConstant) {
			if(CInnoScriptEx::m_constants[nPos].m_bFolder)
				AddString(CInnoScriptEx::m_constants[nPos].m_pszConstant);
			nPos++;
		}
		return res;
	}

	void Show(int x,int y) {
		SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
		SetFocus();
	}
};

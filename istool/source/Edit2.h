#pragma once

#include "FilesHelper.h"
#include "MyDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CEdit2 window

class CEdit2 : public CWindowImpl<CEdit2,CEdit> {
public:
	CEdit2(CMyDoc* pDoc) : m_pDoc(pDoc) {}

	BEGIN_MSG_MAP(CEdit2)
		MESSAGE_HANDLER(WM_CONTEXTMENU,OnContextMenu)
		COMMAND_ID_HANDLER(ID_EDIT_CUT, OnEditCut)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
		COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
		COMMAND_ID_HANDLER(ID_EDIT_PASTE, OnEditPaste)
		COMMAND_ID_HANDLER(ID_EDIT_DELETEITEM, OnEditDelete)
		COMMAND_ID_HANDLER(ID_EDIT_SELECT_ALL, OnEditSelectAll)
		COMMAND_RANGE_HANDLER(0xE00, 0xE7F, OnDirectoryConstant)
		COMMAND_RANGE_HANDLER(0xE80, 0xEFF, OnOtherConstant)
		COMMAND_RANGE_HANDLER(0xF00, 0xFFF, OnSetupDirectory)
	END_MSG_MAP()

	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/) {
		CPoint point(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		if (point.x == -1 && point.y == -1) {
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		menu.LoadMenu(IDR_COMBOPOPUP);

		CMenuHandle pPopup = menu.GetSubMenu(0);
		CMenuHandle pSubMenu = pPopup.GetSubMenu(9);

		// Undo
		pPopup.EnableMenuItem(ID_EDIT_UNDO,SendMessage(EM_CANUNDO,0,0)?MF_ENABLED:MF_GRAYED);
		// Cut/copy/clear
		int nStart, nEnd;
		GetSel(nStart,nEnd);
		pPopup.EnableMenuItem(ID_EDIT_CUT,nEnd>nStart?MF_ENABLED:MF_GRAYED);
		pPopup.EnableMenuItem(ID_EDIT_COPY,nEnd>nStart?MF_ENABLED:MF_GRAYED);
		pPopup.EnableMenuItem(ID_EDIT_DELETEITEM,nEnd>nStart?MF_ENABLED:MF_GRAYED);
		// Paste
		pPopup.EnableMenuItem(ID_EDIT_PASTE,IsClipboardFormatAvailable(CF_TEXT)?MF_ENABLED:MF_GRAYED);

		while(pSubMenu.DeleteMenu(0, MF_BYPOSITION));
		int nPos = 0;
		while(CInnoScriptEx::m_constants[nPos].m_pszConstant) {
			if(CInnoScriptEx::m_constants[nPos].m_bFolder) {
				if(CInnoScriptEx::m_constants[nPos].m_pszConstant[0]=='-') {
					pSubMenu.AppendMenu(MF_STRING | MF_SEPARATOR, 0);
				} else {
					CString str;
					str.Format("%s\t%s",
						CInnoScriptEx::m_constants[nPos].m_pszConstant,
						CInnoScriptEx::m_constants[nPos].m_pszName
					);
					pSubMenu.AppendMenu(MF_STRING | MF_ENABLED, 0xE00 + nPos, str);
				}
			}
			nPos++;
		}

		pSubMenu = pPopup.GetSubMenu(10);
		ATLASSERT(pSubMenu != NULL);
		while(pSubMenu.DeleteMenu(0, MF_BYPOSITION));

		nPos = 0;
		while(CInnoScriptEx::m_constants[nPos].m_pszConstant) {
			if(!CInnoScriptEx::m_constants[nPos].m_bFolder) {
				if(CInnoScriptEx::m_constants[nPos].m_pszConstant[0]=='-') {
					pSubMenu.AppendMenu(MF_STRING | MF_SEPARATOR, 0);
				} else {
					CString str;
					str.Format("%s\t%s",
						CInnoScriptEx::m_constants[nPos].m_pszConstant,
						CInnoScriptEx::m_constants[nPos].m_pszName
					);
					pSubMenu.AppendMenu(MF_STRING | MF_ENABLED, 0xE80 + nPos, str);
				}
			}
			nPos++;
		}


		pSubMenu = pPopup.GetSubMenu(11);
		ATLASSERT(pSubMenu != NULL);
		while(pSubMenu.DeleteMenu(0, MF_BYPOSITION));
		nPos = 0;

		CScriptList dirs;
		m_pDoc->GetScript().GetList(CInnoScript::SEC_DIRS,dirs);

		for(int nPos2=0;nPos2<dirs.GetSize();nPos2++) {
			CScriptLine* pLine = dirs[nPos2];
			pSubMenu.AppendMenu(MF_STRING|MF_ENABLED, 0xF00 + nPos++, SAFESTR(pLine->GetParameter("Name")));
		}
		if(!nPos)
			pSubMenu.AppendMenu(MF_STRING|MF_GRAYED, (UINT_PTR)0, "Empty");

		pPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, *this);

		return 0;
	}

	LRESULT OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		Cut();
		return 0;
	}
	LRESULT OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		Copy();
		return 0;
	}
	LRESULT OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		Undo();
		return 0;
	}
	LRESULT OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		Paste();
		return 0;
	}
	LRESULT OnEditDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		Clear();
		return 0;
	}
	LRESULT OnEditSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		SetSel(0,-1);
		return 0;
	}

	LRESULT OnDirectoryConstant(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		LPCTSTR lpszConstant = CInnoScriptEx::m_constants[wID-0xE00].m_pszConstant;

		CString str;
		GetWindowText(str.GetBuffer(256),256);
		int nPos = str.Find('}');
		if(nPos>0 && str[0]=='{') {
			str = str.Mid(nPos+1);
			if(str.GetLength()>0 && str[0]=='\\') str = str.Mid(1);
		}
		str = CString(lpszConstant) + "\\" + str;
		SetWindowText(str);
		return 0;
	}

	LRESULT OnOtherConstant(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		LPCTSTR lpszConstant = CInnoScriptEx::m_constants[wID-0xE80].m_pszConstant;
		ReplaceSel(lpszConstant,TRUE);
		return 0;
	}

	LRESULT OnSetupDirectory(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
		UINT nDir = wID - 0xF00;
		CScriptLine* pLine = NULL;

		CScriptList dirs;
		m_pDoc->GetScript().GetList(CInnoScript::SEC_DIRS,dirs);

		pLine = dirs[nDir];

		CString str, str2(pLine->GetParameter("Name"));
		GetWindowText(str.GetBuffer(256),256);
		int nPos = str.ReverseFind('\\');
		if(nPos>=0) {
			str = str.Mid(nPos+1);
			if(str.GetLength()>0 && str[0]=='\\') str = str.Mid(1);
		}
		if(str2[str2.GetLength()-1]!='\\') str2 += "\\";
		str = str2 + str;
		SetWindowText(str);
		return 0;
	}

protected:
	CMyDoc*	m_pDoc;
};

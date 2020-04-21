#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"

LRESULT CMainFrame::OnEditCut(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,WM_CUT,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,WM_COPY,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditPaste(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,WM_PASTE,0,0);
	return 0;
}
LRESULT CMainFrame::OnEditNewItem(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_NEWITEM,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditSelectAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_SELECTALL,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_DELETE,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditMoveUp(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_MOVEUP,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditMoveDown(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_MOVEDOWN,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditFind(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_FIND,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditReplace(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_REPLACE,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditRepeat(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_REPEAT,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditGoto(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_GOTO,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditUndo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_UNDO,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditRedo(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_REDO,0,0);
	return 0;
}

LRESULT CMainFrame::OnEditCommentSelection(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	HWND hWndFocus = GetFocusView();
	if(hWndFocus)
		::SendMessage(hWndFocus,UWM_COMMENTSELECTION,wID==ID_EDIT_COMMENTSELECTION ? 0 : 1,0);
	return 0;
}

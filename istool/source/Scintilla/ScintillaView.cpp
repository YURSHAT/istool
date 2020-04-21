#include "stdafx.h"
#include "ISTool.h"
#include "MainFrm.h"
#include "DlgGoto.h"

LRESULT CScintillaView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	LRESULT res = DefWindowProc();
	static bool bLoaded = false;
	if(!bLoaded) {
		LoadLexerLibrary("isslexer.dll");
		bLoaded = true;
	}
	return res;
}

LRESULT CScintillaView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if(AfxGetMainWnd().m_pFindReplaceDlg)
		AfxGetMainWnd().m_pFindReplaceDlg->EndDialog(IDCANCEL);
	bHandled = FALSE;
	return 0;
}

LRESULT CScintillaView::OnFindReplaceMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/) {
	if(AfxGetMainWnd().m_pFindReplaceDlg->IsTerminating()) {
		AfxGetMainWnd().m_pFindReplaceDlg = NULL;
	} else if(AfxGetMainWnd().m_pFindReplaceDlg->FindNext()) {
		m_strSearch = AfxGetMainWnd().m_pFindReplaceDlg->GetFindString();

		m_iSearchFlags = 0;
		if(AfxGetMainWnd().m_pFindReplaceDlg->MatchCase())
			m_iSearchFlags |= SCFIND_MATCHCASE;
		if(AfxGetMainWnd().m_pFindReplaceDlg->MatchWholeWord())
			m_iSearchFlags |= SCFIND_WHOLEWORD;

		TextToFind ft = {0};
		ft.chrg.cpMin = GetCurrentPos() + 1;
		ft.chrg.cpMax = GetTextLength();
		ft.lpstrText = (LPSTR)(LPCTSTR)m_strSearch;
		long pos = FindText(m_iSearchFlags,&ft);
		if(pos==-1) {
			MessageBeep(MB_ICONERROR);
		} else {
			SetCurrentPos(pos);
			SetSel(ft.chrgText.cpMin,ft.chrgText.cpMax);
		}
	} else if(AfxGetMainWnd().m_pFindReplaceDlg->ReplaceCurrent()) {
		if(GetSelectionStart()<GetSelectionEnd()) {
			m_strReplace = AfxGetMainWnd().m_pFindReplaceDlg->GetReplaceString();
			ReplaceSel(CT2A(m_strReplace));
		}
	} else if(AfxGetMainWnd().m_pFindReplaceDlg->ReplaceAll()) {
		m_strSearch = AfxGetMainWnd().m_pFindReplaceDlg->GetFindString();
		m_strReplace = AfxGetMainWnd().m_pFindReplaceDlg->GetReplaceString();

		m_iSearchFlags = 0;
		if(AfxGetMainWnd().m_pFindReplaceDlg->MatchCase())
			m_iSearchFlags |= SCFIND_MATCHCASE;
		if(AfxGetMainWnd().m_pFindReplaceDlg->MatchWholeWord())
			m_iSearchFlags |= SCFIND_WHOLEWORD;

		long nStart = 0;
		do {
			TextToFind ft = {0};
			ft.chrg.cpMin = nStart;
			ft.chrg.cpMax = GetTextLength();
			ft.lpstrText = (LPSTR)(LPCTSTR)m_strSearch;
			long pos = FindText(m_iSearchFlags,&ft);
			if(pos==-1) {
				//MessageBeep(MB_ICONERROR);
				break;
			} else {
				//SetCurrentPos(pos);
				SetSel(ft.chrgText.cpMin,ft.chrgText.cpMax);
				ReplaceSel(CT2A(m_strReplace));
				nStart = GetCurrentPos() + m_strReplace.GetLength();
			}
		} while(true);
#ifdef _DEBUG
	} else {
		AtlMessageBox(m_hWnd,_T("findreplace unhandled"));
#endif
	}
	return 0;
}

LRESULT CScintillaView::OnFind(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if(!AfxGetMainWnd().m_pFindReplaceDlg) {
		long nStart = GetSelectionStart();
		long nEnd = GetSelectionEnd();
		if(nStart<nEnd)
			GetSelText(CT2A(m_strSearch.GetBuffer(nEnd-nStart)));
		AfxGetMainWnd().m_pFindReplaceDlg = new CFindReplaceDialog();
		AfxGetMainWnd().m_pFindReplaceDlg->Create(TRUE,m_strSearch,NULL,FR_HIDEUPDOWN|FR_DOWN,m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->CenterWindow(m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}

LRESULT CScintillaView::OnRepeat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if(m_strSearch.IsEmpty()) {
		SendMessage(UWM_FIND);
		return 0;
	}

#if 1
	TextToFind ft = {0};
	ft.chrg.cpMin = GetCurrentPos() + 1;
	ft.chrg.cpMax = GetTextLength();
	ft.lpstrText = (LPSTR)(LPCTSTR)m_strSearch;
	long pos = FindText(m_iSearchFlags,&ft);
	if(pos==-1) {
		MessageBeep(MB_ICONERROR);
	} else {
		SetCurrentPos(pos);
		SetSel(ft.chrgText.cpMin,ft.chrgText.cpMax);
	}
#else
	int res;
	if(m_bSearchDown) {
		SetAnchor(GetCurrentPos()+1);
		SearchAnchor();
		res = SearchNext(m_iSearchFlags,m_strSearch);
	} else {
		SetAnchor(GetCurrentPos()-1);
		SearchAnchor();
		res = SearchPrev(m_iSearchFlags,m_strSearch);
	}

	if(res==-1) MessageBeep(MB_ICONERROR);
#endif

	return 0;
}

LRESULT CScintillaView::OnReplace(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	if(!AfxGetMainWnd().m_pFindReplaceDlg) {
		long nStart = GetSelectionStart();
		long nEnd = GetSelectionEnd();
		if(nStart<nEnd) {
			GetSelText(m_strSearch.GetBuffer(nEnd-nStart));
		}

		AfxGetMainWnd().m_pFindReplaceDlg = new CFindReplaceDialog();
		AfxGetMainWnd().m_pFindReplaceDlg->Create(FALSE,m_strSearch,m_strReplace,FR_HIDEUPDOWN|FR_DOWN,m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->CenterWindow(m_hWnd);
		AfxGetMainWnd().m_pFindReplaceDlg->ShowWindow(SW_SHOW);
	}
	return 0;
}

LRESULT CScintillaView::OnGoto(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	CDlgGoto dlg;
	dlg.m_strLineNumber.Format(_T("%d"),LineFromPosition(GetCurrentPos()) + 1);
	if(dlg.DoModal(m_hWnd)!=IDOK)
		return 0;

	long nLine = _ttol(dlg.m_strLineNumber) - 1;
	if(nLine<0)
		return 0;

	long nPos = PositionFromLine(nLine);
	SetCurrentPos(nPos);
	SetSel(nPos,nPos);
	CWindow::SetFocus();

	return 0;
}

LRESULT CScintillaView::OnSelectAll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	SetSel(0,-1);
	return 0;
}

LRESULT CScintillaView::OnDelete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Clear();
	return 0;
}

LRESULT CScintillaView::OnUndo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Undo();
	return 0;
}

LRESULT CScintillaView::OnRedo(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled) {
	Redo();
	return 0;
}

LRESULT CScintillaView::OnEditCommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	return 0;
}

LRESULT CScintillaView::OnEditUncommentSelection(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/) {
	return 0;
}

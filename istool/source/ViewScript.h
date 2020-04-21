#pragma once

#include "MyView.h"

#include "scintilla/ScintillaView.h"

class CViewScript : public CScintillaView, public CMyView<CViewScript>,
	public CPrintJobInfo
{
public:
	CViewScript();

	virtual void UpdateView();
	virtual bool ApplyView();

	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0) 
	{
		if(dwStyle & LVS_REPORT) {
			m_bCode = true;
			dwStyle &= ~LVS_REPORT;
		} else
			m_bCode = false;

		return CScintillaView::Create(hWndParent,rcDefault,szWindowName,dwStyle,dwExStyle);
	}

	bool m_bCode, m_bUpdateModified, m_bModified;

	BEGIN_MSG_MAP(CViewScript)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_CONTEXTMENU,OnContextMenu)
		MESSAGE_HANDLER(WM_TIMER,OnMoveToLine)
		MESSAGE_HANDLER(UWM_PRINT,OnPrint)
		MESSAGE_HANDLER(UWM_CALLTIP,OnCallTip)
		MESSAGE_HANDLER(UWM_STATUSBAR,OnStatusBar)
		MESSAGE_HANDLER(UWM_COMMENTSELECTION,OnCommentSelection)
		MESSAGE_HANDLER(UWM_COMMENTSELECTION,OnCommentSelection)
		MESSAGE_HANDLER(UWM_UPDATEUI,OnUpdateUI)
		MESSAGE_HANDLER(UWM_PROPERTIES,OnProperties)
		REFLECTED_NOTIFY_CODE_HANDLER(SCN_SAVEPOINTREACHED,OnSavePointReached)
		REFLECTED_NOTIFY_CODE_HANDLER(SCN_SAVEPOINTLEFT,OnSavePointLeft)
		REFLECTED_NOTIFY_CODE_HANDLER(SCN_CHARADDED,OnCharAdded)
		REFLECTED_NOTIFY_CODE_HANDLER(SCN_DWELLSTART,OnDwellStart)
		REFLECTED_NOTIFY_CODE_HANDLER(SCN_DWELLEND,OnDwellEnd)
		CHAIN_MSG_MAP(CScintillaView)
		CHAIN_MSG_MAP(CMyView<CViewScript>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
		LRESULT res = CScintillaView::OnCreate(uMsg,wParam,lParam,bHandled);
		if(m_bCode)
			SetupCode();
		else
			SetupISS();

		CallTipSetBack(GetSysColor(COLOR_INFOBK));
		CallTipSetFore(GetSysColor(COLOR_INFOTEXT));
		CallTipSetForeHlt(RGB(255,0,0));
		SetMouseDwellTime(500);
		AutoCSetIgnoreCase(true);

		return res;
	}
	LRESULT OnContextMenu(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMoveToLine(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnPrint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCallTip(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnStatusBar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommentSelection(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnProperties(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnSavePointReached(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnSavePointLeft(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnCharAdded(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnDwellStart(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnDwellEnd(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

	// WTL
	virtual bool IsValidPage(UINT nPage);
	virtual bool PrintPage(UINT nPage, HDC hDC);
//	virtual void BeginPrintJob(HDC hDC);
//	virtual void EndPrintJob(HDC hDC, bool bAborted);
//	virtual void PrePrintPage(UINT nPage, HDC hDC);
//	virtual void PostPrintPage(UINT nPage, HDC hDC);
	long	m_nNextPrintPos;

protected:
	//virtual void OnInitNew();
	virtual void OnUpdate(LONG lHint,void* pParam);

	CInnoScript::SECTION GetSectionFromLine(long nLine);
};
